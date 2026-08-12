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
//! through the header's own `elem`, so `[[i64]]` and `[[str]]` need one descriptor
//! between them rather than one each.
//!
//! The file answers three questions about descriptors and then **writes them**
//! (`definitions`, moved here from `perfn.rs` by the §11 sweep of 2026-08-12): which
//! ones the program needs, how to name one, how to call its `hash` — and the C that
//! defines them. The emitter was the odd one out where it was, since every caller of
//! it is a caller of `generated` one line later.

use std::collections::BTreeSet;

use crate::syntax::Ast;
use crate::types::{Checked, IntKind, Ty, TyId};

use super::ctype::Names;
use super::writer::Writer;

/// The C expression for a pointer to this type's descriptor, or `None` where the
/// backend has no representation for it yet — which `gate.rs` has already refused.
pub(super) fn pointer(checked: &Checked, names: &Names, ty: TyId) -> Option<String> {
    match checked.types.get(ty) {
        // Exhaustive on the width, not `Ty::Int(_)`: a descriptor is a runtime
        // object with a size in it, and `runtime/parts/sort.c` dispatches on the
        // pointer's identity — so a second width silently sharing this one would
        // compare and sort the wrong number of bytes (panel 042).
        Ty::Int(kind) => Some(
            match kind {
                IntKind::I64 => "&hero_desc_int",
                IntKind::I8 => "&hero_desc_i8",
                IntKind::I16 => "&hero_desc_i16",
                IntKind::I32 => "&hero_desc_i32",
                IntKind::U8 => "&hero_desc_u8",
                IntKind::U16 => "&hero_desc_u16",
                IntKind::U32 => "&hero_desc_u32",
                IntKind::U64 => "&hero_desc_u64",
            }
            .to_string(),
        ),
        Ty::F64 => Some("&hero_desc_f64".to_string()),
        Ty::Bool => Some("&hero_desc_bool".to_string()),
        Ty::Str => Some("&hero_desc_str".to_string()),
        // One for all of them: see the module doc.
        Ty::Array(_) => Some("&hero_desc_array".to_string()),
        Ty::Map(_, _) => Some("&hero_desc_map".to_string()),
        // One for every function type, like the array's: a function value owns
        // nothing, so the four operations do not depend on the signature.
        Ty::Func { .. } => Some("&hero_desc_func".to_string()),
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
    while let Some(id) = queue.pop() {
        if super::ctype::mentions_generic(checked, id) {
            continue;
        }
        match checked.types.get(id) {
            // The scalars and `str` are the runtime's, and one shared row serves
            // every array, every map and every function type — none is generated.
            // `Ty::Named` and `Ty::Case` reached as a *field* go through
            // `{name}_hash` directly rather than through a descriptor, but they
            // still need one when they are an element or an option's payload, and
            // an unused-descriptor warning is cheaper than a missing one.
            Ty::Named(_) | Ty::Case(_, _) | Ty::Fallible(_) => {
                wanted.insert(id.0);
            }
            _ => {}
        }
    }
    wanted.into_iter().map(TyId).collect()
}

/// One `static const HeroDesc` per element type the program actually uses.
///
/// Emitted **before** the function definitions, not with the other bodies: an array
/// literal names its element descriptor, so `hero_array_new(&h_m_Point_desc, 2)` in
/// `main` needs the object to exist by then. Measured the other way first — three
/// `error: use of undeclared identifier 'h_m_Point_desc'` on the first program with
/// an array of records in it.
///
/// `static`, so a descriptor nothing points at is `-Wunused-const-variable` — which
/// is exactly why the set is a worklist over the program's arrays rather than a walk
/// over every declaration (panel 022).
///
/// The four function pointers are cast to the descriptor's own signatures. That cast
/// is the price of a generic runtime in C, and it is paid **once per type here**
/// rather than at every call site: `retain`/`release` take a typed pointer so that
/// the emitter's own calls are checked, and only the descriptor erases them.
pub(super) fn definitions(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    reachable: &std::collections::BTreeSet<u32>,
) {
    let wanted = generated(ast, checked, reachable);
    if wanted.is_empty() {
        return;
    }
    for ty in wanted {
        let name = match checked.types.get(ty) {
            Ty::Named(decl) => names.of(decl).to_string(),
            Ty::Case(decl, case) => names.case_of(decl, case).to_string(),
            // A `T?` used as an element needs one too, and its four functions already
            // exist — `option_bodies` writes them for every `T?` in the program.
            Ty::Fallible(_) => names.option_of(ty).to_string(),
            _ => continue,
        };
        let counted = crate::ir::is_refcounted(checked, ty);
        w.at_generated();
        // The adapters. `retain`/`release`/`eq` take TYPED pointers so that every call
        // the emitter itself writes is checked by clang; only these three erase them,
        // once per type, which is the whole price of a generic runtime in C.
        w.line(&format!("static void {name}_desc_copy(void *dst, const void *src) {{"));
        w.line(&format!("    *({name} *)dst = *(const {name} *)src;"));
        if counted {
            // Shallow plus incref, never deep (panel 022): copy-on-write is what makes
            // a deep copy unnecessary, because sharing is unobservable until somebody
            // mutates. A deep copy here would pay for every binding what only a
            // mutation costs.
            w.line(&format!("    {name}_retain((const {name} *)dst);"));
        }
        w.line("}");
        w.line(&format!("static void {name}_desc_drop(void *elem) {{"));
        if counted {
            w.line(&format!("    {name}_release(({name} *)elem);"));
        } else {
            // Nothing inside owns a reference, so there is nothing to release — and
            // `(void)` keeps the parameter used, because `-Wall` is on.
            w.line("    (void)elem;");
        }
        w.line("}");
        w.line(&format!("static bool {name}_desc_eq(const void *a, const void *b) {{"));
        w.line(&format!(
            "    return {name}_eq((const {name} *)a, (const {name} *)b);"
        ));
        w.line("}");
        w.line(&format!("static const HeroDesc {name}_desc = {{"));
        w.line(&format!("    sizeof({name}),"));
        w.line(&format!("    {name}_desc_copy,"));
        w.line(&format!("    {name}_desc_drop,"));
        w.line(&format!("    {name}_desc_eq,"));
        w.line(&format!("    {name}_hash,"));
        w.line("};");
        w.blank();
    }
}
