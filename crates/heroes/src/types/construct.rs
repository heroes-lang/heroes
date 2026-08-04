//! Building a value out of named fields: a record, a variant case, and the two
//! fallible constructors (§4.9, §4.2, §4.6, panel 002).
//!
//! One rule serves all three, and it is §4.9's: **every field is named, always**.
//! There is no positional form to get wrong, so the check is a walk over the
//! declaration's fields against the call's labels, in order.
//!
//! `ok` and `fail` live here rather than in `builtins.rs` because they are
//! ⇐-only: which `T?` they build is the *caller's* question (panel 002 — no `T`
//! is ever promoted to a `T?`), so they need the expected type, and a built-in
//! rule that only sees argument types cannot have it.

use crate::resolve::{Ref, Resolved};
use crate::source::{Source, Span};
use crate::syntax::{Arg, Ast, DeclKind, ExprId, ExprKind};

use super::table::Ty;
use super::expect;
use super::{errors, exprs, lower, Checker, TyId};


/// `Point(x: 3, y: 4)` — §4.9 makes every field name mandatory, always, which is
/// why there is no positional form to get wrong.
pub(super) fn construct_record(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    decl: u32,
    args: &[Arg],
    span: Span,
) -> TyId {
    let DeclKind::Record { fields } = &ast.decls[decl as usize].kind else {
        return checker.error_ty();
    };
    let name = src.slice(ast.decls[decl as usize].name).to_string();
    let expected: Vec<(String, crate::syntax::TypeId)> = fields
        .iter()
        .map(|f| (src.slice(f.name).to_string(), f.ty))
        .collect();
    let (line, _) = src.line_col(ast.decls[decl as usize].name.start);
    let holder = Holder { label: name, line };
    check_named_fields(checker, ast, resolved, src, &holder, &expected, args, span);
    checker.out.types.intern(Ty::Named(decl))
}


/// `.num(v: 12)` in ⇐ position: the expected type says which variant, and the
/// case name says which case (§4.5's ⇐ mode, §4.2's "a case is a small record").
pub(super) fn case(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    at: ExprId,
    called: Span,
    args: &[Arg],
    expected: TyId,
) {
    let span = ast.exprs[at.0 as usize].span;
    let name = src.slice(called);
    let Ty::Named(decl) = checker.out.types.get(expected) else {
        if !checker.out.types.poisoned(expected) {
            let want = checker.show(ast, src, expected);
            let diagnostic = errors::case_not_expected(name, &want, span);
            checker.push_diagnostic(diagnostic);
        }
        checker.record(at, checker.out.types.error());
        return;
    };
    let DeclKind::Variant { cases } = &ast.decls[decl as usize].kind else {
        let want = checker.show(ast, src, expected);
        let diagnostic = errors::case_not_expected(name, &want, span);
        checker.push_diagnostic(diagnostic);
        checker.record(at, checker.out.types.error());
        return;
    };
    let holder = src.slice(ast.decls[decl as usize].name).to_string();
    let Some(found) = cases.iter().find(|c| src.slice(c.name) == name) else {
        let known: Vec<String> = cases.iter().map(|c| src.slice(c.name).to_string()).collect();
        let diagnostic = errors::no_such_case(&holder, name, &known, called);
        checker.push_diagnostic(diagnostic);
        checker.record(at, checker.out.types.error());
        return;
    };
    let expected_fields: Vec<(String, crate::syntax::TypeId)> = found
        .fields
        .iter()
        .map(|f| (src.slice(f.name).to_string(), f.ty))
        .collect();
    let label = format!("{holder}.{name}");
    let (line, _) = src.line_col(found.name.start);
    let holder = Holder { label, line };
    check_named_fields(checker, ast, resolved, src, &holder, &expected_fields, args, span);
    checker.record(at, expected);
}


