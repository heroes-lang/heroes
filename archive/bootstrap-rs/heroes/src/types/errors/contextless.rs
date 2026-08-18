//! **A form the context must give a type to, and could not** (§4.5's ⇐-only list).
//!
//! Split from `values.rs` by the §11 ceiling, 2026-08-15, and the seam is the one
//! §4.5 already draws. `values.rs` reports a value whose type is **known and wrong**
//! — a mismatch, an operator that does not take it, a value standing where none may.
//! These report a form with **no type of its own**: an empty literal, a bare case
//! name, a constructor with nothing to construct against, a built-in used as a value.
//!
//! The two fail differently, which is why they read differently. A mismatch names
//! two types and the reader picks; these can name only one and have to say where the
//! missing half was supposed to come from — which is why almost every message here
//! ends by telling the author to write an annotation.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::Span;


pub(in crate::types) fn cannot_infer_empty(what: &str, span: Span) -> Diagnostic {
    let mut diagnostic = Diagnostic::new(
        "cannot_infer",
        format!(
            "the {what} is empty, so there is nothing to infer its type from — write the annotation"
        ),
        span,
    );
    diagnostic.fixes.push(Fix {
        title: "annotate the binding: `xs: [i64] = []`".to_string(),
        replacement: String::new(),
        span,
        certainty: Certainty::Guess,
    });
    diagnostic
}


// --- forms that cannot say what they are ------------------------------

/// §4.5's ⇐ mode, from the other side: a case name alone has no type until
/// something says which variant is expected.
pub(in crate::types) fn cannot_infer_case(name: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "cannot_infer",
        format!(
            "`.{name}` does not say which variant it belongs to — the type has to come from the context (a signature, an annotation, or the value being returned)"
        ),
        span,
    )
}


pub(in crate::types) fn case_not_expected(name: &str, expected: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "type_mismatch",
        format!("expected `{expected}`, found the case `.{name}`"),
        span,
    )
}


pub(in crate::types) fn constructor_not_expected(name: &str, expected: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "type_mismatch",
        format!(
            "`{name}(…)` builds a fallible value, and `{expected}` is not one — no `T` is ever promoted to a `T?`"
        ),
        span,
    )
}


/// `ok(x)` and `fail(c, m)` are ⇐-only (panel 002), so in a position with no
/// expectation there is nothing for them to build. The message says which
/// positions *do* have one, because "cannot infer" alone sends the reader
/// looking for a type annotation that does not belong on a call.
pub(in crate::types) fn constructor_needs_context(name: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "cannot_infer",
        format!(
            "`{name}(…)` builds a fallible value, and which one comes from the context — return it, bind it with an annotation, or pass it where a `T?` is expected"
        ),
        span,
    )
}

pub(in crate::types) fn record_name_alone(name: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "record_name_alone",
        format!(
            "`{name}` names a record, and a record is built by calling it with every field: `{name}(field: value, …)`"
        ),
        span,
    )
}


/// A built-in used as a value rather than called.
///
/// **Found by M-generics-library's closure-list audit, on its first program**, and it was silent
/// in the worst way: `exprs.rs` answered `error_ty()` with no diagnostic, and an
/// error type with nothing reported is poison that flows into the emitter.
/// `map(xs, to_str)` type-checked at exit 0 and then produced
/// `typedef HeroValue (*h_m_fn0)(int64_t);` — `error: type specifier missing` —
/// which CLAUDE.md §7 reserves for the compiler being wrong.
///
/// The rule already existed and only the message was missing: spec line 89 says
/// *top-level functions* are values, and a built-in is not one. Several of them
/// could not be, whatever the spec said — `print` takes any number of arguments
/// of four types and `len` works on three, which is precisely why they are
/// built-ins rather than declarations (`types/builtins.rs`'s own module doc).
///
/// The fix is a wrapper, and it is named because the reader would otherwise have
/// to invent the shape.
pub(in crate::types) fn builtin_as_value(name: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "builtin_as_value",
        format!(
            "`{name}` is a built-in of the language, not a function value — several \
             take any number of arguments, or arguments of several types, which no \
             single signature can describe"
        ),
        span,
    )
    .with_note(format!(
        "wrap it: `function {name}_of(x: …) -> …` with `return {name}(x)`, and pass that"
    ))
}

pub(in crate::types) fn variant_not_callable(name: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "variant_not_callable",
        format!("`{name}` is a variant, so a value of it is written `.case`, never `{name}(…)`"),
        span,
    )
}


pub(in crate::types) fn builtin_shape(name: &str, given: usize, span: Span) -> Diagnostic {
    Diagnostic::new(
        "builtin_shape",
        format!("`{name}` does not take {given} argument(s) of those types"),
        span,
    )
}

/// `99999999999999999999` — a literal no `i64` holds.
///
/// **The code is snake_case like every other in this compiler.** It was
/// `int-literal-out-of-range`, hyphenated, and it was the only one: a
/// user-visible identifier that did not match its family, in a project whose
/// thesis is that a reader can predict what the compiler says.
///
/// The message names the range because `i64` is the only integer type (§4.3):
/// there is no wider one to suggest, so the fix is a different number and the
/// compiler should not pretend otherwise.
/// Built in `lexer/number.rs`, with the decoder that decides when it fires and
/// the base table that lets it answer in the notation the question was asked in
/// — the frontend and the lowering both raise this diagnostic, and before panel
/// 041 they each spelled it out by hand.
pub(in crate::types) fn int_out_of_range(text: &str, span: Span) -> Diagnostic {
    crate::lexer::int_out_of_range(text, span)
}
