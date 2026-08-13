//! The verifier — GHC's Core Lint, without GHC's tree (panel 019 point 1).
//!
//! The historian's finding is the reason this file exists. GHC is the precedent
//! usually cited for building a separate desugared tree, and what Core actually
//! bought GHC was not the tree: it was **Lint**, "a very powerful consistency
//! check on the compiler itself … an 100% independent check on the type inference
//! engine", of which GHC's own authors write that "in practice we have found that
//! it is surprisingly hard to accidentally write optimisations that are
//! type-correct but not semantically correct". So panel 019 kept the single pass
//! and bought the check separately.
//!
//! Every invariant here is one the lowering could plausibly break, and two of them
//! are load-bearing for milestones that have not happened yet:
//!
//! - **one terminator per block, and never `Open`** — CLAUDE.md §7 emits one
//!   `goto`+label per basic block, so a block with no exit is C that falls through
//!   a label;
//! - **`preds` agrees with the terminators**, recomputed from the same
//!   `successors` the builder used, because M-strings-ownership's cleanup chains walk edges;
//! - **every exit edge copies out every `@` parameter** — §4.8's "copy-out happens
//!   always", which panel 000 named as the place this project would actually stall.
//!
//! A violation is an **internal** error, not a diagnostic: it says the compiler is
//! wrong, so it exits 2 and never speaks in the language's own vocabulary (panel
//! 019's R1 rule, which reserves IR words for the dump and for exit-2 messages).
//!
//! One check deliberately absent: no `Ty::Generic` in a lowered body. A generic
//! body lowers polymorphically at M-ir-lowering by design (§4.12, amended by panel 019), and
//! it is *monomorphisation* — the IR→IR pass at M-generics-library — plus the ownership pass and
//! the emitter that must assert its absence.
//!
//! What an instruction *reads* lives in `uses.rs`, because M-strings-ownership needs the same
//! answer: an ownership pass is a walk over definitions and uses, and there must be
//! exactly one table saying which is which. What a *value* must satisfy — assigned
//! once, and its definition dominating every use — lives in `values.rs`, which is
//! where the dominator computation the second half needs also lives.

use crate::types::Checked;

use super::build::successors;
use super::inst::{Callee, Op, Term};
use super::uses::{operands, slots_of};
use super::values;
use super::phases;
use super::{Block, Function, Program, SlotKind};

/// Whether this function's implementation is C's: an `extern function`'s code, or
/// an `extern constant`'s value (§4.19, panel 038). Both have no blocks, and
/// nothing else in the IR may have none.
fn from_c(ast: &crate::syntax::Ast, function: &Function) -> bool {
    match &ast.decls[function.decl as usize].kind {
        crate::syntax::DeclKind::Function(declared) => declared.is_extern,
        crate::syntax::DeclKind::Constant { header, .. } => header.is_some(),
        _ => false,
    }
}

/// Every invariant violation, or an empty list. The message names the function and
/// the block, because a dump of a large program is not where anyone wants to start
/// searching.
pub fn verify(program: &Program, checked: &Checked, ast: &crate::syntax::Ast) -> Vec<String> {
    let mut problems = Vec::new();
    for function in &program.functions {
        // The phase is in every message, because "which pass produced this" is the
        // first question a violation raises and the compiler already knows the answer
        // (panel 021 R8). rustc bakes the same attribution in:
        // `validate_body(tcx, body, format!("after pass {pass_name}"))`.
        let phase = program.phase;
        let where_ = |what: String| format!("{} ({}): {what}", function.name, phase.name());
        // **What C provides has no blocks, and the question is asked of the
        // declaration rather than of `FnKind`.** An `extern function` lowers to
        // `FnKind::Extern`; an `extern constant` lowers to `FnKind::Constant`,
        // because it is read like any other constant — so a rule written over the
        // kind would have to encode *which kinds can come from a header*, a
        // premise this milestone already falsified once (CLAUDE.md §11).
        if from_c(ast, function) {
            if !function.blocks.is_empty() {
                problems.push(where_("what C provides has a body".to_string()));
            }
            continue;
        }
        if function.blocks.is_empty() {
            problems.push(where_("no entry block".to_string()));
            continue;
        }
        let mutable = mutable_params(function);
        for (index, block) in function.blocks.iter().enumerate() {
            let at = |what: String| where_(format!("bb{index}: {what}"));
            check_terminator(function, block, &at, &mut problems);
            check_preds(function, index, block, &at, &mut problems);
            check_instructions(function, block, &at, &mut problems);
            check_copy_out(function, block, mutable, &at, &mut problems);
            check_return_type(function, block, checked, &at, &mut problems);
            phases::check(program, function, block, checked, &at, &mut problems);
        }
        values::check(function, &where_, &mut problems);
    }
    problems
}

fn mutable_params(function: &Function) -> usize {
    function
        .params
        .iter()
        .filter(|slot| {
            matches!(function.slots[slot.0 as usize].kind, SlotKind::Param { mutable: true })
        })
        .count()
}

fn check_terminator(
    function: &Function,
    block: &Block,
    at: &dyn Fn(String) -> String,
    problems: &mut Vec<String>,
) {
    if block.term == Term::Open {
        problems.push(at("no terminator".to_string()));
        return;
    }
    for target in successors(&block.term) {
        if target.0 as usize >= function.blocks.len() {
            problems.push(at(format!("jumps to bb{}, which does not exist", target.0)));
        }
    }
    if let Term::Switch { cases, .. } = &block.term {
        if cases.is_empty() {
            problems.push(at("a switch with no cases".to_string()));
        }
    }
}

