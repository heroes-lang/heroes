//! The classes where **the header contradicts the declaration** (design.md §4.19;
//! panels 036, 038, 042, 048).
//!
//! Split from `ffi.rs` by the §11 sweep at 573 lines, and the seam is not size: it
//! is *whose statement the verdict is about*. These three read a `_Static_assert`
//! **this emitter wrote** — a result type the header refutes, a `constant` that is
//! not one, a name the header does not have — so the message text is a contract
//! with `extern_assert.rs` rather than a guess about clang's wording. `FAILED`
//! lives here because it belongs to that contract: clang echoes the source line
//! under its diagnostic, and that echo contains the marker verbatim (panel 038).
//!
//! The one exception is `unknown_name`, which matches clang's own *"call to
//! undeclared function"*. It is here rather than in `ffi_build.rs` because the
//! question it answers is the same one: does the header say what the author says
//! it says. What makes it safe is not whose text it reads but `declaration()` —
//! see `ffi.rs`.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::Source;
use crate::syntax::Ast;

use super::ffi::{ASSERTION, CONSTANCY, declaration, name_span};

/// What clang prints on the line of a `_Static_assert` that failed. Required
/// before a line is read as one of ours, because clang **echoes the source line
/// under its diagnostic** — and that echo contains the assertion's message text
/// verbatim, marker and all.
///
/// Without this the echo parsed as a second, malformed report of the same
/// failure: `` `sqlite3_openn` does not return `int");` ``. It was invisible
/// because the message line comes first and the duplicate was dropped by span —
/// so the good path was right by ordering rather than by matching, which is a
/// premise about clang's output order rather than a fact about the line in hand
/// (CLAUDE.md §11). Found by panel 038's ffi-pragmatist, compiling.
const FAILED: &str = "static assertion failed";

/// `_Static_assert(HERO_RET_INT(sqrt(…)), "heroes-ffi-return sqrt int")` failed:
/// the header disagrees with the declared type.
///
/// Two codes from one assertion, because the same `_Generic` asks the same
/// question of a call and of a constant, and the reader's mistake is not the
/// same: a function *returns* the wrong type, a constant **is** one.
pub(super) fn wrong_type(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    if !line.contains(FAILED) {
        return None;
    }
    let rest = line.split(ASSERTION).nth(1)?;
    // `<name> <type>` — written by the emitter, so the split is on its own
    // format rather than on a guess about clang's.
    let mut parts = rest.split_whitespace();
    let (name, declared) = (parts.next()?, parts.next()?);
    let (span, header, is_constant) = declaration(ast, src, name)?;
    let diagnostic = if is_constant {
        Diagnostic::new(
            "ffi_constant_type",
            format!(
                "`{name}` is not `{declared}` in `{header}` — clang read the header and the types disagree"
            ),
            span,
        )
        .with_note(format!(
            "an `extern constant` takes its value from the header (§4.19), so its type is the header's too: correct the declared type, or name the header that spells `{name}` as `{declared}`"
        ))
    } else {
        Diagnostic::new(
            "ffi_return_type",
            format!(
                "`{name}` does not return `{declared}` — that is what `{header}` says, and clang read it"
            ),
            span,
        )
        .with_note(format!(
            "an `extern` is checked against the real header (§4.19): correct the result type, or name the header that declares this `{name}`"
        ))
    };
    Some(diagnostic)
}

/// `_Static_assert(__builtin_constant_p(stdout), "heroes-ffi-const stdout")`
/// failed: the header has the name, and it is an **object** rather than a value.
///
/// This is the one refusal no other language makes as a rule (panel 038's
/// historian: Nim, Swift and Go all accept it, Zig refuses it by accident). The
/// reason Heroes must is §4.2's: a zero-argument accessor over `errno` returns a
/// different value on two calls, which is a mutable global arriving through the
/// back door.
pub(super) fn not_constant(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    if !line.contains(FAILED) {
        return None;
    }
    let rest = line.split(CONSTANCY).nth(1)?;
    let name = rest.split_whitespace().next()?.trim_end_matches(['"', ',', ')', '\'']);
    let (span, header, _) = declaration(ast, src, name)?;
    let diagnostic = Diagnostic::new(
        "ffi_not_constant",
        format!(
            "`{header}` has `{name}`, but not as a constant — clang cannot read its value while compiling"
        ),
        span,
    );
    Some(diagnostic.with_note(format!(
        "a `constant` names one value forever (§4.4), and `{name}` is an object or a macro that expands to one — reading it twice could give two answers, which is the mutable global §4.2 forbids. A C object is reached through an `extern function` that returns it"
    )))
}

/// The header has no such name at all. A different mistake from the one above and
/// it was being reported as that one — *"`sqlite3_openn` does not return `i64`"*,
/// whose remedy, correcting the result type, cannot fix a name that does not
/// exist (§4.17: the error carries what is needed to repair the program).
///
/// Two spellings, because C has two: a name used as a call is an undeclared
/// *function*, a name used as a value is an undeclared *identifier* — which is
/// what a misspelled `constant` produces.
pub(super) fn unknown_name(line: &str, stderr: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
    let quoted = ["call to undeclared function '", "use of undeclared identifier '"]
        .iter()
        .find_map(|marker| line.split(marker).nth(1))?;
    let name = quoted.split('\'').next()?;
    let (span, header, _) = declaration(ast, src, name)?;
    let mut diagnostic = Diagnostic::new(
        "ffi_unknown_name",
        format!("`{header}` declares no `{name}` — clang read the header and could not find it"),
        span,
    );
    diagnostic = diagnostic.with_note(format!(
        "an `extern` names what the header already has (§4.19): check the spelling, or name the header that does declare `{name}`"
    ));
    // clang's own typo correction, when it offered one. A `Guess`: it is a
    // suggestion from a search over the header's names, not a fact about this
    // program, so `--apply` must not take it (CLAUDE.md §8).
    if let Some(meant) = did_you_mean(stderr, name) {
        let Some(at) = name_span(ast, src, name) else { return Some(diagnostic) };
        diagnostic.fixes.push(Fix {
            title: format!("the header declares `{meant}`"),
            replacement: meant,
            span: at,
            certainty: Certainty::Guess,
        });
    }
    Some(diagnostic)
}

/// The name clang suggested instead. Read from the whole of stderr rather than
/// from one line, because the note sits on a line of its own under the error.
fn did_you_mean(stderr: &str, name: &str) -> Option<String> {
    for line in stderr.lines() {
        if !line.contains(&format!("'{name}'")) && !line.contains("did you mean") {
            continue;
        }
        let Some(rest) = line.split("did you mean '").nth(1) else { continue };
        let meant = rest.split('\'').next()?;
        if meant != name {
            return Some(meant.to_string());
        }
    }
    None
}
