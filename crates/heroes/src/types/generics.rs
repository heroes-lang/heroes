//! §4.12's inference: on functions only, no constraints, never written at the
//! call site.
//!
//! Two functions, and between them they are the whole of it. `bind` walks a
//! parameter's written type beside the argument's actual type; the first time a
//! type parameter meets something, it takes it, and every later meeting must
//! agree. `substitute` then rewrites the result type with what was bound.
//!
//! There are **no unification variables and no constraint set** — which is what
//! keeps §4.5's promise that errors stay local: a mismatch is reported at the
//! argument that disagreed, not at the end of a solve.

use crate::source::{Source, Span};
use crate::syntax::Ast;

use super::table::Ty;
use super::{errors, Checker, TyId};


/// §4.12's inference, in one direction: a type parameter meets an argument and
/// takes its type; meeting a second one, it must agree.
pub(super) fn bind(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    param: TyId,
    got: TyId,
    bindings: &mut Vec<Option<TyId>>,
    span: Span,
) {
    match checker.out.types.get(param) {
        Ty::Generic(position) => {
            let slot = position as usize;
            if slot >= bindings.len() {
                return;
            }
            match bindings[slot] {
                None => bindings[slot] = Some(got),
                Some(bound) if bound == got => {}
                Some(bound) => {
                    if !checker.out.types.poisoned(got) {
                        let (a, b) =
                            (checker.show(ast, src, bound), checker.show(ast, src, got));
                        let diagnostic = errors::mismatch(&a, &b, span);
                        checker.push_diagnostic(diagnostic);
                    }
                }
            }
        }
        Ty::Array(inner) => {
            if let Ty::Array(actual) = checker.out.types.get(got) {
                bind(checker, ast, src, inner, actual, bindings, span);
            } else if !checker.out.types.poisoned(got) {
                let (a, b) = (checker.show(ast, src, param), checker.show(ast, src, got));
                let diagnostic = errors::mismatch(&a, &b, span);
                checker.push_diagnostic(diagnostic);
            }
        }
        Ty::Fallible(inner) => {
            if let Ty::Fallible(actual) = checker.out.types.get(got) {
                bind(checker, ast, src, inner, actual, bindings, span);
            }
        }
        Ty::Map(key, value) => {
            if let Ty::Map(a, b) = checker.out.types.get(got) {
                bind(checker, ast, src, key, a, bindings, span);
                bind(checker, ast, src, value, b, bindings, span);
            }
        }
        Ty::Func { params, result } => {
            if let Ty::Func { params: actual, result: actual_result } =
                checker.out.types.get(got)
            {
                let wanted = checker.out.types.params_of(params);
                let given = checker.out.types.params_of(actual);
                if wanted.len() == given.len() {
                    for (w, g) in wanted.iter().zip(given.iter()) {
                        bind(checker, ast, src, *w, *g, bindings, span);
                    }
                    bind(checker, ast, src, result, actual_result, bindings, span);
                    return;
                }
            }
            if !checker.out.types.poisoned(got) {
                let (a, b) = (checker.show(ast, src, param), checker.show(ast, src, got));
                let diagnostic = errors::mismatch(&a, &b, span);
                checker.push_diagnostic(diagnostic);
            }
        }
        _ => {
            if param != got
                && !checker.out.types.poisoned(param)
                && !checker.out.types.poisoned(got)
            {
                let (a, b) = (checker.show(ast, src, param), checker.show(ast, src, got));
                let diagnostic = errors::mismatch(&a, &b, span);
                checker.push_diagnostic(diagnostic);
            }
        }
    }
}


/// Replace every `Generic(i)` with what the call bound it to.
pub(super) fn substitute(checker: &mut Checker, ty: TyId, bindings: &[Option<TyId>]) -> TyId {
    if bindings.is_empty() {
        return ty;
    }
    match checker.out.types.get(ty) {
        Ty::Generic(position) => match bindings.get(position as usize) {
            Some(Some(bound)) => *bound,
            _ => checker.error_ty(),
        },
        Ty::Array(inner) => {
            let inner = substitute(checker, inner, bindings);
            checker.out.types.intern(Ty::Array(inner))
        }
        Ty::Fallible(inner) => {
            let inner = substitute(checker, inner, bindings);
            checker.out.types.intern(Ty::Fallible(inner))
        }
        Ty::Map(key, value) => {
            let key = substitute(checker, key, bindings);
            let value = substitute(checker, value, bindings);
            checker.out.types.intern(Ty::Map(key, value))
        }
        Ty::Func { params, result } => {
            let params: Vec<TyId> = checker
                .out
                .types
                .params_of(params)
                .iter()
                .map(|p| substitute(checker, *p, bindings))
                .collect();
            let result = substitute(checker, result, bindings);
            checker.out.types.func(&params, result)
        }
        _ => ty,
    }
}
