//! The top level: what a name can be bound to, plus `extern` and `test`
//! (design.md §4.1, §4.2, §4.18, §4.19).
//!
//! One shape carries the four entities — `NAME = entity: type` — so the
//! dispatcher reads a name, an `=`, and one keyword; the arms differ only in
//! what follows. `record` and `variant` live next door in `data.rs`.
//!
//! The rule that is easy to get wrong, written down once here: **a body is
//! not introduced by a terminator.** The lexer plants one only when a line's
//! last token could end a statement (panel 007), so `MAX = constant: int`
//! gets one and `Point = record` does not. Every body therefore skips
//! terminators and asks only for the `Indent`.

use crate::lexer::TokenKind;
use crate::source::{Source, Span};

use super::ast::{Ast, Decl, DeclKind, Function, TypeKind, TypeNode};
use super::cursor::Cursor;
use super::data::{record, variant};
use super::members::{generics, params};
use super::stmt::block;
use super::types::parse_type;

/// Read declarations until end of file. Every path either builds a
/// declaration or recovers past one, so this loop always makes progress.
pub(super) fn file(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    loop {
        cur.skip_terminators();
        match cur.kind() {
            TokenKind::Eof => return,
            TokenKind::Ident => named(cur, ast, src),
            TokenKind::KwExtern => extern_function(cur, ast, src),
            TokenKind::KwTest => test(cur, ast, src),
            _ => {
                if !cur.at_reported_error() {
                    let message = format!(
                        "expected a declaration, found {} — every top-level line names something: `name = constant|function|record|variant`, `test \"…\"`, or `extern`",
                        cur.found(src)
                    );
                    cur.error("expected_declaration", message, cur.span());
                }
                // The one arm that has consumed nothing yet, so it consumes
                // here: recovery only drops the *rest* of a broken line, and
                // this loop must always move. An indented block at top level
                // belongs to no declaration, so it is dropped whole — and
                // nothing else is, because the next line is innocent.
                if cur.at(TokenKind::Indent) {
                    cur.balanced_block();
                } else {
                    cur.bump();
                    cur.recover_to_next_decl(src);
                }
            }
        }
    }
}

/// `NAME = <entity>` — the four entities share this head.
fn named(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    let name = cur.span();
    let doc = cur.take_docs(src, name);
    cur.bump();
    if !cur.expect(
        TokenKind::Eq,
        "expected_eq",
        "`=` — a top-level line binds a name to one of the four entities",
        src,
    ) {
        cur.recover_to_next_decl(src);
        return;
    }
    match cur.kind() {
        TokenKind::KwConstant => constant(cur, ast, src, name, doc),
        TokenKind::KwFunction => function(cur, ast, src, name, doc, false),
        TokenKind::KwRecord => record(cur, ast, src, name, doc),
        TokenKind::KwVariant => variant(cur, ast, src, name, doc),
        _ => {
            if !cur.at_reported_error() {
                let message = format!(
                    "expected `constant`, `function`, `record` or `variant`, found {} — those four are everything a name can be",
                    cur.found(src)
                );
                cur.error("expected_entity", message, cur.span());
            }
            cur.recover_to_next_decl(src);
        }
    }
}

/// `MAX_DEPTH = constant: int` + the value, indented (§4.2). Constants use
/// SCREAMING_CASE by convention; the parser does not police it.
fn constant(cur: &mut Cursor, ast: &mut Ast, src: &Source, name: Span, doc: Vec<Span>) {
    cur.bump(); // `constant`
    if !cur.expect(
        TokenKind::Colon,
        "expected_constant_type",
        "`:` and the constant's type — a `constant` declares a value, so it has one",
        src,
    ) {
        cur.recover_to_next_decl(src);
        return;
    }
    let ty = parse_type(cur, ast, src);
    let Some(body) = block(cur, ast, src, "a `constant`") else { return };
    let span = name.to(body.span);
    ast.decls.push(Decl { name, doc, span, kind: DeclKind::Constant { ty, body } });
}

