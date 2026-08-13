//! One copy of a function, with its type parameters replaced (design.md §4.12;
//! panel 029 R4c, R6).
//!
//! Split out of `mono.rs` by the §11 sweep. `mono.rs` decides *which* instances
//! exist; this decides what one instance *is*.
//!
//! **Every type in the body goes through `apply`, which interns as it
//! substitutes** — so the descriptor the emitter later asks for comes from the
//! substituted `TyId` through `descriptors::pointer`, and is never carried across
//! as a name. That is the rule this file exists to keep, and the reason it is worth
//! a file: a wrong descriptor here is invisible to clang, ASan, UBSan and the leak
//! counter alike. `[-0.0] == [0.0]` would print `false` at exit 0.

use crate::types::{Checked, Ty, TyId};

use super::Function;

/// One copy of `template`, with every `Ty::Generic(i)` replaced by `args[i]`.
pub(super) fn instantiate(template: &Function, args: &[TyId], checked: &mut Checked) -> Function {
    let slots = template
        .slots
        .iter()
        .map(|slot| super::Slot {
            name: slot.name.clone(),
            ty: apply(checked, slot.ty, args),
            kind: slot.kind,
        })
        .collect();
    let values = template.values.iter().map(|v| apply(checked, *v, args)).collect();
    let blocks = template
        .blocks
        .iter()
        .map(|block| super::Block {
            preds: block.preds.clone(),
            insts: block
                .insts
                .iter()
                .map(|inst| super::Inst {
                    dest: inst.dest,
                    op: inst.op,
                    ty: apply(checked, inst.ty, args),
                    span: inst.span,
                })
                .collect(),
            term: clone_term(&block.term),
            note: block.note.clone(),
        })
        .collect();
    Function {
        name: template.name.clone(),
        decl: template.decl,
        kind: template.kind,
        generics: Vec::new(),
        params: template.params.clone(),
        result: apply(checked, template.result, args),
        slots,
        blocks,
        values,
        args: template.args.clone(),
        steps: template.steps.clone(),
        instance: args.to_vec(),
        span: template.span,
    }
}

/// `Term` is not `Copy` — it carries a `switch`'s arm table — so the clone is
/// written out rather than derived, which keeps a new terminator kind a compile
/// error here rather than a silently dropped arm.
fn clone_term(term: &super::Term) -> super::Term {
    match term {
        super::Term::Return(value) => super::Term::Return(*value),
        super::Term::Jump(block) => super::Term::Jump(*block),
        super::Term::Branch { cond, then, otherwise } => {
            super::Term::Branch { cond: *cond, then: *then, otherwise: *otherwise }
        }
        super::Term::Switch { tag, cases } => {
            super::Term::Switch { tag: *tag, cases: cases.clone() }
        }
        super::Term::Unreachable => super::Term::Unreachable,
        super::Term::Open => super::Term::Open,
    }
}

/// `ty` with the type parameters substituted, interning whatever is new.
pub(super) fn apply(checked: &mut Checked, ty: TyId, args: &[TyId]) -> TyId {
    match checked.types.get(ty) {
        Ty::Generic(position) => match args.get(position as usize) {
            Some(bound) => *bound,
            // The checker reported an uninferable parameter; this keeps the shape.
            None => ty,
        },
        Ty::Array(element) => {
            let element = apply(checked, element, args);
            checked.types.intern(Ty::Array(element))
        }
        Ty::Fallible(inner) => {
            let inner = apply(checked, inner, args);
            checked.types.intern(Ty::Fallible(inner))
        }
        Ty::Map(key, value) => {
            let key = apply(checked, key, args);
            let value = apply(checked, value, args);
            checked.types.intern(Ty::Map(key, value))
        }
        Ty::Func { params, result } => {
            let spelled: Vec<TyId> = checked
                .types
                .params_of(params)
                .into_iter()
                .map(|p| apply(checked, p, args))
                .collect();
            let result = apply(checked, result, args);
            checked.types.func(&spelled, result)
        }
        _ => ty,
    }
}
