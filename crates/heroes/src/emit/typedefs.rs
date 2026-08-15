//! What every declared type is CALLED in C (design.md §3.1; panel 031 R10).
//!
//! Split out of `ctype.rs` by the §11 sweep. `ctype.rs` answers what a type *is*
//! in C; this answers what it is *named*, and the two are different jobs with
//! different failure modes — a wrong spelling is a clang error, a wrong name is a
//! **collision**, which clang reports about a line nobody wrote.
//!
//! A table rather than a lookup on demand, because a name needs the *module* and
//! the source text and `Checked` carries neither: threading both into `c_type`
//! would put four parameters on the emitter's smallest function. Built once per
//! translation unit, where the module is decided.
//!
//! **The rule that keeps the emitter's own vocabulary apart from the author's is a
//! leading digit** — something `h_<component>_<name>` can carry and a Heroes
//! identifier cannot. The option and function typedefs belong to no declaration, so
//! they are named `h_<module>_<N>opt<N>`; until 2026-08-12 they were
//! `h_<module>_opt<N>`, which a `record opt0` in the same module spells exactly.
//! Measured: `redefinition of 'h_m_opt0'`, exit 2, on a legal program — and only
//! under some roots, since a user type is named in *its own* module while these
//! take the root's. This is panel 031 R10 extended to the names the compiler makes
//! up: R10 refuses two module names that sanitise to one component, and the
//! residual it left open was that the emitter's own vocabulary was never reserved
//! at all. A leading digit closes it by construction rather than by a check —
//! `0foo` is `error[expected_name]` in the lexer, so there is nothing to collide
//! with and no diagnostic to write.

use crate::source::Source;
use crate::syntax::{Ast, DeclKind};
use crate::types::{Checked, Ty, TyId};

use super::mangle;

/// The two names one aggregate answers to: the prefix its generated functions
/// carry, and the C type it *is*.
///
/// **They differ for exactly one kind of declaration** — a `record` inside an
/// `extern` group, whose type is the header's and whose `_eq` must still be this
/// compiler's — and agree for every other, which is what made a single name look
/// right for as long as no header was involved. Owned rather than borrowed
/// (CLAUDE.md §5), and one value rather than two parameters, because the
/// functions that need it already carry nine and ten.
#[derive(Clone)]
pub(super) struct Aggregate {
    /// This compiler's mangled name — what `_eq`, `_hash`, `_desc`, `_retain` and
    /// `_release` are spelled with. Never the header's, or the emitter writes a
    /// global unmangled `Color_eq` beside the library that declared `Color`.
    pub(super) prefix: String,
    /// The C type — the header's own spelling for a group's `record`.
    pub(super) c_type: String,
    /// Whether the struct is the **header's** rather than this compiler's, which
    /// decides how its members are spelled as well as what it is called.
    pub(super) foreign: bool,
    /// Whether the field list names only **some** of that struct — which decides
    /// what the generated `_eq` and `_hash` are allowed to do (panel 061).
    pub(super) partial: bool,
}

