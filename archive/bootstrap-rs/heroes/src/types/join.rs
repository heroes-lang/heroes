//! One rule for every branch join in the language.
//!
//! An `if` used as a value, a `match` used as a value, and a block whose value is
//! its last statement all ask the same question: *given these branches, what is
//! the type of the whole thing?* This file answers it once.
//!
//! **A branch that diverges contributes no type.** `return`, `break` and
//! `continue` produce no value (§4.7, panel 017 A1), so they arrive here as
//! `None` and are skipped. If every branch diverges the join produces nothing,
//! which is legal in statement position and an error where a value is wanted.
//!
//! Why one routine rather than a rule per construct: panel 017's historian
//! objected to A1 on exactly that axis — C++ pays for the same design with a
//! bespoke clause in `[expr.cond]`, and *because* it is per-construct, `throw`
//! is usable in value position nowhere else. Its stated withdrawal condition was
//! "A1 lands as a single shared join routine whose branch list carries
//! `diverges` as data, with no per-construct branch", which is what this is. The
//! compiler-engineer's condition — no `Never` variant in the type table, so the
//! descriptor pass never sees a type nothing inhabits — holds at the same time.

use crate::source::{Source, Span};
use crate::syntax::Ast;

use super::{errors, Checker, TyId};

/// One branch of a join: the type it produces, or `None` if it diverges, plus
/// where to point if it disagrees with the others.
pub(super) struct Branch {
    pub value: Option<TyId>,
    pub span: Span,
}

/// The type of a join, or `None` when every branch diverges.
///
/// The first branch that produces a value sets the type; every later one is
/// checked against it, which keeps the error where the *disagreement* is rather
/// than blaming the last branch for the first one's choice.
pub(super) fn join(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    branches: &[Branch],
) -> Option<TyId> {
    let mut result: Option<TyId> = None;
    for branch in branches {
        let Some(ty) = branch.value else { continue };
        match result {
            None => result = Some(ty),
            Some(expected) => {
                if ty != expected
                    && !checker.out.types.poisoned(ty)
                    && !checker.out.types.poisoned(expected)
                {
                    let (a, b) =
                        (checker.show(ast, src, expected), checker.show(ast, src, ty));
                    let diagnostic = errors::mismatch(&a, &b, branch.span);
                    checker.push_diagnostic(diagnostic);
                }
            }
        }
    }
    result
}

/// The join, where a value is required. `what` names the construct for the
/// message: an `if` and a `match` fail the same way and say so in their own
/// words.
pub(super) fn join_for_value(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    branches: &[Branch],
    what: &str,
    span: Span,
) -> TyId {
    match join(checker, ast, src, branches) {
        Some(ty) => ty,
        None => {
            let diagnostic = errors::no_value(what, span);
            checker.push_diagnostic(diagnostic);
            checker.error_ty()
        }
    }
}
