//! Records as C: construction, field reads, places with a path, and the calls that
//! keep a counted field alive (design.md §4.9, §4.10; panels 021, 022).
//!
//! Its own file because `inst.rs` was already past CLAUDE.md §11's ceiling before
//! this step added a line, and because everything here shares one question: **given a
//! type and a field index, what is the C for it.** The field's *name* is needed to
//! spell the member, and by the IR a field is an index (`ir/layout.rs` explains why),
//! so every function here walks from a type to a declaration and back.
//!
//! **A record is a C struct by value** (panel 022, confirming spike 04), so:
//!
//! | Heroes | C |
//! |---|---|
//! | `Point(x: 1, y: 2)` | `(h_m_Point){.f_x = t1, .f_y = t2}` |
//! | `p.x` | `t2 = t1.f_x` |
//! | `p.inner.name` as a place | `h0_p.f_inner.f_name` |
//! | `a == b` | `h_m_Point_eq(&t1, &t2)` |
//!
//! The compound literal is designated, never positional: the IR already holds the
//! arguments in declared order (Part 5's "named arguments → positional" row costs
//! zero lines because a Heroes label never reorders an argument), so the designators
//! are redundant — and that is exactly why they are written. A field added to a
//! record in the middle is then a clang error at every construction site instead of a
//! silent shift of every value one field along.
//!
//! **Records and variant payloads only** (the §11 sweep of 2026-08-12 took the rest):
//! `container.rs` has `[T]` and `{K: V}`, which are a runtime pointer rather than a C
//! aggregate, and `fallible.rs` has `T?`, whose members are alternatives. What stayed
//! is what answers this file's one question. The `Types` bundle stayed too, because all
//! three files need it and it belongs with the field walk it exists for.

use crate::ir::{Function, Place, Step, ValueId};
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};
use crate::types::{Checked, Ty, TyId};

use super::typedefs::Names;
use super::mangle;

/// What every function here needs: the declarations, the types, the C names, and the
/// text the field names live in. Bundled because threading four references through
/// six functions is what makes a printer unreadable.
pub(super) struct Types<'a> {
    pub ast: &'a Ast,
    pub checked: &'a Checked,
    pub names: &'a Names,
    pub src: &'a Source,
}

impl<'a> Types<'a> {
    /// The C member name of a field, and the type it holds.
    ///
    /// `None` where the owner is not an aggregate or the index is past its fields,
    /// which by here means the checker reported something and the emitter is running
    /// on a program it should never have been handed.
    pub(super) fn field(&self, owner: TyId, index: u32) -> Option<(String, TyId)> {
        // §4.6's built-in record: two `str`s, `code` then `msg`, with no declaration
        // anywhere to read them from — `ir/layout.rs` fixes the order for the same
        // reason and this is the C side of it.
        if self.checked.types.get(owner) == Ty::Failure {
            let member = match index {
                0 => "code",
                _ => "msg",
            };
            return Some((member.to_string(), self.checked.types.str()));
        }
        let decl = match self.checked.types.get(owner) {
            Ty::Named(decl) | Ty::Case(decl, _) => decl,
            _ => return None,
        };
        // A record's fields, or one case's payload fields — `Ty::Case` is a real type
        // and `field $t6.v` reads it exactly as it reads a record's.
        let fields = match (&self.ast.decls[decl as usize].kind, self.checked.types.get(owner)) {
            (DeclKind::Record { fields }, _) => fields,
            (DeclKind::Variant { cases }, Ty::Case(_, case)) => {
                &cases.get(case as usize)?.fields
            }
            _ => return None,
        };
        let field = fields.get(index as usize)?;
        let ty = self.checked.written_type(field.ty)?;
        Some((mangle::field(self.src.slice(field.name)), ty))
    }

    /// The C type name a per-type function belongs to.
    pub(super) fn aggregate_name(&self, ty: TyId) -> Option<&str> {
        match self.checked.types.get(ty) {
            Ty::Named(decl) => Some(self.names.of(decl)),
            Ty::Case(decl, case) => Some(self.names.case_of(decl, case)),
            Ty::Fallible(_) => Some(self.names.option_of(ty)),
            Ty::Failure => Some("hero_failure"),
            _ => None,
        }
    }

    /// The variant a case belongs to, and the case's own name, for spelling a tag or a
    /// union member.
    fn case_names(&self, decl: u32, case: u32) -> Option<(String, String)> {
        let cases = match &self.ast.decls[decl as usize].kind {
            DeclKind::Variant { cases } => cases,
            _ => return None,
        };
        let name = self.src.slice(cases.get(case as usize)?.name).to_string();
        Some((self.names.of(decl).to_string(), name))
    }
}

