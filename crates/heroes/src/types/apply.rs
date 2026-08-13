//! A call to a declared function, and the one place generics are inferred
//! (design.md §4.12, §4.9, §4.5).
//!
//! Split out of `calls.rs` by the §11 sweep, and it is the file the §4.12 paragraph
//! belongs to:
//!
//! **Generics are inferred here and nowhere else** (§4.12: on functions only, no
//! constraints, never written at the call site). The inference is one pass over the
//! parameters: the first argument that meets a type parameter binds it, later ones
//! are checked against the binding. No unification variables, no constraints to
//! solve — which is what keeps §4.5's promise that errors stay local.

use crate::resolve::Resolved;
use crate::source::{Source, Span};
use crate::syntax::{Arg, Ast, DeclKind};

use super::generics::{bind, substitute};
use super::{errors, expect, exprs, lower, Checker, TyId};

/// A call to a declared function, with `receiver` prepended when it was written
/// as `x.f(y)`.
pub(super) fn user_call(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    decl: u32,
    args: &[Arg],
    receiver: Option<TyId>,
    span: Span,
) -> TyId {
    let DeclKind::Function(function) = &ast.decls[decl as usize].kind else {
        return checker.error_ty();
    };
    let name = src.slice(ast.decls[decl as usize].name).to_string();
    let arity = function.params.len();
    let given = args.len() + usize::from(receiver.is_some());
    if given != arity {
        let signature = crate::printer::render_signature(ast, src, decl);
        let at = src.elsewhere(span.start, ast.decls[decl as usize].name.start);
        let diagnostic = errors::arity(&name, arity, given, Some((signature, at)), span);
        checker.push_diagnostic(diagnostic);
        return checker.error_ty();
    }
    let generics = function.generics.len();
    let mut bindings: Vec<Option<TyId>> = vec![None; generics];
    let params: Vec<TyId> = function
        .params
        .iter()
        .map(|p| lower::ty(checker, ast, resolved, p.ty))
        .collect();
    let mutable: Vec<bool> = function.params.iter().map(|p| p.mutable).collect();
    let result = lower::ty(checker, ast, resolved, function.result);

    // §4.9's same-typed-argument rule: when two parameters share a type, the
    // call site must name them. This is the rule that spends tokens exactly where
    // argument inversion happens — `save(user.name, user.id)` type-checks
    // perfectly and does the wrong thing, and it is the mistake metric 3's
    // `swap-args` operator is built to produce.
    //
    // The receiver of a UFCS call is exempt: `p.copy_to(other)` names nothing for
    // the first argument because the dot *is* its position (§4.11).
    let ambiguous: Vec<usize> = params
        .iter()
        .enumerate()
        .filter(|(index, ty)| {
            params.iter().enumerate().any(|(other, candidate)| other != *index && candidate == *ty)
        })
        .map(|(index, _)| index)
        .collect();

    let mut offset = 0;
    if let Some(receiver_ty) = receiver {
        bind(checker, ast, src, params[0], receiver_ty, &mut bindings, span);
        offset = 1;
    }
    for (index, arg) in args.iter().enumerate() {
        let param = params[index + offset];
        let position = index + offset;
        if ambiguous.contains(&position) {
            let wanted = src.slice(function.params[position].name).to_string();
            match arg.name {
                Some(label) if src.slice(label) == wanted.as_str() => {}
                Some(label) => {
                    let diagnostic =
                        errors::wrong_label(&name, src.slice(label), &wanted, label);
                    checker.push_diagnostic(diagnostic);
                }
                None => {
                    let at = ast.exprs[arg.value.0 as usize].span;
                    // Rendered with the *callee's* type-parameter names: at a
                    // call site the enclosing function's letters are the wrong
                    // dictionary, and `#0` is nobody's type.
                    let callee_generics: Vec<String> = function
                        .generics
                        .iter()
                        .map(|span| src.slice(*span).to_string())
                        .collect();
                    let shared = super::render_ty(
                        &checker.out.types,
                        ast,
                        src,
                        param,
                        &callee_generics,
                    );
                    let diagnostic = errors::needs_label(&name, &wanted, &shared, at);
                    checker.push_diagnostic(diagnostic);
                }
            }
        } else if let Some(label) = arg.name {
            // A label where the signature does not need one still has to be the
            // right label: a wrong one is a wrong argument with a comment on it.
            let wanted = src.slice(function.params[position].name);
            if src.slice(label) != wanted {
                let diagnostic = errors::wrong_label(&name, src.slice(label), wanted, label);
                checker.push_diagnostic(diagnostic);
            }
        }
        // §4.8: the `@` marker is repeated at the call site, and a marked
        // argument must meet a marked parameter.
        if arg.mutable != mutable[index + offset] {
            let at = ast.exprs[arg.value.0 as usize].span;
            let diagnostic = errors::marker_mismatch(&name, mutable[index + offset], at);
            checker.push_diagnostic(diagnostic);
        }
        if generics == 0 {
            expect::check(checker, ast, resolved, src, arg.value, param);
        } else {
            let got = exprs::synth(checker, ast, resolved, src, arg.value);
            let at = ast.exprs[arg.value.0 as usize].span;
            bind(checker, ast, src, param, got, &mut bindings, at);
        }
    }
    // **Recorded, not dropped.** The bindings were computed to type this call and
    // used to be discarded here; monomorphisation needs exactly them, and
    // recomputing them at IR level would be a second answer to one question
    // (panel 029 R2). Only a generic call has any, so a monomorphic program adds
    // no entries at all.
    if generics > 0 {
        let resolved_args: Vec<TyId> =
            bindings.iter().map(|b| b.unwrap_or_else(|| checker.error_ty())).collect();
        checker.out.instantiations.insert(span.start, resolved_args);
    }
    substitute(checker, result, &bindings)
}
