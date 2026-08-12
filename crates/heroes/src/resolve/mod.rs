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
//! 4. **Which written type is which?** `int` the primitive, `Point` the
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
use crate::source::{Source, Span};
use crate::syntax::{Ast, ExprId, ExprKind, TypeId};

mod builtins;
mod cycles;
mod decls;
mod errors;
mod exprs;
mod qualified;
mod scope;
mod stmts;
mod top;
mod types;

#[cfg(test)]
mod tests;

pub use builtins::{index_of, Builtin, Tier, BUILTINS};
pub use types::Prim;

use scope::Scopes;

/// What a name refers to. `Unresolved` covers both "this node is not a name"
/// and "this name has no answer" — the array is dense, one entry per
/// expression, because §4.10 makes the array Heroes' only indirection and the
/// port reads `uses: [Ref]` unchanged.
///
/// Two kinds of node are keyed here, and they mean subtly different things:
/// for a `Name` the entry is *that expression's* binding; for a `Method`
/// (`x.f(y)`) it is the binding of the **name after the dot**, since the
/// parser gives that name no `ExprId` of its own.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Ref {
    Unresolved,
    /// Index into `Resolved::locals`.
    Local(u32),
    /// Index into `Ast::decls`.
    Top(u32),
    /// Index into `BUILTINS`.
    Builtin(u32),
    /// A module name in receiver position: `geom` in `geom.dist2(a, b)`.
    ///
    /// Recorded on the **receiver** expression, and it is what tells every later
    /// pass that this dot is qualification and not UFCS. Those are the same
    /// three tokens with two meanings, so the answer has to be recorded once —
    /// this file's standing rule (see the module doc): a pass that decided it
    /// again after Part 5 erased UFCS would give the erased call the other one.
    Module,
}

/// What a written type name refers to. Dense over `Ast::types`, same reasons.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum TypeRef {
    Unresolved,
    Prim(Prim),
    /// A `record` or `variant` declaration: index into `Ast::decls`.
    Top(u32),
    /// A type parameter of the enclosing function: index into its `generics`.
    Generic(u32),
}

/// Where a local came from. The kind is not decoration: it decides the word the
/// unused-binding error uses, and whether the name may be written.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum LocalKind {
    /// A function parameter. `@p` is `mutable`.
    Param,
    /// `x = 5` — binds once, forever.
    Bind,
    /// `v: int @ 0` — a mutable cell.
    Cell,
    /// `for x in xs` — bound afresh per element, never mutable.
    Loop,
    /// `.num n` — a variant payload bound by a pattern.
    Payload,
}

/// One binding site.
///
/// `reads` and `writes` are counted separately on purpose: a cell that is
/// written and never read is a value nobody consumes (panel 015 question B).
/// The declaration's own initialiser is **not** a write — counting it would
/// make the unused rule blind to every cell in the language.
///
/// `ty` and `value` are the syntax's own answer to "what is this local", kept
/// because M-checker-core would otherwise re-walk the tree to find it: a parameter and a
/// cell always carry a written type (§4.4), a plain `x = e` carries only its
/// value (§4.5 — inference is local), and a loop variable carries the iterable
/// it draws elements from.
pub struct Local {
    pub name: Span,
    pub kind: LocalKind,
    pub mutable: bool,
    /// Index into `Ast::decls` — which declaration this local lives in.
    pub owner: u32,
    /// Scope nesting inside that declaration, 0 for parameters. Only
    /// `--dump-scopes` reads it; it is what makes the nesting visible.
    pub depth: u32,
    pub ty: Option<TypeId>,
    pub value: Option<ExprId>,
    pub reads: u32,
    pub writes: u32,
}

