//! How a **fixed array field** answers `==` (design.md §4.3; panel 062).
//!
//! Split from `structural.rs` on 2026-08-15, and the seam is a fact about C rather
//! than a line count: every other field kind is compared by one expression this
//! emitter can write inline, and an array is compared **N times**. `a->arr ==
//! b->arr` compiles with zero warnings under this project's twelve flags and is a
//! **pointer** comparison — so two byte-identical structs answer `false` and
//! `eq(a, a)` answers `true`, which is reflexivity standing in for equality,
//! silently, from the day the field form exists. Panel 062's ffi-pragmatist
//! measured exactly that before this file was written.
//!
//! There is no `memcmp` shortcut that is also correct: padding **inside an
//! element** would decide the answer, and CLAUDE.md §7 forbids a byte walk for
//! that reason — two equal records would hash differently.

use crate::types::{Checked, Ty};

use super::typedefs::Names;

/// The comparison of one element of a fixed array field, given the two places.
pub(super) fn element_eq(
    checked: &Checked,
    names: &Names,
    a: &str,
    b: &str,
    inner: crate::types::TyId,
) -> String {
    match checked.types.get(inner) {
        Ty::Int(_) | Ty::Bool | Ty::Float(_) | Ty::Ptr | Ty::Cstr => format!("{a} == {b}"),
        Ty::Named(decl) => format!("{}_eq(&{a}, &{b})", names.satellite(decl)),
        // The checker admits only the row above as a fixed array's element, so
        // anything here is a type the frontend should have stopped.
        other => unreachable!("a fixed array's element is a boundary type, not {other:?}"),
    }
}

/// `N` comparisons folded into one expression, without a statement.
///
/// **An expression rather than a loop, because the caller is building one.**
/// `equality_body` writes `if (!(<test>)) return false;` per field, and a `for`
/// there would need the whole shape restructured. `N` is a compile-time constant
/// and small — raylib's largest is 16 — so the unrolled form is what a C compiler
/// would have produced from the loop anyway, and it stays a pure expression.
pub(super) fn fixed_eq_expr(element: &str, n: u32) -> String {
    (0..n)
        .map(|i| element.replace("[i]", &format!("[{i}]")))
        .collect::<Vec<_>>()
        .join(" && ")
}
