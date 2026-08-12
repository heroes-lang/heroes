//! **The order** every typedef comes out in — one order over two kinds
//! (design.md §4.2, §4.6, §4.20; panel 023 R3).
//!
//! `types.rs` is the *shape* a declaration takes in C; this file is the sequence.
//! They are separate concerns and they became separate files the day the sequence
//! stopped being "the declared ones, then the ones the emitter invents".
//!
//! **C needs every struct complete before it is used by value**, and a forward
//! `typedef struct T T;` does not rescue a by-value field — measured, the same two
//! clang errors with and without it. So the sequence is a real containment order,
//! and it has to span both kinds at once, because each can contain the other:
//! `record Box { v: i64? }` needs the option first and `function grab() -> P?`
//! needs the record first. Both are ordinary programs; the two-pass version
//! answered the first with `error: unknown type name 'h_M_0opt0'` at exit 2.

use crate::source::Source;
use crate::syntax::Ast;
use crate::types::{Checked, Ty, TyId};

use super::ctype::Names;
use super::types::{aggregates, cases_of, declared, fields_of, function_type, option};
use super::writer::Writer;

/// One typedef, identified by where it comes from.
///
/// Two kinds and **one order**, which is the whole point. A declared aggregate
/// may hold a `T?` or a function value, and a `T?` may hold a declared aggregate:
/// `record Box { v: i64? }` needs the option first, `function grab() -> P?` needs
/// the record first, and both are ordinary programs. Emitting the declared ones
/// wholesale and then the generated ones — which is what this file did until
/// 2026-08-12 — answers the first with `error: unknown type name 'h_M_0opt0'`,
/// exit 2.
#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
enum Item {
    Declared(u32),
    Generated(TyId),
}

/// What `item` must be declared **after**.
///
/// By-value containment only, which is exactly what C needs a complete type for.
/// A cycle cannot occur: a record holding itself by value is `error[no_size]` in
/// the checker (panel 023), so the graph is a DAG and the `open` guard below is
/// belt to that braces rather than a real case.
fn needs(ast: &Ast, checked: &Checked, names: &Names, item: Item) -> Vec<Item> {
    let mut written = Vec::new();
    match item {
        Item::Declared(decl) => {
            for field in fields_of(ast, decl) {
                written.push(checked.written_type(field.ty));
            }
            for case in cases_of(ast, decl) {
                for field in &case.fields {
                    written.push(checked.written_type(field.ty));
                }
            }
        }
        Item::Generated(ty) => match checked.types.get(ty) {
            Ty::Fallible(payload) => written.push(Some(payload)),
            Ty::Func { params, result } => {
                for param in checked.types.params_of(params).to_vec() {
                    written.push(Some(param));
                }
                written.push(Some(result));
            }
            _ => {}
        },
    }
    let mut out = Vec::new();
    for ty in written.into_iter().flatten() {
        match checked.types.get(ty) {
            Ty::Named(inner) => out.push(Item::Declared(inner)),
            Ty::Case(inner, _) => out.push(Item::Declared(inner)),
            Ty::Fallible(_) => out.push(Item::Generated(ty)),
            Ty::Func { .. } if names.func_name(ty).is_some() => out.push(Item::Generated(ty)),
            _ => {}
        }
    }
    out
}

/// Every typedef the program needs, each after everything it contains by value.
///
/// The seed order is the two old orders concatenated — `aggregates` (which is
/// `Checked::type_order`, panel 023's walk) then ascending `TyId` — so a program
/// with no cross-kind containment emits byte-identically to before, and the
/// double-emit determinism test still covers the result.
pub(super) fn definitions(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
) {
    let mut seeds: Vec<Item> = aggregates(ast, checked).into_iter().map(Item::Declared).collect();
    for index in 0..checked.types.len() {
        let id = TyId(index as u32);
        if super::ctype::mentions_generic(checked, id) {
            continue;
        }
        match checked.types.get(id) {
            Ty::Fallible(_) => seeds.push(Item::Generated(id)),
            Ty::Func { .. } if names.func_name(id).is_some() => seeds.push(Item::Generated(id)),
            _ => {}
        }
    }
    let mut done: std::collections::BTreeSet<Item> = std::collections::BTreeSet::new();
    let mut open: std::collections::BTreeSet<Item> = std::collections::BTreeSet::new();
    // A struct typedef ends with its own blank line and a function-pointer
    // typedef does not, so the group owes one exactly when the last thing written
    // was the latter. Tracked rather than always emitted: `wrote` used to mean
    // "this pass produced anything", and a pass that now also writes the structs
    // would have doubled every aggregate-only program's separator.
    let mut owed_blank = false;
    for seed in seeds {
        emit_item(w, ast, checked, names, src, seed, &mut done, &mut open, &mut owed_blank);
    }
    if owed_blank {
        w.blank();
    }
}

/// One item, after everything it needs. Post-order, so a dependency reached from
/// two places is written once.
#[allow(clippy::too_many_arguments)]
fn emit_item(
    w: &mut Writer,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
    item: Item,
    done: &mut std::collections::BTreeSet<Item>,
    open: &mut std::collections::BTreeSet<Item>,
    owed_blank: &mut bool,
) {
    if done.contains(&item) || !open.insert(item) {
        return;
    }
    for need in needs(ast, checked, names, item) {
        emit_item(w, ast, checked, names, src, need, done, open, owed_blank);
    }
    open.remove(&item);
    done.insert(item);
    match item {
        Item::Declared(decl) => {
            declared(w, ast, checked, names, src, decl);
            *owed_blank = false;
        }
        Item::Generated(ty) => match checked.types.get(ty) {
            Ty::Fallible(payload) => {
                option(w, checked, names, names.option_of(ty), payload);
                *owed_blank = false;
            }
            Ty::Func { params, result } => {
                let name = names.func_name(ty).expect("seeded only when it has a name");
                function_type(w, checked, names, &name, params, result);
                *owed_blank = true;
            }
            _ => {}
        },
    }
}

