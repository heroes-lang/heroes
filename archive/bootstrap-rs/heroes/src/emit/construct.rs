//! A composite value coming into existence (design.md §4.20, §4.6; panels 021,
//! 022, 023).
//!
//! Split out of `inst.rs` by the §11 sweep. The five `Op::Construct` arms stay in
//! that file's exhaustive `match`, so landing a sixth shape is still a compile error
//! there; only their bodies moved here.
//!
//! The concern is one question asked five times — **where do the arguments go** —
//! and the five answers divide into two kinds, which is the whole content of this
//! file:
//!
//! - a **record**, a **variant case** and a **`T?`** are one C expression: a
//!   compound literal, built by value, assigned once.
//! - an **array** and a **map** are several statements, because each element that
//!   crosses into the container does so through its descriptor and each step has to
//!   release the container it grew from.
//!
//! That is why `build_array` and `build_map` return lines where `construct` returns
//! text, and it is not a stylistic difference: an expression that allocated twice
//! would leak the first one with nothing to report it.

use crate::ir::{Arg, Shape};
use crate::types::TyId;

use super::aggregate;
use super::storageless;
use super::container;
use super::fallible;
use super::mangle;
use super::writer::Writer;

/// Every argument as a C name. An `@` place arrives by address (§4.8).
fn arguments(function: &crate::ir::Function, args: crate::ir::Args) -> Vec<String> {
    function
        .args_of(args)
        .into_iter()
        .filter_map(|arg| match arg {
            Arg::Value(value) => Some(mangle::value(value.0)),
            Arg::InOut(_) => None,
        })
        .collect()
}

/// `Point(x: 1, y: 2)` and `.num(v: 7)` — one compound literal either way, and the
/// only difference is whether a case tag comes with it.
pub(super) fn record_or_case(
    w: &mut Writer,
    types: &aggregate::Types,
    function: &crate::ir::Function,
    shape: Shape,
    args: crate::ir::Args,
    target: Option<String>,
) {
    let Some(name) = target else { return };
    let arguments: Vec<String> = function
        .args_of(args)
        .into_iter()
        .map(|arg| match arg {
            // **A fixed-array argument is rendered as its braced list, here**, and
            // it has to be here because C gives it nowhere else to live: an array
            // is not assignable, so the value has no temporary (`ctype.rs` declares
            // none, the same rule `()` follows), and the only legal place for the
            // elements is inside the enclosing initialiser. `emit/inst.rs` emits no
            // statement for the construction at all; this is where it becomes text.
            Arg::Value(value) => storageless::fixed_text(types, function, value)
                .unwrap_or_else(|| mangle::value(value.0)),
            Arg::InOut(place) => format!("&{}", aggregate::place(types, function, place)),
        })
        .collect();
    let literal = match shape {
        Shape::Case(decl, case) => aggregate::construct_case(types, decl, case, &arguments),
        Shape::Record(decl) => aggregate::construct(types, decl, &arguments),
        _ => None,
    };
    match literal {
        Some(text) => w.line(&format!("    {name} = {text};")),
        None => w.line("    hero_unreachable(); /* not an aggregate */"),
    }
}

/// An array literal is several statements rather than one expression, because each
/// push has to release the array it grew from.
pub(super) fn array(
    w: &mut Writer,
    types: &aggregate::Types,
    function: &crate::ir::Function,
    ty: TyId,
    args: crate::ir::Args,
    target: Option<String>,
) {
    let Some(name) = target else { return };
    match container::build_array(types, ty, &arguments(function, args), &name) {
        Some(lines) => lines.iter().for_each(|line| w.line(&format!("    {line}"))),
        None => w.line("    hero_unreachable(); /* not an array */"),
    }
}

/// A map literal, for the same reason an array is: one statement per entry.
pub(super) fn map(
    w: &mut Writer,
    types: &aggregate::Types,
    function: &crate::ir::Function,
    ty: TyId,
    args: crate::ir::Args,
    target: Option<String>,
) {
    let Some(name) = target else { return };
    match container::build_map(types, ty, &arguments(function, args), &name) {
        Some(lines) => lines.iter().for_each(|line| w.line(&format!("    {line}"))),
        None => w.line("    hero_unreachable(); /* not a map */"),
    }
}

/// `ok(x)`, `fail(c, m)`, and the `err` that `?` produces — §4.6's three ways into
/// a `T?`, all one compound literal with a tag.
pub(super) fn option(
    w: &mut Writer,
    types: &aggregate::Types,
    function: &crate::ir::Function,
    ty: TyId,
    shape: Shape,
    args: crate::ir::Args,
    target: Option<String>,
) {
    let Some(name) = target else { return };
    match fallible::construct_option(types, ty, shape, &arguments(function, args)) {
        Some(text) => w.line(&format!("    {name} = {text};")),
        None => w.line("    hero_unreachable(); /* not a T? */"),
    }
}
