//! Reaching inside a value: a field, an index, and `?` (§4.3, §4.6, §4.9).
//!
//! Three rules, and each one is a place where the language's shape shows:
//!
//! - **a field** belongs to a record, to a *case* of a variant (§4.2: a case with
//!   fields is a small record), or to the error payload — and to nothing else, so
//!   a variant's own name has no fields and the message says where to look;
//! - **an index** is `str` → `int`, `[T]` → `T`, and `{K: V}` → **`V?`**, which is
//!   what makes a missing key a value rather than a crash (§4.9);
//! - **`?`** needs two things and each gets its own message: the value must be
//!   fallible, and the enclosing function must be able to fail (§4.6).

use crate::resolve::Resolved;
use crate::source::{Source, Span};
use crate::syntax::{Ast, DeclKind};

use super::table::Ty;
use super::{errors, Checker, TyId};


/// `p.x` — a field of a record, of a variant's case, or of the error payload.
pub(super) fn field_type(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    base: TyId,
    field: Span,
) -> TyId {
    let name = src.slice(field);
    if checker.out.types.poisoned(base) {
        return checker.error_ty();
    }
    match checker.out.types.get(base) {
        Ty::Named(decl) => match &ast.decls[decl as usize].kind {
            DeclKind::Record { fields } => {
                for candidate in fields {
                    if src.slice(candidate.name) == name {
                        return super::lower::ty(checker, ast, resolved, candidate.ty);
                    }
                }
                let known: Vec<String> =
                    fields.iter().map(|f| src.slice(f.name).to_string()).collect();
                let holder = src.slice(ast.decls[decl as usize].name).to_string();
                let diagnostic = errors::no_such_field(&holder, name, &known, field);
                checker.push_diagnostic(diagnostic);
                checker.error_ty()
            }
            // A variant's fields belong to its *cases*: `t.v` is not a thing,
            // `match t` then `.num n => n.v` is.
            _ => {
                let holder = checker.show(ast, src, base);
                let diagnostic = errors::field_of_variant(&holder, name, field);
                checker.push_diagnostic(diagnostic);
                checker.error_ty()
            }
        },
        Ty::Case(decl, case) => {
            if let DeclKind::Variant { cases } = &ast.decls[decl as usize].kind {
                let case = &cases[case as usize];
                for candidate in &case.fields {
                    if src.slice(candidate.name) == name {
                        return super::lower::ty(checker, ast, resolved, candidate.ty);
                    }
                }
                let known: Vec<String> =
                    case.fields.iter().map(|f| src.slice(f.name).to_string()).collect();
                let holder = checker.show(ast, src, base);
                let diagnostic = errors::no_such_field(&holder, name, &known, field);
                checker.push_diagnostic(diagnostic);
            }
            checker.error_ty()
        }
        // §4.6 states the error's two fields and nothing more.
        Ty::Failure => {
            if name == "code" || name == "msg" {
                return checker.out.types.str();
            }
            let known = vec!["code".to_string(), "msg".to_string()];
            let diagnostic = errors::no_such_field("the error", name, &known, field);
            checker.push_diagnostic(diagnostic);
            checker.error_ty()
        }
        _ => {
            let got = checker.show(ast, src, base);
            let diagnostic = errors::not_a_record(&got, name, field);
            checker.push_diagnostic(diagnostic);
            checker.error_ty()
        }
    }
}


pub(super) fn index_type(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    base: TyId,
    index: TyId,
    span: Span,
) -> TyId {
    if checker.out.types.poisoned(base) {
        return checker.error_ty();
    }
    let int = checker.out.types.int();
    match checker.out.types.get(base) {
        // `s[i]` is a byte (§4.3), which is why iterating characters is a
        // different call.
        Ty::Str => {
            expect_index(checker, ast, src, index, int, span);
            int
        }
        Ty::Array(element) => {
            expect_index(checker, ast, src, index, int, span);
            element
        }
        // §4.9: map access yields `V?`, which is what makes a missing key a
        // value rather than a crash.
        Ty::Map(key, value) => {
            expect_index(checker, ast, src, index, key, span);
            checker.out.types.intern(Ty::Fallible(value))
        }
        _ => {
            let got = checker.show(ast, src, base);
            let diagnostic = errors::not_indexable(&got, span);
            checker.push_diagnostic(diagnostic);
            checker.error_ty()
        }
    }
}


fn expect_index(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    got: TyId,
    want: TyId,
    span: Span,
) {
    if got != want && !checker.out.types.poisoned(got) {
        let (a, b) = (checker.show(ast, src, want), checker.show(ast, src, got));
        let diagnostic = errors::mismatch(&a, &b, span);
        checker.push_diagnostic(diagnostic);
    }
}


/// `e?` — hand the error to the caller (§4.6). Two conditions, and both are
/// worth their own message: the value must be fallible, and the function must be
/// able to fail.
pub(super) fn try_type(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    inner: TyId,
    span: Span,
    operand: Span,
) -> TyId {
    if checker.out.types.poisoned(inner) {
        return checker.error_ty();
    }
    let Ty::Fallible(value) = checker.out.types.get(inner) else {
        let got = checker.show(ast, src, inner);
        let diagnostic = errors::not_fallible(&got, span, operand);
        checker.push_diagnostic(diagnostic);
        return checker.error_ty();
    };
    if !checker.fallible {
        let result = checker.show(ast, src, checker.result);
        let diagnostic = errors::try_in_infallible(&result, span);
        checker.push_diagnostic(diagnostic);
    }
    value
}
