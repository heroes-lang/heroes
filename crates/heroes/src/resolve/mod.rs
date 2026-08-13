//! The resolver: every name, and what it means (design.md Part 10 step 4,
//! §4.4 bindings, §4.16 the hole exemption; ROADMAP M-name-resolution).
//!
//! One pass over a *clean* tree answers four questions the checker would
//! otherwise have to answer while it is also doing arithmetic:
//!
//! 1. **What does this name refer to?** A local, a top-level declaration, or a
//!    built-in — recorded per occurrence in `Resolved::uses`, so no later pass
//!    ever searches for a name again.
//! 2. **Is this name defined at all?** An unknown name is a compile error
//!    here, before any type exists, which is what keeps a typo from being
//!    reported as a type mismatch.
//! 3. **Is anything declared twice, or never read?** §4.4 makes both an error;
//!    §4.16 suspends the second one file-wide while a `???` is still in the
//!    file.
//! 4. **Which written type is which?** `i64` the primitive, `Point` the
//!    record, `A` the generic parameter — `Resolved::type_uses`, indexed by the
//!    arena the parser already built.
//!
//! | file | idea |
//! |------|------|
//! | `builtins.rs` | the names no file declares (§4.20's inventory) |
//! | `top.rs`      | the order-free top level, collected before any body |
//! | `scope.rs`    | the scope stack, shadowing, and the read/write counts |
//! | `decls.rs`    | one declaration at a time: generics, signature, body |
//! | `stmts.rs`    | statements: what binds, what reads, what writes |
//! | `exprs.rs`    | expressions, patterns, and the root of a mutated place |
//! | `qualified.rs`| `geom.f` — a module in receiver position, and what it is not |
//! | `types.rs`    | written types against primitives, declarations, generics |
//! | `errors.rs`   | the messages, and the one-candidate `Certain` rename |
//! | `cycles.rs`   | a `constant` defined in terms of itself, directly or not |
//! | `resolved.rs` | what the pass leaves behind — the table every later pass reads |
//!
//! **The resolver only ever runs on a tree that parsed clean.** After a parse
//! error the tree holds recovery guesses, and a name error about a line the
//! author did not write is worse than no name error at all — the same rule
//! `--dump-ast` follows. `resolve` never panics on a dirty tree; it is the
//! caller that declines to ask.
//!
//! **Later passes consume `uses`; they never resolve a name again.** This is
//! not an optimisation, it is a correctness rule, and the compiler-engineer
//! measured why while costing panel 015: in one function `len(xs)` can be a
//! local and `xs.len()` the built-in, legitimately. Part 5 erases UFCS in the
//! frontend, so a pass that re-resolved `f(x, y)` after erasure would silently
//! give the erased call the *other* meaning. One resolution, recorded once.

use crate::diagnostics::Diagnostic;
use crate::source::Source;
use crate::syntax::{Ast, ExprId, ExprKind};

mod builtins;
mod constbody;
mod cycles;
mod decls;
mod errors;
mod exprs;
mod places;
mod qualified;
mod resolved;
mod scope;
mod stmts;
mod top;
mod types;

#[cfg(test)]
mod tests;

pub use builtins::{index_of, Builtin, Tier, BUILTINS};
pub use resolved::{Local, LocalKind, Ref, Resolved, TypeRef};
pub use types::Prim;

use scope::Scopes;

/// Resolve one parsed file.
pub fn resolve(ast: &Ast, src: &Source) -> Resolved {
    let mut r = Resolver {
        out: Resolved {
            uses: vec![Ref::Unresolved; ast.exprs.len()],
            type_uses: vec![TypeRef::Unresolved; ast.types.len()],
            locals: Vec::new(),
            top: std::collections::BTreeMap::new(),
            module_uses: std::collections::BTreeMap::new(),
            // A flat scan of the arena, not a walk: a hole suspends the unused
            // rule wherever it is, including inside a construct the walk gives
            // up on. The file table is what turns each one into a module.
            holes_in: ast
                .exprs
                .iter()
                .filter(|e| matches!(e.kind, ExprKind::Hole))
                .map(|e| src.file(e.span.start).module.clone())
                .collect(),
            diagnostics: Vec::new(),
        },
        scopes: Scopes::new(),
        generics: Vec::new(),
        fields: std::collections::BTreeSet::new(),
        suggested: std::collections::BTreeSet::new(),
        owner: 0,
        module: String::new(),
        module_reads: std::collections::BTreeSet::new(),
    };
    top::collect(&mut r, ast, src);
    for index in 0..ast.decls.len() {
        r.owner = index as u32;
        r.module = src.module_at(ast.decls[index].name.start).to_string();
        decls::declaration(&mut r, ast, src, index);
    }
    top::unused_uses(&mut r, ast, src);
    r.report_unused(src);
    // Last, because it reads `uses` and `uses` is only complete once every body
    // has been walked. A definitional cycle is a fact about the finished table,
    // not about any one declaration.
    cycles::report(&mut r.out, ast, src);
    // Within the pass, source order. Diagnostics from earlier stages stay ahead
    // of these (see `syntax::parse`): grouped by the stage that can explain
    // them, ordered by position inside it.
    r.out.diagnostics.sort_by_key(|d| d.span.start);
    r.out
}

