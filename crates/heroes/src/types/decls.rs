//! One declaration at a time: what its body is *for*, and what `return` owes it.
//!
//! Three shapes, and the difference between them is the whole reason `Want`
//! exists:
//!
//! - a **constant**'s body *is* its value, so its last statement is checked
//!   against the declared type (§4.2);
//! - a **function**'s body is run for effect and hands its value back through
//!   `return`, so the body wants nothing and `checker.result` carries the
//!   signature's promise;
//! - a **test**'s body is run for effect and returns nothing at all (§4.18).
//!
//! `record` and `variant` declare types, not values: their field types were
//! already resolved, and there is nothing here to check.

use crate::resolve::Resolved;
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};

use super::stmts::{block, Want};
use super::table::Ty;
use super::{errors, lower, Checker, TyId};



pub(super) fn file(checker: &mut Checker, ast: &Ast, resolved: &Resolved, src: &Source) {
    for (index, decl) in ast.decls.iter().enumerate() {
        match &decl.kind {
            DeclKind::Constant { ty, body } => {
                let declared = lower::ty(checker, ast, resolved, *ty);
                checker.result = declared;
                checker.fallible = is_fallible(checker, declared);
                checker.generic_names.clear();
                // A constant's body *is* its value: its last statement produces
                // it, unlike a function, which returns.
                let (_, value) = block(checker, ast, resolved, src, body, Want::Value(declared));
                if value.is_none() {
                    let want = checker.show(ast, src, declared);
                    let diagnostic = errors::no_value(&want, body.span);
                    checker.push_diagnostic(diagnostic);
                }
            }
            DeclKind::Function(function) => {
                checker.generic_names = function
                    .generics
                    .iter()
                    .map(|span| src.slice(*span).to_string())
                    .collect();
                let result = lower::ty(checker, ast, resolved, function.result);
                checker.out.results.insert(index as u32, result);
                checker.result = result;
                checker.fallible = is_fallible(checker, result);
                for param in &function.params {
                    let ty = lower::ty(checker, ast, resolved, param.ty);
                    checker.bind_local(param.name, ty);
                }
                if let Some(body) = &function.body {
                    block(checker, ast, resolved, src, body, Want::Nothing);
                }
            }
            DeclKind::Test { body } => {
                checker.generic_names.clear();
                checker.result = checker.out.types.unit();
                checker.fallible = false;
                block(checker, ast, resolved, src, body, Want::Nothing);
            }
            DeclKind::Record { .. } | DeclKind::Variant { .. } => {}
        }
    }
}



fn is_fallible(checker: &Checker, ty: TyId) -> bool {
    matches!(checker.out.types.get(ty), Ty::Fallible(_))
}
