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

use crate::syntax::Ast;
use crate::types::{Checked, IntKind, Ty, TyId};

use super::descriptor_set::generated;
use super::typedefs::Names;
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
        Ty::Float(kind) => Some(format!("&hero_desc_{}", kind.name())),
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
        Ty::Named(decl) => Some(format!("&{}_desc", names.satellite(decl))),
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
        Ty::Named(decl) => Some(format!("{}_hash({place})", names.satellite(decl))),
        Ty::Case(decl, case) => Some(format!("{}_hash({place})", names.case_of(decl, case))),
        _ => pointer(checked, names, ty).map(|desc| format!("({desc})->hash({place})")),
    }
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
        // **Both names, because this loop writes both kinds of string.**
        // `sizeof({c_type})` and `(const {c_type} *)a` are the type; `{prefix}_desc`
        // and `{prefix}_eq` are functions this compiler generated. They are the same
        // word for everything except a group's `record`, which is why one variable
        // did the job until a header was involved — and then emitted
        // `static const HeroDesc Color_desc`, a **global unmangled name beside the
        // library that declared `Color`** (CLAUDE.md §7). Panel 061.
        let (prefix, c_type) = match checked.types.get(ty) {
            Ty::Named(decl) => (names.satellite(decl).to_string(), names.of(decl).to_string()),
            Ty::Case(decl, case) => {
                let n = names.case_of(decl, case).to_string();
                (n.clone(), n)
            }
            // A `T?` used as an element needs one too, and its four functions already
            // exist — `option_bodies` writes them for every `T?` in the program.
            Ty::Fallible(_) => {
                let n = names.option_of(ty).to_string();
                (n.clone(), n)
            }
            _ => continue,
        };
        let counted = crate::ir::is_refcounted(checked, ty);
        w.at_generated();
        // The adapters. `retain`/`release`/`eq` take TYPED pointers so that every call
        // the emitter itself writes is checked by clang; only these three erase them,
        // once per type, which is the whole price of a generic runtime in C.
        w.line(&format!("static void {prefix}_desc_copy(void *dst, const void *src) {{"));
        w.line(&format!("    *({c_type} *)dst = *(const {c_type} *)src;"));
        if counted {
            // Shallow plus incref, never deep (panel 022): copy-on-write is what makes
            // a deep copy unnecessary, because sharing is unobservable until somebody
            // mutates. A deep copy here would pay for every binding what only a
            // mutation costs.
            w.line(&format!("    {prefix}_retain((const {c_type} *)dst);"));
        }
        w.line("}");
        w.line(&format!("static void {prefix}_desc_drop(void *elem) {{"));
        if counted {
            w.line(&format!("    {prefix}_release(({c_type} *)elem);"));
        } else {
            // Nothing inside owns a reference, so there is nothing to release — and
            // `(void)` keeps the parameter used, because `-Wall` is on.
            w.line("    (void)elem;");
        }
        w.line("}");
        w.line(&format!("static bool {prefix}_desc_eq(const void *a, const void *b) {{"));
        w.line(&format!(
            "    return {prefix}_eq((const {c_type} *)a, (const {c_type} *)b);"
        ));
        w.line("}");
        w.line(&format!("static const HeroDesc {prefix}_desc = {{"));
        w.line(&format!("    sizeof({c_type}),"));
        w.line(&format!("    {prefix}_desc_copy,"));
        w.line(&format!("    {prefix}_desc_drop,"));
        w.line(&format!("    {prefix}_desc_eq,"));
        w.line(&format!("    {prefix}_hash,"));
        w.line("};");
        w.blank();
    }
}
