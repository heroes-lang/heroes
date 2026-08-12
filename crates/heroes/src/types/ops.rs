//! §4.14's operators, and there are no promotions anywhere.
//!
//! Every rule here is monomorphic on purpose: `1 + 2.0` is an error, and the
//! error names the conversion the spec provides. That is the whole of "no
//! implicit conversions" — one table, no coercion lattice, and a model that
//! guesses wrong is told which function to write.
//!
//! Three shapes:
//!
//! - **arithmetic** `+ - * / %` — `int` with `int`, `f64` with `f64`, and (panel
//!   017 B) `+` also joins two `str`. Nothing else, and never mixed.
//! - **comparison** `< <= > >=` — `int` and `f64` only. Ordering on `str` is not
//!   offered: it would need a collation the language does not specify, and a
//!   rejection can be relaxed later.
//! - **equality** `== !=` — any two values of *the same* type, because §4.3
//!   makes `==` structural on everything, recursively.
//! - **boolean** `&& || !` — `bool` only, and they short-circuit.

use crate::source::{Source, Span};
use crate::syntax::{Ast, BinaryOp, UnaryOp};

use super::table::Ty;
use super::{errors, Checker, TyId};

pub(super) fn binary(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    op: BinaryOp,
    left: TyId,
    right: TyId,
    span: Span,
) -> TyId {
    // A type that is already wrong was already reported: stay quiet (§4.17 —
    // one mistake, one message).
    if checker.out.types.poisoned(left) || checker.out.types.poisoned(right) {
        return checker.error_ty();
    }
    match op {
        BinaryOp::Add
        | BinaryOp::Sub
        | BinaryOp::Mul
        | BinaryOp::Div
        | BinaryOp::Rem => arithmetic(checker, ast, src, op, left, right, span),
        BinaryOp::Eq | BinaryOp::Ne => {
            if left != right {
                let (a, b) = (checker.show(ast, src, left), checker.show(ast, src, right));
                let diagnostic = errors::mismatch(&a, &b, span);
                checker.push_diagnostic(diagnostic);
                return checker.error_ty();
            }
            checker.out.types.bool()
        }
        // **`int` only, and both sides** (§4.14). A bit pattern is what these
        // operate on, and `f64`'s bits are not its value — `1.5 & 1` would be a
        // question about an IEEE encoding the language never exposes. `bool` is
        // excluded for the reason §4.14 gives for keeping `&&` and `&` apart: one
        // short-circuits and the other does not, and a language where both work on
        // `bool` invites the reader to assume they are the same operator.
        BinaryOp::BitAnd
        | BinaryOp::BitOr
        | BinaryOp::BitXor
        | BinaryOp::Shl
        | BinaryOp::Shr => {
            let int = checker.out.types.int();
            for side in [left, right] {
                if side != int {
                    let got = checker.show(ast, src, side);
                    let diagnostic = errors::bad_operand(name(op), "`int`", &got, span);
                    checker.push_diagnostic(diagnostic);
                    return checker.error_ty();
                }
            }
            int
        }
        BinaryOp::Lt | BinaryOp::Le | BinaryOp::Gt | BinaryOp::Ge => {
            let ordered = matches!(checker.out.types.get(left), Ty::Int | Ty::F64);
            if !ordered {
                let got = checker.show(ast, src, left);
                let diagnostic = errors::bad_operand(name(op), "`int` or `f64`", &got, span);
                checker.push_diagnostic(diagnostic);
                return checker.error_ty();
            }
            if left != right {
                let (a, b) = (checker.show(ast, src, left), checker.show(ast, src, right));
                let diagnostic = errors::mixed_arithmetic(name(op), &a, &b, span);
                checker.push_diagnostic(diagnostic);
                return checker.error_ty();
            }
            checker.out.types.bool()
        }
        BinaryOp::And | BinaryOp::Or => {
            let bool_ty = checker.out.types.bool();
            for side in [left, right] {
                if side != bool_ty {
                    let got = checker.show(ast, src, side);
                    let diagnostic = errors::bad_operand(name(op), "`bool`", &got, span);
                    checker.push_diagnostic(diagnostic);
                    return checker.error_ty();
                }
            }
            bool_ty
        }
    }
}

