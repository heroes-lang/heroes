//! What may be a map's **key** — one walk, every rule (§4.19, spec § Strings,
//! arrays, maps; panels 061 and 069).
//!
//! **A map key is the one refusal with no operator behind it.** `==` fires on a
//! token the author typed; `m[k] @ v` fires on nothing — the hazard is in the
//! *type* `{Color: i64}`, which may be written once in an annotation and never
//! mentioned again. So the check cannot live where the operator rules do, and it
//! walks the program's written types and map literals instead.
//!
//! **It asks two questions in one pass, and that is the point of the file.** The
//! `partial` rule arrived at panel 061 and the float rule at panel 069, and a
//! second walk in a second file asking *may this be a key* is exactly the shape
//! that hid `sort`'s defect for a milestone: a rule moved, its old copy left
//! behind, and the two answers free to drift. One walk cannot drift from itself.
//!
//! **Two routes, and the second is the one a syntax rule misses.** A key type may
//! be written (`m: {Color: i64} @ {}`) or it may exist only as a literal's
//! inferred type (`m = {c: 1}`, which interns `Ty::Map` straight from the
//! entries). `partial`'s first version walked only the written nodes — its own doc
//! said *"the hazard is in the type"* and it then implemented a rule about the
//! **syntax**, written by the seat auditing for that class, twenty minutes after
//! writing the audit's brief. It was caught by a named `hero_panic` rather than by
//! a test, which is CLAUDE.md §11's loud direction paying for itself.
//!
//! **What this walk cannot see, stated rather than left implicit**: a key type
//! that is still a type *parameter*. Inside `function count<K>(ks: [K])`, the
//! annotation `m: {K: i64}` gives `Ty::Generic`, and only monomorphisation knows
//! what the call chose — measured 2026-08-16, `count([1.5, 2.5])` runs and prints
//! `2` while `count([nan])` reaches `hero_map_slot_of`'s abort. Refusing the
//! generic body would delete the working call. So the runtime guard **stays**, and
//! the float diagnostic says so in its own note: this rule removes the reachable
//! cases, and the guard covers the case a type cannot decide.

use crate::source::{Source, Span};
use crate::syntax::{Ast, DeclKind, ExprKind, Field, TypeKind};

use super::table::Ty;
use super::{errors, partial, Checker, TyId};

/// Check every map key the program writes or builds.
pub(super) fn check(checker: &mut Checker, ast: &Ast, src: &Source) {
    for (key, span) in written(checker, ast) {
        refuse(checker, ast, src, key, span);
    }
    for (key, span) in in_literals(checker, ast) {
        refuse(checker, ast, src, key, span);
    }
}

/// `m: {Color: i64} @ {}` — the annotation's own key node.
fn written(checker: &Checker, ast: &Ast) -> Vec<(TyId, Span)> {
    let mut found = Vec::new();
    for node in &ast.types {
        let TypeKind::Map(key, _) = &node.kind else { continue };
        let Some(ty) = checker.out.written_type(*key) else { continue };
        found.push((ty, ast.types[key.0 as usize].span));
    }
    found
}

/// `m = {c: 1}` — no `TypeKind::Map` node exists, so the type comes from the
/// expression table and the span from the first entry's key, which is the thing
/// the reader has to change.
fn in_literals(checker: &Checker, ast: &Ast) -> Vec<(TyId, Span)> {
    let mut found = Vec::new();
    for (index, expr) in ast.exprs.iter().enumerate() {
        let ExprKind::Map(entries) = &expr.kind else { continue };
        let Some(first) = entries.first() else { continue };
        let Ty::Map(key, _) = checker.out.types.get(checker.out.expr_types[index]) else {
            continue;
        };
        found.push((key, ast.exprs[first.key.0 as usize].span));
    }
    found
}

