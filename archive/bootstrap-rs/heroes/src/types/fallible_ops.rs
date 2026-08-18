//! What can be done with a `T?`, and what happens when there is no `T?`
//! (design.md §4.6; spec § Failure; panel 043).
//!
//! Split out of `builtins.rs` by the §11 sweep. `must`, `default` and `is_err` are
//! the three built-ins whose whole rule is the same one — **the argument must be
//! fallible** — and the diagnostic for when it is not belongs with them, because it
//! is the answer all three give to the same mistake.

use crate::source::{Source, Span};
use crate::syntax::Ast;
use crate::types::{Ty, TyId};

use super::builtins::arg_error;
use super::{errors, Checker};

/// The three `T?` built-ins. `None` means a diagnostic was pushed instead.
pub(super) fn builtin(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    name: &str,
    args: &[TyId],
    span: Span,
) -> Option<TyId> {
    let value = match (name, args) {
        ("must", [one]) => match checker.out.types.get(*one) {
            Ty::Fallible(inner) => inner,
            _ => return unwrapping_nothing(checker, ast, src, "must", *one, span),
        },
        ("default", [one, fallback]) => match checker.out.types.get(*one) {
            Ty::Fallible(inner) if inner == *fallback => inner,
            Ty::Fallible(inner) => {
                let (want, got) =
                    (checker.show(ast, src, inner), checker.show(ast, src, *fallback));
                let diagnostic = errors::mismatch(&want, &got, span);
                checker.push_diagnostic(diagnostic);
                return Some(checker.error_ty());
            }
            _ => return unwrapping_nothing(checker, ast, src, "default", *one, span),
        },
        ("is_err", [one]) => match checker.out.types.get(*one) {
            Ty::Fallible(_) => checker.out.types.bool(),
            _ => return arg_error(checker, ast, src, "is_err", "a fallible value", *one, span),
        },
        _ => return None,
    };
    Some(value)
}

/// `.must()` or `.default(v)` on a value that cannot fail, with the repair
/// attached (panel 043's load-bearing condition).
///
/// **The `certain` fix is the point, and the reason it exists outlived the rule it
/// was written for.** It was added as panel 043's load-bearing condition, when a
/// widening returned a plain `T` and a reader carrying the uniform habit would
/// write a `.must()` with nothing to unwrap. The author's ratification of
/// 2026-08-13 restored the uniform `T?`, so that particular source of the mistake
/// is gone — and the fix is worth more now, not less: every `.must()` on a value
/// that cannot fail is still a §1.2 round-trip (read, understand, edit) left as a
/// bare diagnostic, and a machine-applied repair instead.
///
/// The replacement is computed **from the characters in hand** — the call's own
/// source text, minus a trailing `.must()` — never from a premise about what the
/// caller looks like (CLAUDE.md §11). Where the text does not end that way, UFCS
/// was not used and there is nothing certain to offer, so nothing is offered.
fn unwrapping_nothing(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    name: &str,
    got: TyId,
    span: Span,
) -> Option<TyId> {
    let shown = checker.show(ast, src, got);
    let mut diagnostic = errors::bad_operand(
        name,
        "a fallible value",
        &shown,
        span,
    )
    .with_note(format!(
        "`{shown}` cannot fail, so there is no error case to handle — this is the shape a `fit_<width>` that widens hands back, because a widening cannot fail"
    ));
    let text = src.slice(span);
    let suffix = format!(".{name}(");
    if let Some(at) = text.rfind(&suffix) {
        if text.ends_with(')') {
            diagnostic.fixes.push(crate::diagnostics::Fix {
                title: format!("remove the `.{name}(…)`"),
                replacement: text[..at].to_string(),
                span,
                certainty: crate::diagnostics::Certainty::Certain,
            });
        }
    }
    checker.push_diagnostic(diagnostic);
    Some(checker.error_ty())
}
