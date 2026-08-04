//! The statement half of `heroes fmt`: blocks, the three line shapes of
//! §4.4, the control forms, and the one decision a formatter cannot avoid —
//! when a line is too long.
//!
//! Breaking only happens **inside brackets**, because panel 007 allows no
//! other continuation: at bracket depth zero a long expression is broken in
//! parentheses or not at all. So a too-long line breaks its outermost list
//! (`[…]`, `{…}`) by newline, per §4.9, or its outermost argument list by
//! comma — and nothing else. A statement that is still too long stays long,
//! which is honest: there is nowhere legal to put the break.

use crate::source::{Source, Span};
use crate::syntax::{Arg, ArmBody, Ast, Block, ExprId, ExprKind, MapEntry, StmtKind};

use super::fmt::{one_line, Fmt, WIDTH};
use super::fmt_expr::{arg_text, render};
use super::types::render_type;

impl Fmt {
    pub(super) fn block(
        &mut self,
        ast: &Ast,
        src: &Source,
        comments: &[Span],
        block: &Block,
        indent: usize,
    ) {
        for (i, id) in block.stmts.iter().enumerate() {
            let stmt = &ast.stmts[id.0 as usize];
            let (line, _) = src.line_col(stmt.span.start);
            self.comments_before(src, comments, line, indent);
            // A blank line the author left between two statements survives —
            // it is the only grouping a body has. Never before the *first*
            // statement of a block: a block does not open on an empty line.
            if i > 0 && self.last_line > 0 && line > self.last_line + 1 {
                self.blank_line();
            }
            self.statement(ast, src, comments, *id, indent, "");
            // A statement that carried a block has already moved `last_line`
            // past its own body. One that did not, ends where it started —
            // *unless* its value is a list literal written down the page, which
            // ends several lines lower and, until this was added, made the rule
            // below invent a blank line after every such list.
            //
            // The statement's own `span` cannot answer this and must not be
            // used: a statement ending in a block ends at a `Dedent` whose span
            // sits on the *next* line, and trusting it made `trailing_comment`
            // steal the following declaration's doc comment — twice, once in
            // 2026-08-04's first attempt at this very line.
            let end = literal_end_line(ast, src, stmt).unwrap_or(line);
            if self.last_line < end {
                self.last_line = end;
            }
            self.trailing_comment(src, comments, self.last_line);
        }
    }

    /// One statement, optionally behind a `head`. A `match` arm passes its
    /// patterns as the head, which is what routes an arm whose body is an `if`
    /// through `valued()` — the path that knows how to print a block under a
    /// control head. Before panel 014 an inline arm body had its own path, and
    /// that path printed `.a => if c` and **dropped the branches**.
    fn statement(
        &mut self,
        ast: &Ast,
        src: &Source,
        comments: &[Span],
        id: crate::syntax::StmtId,
        indent: usize,
        head: &str,
    ) {
        match &ast.stmts[id.0 as usize].kind {
            StmtKind::Bind { name, ty, value } => {
                let annotation = match ty {
                    Some(ty) => format!(": {}", render_type(ast, *ty, src)),
                    None => String::new(),
                };
                let head = format!("{head}{}{annotation} = ", src.slice(*name));
                self.valued(ast, src, comments, &head, *value, indent);
            }
            StmtKind::Declare { name, ty, value } => {
                let head =
                    format!("{head}{}: {} @ ", src.slice(*name), render_type(ast, *ty, src));
                self.valued(ast, src, comments, &head, *value, indent);
            }
            StmtKind::Mutate { place, value } => {
                let head = format!("{head}{} @ ", render(ast, src, *place));
                self.valued(ast, src, comments, &head, *value, indent);
            }
            StmtKind::Return(Some(value)) => {
                self.valued(ast, src, comments, &format!("{head}return "), *value, indent)
            }
            StmtKind::Return(None) => self.line(indent, &format!("{head}return")),
            StmtKind::Break => self.line(indent, &format!("{head}break")),
            StmtKind::Continue => self.line(indent, &format!("{head}continue")),
            StmtKind::Assert(value) => {
                self.valued(ast, src, comments, &format!("{head}assert "), *value, indent)
            }
            StmtKind::While { cond, block } => {
                self.line(indent, &format!("{head}while {}", render(ast, src, *cond)));
                self.last_line = src.line_col(ast.exprs[cond.0 as usize].span.end).0;
                self.block(ast, src, comments, block, indent + 4);
            }
            StmtKind::ForIn { name, iterable, block } => {
                let head = format!(
                    "{head}for {} in {}",
                    src.slice(*name),
                    render(ast, src, *iterable)
                );
                self.line(indent, &head);
                self.last_line = src.line_col(ast.exprs[iterable.0 as usize].span.end).0;
                self.block(ast, src, comments, block, indent + 4);
            }
            StmtKind::Expr(value) => self.valued(ast, src, comments, head, *value, indent),
            // Unreachable: `fmt` refuses a file with diagnostics.
            StmtKind::Error => self.line(indent, "???"),
        }
    }

