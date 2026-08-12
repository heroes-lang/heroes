//! Every invariant, **broken on purpose** — one test per check in `verify.rs`.
//!
//! Copied from LLVM, whose `test/Verifier/README.txt` states the purpose in one
//! sentence: "This directory contains testcases that the verifier is supposed to
//! detect as malformed LLVM code." Before this file, `verify.rs` was 257 lines of
//! assertions that nothing tested: it ran on every build, said nothing, and there was
//! no evidence any of it could speak. A safety net nobody has ever fallen into is
//! indistinguishable from no net.
//!
//! The method is the one thing that differs from LLVM's, and it differs because
//! Heroes has no IR *parser*: there is no way to write a broken function down as
//! text. So each case lowers a correct program and then damages the data structure
//! — which has a side benefit LLVM's text cases lack, in that the starting point is
//! always something the real lowering produced.
//!
//! Each test names the message it expects — LLVM's cases assert the text too, with a
//! `; CHECK:` line beside the malformed construct. That is deliberate: a check that
//! fires with the wrong message sends the reader to the wrong place, and at exit
//! code 2 the reader is the person maintaining this compiler.

use super::{owned, unverified};
use crate::ir::{verify, Abort, Callee, Inst, Op, Place, Steps, Term, ValueId};

/// A one-function program with a loop, a branch and a return — enough structure for
/// most of the damage below.
const SUBJECT: &str = "function f(n: int) -> int\n    total: int @ 0\n    i: int @ 0\n    while i < n\n        total @ total + i\n        i @ i + 1\n    return total\n";

/// A function with a mutable parameter, for the copy-out checks.
const MUTATING: &str = "record R\n    pos: int\n\nfunction step(@r: R, n: int) -> int\n    r.pos @ r.pos + n\n    return n\n";

/// **Two** mutable parameters — the shape that separates counting from checking.
const TWO_MUTABLE: &str = "record R\n    pos: int\n\nfunction both(@a: R, @b: R, n: int) -> int\n    a.pos @ a.pos + n\n    b.pos @ b.pos + n\n    return n\n";

fn says(problems: &[String], expected: &str) {
    assert!(
        problems.iter().any(|p| p.contains(expected)),
        "expected a problem mentioning {expected:?}, got {problems:?}"
    );
}

/// The control. Everything below damages this same program, so if the undamaged
/// version ever stopped verifying, every test in this file would be meaningless.
#[test]
fn the_undamaged_subject_verifies_clean() {
    let (program, checked) = unverified(SUBJECT);
    assert_eq!(verify(&program, &checked), Vec::<String>::new());
}

#[test]
fn a_block_with_no_terminator_is_caught() {
    let (mut program, checked) = unverified(SUBJECT);
    program.functions[0].blocks[1].term = Term::Open;
    says(&verify(&program, &checked), "no terminator");
}

#[test]
fn a_jump_to_a_block_that_does_not_exist_is_caught() {
    let (mut program, checked) = unverified(SUBJECT);
    program.functions[0].blocks[0].term = Term::Jump(crate::ir::BlockId(99));
    says(&verify(&program, &checked), "jumps to bb99, which does not exist");
}

/// The check that keeps `preds` honest, and the reason it exists: M-strings-ownership's cleanup
/// chains walk edges, so a `preds` list that disagrees with the terminators is a
/// refcount bug waiting for a milestone.
#[test]
fn preds_that_disagree_with_the_terminators_are_caught() {
    let (mut program, checked) = unverified(SUBJECT);
    program.functions[0].blocks[1].preds.clear();
    says(&verify(&program, &checked), "jump here");
}

/// `Op::Missing` is M-ir-lowering's scaffolding for a form the lowering does not handle. The
/// verifier is what stops it reaching a golden or a backend.
#[test]
fn a_form_the_lowering_skipped_is_caught() {
    let (mut program, checked) = unverified(SUBJECT);
    program.functions[0].blocks[0].insts[0].op = Op::Missing;
    says(&verify(&program, &checked), "a form the lowering does not handle");
}

