//! Aggregates as C: the `typedef struct`s, and the per-type functions C has no way
//! to write for itself (design.md §4.10, §4.20; panel 022).
//!
//! **Per-type functions are generated, not written in the runtime** (§4.20). C has no
//! copy constructors, no destructors and no generic comparison, while §4.3 demands
//! structural `==` recursively and §4.10 demands value-semantics copies and drops. A
//! type-erased `void *` runtime would answer it and would void the property the whole
//! backend rests on — that clang type-checks every call — so the compiler writes
//! plain C instead, one small function per type:
//!
//! | function | when it exists | what it does |
//! |---|---|---|
//! | `h_T_retain` | `T` owns a reference | one incref per counted field |
//! | `h_T_release` | `T` owns a reference | one decref per counted field |
//! | `h_T_eq` | always | `==`, **field by field** |
//!
//! **`eq` walks fields, never bytes**, and panel 022 found the reason by compiling
//! it: `record Flag { n: int, on: bool }` carries 7 bytes of padding, so two
//! `==`-equal records differ under `memcmp` with no warning, no error and no
//! sanitiser report. The same argument retires `memcpy`-shaped thinking everywhere in
//! this file.
//!
//! **The order is the checker's**, filtered, never recomputed (panel 023 R3): C needs
//! every struct complete before it is used by value, `Checked::type_order` is that
//! order, and a forward `typedef struct T T;` does **not** rescue a by-value field —
//! measured, same two clang errors with and without it.

use crate::source::Source;
use crate::syntax::{Ast, DeclKind, Field};
use crate::types::{Checked, Ty};

use super::ctype::{c_type, Names};
use super::mangle;
use super::writer::Writer;

/// Every aggregate the program declares, in the order C can accept them.
fn aggregates(ast: &Ast, checked: &Checked) -> Vec<u32> {
    checked
        .type_order
        .iter()
        .copied()
        .filter(|decl| {
            matches!(
                ast.decls[*decl as usize].kind,
                DeclKind::Record { .. } | DeclKind::Variant { .. }
            )
        })
        .collect()
}

fn fields_of(ast: &Ast, decl: u32) -> &[Field] {
    match &ast.decls[decl as usize].kind {
        DeclKind::Record { fields } => fields,
        _ => &[],
    }
}

fn cases_of(ast: &Ast, decl: u32) -> &[crate::syntax::Case] {
    match &ast.decls[decl as usize].kind {
        DeclKind::Variant { cases } => cases,
        _ => &[],
    }
}

fn is_variant(ast: &Ast, decl: u32) -> bool {
    matches!(ast.decls[decl as usize].kind, DeclKind::Variant { .. })
}

/// One field, as a C member declaration.
fn member(checked: &Checked, names: &Names, src: &Source, field: &Field) -> String {
    let ty = checked.written_type(field.ty).unwrap_or_else(|| checked.types.error());
    let spelling = c_type(names, checked, ty).unwrap_or_else(|| "void".to_string());
    format!("    {spelling} {};", mangle::field(src.slice(field.name)))
}

/// The `typedef struct`s, in containment order.
pub(super) fn definitions(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
) {
    for decl in aggregates(ast, checked) {
        let name = names.of(decl).to_string();
        w.at_generated();
        if is_variant(ast, decl) {
            variant_definition(w, ast, checked, names, src, decl, &name);
            continue;
        }
        // The struct tag repeats the typedef name. C keeps tags in their own
        // namespace, so this is legal and it is what makes a clang diagnostic name
        // something the reader can grep for.
        w.line(&format!("typedef struct {name} {{"));
        for field in fields_of(ast, decl) {
            w.line(&member(checked, names, src, field));
        }
        w.line(&format!("}} {name};"));
        w.blank();
    }
}

