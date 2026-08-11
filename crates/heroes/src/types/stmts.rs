//! Statements, and the three things only a statement can decide (§4.4, §4.7,
//! §4.14, §4.18).
//!
//! 1. **Divergence.** `return`, `break` and `continue` are statements, so this is
//!    the only file that has to know a jump when it sees one. A block reports
//!    whether it falls through, and that is all the machinery panel 017's A1
//!    needs — `join.rs` does the rest, once, for every branching construct.
//! 2. **Where a value is not allowed.** Panel 003: a non-`()` expression alone on
//!    a line is an error with a certain fix. Panel 017 C: a `()` value bound to a
//!    name is the mirror error.
//! 3. **What a pattern binds.** A payload binding's type comes from the case the
//!    pattern names, which is why `.num n` gives `n` a type nothing else can
//!    write down.

use crate::resolve::Resolved;
use crate::source::Source;
use crate::syntax::{Ast, Block, ExprId, ExprKind, StmtId, StmtKind};

use super::table::Ty;
use super::{errors, exprs, expect, lower, Checker, TyId};

/// Does control leave this statement, or fall through to the next one?
#[derive(Clone, Copy, PartialEq, Eq)]
pub(super) enum Flow {
    Falls,
    Jumps,
}



/// What a block or an arm is *for*. The three cases are the whole of §4.5's two
/// modes, seen from the statement side:
///
/// - `Nothing` — a body run for effect. Its last line is a statement like any
///   other, so §4.14's `()` rule applies to it too.
/// - `Value(t)` — a body whose value is checked against `t` (⇐). This is what
///   carries an expectation into a `match` arm, and the ⇐-only forms (`ok`,
///   `fail`, `.case`, `[]`) are unwritable there without it.
/// - `Unknown` — a body whose value is synthesised (⇒), because the context has
///   no expectation to give: `x = match k` infers `x` from the arms.
#[derive(Clone, Copy, PartialEq, Eq)]
pub(super) enum Want {
    Nothing,
    Value(TyId),
    Unknown,
}



/// A block: every statement checked, and — when `wants_value` — the type its last
/// statement produces.
/// The type a mutation target accepts, which differs from the type it *reads* in
/// exactly one case: an index into a map.
fn write_target(checker: &Checker, ast: &Ast, place: ExprId, read: TyId) -> TyId {
    let ExprKind::Index { base, .. } = ast.exprs[place.0 as usize].kind else { return read };
    let base_ty = checker.out.expr_types[base.0 as usize];
    match (checker.out.types.get(base_ty), checker.out.types.get(read)) {
        (Ty::Map(_, _), Ty::Fallible(value)) => value,
        _ => read,
    }
}

pub(super) fn block(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    body: &Block,
    want: Want,
) -> (Flow, Option<TyId>) {
    let mut flow = Flow::Falls;
    let mut value = None;
    let last = body.stmts.len().saturating_sub(1);
    for (index, id) in body.stmts.iter().enumerate() {
        let is_last = index == last;
        // Only the last statement of a value block may produce a value; every
        // other line is a statement, and §4.14 wants it to be `()`.
        let want = if is_last { want } else { Want::Nothing };
        let (statement_flow, produced) =
            statement(checker, ast, resolved, src, *id, want);
        if statement_flow == Flow::Jumps {
            flow = Flow::Jumps;
        }
        if is_last {
            value = produced;
        }
    }
    // A block that jumps produces no value, whatever its last line looked like.
    if flow == Flow::Jumps {
        return (flow, None);
    }
    // Asked for a value, falls through, and has none: its last line is a
    // statement. That is *not* the same as diverging, and treating the two alike
    // would let `x = if c` with a loop in the branch pass silently.
    if want != Want::Nothing && value.is_none() {
        let diagnostic = errors::branch_without_value(body.span);
        checker.push_diagnostic(diagnostic);
    }
    (flow, value)
}



