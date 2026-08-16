//! The types this backend can represent, one row per §4.3 table entry (panel 020
//! R1; panel 029 R4b).
//!
//! Split out of `gate.rs` by the §11 sweep. The rows died one milestone at a time,
//! which was the design: `str` and `f64` at M-strings-ownership, records and
//! variants at M-value-aggregates, `T?` and `{K: V}` at M-optional-map, function
//! types and type parameters at M-generics-library, and §4.19's `ptr` and `cstr` at
//! M-ffi-ladder — the last two, and the only ones whose row was a veto rather than
//! a schedule.
//!
//! **The table is empty of refusals today, and this is what that looks like**:
//! every arm returns, and what is left is descent into the types a container holds.
//! That descent is not tidiness — it is a filed defect. `Ty::Array(_) => return`
//! did not look at what the array held, so `xs: [()] @ []`, `xs: [ptr] @ []` and
//! `m: {str: ptr} @ {}` all checked clean at exit 0, built a binary, and aborted
//! with *"entered unreachable code — this is a compiler bug"*. The generated C said
//! so itself — `hero_unreachable(); /* not an array */` — and nobody read it.

use crate::source::{Source, Span};
use crate::syntax::Ast;
use crate::types::{Checked, Ty, TyId};

use super::gate::note;
use crate::ir::Function;

/// A type the runtime has no representation for yet. One row per §4.3 table entry,
/// so M-strings-ownership and M-value-aggregates delete rows rather than discovering cases.
///
/// **It descends into element and payload types**, and that was a filed defect
/// rather than a design: `Ty::Array(_) => return` did not look at what the array
/// held, so `xs: [()] @ []` and `xs: [ptr] @ []` and `m: {str: ptr} @ {}` all
/// checked clean at exit 0, built a binary, and aborted with `entered
/// unreachable code — this is a compiler bug`. The generated C said so itself —
/// `hero_unreachable(); /* not an array */` — and nobody read it (panel 029 R4b).
///
/// Until M-generics-library step 6 an author had to *write* `[ptr]` to reach it. After it they
/// **infer** it: `map(nums, print)` infers `B := ()`.
/// **The type table is empty of refusals as of M-ffi-ladder step 6, and this is
/// what that looks like.** Every arm returns; what is left is descent into the
/// types a container holds, because `()` as an *element* is still refused (a
/// descriptor that does not exist) while `()` as a type in its own right is
/// fine.
///
/// The rows died one milestone at a time, which was the file's whole design:
/// `str` and `f64` at M-strings-ownership, records and variants at M-value-aggregates,
/// `T?` and `{K: V}` at M-optional-map, function types and type parameters at
/// M-generics-library, and §4.19's `ptr` and `cstr` here — the last two, and the
/// only ones whose row was a veto rather than a schedule. A `ptr` local is what
/// holds a C out-parameter, so refusing it would have refused the milestone's own
/// acceptance test.
pub(super) fn check_type(
    found: &mut Vec<(String, String, Span)>,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
    function: &Function,
    ty: TyId,
    span: Span,
) {
    match checked.types.get(ty) {
        // The container emits; whether its ELEMENT does is the element's own row.
        Ty::Array(element) => {
            check_element(found, ast, checked, src, function, element, span, "an array")
        }
        Ty::Map(key, value) => {
            check_element(found, ast, checked, src, function, key, span, "a map key");
            check_element(found, ast, checked, src, function, value, span, "a map value");
        }
        // **A `()?` is representable and the other containers are not.** An array
        // or a map needs a descriptor for its element and `()` has none; a `T?` is
        // a tagged union, and a union whose ok side carries nothing is just a tag.
        // `write_file(path, text) -> ()?` is the signature that needed it.
        Ty::Fallible(payload) if checked.types.get(payload) != Ty::Unit => {
            check_element(found, ast, checked, src, function, payload, span, "a `T?`")
        }
        // Every other type in the language has a C representation (`ctype.rs`),
        // including `Ty::Error`, which the checker has already reported — one
        // mistake, one message.
        _ => {}
    }
}

