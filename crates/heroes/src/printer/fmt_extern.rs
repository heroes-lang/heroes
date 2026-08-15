//! Putting an `extern` group back together (design.md §4.19; panels 036, 038).
//!
//! Split out of `fmt.rs` by the §11 sweep. The concern is one the parser created:
//! it **flattens** a group, giving every member its own copy of the head's two
//! spans, so the formatter is what decides where a group begins and ends again.
//!
//! The canonical form is **a run of consecutive members sharing a head line**, and
//! that rule is not a preference — it is the only one that survives
//! `fmt(fmt(x)) == fmt(x)`. Printing one head per member would round-trip a
//! two-signature group into two groups; re-grouping non-adjacent members would move
//! declarations past each other.
//!
//! One reader for the two spans, so `function` and `constant` cannot drift apart on
//! where a group begins — the whole reconstruction below rests on that.

use crate::source::{Source, Span};
use crate::syntax::{Decl, DeclKind, Library};

use super::fmt::Fmt;

/// `extern "sqlite3.h" link "sqlite3"` — the group's head line, rebuilt from any
/// one of its members, since every member carries both spans (§4.19).
///
/// **The two spans are the caller's, taken from the arm that destructured the
/// declaration** (panel 062), not re-derived here from the `Decl`. That is what
/// makes `fmt_decl.rs`'s bindings load-bearing: a head-line field an arm forgets
/// to pass on is an unread binding, which CI denies, instead of a head line
/// silently printed from somewhere else.
pub(super) fn extern_head(src: &Source, header: Option<Span>, library: Option<Library>) -> String {
    let mut out = String::from("extern ");
    if let Some(header) = header {
        out.push_str(src.slice(header)); // quotes included
    }
    if let Some(library) = library {
        // The word is chosen by the kind, so a `package` group cannot be printed
        // back as a `link` one — which would be `fmt` changing what the program
        // asks the machine.
        out.push_str(match library {
            Library::Link(_) => " link ",
            Library::Package(_) => " package ",
        });
        out.push_str(src.slice(match library {
            Library::Link(span) | Library::Package(span) => span,
        }));
    }
    out
}

/// The two spans a group's members all carry, whatever kind of member they are
/// (§4.19, panel 038). **One reader**, so `function` and `constant` cannot drift
/// apart on where a group begins.
///
/// **Enumerated, never `_`** (panel 060, and it was measured before it was
/// reasoned about). This match had a `_ => (None, None)` arm, so the moment
/// `record` became a group member the printer answered "not in a group" for it and
/// `heroes fmt` **hoisted it to the top level** — emitting a different program that
/// still parses, which is the worst shape a formatter's bug can take. A catch-all
/// here rests on a premise about the world (*"these are all the members there
/// are"*) and CLAUDE.md §11 says such a premise expires silently. Listing the three
/// that cannot carry a header makes the next member kind a compile error instead.
fn extern_spans(decl: &Decl) -> (Option<Span>, Option<Library>) {
    match &decl.kind {
        DeclKind::Function(function) => (function.header, function.library),
        DeclKind::Constant { header, library, .. } => (*header, *library),
        DeclKind::Record { header, library, .. } => (*header, *library),
        // A `variant` is not a C type and a `test` is not a declaration a header
        // can make, so neither is ever a group member — refused in the parser,
        // stated here.
        DeclKind::Variant { .. } | DeclKind::Test { .. } => (None, None),
    }
}

/// The header and library a declaration belongs to, as text — `None` for
/// anything that is not an `extern`. Compared by **text, not by span**: two
/// members of one group have different spans and the same words.
pub(super) fn extern_group<'a>(
    src: &'a Source,
    decl: &Decl,
) -> Option<(&'a str, Option<&'a str>)> {
    let (header, library) = extern_spans(decl);
    // The group key is the head line's *text*, so two groups that differ only in
    // `link` versus `package` are two groups — which they are.
    Some((
        src.slice(header?),
        library.map(|library| match library {
            Library::Link(span) | Library::Package(span) => src.slice(span),
        }),
    ))
}

/// The head line's header string, as a span — where the group *begins*, which is
/// earlier than any member's own span.
pub(super) fn extern_header_span(decl: &Decl) -> Option<Span> {
    extern_spans(decl).0
}

impl Fmt {
    /// The group's head line, printed by its **first** member only, and the
    /// indent every member of a group takes. `0` for a declaration that is not
    /// one (§4.19, panel 038: two kinds of member, one head line).
    ///
    /// **The first member's own comments are flushed here, after the head.** The
    /// caller keys every comment rule on the *head's* line for a first member —
    /// it has to, or §4.1's blank-line rule fires on a gap the head itself
    /// created — and the consequence was that a comment written between the head
    /// and the first member belonged to neither: too late for the caller, and
    /// flushed by whatever declaration came next, which printed it above the
    /// **second** member. Reproduced with functions alone, so it predates
    /// constants; found by panel 038's compiler-engineer, and fixed before the
    /// examples migrated onto it, since `examples/curl/main.hero` moves a
    /// four-line comment into exactly that position.
    pub(super) fn extern_head_once(
        &mut self,
        src: &Source,
        comments: &[Span],
        decl: &Decl,
        header: Option<Span>,
        library: Option<Library>,
        continues: bool,
    ) -> usize {
        let Some(header) = header else { return 0 };
        if !continues {
            self.line(0, &extern_head(src, Some(header), library));
            self.last_line = src.line_of(header.start);
            self.comments_before(src, comments, src.line_of(decl.name.start), 4);
        }
        4
    }
}
