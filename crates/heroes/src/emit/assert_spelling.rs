//! How a type is spelled inside an assertion that is **never evaluated**.
//!
//! Split from `extern_assert.rs` by the §11 ceiling, 2026-08-15, and the seam is
//! the never-evaluated part rather than the line count. `extern_assert.rs` walks
//! the program and writes `_Static_assert` lines; these two answer a narrower
//! question that has nothing to do with walking anything — *given a declared
//! Heroes type, what C spelling makes the call expression well-formed without
//! running it*. `_Generic`'s controlling expression is unevaluated (C11
//! 6.5.1.1p3), which is what makes the whole mechanism free and is also why
//! nothing here may have a side effect or a cost.
//!
//! It is the half that gets subtly wrong answers, and both of this file's
//! comments record a repair that was built and measured rather than reasoned.

use crate::types::{Checked, IntKind, Ty, TyId};
use crate::types::FloatKind;

/// Which assertion a declared result type asks for.
pub(super) fn return_check(checked: &Checked, ty: TyId) -> Option<&'static str> {
    match checked.types.get(ty) {
        // Exhaustive: `HERO_RET_INT` accepts every signed C integer plus every
        // unsigned one narrower than 64 bits, and that set is a fact about
        // `i64`'s range and about nothing else. A second width reusing it would
        // accept a C `long` for an `i32` and truncate in silence (panel 042).
        Ty::Int(kind) => Some(match kind {
            IntKind::I64 => "HERO_RET_INT",
            IntKind::I8 => "HERO_RET_I8",
            IntKind::I16 => "HERO_RET_I16",
            IntKind::I32 => "HERO_RET_I32",
            IntKind::U8 => "HERO_RET_U8",
            IntKind::U16 => "HERO_RET_U16",
            IntKind::U32 => "HERO_RET_U32",
            IntKind::U64 => "HERO_RET_U64",
        }),
        Ty::Float(FloatKind::F32) => Some("HERO_RET_F32"),
        Ty::Float(FloatKind::F64) => Some("HERO_RET_F64"),
        Ty::Bool => Some("HERO_RET_BOOL"),
        Ty::Str => Some("HERO_RET_STR"),
        Ty::Unit => Some("HERO_RET_UNIT"),
        Ty::Ptr | Ty::Cstr => Some("HERO_RET_PTR"),
        // **A struct result gets no assertion, and this arm is where that is
        // decided rather than where it is hidden** (panel 062's audit). The `_ =>
        // None` this replaces carried the sentence *"no other type crosses the
        // boundary: `ffi_type` refuses them in the checker"*, which stopped being
        // true at panel 060: a `record` a header declares crosses, and there is no
        // `HERO_RET_<struct>` macro for it, so `extern_assertions` skips it.
        //
        // Measured on this tree, 2026-08-15, with raylib installed: `extern
        // function ColorAlpha(color: Color, alpha: f32) -> Vector2` — raylib
        // returns `Color` — **builds at exit 0** when nothing calls it, and when
        // something does, clang refuses the *call site* with `assigning to
        // 'Vector2' from incompatible type 'Color'` at **exit 2**, the compiler
        // blaming itself for the author's declaration. That is panel 036's
        // original defect, alive again for the 349 of raylib's 600 entry points
        // that return a struct.
        //
        // It is left as it is here on purpose: closing it means a new assertion
        // macro and a new `heroes-ffi-` class, which is CLAUDE.md §4's panel
        // trigger, not an audit's repair.
        Ty::Named(_) => None,
        // **The loud direction** (CLAUDE.md §11). Everything else is refused by
        // `ffi_decls::crosses_the_boundary` before a program can reach the emitter,
        // so a new FFI type arrives here as a compile-time decision instead of
        // silently getting no assertion — which is exactly what the deleted comment
        // promised and did not do.
        other => unreachable!("this type does not cross the FFI boundary: {other:?}"),
    }
}

/// A zero of the declared parameter type, cast so the call type-checks. It is
/// never evaluated — it exists only to make the call expression well-formed.
///
/// **A `cstr`'s zero is the bare `0`, and it is the one type here that must not be
/// cast** (author decision 2026-08-15, panel 058 condition 1). C11 6.3.2.3p3 makes
/// an integer constant expression of value 0 a *null pointer constant*, assignable
/// to any pointer type **at any qualification** — so `0` type-checks against `char
/// *`, `const char *`, `char **` and `const char **` alike, and this expression is
/// never evaluated anyway.
///
/// A cast cannot do that, and the two ways of getting it wrong are symmetric:
/// `(const char *)0` — what this wrote until today — makes the emitter warn about
/// **its own** `_Static_assert` for every `char *` parameter in any header, which
/// is `strtok`, `getcwd` and 157 more; `(char *)0` fixes those and then discards
/// qualifiers on a **correct** `const char **` out-parameter, measured on
/// `sqlite3_prepare_v2`'s `pzTail`. Both were built and reproduced before this
/// landed. There is no third spelling of a cast that is right for both, because
/// the parameter's qualification is the header's to choose and this function does
/// not read it.
///
/// This is a repair on its own merits: with it, the only qualifier diagnostic left
/// on a `char *` binding is the **probe's**, on the author's own line, which is
/// where panel 058 wants it.
pub(super) fn zero_of(
    names: &super::typedefs::Names,
    checked: &Checked,
    ty: TyId,
    mutable: bool,
) -> String {
    if matches!(checked.types.get(ty), Ty::Cstr) {
        return "0".to_string();
    }
    let value = match checked.types.get(ty) {
        Ty::Int(kind) => kind.c_type(),
        Ty::Float(kind) => kind.c_type(),
        Ty::Bool => "bool",
        Ty::Str => "HeroStr",
        Ty::Cstr => unreachable!("handled above: a cstr zero is the bare null pointer constant"),
        // **A struct's zero is a compound literal of the header's own type**
        // (panel 060). `(void *)0` is what this arm produced while the catch-all
        // covered `Ty::Named`, and clang refused it correctly — *passing 'void *'
        // to parameter of incompatible type 'Color'* — at **exit 2**, which is the
        // compiler blaming itself for a binding the author was entitled to write.
        Ty::Named(decl) => {
            let c_type = names.of(decl);
            return if mutable { format!("({c_type} *)0") } else { format!("({c_type}){{0}}") };
        }
        // §4.19's opaque pointer. It was the reason the catch-all below looked
        // load-bearing, and it is one arm rather than a fallback.
        Ty::Ptr => "void *",
        // **`()` as a parameter type reaches here, and `(void *)0` is the wrong
        // answer** (measured 2026-08-15, panel 062's audit). `ffi_decls`'s
        // `crosses_the_boundary` is shared by results and parameters, and `()` is a
        // legal *result*, so `extern function abs(n: ()) -> i32` type-checks — and
        // then the assertion this writes is `abs((void *)0)`, which is `error:
        // incompatible pointer to integer conversion` at **exit 2**: the compiler
        // blaming itself for the author's declaration.
        //
        // The spelling is left unchanged because no C spelling of a `()` argument
        // exists to replace it with; the repair is a frontend refusal of a `()`
        // parameter, which is a diagnostic class and therefore CLAUDE.md §4's
        // panel. What the arm buys today is that the mistake is *named* here rather
        // than carried by a `_`.
        Ty::Unit => "void *",
        other => unreachable!("this type does not cross the FFI boundary: {other:?}"),
    };
    if mutable {
        return format!("({value} *)0");
    }
    match checked.types.get(ty) {
        Ty::Str => "(HeroStr){0}".to_string(),
        _ => format!("({value})0"),
    }
}

