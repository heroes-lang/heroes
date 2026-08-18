//! Control flow, and three rows of Part 5's table erased (design.md §4.7,
//! §4.14's short-circuit rule; Part 5).
//!
//! **The IR has no `if`, no `for` and no `&&`.** It has blocks and two-way
//! branches, and each of those three surface forms is a shape built out of them:
//!
//! | surface | blocks |
//! |---|---|
//! | `while cond` | test · body · exit |
//! | `for x in xs` | test · body · **step** · exit |
//! | `if` / `else if` / `else` | one test and one arm per branch, then a join |
//! | `a && b` | the right side, then a join |
//!
//! The `for` loop's fourth block is the one worth knowing about: the increment
//! must happen on the `continue` path too, so `continue` targets the *step* block
//! and not the test. A `while`'s `continue` targets its test, because a `while`
//! has nothing to step.
//!
//! **A branching expression's value is a slot** (panel 019 point 2): each arm
//! stores into `$r`, and the join loads it. That is what replaces a phi node, and
//! it is why a join needs no argument list — the llm-ergonomist's blind A/B found
//! the argument form unreadable for a reason that generalises, that `%3`'s meaning
//! *is* the set of jumps elsewhere in the function.
//!
//! When every arm diverges the join has no predecessors, and lowering says so:
//! the block is terminated `unreachable` rather than left holding a load nothing
//! can reach.

use crate::resolve::Resolved;
use crate::source::{Source, Span};
use crate::syntax::{Arm, Ast, BinaryOp, Block as AstBlock, Branch, ExprId};
use crate::types::{Checked, TyId};

use super::build::{Loop, Lowering};
use super::inst::{BinOp, Const, Op, Term};
use super::ids::{ValueId};
use super::{exprs, matches, stmts, SlotKind};

/// `while cond` — the condition loop (§4.7, panel 018 gave it its own keyword).
pub(super) fn while_loop(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    cond: ExprId,
    body: &AstBlock,
) {
    let test = b.block("while: test");
    let inside = b.block("while: body");
    let exit = b.block("while: exit");
    b.terminate(Term::Jump(test));

    b.switch_to(test);
    let value = exprs::expr(b, ast, resolved, checked, src, cond);
    b.terminate(Term::Branch { cond: value, then: inside, otherwise: exit });

    b.loops.push(Loop { break_to: exit, continue_to: test });
    b.switch_to(inside);
    stmts::block(b, ast, resolved, checked, src, body, false);
    b.terminate(Term::Jump(test));
    b.loops.pop();

    b.switch_to(exit);
}

/// `for x in xs` — Part 5's row, erased: a `while` with an index.
///
/// The iterable is evaluated **once**, into a slot, because `for x in f()` must
/// not call `f` per iteration. The index is `$i<n>`: a name the author cannot
/// have written, so it cannot shadow anything (§4.4 makes shadowing an error) and
/// cannot be reported unused (§4.4 again).
pub(super) fn for_loop(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    name: Span,
    iterable: ExprId,
    body: &AstBlock,
) {
    let int = checked.types.int();
    let bool_ty = checked.types.bool();
    let sequence_ty = checked.expr_types[iterable.0 as usize];

    let sequence = exprs::expr(b, ast, resolved, checked, src, iterable);
    let holder = b.synthetic("xs", sequence_ty);
    b.store(holder, sequence, name);
    let index = b.synthetic("i", int);
    let zero = b.emit(Op::Const(Const::Int(0)), int, name);
    b.store(index, zero, name);

    // The loop variable is a real local: the resolver made it (`LocalKind::Loop`)
    // and the body reads it by name. `for _ in xs` names nothing (§4.7), so it gets
    // no slot and the body never reads an element — the loop is being run for its
    // count, and `range(0, times)` in the gallery is exactly that.
    let element = match b.local_at(name) {
        Some(local) => {
            let ty = checked.local_type(local as usize);
            let slot = b.slot(src.slice(name).to_string(), ty, SlotKind::Local);
            b.bind_local(local, slot);
            Some((slot, ty))
        }
        None => None,
    };

    let test = b.block("for: test");
    let inside = b.block("for: body");
    let step = b.block("for: step");
    let exit = b.block("for: exit");
    b.terminate(Term::Jump(test));

    b.switch_to(test);
    let i = b.load(index, name);
    let xs = b.load(holder, name);
    let len = b.emit(Op::Len(xs), int, name);
    let more = b.emit(Op::Binary { op: BinOp::Lt, left: i, right: len }, bool_ty, name);
    b.terminate(Term::Branch { cond: more, then: inside, otherwise: exit });

    b.switch_to(inside);
    if let Some((slot, element_ty)) = element {
        let xs = b.load(holder, name);
        let i = b.load(index, name);
        let item = b.emit(Op::Index { base: xs, index: i }, element_ty, name);
        b.store(slot, item, name);
    }
    // `continue` lands on the step, not the test: the increment owes itself to
    // that path as much as to the fall-through.
    b.loops.push(Loop { break_to: exit, continue_to: step });
    stmts::block(b, ast, resolved, checked, src, body, false);
    b.terminate(Term::Jump(step));
    b.loops.pop();

    b.switch_to(step);
    let i = b.load(index, name);
    let one = b.emit(Op::Const(Const::Int(1)), int, name);
    let next = b.emit(Op::Binary { op: BinOp::Add, left: i, right: one }, int, name);
    b.store(index, next, name);
    b.terminate(Term::Jump(test));

    b.switch_to(exit);
}