pub struct Resolved {
    /// One entry per expression in `Ast::exprs`.
    pub uses: Vec<Ref>,
    /// One entry per node in `Ast::types`.
    pub type_uses: Vec<TypeRef>,
    pub locals: Vec<Local>,
    /// Every top-level name, keyed by **(module, name)** and sorted —
    /// declaration order carries no meaning (§4.2), so the table that holds them
    /// has no order either. The value is an index into `Ast::decls`.
    ///
    /// The module is in the key because two modules may each declare `Point`
    /// and they are two types (M-module-namespace). It is also what makes an unqualified name
    /// see only its own file: `top_in` is asked for one module, never for all
    /// of them, and that single fact is most of what "always qualified" costs
    /// the resolver.
    pub top: std::collections::BTreeMap<(String, String), u32>,
    /// Every `use` line, keyed by (the module that wrote it, the module it
    /// names). The value indexes `Ast::uses`, so a diagnostic can point at the
    /// line itself.
    pub module_uses: std::collections::BTreeMap<(String, String), u32>,
    /// Every module that contains a `???`. While one does, unused bindings and
    /// unused parameters are not reported **in that module** (§4.16, normative:
    /// *"The suppression is file-wide"* — the section's own example binds a name
    /// that is read only inside the hole).
    ///
    /// A set rather than a flag, and the difference is a fixed defect: with one
    /// `Ast` covering every module since M-module-namespace, a single `bool` made the exemption
    /// program-wide, so an unfinished `geom.hero` silently suspended the rule in
    /// a `main.hero` nobody was editing (panel 033 D1).
    pub holes_in: std::collections::BTreeSet<String>,
    pub diagnostics: Vec<Diagnostic>,
}

impl Resolved {
    /// A top-level name, looked up in one module and nowhere else.
    pub fn top_in(&self, module: &str, name: &str) -> Option<u32> {
        self.top.get(&(module.to_string(), name.to_string())).copied()
    }

    /// Does `module` name a module that `from` said `use` about?
    pub fn is_used_module(&self, from: &str, module: &str) -> bool {
        self.module_uses.contains_key(&(from.to_string(), module.to_string()))
    }

    /// Every name one module declares, for the "did you mean" lists and for
    /// `--dump-scopes`.
    pub fn names_in<'a>(&'a self, module: &'a str) -> impl Iterator<Item = (&'a str, u32)> {
        self.top
            .iter()
            .filter(move |((m, _), _)| m == module)
            .map(|((_, n), d)| (n.as_str(), *d))
    }

    /// Does the module that owns `offset` contain a hole? The question every
    /// §4.16 exemption asks, and it takes an offset rather than a module name
    /// because every caller is holding a span it is about to report.
    pub fn hole_covers(&self, src: &Source, offset: u32) -> bool {
        self.holes_in.contains(&src.file(offset).module)
    }

    /// Which module declares `name`, if any does — **preferring one that `from`
    /// can actually see**. Used by the diagnostics that turn an unknown name into
    /// "it is in `geom`, write `geom.f`".
    ///
    /// The preference is the whole function. `top` is a `BTreeMap` keyed by
    /// `(module, name)`, so a bare `find` returns the **alphabetically first**
    /// module of however many declare that name — which had exactly one possible
    /// answer while a program was one file, and since M-module-namespace picks by sort order.
    /// Measured (2026-08-12): with `use geom` written and both `alpha` and `geom`
    /// declaring `scale`, the compiler named `alpha` — a module the file cannot
    /// see — attached a `guess` fix that gives `wrong_arity` if followed, and
    /// **cascaded a false `unused_binding` telling the author to delete the
    /// `use geom` line that was the real fix**. Renaming `alpha.hero` to
    /// `zeta.hero` produced the right answer with a `certain` fix.
    ///
    /// Ties within each group are still broken by sort order, which is
    /// deterministic and is what the double-emit and golden tests need.
    pub fn module_declaring(&self, from: &str, name: &str) -> Option<&str> {
        let declaring = || self.top.iter().filter(move |((_, n), _)| n == name);
        declaring()
            .map(|((m, _), _)| m.as_str())
            .find(|module| *module == from || self.is_used_module(from, module))
            .or_else(|| declaring().map(|((m, _), _)| m.as_str()).next())
    }

    pub fn use_at(&self, id: ExprId) -> Ref {
        self.uses[id.0 as usize]
    }

    pub fn type_at(&self, id: TypeId) -> TypeRef {
        self.type_uses[id.0 as usize]
    }
}

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
