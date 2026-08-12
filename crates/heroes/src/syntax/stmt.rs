//! Statements, and the blocks that hold them (design.md §4.4 bindings,
//! §4.7 loops, §4.14 statement position, §4.18 `assert`).
//!
//! Heroes has three line shapes that begin with a name, and telling them
//! apart is this file's whole difficulty — solved with one token of
//! lookahead, exactly as §4.4 designed it:
//!
//! ```text
//! x = 5            name  =        immutable binding
//! v: i64 @ 0       name  :        declaration (the type is mandatory)
//! v @ v + 1        place @        mutation
//! ```
//!
//! That third line is why the type is mandatory on a mutable declaration: a
//! typo (`totl @ total + x`) must not silently declare a new variable, and
//! the shapes have to be distinguishable without scanning the block (§4.4).

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::lexer::TokenKind;
use crate::source::{Source, Span};

use super::ast::{Ast, Block, ExprId, ExprKind, Stmt, StmtId, StmtKind};
use super::cursor::Cursor;
use super::expr::expr;
use super::types::parse_type;

/// The indented body of a declaration, a loop, an `if` or a `match` arm.
/// `None` means the block was missing and a diagnostic said so; the caller
/// gives up on that construct.
/// `function f():` — the Python suite colon, the one autopilot slip panel
/// 018's blind experiment pre-registered above every other (12-18% of
/// files). Eaten with a `Certain` fix so one habit costs one diagnostic and
/// the block still parses. A `:` is never legal at a block boundary —
/// annotations, named arguments and map literals all live inside other
/// structure — so this can never misfire.
pub(super) fn eat_python_colon(cur: &mut Cursor) {
    if !cur.at(TokenKind::Colon) {
        return;
    }
    let span = cur.span();
    let mut diag = Diagnostic::new(
        "trailing_colon",
        "a block is opened by the indentation alone — drop the `:` (this is not Python)".to_string(),
        span,
    );
    diag.fixes.push(Fix {
        title: "drop the `:`".to_string(),
        replacement: String::new(),
        span,
        certainty: Certainty::Certain,
    });
    cur.push_diagnostic(diag);
    cur.bump();
}

pub(super) fn block(
    cur: &mut Cursor,
    ast: &mut Ast,
    src: &Source,
    owner: &str,
) -> Option<Block> {
    eat_python_colon(cur);
    cur.skip_terminators();
    if !cur.at(TokenKind::Indent) {
        if !cur.at_reported_error() {
            let message = format!(
                "{owner} needs an indented body — one level deeper, exactly 4 spaces (found {})",
                cur.found(src)
            );
            cur.error("missing_body", message, cur.span());
        }
        return None;
    }
    let open = cur.bump().span;
    let mut stmts: Vec<StmtId> = Vec::new();
    loop {
        cur.skip_terminators();
        match cur.kind() {
            TokenKind::Dedent => {
                cur.bump();
                break;
            }
            TokenKind::Eof => break,
            // A block one level deeper than any statement asked for. It
            // belongs to nothing, so it goes whole rather than one
            // diagnostic per line.
            TokenKind::Indent => {
                if !cur.at_reported_error() {
                    cur.error(
                        "unexpected_block",
                        "this block is indented deeper than anything that opens one".to_string(),
                        cur.span(),
                    );
                }
                cur.balanced_block();
            }
            _ => stmts.push(statement(cur, ast, src)),
        }
    }
    let span = Span { start: open.end, end: cur.previous_span().end };
    Some(Block { span, stmts })
}