/// `ok(x)` and `fail(code, msg)` — ⇐-only (panel 002): no implicit `T` → `T?`
/// ever, and the expected type is what decides which `T?` is being built.
pub(super) fn fallible_constructor(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    callee: ExprId,
    args: &[Arg],
    expected: TyId,
    span: Span,
) -> Option<TyId> {
    if !matches!(ast.exprs[callee.0 as usize].kind, ExprKind::Name) {
        return None;
    }
    let Ref::Builtin(index) = resolved.use_at(callee) else { return None };
    let name = crate::resolve::BUILTINS[index as usize].name;
    if name != "ok" && name != "fail" {
        return None;
    }
    let Ty::Fallible(inner) = checker.out.types.get(expected) else {
        if !checker.out.types.poisoned(expected) {
            let want = checker.show(ast, src, expected);
            let diagnostic = errors::constructor_not_expected(name, &want, span);
            checker.push_diagnostic(diagnostic);
        }
        return Some(checker.error_ty());
    };
    if name == "ok" {
        if args.len() != 1 {
            let diagnostic = errors::arity("ok", 1, args.len(), None, span);
            checker.push_diagnostic(diagnostic);
            return Some(checker.error_ty());
        }
        expect::check(checker, ast, resolved, src, args[0].value, inner);
        return Some(expected);
    }
    if args.len() != 2 {
        let diagnostic = errors::arity("fail", 2, args.len(), None, span);
        checker.push_diagnostic(diagnostic);
        return Some(checker.error_ty());
    }
    let str_ty = checker.out.types.str();
    for arg in args {
        expect::check(checker, ast, resolved, src, arg.value, str_ty);
    }
    Some(expected)
}


/// Named fields, for a record or a case: every one mandatory, none repeated,
/// none unknown, and the label must match the field at that position.
/// What is being built, as one value: the name a message calls it and the line
/// its declaration sits on. Grouped because those two always travel together —
/// and because the alternative is a nine-parameter function.
struct Holder {
    label: String,
    line: u32,
}

fn check_named_fields(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    holder: &Holder,
    expected: &[(String, crate::syntax::TypeId)],
    args: &[Arg],
    span: Span,
) {
    if args.len() != expected.len() {
        let names: Vec<String> = expected.iter().map(|(n, _)| n.clone()).collect();
        let diagnostic =
            errors::missing_fields(&holder.label, &names, holder.line, span);
        checker.push_diagnostic(diagnostic);
        for arg in args {
            exprs::synth(checker, ast, resolved, src, arg.value);
        }
        return;
    }
    for (index, arg) in args.iter().enumerate() {
        let (field, written) = &expected[index];
        match arg.name {
            Some(label) if src.slice(label) == field.as_str() => {}
            Some(label) => {
                let diagnostic =
                    errors::wrong_label(&holder.label, src.slice(label), field, label);
                checker.push_diagnostic(diagnostic);
            }
            None => {
                let at = ast.exprs[arg.value.0 as usize].span;
                let diagnostic = errors::missing_label(&holder.label, field, at);
                checker.push_diagnostic(diagnostic);
            }
        }
        let ty = lower::ty(checker, ast, resolved, *written);
        expect::check(checker, ast, resolved, src, arg.value, ty);
    }
}


/// A field of the receiver whose type is a function — §4.11's first lookup.
pub(super) fn field_of_function_type(
    checker: &mut Checker,
    ast: &Ast,
    resolved: &Resolved,
    src: &Source,
    receiver: TyId,
    name: &str,
) -> Option<TyId> {
    let Ty::Named(decl) = checker.out.types.get(receiver) else { return None };
    let DeclKind::Record { fields } = &ast.decls[decl as usize].kind else { return None };
    let found = fields.iter().find(|f| src.slice(f.name) == name)?;
    let ty = lower::ty(checker, ast, resolved, found.ty);
    match checker.out.types.get(ty) {
        Ty::Func { .. } => Some(ty),
        _ => None,
    }
}
