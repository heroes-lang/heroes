//! A type, written the way the author would have written it.
//!
//! Every message in `errors.rs` goes through this, and it prints **surface
//! syntax** — `[{str: Point}?]`, not a tree. design.md §4.17: an error carries
//! what is needed to fix the program, and a type spelled in a notation the
//! language does not have fails that test.
//!
//! A `record` or `variant` prints its declared name, which is why this needs the
//! tree: nominal types are names, and the name lives in `Ast::decls`.

use crate::source::Source;
use crate::syntax::Ast;

use super::table::{Ty, TyId, Types};

/// `generics` names the type parameters of the function the message is about
/// (§4.12 keeps generics on functions, and a message is always about one), so
/// `A` prints as `A`. An empty slice is legal and prints `#0`, which only
/// happens where a caller outside the checker has no function in hand.
pub fn render_ty(
    types: &Types,
    ast: &Ast,
    src: &Source,
    id: TyId,
    generics: &[String],
) -> String {
    match types.get(id) {
        Ty::Int(kind) => kind.name().to_string(),
        Ty::Float(kind) => kind.name().to_string(),
        Ty::Bool => "bool".to_string(),
        Ty::Str => "str".to_string(),
        Ty::Ptr => "ptr".to_string(),
        Ty::Cstr => "cstr".to_string(),
        Ty::Unit => "()".to_string(),
        Ty::Array(inner) => format!("[{}]", render_ty(types, ast, src, inner, generics)),
        Ty::Map(key, value) => format!(
            "{{{}: {}}}",
            render_ty(types, ast, src, key, generics),
            render_ty(types, ast, src, value, generics)
        ),
        Ty::Fallible(inner) => format!("{}?", render_ty(types, ast, src, inner, generics)),
        Ty::Func { params, result } => {
            let rendered: Vec<String> = types
                .params_of(params)
                .iter()
                .map(|p| render_ty(types, ast, src, *p, generics))
                .collect();
            format!(
                "(function({}) -> {})",
                rendered.join(", "),
                render_ty(types, ast, src, result, generics)
            )
        }
        Ty::Named(decl) => src.slice(ast.decls[decl as usize].name).to_string(),
        // `Token.num` — a case is not writable as a type, and pretending it is
        // would send the reader looking for a syntax that does not exist.
        Ty::Case(decl, case) => {
            let name = src.slice(ast.decls[decl as usize].name);
            match &ast.decls[decl as usize].kind {
                crate::syntax::DeclKind::Variant { cases } => {
                    format!("{name}.{}", src.slice(cases[case as usize].name))
                }
                _ => name.to_string(),
            }
        }
        Ty::Failure => "the error (`code`, `msg`)".to_string(),
        // A type parameter prints as it was declared: `A`, not `#0`.
        Ty::Generic(position) => generics
            .get(position as usize)
            .cloned()
            .unwrap_or_else(|| format!("#{position}")),
        Ty::Error => "?".to_string(),
    }
}