/// `()` as the type of a **declared field**, which the walk above cannot see.
///
/// Everything else in this file reads the IR — slots, instruction types,
/// parameters, results — and a declaration's field list is in none of those. So
/// `record Box { u: () }` walked straight past the gate and reached clang as
/// `void f_u;`: `error: field has incomplete type 'void'`, exit 2, the compiler
/// blaming itself for the author's program, which `check_op`'s own comment calls
/// the one failure the gate exists to prevent (2026-08-12).
///
/// It is `check_element`'s rule at the other end of the same argument: `()` is a
/// perfectly good type and no kind of *member*. CLAUDE.md §7 already says a
/// unit-typed temporary is never declared at all and `void t0;` is a hard error;
/// the rule reached temporaries and not fields.
pub(super) fn unit_fields(
    found: &mut Vec<(String, String, Span)>,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
) {
    for decl in super::types::aggregates(ast, checked) {
        let owner = src.slice(ast.decls[decl as usize].name).to_string();
        let mut fields: Vec<&crate::syntax::Field> = super::types::fields_of(ast, decl).iter().collect();
        for case in super::types::cases_of(ast, decl) {
            fields.extend(case.fields.iter());
        }
        for field in fields {
            if checked.written_type(field.ty) != Some(checked.types.unit()) {
                continue;
            }
            note(
                found,
                "unit_field",
                format!("`()` as the type of `{owner}.{}`", src.slice(field.name)),
                field.name,
            );
        }
    }
}

/// What a container holds, which is a narrower question than what a type is.
///
/// **Three types are no kind of element, and each for its own reason.** `()` has
/// no C declaration, so there is no descriptor to hand the runtime. `ptr` and
/// `cstr` have a C declaration and still have no descriptor: `descriptors.rs`
/// answers `None` for both, because the runtime ships no row for a value that is
/// an address the program does not own.
///
/// **The `ptr`/`cstr` half was a dead premise until 2026-08-16**, and this doc
/// carried it: it said *"an `[[ptr]]` is refused for its `ptr` and says so
/// once"*, while `check_type`'s fallthrough let both past. Measured (panel 067,
/// ffi-pragmatist): `[ptr]`, `[cstr]`, `{i64: ptr}`, `{i64: cstr}`, `{ptr: i64}`
/// and `{cstr: i64}` all passed `heroes check` at **exit 0** and aborted at
/// **exit 134** — `hero_unreachable()` reached from a correct program, which is
/// §1.12's class and the one thing the gate exists to prevent. Panel 029 filed
/// three types; the fix covered one.
///
/// It is a **refusal rather than a feature**: the runtime could grow descriptors
/// for both, and that is a panel question with an ABI bump in it. Refusing is
/// what turns a crash into a diagnostic today, which is the loud direction
/// (CLAUDE.md §11).
pub(super) fn check_element(
    found: &mut Vec<(String, String, Span)>,
    ast: &Ast,
    checked: &Checked,
    src: &Source,
    function: &Function,
    element: TyId,
    span: Span,
    container: &str,
) {
    if checked.types.get(element) == Ty::Unit {
        note(found, "unit_element", format!("`()` as the element of {container}"), span);
        return;
    }
    // The two that have a C declaration and no descriptor. Named separately from
    // `()` because the reason differs and so does the future: `()` can never be an
    // element, while these two wait on a runtime row.
    if checked.types.get(element) == Ty::Ptr {
        note(found, "pointer_element", format!("`ptr` as the element of {container}"), span);
        return;
    }
    if checked.types.get(element) == Ty::Cstr {
        note(found, "pointer_element", format!("`cstr` as the element of {container}"), span);
        return;
    }
    check_type(found, ast, checked, src, function, element, span);
}
