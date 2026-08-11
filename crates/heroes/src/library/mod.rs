//! Tier 2 of design.md §1.11: the built-ins written in Heroes itself.
//!
//! `map`, `filter`, `fold`, `find`, `any`, `all` and `range` are specified as
//! ordinary Heroes functions rather than compiler magic. This module is how they
//! reach a program: `source.hero` is embedded in the compiler and **appended to
//! the user's file**, so one `Source` carries both and the whole pipeline stays
//! single-file.
//!
//! **Appended, not prepended, and that is the entire reason concatenation works
//! at all.** Every span in this compiler is a byte offset into one text, and
//! every diagnostic renders a line number from it. Put the library first and
//! every line number in every message shifts by however many lines the library
//! happens to have. Put it last and the user's offsets are untouched, while the
//! library occupies a region past the end that `Source::is_library` can name.
//!
//! Three consequences, each handled where it lands:
//!
//! - a **diagnostic** with a span in the library region is a compiler bug, not a
//!   user error, and [`Source::is_library`] is what lets the pipeline say so;
//! - a **`#line`** for a library function must not claim the user's file, or
//!   clang would point an error at a line the author cannot see. The emitter's
//!   writer carries a third file name for it;
//! - the **mangler** gives library functions their own module component, so
//!   `range` is `h_library_range` in every program rather than
//!   `h_<whatever the user called their file>_range`.
//!
//! Why not a file on disk, a cached IR, or a prebuilt object: panel 028 costed
//! all three. The short forms — a file has no analogue of the runtime's
//! `_Static_assert`, so a decoy would be undetectable; cached IR does not remove
//! the merge, because emission still needs the AST and the types; and a prebuilt
//! object dies at the first generic, where its only shape is the type erasure
//! §4.20 forbids.

use crate::source::Source;

/// The library's own source, embedded at compiler build time.
pub const SOURCE: &str = include_str!("source.hero");

/// A `Source` holding the user's program with the library appended.
///
/// The blank line between them is not cosmetic: two declarations butted
/// together would be one line, and the layout rules that make this language
/// readable are the same ones that would then misread it.
pub fn attach(name: String, user: String) -> Source {
    Source::with_library(name, user, SOURCE.to_string())
}

