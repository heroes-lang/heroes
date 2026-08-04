//! A Heroes type as a C type — and the one case where the answer is "no
//! declaration at all" (design.md §3.1, panel 020).
//!
//! **The unit rule.** `()` is a real type in the IR: `$t0` is the unit value of
//! every function (`ir/build.rs`), `Function::values` is dense over `ValueId`, and
//! the natural way to hoist temporaries is to walk that table. Walk it naively and
//! the emitter writes `void t0;`, which is `error: variable has incomplete type
//! 'void'` — a hard error, on the first program. So a unit-typed temporary is
//! **never declared and never named**, and a `Return(Some(v))` whose value is unit
//! is `return;`. The panel's compiler-engineer measured this one before any line of
//! the emitter existed.
//!
//! Only `int`, `bool` and `()` occur here at M5a. The rest of the table is present
//! because `gate.rs` — not this file — is where a form is refused: a type that has
//! a C spelling but no runtime support yet is the gate's business, and keeping the
//! spellings here means M5b and M5c delete gate rows instead of adding cases.

use crate::types::{Checked, Ty, TyId};

/// The C type, or `None` for `()`, which has no declaration.
pub(super) fn c_type(checked: &Checked, ty: TyId) -> Option<String> {
    match checked.types.get(ty) {
        Ty::Unit => None,
        Ty::Int => Some("int64_t".to_string()),
        Ty::Bool => Some("bool".to_string()),
        Ty::F64 => Some("double".to_string()),
        // A fat pointer, passed BY VALUE (§4.20, panel 021): 16 bytes, two
        // registers, refcount and magic in a heap header before the bytes. By
        // value because of the FFI, not for comfort — written as a pointer, the
        // wrong `str`→`cstr` conversion compiles clean *with an explicit cast* and
        // hands a refcount word to `sqlite3_open`; written by value it is
        // `error: operand of type 'HeroStr' where arithmetic or pointer type is
        // required`, which is inexpressible rather than wrong.
        Ty::Str => Some("HeroStr".to_string()),
        // §4.19's two opaque types. They reach C only through an `extern`, which
        // this backend refuses until M7.
        Ty::Ptr => Some("void *".to_string()),
        Ty::Cstr => Some("const char *".to_string()),
        // Everything else is a container, a descriptor or a `T?`: M5b and M5c own
        // the representation, and `gate.rs` refuses them until then.
        _ => Some("HeroValue".to_string()),
    }
}

/// The type a *function* returns, in C. Unit is `void` — the one place the absence
/// of a type has a spelling.
pub(super) fn c_result(checked: &Checked, ty: TyId) -> String {
    match c_type(checked, ty) {
        Some(name) => name,
        None => "void".to_string(),
    }
}

pub(super) fn is_unit(checked: &Checked, ty: TyId) -> bool {
    checked.types.get(ty) == Ty::Unit
}
