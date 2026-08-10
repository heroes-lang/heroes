//! Aggregates as C: what a `record` and a `variant` look like (design.md §4.2, §4.10,
//! §4.20; panel 022).
//!
//! This file is the **shape**; `perfn.rs` is what the compiler writes *for* that shape.
//! The split is where the reading changes: here a declaration becomes a struct, an
//! enum and a union, and there it becomes four small functions C has no way to write
//! for itself.
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
use crate::types::Checked;

use super::ctype::{c_type, Names};
use super::mangle;
use super::writer::Writer;

/// Every aggregate the program declares, in the order C can accept them.
pub(super) fn aggregates(ast: &Ast, checked: &Checked) -> Vec<u32> {
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

pub(super) fn fields_of(ast: &Ast, decl: u32) -> &[Field] {
    match &ast.decls[decl as usize].kind {
        DeclKind::Record { fields } => fields,
        _ => &[],
    }
}

pub(super) fn cases_of(ast: &Ast, decl: u32) -> &[crate::syntax::Case] {
    match &ast.decls[decl as usize].kind {
        DeclKind::Variant { cases } => cases,
        _ => &[],
    }
}

pub(super) fn is_variant(ast: &Ast, decl: u32) -> bool {
    matches!(ast.decls[decl as usize].kind, DeclKind::Variant { .. })
}

/// One field, as a C member declaration.
pub(super) fn member(checked: &Checked, names: &Names, src: &Source, field: &Field) -> String {
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
