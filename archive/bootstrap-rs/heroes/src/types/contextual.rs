//! Which expressions have no type of their own (design.md §4.5; spec § Types;
//! panels 023, 042).
//!
//! Split out of `exprs.rs` by the §11 sweep. §4.5 gives the checker two modes —
//! synthesise a type (⇒) or check against one (⇐) — and this file answers the one
//! question both modes need first: **is this form ⇐-only?** It is listed in one
//! place precisely so that every rule needing the answer agrees about what the list
//! is; two readings of it would be two languages.
//!
//! Two facts, and the second is the one that gets rediscovered:
//!
//! - a `.case`, a `???` and an empty literal have **no type at all**, so an
//!   expectation is the only information there is and they adopt anything.
//! - a **number literal** is ⇐-only too — that is what makes eight widths usable
//!   rather than merely present, since `255` has a value but no width — but it
//!   does **not** adopt anything, because it already has a *value*. Offering an
//!   `f64` to an integer literal is how `1 + 2.0` stops being an error, which is a
//!   defect this project has fixed three times.

use crate::source::Source;
use crate::syntax::{Ast, ExprId, ExprKind, UnaryOp};

use super::table::Ty;
use super::{Checker, TyId};

/// True where an expression has no type of its own and must be checked against
/// one — §4.5's ⇐-only forms, listed in one place so every rule that needs to
/// know agrees about what they are.
pub(super) fn contextual(ast: &Ast, id: ExprId) -> bool {
    match &ast.exprs[id.0 as usize].kind {
        ExprKind::Case { .. } | ExprKind::Hole => true,
        ExprKind::Array(items) => items.is_empty(),
        ExprKind::Map(entries) => entries.is_empty(),
        // **A number literal joined this list at M-sized-integers**, and it is
        // what makes eight widths usable rather than merely present. `255` has a
        // value but no width, so in `b: u8 @ 255` and in `b + 1` it takes the
        // one the context asks for; with nothing asking, `synth` gives it `i64`.
        //
        // Six languages do this — Rust, Swift, Zig, Nim, Kotlin, Go — and the one
        // that does not is **Java, which does it HALF**: `byte b = 42;` compiles
        // and `b = b + 1;` does not, because arithmetic promotes and declaration
        // does not. That shape is worse than either whole one for this language's
        // thesis, since the two lines are one token apart and the diagnostic
        // would have to explain a promotion lattice §4.3 says does not exist
        // (panel 042, historian, 6-for-1 with sources).
        //
        // A character literal comes too: §4.3 makes it a number, its value is one
        // ASCII character, and it fits every width. Without it `b == 'a'` for a
        // `u8` byte would be a type error — which is lexer code, the closure
        // list's own shape.
        // **`nullptr` joined at panel 053**, and it joined for a reason the other
        // members share: the value exists and the *type* does not. C's null
        // pointer constant is one value that inhabits `ptr` and `cstr` alike —
        // `((void *)0)` is a valid `const char *` by C11 6.3.2.3p3 — so fixing it
        // at `ptr` made `v == nullptr` unwritable for a `cstr`, which is the one
        // question anybody asks of a C string. With nothing asking, `synth` still
        // gives `ptr`, exactly as a number literal still defaults to `i64`.
        ExprKind::NullPtr => true,
        // A **float** literal is contextual for the reason a number literal is:
        // `1.5` has a value and no width, so `v: f32 @ 1.5` takes the one the
        // context asks for and `synth` gives `f64` when nothing does. Without this
        // arm `f32` exists and cannot be written to (panel 060).
        _ => number_literal(ast, id) || float_literal(ast, id),
    }
}

/// Whether this expression **is** a number literal, sign included.
///
/// `-128` is unary minus applied to `128`, so the sign has to be read with the
/// digits rather than after them: `128` does not fit an `i8` on its own and
/// `-128` does, so a check that saw them one at a time would refuse the value
/// the width exists to hold.
///
/// **And the recursion is the whole predicate**: `-x` is not a literal, because
/// nothing under the minus is one. Two rules read that question flatly —
/// `ExprKind::Unary { op: Neg, .. }` with no look at the operand — and both were
/// wrong in the same way (fixedbugs, 2026-08-13): `x @ -x` on an `f64` was
/// *expected `f64`, found `i64`*, and `ok(-n)` on an `i64` was *`-n` does not
/// fit an `i64`*, a range diagnostic about an expression with no value to
/// range-check. That is why the answer lives here beside `contextual`, in one
/// function that every rule calls: this module's own doc says two readings of
/// the list would be two languages, and for one day they were.
pub(super) fn number_literal(ast: &Ast, id: ExprId) -> bool {
    match &ast.exprs[id.0 as usize].kind {
        ExprKind::Int | ExprKind::Char => true,
        ExprKind::Unary { op: UnaryOp::Neg, operand } => number_literal(ast, *operand),
        _ => false,
    }
}

