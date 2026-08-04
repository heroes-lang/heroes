//! The atoms of an expression, and the two bracketed literals (design.md
//! §4.3 literals, §4.9 calls and containers, §4.5 leading-dot variants,
//! §4.16 `???`).
//!
//! One rule shows up three times here and is worth naming once: **inside
//! brackets, a newline is a separator, not a statement end.** The lexer
//! keeps planting terminators at line ends even inside `(` `[` `{` (panel
//! 007), and §4.9 makes newline the separator of a multi-line literal — so
//! every list in this file skips terminators, and the array and map literals
//! accept a newline *as* the comma.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::lexer::TokenKind;
use crate::source::{Source, Span};

use super::ast::{Arg, Ast, Expr, ExprId, ExprKind, MapEntry};
use super::control::{if_expr, match_expr};
use super::cursor::Cursor;
use super::expr::expr;

pub(super) fn primary(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> ExprId {
    let span = cur.span();
    let kind = match cur.kind() {
        TokenKind::Int => ExprKind::Int,
        TokenKind::Float => ExprKind::Float,
        TokenKind::Str => ExprKind::Str,
        TokenKind::Char => ExprKind::Char,
        TokenKind::KwTrue | TokenKind::KwFalse => ExprKind::Bool,
        TokenKind::Hole => ExprKind::Hole,
        // `fail` is a name like any other here: `fail(code, msg)` is a call,
        // and the compiler knows the callee (§4.6). Keeping it out of the
        // grammar keeps the grammar smaller.
        TokenKind::Ident | TokenKind::KwFail => ExprKind::Name,
        TokenKind::LParen => return group(cur, ast, src),
        TokenKind::LBracket => return array(cur, ast, src),
        TokenKind::LBrace => return map(cur, ast, src),
        TokenKind::Dot => return case(cur, ast, src),
        TokenKind::KwIf => return if_expr(cur, ast, src),
        TokenKind::KwMatch => return match_expr(cur, ast, src),
        _ => {
            if !cur.at_reported_error() {
                let message = format!(
                    "expected an expression, found {} — a value, a name, a call, `[`, `{{`, `.case`, `if`, `match`, or `???`",
                    cur.found(src)
                );
                cur.error("expected_expression", message, span);
            }
            // Not consumed: the statement parser decides where the line ends.
            return ast.push_expr(Expr { kind: ExprKind::Error, span });
        }
    };
    cur.bump();
    ast.push_expr(Expr { kind, span })
}

/// `(e)` — grouping only. There is no tuple type, so a parenthesised
/// expression is exactly its contents; `()` as a *value* does not exist
/// either (§4.3: `()` is the type of no value).
fn group(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> ExprId {
    cur.bump(); // `(`
    cur.skip_terminators();
    let inner = expr(cur, ast, src);
    cur.skip_terminators();
    if !cur.expect(TokenKind::RParen, "expected_group_close", "`)` to close the group", src) {
        cur.recover_past_closer(TokenKind::LParen, TokenKind::RParen);
    }
    inner
}

/// `[1, 2, 3]`, `[]`, or one element per line (§4.9). The canonical
/// formatter picks between the two by length, so the writer never chooses.
fn array(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> ExprId {
    let open = cur.bump().span;
    let mut items: Vec<ExprId> = Vec::new();
    loop {
        cur.skip_terminators();
        if cur.at(TokenKind::RBracket) || cur.at(TokenKind::Eof) {
            break;
        }
        items.push(expr(cur, ast, src));
        if !separator(cur, src, TokenKind::RBracket, "element") {
            break;
        }
    }
    let close = cur.span();
    if !cur.expect(TokenKind::RBracket, "expected_array_close", "`]` to close the list", src) {
        cur.recover_past_closer(TokenKind::LBracket, TokenKind::RBracket);
    }
    ast.push_expr(Expr { kind: ExprKind::Array(items), span: open.to(close) })
}

/// `{ "mario": 30 }`, `{}`, or one entry per line.
fn map(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> ExprId {
    let open = cur.bump().span;
    let mut entries: Vec<MapEntry> = Vec::new();
    loop {
        cur.skip_terminators();
        if cur.at(TokenKind::RBrace) || cur.at(TokenKind::Eof) {
            break;
        }
        let key = expr(cur, ast, src);
        cur.expect(
            TokenKind::Colon,
            "expected_map_entry_colon",
            "`:` between a map's key and its value",
            src,
        );
        let value = expr(cur, ast, src);
        entries.push(MapEntry { key, value });
        if !separator(cur, src, TokenKind::RBrace, "entry") {
            break;
        }
    }
    let close = cur.span();
    if !cur.expect(TokenKind::RBrace, "expected_map_close", "`}` to close the map", src) {
        cur.recover_past_closer(TokenKind::LBrace, TokenKind::RBrace);
    }
    ast.push_expr(Expr { kind: ExprKind::Map(entries), span: open.to(close) })
}

/// The separator between elements of a bracketed literal: a comma on one
/// line, a newline across several (§4.9). Returns false when the list is
/// over — either properly, at the closer, or because the separator is
/// missing, which is reported here.
fn separator(cur: &mut Cursor, src: &Source, closer: TokenKind, what: &str) -> bool {
    let comma_span = cur.span();
    let comma = cur.eat(TokenKind::Comma);
    let newline = cur.at(TokenKind::Terminator);
    cur.skip_terminators();
    if cur.at(closer) || cur.at(TokenKind::Eof) {
        if comma {
            trailing_comma(cur, comma_span);
        }
        return false;
    }
    if comma || newline {
        return true;
    }
    if !cur.at_reported_error() {
        let message = format!(
            "expected `,` or a new line between one {what} and the next, found {}",
            cur.found(src)
        );
        cur.error("expected_separator", message, cur.span());
    }
    false
}

/// `.plus`, `.num(v: 12)` — a variant case whose type comes from context
/// (§4.5's ⇐ mode). Leading dot, because writing `Token.num` would repeat
/// what the context already knows.
fn case(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> ExprId {
    let dot = cur.bump().span;
    if !cur.at(TokenKind::Ident) {
        if !cur.at_reported_error() {
            let message = format!(
                "expected a variant case name after `.`, found {} — `.plus`, or `.num(v: 12)` with its fields",
                cur.found(src)
            );
            cur.error("expected_case_name", message, cur.span());
        }
        return ast.push_expr(Expr { kind: ExprKind::Error, span: dot });
    }
    let name = cur.bump().span;
    let args = if cur.at(TokenKind::LParen) {
        call_args(cur, ast, src)
    } else {
        Vec::new()
    };
    ast.push_expr(Expr { kind: ExprKind::Case { name, args }, span: dot.to(cur.previous_span()) })
}

/// `(x, name: y, @l)` — the argument list of a call, of a record
/// construction, and of a variant case, which are the same thing (§4.9).
pub(super) fn call_args(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> Vec<Arg> {
    let mut args: Vec<Arg> = Vec::new();
    cur.bump(); // `(`
    cur.skip_terminators();
    if cur.eat(TokenKind::RParen) {
        return args;
    }
    loop {
        cur.skip_terminators();
        if cur.at(TokenKind::RParen) {
            // Reached only after a comma: `f(a, b,)`.
            trailing_comma(cur, cur.previous_span());
            break;
        }
        let name = if cur.at(TokenKind::Ident) && cur.peek(1) == TokenKind::Colon {
            let name = cur.bump().span;
            cur.bump(); // `:`
            Some(name)
        } else {
            None
        };
        let mutable = cur.eat(TokenKind::At);
        let value = expr(cur, ast, src);
        if cur.at(TokenKind::At) {
            // `f(a @ n)`: recovered as the `f(a: @n)` it meant, so the
            // mistake costs one diagnostic and the tree stays usable.
            let named = ast.exprs[value.0 as usize].span;
            misplaced_mutable_marker(cur);
            let value = expr(cur, ast, src);
            args.push(Arg { name: Some(named), mutable: true, value });
        } else {
            args.push(Arg { name, mutable, value });
        }
        cur.skip_terminators();
        if !cur.eat(TokenKind::Comma) {
            break;
        }
    }
    cur.skip_terminators();
    if !cur.expect(
        TokenKind::RParen,
        "expected_args_close",
        "`)`, or `,` and another argument",
        src,
    ) {
        cur.recover_past_closer(TokenKind::LParen, TokenKind::RParen);
    }
    args
}

/// `f(a, b,)` — the Python and JavaScript habit. Heroes has one spelling per
/// program (§4.15), so the trailing comma cannot also be it; the fix is
/// `certain` because deleting it is the whole repair.
fn trailing_comma(cur: &mut Cursor, comma: Span) {
    let mut diag = Diagnostic::new(
        "trailing_comma",
        "a trailing comma is not part of the list — write `f(a, b)`, `[1, 2]`".to_string(),
        comma,
    );
    diag.fixes.push(Fix {
        title: "delete the trailing comma".to_string(),
        replacement: String::new(),
        span: comma,
        certainty: Certainty::Certain,
    });
    cur.push_diagnostic(diag);
}

/// `f(a @ n)` — `@` where the `:` of a named argument belongs. The two rules
/// compose the other way round: `:` names the parameter (§4.9), `@` marks
/// the argument (§4.8), so it is `f(a: @n)`. The fix is `certain`.
fn misplaced_mutable_marker(cur: &mut Cursor) {
    let at = cur.span();
    let mut diag = Diagnostic::new(
        "misplaced_mutable_marker",
        "a named mutable argument is written `name: @value` — `:` names the parameter, `@` marks the argument"
            .to_string(),
        at,
    );
    diag.fixes.push(Fix {
        title: "write `: @` instead of `@`".to_string(),
        replacement: ": @".to_string(),
        span: at,
        certainty: Certainty::Certain,
    });
    cur.push_diagnostic(diag);
    cur.bump(); // the `@`
}
