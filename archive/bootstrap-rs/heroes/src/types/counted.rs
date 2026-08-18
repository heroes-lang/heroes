//! Which types carry a reference the compiler must count (design.md §4.10, §4.20;
//! panels 021, 022).
//!
//! `str`, `[T]` and `{K: V}` are the only reference-counted things in the language
//! (§4.20). A record or a variant is **by value** and owns no block of its own — but
//! it can *contain* one, and then copying it is an incref of every counted field
//! and dropping it a decref. So the question is transitive, and answering it needs
//! the declarations: a `TyId` for `Ty::Named(d)` carries the declaration index and
//! nothing about its fields.
//!
//! That is the whole reason this table is built here rather than asked for in the
//! backend. `ir/layout.rs::is_refcounted` stays the one place the question is
//! *asked* — it becomes a lookup — and this is the one place it is *answered*,
//! where the AST is in hand. Two answers that disagree about which types are
//! counted is a refcount bug that reproduces once a week.
//!
//! **The walk terminates because `sized.rs` ran first.** A cycle through by-value
//! fields is `no_size` and the file has already failed; a cycle through `[T]` or
//! `{K: V}` stops at the container, which is counted whatever it holds. Step 1 of
//! this milestone is what makes step 2's recursion safe, and without it
//! `record Node { child: Node }` would hang here instead of in clang.

use std::collections::BTreeMap;

use crate::syntax::{Ast, DeclKind};

use super::table::{Ty, TyId, Types};

/// What the checker recorded for every *written* type node, by its index in
/// `Ast::types`. A field's declared type is looked up here rather than lowered
/// again: lowering a type twice is two answers to one question.
type Written = BTreeMap<u32, TyId>;

/// One entry per interned type: does a value of it own a counted reference.
///
/// Dense over `TyId` because `Types` is an interner and the table is built after
/// checking, when no more types can appear.
pub(super) fn table(types: &Types, ast: &Ast, written: &Written) -> Vec<bool> {
    let count = types.len();
    let mut answer = vec![false; count];
    // Fixpoint rather than recursion, for the same reason `sized.rs` walks
    // iteratively: this is a graph, the graph is the author's, and a `record`
    // reachable only through `[T]` can be interned before the type that holds it.
    // Two passes would be enough today; a loop to stability cannot be wrong.
    let mut changed = true;
    while changed {
        changed = false;
        for index in 0..count {
            if answer[index] {
                continue;
            }
            if counts(types, ast, written, &answer, TyId(index as u32)) {
                answer[index] = true;
                changed = true;
            }
        }
    }
    answer
}

/// Every arm listed, never a catch-all: a type added to the language must not
/// become silently uncounted, which is the same rule `sized.rs` and the old
/// `is_refcounted` were both written under.
fn counts(types: &Types, ast: &Ast, written: &Written, answer: &[bool], ty: TyId) -> bool {
    match types.get(ty) {
        // Owns a heap block (§4.20).
        Ty::Str => true,
        // By value, and counted exactly when something inside them is.
        Ty::Named(decl) => {
            fields_of(ast, written, decl).iter().any(|f| answer[f.0 as usize])
        }
        Ty::Case(decl, case) => {
            case_fields_of(ast, written, decl, case).iter().any(|f| answer[f.0 as usize])
        }
        // Staged one step at a time while they landed, and the staging is done: every
        // row here now emits. The discipline is worth keeping in mind — a row and the
        // emission for it land together, because turning one on early writes increfs
        // into the IR of programs the gate refuses, and that is a golden diff nobody
        // can validate by running the program.
        //
        Ty::Array(_) | Ty::Map(_, _) => true,
        // **Every** `T?` is counted whatever `T` is, and a `Failure` always: it is two
        // `str`s, so even `int?` owns a reference on its error side. The union means the
        // release has to switch on the tag rather than release both.
        Ty::Fallible(_) | Ty::Failure => true,
        // Scalars, the FFI's opaque types, a function pointer, and the two the
        // checker uses for its own bookkeeping.
        Ty::Int(_) | Ty::Float(_) | Ty::Bool | Ty::Unit | Ty::Ptr | Ty::Cstr => false,
        // **A fixed array owns exactly what its element owns.** It is inline bytes,
        // so it adds no reference of its own — but if the element carried one, N
        // of them would need retaining. The checker refuses a counted element as a
        // group record's field, so this is `false` in practice today; it is written
        // as the honest rule rather than as `false`, because the day a fixed array
        // is permitted outside a group the wrong answer here is a leak.
        Ty::Fixed(inner, _) => counts(types, ast, written, answer, inner),
        Ty::Func { .. } | Ty::Generic(_) | Ty::Error => false,
    }
}

/// Every field a value of this declaration can hold, as interned ids.
///
/// **Both kinds**, because `Ty::Named(d)` is the type of the *whole* declaration and
/// a variant is one of the things it can name — `Ty::Case(d, c)` is the narrower
/// question about one case. Reading only `DeclKind::Record` here made every variant
/// uncounted, so `variant Token { word { text: str } }` copied with `=` and never
/// increfed; the test that names one case counted is what found it.
///
/// A variant's cases are alternatives, so **any** case carrying a reference makes the
/// variant counted: the generated drop switches on the tag and releases whichever
/// case is live.
fn fields_of<'a>(ast: &'a Ast, written: &'a Written, decl: u32) -> Vec<TyId> {
    let types = |fields: &[crate::syntax::Field]| -> Vec<TyId> {
        fields.iter().filter_map(|field| written.get(&field.ty.0).copied()).collect()
    };
    match &ast.decls[decl as usize].kind {
        DeclKind::Record { fields, .. } => types(fields),
        DeclKind::Variant { cases } => cases.iter().flat_map(|case| types(&case.fields)).collect(),
        _ => Vec::new(),
    }
}

/// One case's payload fields — the narrower question `Ty::Case(d, c)` asks.
fn case_fields_of(ast: &Ast, written: &Written, decl: u32, case: u32) -> Vec<TyId> {
    let fields = match &ast.decls[decl as usize].kind {
        DeclKind::Variant { cases } => match cases.get(case as usize) {
            Some(one) => one.fields.as_slice(),
            None => &[],
        },
        _ => &[],
    };
    fields.iter().filter_map(|field| written.get(&field.ty.0).copied()).collect()
}

/// Extend the table to cover every type interned since it was built.
///
/// The table is dense over the interner and built last, on the stated ground that
/// nothing may intern a type after it — "a `TyId` past the end of this table
/// would read as uncounted, which is a leak that no test can see". Then
/// monomorphisation arrived, whose whole job is to intern `[str]` where only
/// `[A]` existed (panel 029 R3).
///
/// So the invariant moves rather than breaking: nothing may intern a type after
/// this table *without extending it*, and there is exactly one entry point that
/// does. It recomputes the whole table rather than appending, because the answer
/// for an existing type can only be the same and a rebuild cannot drift from an
/// append.
pub(crate) fn extend(types: &Types, ast: &Ast, written: &Written) -> Vec<bool> {
    table(types, ast, written)
}
