//! Statements: what binds, what writes, and where control leaves
//! (design.md §4.4 bindings, §4.7 control flow, §4.8 copy-out, §4.18 `assert`).
//!
//! **A block is a straight line until something branches.** Statements are
//! lowered in order into the current block; a construct that needs edges opens
//! new blocks and leaves the builder pointing at the one control continues in.
//! Nothing after a terminator is lowered — statements after a `return` cannot
//! execute, and putting them in the dump would show code that does not exist.
//!
//! `=` and `@` differ in the surface and not here: both are slots. §4.4's rule
//! ("only a declared `@` name can be mutated") was enforced by the resolver, and
//! the IR does not re-litigate it — the same reason `Ref::Local` is read rather
//! than recomputed.
//!
//! The three exit edges of a body are all in this file, and each owes §4.8 its
//! copy-out: `return e`, bare `return`, and (in `fallible.rs`) the error side of
//! `?`. `break` and `continue` are *not* exits — they leave a loop, not the
//! function — which is why they jump without copying out.

use crate::resolve::Resolved;
use crate::source::Source;
use crate::syntax::{Ast, Block, StmtId, StmtKind};
use crate::types::{Checked, TyId};

use super::build::Lowering;
use super::inst::{Op, Term, ValueId};
use super::{asserts, control, decls, exprs, places, SlotKind};

/// Lowers a block's statements in order.
///
/// `wants_value` is the constant-and-arm case: a block's value is its last
/// statement when that statement is an expression (§4.7). A function body wants
/// nothing — it hands its value back through `return`.
pub(super) fn block(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    body: &Block,
    wants_value: bool,
) -> Option<ValueId> {
    let mut value = None;
    let last = body.stmts.len().saturating_sub(1);
    for (index, id) in body.stmts.iter().enumerate() {
        if b.is_terminated() {
            break;
        }
        let produced = statement(b, ast, resolved, checked, src, *id);
        if wants_value && index == last {
            value = produced;
        }
    }
    value
}

/// The type a block produces: its last statement's, when that statement is an
/// expression. Read straight out of the checker's dense table — lowering never
/// re-derives a type it was given.
pub(super) fn block_value_type(ast: &Ast, checked: &Checked, body: &Block) -> TyId {
    match body.stmts.last() {
        Some(id) => match ast.stmts[id.0 as usize].kind {
            StmtKind::Expr(expr) => checked.expr_types[expr.0 as usize],
            _ => checked.types.unit(),
        },
        None => checked.types.unit(),
    }
}

/// Lowers one statement. The `Option` is its value, which only an expression
/// statement has.
pub(super) fn statement(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    id: StmtId,
) -> Option<ValueId> {
    let stmt = &ast.stmts[id.0 as usize];
    match &stmt.kind {
        StmtKind::Bind { name, value, .. } | StmtKind::Declare { name, value, .. } => {
            let produced = exprs::expr(b, ast, resolved, checked, src, *value);
            // `_ = f(x)` binds nothing (§4.7): the resolver made no local for it,
            // so the call happens and the value is dropped.
            if let Some(local) = b.local_at(*name) {
                let ty = checked.local_type(local as usize);
                let slot = b.slot(src.slice(*name).to_string(), ty, SlotKind::Local);
                b.bind_local(local, slot);
                b.store(slot, produced, stmt.span);
            }
            None
        }
        StmtKind::Mutate { place, value } => {
            let target = places::place(b, ast, resolved, checked, src, *place);
            let produced = exprs::expr(b, ast, resolved, checked, src, *value);
            let ty = b.value_type(produced);
            b.emit_void(Op::Store { place: target, value: produced }, ty, stmt.span);
            None
        }
        StmtKind::Return(None) => {
            decls::copy_out(b, checked);
            b.terminate(Term::Return(None));
            None
        }
        StmtKind::Return(Some(expr)) => {
            let produced = exprs::expr(b, ast, resolved, checked, src, *expr);
            // The copy-out chain goes *after* the value is computed and before
            // control leaves: the returned value was read from the pre-copy-out
            // state, which is what "copy in, copy out" means.
            decls::copy_out(b, checked);
            b.terminate(Term::Return(Some(produced)));
            None
        }
        StmtKind::Break => {
            if let Some(target) = b.loops.last().map(|l| l.break_to) {
                b.terminate(Term::Jump(target));
            }
            None
        }
        StmtKind::Continue => {
            if let Some(target) = b.loops.last().map(|l| l.continue_to) {
                b.terminate(Term::Jump(target));
            }
            None
        }
        StmtKind::Assert(expr) => {
            asserts::assert(b, ast, resolved, checked, src, *expr, stmt.span);
            None
        }
        StmtKind::While { cond, block: body } => {
            control::while_loop(b, ast, resolved, checked, src, *cond, body);
            None
        }
        StmtKind::ForIn { name, iterable, block: body } => {
            control::for_loop(b, ast, resolved, checked, src, *name, *iterable, body);
            None
        }
        StmtKind::Expr(expr) => {
            // A statement's type must be `()` (panel 003), with one exception the
            // checker allows and this pass has to honour: a `match` or `if` in
            // statement position. Both lower the same way in either position, so
            // there is nothing special here — the value is simply not used.
            let produced = exprs::expr(b, ast, resolved, checked, src, *expr);
            Some(produced)
        }
        StmtKind::Error => None,
    }
}
