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
//! Only `i64`, `bool` and `()` occur here at M-scalars-run. The rest of the table is present
//! because `gate.rs` — not this file — is where a form is refused: a type that has
//! a C spelling but no runtime support yet is the gate's business, and keeping the
//! spellings here means M-strings-ownership and M-value-aggregates delete gate rows instead of adding cases.

use crate::types::{Checked, Ty, TyId};

pub(super) use super::typedefs::Names;

/// Does this type mention a type parameter anywhere inside it?
///
/// Asked of every generated declaration, because the interner keeps the
/// templates' types after monomorphisation has deleted the functions that used
/// them. `ir/phases.rs` asserts no *instruction* carries one; this is the arena's
/// half of the same claim.
pub(super) fn mentions_generic(checked: &Checked, ty: TyId) -> bool {
    match checked.types.get(ty) {
        Ty::Generic(_) => true,
        Ty::Array(element) => mentions_generic(checked, element),
        Ty::Fallible(inner) => mentions_generic(checked, inner),
        Ty::Map(key, value) => {
            mentions_generic(checked, key) || mentions_generic(checked, value)
        }
        Ty::Func { params, result } => {
            checked.types.params_of(params).iter().any(|p| mentions_generic(checked, *p))
                || mentions_generic(checked, result)
        }
        _ => false,
    }
}

/// Every function type reachable from `ty`, including `ty` itself.
///
/// Closed under nesting, because a `[(function(A) -> B)]` names the function type
/// without being one, and so does `(function(A) -> B)?`.
pub(super) fn collect(checked: &Checked, ty: TyId, into: &mut std::collections::BTreeSet<u32>) {
    match checked.types.get(ty) {
        Ty::Func { params, result } => {
            if !into.insert(ty.0) {
                return;
            }
            for param in checked.types.params_of(params) {
                collect(checked, param, into);
            }
            collect(checked, result, into);
        }
        Ty::Array(element) => collect(checked, element, into),
        Ty::Fallible(inner) => collect(checked, inner, into),
        Ty::Map(key, value) => {
            collect(checked, key, into);
            collect(checked, value, into);
        }
        _ => {}
    }
}

/// The C type, or `None` for `()`, which has no declaration.
pub(super) fn c_type(names: &Names, checked: &Checked, ty: TyId) -> Option<String> {
    match checked.types.get(ty) {
        Ty::Unit => None,
        Ty::Int(kind) => Some(kind.c_type().to_string()),
        Ty::Bool => Some("bool".to_string()),
        Ty::Float(kind) => Some(kind.c_type().to_string()),
        // A fat pointer, passed BY VALUE (§4.20, panel 021): 16 bytes, two
        // registers, refcount and magic in a heap header before the bytes. By
        // value because of the FFI, not for comfort — written as a pointer, the
        // wrong `str`→`cstr` conversion compiles clean *with an explicit cast* and
        // hands a refcount word to `sqlite3_open`; written by value it is
        // `error: operand of type 'HeroStr' where arithmetic or pointer type is
        // required`, which is inexpressible rather than wrong.
        Ty::Str => Some("HeroStr".to_string()),
        // §4.19's two opaque types. They reach C only through an `extern`, which
        // this backend refuses until M-ffi-ladder.
        Ty::Ptr => Some("void *".to_string()),
        Ty::Cstr => Some("const char *".to_string()),
        // A record is a C struct BY VALUE and a variant a tagged union by value
        // (§4.10, §4.20, panel 022 confirming spike 04). `Ty::Case(d, c)` is the same
        // C type as `Ty::Named(d)`: a case is not a type of its own at runtime, it is
        // the whole variant with a known tag, and the checker's narrower view of it
        // stops mattering once the tag is a field.
        // A variant is the whole tagged union; a *case* is its payload alone, which
        // is a type of its own because the IR puts one in a temporary
        // (`$t5: Token.num = payload $t4 .num`).
        Ty::Named(decl) => Some(names.of(decl).to_string()),
        Ty::Case(decl, case) => Some(names.case_of(decl, case).to_string()),
        // One pointer, whatever it holds — which is what gives a recursive type a
        // finite size (§4.10) and what makes an array field impose no ordering
        // constraint on C.
        Ty::Array(_) => Some("HeroArrayHeader *".to_string()),
        // A `T?` is a by-value tagged union, one generated struct per payload type;
        // its error side is the runtime's own record, since §4.6 fixes its shape.
        Ty::Fallible(_) => Some(names.option_of(ty).to_string()),
        Ty::Failure => Some("HeroFailure".to_string()),
        // One pointer, like the array: a header with three parallel regions after it.
        Ty::Map(_, _) => Some("HeroMapHeader *".to_string()),
        // A C function pointer, and nothing more — no captured environment, because
        // v1 has no closures. panel 013 recorded the consequence in advance: a
        // capturing closure is a record plus a pointer, so the day closures arrive
        // the type system has to distinguish capture-free AT THE BOUNDARY. Until
        // then a Heroes function value and a C callback are the same eight bytes,
        // which is what makes `qsort` and every raylib callback expressible.
        Ty::Func { .. } => Some(names.func_of(ty).to_string()),
        // The two the checker keeps for its own bookkeeping never reach here.
        _ => Some("HeroValue".to_string()),
    }
}

/// The type a *function* returns, in C. Unit is `void` — the one place the absence
/// of a type has a spelling.
pub(super) fn c_result(names: &Names, checked: &Checked, ty: TyId) -> String {
    match c_type(names, checked, ty) {
        Some(name) => name,
        None => "void".to_string(),
    }
}

pub(super) fn is_unit(checked: &Checked, ty: TyId) -> bool {
    checked.types.get(ty) == Ty::Unit
}
