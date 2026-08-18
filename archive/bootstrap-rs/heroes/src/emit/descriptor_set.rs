//! **Which** descriptors a program needs — the reachability half of
//! `descriptors.rs` (design.md §4.20; panel 022).
//!
//! Split from it 2026-08-16 by §11's ceiling, and the seam is the one that file
//! already named: it "answers three questions about descriptors and then writes
//! them", and *which ones exist* is a question about the **type table**, while
//! naming, calling and emitting one are questions about the **C**. Nothing here
//! writes a byte of output; nothing there walks the arena.
//!
//! The walk earns its own file for a second reason, learned the hard way: it is
//! the part that goes wrong silently. A descriptor nothing points at is a
//! warning; a descriptor something points at and nobody defined is `use of
//! undeclared identifier` at **exit 2**, the compiler blaming itself for a
//! correct program — and that is what a stale pre-pass produced
//! (`tests/golden/run/fixedbugs-option-payload-descriptor`).

use std::collections::BTreeSet;

use crate::syntax::Ast;
use crate::types::{Checked, Ty, TyId};

/// Every *generated* descriptor the program needs, as `TyId`s.
///
/// **The question is "which descriptors does the emitter name", not "which types
/// did the program mention inside a container".** Those were the same set while
/// arrays and maps were the only things that reached a descriptor, and they
/// stopped being the same set when `option_bodies` started naming one and
/// `hash_body` started routing fields through `pointer`. Asking the old question
/// produced `use of undeclared identifier 'h_M_P_desc'` at exit 2 on
/// `function grab() -> P?` — with the tell that adding `[P]` *anywhere in the
/// program*, in any module, made it compile (2026-08-12).
///
/// Three sources, and each one is a place the emitter writes `&…_desc`:
///
/// 1. **Container elements** — an array its element, a map its key *and* its
///    value, since the runtime reaches all three through descriptors. No second
///    pass is needed for nesting: an inner `[T]` is itself an interned array
///    type, which is why `[[Point]]` reaches `Point` without descending.
/// 2. **Every `T?`'s payload**, because `option_bodies` writes
///    `(&{payload}_desc)->eq(…)` and `->hash(…)` for *every* option it emits.
/// 3. **Every field of every aggregate**, because `hash_body` sends a field whose
///    type is not itself an aggregate through `hash_call` → `pointer`.
///
/// A container the emitted functions do not mention is skipped too: `test`
/// blocks intern types, and an ordinary build does not emit them.
///
/// A type that still mentions a type parameter is skipped, exactly as the two
/// sibling walks in `ctype.rs` and `types.rs` do: monomorphisation deletes the
/// generic *functions*, not the `A?` their signatures interned, and asking
/// `Names::option_of` for one panicked the compiler at exit 101 — a code
/// CLAUDE.md §10 does not have.
///
/// Sorted and deduplicated, because the emitted order has to be a function of the
/// program and not of the interner's insertion order — the double-emit determinism
/// test (CLAUDE.md §7) is what that buys.
pub(super) fn generated(
    ast: &Ast,
    checked: &Checked,
    reachable: &BTreeSet<u32>,
) -> Vec<TyId> {
    let mut wanted: BTreeSet<u32> = BTreeSet::new();
    let mut queue: Vec<TyId> = Vec::new();
    for index in 0..checked.types.len() {
        let id = TyId(index as u32);
        if super::ctype::mentions_generic(checked, id) {
            continue;
        }
        // A container the **emitted** functions never mention contributes
        // nothing. The checker interns from `test` blocks too, so a `[P]` that
        // only a test builds used to put `h_M_P_desc` into an ordinary program
        // build and leave it `-Wunused-const-variable` — the warning this
        // worklist's own doc says it exists to prevent (2026-08-12).
        if !reachable.contains(&id.0) {
            continue;
        }
        match checked.types.get(id) {
            Ty::Array(element) => queue.push(element),
            Ty::Map(key, value) => {
                queue.push(key);
                queue.push(value);
            }
            Ty::Fallible(payload) => queue.push(payload),
            _ => {}
        }
    }
    for decl in super::types::aggregates(ast, checked) {
        let mut field_types = Vec::new();
        for field in super::types::fields_of(ast, decl) {
            field_types.push(field.ty);
        }
        for case in super::types::cases_of(ast, decl) {
            for field in &case.fields {
                field_types.push(field.ty);
            }
        }
        for written in field_types {
            // Only a `T?` field. `hash_call` sends `Ty::Named` and `Ty::Case`
            // straight to `{name}_hash`, and every other field kind reaches one
            // of the runtime's five shared rows — so pushing them all generates
            // descriptors nothing points at, which is
            // `-Wunused-const-variable` under §3.1's flags and the exact warning
            // this worklist exists to avoid.
            let Some(ty) = checked.written_type(written) else { continue };
            if matches!(checked.types.get(ty), Ty::Fallible(_)) {
                queue.push(ty);
            }
        }
    }
    // **The worklist expands as it drains**, because the loop above sees only
    // types the *program* reaches while the aggregates loop pushes `T?` field
    // types after it — and a case declared but never constructed still gets an
    // `eq` arm that calls its payload's descriptor. Collecting without expanding
    // left that payload out at exit 2
    // (`tests/golden/run/fixedbugs-option-payload-descriptor`). `seen` guards the
    // queue rather than `wanted`: an array is expanded and never generated.
    let mut seen: BTreeSet<u32> = BTreeSet::new();
    while let Some(id) = queue.pop() {
        if super::ctype::mentions_generic(checked, id) || !seen.insert(id.0) {
            continue;
        }
        match checked.types.get(id) {
            // The scalars and `str` are the runtime's, and one shared row serves
            // every array, every map and every function type — none is generated.
            // `Ty::Named` and `Ty::Case` reached as a *field* go through
            // `{name}_hash` directly rather than through a descriptor, but they
            // still need one when they are an element or an option's payload, and
            // an unused-descriptor warning is cheaper than a missing one.
            Ty::Named(_) | Ty::Case(_, _) => {
                wanted.insert(id.0);
            }
            Ty::Fallible(payload) => {
                wanted.insert(id.0);
                queue.push(payload);
            }
            Ty::Array(element) => queue.push(element),
            Ty::Map(key, value) => {
                queue.push(key);
                queue.push(value);
            }
            _ => {}
        }
    }
    // ORDER: ascending TyId — `definitions` walks this into the emitted `_desc`
    // table, so the Heroes port owes an explicit sort here (design.md §4.9).
    wanted.into_iter().map(TyId).collect()
}
