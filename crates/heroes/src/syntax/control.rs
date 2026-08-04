//! `if` and `match`, which are expressions (design.md §4.7), and the
//! patterns `match` destructures with.
//!
//! `match` is the only destructuring construct and the only conditional in
//! the core; `if` is sugar for a `match` on `bool` (Part 5). They live in one
//! file because they are the two forms whose *body* is a block, which is the
//! property that makes them awkward: an expression that spans lines and
//! carries indentation.
//!
//! Two rules the parser records but does not enforce — both are the
//! checker's at M3c, and both are the reason `match` exists:
//! exhaustiveness, and the ban on `_` as a whole arm over a variant (§4.7:
//! with `_` allowed, adding a case would stop breaking compilation, and
//! exhaustiveness becomes theatre).

use crate::lexer::TokenKind;
use crate::source::Source;

use super::ast::{Arm, ArmBody, Ast, Branch, Expr, ExprId, ExprKind, Pattern, PatternKind};
use super::cursor::Cursor;
use super::expr::expr;
use super::stmt::block;

/// `if cond` + block, then any number of `else if`, then an optional `else`.
/// No parentheses around the condition (§4.15) and no truthiness: the
/// condition must be `bool`, checked at M3.
pub(super) fn if_expr(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> ExprId {
    let start = cur.bump().span; // `if`
    let mut branches: Vec<Branch> = Vec::new();
    let cond = expr(cur, ast, src);
    let Some(first) = block(cur, ast, src, "an `if`") else {
        return ast.push_expr(Expr { kind: ExprKind::Error, span: start });
    };
    branches.push(Branch { cond, block: first });
    let mut otherwise = None;
    // `else` closes the `if`'s block, so the cursor is back at this level.
    while cur.at(TokenKind::KwElse) {
        cur.bump();
        if cur.eat(TokenKind::KwIf) {
            let cond = expr(cur, ast, src);
            let Some(more) = block(cur, ast, src, "an `else if`") else { break };
            branches.push(Branch { cond, block: more });
            continue;
        }
        let Some(last) = block(cur, ast, src, "an `else`") else { break };
        otherwise = Some(last);
        break;
    }
    let end = otherwise
        .as_ref()
        .map_or_else(|| branches[branches.len() - 1].block.span, |block| block.span);
    ast.push_expr(Expr { kind: ExprKind::If { branches, otherwise }, span: start.to(end) })
}

/// `match e` + one arm per line, indented.
pub(super) fn match_expr(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> ExprId {
    let start = cur.bump().span; // `match`
    let scrutinee = expr(cur, ast, src);
    cur.skip_terminators();
    if !cur.at(TokenKind::Indent) {
        if !cur.at_reported_error() {
            let message = format!(
                "a `match` needs its arms indented one level below it, found {}",
                cur.found(src)
            );
            cur.error("missing_match_arms", message, cur.span());
        }
        return ast.push_expr(Expr { kind: ExprKind::Error, span: start });
    }
    cur.bump(); // the Indent
    let mut arms: Vec<Arm> = Vec::new();
    loop {
        cur.skip_terminators();
        match cur.kind() {
            TokenKind::Dedent => {
                cur.bump();
                break;
            }
            TokenKind::Eof => break,
            // A block where an arm should start: it belongs to an arm that
            // failed, so it goes whole. Without this case the loop can spin
            // forever — `skip_line` deliberately stops *before* an `Indent`
            // (it must not leave the block it is cleaning), so a failed arm
            // followed by an indented body consumed nothing at all and the
            // parser never terminated. Found by the compiler-engineer while
            // costing panel 014, on `1 => for x in xs` + its body.
            TokenKind::Indent => {
                cur.balanced_block();
            }
            _ => {
                let reported_before = cur.diagnostic_count();
                if let Some(arm) = arm(cur, ast, src) {
                    let failed = cur.diagnostic_count() > reported_before;
                    arms.push(arm);
                    if failed {
                        // The rest of the line is debris, not a second arm:
                        // this is what turned one `=> assert false` into
                        // `expected_expression` *and* a spurious
                        // `expected_pattern`.
                        cur.skip_line();
                    }
                } else {
                    cur.skip_line();
                }
            }
        }
    }
    let span = start.to(cur.previous_span());
    ast.push_expr(Expr { kind: ExprKind::Match { scrutinee, arms }, span })
}

/// One arm: patterns joined by `|`, `=>`, then an expression on the same
/// line or a block below it (§4.7).
fn arm(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> Option<Arm> {
    let start = cur.span();
    let mut patterns = vec![pattern(cur, ast, src)?];
    while cur.eat(TokenKind::Pipe) {
        patterns.push(pattern(cur, ast, src)?);
    }
    if !cur.expect(
        TokenKind::FatArrow,
        "expected_arm_arrow",
        "`=>` and the arm's value",
        src,
    ) {
        return None;
    }
    cur.skip_terminators();
    let body = if cur.at(TokenKind::Indent) {
        ArmBody::Block(block(cur, ast, src, "a `match` arm")?)
    } else {
        ArmBody::Expr(expr(cur, ast, src))
    };
    Some(Arm { patterns, body, span: start.to(cur.previous_span()) })
}

/// `.num n`, `.num _`, `.plus`, `_`, or a literal.
///
/// The payload name is optional and may be `_`: `.num _ => 0` is legal,
/// `_ => 0` on a variant is not (§4.7). That distinction is not obvious,
/// which is why the spec states it and the tree keeps the two apart.
fn pattern(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> Option<Pattern> {
    let start = cur.span();
    match cur.kind() {
        TokenKind::Dot => {
            cur.bump();
            if !cur.at(TokenKind::Ident) {
                if !cur.at_reported_error() {
                    let message = format!(
                        "expected a case name after `.`, found {} — an arm is `.name => value`",
                        cur.found(src)
                    );
                    cur.error("expected_pattern_case", message, cur.span());
                }
                return None;
            }
            let name = cur.bump().span;
            // A name here binds the payload; `_` binds it and says it is
            // unused, which is what keeps §4.4's unused-binding rule honest.
            let binding = if cur.at(TokenKind::Ident) {
                Some(cur.bump().span)
            } else {
                None
            };
            Some(Pattern { kind: PatternKind::Case { name, binding }, span: start.to(cur.previous_span()) })
        }
        // `_` lexes as an identifier: it is the wildcard only here.
        TokenKind::Ident if src.slice(cur.span()) == "_" => {
            cur.bump();
            Some(Pattern { kind: PatternKind::Wildcard, span: start })
        }
        TokenKind::Int | TokenKind::Str | TokenKind::Char | TokenKind::Minus => {
            let value = expr(cur, ast, src);
            Some(Pattern { kind: PatternKind::Literal(value), span: start.to(cur.previous_span()) })
        }
        _ => {
            if !cur.at_reported_error() {
                let message = format!(
                    "expected a pattern, found {} — `.case`, `.case name`, a literal, or `_` (on `int`/`str` only)",
                    cur.found(src)
                );
                cur.error("expected_pattern", message, cur.span());
            }
            None
        }
    }
}
