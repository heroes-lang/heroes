//! The per-type functions C cannot write for itself (design.md §4.3, §4.10, §4.20;
//! panel 022).
//!
//! C has no copy constructors, no destructors and no generic comparison, while §4.3
//! demands structural `==` recursively and §4.10 demands value-semantics copies and
//! drops. A type-erased `void *` runtime would answer it and would void the property
//! the whole backend rests on — that clang type-checks every call — so the compiler
//! writes plain C instead, one small function per type:
//!
//! | function | when it exists | what it does |
//! |---|---|---|
//! | `h_T_retain` | `T` owns a reference | one incref per counted field |
//! | `h_T_release` | `T` owns a reference | one decref per counted field |
//! | `h_T_eq` | always | `==`, **field by field** |
//!
//! **`eq` walks fields, never bytes**, and panel 022 found the reason by compiling it:
//! `record Flag { n: int, on: bool }` carries 7 bytes of padding, so two `==`-equal
//! records differ under `memcmp` with no warning, no error and no sanitiser report.
//! The same argument retires `memcpy`-shaped thinking everywhere in this file.
//!
//! A variant contributes its **case payloads first** and then itself, in that order and
//! for the same reason the typedefs are ordered: the outer function calls the inner
//! ones, and a case with fields *is* a small record (§4.2), so it gets exactly what a
//! record gets.

use crate::source::Source;
use crate::syntax::{Ast, Field};
use crate::types::{Checked, Ty};

use super::ctype::Names;
use super::mangle;
use super::ctype::c_type;
use super::types::{aggregates, cases_of, fields_of, is_variant};
use super::writer::Writer;

/// Prototypes for every per-type function, before any body.
///
/// All of them first, because `h_T_eq` on a nested aggregate calls the inner one and a
/// variant's calls its cases': the typedefs are ordered so the *types* are complete in
/// time, and this makes the *functions* visible in time without depending on that
/// order twice.
pub(super) fn prototypes(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
) {
    let mut any = false;
    for (name, counted) in every_generated_type(ast, checked, names, src) {
        w.at_generated();
        if counted {
            w.line(&format!("void {name}_retain(const {name} *v);"));
            w.line(&format!("void {name}_release({name} *v);"));
        }
        w.line(&format!("bool {name}_eq(const {name} *a, const {name} *b);"));
        w.line(&format!("uint64_t {name}_hash(const void *elem);"));
        any = true;
    }
    for (name, _) in names.options(checked) {
        w.at_generated();
        w.line(&format!("void {name}_retain(const {name} *v);"));
        w.line(&format!("void {name}_release({name} *v);"));
        w.line(&format!("bool {name}_eq(const {name} *a, const {name} *b);"));
        w.line(&format!("uint64_t {name}_hash(const void *elem);"));
        any = true;
    }
    if any {
        w.blank();
    }
}

/// Every C type this pass generates functions for, in emission order, with whether it
/// owns a counted reference.
///
/// A variant contributes its **case payloads first** and then itself, which is the
/// same order the typedefs need and for the same reason: the outer function calls the
/// inner ones.
fn every_generated_type(
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
) -> Vec<(String, bool)> {
    let _ = src;
    let mut out = Vec::new();
    for decl in aggregates(ast, checked) {
        let name = names.of(decl).to_string();
        if is_variant(ast, decl) {
            for (at, case) in cases_of(ast, decl).iter().enumerate() {
                if case.fields.is_empty() {
                    continue;
                }
                let payload = names.case_of(decl, at as u32).to_string();
                out.push((payload, owns_case(ast, checked, decl, at as u32)));
            }
        }
        out.push((name, owns(ast, checked, decl)));
    }
    out
}

/// The bodies.
pub(super) fn bodies(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
) {
    for decl in aggregates(ast, checked) {
        let name = names.of(decl).to_string();
        if is_variant(ast, decl) {
            // The payloads first: a case is a small record (§4.2), so it gets exactly
            // what a record gets.
            for (at, case) in cases_of(ast, decl).iter().enumerate() {
                if case.fields.is_empty() {
                    continue;
                }
                let payload = names.case_of(decl, at as u32).to_string();
                if owns_case(ast, checked, decl, at as u32) {
                    reference_body(w, checked, names, src, &payload, &case.fields, true);
                    reference_body(w, checked, names, src, &payload, &case.fields, false);
                }
                equality_body(w, checked, names, src, &payload, &case.fields);
                hash_body(w, checked, names, src, &payload, &case.fields);
            }
            if owns(ast, checked, decl) {
                variant_reference_body(w, ast, checked, names, src, decl, &name, true);
                variant_reference_body(w, ast, checked, names, src, decl, &name, false);
            }
            variant_equality_body(w, ast, checked, names, src, decl, &name);
            variant_hash_body(w, ast, checked, names, src, decl, &name);
            continue;
        }
        let fields = fields_of(ast, decl);
        if owns(ast, checked, decl) {
            reference_body(w, checked, names, src, &name, fields, true);
            reference_body(w, checked, names, src, &name, fields, false);
        }
        equality_body(w, checked, names, src, &name, fields);
        hash_body(w, checked, names, src, &name, fields);
    }
    option_bodies(w, checked, names);
}

