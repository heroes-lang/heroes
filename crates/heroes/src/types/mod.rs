//! The checker: every expression gets a type, or a diagnostic says why not
//! (design.md Part 10 step 5, §4.5 bidirectional checking; ROADMAP M3b–M3c).
//!
//! **Bidirectional, and the two modes are the whole design** (§4.5). Signatures
//! are always explicit and inference is local, so the checker never solves
//! constraints: it either *synthesises* a type from an expression (⇒) or
//! *checks* an expression against a type it already knows (⇐). Errors stay
//! local because nothing travels: the wrong type is reported where it was
//! written, not thirty lines later where a global inference would have noticed.
//!
//! Exactly four forms need ⇐ and cannot synthesise, and each is in the language
//! for a reason:
//!
//! | form | why it cannot synthesise |
//! |------|--------------------------|
//! | `[]`, `{}` | an empty literal does not say what it holds (§4.5) |
//! | `ok(x)`, `fail(c, m)` | which `T?` is the caller's question (panel 002) |
//! | `.case` | which variant it belongs to comes from context (§4.5) |
//! | `???` | it has no type; it *reports* the one expected (§4.16) |
//!
//! | file | idea |
//! |------|------|
//! | `table.rs`  | interned types: `TyId` equality **is** type equality |
//! | `lower.rs`  | a written type (`[Point?]`) becomes a `TyId` |
//! | `render.rs` | a `TyId` becomes surface syntax, for messages |
//! | `ops.rs`    | §4.14's operators, monomorphic, no promotions |
//! | `calls.rs`  | calls, built-ins, and record construction |
//! | `builtins.rs` | the built-ins' type rules (§4.20's inventory) |
//! | `join.rs`   | one rule for every branch join, diverging arms skipped |
//! | `patterns.rs` | patterns, `_`'s ban, and exhaustiveness |
//! | `holes.rs`    | §4.16's output: what the compiler knew and threw away |
//! | `exprs.rs`  | ⇒ synthesise a type from an expression |
//! | `expect.rs` | ⇐ check an expression against one, and the four forms that need it |
//! | `decls.rs`  | one declaration at a time: what its body is for |
//! | `stmts.rs`  | statements, and where a value is not allowed |
//! | `errors.rs` | the mismatch messages and their fixes |
//!
//! **A jump has no type** (panel 017 A1): `return`, `break` and `continue`
//! produce no value, so synthesis returns `None` for them and a `match` takes
//! its type from the arms that *can* produce one. There is deliberately no
//! bottom type in the table — the compiler-engineer's panel-014 prediction says
//! adding one would cost lines in desugaring and lowering that A1 does not, and
//! `Option<TyId>` keeps the fact local to this pass, where it is control flow
//! rather than a type anybody can write.

use crate::diagnostics::Diagnostic;
use crate::resolve::Resolved;
use crate::source::{Source, Span};
use crate::syntax::{Ast, ExprId};

mod access;
mod arms;
mod builtins;
mod calls;
mod construct;
mod decls;
mod errors;
mod expect;
mod exprs;
mod generics;
mod holes;
mod join;
mod lower;
mod ops;
mod patterns;
mod render;
mod stmts;
mod table;

#[cfg(test)]
mod tests;

pub use holes::report as report_holes;
pub use render::render_ty;
pub use table::{Params, Ty, TyId, Types};

pub struct Checked {
    pub types: Types,
    /// The type of every local, indexed as `Resolved::locals` is — kept because
    /// §4.16's hole output has to say what is in scope, and M4's lowering will
    /// ask the same question.
    pub local_types: Vec<TyId>,
    /// Each function declaration's result type, by index into `Ast::decls`.
    pub results: std::collections::BTreeMap<u32, TyId>,
    /// Every *written* type, by its node in `Ast::types`. The one table that answers
    /// "what did the author write here" without a local to hang it on — which is
    /// what an `extern`'s parameters need (§4.19: no body, so no locals).
    pub written_types: std::collections::BTreeMap<u32, TyId>,
    /// The type of every expression, dense over `Ast::exprs`. A diverging or
    /// unreported-error expression carries `Types::error()`.
    pub expr_types: Vec<TyId>,
    /// What the checker expected where a `???` stands, in source order. §4.16's
    /// output is built from this at M3d, and the cap of 5 is applied there.
    pub holes: Vec<Hole>,
    pub diagnostics: Vec<Diagnostic>,
}

impl Checked {
    /// The type of a local, by its index in `Resolved::locals`. `holes.rs` reads
    /// it to print what is in scope.
    pub fn local_type(&self, index: usize) -> TyId {
        self.local_types.get(index).copied().unwrap_or(TyId(0))
    }

