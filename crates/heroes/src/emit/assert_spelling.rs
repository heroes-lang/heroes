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
        // No other type crosses the boundary: `ffi_type` refuses them in the
        // checker, so this arm is where a new FFI type would have to declare
        // what its assertion is rather than silently getting none.
        _ => None,
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
pub(super) fn zero_of(checked: &Checked, ty: TyId, mutable: bool) -> String {
    if matches!(checked.types.get(ty), Ty::Cstr) {
        return "0".to_string();
    }
    let value = match checked.types.get(ty) {
        Ty::Int(kind) => kind.c_type(),
        Ty::Float(kind) => kind.c_type(),
        Ty::Bool => "bool",
        Ty::Str => "HeroStr",
        Ty::Cstr => unreachable!("handled above: a cstr zero is the bare null pointer constant"),
        _ => "void *",
    };
    if mutable {
        return format!("({value} *)0");
    }
    match checked.types.get(ty) {
        Ty::Str => "(HeroStr){0}".to_string(),
        _ => format!("({value})0"),
    }
}

