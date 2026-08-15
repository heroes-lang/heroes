//! Reading one part of a composite value (design.md §4.6, §4.9, §4.20; panels
//! 022, 023).
//!
//! Split out of `inst.rs` by the §11 sweep. The arms stay in that file's exhaustive
//! `match`; only their bodies are here.
//!
//! The concern is one shape asked five ways — `Tag`, `Payload`, `Field`, `Index`,
//! `Len` — and what unites them is that each is a **read that cannot allocate**: a
//! member access, a subscript, a counter. Two of them are dispatched by the
//! operand's type rather than by the op, and both times for the same reason:
//!
//! - a `T?` **is** a variant by the time it reaches here (§4.6's `ok`/`err`), so
//!   `Tag` and `Payload` read the same two members either way, and the only
//!   difference is where the member names come from.
//! - `len` counts bytes on a `str` and elements on an array or a map, and `s[i]` is
//!   a byte where `xs[i]` is an element, so the op is the same op and the split is
//!   by operand type — the same shape `gate.rs` uses to ask about it.

use crate::types::{Checked, Ty};

use super::aggregate;
use super::container;
use super::fallible;
use super::mangle;
use super::writer::Writer;

/// `len` — one built-in, three runtime entry points, chosen by what is being
/// counted.
pub(super) fn len(
    w: &mut Writer,
    checked: &Checked,
    function: &crate::ir::Function,
    value: crate::ir::ValueId,
    target: Option<String>,
) {
    let Some(name) = target else { return };
    // **The three the checker accepts, named** (CLAUDE.md §11; panel 062's audit).
    // `types/builtins.rs` types `len` on `str`, `[T]` and `{K: V}` and refuses
    // everything else, so the array is one of three answers rather than the
    // fallback: as `_` it also answered for a record, a `ptr` and a `T?`, and
    // `hero_array_len` on any of them is a load through a pointer that is not a
    // header.
    let counter = match checked.types.get(function.value_type(value)) {
        Ty::Str => "hero_str_len",
        Ty::Map(_, _) => "hero_map_len",
        Ty::Array(_) => "hero_array_len",
        other => unreachable!("len counts a str, an array or a map, not {other:?}"),
    };
    w.line(&format!("    {name} = {counter}({});", mangle::value(value.0)));
}

/// `s[i]`: a byte, aborting out of range (spec line 142).
pub(super) fn string_byte(
    w: &mut Writer,
    base: crate::ir::ValueId,
    index: crate::ir::ValueId,
    target: Option<String>,
) {
    let Some(name) = target else { return };
    w.line(&format!(
        "    {name} = hero_str_byte({}, {});",
        mangle::value(base.0),
        mangle::value(index.0)
    ));
}

/// `xs[i]`: an element, read through the descriptor.
pub(super) fn element(
    w: &mut Writer,
    types: &aggregate::Types,
    function: &crate::ir::Function,
    base: crate::ir::ValueId,
    index: crate::ir::ValueId,
    target: Option<String>,
) {
    let Some(name) = target else { return };
    match container::read_element(types, function, base, index) {
        Some(text) => w.line(&format!("    {name} = {text};")),
        None => w.line("    hero_unreachable(); /* not an array */"),
    }
}

/// Which case a variant — or a `T?` — is holding.
pub(super) fn tag(
    w: &mut Writer,
    checked: &Checked,
    function: &crate::ir::Function,
    base: crate::ir::ValueId,
    target: Option<String>,
) {
    let Some(name) = target else { return };
    let text = match checked.types.get(function.value_type(base)) {
        Ty::Fallible(_) => fallible::option_tag(base),
        _ => aggregate::tag(base),
    };
    w.line(&format!("    {name} = {text};"));
}

/// What that case is carrying.
pub(super) fn payload(
    w: &mut Writer,
    types: &aggregate::Types,
    function: &crate::ir::Function,
    base: crate::ir::ValueId,
    case: u32,
    target: Option<String>,
) {
    let Some(name) = target else { return };
    let text = match types.checked.types.get(function.value_type(base)) {
        Ty::Fallible(_) => Some(fallible::option_payload(base, case)),
        _ => aggregate::payload(types, function, base, case),
    };
    match text {
        Some(text) => w.line(&format!("    {name} = {text};")),
        None => w.line("    hero_unreachable(); /* not a variant */"),
    }
}

/// One field of a record.
pub(super) fn field(
    w: &mut Writer,
    types: &aggregate::Types,
    function: &crate::ir::Function,
    base: crate::ir::ValueId,
    index: u32,
    target: Option<String>,
) {
    let Some(name) = target else { return };
    match aggregate::read_field(types, function, base, index) {
        Some(text) => w.line(&format!("    {name} = {text};")),
        None => w.line("    hero_unreachable(); /* the gate refuses this base */"),
    }
}
