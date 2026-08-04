//! Slots, runs, and the `$` names — the half of the pass's state that holds a
//! value (design.md §4.4 bindings, §4.8 places; panel 019 point 2).
//!
//! **A synthetic name cannot collide.** It starts with `$`, and `$` is not in the
//! language — not an identifier character, not an operator, not a comment marker.
//! This is the llm-ergonomist's condition, and the reason it is a condition rather
//! than a preference is that `t0: int @ 0` is legal Heroes: without the sigil the
//! dump would be ambiguous on input the author may legally write.
//!
//! The two run arenas (`args`, `steps`) are here for the Cyclone rule (CLAUDE.md
//! §5): a call's arity is variable and a place's path is variable, and holding
//! either in a `Vec` inside `Op` would stop it being `Copy`. `Ty` keeps a function
//! type's parameters the same way, for the same reason.

use crate::source::Span;
use crate::types::TyId;

use super::build::Lowering;
use super::inst::{Arg, Args, Op, Place, SlotId, Step, Steps, ValueId};
use super::{Slot, SlotKind};

impl Lowering {
    // --- slots -----------------------------------------------------------

    pub fn param(&mut self, name: String, ty: TyId, mutable: bool) -> SlotId {
        let slot = self.slot(name, ty, SlotKind::Param { mutable });
        self.func.params.push(slot);
        slot
    }

    pub fn slot(&mut self, name: String, ty: TyId, kind: SlotKind) -> SlotId {
        self.func.slots.push(Slot { name, ty, kind });
        SlotId(self.func.slots.len() as u32 - 1)
    }

    /// A slot the author did not write: `$i0` for a loop index, `$r0` for a
    /// branch's value, `$s0` for a scrutinee held across arms.
    pub fn synthetic(&mut self, prefix: &str, ty: TyId) -> SlotId {
        let count = self.counters.entry(prefix.to_string()).or_insert(0);
        let name = format!("${prefix}{count}");
        *count += 1;
        self.slot(name, ty, SlotKind::Synthetic)
    }

    /// Binds a resolver local to a slot, so every later use of that name loads
    /// from the same place.
    pub fn bind_local(&mut self, local: u32, slot: SlotId) {
        self.slots_by_local.insert(local, slot);
    }

    pub fn slot_of_local(&self, local: u32) -> Option<SlotId> {
        self.slots_by_local.get(&local).copied()
    }

    /// The local index a binding name belongs to, by where the name starts.
    pub fn local_at(&self, name: Span) -> Option<u32> {
        self.locals_by_span.get(&name.start).copied()
    }

    pub fn slot_type(&self, slot: SlotId) -> TyId {
        self.func.slots[slot.0 as usize].ty
    }

    pub fn value_type(&self, value: ValueId) -> TyId {
        self.func.values[value.0 as usize]
    }

    /// The `@` parameters, in declaration order — what every exit edge owes a
    /// `CopyOut` to (§4.8).
    pub fn mutable_params(&self) -> Vec<SlotId> {
        self.func
            .params
            .iter()
            .copied()
            .filter(|slot| {
                matches!(self.func.slots[slot.0 as usize].kind, SlotKind::Param { mutable: true })
            })
            .collect()
    }

    // --- runs ------------------------------------------------------------

    pub fn args(&mut self, args: &[Arg]) -> Args {
        let start = self.func.args.len() as u32;
        self.func.args.extend_from_slice(args);
        Args { start, len: args.len() as u32 }
    }

    pub fn steps(&mut self, steps: &[Step]) -> Steps {
        let start = self.func.steps.len() as u32;
        self.func.steps.extend_from_slice(steps);
        Steps { start, len: steps.len() as u32 }
    }

    /// The place that is just a slot — by far the commonest, and it allocates no
    /// steps.
    pub fn whole(&self, root: SlotId) -> Place {
        Place { root, path: Steps { start: 0, len: 0 } }
    }

    pub fn load(&mut self, slot: SlotId, span: Span) -> ValueId {
        let place = self.whole(slot);
        let ty = self.slot_type(slot);
        self.emit(Op::Load(place), ty, span)
    }

    pub fn store(&mut self, slot: SlotId, value: ValueId, span: Span) {
        let place = self.whole(slot);
        let ty = self.slot_type(slot);
        self.emit_void(Op::Store { place, value }, ty, span);
    }
}
