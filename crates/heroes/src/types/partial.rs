//! What a **partial** `record` may not do, and how far the refusal reaches
//! (spec § FFI; panel 061, ratified 2026-08-15).
//!
//! A group's `record` marked `partial` names some of the header's struct. The
//! layout is still C's and still exact — the emitter declares nothing — so every
//! operation that **copies** the value is safe at any size: `desc_copy` is
//! `*(Font *)dst = *(const Font *)src`, `@`'s copy-in and copy-out are whole-struct
//! assignments, an array element moves `elem->size` bytes. All of that was compiled
//! and counted at panel 061: **124 of 124 unnamed bytes survive** a full round trip.
//!
//! What is refused is the three operations that **read the fields**:
//!
//! - `==` — §4.3 makes it structural, and a generated `_eq` walks the field list.
//!   Two values equal on every named field are not equal, and the compiler cannot
//!   know it.
//! - `hash` — the same walk, and it must agree with `==` or a map breaks.
//! - a **map key**, which is `hash` reached from a place the author wrote no
//!   operator at.
//!
//! **Construction is permitted, and that is a decision rather than an omission.**
//! The ffi-pragmatist threatened a veto over it and the ground held: refusing it
//! deletes the only expression in the language that produces an `SDL_Event`, so
//! `SDL_PollEvent(@e)` — the binding this feature was adopted for — becomes
//! unwritable and needs a hand-written shim forever. And it does not buy safety:
//! the zero-fill is *deterministic*, not undefined, and the identical SEGV is
//! reachable from a **complete** binding whose `ptr` field is `nullptr`, measured
//! at exit 134. What protects the author instead is `extern_record.rs`'s
//! completeness probe, which makes the **unmarked** record — the one nobody was
//! defending — a compile error.
//!
//! **The reach differs between the two halves, and getting it uniform costs the
//! milestone its own use case.** Comparison is **transitive**: `[Font] == [Font]`
//! reaches `Font_eq` through `hero_array_eq`, `{i64: Font}` through the map's
//! descriptor, `Outer == Outer` through the generated `Outer_eq`, and a variant
//! case's payload through `variant_equality_body` — four routes, all measured
//! firing. Construction is **per-declaration**: `Outer(f: a)` copies a whole `Font`
//! that came from C, which is safe, and a transitive rule there would make
//! raylib's own `RenderTexture { Texture texture; Texture depth; }` unbuildable.

use crate::syntax::{Ast, DeclKind};

use super::table::Ty;
use super::{Checker, TyId};

/// Is this declaration a `record` its group marked `partial`?
pub(super) fn is_partial(ast: &Ast, decl: u32) -> bool {
    matches!(ast.decls[decl as usize].kind, DeclKind::Record { partial: true, .. })
}

/// The partial `record` this type **reaches**, if any — itself, or one inside it.
///
/// Descends `[T]`, `{K: V}`, `T?`, a record's fields and a **variant case's**
/// fields, because each of those is a route by which the generated `_eq` or `_hash`
/// of the inner type is called. Panel 061 measured all four; the variant case is the
/// one a list of container kinds would have missed, because a case payload is not a
/// container in the surface language.
///
/// Returns the **declaration reached** rather than a boolean, so the diagnostic can
/// name it: §4.17 asks the message to carry the repair, and *"`[Font]` names only
/// some of its C struct's members"* is false — `[Font]` is an array and has no C
/// struct.
pub(super) fn reaches(checker: &Checker, ast: &Ast, ty: TyId) -> Option<u32> {
    reaches_within(checker, ast, ty, 0)
}

