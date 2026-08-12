//! What sits *inside* a declaration: type parameters, value parameters,
//! record fields, variant cases (design.md §4.2, §4.8, §4.12).
//!
//! One rule explains most of this file: **the lexer keeps planting
//! terminators inside brackets** (panel 007 — a line ending in a name can
//! always be over, brackets or not), so every list here skips terminators
//! where a separator may appear. That is what lets a long signature break
//! across lines while `,` stays the separator.

use crate::lexer::TokenKind;
use crate::source::{Source, Span};

use super::ast::{Ast, Case, Field, Param};
use super::cursor::Cursor;
use super::types::parse_type;

/// `function map<A, B>(…)` — type parameter names after the declared name,
/// no constraints, never written at the call site (§4.12). Absent `<` means
/// a non-generic function.
pub(super) fn generics(cur: &mut Cursor, src: &Source) -> Vec<Span> {
    let mut names: Vec<Span> = Vec::new();
    if !cur.eat(TokenKind::Lt) {
        return names;
    }
    loop {
        if !cur.at(TokenKind::Ident) {
            if !cur.at_reported_error() {
                let message = format!(
                    "expected a type parameter name, found {} — generics carry no constraints, only names",
                    cur.found(src)
                );
                cur.error("expected_type_parameter", message, cur.span());
            }
            break;
        }
        names.push(cur.bump().span);
        if !cur.eat(TokenKind::Comma) {
            break;
        }
    }
    cur.expect(
        TokenKind::Gt,
        "expected_generics_close",
        "`>` to close the type parameters",
        src,
    );
    names
}

/// `(a: i64, @l: Lex)` — always parenthesised, `()` when empty. A parameter
/// marked `@` is in-out: copy in, copy out (§4.8).
pub(super) fn params(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> Vec<Param> {
    let mut params: Vec<Param> = Vec::new();
    if !cur.expect(
        TokenKind::LParen,
        "expected_params",
        "`(` — a signature always has a parameter list, `()` when it takes nothing",
        src,
    ) {
        return params;
    }
    cur.skip_terminators();
    if cur.eat(TokenKind::RParen) {
        return params;
    }
    loop {
        cur.skip_terminators();
        let mutable = cur.eat(TokenKind::At);
        if !cur.at(TokenKind::Ident) {
            if !cur.at_reported_error() {
                let message = format!(
                    "expected a parameter name, found {} — a parameter is `name: type`, or `@name: type` when the callee may change it",
                    cur.found(src)
                );
                cur.error("expected_parameter", message, cur.span());
            }
            break;
        }
        let name = cur.bump().span;
        if !cur.expect(
            TokenKind::Colon,
            "expected_parameter_type",
            "`:` and the parameter's type — signatures are always explicit",
            src,
        ) {
            break;
        }
        let ty = parse_type(cur, ast, src);
        params.push(Param { name, ty, mutable });
        cur.skip_terminators();
        if !cur.eat(TokenKind::Comma) {
            break;
        }
    }
    cur.skip_terminators();
    if !cur.expect(
        TokenKind::RParen,
        "expected_params_close",
        "`)`, or `,` and another parameter",
        src,
    ) {
        // The signature's own closer is the landmark: resume just past it, so
        // the result type and the body are still read (one mistake, one
        // diagnostic).
        cur.recover_past_closer(TokenKind::LParen, TokenKind::RParen);
    }
    params
}

/// One `name: type` per line: a record's fields, and a variant case's
/// payload. Consumes the whole `Indent` … `Dedent` block.
///
/// Fields carry doc comments of their own — §4.1's adjacency rule is about
/// declarations, and a field is one.
pub(super) fn field_block(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> Vec<Field> {
    let mut fields: Vec<Field> = Vec::new();
    cur.bump(); // the Indent
    loop {
        cur.skip_terminators();
        match cur.kind() {
            TokenKind::Dedent => {
                cur.bump();
                return fields;
            }
            TokenKind::Eof => return fields,
            TokenKind::Ident => {
                let name = cur.span();
                let doc = cur.take_docs(src, name);
                cur.bump();
                if !cur.expect(
                    TokenKind::Colon,
                    "expected_field_type",
                    "`:` and the field's type",
                    src,
                ) {
                    cur.skip_line();
                    continue;
                }
                let ty = parse_type(cur, ast, src);
                fields.push(Field { name, ty, doc });
            }
            TokenKind::Indent => {
                if !cur.at_reported_error() {
                    cur.error(
                        "unexpected_block",
                        "a field takes no indented block — a field is one line, `name: type`"
                            .to_string(),
                        cur.span(),
                    );
                }
                cur.balanced_block();
            }
            _ => {
                if !cur.at_reported_error() {
                    let message = format!(
                        "expected a field name, found {} — one field per line, `name: type`",
                        cur.found(src)
                    );
                    cur.error("expected_field", message, cur.span());
                }
                cur.skip_line();
            }
        }
    }
}

/// The cases of a `variant`: a name on its own line, optionally followed by
/// an indented field block. A case with fields *is* a small record (§4.2) —
/// which is why the surface needs no separator syntax at all.
pub(super) fn case_block(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> Vec<Case> {
    let mut cases: Vec<Case> = Vec::new();
    cur.bump(); // the Indent
    loop {
        cur.skip_terminators();
        match cur.kind() {
            TokenKind::Dedent => {
                cur.bump();
                return cases;
            }
            TokenKind::Eof => return cases,
            TokenKind::Ident => {
                let name = cur.span();
                let doc = cur.take_docs(src, name);
                cur.bump();
                cur.skip_terminators();
                let fields = if cur.at(TokenKind::Indent) {
                    field_block(cur, ast, src)
                } else {
                    Vec::new()
                };
                cases.push(Case { name, fields, doc });
            }
            _ => {
                if !cur.at_reported_error() {
                    let message = format!(
                        "expected a case name, found {} — a variant case is a name on its own line, with its fields indented below it",
                        cur.found(src)
                    );
                    cur.error("expected_case", message, cur.span());
                }
                cur.skip_line();
            }
        }
    }
}
