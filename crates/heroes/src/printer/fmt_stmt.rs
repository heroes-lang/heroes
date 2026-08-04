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
            // past its own body. One that did not, ends where it started.
            //
            // Its span cannot answer this: a statement ending in a block ends
            // at a `Dedent`, whose span sits on the *next* line — which is
            // exactly how two blank lines went missing the first time.
            if self.last_line < line {
                self.last_line = line;
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
                self.line(indent, &format!("{head}for {}", render(ast, src, *cond)));
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
                for arm in arms {
                    let patterns: Vec<String> = arm
                        .patterns
                        .iter()
                        .map(|pattern| super::bodies::render_pattern_public(ast, src, pattern))
                        .collect();
                    let left = patterns.join(" | ");
                    let (line, _) = src.line_col(arm.span.start);
                    self.comments_before(src, comments, line, indent + 4);
                    match &arm.body {
                        ArmBody::Stmt(id) => {
                            self.statement(
                                ast,
                                src,
                                comments,
                                *id,
                                indent + 4,
                                &format!("{left} => "),
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
                            self.line(indent + 4, &format!("{left} =>"));
                            self.last_line = line;
                            self.block(ast, src, comments, block, indent + 8);
                        }
                    }
                }
            }
            _ => {
                let text = one_line(ast, src, head, value);
                if indent + text.len() <= WIDTH {
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
