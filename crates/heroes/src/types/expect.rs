//! ⇐ — check an expression against a type the context already knows (§4.5).
//!
//! This is the half of bidirectional checking that makes four forms writable at
//! all, and each of them is in the language for a reason:
//!
//! | form | why it cannot synthesise |
//! |------|--------------------------|
//! | `[]`, `{}` | an empty literal does not say what it holds (§4.5) |
//! | `ok(x)`, `fail(c, m)` | which `T?` is the caller's question (panel 002) |
//! | `.case` | which variant it belongs to comes from context (§4.5) |
//! | `???` | it has no type; it *reports* the one expected (§4.16) |
//!
//! Everything else checks by synthesising and comparing — and an `if` or a
//! `match` pushes the expectation down into every branch, so that a `.case` or an
//! `ok(…)` can be written *inside* an arm and the error lands on the branch that
//! disagrees rather than on the whole construct.

use crate::resolve::Resolved;
use crate::source::{Source, Span};
use crate::syntax::{Ast, ExprId, ExprKind, UnaryOp};

use super::exprs::synth;
use super::stmts::Want;
use super::table::Ty;
use super::{construct, errors, stmts, Checker, Hole, TyId};



/// ⇐ — check an expression against a type the context already knows.
pub(super) fn check(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    id: ExprId,
    expected: TyId,
) {
    let span = ast.exprs[id.0 as usize].span;
    match &ast.exprs[id.0 as usize].kind {
        // §4.16: the hole reports the type it was checked against. This is the
        // information the compiler already had and used to throw away.
        ExprKind::Hole => {
            checker.out.holes.push(Hole { at: id, span, expected });
            checker.record(id, expected);
        }
        // **A number literal, checked against the width the context asked for.**
        // This is the one place the value in the source and the type in the
        // annotation meet, so it is the one place that can say `300` does not fit
        // a `u8` — and it says it with the range, because a reader who wrote 300
        // needs to know what would have fitted (§4.17).
        //
        // A character literal shares the arm: §4.3 makes it a number whose value
        // is one ASCII character, so it fits every width and the check is the
        // same one.
        ExprKind::Int | ExprKind::Char | ExprKind::Unary { op: UnaryOp::Neg, .. } => {
            match checker.out.types.get(expected) {
                Ty::Int(kind) => {
                    let text = src.slice(span);
                    let value = literal_value(ast, src, id);
                    let (low, high) = kind.range();
                    let fits = value.is_some_and(|v| v >= i128::from(low) && v <= i128::from(high));
                    if !fits {
                        let diagnostic = crate::lexer::out_of_range(text, span, Some(kind));
                        checker.push_diagnostic(diagnostic);
                    }
                    // The digits under a minus sign need the type too: the
                    // lowering walks to them and would otherwise find no type
                    // recorded, which reaches the emitter as `HeroValue` — a
                    // clang error naming a type nobody wrote. Recorded rather
                    // than re-checked, because the range question was answered
                    // above with the sign applied, and `128` on its own does not
                    // fit the `i8` that `-128` does.
                    let mut at = id;
                    while let ExprKind::Unary { op: UnaryOp::Neg, operand } =
                        &ast.exprs[at.0 as usize].kind
                    {
                        at = *operand;
                        checker.record(at, expected);
                    }
                    checker.record(id, expected);
                }
                _ => {
                    let got = checker.out.types.int();
                    let shown = checker.show(ast, src, got);
                    mismatch(checker, ast, src, expected, &shown, span);
                    checker.record(id, got);
                }
            }
        }
        // §4.5's two inference failures, answered instead of reported: the
        // annotation *is* the expected type.
        ExprKind::Array(items) if items.is_empty() => {
            if !matches!(checker.out.types.get(expected), Ty::Array(_)) {
                mismatch(checker, ast, src, expected, "[]", span);
            }
            checker.record(id, expected);
        }
        ExprKind::Map(entries) if entries.is_empty() => {
            if !matches!(checker.out.types.get(expected), Ty::Map(_, _)) {
                mismatch(checker, ast, src, expected, "{}", span);
            }
            checker.record(id, expected);
        }
        ExprKind::Array(items) => {
            if let Ty::Array(element) = checker.out.types.get(expected) {
                for item in items {
                    check(checker, ast, resolved, src, *item, element);
                }
                checker.record(id, expected);
                return;
            }
            let got = synth(checker, ast, resolved, src, id);
            compare(checker, ast, src, expected, got, span);
        }
        ExprKind::Case { name: case, args } => {
            construct::case(checker, ast, resolved, src, id, *case, args, expected);
        }
        ExprKind::Call { callee, args } => {
            // `ok`/`fail` are ⇐-only (panel 002): which `T?` they make is the
            // caller's question, so they are checked here and nowhere else.
            if let Some(ty) =
                construct::fallible_constructor(checker, ast, resolved, src, *callee, args, expected, span)
            {
                checker.record(id, ty);
                return;
            }
            let got = synth(checker, ast, resolved, src, id);
            compare(checker, ast, src, expected, got, span);
        }
        // An `if` or `match` in ⇐ position pushes the expectation into every
        // branch, so the error lands in the branch that disagrees — and so that
        // a `.case`, an `ok(…)` or a `[]` can be written in an arm at all.
        ExprKind::If { branches, otherwise } => {
            let bool_ty = checker.out.types.bool();
            for branch in branches {
                let cond = synth(checker, ast, resolved, src, branch.cond);
                if cond != bool_ty && !checker.out.types.poisoned(cond) {
                    let got = checker.show(ast, src, cond);
                    let at = ast.exprs[branch.cond.0 as usize].span;
                    let diagnostic = errors::not_bool("an `if` condition", &got, at);
                    checker.push_diagnostic(diagnostic);
                }
                stmts::block(checker, ast, resolved, src, &branch.block, Want::Value(expected));
            }
            match otherwise {
                Some(block) => {
                    stmts::block(checker, ast, resolved, src, block, Want::Value(expected));
                }
                None => {
                    let diagnostic = errors::if_without_else(span);
                    checker.push_diagnostic(diagnostic);
                }
            }
            checker.record(id, expected);
        }
        ExprKind::Match { scrutinee, arms } => {
            let subject = synth(checker, ast, resolved, src, *scrutinee);
            let branches = super::arms::arms(
                checker,
                ast,
                resolved,
                src,
                arms,
                subject,
                Want::Value(expected),
                span,
            );
            if !branches.is_empty() && branches.iter().all(|b| b.value.is_none()) {
                let diagnostic = errors::no_value("match", span);
                checker.push_diagnostic(diagnostic);
            }
            checker.record(id, expected);
        }
        _ => {
            let got = synth(checker, ast, resolved, src, id);
            compare(checker, ast, src, expected, got, span);
        }
    }
}