/// The C typedef name of every declared aggregate, by declaration index.
///
/// A table rather than a lookup on demand, because the name needs the *module* and
/// the source text, and `Checked` carries neither: threading both into `c_type`
/// would put four parameters on the emitter's smallest function. Built once per
/// translation unit, where the module is decided.
pub(super) struct Names {
    /// The C **type** name of each declared aggregate — the header's own spelling
    /// for a group's `record`, this compiler's mangled one for everything else.
    aggregates: std::collections::BTreeMap<u32, String>,
    /// The prefix the **generated functions** for that aggregate carry — `_eq`,
    /// `_hash`, `_desc`, `_retain`, `_release` — and **always mangled**, whatever
    /// `aggregates` says.
    ///
    /// Two tables because one name was doing two jobs, and the jobs part *inside*
    /// single format strings: `structural.rs`'s signature is
    /// `bool {name}_eq(const {name} *a, …)`, where the first must be ours and the
    /// second must be the header's. Unsplit, a `record Color` in an `extern`
    /// group made this emitter write a **global, unmangled `Color_eq`** beside the
    /// library that declared `Color` — CLAUDE.md §7's mangler rule, and the exact
    /// collision `extern_probe.rs`'s own doc records panel 053 finding for probes
    /// (panel 060, compiler-engineer's condition 3).
    satellites: std::collections::BTreeMap<u32, String>,
    /// The declarations whose struct is the header's.
    foreign: std::collections::BTreeSet<u32>,
    /// Those of them whose field list is only part of it.
    partial: std::collections::BTreeSet<u32>,
    /// The C type of each distinct `T?`, by the `TyId` of the whole `T?`.
    ///
    /// Generated rather than declared: there is no `record` in the source for a
    /// `T?`, and every `T` needs its own struct because the payload is by value.
    /// Named by INDEX rather than by spelling — `int?` and `[i64]?` sanitise to the
    /// same identifier, and a collision here is two types sharing one C name. The
    /// index is assigned in `TyId` order, which is a function of the program, so
    /// the double-emit determinism test (CLAUDE.md §7) covers it.
    pub(super) options: std::collections::BTreeMap<u32, String>,
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
    pub(super) funcs: std::collections::BTreeMap<u32, String>,
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
pub(super) const SYNTHETIC: &str = "0";

impl Names {
    pub(super) fn new(ast: &Ast, src: &Source) -> Names {
        let mut aggregates = std::collections::BTreeMap::new();
        let mut satellites = std::collections::BTreeMap::new();
        let mut foreign = std::collections::BTreeSet::new();
        let mut partial = std::collections::BTreeSet::new();
        let mut cases = std::collections::BTreeMap::new();
        for (index, decl) in ast.decls.iter().enumerate() {
            // **Each type is named in its OWN module**, never the root's: two
            // modules may each declare `Point`, and they are two C structs.
            let name = mangle::ty(src.component_at(decl.name.start), src.slice(decl.name));
            match &decl.kind {
                DeclKind::Record { header, partial: is_partial, .. } => {
                    if *is_partial {
                        partial.insert(index as u32);
                    }
                    satellites.insert(index as u32, name.clone());
                    // **A group's `record` IS the header's struct**, so its C type
                    // name is the one the author wrote, unmangled — the same
                    // exception §4.19 makes for an `extern` function's linker
                    // name, and for the same reason: it must match something this
                    // compiler did not write. The emitter writes no `typedef` for
                    // it at all (`types.rs`), so there is nothing to collide with.
                    let c_name = match header {
                        Some(_) => {
                            foreign.insert(index as u32);
                            src.slice(decl.name).to_string()
                        }
                        None => name,
                    };
                    aggregates.insert(index as u32, c_name);
                }
                DeclKind::Variant { cases: declared } => {
                    satellites.insert(index as u32, name.clone());
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
            satellites,
            foreign,
            partial,
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
        // ORDER: ascending TyId — the per-option generated functions are emitted
        // in this order, so the Heroes port owes an explicit sort (design.md §4.9).
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

    /// The C **type** name of an aggregate — what a declaration, a parameter or a
    /// cast writes. A `TyId` naming a declaration that is not one is a compiler
    /// bug: the checker would have had to intern `Ty::Named` for a function.
    pub(super) fn of(&self, decl: u32) -> &str {
        self.aggregates
            .get(&decl)
            .map(|name| name.as_str())
            .expect("a Ty::Named always names a record or a variant")
    }

    /// The prefix this aggregate's **generated functions** carry — always this
    /// compiler's mangled name, never the header's.
    ///
    /// Use it wherever the string is followed by `_eq`, `_hash`, `_desc`,
    /// `_retain` or `_release`; use `of` wherever it stands as a type. The two
    /// differ for exactly one kind of declaration and agree for every other, which
    /// is what makes picking the wrong one silent until a header is involved.
    pub(super) fn satellite(&self, decl: u32) -> &str {
        self.satellites
            .get(&decl)
            .map(|name| name.as_str())
            .expect("a Ty::Named always names a record or a variant")
    }

    /// Both names at once, for the emitters that write a definition rather than a
    /// mention.
    pub(super) fn aggregate(&self, decl: u32) -> Aggregate {
        Aggregate {
            prefix: self.satellite(decl).to_string(),
            c_type: self.of(decl).to_string(),
            foreign: self.foreign.contains(&decl),
            partial: self.partial.contains(&decl),
        }
    }

    /// A generated payload type is this compiler's own invention, so both names
    /// are the same one — stated rather than left implicit, because the pair
    /// exists precisely where that is false.
    pub(super) fn generated(name: &str) -> Aggregate {
        Aggregate {
            prefix: name.to_string(),
            c_type: name.to_string(),
            foreign: false,
            partial: false,
        }
    }
}
