//! A literal checked against the type its context asks for (spec § Types; panels
//! 042, 060, 062).
//!
//! Split from `expect.rs` on 2026-08-15. That file answers *does this expression
//! have the expected type*, one arm per expression shape; these are the arms where
//! the answer is *it does now*, because a literal carries values and no type and
//! the context carries the type. They are the ⇐ direction at its purest, and they
//! are the ones that keep growing — an integer width, a float width, and a fixed
//! array's length are three instances of one rule.

use crate::source::{Source, Span};
use crate::syntax::{Ast, ExprId, ExprKind, UnaryOp};

use super::table::Ty;
use super::{Checker, TyId};

/// A float literal at the width the context asks for.
pub(super) fn float_at(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    id: ExprId,
    expected: TyId,
    span: Span,
) {

            if matches!(checker.out.types.get(expected), Ty::Float(_)) {
                let mut at = id;
                while let ExprKind::Unary { op: UnaryOp::Neg, operand } =
                    &ast.exprs[at.0 as usize].kind
                {
                    at = *operand;
                    checker.record(at, expected);
                }
                checker.record(id, expected);
            } else {
                let got = checker.out.types.f64();
                let shown = checker.show(ast, src, got);
                super::expect::mismatch(checker, ast, src, expected, &shown, span);
                checker.record(id, got);
            }
}