#[test]
fn the_address_of_an_address_is_caught() {
    let (mut program, checked) = unverified(SUBJECT);
    program.functions[0].blocks[0].insts[0].op = Op::FuncRef(Callee::Indirect(ValueId(1)));
    says(&verify(&program, &checked), "the address of an address");
}

/// An abort ends the program, so nothing may follow it — and its block must say
/// `unreachable`, which is what becomes `hero_unreachable()` in C (CLAUDE.md §7).
#[test]
fn an_abort_with_instructions_after_it_is_caught() {
    let (mut program, checked) = unverified(SUBJECT);
    let function = &mut program.functions[0];
    let unit = checked.types.unit();
    let args = crate::ir::Args { start: 0, len: 0 };
    let abort = Inst {
        dest: None,
        op: Op::Abort { reason: Abort::Must, args },
        ty: unit,
        span: function.span,
    };
    function.blocks[0].insts.insert(0, abort);
    let problems = verify(&program, &checked);
    says(&problems, "an abort with instructions after it");
    says(&problems, "does not end unreachable");
}

#[test]
fn a_value_used_and_never_produced_is_caught() {
    let (mut program, checked) = unverified(SUBJECT);
    // Delete the instruction that produced $t1, leaving the store that reads it.
    program.functions[0].blocks[0].insts.remove(0);
    says(&verify(&program, &checked), "is used and never produced");
}

/// A temporary is assigned once *by construction* — this test is what proves the
/// construction, not the intention.
#[test]
fn a_value_assigned_twice_is_caught() {
    let (mut program, checked) = unverified(SUBJECT);
    let first = program.functions[0].blocks[0].insts[0];
    program.functions[0].blocks[0].insts.insert(1, first);
    says(&verify(&program, &checked), "is assigned twice");
}

#[test]
fn a_value_that_is_not_a_value_is_caught() {
    let (mut program, checked) = unverified(SUBJECT);
    program.functions[0].blocks[0].insts[1].op = Op::Store {
        place: Place { root: crate::ir::SlotId(0), path: Steps { start: 0, len: 0 } },
        value: ValueId(999),
    };
    says(&verify(&program, &checked), "which is not a value");
}

#[test]
fn a_slot_that_does_not_exist_is_caught() {
    let (mut program, checked) = unverified(SUBJECT);
    program.functions[0].blocks[0].insts[1].op = Op::Store {
        place: Place { root: crate::ir::SlotId(99), path: Steps { start: 0, len: 0 } },
        value: ValueId(1),
    };
    says(&verify(&program, &checked), "names slot 99, which does not exist");
}

/// §4.8's "copy-out happens always", as a check that can fail. This is the invariant
/// panel 000 named as the place this project would actually stall, so it is the one
/// this file exists for.
#[test]
fn a_return_that_forgets_to_copy_out_is_caught() {
    let (mut program, checked) = unverified(MUTATING);
    let step = program.functions.iter_mut().find(|f| f.name == "step").expect("step");
    for block in &mut step.blocks {
        block.insts.retain(|inst| !matches!(inst.op, Op::CopyOut { .. }));
    }
    says(&verify(&program, &checked), "returns after copying out 0 of 1");
}

/// **Copying one parameter out twice and the other not at all**, which is the
/// wrong program §4.8 refuses and which the check could not see while it counted
/// `CopyOut`s and compared the number to the number of `@` parameters: two and
/// two, so it said nothing (2026-08-12, sweep 001 audit S8).
///
/// `phases.rs` had already rejected counting as a proxy for the decref sweep,
/// twenty lines away and for the same reason. `CopyOut` carries the parameter's
/// identity; the check threw it away along with the params list.
#[test]
fn copying_one_parameter_out_twice_and_the_other_never_is_caught() {
    let (mut program, checked) = unverified(TWO_MUTABLE);
    let both = program.functions.iter_mut().find(|f| f.name == "both").expect("both");
    // Whichever slot the first copy-out names, make the second one name it too.
    let mut first: Option<Op> = None;
    for block in &mut both.blocks {
        for inst in &mut block.insts {
            if !matches!(inst.op, Op::CopyOut { .. }) {
                continue;
            }
            match first {
                None => first = Some(inst.op),
                Some(op) => inst.op = op,
            }
        }
    }
    says(&verify(&program, &checked), "twice");
}

