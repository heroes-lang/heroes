//! A written type becomes a `TyId` (design.md §4.3, §4.6, §4.12, §4.13).
//!
//! The resolver already answered the hard half: which declaration or primitive
//! each `Named` node refers to (`Resolved::type_uses`). This file only rebuilds
//! the shape, interning as it goes, so `[Point?]` written twice in one file is
//! one id both times.
//!
//! Nothing here reports a diagnostic. An unresolved type name was already
//! reported by the resolver, and saying it twice is the one thing this compiler
//! never does — it becomes `Ty::Error`, which every rule downstream stays quiet
//! about.

use crate::resolve::{Prim, Resolved, TypeRef};
use crate::syntax::{Ast, TypeId, TypeKind};

use super::table::Ty;
use super::{Checker, TyId};

pub(super) fn ty(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    id: TypeId,
) -> TyId {
    match &ast.types[id.0 as usize].kind {
        TypeKind::Named => named(checker, resolved, id),
        TypeKind::Unit => checker.out.types.unit(),
        TypeKind::Error => checker.error_ty(),
        TypeKind::Array(inner) => {
            let inner = ty(checker, ast, resolved, *inner);
            checker.out.types.intern(Ty::Array(inner))
        }
        TypeKind::Fallible(inner) => {
            let inner = ty(checker, ast, resolved, *inner);
            checker.out.types.intern(Ty::Fallible(inner))
        }
        TypeKind::Map(key, value) => {
            let key = ty(checker, ast, resolved, *key);
            let value = ty(checker, ast, resolved, *value);
            checker.out.types.intern(Ty::Map(key, value))
        }
        TypeKind::Func { params, result } => {
            let params: Vec<TyId> =
                params.iter().map(|p| ty(checker, ast, resolved, *p)).collect();
            let result = ty(checker, ast, resolved, *result);
            checker.out.types.func(&params, result)
        }
    }
}

fn named(checker: &mut Checker, resolved: &Resolved, id: TypeId) -> TyId {
    match resolved.type_at(id) {
        TypeRef::Prim(prim) => {
            let ty = match prim {
                Prim::Int => Ty::Int,
                Prim::F64 => Ty::F64,
                Prim::Bool => Ty::Bool,
                Prim::Str => Ty::Str,
                Prim::Ptr => Ty::Ptr,
                Prim::Cstr => Ty::Cstr,
            };
            checker.out.types.intern(ty)
        }
        TypeRef::Top(decl) => checker.out.types.intern(Ty::Named(decl)),
        TypeRef::Generic(position) => checker.out.types.intern(Ty::Generic(position)),
        // The resolver already reported it.
        TypeRef::Unresolved => checker.error_ty(),
    }
}
