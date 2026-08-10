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
///
/// Records only at this step; a variant is still refused by `gate.rs`, so a
/// declaration that is one contributes nothing here rather than being half-emitted.
fn records(ast: &Ast, checked: &Checked) -> Vec<u32> {
    checked
        .type_order
        .iter()
        .copied()
        .filter(|decl| matches!(ast.decls[*decl as usize].kind, DeclKind::Record { .. }))
        .collect()
}

fn fields_of(ast: &Ast, decl: u32) -> &[Field] {
    match &ast.decls[decl as usize].kind {
        DeclKind::Record { fields } => fields,
        _ => &[],
    }
}

/// The `typedef struct`s, in containment order.
pub(super) fn definitions(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
) {
    for decl in records(ast, checked) {
        let name = names.of(decl);
        w.at_generated();
        // The struct tag repeats the typedef name. C keeps tags in their own
        // namespace, so this is legal and it is what makes a clang diagnostic name
        // something the reader can grep for.
        w.line(&format!("typedef struct {name} {{"));
        for field in fields_of(ast, decl) {
            let ty = checked.written_type(field.ty).unwrap_or_else(|| checked.types.error());
            let spelling = c_type(names, checked, ty).unwrap_or_else(|| "void".to_string());
            w.line(&format!("    {spelling} {};", mangle::field(src.slice(field.name))));
        }
        w.line(&format!("}} {name};"));
        w.blank();
    }
}

/// Prototypes for every per-type function, before any body.
///
/// All of them first, because `h_T_eq` on a nested record calls `h_Inner_eq`: the
/// typedefs are ordered so the *types* are complete in time, and this makes the
/// *functions* visible in time without depending on that order twice.
pub(super) fn prototypes(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
) {
    let _ = src;
    let mut any = false;
    for decl in records(ast, checked) {
        let name = names.of(decl);
        w.at_generated();
        if owns(ast, checked, decl) {
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

/// The bodies.
pub(super) fn bodies(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
) {
    for decl in records(ast, checked) {
        let name = names.of(decl).to_string();
        if owns(ast, checked, decl) {
            reference_body(w, ast, checked, names, src, decl, &name, Direction::Retain);
            reference_body(w, ast, checked, names, src, decl, &name, Direction::Release);
        }
        equality_body(w, ast, checked, names, src, decl, &name);
    }
}

/// Whether a value of this declaration owns a counted reference — asked through
/// `ir::is_refcounted`, which is the one home for the question.
fn owns(ast: &Ast, checked: &Checked, decl: u32) -> bool {
    let _ = ast;
    match checked.types.lookup(Ty::Named(decl)) {
        Some(id) => crate::ir::is_refcounted(checked, id),
        // Declared and never mentioned by any written type, so nothing can hold one.
        None => false,
    }
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum Direction {
    Retain,
    Release,
}

fn reference_body(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
    decl: u32,
    name: &str,
    how: Direction,
) {
    w.at_generated();
    match how {
        Direction::Retain => w.line(&format!("void {name}_retain(const {name} *v) {{")),
        Direction::Release => w.line(&format!("void {name}_release({name} *v) {{")),
    }
    for field in fields_of(ast, decl) {
        let ty = checked.written_type(field.ty).unwrap_or_else(|| checked.types.error());
        if !crate::ir::is_refcounted(checked, ty) {
            continue;
        }
        let member = mangle::field(src.slice(field.name));
        let line = match checked.types.get(ty) {
            Ty::Str => match how {
                Direction::Retain => format!("    hero_str_incref(v->{member});"),
                Direction::Release => format!("    hero_str_decref(v->{member});"),
            },
            // A nested aggregate is by value, so it is reached by address and its own
            // generated function decides what inside it counts.
            Ty::Named(inner) | Ty::Case(inner, _) => {
                let called = names.of(inner);
                match how {
                    Direction::Retain => format!("    {called}_retain(&v->{member});"),
                    Direction::Release => format!("    {called}_release(&v->{member});"),
                }
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

/// Structural `==`, field by field (§4.3).
fn equality_body(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
    decl: u32,
    name: &str,
) {
    w.at_generated();
    w.line(&format!("bool {name}_eq(const {name} *a, const {name} *b) {{"));
    let fields = fields_of(ast, decl);
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
            Ty::Named(inner) | Ty::Case(inner, _) => {
                format!("{}_eq(&a->{member}, &b->{member})", names.of(inner))
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
