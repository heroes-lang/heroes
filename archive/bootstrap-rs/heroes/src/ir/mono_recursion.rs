//! Why monomorphisation terminates (design.md §4.12; panel 029).
//!
//! Split out of `mono.rs` by the §11 sweep, and it is the half of that pass with a
//! literature behind it. **Termination is not an accident and not a depth limit.**
//! Polymorphic recursion — `f<T>` calling `f<[T]>` — instantiates forever,
//! type-checks clean today, and design.md does not mention it. MLton's
//! whole-program monomorphiser has been total for twenty-five years *because* SML
//! bans it; inference for it is undecidable (Henglein 1993; Kfoury–Tiuryn–Urzyczyn
//! 1993). Rust is the warning: it accepts at type-check and blows up at codegen
//! with `reached the recursion limit while instantiating`, no error code, late and
//! unattributable.
//!
//! So the refusal is **structural** — instantiating `f` at `S` from within `f` at
//! `T` where `S` properly contains `T` is unbounded — and it is a **program**
//! diagnostic at exit 1, because the program is legal under §4.12 as written and
//! "the compiler could not run" would be a lie about whose mistake it is.

use crate::diagnostics::Diagnostic;
use crate::source::{Source, Span};
use crate::syntax::Ast;
use crate::types::{render_ty, Checked, Ty, TyId};

use super::mono::Instance;

/// Is this instance a *growing* repeat of one on its own parent chain?
///
/// The test is containment, not equality: `f<[T]>` reached from `f<T>` is
/// unbounded, while `f<int>` reached from `f<int>` is the ordinary recursion that
/// every compiler has and that terminates because the instance already exists.
pub(super) fn recursive(
    queue: &[(Instance, Option<usize>, Span)],
    at: usize,
    decl: u32,
    args: &[TyId],
    checked: &Checked,
    ast: &Ast,
    src: &Source,
    span: Span,
) -> Option<Diagnostic> {
    let mut walk = queue[at].1;
    while let Some(index) = walk {
        let ((ancestor_decl, ancestor_args), parent, _) = &queue[index];
        if *ancestor_decl == decl
            && ancestor_args.len() == args.len()
            && ancestor_args
                .iter()
                .zip(args)
                .all(|(small, big)| contains(checked, *big, *small))
            && ancestor_args.iter().zip(args).any(|(small, big)| small != big)
        {
            let show = |list: &[TyId]| {
                list.iter()
                    .map(|t| render_ty(&checked.types, ast, src, *t, &[]))
                    .collect::<Vec<String>>()
                    .join(", ")
            };
            return Some(polymorphic_recursion(&show(ancestor_args), &show(args), span));
        }
        walk = *parent;
    }
    None
}

/// §4.12 has no shape for this, so the message carries the whole explanation: what
/// was instantiated, what it reached, and the one edit that ends it.
fn polymorphic_recursion(from: &str, to: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "polymorphic_recursion",
        format!(
            "this call instantiates the function that contains it at a LARGER type — \
             `<{from}>` reaches `<{to}>`, which reaches a larger one again, without end"
        ),
        span,
    )
    .with_note(
        "a generic function is compiled once per type it is used at, so a chain that \
         never repeats a type never ends"
            .to_string(),
    )
    .with_note(
        "pass the value along unchanged, or take the recursive step in a \
         non-generic helper"
            .to_string(),
    )
}

/// Is `small` `big`, or a part of it? `[i64]` contains `i64`; `i64` does not
/// contain `[i64]`.
fn contains(checked: &Checked, big: TyId, small: TyId) -> bool {
    if big == small {
        return true;
    }
    match checked.types.get(big) {
        Ty::Array(element) => contains(checked, element, small),
        Ty::Fallible(inner) => contains(checked, inner, small),
        Ty::Map(key, value) => contains(checked, key, small) || contains(checked, value, small),
        Ty::Func { params, result } => {
            checked.types.params_of(params).iter().any(|p| contains(checked, *p, small))
                || contains(checked, result, small)
        }
        _ => false,
    }
}
