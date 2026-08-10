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
//! **Six rules, and every one of them has a compiled counterexample** — five from
//! panel 021 R2, where a judge wrote the C and ran it under ASan, and the sixth from
//! the first program in this project that held a `str` inside a record:
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
//!    string-building loop leaked 999 blocks — measured. **A field path is the same
//!    store**: `p.cells @ [7, 8]` reaches a counted place through a record the slot
//!    already owns, and reading the *root slot's* type instead of the stored value's
//!    left it uncounted — one leaked block, printed by the leak counter on the first
//!    program that wrote one. An *index* path is not this rule: it needs
//!    copy-on-write, and the gate refuses it until the primitive exists.
//! 4. **A returned value is increfed before the sweep**, or `return prefix + name`
//!    is a use-after-free: silent at `-O0`, and a heap-use-after-free under ASan.
//! 5. **An owning temporary is MOVED into a synthetic slot, in the block that defines
//!    it.** Nothing is ever owned by a temporary past its own instruction, so nothing
//!    has to be released at a block boundary — and that is what cashes panel 019's
//!    reason for choosing slots over phi nodes, properly this time: **ownership lives
//!    in slots, and cleanup is a walk over a table.**
//!
//! 6. **An aggregate constructor retains every counted field it captures.** A record
//!    is by value, so `Pair(one: p, two: q)` copies the `str` handle inside `p` and the
//!    new value is a second owner of one block. `ok(x)` is the opposite — it *wraps*,
//!    consuming its argument — which is why the shapes are listed one at a time.
//!
//! Rule 5 was first written the way panel 021 costed it — decref an owning temporary
//! at the end of its defining block, made safe by a checked invariant — and the
//! verifier refuted it on two real programs within the hour. `.must()`, `?`, `&&` and
//! `if`-as-an-expression all open a block in the *middle* of an expression, so
//! `name + " scored " + got.must().to_str()` computes a concatenation in one block and
//! consumes it in another. That is not a corner case; it is what any expression
//! containing a fallible call looks like. The alternative would have been liveness —
//! the thing choosing slots was supposed to avoid — and the move to a slot avoids it
//! for real, because a slot's release point is already known.
//!
//! The consequence is a uniform reading: **every value that reaches a store is
//! borrowed**, because the only +1 in the program was moved into a slot the
//! instruction after it was created. So a store always increfs, a return always
//! increfs, a call argument does neither, and every release is a slot's.
//!
//! The cost is over-retention rather than a leak: a string is held by its synthetic
//! slot until the function returns, even if nothing reads it again. Performance is a
//! non-goal (Part 2) and correctness is not, and a loop does not accumulate — the
//! synthetic slot is overwritten each iteration, and the overwrite releases.

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

/// **Two walks, and the order is the whole correctness argument.** The first inserts
/// the increfs and the moves, which is what *creates* the synthetic slots; the second
/// appends the exit sweeps, which need the slot list to be complete. Done in one walk,
/// a block that returns early sweeps only the slots invented before it — which the
/// verifier caught immediately as `returns without releasing $own7`.
fn rewrite(function: &mut Function, checked: &Checked) {
    let mut next_value = function.values.len() as u32;
    for index in 0..function.blocks.len() {
        let mut out: Vec<Inst> = Vec::new();
        for inst in std::mem::take(&mut function.blocks[index].insts) {
            // Rule 3: the old value dies when the slot is overwritten, and the new one
            // must be alive before it does. Every value reaching a store is borrowed
            // (rule 5 moved the owning ones out), so the incref is unconditional.
            if let Op::Store { place, value } = inst.op {
                // The *stored value's* type, not the root slot's: for a whole-slot
                // store they are the same, and for a path store only the value's is
                // right. `p.cells @ [7, 8]` writes a counted `[int]` into a `Row` slot
                // that is itself counted for a different reason, and reading the root's
                // type answered a question nobody asked.
                let ty = function.value_type(value);
                // An index step needs copy-on-write and its own primitive, so it is
                // refused by the gate rather than counted here (panel 022). A field
                // path needs nothing extra: the record is ours, in a slot we own.
                let indexed = function
                    .steps_of(place.path)
                    .iter()
                    .any(|step| matches!(step, crate::ir::Step::Index(_)));
                // An indexed store hands the value OVER: `hero_array_set` releases the
                // element that was there and moves the new one in, so the pass increfs
                // and does not decref. The order matters and this is where it is set —
                // the incref precedes the store, therefore the outermost unshare,
                // because the value may live inside the container being copied
                // (`n.children[0] @ n`).
                if is_refcounted(checked, ty) && indexed {
                    out.push(plain(None, Op::Incref(value), ty, inst.span));
                    out.push(inst);
                    continue;
                }
                if is_refcounted(checked, ty) && !indexed {
                    let old = fresh(&mut next_value, function, ty);
                    out.push(plain(Some(old), Op::Load(place), ty, inst.span));
                    out.push(plain(None, Op::Incref(value), ty, inst.span));
                    out.push(inst);
                    out.push(plain(None, Op::Decref(old), ty, inst.span));
                    continue;
                }
            }
            // **Rule 6: an aggregate constructor retains every counted field it
            // captures.** A record is by value, so `Pair(one: p, two: q)` copies the
            // bytes of `p` — including the `str` handle inside it — and the new value
            // is a second owner of the same block. Nothing else in the pass adds that
            // reference: rule 5 moved the *result* into a slot, which is about the
            // aggregate, and every value reaching an argument is borrowed by rule 5's
            // own consequence.
            //
            // Found by running it: `both = Pair(one: p, two: q)` printed correctly and
            // then double-freed at exit, with ASan naming `hero_str_decref` and the
            // magic word already clobbered — the program was right for its whole
            // visible life and wrong once, at the sweep.
            if let Op::Construct { shape, args } = inst.op {
                if retains_fields(shape) {
                    for arg in function.args_of(args) {
                        if let crate::ir::Arg::Value(value) = arg {
                            let ty = function.value_type(value);
                            if is_refcounted(checked, ty) {
                                out.push(plain(None, Op::Incref(value), ty, inst.span));
                            }
                        }
                    }
                }
            }
            let owning = inst
                .dest
                .filter(|_| allocates(inst.op) && is_refcounted(checked, inst.ty))
                .map(|dest| (dest, inst.ty, inst.span));
            out.push(inst);
            // Rule 5: move it into a slot, now, in this block. The reference is
            // *moved*, so there is no incref and no decref of the temporary — which is
            // what makes every later value borrowed.
            if let Some((dest, ty, span)) = owning {
                let slot = own_slot(function, ty);
                let place = whole(slot);
                let old = fresh(&mut next_value, function, ty);
                out.push(plain(Some(old), Op::Load(place), ty, span));
                out.push(plain(None, Op::Store { place, value: dest }, ty, span));
                out.push(plain(None, Op::Decref(old), ty, span));
            }
        }
        function.blocks[index].insts = out;
    }
    // The second walk. Rules 1 and 2 decide the list: local and synthetic slots only,
    // because a plain parameter is borrowed and an `@` parameter is copied out. Taking
    // the other reading was the first program panel 021's ffi-pragmatist compiled, and
    // ASan failed it on the first run with a heap-use-after-free.
    let sweep: Vec<u32> = function
        .slots
        .iter()
        .enumerate()
        .filter(|(index, slot)| {
            matches!(slot.kind, SlotKind::Local | SlotKind::Synthetic)
                && is_refcounted(checked, slot.ty)
                && !function.params.contains(&crate::ir::SlotId(*index as u32))
        })
        .map(|(index, _)| index as u32)
        .collect();
    for index in 0..function.blocks.len() {
        let term = function.blocks[index].term.clone();
        if !matches!(term, Term::Return(_)) {
            continue;
        }
        let span =
            function.blocks[index].insts.last().map(|i| i.span).unwrap_or(function.span);
        let mut out = std::mem::take(&mut function.blocks[index].insts);
        // Rule 4, before the sweep: the returned value is borrowed by here — it lives
        // in a slot — so the caller's reference has to be a new one.
        if let Term::Return(Some(value)) = term {
            let ty = function.value_type(value);
            if is_refcounted(checked, ty) {
                out.push(plain(None, Op::Incref(value), ty, span));
            }
        }
        for slot in &sweep {
            let ty = function.slots[*slot as usize].ty;
            let loaded = fresh(&mut next_value, function, ty);
            let place = whole(crate::ir::SlotId(*slot));
            out.push(plain(Some(loaded), Op::Load(place), ty, span));
            out.push(plain(None, Op::Decref(loaded), ty, span));
        }
        function.blocks[index].insts = out;
    }
}

