//! Statements: what binds, what reads, what writes (§4.4 the three line
//! shapes, §4.7 loops).
//!
//! One rule decides the order of every branch in this file: **the value is
//! resolved before the name is bound.** `x = x + 1` must say that the `x` on
//! the right is not defined, not resolve it to the binding being created on
//! the same line. Bindings become visible after their own statement and stay
//! visible to the end of their block, which is what makes `for` and `match`
//! open scopes of their own.
//!
//! A block is a scope. A `for x in xs` binds `x` in a scope that also holds the
//! block, so the loop variable is not visible after the loop — and two loops
//! in one function may both call their variable `x`.

use crate::source::Source;
use crate::syntax::{Ast, Block, StmtId, StmtKind};

use super::scope::Binding;
use super::{exprs, types, Resolver};

/// A block, in its own scope.
pub(super) fn block(r: &mut Resolver, ast: &Ast, src: &Source, block: &Block) {
    r.open_scope();
    statements(r, ast, src, &block.stmts);
    r.close_scope();
}

/// The statements of a block, in a scope the caller has already opened —
/// which is how a loop variable and an arm's payload end up in the same scope
/// as the body they belong to.
pub(super) fn statements(r: &mut Resolver, ast: &Ast, src: &Source, stmts: &[StmtId]) {
    for id in stmts {
        statement(r, ast, src, *id);
    }
}

pub(super) fn statement(r: &mut Resolver, ast: &Ast, src: &Source, id: StmtId) {
    match &ast.stmts[id.0 as usize].kind {
        StmtKind::Bind { name, ty, value } => {
            exprs::expr(r, ast, src, *value);
            if let Some(ty) = ty {
                types::resolve(r, ast, src, *ty);
            }
            r.declare(ast, src, *name, Binding::bind(*ty, *value));
        }
        StmtKind::Declare { name, ty, value } => {
            exprs::expr(r, ast, src, *value);
            types::resolve(r, ast, src, *ty);
            r.declare(ast, src, *name, Binding::cell(*ty, *value));
        }
        // The initialiser above is not counted as a write: a cell that is only
        // ever initialised has been written once by definition, and counting
        // it would make the unused rule unable to see any cell at all.
        StmtKind::Mutate { place, value } => {
            exprs::expr(r, ast, src, *value);
            exprs::place(r, ast, src, *place);
        }
        StmtKind::Return(Some(value)) => exprs::expr(r, ast, src, *value),
        StmtKind::Assert(value) | StmtKind::Expr(value) => exprs::expr(r, ast, src, *value),
        StmtKind::Return(None) | StmtKind::Break | StmtKind::Continue | StmtKind::Error => {}
        StmtKind::While { cond, block: body } => {
            exprs::expr(r, ast, src, *cond);
            block(r, ast, src, body);
        }
        StmtKind::ForIn { name, iterable, block: body } => {
            // The iterable is resolved outside the loop's scope: `for x in x`
            // must not find its own binding.
            exprs::expr(r, ast, src, *iterable);
            r.open_scope();
            r.declare(ast, src, *name, Binding::loop_var(*iterable));
            statements(r, ast, src, &body.stmts);
            r.close_scope();
        }
    }
}