/// The rules, in the order a reader meets them. **At most one fires per key**:
/// a partial record reaching a float is one mistake, and §4.17 gives one mistake
/// one message.
fn refuse(checker: &mut Checker, ast: &Ast, src: &Source, key: TyId, span: Span) {
    if let Some(decl) = partial::reaches(checker, ast, key) {
        let shown = checker.show(ast, src, key);
        let name = src.slice(ast.decls[decl as usize].name).to_string();
        let diagnostic = errors::partial_operation(&name, &shown, "a map key", span);
        checker.push_diagnostic(diagnostic);
        return;
    }
    if let Some(what) = reaches_float(checker, ast, src, key, 0) {
        let shown = checker.show(ast, src, key);
        let diagnostic = errors::float_map_key(&shown, what, span);
        checker.push_diagnostic(diagnostic);
    }
}

/// Does this key type **reach** an `f32` or an `f64`?
///
/// Depth-bounded and descending exactly as `partial::reaches_within` does, for
/// the reason that file gives: a generic type in the arena can still be cyclic
/// through a type parameter, because this runs before monomorphisation.
///
/// Returns **where** it found one — an empty string when the key *is* the float,
/// a dotted path when it is inside — so the message can name a field the reader
/// can change rather than restating the type they already typed. `None` at the
/// bound is safe in the direction that matters: the runtime guard still sits
/// behind this rule, so a missed refusal costs a named abort rather than a wrong
/// answer.
fn reaches_float(
    checker: &Checker,
    ast: &Ast,
    src: &Source,
    ty: TyId,
    depth: u32,
) -> Option<String> {
    if depth > 16 {
        return None;
    }
    match checker.out.types.get(ty) {
        Ty::Float(_) => Some(String::new()),
        Ty::Named(decl) => {
            let owner = src.slice(ast.decls[decl as usize].name).to_string();
            match &ast.decls[decl as usize].kind {
                DeclKind::Record { fields, .. } => inside(checker, ast, src, &owner, fields, depth),
                DeclKind::Variant { cases } => cases
                    .iter()
                    .find_map(|case| inside(checker, ast, src, &owner, &case.fields, depth)),
                _ => None,
            }
        }
        Ty::Fixed(element, _) => reaches_float(checker, ast, src, element, depth + 1),
        // `[T]`, `{K: V}` and `T?` are not key types today — each is refused
        // before this walk runs — but the arms are here rather than folded into a
        // `_` so that the day one becomes a key, this rule does not silently stop
        // applying. A `_ =>` in the quiet direction is the defect CLAUDE.md §11
        // names twice.
        Ty::Array(element) | Ty::Fallible(element) => {
            reaches_float(checker, ast, src, element, depth + 1)
        }
        Ty::Map(k, v) => reaches_float(checker, ast, src, k, depth + 1)
            .or_else(|| reaches_float(checker, ast, src, v, depth + 1)),
        _ => None,
    }
}

/// The first field or case payload that reaches a float, as a dotted path the
/// reader can follow — threaded downward rather than composed on the return, for
/// the reason `types/ordering.rs` records: composing on the way back up produces
/// a sentence with two subjects and no path.
fn inside(
    checker: &Checker,
    ast: &Ast,
    src: &Source,
    owner: &str,
    fields: &[Field],
    depth: u32,
) -> Option<String> {
    fields.iter().find_map(|field| {
        let ty = checker.out.written_type(field.ty)?;
        let found = reaches_float(checker, ast, src, ty, depth + 1)?;
        let here = format!("{owner}.{}", src.slice(field.name));
        // **The DEEPEST owner wins, not the full path** — `Wrapper { inner: Point }`
        // reports `Point.y` rather than `Wrapper.inner.y`, and that is deliberate
        // where `types/ordering.rs` builds the path instead. The two questions
        // differ: `sort` refuses the whole value and the reader needs the route in,
        // while a map key is repaired by changing one declaration, and `Point.y`
        // names the declaration to open. A path through `Wrapper` would send them
        // to a record with nothing wrong in it.
        Some(if found.is_empty() { here } else { found })
    })
}
