//! One declaration at a time: its generics, its signature, its body (§4.2 the
//! four entities, §4.12 generics, §4.18 `test`, §4.19 `extern`).
//!
//! Every declaration starts from an empty scope stack. That is the point of
//! the top-level table: nothing a function binds can be seen by another
//! function, so a file's declarations can be resolved in any order — which is
//! what "declaration order never matters" means once you are the compiler.
//!
//! **An `extern`'s parameters are not bindings.** Its body is in C (§4.19), so
//! its parameter names can never be read, and declaring them would make every
//! FFI declaration in the file an unused-binding error. Their *types* are
//! resolved: `extern sqrt = function: (x: f64) -> f64` must still name types
//! that exist, because clang is going to be handed those names.

use crate::source::Source;
use crate::syntax::{Ast, DeclKind, Function};

use super::builtins::index_of;
use super::scope::Binding;
use super::{errors, stmts, types, Resolver};

pub(super) fn declaration(r: &mut Resolver, ast: &Ast, src: &Source, index: usize) {
    let decl = &ast.decls[index];
    r.generics.clear();
    match &decl.kind {
        DeclKind::Constant { ty, body } => {
            types::resolve(r, ast, src, *ty);
            r.open_scope();
            stmts::block(r, ast, src, body);
            r.close_scope();
        }
        DeclKind::Function(function) => function_decl(r, ast, src, function),
        DeclKind::Record { fields } => {
            for field in fields {
                types::resolve(r, ast, src, field.ty);
            }
        }
        DeclKind::Variant { cases } => {
            for case in cases {
                for field in &case.fields {
                    types::resolve(r, ast, src, field.ty);
                }
            }
        }
        DeclKind::Test { body } => {
            r.open_scope();
            stmts::block(r, ast, src, body);
            r.close_scope();
        }
    }
}

fn function_decl(r: &mut Resolver, ast: &Ast, src: &Source, function: &Function) {
    generics(r, ast, src, function);
    // The signature is resolved before the parameters are bound, so a
    // parameter's type can never accidentally see a parameter's name: types
    // and values are separate namespaces, and this is where that is enforced.
    for param in &function.params {
        types::resolve(r, ast, src, param.ty);
    }
    types::resolve(r, ast, src, function.result);
    let Some(body) = &function.body else {
        return; // `extern`: the code is in C, so nothing here binds
    };
    r.open_scope();
    for param in &function.params {
        r.declare(ast, src, param.name, Binding::param(param.ty, param.mutable));
    }
    stmts::block(r, ast, src, body);
    r.close_scope();
}

/// `function<A, B>:` — the type parameters, in their own namespace (§4.12: on
/// functions only, no constraints, always inferred).
///
/// They are checked against the same two things a type name is checked
/// against — the primitives and the file's own type declarations — because
/// `function<int>` and `function<Point>` are both a name whose meaning is
/// already taken, and a type parameter that shadows a record makes every type
/// in the signature ambiguous to a reader.
fn generics(r: &mut Resolver, ast: &Ast, src: &Source, function: &Function) {
    for (position, span) in function.generics.iter().enumerate() {
        let name = src.slice(*span);
        if let Some(previous) = r.generics.iter().find(|(n, _)| n == name) {
            let (line, _) = src.line_col(function.generics[previous.1 as usize].start);
            let diagnostic = errors::shadowed(name, line, *span);
            r.push_diagnostic(diagnostic);
            continue;
        }
        if types::primitive(name).is_some() || index_of(name).is_some() {
            let diagnostic = errors::builtin_name_taken(name, *span);
            r.push_diagnostic(diagnostic);
            continue;
        }
        if let Some(&decl) = r.out.top.get(name) {
            let (line, _) = src.line_col(ast.decls[decl as usize].name.start);
            let diagnostic = errors::shadows_top_level(name, line, *span);
            r.push_diagnostic(diagnostic);
            continue;
        }
        r.generics.push((name.to_string(), position as u32));
    }
}