/// A synthetic slot for one owning temporary. Its name starts with `$`, which no
/// Heroes program contains, so it can collide with nothing the author wrote.
fn own_slot(function: &mut Function, ty: TyId) -> crate::ir::SlotId {
    let index = function.slots.len() as u32;
    function.slots.push(crate::ir::Slot {
        name: format!("$own{}", index),
        ty,
        kind: SlotKind::Synthetic,
    });
    crate::ir::SlotId(index)
}

fn whole(slot: crate::ir::SlotId) -> Place {
    Place { root: slot, path: crate::ir::Steps { start: 0, len: 0 } }
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
pub(crate) fn allocates(op: Op) -> bool {
    match op {
        Op::Call { .. } | Op::Construct { .. } | Op::Cast { .. } | Op::Binary { .. } => true,
        // **`m[k]` allocates**, and it is the only one of these four that does. The
        // other three hand back a *borrowed* view — a field read, an element read and a
        // payload read all copy bytes out of something that keeps its own reference — but
        // a map lookup builds a `V?` around the value, and building it copies the value
        // through its descriptor, which increfs. So the `V?` arrives owning something.
        //
        // The comment this replaces said "listed rather than defaulted so that switching
        // one on is a decision here", and this is that decision. It was found by the leak
        // counter: `{1: "one" + "!"}` then `.default(...)` leaked exactly one block, and
        // only with a heap-allocated value — a `str` literal has a negative refcount, so
        // the same missing decref is invisible.
        Op::MapGet { .. } => true,
        Op::Field { .. } | Op::Index { .. } | Op::Payload { .. } => false,
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

/// Whether a constructor makes its result a **second owner** of every counted
/// argument (rule 6).
///
/// A record and a variant case do: they hold the field by value, so two places now
/// name one block. `ok(x)` and the `?`-propagating `err` do **not** — they *wrap*,
/// taking ownership of the argument rather than adding a reference — and an array or
/// a map literal is the same wrap repeated per element, because `push` moves. Listed
/// one shape at a time rather than defaulted, so that landing a container is a
/// decision here.
fn retains_fields(shape: Shape) -> bool {
    match shape {
        // A record, a variant case and both sides of a `T?` all hold their argument by
        // value, so the new value is a second owner. `ok(x)` was first written as a
        // *wrap* that consumes its argument, on panel 021's note — but rule 5 came
        // later and made every value reaching a constructor borrowed, so consuming one
        // is a release the program never made. The note is older than the rule.
        Shape::Record(_) | Shape::Case(_, _) => true,
        Shape::Ok | Shape::Err | Shape::Fail => true,
        // A container literal is built by `push`, whose `copy` already takes the
        // array's own reference per element.
        Shape::Array | Shape::Map => false,
    }
}
