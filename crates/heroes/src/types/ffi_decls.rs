//! What may cross the FFI boundary, asked of a declaration (design.md §4.19;
//! panels 036, 038, 060).
//!
//! Split out of `decls.rs` on 2026-08-15, and the seam is a different question
//! rather than a line count. `decls.rs` asks *what does this declaration\'s body
//! owe* — a constant\'s value, a function\'s `return`, a test\'s nothing. These
//! four functions ask *can C spell this at all*, which is a question about the
//! **type vocabulary** and is answered without looking at a body, because an
//! `extern` has none.
//!
//! **All four refuse in the loud direction** (CLAUDE.md §11). A container or a
//! record with no header behind it has no C counterpart, so leaving it to clang
//! costs an internal error naming generated C at exit 2 — the compiler blaming
//! itself for a mistake in a `.hero` file, which is the failure §4.19\'s own
//! guarantee exists to prevent.

use crate::source::Source;
use crate::syntax::Ast;

use super::table::Ty;
use super::{errors, Checker, TyId};

/// Every type in an `extern`'s signature must be one a C header can declare
/// (§4.19). The list is `ctype.rs`'s scalars plus §4.19's two opaque types, plus
/// `()` for a function that returns nothing — and `str`, which is a `HeroStr` by
/// value and reaches C only from a function that builds one (§4.20).
///
/// **The refusal is the loud direction** (CLAUDE.md §11). A container or a record
/// in an `extern` has no header counterpart at all, so leaving it to clang costs
/// an internal error naming generated C; refusing it costs one message naming the
/// parameter.
pub(super) fn ffi_signature(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    function: &crate::syntax::Function,
    result: TyId,
) {
    // Collected first, then checked: a closure that borrows the checker mutably
    // cannot also read it (and CLAUDE.md §5 keeps stored closures out anyway).
    let mut wanted: Vec<(TyId, crate::source::Span, &str)> =
        vec![(result, ast.types[function.result.0 as usize].span, "an `extern`'s result")];
    for param in &function.params {
        if let Some(declared) = checker.out.written_type(param.ty) {
            wanted.push((declared, ast.types[param.ty.0 as usize].span, "an `extern`'s parameter"));
        }
    }
    for (ty, span, what) in wanted {
        if crosses_the_boundary(checker, ast, ty) {
            continue;
        }
        let name = checker.show(ast, src, ty);
        let diagnostic = errors::ffi_type(&name, what, span);
        checker.push_diagnostic(diagnostic);
    }
}

/// The type of an `extern constant`, which is a narrower question than an
/// `extern`'s signature asks (§4.19, panel 038).
///
/// Two of the seven boundary types cannot be a *value* a header holds, and both
/// refusals are facts about **Heroes** rather than about C headers — which is why
/// they belong here and not in an assertion clang evaluates. A `str` is a
/// `HeroStr`, built by this runtime and carrying its magic word; a `()` names no
/// value at all.
///
/// `bool` is deliberately **not** refused here. Under `-std=c11` no header
/// constant has type `_Bool` — `stdbool.h` spells `true` as `#define true 1` —
/// but that is a premise about the world, and a premise expires silently
/// (CLAUDE.md §11). The per-constant type assertion asks the token in hand
/// instead, and refuses `constant true: bool` loudly with what the header really
/// says.
pub(super) fn ffi_constant(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    declared: TyId,
    span: crate::source::Span,
) {
    let what = "an `extern constant`";
    if !crosses_the_boundary(checker, ast, declared) {
        let name = checker.show(ast, src, declared);
        let diagnostic = errors::ffi_type(&name, what, span);
        checker.push_diagnostic(diagnostic);
        return;
    }
    let refusal = match checker.out.types.get(declared) {
        Ty::Str => Some(
            "a `str` is built by this runtime, so no C header holds one: declare it `cstr` and convert with `to_str`, which copies (§4.20)",
        ),
        Ty::Unit => Some("a `constant` names a value, and `()` is a type rather than a value"),
        _ => None,
    };
    if let Some(why) = refusal {
        let name = checker.show(ast, src, declared);
        let diagnostic = errors::ffi_constant_type(&name, why, span);
        checker.push_diagnostic(diagnostic);
    }
}