fn compare(
    checker: &mut Checker,
    ast: &Ast,
    src: &Source,
    expected: TyId,
    got: TyId,
    span: Span,
) {
    if expected == got
        || checker.out.types.poisoned(expected)
        || checker.out.types.poisoned(got)
    {
        return;
    }
    let (a, b) = (checker.show(ast, src, expected), checker.show(ast, src, got));
    let mut diagnostic = errors::mismatch(&a, &b, span);
    // **A `T?` where a `T` was wanted has one obvious repair, and it is a guess**
    // (panel 043). `.must()` aborts on the error case, and whether that is what
    // the author wants is theirs to say — `?`, `.default(v)` and a `match` are
    // the other three answers §4.6 gives. So the fix is offered and not applied,
    // which is exactly CLAUDE.md §8's line between `certain` and `guess`.
    //
    // This is the other half of the `fit_<width>` repair: the widening case gets
    // a `certain` fix removing a `.must()`, the narrowing case gets a `guess`
    // adding one, and between them the extra hop the rule costs is answered in
    // the diagnostic rather than in the reader's head.
    if checker.out.types.get(got) == crate::types::Ty::Fallible(expected) {
        diagnostic.fixes.push(crate::diagnostics::Fix {
            title: format!("`.must()` — abort on the error case, giving `{a}`"),
            replacement: format!("{}.must()", src.slice(span)),
            span,
            certainty: crate::diagnostics::Certainty::Guess,
        });
    }
    checker.push_diagnostic(diagnostic);
}



fn mismatch(checker: &mut Checker, ast: &Ast, src: &Source, expected: TyId, got: &str, span: Span) {
    let a = checker.show(ast, src, expected);
    let diagnostic = errors::mismatch(&a, got, span);
    checker.push_diagnostic(diagnostic);
}


/// The value a literal denotes, wide enough for every width, with the minus sign
/// applied where there is one.
///
/// The sign has to be folded in **here** rather than checked separately: `-128`
/// fits an `i8` and `128` does not, so a check that saw them one at a time would
/// refuse the only way to write that type's lowest value.
fn literal_value(ast: &Ast, src: &Source, id: ExprId) -> Option<i128> {
    let node = &ast.exprs[id.0 as usize];
    match &node.kind {
        ExprKind::Int => crate::lexer::decode_wide(src.slice(node.span)),
        // §4.3: a character literal *is* a number, and its value is one ASCII
        // character — so it fits every width and needs no special range.
        ExprKind::Char => Some(i128::from(
            crate::lexer::unescape(src, node.span).chars().next().unwrap_or('\0') as u32,
        )),
        ExprKind::Unary { op: UnaryOp::Neg, operand } => {
            literal_value(ast, src, *operand).map(|v| -v)
        }
        _ => None,
    }
}
