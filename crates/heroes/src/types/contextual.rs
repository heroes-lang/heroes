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
        ExprKind::Int | ExprKind::Char => true,
        // `-128` is unary minus applied to `128`, so a negative literal is only
        // contextual if the sign is. Without this arm `b: i8 @ -128` is a type
        // error *and* `128` does not fit an `i8` on its own — the value is only
        // in range once the minus is applied, which is why the check below has to
        // see them together rather than one at a time.
        ExprKind::Unary { op: UnaryOp::Neg, operand } => contextual(ast, *operand),
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
    match &ast.exprs[id.0 as usize].kind {
        ExprKind::Int | ExprKind::Char | ExprKind::Unary { op: UnaryOp::Neg, .. } => {
            matches!(checker.out.types.get(offered), Ty::Int(_))
        }
        _ => true,
    }
}