/// The four functions for each `T?`. Every one switches on the tag, because the two
/// sides of a `T?` are alternatives and touching the wrong one is reading a union
/// member that was never written.
fn option_bodies(w: &mut Writer, checked: &Checked, names: &Names) {
    for (name, payload) in names.options(checked) {
        let counted = crate::ir::is_refcounted(checked, payload);
        let has_ok = c_type(names, checked, payload).is_some();
        w.at_generated();
        // retain / release. The error side is ALWAYS counted (two `str`s), which is why
        // every `T?` is counted whatever `T` is — `int?` included.
        for keep in [true, false] {
            let verb = if keep { "retain" } else { "release" };
            if keep {
                w.line(&format!("void {name}_retain(const {name} *v) {{"));
            } else {
                w.line(&format!("void {name}_release({name} *v) {{"));
            }
            w.line("    if (v->tag == INT64_C(0)) {");
            if counted && has_ok {
                // The same dispatch a record field gets, and NOT the descriptor's
                // `copy`: a descriptor has `copy(dst, src)` and no retain-in-place, so
                // going through it would mean copying the payload onto itself and
                // casting away the `const` on the retain's parameter. One helper, two
                // call sites, no cast.
                match reference_line(checked, names, payload, "v->as.ok", keep) {
                    Some(line) => w.line(&format!("    {line}")),
                    None => w.line("        hero_unreachable();"),
                }
            } else {
                w.line("        return;");
            }
            w.line("    } else {");
            w.line(&format!("        hero_failure_{verb}(&v->as.err);"));
            w.line("    }");
            w.line("}");
            w.blank();
        }
        // equality: different tags are never equal.
        w.line(&format!("bool {name}_eq(const {name} *a, const {name} *b) {{"));
        w.line("    if (a->tag != b->tag) return false;");
        w.line("    if (a->tag != INT64_C(0)) return hero_failure_eq(&a->as.err, &b->as.err);");
        if has_ok {
            match super::descriptors::pointer(checked, names, payload) {
                Some(desc) => w.line(&format!("    return ({desc})->eq(&a->as.ok, &b->as.ok);")),
                None => w.line("    hero_unreachable();"),
            }
        } else {
            // Two `()?` that are both ok carry nothing to compare.
            w.line("    return true;");
        }
        w.line("}");
        w.blank();
        w.line(&format!("uint64_t {name}_hash(const void *elem) {{"));
        w.line(&format!("    const {name} *v = elem;"));
        w.line("    uint64_t h = (UINT64_C(0xcbf29ce484222325) ^ (uint64_t)v->tag)");
        w.line("        * UINT64_C(0x100000001b3);");
        w.line("    if (v->tag != INT64_C(0)) {");
        w.line("        return (h ^ hero_failure_hash(&v->as.err)) * UINT64_C(0x100000001b3);");
        w.line("    }");
        if has_ok {
            match super::descriptors::pointer(checked, names, payload) {
                Some(desc) => {
                    w.line(&format!("    return (h ^ ({desc})->hash(&v->as.ok)) * UINT64_C(0x100000001b3);"));
                }
                None => w.line("    hero_unreachable();"),
            }
        } else {
            w.line("    return h;");
        }
        w.line("}");
        w.blank();
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
pub(super) fn descriptors(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    reachable: &std::collections::BTreeSet<u32>,
) {
    let wanted = super::descriptors::generated(ast, checked, reachable);
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

/// Whether a value of this declaration owns a counted reference — asked through
/// `ir::is_refcounted`, which is the one home for the question.
///
/// The `None` arm used to be `false`, under the reasoning that a declaration no
/// written type mentions is one nothing can hold. That is the same sentence
/// `owns_case` carried, and there it was wrong (D3): the interner records what a
/// program *mentioned*, and the question is what a declaration *is*. It was never
/// reproduced here and may well be unreachable — a `Ty::Named` is interned by any
/// written type or any construction, so an uninterned declaration is one nothing
/// builds. It is asked of the fields anyway, because "I could not build the
/// program that reaches it" is exactly what was true of D3's twin the day before
/// it leaked, and the two arms now give the same answer for the same reason.
fn owns(ast: &Ast, checked: &Checked, decl: u32) -> bool {
    match checked.types.lookup(Ty::Named(decl)) {
        Some(id) => crate::ir::is_refcounted(checked, id),
        None => any_field_is_counted(ast, checked, fields_of(ast, decl)),
    }
}

fn owns_case(ast: &Ast, checked: &Checked, decl: u32, case: u32) -> bool {
    match checked.types.lookup(Ty::Case(decl, case)) {
        Some(id) => crate::ir::is_refcounted(checked, id),
        // No `match` arm binds this payload, so no *temporary* of it exists — but the
        // variant is still built, copied and released, and its release has to reach
        // the fields inside it. The answer therefore comes from the fields, never
        // from the interner (panel 034 D1).
        None => any_field_is_counted(ast, checked, &cases_of(ast, decl)[case as usize].fields),
    }
}

/// Does any of these fields own a counted reference?
///
/// **This is the repair, and it is one function because it was one mistake.**
/// `Ty::Case` is interned when a pattern *destructures* a payload and `Ty::Named`
/// when a written type or a construction names it — both are facts about what a
/// program happened to mention. Whether a value owns a reference is a fact about
/// its declaration. A module may build `.name(s: …)` and never destructure it,
/// which made the same declaration emit a walking `release` in one compilation
/// and an empty one in another, purely because a *different* module held the
/// `match`. The fields do not move between compilations; the interner does.
fn any_field_is_counted(ast: &Ast, checked: &Checked, fields: &[Field]) -> bool {
    let _ = ast;
    fields.iter().any(|field| {
        let ty = checked.written_type(field.ty).unwrap_or_else(|| checked.types.error());
        crate::ir::is_refcounted(checked, ty)
    })
}

/// The one call that retains or releases a counted value sitting at `place`.
///
/// Shared by a record's fields and a `T?`'s payload, because the question is the same
/// one — what does *this type* need to keep a reference alive — and two answers to it is
/// the refcount bug `ir/layout.rs` documents.
fn reference_line(
    checked: &Checked,
    names: &Names,
    ty: crate::types::TyId,
    place: &str,
    keep: bool,
) -> Option<String> {
    let verb = if keep { "retain" } else { "release" };
    Some(match checked.types.get(ty) {
        Ty::Str if keep => format!("    hero_str_incref({place});"),
        Ty::Str => format!("    hero_str_decref({place});"),
        Ty::Array(_) if keep => format!("    hero_array_incref({place});"),
        Ty::Array(_) => format!("    hero_array_decref({place});"),
        Ty::Map(_, _) if keep => format!("    hero_map_incref({place});"),
        Ty::Map(_, _) => format!("    hero_map_decref({place});"),
        Ty::Failure => format!("    hero_failure_{verb}(&{place});"),
        // Reached by address, and its own generated function decides what inside counts.
        Ty::Named(inner) => format!("    {}_{verb}(&{place});", names.of(inner)),
        Ty::Case(inner, case) => format!("    {}_{verb}(&{place});", names.case_of(inner, case)),
        Ty::Fallible(_) => format!("    {}_{verb}(&{place});", names.option_of(ty)),
        _ => return None,
    })
}

/// `retain`/`release` for a record or a case payload: one call per counted field.
fn reference_body(
    w: &mut Writer,
    checked: &Checked,
    names: &Names,
    src: &Source,
    name: &str,
    fields: &[Field],
    keep: bool,
) {
    w.at_generated();
    if keep {
        w.line(&format!("void {name}_retain(const {name} *v) {{"));
    } else {
        w.line(&format!("void {name}_release({name} *v) {{"));
    }
    for field in fields {
        let ty = checked.written_type(field.ty).unwrap_or_else(|| checked.types.error());
        if !crate::ir::is_refcounted(checked, ty) {
            continue;
        }
        let member = mangle::field(src.slice(field.name));
        // Missing a row here is how `variant Expr` with a `[Expr]` payload aborted on
        // its first run: the fallback emits `hero_unreachable()` rather than nothing, so
        // the program stopped instead of leaking. That is the reason every row is listed
        // — and the reason the *last* one is loud.
        let line = reference_line(checked, names, ty, &format!("v->{member}"), keep)
            .unwrap_or_else(|| {
                "    hero_unreachable(); /* the gate refuses this field type */".to_string()
            });
        w.line(&line);
    }
    w.line("}");
    w.blank();
}

/// A variant's `retain`/`release`: switch on the tag, touch only the live case.
///
/// `default: break;` rather than every enumerator, because only the counted cases have
/// anything to do — and it satisfies `-Wswitch` without listing rows that would be
/// empty.
fn variant_reference_body(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
    decl: u32,
    name: &str,
    keep: bool,
) {
    let verb = if keep { "retain" } else { "release" };
    w.at_generated();
    if keep {
        w.line(&format!("void {name}_retain(const {name} *v) {{"));
    } else {
        w.line(&format!("void {name}_release({name} *v) {{"));
    }
    w.line("    switch (v->tag) {");
    for (at, case) in cases_of(ast, decl).iter().enumerate() {
        if case.fields.is_empty() || !owns_case(ast, checked, decl, at as u32) {
            continue;
        }
        let case_name = src.slice(case.name);
        w.line(&format!(
            "        case {}: {}_{verb}(&v->as.{}); break;",
            mangle::tag_of(name, case_name),
            names.case_of(decl, at as u32),
            mangle::case(case_name)
        ));
    }
    w.line("        default: break;");
    w.line("    }");
    w.line("}");
    w.blank();
}

/// A variant's `==`: different tags are never equal, same tags compare payloads.
fn variant_equality_body(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
    decl: u32,
    name: &str,
) {
    let _ = checked;
    w.at_generated();
    w.line(&format!("bool {name}_eq(const {name} *a, const {name} *b) {{"));
    w.line("    if (a->tag != b->tag) return false;");
    w.line("    switch (a->tag) {");
    for (at, case) in cases_of(ast, decl).iter().enumerate() {
        let case_name = src.slice(case.name);
        let tag = mangle::tag_of(name, case_name);
        if case.fields.is_empty() {
            // Two payload-free cases with the same tag *are* the same value: there is
            // nothing else to compare.
            w.line(&format!("        case {tag}: return true;"));
        } else {
            w.line(&format!(
                "        case {tag}: return {}_eq(&a->as.{}, &b->as.{});",
                names.case_of(decl, at as u32),
                mangle::case(case_name),
                mangle::case(case_name)
            ));
        }
    }
    w.line("    }");
    // Every enumerator is covered above, so this is a claim rather than a fallback —
    // and `hero_unreachable` is `_Noreturn`, which is what satisfies
    // `-Werror=return-type` without inventing an answer (spike 04's own shape).
    w.line("    hero_unreachable();");
    w.line("}");
    w.blank();
}

/// `hash`, field by field, for **every** aggregate — never null (panel 022).
///
/// Generated even where no map uses it, because a descriptor's `hash` being null is
/// `SEGV on unknown address 0x0, pc 0x0`: no type name, no source line. Go's cheaper
/// rule — emit it only for map-key types — reintroduces exactly that.
///
/// Fields, never bytes, and the reason is sharper here than for `eq`: two records
/// that compare equal must hash equal, and `record Flag { n: int, on: bool }` has 7
/// padding bytes whose contents are whatever was there before. Hashing them makes
/// `eq` and `hash` disagree, which is a map that loses a key it contains.
///
/// FNV-1a over the field hashes, the same mix the runtime uses, so there is one hash
/// function in this project rather than two that drift. A nested aggregate calls its
/// own `hash` rather than going through a descriptor, because a descriptor exists only
/// for an array's element type — see `descriptors::hash_call`.
fn hash_body(
    w: &mut Writer,
    checked: &Checked,
    names: &Names,
    src: &Source,
    name: &str,
    fields: &[Field],
) {
    w.at_generated();
    w.line(&format!("uint64_t {name}_hash(const void *elem) {{"));
    w.line(&format!("    const {name} *v = elem;"));
    w.line("    uint64_t h = UINT64_C(0xcbf29ce484222325);");
    for field in fields {
        let ty = checked.written_type(field.ty).unwrap_or_else(|| checked.types.error());
        let member = mangle::field(src.slice(field.name));
        let one = super::descriptors::hash_call(checked, names, ty, &format!("&v->{member}"))
            .unwrap_or_else(|| "(hero_unreachable(), UINT64_C(0))".to_string());
        w.line(&format!("    h = (h ^ {one}) * UINT64_C(0x100000001b3);"));
    }
    w.line("    return h;");
    w.line("}");
    w.blank();
}

/// A variant's `hash`: the tag mixed in, then the live case's payload.
///
/// The tag has to be part of it, and not as decoration: two payload-free cases differ
/// only by their tag, so leaving it out gives `.red` and `.green` the same hash — legal
/// but useless, and it is exactly the kind of degenerate hash that turns a map into a
/// list. Every enumerator is covered, so the fall-through is `hero_unreachable()`,
/// which is `_Noreturn` and satisfies `-Werror=return-type` without inventing a value.
fn variant_hash_body(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
    decl: u32,
    name: &str,
) {
    let _ = checked;
    w.at_generated();
    w.line(&format!("uint64_t {name}_hash(const void *elem) {{"));
    w.line(&format!("    const {name} *v = elem;"));
    w.line("    uint64_t h = (UINT64_C(0xcbf29ce484222325) ^ (uint64_t)v->tag)");
    w.line("        * UINT64_C(0x100000001b3);");
    w.line("    switch (v->tag) {");
    for (at, case) in cases_of(ast, decl).iter().enumerate() {
        let case_name = src.slice(case.name);
        let tag = mangle::tag_of(name, case_name);
        if case.fields.is_empty() {
            // Nothing else to mix: the tag already told the two apart.
            w.line(&format!("        case {tag}: return h;"));
        } else {
            w.line(&format!(
                "        case {tag}: return (h ^ {}_hash(&v->as.{})) * UINT64_C(0x100000001b3);",
                names.case_of(decl, at as u32),
                mangle::case(case_name)
            ));
        }
    }
    w.line("    }");
    w.line("    hero_unreachable();");
    w.line("}");
    w.blank();
}

/// Structural `==`, field by field (§4.3).
fn equality_body(
    w: &mut Writer,
    checked: &Checked,
    names: &Names,
    src: &Source,
    name: &str,
    fields: &[Field],
) {
    w.at_generated();
    w.line(&format!("bool {name}_eq(const {name} *a, const {name} *b) {{"));
    if fields.is_empty() {
        // `record E` is `error[empty_record]` in the checker, so this is belt to that
        // braces — and `(void)` keeps the parameters used, because `-Wall` is on.
        w.line("    (void)a;");
        w.line("    (void)b;");
        w.line("    return true;");
    }
    for field in fields {
        let ty = checked.written_type(field.ty).unwrap_or_else(|| checked.types.error());
        let member = mangle::field(src.slice(field.name));
        let test = match checked.types.get(ty) {
            Ty::Int | Ty::Bool | Ty::F64 => format!("a->{member} == b->{member}"),
            Ty::Str => format!("hero_str_eq(a->{member}, b->{member})"),
            Ty::Array(_) => format!("hero_array_eq(a->{member}, b->{member})"),
            // The three rows this table went without until 2026-08-12, each one a
            // field kind the checker has always accepted. `hash_body` routes
            // through `descriptors::hash_call`, which covers all three — so the
            // gap broke CLAUDE.md §7's "eq and hash agree" in the loudest
            // direction: `{Box: int}` inserted fine and aborted on lookup.
            Ty::Map(_, _) => format!("hero_map_eq(a->{member}, b->{member})"),
            Ty::Fallible(_) => {
                format!("{}_eq(&a->{member}, &b->{member})", names.option_of(ty))
            }
            // A function value is a bare C function pointer, and panel 029 made
            // its equality pointer identity.
            Ty::Func { .. } => format!("a->{member} == b->{member}"),
            Ty::Named(inner) => format!("{}_eq(&a->{member}, &b->{member})", names.of(inner)),
            Ty::Case(inner, case) => {
                format!("{}_eq(&a->{member}, &b->{member})", names.case_of(inner, case))
            }
            // Still loud, and it must stay loud: `Ty::Failure` has no surface
            // spelling (`types/render.rs`), so a field of that type is a
            // compiler bug rather than a program.
            _ => "(hero_unreachable(), false)".to_string(),
        };
        // One early return per field rather than a chain of `&&`: the same short
        // circuit, and a clang diagnostic about a field lands on that field's line.
        w.line(&format!("    if (!({test})) return false;"));
    }
    if !fields.is_empty() {
        w.line("    return true;");
    }
    w.line("}");
    w.blank();
}
