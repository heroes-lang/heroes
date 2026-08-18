//! Every name error the resolver can produce, in one file — because they are
//! one diagnostic class and design.md §4.17 judges them together: the error
//! carries what is needed to fix the program without opening another file.
//!
//! Two shapes of repair, and the line between them is §4.17's:
//!
//! - **`Certain`**, machine-applicable: renaming to the *one* candidate in
//!   scope, and renaming an unread binding to `_`. Both preserve meaning, and
//!   the compiler is the one that knows the candidate is unique.
//! - **`Guess`**, prose for the reader: everything whose repair depends on
//!   intent — removing a parameter (its call sites are elsewhere), deleting a
//!   binding, or writing the type on a cell the resolver cannot infer, because
//!   at M-name-resolution no type exists yet.
//!
//! Wording follows the lexer's house style: what is wrong, then an em-dash
//! clause that teaches the rule, then the repair. No section numbers — the
//! reader is a model with one file, not a language lawyer.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::Span;

use super::LocalKind;

#[path = "errors_modules.rs"]
mod modules;
pub(super) use modules::*;


// --- names that resolve to nothing -------------------------------------

pub(super) fn unknown_name(name: &str, near: &[String], span: Span) -> Diagnostic {
    let mut diagnostic = Diagnostic::new(
        "unknown_name",
        format!("nothing named `{name}` is in scope"),
        span,
    );
    suggest(&mut diagnostic, near, span);
    diagnostic
}

/// `x.f(y)` is `f(x, y)` and nothing else (§4.11: there are no methods), so
/// an unknown name after the dot is answerable now — before types exist.
pub(super) fn unknown_function(name: &str, near: &[String], span: Span) -> Diagnostic {
    let mut diagnostic = Diagnostic::new(
        "unknown_function",
        format!(
            "no function named `{name}` — `x.{name}(…)` means `{name}(x, …)`, so `{name}` has to be a top-level function or a built-in"
        ),
        span,
    );
    suggest(&mut diagnostic, near, span);
    diagnostic
}

pub(super) fn unknown_type(name: &str, near: &[String], span: Span) -> Diagnostic {
    let mut diagnostic =
        Diagnostic::new("unknown_type", format!("no type named `{name}`"), span);
    suggest(&mut diagnostic, near, span);
    diagnostic
}

pub(super) fn not_a_type(name: &str, what: &str, span: Span) -> Diagnostic {
    Diagnostic::new("not_a_type", format!("`{name}` is {what}, not a type"), span)
}

/// The name exists and is not a function, so "unknown" would be a lie and a
/// did-you-mean would be noise: say what it is instead.
pub(super) fn not_a_function(name: &str, what: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "not_a_function",
        format!("`{name}` is {what}, so `x.{name}(…)` cannot call it"),
        span,
    )
}

/// A variant name in value position. The cases are the values (§4.5's ⇐
/// mode), which is exactly what a model carrying a Rust prior forgets.
pub(super) fn variant_in_value_position(name: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "variant_in_value_position",
        format!(
            "`{name}` is a variant, so it names a type and not a value — write one of its cases, `.case` or `.case(field: value)`"
        ),
        span,
    )
}

// --- names declared more than once ------------------------------------

pub(super) fn declared_twice(name: &str, first: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "declared_twice",
        format!(
            "`{name}` is already declared at {first} — one file is one module, and a name means one thing in it"
        ),
        span,
    )
}

pub(super) fn shadowed(name: &str, bound_at: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "shadowed_binding",
        format!(
            "`{name}` is already in scope, bound at {bound_at} — shadowing is an error here: a name means one thing for as long as it is visible"
        ),
        span,
    )
}

pub(super) fn shadows_top_level(name: &str, declared_at: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "shadowed_binding",
        format!(
            "`{name}` is the name of the declaration at {declared_at}, which is in scope everywhere — pick another name"
        ),
        span,
    )
}

pub(super) fn builtin_name_taken(name: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "builtin_name_taken",
        format!(
            "`{name}` is a built-in of the language, so the name is taken everywhere — pick another name"
        ),
        span,
    )
}

pub(super) fn duplicate_pattern_binding(name: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "duplicate_pattern_binding",
        format!(
            "`{name}` is bound by two patterns of the same arm — one arm binds each name once"
        ),
        span,
    )
}

// --- bindings nobody reads --------------------------------------------

