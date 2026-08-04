//! The ownership pass: values acquire lifetimes (design.md Part 5, §4.10; panel
//! 021).
//!
//! The **first IR→IR pass**, and Part 5 was amended to say so: the original wording
//! was "during lowering", and a separate pass is what makes `--dump-ir` show the
//! result and keeps the emitter a printer. Swift's SIL is the precedent —
//! `strong_retain`/`strong_release` are instructions, and OSSA's form "can be
//! validated statically as not containing use after free errors or leaked memory".
//! LLVM D92808 records the failure mode of the alternative: ARC's `retainRV` pairing
//! lived only in the backend, and passes separated the calls from their markers.
//!
//! **Five rules, and every one of them has a compiled counterexample** (panel 021
//! R2, where a judge wrote the C and ran it under ASan):
//!
//! 1. **A plain parameter is borrowed.** It is excluded from the exit sweep, which
//!    covers local and synthetic slots only. Heroes parameters are immutable, so a
//!    borrowed parameter is never overwritten and never needs releasing — and the
//!    caller increfs nothing per argument. Taking the other reading was the *first*
//!    program written for the panel, and ASan failed it on its first run with a
//!    heap-use-after-free.
//! 2. **An `@` parameter is moved in and moved out.** The prologue's copy-in takes no
//!    reference and the copy-out (`*p_s = s`) replaces the decref, so the callee's
//!    copy-in consumes the caller's reference and the copy-out hands one back. M5a's
//!    pointer ABI is unchanged.
//! 3. **A store increfs the new value before decrefing the old.** `s @ s` otherwise
//!    frees the buffer and then increfs a dead one. Without the decref, a
//!    string-building loop leaked 999 blocks — measured.
//! 4. **A returned value is increfed before the sweep**, or `return prefix + name`
//!    is a use-after-free: silent at `-O0`, and a heap-use-after-free under ASan.
//! 5. **An owning temporary is decrefed at the end of its defining block.** A value
//!    is owning iff its defining op *allocates*. `print(a + b)` leaks without this,
//!    and the alternative — a liveness pass — would spend exactly what panel 019
//!    bought by choosing slots over phi nodes: cleanup as a table walk.
//!
//! Rule 5 is made safe by a **checked invariant** rather than by an analysis: at
//! phase `Owned` the verifier asserts that a refcounted temporary is read only in the
//! block that defines it. That is the same move `ir/values.rs` represents — buy the
//! check, not the machinery.
//!
//! A `Load` is **borrowed** (+0), which is what makes the whole scheme uniform: a
//! store increfs, a return increfs, a call argument does neither, and the only +1s in
//! the program come from allocating instructions and are released at the end of their
//! block.

use crate::ir::{is_refcounted, Function, Inst, Op, Phase, Place, Program, Shape, SlotKind, Term};
use crate::source::Span;
use crate::types::{Checked, TyId};

/// Insert the reference counting. Idempotent by construction: `advance_to` refuses a
/// second run.
pub fn run(program: &mut Program, checked: &Checked) {
    for function in program.functions.iter_mut() {
        rewrite(function, checked);
    }
    program.advance_to(Phase::Owned);
}

