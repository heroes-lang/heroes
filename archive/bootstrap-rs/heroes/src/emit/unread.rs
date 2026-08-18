//! What an instruction still means when **nobody reads its result**.
//!
//! Split from `inst.rs` by the §11 ceiling, 2026-08-15, and the seam is not a line
//! count: nothing here emits anything. `inst.rs` answers *what does this
//! instruction look like in C*; these two answer *may it lose its destination, and
//! may it disappear entirely* — the question the emitter asks **before** printing,
//! and the one `-Wunused-but-set-variable` turns on. They are `pub(super)` and read
//! from elsewhere in the backend, which is the other half of the argument: a
//! question other files ask is not a private detail of the printer.
//!
//! **`body.rs`'s liveness half joined it the same day, and that is what named the
//! file.** Four functions there asked the same question of a whole function rather
//! than of one op — which temporaries the emitted C actually assigns and reads —
//! and they were sitting next to the printer that consumes their answer. Together
//! the two halves are one concern: *what the emitted C reads*, per op and per
//! function. `body.rs` prints; this decides what is worth printing.

use crate::ir::{Function, Op};
use crate::types::Checked;

use super::ctype::is_unit;

/// Whether this instruction is still correct with its **destination dropped** —
/// the question `-Wunused-but-set-variable` turns on, asked of the op.
///
/// Two groups may lose it. A **pure read** simply vanishes: nothing happens, so
/// nothing is lost. A **call** is emitted for its effect and merely goes
/// unassigned, which is what `_ = f(x)` means.
///
/// Everything else keeps its destination, and each for a reason worth stating.
/// `Index`, `Cast`, `Unary` and `Binary` can **abort** — out of bounds, out of
/// range, on overflow — and their emitters print the check *with* the
/// assignment, so dropping one drops the other. `Construct` and `MapGet` build a
/// value, which for a refcounted type is a retain. `Store`, `CopyOut`, `Incref`,
/// `Decref` and `Abort` are effects with no destination to drop in the first
/// place. `Hole` and `Missing` never reach a backend.
///
/// **The match is exhaustive on purpose, and that is the repair.** This was a
/// three-name list — `Call`, `Load`, `Const` — resting on the premise that
/// nothing else could produce a value the emitted C never reads. `_ = f(x)?` on
/// a fallible produces a `Payload` that does, and the premise died in silence
/// with a clang warning as its only trace: found by a corpus program, three
/// milestones after the list was written (fixedbugs, 2026-08-13). A new `Op` now
/// has to answer this question before the compiler will build — which is
/// CLAUDE.md §11's rule, that a narrowing asks the value and never the world.
pub(super) fn may_lose_its_destination(op: &Op) -> bool {
    match op {
        Op::Const(_)
        | Op::Load(_)
        | Op::Field { .. }
        | Op::Payload { .. }
        | Op::Tag(_)
        | Op::Len(_)
        | Op::Call { .. } => true,
        Op::FuncRef(_)
        | Op::Store { .. }
        | Op::Unary { .. }
        | Op::Binary { .. }
        | Op::Cast { .. }
        | Op::Construct { .. }
        | Op::Index { .. }
        | Op::MapGet { .. }
        | Op::CopyOut { .. }
        | Op::Abort { .. }
        | Op::Incref(_)
        | Op::Decref(_)
        | Op::Hole
        | Op::Missing => false,
    }
}

/// Whether the instruction disappears **entirely** when its result is unread —
/// the narrower half of `may_lose_its_destination`, and the one a fixpoint has
/// to iterate over.
///
/// A discarded `Call` still prints its call, so it still reads its arguments; a
/// discarded pure read prints nothing at all, so its operand loses a reader and
/// may itself become unread. That chain is why `emitted_reads` is a loop.
pub(super) fn vanishes_when_unread(op: &Op) -> bool {
    may_lose_its_destination(op) && !matches!(op, Op::Call { .. })
}

/// Whether any *emitted* instruction assigns this temporary. `$t0` (the unit) is
/// assigned by nothing and named by nothing, and a temporary that only exists
/// inside a block nobody reaches would be an unused variable.
pub(super) fn assigned(function: &Function, live: &[bool], value: u32) -> bool {
    for (index, block) in function.blocks.iter().enumerate() {
        if !live[index] {
            continue;
        }
        for one in &block.insts {
            if one.dest == Some(crate::ir::ValueId(value)) {
                return true;
            }
        }
    }
    false
}

/// A temporary that a call, a load or a constant writes and nothing reads:
/// `_ = f(x)`, and the load `r.must()` leaves behind when the payload is `()`.
///
/// The pair of this and `inst::emit`'s own test is what keeps the declaration and
/// the assignment in step — they agree because they ask the same question of the
/// same set. The three ops are the safe ones: a pure result may disappear, but an
/// *aborting* op's must not, or dropping the assignment drops the check.
pub(super) fn discarded(
    function: &Function,
    live: &[bool],
    value: u32,
    read: &std::collections::BTreeSet<u32>,
) -> bool {
    if read.contains(&value) {
        return false;
    }
    for (index, block) in function.blocks.iter().enumerate() {
        if !live[index] {
            continue;
        }
        for one in &block.insts {
            if one.dest == Some(crate::ir::ValueId(value)) {
                return super::unread::may_lose_its_destination(&one.op);
            }
        }
    }
    false
}

