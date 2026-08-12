//! Control flow and fallibility: what `return` owes the signature, where a jump
//! is meaningful, what `?` needs, and the difference between a branch that
//! *cannot* produce a value and one that simply did not.
//!
//! That last distinction is the whole of panel 017 A, seen from the message side:
//! `no_value` fires where every branch jumps, `branch_without_value` where a
//! branch falls off the end of a statement. Collapsing them would have let a
//! binding take its value from nothing.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::Span;


/// `inner` is the operand's own span, so the fix deletes exactly what lies
/// between the operand's end and the expression's — the `?`, and any space
/// before it. Both extents come from the expression in hand; neither rests on
/// the `?` being the last character (CLAUDE.md §11).
///
/// **The fix is `Certain` because the message already was.** Panel 036 deleted
/// the spec's sentence about `?` on a non-fallible value on the grounds that the
/// compiler says it better — and a message that says "remove the `?`" while
/// `--apply` cannot is not saying it better, it is asking for the same edit twice
/// (CLAUDE.md §8).
pub(in crate::types) fn not_fallible(got: &str, span: Span, inner: Span) -> Diagnostic {
    let mut diagnostic = Diagnostic::new(
        "not_fallible",
        format!("`?` propagates an error, and `{got}` cannot fail — remove the `?`"),
        span,
    );
    diagnostic.fixes.push(Fix {
        title: "remove the `?`".to_string(),
        replacement: String::new(),
        span: Span { start: inner.end, end: span.end },
        certainty: Certainty::Certain,
    });
    diagnostic
}


pub(in crate::types) fn try_in_infallible(result: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "try_in_infallible",
        format!(
            "`?` hands the error to the caller, so this function's result must be fallible — it is `{result}`, not `{result}?`"
        ),
        span,
    )
}


pub(in crate::types) fn not_indexable(got: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "not_indexable",
        format!("`{got}` cannot be indexed — `s[i]`, `xs[i]` and `m[k]` are for `str`, `[T]` and `{{K: V}}`"),
        span,
    )
}


pub(in crate::types) fn jump_outside_loop(word: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "jump_outside_loop",
        format!("`{word}` is only meaningful inside a `while` or a `for`"),
        span,
    )
}


/// Panel 014's value rule, and the llm-ergonomist's finding that made it: an
/// `i64`-valued `match` whose every arm jumps produces nothing, and today that
/// is silent under both readings of the arm-body question.
pub(in crate::types) fn no_value(what: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "no_value",
        format!(
            "this `{what}` produces no value — every branch jumps, so there is nothing to bind"
        ),
        span,
    )
}


/// A branch that falls off its end without a value. Distinct from a diverging
/// branch, and the distinction is the point: a jump *cannot* produce a value, a
/// statement simply did not.
pub(in crate::types) fn branch_without_value(span: Span) -> Diagnostic {
    Diagnostic::new(
        "no_value",
        "this branch ends on a statement, so it produces no value — its last line has to be the value"
            .to_string(),
        span,
    )
}


pub(in crate::types) fn missing_value(result: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "missing_value",
        format!("this function returns `{result}`, so `return` needs a value"),
        span,
    )
}


// --- statements -------------------------------------------------------

pub(in crate::types) fn returns_nothing(span: Span) -> Diagnostic {
    Diagnostic::new(
        "returns_nothing",
        "this function returns nothing, so `return` takes no value".to_string(),
        span,
    )
}


pub(in crate::types) fn not_iterable(got: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "not_iterable",
        format!(
            "`for x in …` walks an array, and this is a `{got}` — a string's characters come from `s.chars()`, a count from `range(a, b)`"
        ),
        span,
    )
}


pub(in crate::types) fn if_without_else(span: Span) -> Diagnostic {
    Diagnostic::new(
        "if_without_else",
        "an `if` used as a value needs an `else`, or it has no value when the condition is false"
            .to_string(),
        span,
    )
}


/// A function that can run off its end (§4.7; panel 020).
///
/// The span is the **written result type**, which is what rustc points at for the
/// same mistake and for the same reason: the promise is what was broken, and the
/// promise is on that line. Pointing at the last statement instead would say "this
/// line is wrong" about a line that may be perfectly correct — the missing one is the
/// one that is not there.
///
/// No fix is offered. There are two repairs (add a `return`, or make the last
/// statement the value) and neither is `certain`: which one preserves the author's
/// meaning depends on what the function is for, and CLAUDE.md §8 says only `certain`
/// fixes are machine-applicable. A `guess` that rewrites control flow is worse than
/// no fix at all.
pub(in crate::types) fn missing_return(name: &str, want: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "missing_return",
        format!("`{name}` must return `{want}`, and one path through it returns nothing"),
        span,
    )
    .with_note(
        "every path must end in a `return`, or the last statement must be the value".to_string(),
    )
}
