//! The two functions that must agree: generated `eq` and generated `hash`
//! (design.md §4.3, §4.9; panel 022, panel 029).
//!
//! §4.3 makes `==` structural on everything, recursively, and a map needs a hash of the
//! same values. C provides neither, so the compiler writes both — and they are in one
//! file because **they are one invariant**: two values that compare equal must hash
//! equal, and the moment the two walks disagree a map loses a key it contains. That
//! failure is silent on insert and loud only much later, which is why the two live where
//! one diff shows both.
//!
//! **Fields, never bytes**, for both, and panel 022 found the reason by compiling it:
//! `record Flag { n: i64, on: bool }` carries 7 bytes of padding whose contents are
//! whatever was there before, so `memcmp` reports two `==`-equal records as different and
//! a byte hash gives them different hashes — with no warning, no error and no sanitiser
//! report. `hash` is generated for **every** aggregate rather than only for map-key
//! types, because a descriptor's null `hash` is `SEGV on unknown address 0x0, pc 0x0`:
//! no type name, no source line.
//!
//! The tables here are exhaustive by construction and their last arm is loud. The `eq`
//! table went without three rows until 2026-08-12 — `Ty::Map`, `Ty::Fallible`,
//! `Ty::Func` — while `hash` routed through `descriptors::hash_call`, which covered all
//! three; the disagreement broke CLAUDE.md §7 in the loudest available direction,
//! `{Box: i64}` inserting fine and aborting on lookup.

use crate::source::Source;
use crate::syntax::{Ast, Field};
use crate::types::{Checked, Ty};

use super::typedefs::Names;
use super::mangle;
use super::types::cases_of;
use super::writer::Writer;

/// Structural `==`, field by field (§4.3).
pub(super) fn equality_body(
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
            Ty::Int(_) | Ty::Bool | Ty::Float(_) => format!("a->{member} == b->{member}"),
            Ty::Str => format!("hero_str_eq(a->{member}, b->{member})"),
            Ty::Array(_) => format!("hero_array_eq(a->{member}, b->{member})"),
            // The three rows this table went without until 2026-08-12, each one a
            // field kind the checker has always accepted. `hash_body` routes
            // through `descriptors::hash_call`, which covers all three — so the
            // gap broke CLAUDE.md §7's "eq and hash agree" in the loudest
            // direction: `{Box: i64}` inserted fine and aborted on lookup.
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

/// A variant's `==`: different tags are never equal, same tags compare payloads.
pub(super) fn variant_equality_body(
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
/// that compare equal must hash equal, and `record Flag { n: i64, on: bool }` has 7
/// padding bytes whose contents are whatever was there before. Hashing them makes
/// `eq` and `hash` disagree, which is a map that loses a key it contains.
///
/// FNV-1a over the field hashes, the same mix the runtime uses, so there is one hash
/// function in this project rather than two that drift. A nested aggregate calls its
/// own `hash` rather than going through a descriptor, because a descriptor exists only
/// for an array's element type — see `descriptors::hash_call`.
pub(super) fn hash_body(
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
pub(super) fn variant_hash_body(
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
