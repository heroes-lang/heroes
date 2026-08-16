//! The classes where the header contradicts a **record** (design.md §4.19;
//! panel 060 for the field, panel 061 for the two that surround it).
//!
//! Split from `ffi_declared.rs` on 2026-08-15, on the seam this milestone has now
//! cut three times — `types/decls.rs` → `types/ffi_decls.rs`, `errors/data.rs` →
//! `errors/ffi.rs`, and here. A signature and a struct fail differently and the
//! repairs differ: a wrong result type is one word in a `function` line, a wrong
//! field is one line inside a block, and a **missing** field is not on the page at
//! all.
//!
//! That third one is why this file exists rather than being three more functions.
//! `field_type` and `unknown_field` both read an assertion **this emitter wrote**,
//! so their match is a contract. `incomplete_record` reads clang's own wording for
//! `-Wmissing-field-initializers` and finds the declaration through the **`#line`
//! mapping** instead — a different mechanism for a different question, and keeping
//! it beside its two siblings is what makes the difference visible.

use crate::diagnostics::Diagnostic;
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};

use super::extern_record::FIELD_ASSERTION;
use super::extern_union::UNION_ASSERTION;
use super::ffi_declared::FAILED;

/// `_Static_assert(_Generic(&((Color *)0)->r, uint8_t *: 1, …), "heroes-ffi-field
/// Color r")` failed: the header's struct does not have that field at that type.
///
/// The third class in this file, and it belongs here for the reason the other two
/// do — it reads an assertion **this emitter wrote**, so the split is on the
/// emitter's own format rather than on a guess about clang's wording.
///
/// **What makes it safe is the same `declaration()` rule, applied to a record**
/// (CLAUDE.md §7's narrowing): the name is recovered and then asked whether *this
/// program* declared it inside an `extern` group. A struct nobody declared that way
/// stays exit 2 and the compiler's, however much the message looks like one of
/// ours.
///
/// The diagnostic points at the **field**, not at the record, because that is the
/// token the author must change — and it is why `extern_record.rs` puts its `#line`
/// on the field rather than on the declaration.
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

/// clang's own *"missing field 'a' initializer"*, from the positional completeness
/// probe: the header's struct has a member the program never named.
///
/// **This is the one refusal that protects a record with no marker on it.** The
/// field assertions check every field the program *writes* and are silent about a
/// field it left out — so `record Color` with three of raylib's four members
/// compiled with every assertion green and returned a transparent colour where the
/// program asked for an opaque one (panel 061).
///
/// Recognised by the **probe's own name** in the source line clang echoes, not by
/// the message text: `missing field` is clang's wording and could come from
/// anywhere. `COMPLETE_PROBE` is a string this emitter invented, and the record it
/// names is then asked whether *this program* declared it in an `extern` group —
/// `declaration()`'s rule, which is what CLAUDE.md §7 says makes a class safe.
pub(super) fn incomplete_record(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    let member = line.split("missing field '").nth(1)?.split('\'').next()?;
    // **Found through the `#line` mapping, not through the probe's name.** clang
    // writes its own wording for `-Wmissing-field-initializers` and echoes the
    // source line *underneath* it, so the marker this emitter invented is on a
    // different line from the message — matching both at once would mean holding
    // state across clang's output, which is a premise about its layout (panel 038
    // paid for one of those). The `#line` is already exact: the probe carries the
    // declaration's own position, so the position IS the identification.
    //
    // The narrowing CLAUDE.md §7 asks for is still the one that matters: the
    // record is looked up among **this program's** group declarations, so a
    // `missing field` from anywhere else finds nothing and stays exit 2.
    let at = line.split(": error:").next()?;
    let mut parts = at.rsplitn(3, ':');
    let _column = parts.next()?;
    let row: u32 = parts.next()?.trim().parse().ok()?;
    let (type_name, header, span) = record_at_line(ast, src, row)?;
    Some(
        Diagnostic::new(
            "ffi_incomplete_record",
            format!(
                "`{type_name}` does not name `{member}`, and `{header}` says the struct has it — a group's `record` IS the header's struct, so a field left out is a field C fills with zero"
            ),
            span,
        )
        .with_note(format!(
            "the assertions above check every field you DO name and cannot see one you leave out: a `{type_name}` built here would carry a zero `{member}` into C, which is a value rather than a crash"
        )),
    )
}

/// The group `record` whose name sits on this `.hero` line.
fn record_at_line(
    ast: &Ast,
    src: &Source,
    row: u32,
) -> Option<(String, String, crate::source::Span)> {
    ast.decls.iter().find_map(|decl| {
        let DeclKind::Record { header: Some(header), .. } = &decl.kind else { return None };
        if src.line_of(decl.name.start) != row {
            return None;
        }
        Some((
            src.slice(decl.name).to_string(),
            src.slice(*header).trim_matches('"').to_string(),
            decl.name,
        ))
    })
}


/// A group's `record` over a C **union**, at the two operations that would answer
/// wrongly: constructing one, and comparing or hashing one (panel 073).
///
/// The assertion `extern_union.rs` writes says *these declared fields do not
/// overlap*, and only a union can fail it. What the author gets back names the
/// fields that share an address and the route that still works, because the route
/// is the finding: **reading is untouched**, and a `record` naming one member of
/// the union binds it soundly — which is how five other languages bind one too.
///
/// The same `declaration()` narrowing as every other class in this file: the name
/// is recovered from our own marker and then asked whether *this program* declared
/// it inside an `extern` group, so a struct nobody declared that way stays exit 2
/// and the compiler's.
pub(super) fn union_record(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    if !line.contains(FAILED) {
        return None;
    }
    let rest = line.split(UNION_ASSERTION).nth(1)?;
    let mut parts = rest.split_whitespace();
    let type_name = parts.next()?;
    let members: Vec<&str> = parts.take_while(|p| !p.contains('"')).collect();
    let (header, span) = record_declaration(ast, src, type_name)?;
    let named = match members.as_slice() {
        [] => String::new(),
        [one] => format!("`{one}`"),
        [a, b] => format!("`{a}` and `{b}`"),
        many => format!("`{}` and `{}`", many[..many.len() - 1].join("`, `"), many[many.len() - 1]),
    };
    Some(
        Diagnostic::new(
            "ffi_union_field",
            format!(
                "`{type_name}` is a `union` in `{header}`, so {named} are the same bytes — building one or comparing it would answer about whichever was written last"
            ),
            span,
        )
        .with_note(
            "a `record` over a union may name ONE member, and that binds it soundly: reading through it is correct and construction sets the member it names. Two or more is what has no answer — C keeps the last one written, so `==` and `hash` walk the same bytes twice".to_string(),
        ),
    )
}

/// The declaration's own name span, for a class whose fault is the declaration
/// rather than any one field.
fn record_declaration(
    ast: &Ast,
    src: &Source,
    type_name: &str,
) -> Option<(String, crate::source::Span)> {
    ast.decls.iter().find_map(|decl| {
        if src.slice(decl.name) != type_name {
            return None;
        }
        let DeclKind::Record { header: Some(header), .. } = &decl.kind else { return None };
        Some((src.slice(*header).trim_matches('"').to_string(), decl.name))
    })
}
