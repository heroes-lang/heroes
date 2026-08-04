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
use super::uses::operands;
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
        Phase::Owned => released_on_return(function, block, checked, at, problems),
    }
    if program.phase == Phase::Owned {
        released_where_they_are_read(function, block, at, problems);
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
    let owed = function
        .slots
        .iter()
        .enumerate()
        .filter(|(index, slot)| {
            matches!(slot.kind, SlotKind::Local | SlotKind::Synthetic)
                && is_refcounted(checked, slot.ty)
                && !function.params.contains(&SlotId(*index as u32))
        })
        .count();
    let released = block.insts.iter().filter(|inst| matches!(inst.op, Op::Decref(_))).count();
    if released < owed {
        problems
            .push(at(format!("returns after releasing {released} of {owed} owned slots")));
    }
}

/// A temporary the pass **releases** is read only in the block that releases it.
///
/// This is the invariant that makes `own.rs`'s cheap classification safe. That pass
/// decrefs an owning temporary at the end of its defining block instead of computing
/// liveness, so what must hold is that nothing reads such a value from another block —
/// where, physically, it may already be freed.
///
/// The first wording was "a **counted** temporary is read only in the block that defines
/// it", and it fired immediately on `assert s == "x"`: the `assert` lowering computes
/// both operands in the test block and reads them in the abort block, and one of them is
/// the source text. That read is **harmless**, because a string literal is a static
/// block the pass never releases — so the property is not about being counted, it is
/// about being *released*. Keying the check on where the `Decref` actually is says
/// exactly that, and needs no second copy of the pass's own classification.
///
/// Panel 021 predicted the `assert` collision and queued it as an M6 repair. It arrived
/// early, and what it corrected was the invariant rather than the lowering.
fn released_where_they_are_read(
    function: &Function,
    block: &Block,
    at: &dyn Fn(String) -> String,
    problems: &mut Vec<String>,
) {
    for (index, other) in function.blocks.iter().enumerate() {
        for inst in &other.insts {
            let Op::Decref(released) = inst.op else { continue };
            // Only a temporary: the exit sweep decrefs slots through a fresh load, and
            // that load is defined in the block that reads it.
            let defined_here = block.insts.iter().any(|one| one.dest == Some(released));
            if defined_here {
                continue;
            }
            let read_here = block
                .insts
                .iter()
                .any(|one| operands(function, one.op).contains(&released));
            if read_here {
                problems.push(at(format!(
                    "reads ${}, which bb{index} releases — a released temporary never \
                     crosses a block",
                    released.0
                )));
            }
        }
    }
}
