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

use crate::source::Source;
use crate::syntax::{Ast, DeclKind};
use crate::types::{Checked, Ty, TyId};

use super::mangle;

/// The C typedef name of every declared aggregate, by declaration index.
///
/// A table rather than a lookup on demand, because the name needs the *module* and
/// the source text, and `Checked` carries neither: threading both into `c_type`
/// would put four parameters on the emitter's smallest function. Built once per
/// translation unit, where the module is decided.
pub(super) struct Names {
    aggregates: std::collections::BTreeMap<u32, String>,
    /// The C type of one case's payload, by `(declaration, case)`. A separate table
    /// because `Ty::Case` is a real type in the IR and needs a real C name.
    cases: std::collections::BTreeMap<(u32, u32), String>,
}

impl Names {
    pub(super) fn new(module: &str, ast: &Ast, src: &Source) -> Names {
        let mut aggregates = std::collections::BTreeMap::new();
        let mut cases = std::collections::BTreeMap::new();
        for (index, decl) in ast.decls.iter().enumerate() {
            let name = mangle::ty(module, src.slice(decl.name));
            match &decl.kind {
                DeclKind::Record { .. } => {
                    aggregates.insert(index as u32, name);
                }
                DeclKind::Variant { cases: declared } => {
                    for (at, case) in declared.iter().enumerate() {
                        cases.insert(
                            (index as u32, at as u32),
                            mangle::case_type(&name, src.slice(case.name)),
                        );
                    }
                    aggregates.insert(index as u32, name);
                }
                _ => {}
            }
        }
        Names { aggregates, cases }
    }

    /// The C type of one case's payload.
    pub(super) fn case_of(&self, decl: u32, case: u32) -> &str {
        self.cases
            .get(&(decl, case))
            .map(|name| name.as_str())
            .expect("a Ty::Case always names a case of a declared variant")
    }

    /// The C name of an aggregate. A `TyId` naming a declaration that is not one is a
    /// compiler bug: the checker would have had to intern `Ty::Named` for a function.
    pub(super) fn of(&self, decl: u32) -> &str {
        self.aggregates
            .get(&decl)
            .map(|name| name.as_str())
            .expect("a Ty::Named always names a record or a variant")
    }
}

/// The C type, or `None` for `()`, which has no declaration.
pub(super) fn c_type(names: &Names, checked: &Checked, ty: TyId) -> Option<String> {
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
        // The map and `T?`: later steps own the representation, and `gate.rs`
        // refuses them until then.
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
