//! What may cross the FFI boundary, asked of a declaration (design.md §4.19;
//! panels 036, 038, 060).
//!
//! Split out of `decls.rs` on 2026-08-15, and the seam is a different question
//! rather than a line count. `decls.rs` asks *what does this declaration\'s body
//! owe*; these functions ask *can C spell this at all*, which is about the **type
//! vocabulary** and needs no body, because an `extern` has none.
//!
//! **All four refuse in the loud direction** (CLAUDE.md §11). A container or a
//! record with no header behind it has no C counterpart, so leaving it to clang
//! costs an internal error naming generated C at exit 2 — the compiler blaming
//! itself for a mistake in a `.hero` file, which is the failure §4.19\'s own
//! guarantee exists to prevent.

use crate::source::Source;
use crate::syntax::Ast;

use super::table::Ty;
use super::{errors, ffi_position, Checker, TyId};

/// Every type in an `extern`'s signature must be one a C header can declare
/// (§4.19): `ctype.rs`'s scalars, §4.19's two opaque types, `()` for a function
/// that returns nothing, and `str` — a `HeroStr` by value, which `hero_os.h`
/// really does take. **Where each may STAND is `ffi_position.rs`'s question**,
/// asked after this one so a type refused at both gets one message.
///
/// **The refusal is the loud direction** (CLAUDE.md §11): a container or a record
/// with no header behind it costs an internal error naming generated C if left to
/// clang, and one message naming the parameter if refused here.
pub(super) fn ffi_signature(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    function: &crate::syntax::Function,
    result: TyId,
    declared_name: crate::source::Span,
) {
    // Collected first, then checked: a closure borrowing the checker mutably
    // cannot also read it (CLAUDE.md §5 keeps stored closures out anyway).
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
    ffi_position::arguments(checker, ast, src, function, declared_name);
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
        // **`i32[4]` — a C array member, and its element is the same question
        // again** (panel 062, author instruction 2026-08-15: *"è fondamentale
        // chiamare il C"*). It is the last field form raylib needs: `VrStereoConfig`
        // is eight array members and nothing else, and five more structs bind only
        // through `partial` today — which costs them `==` and `hash` for a reason
        // that has nothing to do with equality.
        //
        // The element is asked recursively rather than restricted to scalars,
        // because `Matrix projection[2]` is an array **of a group record** and is
        // exactly what `VrStereoConfig` opens with.
        Ty::Fixed(inner, _) => {
            let ok = matches!(
                checker.out.types.get(inner),
                Ty::Int(_) | Ty::Float(_) | Ty::Bool | Ty::Ptr | Ty::Cstr | Ty::Error
            );
            ok || matches!(checker.out.types.get(inner), Ty::Named(decl)
                if matches!(
                    ast.decls[decl as usize].kind,
                    crate::syntax::DeclKind::Record { header: Some(_), .. }
                ))
        }
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
        // Was "no C header can declare a field of this type" — §11's false claim about the world; `tiffio.h` has `float d_mat[3][3]` (panel 081).
        _ => "this group has no spelling for that: a field is a number, `bool`, `ptr`, `cstr`, another record of the group, or a fixed array of one",
    };
    let name = checker.show(ast, src, ty);
    let diagnostic = errors::ffi_field_type(&name, why, span);
    checker.push_diagnostic(diagnostic);
}

/// A fixed array where no header owns the layout (panel 062).
pub(super) fn fixed_outside_a_group(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    ty: TyId,
    span: crate::source::Span,
) {
    if !matches!(checker.out.types.get(ty), Ty::Fixed(_, _)) {
        return;
    }
    let name = checker.show(ast, src, ty);
    let diagnostic = errors::fixed_outside_a_group(&name, span);
    checker.push_diagnostic(diagnostic);
}

