//! Which types need a `HeroDesc`, and what to call it (design.md §4.20; panel 022).
//!
//! The runtime works on an array through a descriptor because C has no generic
//! copy, drop, comparison or hash — and it must be a *descriptor* rather than a
//! type-erased runtime that inspects values, because inspection would void the
//! property this backend rests on: that clang type-checks every call.
//!
//! **A reachability worklist, not a walk over the interned arena** (panel 022). A
//! descriptor is a `static const` object, so one nothing points at is
//! `-Wunused-const-variable` under §3.1's own flag set — the same warning that made
//! a `test` block's string literals a defect at step 3. The set is therefore
//! *derived*: exactly the element types of the arrays the program contains, closed
//! under nesting.
//!
//! **Five come from the runtime and are shared.** The four scalars, and — the one
//! worth stating — a single descriptor for **every** `[T]`, whatever `T` is:
//! `copy`, `drop`, `eq` and `hash` on an array value all reach the element type
//! through the header's own `elem`, so `[[int]]` and `[[str]]` need one descriptor
//! between them rather than one each.

use std::collections::BTreeSet;

use crate::types::{Checked, Ty, TyId};

use super::ctype::Names;

/// The C expression for a pointer to this type's descriptor, or `None` where the
/// backend has no representation for it yet — which `gate.rs` has already refused.
pub(super) fn pointer(checked: &Checked, names: &Names, ty: TyId) -> Option<String> {
    match checked.types.get(ty) {
        Ty::Int => Some("&hero_desc_int".to_string()),
        Ty::F64 => Some("&hero_desc_f64".to_string()),
        Ty::Bool => Some("&hero_desc_bool".to_string()),
        Ty::Str => Some("&hero_desc_str".to_string()),
        // One for all of them: see the module doc.
        Ty::Array(_) => Some("&hero_desc_array".to_string()),
        Ty::Map(_, _) => Some("&hero_desc_map".to_string()),
        // Every `T?` needs its own, because its payload is by value — and so does the
        // failure side, which the runtime ships.
        Ty::Fallible(_) => Some(format!("&{}_desc", names.option_of(ty))),
        Ty::Failure => Some("&hero_desc_failure".to_string()),
        Ty::Named(decl) => Some(format!("&{}_desc", names.of(decl))),
        Ty::Case(decl, case) => Some(format!("&{}_desc", names.case_of(decl, case))),
        _ => None,
    }
}

/// The C expression that hashes one value at `place`.
///
/// **Not** `descriptor->hash(&place)` for an aggregate, and the difference is a real
/// one: a descriptor exists only for a type used as an array *element*, while `hash` is
/// generated for every aggregate. `record Rect { corner: Point }` hashes a `Point` that
/// no array holds, and going through a descriptor there is `error: use of undeclared
/// identifier 'h_m_Point_desc'` — measured on the gallery's own second program.
///
/// The runtime's five descriptors always exist, so a scalar, a `str` and an array go
/// through theirs; an aggregate calls its own function.
pub(super) fn hash_call(checked: &Checked, names: &Names, ty: TyId, place: &str) -> Option<String> {
    match checked.types.get(ty) {
        Ty::Named(decl) => Some(format!("{}_hash({place})", names.of(decl))),
        Ty::Case(decl, case) => Some(format!("{}_hash({place})", names.case_of(decl, case))),
        _ => pointer(checked, names, ty).map(|desc| format!("({desc})->hash({place})")),
    }
}

/// Every *generated* descriptor the program needs: the aggregate element types,
/// closed under nesting, as `TyId`s.
///
/// Sorted and deduplicated, because the emitted order has to be a function of the
/// program and not of the interner's insertion order — the double-emit determinism
/// test (CLAUDE.md §7) is what that buys.
pub(super) fn generated(checked: &Checked) -> Vec<TyId> {
    let mut wanted: BTreeSet<u32> = BTreeSet::new();
    // Every container in the program contributes what it holds. No second pass is
    // needed for nesting: an inner `[T]` is itself an interned array type, so it is
    // already in the list below — which is why `[[Point]]` reaches `Point` without the
    // walk ever descending.
    // Both containers contribute: an array its element, a map its key *and* its
    // value, since the runtime reaches all three through descriptors.
    let mut queue: Vec<TyId> = (0..checked.types.len())
        .map(|index| TyId(index as u32))
        .filter(|id| matches!(checked.types.get(*id), Ty::Array(_) | Ty::Map(_, _)))
        .collect();
    while let Some(id) = queue.pop() {
        let elements: Vec<TyId> = match checked.types.get(id) {
            Ty::Array(element) => vec![element],
            Ty::Map(key, value) => vec![key, value],
            _ => Vec::new(),
        };
        for element in elements {
            match checked.types.get(element) {
                // The scalars and `str` are the runtime's, and an array's descriptor
                // is the one shared row — none of the three is generated.
                Ty::Int | Ty::F64 | Ty::Bool | Ty::Str => {}
                Ty::Array(_) | Ty::Map(_, _) | Ty::Failure => {}
                // A `T?` element needs its own descriptor, and so does an aggregate.
                Ty::Named(_) | Ty::Case(_, _) | Ty::Fallible(_) => {
                    wanted.insert(element.0);
                }
                // Refused by the gate until the step that lands it.
                _ => {}
            }
        }
    }
    wanted.into_iter().map(TyId).collect()
}
