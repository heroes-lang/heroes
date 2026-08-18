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
        Some(text) => {
            let cast = if reads_as_opaque(types, function, base, index) { "(void *)" } else { "" };
            w.line(&format!("    {name} = {cast}{text};"));
        }
        None => w.line("    hero_unreachable(); /* the gate refuses this base */"),
    }
}

/// Whether this field arrives in Heroes as a `ptr` — the one read that needs the
/// qualifier written off explicitly.
///
/// **A C header may declare a member `const T *`, and Heroes' `ptr` has no
/// qualifier to receive it.** Without the cast the emitted `t5 = t4.pMethods;` is a
/// **constraint violation** under C11 6.5.16.1 — the left type must have all the
/// qualifiers of the right — so clang diagnoses it, and under
/// `-Werror=incompatible-pointer-types-discards-qualifiers` the build dies at exit
/// **2**, the compiler blaming itself for a header the author is entitled to bind
/// (CLAUDE.md §7). With the cast the same C is *conforming*: 6.3.2.3p7 makes a
/// pointer-to-object conversion legal, and it is what the C library itself does —
/// `strchr` and `strstr` are specified to return non-const pointers into
/// const-qualified arguments (SEI CERT EXP05-C, a **recommendation** at P4/L3, and
/// not the rule; the rule is EXP40-C, which forbids *modifying* a const object).
///
/// **The cast adds no hazard, and that is the finding rather than an assumption.**
/// C, Rust and Zig converged independently on the same rule — undefined behaviour
/// attaches to the **object**, not to the pointer's qualifier — and Heroes has no
/// dereference for a `ptr` at all, so the program cannot write through one without
/// handing it to C, which every other `ptr` already permits.
///
/// **What this must not weaken, and does not**: panel 058's
/// `-Werror=incompatible-pointer-types-discards-qualifiers` exists for the *write*
/// direction — a C function taking `char *` writing through `.cstr()`'s
/// copy-on-write buffer, measured to change a value the program never passed. That
/// guard lives in `emit/ffi_mutable.rs`, keys on a **parameter** the author
/// declared, and is gated by `extern_at_line`. This cast is on a **field read** the
/// emitter generates, which that path never inspects. `-Wcast-qual` is not in
/// `commands/flags.rs::FLAGS` — checked, all thirteen — so an explicit cast is not
/// itself diagnosed.
fn reads_as_opaque(
    types: &aggregate::Types,
    function: &crate::ir::Function,
    base: crate::ir::ValueId,
    index: u32,
) -> bool {
    let owner = function.values[base.0 as usize];
    types
        .field(owner, index)
        .is_some_and(|(_, ty)| types.checked.types.get(ty) == crate::types::Ty::Ptr)
}
