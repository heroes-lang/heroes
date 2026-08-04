//! Where a field lives, and which case a name is (design.md §4.2, §4.6, §4.9).
//!
//! By here a field is an **index**, not a name. The checker already matched the
//! name against the declaration to give the expression its type; comparing
//! strings again would be a second answer to a question that was already
//! answered, and the two could disagree. This file is the one place the
//! translation happens, and it is deliberately small.
//!
//! Three shapes carry fields, and the third is the one that surprises: a `T?`'s
//! error side is a **built-in** record of two `str` fields, `code` and `msg`
//! (§4.6). The spec states those two fields and nothing else, so their indices
//! are fixed here rather than read from a declaration that does not exist.
//!
//! Nothing here reorders anything, and that is a finding rather than an omission:
//! Part 5's "named arguments → positional" row costs zero lines because a label in
//! Heroes never reorders an argument. `types/calls.rs` checks the label written at
//! a position against the parameter *at that position* (§4.9), so the arguments
//! were already in declaration order before lowering saw them.

use crate::source::{Source, Span};
use crate::syntax::{Ast, DeclKind};
use crate::types::{Checked, Ty, TyId};

/// The two fields of the built-in failure record, in order.
pub(super) const FAILURE_FIELDS: [&str; 2] = ["code", "msg"];

/// The index of a field within the type that holds it, or `None` if that type
/// holds no such field — which by here means the checker reported something and
/// lowering should stay quiet.
pub(super) fn field_index(
    ast: &Ast,
    checked: &Checked,
    src: &Source,
    owner: TyId,
    name: Span,
) -> Option<u32> {
    let wanted = src.slice(name);
    match checked.types.get(owner) {
        Ty::Named(decl) => match &ast.decls[decl as usize].kind {
            DeclKind::Record { fields } => {
                position(fields.iter().map(|f| src.slice(f.name)), wanted)
            }
            _ => None,
        },
        Ty::Case(decl, case) => match &ast.decls[decl as usize].kind {
            DeclKind::Variant { cases } => {
                let fields = &cases[case as usize].fields;
                position(fields.iter().map(|f| src.slice(f.name)), wanted)
            }
            _ => None,
        },
        Ty::Failure => position(FAILURE_FIELDS.into_iter(), wanted),
        _ => None,
    }
}

/// The index of a variant's case by name: what a `Switch` arm and a `.case`
/// construction both need.
pub(super) fn case_index(ast: &Ast, src: &Source, decl: u32, name: Span) -> Option<u32> {
    let wanted = src.slice(name);
    match &ast.decls[decl as usize].kind {
        DeclKind::Variant { cases } => position(cases.iter().map(|c| src.slice(c.name)), wanted),
        _ => None,
    }
}

fn position<'a>(mut names: impl Iterator<Item = &'a str>, wanted: &str) -> Option<u32> {
    names.position(|name| name == wanted).map(|index| index as u32)
}

/// Whether a value of this type carries a reference the compiler must count
/// (M5b, panel 021 R2).
///
/// **One home for this question**, and that is the whole reason it is here rather
/// than in the ownership pass: M5c's descriptor pass asks it again for
/// `copy`/`drop`/`eq`/`hash`, and two answers that disagree about which types are
/// counted is a refcount bug that reproduces once a week — the same failure
/// `uses.rs` was split out to prevent.
///
/// At M5b only `str` is counted. Every commented row below is a row M5c turns on,
/// and each is listed rather than folded into a catch-all so that adding a type to
/// the language cannot silently make it uncounted.
pub(crate) fn is_refcounted(checked: &Checked, ty: TyId) -> bool {
    match checked.types.get(ty) {
        Ty::Str => true,
        // M5c, with the descriptor pass: Array, Map, Named, Case, Fallible, Failure.
        Ty::Array(_) | Ty::Map(_, _) | Ty::Named(_) | Ty::Case(_, _) => false,
        Ty::Fallible(_) | Ty::Failure => false,
        // Scalars, the FFI's opaque types, and a type the checker gave up on.
        Ty::Int | Ty::F64 | Ty::Bool | Ty::Unit | Ty::Ptr | Ty::Cstr => false,
        Ty::Func { .. } | Ty::Generic(_) | Ty::Error => false,
    }
}