    /// The result type of a top-level function, or `None` if that declaration is
    /// not one. `holes.rs` ranks suggestions by it.
    pub fn result_type(&self, decl: u32) -> Option<TyId> {
        self.results.get(&decl).copied()
    }

    /// The type a written type node resolved to.
    pub fn written_type(&self, id: crate::syntax::TypeId) -> Option<TyId> {
        self.written_types.get(&id.0).copied()
    }
}

/// One typed hole: where it is, and what belongs there.
pub struct Hole {
    pub at: ExprId,
    pub span: Span,
    /// The type the context expects, or `Types::error()` where the hole stands
    /// in statement position and nothing is expected of it.
    pub expected: TyId,
}

/// Check one resolved file.
///
/// The resolver ran first and its answers are inputs, never recomputed: every
/// name already knows what it refers to (`Resolved::uses`), and every written
/// type already knows which declaration or primitive it names
/// (`Resolved::type_uses`).
pub fn check(ast: &Ast, resolved: &Resolved, src: &Source) -> Checked {
    let types = Types::new();
    let local_types = vec![types.error(); resolved.locals.len()];
    let mut checker = Checker {
        out: Checked {
            types,
            local_types,
            results: std::collections::BTreeMap::new(),
            written_types: std::collections::BTreeMap::new(),
            expr_types: Vec::new(),
            holes: Vec::new(),
            diagnostics: Vec::new(),
        },
        result: TyId(0),
        fallible: false,
        loops: 0,
        locals_by_span: resolved
            .locals
            .iter()
            .enumerate()
            .map(|(index, local)| (local.name.start, index as u32))
            .collect(),
        generic_names: Vec::new(),
        missing_returns: Vec::new(),
    };
    checker.out.expr_types = vec![checker.out.types.error(); ast.exprs.len()];
    decls::file(&mut checker, ast, resolved, src);
    // §4.16's **file-wide hole exemption**, the same one the unused rule takes: a
    // body that is `???` falls off its end by construction — that is what an
    // unwritten thing does — and demanding a `return` from it would make `???`
    // unusable for the case design.md's own appendix uses it for
    // (`function simplify(e: Expr) -> Expr` / `???`). Found by this check firing on
    // the appendix within an hour of being written.
    if checker.out.holes.is_empty() {
        let pending = std::mem::take(&mut checker.missing_returns);
        checker.out.diagnostics.extend(pending);
    }
    checker.out.diagnostics.sort_by_key(|d| d.span.start);
    checker.out
}

/// The pass's working state. Owns everything; `&Ast`, `&Resolved` and `&Source`
/// are parameters (the Cyclone rule, CLAUDE.md §5).
struct Checker {
    out: Checked,
    /// The result type of the function being checked — what `return` is checked
    /// against.
    result: TyId,
    /// Whether that function's result is a `T?`, which is what makes `?` legal
    /// (§4.6: `?` in a function that cannot fail is a compile error).
    fallible: bool,
    /// Loop nesting, so `break` outside a loop is caught here rather than in C.
    loops: u32,
    /// Where each local's binding name starts → its index in
    /// `Resolved::locals`. Built once, because the checker must not depend on
    /// walking the tree in the same order the resolver did.
    locals_by_span: std::collections::BTreeMap<u32, u32>,
    /// The names of the enclosing function's type parameters (§4.12), in
    /// order — a `Ty::Generic(i)` is only meaningful inside it, and messages
    /// must print the letter the author wrote.
    generic_names: Vec<String>,
    /// A `missing_return` per function that can run off its end, held back until the
    /// whole file is checked so that §4.16's file-wide hole exemption can apply.
    missing_returns: Vec<Diagnostic>,
}

impl Checker {
    fn error_ty(&mut self) -> TyId {
        self.out.types.error()
    }

    fn push_diagnostic(&mut self, diagnostic: Diagnostic) {
        self.out.diagnostics.push(diagnostic);
    }

    /// A type as the author would have written it — every message goes through
    /// here so that no diagnostic ever prints a notation the language lacks.
    fn show(&self, ast: &Ast, src: &Source, id: TyId) -> String {
        render::render_ty(&self.out.types, ast, src, id, &self.generic_names)
    }

    /// Gives a local the type its declaration says it has. Keyed by the span of
    /// the name, so the checker and the resolver agree by *position in the
    /// source* rather than by both happening to walk in the same order.
    fn bind_local(&mut self, name: Span, ty: TyId) {
        if let Some(index) = self.locals_by_span.get(&name.start) {
            self.out.local_types[*index as usize] = ty;
        }
    }

    /// Records an expression's type and hands it back, so the caller can write
    /// `self.record(id, ty)` as the tail of a synthesis rule.
    fn record(&mut self, at: ExprId, ty: TyId) -> TyId {
        self.out.expr_types[at.0 as usize] = ty;
        ty
    }
}
