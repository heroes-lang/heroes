//! Patterns, and the rule `match` exists for (§4.7).
//!
//! **Exhaustive or a compile error**, and `_` is forbidden on a variant. Those
//! two rules hold each other up: without the ban, adding a case to a variant
//! would stop breaking compilation, and exhaustiveness would be theatre. With it,
//! every `match` over a variant is a list of every case, checked.
//!
//! Three subjects can be matched, and they differ in whether exhaustiveness is
//! *possible*:
//!
//! | subject | patterns | `_` |
//! |---------|----------|-----|
//! | a `variant` | `.case [binding]` | forbidden — the cases are countable |
//! | a `T?` | `.ok x`, `.err e` | forbidden — it is a two-case variant |
//! | `int`, `str` | literals | **required**: the values are not countable |

use crate::resolve::Resolved;
use crate::source::{Source, Span};
use crate::syntax::{Ast, DeclKind, ExprId};

use super::table::Ty;
use super::{errors, exprs, Checker, TyId};

/// `.case name` — records the case as covered and hands back what the payload
/// binding is worth.
pub(super) fn case_pattern(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    subject: TyId,
    name: Span,
    span: Span,
    covered: &mut Vec<u32>,
) -> TyId {
    let case = src.slice(name);
    if checker.out.types.poisoned(subject) {
        return checker.error_ty();
    }
    match checker.out.types.get(subject) {
        Ty::Named(decl) => {
            let DeclKind::Variant { cases } = &ast.decls[decl as usize].kind else {
                let got = checker.show(ast, src, subject);
                let diagnostic = errors::not_matchable(&got, span);
                checker.push_diagnostic(diagnostic);
                return checker.error_ty();
            };
            match cases.iter().position(|c| src.slice(c.name) == case) {
                Some(index) => {
                    if covered.contains(&(index as u32)) {
                        let diagnostic = errors::duplicate_arm(case, span);
                        checker.push_diagnostic(diagnostic);
                    }
                    covered.push(index as u32);
                    checker.out.types.intern(Ty::Case(decl, index as u32))
                }
                None => {
                    let known: Vec<String> =
                        cases.iter().map(|c| src.slice(c.name).to_string()).collect();
                    let holder = src.slice(ast.decls[decl as usize].name).to_string();
                    let diagnostic = errors::no_such_case(&holder, case, &known, name);
                    checker.push_diagnostic(diagnostic);
                    checker.error_ty()
                }
            }
        }
        // §4.6's two cases, and they are the only ones.
        Ty::Fallible(inner) => match case {
            "ok" => {
                covered.push(0);
                inner
            }
            "err" => {
                covered.push(1);
                checker.out.types.failure()
            }
            _ => {
                let known = vec!["ok".to_string(), "err".to_string()];
                let diagnostic = errors::no_such_case("a fallible value", case, &known, name);
                checker.push_diagnostic(diagnostic);
                checker.error_ty()
            }
        },
        _ => {
            let got = checker.show(ast, src, subject);
            let diagnostic = errors::case_on_non_variant(&got, case, span);
            checker.push_diagnostic(diagnostic);
            checker.error_ty()
        }
    }
}

/// `_` as a whole arm: legal only where exhaustiveness is impossible (§4.7).
pub(super) fn wildcard(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    subject: TyId,
    span: Span,
) {
    if checker.out.types.poisoned(subject) {
        return;
    }
    let countable = match checker.out.types.get(subject) {
        Ty::Named(decl) => matches!(ast.decls[decl as usize].kind, DeclKind::Variant { .. }),
        Ty::Fallible(_) | Ty::Bool => true,
        _ => false,
    };
    if countable {
        let got = checker.show(ast, src, subject);
        let diagnostic = errors::wildcard_on_variant(&got, span);
        checker.push_diagnostic(diagnostic);
    }
}

/// A literal arm: `0 => …`, `"plus" => …`. Its type must be the subject's.
pub(super) fn literal(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    subject: TyId,
    value: ExprId,
    span: Span,
) {
    let got = exprs::synth(checker, ast, resolved, src, value);
    if got != subject
        && !checker.out.types.poisoned(got)
        && !checker.out.types.poisoned(subject)
    {
        let (a, b) = (checker.show(ast, src, subject), checker.show(ast, src, got));
        let diagnostic = errors::mismatch(&a, &b, span);
        checker.push_diagnostic(diagnostic);
    }
}

/// The rule the language is built around: every case named, or the compiler names
/// the ones that are missing (§4.17 — "non-exhaustive `match` lists the missing
/// cases by name").
pub(super) fn exhaustive(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    subject: TyId,
    covered: &[u32],
    wildcard: bool,
    span: Span,
) {
    if wildcard || checker.out.types.poisoned(subject) {
        return;
    }
    match checker.out.types.get(subject) {
        Ty::Named(decl) => {
            if let DeclKind::Variant { cases } = &ast.decls[decl as usize].kind {
                let missing: Vec<String> = cases
                    .iter()
                    .enumerate()
                    .filter(|(index, _)| !covered.contains(&(*index as u32)))
                    .map(|(_, case)| format!(".{}", src.slice(case.name)))
                    .collect();
                if !missing.is_empty() {
                    let diagnostic = errors::non_exhaustive(&missing, span);
                    checker.push_diagnostic(diagnostic);
                }
            }
        }
        Ty::Fallible(_) => {
            let mut missing: Vec<String> = Vec::new();
            if !covered.contains(&0) {
                missing.push(".ok".to_string());
            }
            if !covered.contains(&1) {
                missing.push(".err".to_string());
            }
            if !missing.is_empty() {
                let diagnostic = errors::non_exhaustive(&missing, span);
                checker.push_diagnostic(diagnostic);
            }
        }
        // `int` and `str` cannot be enumerated, so `_` is how they are finished —
        // and its absence is the error.
        Ty::Int | Ty::Str => {
            let diagnostic = errors::needs_wildcard(span);
            checker.push_diagnostic(diagnostic);
        }
        _ => {}
    }
}
