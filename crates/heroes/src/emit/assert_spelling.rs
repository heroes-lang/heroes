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
        // **A struct result, and it took a panel to get an assertion at all**
        // (panel 063, soundness lane, both judges `approve`). Until then this arm
        // was `None` above a sentence claiming *"no other type crosses the
        // boundary"*, false since panel 060.
        //
        // Measured with raylib: `-> Vector2` for a `Color`-returning function
        // **builds at exit 0** while nothing calls it, and calling it is `assigning
        // to 'Vector2' from incompatible type 'Color'` at **exit 2** — the compiler
        // blaming itself for the author's declaration. **113 of raylib's 600** entry
        // points return a struct by value; SDL3, 4 of 1248.
        //
        // The class is **misattributed exit 2, never a silent wrong answer**, and
        // that is worse rather than better. C struct assignment is *nominal* (C11
        // 6.2.7p1), so two distinct tags are incompatible whatever they contain —
        // `Vector4` declared as `Rectangle`, identical size, alignment and members,
        // is refused. A silent wrong answer is a bug in a program; *"internal
        // error: compiling the generated C failed"* is CLAUDE.md §7's named
        // exception defeated.
        //
        // `HERO_RET_RECORD` takes **two** arguments where every other macro takes
        // one, which is why `return_argument` exists. `_Generic` was measured to
        // work and is refused anyway: its association type must be **complete**
        // (C11 6.5.1.1p2), and an incomplete one is a C2y extension Apple clang
        // accepts *silently* at this project's flags — the `HERO_RET_UNIT` defect
        // three lines of comment away, which survived two milestones on this laptop
        // and failed on the first CI run that compared two machines. `sizeof` was
        // refused for accepting panel 060's veto case.
        Ty::Named(_) => Some("HERO_RET_RECORD"),
        // **The loud direction** (CLAUDE.md §11). Everything else is refused by
        // `ffi_decls::crosses_the_boundary` before a program can reach the emitter,
        // so a new FFI type arrives here as a compile-time decision instead of
        // silently getting no assertion — which is exactly what the deleted comment
        // promised and did not do.
        other => unreachable!("this type does not cross the FFI boundary: {other:?}"),
    }
}

/// The **second** argument a return check takes, or `""` for the eight that take
/// one (panel 063).
///
/// Every other `HERO_RET_*` names the expected type in the macro itself, so the
/// call site is `HERO_RET_INT(f(...))`. A struct cannot: there is one macro and a
/// header's worth of struct names, so the type travels as an argument and the call
/// site is `HERO_RET_RECORD(f(...), Color)`.
///
/// **The name is `Names::of`, the C type table, and not `Names::satellite`.** The
/// two were split at panel 060 and five call sites read the wrong one for a
/// milestone, so `a == b` on any group record was `call to undeclared function
/// 'Color_eq'` at exit 2. What goes inside a `_Static_assert` about a header's own
/// function is the header's own spelling.
pub(super) fn return_argument(
    names: &super::typedefs::Names,
    checked: &Checked,
    ty: TyId,
) -> String {
    match checked.types.get(ty) {
        Ty::Named(decl) => format!(", {}", names.of(decl)),
        _ => String::new(),
    }
}

/// Whether a result type can be asked `__builtin_constant_p` — **false for a
/// struct, and the hole that leaves is written here rather than absorbed** (panel
/// 063, the compiler-engineer's condition 1).
///
/// The falsifiable claim: **`__builtin_constant_p` answers 0 for every struct on
/// this toolchain, including a fully-constant compound literal.** It is a fact
/// about clang today, not about C, and it is why raylib's 26 `CLITERAL(Color)`
/// macros bind and run at exit 0 while a naive `Ty::Named` arm would newly refuse
/// every one of them.
///
/// What it costs is a real hole and the fix makes it *look* closed. §4.2's back
/// door — a zero-argument accessor over a mutable global returning two answers on
/// two reads — stays open for a **struct** `extern constant`, and after this change
/// an auditor sees a type assertion on that line where there were none at all.
/// The test that fires when the premise dies is
/// `a_struct_constant_cannot_be_asked_for_a_value`.
pub(super) fn asks_for_a_value(checked: &Checked, ty: TyId) -> bool {
    !matches!(checked.types.get(ty), Ty::Named(_))
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