/// What the **emitted C** reads, which is not what the IR references.
///
/// An instruction whose result is unit emits nothing at all (`is_unit` in
/// `inst::emit`), so its operands are referenced by the IR and read by no C. The
/// case that produced this is `()?`: `r.must()` lowers to a load and a payload
/// extraction, the extraction's result is `()`, and the load was left assigned
/// and unread — `-Wunused-but-set-variable` on a correct program.
///
/// **A fixpoint, and the chain that made it one arrived on schedule.** This was
/// one pass, with the limit written down rather than discovered: *a suppressed
/// reader whose own operand is produced by another suppressed reader would still
/// leave a warning; no such chain exists today. If one appears, this is where it
/// is answered, and the answer is a worklist.* `_ = f(x)?` on a fallible is that
/// chain — the payload extraction vanishes, and the load that fed it becomes
/// unread in the same step — and it arrived with a corpus program on 2026-08-13.
/// So the note is now the code.
///
/// Each round asks which instructions print nothing, and an instruction that
/// prints nothing reads nothing. `gone` only grows and is bounded by the number
/// of values, so the loop ends.
pub(super) fn emitted_reads(
    function: &Function,
    checked: &Checked,
) -> std::collections::BTreeSet<u32> {
    let mut gone: std::collections::BTreeSet<u32> = std::collections::BTreeSet::new();
    loop {
        let read = reads_given(function, checked, &gone);
        let mut grew = false;
        for block in &function.blocks {
            for one in &block.insts {
                let Some(dest) = one.dest else { continue };
                if gone.contains(&dest.0) || read.contains(&dest.0) {
                    continue;
                }
                if super::unread::vanishes_when_unread(&one.op) {
                    gone.insert(dest.0);
                    grew = true;
                }
            }
        }
        if !grew {
            return read;
        }
    }
}

/// One round: what the C reads, given the instructions already known to print
/// nothing.
fn reads_given(
    function: &Function,
    checked: &Checked,
    gone: &std::collections::BTreeSet<u32>,
) -> std::collections::BTreeSet<u32> {
    let mut read = std::collections::BTreeSet::new();
    for block in &function.blocks {
        for one in &block.insts {
            // A pure op with a unit result prints nothing, so it reads nothing.
            // The case that produced this rule is `()?`: `r.must()` lowers to a
            // load and a payload extraction, the extraction's result is `()`, and
            // the load was left assigned and unread.
            let silent = one.dest.is_some()
                && is_unit(checked, one.ty)
                && !matches!(one.op, crate::ir::Op::Call { .. } | crate::ir::Op::Abort { .. });
            if silent || one.dest.is_some_and(|d| gone.contains(&d.0)) {
                continue;
            }
            for value in crate::ir::uses::operands(function, one.op) {
                read.insert(value.0);
            }
        }
        for value in crate::ir::uses::terminator_operands(&block.term) {
            read.insert(value.0);
        }
    }
    read
}

/// Whether anything in this function **reads** the slot — the third question this
/// file answers, and the one `-Wunused-but-set-variable` turns on for a *named*
/// binding rather than a temporary.
///
/// **`_ = v` is the only way to reach it**, and that is what makes it worth
/// answering rather than suppressing: an unused binding is already a compile error
/// (§4.4), so a slot that is written and never read exists exactly when the author
/// wrote the discard and said *I deliberately do not use this*. The C is then
/// honest — `int64_t h0_v;` really is set and never used — and clang is right to
/// say so. What is missing is the emitter telling C that the silence is on
/// purpose, which is `__attribute__((unused))`, the same idiom `extern_complete.rs`
/// already writes on its probes.
///
/// **A refcounted slot never reaches here and the reason is structural, not a
/// carve-out**: the exit sweep loads it to `decref`, so it is read by
/// construction. Measured — `s = "ziggy"` then `_ = s` emits `t4 = h0_s;` and
/// `t5 = h0_s;` and draws no warning at all. So the rule asks *does anything read
/// it*, which is a fact about this function, rather than *is it refcounted*, which
/// would be a premise about the ownership pass that could expire (CLAUDE.md §11).
///
/// Conservative in the safe direction: anything that so much as mentions the slot
/// counts as a read. The attribute means *may* be unused, never *must* be, so an
/// over-count silences nothing and an under-count only leaves the warning standing.
pub(super) fn slot_is_read(
    function: &Function,
    live: &[bool],
    read: &std::collections::BTreeSet<u32>,
    slot: crate::ir::SlotId,
) -> bool {
    let touches = |place: &crate::ir::Place| place.root == slot;
    function.blocks.iter().enumerate().any(|(block, b)| {
        live[block]
            && b.insts.iter().any(|inst| match &inst.op {
                // **A load only counts if the load itself survives into C**, which
                // is the whole subtlety: `_ = v` lowers to a load whose destination
                // is then discarded, so the slot is read in the IR and not in the
                // emitted C. Asking the IR alone answers *true* for every discard
                // and the rule catches nothing.
                Op::Load(place) => {
                    touches(place)
                        && inst
                            .dest
                            .is_some_and(|d| !discarded(function, live, d.0, read))
                }
                // A store *through a path* reads the root on the way (copy-on-write
                // walks it); a store straight to the slot is the write this asks
                // about.
                Op::Store { place, .. } => touches(place) && place.path.len > 0,
                // §4.8's copy-out reads the local and writes it back through the
                // pointer.
                Op::CopyOut { param } => *param == slot,
                _ => false,
            })
    })
}
