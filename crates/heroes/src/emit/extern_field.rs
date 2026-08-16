//! What **one field** asserts about the header, as C (design.md §4.19; panels
//! 060, 062, 064, 071, 079).
//!
//! Split from `extern_record.rs` by CLAUDE.md §11, on the seam a reader meets
//! first: that file decides **where** an assertion goes — which declarations get
//! one, on whose `#line`, in what order — and this one decides **what one field
//! claims**, which is a table of type branches and the measurement each of them
//! rests on. The two change for different reasons: a new field TYPE lands here,
//! a new placement rule lands there.
//!
//! Every branch answers the same question in the same shape — *is the header's
//! member this type* — and the module doc of the file this came from carries the
//! argument for asking it with `_Generic` on the **address** rather than with
//! `sizeof` or `offsetof` alone.

use crate::source::Source;
use crate::syntax::Field;
use crate::types::{Checked, Ty, TyId};

use super::extern_record::{FIELD_ASSERTION, FLEX_ASSERTION};
use super::mangle;
use super::typedefs::Names;

/// naming a type with no C spelling would be the compiler's own error.
/// One field's assertion, or `None` for a field whose type the checker has already
/// refused — in which case the program does not reach a binary and an assertion
/// naming a type with no C spelling would be the compiler's own error.
pub(super) fn assertion(
    checked: &Checked,
    names: &Names,
    src: &Source,
    c_type: &str,
    hero_name: &str,
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
             \"{FIELD_ASSERTION} {hero_name} {member}\");"
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
    // from `float *`, measured across ten clang verdicts. The second conjunct is
    // what refuses a **flexible** array member (`char data[]`), whose type is
    // compatible with any sized one — and it is a **tail measurement, not a
    // `sizeof` of the field** (panel 071, ffi-pragmatist, compiled).
    //
    // It used to be `sizeof({place}) == sizeof({elem}[{n}])`, and a flexible array
    // member made that a **hard error**: `invalid application of 'sizeof' to an
    // incomplete type 'char[]'`. clang cannot evaluate the assertion, so nothing
    // carries `heroes-ffi-field`, so §7's named exception does not reach it and the
    // build is **exit 2 — the compiler blaming itself** for a header the author is
    // entitled to bind.
    //
    // `sizeof(struct) - offsetof(struct, member)` is defined for both and
    // separates them: a flexible array member contributes **0** bytes to its
    // struct's size, every sized array contributes **at least `n * sizeof(elem)`**,
    // and that held in the judge's probe for the padded `char[1]` case and the
    // over-aligned `int32_t data[]` case alike. The assertion now **fails** instead
    // of failing to compile, so it carries its marker and the author gets exit 1 on
    // their own line.
    //
    // Deliberately **not** `__builtin_types_compatible_p(__typeof__(place), char[])`:
    // measured, clang answers **1** for `char[4]` against `char[]`, so that form
    // cannot separate them at all. And deliberately not a match on clang's prose —
    // `ffi_narrowed.rs` quotes cgo's reason for never doing that.
    if let Ty::Fixed(inner, n) = checked.types.get(ty) {
        let elem = c_spelling(checked, names, inner)?;
        // **Two assertions, because one message cannot answer two questions**
        // (panel 079, discharging the ffi-pragmatist's panel-071 condition). The
        // tail measurement below is *defined* for a flexible array member — that
        // is why it replaced a `sizeof` of the member, which is a hard error on
        // one — but being defined is not the same as being informative: it failed
        // with `ffi_field_type`, which tells the author the field *disagrees* and
        // to *correct* it, and for `char data[]` **there is no length that would
        // be correct**. A message naming a repair that does not exist is the one
        // thing §4.17 forbids, and a reader acting on it hunts for a number the
        // header does not have.
        //
        // The distinguishing fact is a constant expression the emitter already
        // computes. Measured: a flexible array member sits at
        // `offsetof(T, m) == sizeof(T)`, so its **tail is 0**, while a sized array
        // leaves a positive one — `{4, 4, 0}` against `{12, 4, 8}` on this
        // machine. So it is asked first and separately, and only when it holds
        // does the length question get asked at all.
        return Some(format!(
            "_Static_assert(sizeof({c_type}) - __builtin_offsetof({c_type}, {member}) != 0, \
             \"{FLEX_ASSERTION} {hero_name} {member}\");\n             _Static_assert(_Generic(&{place}, {elem} (*)[{n}]: 1, default: 0) \
             && sizeof({c_type}) - __builtin_offsetof({c_type}, {member}) >= sizeof({elem}[{n}]), \
             \"{FIELD_ASSERTION} {hero_name} {member}\");"
        ));
    }
    // **An integer field asks width and sign, never type identity** (panel 064,
    // the compiler-engineer's alternative to the vocabulary it vetoed).
    //
    // Type identity is the right question for a float, a pointer, a `bool`, an
    // enum and a nested record, and the branches above keep it there — panel 060
    // won its veto on exactly that, `{float,float}` and `{int32_t,int32_t}` being
    // one size and two register classes. It is the **wrong** question for an
    // integer, because C's integer ABI is width and sign and nothing else, while
    // C's type identity is finer than its ABI: on Darwin `int64_t` is `long long`
    // and `time_t` is `long`, same width, same sign, **distinct types**.
    //
    // What the identity form refused, measured against real SDKs: `size_t`,
    // `time_t`, `clock_t`, `ldiv_t.quot`, `struct timespec.tv_sec`, `struct
    // timeval.tv_sec`, four `struct rusage` members — 9 of 24 POSIX fields — and
    // `z_stream.total_in/total_out/adler`, which is the struct zlib cannot be used
    // without. Not *bound wrongly*: **not bindable**, under any of the eight
    // widths, with `partial` offering only the choice to drop the field. That is
    // bug-proof and not complete, and CLAUDE.md §12 asks the FFI for both.
    //
    // **The relaxation is strictly monotone** — an exact-identity match has the
    // same class, size and sign by construction — so it cannot accept less than
    // before, and that is provable rather than tested. What it still refuses is
    // everything that matters: a wrong width, a wrong sign, a float declared as an
    // integer (class 8), a pointer (5), a `bool` (4).
    //
    // **This list used to end "an enum (3)" and that was false** (panel 071,
    // measured): clang applies the default argument promotions to
    // `__builtin_classify_type`'s operand, so an enum field answers **1** — the
    // same as an integer — and the refusal the sentence described never happened.
    // What actually refuses a wrong enum field is the **sign** conjunct, and
    // correctly: SDL3's enums are unsigned, so `kind: i32` fails and `kind: u32`
    // binds, measured end-to-end on the real header. A premise about the world,
    // reading as a rule, for as long as nobody compiled it (CLAUDE.md §11).
    //
    // Deliberately **not** `__builtin_types_compatible_p`: that would need a list
    // of which C types are 64-bit-signed on this target, which is a premise about
    // the world and expires in silence (CLAUDE.md §11). Class, size and sign ask
    // the value in hand.
    //
    // It does **not** make a program portable, and the row that defers a C-width
    // vocabulary keeps that half: `timespec.tv_sec: i64` is accepted here and on
    // Linux and refused on Windows, correctly, because `long` is 32 there. What it
    // buys is *bindable on the machine in front of the author*.
    if let Ty::Int(kind) = checked.types.get(ty) {
        let spelling = kind.c_type();
        return Some(format!(
            "_Static_assert(__builtin_classify_type({place}) == 1 \
             && sizeof({place}) == sizeof({spelling}) \
             && (((__typeof__({place}))-1 < 0) == (({spelling})-1 < 0)), \
             \"{FIELD_ASSERTION} {hero_name} {member}\");"
        ));
    }
    let spelling = c_spelling(checked, names, ty)?;
    Some(format!(
        "_Static_assert(_Generic(&{place}, {spelling} *: 1, default: 0) \
         && sizeof({place}) == sizeof({spelling}), \
         \"{FIELD_ASSERTION} {hero_name} {member}\");"
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
