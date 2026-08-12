//! Mismatches, operators, and the two rules about where a value may stand.
//!
//! The pair worth reading together is `discarded_value` and `bound_unit`: panel
//! 003 made a non-`()` expression alone on a line an error, and panel 017 C made
//! a `()` value in binding position the mirror of it. Between them, a value is
//! either received or explicitly discarded, and nothing is silently dropped.

use crate::diagnostics::{Certainty, Diagnostic, Fix};
use crate::source::Span;

pub(in crate::types) fn mismatch(expected: &str, got: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "type_mismatch",
        format!("expected `{expected}`, found `{got}`"),
        span,
    )
}


/// The two numeric types never mix (§4.14), and the repair is the conversion
/// the spec names — a `Guess`, because which side to convert is the author's.
pub(in crate::types) fn mixed_arithmetic(op: &str, left: &str, right: &str, span: Span) -> Diagnostic {
    let mut diagnostic = Diagnostic::new(
        "mixed_arithmetic",
        format!(
            "`{op}` takes `int` with `int` or `f64` with `f64`, never mixed — found `{left}` and `{right}`"
        ),
        span,
    );
    diagnostic.fixes.push(Fix {
        title: "convert one side: `to_f64(x)` or `to_int(x)`".to_string(),
        replacement: String::new(),
        span,
        certainty: Certainty::Guess,
    });
    diagnostic
}


pub(in crate::types) fn bad_operand(op: &str, allowed: &str, got: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "bad_operand",
        format!("`{op}` takes {allowed}, found `{got}`"),
        span,
    )
}


/// There is no truthiness (§4.14), which is worth saying in the message: a model
/// carrying a C or Python prior needs the rule, not just the mismatch.
pub(in crate::types) fn not_bool(what: &str, got: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "not_bool",
        format!(
            "{what} must be `bool`, found `{got}` — there is no truthiness in this language"
        ),
        span,
    )
}


/// Panel 003: a non-`()` expression alone on a line is an error, and the fix is
/// machine-applicable because it preserves meaning exactly.
/// `main` has no result, and a program that fails has to say so some other way.
///
/// Not a style rule: with a result type, `return fail(…)` from `main` printed
/// nothing and exited 0, so the one thing a shell can read said the program had
/// worked. Refused here rather than at emission because `heroes check` is where
/// a reader looks (panel 035).
pub(in crate::types) fn main_returns(got: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "main_returns",
        format!(
            "`main` produces nothing, and this one is declared `-> {got}` — a program reports failure by what it prints, not by what it returns"
        ),
        span,
    )
    .with_note(
        "drop the result type; a value `main` computed and returned would have nowhere to go"
            .to_string(),
    )
}

pub(in crate::types) fn discarded_value(got: &str, span: Span) -> Diagnostic {
    let mut diagnostic = Diagnostic::new(
        "discarded_value",
        format!(
            "this line's value has type `{got}` and nothing receives it — write `_ = …` to discard it on purpose"
        ),
        span,
    );
    diagnostic.fixes.push(Fix {
        title: "discard it explicitly".to_string(),
        replacement: "_ = ".to_string(),
        span: Span { start: span.start, end: span.start },
        certainty: Certainty::Certain,
    });
    diagnostic
}


/// Panel 017 C: the mirror of the rule above. A `()` value binds to nothing,
/// and the certain repair is to keep the call and drop the binding.
pub(in crate::types) fn bound_unit(name: &str, span: Span) -> Diagnostic {
    let mut diagnostic = Diagnostic::new(
        "bound_unit",
        format!(
            "`{name}` would hold `()`, which is no value — keep the call and drop the binding"
        ),
        span,
    );
    diagnostic.fixes.push(Fix {
        title: format!("remove `{name} = `"),
        replacement: String::new(),
        span,
        certainty: Certainty::Guess,
    });
    diagnostic
}


pub(in crate::types) fn not_callable(got: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "not_callable",
        format!("this is a `{got}`, and a `{got}` cannot be called"),
        span,
    )
}


/// §4.17: the error carries the signature, because that is the file the reader
/// would otherwise open.
pub(in crate::types) fn arity(
    name: &str,
    expected: usize,
    got: usize,
    declared: Option<(String, String)>,
    span: Span,
) -> Diagnostic {
    let diagnostic = Diagnostic::new(
        "wrong_arity",
        format!("`{name}` takes {expected} argument(s), found {got}"),
        span,
    );
    // A built-in and a function *value* have no declaration to cite, and a note
    // saying so would be worse than no note.
    match declared {
        Some((signature, at)) => {
            diagnostic.with_note(format!("declared at {at}: {signature}"))
        }
        None => diagnostic,
    }
}


pub(in crate::types) fn cannot_infer_empty(what: &str, span: Span) -> Diagnostic {
    let mut diagnostic = Diagnostic::new(
        "cannot_infer",
        format!(
            "the {what} is empty, so there is nothing to infer its type from — write the annotation"
        ),
        span,
    );
    diagnostic.fixes.push(Fix {
        title: "annotate the binding: `xs: [int] = []`".to_string(),
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

/// `99999999999999999999` — a literal no `int` holds.
///
/// **The code is snake_case like every other in this compiler.** It was
/// `int-literal-out-of-range`, hyphenated, and it was the only one: a
/// user-visible identifier that did not match its family, in a project whose
/// thesis is that a reader can predict what the compiler says.
///
/// The message names the range because `int` is the only integer type (§4.3):
/// there is no wider one to suggest, so the fix is a different number and the
/// compiler should not pretend otherwise.
/// Built in `lexer/number.rs`, with the decoder that decides when it fires and
/// the base table that lets it answer in the notation the question was asked in
/// — the frontend and the lowering both raise this diagnostic, and before panel
/// 041 they each spelled it out by hand.
pub(in crate::types) fn int_out_of_range(text: &str, span: Span) -> Diagnostic {
    crate::lexer::int_out_of_range(text, span)
}
