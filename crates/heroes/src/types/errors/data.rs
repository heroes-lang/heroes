//! Fields, labels, cases and patterns — the messages about *shapes*.
//!
//! Two of them carry `Certain` fixes, and both are renames the compiler knows are
//! right: a wrong field label when the position says which field it is, and a
//! missing one when every field must be named anyway (§4.9).
//!
//! `no_field_and_no_function` is the message the compiler-engineer's panel-015
//! veto bought: with the receiver's type in hand, both halves of §4.11's lookup
//! fit in one diagnostic instead of two passes each guessing one.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::Span;


pub(in crate::types) fn not_a_record(got: &str, field: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "no_such_field",
        format!("`{got}` has no fields, so `.{field}` cannot be read from it"),
        span,
    )
}


pub(in crate::types) fn no_such_field(
    holder: &str,
    field: &str,
    known: &[String],
    span: Span,
) -> Diagnostic {
    let list: Vec<String> = known.iter().map(|f| format!("`{f}`")).collect();
    let tail = if list.is_empty() {
        String::new()
    } else {
        format!(" — `{holder}` has {}", list.join(", "))
    };
    Diagnostic::new(
        "no_such_field",
        format!("`{holder}` has no field `{field}`{tail}"),
        span,
    )
}


/// The combined message the compiler-engineer's panel-015 veto asked for: with
/// the receiver's type in hand, both halves of §4.11's lookup are answerable in
/// one diagnostic instead of two passes each guessing one.
pub(in crate::types) fn no_field_and_no_function(
    holder: &str,
    name: &str,
    span: Span,
) -> Diagnostic {
    Diagnostic::new(
        "unknown_function",
        format!(
            "`{holder}` has no field `{name}`, and no function is named `{name}` — `x.{name}(…)` means `{name}(x, …)`"
        ),
        span,
    )
}


// --- fields, labels and markers ---------------------------------------

pub(in crate::types) fn field_of_variant(holder: &str, field: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "no_such_field",
        format!(
            "`{holder}` is a variant, so `.{field}` belongs to one of its cases — reach it through `match`"
        ),
        span,
    )
}


pub(in crate::types) fn missing_fields(
    holder: &str,
    fields: &[String],
    declared_at: &str,
    span: Span,
) -> Diagnostic {
    let list: Vec<String> = fields.iter().map(|f| format!("{f}:")).collect();
    Diagnostic::new(
        "missing_fields",
        format!(
            "`{holder}` is built with every field, named: {} — all of them, always",
            list.join(", ")
        ),
        span,
    )
    .with_note(format!("`{holder}` is declared at {declared_at}"))
}


pub(in crate::types) fn wrong_label(holder: &str, written: &str, expected: &str, span: Span) -> Diagnostic {
    let mut diagnostic = Diagnostic::new(
        "wrong_label",
        format!("`{holder}` has nothing called `{written}` at this position — it is `{expected}`"),
        span,
    );
    // The span is the label's *identifier* — the `:` is not in it — so the
    // replacement is the bare name. Applying `y:` here produced `y:: 2`, which
    // the `.fixed` goldens caught the first time they ran: a `certain` fix is a
    // claim that the result compiles, and only running it proves the claim.
    diagnostic.fixes.push(Fix {
        title: format!("write `{expected}:`"),
        replacement: expected.to_string(),
        span,
        certainty: Certainty::Certain,
    });
    diagnostic
}


/// §4.9's same-typed-argument rule. The message names the *reason* — two
/// parameters share a type — because the rule looks arbitrary without it, and a
/// model that does not know why will drop the label again at the next call.
pub(in crate::types) fn needs_label(
    name: &str,
    expected: &str,
    shared: &str,
    span: Span,
) -> Diagnostic {
    let mut diagnostic = Diagnostic::new(
        "needs_label",
        format!(
            "two of `{name}`'s parameters are `{shared}`, so every one of them is named at the call site — this one is `{expected}`"
        ),
        span,
    );
    diagnostic.fixes.push(Fix {
        title: format!("write `{expected}: `"),
        replacement: format!("{expected}: "),
        span: Span { start: span.start, end: span.start },
        certainty: Certainty::Certain,
    });
    diagnostic
}

pub(in crate::types) fn missing_label(holder: &str, expected: &str, span: Span) -> Diagnostic {
    let mut diagnostic = Diagnostic::new(
        "missing_label",
        format!("`{holder}`'s fields are always named — this one is `{expected}`"),
        span,
    );
    diagnostic.fixes.push(Fix {
        title: format!("write `{expected}: `"),
        replacement: format!("{expected}: "),
        span: Span { start: span.start, end: span.start },
        certainty: Certainty::Certain,
    });
    diagnostic
}


/// §4.8: the marker is on the parameter *and* at the call site, so a mutating
/// call cannot look like a reading one.
pub(in crate::types) fn marker_mismatch(name: &str, wants: bool, span: Span) -> Diagnostic {
    let message = if wants {
        format!("`{name}` changes this argument, so the call site writes `@` too")
    } else {
        format!("`{name}` does not change this argument — the `@` says it does")
    };
    let mut diagnostic = Diagnostic::new("marker_mismatch", message, span);
    if wants {
        diagnostic.fixes.push(Fix {
            title: "mark the argument: `@x`".to_string(),
            replacement: "@".to_string(),
            span: Span { start: span.start, end: span.start },
            certainty: Certainty::Certain,
        });
    }
    diagnostic
}


pub(in crate::types) fn ufcs_on_mutable(name: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "ufcs_on_mutable",
        format!(
            "`{name}` changes its first argument, so it is called `{name}(@x, …)` — the dotted form would hide the `@`"
        ),
        span,
    )
}


// --- patterns ---------------------------------------------------------

pub(in crate::types) fn not_matchable(got: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "not_matchable",
        format!("`{got}` has no cases to match on"),
        span,
    )
}


pub(in crate::types) fn case_on_non_variant(got: &str, case: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "not_matchable",
        format!("`.{case}` is a variant case, and this value is a `{got}`"),
        span,
    )
}


pub(in crate::types) fn no_such_case(
    holder: &str,
    case: &str,
    known: &[String],
    span: Span,
) -> Diagnostic {
    let list: Vec<String> = known.iter().map(|c| format!("`.{c}`")).collect();
    Diagnostic::new(
        "no_such_case",
        format!("`{holder}` has no case `.{case}` — it has {}", list.join(", ")),
        span,
    )
}


pub(in crate::types) fn duplicate_arm(case: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "duplicate_arm",
        format!("`.{case}` is already covered by an earlier arm"),
        span,
    )
}


/// §4.7's ban, with the reason attached: it is what keeps exhaustiveness from
/// becoming theatre when a case is added later.
pub(in crate::types) fn wildcard_on_variant(got: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "wildcard_on_variant",
        format!(
            "`_` is not allowed as an arm over `{got}` — name every case, so that adding one breaks this `match` instead of silently falling through"
        ),
        span,
    )
}


pub(in crate::types) fn non_exhaustive(missing: &[String], span: Span) -> Diagnostic {
    let list: Vec<String> = missing.iter().map(|c| format!("`{c}`")).collect();
    Diagnostic::new(
        "non_exhaustive",
        format!("this `match` does not cover {}", list.join(", ")),
        span,
    )
}


pub(in crate::types) fn needs_wildcard(span: Span) -> Diagnostic {
    Diagnostic::new(
        "non_exhaustive",
        "matching on `int` or `str` cannot be exhaustive, so it needs a `_` arm".to_string(),
        span,
    )
}
