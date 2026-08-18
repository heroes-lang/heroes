//! A place: what a mutation writes, and what an `@` argument names
//! (design.md §4.4's `@`, §4.8 mutable parameters, §4.9 field and index access).
//!
//! §4.8 already wrote the specification for this file in one sentence — **"every
//! place has exactly one root"** — and that is why a place is a slot plus a path
//! of steps rather than an expression. `l.pos @ l.pos + 1` writes *through* `l`;
//! `xs[i] @ 0` writes through `xs`. Nothing in the language can produce a place
//! whose root is not a binding, which is also why aliasing cannot exist (§4.3).
//!
//! Panel 019 found this missing from the proposed instruction set: without a
//! place, a store's destination would have to be a value, and `l.pos @ l.pos + 1`
//! — a line in design.md's own appendix — would be unwritable.
//!
//! Evaluation order is left to right: the path's index expressions are lowered
//! before the value being stored, because `xs[f()] @ g()` calls `f` first.

use crate::resolve::{Ref, Resolved};
use crate::source::Source;
use crate::syntax::{Ast, ExprId, ExprKind};
use crate::types::Checked;

use super::build::Lowering;
use super::ids::{Place, SlotId, Step};
use super::{exprs, layout, SlotKind};

pub(super) fn place(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    id: ExprId,
) -> Place {
    let mut steps = Vec::new();
    let root = walk(b, ast, resolved, checked, src, id, &mut steps);
    let path = b.steps(&steps);
    Place { root, path }
}

/// Descends to the root, pushing steps on the way back out, so the path reads
/// root-first — the order the emitter needs to spell `l.pos` and the order a
/// reader expects.
fn walk(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    id: ExprId,
    steps: &mut Vec<Step>,
) -> SlotId {
    let node = &ast.exprs[id.0 as usize];
    match &node.kind {
        ExprKind::Name => match resolved.use_at(id) {
            Ref::Local(local) => match b.slot_of_local(local) {
                Some(slot) => slot,
                None => orphan(b, checked, id),
            },
            _ => orphan(b, checked, id),
        },
        ExprKind::Field { base, name } => {
            let owner = checked.expr_types[base.0 as usize];
            let root = walk(b, ast, resolved, checked, src, *base, steps);
            if let Some(index) = layout::field_index(ast, checked, src, owner, *name) {
                steps.push(Step::Field(index));
            }
            root
        }
        ExprKind::Index { base, index } => {
            let root = walk(b, ast, resolved, checked, src, *base, steps);
            let key = exprs::expr(b, ast, resolved, checked, src, *index);
            steps.push(Step::Index(key));
            root
        }
        // Unreachable on a tree that checked clean: the parser only accepts a
        // name, a field path or an index path on the left of `@` (§4.4), and the
        // resolver rejected a root that is not a binding. A slot is invented
        // rather than panicking, because a pass that panics on a shape it did not
        // expect is a pass that cannot be trusted with the next one.
        _ => orphan(b, checked, id),
    }
}

fn orphan(b: &mut Lowering, checked: &Checked, id: ExprId) -> SlotId {
    let ty = checked.expr_types[id.0 as usize];
    b.slot("$orphan".to_string(), ty, SlotKind::Synthetic)
}
