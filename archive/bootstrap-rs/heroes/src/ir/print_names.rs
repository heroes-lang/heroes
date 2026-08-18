//! Recovering a name for the reader (panel 019 point 7).
//!
//! The IR holds **indices**: a field is a number, a case is a number, a callee is a
//! declaration. That is deliberate — no pass after the checker compares a string to
//! decide what a name means (`layout.rs` says why). But a dump full of numbers is a
//! dump nobody reads, so every index is turned back into the name the author wrote,
//! here, at the last possible moment, out of the type the value already carries.
//!
//! One of these renderings is load-bearing rather than cosmetic. `target` spells
//! the **linkage** — `heroes`, `extern`, `builtin`, `indirect` — because panel 019's
//! most expensive finding was that an unmangled Heroes `function open` silently
//! replaces libc's. A dump that did not say which kind of call this is would hide
//! exactly the difference that goes wrong.

use crate::source::Source;
use crate::syntax::{Ast, DeclKind};
use crate::types::{Checked, Ty, TyId};

use super::inst::{Callee, Shape};
use super::ids::{ValueId};
use super::print_inst::value_name;
use super::Function;

/// The linkage, spelled out. This is panel 019's most expensive finding made
/// visible: `heroes` names are mangled, `extern` names are not, and a dump that
/// did not say which would hide the difference that silently replaces libc's
/// `open`.
pub(super) fn target(callee: Callee, ast: &Ast, src: &Source) -> String {
    match callee {
        Callee::Heroes(decl) => {
            format!("heroes {}", src.slice(ast.decls[decl as usize].name))
        }
        Callee::Extern(decl) => {
            format!("extern {}", src.slice(ast.decls[decl as usize].name))
        }
        Callee::Builtin(index) => {
            format!("builtin {}", crate::resolve::BUILTINS[index as usize].name)
        }
        Callee::Indirect(value) => format!("indirect {}", value_name(value)),
    }
}

pub(super) fn shape_name(shape: Shape, ast: &Ast, src: &Source) -> String {
    match shape {
        Shape::Record(decl) => src.slice(ast.decls[decl as usize].name).to_string(),
        Shape::Case(decl, case) => {
            let holder = src.slice(ast.decls[decl as usize].name);
            match &ast.decls[decl as usize].kind {
                DeclKind::Variant { cases } => {
                    format!("{holder}.{}", src.slice(cases[case as usize].name))
                }
                _ => holder.to_string(),
            }
        }
        Shape::Array => "array".to_string(),
        Shape::Map => "map".to_string(),
        Shape::Ok => "ok".to_string(),
        Shape::Fail => "fail".to_string(),
        Shape::Err => "err".to_string(),
    }
}

pub(super) fn field_name(
    function: &Function,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
    base: ValueId,
    index: u32,
) -> String {
    name_of_field(ast, checked, src, function.value_type(base), index)
}

/// The type a field *holds*, so a path can keep naming its steps past the first
/// one. Without it `g.rows[$t1].cells[$t2]` printed `g.rows[$t1].0[$t2]`: the type
/// was dropped after one field step, and every name after it fell back to an index.
pub(super) fn field_type(
    ast: &Ast,
    checked: &Checked,
    owner: TyId,
    index: u32,
) -> TyId {
    let written = match checked.types.get(owner) {
        Ty::Named(decl) => match &ast.decls[decl as usize].kind {
            DeclKind::Record { fields, .. } => fields.get(index as usize).map(|field| field.ty),
            _ => None,
        },
        Ty::Case(decl, case) => match &ast.decls[decl as usize].kind {
            DeclKind::Variant { cases } => {
                cases[case as usize].fields.get(index as usize).map(|field| field.ty)
            }
            _ => None,
        },
        // The two `str` fields of a failure (§4.6).
        Ty::Failure => return checked.types.str(),
        _ => None,
    };
    written
        .and_then(|id| checked.written_type(id))
        .unwrap_or_else(|| checked.types.error())
}

pub(super) fn name_of_field(
    ast: &Ast,
    checked: &Checked,
    src: &Source,
    owner: TyId,
    index: u32,
) -> String {
    let fallback = || index.to_string();
    match checked.types.get(owner) {
        Ty::Named(decl) => match &ast.decls[decl as usize].kind {
            DeclKind::Record { fields, .. } => {
                fields.get(index as usize).map(|f| src.slice(f.name).to_string()).unwrap_or_else(fallback)
            }
            _ => fallback(),
        },
        Ty::Case(decl, case) => match &ast.decls[decl as usize].kind {
            DeclKind::Variant { cases } => cases[case as usize]
                .fields
                .get(index as usize)
                .map(|f| src.slice(f.name).to_string())
                .unwrap_or_else(fallback),
            _ => fallback(),
        },
        Ty::Failure => super::layout::FAILURE_FIELDS
            .get(index as usize)
            .map(|name| name.to_string())
            .unwrap_or_else(fallback),
        _ => fallback(),
    }
}

/// `.num`, `.ok` — the case a payload read narrows to.
pub(super) fn case_name(
    function: &Function,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
    base: ValueId,
    case: u32,
) -> String {
    match checked.types.get(function.value_type(base)) {
        Ty::Named(decl) => match &ast.decls[decl as usize].kind {
            DeclKind::Variant { cases } => {
                format!(".{}", src.slice(cases[case as usize].name))
            }
            _ => format!("case {case}"),
        },
        // §4.6's two built-in cases.
        Ty::Fallible(_) => {
            if case == 0 {
                ".ok".to_string()
            } else {
                ".err".to_string()
            }
        }
        _ => format!("case {case}"),
    }
}
