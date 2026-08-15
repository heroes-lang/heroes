//! A function's C body: the prologue, the blocks, and what is left out of them
//! (design.md §3.1, panel 020).
//!
//! Split out of `decls.rs` on 2026-08-12 by author decision, the second cut of two.
//!
//! **The prologue exists because `goto` may not jump over an initialisation.**
//! Every slot and every temporary is declared at the top of the function, and
//! nothing is given an initial value: a Heroes binding is always initialised (§4.4)
//! and a synthetic join slot is written on every arm, so a C local that clang
//! thinks may be read uninitialised is a *lowering* bug — which is exactly what
//! `-Werror=uninitialized` is in the flag set to catch. Zero-initialising them
//! would hide it.
//!
//! **A block nothing jumps to is omitted entirely**, not merely unlabelled, because
//! C is physical and an unlabelled block would fall through from the one above it.
//! A *correct* Heroes program produces one: an `if` whose both arms `return` leaves
//! an unreachable join block, and emitting its label costs `-Wunused-label` on
//! every build — which would then have to be switched off, taking the emitter's own
//! bugs with it.

use crate::ir::{is_refcounted, Function, Program, SlotId, SlotKind};
use crate::source::Source;
use crate::syntax::Ast;
use crate::types::Checked;

use super::Target;
use super::ctype::{c_type, is_unit};
use super::typedefs::Names;
use super::decls::emitted;
use super::externs;
use super::mangle;
use super::signature::{render_instance, signature};
use super::writer::Writer;
use super::{inst, term};

#[allow(clippy::too_many_arguments)] // PORT-DEBT: one emitter call site
pub(super) fn definition(
    w: &mut Writer,
    program: &Program,
    function: &Function,
    target: Target,
    ast: &Ast,
    checked: &Checked,
    names: &Names,
    src: &Source,
) {
    if !emitted(function, target) {
        return;
    }
    w.blank();
    // The signature is pointed at the author's declaration: a clang error about a
    // parameter type has to land on the line that wrote it.
    super::writer::at_span(w, src, function.span.start);
    // The instance's types, above it, because the hash in the name does not carry
    // them and a reader of `--emit-c` needs them (panel 029 R5).
    if !function.instance.is_empty() {
        w.line(&format!(
            "/* {}<{}> */",
            function.name,
            render_instance(ast, checked, src, &function.instance)
        ));
    }
    // **An `extern constant` has no blocks, so it takes none of the machinery
    // below.** The prologue, the entry `goto bb0` and the label walk all assume a
    // body; a blockless function through that path emits a jump to a label that is
    // never printed. What it needs instead is one line — the C name, read where
    // the preprocessor can see it (§4.19, panel 038).
    if externs::is_extern_constant(ast, function) {
        let name = src.slice(ast.decls[function.decl as usize].name);
        w.line(&format!("{} {{", signature(function, ast, checked, src, names)));
        w.at_generated();
        w.line(&format!("    return {name};"));
        w.line("}");
        return;
    }
    w.line(&format!("{} {{", signature(function, ast, checked, src, names)));
    w.at_generated();
    let types = super::aggregate::Types { ast, checked, names, src };
    let live = reachable(function);
    let read = super::unread::emitted_reads(function, checked);
    prologue(w, function, checked, names, &live, &read);
    w.line(&format!("    goto {};", mangle::block(0)));
    for (index, block) in function.blocks.iter().enumerate() {
        if !live[index] {
            continue;
        }
        w.line(&format!("{}:", mangle::block(index)));
        for one in &block.insts {
            inst::emit(w, program, &types, function, one, &read);
        }
        term::emit(w, function, checked, &block.term);
    }
    w.at_generated();
    w.line("}");
}


/// Which blocks are reachable. A block nothing jumps to is **omitted entirely** —
/// not merely unlabelled, because C is physical and an unlabelled block would fall
/// through from the one above it.
///
/// The IR already carries the answer (`Block::preds`), so this is a lookup rather
/// than an analysis. It exists because a *correct* Heroes program produces one: an
/// `if` whose both arms `return` leaves an unreachable join block, and emitting its
/// label costs `-Wunused-label` on every build — which would then have to be
/// switched off, taking the emitter's own bugs with it.
fn reachable(function: &Function) -> Vec<bool> {
    let mut live = vec![false; function.blocks.len()];
    for (index, block) in function.blocks.iter().enumerate() {
        live[index] = index == 0 || !block.preds.is_empty();
    }
    live
}

fn prologue(
    w: &mut Writer,
    function: &Function,
    checked: &Checked,
    names: &Names,
    live: &[bool],
    read: &std::collections::BTreeSet<u32>,
) {
    for (index, slot) in function.slots.iter().enumerate() {
        // A parameter is already declared by the signature. A mutable one is
        // declared here instead: the pointer is the parameter, and the slot is the
        // local copy §4.8 says the callee works on.
        let is_param = function.params.contains(&SlotId(index as u32));
        let mutable = matches!(slot.kind, SlotKind::Param { mutable: true });
        if is_param && !mutable {
            continue;
        }
        if let Some(ty) = c_type(names, checked, slot.ty) {
            // **The one exception to "nothing is initialised here"** (panel 021 R3).
            // A refcounted slot is zeroed so that the exit sweep is unconditional:
            // `decref` of the null non-value is a no-op, and the alternative is a
            // liveness pass. clang's ARC specification licenses exactly this for
            // `__strong` locals — "may be properly initialized by filling it with
            // the representation of a null pointer" — and rustc's `ElaborateDrops`
            // does the alternative and then optimises it into the same thing.
            //
            // The cost is real and stated: `-Werror=uninitialized` stops covering
            // these, and without the zero clang would have reported a *compile
            // error*. What replaces it is `ptr == NULL` being the one non-value that
            // every runtime entry point rejects loudly, plus the verifier's own
            // store-before-load check.
            let initialiser = if is_refcounted(checked, slot.ty) { " = {0}" } else { "" };
            w.line(&format!(
                "    {ty} {}{initialiser};",
                mangle::slot(index as u32, &slot.name)
            ));
        }
    }
    for (index, ty) in function.values.iter().enumerate() {
        if is_unit(checked, *ty) || !super::unread::assigned(function, live, index as u32) {
            continue;
        }
        // A discarded result is never assigned either (`inst::emit`), so declaring
        // it here would be the unused variable this rule exists to stop. The two
        // decisions have to agree, and they agree by reading the same set.
        if super::unread::discarded(function, live, index as u32, read) {
            continue;
        }
        if let Some(name) = c_type(names, checked, *ty) {
            let initialiser = if is_refcounted(checked, *ty) { " = {0}" } else { "" };
            w.line(&format!("    {name} {}{initialiser};", mangle::value(index as u32)));
        }
    }
    // Copy-in, after every declaration: §4.8's first half.
    for slot in &function.params {
        let declared = &function.slots[slot.0 as usize];
        if !matches!(declared.kind, SlotKind::Param { mutable: true }) {
            continue;
        }
        w.line(&format!(
            "    {} = *{};",
            mangle::slot(slot.0, &declared.name),
            mangle::out_param(slot.0, &declared.name)
        ));
    }
}