/// The recursion, with a depth bound.
///
/// **The bound is not a guess about nesting**, it is what makes the walk total: a
/// record may not contain itself by value (§4.3's sized rule refuses it), but a
/// *generic* type in the arena can still be cyclic through a type parameter, and
/// this walk runs before monomorphisation has removed those. Sixteen is past
/// anything a struct a C header declares can reach, and the fallback is `None`,
/// which is the safe direction here: a missed refusal is a wrong answer, and a
/// depth-16 nest of extern records does not exist.
fn reaches_within(checker: &Checker, ast: &Ast, ty: TyId, depth: u32) -> Option<u32> {
    if depth > 16 {
        return None;
    }
    match checker.out.types.get(ty) {
        Ty::Named(decl) => {
            if is_partial(ast, decl) {
                return Some(decl);
            }
            let fields = match &ast.decls[decl as usize].kind {
                DeclKind::Record { fields, .. } => fields,
                DeclKind::Variant { cases } => {
                    return cases.iter().flat_map(|case| &case.fields).find_map(|field| {
                        let ty = checker.out.written_type(field.ty)?;
                        reaches_within(checker, ast, ty, depth + 1)
                    })
                }
                _ => return None,
            };
            fields.iter().find_map(|field| {
                let ty = checker.out.written_type(field.ty)?;
                reaches_within(checker, ast, ty, depth + 1)
            })
        }
        Ty::Array(element) => reaches_within(checker, ast, element, depth + 1),
        Ty::Fallible(inner) => reaches_within(checker, ast, inner, depth + 1),
        Ty::Map(key, value) => reaches_within(checker, ast, key, depth + 1)
            .or_else(|| reaches_within(checker, ast, value, depth + 1)),
        _ => None,
    }
}

/// Every `{K: V}` the program **wrote**, checked for a partial key.
///
/// **A pass of its own, and the reason is that a map key is the one refusal with no
/// operator behind it.** `==` fires on a token the author typed; `m[k] @ v` fires
/// on nothing — the hazard is in the *type* `{Color: i64}`, which may be written
/// once in an annotation and never mentioned again. So the check cannot live where
/// the others do.
///
/// It is not in `lower::ty` either, where `Ty::Map` is interned, and that is a
/// measurement rather than a preference: that function has no `&Source`, so naming
/// the offending type would put a fifth parameter on the call every declaration in
/// the program passes through. Walking the written type nodes afterwards costs one
/// pass over a vector and threads nothing.
pub(super) fn map_keys(checker: &mut Checker, ast: &Ast, src: &crate::source::Source) {
    let mut found: Vec<(u32, TyId, crate::source::Span)> = Vec::new();
    // The written annotations — `m: {Color: i64} @ {}`.
    for node in &ast.types {
        let crate::syntax::TypeKind::Map(key, _) = &node.kind else { continue };
        let Some(ty) = checker.out.written_type(*key) else { continue };
        if let Some(decl) = reaches(checker, ast, ty) {
            found.push((decl, ty, ast.types[key.0 as usize].span));
        }
    }
    // **And every map LITERAL, because a `{K: V}` need not be written down.**
    // `m = {c: 1}` interns `Ty::Map` straight from the entries' types
    // (`types/exprs.rs`), so no `TypeKind::Map` node exists and the walk above
    // cannot see it. This function's first version had exactly that hole: its own
    // doc said *"the hazard is in the type"* and it then implemented a rule about
    // the **syntax** — the defect class this whole milestone kept finding, written
    // by the seat auditing for it, twenty minutes after writing the audit's brief.
    //
    // It was caught by `structural.rs`'s `hero_panic` body rather than by a test:
    // `heroes check` passed at exit 0 and the program aborted **by name**. That is
    // the compiler-engineer's condition 3 doing precisely what it was asked to do —
    // a hole in this walk costs a named abort instead of two structs colliding as
    // one map key at exit 0 — and it is why the loud fallback is not belt-and-braces
    // (CLAUDE.md §11).
    for (index, expr) in ast.exprs.iter().enumerate() {
        let crate::syntax::ExprKind::Map(entries) = &expr.kind else { continue };
        let Some(first) = entries.first() else { continue };
        let ty = checker.out.expr_types[index];
        let Ty::Map(key, _) = checker.out.types.get(ty) else { continue };
        if let Some(decl) = reaches(checker, ast, key) {
            found.push((decl, key, ast.exprs[first.key.0 as usize].span));
        }
    }
    for (decl, ty, span) in found {
        let shown = checker.show(ast, src, ty);
        let name = src.slice(ast.decls[decl as usize].name).to_string();
        let diagnostic = super::errors::partial_operation(&name, &shown, "a map key", span);
        checker.push_diagnostic(diagnostic);
    }
}
