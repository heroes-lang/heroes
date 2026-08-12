//! One function per operator: every mutant it makes from one source, as span
//! edits (design.md Part 11, `harness/mutations/operators.md`).
//!
//! Each one imitates a *plausible* mistake — a habit carried from another
//! language, a one-character slip, a forgotten piece — and each is applied to
//! every applicable site, one mutant per site, in arena order so a run is
//! reproducible.
//!
//! Every mutation is a **text edit guided by the tree**: the parser found the
//! site, the edit rewrites those bytes, and the mutant goes back through the real
//! frontend. Nothing here builds a tree by hand, which is what keeps the mutants
//! plausible — they are programs someone could have typed.

use crate::source::{Source, Span};
use crate::syntax::{Arg, Ast, BinaryOp, DeclKind, ExprId, ExprKind, StmtKind};

/// Replace a span with new text.
fn edit(src: &Source, span: Span, replacement: &str) -> String {
    let mut text = src.text.clone();
    text.replace_range(span.start as usize..span.end as usize, replacement);
    text
}

/// Swap two spans' text. Used by the inversion operator, which is the whole
/// reason §4.9's same-typed-argument rule exists.
fn swap(src: &Source, first: Span, second: Span) -> String {
    let mut text = src.text.clone();
    let a = src.slice(first).to_string();
    let b = src.slice(second).to_string();
    // Back to front, so the first span's offsets stay valid.
    text.replace_range(second.start as usize..second.end as usize, &a);
    text.replace_range(first.start as usize..first.end as usize, &b);
    text
}

pub(super) fn swap_args(ast: &Ast, src: &Source) -> Vec<String> {
    let mut out = Vec::new();
    for expr in &ast.exprs {
        let args = match &expr.kind {
            ExprKind::Call { args, .. } | ExprKind::Method { args, .. } => args,
            ExprKind::Case { args, .. } => args,
            _ => continue,
        };
        if args.len() < 2 {
            continue;
        }
        // The first two arguments, whole: label and value together, so a named
        // call stays named and an inversion of `from:`/`to:` is a real swap.
        let first = whole_arg(ast, &args[0]);
        let second = whole_arg(ast, &args[1]);
        out.push(swap(src, first, second));
    }
    out
}

/// An argument's full extent — its label, where it has one, through its value.
fn whole_arg(ast: &Ast, arg: &Arg) -> Span {
    let value = ast.exprs[arg.value.0 as usize].span;
    match arg.name {
        Some(label) => Span { start: label.start, end: value.end },
        None => value,
    }
}

pub(super) fn drop_case(ast: &Ast, src: &Source) -> Vec<String> {
    let mut out = Vec::new();
    for expr in &ast.exprs {
        let ExprKind::Match { arms, .. } = &expr.kind else { continue };
        if arms.len() < 2 {
            continue;
        }
        // The last arm, and the newline before it: deleting a middle arm would
        // leave the indentation the parser reads, which is a different mutation.
        let arm = &arms[arms.len() - 1];
        let start = line_start(src, arm.span.start);
        out.push(edit(src, Span { start, end: arm.span.end }, ""));
    }
    out
}

pub(super) fn forget_at_decl(ast: &Ast, src: &Source) -> Vec<String> {
    let mut out = Vec::new();
    for stmt in &ast.stmts {
        let StmtKind::Declare { name, value, .. } = &stmt.kind else { continue };
        // `v: int @ 0` becomes `v = 0`: the annotation and the marker go, and the
        // later `v @ …` lines are left alone, which is the mistake.
        let value_span = ast.exprs[value.0 as usize].span;
        let span = Span { start: name.start, end: value_span.start };
        out.push(edit(src, span, &format!("{} = ", src.slice(*name))));
    }
    out
}

pub(super) fn mutate_undeclared(ast: &Ast, src: &Source) -> Vec<String> {
    let mut out = Vec::new();
    for stmt in &ast.stmts {
        let StmtKind::Mutate { place, .. } = &stmt.kind else { continue };
        let target = &ast.exprs[place.0 as usize];
        if !matches!(target.kind, ExprKind::Name) {
            continue;
        }
        let name = src.slice(target.span);
        out.push(edit(src, target.span, &typo(name)));
    }
    out
}

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

