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
