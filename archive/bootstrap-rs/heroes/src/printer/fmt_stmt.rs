//! The statement walk of `heroes fmt`: a block's statements in order, with the comments
//! and blank lines the author put between them (design.md §4.4).
//!
//! **A blank line survives, because layout is content.** It is the only grouping a body
//! has, so the walk keeps one the author left between two statements — and never one
//! before the *first* statement of a block, since a block does not open on an empty
//! line. Knowing where a statement *ended* is what makes that rule work, which is why
//! `fmt_fits.rs` exists one door down.
//!
//! The §11 sweep of 2026-08-12 split the rest out: `fmt_break.rs` for a line that is too
//! long, `fmt_arms.rs` for aligning a `match`, `fmt_fits.rs` for the three measurements
//! all of them ask.

use crate::source::{Source, Span};
use crate::syntax::{Ast, Block, StmtKind};

use super::fmt::Fmt;
use super::fmt_expr::render;
use super::fmt_fits::literal_end_line;
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
            let line = src.line_of(stmt.span.start);
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
    pub(super) fn statement(
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
                self.last_line = src.line_of(ast.exprs[cond.0 as usize].span.end);
                self.block(ast, src, comments, block, indent + 4);
            }
            StmtKind::ForIn { name, iterable, block } => {
                let head = format!(
                    "{head}for {} in {}",
                    src.slice(*name),
                    render(ast, src, *iterable)
                );
                self.line(indent, &head);
                self.last_line = src.line_of(ast.exprs[iterable.0 as usize].span.end);
                self.block(ast, src, comments, block, indent + 4);
            }
            StmtKind::Expr(value) => self.valued(ast, src, comments, head, *value, indent),
            // Unreachable: `fmt` refuses a file with diagnostics.
            StmtKind::Error => self.line(indent, "???"),
        }
    }
}
