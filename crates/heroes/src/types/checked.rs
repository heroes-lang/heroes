//! What the checker leaves behind (design.md §4.3, §4.16; panels 023, 029).
//!
//! Split out of `mod.rs` by the §11 sweep, and it is the same seam
//! `resolve/resolved.rs` was cut on one hour earlier — which is worth noticing:
//! the two passes have the same shape, and in both the file held **how the answer
//! is computed** and **what the answer is**, with only the second read by anyone
//! else.
//!
//! Everything here is `pub`; nothing here walks a tree.
//!
//! **`counted` is dense over the interner and is built last**, on the stated ground
//! that nothing may intern a type after it. `extend_counted` is the one sanctioned
//! way to break that rule and its only caller is monomorphisation — a public entry
//! point rather than a field assignment, so the invariant has a name to be violated
//! through.

use crate::diagnostics::Diagnostic;
use crate::source::Span;
use crate::syntax::{Ast, ExprId};

use super::counted;
use super::table::{Types, TyId};

pub struct Checked {
    pub types: Types,
    /// The type of every local, indexed as `Resolved::locals` is — kept because
    /// §4.16's hole output has to say what is in scope, and M-ir-lowering's lowering will
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
    /// output is built from this at M-rich-diagnostics, and the cap of 5 is applied there.
    pub holes: Vec<Hole>,
    /// `record` and `variant` declarations in an order where every by-value
    /// dependency precedes its user — C needs every struct complete before it is
    /// used by value, and Heroes' top level is order-free (§3.1, panel 023).
    ///
    /// Published here rather than recomputed in the backend because the order and
    /// the `no_size` cycles are **one walk** (`sized.rs`), and two answers to one
    /// question is the failure `ir/layout.rs` documents. The emitter *filters*
    /// this to the descriptor worklist's reachable subset: a total order
    /// restricted to a subset is still a total order.
    pub type_order: Vec<u32>,
    /// What each call to a **generic** function instantiated it at, by the span of
    /// the call (§4.12). Empty for every monomorphic call, so its size is the
    /// number of generic calls and not the number of calls.
    ///
    /// **Recorded here rather than recomputed in the pass** (panel 029 R2). The
    /// checker already binds these to type the call and used to drop them on the
    /// floor; monomorphisation would otherwise have to redo `generics::bind` at IR
    /// level, and `counted.rs` already ruled on that shape by name — "two answers
    /// that disagree … is a refcount bug that reproduces once a week".
    ///
    /// Keyed by span because a span is what an IR instruction carries back to the
    /// source, and two calls cannot share one: a span is a byte range, and the
    /// ranges of two distinct calls differ.
    pub instantiations: std::collections::BTreeMap<u32, Vec<TyId>>,
    /// One entry per interned type: does a value of it own a reference the compiler
    /// must count (§4.10, §4.20). Built by `counted.rs` after checking, because the
    /// question is transitive through a record's fields and a `TyId` carries a
    /// declaration index rather than its contents.
    ///
    /// `ir::is_refcounted` is the one place this is *asked*; that is why it reads
    /// this table instead of holding a second opinion.
    pub counted: Vec<bool>,
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

/// Rebuild `counted` after a pass has interned new types.
///
/// The only sanctioned way to break the "nothing may intern after it" rule above,
/// and the only caller is monomorphisation. It is a public entry point rather
/// than a field assignment so the invariant has a name to be violated through.
pub fn extend_counted(checked: &mut Checked, ast: &Ast) {
    checked.counted = counted::extend(&checked.types, ast, &checked.written_types);
}