fn rewrite(function: &mut Function, checked: &Checked) {
    // The sweep's targets, computed once: local and synthetic slots whose type is
    // counted. Parameters are borrowed (rule 1) and `@` parameters are moved out
    // (rule 2), so neither appears here.
    let sweep: Vec<u32> = function
        .slots
        .iter()
        .enumerate()
        .filter(|(_, slot)| {
            matches!(slot.kind, SlotKind::Local | SlotKind::Synthetic)
                && is_refcounted(checked, slot.ty)
        })
        .map(|(index, _)| index as u32)
        .collect();

    let mut next_value = function.values.len() as u32;
    for index in 0..function.blocks.len() {
        let mut out: Vec<Inst> = Vec::new();
        let mut owning: Vec<crate::ir::ValueId> = Vec::new();
        for inst in std::mem::take(&mut function.blocks[index].insts) {
            // Rule 3: the old value dies when the slot is overwritten, and the new
            // one must be alive before it does.
            if let Op::Store { place, value } = inst.op {
                let ty = slot_type(function, place);
                if is_refcounted(checked, ty) && place.path.len == 0 {
                    let old = fresh(&mut next_value, function, ty);
                    out.push(plain(Some(old), Op::Load(place), ty, inst.span));
                    out.push(plain(None, Op::Incref(value), ty, inst.span));
                    out.push(inst);
                    out.push(plain(None, Op::Decref(old), ty, inst.span));
                    continue;
                }
            }
            if let Some(dest) = inst.dest {
                if allocates(inst.op) && is_refcounted(checked, inst.ty) {
                    owning.push(dest);
                }
            }
            out.push(inst);
        }
        // Rule 4, then the sweep, then rule 5 — in that order, because the returned
        // value may be one of the temporaries the last step releases.
        let term = function.blocks[index].term.clone();
        let span = out.last().map(|i| i.span).unwrap_or(function.span);
        if let Term::Return(Some(value)) = term {
            let ty = function.value_type(value);
            if is_refcounted(checked, ty) {
                out.push(plain(None, Op::Incref(value), ty, span));
            }
        }
        if matches!(term, Term::Return(_)) {
            for slot in &sweep {
                let ty = function.slots[*slot as usize].ty;
                let loaded = fresh(&mut next_value, function, ty);
                let place = Place {
                    root: crate::ir::SlotId(*slot),
                    path: crate::ir::Steps { start: 0, len: 0 },
                };
                out.push(plain(Some(loaded), Op::Load(place), ty, span));
                out.push(plain(None, Op::Decref(loaded), ty, span));
            }
        }
        for value in owning {
            let ty = function.value_type(value);
            out.push(plain(None, Op::Decref(value), ty, span));
        }
        function.blocks[index].insts = out;
    }
}

/// A value the pass invented. It extends `Function::values`, which is dense over
/// `ValueId`, so the type table stays complete.
fn fresh(next: &mut u32, function: &mut Function, ty: TyId) -> crate::ir::ValueId {
    let id = crate::ir::ValueId(*next);
    *next += 1;
    function.values.push(ty);
    id
}

fn plain(dest: Option<crate::ir::ValueId>, op: Op, ty: TyId, span: Span) -> Inst {
    Inst { dest, op, ty, span }
}

fn slot_type(function: &Function, place: Place) -> TyId {
    function.slots[place.root.0 as usize].ty
}

/// Whether this operation hands back a **new** reference. Everything else either
/// borrows (a `Load`, a field read) or produces something uncounted.
///
/// A call is owning because a Heroes function increfs what it returns (rule 4), and a
/// built-in is owning because the runtime's constructors return +1. A `Const` of a
/// string literal is **not**: a literal is a static block whose refcount is negative,
/// so nothing owns it and decrefing it is a no-op.
/// **`+` on a `str` allocates**, and forgetting it was this pass's first defect: the
/// leak counter said `3 heap blocks still live at exit` on the first program with a
/// string in it, and all three traced to one missing row. The concatenation is a
/// `Binary`, which reads like arithmetic and is a constructor — the operator table
/// (§4.14) puts `+` on `str` in the same row as `+` on `int`, and only the *result
/// type* tells them apart. This function is called only when the result is
/// refcounted, so `Op::Binary` here can be nothing else.
fn allocates(op: Op) -> bool {
    match op {
        Op::Call { .. } | Op::Construct { .. } | Op::Cast { .. } | Op::Binary { .. } => true,
        // M5c: a field read of a counted field, an index, a map get, and a payload
        // all hand back a reference and will need the same treatment. They are
        // refused by the gate today, and listed rather than defaulted so that
        // switching one on is a decision here.
        Op::Field { .. } | Op::Index { .. } | Op::MapGet { .. } | Op::Payload { .. } => false,
        Op::Const(_)
        | Op::Load(_)
        | Op::Store { .. }
        | Op::Unary { .. }
        | Op::Len(_)
        | Op::Tag(_)
        | Op::FuncRef(_)
        | Op::CopyOut { .. }
        | Op::Abort { .. }
        | Op::Incref(_)
        | Op::Decref(_)
        | Op::Hole
        | Op::Missing => false,
    }
}

/// A `Construct` that only wraps: it takes ownership of its argument rather than
/// making a new reference. Listed for M5c, where `ok(s)` lands.
#[allow(dead_code)]
fn wraps(shape: Shape) -> bool {
    matches!(shape, Shape::Ok | Shape::Err)
}
