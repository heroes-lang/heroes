//! What the resolver leaves behind (design.md §4.4, §4.16; panels 015, 031, 033).
//!
//! Split out of `mod.rs` by the §11 sweep, and the seam is the pass's own standing
//! rule: **later passes consume this table and never resolve a name again.** So
//! there are two concerns in one file — how the answer is computed, and what the
//! answer *is* — and only the second is read by anybody else. Everything here is
//! `pub`; nothing here walks a tree.
//!
//! Both dense tables (`uses`, `type_uses`) are indexed by the arena the parser
//! already built, because §4.10 makes the array Heroes' only indirection and the
//! port reads `uses: [Ref]` unchanged.

use crate::diagnostics::Diagnostic;
use crate::source::{Source, Span};
use crate::syntax::{ExprId, TypeId};

use super::types::Prim;

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
    /// `v: i64 @ 0` — a mutable cell.
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

