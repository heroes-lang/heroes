//! The ten operators of `harness/mutations/operators.md`, as span edits.
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
use crate::syntax::{parse, Ast, ExprKind, StmtKind};

pub struct Operator {
    pub id: &'static str,
    /// The plausible-mistake class, from the operators table.
    pub imitates: &'static str,
}

pub const OPERATORS: [Operator; 10] = [
    Operator { id: "swap-args", imitates: "classic LLM argument inversion" },
    Operator { id: "drop-case", imitates: "a forgotten variant case" },
    Operator { id: "forget-at-decl", imitates: "mutability confusion" },
    Operator { id: "mutate-undeclared", imitates: "a silent new variable in other languages" },
    Operator { id: "typo-ident", imitates: "a one-character edit" },
    Operator { id: "wildcard-variant", imitates: "a lazy catch-all" },
    Operator { id: "positional-named", imitates: "style transfer from Python" },
    Operator { id: "mix-int-float", imitates: "an implicit-conversion prior" },
    Operator { id: "shadow", imitates: "an inner-scope habit" },
    Operator { id: "drop-question", imitates: "forgotten error propagation" },
];

/// Every mutant one operator makes from one source.
pub fn apply(id: &str, name: &str, text: &str) -> Vec<String> {
    let src = Source::new(name.to_string(), text.to_string());
    let parsed = parse(&src);
    if !parsed.diagnostics.is_empty() {
        return Vec::new();
    }
    let ast = &parsed.ast;
    match id {
        "swap-args" => swap_args(ast, &src),
        "drop-case" => drop_case(ast, &src),
        "forget-at-decl" => forget_at_decl(ast, &src),
        "mutate-undeclared" => mutate_undeclared(ast, &src),
        "typo-ident" => typo_ident(ast, &src),
        "wildcard-variant" => wildcard_variant(ast, &src),
        "positional-named" => positional_named(ast, &src),
        "mix-int-float" => mix_int_float(ast, &src),
        "shadow" => shadow(ast, &src),
        "drop-question" => drop_question(ast, &src),
        _ => Vec::new(),
    }
}

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

fn swap_args(ast: &Ast, src: &Source) -> Vec<String> {
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
fn whole_arg(ast: &Ast, arg: &crate::syntax::Arg) -> Span {
    let value = ast.exprs[arg.value.0 as usize].span;
    match arg.name {
        Some(label) => Span { start: label.start, end: value.end },
        None => value,
    }
}

fn drop_case(ast: &Ast, src: &Source) -> Vec<String> {
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

fn forget_at_decl(ast: &Ast, src: &Source) -> Vec<String> {
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

fn mutate_undeclared(ast: &Ast, src: &Source) -> Vec<String> {
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

fn typo_ident(ast: &Ast, src: &Source) -> Vec<String> {
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

fn wildcard_variant(ast: &Ast, src: &Source) -> Vec<String> {
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

fn positional_named(ast: &Ast, src: &Source) -> Vec<String> {
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

fn mix_int_float(ast: &Ast, src: &Source) -> Vec<String> {
    let mut out = Vec::new();
    for expr in &ast.exprs {
        let ExprKind::Binary { left, right, .. } = &expr.kind else { continue };
        for side in [left, right] {
            let operand = &ast.exprs[side.0 as usize];
            if matches!(operand.kind, ExprKind::Int) {
                out.push(edit(src, operand.span, &format!("{}.0", src.slice(operand.span))));
            }
        }
    }
    out
}

fn shadow(ast: &Ast, src: &Source) -> Vec<String> {
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

fn drop_question(ast: &Ast, src: &Source) -> Vec<String> {
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
