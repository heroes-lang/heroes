//! Qualified names: `geom.dist2(a: p, b: q)` and `geom.Point(x: 3, y: 4)`.
//!
//! Three tokens with two meanings. `x.f(y)` is UFCS — sugar for `f(x, y)` —
//! and `geom.f(y)` is a name from another module, and the surface syntax is
//! identical. This file is where the two are told apart, and the answer is
//! cheap for one reason: **`use` binds** (panel 031 R3), so the resolver
//! already knows `geom` is a module and does not have to guess.
//!
//! The decision is made **before the receiver is resolved as a value**, which
//! is the whole trick: a module has no type and is in no scope, so anything
//! that types the receiver first reports `geom` as an unknown name and never
//! reaches the point. It is recorded on the receiver as `Ref::Module`, once,
//! because Part 5 erases UFCS in the frontend and a later pass re-deciding it
//! would give the erased call the other meaning (`resolve`'s standing rule).
//!
//! The other half is `elsewhere`, which is what the spec bought instead of a
//! sentence about qualification: an unqualified name that exists in another
//! module is an error carrying the repair, at the call site (panel 031 R5).

use crate::diagnostics::Diagnostic;
use crate::source::{Source, Span};
use crate::syntax::{Ast, DeclKind, ExprId, ExprKind};

use super::{errors, Ref, Resolver};

/// `geom.f(...)` — the receiver is a module this file names.
///
/// Returns whether it handled the expression. Everything it does not handle
/// falls through to UFCS, which is the ordinary meaning of a dot.
pub(super) fn qualified(
    r: &mut Resolver,
    ast: &Ast,
    src: &Source,
    at: ExprId,
    receiver: ExprId,
    called: Span,
) -> bool {
    let Some(module) = module_at(r, ast, src, receiver) else {
        return false;
    };
    r.module_reads.insert((r.module.clone(), module.clone()));
    r.record(receiver, Ref::Module);
    let text = src.slice(called);
    if let Some(decl) = r.out.top_in(&module, text) {
        match &ast.decls[decl as usize].kind {
            DeclKind::Variant { .. } => {
                let diagnostic = errors::variant_in_value_position(text, called);
                r.push_diagnostic(diagnostic);
            }
            _ => r.record(at, Ref::Top(decl)),
        }
        return true;
    }
    let names: Vec<String> = r.out.names_in(&module).map(|(n, _)| n.to_string()).collect();
    let near = r.near_names(text, &names);
    let diagnostic = errors::not_in_module(&module, text, &near, called);
    r.push_diagnostic(diagnostic);
    true
}

/// The module this expression names, if it names one.
///
/// Three conditions and they are all about *this* expression: it is a bare name,
/// no local of that name is in scope, and this file `use`s it. The second is not
/// an ambiguity to resolve — a local named after a module is the shadowing
/// error, reported where the local is bound — it simply means the dot beside it
/// is UFCS.
///
/// One function because three passes ask the same question: a qualified call, a
/// qualified constant, and a write to one.
pub(super) fn module_at(
    r: &Resolver,
    ast: &Ast,
    src: &Source,
    id: ExprId,
) -> Option<String> {
    if !matches!(ast.exprs[id.0 as usize].kind, ExprKind::Name) {
        return None;
    }
    let name = src.slice(ast.exprs[id.0 as usize].span);
    if r.lookup_local(name).is_some() || !r.out.is_used_module(&r.module, name) {
        return None;
    }
    Some(name.to_string())
}

/// The name is real and lives somewhere else. Panel 031 R5's diagnostic, which
/// is what the spec buys instead of a sentence about qualification.
pub(super) fn elsewhere(r: &mut Resolver, text: &str, span: Span) -> Option<Diagnostic> {
    let module = r.out.module_declaring(&r.module, text)?.to_string();
    if module == r.module || module == crate::source::LIBRARY_MODULE {
        return None;
    }
    if r.out.is_used_module(&r.module, &module) {
        r.module_reads.insert((r.module.clone(), module.clone()));
        return Some(errors::needs_qualifying(text, &module, span));
    }
    Some(errors::needs_a_use(text, &module, span))
}
