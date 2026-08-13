//! Who owns a counted reference, and the `retain`/`release` that follows from it
//! (design.md §4.10, §4.20; panel 022, panel 034).
//!
//! Two questions live here and they are one concern: **does this declaration own a
//! reference**, and **what is the one line that keeps it alive**. They are together
//! because the first is the guard on the second — `bodies` asks `owns` before it emits
//! anything, and a type that answers `false` gets no `retain` at all, so a wrong answer
//! here is a leak or a double free rather than a bad-looking function.
//!
//! The ownership question is asked of the **declaration**, never of the interner, and
//! that distinction is the repair D3 paid for: `Ty::Case` is interned when a pattern
//! destructures a payload and `Ty::Named` when a written type or a construction names
//! it, so both are facts about what a program happened to *mention*. Whether a value
//! owns a reference is a fact about what it *is*. The same declaration emitted a walking
//! `release` in one compilation and an empty one in another because a different module
//! held the `match`.
//!
//! Every fallback in this file is **loud**. A field kind with no row emits
//! `hero_unreachable()` rather than nothing, because nothing is a leak that no
//! instrument in this project reports, and `hero_unreachable()` is a program that stops.

use crate::source::Source;
use crate::syntax::{Ast, Field};
use crate::types::{Checked, Ty};

use super::typedefs::Names;
use super::mangle;
use super::types::{cases_of, fields_of};

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
pub(super) fn owns(ast: &Ast, checked: &Checked, decl: u32) -> bool {
    match checked.types.lookup(Ty::Named(decl)) {
        Some(id) => crate::ir::is_refcounted(checked, id),
        None => any_field_is_counted(ast, checked, fields_of(ast, decl)),
    }
}

pub(super) fn owns_case(ast: &Ast, checked: &Checked, decl: u32, case: u32) -> bool {
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
pub(super) fn reference_line(
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
pub(super) fn reference_body(
    w: &mut super::writer::Writer,
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
pub(super) fn variant_reference_body(
    w: &mut super::writer::Writer,
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
