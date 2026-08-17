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
//! **Narrowed again by panel 079**, when the field half outgrew the file: the
//! per-field classes moved to `ffi_field.rs` and what stayed is the two that
//! fault the **declaration** — a record that names too few of the header's fields,
//! and one whose header type is a `union`. Both are repaired by changing or
//! marking the declaration, where a field mistake is repaired on a field line.
//!
//! The two differ in mechanism and that is worth seeing side by side:
//! `union_record` reads an assertion **this emitter wrote**, so its match is a
//! contract, while `incomplete_record` reads clang's own wording for
//! `-Wmissing-field-initializers` and finds the declaration through the **`#line`
//! mapping** — the probe's name is echoed on a different line from the message.

use crate::diagnostics::Diagnostic;
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};

use super::extern_union::UNION_ASSERTION;
use super::ffi_declared::FAILED;

/// The header's struct has a field the `record` never names, and C fills it with
/// zero (panel 061).
///
/// **The one class here that does NOT read a marker this emitter wrote.** Its two
/// siblings match an assertion's own text; this one matches clang's wording for
/// `-Wmissing-field-initializers` and finds the declaration through the **`#line`
/// mapping**, because the probe's name is echoed on a different line from the
/// message. A different mechanism for a different question, and the reason this
/// file's module doc names it out.
///
/// Same `declaration()` narrowing all the same (CLAUDE.md §7): the record is
/// recovered and then asked whether *this program* declared it inside an `extern`
/// group.
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
    let path = parts.next()?;
    let (type_name, header, span) = record_at_line(ast, src, path, row)?;
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
    path: &str,
    row: u32,
) -> Option<(String, String, crate::source::Span)> {
    ast.decls.iter().find_map(|decl| {
        let DeclKind::Record { header: Some(header), .. } = &decl.kind else { return None };
        // **The row clang echoes is the `#line` mapping's — 1-based within its own
        // file — so the whole-text line is the wrong denominator the moment a
        // program has two files.** It matched anyway for every single-file golden,
        // which is how it survived: `line_of` and `locate`'s line agree exactly
        // when `lines_before` is zero. Found by the port, whose reader asked which
        // of the two source questions this one is.
        let (file, line, _) = src.locate(decl.name.start);
        if file != path || line != row {
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
    // **The two cases read differently because they ARE different** (panel 077).
    // With one declared member the record is buildable and readable and it is
    // `==`/`hash` that has no answer: the bytes may hold another arm. With two or
    // more, building it is already wrong, because C keeps the last one written.
    let (message, note) = match members.as_slice() {
        [] | [_] => {
            let one = members.first().copied().unwrap_or("its member");
            (
                format!(
                    "`{type_name}` is a `union` in `{header}`, so comparing or hashing one asks about bytes that may hold another member — `{one}` is only what was written last"
                ),
                format!(
                    "reading `{one}` and building one that names it are both sound and stay legal; what has no answer is `==`, `hash` and being a map key, because two values holding different members are equal whenever the bytes match. Mark the `record` `partial` if you want that refusal stated at the declaration"
                ),
            )
        }
        many => {
            let named = match many {
                [a, b] => format!("`{a}` and `{b}`"),
                rest => {
                    format!("`{}` and `{}`", rest[..rest.len() - 1].join("`, `"), rest[rest.len() - 1])
                }
            };
            (
                format!(
                    "`{type_name}` is a `union` in `{header}`, so {named} are the same bytes — building one or comparing it would answer about whichever was written last"
                ),
                "a `record` over a union may name ONE member, and that binds it soundly: reading through it is correct and construction sets the member it names. Two or more is what has no answer — C keeps the last one written, so `==` and `hash` walk the same bytes twice".to_string(),
            )
        }
    };
    Some(Diagnostic::new("ffi_union_field", message, span).with_note(note))
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
