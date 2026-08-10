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

use crate::ir::{Function, Place, Step, ValueId};
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};
use crate::types::{Checked, Ty, TyId};

use super::ctype::Names;
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
    fn field(&self, owner: TyId, index: u32) -> Option<(String, TyId)> {
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
    fn aggregate_name(&self, ty: TyId) -> Option<&str> {
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

// --- arrays ---------------------------------------------------------------

/// `t4 = hero_array_new(&hero_desc_int, 3);` plus one push per element.
///
/// Built by pushing rather than by writing the elements directly, because `push` is
/// where the element's `copy` runs — and that copy is what takes the array's own
/// reference to a counted element. Writing the bytes would share the caller's
/// reference without counting it.
///
/// Each push hands back a NEW array and the previous one is released, so a literal of
/// n elements allocates n+1 blocks and frees n. That is §4.10's declared bill in its
/// smallest form; performance is a non-goal (Part 2), and the alternative — a `push`
/// that appends in place — has no reading under value semantics, because a refcount of
/// 1 means "the slot has it" and appending would change what the slot sees.
pub(super) fn build_array(
    types: &Types,
    result: TyId,
    arguments: &[String],
    into: &str,
) -> Option<Vec<String>> {
    let element = match types.checked.types.get(result) {
        Ty::Array(element) => element,
        _ => return None,
    };
    let desc = super::descriptors::pointer(types.checked, types.names, element)?;
    let mut lines = vec![format!("{into} = hero_array_new({desc}, {});", arguments.len().max(1))];
    for value in arguments {
        // The temporary the push returns replaces the one it grew from, and the old one
        // is released — one line each, and no temporary left owning anything.
        lines.push(format!("{{ HeroArrayHeader *grown = hero_array_push({into}, &{value});"));
        lines.push(format!("  hero_array_decref({into}); {into} = grown; }}"));
    }
    Some(lines)
}

/// `t9 = *(const int64_t *)hero_array_at(t7, t8);`
///
/// The read goes through the runtime because that is where the bounds check lives:
/// spec line 126 says an out-of-bounds index aborts, and §4.9 says it never reads
/// arbitrary memory. The cast is on the *result*, so the element type is stated at
/// every read and clang checks the assignment.
pub(super) fn read_element(
    types: &Types,
    function: &Function,
    base: ValueId,
    index: ValueId,
) -> Option<String> {
    let element = match types.checked.types.get(function.value_type(base)) {
        Ty::Array(element) => element,
        _ => return None,
    };
    let spelling = super::ctype::c_type(types.names, types.checked, element)?;
    // `{spelling} const *`, not `const {spelling} *`: the two differ exactly when the
    // element is itself a pointer. For `[[int]]` the element spelling is
    // `HeroArrayHeader *`, and the prefix form reads as pointer-to-pointer-to-const,
    // whose dereference is a `const HeroArrayHeader *` — assigning that to the
    // temporary is `-Wincompatible-pointer-types-discards-qualifiers`. The suffix form
    // says what is meant: a const pointer to the element, whatever the element is.
    Some(format!(
        "*({spelling} const *)hero_array_at({}, {})",
        mangle::value(base.0),
        mangle::value(index.0)
    ))
}

/// `xs[i] @ v`, and every deeper spelling of it: **one unshare per array step of the
/// place, each writing back at its own level** (panel 022).
///
/// The walk keeps a C *lvalue* for the place reached so far rather than an address,
/// because a field step is `.member` on an lvalue and `&` is only needed where a
/// primitive takes one. For `g.rows[0].cells[0] @ 7`:
///
/// ```text
///   h0_g                                        the slot, ours already
///   h0_g.f_rows                                 a field: no unshare, we own the record
///   hero_array_unshare(&(h0_g.f_rows));         an array step: make THIS level unique
///   (*(h_m_Row *)hero_array_at_mut(h0_g.f_rows, t12))          descend into the element
///   ....f_cells                                 a field of the element
///   hero_array_unshare(&(....f_cells));         the second array step, unique too
///   hero_array_set(&(....f_cells), t13, &t14);  and the write, which unshares again
/// ```
///
/// The last step's `set` unshares a third time and that is deliberate: it is a
/// refcount test, it is idempotent, and having one entry point that cannot be reached
/// without it beats an emitter that has to remember.
pub(super) fn write_element(
    types: &Types,
    function: &Function,
    at: Place,
    value: ValueId,
) -> Option<Vec<String>> {
    let slot = &function.slots[at.root.0 as usize];
    let mut lvalue = mangle::slot(at.root.0, &slot.name);
    let mut ty = slot.ty;
    let steps = function.steps_of(at.path);
    let mut lines: Vec<String> = Vec::new();
    for (position, step) in steps.iter().enumerate() {
        let last = position + 1 == steps.len();
        match step {
            Step::Field(index) => {
                let (member, next) = types.field(ty, *index)?;
                lvalue = format!("{lvalue}.{member}");
                ty = next;
                // A field is not an indirection: the record is inside a place we
                // already own, so nothing to unshare. A trailing field store is
                // rule 3's plain assignment and never reaches this function.
                if last {
                    return None;
                }
            }
            Step::Index(index) => {
                let element = match types.checked.types.get(ty) {
                    Ty::Array(element) => element,
                    _ => return None,
                };
                if last {
                    lines.push(format!(
                        "hero_array_set(&({lvalue}), {}, &{});",
                        mangle::value(index.0),
                        mangle::value(value.0)
                    ));
                    return Some(lines);
                }
                lines.push(format!("hero_array_unshare(&({lvalue}));"));
                let spelling = super::ctype::c_type(types.names, types.checked, element)?;
                lvalue = format!(
                    "(*({spelling} *)hero_array_at_mut({lvalue}, {}))",
                    mangle::value(index.0)
                );
                ty = element;
            }
        }
    }
    None
}

// --- `T?` -----------------------------------------------------------------

/// `ok(x)`, `fail(code, msg)` and the `err` that `?` produces (design.md §4.6).
///
/// One compound literal each, and the tag is written explicitly even though C would
/// zero it for `ok`: the two sides of a `T?` are the whole point of the type, and a
/// reader of the generated C should not have to know C's initialiser rules to see
/// which one this is.
pub(super) fn construct_option(
    types: &Types,
    result: TyId,
    shape: crate::ir::Shape,
    arguments: &[String],
) -> Option<String> {
    use crate::ir::Shape;
    let name = types.names.option_of(result);
    match shape {
        Shape::Ok => {
            let value = arguments.first();
            match value {
                Some(text) => Some(format!("({name}){{.tag = INT64_C(0), .as.ok = {text}}}")),
                // `ok(())` — a unit payload has no member at all, so the tag is the
                // whole value.
                None => Some(format!("({name}){{.tag = INT64_C(0)}}")),
            }
        }
        Shape::Fail => {
            let code = arguments.first()?;
            let msg = arguments.get(1)?;
            Some(format!(
                "({name}){{.tag = INT64_C(1), .as.err = {{.code = {code}, .msg = {msg}}}}}"
            ))
        }
        // `?` propagates the failure UNCHANGED into the caller's `T?`: nothing re-reads
        // its code and msg on the way (§4.6), so this is one struct assignment.
        Shape::Err => {
            let failure = arguments.first()?;
            Some(format!("({name}){{.tag = INT64_C(1), .as.err = {failure}}}"))
        }
        _ => None,
    }
}

/// `t16 = t15.tag;` for a `T?`, the same member a variant uses.
pub(super) fn option_tag(base: ValueId) -> String {
    format!("{}.tag", mangle::value(base.0))
}

/// `.as.ok` for case 0 and `.as.err` for case 1 — `ir/inst.rs`'s own numbering.
pub(super) fn option_payload(base: ValueId, case: u32) -> String {
    let member = if case == 0 { "ok" } else { "err" };
    format!("{}.as.{member}", mangle::value(base.0))
}

// --- `{K: V}` -------------------------------------------------------------

/// `t5 = hero_map_new(&hero_desc_str, &hero_desc_int, 2);` plus one `put` per pair.
///
/// The arguments alternate key, value (`ir/inst.rs`'s own shape for `Shape::Map`), and
/// both go in **by address**: the runtime copies them through their descriptors, which
/// is the only way one function can insert a `str` key and a `Point` value.
pub(super) fn build_map(
    types: &Types,
    result: TyId,
    arguments: &[String],
    into: &str,
) -> Option<Vec<String>> {
    let (key, value) = match types.checked.types.get(result) {
        Ty::Map(key, value) => (key, value),
        _ => return None,
    };
    let key_desc = super::descriptors::pointer(types.checked, types.names, key)?;
    let value_desc = super::descriptors::pointer(types.checked, types.names, value)?;
    let pairs = arguments.len() / 2;
    let mut lines = vec![format!("{into} = hero_map_new({key_desc}, {value_desc}, {pairs});")];
    for pair in arguments.chunks(2) {
        if pair.len() == 2 {
            lines.push(format!("hero_map_put({into}, &{}, &{});", pair[0], pair[1]));
        }
    }
    Some(lines)
}

/// `m[k]`, which is **not** `xs[i]`: it yields a `V?` and cannot abort (§4.9).
///
/// The runtime hands back the value's address or NULL, because it cannot build the
/// option — that struct is generated per payload type. So the wrapping is here, and the
/// found value is copied through its descriptor rather than assigned: the map still
/// owns its copy, and the `V?` needs one of its own.
pub(super) fn map_get(
    types: &Types,
    function: &Function,
    map: ValueId,
    key: ValueId,
    result: TyId,
    into: &str,
) -> Option<Vec<String>> {
    let value = match types.checked.types.get(function.value_type(map)) {
        Ty::Map(_, value) => value,
        _ => return None,
    };
    let option = types.names.option_of(result);
    let mut lines = vec![
        "{".to_string(),
        format!(
            "  const void *found = hero_map_find({}, &{});",
            mangle::value(map.0),
            mangle::value(key.0)
        ),
        "  if (found == NULL) {".to_string(),
        format!(
            "    {into} = ({option}){{.tag = INT64_C(1), .as.err = hero_failure_missing_key()}};"
        ),
        "  } else {".to_string(),
        format!("    {into}.tag = INT64_C(0);"),
    ];
    // A unit value type has no `ok` member at all (`ctype.rs`'s unit rule), so the tag
    // is the whole answer.
    if super::ctype::c_type(types.names, types.checked, value).is_some() {
        let desc = super::descriptors::pointer(types.checked, types.names, value)?;
        lines.push(format!("    ({desc})->copy(&{into}.as.ok, found);"));
    }
    lines.push("  }".to_string());
    lines.push("}".to_string());
    Some(lines)
}