/// `preds` is recomputed from the terminators with the builder's own `successors`,
/// so the two cannot drift: if this fires, one of them was edited by hand.
fn check_preds(
    function: &Function,
    index: usize,
    block: &Block,
    at: &dyn Fn(String) -> String,
    problems: &mut Vec<String>,
) {
    let me = super::ids::BlockId(index as u32);
    let mut expected: Vec<u32> = Vec::new();
    for (other, candidate) in function.blocks.iter().enumerate() {
        if successors(&candidate.term).contains(&me) && !expected.contains(&(other as u32)) {
            expected.push(other as u32);
        }
    }
    let recorded: Vec<u32> = block.preds.iter().map(|p| p.0).collect();
    let mut sorted = recorded.clone();
    sorted.sort_unstable();
    expected.sort_unstable();
    if sorted != expected {
        problems.push(at(format!("preds {recorded:?} but {expected:?} jump here")));
    }
}

fn check_instructions(
    function: &Function,
    block: &Block,
    at: &dyn Fn(String) -> String,
    problems: &mut Vec<String>,
) {
    for (index, inst) in block.insts.iter().enumerate() {
        if inst.op == Op::Missing {
            problems.push(at("a form the lowering does not handle".to_string()));
        }
        if let Op::FuncRef(Callee::Indirect(_)) = inst.op {
            problems.push(at("the address of an address".to_string()));
        }
        // An abort ends the program, so nothing may follow it and its block's
        // terminator must say so.
        if matches!(inst.op, Op::Abort { .. }) {
            if index + 1 != block.insts.len() {
                problems.push(at("an abort with instructions after it".to_string()));
            }
            if block.term != Term::Unreachable {
                problems.push(at("an abort in a block that does not end unreachable".to_string()));
            }
        }
        for value in operands(function, inst.op) {
            if value.0 as usize >= function.values.len() {
                problems.push(at(format!("names ${}, which is not a value", value.0)));
            }
        }
        for slot in slots_of(function, inst.op) {
            if slot as usize >= function.slots.len() {
                problems.push(at(format!("names slot {slot}, which does not exist")));
            }
        }
    }
}

/// §4.8: **copy-out happens always.** Every block that returns must write back
/// every `@` parameter, and this is the check that says so — one per exit edge, no
/// exceptions for early `return` or for the error side of `?`.
/// **Which** parameters, not how many.
///
/// This counted `CopyOut`s and compared the number to the number of `@`
/// parameters — so two copy-outs of one parameter and none of the other passed,
/// which is precisely the wrong program §4.8 exists to refuse. `CopyOut` carries
/// the identity in `param`, and the check threw it away along with the params
/// list (`let _ = function;`). `phases.rs` twenty lines up already records that
/// counting was rejected as a proxy for the decref sweep, for the same reason, on
/// the same kind of evidence (2026-08-12, sweep 001 audit S8).
fn check_copy_out(
    function: &Function,
    block: &Block,
    mutable: usize,
    at: &dyn Fn(String) -> String,
    problems: &mut Vec<String>,
) {
    if !matches!(block.term, Term::Return(_)) || mutable == 0 {
        return;
    }
    let mut written: Vec<u32> = block
        .insts
        .iter()
        .filter_map(|inst| match inst.op {
            Op::CopyOut { param } => Some(param.0),
            _ => None,
        })
        .collect();
    written.sort_unstable();
    let mut owed: Vec<u32> = function
        .params
        .iter()
        .filter(|slot| {
            matches!(function.slots[slot.0 as usize].kind, SlotKind::Param { mutable: true })
        })
        .map(|slot| slot.0)
        .collect();
    owed.sort_unstable();
    if written != owed {
        let missing: Vec<String> = owed
            .iter()
            .filter(|slot| !written.contains(slot))
            .map(|slot| format!("s{slot}"))
            .collect();
        // A duplicate is an adjacent pair once sorted — multiplicity, not
        // membership: copying `s0` out twice and `s1` never is two writes for
        // two parameters, which is exactly what counting could not see.
        let mut extra: Vec<String> = Vec::new();
        for pair in written.windows(2) {
            if pair[0] == pair[1] && !extra.contains(&format!("s{}", pair[0])) {
                extra.push(format!("s{}", pair[0]));
            }
        }
        for slot in &written {
            if !owed.contains(slot) {
                let name = format!("s{slot}");
                if !extra.contains(&name) {
                    extra.push(name);
                }
            }
        }
        let mut said =
            format!("returns after copying out {} of {mutable} `@` parameters", written.len());
        if !missing.is_empty() {
            said.push_str(&format!(" — never {}", missing.join(", ")));
        }
        if !extra.is_empty() {
            said.push_str(&format!(" — twice {}", extra.join(", ")));
        }
        problems.push(at(said));
    }
}

fn check_return_type(
    function: &Function,
    block: &Block,
    checked: &Checked,
    at: &dyn Fn(String) -> String,
    problems: &mut Vec<String>,
) {
    let unit = checked.types.unit();
    match block.term {
        Term::Return(Some(value)) => {
            if (value.0 as usize) < function.values.len() {
                let got = function.value_type(value);
                if got != function.result && got != checked.types.error() {
                    problems.push(at("returns a value of the wrong type".to_string()));
                }
            }
        }
        // A body that runs off its end returns nothing. That is right for a `()`
        // result and a *program* error otherwise, which the frontend does not
        // reject yet — so the verifier does not either: `-Werror=return-type` is
        // the net at M-scalars-run (CLAUDE.md §7), and the diagnostic class is queued.
        Term::Return(None) => {
            let _ = unit;
        }
        _ => {}
    }
}

