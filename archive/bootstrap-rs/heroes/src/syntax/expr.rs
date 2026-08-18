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
        // **The bitwise three sit between the comparisons and the shifts, each on
        // its own level** — C's order, which Go, Rust, Java and JavaScript all
        // kept. C's own mistake here is the *other* one: `&` binding looser than
        // `==`, so `x & 1 == 0` means `x & (1 == 0)`. Heroes does not inherit it,
        // because `&` is tighter than `==` in this table. Each operator gets a
        // level of its own rather than sharing one, which is what makes
        // `a | b ^ c & d` parse the way every other language parses it.
        TokenKind::Pipe => (BinaryOp::BitOr, 4),
        TokenKind::Caret => (BinaryOp::BitXor, 5),
        TokenKind::Amp => (BinaryOp::BitAnd, 6),
        TokenKind::Shl => (BinaryOp::Shl, 7),
        TokenKind::Shr => (BinaryOp::Shr, 7),
        TokenKind::Plus => (BinaryOp::Add, 8),
        TokenKind::Minus => (BinaryOp::Sub, 8),
        TokenKind::Star => (BinaryOp::Mul, 9),
        TokenKind::Slash => (BinaryOp::Div, 9),
        TokenKind::Percent => (BinaryOp::Rem, 9),
        _ => return None,
    };
    Some((op, power))
}

/// A pattern's operand: a literal, or a literal with a leading `-`.
///
/// **Deliberately not `expr`**, and the reason is a premise that expired silently.
/// Until the bitwise set landed on 2026-08-12, no binary operator could follow a
/// literal in pattern position, so parsing a pattern with the full expression
/// parser was safe and nobody wrote the premise down. `|` can follow one — it is
/// the pattern join (§4.7) — and `1 | 2 => "small"` quietly became the expression
/// `3`, so `name(1)` matched nothing and fell through to `_`. A silent wrong
/// answer, which is the class this language spends tokens to avoid.
///
/// Calling `unary` rather than `binary` is what makes the join unreachable from
/// here: no binary operator is ever consumed in a pattern, whatever the table
/// grows. `a_pattern_join_is_not_bitwise_or` is the test that fires if this is
/// ever widened back.
pub(super) fn pattern_operand(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> ExprId {
    unary(cur, ast, src)
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
/// `!` accepts only `bool`, which is a type judgment at M-typed-frontend (§4.14: there is
/// no truthiness).
fn unary(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> ExprId {
    let op = match cur.kind() {
        TokenKind::Minus => UnaryOp::Neg,
        TokenKind::Bang => UnaryOp::Not,
        TokenKind::Tilde => UnaryOp::BitNot,
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
    // §4.7 does not have. Carried open since M-ir-lowering; five lines
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
                    // **`previous_significant_span`, and this is that repair's
                    // adjacent shape** (CLAUDE.md §1's fourth rule). The cursor
                    // doc one file over describes the defect and dates it: built
                    // from `previous_span` a span reaches the line's
                    // `Terminator`, which sits past any trailing comment, so
                    // `f() # a note` was underlined twenty-three columns wide for
                    // a three-column statement. That was repaired for
                    // **statements** on 2026-08-12 and left here, so a call and a
                    // method kept swallowing the comment: measured 2026-08-17,
                    // `sort(ps)` underlined 46 columns for 8, on six of the
                    // `check/` goldens which had pinned the wrong width.
                    //
                    // Found by the SELFHOST PORT, which computes it correctly —
                    // the differential over 257 corpus inputs is the instrument,
                    // and CLAUDE.md §12's rule about which artifact wins applies
                    // to two compilers as well as to a document: §4.17 says the
                    // caret is the span, and a caret over a comment points at
                    // something the author cannot fix.
                    let span = start.to(cur.previous_significant_span());
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
                // The same repair as the method above, and the same reason.
                let span = start.to(cur.previous_significant_span());
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
            // both are checked at M-typed-frontend.
            TokenKind::Question => {
                let question = cur.bump().span;
                let span = ast.exprs[base.0 as usize].span.to(question);
                base = ast.push_expr(super::ast::Expr { kind: ExprKind::Try(base), span });
            }
            _ => return base,
        }
    }
}
