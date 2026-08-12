//! The parser: tokens → syntax tree (design.md Part 10 step 2, ROADMAP M-syntax-tree).
//!
//! Recursive descent, one file per idea:
//!
//! | file          | idea |
//! |---------------|------|
//! | `ast.rs`      | the tree: arena of types, declarations that own their members |
//! | `cursor.rs`   | movement, expectations, and where documentation comes from |
//! | `recover.rs`  | what to drop after a mistake, and its landmarks |
//! | `decl.rs`     | the top level: names, `constant`, `function`, `extern`, `test` |
//! | `data.rs`     | the two type declarations: `record` and `variant` |
//! | `members.rs`  | generics, parameters, record fields, variant cases |
//! | `stmt.rs`     | blocks and statements: the three line shapes of §4.4 |
//! | `expr.rs`     | the §4.14 precedence table, and the postfix chain |
//! | `primary.rs`  | atoms: literals, calls, `[…]`, `{…}`, `.case`, `???` |
//! | `control.rs`  | `if` and `match` — expressions whose body is a block |
//! | `types.rs`    | the type grammar |
//! | `describe.rs` | how a token is named in a diagnostic |
//!
//! Two invariants, inherited from the lexer because they are the same bet:
//!
//! - **The parser never stops.** Every failure is a diagnostic plus a
//!   recovery move, so a file with ten mistakes reports about ten of them.
//! - **Each mistake is reported once.** Where the lexer already emitted an
//!   `Error` token the parser stays silent — one mistake, one diagnostic
//!   (design.md §4.17: the error is the deliverable).

#[cfg(test)]
mod tests;

pub mod ast;

mod control;
mod cursor;
mod data;
mod decl;
mod describe;
mod expr;
mod members;
mod primary;
mod recover;
mod stmt;
mod types;

pub use ast::{
    Arg, Arm, ArmBody, Ast, BinaryOp, Block, Branch, Case, Decl, DeclKind, Expr, ExprId, ExprKind,
    Field, Function, MapEntry, Param, Pattern, PatternKind, Stmt, StmtId, StmtKind, TypeId,
    TypeKind, TypeNode, UnaryOp, Use,
};

use crate::diagnostics::Diagnostic;
use crate::lexer::{lex, TokenKind};
use crate::source::{Source, Span};

use cursor::Cursor;

pub struct ParseOutput {
    pub ast: Ast,
    /// Every comment in the file, in source order. The tree keeps only the
    /// ones that are *documentation* (§4.1); `heroes fmt` needs all of them,
    /// because a formatter that drops a comment is a formatter nobody runs.
    pub comments: Vec<Span>,
    pub diagnostics: Vec<Diagnostic>,
}

/// Lex and parse one file in a single call — the pipeline is an
/// implementation detail of the frontend, not of its clients (design.md
/// §3.3: a library with a thin CLI on top).
///
/// Diagnostics come out lexer-first, then parser, which keeps them grouped
/// by the stage that can explain them; within a stage they are in source
/// order. Never sorted across stages: a shape error and the word error that
/// caused it read better together than interleaved by column.
pub fn parse(src: &Source) -> ParseOutput {
    let lexed = lex(src);
    let comments: Vec<Span> = lexed
        .tokens
        .iter()
        .filter(|token| token.kind == TokenKind::Comment)
        .map(|token| token.span)
        .collect();
    let mut cur = Cursor::new(lexed.tokens);
    let mut ast = Ast::default();
    decl::file(&mut cur, &mut ast, src);
    let mut diagnostics = lexed.diagnostics;
    diagnostics.append(&mut cur.diagnostics);
    ParseOutput { ast, comments, diagnostics }
}
