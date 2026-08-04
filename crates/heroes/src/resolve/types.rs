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
    if let Some(&decl) = r.out.top.get(name) {
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
    for (name, decl) in &r.out.top {
        if matches!(
            ast.decls[*decl as usize].kind,
            DeclKind::Record { .. } | DeclKind::Variant { .. }
        ) {
            candidates.push(name.clone());
        }
    }
    for prim in ["int", "f64", "bool", "str", "ptr", "cstr"] {
        candidates.push(prim.to_string());
    }
    candidates
}
