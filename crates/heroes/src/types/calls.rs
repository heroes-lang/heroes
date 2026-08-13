//! Calls, construction, and UFCS (design.md §4.9, §4.11, §4.12, §4.13, §4.19).
//!
//! Four things arrive here looking identical in the tree, and the callee's
//! *resolution* is what tells them apart — which is why the resolver runs first:
//!
//! | written | what it is |
//! |---------|------------|
//! | `f(x)` where `f` is a declaration | a call |
//! | `Point(x: 1, y: 2)` | record construction — a call with named arguments (§4.9) |
//! | `len(xs)` | a built-in form |
//! | `x.f(y)` | `f(x, y)`, unless `f` is a field holding a function (§4.11) |
//!
//! This file is the **dispatch**: it reads what the callee resolved to and sends
//! the call to whoever answers it. The §11 sweep took the two answers long enough
//! to be their own concern — `ufcs.rs` for the dot form, `apply.rs` for a call to a
//! declared function, which is where generics are inferred.
//!
use crate::resolve::{Ref, Resolved};
use crate::source::{Source, Span};
use crate::syntax::{Arg, Ast, DeclKind, ExprId, ExprKind};

use super::construct::construct_record;

use super::table::Ty;
use super::expect;
use super::apply::user_call;
use super::{builtins, errors, exprs, lower, Checker, TyId};

/// A declaration's type, from its written signature.
pub(super) fn signature_of(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    decl: u32,
) -> TyId {
    let DeclKind::Function(function) = &ast.decls[decl as usize].kind else {
        return checker.error_ty();
    };
    let params: Vec<TyId> = function
        .params
        .iter()
        .map(|p| lower::ty(checker, ast, resolved, p.ty))
        .collect();
    let result = lower::ty(checker, ast, resolved, function.result);
    checker.out.types.func(&params, result)
}


pub(super) fn call(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    at: ExprId,
    callee: ExprId,
    args: &[Arg],
) -> TyId {
    let span = ast.exprs[at.0 as usize].span;
    if matches!(ast.exprs[callee.0 as usize].kind, ExprKind::Name) {
        match resolved.use_at(callee) {
            Ref::Top(decl) => match &ast.decls[decl as usize].kind {
                DeclKind::Function(_) => {
                    // The callee is a value in its own right (§4.13), so it gets
                    // its signature recorded even when it is called directly —
                    // M-ir-lowering's lowering reads `expr_types`, not the tree.
                    let signature = signature_of(checker, ast, resolved, decl);
                    checker.record(callee, signature);
                    return user_call(checker, ast, resolved, src, decl, args, None, span);
                }
                DeclKind::Record { .. } => {
                    return construct_record(checker, ast, resolved, src, decl, args, span)
                }
                DeclKind::Variant { .. } => {
                    let name = src.slice(ast.decls[decl as usize].name).to_string();
                    let diagnostic = errors::variant_not_callable(&name, span);
                    checker.push_diagnostic(diagnostic);
                    return checker.error_ty();
                }
                _ => {}
            },
            Ref::Builtin(index) => {
                return builtin_call(checker, ast, resolved, src, index, args, None, span)
            }
            _ => {}
        }
    }
    // Anything else must be a function *value* (§4.13): a parameter, a local, or
    // a field holding one.
    let callee_ty = exprs::synth(checker, ast, resolved, src, callee);
    indirect_call(checker, ast, resolved, src, callee_ty, args, None, span)
}


pub(super) fn builtin_call(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    index: u32,
    args: &[Arg],
    receiver: Option<TyId>,
    span: Span,
) -> TyId {
    let name = crate::resolve::BUILTINS[index as usize].name;
    let mut types: Vec<TyId> = Vec::new();
    if let Some(receiver_ty) = receiver {
        types.push(receiver_ty);
    }
    for arg in args {
        // The first argument decides what the rest must be, so from the second
        // on they are *checked* rather than synthesised — which is what lets a
        // `.case` or an `ok(…)` be written as a built-in's argument.
        let expected = types
            .first()
            .and_then(|first| builtins::expectation(checker, name, *first, types.len()));
        match expected {
            Some(want) => {
                expect::check(checker, ast, resolved, src, arg.value, want);
                types.push(want);
            }
            None => types.push(exprs::synth(checker, ast, resolved, src, arg.value)),
        }
    }
    // `ok` and `fail` are not in the built-in table: they are ⇐-only and live in
    // `construct.rs`, so reaching them here means they were written where nothing
    // expects a fallible value.
    if name == "ok" || name == "fail" {
        let diagnostic = errors::constructor_needs_context(name, span);
        checker.push_diagnostic(diagnostic);
        return checker.error_ty();
    }
    match builtins::call(checker, ast, src, name, &types, span) {
        Some(ty) => ty,
        None => {
            let diagnostic = errors::builtin_shape(name, types.len(), span);
            checker.push_diagnostic(diagnostic);
            checker.error_ty()
        }
    }
}


pub(super) fn indirect_call(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    callee: TyId,
    args: &[Arg],
    receiver: Option<TyId>,
    span: Span,
) -> TyId {
    if checker.out.types.poisoned(callee) {
        return checker.error_ty();
    }
    let Ty::Func { params, result } = checker.out.types.get(callee) else {
        let got = checker.show(ast, src, callee);
        let diagnostic = errors::not_callable(&got, span);
        checker.push_diagnostic(diagnostic);
        return checker.error_ty();
    };
    let params = checker.out.types.params_of(params);
    let given = args.len() + usize::from(receiver.is_some());
    if given != params.len() {
        let diagnostic = errors::arity("this function", params.len(), given, None, span);
        checker.push_diagnostic(diagnostic);
        return checker.error_ty();
    }
    let offset = usize::from(receiver.is_some());
    for (index, arg) in args.iter().enumerate() {
        expect::check(checker, ast, resolved, src, arg.value, params[index + offset]);
    }
    result
}
