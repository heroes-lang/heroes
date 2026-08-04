//! The type grammar (design.md §4.3 the type table, §4.6 `T?`, §4.13
//! function types).
//!
//! Six shapes and no ambiguity, because each one is decided by its first
//! token: a name, `[`, `{`, or `(`. The last is the interesting one — `()`
//! is "no value" and `(function(A) -> B)` is a function type, so one
//! lookahead separates them.
//!
//! Only `?` is postfix, and it is applied exactly once: `T??` is the level
//! ambiguity §4.6 refuses to have in the language, so the parser refuses it
//! in the surface.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::lexer::TokenKind;
use crate::source::Source;

use super::ast::{Ast, TypeId, TypeKind, TypeNode};
use super::cursor::Cursor;

/// Read one type. Always returns a node: on failure it is `TypeKind::Error`,
/// so the tree keeps its shape and the errors after this one still get found.
pub(super) fn parse_type(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> TypeId {
    let start = cur.span();
    let inner = prefix(cur, ast, src);
    if !cur.at(TokenKind::Question) {
        return inner;
    }
    let question = cur.bump().span;
    while cur.at(TokenKind::Question) {
        let extra = cur.bump().span;
        cur.error(
            "nested_fallible",
            "a fallible type cannot be fallible twice — `T?` already carries the error case (§4.6)"
                .to_string(),
            extra,
        );
    }
    ast.push_type(TypeNode { kind: TypeKind::Fallible(inner), span: start.to(question) })
}

fn prefix(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> TypeId {
    match cur.kind() {
        TokenKind::Ident => {
            let span = cur.bump().span;
            ast.push_type(TypeNode { kind: TypeKind::Named, span })
        }
        TokenKind::LBracket => array(cur, ast, src),
        TokenKind::LBrace => map(cur, ast, src),
        TokenKind::LParen => unit_or_function(cur, ast, src),
        _ => {
            if !cur.at_reported_error() {
                let message = format!(
                    "expected a type, found {} — a type is `int`, `f64`, `bool`, `str`, `[T]`, `{{K: V}}`, `T?`, `()`, or the name of a `record` or `variant`",
                    cur.found(src)
                );
                cur.error("expected_type", message, cur.span());
            }
            // Not consumed: the caller's recovery decides where the line ends.
            ast.push_type(TypeNode { kind: TypeKind::Error, span: cur.span() })
        }
    }
}

/// `[T]` — the dynamic array, and Heroes' only indirection (§4.10).
fn array(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> TypeId {
    let open = cur.bump().span;
    let elem = parse_type(cur, ast, src);
    let close = cur.span();
    cur.expect(
        TokenKind::RBracket,
        "expected_array_close",
        "`]` to close the array type",
        src,
    );
    ast.push_type(TypeNode { kind: TypeKind::Array(elem), span: open.to(close) })
}

/// `{K: V}` — the map. Access returns `V?`, always (§4.9).
fn map(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> TypeId {
    let open = cur.bump().span;
    let key = parse_type(cur, ast, src);
    cur.expect(
        TokenKind::Colon,
        "expected_map_colon",
        "`:` between the map's key and value types",
        src,
    );
    let value = parse_type(cur, ast, src);
    let close = cur.span();
    cur.expect(TokenKind::RBrace, "expected_map_close", "`}` to close the map type", src);
    ast.push_type(TypeNode { kind: TypeKind::Map(key, value), span: open.to(close) })
}

/// `()` or `(function(A, B) -> C)`.
///
/// The marker is the same word that declares a function — one word, one
/// meaning, everywhere (panel 013). The parentheses are mandatory because
/// declarations dropped theirs: `f: function A -> B -> [B]` would be
/// ambiguous (§4.13).
fn unit_or_function(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> TypeId {
    let open = cur.bump().span;
    if cur.at(TokenKind::RParen) {
        let close = cur.bump().span;
        return ast.push_type(TypeNode { kind: TypeKind::Unit, span: open.to(close) });
    }
    if !cur.expect(
        TokenKind::KwFunction,
        "expected_function_type",
        "`function` — in a type, `(` opens either `()` or a function type, `(function(A) -> B)`",
        src,
    ) {
        return ast.push_type(TypeNode { kind: TypeKind::Error, span: open });
    }
    let params = function_params(cur, ast, src);
    cur.expect(
        TokenKind::Arrow,
        "expected_function_type_arrow",
        "`->` and the result type — a function type always states what it returns, `()` when nothing",
        src,
    );
    let result = parse_type(cur, ast, src);
    let close = cur.span();
    cur.expect(
        TokenKind::RParen,
        "expected_function_type_close",
        "`)` to close the function type",
        src,
    );
    ast.push_type(TypeNode { kind: TypeKind::Func { params, result }, span: open.to(close) })
}

/// The parameter *types* of a function type: `(int, int)`, or `()` for a
/// function of no arguments. Names belong to a signature, not to a type —
/// writing one is a plausible mistake, so it gets its own diagnostic and a
/// machine-applicable repair.
fn function_params(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> Vec<TypeId> {
    let mut params: Vec<TypeId> = Vec::new();
    if !cur.expect(
        TokenKind::LParen,
        "expected_function_type_params",
        "`(` and the parameter types",
        src,
    ) {
        return params;
    }
    if cur.eat(TokenKind::RParen) {
        return params;
    }
    loop {
        let ty = parse_type(cur, ast, src);
        if cur.at(TokenKind::Colon) {
            named_parameter(cur, ast, ty);
            continue;
        }
        params.push(ty);
        if !cur.eat(TokenKind::Comma) {
            break;
        }
    }
    if !cur.expect(
        TokenKind::RParen,
        "expected_function_type_params_close",
        "`)`, or `,` and another parameter type",
        src,
    ) {
        cur.recover_past_closer(TokenKind::LParen, TokenKind::RParen);
    }
    params
}

/// `(function(x: int) -> int)` — the name that slipped in from the signature
/// it was copied from. The fix is `certain`: drop the name and the colon.
fn named_parameter(cur: &mut Cursor, ast: &Ast, name: TypeId) {
    let name_span = ast.types[name.0 as usize].span;
    let colon = cur.bump().span;
    let mut diag = Diagnostic::new(
        "named_parameter_in_function_type",
        "a function type lists types, not names — write `(function(int) -> int)`".to_string(),
        name_span.to(colon),
    );
    diag.fixes.push(Fix {
        title: "drop the parameter name".to_string(),
        replacement: String::new(),
        span: name_span.to(colon),
        certainty: Certainty::Certain,
    });
    cur.push_diagnostic(diag);
}
