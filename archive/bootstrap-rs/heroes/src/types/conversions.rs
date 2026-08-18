//! What `to_<type>` means: the conversions, and the one built-in family whose
//! **result type is a function of the name** (spec § Types; design.md §4.3;
//! panels 042, 043, 044, 045 for the scheme, 060 for the second float width).
//!
//! Split out of `builtins.rs` on 2026-08-15, when `f32` pushed that file past
//! CLAUDE.md §11's ceiling. The seam is not the line count — it is that every
//! other built-in answers *what may I be applied to*, while these answer *what do
//! I turn into*, and the spec treats them as their own paragraph for the same
//! reason. `print`, `len` and `sort` read their argument; `to_u8` reads its own
//! name.
//!
//! Two rules govern the family and both were decided rather than inherited:
//!
//! - **the name says whether it can fail** — `to_i8` … `to_u64` are `T?` at every
//!   pair including the ones that cannot fail (author ratification 2026-08-13,
//!   restoring the uniform rule step 7 had bent), while `to_str`, `to_f32` and
//!   `to_f64` give a value;
//! - **nothing fails to fit a float**, which is why the second bullet has three
//!   members rather than two. A magnitude past a float's maximum is `inf` and one
//!   below its minimum is a subnormal or zero, and all three are values of the
//!   type — so `to_f32` rounds where `to_u8` refuses.

use crate::source::{Source, Span};
use crate::syntax::Ast;

use super::table::Ty;
use super::builtins::arg_error;
use super::{Checker, TyId};
use crate::types::FloatKind;

/// The conversion rules, or `None` if this name is not one — in which case
/// `builtins::call` goes on to the list.
///
/// The double `Option` is the module's own contract rather than an accident: the
/// outer one says *is this a conversion at all*, and the inner one is
/// `builtins::call`'s existing answer type, where `None` means the checker
/// already reported and has nothing to offer.
pub(super) fn call(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    name: &str,
    args: &[TyId],
    span: Span,
) -> Option<Option<TyId>> {
    Some(match (name, args) {
        // **`to_<width>(x) -> <width>?`, and `to_i64` is one of these.**
        //
        // The source is an integer of any width, or — for `to_i64` alone — an
        // `f64`, which is the conversion this arm absorbed when the family took
        // the `to_` scheme (author decision 2026-08-13). One name, one shape:
        // converting to an `i64` can fail whether the source is a float above
        // 2^63 or a `u64` above it, so a single fallible answer is the honest one
        // and the old aborting form is gone.
        (name, [one])
            if name.starts_with("to_")
                && crate::types::INT_KINDS.iter().any(|k| k.name() == &name[3..]) =>
        {
            let kind = *crate::types::INT_KINDS
                .iter()
                .find(|k| k.name() == &name[3..])
                .expect("just matched");
            // **Either float width, not just `f64`** (panel 060's llm-ergonomist,
            // condition C3). The spec's *"`to_i64` takes an `f64` too"* read as
            // exclusive, and `GetMousePosition().x` is an `f32` that the very next
            // line of any raylib program wants as an index — so refusing it would
            // leave the width with no route to an integer at all. Truncation
            // toward zero and the same fallible result: nothing about the rule
            // depends on which width the source was.
            let from_float =
                matches!(checker.out.types.get(*one), Ty::Float(_)) && kind.name() == "i64";
            if !from_float && !matches!(checker.out.types.get(*one), Ty::Int(_)) {
                let allowed =
                    if kind.name() == "i64" { "an integer or a float" } else { "an integer" };
                return Some(arg_error(checker, ast, src, name, allowed, *one, span));
            }
            let target = checker.out.types.intern(Ty::Int(kind));
            // **`T?` at every pair, including the ones that cannot fail** (author
            // ratification 2026-08-13, closing panel 043's Q1 and restoring the
            // author's original ruling, which step 7 had bent).
            //
            // The bent rule returned `T` where `IntKind::contains` said the
            // conversion was safe. It was *correct* — panel 043's
            // compiler-engineer generated all 64 pairs from an independent
            // reimplementation and found no fault — and still the wrong shape,
            // for three reasons that outrank the `.must()` it saved:
            //
            // - **one rule.** A reader knows the result's shape from the name,
            //   without resolving the argument's declaration to learn its width.
            //   That resolution is §1.3's locality test, measured by the same
            //   judge as "worse by exactly one write-side hop".
            // - **it was an invention.** Panel 043's historian searched Zig, Rust,
            //   Swift, D and Ada for one conversion name yielding a plain value
            //   for some argument types and an optional for others, and found
            //   none. Zig's `math.cast(comptime T: type, x) ?T` is the closest
            //   live analogue and returns `?T` unconditionally.
            // - **reversibility.** 8 call sites today against ≥25 at
            //   M-selfhost-probe on that judge's own measurement, so this was the
            //   cheap moment and every later one is dearer.
            //
            // The cost is real and not hidden: `to_i64(l.here() - '0')` in the
            // calculator's lexer now needs a `.must()` it can never exercise.
            // §1.4 calls that redundancy spent where errors cannot occur, and it
            // is the price of the three reasons above.
            Some(checker.out.types.intern(Ty::Fallible(target)))
        }
        // **The two float conversions, and both are infallible** (panel 060,
        // author instruction 2026-08-15).
        //
        // The spec's rule is that the *name* says whether a conversion can fail,
        // and `to_i8`…`to_u64` are fallible *"because the number may not fit"*.
        // Nothing fails to fit a float: a magnitude too large becomes `inf`, one
        // too small becomes a subnormal or zero, and all three are values of the
        // type. `to_f32(1e300)` is `inf` for the same reason `to_f64` of a large
        // `i64` rounds — the language already calls that infallible, and a second
        // answer here would be two rules for one question.
        //
        // Rounding is therefore **not** a failure, and that is the honest cost of
        // this pairing: `to_f32` loses precision silently, exactly as C's
        // `(float)` does. Making it fallible would put a `.must()` on every
        // boundary read of a `float` field, which is the noise panel 043 spent a
        // sitting removing from the other conversions.
        ("to_f64", [one])
            if matches!(checker.out.types.get(*one), Ty::Int(_) | Ty::Float(_)) =>
        {
            Some(checker.out.types.f64())
        }
        ("to_f32", [one])
            if matches!(checker.out.types.get(*one), Ty::Int(_) | Ty::Float(_)) =>
        {
            Some(checker.out.types.intern(Ty::Float(FloatKind::F32)))
        }
        ("to_f32", [one]) => {
            return Some(arg_error(checker, ast, src, "to_f32", "an integer or a float", *one, span))
        }
        // §4.20's inventory calls this one `.str()`; panel 017 renames it
        // `to_str`, so the three conversions share one scheme and a model can
        // derive the third from the two the spec already lists.
        // `cstr` is here and not in a conversion of its own: §4.19's boundary has
        // two directions, `.cstr()` out and this one back, and giving the return
        // path a new name would have cost spec tokens for a conversion the three
        // that exist already teach the shape of.
        ("to_str", [one]) => match checker.out.types.get(*one) {
            Ty::Int(_) | Ty::Float(_) | Ty::Bool | Ty::Str | Ty::Cstr => Some(checker.out.types.str()),
            _ => {
                return Some(arg_error(
                    checker, ast, src, "to_str", "an integer, a float, `bool`, `str` or `cstr`",
                    *one, span,
                ))
            }
        },
        _ => return None,
    })
}
