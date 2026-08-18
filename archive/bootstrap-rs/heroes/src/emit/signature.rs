//! A function's C signature: the prototype, and the name a monomorphised instance
//! gets (design.md §3.1, §4.12).
//!
//! Split out of `decls.rs` on 2026-08-12 by author decision, the second cut of two
//! — the first (`externs.rs` + `main.rs`) left the file at 493 lines against
//! CLAUDE.md §11's ~300, and what remained was still three concerns rather than
//! one. This is the middle one, and it is shared: a prototype *is* a signature and
//! a `;`, and a definition is the same signature and a body, so writing it in one
//! place is what keeps a declaration and its definition from disagreeing — which
//! C answers with `conflicting types` and the emitter must never provoke.

use crate::ir::{Function, SlotId, SlotKind};
use crate::source::Source;
use crate::syntax::Ast;
use crate::types::Checked;

use super::Target;
use super::ctype::{c_result, c_type};
use super::typedefs::Names;
use super::decls::emitted;
use super::mangle;
use super::writer::Writer;

pub(super) fn prototype(
    w: &mut Writer,
    function: &Function,
    target: Target,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
    names: &Names,
) {
    if !emitted(function, target) {
        return;
    }
    w.at_generated();
    w.line(&format!("{};", signature(function, ast, checked, src, names)));
}

/// `int64_t h_mod_dist(int64_t h0_a, int64_t *ph1_b)`.
///
/// A mutable parameter is a **pointer** parameter — §4.8's copy-in/copy-out has no
/// other shape in C, because `Op::CopyOut` writes the *caller's* place and the
/// callee cannot otherwise reach it.
/// The C name of one function: its mangled name, plus the hash of what it was
/// instantiated at when it is a monomorphised instance (§4.12, panel 029 R5).
///
/// The rendering that is hashed is the *canonical* one — `render_instance` — so
/// the port reproduces the same symbol from the same public spelling, and the
/// M-selfhost-fixpoint fixpoint does not depend on two implementations interning in the same
/// order.
pub(super) fn instance_name(
    function: &Function,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
) -> String {
    let module = src.component_at(function.span.start);
    if function.kind == crate::ir::FnKind::Test {
        return mangle::test(module, function.decl as usize);
    }
    if function.instance.is_empty() {
        return mangle::function(module, &function.name);
    }
    let rendered = render_instance(ast, checked, src, &function.instance);
    mangle::instance_of(module, &function.name, &rendered)
}

/// The type arguments as one string, which is what the hash is taken over and
/// what the comment above an instance shows. Independent of `TyId`, of the
/// module, and of the file's name.
pub(super) fn render_instance(
    ast: &Ast,
    checked: &Checked,
    src: &Source,
    args: &[crate::types::TyId],
) -> String {
    args.iter()
        .map(|t| crate::types::render_ty(&checked.types, ast, src, *t, &[]))
        .collect::<Vec<String>>()
        .join(", ")
}

pub(super) fn signature(
    function: &Function,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
    names: &Names,
) -> String {
    let name = instance_name(function, ast, checked, src);
    let result = c_result(names, checked, function.result);
    let mut params: Vec<String> = Vec::new();
    for slot in &function.params {
        params.push(param(function, checked, names, *slot));
    }
    let list = if params.is_empty() { "void".to_string() } else { params.join(", ") };
    format!("{result} {name}({list})")
}

fn param(function: &Function, checked: &Checked, names: &Names, slot: SlotId) -> String {
    let index = slot.0;
    let declared = &function.slots[index as usize];
    let ty = c_type(names, checked, declared.ty).unwrap_or_else(|| "void".to_string());
    if matches!(declared.kind, SlotKind::Param { mutable: true }) {
        format!("{ty} *{}", mangle::out_param(index, &declared.name))
    } else {
        format!("{ty} {}", mangle::slot(index, &declared.name))
    }
}
