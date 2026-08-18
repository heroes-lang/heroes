//! The half of §4.19's guarantee that asks what a header's **struct** contains
//! (design.md §4.19; CLAUDE.md §7's named exception; panel 060).
//!
//! A group's `record` names a struct this compiler never declares, so nothing in
//! the generated unit states its layout and nothing can state it wrongly. What
//! *can* be wrong is the program's belief about a **field**: `r: i32` where
//! `raylib.h` says `unsigned char` compiles, runs, and prints a plausible number
//! — measured before this file existed — because C converts at the designator and
//! converts back at the read. The value is right until it isn't: `r @ 300`
//! truncates to 44, silently, at exit 0.
//!
//! So one `_Static_assert` per field, and its shape is two panels' worth of
//! measurement:
//!
//! ```c
//! #line 3 "examples/raylib/main.hero"
//! _Static_assert(_Generic(&((Color *)0)->r, uint8_t *: 1, default: 0)
//!     && sizeof(((Color *)0)->r) == sizeof(uint8_t), "heroes-ffi-field Color r u8");
//! ```
//!
//! **`_Generic`, never `sizeof`/`offsetof` alone.** Panel 060's spec-warden
//! compiled a `record Vector2 { x: f64, y: f64 }` over the header's
//! `{float, float}` under this project's exact flags: **exit 0, zero
//! diagnostics**, with `sizeof(((Vector2*)0)->x) == sizeof(int32_t)` passing. Size
//! and offset are blind to the difference between `float` and `int32_t`, and on
//! arm64 that difference is the register file the value travels in (AAPCS64
//! §5.10.5.1's homogeneous float aggregate). Type identity is the only question
//! that separates them.
//!
//! **The address, not the value.** `_Generic` applies array-to-pointer decay to
//! its controlling expression (C11 6.5.1.1p2), so `_Generic(((T*)0)->arr,
//! float *: 1, …)` **passes** for a `float[4]` field and binds 16 bytes as an
//! 8-byte `ptr` — measured by panel 060's ffi-pragmatist on raylib's
//! `VrDeviceInfo`. `&` does not convert its operand: `&((T*)0)->arr` has type
//! `float (*)[4]`, which matches no association this emitter writes, so the array
//! case is refused **by construction** rather than by a second check.
//!
//! **The `sizeof` conjunct stays anyway**, and it is deliberate belt-and-braces
//! rather than a leftover. It costs nothing, it closes the same hole in the
//! *value* form that a later refactor could drift into, and CLAUDE.md §11 says to
//! put the fallback in the loud direction. The `float[4]` case owes a golden that
//! fires either way.
//!
//! **`#line` at the field**, so a failure lands on the author's own line and
//! `emit/ffi.rs` can tell whose mistake it is from the file clang names — the same
//! contract `extern_probe.rs` keeps for parameters.

use crate::source::Source;
use crate::syntax::{Ast, DeclKind};
use crate::types::Checked;

use super::typedefs::Names;
use super::writer::Writer;

/// The marker a **completeness** probe carries, so `emit/ffi.rs` can tell one of
/// ours from any other `missing field` clang might print.
///
/// Unlike the field assertion below, this one does not appear in clang's message —
/// clang writes its own wording for `-Wmissing-field-initializers`. It appears in
/// the probe's **function name**, which clang echoes in the source line under the
/// diagnostic, and that is what `ffi.rs` matches on.
pub(crate) const COMPLETE_PROBE: &str = "hero_ffi_complete_";

/// The marker every field assertion carries, so `emit/ffi.rs` can recognise one of
/// ours in clang's output without matching prose. **One writer, one reader** — the
/// contract `ffi::ASSERTION` already keeps for the result assertions.
pub(crate) const FIELD_ASSERTION: &str = "heroes-ffi-field";

/// The marker the **flexible array member** assertion carries, so `emit/ffi.rs`
/// can say *this field cannot be a member at all* instead of *this length is
/// wrong* (panel 079). Separate from `FIELD_ASSERTION` because the two name
/// different repairs, and only one of them exists.
pub(crate) const FLEX_ASSERTION: &str = "heroes-ffi-flex";

/// One assertion per field of every `record` a group declares.
///
/// **The narrowing is `header.is_some()`**, a fact about the declaration in hand
/// rather than about where the type is used: a record mentioned in an `extern`
/// signature may still be this compiler's own, and one never mentioned in a
/// signature is still the header's if the author declared it there.
pub(super) fn extern_record_assertions(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
) {
    let mut any = false;
    for (index, decl) in ast.decls.iter().enumerate() {
        let DeclKind::Record { fields, header: Some(_), .. } = &decl.kind else { continue };
        let c_type = names.of(index as u32).to_string();
        // **The marker carries the DECLARATION's name, never the C type's**
        // (panel 072 rider 1, landed with the marker that made it reachable). With
        // `tag`, `c_type` is two words — `struct stat` — and `emit/ffi_record.rs`
        // splits this marker on whitespace and reads token 1 as the name to
        // recover. Keyed on the C type, the same author mistake got exit 1 on a
        // typedef'd record and exit 2 *"internal error"* on a tagged one. The
        // declaration's name is one token by construction: it is an identifier.
        let hero_name = src.slice(decl.name).to_string();
        for field in fields {
            let Some(line) = super::extern_field::assertion(checked, names, src, &c_type, &hero_name, field) else {
                continue;
            };
            let at = field.name.start;
            let (file, at_line, _) = src.locate(at);
            let file = file.to_string();
            w.at_file(&file, at_line);
            w.line(&line);
            any = true;
        }
    }
    if any {
        w.at_generated();
        w.blank();
    }
    super::extern_complete::completeness_probes(w, ast, checked, names, src);
}
