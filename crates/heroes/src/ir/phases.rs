//! What must be true **after which pass** (panel 021 R8).
//!
//! Before the ownership pass there is no reference counting; after it, every exit edge
//! releases what it owns. Naming the phase is what makes the second half assertable at
//! all: a missing `decref` in a `Lowered` program is correct, and the same absence in
//! an `Owned` one is a leak.
//!
//! The phase lives on the `Program` rather than in `verify`'s signature, and the reason
//! decided it: `verify(program, checked, Lowered)` on an owned program would silently
//! skip these checks — a verifier that quietly stops checking. As a field of the thing
//! it describes, it cannot disagree with it.
//!
//! The precedent is three-fold and was audited verbatim: rustc bakes the attribution in
//! (`validate_body(tcx, body, format!("after pass {pass_name}"))`), LLVM's
//! `--verify-each` exists "for cases where it is suspected that a pass is creating an
//! invalid module but it is not clear which pass is doing it", and Go runs `checkFunc`
//! "between each phase" under `-d=ssa/check/on` — which caught a real ARM bug in
//! `runtime/malloc.go` (golang/go#22499).
//!
//! The `Owned` half is also what makes `own.rs`'s cheap classification *safe*. That
//! pass releases an owning temporary at the end of its defining block rather than
//! computing liveness, so what must be true is that a counted temporary is never read
//! anywhere else — checked here rather than argued there. The same trade `values.rs`
//! represents: buy the check, not the machinery.

use crate::types::Checked;

use super::inst::{Op, SlotId, Term};
use super::{is_refcounted, Block, Function, Phase, Program, SlotKind};

pub(super) fn check(
    program: &Program,
    function: &Function,
    block: &Block,
    checked: &Checked,
    at: &dyn Fn(String) -> String,
    problems: &mut Vec<String>,
) {
    match program.phase {
        Phase::Lowered => {
            if block.insts.iter().any(|inst| matches!(inst.op, Op::Incref(_) | Op::Decref(_))) {
                problems.push(at("a refcount operation before the ownership pass".to_string()));
            }
        }
        // **No `Ty::Generic` survives monomorphisation**, and this is where that is
        // asserted rather than hoped for. One that did would reach `is_refcounted`,
        // which answers `false` for it — right for `T = int`, a leak for `T = str`
        // (panel 029 R1). `mono::tests` makes it fire.
        Phase::Mono => {
            no_generic_survives(function, block, checked, at, problems);
        }
        Phase::Owned => {
            no_generic_survives(function, block, checked, at, problems);
            released_on_return(function, block, checked, at, problems);
            owning_temporaries_move_into_slots(block, checked, at, problems);
        }
    }
}

/// Every type in this block is concrete.
///
/// Checked at `Mono` **and** at `Owned`, because the second is where a survivor
/// would do its damage: the ownership pass asks `is_refcounted`, which reads a
/// table that has no honest answer for a type parameter.
fn no_generic_survives(
    function: &Function,
    block: &Block,
    checked: &Checked,
    at: &dyn Fn(String) -> String,
    problems: &mut Vec<String>,
) {
    let mut complain = |ty: crate::types::TyId, what: &str| {
        if mentions_generic(checked, ty) {
            problems.push(at(format!("{what} still has a type parameter in it")));
        }
    };
    for inst in &block.insts {
        complain(inst.ty, "an instruction");
    }
    for slot in &function.slots {
        complain(slot.ty, "a slot");
    }
    complain(function.result, "the result");
}

/// Does this type mention a `Ty::Generic` anywhere inside it?
fn mentions_generic(checked: &Checked, ty: crate::types::TyId) -> bool {
    match checked.types.get(ty) {
        crate::types::Ty::Generic(_) => true,
        crate::types::Ty::Array(element) => mentions_generic(checked, element),
        crate::types::Ty::Fallible(inner) => mentions_generic(checked, inner),
        crate::types::Ty::Map(key, value) => {
            mentions_generic(checked, key) || mentions_generic(checked, value)
        }
        crate::types::Ty::Func { params, result } => {
            checked.types.params_of(params).iter().any(|p| mentions_generic(checked, *p))
                || mentions_generic(checked, result)
        }
        _ => false,
    }
}

