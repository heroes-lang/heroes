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
    super::extern_complete::completeness_probes(w, ast, checked, names, src);
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
    let member = mangle::field_of(true, src.slice(field.name));
    let place = format!("(({c_type} *)0)->{member}");
    // **A `ptr` field asks *is this a pointer*, not *is it spelled `void *`***
    // (measured 2026-08-15, 139 fields of all 35 raylib structs swept).
    //
    // The exact-identity form refuses `Rectangle *` for a `ptr` field, because
    // `Rectangle *` is not `void *` — and there is **no other spelling** the author
    // could write, so `error[ffi_field_type]` named a repair that does not exist.
    // Twenty-eight typed-pointer fields across ten structs — `Font`, `Mesh`,
    // `Model`, `Shader`, `Sound`, `Music`, `AudioStream`, `FilePathList`,
    // `AutomationEventList`, `ModelSkeleton` — were unbindable for that alone.
    //
    // Nothing checkable is lost. `void **` was never asking *is this the right
    // pointer*; it asked *does the header spell this member `void *`*, which is a
    // fact about the header's prose. Heroes' `ptr` is opaque — nothing in the
    // language dereferences it, indexes it or knows its pointee — so the pointee
    // type carries no obligation a check could enforce.
    //
    // **The array refusal is kept, and by a stronger mechanism than before.**
    // `__builtin_classify_type` decays an array and would let `float[4]` through
    // alone; the `_Generic` conjunct is what refuses it, because its controlling
    // expression also decays, so the association `__typeof__(…)` — an array type —
    // can never match and the array falls to `default: 0`. C11 6.5.1.1p1 and
    // 6.3.2.1p3 do that work, not a size comparison, **and that matters**: six of
    // raylib's array fields are `float[2]`, exactly `sizeof(void *)`, so a
    // widening that leaned on `sizeof` would have bound eight bytes of a
    // two-element array and said nothing.
    //
    // The `sizeof` conjunct stays for the one case it does decide: a function
    // pointer. C does not guarantee `void *` round-trips one; POSIX does, and both
    // targets do — so this makes it a **checked** assumption rather than a silent
    // one, and SDL3 has 25 such fields waiting.
    if matches!(checked.types.get(ty), Ty::Ptr) {
        return Some(format!(
            "_Static_assert(__builtin_classify_type({place}) == 5 \
             && _Generic({place}, __typeof__({place}): 1, default: 0) \
             && sizeof({place}) == sizeof(void *), \
             \"{FIELD_ASSERTION} {c_type} {member}\");"
        ));
    }
    // **An array field is a BRANCH, not a row** (panel 062's compiler-engineer,
    // whose veto this closes). C's pointer-to-array declarator is `T (*)[N]`, not
    // `T[N] *`, and `c_spelling` returns a name the caller suffixes with ` *` —
    // there is no string for which that composes. So `c_spelling` answered `None`
    // and **no assertion was emitted at all**: `leftLensCenter: f64[2]` over a
    // header's `float[2]` ran at exit 0 with the field unchecked, which is the
    // exact class this file exists to prevent.
    //
    // It arrived, in that judge's words, *"as a `None` nobody chose"* — through the
    // arm whose own comment says the next type to reach it should be a `None`
    // somebody did.
    //
    // `_Generic` on the **address** distinguishes `float[2]` from `float[4]` and
    // from `float *`, measured across ten clang verdicts, and the `sizeof`
    // conjunct is what refuses a **flexible** array member (`char data[]`), whose
    // type is compatible with any sized one.
    if let Ty::Fixed(inner, n) = checked.types.get(ty) {
        let elem = c_spelling(checked, names, inner)?;
        return Some(format!(
            "_Static_assert(_Generic(&{place}, {elem} (*)[{n}]: 1, default: 0) \
             && sizeof({place}) == sizeof({elem}[{n}]), \
             \"{FIELD_ASSERTION} {c_type} {member}\");"
        ));
    }
    let spelling = c_spelling(checked, names, ty)?;
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
