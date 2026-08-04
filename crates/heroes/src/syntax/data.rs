//! The two type declarations: `record` and `variant` — product and sum
//! (design.md §4.2).
//!
//! They are the two entities that declare a *type* rather than a value, so
//! they take no `:` and no signature: a keyword, then a block. Both share
//! the same trap, which is why they share a file: neither `record` nor
//! `variant` can end a statement, so the lexer plants **no terminator**
//! after the header (panel 007). A missing block is therefore discovered on
//! the *next line*, and the diagnostic must not take that line with it —
//! see `Cursor::at_line_start`.

use crate::lexer::TokenKind;
use crate::source::{Source, Span};

use super::ast::{Ast, Decl, DeclKind};
use super::cursor::Cursor;
use super::members::{case_block, field_block};

/// `Point = record` + one field per line (§4.2).
pub(super) fn record(cur: &mut Cursor, ast: &mut Ast, src: &Source, name: Span, doc: Vec<Span>) {
    cur.bump(); // `record`
    cur.skip_terminators();
    if !cur.at(TokenKind::Indent) {
        if !cur.at_reported_error() {
            cur.error(
                "empty_record",
                "a `record` needs at least one field, indented one level below it".to_string(),
                cur.span(),
            );
        }
        cur.recover_to_next_decl(src);
        return;
    }
    let start = cur.span();
    let fields = field_block(cur, ast, src);
    let end = fields.last().map_or(start, |field| field.name);
    ast.decls.push(Decl {
        name,
        doc,
        span: name.to(end),
        kind: DeclKind::Record { fields },
    });
}

/// `Token = variant` + one case per line, each case optionally carrying
/// fields — a case with fields *is* a small record (§4.2).
pub(super) fn variant(cur: &mut Cursor, ast: &mut Ast, src: &Source, name: Span, doc: Vec<Span>) {
    cur.bump(); // `variant`
    cur.skip_terminators();
    if !cur.at(TokenKind::Indent) {
        if !cur.at_reported_error() {
            cur.error(
                "empty_variant",
                "a `variant` needs at least one case, indented one level below it".to_string(),
                cur.span(),
            );
        }
        cur.recover_to_next_decl(src);
        return;
    }
    let start = cur.span();
    let cases = case_block(cur, ast, src);
    let end = cases.last().map_or(start, |case| case.name);
    ast.decls.push(Decl { name, doc, span: name.to(end), kind: DeclKind::Variant { cases } });
}