#[test]
fn a_return_of_the_wrong_type_is_caught() {
    let (mut program, checked) = unverified(MUTATING);
    let step = program.functions.iter_mut().find(|f| f.name == "step").expect("step");
    // $t1 is the record loaded from `r`; the function promised an `int`.
    for block in &mut step.blocks {
        if matches!(block.term, Term::Return(Some(_))) {
            block.term = Term::Return(Some(ValueId(1)));
        }
    }
    says(&verify(&program, &checked), "returns a value of the wrong type");
}

/// §4.19: an `extern`'s implementation is C's. Blocks on one would mean the emitter
/// had two answers to "who defines this".
#[test]
fn an_extern_with_a_body_is_caught() {
    let (mut program, checked) = unverified(
        "extern \"math.h\"\n    function sqrt(x: f64) -> f64\n\nfunction f() -> f64\n    return sqrt(2.0)\n",
    );
    let body = program.functions[1].blocks.remove(0);
    program.functions[0].blocks.push(body);
    says(&verify(&program, &checked), "an extern has a body");
}

#[test]
fn a_switch_with_no_cases_is_caught() {
    let (mut program, checked) = unverified(
        "variant Step\n    stop\n    skip\n\nfunction f(s: Step) -> int\n    return match s\n        .stop => 0\n        .skip => 1\n",
    );
    for block in &mut program.functions[0].blocks {
        if let Term::Switch { tag, .. } = block.term {
            block.term = Term::Switch { tag, cases: Vec::new() };
        }
    }
    says(&verify(&program, &checked), "a switch with no cases");
}

/// The message names the function and the block, because a dump of a large program
/// is not where anyone wants to start searching. Worth pinning: an exit-2 message
/// that says only *what* is wrong and not *where* costs the reader the whole file.
#[test]
fn a_problem_names_the_function_and_the_block() {
    let (mut program, checked) = unverified(SUBJECT);
    // The exit block, deliberately: it ends in a `return`, so opening its terminator
    // removes no edge and the damage stays one problem. Opening the loop body's
    // instead reports two — the missing terminator *and* the predecessor that
    // vanished with it, which is the verifier being right and the test being
    // careless.
    program.functions[0].blocks[3].term = Term::Open;
    let problems = verify(&program, &checked);
    assert_eq!(problems.len(), 1, "{problems:?}");
    // The phase is in the message from M-strings-ownership on, because "which pass produced this" is
    // the first question a violation raises and the compiler knows the answer.
    assert_eq!(problems[0], "f (after lowering): bb3: no terminator");
}

/// **Dominance** (panel 020, M-scalars-run). A value read in a block its definition does not
/// dominate becomes, in the emitted C, a read of an uninitialised prologue local —
/// which `-Werror=uninitialized` reports as `variable 't3' is used uninitialized`
/// against the *author's* line. The verifier says it first, and says whose fault it
/// is.
///
/// The damage: move the loop body's `add` into the block after the loop, where the
/// path that skips the loop never passes through it.
#[test]
fn a_use_its_definition_does_not_dominate_is_caught() {
    let (mut program, checked) = unverified(SUBJECT);
    let function = &mut program.functions[0];
    // Find the block that computes the addition, and the exit block that returns.
    let mut moved: Option<Inst> = None;
    for block in function.blocks.iter_mut() {
        if let Some(position) = block.insts.iter().position(|i| {
            matches!(i.op, Op::Binary { op: crate::ir::BinOp::Add, .. })
        }) {
            moved = Some(block.insts.remove(position));
            break;
        }
    }
    let moved = moved.expect("the loop body adds");
    let exit = function
        .blocks
        .iter()
        .position(|b| matches!(b.term, Term::Return(_)))
        .expect("an exit block");
    function.blocks[exit].insts.insert(0, moved);
    says(&verify(&program, &checked), "does not dominate it");
}

