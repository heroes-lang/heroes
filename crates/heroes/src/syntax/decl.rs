//! The top level: the six declaration heads, one shape (design.md §4.1,
//! §4.2, §4.18, §4.19; panel 018).
//!
//! Every top-level line starts with its kind — `constant`, `function`,
//! `record`, `variant`, `test`, `extern` — so the dispatcher is a switch on
//! the first token of the line, and an identifier at top level is never a
//! declaration. That closed keyword set is also recovery's anchor: one
//! broken declaration can never swallow the next one.
//!
//! A useful consequence of the keyword-first shape: every header line now
//! ends in a token that can end a statement (a name, `)`, a type, a
//! string), so every header gets a terminator and the panel-007 trap —
//! headers ending in non-enders — is gone. Bodies still skip terminators
//! and ask only for the `Indent`.

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
            TokenKind::KwConstant => constant(cur, ast, src),
            TokenKind::KwFunction => function_decl(cur, ast, src),
            TokenKind::KwRecord => record(cur, ast, src),
            TokenKind::KwVariant => variant(cur, ast, src),
            TokenKind::KwExtern => extern_function(cur, ast, src),
            TokenKind::KwTest => test(cur, ast, src),
            _ => {
                if !cur.at_reported_error() {
                    let message = format!(
                        "expected a declaration, found {} — every top-level line starts with its kind: `constant`, `function`, `record`, `variant`, `test \"…\"`, or `extern`",
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
                    cur.recover_to_next_decl();
                }
            }
        }
    }
}

/// The shared head of every named declaration: the kind keyword —
/// documentation attaches to it, since it opens the line — then the declared
/// name. Returns `None` (already recovered) when the name is missing.
pub(super) fn head(cur: &mut Cursor, src: &Source, example: &str) -> Option<(Span, Vec<Span>, Span)> {
    let keyword = cur.span();
    let doc = cur.take_docs(src, keyword);
    let kind = cur.found(src);
    cur.bump();
    if !cur.at(TokenKind::Ident) {
        if !cur.at_reported_error() {
            let message = format!(
                "expected the declared name after {kind}, found {} — `{example}`",
                cur.found(src)
            );
            cur.error("expected_name", message, cur.span());
        }
        cur.recover_to_next_decl();
        return None;
    }
    let name = cur.bump().span;
    Some((keyword, doc, name))
}

/// `constant MAX_DEPTH: int` + the value, indented (§4.2). Constants use
/// SCREAMING_CASE by convention; the parser does not police it.
fn constant(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    let Some((keyword, doc, name)) = head(cur, src, "constant MAX_DEPTH: int") else {
        return;
    };
    if !cur.expect(
        TokenKind::Colon,
        "expected_constant_type",
        "`:` and the constant's type — a `constant` declares a value, so it has one",
        src,
    ) {
        cur.recover_to_next_decl();
        return;
    }
    let ty = parse_type(cur, ast, src);
    let Some(body) = block(cur, ast, src, "a `constant`") else {
        cur.recover_to_next_decl();
        return;
    };
    let span = keyword.to(body.span);
    ast.decls.push(Decl { name, doc, span, kind: DeclKind::Constant { ty, body } });
}

/// `function dist2(a: Point, b: Point) -> int` + body (§4.2). The parameter
/// list attaches to the name, as at the call site.
fn function_decl(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    let Some((keyword, doc, name)) =
        head(cur, src, "function dist2(a: Point, b: Point) -> int")
    else {
        return;
    };
    function_tail(cur, ast, src, keyword, doc, name, false);
}

/// Everything after a function's name: generics, the signature, and — for a
/// non-extern — the body. Shared by `function` and `extern function`.
fn function_tail(
    cur: &mut Cursor,
    ast: &mut Ast,
    src: &Source,
    keyword: Span,
    doc: Vec<Span>,
    name: Span,
    is_extern: bool,
) {
    let generic_names = generics(cur, src);
    if !cur.at(TokenKind::LParen) {
        params(cur, ast, src); // reports the missing `(` with its own message
        cur.recover_to_next_decl();
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
            None => {
                cur.recover_to_next_decl();
                return;
            }
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
    ast.decls.push(Decl { name, doc, span: keyword.to(end), kind });
}

/// `extern function sqrt(x: f64) -> f64` (§4.19). Only a function can be
/// `extern`: the implementation comes from C, and C has functions. The
/// keyword carries the *kind* — when extern globals arrive with the FFI
/// milestone, they will spell theirs (panel 018 watch list).
fn extern_function(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    let keyword = cur.span();
    let doc = cur.take_docs(src, keyword);
    cur.bump(); // `extern`
    if !cur.at(TokenKind::KwFunction) {
        if !cur.at_reported_error() {
            let message = format!(
                "only a `function` can be `extern`, found {} — `extern function sqrt(x: f64) -> f64` (§4.19)",
                cur.found(src)
            );
            cur.error("extern_not_function", message, cur.span());
        }
        cur.recover_to_next_decl();
        return;
    }
    cur.bump(); // `function`
    if !cur.at(TokenKind::Ident) {
        if !cur.at_reported_error() {
            let message = format!(
                "expected the C function's name, found {} — `extern function sqrt(x: f64) -> f64`",
                cur.found(src)
            );
            cur.error("expected_name", message, cur.span());
        }
        cur.recover_to_next_decl();
        return;
    }
    let name = cur.bump().span;
    function_tail(cur, ast, src, keyword, doc, name, true);
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
        cur.recover_to_next_decl();
        return;
    }
    let name = cur.bump().span;
    let Some(body) = block(cur, ast, src, "a `test`") else {
        cur.recover_to_next_decl();
        return;
    };
    let span = keyword.to(body.span);
    ast.decls.push(Decl { name, doc, span, kind: DeclKind::Test { body } });
}