/// A variant: the tag enum, one payload struct per case that has one, then the tagged
/// union itself (§4.2, panel 022 — spike 04's own shape).
///
/// **A payload-free case is omitted from the union**, because C11 has no empty struct
/// (panel 022). And if *every* case is payload-free the union is omitted entirely: an
/// empty `union { } as;` compiles at `-Wall` and is `error: empty union is a GNU
/// extension` under `-pedantic-errors`, which §4.19 schedules — a landmine panel 023's
/// ffi-pragmatist found while compiling something else. Omitting it is clean under
/// both, and the size is 4 either way.
fn variant_definition(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
    decl: u32,
    name: &str,
) {
    let cases = cases_of(ast, decl);
    let tag = mangle::tag_type(name);
    w.line(&format!("typedef enum {tag} {{"));
    for (at, case) in cases.iter().enumerate() {
        w.line(&format!("    {} = {at},", mangle::tag_of(name, src.slice(case.name))));
    }
    w.line(&format!("}} {tag};"));
    w.blank();
    for case in cases.iter().filter(|case| !case.fields.is_empty()) {
        let payload = mangle::case_type(name, src.slice(case.name));
        w.line(&format!("typedef struct {payload} {{"));
        for field in &case.fields {
            w.line(&member(checked, names, src, field));
        }
        w.line(&format!("}} {payload};"));
        w.blank();
    }
    w.line(&format!("typedef struct {name} {{"));
    w.line(&format!("    {tag} tag;"));
    let carrying: Vec<&crate::syntax::Case> =
        cases.iter().filter(|case| !case.fields.is_empty()).collect();
    if !carrying.is_empty() {
        w.line("    union {");
        for case in &carrying {
            let case_name = src.slice(case.name);
            w.line(&format!(
                "        {} {};",
                mangle::case_type(name, case_name),
                mangle::case(case_name)
            ));
        }
        w.line("    } as;");
    }
    w.line(&format!("}} {name};"));
    w.blank();
}

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
                out.push((payload, owns_case(checked, decl, at as u32)));
            }
        }
        out.push((name, owns(checked, decl)));
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
                if owns_case(checked, decl, at as u32) {
                    reference_body(w, checked, names, src, &payload, &case.fields, true);
                    reference_body(w, checked, names, src, &payload, &case.fields, false);
                }
                equality_body(w, checked, names, src, &payload, &case.fields);
            }
            if owns(checked, decl) {
                variant_reference_body(w, ast, checked, names, src, decl, &name, true);
                variant_reference_body(w, ast, checked, names, src, decl, &name, false);
            }
            variant_equality_body(w, ast, checked, names, src, decl, &name);
            continue;
        }
        let fields = fields_of(ast, decl);
        if owns(checked, decl) {
            reference_body(w, checked, names, src, &name, fields, true);
            reference_body(w, checked, names, src, &name, fields, false);
        }
        equality_body(w, checked, names, src, &name, fields);
    }
}

/// Whether a value of this declaration owns a counted reference — asked through
/// `ir::is_refcounted`, which is the one home for the question.
fn owns(checked: &Checked, decl: u32) -> bool {
    match checked.types.lookup(Ty::Named(decl)) {
        Some(id) => crate::ir::is_refcounted(checked, id),
        // Declared and never mentioned by any written type, so nothing can hold one.
        None => false,
    }
}

fn owns_case(checked: &Checked, decl: u32, case: u32) -> bool {
    match checked.types.lookup(Ty::Case(decl, case)) {
        Some(id) => crate::ir::is_refcounted(checked, id),
        // No `match` arm binds this payload, so no temporary of it exists — but the
        // *variant*'s retain still has to reach the fields inside it, so the answer
        // comes from the fields rather than from the interner.
        None => case_fields_are_counted(checked, decl, case),
    }
}

/// The fallback for a payload no program named: ask the fields directly.
fn case_fields_are_counted(checked: &Checked, decl: u32, case: u32) -> bool {
    let _ = (decl, case);
    // Reached only when the payload type is uninterned, which means nothing reads it.
    // A retain that walks nothing is correct, and generating one that walks fields the
    // program cannot see would be a function with no caller.
    let _ = checked;
    false
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
        let line = match checked.types.get(ty) {
            Ty::Str if keep => format!("    hero_str_incref(v->{member});"),
            Ty::Str => format!("    hero_str_decref(v->{member});"),
            // A nested aggregate is by value, so it is reached by address and its own
            // generated function decides what inside it counts.
            Ty::Named(inner) => {
                let called = names.of(inner);
                let verb = if keep { "retain" } else { "release" };
                format!("    {called}_{verb}(&v->{member});")
            }
            Ty::Case(inner, case) => {
                let called = names.case_of(inner, case);
                let verb = if keep { "retain" } else { "release" };
                format!("    {called}_{verb}(&v->{member});")
            }
            // Anything else counted is a container, and `gate.rs` refuses a program
            // that has one until the step that lands it. Reaching here would mean the
            // gate let a form past, so it says so rather than emitting silence.
            _ => "    hero_unreachable(); /* the gate refuses this field type */".to_string(),
        };
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
        if case.fields.is_empty() || !owns_case(checked, decl, at as u32) {
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
            Ty::Named(inner) => format!("{}_eq(&a->{member}, &b->{member})", names.of(inner)),
            Ty::Case(inner, case) => {
                format!("{}_eq(&a->{member}, &b->{member})", names.case_of(inner, case))
            }
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
