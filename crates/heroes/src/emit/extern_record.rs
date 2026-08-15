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
use crate::syntax::{Ast, DeclKind, Field};
use crate::types::{Checked, Ty, TyId};

use super::typedefs::Names;
use super::writer::Writer;
use super::mangle;

/// The marker every field assertion carries, so `emit/ffi.rs` can recognise one of
/// ours in clang's output without matching prose. **One writer, one reader** — the
/// contract `ffi::ASSERTION` already keeps for the result assertions.
pub(crate) const FIELD_ASSERTION: &str = "heroes-ffi-field";

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
        for field in fields {
            let Some(line) = assertion(checked, names, src, &c_type, field) else { continue };
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
}

/// One field's assertion, or `None` for a field whose type the checker has already
/// refused — in which case the program does not reach a binary and an assertion
/// naming a type with no C spelling would be the compiler's own error.
fn assertion(
    checked: &Checked,
    names: &Names,
    src: &Source,
    c_type: &str,
    field: &Field,
) -> Option<String> {
    let ty = checked.written_type(field.ty)?;
    let spelling = c_spelling(checked, names, ty)?;
    let member = mangle::field_of(true, src.slice(field.name));
    let place = format!("(({c_type} *)0)->{member}");
    Some(format!(
        "_Static_assert(_Generic(&{place}, {spelling} *: 1, default: 0) \
         && sizeof({place}) == sizeof({spelling}), \
         \"{FIELD_ASSERTION} {c_type} {member}\");"
    ))
}

/// The C type a field is declared as, for the association list.
///
/// **A nested record is the header's own name**, which is what makes
/// `RenderTexture { Texture texture; }` checkable at all — and it was measured
/// missing: panel 060's compiler-engineer found its own prototype emitting **no
/// assertion at all** for a nested record, through a `_ => return None` that read
/// as "a type with no spelling" and meant "a type I forgot". The arm is written
/// out here so the next type to arrive is a `None` somebody chose.
fn c_spelling(checked: &Checked, names: &Names, ty: TyId) -> Option<String> {
    Some(match checked.types.get(ty) {
        Ty::Int(kind) => kind.c_type().to_string(),
        Ty::Float(kind) => kind.c_type().to_string(),
        Ty::Bool => "bool".to_string(),
        Ty::Ptr => "void *".to_string(),
        Ty::Cstr => "const char *".to_string(),
        Ty::Named(decl) => names.of(decl).to_string(),
        // `str`, `[T]`, `{K: V}`, `T?` and `()` have no header layout, and the
        // checker refuses each of them as a group record's field. Reaching here
        // means the frontend let one through.
        _ => return None,
    })
}