pub(super) fn statement(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    id: StmtId,
    want: Want,
) -> (Flow, Option<TyId>) {
    let span = ast.stmts[id.0 as usize].span;
    match &ast.stmts[id.0 as usize].kind {
        StmtKind::Bind { name, ty, value } => {
            let declared = ty.map(|written| lower::ty(checker, ast, resolved, written));
            let bound = match declared {
                Some(want) => {
                    expect::check(checker, ast, resolved, src, *value, want);
                    want
                }
                None => exprs::synth(checker, ast, resolved, src, *value),
            };
            // Panel 017 C: `()` is not a value, so nothing can hold one.
            let unit = checker.out.types.unit();
            let bound = if bound == unit {
                let diagnostic = errors::bound_unit(src.slice(*name), *name);
                checker.push_diagnostic(diagnostic);
                // Poisoned, not `()`: every later read of the name stays quiet,
                // because there is one mistake here and it has been reported.
                checker.error_ty()
            } else {
                bound
            };
            checker.bind_local(*name, bound);
            (Flow::Falls, None)
        }
        StmtKind::Declare { name, ty, value } => {
            let declared = lower::ty(checker, ast, resolved, *ty);
            expect::check(checker, ast, resolved, src, *value, declared);
            checker.bind_local(*name, declared);
            (Flow::Falls, None)
        }
        StmtKind::Mutate { place, value } => {
            let target = exprs::synth(checker, ast, resolved, src, *place);
            // **The write side of `m[k]` is `V`, not `V?`** (§4.9, panel 026). Reading a
            // map yields a fallible because the key may be absent; *writing* one cannot
            // fail, because `m[k] @ v` inserts — which is exactly where it diverges from
            // `xs[i] @ v`, and the spec names the divergence. So the target type sheds
            // one `Fallible` layer, and only for this shape: a `T?` cell assigned a `T?`
            // is still a plain store.
            let target = write_target(checker, ast, *place, target);
            expect::check(checker, ast, resolved, src, *value, target);
            (Flow::Falls, None)
        }
        StmtKind::Return(value) => {
            let unit = checker.out.types.unit();
            match value {
                Some(expr) => {
                    if checker.result == unit {
                        let diagnostic = errors::returns_nothing(span);
                        checker.push_diagnostic(diagnostic);
                        exprs::synth(checker, ast, resolved, src, *expr);
                    } else {
                        let want = checker.result;
                        expect::check(checker, ast, resolved, src, *expr, want);
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
        StmtKind::Break | StmtKind::Continue => {
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
        StmtKind::Assert(value) => {
            let bool_ty = checker.out.types.bool();
            let got = exprs::synth(checker, ast, resolved, src, *value);
            if got != bool_ty && !checker.out.types.poisoned(got) {
                let shown = checker.show(ast, src, got);
                let at = ast.exprs[value.0 as usize].span;
                let diagnostic = errors::not_bool("an `assert`", &shown, at);
                checker.push_diagnostic(diagnostic);
            }
            (Flow::Falls, None)
        }
        StmtKind::While { cond, block: body } => {
            let bool_ty = checker.out.types.bool();
            let got = exprs::synth(checker, ast, resolved, src, *cond);
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
        StmtKind::ForIn { name, iterable, block: body } => {
            let over = exprs::synth(checker, ast, resolved, src, *iterable);
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
            checker.bind_local(*name, element);
            checker.loops += 1;
            block(checker, ast, resolved, src, body, Want::Nothing);
            checker.loops -= 1;
            (Flow::Falls, None)
        }
        StmtKind::Expr(value) => expression_statement(checker, ast, resolved, src, *value, want),
        StmtKind::Error => (Flow::Falls, None),
    }
}



/// An expression alone on a line. Two jobs: route `if`/`match` in statement
/// position (where they need no value and may have arms that all jump), and
/// enforce panel 003's rule everywhere else.
fn expression_statement(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    value: crate::syntax::ExprId,
    want: Want,
) -> (Flow, Option<TyId>) {
    let span = ast.exprs[value.0 as usize].span;
    match &ast.exprs[value.0 as usize].kind {
        ExprKind::If { branches, otherwise } if want == Want::Nothing => {
            let bool_ty = checker.out.types.bool();
            let mut all_jump = otherwise.is_some();
            for branch in branches {
                let got = exprs::synth(checker, ast, resolved, src, branch.cond);
                if got != bool_ty && !checker.out.types.poisoned(got) {
                    let shown = checker.show(ast, src, got);
                    let at = ast.exprs[branch.cond.0 as usize].span;
                    let diagnostic = errors::not_bool("an `if` condition", &shown, at);
                    checker.push_diagnostic(diagnostic);
                }
                let (flow, _) = block(checker, ast, resolved, src, &branch.block, Want::Nothing);
                all_jump = all_jump && flow == Flow::Jumps;
            }
            if let Some(block_body) = otherwise {
                let (flow, _) = block(checker, ast, resolved, src, block_body, Want::Nothing);
                all_jump = all_jump && flow == Flow::Jumps;
            }
            let flow = if all_jump { Flow::Jumps } else { Flow::Falls };
            // As a statement it has the type a statement must have (§4.14).
            let unit = checker.out.types.unit();
            checker.record(value, unit);
            (flow, None)
        }
        ExprKind::Match { scrutinee, arms: match_arms } if want == Want::Nothing => {
            let subject = exprs::synth(checker, ast, resolved, src, *scrutinee);
            let branches = super::arms::arms(
                checker,
                ast,
                resolved,
                src,
                match_arms,
                subject,
                Want::Nothing,
                span,
            );
            let all_jump =
                !branches.is_empty() && branches.iter().all(|b| b.value.is_none());
            let flow = if all_jump { Flow::Jumps } else { Flow::Falls };
            let unit = checker.out.types.unit();
            checker.record(value, unit);
            (flow, None)
        }
        _ => {
            // A value block's last statement is checked against what the block
            // is *for*, which is what carries an expectation into an arm — the
            // ⇐-only forms (`ok`, `fail`, `.case`, `[]`) live or die on this.
            if let Want::Value(expected) = want {
                expect::check(checker, ast, resolved, src, value, expected);
                return (Flow::Falls, Some(expected));
            }
            let got = exprs::synth(checker, ast, resolved, src, value);
            if want == Want::Unknown {
                return (Flow::Falls, Some(got));
            }
            // Panel 003: the value of a statement is discarded, and discarding
            // one silently is the mistake. `_ = expr` says it on purpose.
            let unit = checker.out.types.unit();
            if got != unit && !checker.out.types.poisoned(got) {
                let shown = checker.show(ast, src, got);
                let diagnostic = errors::discarded_value(&shown, span);
                checker.push_diagnostic(diagnostic);
            }
            (Flow::Falls, None)
        }
    }
}