/// `if c` / `else if c` / `else` — an expression like everything else (§4.7), so
/// one routine serves both positions. In statement position the join slot is
/// simply never loaded.
#[allow(clippy::too_many_arguments)]
pub(super) fn if_expr(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    branches: &[Branch],
    otherwise: &Option<AstBlock>,
    ty: TyId,
    span: Span,
) -> ValueId {
    let wants_value = ty != b.unit_ty() && ty != checked.types.error();
    let result = if wants_value { Some(b.synthetic("r", ty)) } else { None };
    let join = b.block("if: join");

    for branch in branches {
        let cond = exprs::expr(b, ast, resolved, checked, src, branch.cond);
        let then = b.block("if: then");
        let next = b.block("if: else");
        b.terminate(Term::Branch { cond, then, otherwise: next });

        b.switch_to(then);
        let value = stmts::block(b, ast, resolved, checked, src, &branch.block, wants_value);
        store_arm(b, result, value, span);
        b.terminate(Term::Jump(join));

        b.switch_to(next);
    }
    // No `else` means the fall-through *is* the missing arm. An `if` used as a
    // value always has one (§4.7, checked at M-checker-core), so that only happens in
    // statement position.
    if let Some(block) = otherwise {
        let value = stmts::block(b, ast, resolved, checked, src, block, wants_value);
        store_arm(b, result, value, span);
    }
    b.terminate(Term::Jump(join));

    b.switch_to(join);
    seal_join(b, join, result, ty, span)
}

/// `match e` — delegated, because a `match` on a variant and a `match` on
/// literals are two different shapes and this file is about the shapes that are
/// one.
#[allow(clippy::too_many_arguments)]
pub(super) fn match_expr(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    scrutinee: ExprId,
    arms: &[Arm],
    ty: TyId,
    span: Span,
) -> ValueId {
    matches::lower(b, ast, resolved, checked, src, scrutinee, arms, ty, span)
}

/// `a && b` and `a || b` — not instructions. §4.14 makes them short-circuit, so
/// the right side is evaluated on one edge only, and that is a branch.
#[allow(clippy::too_many_arguments)]
pub(super) fn short_circuit(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    op: BinaryOp,
    left: ExprId,
    right: ExprId,
    ty: TyId,
    span: Span,
) -> ValueId {
    let result = b.synthetic("b", ty);
    let first = exprs::expr(b, ast, resolved, checked, src, left);
    b.store(result, first, span);
    let rest = b.block(if op == BinaryOp::And { "&&: right" } else { "||: right" });
    let join = b.block(if op == BinaryOp::And { "&&: join" } else { "||: join" });
    // The asymmetry *is* the operator: `&&` evaluates the right side when the left
    // is true, `||` when it is false.
    let term = if op == BinaryOp::And {
        Term::Branch { cond: first, then: rest, otherwise: join }
    } else {
        Term::Branch { cond: first, then: join, otherwise: rest }
    };
    b.terminate(term);

    b.switch_to(rest);
    let second = exprs::expr(b, ast, resolved, checked, src, right);
    b.store(result, second, span);
    b.terminate(Term::Jump(join));

    b.switch_to(join);
    b.load(result, span)
}

/// Writes an arm's value into the join slot, when there is one of each.
pub(super) fn store_arm(
    b: &mut Lowering,
    result: Option<super::ids::SlotId>,
    value: Option<ValueId>,
    span: Span,
) {
    if let (Some(slot), Some(value)) = (result, value) {
        b.store(slot, value, span);
    }
}

/// Closes a join: load the value if there is one, and say `unreachable` when
/// nothing reaches here — which happens when every arm jumped or returned. A
/// `match` all of whose arms diverge is legal in statement position (panel 017 A),
/// so this is a real shape and not a defensive branch.
pub(super) fn seal_join(
    b: &mut Lowering,
    join: super::ids::BlockId,
    result: Option<super::ids::SlotId>,
    ty: TyId,
    span: Span,
) -> ValueId {
    if !b.has_preds(join) {
        b.terminate(Term::Unreachable);
        return b.unit_value();
    }
    match result {
        Some(slot) => b.load(slot, span),
        None => {
            let _ = ty;
            b.unit_value()
        }
    }
}
