//! What a header calls the **type**: the tag it keeps in C's other namespace, and
//! the two ways naming it can go wrong (design.md §4.19; panel 074).
//!
//! Split from `ffi_record.rs` by CLAUDE.md §11, along the seam the two halves
//! already had: that file answers *what are this struct's FIELDS* — their width,
//! their sign, the ones left out, the ones that share an address — and this one
//! answers *what is this struct CALLED*. They fail differently and they are
//! repaired differently. A field mistake is fixed by changing a type on a field
//! line; a naming mistake is fixed by adding or correcting the `tag` marker, and
//! one of the two carries a `Certainty::Certain` fix that writes it for you.
//!
//! Both recognisers keep §7's narrowing: the name is recovered from clang's own
//! text and then asked whether *this program* declared it inside an `extern`
//! group, so a struct nobody declared that way stays exit 2 and the compiler's.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};

/// clang's *"must use 'struct' tag to refer to type 'Plain'"* — the header
/// declares the struct **only as a tag**, and the group's `record` did not say so
/// (panel 074).
///
/// **It matches clang's own wording rather than one of ours, and that is safe for
/// the reason `unknown_field` is**: the narrowing is `declaration()`, which asks
/// whether *this program* declared that name inside an `extern` group. A type
/// nobody declared that way stays exit 2 and the compiler's, however much the
/// message looks like ours.
///
/// The fix is **`certain`**: clang emits this only when the bare name resolves to
/// a struct tag of that same spelling, so `tag <name>` is what the header says,
/// not a guess. The author is free to rename afterwards — the whole point of the
/// marker is that the Heroes name and C's need not agree — but they need not, and
/// `heroes check --apply` gets them compiling first.
pub(super) fn missing_tag(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    let quoted = line.split("must use 'struct' tag to refer to type '").nth(1)?;
    let name = quoted.split('\'').next()?;
    let (span, header, _) = super::ffi::declaration(ast, src, name)?;
    let at = super::ffi::name_span(ast, src, name).unwrap_or(span);
    Some(
        Diagnostic::new(
            "ffi_missing_tag",
            format!(
                "`{header}` declares `{name}` as a struct **tag** and gives it no typedef, so C reaches it only as `struct {name}`"
            ),
            at,
        )
        .with_note(
            "C keeps struct tags in a namespace of their own: say which tag with `tag`, and the Heroes name stays yours — `record FileStat tag stat` binds `struct stat` beside the `function stat` that fills it".to_string(),
        )
        .with_fix(Fix {
            title: format!("name the header's tag: `tag {name}`"),
            replacement: format!("{name} tag {name}"),
            span: at,
            certainty: Certainty::Certain,
        }),
    )
}

/// clang's *"incomplete definition of type 'struct NoSuchTag'"* — the group's
/// `record` named a tag the header does not declare (panel 074).
///
/// **This is the llm-ergonomist's condition of approval, and without it the
/// marker's own typo is exit 2.** That seat approved `tag` on the ground that *"the
/// compiler checks the tag against the header, so a swapped pair is refused by
/// name rather than compiled"* — and measured before this function existed, a tag
/// with no struct behind it produced *"internal error: compiling the generated C
/// failed"*, the compiler blaming itself for the author's word. It also said which
/// way it would fall without the check: its first preference becomes `c_name`,
/// because the word alone would then carry the whole burden of preventing a swap.
///
/// The caret lands on the **tag**, not on the record's name, because the tag is
/// the token to change — and the two are different words now, which is the whole
/// point of the marker.
pub(super) fn unknown_tag(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    let quoted = line.split("incomplete definition of type 'struct ").nth(1)?;
    let tag_name = quoted.split('\'').next()?;
    let (span, header) = record_by_tag(ast, src, tag_name)?;
    Some(
        Diagnostic::new(
            "ffi_unknown_tag",
            format!(
                "`{header}` declares no `struct {tag_name}` — clang read the header and the tag is not there"
            ),
            span,
        )
        .with_note(format!(
            "`tag` names the struct tag C uses, and clang checks it against the header (§4.19): correct the spelling, or name the header that declares `struct {tag_name}`"
        )),
    )
}

/// The declaration whose `tag` is this name, and the header it was declared under.
///
/// The same `declaration()` narrowing one field over: a tag nobody wrote stays
/// exit 2 and the compiler's.
fn record_by_tag(ast: &Ast, src: &Source, tag_name: &str) -> Option<(crate::source::Span, String)> {
    ast.decls.iter().find_map(|decl| {
        let DeclKind::Record { header: Some(header), tag: Some(tag), .. } = &decl.kind else {
            return None;
        };
        if src.slice(*tag) != tag_name {
            return None;
        }
        Some((*tag, src.slice(*header).trim_matches('"').to_string()))
    })
}

/// clang's *"use of 'utag' with tag type that does not match previous
/// declaration"* — the `tag` names a **union** and the emitter spelled `struct`
/// (panel 077, predicted by its historian from the standard before anyone ran it).
///
/// **C11 6.7.2.3#1a (DR 251) is a constraint**: *"Where two declarations that use
/// the same tag declare the same type, they shall both use the same choice of
/// `struct`, `union`, or `enum`."* A constraint obliges a diagnostic, so clang
/// **must** refuse `struct utag` against a header's `union utag` — which means
/// panel 074's marker closes every **tagged** union for free, and all that was
/// missing was the wiring. It does not reach `typedef union { … } X;`, where there
/// is no tag to mismatch; that shape is `extern_union.rs`'s.
///
/// The caret lands on the **tag**, because the tag is the token whose spelling is
/// wrong, and the note says what the author must do instead — there is no `union`
/// marker in this language today, so the route is a `ptr` and C accessors.
pub(super) fn tag_is_a_union(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    if !line.contains("with tag type that does not match previous declaration") {
        return None;
    }
    let quoted = line.split("use of '").nth(1)?;
    let tag_name = quoted.split('\'').next()?;
    let (span, header) = record_by_tag(ast, src, tag_name)?;
    Some(
        Diagnostic::new(
            "ffi_tag_is_a_union",
            format!(
                "`{header}` declares `{tag_name}` as a `union`, not a struct — C keeps the two in one namespace and will not let one stand for the other"
            ),
            span,
        )
        .with_note(
            "a group's `record` is the header's STRUCT (§4.19). A union has no `record` spelling in this language: reach it as a `ptr` and read it through C functions, or bind the one member you need as its own type".to_string(),
        ),
    )
}