/// The one thing that must never happen: a diagnostic pointing into the library.
///
/// The library is shipped with the compiler and covered by its tests, so a
/// message about it is a **compiler bug**, not a program error — and it would be
/// unactionable besides, because the line it names is in a file the author
/// cannot open. Returning the offender lets the caller exit 2 and say the
/// compiler is wrong (CLAUDE.md §10's contract, §8's discipline).
///
/// The historian predicted this class would produce the first library defect,
/// ahead of name collisions, so it is checked rather than hoped for.
pub fn misplaced(diagnostics: &[crate::diagnostics::Diagnostic], src: &Source) -> Option<String> {
    let first = diagnostics.iter().find(|d| src.is_library(d.span.start))?;
    Some(format!(
        "a diagnostic landed inside the Heroes library, at its line {}: [{}] {}",
        src.library_line_of(first.span.start),
        first.code,
        first.message
    ))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::resolve::index_of;

    /// Every diagnostic a program produces, rendered one per line, with the
    /// library attached exactly as a real invocation attaches it.
    fn check(text: &str) -> Vec<String> {
        let src = attach("test.hero".to_string(), text.to_string());
        let parsed = crate::syntax::parse(&src);
        assert!(parsed.diagnostics.is_empty(), "{:?}", parsed.diagnostics);
        let resolved = crate::resolve::resolve(&parsed.ast, &src);
        let checked = crate::types::check(&parsed.ast, &resolved, &src);
        assert_eq!(misplaced(&resolved.diagnostics, &src), None, "a diagnostic landed in the library");
        assert_eq!(misplaced(&checked.diagnostics, &src), None, "a diagnostic landed in the library");
        resolved
            .diagnostics
            .iter()
            .chain(checked.diagnostics.iter())
            .map(|d| d.render_line(&src))
            .collect()
    }

    /// **Every name the library declares is in the built-in inventory**, and
    /// nothing else. Two rules depend on it: `mangle::function` decides that a
    /// reserved name belongs to `h_library_…` (panel 028 R3b's one-definition
    /// rule), and a helper declared here would take a name a user program is
    /// entitled to. Asserted rather than trusted.
    #[test]
    fn the_library_declares_only_names_the_language_reserves() {
        let declared: Vec<&str> = SOURCE
            .lines()
            .filter_map(|line| line.strip_prefix("function "))
            .filter_map(|rest| rest.split(['(', '<']).next())
            .collect();
        assert!(!declared.is_empty(), "the library declares nothing at all");
        for name in &declared {
            assert!(
                index_of(name).is_some(),
                "the library declares `{name}`, which is not a reserved built-in — \
                 a user program is entitled to that name"
            );
        }
    }

    /// `range` is an ordinary Heroes function now, so it obeys §4.11: two `int`
    /// parameters mean every call site names them. That is the rule this step
    /// collided with, and the collision is the test — the positional form the
    /// spec used to write is a compile error carrying a machine-applicable fix.
    #[test]
    fn range_is_a_library_function_and_its_labels_are_mandatory() {
        let labelled = check("function main()\n    for i in range(from: 0, to: 3)\n        print(i)\n");
        assert!(labelled.is_empty(), "{labelled:?}");

        let positional = check("function main()\n    for i in range(0, 3)\n        print(i)\n");
        assert_eq!(positional.len(), 2, "one per unlabelled argument: {positional:?}");
        assert!(positional[0].contains("needs_label"), "{positional:?}");
        assert!(positional[0].contains("`range`"), "{positional:?}");
        assert!(positional[0].ends_with("this one is `from`"), "{positional:?}");
        assert!(positional[1].ends_with("this one is `to`"), "{positional:?}");
    }

    /// Its result is `[int]` and it is a value like any other — bindable,
    /// countable, sliceable. That is what Tier 2 claims and what the tier phrase
    /// in the spec tells a reader (panel 028 R6).
    #[test]
    fn range_yields_an_ordinary_array() {
        let clean = check(
            "function main()\n    xs = range(from: 2, to: 5)\n    print(len(xs), xs[0])\n",
        );
        assert!(clean.is_empty(), "{clean:?}");
    }

    /// Every name it declares is reserved, so a user file declaring one is told
    /// the *language* took the name — never that it collides with a declaration
    /// at a line in a file they cannot open.
    #[test]
    fn a_user_redeclaring_a_library_name_is_told_the_language_took_it() {
        let said = check("function range(a: int) -> int\n    return a\n\nfunction main()\n    print(0)\n");
        assert!(said[0].contains("builtin_name_taken"), "{said:?}");
        assert!(!said.iter().any(|d| d.contains("line 2")), "no library line: {said:?}");
    }

    /// The library compiles clean **on its own terms**: attaching it to an empty
    /// program must produce no diagnostic at all. Without this the first sign of
    /// a broken library is somebody else's program failing to build.
    #[test]
    fn the_library_alone_produces_no_diagnostic() {
        let src = attach("empty.hero".to_string(), "function main()\n    print(0)\n".to_string());
        let parsed = crate::syntax::parse(&src);
        assert!(parsed.diagnostics.is_empty(), "{:?}", parsed.diagnostics);
        let resolved = crate::resolve::resolve(&parsed.ast, &src);
        assert!(resolved.diagnostics.is_empty(), "{:?}", resolved.diagnostics);
        let checked = crate::types::check(&parsed.ast, &resolved, &src);
        assert!(checked.diagnostics.is_empty(), "{:?}", checked.diagnostics);
        assert_eq!(misplaced(&checked.diagnostics, &src), None);
    }
}
