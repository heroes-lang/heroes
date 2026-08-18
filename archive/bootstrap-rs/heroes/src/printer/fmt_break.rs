//! The one decision a formatter cannot avoid: a line that is too long (panel 007,
//! design.md §4.4, §4.9).
//!
//! **Breaking only happens inside brackets**, because panel 007 allows no other
//! continuation: at bracket depth zero a long expression is broken in parentheses or not
//! at all. So a too-long line breaks its outermost list (`[…]`, `{…}`) by newline, per
//! §4.9, or its outermost argument list by comma — and nothing else.
//!
//! A statement that is still too long **stays long**. That is the honest outcome rather
//! than a failure: there is nowhere legal to put the break, and a formatter that invents
//! one invents a continuation the language does not have.

use crate::source::{Source, Span};
use crate::syntax::{Arg, ArmBody, Ast, ExprId, ExprKind, MapEntry};

use super::fmt::{one_line, Fmt, WIDTH};
use super::fmt_expr::{arg_text, render};
use super::fmt_arms::{arm_alignment, arm_heads};
use super::fmt_fits::spans_lines;

impl Fmt {
    /// A statement's head plus its value: on one line when it fits, broken
    /// inside its outermost bracket when it does not, and continued below when
    /// the value is an `if` or a `match`.
    pub(super) fn valued(
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
                    self.last_line = src.line_of(ast.exprs[branch.cond.0 as usize].span.end);
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
                self.last_line = src.line_of(ast.exprs[scrutinee.0 as usize].span.end);
                let lefts = arm_heads(ast, src, arms);
                let pads = arm_alignment(ast, src, arms, &lefts, indent + 4);
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
                    let line = src.line_of(arm.span.start);
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
                if indent + text.chars().count() <= WIDTH && !spans_lines(ast, src, value) {
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
