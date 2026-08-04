//! What an instruction reads (design.md Part 5's ownership pass; panel 019).
//!
//! One function per question — the values an operation reads, the values a
//! terminator reads, the slots an operation names — and each is an exhaustive
//! `match`, so an `Op` variant added without listing its operands is a **compile**
//! error rather than a verifier that quietly stops checking it.
//!
//! Split out of `verify.rs` because it is not only the verifier's question. M5b's
//! ownership pass inserts `incref`/`decref` by walking definitions and uses, and two
//! tables that disagree about which is which would be a refcount bug that reproduces
//! once a week. There is one table, and it is this file.

use super::inst::{Arg, Callee, Op, Place, Step, Term, ValueId};
use super::Function;

/// Every value an operation reads. One function, so a new `Op` variant that forgets
/// to list its operands fails the exhaustiveness check rather than the verifier.
pub(super) fn operands(function: &Function, op: Op) -> Vec<ValueId> {
    match op {
        Op::Const(_) | Op::Hole | Op::Missing | Op::CopyOut { .. } | Op::FuncRef(_) => Vec::new(),
        Op::Load(place) => path_values(function, place),
        Op::Store { place, value } => {
            let mut values = path_values(function, place);
            values.push(value);
            values
        }
        Op::Unary { operand, .. } | Op::Cast { operand, .. } => vec![operand],
        // A refcount operation reads the value it adjusts. Saying so here is what
        // makes the dominance check cover the ownership pass's own output.
        Op::Incref(value) | Op::Decref(value) => vec![value],
        Op::Binary { left, right, .. } => vec![left, right],
        Op::Call { callee, args, .. } => {
            let mut values = match callee {
                Callee::Indirect(value) => vec![value],
                _ => Vec::new(),
            };
            values.extend(arg_values(function, args));
            values
        }
        Op::Construct { args, .. } | Op::Abort { args, .. } => arg_values(function, args),
        Op::Field { base, .. } | Op::Payload { base, .. } => vec![base],
        Op::Index { base, index } => vec![base, index],
        Op::MapGet { map, key } => vec![map, key],
        Op::Len(value) | Op::Tag(value) => vec![value],
    }
}

pub(super) fn terminator_operands(term: &Term) -> Vec<ValueId> {
    match term {
        Term::Branch { cond, .. } => vec![*cond],
        Term::Switch { tag, .. } => vec![*tag],
        Term::Return(Some(value)) => vec![*value],
        Term::Return(None) | Term::Jump(_) | Term::Unreachable | Term::Open => Vec::new(),
    }
}

pub(super) fn arg_values(function: &Function, args: super::inst::Args) -> Vec<ValueId> {
    let mut values = Vec::new();
    if args.start as usize + args.len as usize > function.args.len() {
        return values;
    }
    for arg in function.args_of(args) {
        match arg {
            Arg::Value(value) => values.push(value),
            Arg::InOut(place) => values.extend(path_values(function, place)),
        }
    }
    values
}

pub(super) fn path_values(function: &Function, place: Place) -> Vec<ValueId> {
    if place.path.start as usize + place.path.len as usize > function.steps.len() {
        return Vec::new();
    }
    function
        .steps_of(place.path)
        .into_iter()
        .filter_map(|step| match step {
            Step::Index(value) => Some(value),
            Step::Field(_) => None,
        })
        .collect()
}

pub(super) fn slots_of(function: &Function, op: Op) -> Vec<u32> {
    let _ = function;
    match op {
        Op::Load(place) | Op::Store { place, .. } => vec![place.root.0],
        Op::CopyOut { param } => vec![param.0],
        _ => Vec::new(),
    }
}
