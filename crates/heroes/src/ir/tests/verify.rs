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

use super::unverified;
use crate::ir::{verify, Abort, Callee, Inst, Op, Place, Steps, Term, ValueId};

/// A one-function program with a loop, a branch and a return — enough structure for
/// most of the damage below.
const SUBJECT: &str = "function f(n: int) -> int\n    total: int @ 0\n    i: int @ 0\n    while i < n\n        total @ total + i\n        i @ i + 1\n    return total\n";

/// A function with a mutable parameter, for the copy-out checks.
const MUTATING: &str = "record R\n    pos: int\n\nfunction step(@r: R, n: int) -> int\n    r.pos @ r.pos + n\n    return n\n";

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

/// The check that keeps `preds` honest, and the reason it exists: M5b's cleanup
/// chains walk edges, so a `preds` list that disagrees with the terminators is a
/// refcount bug waiting for a milestone.
#[test]
fn preds_that_disagree_with_the_terminators_are_caught() {
    let (mut program, checked) = unverified(SUBJECT);
    program.functions[0].blocks[1].preds.clear();
    says(&verify(&program, &checked), "jump here");
}

/// `Op::Missing` is M4's scaffolding for a form the lowering does not handle. The
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
        "extern function sqrt(x: f64) -> f64\n\nfunction f() -> f64\n    return sqrt(2.0)\n",
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
    assert_eq!(problems[0], "f: bb3: no terminator");
}
