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

use crate::diagnostics::Diagnostic;
use crate::source::Source;
use crate::syntax::{Ast, DeclKind};

/// The prefix every return-type assertion's message carries, and the contract
/// between `decls::extern_assertions` and this file. One string, two readers.
pub const ASSERTION: &str = "heroes-ffi-return ";

/// Every `extern` whose declared result type the header refutes, as diagnostics
/// pointing at the `.hero` line rather than at generated C.
///
/// Empty when clang failed for any other reason — which is the common case and
/// still means the compiler is wrong.
pub fn explain(stderr: &str, ast: &Ast, src: &Source) -> Vec<Diagnostic> {
    let mut found: Vec<Diagnostic> = Vec::new();
    for line in stderr.lines() {
        let Some(rest) = line.split(ASSERTION).nth(1) else { continue };
        // `<name> <type>"` — written by the emitter, so the split is on its own
        // format rather than on a guess about clang's.
        let mut parts = rest.split_whitespace();
        let (Some(name), Some(declared)) = (parts.next(), parts.next()) else { continue };
        let declared = declared.trim_end_matches(['"', ',', ')']);
        let Some((span, header)) = declaration(ast, src, name) else { continue };
        let mut diagnostic = Diagnostic::new(
            "ffi_return_type",
            format!(
                "`{name}` does not return `{declared}` — that is what `{header}` says, and clang read it"
            ),
            span,
        );
        diagnostic = diagnostic.with_note(format!(
            "an `extern` is checked against the real header (§4.19): correct the result type, or name the header that declares this `{name}`"
        ));
        if !found.iter().any(|d: &Diagnostic| d.span == span) {
            found.push(diagnostic);
        }
    }
    found
}

/// The `extern` declaration with this C name, and the header it was declared
/// under. `None` for a name no group declares, which is how a message that only
/// looks like one of ours is refused.
fn declaration(ast: &Ast, src: &Source, name: &str) -> Option<(crate::source::Span, String)> {
    ast.decls.iter().find_map(|decl| {
        let DeclKind::Function(function) = &decl.kind else { return None };
        let header = function.header?;
        if src.slice(decl.name) != name {
            return None;
        }
        Some((decl.span, src.slice(header).trim_matches('"').to_string()))
    })
}
