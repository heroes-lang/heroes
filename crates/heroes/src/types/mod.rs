//! The checker: every expression gets a type, or a diagnostic says why not
//! (design.md Part 10 step 5, §4.5 bidirectional checking; ROADMAP M-checker-core–M-data-declarations).
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
mod apply;
mod arms;
mod builtins;
pub(crate) mod calls;
mod checked;
mod construct;
mod conversions;
mod contextual;
mod counted;
mod decls;
mod errors;
mod expect;
mod floats;
mod fallible_ops;
mod ffi_decls;
mod exprs;
mod generics;
mod holes;
mod join;
mod jumps;
mod literals;
mod lower;
mod ops;
mod partial;
mod patterns;
mod render;
mod sized;
mod sized_cycle;
mod stmts;
mod table;
mod ufcs;
mod widths;

#[cfg(test)]
mod tests;

pub use holes::report as report_holes;
pub use render::render_ty;
pub use table::{Params, Ty, TyId, Types};
pub use floats::{FloatKind, FLOAT_KINDS};
pub use widths::{IntKind, INT_KINDS};

pub use checked::{extend_counted, Checked, Hole};


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
            type_order: Vec::new(),
            instantiations: std::collections::BTreeMap::new(),
            counted: Vec::new(),
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
    // Sizes first, and it needs no expression types: a containment cycle makes
    // every *later* answer about those types a guess, and a file with one produces
    // no binary anyway. Reporting it here also keeps the final sort's job trivial.
    let (order, no_size) = sized::order_and_cycles(ast, resolved, src);
    checker.out.type_order = order;
    checker.out.diagnostics.extend(no_size);
    decls::file(&mut checker, ast, resolved, src);
    // After the declarations, because it reads `written_type` — the table
    // `lower::ty` fills. Asking first is silent: it finds nothing and passes.
    partial::map_keys(&mut checker, ast, src);
    // Same reason, same table, and the same defect one milestone apart: panel 062
    // wired its rule to one call site and three other positions shipped.
    ffi_decls::fixed_only_in_a_group(&mut checker, ast, src);
    // §4.16's **file-wide hole exemption**, the same one the unused rule takes: a
    // body that is `???` falls off its end by construction — that is what an
    // unwritten thing does — and demanding a `return` from it would make `???`
    // unusable for the case design.md's own appendix uses it for
    // (`function simplify(e: Expr) -> Expr` / `???`). Found by this check firing on
    // the appendix within an hour of being written.
    //
    // **Per module, not per program** (2026-08-12). This test was
    // `holes.is_empty()`, and since M-module-namespace one `Checked` spans every module — so an
    // unfinished `geom.hero` held back `missing_return` in a `main.hero` nobody
    // was editing. It is D1's defect in the pass D1's fix did not reach, and the
    // sentence above already said "file-wide" while the code asked about the
    // program.
    let held: std::collections::BTreeSet<&str> = checker
        .out
        .holes
        .iter()
        .map(|hole| src.file(hole.span.start).module.as_str())
        .collect();
    let pending = std::mem::take(&mut checker.missing_returns);
    for diagnostic in pending {
        if held.contains(src.file(diagnostic.span.start).module.as_str()) {
            continue;
        }
        checker.out.diagnostics.push(diagnostic);
    }
    checker.out.diagnostics.sort_by_key(|d| d.span.start);
    // Last, because it is dense over the interner and nothing may intern a type
    // after it *without saying so*: a `TyId` past the end of this table would read
    // as uncounted, which is a leak that no test can see. Monomorphisation does
    // intern — that is its job — and calls `extend_counted` for exactly this
    // reason (panel 029 R3).
    checker.out.counted =
        counted::table(&checker.out.types, ast, &checker.out.written_types);
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
