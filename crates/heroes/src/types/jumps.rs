//! Where control goes (design.md §4.5, §4.7; spec § Control flow).
//!
//! Split out of `stmts.rs` by the §11 sweep. Every arm here answers the question
//! `Flow` exists to record — does control leave this statement, or fall through —
//! and each answers it differently for a reason worth having in one place:
//!
//! - `return` **jumps**, and its two error cases are opposites: a value where the
//!   signature promised none, and none where it promised one.
//! - `break`/`continue` **jump**, and are legal only inside a loop.
//! - `while` and `for` **fall through**, always: a loop may run zero times, so it
//!   never counts as diverging. That one line is why a function whose only
//!   `return` sits inside a loop is still `missing_return`.

use crate::resolve::Resolved;
use crate::source::{Source, Span};
use crate::syntax::{Ast, Block, ExprId, StmtId, StmtKind};

use super::stmts::{block, Flow, Want};
use super::table::Ty;
use super::{errors, expect, exprs, Checker, TyId};

/// `return e` and `return` — checked against the signature, never inferred.
pub(super) fn returns(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    value: Option<ExprId>,
    span: Span,
) -> (Flow, Option<TyId>) {
            let unit = checker.out.types.unit();
            match value {
                Some(expr) => {
                    if checker.result == unit {
                        let diagnostic = errors::returns_nothing(span);
                        checker.push_diagnostic(diagnostic);
                        exprs::synth(checker, ast, resolved, src, expr);
                    } else {
                        let want = checker.result;
                        expect::check(checker, ast, resolved, src, expr, want);
                    }
                }
                None => {
                    if checker.result != unit {
                        let want = checker.show(ast, src, checker.result);
                        let diagnostic = errors::missing_value(&want, span);
                        checker.push_diagnostic(diagnostic);
                    }
                }
            }
            (Flow::Jumps, None)
}

/// `break` and `continue`, which need a loop to be inside of.
pub(super) fn jump(checker: &mut Checker, ast: &Ast, id: StmtId, span: Span) -> (Flow, Option<TyId>) {
            if checker.loops == 0 {
                let word = if matches!(ast.stmts[id.0 as usize].kind, StmtKind::Break) {
                    "break"
                } else {
                    "continue"
                };
                let diagnostic = errors::jump_outside_loop(word, span);
                checker.push_diagnostic(diagnostic);
            }
            (Flow::Jumps, None)
}

/// `while cond` — a `bool` and a body, and no value either way.
pub(super) fn while_loop(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    cond: ExprId,
    body: &Block,
) -> (Flow, Option<TyId>) {
            let bool_ty = checker.out.types.bool();
            let got = exprs::synth(checker, ast, resolved, src, cond);
            if got != bool_ty && !checker.out.types.poisoned(got) {
                let shown = checker.show(ast, src, got);
                let at = ast.exprs[cond.0 as usize].span;
                let diagnostic = errors::not_bool("a `while` condition", &shown, at);
                checker.push_diagnostic(diagnostic);
            }
            checker.loops += 1;
            block(checker, ast, resolved, src, body, Want::Nothing);
            checker.loops -= 1;
            // A loop may run zero times, so it never counts as diverging.
            (Flow::Falls, None)
}

/// `for x in xs` — the element type comes from the iterable, and a bare `str` is
/// the mistake worth naming.
pub(super) fn for_in(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    name: Span,
    iterable: ExprId,
    body: &Block,
) -> (Flow, Option<TyId>) {
            let over = exprs::synth(checker, ast, resolved, src, iterable);
            let element = match checker.out.types.get(over) {
                Ty::Array(element) => element,
                // `s.chars()` is how a string is walked (§4.3), so a bare `str`
                // here is the mistake worth naming.
                Ty::Error => checker.error_ty(),
                _ => {
                    let got = checker.show(ast, src, over);
                    let at = ast.exprs[iterable.0 as usize].span;
                    let diagnostic = errors::not_iterable(&got, at);
                    checker.push_diagnostic(diagnostic);
                    checker.error_ty()
                }
            };
            checker.bind_local(name, element);
            checker.loops += 1;
            block(checker, ast, resolved, src, body, Want::Nothing);
            checker.loops -= 1;
            (Flow::Falls, None)
}
