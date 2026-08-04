//! The two type declarations: `record` and `variant` — product and sum
//! (design.md §4.2; panel 018 for the keyword-first shape).
//!
//! They are the two entities that declare a *type* rather than a value, so
//! no `:` and no signature follow the name: keyword, name, block. Their old
//! shared trap — headers ending in a non-ender, so no terminator (panel
//! 007) — died with the inversion: `record Point` ends in a name, and the
//! lexer terminates the line like any other.

use crate::lexer::TokenKind;
use crate::source::Source;

use super::ast::{Ast, Decl, DeclKind};
use super::cursor::Cursor;
use super::decl::head;
use super::members::{case_block, field_block};
use super::stmt::eat_python_colon;

/// `record Point` + one field per line (§4.2).
pub(super) fn record(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    let Some((keyword, doc, name)) = head(cur, src, "record Point") else {
        return;
    };
    eat_python_colon(cur);
    cur.skip_terminators();
    if !cur.at(TokenKind::Indent) {
        if !cur.at_reported_error() {
            cur.error(
                "empty_record",
                "a `record` needs at least one field, indented one level below it".to_string(),
                cur.span(),
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
        kind: DeclKind::Record { fields },
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
                cur.span(),
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
