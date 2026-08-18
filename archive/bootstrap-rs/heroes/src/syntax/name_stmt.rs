//! The three line shapes that begin with a name (design.md §4.4, §4.5; spec
//! § Bindings).
//!
//! Split out of `stmt.rs` by the §11 sweep, and it is the difficulty that file's
//! own doc opens with. Three statements start with an identifier and one token of
//! lookahead tells them apart — exactly as §4.4 designed it, so that the shape of a
//! line is decidable without scanning the block:
//!
//! ```text
//! x = 5            name  =        immutable binding
//! v: i64 @ 0       name  :        declaration (the type is mandatory)
//! v @ v + 1        place @        mutation
//! ```
//!
//! The third line is why the type is mandatory on a mutable declaration: without
//! it, `v @ 0` and `v @ v + 1` would be the same shape, and whether a name was
//! being declared or written would depend on a declaration somewhere else in the
//! file — which is §1.3's locality test failing.
//!
//! A mutation's left side is a **place**, not any expression, and `is_place` is
//! where that is decided: §4.8's rule is that every place has exactly one root,
//! reached through fields and indices only.

use crate::lexer::TokenKind;
use crate::source::Source;

use super::ast::{Ast, ExprId, ExprKind, StmtKind};
use super::cursor::Cursor;
use super::expr::expr;
use super::types::parse_type;

/// `x = 5` — binds once, forever (§4.4). The type is inferred, and inference
/// is local: the value on this line decides it (§4.5).
pub(super) fn bind(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> StmtKind {
    let name = cur.bump().span;
    cur.bump(); // `=`
    StmtKind::Bind { name, ty: None, value: expr(cur, ast, src) }
}

/// `v: i64 @ 0` — a mutable cell, type mandatory. `xs: [i64] = []` — an
/// immutable binding that needs its annotation because an empty literal
/// cannot say what it holds (§4.5).
pub(super) fn annotated(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> StmtKind {
    let name = cur.bump().span;
    cur.bump(); // `:`
    let ty = parse_type(cur, ast, src);
    if cur.eat(TokenKind::At) {
        return StmtKind::Declare { name, ty, value: expr(cur, ast, src) };
    }
    if cur.eat(TokenKind::Eq) {
        return StmtKind::Bind { name, ty: Some(ty), value: expr(cur, ast, src) };
    }
    if !cur.at_reported_error() {
        let message = format!(
            "expected `@` to declare a mutable, or `=` to bind, found {} — `v: i64 @ 0` declares a cell, `xs: [i64] = []` binds once",
            cur.found(src)
        );
        cur.error("expected_binding_symbol", message, cur.span());
    }
    StmtKind::Error
}

/// Everything else: an expression alone on its line, or the place on the
/// left of an `@`.
pub(super) fn expression_or_mutation(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> StmtKind {
    let value = expr(cur, ast, src);
    if !cur.eat(TokenKind::At) {
        return StmtKind::Expr(value);
    }
    if !is_place(ast, value) {
        let span = ast.exprs[value.0 as usize].span;
        cur.error(
            "not_a_place",
            "only a name, a field or an element can be mutated — the left of `@` must name where the value goes"
                .to_string(),
            span,
        );
    }
    StmtKind::Mutate { place: value, value: expr(cur, ast, src) }
}

/// A *place*: a name, or a field or index path rooted at one. Since Heroes
/// has no references, every place has exactly one root — which is what makes
/// panel 010's alias test a comparison of roots rather than a dataflow
/// analysis.
fn is_place(ast: &Ast, id: ExprId) -> bool {
    match &ast.exprs[id.0 as usize].kind {
        ExprKind::Name => true,
        ExprKind::Field { base, .. } => is_place(ast, *base),
        ExprKind::Index { base, .. } => is_place(ast, *base),
        _ => false,
    }
}

