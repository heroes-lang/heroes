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
use crate::types::{Checked, Ty};

use super::ctype::{c_result, c_type, Names};
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

/// One `typedef` per distinct `T?` (design.md §4.6).
///
/// A tagged union by value, exactly like a variant with two cases — but generated from
/// a *type* rather than a declaration, because no `record` in the source describes it.
/// The tag is a plain `int64_t` rather than an enum: `Op::Tag`'s result is an `int` in
/// the IR and lowering already compares it against `const 0`, so an enum here would be
/// a name the switch could not use.
///
/// `ok` is case 0 and `err` is case 1, which is `ir/inst.rs`'s own numbering, and the
/// comment above each typedef names the Heroes type — the struct is called `optN`
/// because `int?` and `[int]?` sanitise to the same identifier and a collision here is
/// two types sharing one C name.
/// Every type the compiler generates a C declaration for, **in `TyId` order**.
///
/// Two kinds share this pass and must: a `T?` may hold a function
/// (`(function(int) -> int)?`) and a function's signature may mention a `T?`
/// (`(function(int) -> int?)`), so neither kind can be emitted wholesale before
/// the other. Measured, when they were two passes: a function typedef naming an
/// option that had not been declared yet parsed as an implicit-`int` function
/// type, and clang reported `'const' qualifier on function type … has no effect`
/// on a line about something else entirely.
///
/// **`TyId` order IS containment order**, and that is an invariant of the type
/// table rather than a coincidence: the checker interns a composite only after
/// the types it is built from, because it needs their ids to build it. So one
/// ascending walk emits every declaration after everything it names.
pub(super) fn generated(w: &mut Writer, checked: &Checked, names: &Names) {
    let mut wrote = false;
    for index in 0..checked.types.len() {
        let id = crate::types::TyId(index as u32);
        if super::ctype::mentions_generic(checked, id) {
            continue;
        }
        match checked.types.get(id) {
            Ty::Fallible(payload) => {
                option(w, checked, names, names.option_of(id), payload);
                wrote = true;
            }
            // Only the ones the program uses as a type have a name; the rest are
            // signatures the checker interned for declarations nobody takes the
            // address of.
            Ty::Func { params, result } => {
                if let Some(name) = names.func_name(id) {
                    function_type(w, checked, names, &name, params, result);
                    wrote = true;
                }
            }
            _ => {}
        }
    }
    if wrote {
        w.blank();
    }
}

/// `typedef int64_t (*h_m_fn0)(int64_t, int64_t);` — a plain C function pointer,
/// because a Heroes function value IS one (§1.11's founding constraint). That is
/// what lets `qsort`'s comparator and raylib's callbacks be Heroes functions at
/// M7 with no shim; design.md:1768 counts nine of ten ladder cases needing it.
///
/// A zero-parameter function is `(void)` and not `()`: an empty parameter list in
/// C means "unspecified", which turns a wrong-arity call through the pointer from
/// a compile error into undefined behaviour — the one thing this backend's whole
/// "clang type-checks every call" property exists to prevent.
fn function_type(
    w: &mut Writer,
    checked: &Checked,
    names: &Names,
    name: &str,
    params: crate::types::Params,
    result: crate::types::TyId,
) {
    let spelled: Vec<String> = checked
        .types
        .params_of(params)
        .iter()
        .map(|p| c_type(names, checked, *p).unwrap_or_else(|| "void".to_string()))
        .collect();
    let list = if spelled.is_empty() { "void".to_string() } else { spelled.join(", ") };
    w.at_generated();
    w.line(&format!("typedef {} (*{name})({list});", c_result(names, checked, result)));
}

/// One `T?`, as a by-value tagged union. Its error side is the runtime's own
/// record, since §4.6 fixes that shape.
fn option(
    w: &mut Writer,
    checked: &Checked,
    names: &Names,
    name: &str,
    payload: crate::types::TyId,
) {
    let spelling = c_type(names, checked, payload);
    w.at_generated();
    w.line(&format!("typedef struct {name} {{"));
    w.line("    int64_t tag;");
    w.line("    union {");
    // A unit payload has no declaration (`ctype.rs`'s unit rule), so `()?` carries
    // nothing on its ok side and the union holds the failure alone.
    if let Some(text) = &spelling {
        w.line(&format!("        {text} ok;"));
    }
    w.line("        HeroFailure err;");
    w.line("    } as;");
    w.line(&format!("}} {name};"));
    w.blank();
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
