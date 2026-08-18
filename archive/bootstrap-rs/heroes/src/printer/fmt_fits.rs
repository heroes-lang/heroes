//! The three measurements the formatter asks before it lays anything out: does this arm
//! fit, did the author spread this literal, where did this statement end.
//!
//! One file because all three are the same *kind* of question — the **extent of a value
//! in the source or on the page** — and each is asked by a different printer, so none of
//! them belongs to any one of them.
//!
//! `spans_lines` is the subtle one and it is a **veto on joining**: a list literal
//! written down the page keeps its shape even where it would fit on one line, because
//! §4.9 gives the two forms different separators. An array written across lines is not a
//! wrapped array, it is the other spelling — and a formatter that joins it deletes the
//! grouping the author chose. Only literals, and only the value's outermost node: a call
//! keeps its commas, because a signature's shape carries no grouping.

use crate::source::Source;
use crate::syntax::{ArmBody, Ast, ExprId, ExprKind, StmtKind};

use super::fmt::{one_line, WIDTH};

/// The printer's one-line test for an arm, with the arm's own unpadded head —
/// and, when it passes, the length that test measured.
pub(super) fn fits_on_one_line(
    ast: &Ast,
    src: &Source,
    arm: &crate::syntax::Arm,
    left: usize,
    indent: usize,
) -> Option<usize> {
    let ArmBody::Stmt(id) = arm.body else { return None };
    let (StmtKind::Expr(value) | StmtKind::Return(Some(value))) =
        ast.stmts[id.0 as usize].kind
    else {
        // Any other statement shape prints on its own line by construction, so
        // it is in the run and its length is its head's.
        return Some(indent + left + 4);
    };
    if spans_lines(ast, src, value) {
        return None;
    }
    // `left` spaces for the pattern, then ` => `.
    let head = " ".repeat(left + 4);
    let len = one_line(ast, src, &head, value).chars().count();
    if indent + len <= WIDTH {
        Some(len)
    } else {
        None
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
pub(super) fn spans_lines(ast: &Ast, src: &Source, value: ExprId) -> bool {
    let expr = &ast.exprs[value.0 as usize];
    // `if` and `match` carry blocks and the statement printer moves `last_line`
    // for them itself; their spans reach a `Dedent` sitting on the *next* line,
    // which is the hazard `literal_end_line`'s doc warns about. Every other kind
    // is answered by its own extent.
    //
    // It used to be `Array` and `Map` alone, on the reasoning that a multi-line
    // list is the only value that does not end on the line it started. That was
    // true of source a person writes and false of source **this formatter
    // writes**: the 88-column rule breaks a long call across lines, so
    // formatting twice inserted a blank line after every one of them
    // (`fixedbugs_formatting_twice_does_not_grow_a_blank_line`).
    if matches!(expr.kind, ExprKind::If { .. } | ExprKind::Match { .. }) {
        return false;
    }
    let first = src.line_of(expr.span.start);
    let last = src.line_of(expr.span.end.saturating_sub(1));
    last > first
}

/// The last source line a statement's own text occupies, when its **value**
/// crosses lines — and `None` otherwise.
///
/// The blank-line rule needs to know where a statement stopped, and the answer
/// is the *value expression's* extent, never the statement's own span: a
/// statement ending in a block ends at a `Dedent` whose span sits on the next
/// line, and asking it made `trailing_comment` steal the following
/// declaration's doc comment.
pub(super) fn literal_end_line(ast: &Ast, src: &Source, stmt: &crate::syntax::Stmt) -> Option<u32> {
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
