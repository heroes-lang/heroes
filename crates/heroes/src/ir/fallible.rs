//! `T?`, and the four rows of Part 5's table that read a tag (design.md §4.6).
//!
//! A `T?` is a built-in variant with two cases, `ok` at tag 0 and `err` at tag 1.
//! Everything in this file is that one fact, applied:
//!
//! | surface | what it becomes |
//! |---|---|
//! | `ok(x)`, `fail(c, m)` | a construction — the two sides, built |
//! | `.is_err()` | a tag read and one comparison. **No branch** |
//! | `.must()` | a branch: the payload, or an abort |
//! | `.default(v)` | a branch, and a join slot |
//! | `e?` | a branch, and an **early return that copies out** |
//!
//! `?` is the one that matters, and §4.8's sentence is why: "copy-out always
//! happens, **including on early return and `?`**". So the error edge is a real
//! exit edge and owes the same chain a `return` does. The error itself propagates
//! unchanged — `Shape::Err` wraps the failure that already exists rather than
//! re-reading its code and msg — because a `?` that rebuilt the error would lose
//! whatever the callee put there.

use crate::resolve::Resolved;
use crate::source::{Source, Span};
use crate::syntax::{Arg as AstArg, Ast, ExprId};
use crate::types::{Checked, TyId};

use super::build::Lowering;
use super::inst::{Abort, BinOp, Const, Op, Shape, Term};
use super::ids::{SlotId, ValueId};
use super::{decls, exprs};

/// `ok` and `fail` are built-ins so that the names resolve; here they are
/// constructions, because that is what they do (panel 002: they are ⇐-only, so
/// which `T?` is being built was decided by the checker).
pub(super) fn constructor(name: &str) -> Option<Shape> {
    match name {
        "ok" => Some(Shape::Ok),
        "fail" => Some(Shape::Fail),
        _ => None,
    }
}

pub(super) fn is_operation(name: &str) -> bool {
    matches!(name, "must" | "default" | "is_err")
}

/// `.must()`, `.default(v)`, `.is_err()` — Part 5 calls all three "`match`", and
/// only two of them need an edge.
#[allow(clippy::too_many_arguments)]
pub(super) fn operation(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    name: &str,
    receiver: ExprId,
    args: &[AstArg],
    ty: TyId,
    span: Span,
) -> ValueId {
    let subject = hold(b, ast, resolved, checked, src, receiver, span);
    match name {
        // A tag read and a comparison: no control flow at all, which is why this
        // row of the table is the cheap one.
        "is_err" => {
            let tag = read_tag(b, checked, subject, span);
            let one = b.emit(Op::Const(Const::Int(1)), checked.types.int(), span);
            b.emit(Op::Binary { op: BinOp::Eq, left: tag, right: one }, ty, span)
        }
        "must" => {
            let good = b.block("must: ok");
            let bad = b.block("must: abort");
            branch_on_ok(b, checked, subject, good, bad, span);

            b.switch_to(bad);
            // **The failure travels with the abort.** Without it the panic can only say
            // that a `.must()` failed, which is the one thing the reader already knows;
            // with it, it says the `code` and `msg` the author wrote (§4.6). The read
            // happens *inside* the abort block, so nothing counted crosses a block edge
            // and `asserts.rs`'s synthetic-slot repair is not needed here.
            let failure = payload(b, subject, 1, checked.types.failure(), span);
            let carried = b.args(&[super::Arg::Value(failure)]);
            b.emit_void(Op::Abort { reason: Abort::Must, args: carried }, b.unit_ty(), span);
            b.terminate(Term::Unreachable);

            b.switch_to(good);
            payload(b, subject, 0, ty, span)
        }
        // `.default(v)`: two edges into one slot, the same shape an `if` with a
        // value has.
        _ => {
            let result = b.synthetic("r", ty);
            let good = b.block("default: ok");
            let fallback = b.block("default: else");
            let join = b.block("default: join");
            branch_on_ok(b, checked, subject, good, fallback, span);

            b.switch_to(good);
            let value = payload(b, subject, 0, ty, span);
            b.store(result, value, span);
            b.terminate(Term::Jump(join));

            b.switch_to(fallback);
            let value = match args.first() {
                Some(arg) => exprs::expr(b, ast, resolved, checked, src, arg.value),
                None => b.emit(Op::Missing, ty, span),
            };
            b.store(result, value, span);
            b.terminate(Term::Jump(join));

            b.switch_to(join);
            b.load(result, span)
        }
    }
}

/// `e?` — propagate (§4.6). The error edge is an exit edge, so it copies out.
pub(super) fn try_expr(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    inner: ExprId,
    ty: TyId,
    span: Span,
) -> ValueId {
    let subject = hold(b, ast, resolved, checked, src, inner, span);
    let good = b.block("?: ok");
    let bad = b.block("?: propagate");
    branch_on_ok(b, checked, subject, good, bad, span);

    b.switch_to(bad);
    let failure = payload(b, subject, 1, checked.types.failure(), span);
    // The caller's `T?`, carrying the callee's failure unchanged. The result type
    // is the enclosing function's, which `?` already required to be fallible.
    let result = b.result_type();
    let args = b.args(&[super::ids::Arg::Value(failure)]);
    let wrapped = b.emit(Op::Construct { shape: Shape::Err, args }, result, span);
    decls::copy_out(b, checked);
    b.terminate(Term::Return(Some(wrapped)));

    b.switch_to(good);
    payload(b, subject, 0, ty, span)
}

/// Puts the subject in a slot. Every one of these forms reads it twice — once for
/// the tag, once for the payload — and `f()?` must not call `f` twice.
fn hold(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    expr: ExprId,
    span: Span,
) -> SlotId {
    let ty = checked.expr_types[expr.0 as usize];
    let value = exprs::expr(b, ast, resolved, checked, src, expr);
    let slot = b.synthetic("f", ty);
    b.store(slot, value, span);
    slot
}

fn read_tag(b: &mut Lowering, checked: &Checked, subject: SlotId, span: Span) -> ValueId {
    let held = b.load(subject, span);
    b.emit(Op::Tag(held), checked.types.int(), span)
}

/// The branch all three operations share: tag 0 is `ok`.
fn branch_on_ok(
    b: &mut Lowering,
    checked: &Checked,
    subject: SlotId,
    good: super::ids::BlockId,
    bad: super::ids::BlockId,
    span: Span,
) {
    let tag = read_tag(b, checked, subject, span);
    let zero = b.emit(Op::Const(Const::Int(0)), checked.types.int(), span);
    let ok = b.emit(
        Op::Binary { op: BinOp::Eq, left: tag, right: zero },
        checked.types.bool(),
        span,
    );
    b.terminate(Term::Branch { cond: ok, then: good, otherwise: bad });
}

fn payload(b: &mut Lowering, subject: SlotId, case: u32, ty: TyId, span: Span) -> ValueId {
    let held = b.load(subject, span);
    b.emit(Op::Payload { base: held, case }, ty, span)
}