/// The same rule as above, asked of **every written type in the file** rather
/// than of the one position panel 062 happened to be looking at.
///
/// **The rule is that a fixed array may appear in exactly one place** — as the
/// declared type of a field of a `record` inside an `extern` group — because that
/// is the only place a C compiler chose the layout (§4.19). Panel 062 stated it
/// and wired it to a single call site, `decls.rs`'s ordinary-record field walk, so
/// three other positions reached the backend unchecked and **all three shipped**:
///
/// ```text
/// xs: [i64[4]] @ []      heroes check exit 0, a binary, then abort 134
/// m: {str: i64[2]} @ {}  heroes check exit 0, a binary, then abort 134
/// x: i64[4] @ [1,2,3,4]  exit 2, "internal error: compiling the generated C failed"
/// ```
///
/// The first two are the worse pair: `heroes build` succeeds, writes a binary, and
/// the abort is the **program's** — `hero_unreachable()` from `emit/construct.rs`
/// under a `#line` pointing at the author's own source. CLAUDE.md §7 puts
/// `hero_unreachable()` at *type-system-proven-unreachable* points, and here the
/// type system proved nothing.
///
/// **It asks the value, not the world** (CLAUDE.md §11). The legal set is built by
/// reading the declarations in hand — which `TypeId` nodes are group-record field
/// types — rather than by an allow-list of positions somebody enumerated; a
/// position added later is refused by default instead of silently admitted, which
/// is the loud direction. And it walks the **type table**, never the syntax:
/// panel 061's `map_keys` walked `TypeKind` nodes and missed every type a literal
/// interned, so the same defect is a `written_types` lookup away here.
pub(super) fn fixed_only_in_a_group(checker: &mut Checker, ast: &Ast, src: &Source) {
    let mut owned_by_a_header = std::collections::BTreeSet::new();
    for decl in &ast.decls {
        if let crate::syntax::DeclKind::Record { fields, header: Some(_), .. } = &decl.kind {
            for field in fields {
                claim(ast, field.ty, &mut owned_by_a_header);
            }
        }
    }
    // ORDER: ascending type-node id — span.start ties on nested fixed arrays
    // (`i64[2]` inside `i64[2][3]`), so the later stable span sort cannot
    // replace this key: it decides which diagnostic prints first. The Heroes
    // port owes an explicit sort (design.md §4.9).
    let offenders: Vec<(TyId, crate::source::Span)> = checker
        .out
        .written_types
        .iter()
        .filter(|(node, _)| !owned_by_a_header.contains(node))
        .filter(|(_, ty)| matches!(checker.out.types.get(**ty), Ty::Fixed(_, _)))
        .map(|(node, ty)| (*ty, ast.types[*node as usize].span))
        .collect();
    for (ty, span) in offenders {
        // `decls.rs` already refuses an ordinary record's field on the same rule,
        // and reporting one mistake twice is the thing this compiler never does.
        if checker.out.diagnostics.iter().any(|d| d.span == span) {
            continue;
        }
        let name = checker.show(ast, src, ty);
        checker.push_diagnostic(errors::fixed_outside_a_group(&name, span));
    }
}

/// A group record's field type **and everything written inside it**.
///
/// The whole node, not just its root, and that is the difference between a rule
/// and a second opinion: `i32[2][3]` in a group's `record` is already refused by
/// `ffi_field` — the header owns the layout and panel 062 admitted no array of
/// arrays — so claiming only the outer node left the walk above reporting
/// `i32[2]` on the same column, one mistake with two messages. Whatever is
/// written inside a group field is that field's business, and `ffi_field` is the
/// seat that judges it.
fn claim(ast: &Ast, id: crate::syntax::TypeId, claimed: &mut std::collections::BTreeSet<u32>) {
    claimed.insert(id.0);
    match &ast.types[id.0 as usize].kind {
        crate::syntax::TypeKind::Fixed(inner, _)
        | crate::syntax::TypeKind::Array(inner)
        | crate::syntax::TypeKind::Fallible(inner) => claim(ast, *inner, claimed),
        crate::syntax::TypeKind::Map(key, value) => {
            claim(ast, *key, claimed);
            claim(ast, *value, claimed);
        }
        crate::syntax::TypeKind::Func { params, result } => {
            for param in params {
                claim(ast, *param, claimed);
            }
            claim(ast, *result, claimed);
        }
        // A leaf writes no inner node. Enumerated rather than `_`, so a type form
        // added later is a compile error here instead of a silently unclaimed
        // child (CLAUDE.md §11, and panel 060 paid for the catch-all version).
        crate::syntax::TypeKind::Named
        | crate::syntax::TypeKind::Unit
        | crate::syntax::TypeKind::Error => {}
    }
}
