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
/// (M5b, panel 021; extended to aggregates at M5c).
///
/// **One home for this question**, and that is still the whole reason it is here:
/// the ownership pass, the prologue's zero-initialisation, the exit sweep and the
/// descriptor pass all ask it, and two answers that disagree about which types are
/// counted is a refcount bug that reproduces once a week — the same failure
/// `uses.rs` was split out to prevent.
///
/// It is now a **lookup**. The answer is transitive — `record Person { name: str }`
/// owns a reference and a `TyId` for `Ty::Named(d)` carries only the declaration
/// index — so it is computed once by `types/counted.rs`, where the AST is in hand,
/// and asked here. A missing entry reads as *not counted*, which would be a silent
/// leak, so the table is built after every type is interned and this asserts
/// nothing quietly: an out-of-range `TyId` is a compiler bug and panics.
pub(crate) fn is_refcounted(checked: &Checked, ty: TyId) -> bool {
    checked.counted[ty.0 as usize]
}
