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
            TokenKind::KwExtern => super::externs::group(cur, ast, src),
            TokenKind::KwTest => test(cur, ast, src),
            TokenKind::KwUse => super::uses::declaration(cur, ast, src),
            _ => {
                if !cur.at_reported_error() {
                    let message = format!(
                        "expected a declaration, found {} — every top-level line starts with its kind: `use`, `constant`, `function`, `record`, `variant`, `test \"…\"`, or `extern`",
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

/// `constant MAX_DEPTH: i64` + the value, indented (§4.2). Constants use
/// SCREAMING_CASE by convention; the parser does not police it.
fn constant(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    let Some((keyword, doc, name)) = head(cur, src, "constant MAX_DEPTH: i64") else {
        return;
    };
    constant_tail(cur, ast, src, keyword, doc, name, Linkage::Heroes);
}

/// Everything after a constant's name: its type, and — for a non-extern — the
/// indented value. Shared by `constant` and a group's members, exactly as
/// `function_tail` is (§4.2, §4.19).
///
/// The symmetry is the whole rule this form rests on: **inside a group a
/// declaration is a signature, not a definition**, so `constant` gives up its
/// body for the same reason `function` gives up its own — C has the thing.
pub(super) fn constant_tail(
    cur: &mut Cursor,
    ast: &mut Ast,
    src: &Source,
    keyword: Span,
    doc: Vec<Span>,
    name: Span,
    linkage: Linkage,
) {
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
    let (header, library) = match linkage {
        Linkage::Heroes => (None, None),
        Linkage::Extern { header, library } => (Some(header), library),
    };
    // An `extern` constant has no body, and an indented one under it is the
    // mistake `externs::body_check` names. Everything else keeps the old shape.
    let body = if header.is_some() {
        super::externs::body_check(cur, "constant");
        None
    } else {
        let Some(body) = block(cur, ast, src, "a `constant`") else {
            cur.recover_to_next_decl();
            return;
        };
        Some(body)
    };
    let span = match &body {
        Some(block) => keyword.to(block.span),
        None => keyword.to(cur.previous_span()),
    };
    ast.decls.push(Decl { name, doc, span, kind: DeclKind::Constant { ty, body, header, library } });
}

/// `function dist2(a: Point, b: Point) -> i64` + body (§4.2). The parameter
/// list attaches to the name, as at the call site.
fn function_decl(cur: &mut Cursor, ast: &mut Ast, src: &Source) {
    let Some((keyword, doc, name)) =
        head(cur, src, "function dist2(a: Point, b: Point) -> i64")
    else {
        return;
    };
    function_tail(cur, ast, src, keyword, doc, name, Linkage::Heroes);
}

/// Where a signature's implementation comes from, and — when it comes from C —
/// the two strings the group's head line carried (§4.19, panel 036).
///
/// A copy per member rather than a shared index: the group exists only in the
/// source text, and every later pass sees N ordinary declarations.
#[derive(Clone, Copy)]
pub(super) enum Linkage {
    Heroes,
    Extern { header: Span, library: Option<super::ast::Library> },
}

/// Everything after a function's name: generics, the signature, and — for a
/// non-extern — the body. Shared by `function` and a group's members.
pub(super) fn function_tail(
    cur: &mut Cursor,
    ast: &mut Ast,
    src: &Source,
    keyword: Span,
    doc: Vec<Span>,
    name: Span,
    linkage: Linkage,
) {
    let is_extern = matches!(linkage, Linkage::Extern { .. });
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
        super::externs::body_check(cur, "function");
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
        None => cur.previous_span(),
    };
    let (header, library) = match linkage {
        Linkage::Heroes => (None, None),
        Linkage::Extern { header, library } => (Some(header), library),
    };
    let kind = DeclKind::Function(Function {
        generics: generic_names,
        params: parameters,
        result,
        body,
        is_extern,
        header,
        library,
    });
    ast.decls.push(Decl { name, doc, span: keyword.to(end), kind });
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
