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
//! Only `int`, `bool` and `()` occur here at M-scalars-run. The rest of the table is present
//! because `gate.rs` — not this file — is where a form is refused: a type that has
//! a C spelling but no runtime support yet is the gate's business, and keeping the
//! spellings here means M-strings-ownership and M-value-aggregates delete gate rows instead of adding cases.

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
    /// The C type of each distinct `T?`, by the `TyId` of the whole `T?`.
    ///
    /// Generated rather than declared: there is no `record` in the source for a
    /// `T?`, and every `T` needs its own struct because the payload is by value.
    /// Named by INDEX rather than by spelling — `int?` and `[int]?` sanitise to the
    /// same identifier, and a collision here is two types sharing one C name. The
    /// index is assigned in `TyId` order, which is a function of the program, so
    /// the double-emit determinism test (CLAUDE.md §7) covers it.
    options: std::collections::BTreeMap<u32, String>,
    /// The C type of one case's payload, by `(declaration, case)`. A separate table
    /// because `Ty::Case` is a real type in the IR and needs a real C name.
    cases: std::collections::BTreeMap<(u32, u32), String>,
    /// The C typedef of each distinct function type, by the `TyId` of the whole
    /// `(function(A) -> B)`.
    ///
    /// A typedef and not an inline spelling, because C puts the declarator's name
    /// **inside** the type: `int64_t (*f)(int64_t)`. Every other type in this
    /// backend is `<type> <name>`, and one exception would have to be threaded
    /// through the prologue, the parameter list, the struct fields and the
    /// temporaries. One `typedef` keeps the rule.
    ///
    /// Named by INDEX for the reason the options are: two different function types
    /// sanitise to the same identifier, and a collision here is two types sharing
    /// one C name. The index follows `TyId` order, which is a function of the
    /// program, so the double-emit determinism test covers it.
    funcs: std::collections::BTreeMap<u32, String>,
}

/// What separates a name the **emitter invents** from a name an author could
/// write: a leading digit, which `h_<component>_<name>` can carry and a Heroes
/// identifier cannot.
///
/// The option and function typedefs belong to no declaration, so they are named
/// `h_<module>_<N>opt<N>` rather than after anything in the source — and until
/// 2026-08-12 they were `h_<module>_opt<N>`, which a `record opt0` in the same
/// module spells exactly. Measured: `redefinition of 'h_m_opt0'`, exit 2, on a
/// legal program — and only under some roots, since a user type is named in *its
/// own* module while these take the root's.
///
/// This is panel 031 R10's rule extended to the names the compiler makes up.
/// R10 refuses two module names that sanitise to one component; the residual it
/// left open is that the emitter's own vocabulary was never reserved at all. A
/// leading digit closes it by construction rather than by a check: `0foo` is
/// `error[expected_name]` in the lexer, so there is nothing to collide with and
/// no diagnostic to write.
const SYNTHETIC: &str = "0";

impl Names {
    pub(super) fn new(ast: &Ast, src: &Source) -> Names {
        let mut aggregates = std::collections::BTreeMap::new();
        let mut cases = std::collections::BTreeMap::new();
        for (index, decl) in ast.decls.iter().enumerate() {
            // **Each type is named in its OWN module**, never the root's: two
            // modules may each declare `Point`, and they are two C structs.
            let name = mangle::ty(src.component_at(decl.name.start), src.slice(decl.name));
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
        Names {
            aggregates,
            cases,
            options: std::collections::BTreeMap::new(),
            funcs: std::collections::BTreeMap::new(),
        }
    }

    /// Assigns a C name to every `T?` the program interned. Called once, after the
    /// aggregates, because it needs the type table rather than the syntax.
    /// **Walked over the interned arena, unlike `with_functions` below**, and
    /// M-ffi-ladder is when that started to cost something: `read_file -> str?` and
    /// `write_file -> ()?` are the first library functions with a `T?` in their
    /// signatures, so every translation unit in the language now carries two
    /// option structs and eight per-type functions — including one that only adds
    /// two integers.
    ///
    /// **Deriving the set from the emitted functions was tried and reverted.** A
    /// walk over slots, results and instruction types misses a `T?` reached
    /// through a declared record's field, and a missing name is
    /// `every `T?` is named before anything can mention one` — a hard error, found
    /// by the mutant corpus rather than by any case somebody wrote. The honest
    /// filter needs the declaration graph as well as the IR, and it is queued
    /// rather than half-done.
    pub(super) fn with_options(mut self, module: &str, checked: &Checked) -> Names {
        for index in 0..checked.types.len() {
            let id = TyId(index as u32);
            // **A type that still mentions a parameter has no C declaration**, and
            // the arena still holds the templates': monomorphisation deletes the
            // generic *functions*, not the `A?` their signatures interned. Emitting
            // one gave `HeroValue ok;` — `error: unknown type name 'HeroValue'` —
            // because `c_type`'s catch-all is the only arm a `Ty::Generic` reaches.
            if mentions_generic(checked, id) {
                continue;
            }
            if matches!(checked.types.get(id), Ty::Fallible(_)) {
                let at = self.options.len();
                self.options.insert(id.0, format!("h_{module}_{SYNTHETIC}opt{at}"));
            }
        }
        self
    }

    /// Assigns a C typedef name to every function type the program **uses as a
    /// type** — a slot, a temporary, a parameter.
    ///
    /// Derived from the program, not walked over the interned arena, and for the
    /// same reason `descriptors.rs` derives its set: the checker interns a
    /// `Ty::Func` for *every* top-level declaration, so an arena walk emits a
    /// typedef per function in the file, almost all of them named by nothing. An
    /// unused typedef is not a warning the way an unused `static const` is, which
    /// is exactly why it has to be deliberate — it would sit in `--emit-c`
    /// forever with nothing to make it fall out.
    pub(super) fn with_functions(
        mut self,
        module: &str,
        checked: &Checked,
        program: &crate::ir::Program,
    ) -> Names {
        let mut used: std::collections::BTreeSet<u32> = std::collections::BTreeSet::new();
        for function in &program.functions {
            for slot in &function.slots {
                collect(checked, slot.ty, &mut used);
            }
            for value in &function.values {
                collect(checked, *value, &mut used);
            }
        }
        // Named in `TyId` order so the C is a function of the program and not of
        // the walk — which is what the double-emit determinism test covers.
        for id in &used {
            let at = self.funcs.len();
            self.funcs.insert(*id, format!("h_{module}_{SYNTHETIC}fn{at}"));
        }
        self
    }

    /// The typedef name, if this function type is one the program uses.
    pub(super) fn func_name(&self, ty: TyId) -> Option<String> {
        self.funcs.get(&ty.0).cloned()
    }

    pub(super) fn func_of(&self, ty: TyId) -> &str {
        self.funcs
            .get(&ty.0)
            .map(|name| name.as_str())
            .expect("every function type is named before anything can mention one")
    }

    pub(super) fn option_of(&self, ty: TyId) -> &str {
        self.options
            .get(&ty.0)
            .map(|name| name.as_str())
            .expect("every `T?` is named before anything can mention one")
    }

    /// Every `T?`, as `(name, the payload type)`, in emission order — the
    /// per-option generated functions walk this.
    pub(super) fn options(&self, checked: &Checked) -> Vec<(String, TyId)> {
        self.options
            .iter()
            .filter_map(|(id, name)| match checked.types.get(TyId(*id)) {
                Ty::Fallible(inner) => Some((name.clone(), inner)),
                _ => None,
            })
            .collect()
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
fn collect(checked: &Checked, ty: TyId, into: &mut std::collections::BTreeSet<u32>) {
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