pub(super) fn statement(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> StmtId {
    let start = cur.span();
    let reported_before = cur.diagnostic_count();
    let kind = match cur.kind() {
        TokenKind::KwReturn => return_stmt(cur, ast, src),
        TokenKind::KwBreak => {
            cur.bump();
            StmtKind::Break
        }
        TokenKind::KwContinue => {
            cur.bump();
            StmtKind::Continue
        }
        TokenKind::KwAssert => {
            cur.bump();
            StmtKind::Assert(expr(cur, ast, src))
        }
        TokenKind::KwFor => for_stmt(cur, ast, src),
        TokenKind::KwWhile => while_stmt(cur, ast, src),
        // `x = …` and `v: … @ …` are the two declaration shapes; everything
        // else starting with a name is an expression, and possibly the place
        // of a mutation.
        TokenKind::Ident if cur.peek(1) == TokenKind::Eq => bind(cur, ast, src),
        TokenKind::Ident if cur.peek(1) == TokenKind::Colon => annotated(cur, ast, src),
        _ => expression_or_mutation(cur, ast, src),
    };
    let failed = cur.diagnostic_count() > reported_before;
    finish(cur, ast, src, start, kind, failed)
}

/// Every statement ends at its line end. Consuming the terminator here — in
/// one place — is what keeps the block loop from having to know which
/// statements span lines.
fn finish(
    cur: &mut Cursor,
    ast: &mut Ast,
    src: &Source,
    start: Span,
    kind: StmtKind,
    failed: bool,
) -> StmtId {
    // The last significant token, never a trailing comment: the span is what
    // the caret underlines (§4.17).
    let span = start.to(cur.previous_significant_span());
    // A statement that already failed has said what is wrong: the rest of
    // its line is debris, not a second mistake — and so is any block hanging
    // off it, which is how a depth-zero line continuation (illegal, panel 007)
    // used to cost three diagnostics instead of one.
    if failed {
        cur.skip_line();
        if cur.at(TokenKind::Indent) {
            cur.balanced_block();
        }
        return ast.push_stmt(Stmt { kind, span });
    }
    // A statement that ended with a block — a loop, an `if`, a `match` —
    // consumed its own `Dedent`, and a `Dedent` *is* the end of a line.
    let ended_with_block = cur.previous_kind() == TokenKind::Dedent;
    if !ended_with_block
        && !cur.at(TokenKind::Dedent)
        && !cur.at(TokenKind::Eof)
        && !cur.eat(TokenKind::Terminator)
    {
        if !cur.at_reported_error() {
            let message = format!(
                "expected the end of the line, found {} — one statement per line, no semicolons",
                cur.found(src)
            );
            cur.error("expected_end_of_line", message, cur.span());
        }
        cur.skip_line();
    }
    ast.push_stmt(Stmt { kind, span })
}

/// `return e`, or bare `return` where the function returns nothing.
fn return_stmt(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> StmtKind {
    cur.bump(); // `return`
    if cur.at(TokenKind::Terminator) || cur.at(TokenKind::Dedent) || cur.at(TokenKind::Eof) {
        return StmtKind::Return(None);
    }
    StmtKind::Return(Some(expr(cur, ast, src)))
}

/// `while cond` — the condition loop (§4.7; panel 018 split it out of the
/// old two-grammar `for`).
fn while_stmt(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> StmtKind {
    cur.bump(); // `while`
    let cond = expr(cur, ast, src);
    match block(cur, ast, src, "a `while`") {
        Some(block) => StmtKind::While { cond, block },
        None => StmtKind::Error,
    }
}

/// `for x in xs` — iteration only (§4.7). A condition after `for` is the one
/// autopilot mistake this arm exists to catch. The `while` fix is `Certain`
/// only when no loop variable can be present (`for !done`, `for 0 < x`): no
/// for-in could have produced that line. `for x > 0` gets the same fix as a
/// `Guess`, because `for x of xs` — a mangled iteration, not a condition —
/// reaches the identical parse state, and a machine-applied `while` there
/// would not compile.
fn for_stmt(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> StmtKind {
    let keyword = cur.bump().span; // `for`
    if cur.at(TokenKind::Ident) && cur.peek(1) == TokenKind::KwIn {
        let name = cur.bump().span;
        cur.bump(); // `in`
        let iterable = expr(cur, ast, src);
        return match block(cur, ast, src, "a `for`") {
            Some(block) => StmtKind::ForIn { name, iterable, block },
            None => StmtKind::Error,
        };
    }
    if !cur.at_reported_error() {
        let certainty = if cur.at(TokenKind::Ident) {
            Certainty::Guess
        } else {
            Certainty::Certain
        };
        let message = format!(
            "`for` iterates — `for x in xs`; found {} — a loop over a condition is `while`",
            cur.found(src)
        );
        let mut diag = Diagnostic::new("for_missing_in", message, cur.span());
        diag.fixes.push(Fix {
            title: "use `while`".to_string(),
            replacement: "while".to_string(),
            span: keyword,
            certainty,
        });
        cur.push_diagnostic(diag);
    }
    // Parse the rest as the condition loop it most likely was: one mistake,
    // one diagnostic, and the body still gets checked.
    let cond = expr(cur, ast, src);
    match block(cur, ast, src, "a `for`") {
        Some(block) => StmtKind::While { cond, block },
        None => StmtKind::Error,
    }
}

/// `x = 5` — binds once, forever (§4.4). The type is inferred, and inference
/// is local: the value on this line decides it (§4.5).
fn bind(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> StmtKind {
    let name = cur.bump().span;
    cur.bump(); // `=`
    StmtKind::Bind { name, ty: None, value: expr(cur, ast, src) }
}

/// `v: i64 @ 0` — a mutable cell, type mandatory. `xs: [i64] = []` — an
/// immutable binding that needs its annotation because an empty literal
/// cannot say what it holds (§4.5).
fn annotated(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> StmtKind {
    let name = cur.bump().span;
    cur.bump(); // `:`
    let ty = parse_type(cur, ast, src);
    if cur.eat(TokenKind::At) {
        return StmtKind::Declare { name, ty, value: expr(cur, ast, src) };
    }
    if cur.eat(TokenKind::Eq) {
        return StmtKind::Bind { name, ty: Some(ty), value: expr(cur, ast, src) };
    }
    if !cur.at_reported_error() {
        let message = format!(
            "expected `@` to declare a mutable, or `=` to bind, found {} — `v: i64 @ 0` declares a cell, `xs: [i64] = []` binds once",
            cur.found(src)
        );
        cur.error("expected_binding_symbol", message, cur.span());
    }
    StmtKind::Error
}

/// Everything else: an expression alone on its line, or the place on the
/// left of an `@`.
fn expression_or_mutation(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> StmtKind {
    let value = expr(cur, ast, src);
    if !cur.eat(TokenKind::At) {
        return StmtKind::Expr(value);
    }
    if !is_place(ast, value) {
        let span = ast.exprs[value.0 as usize].span;
        cur.error(
            "not_a_place",
            "only a name, a field or an element can be mutated — the left of `@` must name where the value goes"
                .to_string(),
            span,
        );
    }
    StmtKind::Mutate { place: value, value: expr(cur, ast, src) }
}

/// A *place*: a name, or a field or index path rooted at one. Since Heroes
/// has no references, every place has exactly one root — which is what makes
/// panel 010's alias test a comparison of roots rather than a dataflow
/// analysis.
fn is_place(ast: &Ast, id: ExprId) -> bool {
    match &ast.exprs[id.0 as usize].kind {
        ExprKind::Name => true,
        ExprKind::Field { base, .. } => is_place(ast, *base),
        ExprKind::Index { base, .. } => is_place(ast, *base),
        _ => false,
    }
}
