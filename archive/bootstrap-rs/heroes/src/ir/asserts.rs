//! `assert` (design.md §4.18, spec line 157: "an `assert` failure shows the
//! source expression and both sides").
//!
//! Part 5's row reads "`if` plus `panic`, with source text attached", and the
//! **and both sides** half is what makes lowering it at M-ir-lowering rather than M-generics-library worth
//! doing. The spans have existed since M-syntax-tree; a lowering that emitted only the text
//! would have to be rewritten when the test runner arrives, which is §1.2's cost
//! formula turned on this compiler's own source. So the abort carries three
//! operands where the asserted expression is a comparison — the text, the left
//! value, the right value — and one where it is not.
//!
//! Note what is *not* here: no `panic` in the built-in inventory. `resolve/
//! builtins.rs` says why — the runtime has it, the user gets `assert`, and making
//! it callable is a language addition that would need a panel. The abort is an
//! instruction rather than a call for exactly that reason.

use crate::resolve::Resolved;
use crate::source::{Source, Span};
use crate::syntax::{Ast, BinaryOp, ExprId, ExprKind};
use crate::types::Checked;

use super::build::Lowering;
use super::inst::{Abort, Const, Op, Term};
use super::ids::{Arg, ValueId};
use super::exprs;

pub(super) fn assert(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    expr: ExprId,
    span: Span,
) {
    let text = source_text(b, checked, src, span, span);
    let (cond, sides) = condition(b, ast, resolved, checked, src, expr);

    // **A counted operand travels through a slot, not across the edge.**
    //
    // The two sides are computed here, in the test block, and read over there, in the
    // failure block — which is the whole reason they are named at all. For an `i64`
    // that is free. For a `str` it is a use-after-free, because M-strings-ownership's ownership pass
    // releases an owning temporary at the end of the block that defines it, and a
    // block-crossing read is precisely what it cannot see.
    //
    // Panel 021 predicted this and scheduled the repair for M-generics-library, with `Abort::Assert`.
    // It arrived early: the verifier's `Owned` invariant fired on
    // `examples/gallery/07-strings.hero`, on `assert sentence_of([…]) == "one two"`.
    // A synthetic slot is the fix the panel named, and it costs two instructions on a
    // path that is about to abort the program.
    let carried: Vec<Carried> = sides
        .into_iter()
        .map(|value| {
            let ty = b.value_type(value);
            if !super::is_refcounted(checked, ty) {
                return Carried::Value(value);
            }
            let slot = b.synthetic("assert", ty);
            b.store(slot, value, span);
            Carried::Slot(slot)
        })
        .collect();

    let good = b.block("assert: held");
    let bad = b.block("assert: failed");
    b.terminate(Term::Branch { cond, then: good, otherwise: bad });

    b.switch_to(bad);
    let mut operands = vec![Arg::Value(text)];
    for value in carried {
        let value = match value {
            Carried::Value(value) => value,
            Carried::Slot(slot) => b.load(slot, span),
        };
        operands.push(Arg::Value(value));
    }
    let args = b.args(&operands);
    let unit = b.unit_ty();
    b.emit_void(Op::Abort { reason: Abort::Assert, args }, unit, span);
    b.terminate(Term::Unreachable);

    b.switch_to(good);
}

/// How an operand reaches the failure block: directly, or through a slot because it
/// carries a reference.
enum Carried {
    Value(ValueId),
    Slot(super::ids::SlotId),
}

/// The condition, and the two sides when there are two.
///
/// A comparison is lowered here rather than through `exprs::expr` so that the
/// operands are named: the same two temporaries feed the branch *and* the failure
/// message, which is what lets the message quote values instead of re-evaluating
/// an expression that may have side effects.
fn condition(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    expr: ExprId,
) -> (ValueId, Vec<ValueId>) {
    let span = ast.exprs[expr.0 as usize].span;
    if let ExprKind::Binary { op, left, right } = &ast.exprs[expr.0 as usize].kind {
        if let Some(op) = comparison(*op) {
            let left = exprs::expr(b, ast, resolved, checked, src, *left);
            let right = exprs::expr(b, ast, resolved, checked, src, *right);
            let bool_ty = checked.types.bool();
            let cond = b.emit(Op::Binary { op, left, right }, bool_ty, span);
            return (cond, vec![left, right]);
        }
    }
    (exprs::expr(b, ast, resolved, checked, src, expr), Vec::new())
}

/// The asserted expression, as the author wrote it. Interned like any other
/// string, so the emitter has nothing special to do with it.
/// The asserted expression **as the author wrote it**, which is the statement's own
/// text with the keyword taken off the front.
///
/// **It used to be the expression node's span, and that lost the parentheses.**
/// Measured 2026-08-16: `assert (1 == 1) == (1 == 2)` reported
/// `assert failed: 1 == 1) == (1 == 2` — spec:190 says an `assert` failure "shows
/// the source expression", and that is not it. The cause is that a parenthesised
/// operand's span covers what is *inside* the parentheses, so the outer `==`'s span
/// runs from the left operand's first token to the right operand's last and steps
/// over both brackets on the way. The expression node is the wrong thing to slice,
/// because what the reader is looking at is a line of text rather than a tree.
///
/// Taking the statement instead cannot lose a character the author typed. The
/// keyword is fixed and always leading, so stripping it is a fact about the value
/// rather than a guess about the shape (CLAUDE.md §11), and the trailing trim is
/// what keeps a comment after the expression — the statement span reaches one — out
/// of the message.
fn source_text(
    b: &mut Lowering,
    checked: &Checked,
    src: &Source,
    text: Span,
    span: Span,
) -> ValueId {
    let written = src.slice(text);
    let expression = written.strip_prefix("assert").unwrap_or(written).trim();
    let id = b.intern(expression.to_string());
    b.emit(Op::Const(Const::Str(id)), checked.types.str(), span)
}

fn comparison(op: BinaryOp) -> Option<super::inst::BinOp> {
    use super::inst::BinOp;
    match op {
        BinaryOp::Eq => Some(BinOp::Eq),
        BinaryOp::Ne => Some(BinOp::Ne),
        BinaryOp::Lt => Some(BinOp::Lt),
        BinaryOp::Le => Some(BinOp::Le),
        BinaryOp::Gt => Some(BinOp::Gt),
        BinaryOp::Ge => Some(BinOp::Ge),
        _ => None,
    }
}