/// `dist2 = function: (a: Point, b: Point) -> int` + body, and the same
/// signature without a body for an `extern` (§4.19).
fn function(
    cur: &mut Cursor,
    ast: &mut Ast,
    src: &Source,
    name: Span,
    doc: Vec<Span>,
    is_extern: bool,
) {
    cur.bump(); // `function`
    let generic_names = generics(cur, src);
    if !cur.expect(
        TokenKind::Colon,
        "expected_signature",
        "`:` and the signature — `function: (a: int) -> bool`",
        src,
    ) {
        cur.recover_to_next_decl(src);
        return;
    }
    let parameters = params(cur, ast, src);
    // No `->` means no value. The `Unit` node is synthesised at the end of
    // the header so later passes never have to ask whether the arrow was
    // written (§4.3: `()` is a type like any other).
    let result = if cur.eat(TokenKind::Arrow) {
        parse_type(cur, ast, src)
    } else {
        let here = cur.span();
        ast.push_type(TypeNode { kind: TypeKind::Unit, span: Span { start: here.start, end: here.start } })
    };
    let body = if is_extern {
        extern_body_check(cur);
        None
    } else {
        match block(cur, ast, src, "a `function`") {
            Some(body) => Some(body),
            None => return,
        }
    };
    let end = match &body {
        Some(block) => block.span,
        None => cur.span(),
    };
    let kind = DeclKind::Function(Function {
        generics: generic_names,
        params: parameters,
        result,
        body,
        is_extern,
    });
    ast.decls.push(Decl { name, doc, span: name.to(end), kind });
}

/// `extern sqrt = function: (x: f64) -> f64` (§4.19). Only a function can be
/// `extern`: the implementation comes from C, and C has functions.
fn extern_function(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    let keyword = cur.span();
    let doc = cur.take_docs(src, keyword);
    cur.bump(); // `extern`
    if !cur.at(TokenKind::Ident) {
        if !cur.at_reported_error() {
            let message = format!(
                "expected the C function's name, found {} — `extern sqrt = function: (x: f64) -> f64`",
                cur.found(src)
            );
            cur.error("expected_extern_name", message, cur.span());
        }
        cur.recover_to_next_decl(src);
        return;
    }
    let name = cur.bump().span;
    if !cur.expect(TokenKind::Eq, "expected_eq", "`=` and the signature", src) {
        cur.recover_to_next_decl(src);
        return;
    }
    if !cur.at(TokenKind::KwFunction) {
        if !cur.at_reported_error() {
            let message = format!(
                "only a `function` can be `extern`, found {} — everything else comes from C through a function (§4.19)",
                cur.found(src)
            );
            cur.error("extern_not_function", message, cur.span());
        }
        cur.recover_to_next_decl(src);
        return;
    }
    function(cur, ast, src, name, doc, true);
}

/// An `extern` with a body is a mistake worth naming: the body would never
/// be compiled, and silently ignoring it is the class of thing this language
/// exists to make loud.
fn extern_body_check(cur: &mut Cursor) {
    cur.skip_terminators();
    if !cur.at(TokenKind::Indent) {
        return;
    }
    let span = cur.span();
    cur.error(
        "extern_has_body",
        "an `extern` declaration has no body — it names a C function, and C provides the code"
            .to_string(),
        span,
    );
    cur.balanced_block();
}

/// `test "3-4-5 triangle"` + body (§4.18). The name is a string, not an
/// identifier: it is a title, and it is printed when the test fails.
fn test(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    let keyword = cur.span();
    let doc = cur.take_docs(src, keyword);
    cur.bump(); // `test`
    if !cur.at(TokenKind::Str) {
        if !cur.at_reported_error() {
            let message = format!(
                "expected the test's name as a string, found {} — `test \"3-4-5 triangle\"`",
                cur.found(src)
            );
            cur.error("expected_test_name", message, cur.span());
        }
        cur.recover_to_next_decl(src);
        return;
    }
    let name = cur.bump().span;
    let Some(body) = block(cur, ast, src, "a `test`") else { return };
    let span = keyword.to(body.span);
    ast.decls.push(Decl { name, doc, span, kind: DeclKind::Test { body } });
}
