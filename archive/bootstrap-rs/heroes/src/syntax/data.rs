//! The two type declarations: `record` and `variant` — product and sum
//! (design.md §4.2; panel 018 for the keyword-first shape).
//!
//! They are the two entities that declare a *type* rather than a value, so
//! no `:` and no signature follow the name: keyword, name, block. Their old
//! shared trap — headers ending in a non-ender, so no terminator (panel
//! 007) — died with the inversion: `record Point` ends in a name, and the
//! lexer terminates the line like any other.

use crate::lexer::TokenKind;
use crate::source::{Source, Span};

use super::ast::{Ast, Decl, DeclKind};
use super::cursor::Cursor;
use super::decl::{head, Linkage};
use super::members::{case_block, field_block};
use super::stmt::eat_python_colon;

/// `record Point` + one field per line (§4.2) — and, since panel 060, the same
/// shape inside an `extern` group, where the fields describe a struct the header
/// already declares (§4.19).
///
/// One entry point for both, because the *syntax* is identical: what differs is
/// the linkage handed in, and every question a later pass asks — may this field
/// type cross the boundary, is a typedef emitted, is the name mangled — reads
/// `header` rather than re-deriving it from where the declaration was parsed.
pub(super) fn record(cur: &mut Cursor, ast: &mut Ast, src: &Source, linkage: Linkage) {
    let Some((keyword, doc, name)) = head(cur, src, "record Point") else {
        return;
    };
    record_tail(cur, ast, src, keyword, doc, name, linkage);
}

/// The same declaration when the caller has **already taken the doc comment** —
/// which is what a group member needs, because a group's own doc documents its
/// first member and `head` would take the run a second time (§4.1's adjacency
/// rule, applied to a block).
pub(super) fn record_with_doc(
    cur: &mut Cursor,
    ast: &mut Ast,
    src: &Source,
    doc: Vec<Span>,
    linkage: Linkage,
) {
    let keyword = cur.span();
    cur.bump(); // `record`
    if !cur.at(TokenKind::Ident) {
        if !cur.at_reported_error() {
            let message = format!(
                "expected the C struct's name after `record`, found {} — `record Color`",
                cur.found(src)
            );
            cur.error("expected_name", message, cur.span());
        }
        cur.skip_line();
        return;
    }
    let name = cur.bump().span;
    record_tail(cur, ast, src, keyword, doc, name, linkage);
}

/// `partial` after a group `record`'s name, if it is there.
///
/// **A contextual keyword and not a reserved word** (panel 061). It is consumed
/// only here, in the one position where a declaration can carry it, so `partial`
/// stays an ordinary identifier everywhere in the language — a `record partial`
/// with a field named `partial` compiles, measured. Reserving a word would cost the
/// whole program's namespace to buy one line's grammar, and `spec/reserved-words.md`
/// is gated separately by §1.6 for exactly that reason.
///
/// **Only inside a group.** An ordinary `record` has no header behind it, so there
/// is no struct for its field list to be a subset *of*; the word there is a name
/// the parser has no reason to expect, and the field block's own diagnostic says so.
fn partial_marker(cur: &mut Cursor, src: &Source, in_group: bool) -> bool {
    if !in_group || !cur.at(TokenKind::Ident) || src.slice(cur.span()) != "partial" {
        return false;
    }
    cur.bump();
    true
}

/// `tag stat` after a group `record`'s name, if it is there (panel 074).
///
/// **Contextual exactly like `partial`, and for the same reason**: it is consumed
/// only in this position, so `tag` stays an ordinary identifier everywhere in the
/// language and a field named `tag` goes on compiling. What follows it must be an
/// identifier — C's tag — and a missing one is reported here rather than left to
/// the field block, which would say something about indentation.
///
/// **Order is `tag <name>` then `partial`**, and the other order is a parse error
/// on purpose: one order that parses is one order to learn, and a reader who
/// writes the other gets told immediately instead of getting a second grammar.
fn tag_marker(cur: &mut Cursor, src: &Source, in_group: bool) -> Option<Span> {
    if !in_group || !cur.at(TokenKind::Ident) || src.slice(cur.span()) != "tag" {
        return None;
    }
    let keyword = cur.bump().span;
    if !cur.at(TokenKind::Ident) {
        cur.error(
            "expected_declaration",
            "`tag` names the struct tag C uses for this type: `record FileStat tag stat`"
                .to_string(),
            cur.here_or(src, keyword),
        );
        return None;
    }
    Some(cur.bump().span)
}

#[allow(clippy::too_many_arguments)]
fn record_tail(
    cur: &mut Cursor,
    ast: &mut Ast,
    src: &Source,
    keyword: Span,
    doc: Vec<Span>,
    name: Span,
    linkage: Linkage,
) {
    let (header, library) = match linkage {
        Linkage::Heroes => (None, None),
        Linkage::Extern { header, library } => (Some(header), library),
    };
    let tag = tag_marker(cur, src, header.is_some());
    let partial = partial_marker(cur, src, header.is_some());
    eat_python_colon(cur);
    cur.skip_terminators();
    if !cur.at(TokenKind::Indent) {
        if !cur.at_reported_error() {
            cur.error(
                "empty_record",
                "a `record` needs at least one field, indented one level below it".to_string(),
                cur.here_or(src, keyword),
            );
        }
        cur.recover_to_next_decl();
        return;
    }
    let start = cur.span();
    let fields = field_block(cur, ast, src);
    let end = fields.last().map_or(start, |field| field.name);
    ast.decls.push(Decl {
        name,
        doc,
        span: keyword.to(end),
        kind: DeclKind::Record { fields, header, library, partial, tag },
    });
}

/// `variant Token` + one case per line, each case optionally carrying
/// fields — a case with fields *is* a small record (§4.2).
pub(super) fn variant(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    let Some((keyword, doc, name)) = head(cur, src, "variant Token") else {
        return;
    };
    eat_python_colon(cur);
    cur.skip_terminators();
    if !cur.at(TokenKind::Indent) {
        if !cur.at_reported_error() {
            cur.error(
                "empty_variant",
                "a `variant` needs at least one case, indented one level below it".to_string(),
                cur.here_or(src, keyword),
            );
        }
        cur.recover_to_next_decl();
        return;
    }
    let start = cur.span();
    let cases = case_block(cur, ast, src);
    let end = cases.last().map_or(start, |case| case.name);
    ast.decls.push(Decl { name, doc, span: keyword.to(end), kind: DeclKind::Variant { cases } });
}
