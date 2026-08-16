//! What `sort` can order, and how the refusal explains itself
//! (spec § Strings, arrays, maps; **panel 068 R2**, ratified 2026-08-16).
//!
//! **The rule.** `sort` orders an array whose element is one of twelve types:
//! the eight integer widths, the two floats, `str`, and `bool`. Anything else is
//! `unordered_element` at exit 1, on the author's own line.
//!
//! **Why the refusal lives here and not in the emitter.** It used to be in
//! `emit/builtins.rs`, which meant `heroes check` said a program was fine and
//! `heroes build` then refused it with *"no change to this file will fix this"* —
//! a sentence that is true of an unsupported form and false of this one, since
//! changing the element type fixes it exactly. Panel 068 moved it: a program the
//! checker accepts must build.
//!
//! **Why the set is these twelve and not "types with `<`".** They are the rows
//! `hero_cmp_for` dispatches on in `runtime/parts/sort.c`, and the two questions
//! come apart in both directions. `str` has **no** `<` in the surface — `"a" <
//! "b"` is `bad_operand` — and sorts. A record could be given a `<` and still have
//! no order the compiler may invent: panel 068's compiler-engineer vetoed that
//! (option A) on the ground that a record may hold a **map**, which has no
//! canonical order without sorting its own keys, or a **function value**, whose
//! address comparison C11 6.5.8p5 leaves undefined across distinct objects — so
//! the answer would move with the linker, which is the nondeterminism the fixpoint
//! exists to exclude. So this file answers *what order can the runtime produce*,
//! never *what operator does the surface offer*.
//!
//! **`bool` is in the set because this sitting put it there**, and it cost one
//! comparator and one dispatch row in `sort.c`. `false` before `true`, which is
//! the order every language with the type agrees on and the one the underlying
//! `0`/`1` already has.
//!
//! **The descent is `partial.rs`'s**, for the same reason that file gives: a
//! reader handed *"`[Line]` is none of those"* about a record they believe is made
//! of numbers has been told the verdict and not the cause. Where the element is a
//! record or a variant, this walks its fields and payloads and names the first
//! member that has no order, so the message ends at something the reader can
//! change. It never *permits* anything the twelve-type rule refuses — the walk is
//! for the note, not for the verdict.
//!
//! `sort_by(xs, less)` is priced and refused-for-now (panel 068 R4, +47…+55), so
//! nothing here says an element *cannot* be ordered — only that `sort` does not
//! order it.

use crate::source::Source;
use crate::syntax::{Ast, DeclKind, Field};

use super::table::Ty;
use super::{Checker, TyId};

/// The twelve. Total on `Ty` by construction: every other variant falls through.
pub(super) fn is_ordered(checker: &Checker, ty: TyId) -> bool {
    matches!(checker.out.types.get(ty), Ty::Int(_) | Ty::Float(_) | Ty::Str | Ty::Bool)
}

/// Whether this element type is one the checker may **refuse**, which is a
/// narrower question than whether it is ordered.
///
/// Two types are not ordered and must not be refused here, and both would be
/// silent wrong answers in opposite directions:
///
/// - **`Ty::Error`** — its diagnostic has already been reported. One mistake, one
///   message (this module's rule everywhere, `table.rs` states it on the variant).
/// - **`Ty::Generic`** — inside `function first<A>(xs: [A])`, `sort(xs)` has an
///   element type that is not yet a type. Measured before this file existed:
///   `first([3, 1, 2])` runs and prints, because the emitter's check runs after
///   monomorphisation has replaced `A` with `i64`. Refusing the generic body would
///   delete a program that works today, which is the one thing a refusal must
///   never do.
///
/// The generic *instantiation* at an unordered type is therefore still the
/// emitter's, and is queued as its own question rather than smuggled in here.
///
/// **The reason given here used to be that the frontend cannot see it, and that
/// was false** — corrected 2026-08-16, panel 082 R4, by a judge that ran the thing
/// instead of reading the pipeline. This paragraph said *"there is no third place
/// to put it that `check` would reach"*; `types/apply.rs:139-143` writes
/// `checked.instantiations.insert(span.start, resolved_args)` **inside
/// `types::check`**, so the checker already knows `A = P` — **keyed by the
/// call-site span, which is the one line the author can edit** — and both edges are
/// present when a generic calls a generic. A file-wide pass after `decls::file`
/// would reach it, beside `map_keys::check` and `ffi_decls::fixed_only_in_a_group`,
/// and it was priced at ~90–110 lines.
///
/// The honest reason the check is not here is Principle 0, not architecture: no
/// such pass is built, and `selfhost/` declares **zero** generic functions, so
/// nothing on the closure list needs it. What this file guarantees is the shape
/// panel 068 named — a `sort` whose element type is written down — and that no
/// program reaches `hero_cmp_for`'s `NULL` return.
pub(super) fn is_refusable(checker: &Checker, ty: TyId) -> bool {
    !is_ordered(checker, ty)
        && !matches!(checker.out.types.get(ty), Ty::Error | Ty::Generic(_))
}

