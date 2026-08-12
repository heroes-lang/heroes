//! Turning clang's verdict on an `extern` back into a Heroes diagnostic
//! (design.md §4.19, panel 036 rider 3).
//!
//! **This is the one place a clang failure is not the compiler's fault.**
//! CLAUDE.md §7 says a clang failure exits 2 and says the *compiler* is wrong,
//! and that was true while every line of the translation unit came from this
//! emitter. The return-type assertion breaks that: it is generated code whose
//! whole purpose is to fail when the **author's** declaration disagrees with the
//! real header. Left as an internal error it would print C the author never
//! wrote, name a file under `build/<hash>/`, and blame the compiler for a
//! mistake in a `.hero` file — three of the four things §4.17 exists to prevent.
//!
//! The mapping is deliberately narrow. It reads only the assertion messages this
//! emitter itself writes, matched by their exact prefix, and it recovers the
//! declaration by name from the same tree that generated them. Anything else in
//! clang's output stays what it was: a statement about this compiler.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};

/// The prefix every return-type assertion's message carries, and the contract
/// between `decls::extern_assertions` and this file. One string, two readers.
pub const ASSERTION: &str = "heroes-ffi-return ";

/// The same contract for the constancy assertion an `extern constant` carries
/// (§4.19, panel 038). A separate marker because the repair is a different one:
/// the type was right and the header does not hold a *value*.
pub const CONSTANCY: &str = "heroes-ffi-const ";

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

/// Every `extern` a clang failure blames the author for, as diagnostics pointing
/// at the `.hero` line rather than at generated C. Two classes, both narrow:
/// a declared result type the header refutes, and a name the header does not
/// have.
///
/// Empty when clang failed for any other reason — which is the common case and
/// still means the compiler is wrong.
pub fn explain(stderr: &str, ast: &Ast, src: &Source) -> Vec<Diagnostic> {
    let mut found: Vec<Diagnostic> = Vec::new();
    for line in stderr.lines() {
        if let Some(diagnostic) = wrong_type(line, ast, src) {
            push(&mut found, diagnostic);
        }
        if let Some(diagnostic) = not_constant(line, ast, src) {
            push(&mut found, diagnostic);
        }
        if let Some(diagnostic) = unknown_name(line, stderr, ast, src) {
            push(&mut found, diagnostic);
        }
    }
    found
}

/// One diagnostic per `.hero` line: clang reports a translation unit, and one
/// mistake in it can produce several lines that all mean the same thing.
fn push(found: &mut Vec<Diagnostic>, diagnostic: Diagnostic) {
    if !found.iter().any(|d: &Diagnostic| d.span == diagnostic.span) {
        found.push(diagnostic);
    }
}

/// `_Static_assert(HERO_RET_INT(sqrt(…)), "heroes-ffi-return sqrt int")` failed:
/// the header disagrees with the declared type.
///
/// Two codes from one assertion, because the same `_Generic` asks the same
/// question of a call and of a constant, and the reader's mistake is not the
/// same: a function *returns* the wrong type, a constant **is** one.
fn wrong_type(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
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
fn not_constant(line: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
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
/// it was being reported as that one — *"`sqlite3_openn` does not return `int`"*,
/// whose remedy, correcting the result type, cannot fix a name that does not
/// exist (§4.17: the error carries what is needed to repair the program).
///
/// Two spellings, because C has two: a name used as a call is an undeclared
/// *function*, a name used as a value is an undeclared *identifier* — which is
/// what a misspelled `constant` produces.
fn unknown_name(line: &str, stderr: &str, ast: &Ast, src: &Source) -> Option<Diagnostic> {
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

/// The `extern` declaration with this C name, the header it was declared under,
/// and whether it is a `constant`. `None` for a name no group declares, which is
/// how a message that only looks like one of ours is refused.
fn declaration(ast: &Ast, src: &Source, name: &str) -> Option<(crate::source::Span, String, bool)> {
    ast.decls.iter().find_map(|decl| {
        if src.slice(decl.name) != name {
            return None;
        }
        let (header, is_constant) = match &decl.kind {
            DeclKind::Function(function) => (function.header?, false),
            DeclKind::Constant { header, .. } => ((*header)?, true),
            _ => return None,
        };
        Some((decl.span, src.slice(header).trim_matches('"').to_string(), is_constant))
    })
}

/// The span of the *name* alone, which is what a fix replaces — the declaration's
/// own span covers the whole signature.
fn name_span(ast: &Ast, src: &Source, name: &str) -> Option<crate::source::Span> {
    ast.decls
        .iter()
        .find(|decl| src.slice(decl.name) == name)
        .map(|decl| decl.name)
}
