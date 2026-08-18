//! A type that would have to contain one of itself at every depth (panel 023).
//!
//! Its own file because size is its own subject: `data.rs` answers "does this
//! shape have that field", and this answers "can this shape exist at all". The
//! second question is asked once per file over the declaration graph, not per
//! expression, and it was the only type error in the language whose absence made
//! the *compiler* look wrong — `record Node { child: Node }` reached clang as
//! `field has incomplete type`, which CLAUDE.md §7 reports as exit 2, "the
//! compiler is wrong".
//!
//! **The fix is a `Guess`, on published precedent.** rustc's `Box` suggestion for
//! E0072 carries `Applicability::HasPlaceholders` — its own definition is "cannot
//! be applied automatically" — and Swift, which is in Heroes' exact position with
//! no escape hatch for a value type, emits **no** fix-it for structs at all,
//! reserving one for enums where a keyword suffices. The local reason is
//! decisive: `child: Node` → `children: [Node]` changes the field's cardinality
//! *and* its name, so every use site moves with it. CLAUDE.md §8 machine-applies
//! only `Certain`, and this must never be applied without a human.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::Span;

/// `holder` contains itself; `steps` is the cycle as one line per edge.
///
/// `direct` distinguishes the two messages a reader needs. A self-containing
/// record is one line to read and the path adds nothing; a mutual cycle is
/// unreadable without it, which is why Go's old `cmd/compile` printing only
/// `invalid recursive type T2` for a mutual pair was filed as a bug.
pub(in crate::types) fn no_size(
    kind: &str,
    holder: &str,
    through: &str,
    direct: bool,
    steps: &[String],
    span: Span,
) -> Diagnostic {
    let message = if direct {
        format!("`{kind} {holder}` contains itself, so it has no size")
    } else {
        format!("`{kind} {holder}` contains itself through `{through}`, so it has no size")
    };
    let mut diagnostic = Diagnostic::new("no_size", message, span);
    if !direct {
        diagnostic = diagnostic.with_note(format!("the cycle is: {}", steps.join(", then ")));
    }
    diagnostic = diagnostic
        .with_note(format!(
            "every value is an independent copy, so `{holder}` holding one more `{holder}` \
             would need one at every depth"
        ))
        .with_note(
            "`[T]` holds its contents elsewhere, so it is how a type contains itself".to_string(),
        );
    diagnostic.fixes.push(Fix {
        title: format!("hold them in an array: `[{through}]`"),
        replacement: String::new(),
        span,
        certainty: Certainty::Guess,
    });
    diagnostic
}