pub(super) fn wildcard_variant(ast: &Ast, src: &Source) -> Vec<String> {
    let mut out = Vec::new();
    for expr in &ast.exprs {
        let ExprKind::Match { arms, .. } = &expr.kind else { continue };
        for arm in arms {
            let Some(pattern) = arm.patterns.first() else { continue };
            if !matches!(pattern.kind, crate::syntax::PatternKind::Case { .. }) {
                continue;
            }
            let last = arm.patterns[arm.patterns.len() - 1].span;
            out.push(edit(src, Span { start: pattern.span.start, end: last.end }, "_"));
        }
    }
    out
}

pub(super) fn positional_named(ast: &Ast, src: &Source) -> Vec<String> {
    let mut out = Vec::new();
    for expr in &ast.exprs {
        let args = match &expr.kind {
            ExprKind::Call { args, .. } | ExprKind::Method { args, .. } => args,
            ExprKind::Case { args, .. } => args,
            _ => continue,
        };
        if !args.iter().any(|a| a.name.is_some()) {
            continue;
        }
        // Every label at once: the Python habit is not to write half of them.
        let mut text = src.text.clone();
        for arg in args.iter().rev() {
            if let Some(label) = arg.name {
                let value = ast.exprs[arg.value.0 as usize].span;
                text.replace_range(label.start as usize..value.start as usize, "");
            }
        }
        out.push(text);
    }
    out
}

pub(super) fn mix_int_float(ast: &Ast, src: &Source) -> Vec<String> {
    let mut out = Vec::new();
    for expr in &ast.exprs {
        let ExprKind::Binary { left, right, .. } = &expr.kind else { continue };
        for side in [left, right] {
            let operand = &ast.exprs[side.0 as usize];
            // A **decimal** `int` only. Appending `.0` to `0xff` produces
            // `0xff.0`, which dies in the lexer — so an operator named for an
            // implicit-conversion prior would have been measuring the scanner
            // instead, and scoring a kill it did not earn (panel 041, and the
            // narrowing is a fact about the characters in hand rather than a
            // premise about the corpus: CLAUDE.md §11).
            let text = src.slice(operand.span);
            let based = text.len() > 1 && text.as_bytes()[0] == b'0';
            if matches!(operand.kind, ExprKind::Int) && !based {
                out.push(edit(src, operand.span, &format!("{text}.0")));
            }
        }
    }
    out
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

pub(super) fn shadow(ast: &Ast, src: &Source) -> Vec<String> {
    let mut out = Vec::new();
    for stmt in &ast.stmts {
        let StmtKind::Bind { name, .. } = &stmt.kind else { continue };
        let start = line_start(src, stmt.span.start);
        let indent = src.slice(Span { start, end: name.start }).to_string();
        let line = src.slice(Span { start, end: stmt.span.end }).to_string();
        // The same binding again, immediately after: the inner-scope habit
        // without an inner scope.
        let mut text = src.text.clone();
        let at = stmt.span.end as usize;
        if at > text.len() {
            continue;
        }
        text.insert_str(at, &format!("\n{}", line.trim_end()));
        let _ = indent;
        out.push(text);
    }
    out
}

pub(super) fn drop_question(ast: &Ast, src: &Source) -> Vec<String> {
    let mut out = Vec::new();
    for expr in &ast.exprs {
        let ExprKind::Try(_) = &expr.kind else { continue };
        // The `?` is the last byte of the expression.
        let span = Span { start: expr.span.end - 1, end: expr.span.end };
        if src.slice(span) != "?" {
            continue;
        }
        out.push(edit(src, span, ""));
    }
    out
}

/// A plausible one-character slip: drop the middle character. `total` → `totl`,
/// which is §4.4's own example.
fn typo(name: &str) -> String {
    let chars: Vec<char> = name.chars().collect();
    let middle = chars.len() / 2;
    chars
        .iter()
        .enumerate()
        .filter(|(index, _)| *index != middle)
        .map(|(_, c)| *c)
        .collect()
}

fn line_start(src: &Source, at: u32) -> u32 {
    let bytes = src.text.as_bytes();
    let mut start = at as usize;
    while start > 0 && bytes[start - 1] != b'\n' {
        start -= 1;
    }
    start as u32
}
