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
use crate::syntax::{Decl, DeclKind};

use super::fmt::Fmt;

/// `extern "sqlite3.h" link "sqlite3"` — the group's head line, rebuilt from any
/// one of its members, since every member carries both spans (§4.19).
pub(super) fn extern_head(src: &Source, decl: &Decl) -> String {
    let (header, link) = extern_spans(decl);
    let mut out = String::from("extern ");
    if let Some(header) = header {
        out.push_str(src.slice(header)); // quotes included
    }
    if let Some(link) = link {
        out.push_str(" link ");
        out.push_str(src.slice(link));
    }
    out
}

/// The two spans a group's members all carry, whatever kind of member they are
/// (§4.19, panel 038). **One reader**, so `function` and `constant` cannot drift
/// apart on where a group begins.
fn extern_spans(decl: &Decl) -> (Option<Span>, Option<Span>) {
    match &decl.kind {
        DeclKind::Function(function) => (function.header, function.link),
        DeclKind::Constant { header, link, .. } => (*header, *link),
        _ => (None, None),
    }
}

/// The header and library a declaration belongs to, as text — `None` for
/// anything that is not an `extern`. Compared by **text, not by span**: two
/// members of one group have different spans and the same words.
pub(super) fn extern_group<'a>(
    src: &'a Source,
    decl: &Decl,
) -> Option<(&'a str, Option<&'a str>)> {
    let (header, link) = extern_spans(decl);
    Some((src.slice(header?), link.map(|span| src.slice(span))))
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
        continues: bool,
    ) -> usize {
        let Some(header) = header else { return 0 };
        if !continues {
            self.line(0, &extern_head(src, decl));
            self.last_line = src.line_of(header.start);
            self.comments_before(src, comments, src.line_of(decl.name.start), 4);
        }
        4
    }
}