/// What a C header can spell — the scalars, §4.19's two opaque types, `()`, and,
/// since panel 060, **a `record` the header itself declares**.
///
/// The last one is the whole of `M-struct-passing` and it is one question: was
/// this record declared inside an `extern` group? A record declared in Heroes has
/// a layout this compiler chose — field order, padding, and on arm64 the register
/// class each field travels in — and none of that is knowable from a header. A
/// record declared in a group has the *header's* layout, because the emitter
/// writes no typedef for it and uses the header's own name.
///
/// **The narrowing asks the declaration, never where the type was mentioned**
/// (CLAUDE.md §11). *"A record used in an `extern` signature is a C struct"* is a
/// premise about the world and expires the day somebody passes a Heroes record to
/// one; *"this record carries a header"* is a fact about the value in hand.
fn crosses_the_boundary(checker: &Checker, ast: &Ast, ty: TyId) -> bool {
    if let Ty::Named(decl) = checker.out.types.get(ty) {
        return matches!(
            ast.decls[decl as usize].kind,
            crate::syntax::DeclKind::Record { header: Some(_), .. }
        );
    }
    matches!(
        checker.out.types.get(ty),
        Ty::Int(_) | Ty::Float(_) | Ty::Bool | Ty::Str | Ty::Ptr | Ty::Cstr | Ty::Unit | Ty::Error
    )
}

/// What a group's `record` may hold: the scalars, §4.19's two opaque types, and
/// another record of a group (§4.19, panel 060).
///
/// **Deliberately narrower than `crosses_the_boundary`**, and the two omissions
/// are the whole difference between a signature and a struct. A `str` is a
/// `HeroStr` — a fat pointer this runtime builds, with a refcount in a header
/// before the bytes — so it crosses as an *argument* to a function that knows what
/// it is, and no C header ever declares one as a member. And `()` is a type rather
/// than a value: it is a legal result and cannot be a field, which is the same
/// refusal `unit_field` already makes for an ordinary record.
pub(super) fn ffi_field(checker: &mut Checker, ast: &Ast, src: &Source, ty: TyId, span: crate::source::Span) {
    let ok = match checker.out.types.get(ty) {
        Ty::Int(_) | Ty::Float(_) | Ty::Bool | Ty::Ptr | Ty::Cstr | Ty::Error => true,
        // A nested record is fine exactly when it is also the header's —
        // `RenderTexture` holds two `Texture`s by value, and both are raylib's.
        Ty::Named(decl) => matches!(
            ast.decls[decl as usize].kind,
            crate::syntax::DeclKind::Record { header: Some(_), .. }
        ),
        _ => false,
    };
    if ok {
        return;
    }
    // **The note is chosen by the type, because the repair is** (§4.17: a
    // diagnostic carries what is needed to fix the program without opening
    // another file). A note about `str`'s reference count printed under an
    // `[i64]` field sends the reader to the wrong repair with full confidence,
    // which is worse than printing no note at all.
    let why = match checker.out.types.get(ty) {
        Ty::Str => "a `str` is this runtime's own value, with a reference count before its bytes, so no C struct holds one: declare the field `cstr` and convert with `to_str`, which copies",
        Ty::Array(_) | Ty::Map(_, _) => "a C struct holds a pointer to its elements, never the elements: declare the field `ptr` and reach them through the header's own accessors",
        Ty::Fallible(_) => "`T?` is this language's own two-word value and no header declares one: a C function reports failure in its result or an out-parameter (§4.19)",
        Ty::Unit => "`()` is a type rather than a value, so nothing can hold one — drop the field",
        Ty::Named(_) => "a record can hold another only if the header declares that one too: move it into this `extern` group, or declare the field `ptr` if C holds a pointer to it",
        _ => "no C header can declare a field of this type",
    };
    let name = checker.show(ast, src, ty);
    let diagnostic = errors::ffi_field_type(&name, why, span);
    checker.push_diagnostic(diagnostic);
}
