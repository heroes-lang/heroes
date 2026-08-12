//! Expressions: the precedence table, and everything that binds tighter
//! than it (design.md §4.14 operators, §4.11 UFCS, §4.6 `?`).
//!
//! Precedence climbing, one level per line of the §4.14 table:
//!
//! ```text
//! ||                       weakest
//! &&
//! == != < <= > >=
//! + -
//! * / %
//! unary - !
//! call · . · [ ] · ?        strongest
//! ```
//!
//! Two properties of the table are worth stating because they are choices,
//! not accidents: comparisons sit *above* `&&`, so `a < b && c < d` needs no
//! parentheses; and `?` binds tightest, so `f(x)?.field` reads left to
//! right and `m[k].default(0) + 1` cannot be misread (§4.6's argument for a
//! method over a `??` operator).

use crate::lexer::TokenKind;
use crate::source::Source;

use super::ast::{Ast, BinaryOp, ExprId, ExprKind, UnaryOp};
use super::cursor::Cursor;
use super::primary::primary;

/// Read one expression, weakest binding first.
pub(super) fn expr(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> ExprId {
    binary(cur, ast, src, 0)
}

/// The binding power of each operator, or `None` when the token is not one.
/// Higher binds tighter; the table *is* §4.14, read from the bottom up.
fn binary_op(kind: TokenKind) -> Option<(BinaryOp, u8)> {
    let (op, power) = match kind {
        TokenKind::OrOr => (BinaryOp::Or, 1),
        TokenKind::AndAnd => (BinaryOp::And, 2),
        TokenKind::EqEq => (BinaryOp::Eq, 3),
        TokenKind::BangEq => (BinaryOp::Ne, 3),
        TokenKind::Lt => (BinaryOp::Lt, 3),
        TokenKind::Le => (BinaryOp::Le, 3),
        TokenKind::Gt => (BinaryOp::Gt, 3),
        TokenKind::Ge => (BinaryOp::Ge, 3),
        TokenKind::Plus => (BinaryOp::Add, 4),
        TokenKind::Minus => (BinaryOp::Sub, 4),
        TokenKind::Star => (BinaryOp::Mul, 5),
        TokenKind::Slash => (BinaryOp::Div, 5),
        TokenKind::Percent => (BinaryOp::Rem, 5),
        _ => return None,
    };
    Some((op, power))
}

/// Everything at `min_power` or tighter. All of Heroes' binary operators are
/// left-associative, so the recursive call asks for `power + 1`.
fn binary(cur: &mut Cursor, ast: &mut Ast, src: &Source, min_power: u8) -> ExprId {
    let mut left = unary(cur, ast, src);
    while let Some((op, power)) = binary_op(cur.kind()) {
        if power < min_power {
            break;
        }
        cur.bump();
        let right = binary(cur, ast, src, power + 1);
        let span = ast.exprs[left.0 as usize].span.to(ast.exprs[right.0 as usize].span);
        left = ast.push_expr(super::ast::Expr {
            kind: ExprKind::Binary { op, left, right },
            span,
        });
    }
    left
}

/// `-x` and `!x`. Both take exactly one operand and neither is overloadable;
/// `!` accepts only `bool`, which is a type judgment at M3 (§4.14: there is
/// no truthiness).
fn unary(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> ExprId {
    let op = match cur.kind() {
        TokenKind::Minus => UnaryOp::Neg,
        TokenKind::Bang => UnaryOp::Not,
        _ => return postfix(cur, ast, src),
    };
    let start = cur.bump().span;
    let operand = unary(cur, ast, src);
    let span = start.to(ast.exprs[operand.0 as usize].span);
    ast.push_expr(super::ast::Expr { kind: ExprKind::Unary { op, operand }, span })
}

/// The tightest level, and the only one that loops on *suffixes*: a field, a
/// call, an index, an error propagation — chained in whatever order they
/// were written, which is what makes `l.text.slice(from: a, to: b)?[0]`
/// read left to right.
fn postfix(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> ExprId {
    let mut base = primary(cur, ast, src);
    // **A suffix never reaches across a block that just closed.**
    //
    // `.ok v => match v` with the inner arms indented under it: the inner
    // `match` consumes its own arms and its `Dedent`, and then this loop saw the
    // `.` of the *next outer arm* and read it as a field. The error landed on
    // `_` of `.err _ =>` — the following arm, a line the author did nothing
    // wrong on — while the real message is that the body was written in a shape
    // §4.7 does not have. Carried open since M4; five lines
    // (panel 035, compiler-engineer, who prototyped it).
    let closed_a_block = cur.previous_kind() == TokenKind::Dedent;
    loop {
        if closed_a_block && cur.at(TokenKind::Dot) {
            return base;
        }
        match cur.kind() {
            // `x.f(y)` is UFCS, `x.f` is a field read: the `(` decides
            // (§4.11).
            TokenKind::Dot => {
                cur.bump();
                if !cur.at(TokenKind::Ident) {
                    if !cur.at_reported_error() {
                        let message = format!(
                            "expected a field or function name after `.`, found {}",
                            cur.found(src)
                        );
                        cur.error("expected_field_name", message, cur.span());
                    }
                    return base;
                }
                let name = cur.bump().span;
                let start = ast.exprs[base.0 as usize].span;
                base = if cur.at(TokenKind::LParen) {
                    let args = super::primary::call_args(cur, ast, src);
                    let span = start.to(cur.previous_span());
                    ast.push_expr(super::ast::Expr {
                        kind: ExprKind::Method { receiver: base, name, args },
                        span,
                    })
                } else {
                    ast.push_expr(super::ast::Expr {
                        kind: ExprKind::Field { base, name },
                        span: start.to(name),
                    })
                };
            }
            TokenKind::LParen => {
                let start = ast.exprs[base.0 as usize].span;
                let args = super::primary::call_args(cur, ast, src);
                let span = start.to(cur.previous_span());
                base = ast.push_expr(super::ast::Expr {
                    kind: ExprKind::Call { callee: base, args },
                    span,
                });
            }
            TokenKind::LBracket => {
                cur.bump();
                let index = expr(cur, ast, src);
                let close = cur.span();
                cur.expect(
                    TokenKind::RBracket,
                    "expected_index_close",
                    "`]` to close the index",
                    src,
                );
                let span = ast.exprs[base.0 as usize].span.to(close);
                base = ast.push_expr(super::ast::Expr {
                    kind: ExprKind::Index { base, index },
                    span,
                });
            }
            // `e?` — propagate. The caller's return type must be fallible,
            // and `?` on a non-fallible value is a compile error (§4.6):
            // both are checked at M3.
            TokenKind::Question => {
                let question = cur.bump().span;
                let span = ast.exprs[base.0 as usize].span.to(question);
                base = ast.push_expr(super::ast::Expr { kind: ExprKind::Try(base), span });
            }
            _ => return base,
        }
    }
}