/// A place as a C lvalue: the slot, then one `.member` per step.
///
/// The path is walked with the *type* alongside it, because the member's spelling
/// comes from the declaration and each step changes which declaration that is. An
/// index step cannot appear yet — arrays are refused — and it says so rather than
/// producing something that compiles.
pub(super) fn place(types: &Types, function: &Function, at: Place) -> String {
    let slot = &function.slots[at.root.0 as usize];
    let mut text = mangle::slot(at.root.0, &slot.name);
    let mut ty = slot.ty;
    for step in function.steps_of(at.path) {
        match step {
            Step::Field(index) => match types.field(ty, index) {
                Some((member, next)) => {
                    text.push('.');
                    text.push_str(&member);
                    ty = next;
                }
                None => return text,
            },
            Step::Index(_) => return text,
        }
    }
    text
}

/// `t3 = (h_m_Point){.f_x = t1, .f_y = t2};`
pub(super) fn construct(types: &Types, decl: u32, arguments: &[String]) -> Option<String> {
    let name = types.names.of(decl);
    let fields = match &types.ast.decls[decl as usize].kind {
        DeclKind::Record { fields } => fields,
        _ => return None,
    };
    let parts = designators(types, fields, arguments)?;
    // A record with no fields is `error[empty_record]`, so this cannot be empty — and
    // `(T){}` is not C11 anyway, which is why the case is named rather than defaulted.
    if parts.is_empty() {
        return None;
    }
    Some(format!("({name}){{{}}}", parts.join(", ")))
}

/// `t2 = (h_m_Token){.tag = h_m_Token_tag_word, .as.c_word = {.f_text = t1}};`
///
/// The result is the **whole variant**, not the case: `construct Token.word($t4)` has
/// type `Token` in the IR, and the case only names which arm of the union is written.
/// A payload-free case writes the tag alone — C zeroes the rest of a compound literal,
/// so the union is never left holding stale bytes.
pub(super) fn construct_case(
    types: &Types,
    decl: u32,
    case: u32,
    arguments: &[String],
) -> Option<String> {
    let (name, case_name) = types.case_names(decl, case)?;
    let cases = match &types.ast.decls[decl as usize].kind {
        DeclKind::Variant { cases } => cases,
        _ => return None,
    };
    let fields = &cases.get(case as usize)?.fields;
    let tag = format!(".tag = {}", mangle::tag_of(&name, &case_name));
    if fields.is_empty() {
        return Some(format!("({name}){{{tag}}}"));
    }
    let parts = designators(types, fields, arguments)?;
    Some(format!(
        "({name}){{{tag}, .as.{} = {{{}}}}}",
        mangle::case(&case_name),
        parts.join(", ")
    ))
}

/// `.f_x = t1` per field, in declared order.
///
/// Designated, never positional, even though the IR already holds the arguments in
/// declared order — Part 5's "named arguments → positional" row costs zero lines
/// because a Heroes label never reorders an argument. The designators are therefore
/// redundant, and that is exactly why they are written: a field added in the middle of
/// a record is a clang error at every construction site instead of a silent shift of
/// every value one field along.
fn designators(types: &Types, fields: &[crate::syntax::Field], arguments: &[String]) -> Option<Vec<String>> {
    let mut parts = Vec::new();
    for (index, field) in fields.iter().enumerate() {
        let value = arguments.get(index)?;
        parts.push(format!(".{} = {value}", mangle::field(types.src.slice(field.name))));
    }
    Some(parts)
}

/// `t3 = t2.tag;` — what a `Switch` reads.
pub(super) fn tag(base: ValueId) -> String {
    format!("{}.tag", mangle::value(base.0))
}

/// `t5 = t4.as.c_num;` — the payload of a case the `match` has already selected.
pub(super) fn payload(types: &Types, function: &Function, base: ValueId, case: u32) -> Option<String> {
    let decl = match types.checked.types.get(function.value_type(base)) {
        Ty::Named(decl) => decl,
        _ => return None,
    };
    let (_, case_name) = types.case_names(decl, case)?;
    Some(format!("{}.as.{}", mangle::value(base.0), mangle::case(&case_name)))
}

/// `t2 = t1.f_x;`
pub(super) fn read_field(
    types: &Types,
    function: &Function,
    base: ValueId,
    index: u32,
) -> Option<String> {
    let (member, _) = types.field(function.value_type(base), index)?;
    Some(format!("{}.{member}", mangle::value(base.0)))
}

/// `h_m_Point_eq(&t1, &t2)` — one call, generated per type, walking fields.
pub(super) fn equality(types: &Types, ty: TyId, left: ValueId, right: ValueId) -> Option<String> {
    let name = types.aggregate_name(ty)?;
    Some(format!("{name}_eq(&{}, &{})", mangle::value(left.0), mangle::value(right.0)))
}

/// `h_m_Holder_retain(&t1);` / `..._release(&t1);`
///
/// A value, not a place, because that is what `Op::Incref` names — and the aggregate
/// is by value, so the generated function takes its address. The name is not `copy`:
/// nothing is copied here. The bytes are already where they belong and this only
/// makes the references inside them owned.
pub(super) fn retain(types: &Types, ty: TyId, value: ValueId, keep: bool) -> Option<String> {
    let name = types.aggregate_name(ty)?;
    let verb = if keep { "retain" } else { "release" };
    Some(format!("{name}_{verb}(&{})", mangle::value(value.0)))
}
