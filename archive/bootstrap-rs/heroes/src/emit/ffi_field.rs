//! What the header says about **one field** — its type, its absence, and the one
//! case where it has no size at all (design.md §4.19; CLAUDE.md §7's named
//! exception).
//!
//! Split from `ffi_record.rs` by CLAUDE.md §11, on the seam the two halves
//! already had: that file answers *is this declaration the header's struct* —
//! incomplete, or a union — and this one answers *is this field the header's
//! field*. They fail at different spans and they name different repairs: a field
//! mistake is fixed on a field line, a declaration mistake by changing or marking
//! the declaration.
//!
//! All three keep §7's narrowing: the name is recovered from a marker this
//! emitter wrote, and then asked whether *this program* declared it inside an
//! `extern` group — so a struct nobody declared that way stays exit 2 and the
//! compiler's.

use crate::diagnostics::Diagnostic;
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};

use super::extern_record::{FIELD_ASSERTION, FLEX_ASSERTION};
use super::ffi_declared::FAILED;

/// `_Static_assert(_Generic(&((Color *)0)->r, uint8_t *: 1, …), "heroes-ffi-field
/// Color r")` failed: the header's struct does not have that field at that type.
///
/// It reads an assertion **this emitter wrote**, so the match is a contract on
/// the emitter's own format rather than a guess about clang's wording — and
/// **what makes it safe is `declaration()`** (CLAUDE.md §7's narrowing): the name
/// is recovered and then asked whether *this program* declared it inside an
/// `extern` group. A struct nobody declared that way stays exit 2 and the
/// compiler's, however much the message looks like one of ours.
///
/// The diagnostic points at the **field**, not at the record, because that is the
/// token the author must change — and it is why `extern_record.rs` puts its
/// `#line` on the field rather than on the declaration.
pub(super) fn field_type(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    if !line.contains(FAILED) {
        return None;
    }
    let rest = line.split(FIELD_ASSERTION).nth(1)?;
    let mut parts = rest.split_whitespace();
    let (type_name, member) = (parts.next()?, parts.next()?);
    let (header, field_span, declared) = record_field(ast, src, type_name, member)?;
    Some(
        Diagnostic::new(
            "ffi_field_type",
            format!(
                "`{type_name}.{member}` is not `{declared}` in `{header}` — clang read the header's struct and the field disagrees"
            ),
            field_span,
        )
        .with_note(format!(
            "a group's `record` IS the header's struct (§4.19), so every field is at the header's own width and sign: correct `{member}`, or name the header that spells `{type_name}` this way"
        )),
    )
}

/// The `record` a group declares under this C name, plus the span and the written
/// type of one of its fields. `None` for a name no group declares as a record —
/// which is what keeps a message that merely looks like ours from being read as
/// one.
fn record_field(
    ast: &Ast,
    src: &Source,
    type_name: &str,
    member: &str,
) -> Option<(String, crate::source::Span, String)> {
    ast.decls.iter().find_map(|decl| {
        if src.slice(decl.name) != type_name {
            return None;
        }
        let DeclKind::Record { fields, header: Some(header), .. } = &decl.kind else { return None };
        let field = fields.iter().find(|f| src.slice(f.name) == member)?;
        let written = src.slice(ast.types[field.ty.0 as usize].span).to_string();
        Some((src.slice(*header).trim_matches('"').to_string(), field.name, written))
    })
}