/// The pass's working state. It owns everything and takes `&Ast`/`&Source` as
/// parameters, which is the Cyclone rule (CLAUDE.md §5) rather than a style
/// choice: nothing here may store a reference.
struct Resolver {
    out: Resolved,
    scopes: Scopes,
    /// Type parameters of the function being resolved, `(name, position)`.
    /// Cleared at every declaration: generics are on functions only (§4.12).
    generics: Vec<(String, u32)>,
    /// Every field name declared anywhere in the file — record fields and
    /// variant payload fields alike. It exists for one narrow purpose: to keep
    /// the unknown-function error off `h.cb(n)` where `cb` is a field holding
    /// a function value (§4.13), which §4.11's algorithm resolves by looking at
    /// the receiver's *type* and M-name-resolution has none. See `exprs::method`.
    fields: std::collections::BTreeSet<(String, String)>,
    /// Names the compiler has offered as the repair for an unknown name. They
    /// are exempt from the unused sweep: applying the fix would read them, so
    /// reporting both would be two diagnostics for one typo — and this project
    /// counts that as a defect (the lexer and parser hold the same invariant).
    suggested: std::collections::BTreeSet<(String, String)>,
    owner: u32,
    /// The module whose declaration is being resolved. Every unqualified name
    /// is looked up in it and in nothing else, which is where "always
    /// qualified" is actually enforced (M-module-namespace, panel 031).
    module: String,
    /// `use` lines that were read, keyed as `module_uses` is. What is left over
    /// is what the unused rule reports — spec line 74 covers it for free,
    /// because `use` *binds*.
    module_reads: std::collections::BTreeSet<(String, String)>,
}

impl Resolver {
    /// A top-level name an *unqualified* mention can reach: this module's own
    /// declarations, then the library's.
    ///
    /// The library is the one module every file sees without naming it, and
    /// that is not an exception to "always qualified" — its names are reserved
    /// built-ins (§1.11 Tier 2), spent by the language rather than imported by
    /// the file, which is why no `use` can name it and no program may redeclare
    /// one. Panel 031 recorded it as one spec sentence and no
    /// mechanism; this is the little mechanism that sentence stands for.
    fn top_visible(&self, name: &str) -> Option<u32> {
        self.out
            .top_in(&self.module, name)
            .or_else(|| self.out.top_in(crate::source::LIBRARY_MODULE, name))
    }

    /// The modules the file being resolved names, for a did-you-mean over it.
    fn used_modules(&self) -> Vec<String> {
        self.out
            .module_uses
            .keys()
            .filter(|(from, _)| *from == self.module)
            .map(|(_, named)| named.clone())
            .collect()
    }

    fn push_diagnostic(&mut self, diagnostic: Diagnostic) {
        self.out.diagnostics.push(diagnostic);
    }

    /// Is `text` a field name this file could legitimately be reaching?
    ///
    /// Its own module's fields, plus those of every module it `use`s — because
    /// this file may hold a `geom.Point` and write `p.x`, and may not hold
    /// anything from a module it never named. The set used to be keyed by bare
    /// name over the whole program, so a field declared in `geom.hero`
    /// **removed a `certain` fix** from a diagnostic in `main.hero`: another
    /// module decided whether a repair was machine-applicable (CLAUDE.md §8,
    /// 2026-08-12).
    fn field_in_reach(&self, text: &str) -> bool {
        self.fields.iter().any(|(module, name)| {
            name == text
                && (module == &self.module
                    || module == crate::source::LIBRARY_MODULE
                    || self.out.is_used_module(&self.module, module))
        })
    }

    /// The candidates for a name that resolved to nothing, remembered so the
    /// unused sweep can stay quiet about them.
    fn near_names(&mut self, name: &str, candidates: &[String]) -> Vec<String> {
        let near = errors::nearest(name, candidates);
        for candidate in &near {
            // Keyed by the module that was offered the repair. A did-you-mean in
            // `geom.hero` used to exempt that bare name from the unused sweep in
            // `main.hero`, which dropped a spec-line-77 error outright
            // (2026-08-12).
            self.suggested.insert((self.module.clone(), candidate.clone()));
        }
        near
    }

    /// Records what an occurrence resolved to, and counts the read.
    fn record(&mut self, at: ExprId, to: Ref) {
        self.out.uses[at.0 as usize] = to;
        if let Ref::Local(index) = to {
            self.out.locals[index as usize].reads += 1;
        }
    }
}