/// The same question for a **float** literal, which `f32` made a real one
/// (panel 060, author instruction 2026-08-15).
///
/// Separate from `number_literal` rather than folded into it, and the separation
/// is the whole point: the two adopt **disjoint** sets. An integer literal must
/// not adopt a float width — `1 & 2` where the context wants an `f64` is the
/// two-diagnostics defect `adopts` exists to prevent — and a float literal must
/// not adopt an integer one, because `1.5` has a value no integer holds. Folding
/// them into "a number literal" would make each adopt the other's widths, which
/// is how the integer half acquired that bug in the first hour it existed.
///
/// It recurses through unary minus for the reason the integer rule does, and the
/// reason is a fixed defect rather than symmetry: `x @ -x` on a float read the
/// negation flatly and reported *expected `f64`, found `i64`*.
pub(super) fn float_literal(ast: &Ast, id: ExprId) -> bool {
    match &ast.exprs[id.0 as usize].kind {
        ExprKind::Float => true,
        ExprKind::Unary { op: UnaryOp::Neg, operand } => float_literal(ast, *operand),
        _ => false,
    }
}

/// Whether a contextual expression will actually take the type offered.
///
/// **A number literal adopts an integer width and nothing else.** Without this,
/// `1.5 & 1` produced *two* diagnostics — `&` takes `i64`, found `f64`, and then
/// `expected f64, found i64` from the literal being pushed at a type it can
/// never have. Two diagnostics for one mistake is a defect this compiler has
/// fixed three times, and adopting the widths reintroduced it within the hour.
///
/// The other contextual forms adopt anything: a `.case`, a `???` and an empty
/// literal have no type at all, so the expectation is the only information there
/// is. A number has a *value* already, and a value that cannot be an `f64` is
/// worse off being told to try.
pub(super) fn adopts(checker: &Checker, ast: &Ast, id: ExprId, offered: TyId) -> bool {
    if number_literal(ast, id) {
        return matches!(checker.out.types.get(offered), Ty::Int(_));
    }
    // **A float literal adopts a float width and nothing else** — the mirror of
    // the line above, and disjoint from it by construction.
    if float_literal(ast, id) {
        return matches!(checker.out.types.get(offered), Ty::Float(_));
    }
    // **`nullptr` adopts a pointer and nothing else**, for the same reason a
    // number adopts only an integer width: it has a value already, and a value
    // that cannot be a `str` is worse off being told to try. Two diagnostics for
    // one mistake is the defect this compiler has fixed three times.
    if matches!(ast.exprs[id.0 as usize].kind, ExprKind::NullPtr) {
        return matches!(checker.out.types.get(offered), Ty::Ptr | Ty::Cstr);
    }
    true
}

/// The value a literal denotes, wide enough for every width, with the minus sign
/// applied where there is one.
///
/// The sign has to be folded in **here** rather than checked separately: `-128`
/// fits an `i8` and `128` does not, so a check that saw them one at a time would
/// refuse the only way to write that type's lowest value.
///
/// The **value** of a number literal, walking under the minus signs — the same
/// walk `number_literal` above does to say *what it is*, which is why it lives
/// here rather than beside its one caller. Two files doing one walk is two
/// chances for them to disagree about where a literal ends, and the fixed defect
/// this module's doc records is exactly a disagreement of that kind.
pub(super) fn literal_value(ast: &Ast, src: &Source, id: ExprId) -> Option<i128> {
    let node = &ast.exprs[id.0 as usize];
    match &node.kind {
        ExprKind::Int => crate::lexer::decode_wide(src.slice(node.span)),
        // §4.3: a character literal *is* a number, and its value is one ASCII
        // character — so it fits every width and needs no special range.
        ExprKind::Char => Some(i128::from(
            crate::lexer::unescape(src, node.span).chars().next().unwrap_or('\0') as u32,
        )),
        ExprKind::Unary { op: UnaryOp::Neg, operand } => {
            literal_value(ast, src, *operand).map(|v| -v)
        }
        _ => None,
    }
}
