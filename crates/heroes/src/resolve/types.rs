//! Written types, against the three things a type name can be: a primitive, a
//! declaration in this file, or a type parameter of the enclosing function
//! (§4.3 the type table, §4.12 generics).
//!
//! The recursion is over `Ast::types`, the arena the parser already built, so
//! `[{str: Point}?]` costs one walk and five entries in `type_uses`. A later
//! pass never has to read the written form again: it asks the map.
//!
//! Types are a separate namespace from values, and this file is where that is
//! true rather than merely intended. `Point` names both a type and its
//! constructor (§4.9: record construction *is* a call), and those are two
//! different questions asked in two different places — here, and in `exprs.rs`.

use crate::source::Source;
use crate::syntax::{Ast, DeclKind, TypeId, TypeKind};

use super::errors;
use super::{Resolver, TypeRef};

/// The primitive types (§4.3, plus §4.19's two FFI names). Not keywords: they
/// are ordinary identifiers the resolver knows, which is why a type parameter
/// named `int` is a name collision and not a syntax error.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Prim {
    Int,
    F64,
    Bool,
    Str,
    /// `ptr` — an opaque C pointer (§4.19).
    Ptr,
    /// `cstr` — a C string (§4.19).
    Cstr,
}

pub(super) fn primitive(name: &str) -> Option<Prim> {
    Some(match name {
        "int" => Prim::Int,
        "f64" => Prim::F64,
        "bool" => Prim::Bool,
        "str" => Prim::Str,
        "ptr" => Prim::Ptr,
        "cstr" => Prim::Cstr,
        _ => return None,
    })
}

pub(super) fn resolve(r: &mut Resolver, ast: &Ast, src: &Source, id: TypeId) {
    match &ast.types[id.0 as usize].kind {
        TypeKind::Named => named(r, ast, src, id),
        // `()` needs no resolution, and a type the parser could not read has
        // already been reported: saying more would be a second diagnostic for
        // one mistake.
        TypeKind::Unit | TypeKind::Error => {}
        TypeKind::Array(inner) | TypeKind::Fallible(inner) => resolve(r, ast, src, *inner),
        TypeKind::Map(key, value) => {
            resolve(r, ast, src, *key);
            resolve(r, ast, src, *value);
        }
        TypeKind::Func { params, result } => {
            for param in params {
                resolve(r, ast, src, *param);
            }
            resolve(r, ast, src, *result);
        }
    }
}

fn named(r: &mut Resolver, ast: &Ast, src: &Source, id: TypeId) {
    let span = ast.types[id.0 as usize].span;
    let name = src.slice(span);
    if let Some(prim) = primitive(name) {
        r.out.type_uses[id.0 as usize] = TypeRef::Prim(prim);
        return;
    }
    if let Some((_, position)) = r.generics.iter().find(|(n, _)| n == name) {
        r.out.type_uses[id.0 as usize] = TypeRef::Generic(*position);
        return;
    }
    // A qualified type: `geom.Point`. The module has to be one this file names,
    // exactly as for a value — there is no privacy and no re-export, so being
    // reachable through some other module's `use` is not being reachable here.
    if let Some((module, type_name)) = name.split_once('.') {
        if !r.out.is_used_module(&r.module, module) {
            let known: Vec<String> = r.used_modules();
            let diagnostic = errors::not_a_module(module, &known, span);
            r.push_diagnostic(diagnostic);
            return;
        }
        r.module_reads.insert((r.module.clone(), module.to_string()));
        match r.out.top_in(module, type_name) {
            Some(decl)
                if matches!(
                    ast.decls[decl as usize].kind,
                    DeclKind::Record { .. } | DeclKind::Variant { .. }
                ) =>
            {
                r.out.type_uses[id.0 as usize] = TypeRef::Top(decl);
            }
            Some(_) => {
                let diagnostic = errors::not_a_type(name, "not a type", span);
                r.push_diagnostic(diagnostic);
            }
            None => {
                let names: Vec<String> = r
                    .out
                    .names_in(module)
                    .filter(|(_, decl)| {
                        matches!(
                            ast.decls[*decl as usize].kind,
                            DeclKind::Record { .. } | DeclKind::Variant { .. }
                        )
                    })
                    .map(|(n, _)| n.to_string())
                    .collect();
                let near = r.near_names(type_name, &names);
                let diagnostic = errors::not_in_module(module, type_name, &near, span);
                r.push_diagnostic(diagnostic);
            }
        }
        return;
    }
    if let Some(decl) = r.top_visible(name) {
        match &ast.decls[decl as usize].kind {
            DeclKind::Record { .. } | DeclKind::Variant { .. } => {
                r.out.type_uses[id.0 as usize] = TypeRef::Top(decl);
            }
            // A function or a constant in type position: the name exists, so
            // "unknown type" would be a lie and a did-you-mean would be noise.
            DeclKind::Function(function) => {
                let what = if function.is_extern { "an extern function" } else { "a function" };
                let diagnostic = errors::not_a_type(name, what, span);
                r.push_diagnostic(diagnostic);
            }
            DeclKind::Constant { .. } => {
                let diagnostic = errors::not_a_type(name, "a constant", span);
                r.push_diagnostic(diagnostic);
            }
            DeclKind::Test { .. } => {}
        }
        return;
    }
    let candidates = type_candidates(r, ast);
    let near = r.near_names(name, &candidates);
    let diagnostic = errors::unknown_type(name, &near, span);
    r.push_diagnostic(diagnostic);
}

/// Everything that could have been meant, nearest namespace first: the type
/// parameters in hand, then the file's own types, then the primitives.
fn type_candidates(r: &Resolver, ast: &Ast) -> Vec<String> {
    let mut candidates: Vec<String> = r.generics.iter().map(|(n, _)| n.clone()).collect();
    for (name, decl) in r.out.names_in(&r.module) {
        if matches!(
            ast.decls[decl as usize].kind,
            DeclKind::Record { .. } | DeclKind::Variant { .. }
        ) {
            candidates.push(name.to_string());
        }
    }
    for prim in ["int", "f64", "bool", "str", "ptr", "cstr"] {
        candidates.push(prim.to_string());
    }
    candidates
}
