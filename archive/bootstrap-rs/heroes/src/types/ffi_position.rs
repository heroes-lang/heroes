//! Where a boundary type may **stand**, which is a different question from
//! whether C can spell it (design.md §4.19; panel 062's follow-up, measured
//! 2026-08-16).
//!
//! `ffi_decls.rs` asks *can a C header declare this type at all* — one answer per
//! type, shared by results and parameters, because for six of the seven it is the
//! same answer. This file asks the question that comes after: **may it stand
//! here**. Split out rather than appended because a reader of `crosses_the
//! _boundary` is asking about a vocabulary and a reader of this is asking about a
//! position, and the shared answer is exactly what hid the defect below.
//!
//! **The defect.** `extern function abs(n: ()) -> i32` passed `heroes check` at
//! exit 0, because `()` is a legal *result* — it is how a `void` function is
//! declared — and one predicate served both positions. The emitter then wrote
//! `abs((void *)0)` into the result assertion and clang refused it: *incompatible
//! pointer to integer conversion*, at **exit 2**, which under CLAUDE.md §7 says
//! the compiler is wrong about a declaration the author wrote. Every other way to
//! get an `extern` signature wrong is exit 1 on the `.hero` line.
//!
//! **The test for whether a refusal belongs in the frontend at all**, and it is
//! what keeps this file one function long: is the fact about C's *grammar*, or
//! about some *header*? A parameter list has no spelling for an argument of type
//! `void`, so no header anywhere can take one and there is nothing to read a
//! header for. A `ptr` or a `cstr` parameter, by contrast, is ordinary C that may
//! or may not match the header in front of it — only clang can answer that, and
//! `emit/ffi_narrowed.rs` is where its answer is turned back into a diagnostic on
//! the author's line.

use crate::source::{Source, Span};
use crate::syntax::Ast;

use super::table::Ty;
use super::{errors, Checker, TyId};

/// Refuse every parameter whose declared type cannot stand in a C argument list.
///
/// Runs **after** `ffi_decls::ffi_signature`'s vocabulary check, so a type that
/// can cross at neither position produces one message rather than two.
pub(super) fn arguments(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    function: &crate::syntax::Function,
    declared_name: Span,
) {
    let name_of_function = src.slice(declared_name);
    for param in &function.params {
        let Some(declared) = checker.out.written_type(param.ty) else { continue };
        let Some(why) = argument_refusal(checker, declared) else { continue };
        let shown = checker.show(ast, src, declared);
        let diagnostic = errors::ffi_parameter_position(
            src.slice(param.name),
            name_of_function,
            &shown,
            why,
            ast.types[param.ty.0 as usize].span,
        );
        checker.push_diagnostic(diagnostic);
    }
}

/// Why this type cannot be an `extern`'s **argument**, though it may be its
/// result.
///
/// **Exactly one type, and the list was two for about a minute** — which is kept
/// here because the second entry was refuted by this repository on its first run,
/// and the shape of that mistake is worth more than the minute it cost.
///
/// `str` was the second. The argument for it read well — *a `str` is a `HeroStr`
/// built by this runtime, so no C function takes one* — and it is very nearly the
/// argument `ffi_decls::ffi_constant` makes one position over. It is also
/// **false**: `runtime/hero_os.h:53` declares `int64_t hero_file_write(const char
/// *path, HeroStr text)`, and `library/source.hero:112` binds it as `text: str`.
/// The refusal reported the compiler's own library as broken before any test did.
///
/// That is CLAUDE.md §11 arriving in real time. The premise was about **the
/// world** — which C headers exist — rather than about the value in hand, and a C
/// header written *for* this language is precisely the case such a premise cannot
/// see. `()` survives because its premise is about C's grammar instead, and a
/// grammar does not acquire new headers.
fn argument_refusal(checker: &Checker, ty: TyId) -> Option<&'static str> {
    match checker.out.types.get(ty) {
        Ty::Unit => Some(
            "`()` names no value, so there is no argument to pass: as a *result* it is C's `void`, which a parameter list has no spelling for. Drop the parameter, or declare the type the header uses",
        ),
        _ => None,
    }
}
