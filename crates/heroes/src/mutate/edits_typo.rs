//! The same one-character slip, in three places the compiler has different
//! authority over (design.md Part 11; `harness/mutations/operators.md`).
//!
//! Split out of `edits.rs` by the §11 sweep, and the three belong together because
//! **the mutation is identical and only the target changes** — which is what turns
//! them from three operators into one measurement:
//!
//! - an **identifier**: the resolver catches it, every time, with a `certain` fix.
//! - an error **code**: nothing catches it. §4.6 fixes the error's payload at two
//!   `str`s and says to assert on the code, so the two ends of that contract are a
//!   pair of string literals in different files, checked by nothing. This is the
//!   one operator of the eleven that points at no rule which kills it, and that is
//!   the finding rather than an oversight.
//! - a **constant's digit**: the site is narrow on purpose, and the narrowing is a
//!   fact about the value rather than a premise about the program (CLAUDE.md §11).
//!   A number inside an expression is a choice the program is making; a `constant`
//!   exists to NAME a number, and where that number was copied from somewhere else,
//!   the somewhere else is an authority the compiler could have consulted and did
//!   not.

use crate::source::{Source, Span};
use crate::syntax::{Ast, BinaryOp, DeclKind, ExprId, ExprKind, StmtKind};

use super::edits::edit;

pub(super) fn typo_ident(ast: &Ast, src: &Source) -> Vec<String> {
    let mut out = Vec::new();
    for expr in &ast.exprs {
        if !matches!(expr.kind, ExprKind::Name) {
            continue;
        }
        let name = src.slice(expr.span);
        if name.len() < 3 {
            continue;
        }
        out.push(edit(src, expr.span, &typo(name)));
    }
    out
}

/// The same one-character slip, inside an error **code**: `fail("unknown_char",
/// …)` and the `e.code == "unknown_char"` that reads it back.
///
/// §4.6 fixes the error's payload at two `str`s and says to assert on the code,
/// so the two ends of that contract are a pair of string literals in different
/// files, checked by nothing. This operator is what measures the gap; unlike its
/// ten siblings it points at no rule that kills it, which is the finding rather
/// than an oversight (`harness/mutations/operators.md`).
pub(super) fn typo_code(ast: &Ast, src: &Source) -> Vec<String> {
    let mut out = Vec::new();
    for expr in &ast.exprs {
        let literal = match &expr.kind {
            ExprKind::Call { callee, args } if is_name(ast, src, *callee, "fail") => {
                let Some(first) = args.first() else { continue };
                first.value
            }
            ExprKind::Binary { op: BinaryOp::Eq | BinaryOp::Ne, left, right } => {
                let Some(id) = code_side(ast, src, *left, *right) else { continue };
                id
            }
            _ => continue,
        };
        let span = ast.exprs[literal.0 as usize].span;
        let Some(code) = code_text(src, span) else { continue };
        out.push(edit(src, span, &format!("\"{}\"", typo(&code))));
    }
    out
}

/// True when `id` is written as the bare name `name` — the only spelling a
/// built-in call has, since none of them may be redeclared (spec line 157).
fn is_name(ast: &Ast, src: &Source, id: ExprId, name: &str) -> bool {
    let expr = &ast.exprs[id.0 as usize];
    matches!(expr.kind, ExprKind::Name) && src.slice(expr.span) == name
}

/// The `str` side of `e.code == "…"`, whichever side it was written on.
fn code_side(ast: &Ast, src: &Source, left: ExprId, right: ExprId) -> Option<ExprId> {
    for (literal, other) in [(left, right), (right, left)] {
        if !matches!(ast.exprs[literal.0 as usize].kind, ExprKind::Str) {
            continue;
        }
        let ExprKind::Field { name, .. } = &ast.exprs[other.0 as usize].kind else { continue };
        if src.slice(*name) == "code" {
            return Some(literal);
        }
    }
    None
}

/// A code literal's text, without its quotes. Skipped when it carries a
/// backslash — dropping the middle character of an escape measures the lexer,
/// which rule 2 excludes — and when it is too short for a slip to leave a name
/// anybody would have typed.
fn code_text(src: &Source, span: Span) -> Option<String> {
    let inner = src.slice(span).strip_prefix('"')?.strip_suffix('"')?;
    if inner.len() < 3 || inner.contains('\\') {
        return None;
    }
    Some(inner.to_string())
}

/// A `constant`'s value, off by one digit.
///
/// The site is deliberately narrow, and the narrowing is a fact about the value
/// rather than a premise about the program (CLAUDE.md §11): a number *inside an
/// expression* is a choice this program is making, and nothing outside the file
/// can contradict it — but a `constant` exists to **name** a number, and where
/// that number was copied from somewhere else, the somewhere else is an authority
/// the compiler could have consulted and did not. This row measures the gap.
pub(super) fn typo_digit(ast: &Ast, src: &Source) -> Vec<String> {
    let mut out = Vec::new();
    for decl in &ast.decls {
        // A constant with no body is an `extern constant`: its value is the
        // header's, so there is no digit in this file to move — which is the
        // whole point of the form, and the reason this operator's site count is
        // the number to report (§4.19, panel 038).
        let DeclKind::Constant { body: Some(body), .. } = &decl.kind else { continue };
        // One statement, and it is an expression: a constant computed from
        // several has no single number to be wrong about.
        if body.stmts.len() != 1 {
            continue;
        }
        let StmtKind::Expr(value) = &ast.stmts[body.stmts[0].0 as usize].kind else { continue };
        let literal = &ast.exprs[value.0 as usize];
        if !matches!(literal.kind, ExprKind::Int) {
            continue;
        }
        if let Some(slipped) = neighbouring_digit(src.slice(literal.span)) {
            out.push(edit(src, literal.span, &slipped));
        }
    }
    out
}

/// The last digit, moved by one — and the **top** digit of the base goes *down*
/// rather than carrying, because a carry changes how many digits the number has
/// and that is a different mistake.
///
/// The base has to be read, not assumed. Before M-literal-bases this function
/// tested `is_ascii_digit` and returned `None` on anything else, which was right
/// while `9` was the only top digit — and would have gone silently wrong the day
/// hexadecimal arrived: `0x10` would have been mutated and `0xff` skipped, so the
/// operator's coverage would have depended on which characters a mask happened to
/// end in, and its site count is the number panel 038 made the deliverable. A
/// separator is stepped over, and `None` where the literal does not end in a
/// digit of its own base.
fn neighbouring_digit(text: &str) -> Option<String> {
    // The prefix is lowercase because that is the only spelling the lexer
    // accepts, so a mutant built here is a program the lexer would also accept.
    let (prefix, radix) = match text.as_bytes() {
        [b'0', b'x', ..] => (2, 16),
        [b'0', b'o', ..] => (2, 8),
        [b'0', b'b', ..] => (2, 2),
        _ => (0, 10),
    };
    let mut digits: Vec<char> = text.chars().collect();
    let at = digits.iter().rposition(|c| *c != '_')?;
    if at < prefix {
        return None;
    }
    let value = digits[at].to_digit(radix)?;
    let moved = if value + 1 == radix { value - 1 } else { value + 1 };
    digits[at] = char::from_digit(moved, radix)?;
    Some(digits.into_iter().collect())
}

/// A plausible one-character slip: drop the middle character. `total` → `totl`,
/// which is §4.4's own example.
pub(super) fn typo(name: &str) -> String {
    let chars: Vec<char> = name.chars().collect();
    let middle = chars.len() / 2;
    chars
        .iter()
        .enumerate()
        .filter(|(index, _)| *index != middle)
        .map(|(_, c)| *c)
        .collect()
}