/// A returning block releases every local and synthetic slot it owns.
///
/// Parameters are **borrowed** and `@` parameters are **copied out**, so neither is
/// counted here — which is the calling convention, asserted rather than trusted. Taking
/// the other reading was the first program panel 021's ffi-pragmatist compiled, and ASan
/// failed it on the first run with a heap-use-after-free.
fn released_on_return(
    function: &Function,
    block: &Block,
    checked: &Checked,
    at: &dyn Fn(String) -> String,
    problems: &mut Vec<String>,
) {
    if !matches!(block.term, Term::Return(_)) {
        return;
    }
    // Which slots this block released, and **after their last write** — which is the
    // only reading that distinguishes a sweep from a store.
    //
    // Counting `Decref`s was the first version and it was a proxy rather than a check.
    // Matching a `Decref` to a `Load` of the same slot was the second, and it could not
    // tell the two apart either: a store to a counted slot *also* loads the old value
    // and decrefs it, and that pair is byte-identical to a sweep pair. What separates
    // them is position — the store's load necessarily precedes the store, and the
    // sweep's necessarily follows it. Removing a sweep decref by hand and watching the
    // check stay silent is how that was found.
    let last_write = |slot: u32| -> Option<usize> {
        block.insts.iter().rposition(|inst| match inst.op {
            Op::Store { place, .. } => place.root.0 == slot && place.path.len == 0,
            _ => false,
        })
    };
    let released_after = |slot: u32, after: Option<usize>| -> bool {
        let floor = after.map(|at| at + 1).unwrap_or(0);
        for (index, inst) in block.insts.iter().enumerate().skip(floor) {
            let Op::Decref(value) = inst.op else { continue };
            let loaded = block.insts[..index].iter().skip(floor).any(|one| {
                one.dest == Some(value)
                    && matches!(one.op, Op::Load(place) if place.root.0 == slot && place.path.len == 0)
            });
            if loaded {
                return true;
            }
        }
        false
    };
    for (index, slot) in function.slots.iter().enumerate() {
        let index = index as u32;
        let owed = matches!(slot.kind, SlotKind::Local | SlotKind::Synthetic)
            && is_refcounted(checked, slot.ty)
            && !function.params.contains(&SlotId(index));
        if owed && !released_after(index, last_write(index)) {
            problems.push(at(format!(
                "returns without releasing `{}`, a slot it owns",
                slot.name
            )));
        }
    }
}

/// **Every owning temporary is moved into a slot in the block that defines it.**
///
/// This is the invariant that replaces liveness. `own.rs` never releases a temporary —
/// it moves the reference into a synthetic slot the instruction after it appears — so
/// nothing owned crosses a block edge, and every release is a slot's, at a point
/// already known. Checked here rather than trusted there.
///
/// Two earlier wordings were refuted, and the record is worth keeping because both were
/// too strong rather than too weak. "A **counted** temporary is read only in the block
/// that defines it" fired on `assert s == "x"`, whose operands legitimately cross into
/// the abort block — harmlessly, since a literal is static. "A **released** temporary
/// never crosses a block" then fired on `name + " scored " + got.must().to_str()`,
/// where `.must()` opens a block in the middle of an expression: not a corner case, but
/// what any expression containing a fallible call looks like. Both were found by an
/// existing test or a gallery program within the hour of being written.
fn owning_temporaries_move_into_slots(
    block: &Block,
    checked: &Checked,
    at: &dyn Fn(String) -> String,
    problems: &mut Vec<String>,
) {
    for inst in &block.insts {
        let Some(dest) = inst.dest else { continue };
        if !allocates(inst.op) || !is_refcounted(checked, inst.ty) {
            continue;
        }
        let moved = block.insts.iter().any(|one| match one.op {
            Op::Store { value, .. } => value == dest,
            _ => false,
        });
        if !moved {
            problems.push(at(format!(
                "${} owns a reference and no store in this block takes it — a temporary \
                 never carries ownership across an edge",
                dest.0
            )));
        }
    }
}

/// Which operations hand back a new reference. It has to agree with `own.rs`'s own
/// answer, and the only way to guarantee that is for there to be one — so this is the
/// pass's function, re-exported rather than reimplemented.
fn allocates(op: Op) -> bool {
    crate::own::allocates(op)
}