/// §4.4's unused rule, and the escape valve is the wildcard: where renaming to
/// `_` is legal it is also *certain*, because it changes nothing but the name.
pub(super) fn unused(name: &str, kind: LocalKind, writes: u32, span: Span) -> Diagnostic {
    let message = match kind {
        LocalKind::Param => format!(
            "the parameter `{name}` is never read — remove it from the signature, or write `???` in the body while the function is unfinished"
        ),
        LocalKind::Bind => {
            format!("`{name}` is bound and never read — remove the binding, or read it")
        }
        LocalKind::Cell if writes > 0 => format!(
            "`{name}` is written and never read — a cell nothing reads is a value nobody uses"
        ),
        LocalKind::Cell => {
            format!("`{name}` is declared and never read — remove it, or read it")
        }
        LocalKind::Loop => format!(
            "the loop variable `{name}` is never read — write `for _ in …` when the element is not needed"
        ),
        LocalKind::Payload => format!(
            "the payload `{name}` is never read — write `_` in the pattern when the value is not needed"
        ),
    };
    let mut diagnostic = Diagnostic::new("unused_binding", message, span);
    if matches!(kind, LocalKind::Loop | LocalKind::Payload) {
        diagnostic.fixes.push(Fix {
            title: format!("rename `{name}` to `_`"),
            replacement: "_".to_string(),
            span,
            certainty: Certainty::Certain,
        });
    }
    diagnostic
}

// --- writes to something that is not a cell ---------------------------

pub(super) fn not_mutable(name: &str, kind: LocalKind, span: Span) -> Diagnostic {
    let message = match kind {
        LocalKind::Param => format!(
            "`{name}` is a parameter without `@`, so it cannot be written — mark it `@{name}` in the signature and at every call site (copy in, copy out)"
        ),
        LocalKind::Loop => format!(
            "`{name}` is the loop variable, and it is bound afresh for each element — write to a cell declared outside the loop"
        ),
        LocalKind::Payload => format!(
            "`{name}` is bound by the pattern, and a pattern binding is not a cell — bind what you need with `@` outside the `match`"
        ),
        _ => format!(
            "`{name}` was bound with `=`, which binds once, forever — a cell is declared with its type: `{name}: <type> @ <value>`"
        ),
    };
    let mut diagnostic = Diagnostic::new("not_mutable", message, span);
    if matches!(kind, LocalKind::Bind) {
        // The type is the missing half and M-name-resolution has no types: prose, not a fix
        // a tool may apply (§4.17).
        diagnostic.fixes.push(Fix {
            title: format!("declare `{name}` as a cell: `{name}: <type> @ <value>`"),
            replacement: String::new(),
            span,
            certainty: Certainty::Guess,
        });
    }
    diagnostic
}

pub(super) fn no_mutable_globals(name: &str, what: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "no_mutable_globals",
        format!(
            "`{name}` is a {what}, and there are no mutable globals — pass what changes as an `@` parameter"
        ),
        span,
    )
}

// --- did you mean -----------------------------------------------------

/// One candidate → a `Certain` rename (§4.17 names this case explicitly).
/// Several → the names, in prose, and no fix: a tool that picks one of three is
/// guessing, and only `certain` fixes may be applied.
fn suggest(diagnostic: &mut Diagnostic, near: &[String], span: Span) {
    match near.len() {
        0 => {}
        1 => {
            diagnostic.message.push_str(&format!(" — did you mean `{}`?", near[0]));
            diagnostic.fixes.push(Fix {
                title: format!("rename to `{}`", near[0]),
                replacement: near[0].clone(),
                span,
                certainty: Certainty::Certain,
            });
        }
        _ => {
            let list: Vec<String> = near.iter().map(|c| format!("`{c}`")).collect();
            diagnostic
                .message
                .push_str(&format!(" — the closest names in scope are {}", list.join(", ")));
        }
    }
}

/// Candidates within the edit distance the name's length allows, capped at three
/// and in the order the caller supplied (innermost scope first), so the list is
/// deterministic — §4.16's rule for hole suggestions, applied to every list the
/// compiler prints.
///
/// A difference in **case only** wins alone, and that is not a tie-breaker: the
/// letters are already right, so it is the one repair that can be handed over as
/// certain even when three other names are one edit away. `point` for `Point` is
/// also one edit from `print`.
pub(super) fn nearest(name: &str, candidates: &[String]) -> Vec<String> {
    let lowered = name.to_lowercase();
    if let Some(exact) = candidates.iter().find(|c| *c != name && c.to_lowercase() == lowered)
    {
        return vec![exact.clone()];
    }
    let allowed = if name.chars().count() < 4 { 1 } else { 2 };
    let mut near: Vec<String> = Vec::new();
    for candidate in candidates {
        if candidate == name || near.iter().any(|c| c == candidate) {
            continue;
        }
        if distance(name, candidate) <= allowed {
            near.push(candidate.clone());
        }
        if near.len() == 3 {
            break;
        }
    }
    near
}

/// Levenshtein distance, two rows. Names are short; this is not a hot path.
fn distance(a: &str, b: &str) -> usize {
    let a: Vec<char> = a.chars().collect();
    let b: Vec<char> = b.chars().collect();
    let mut previous: Vec<usize> = (0..=b.len()).collect();
    let mut current = vec![0usize; b.len() + 1];
    for (i, ca) in a.iter().enumerate() {
        current[0] = i + 1;
        for (j, cb) in b.iter().enumerate() {
            let cost = usize::from(ca != cb);
            current[j + 1] =
                (previous[j] + cost).min(previous[j + 1] + 1).min(current[j] + 1);
        }
        previous.clone_from(&current);
    }
    previous[b.len()]
}