/// The same check, from the other side: a definition in the *entry* block dominates
/// everything, so reading it anywhere is legal. This is the case that killed the
/// first version of the invariant — written as "a temporary is read only in the block
/// that defines it", it condemned the `assert` lowering, which computes both sides of
/// the comparison in the test block and reads them in the abort block.
#[test]
fn a_temporary_may_cross_a_block_when_its_definition_dominates() {
    let (program, checked) = unverified(
        "function main()\n    assert 1 + 1 == 2\n",
    );
    assert_eq!(verify(&program, &checked), Vec::<String>::new());
}

// --- M-strings-ownership: the invariants that depend on which pass has run (panel 021) --------

/// A refcount operation before the ownership pass is a lowering that has done the
/// pass's job — which would then do it again.
#[test]
fn a_refcount_before_the_ownership_pass_is_caught() {
    let (mut program, checked) = unverified(SUBJECT);
    let value = program.functions[0].values.len() as u32 - 1;
    let inst = Inst {
        dest: None,
        op: Op::Incref(ValueId(value)),
        ty: checked.types.unit(),
        span: program.functions[0].span,
    };
    program.functions[0].blocks[0].insts.push(inst);
    says(&verify(&program, &checked), "a refcount operation before the ownership pass");
    // …and the message names the pass to suspect.
    says(&verify(&program, &checked), "after lowering");
}

/// The `Owned` half: a returning block that does not release a slot it owns is a leak,
/// and the runtime's counter would find it — this finds it at compile time and names the
/// slot.
#[test]
fn a_return_that_leaves_a_slot_unreleased_is_caught() {
    let (mut program, checked) =
        owned("function f(n: str) -> str\n    s: str @ \"x\"\n    return s + n\n");
    let exit = program.functions[0]
        .blocks
        .iter()
        .position(|b| matches!(b.term, crate::ir::Term::Return(_)))
        .expect("an exit block");
    // Drop the last decref: one slot now goes unreleased, and the message says which.
    let insts = &mut program.functions[0].blocks[exit].insts;
    let last = insts
        .iter()
        .rposition(|inst| matches!(inst.op, Op::Decref(_)))
        .expect("the sweep released something");
    insts.remove(last);
    says(&verify(&program, &checked), "a slot it owns");
    says(&verify(&program, &checked), "after the ownership pass");
}

/// And the invariant that replaces liveness: an owning temporary must be **moved** into
/// a slot in the block that defines it. Remove the store and the reference has nowhere
/// to live, which is what an end-of-block release would have had to guess about.
#[test]
fn an_owning_temporary_that_no_store_takes_is_caught() {
    let (mut program, checked) = owned("function f(n: str) -> str\n    return n + n\n");
    let function = &mut program.functions[0];
    for block in function.blocks.iter_mut() {
        // The pass's own move: a store whose value is a concatenation's result.
        if let Some(at) = block.insts.iter().position(|inst| {
            matches!(inst.op, Op::Store { .. }) && inst.dest.is_none()
        }) {
            block.insts.remove(at);
            break;
        }
    }
    says(&verify(&program, &checked), "never carries ownership across an edge");
}

/// The phase cannot go backwards, and a pass cannot run twice.
#[test]
#[should_panic(expected = "cannot go back")]
fn the_phase_is_monotonic() {
    let (mut program, checked) = owned("function f() -> int\n    return 1\n");
    let _ = &checked;
    program.advance_to(crate::ir::Phase::Owned);
}