/// What inside this type has no order, phrased for a note — `None` when the type
/// is not an aggregate, in which case the type's own name is the whole answer.
///
/// Returns a **description** rather than a `TyId` because the caller needs prose
/// and the interesting cases are the ones with no printable type at all: a map
/// reached through two records is *"a map"*, and naming its key and value types
/// would be a longer sentence about the wrong thing.
/// **Only a record or a variant produces one**, and that is the whole point of the
/// narrowing. For every other unordered element the type's own name is already the
/// complete answer — *"`[[i64]]` is none of those"* leaves nothing to explain, and
/// a note adding *"it reaches an array"* would be restating the printed type as
/// though it were a discovery. A record is the case where the reader is looking at
/// a name and cannot see what is inside it.
pub(super) fn why_unordered(
    checker: &Checker,
    ast: &Ast,
    src: &Source,
    ty: TyId,
) -> Option<String> {
    let Ty::Named(_) = checker.out.types.get(ty) else {
        return None;
    };
    within(checker, ast, src, ty, 0)
}

/// The recursion, bounded exactly as `partial::reaches_within` is bounded, and for
/// the same reason: a generic type in the arena can still be cyclic through a type
/// parameter, because this runs before monomorphisation has removed those.
///
/// The fallback at depth is `None`, which here is the safe direction and not the
/// loud one — losing the *note* costs a sentence, while losing the *verdict* would
/// let a program through. The verdict is `is_ordered`'s and never this walk's.
fn within(
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
        // The three that are the answer wherever they are found. A function value
        // and a `ptr` are addresses whose comparison is the linker's answer
        // (C11 6.5.8p5), and a map's order is the order of a walk nobody has fixed.
        Ty::Map(_, _) => Some("a map".to_string()),
        Ty::Func { .. } => Some("a function value".to_string()),
        Ty::Ptr | Ty::Cstr => Some("a pointer".to_string()),
        Ty::Named(decl) => {
            let name = src.slice(ast.decls[decl as usize].name).to_string();
            path(checker, ast, src, &name, decl, depth)
        }
        Ty::Array(_) => Some("an array".to_string()),
        Ty::Fixed(_, _) => Some("a fixed array".to_string()),
        Ty::Fallible(_) => Some("a fallible value".to_string()),
        Ty::Unit => Some("`()`".to_string()),
        _ => None,
    }
}

/// The first member of this declaration with no order, as **one dotted path and
/// one description** — `` `Line.inner.m` is a map ``.
///
/// The path is threaded down rather than assembled on the way back up, and that is
/// not a style choice: composing the sentences on the return produced
/// `` `Line.inner` is `Inner.m` is a map `` on the first nested case tried, which
/// is a sentence with two subjects and no path. A reader who cannot follow the
/// note to a field they can edit has been given the verdict twice instead of the
/// cause once.
fn path(
    checker: &Checker,
    ast: &Ast,
    src: &Source,
    prefix: &str,
    decl: u32,
    depth: u32,
) -> Option<String> {
    let step = |fields: &[Field]| -> Option<String> {
        fields.iter().find_map(|field| {
            let field_ty = checker.out.written_type(field.ty)?;
            if is_ordered(checker, field_ty) {
                return None;
            }
            let here = format!("{prefix}.{}", src.slice(field.name));
            // A record or variant field: keep descending under the longer path, so
            // the note ends at a field the reader can actually change.
            if let Ty::Named(inner) = checker.out.types.get(field_ty) {
                return path(checker, ast, src, &here, inner, depth + 1).or_else(|| {
                    // Every field of it is ordered, so the record *itself* is the
                    // cause — there is nothing further in to name.
                    Some(format!("`{here}` is `{}`, a record", checker.show(ast, src, field_ty)))
                });
            }
            Some(match within(checker, ast, src, field_ty, depth + 1) {
                Some(what) => format!("`{here}` is {what}"),
                None => format!("`{here}` is `{}`", checker.show(ast, src, field_ty)),
            })
        })
    };
    if depth > 16 {
        return None;
    }
    match &ast.decls[decl as usize].kind {
        DeclKind::Record { fields, .. } => step(fields),
        // A case payload is not a container in the surface language, which is
        // exactly why a list of container kinds would miss it — `partial.rs`
        // records the same trap.
        DeclKind::Variant { cases } => cases.iter().find_map(|case| step(&case.fields)),
        _ => None,
    }
}