    /// A statement's head plus its value: on one line when it fits, broken
    /// inside its outermost bracket when it does not, and continued below when
    /// the value is an `if` or a `match`.
    fn valued(
        &mut self,
        ast: &Ast,
        src: &Source,
        comments: &[Span],
        head: &str,
        value: ExprId,
        indent: usize,
    ) {
        match &ast.exprs[value.0 as usize].kind {
            ExprKind::If { branches, otherwise } => {
                for (i, branch) in branches.iter().enumerate() {
                    let keyword = if i == 0 { head.to_string() } else { "else ".to_string() };
                    let cond = render(ast, src, branch.cond);
                    self.line(indent, &format!("{keyword}if {cond}"));
                    self.last_line = src.line_col(ast.exprs[branch.cond.0 as usize].span.end).0;
                    self.block(ast, src, comments, &branch.block, indent + 4);
                }
                if let Some(block) = otherwise {
                    self.line(indent, "else");
                    self.block(ast, src, comments, block, indent + 4);
                }
            }
            ExprKind::Match { scrutinee, arms } => {
                let head = format!("{head}match {}", render(ast, src, *scrutinee));
                self.line(indent, &head);
                self.last_line = src.line_col(ast.exprs[scrutinee.0 as usize].span.end).0;
                let lefts = arm_heads(ast, src, arms);
                let pads = arm_alignment(src, arms, &lefts);
                // A blank line between arms is content, exactly as it is between
                // statements: it is how a long `match` shows which cases belong
                // together, and it is what stops the alignment run — so deleting
                // it would contradict the padding computed above.
                //
                // Tracked here rather than through `last_line`, which an arm
                // deliberately clamps to its own first line so `trailing_comment`
                // cannot reach past it. The comparison is against where the
                // previous arm *ended*, not where it began: an inline arm body can
                // be an `if` with two blocks under it, and that arm is five lines
                // long while its head is one.
                let mut previous_end: Option<u32> = None;
                for (index, arm) in arms.iter().enumerate() {
                    let left = &lefts[index];
                    let (line, _) = src.line_col(arm.span.start);
                    self.comments_before(src, comments, line, indent + 4);
                    if let Some(previous) = previous_end {
                        if line > previous + 1 {
                            self.blank_line();
                        }
                    }
                    previous_end = Some(src.line_col(arm.span.end.saturating_sub(1)).0);
                    match &arm.body {
                        ArmBody::Stmt(id) => {
                            let width = pads[index];
                            self.statement(
                                ast,
                                src,
                                comments,
                                *id,
                                indent + 4,
                                &format!("{left:<width$} => "),
                            );
                            // The arm's own line, NOT `arm.span.end`: that span
                            // now reaches the terminator, and a `last_line` one
                            // line too far made `trailing_comment` steal the
                            // *next* declaration's doc comment and glue it here,
                            // where §4.1 adjacency then demoted it to a remark.
                            self.last_line = line;
                            self.trailing_comment(src, comments, self.last_line);
                        }
                        ArmBody::Block(block) => {
                            // A block arm never pads: its `=>` ends the line, so
                            // there is nothing to line up with.
                            self.line(indent + 4, &format!("{left} =>"));
                            self.last_line = line;
                            self.block(ast, src, comments, block, indent + 8);
                        }
                    }
                }
            }
            _ => {
                let text = one_line(ast, src, head, value);
                if indent + text.len() <= WIDTH && !spans_lines(ast, src, value) {
                    return self.line(indent, &text);
                }
                self.broken(ast, src, head, value, indent);
            }
        }
    }

    /// The too-long case. Only the outermost bracket of the value is opened
    /// up; if the value has none, the line stays long — panel 007 leaves
    /// nowhere else to break it.
    fn broken(&mut self, ast: &Ast, src: &Source, head: &str, value: ExprId, indent: usize) {
        match &ast.exprs[value.0 as usize].kind {
            // §4.9: a multi-line list separates by newline, not by comma.
            ExprKind::Array(items) => {
                self.line(indent, &format!("{head}["));
                for item in items {
                    self.line(indent + 4, &render(ast, src, *item));
                }
                self.line(indent, "]");
            }
            ExprKind::Map(entries) => {
                self.line(indent, &format!("{head}{{"));
                for MapEntry { key, value } in entries {
                    let text =
                        format!("{}: {}", render(ast, src, *key), render(ast, src, *value));
                    self.line(indent + 4, &text);
                }
                self.line(indent, "}");
            }
            // A call keeps its commas: §4.9's newline rule is about literals,
            // and a signature or call written across lines is still a
            // comma-separated list (the parser requires it).
            ExprKind::Call { callee, args } => {
                let open = format!("{head}{}(", render(ast, src, *callee));
                self.broken_args(ast, src, &open, args, indent);
            }
            ExprKind::Method { receiver, name, args } => {
                let open = format!(
                    "{head}{}.{}(",
                    render(ast, src, *receiver),
                    src.slice(*name)
                );
                self.broken_args(ast, src, &open, args, indent);
            }
            ExprKind::Case { name, args } if !args.is_empty() => {
                let open = format!("{head}.{}(", src.slice(*name));
                self.broken_args(ast, src, &open, args, indent);
            }
            _ => {
                let text = one_line(ast, src, head, value);
                self.line(indent, &text);
            }
        }
    }

