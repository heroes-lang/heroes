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

use super::edits_typo::typo;
use crate::syntax::{Arg, Ast, ExprKind, StmtKind};

/// Replace a span with new text.
pub(super) fn edit(src: &Source, span: Span, replacement: &str) -> String {
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
        // `v: i64 @ 0` becomes `v = 0`: the annotation and the marker go, and the
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
            // A **decimal** `i64` only. Appending `.0` to `0xff` produces
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

fn line_start(src: &Source, at: u32) -> u32 {
    let bytes = src.text.as_bytes();
    let mut start = at as usize;
    while start > 0 && bytes[start - 1] != b'\n' {
        start -= 1;
    }
    start as u32
}