/// clang's own *"no member named 'red' in 'struct Color'"*: the header's struct
/// exists and the field does not.
///
/// **It matches clang's wording rather than one of ours, and that is safe for the
/// reason `unknown_name` is** (CLAUDE.md §7): the narrowing is `record_group`, not
/// whose text this is. The struct name is recovered from clang's message and then
/// asked whether *this program* declared it inside an `extern` group — a struct
/// nobody declared that way stays exit 2 and the compiler's.
///
/// One mistake, **three** clang errors: the `_Generic` and the `sizeof` each name
/// the member, and every construction site adds a *field designator does not refer
/// to any field* on top. `ffi::push` collapses them by span, which is why the
/// author sees one diagnostic about one field.
pub(super) fn unknown_field(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    let rest = line.split("no member named '").nth(1)?;
    let (member, rest) = rest.split_once('\'')?;
    let type_name = rest.split("struct ").nth(1)?.split('\'').next()?.trim();
    let (header, _) = record_group(ast, src, type_name)?;
    let field = field_span(ast, src, type_name, member)?;
    Some(
        Diagnostic::new(
            "ffi_unknown_field",
            format!("`{header}` declares no `{member}` in `{type_name}` — clang read the header's struct and it has no such field"),
            field,
        )
        .with_note(format!(
            "a group's `record` IS the header's struct (§4.19), so a field is named exactly as the header names it — correct the spelling, or name the header that gives `{type_name}` a `{member}`"
        ))
    )
}

/// The `record` a group declares under this C name, and where the declaration is.
fn record_group(ast: &Ast, src: &Source, type_name: &str) -> Option<(String, crate::source::Span)> {
    ast.decls.iter().find_map(|decl| {
        if src.slice(decl.name) != type_name {
            return None;
        }
        let DeclKind::Record { header: Some(header), .. } = &decl.kind else { return None };
        Some((src.slice(*header).trim_matches('"').to_string(), decl.span))
    })
}

/// Where the author wrote that field name, so the caret lands on the token they
/// must change rather than on the record's own line.
fn field_span(ast: &Ast, src: &Source, type_name: &str, member: &str) -> Option<crate::source::Span> {
    ast.decls.iter().find_map(|decl| {
        if src.slice(decl.name) != type_name {
            return None;
        }
        let DeclKind::Record { fields, header: Some(_), .. } = &decl.kind else { return None };
        fields.iter().find(|f| src.slice(f.name) == member).map(|f| f.name)
    })
}

/// A field the header gives **no size**: a flexible array member (panel 079,
/// discharging panel 071's ffi-pragmatist condition).
///
/// **The old message named a repair that does not exist.** It was the generic
/// `ffi_field_type` — *"`WithFlex.data` is not `i8[4]` … the field disagrees"* —
/// with a note saying to *correct* the field, and for `char data[]` no length is
/// correct, so a reader went hunting for a number the header does not have. That
/// is §4.17 inverted: the diagnostic is the deliverable, and this one sent the
/// author to the wrong place.
///
/// The emitter can tell the two apart because C makes the fact a constant: a
/// flexible array member sits at `offsetof(T, m) == sizeof(T)`, so its tail is
/// **0**, while a sized array leaves a positive one — measured `{4, 4, 0}`
/// against `{12, 4, 8}`. `extern_field.rs` asks that first and separately, which
/// is why this message can be certain of what it is looking at rather than
/// guessing from a failed length.
///
/// The route it names is the one `spec:222` already gives — omit the field and
/// mark the `record` `partial`, then reach the bytes through the header's own
/// functions. The spec is not asked to repeat itself (panel 079's spec-warden
/// vetoed a sentence at +26).
pub(super) fn flexible_array_member(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    if !line.contains(FAILED) {
        return None;
    }
    let rest = line.split(FLEX_ASSERTION).nth(1)?;
    let mut parts = rest.split_whitespace();
    let (type_name, member) = (parts.next()?, parts.next()?);
    let (header, field_span, _) = record_field(ast, src, type_name, member)?;
    Some(
        Diagnostic::new(
            "ffi_flexible_array_member",
            format!(
                "`{header}` gives `{type_name}.{member}` no size — it is written there with empty brackets, so it is not a field this record can name at any length"
            ),
            field_span,
        )
        .with_note(format!(
            "leave `{member}` out and write the `record` `partial` (§4.19): what is left is the header's struct minus a member C itself does not size, and those bytes are reached through the functions the header declares for them"
        )),
    )
}
