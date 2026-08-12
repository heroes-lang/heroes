//! `if` and `match`, which are expressions (design.md §4.7), and the
//! patterns `match` destructures with.
//!
//! `match` is the only destructuring construct and the only conditional in
//! the core; `if` is sugar for a `match` on `bool` (Part 5). They live in one
//! file because they are the two forms whose *body* is a block, which is the
//! property that makes them awkward: an expression that spans lines and
//! carries indentation.
//!
//! Two rules the parser records but does not enforce — both are the
//! checker's at M-data-declarations, and both are the reason `match` exists:
//! exhaustiveness, and the ban on `_` as a whole arm over a variant (§4.7:
//! with `_` allowed, adding a case would stop breaking compilation, and
//! exhaustiveness becomes theatre).

use crate::lexer::TokenKind;
use crate::source::Source;

use super::ast::{Arm, ArmBody, Ast, Branch, Expr, ExprId, ExprKind, Pattern, PatternKind};
use super::cursor::Cursor;
use super::expr::expr;
use super::stmt::{block, eat_python_colon, statement};

/// `if cond` + block, then any number of `else if`, then an optional `else`.
/// No parentheses around the condition (§4.15) and no truthiness: the
/// condition must be `bool`, checked at M-typed-frontend.
pub(super) fn if_expr(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> ExprId {
    let start = cur.bump().span; // `if`
    let mut branches: Vec<Branch> = Vec::new();
    let cond = expr(cur, ast, src);
    let Some(first) = block(cur, ast, src, "an `if`") else {
        return ast.push_expr(Expr { kind: ExprKind::Error, span: start });
    };
    branches.push(Branch { cond, block: first });
    let mut otherwise = None;
    // `else` closes the `if`'s block, so the cursor is back at this level.
    while cur.at(TokenKind::KwElse) {
        cur.bump();
        if cur.eat(TokenKind::KwIf) {
            let cond = expr(cur, ast, src);
            let Some(more) = block(cur, ast, src, "an `else if`") else { break };
            branches.push(Branch { cond, block: more });
            continue;
        }
        let Some(last) = block(cur, ast, src, "an `else`") else { break };
        otherwise = Some(last);
        break;
    }
    let end = otherwise
        .as_ref()
        .map_or_else(|| branches[branches.len() - 1].block.span, |block| block.span);
    ast.push_expr(Expr { kind: ExprKind::If { branches, otherwise }, span: start.to(end) })
}

/// `match e` + one arm per line, indented.
pub(super) fn match_expr(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> ExprId {
    let start = cur.bump().span; // `match`
    let scrutinee = expr(cur, ast, src);
    eat_python_colon(cur);
    cur.skip_terminators();
    if !cur.at(TokenKind::Indent) {
        if !cur.at_reported_error() {
            let message = format!(
                "a `match` needs its arms indented one level below it, found {}",
                cur.found(src)
            );
            cur.error("missing_match_arms", message, cur.span());
        }
        return ast.push_expr(Expr { kind: ExprKind::Error, span: start });
    }
    cur.bump(); // the Indent
    let mut arms: Vec<Arm> = Vec::new();
    loop {
        cur.skip_terminators();
        match cur.kind() {
            TokenKind::Dedent => {
                cur.bump();
                break;
            }
            TokenKind::Eof => break,
            // A block where an arm should start: it belongs to an arm that
            // failed, so it goes whole. Without this case the loop can spin
            // forever — `skip_line` deliberately stops *before* an `Indent`
            // (it must not leave the block it is cleaning), so a failed arm
            // followed by an indented body consumed nothing at all and the
            // parser never terminated. Found by the compiler-engineer while
            // costing panel 014, on `1 => for x in xs` + its body.
            TokenKind::Indent => {
                cur.balanced_block();
            }
            _ => match arm(cur, ast, src) {
                // A body that failed has already dropped the rest of its line
                // (`finish` does it for every statement, panel 014), so the
                // loop must NOT skip again — that ate the arm below it.
                Some(arm) => arms.push(arm),
                // A pattern or `=>` that failed consumed nothing reusable.
                None => cur.skip_line(),
            },
        }
    }
    let span = start.to(cur.previous_span());
    ast.push_expr(Expr { kind: ExprKind::Match { scrutinee, arms }, span })
}

/// One arm: patterns joined by `|`, `=>`, then one statement on the same line
/// or a block below it (§4.7, panel 014).
fn arm(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> Option<Arm> {
    let start = cur.span();
    let mut patterns = vec![pattern(cur, ast, src)?];
    while cur.eat(TokenKind::Pipe) {
        patterns.push(pattern(cur, ast, src)?);
    }
    if !cur.expect(
        TokenKind::FatArrow,
        "expected_arm_arrow",
        "`=>` and the arm's value",
        src,
    ) {
        return None;
    }
    cur.skip_terminators();
    // One statement, or a block — the same two shapes every other body has
    // (panel 014). Going through `statement` is what gives an arm `assert`,
    // `break` and `return`, and what stops the printer from having a second
    // path that cannot render a block under a control head.
    let body = if cur.at(TokenKind::Indent) {
        ArmBody::Block(block(cur, ast, src, "a `match` arm")?)
    } else {
        let id = statement(cur, ast, src);
        reject_declaration(cur, ast, src, id);
        ArmBody::Stmt(id)
    };
    Some(Arm { patterns, body, span: start.to(cur.previous_span()) })
}

/// A declaration is not an arm body (panel 017 D). `.a => x = 5` binds a name
/// nothing can read: the arm is one statement, so the binding's scope ends with
/// it.
///
/// The rejection lives here, in the parser, where Rust puts the same rule — as a
/// *grammar* exclusion of `let` from expression position. In the checker it would
/// arrive second: the resolver already reports the name as unused, with a repair
/// ("remove the binding, or read it") that the author cannot apply, and the user
/// would get both messages for one mistake.
///
/// The `Error` guard is not defensive. Without it a body the parser could not read
/// at all reports twice — once as the real syntax error and once as this — which
/// `tests::recovery` catches.
fn reject_declaration(cur: &mut Cursor, ast: &Ast, src: &Source, id: crate::syntax::StmtId) {
    let (name, value) = match &ast.stmts[id.0 as usize].kind {
        crate::syntax::StmtKind::Bind { name, value, .. } => (*name, *value),
        crate::syntax::StmtKind::Declare { name, value, .. } => (*name, *value),
        _ => return,
    };
    if matches!(ast.exprs[value.0 as usize].kind, ExprKind::Error) {
        return;
    }
    let text = src.slice(name);
    let message = format!(
        "an arm's body may not declare a name — `{text}` would be bound where nothing can read it; write the value as the arm's body, or open a block"
    );
    cur.error("declaration_in_arm", message, ast.stmts[id.0 as usize].span);
}

/// `.num n`, `.num _`, `.plus`, `_`, or a literal.
///
/// The payload name is optional and may be `_`: `.num _ => 0` is legal,
/// `_ => 0` on a variant is not (§4.7). That distinction is not obvious,
/// which is why the spec states it and the tree keeps the two apart.
fn pattern(cur: &mut Cursor, ast: &mut Ast, src: &Source) -> Option<Pattern> {
    let start = cur.span();
    match cur.kind() {
        TokenKind::Dot => {
            cur.bump();
            if !cur.at(TokenKind::Ident) {
                if !cur.at_reported_error() {
                    let message = format!(
                        "expected a case name after `.`, found {} — an arm is `.name => value`",
                        cur.found(src)
                    );
                    cur.error("expected_pattern_case", message, cur.span());
                }
                return None;
            }
            let name = cur.bump().span;
            // A name here binds the payload; `_` binds it and says it is
            // unused, which is what keeps §4.4's unused-binding rule honest.
            let binding = if cur.at(TokenKind::Ident) {
                Some(cur.bump().span)
            } else {
                None
            };
            Some(Pattern { kind: PatternKind::Case { name, binding }, span: start.to(cur.previous_span()) })
        }
        // `_` lexes as an identifier: it is the wildcard only here.
        TokenKind::Ident if src.slice(cur.span()) == "_" => {
            cur.bump();
            Some(Pattern { kind: PatternKind::Wildcard, span: start })
        }
        TokenKind::Int | TokenKind::Str | TokenKind::Char | TokenKind::Minus => {
            let value = expr(cur, ast, src);
            Some(Pattern { kind: PatternKind::Literal(value), span: start.to(cur.previous_span()) })
        }
        _ => {
            if !cur.at_reported_error() {
                let message = format!(
                    "expected a pattern, found {} — `.case`, `.case name`, a literal, or `_` (on `int`/`str` only)",
                    cur.found(src)
                );
                cur.error("expected_pattern", message, cur.span());
            }
            None
        }
    }
}