fn arithmetic(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    op: BinaryOp,
    left: TyId,
    right: TyId,
    span: Span,
) -> TyId {
    let both = left == right;
    let kind = checker.out.types.get(left);
    // `+` on two `str` is concatenation (panel 017 B1). It is the only
    // non-numeric arithmetic rule in the language, and design.md §4.20 already
    // commits the runtime to it.
    if both && kind == Ty::Str {
        if op == BinaryOp::Add {
            return checker.out.types.str();
        }
        let diagnostic =
            errors::bad_operand(name(op), "`int` or `f64`", "str", span);
        checker.push_diagnostic(diagnostic);
        return checker.error_ty();
    }
    if both && matches!(kind, Ty::Int | Ty::F64) {
        return left;
    }
    // Two operands the operator accepts, of different types: name both, because
    // the mistake is the pairing and not either one.
    let acceptable = |ty: TyId| {
        numeric(checker, ty) || (op == BinaryOp::Add && checker.out.types.get(ty) == Ty::Str)
    };
    if !both && acceptable(left) && acceptable(right) {
        let (a, b) = (checker.show(ast, src, left), checker.show(ast, src, right));
        let diagnostic = errors::mixed_arithmetic(name(op), &a, &b, span);
        checker.push_diagnostic(diagnostic);
        return checker.error_ty();
    }
    let got = checker.show(ast, src, if acceptable(left) { right } else { left });
    let allowed = if op == BinaryOp::Add {
        "`int` with `int`, `f64` with `f64`, or `str` with `str`"
    } else {
        "`int` with `int` or `f64` with `f64`"
    };
    let diagnostic = errors::bad_operand(name(op), allowed, &got, span);
    checker.push_diagnostic(diagnostic);
    checker.error_ty()
}

pub(super) fn unary(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    op: UnaryOp,
    operand: TyId,
    span: Span,
) -> TyId {
    if checker.out.types.poisoned(operand) {
        return checker.error_ty();
    }
    match op {
        UnaryOp::Neg => {
            if numeric(checker, operand) {
                return operand;
            }
            let got = checker.show(ast, src, operand);
            let diagnostic = errors::bad_operand("-", "`int` or `f64`", &got, span);
            checker.push_diagnostic(diagnostic);
            checker.error_ty()
        }
        UnaryOp::Not => {
            let bool_ty = checker.out.types.bool();
            if operand == bool_ty {
                return bool_ty;
            }
            let got = checker.show(ast, src, operand);
            let diagnostic = errors::bad_operand("!", "`bool`", &got, span);
            checker.push_diagnostic(diagnostic);
            checker.error_ty()
        }
        UnaryOp::BitNot => {
            let int = checker.out.types.int();
            if operand == int {
                return int;
            }
            let got = checker.show(ast, src, operand);
            let diagnostic = errors::bad_operand("~", "`int`", &got, span);
            checker.push_diagnostic(diagnostic);
            checker.error_ty()
        }
    }
}

fn numeric(checker: &Checker, ty: TyId) -> bool {
    matches!(checker.out.types.get(ty), Ty::Int | Ty::F64)
}

/// The operator as written, for the message.
fn name(op: BinaryOp) -> &'static str {
    match op {
        BinaryOp::Add => "+",
        BinaryOp::Sub => "-",
        BinaryOp::Mul => "*",
        BinaryOp::Div => "/",
        BinaryOp::Rem => "%",
        BinaryOp::Eq => "==",
        BinaryOp::Ne => "!=",
        BinaryOp::Lt => "<",
        BinaryOp::Le => "<=",
        BinaryOp::Gt => ">",
        BinaryOp::Ge => ">=",
        BinaryOp::BitAnd => "&",
        BinaryOp::BitOr => "|",
        BinaryOp::BitXor => "^",
        BinaryOp::Shl => "<<",
        BinaryOp::Shr => ">>",
        BinaryOp::And => "&&",
        BinaryOp::Or => "||",
    }
}