    fn broken_args(
        &mut self,
        ast: &Ast,
        src: &Source,
        open: &str,
        args: &[Arg],
        indent: usize,
    ) {
        self.line(indent, open);
        let last = args.len().saturating_sub(1);
        for (i, arg) in args.iter().enumerate() {
            let comma = if i == last { "" } else { "," };
            self.line(indent + 4, &format!("{}{comma}", arg_text(ast, src, arg)));
        }
        self.line(indent, ")");
    }
}

/// The left side of every arm — its patterns, joined by `|`.
fn arm_heads(ast: &Ast, src: &Source, arms: &[crate::syntax::Arm]) -> Vec<String> {
    arms.iter()
        .map(|arm| {
            let patterns: Vec<String> = arm
                .patterns
                .iter()
                .map(|pattern| super::bodies::render_pattern_public(ast, src, pattern))
                .collect();
            patterns.join(" | ")
        })
        .collect()
}

/// How wide each arm's left side is printed, so that `=>` lines up.
///
/// A `match` is the language's most distinctive construct and its arms are a
/// table: the patterns are the keys and the bodies are the values. Lining up the
/// arrow is the same policy gofmt applies to adjacent trailing comments, and it
/// is what design.md's own appendix does by hand — so the reference aesthetic
/// and the canonical form are now the same thing.
///
/// A run stops at a **block-bodied arm** (its `=>` ends the line, so there is
/// nothing to align with) and at a **blank line** between arms, because blank
/// lines are content in this formatter and a group the author separated is two
/// groups. Every other arm in the run is padded to the widest left side in it.
fn arm_alignment(src: &Source, arms: &[crate::syntax::Arm], lefts: &[String]) -> Vec<usize> {
    let mut pads = vec![0usize; arms.len()];
    let mut run: Vec<usize> = Vec::new();
    let mut previous_line = 0u32;
    for (index, arm) in arms.iter().enumerate() {
        let (line, _) = src.line_col(arm.span.start);
        let inline = matches!(arm.body, ArmBody::Stmt(_));
        let joined = !run.is_empty() && line == previous_line + 1;
        if !inline || !joined {
            close_run(&run, lefts, &mut pads);
            run.clear();
        }
        if inline {
            run.push(index);
        }
        previous_line = line;
    }
    close_run(&run, lefts, &mut pads);
    pads
}

fn close_run(run: &[usize], lefts: &[String], pads: &mut [usize]) {
    let widest = run.iter().map(|i| lefts[*i].chars().count()).max().unwrap_or(0);
    for index in run {
        pads[*index] = widest;
    }
}

/// True where the author wrote a **list literal** across more than one line.
///
/// Such a list keeps its shape even when it would fit on one, which is the same
/// policy blank lines get: the layout is content. §4.9 gives the two forms
/// different separators — newline across lines, comma on one — so an array
/// written down the page is not merely a wrapped array, it is the other spelling,
/// and a formatter that joins it deletes the grouping the author chose.
///
/// Only literals, and only the value's outermost node: a call keeps its commas
/// and is joined whenever it fits, because a signature's shape carries no
/// grouping.
fn spans_lines(ast: &Ast, src: &Source, value: ExprId) -> bool {
    let expr = &ast.exprs[value.0 as usize];
    let multi = match &expr.kind {
        ExprKind::Array(items) => items.len() > 1,
        ExprKind::Map(entries) => entries.len() > 1,
        _ => false,
    };
    if !multi {
        return false;
    }
    let (first, _) = src.line_col(expr.span.start);
    let (last, _) = src.line_col(expr.span.end.saturating_sub(1));
    last > first
}

/// The last source line a statement's own text occupies, when its value is a
/// **list literal written across lines** — and `None` otherwise.
///
/// Deliberately narrow. The blank-line rule needs to know where a statement
/// stopped, and a multi-line list is the one case where that is not the line it
/// started on. Every other case is answered by the block printer itself, and
/// asking the statement's span instead would reach past a `Dedent` onto the next
/// declaration's doc comment.
fn literal_end_line(ast: &Ast, src: &Source, stmt: &crate::syntax::Stmt) -> Option<u32> {
    let value = match &stmt.kind {
        StmtKind::Bind { value, .. }
        | StmtKind::Declare { value, .. }
        | StmtKind::Mutate { value, .. }
        | StmtKind::Return(Some(value))
        | StmtKind::Assert(value)
        | StmtKind::Expr(value) => *value,
        _ => return None,
    };
    if !spans_lines(ast, src, value) {
        return None;
    }
    Some(src.line_col(ast.exprs[value.0 as usize].span.end.saturating_sub(1)).0)
}
