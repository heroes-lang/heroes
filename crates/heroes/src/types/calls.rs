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
//! **Generics are inferred here and nowhere else** (§4.12: on functions only, no
//! constraints, never written at the call site). The inference is one pass over
//! the parameters: the first argument that meets a type parameter binds it, later
//! ones are checked against the binding. No unification variables, no
//! constraints to solve — which is what keeps §4.5's promise that errors stay
//! local.

use crate::resolve::{Ref, Resolved};
use crate::source::{Source, Span};
use crate::syntax::{Arg, Ast, DeclKind, ExprId, ExprKind};

use super::construct::{construct_record, field_of_function_type};
use super::generics::{bind, substitute};
use super::table::Ty;
use super::expect;
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
                    // M4's lowering reads `expr_types`, not the tree.
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


/// `x.f(y)` — §4.11's algorithm, in the order §4.11 states it: **a field first**,
/// then a free function. The field half needs the receiver's type, which is why
/// the resolver deliberately left this half unanswered (panel 015 D).
pub(super) fn method(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    at: ExprId,
    receiver: ExprId,
    called: Span,
    args: &[Arg],
) -> TyId {
    let span = ast.exprs[at.0 as usize].span;
    let receiver_ty = exprs::synth(checker, ast, resolved, src, receiver);
    let name = src.slice(called);
    if let Some(field) = field_of_function_type(checker, ast, resolved, src, receiver_ty, name) {
        return indirect_call(checker, ast, resolved, src, field, args, None, span);
    }
    match resolved.use_at(at) {
        Ref::Top(decl) => {
            // §4.8: UFCS does not apply when the first parameter is `@`, because
            // `l.advance()` would hide the mutation the marker exists to show.
            if let DeclKind::Function(function) = &ast.decls[decl as usize].kind {
                if function.params.first().is_some_and(|p| p.mutable) {
                    let diagnostic = errors::ufcs_on_mutable(name, called);
                    checker.push_diagnostic(diagnostic);
                    return checker.error_ty();
                }
            }
            user_call(checker, ast, resolved, src, decl, args, Some(receiver_ty), span)
        }
        Ref::Builtin(index) => {
            builtin_call(checker, ast, resolved, src, index, args, Some(receiver_ty), span)
        }
        // The resolver stayed silent because the name *might* have been a field.
        // Now the receiver's type is known, so both halves fit one message —
        // which is what the compiler-engineer's panel-015 veto asked for.
        Ref::Unresolved | Ref::Local(_) => {
            if !checker.out.types.poisoned(receiver_ty) {
                let holder = checker.show(ast, src, receiver_ty);
                let diagnostic = errors::no_field_and_no_function(&holder, name, called);
                checker.push_diagnostic(diagnostic);
            }
            checker.error_ty()
        }
    }
}


/// A call to a declared function, with `receiver` prepended when it was written
/// as `x.f(y)`.
fn user_call(
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
        let (line, _) = src.line_col(ast.decls[decl as usize].name.start);
        let diagnostic = errors::arity(&name, arity, given, Some((signature, line)), span);
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


fn builtin_call(
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


fn indirect_call(
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
