//! The per-type functions C cannot write for itself: which ones exist, and in what
//! order (design.md §4.3, §4.10, §4.20; panel 022).
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
//! | `h_T_hash` | always | FNV-1a over the field hashes, never null |
//!
//! **This file is the order, not the C.** Each function's body lives with the invariant
//! it serves — `counted.rs` for the two reference walks and the ownership question that
//! guards them, `structural.rs` for `eq` and `hash`, which must agree, and
//! `descriptors.rs` for the `HeroDesc` objects that let the runtime reach any of them.
//! What is left here is what nothing else can own: the prototypes, the traversal order,
//! and the four functions of a `T?`.
//!
//! A variant contributes its **case payloads first** and then itself, in that order and
//! for the same reason the typedefs are ordered: the outer function calls the inner
//! ones, and a case with fields *is* a small record (§4.2), so it gets exactly what a
//! record gets.

use crate::source::Source;
use crate::syntax::Ast;
use crate::types::Checked;

use super::counted::{owns, owns_case, reference_body, reference_line, variant_reference_body};
use super::ctype::c_type;
use super::typedefs::Names;
use super::structural::{equality_body, hash_body, variant_equality_body, variant_hash_body};
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
