//! One declaration at a time: what becomes a function, and what does not
//! (design.md §4.2 the four entities, §4.8 `@` parameters, §4.12 generics,
//! §4.18 tests, §4.19 `extern`).
//!
//! Four of the five declaration kinds produce a function in the IR, and the
//! differences between them are exactly the differences the checker already
//! named (`types/decls.rs`):
//!
//! - a **`function`**'s body runs for effect and hands its value back through
//!   `return`;
//! - a **`constant`**'s body *is* its value, so lowering returns the last
//!   statement's value — a zero-argument function, because there are no mutable
//!   globals to initialise and a call needs no new machinery;
//! - an **`extern`** has no body and no blocks; what it contributes is a
//!   signature and a linkage the emitter must not mangle;
//! - a **`test`** is a zero-argument function entered only by `heroes test`
//!   (§4.18, M-generics-library). Lowering it here rather than at M-generics-library is deliberate: the
//!   milestone's own witness is design.md's appendix, which holds six `test`
//!   blocks and about twenty-five `assert`s, so a lowering that skipped them
//!   would have nothing to prove itself against (panel 019 point 6).
//!
//! `record` and `variant` declare *types*. They produce no function; M-value-aggregates's
//! descriptor pass is what turns them into `copy`/`drop`/`eq`/`hash`.

use crate::resolve::Resolved;
use crate::source::Source;
use crate::syntax::{Ast, DeclKind, Function as AstFunction};
use crate::types::Checked;

use super::build::Lowering;
use super::inst::{Op, Term};
use super::{stmts, FnKind};

pub(super) fn declaration(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    index: u32,
) {
    let decl = &ast.decls[index as usize];
    let name = src.slice(decl.name).to_string();
    match &decl.kind {
        DeclKind::Function(function) => {
            self_function(b, ast, resolved, checked, src, index, name, function)
        }
        DeclKind::Constant { body: Some(body), .. } => {
            // The declared type is what the checker already checked the body
            // against, so it is the type of the value the body produces.
            let result = stmts::block_value_type(ast, checked, body);
            b.begin(name, index, FnKind::Constant, result, Vec::new(), decl.span);
            b.entry();
            let value = stmts::block(b, ast, resolved, checked, src, body, true);
            if !b.is_terminated() {
                b.terminate(Term::Return(value));
            }
            b.end();
        }
        // An `extern constant`: no body, so no blocks — the same shape an
        // `extern function` has, and for the same reason. The result type comes
        // from the checker's table rather than from a body that does not exist,
        // and the emitter reads the C name off `Ast::decls` (§4.19, panel 038).
        DeclKind::Constant { body: None, .. } => {
            let result = checked.result_type(index).unwrap_or_else(|| checked.types.unit());
            b.begin(name, index, FnKind::Constant, result, Vec::new(), decl.span);
            b.end();
        }
        DeclKind::Test { body } => {
            let unit = checked.types.unit();
            // The name is the title, quotes included: it is a string literal, not
            // an identifier, and the dump prints what the author wrote.
            b.begin(name, index, FnKind::Test, unit, Vec::new(), decl.span);
            b.entry();
            stmts::block(b, ast, resolved, checked, src, body, false);
            if !b.is_terminated() {
                b.terminate(Term::Return(None));
            }
            b.end();
        }
        DeclKind::Record { .. } | DeclKind::Variant { .. } => {}
    }
}

fn self_function(
    b: &mut Lowering,
    ast: &Ast,
    resolved: &Resolved,
    checked: &Checked,
    src: &Source,
    index: u32,
    name: String,
    function: &AstFunction,
) {
    let result = checked.result_type(index).unwrap_or_else(|| checked.types.unit());
    let generics: Vec<String> =
        function.generics.iter().map(|span| src.slice(*span).to_string()).collect();
    let kind = if function.is_extern { FnKind::Extern } else { FnKind::Function };
    b.begin(name, index, kind, result, generics, ast.decls[index as usize].span);
    // Parameters are slots before anything else happens, so a body's first
    // instruction can already load one.
    for param in &function.params {
        // The **written** type, not the local's: an `extern` has no locals (§4.19 —
        // no body to use them in) and its parameter types are exactly what the
        // emitter writes a C prototype from.
        let ty = checked.written_type(param.ty).unwrap_or_else(|| checked.types.error());
        let slot = b.param(src.slice(param.name).to_string(), ty, param.mutable);
        if let Some(local) = b.local_at(param.name) {
            b.bind_local(local, slot);
        }
    }
    match &function.body {
        None => {
            // An `extern` keeps its signature and gets no blocks (§4.19): the
            // implementation is in C, and clang type-checks the call against the
            // real header.
            b.end();
        }
        Some(body) => {
            b.entry();
            stmts::block(b, ast, resolved, checked, src, body, false);
            close(b, checked);
            b.end();
        }
    }
}

/// The fall-through edge out of a function body.
///
/// A body that runs off its end returns nothing — which is right for a `()`
/// result and is a *program* error otherwise. The frontend does not reject it
/// today (there is no reachability analysis), so lowering does not invent a
/// value: it emits the copy-out chain and `return`, and CLAUDE.md §7's
/// `-Werror=return-type` is the net that catches the non-unit case at M-scalars-run. That
/// division is queued as a diagnostic class of its own.
fn close(b: &mut Lowering, checked: &Checked) {
    if b.is_terminated() {
        return;
    }
    copy_out(b, checked);
    b.terminate(Term::Return(None));
}

/// §4.8's second half, on one exit edge: every `@` parameter is written back
/// before control leaves. "Copy-out happens always" — including on early
/// `return` and on `?` — so this is called from three places, and the dump shows
/// the instructions rather than leaving them to the emitter's good intentions.
pub(super) fn copy_out(b: &mut Lowering, checked: &Checked) {
    let unit = checked.types.unit();
    for param in b.mutable_params() {
        let span = b.declaration_span();
        b.emit_void(Op::CopyOut { param }, unit, span);
    }
}
