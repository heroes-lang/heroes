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
            "`{op}` takes two of one type — two integers of the SAME width, or two floats of the same width — and found `{left}` and `{right}`"
        ),
        span,
    );
    diagnostic.fixes.push(Fix {
        title: "convert one side: `to_f64(x)`, `to_i64(x)`, or `fit_<width>(x)` between widths".to_string(),
        replacement: String::new(),
        span,
        certainty: Certainty::Guess,
    });
    diagnostic
}


pub(in crate::types) fn bad_operand(op: &str, allowed: &str, got: &str, span: Span) -> Diagnostic {
    let diagnostic = Diagnostic::new(
        "bad_operand",
        format!("`{op}` takes {allowed}, found `{got}`"),
        span,
    );
    // **A `cstr` gets a route, because a hard stop without one is what produced the
    // only genuinely unsafe artifact in this project's record** (panel 059, ratified
    // 2026-08-15). Panel 058's blind reader hit `print(c)` on a `cstr`, found the
    // stop, and reached for a C helper with a `static char[4096]` — an escape one
    // line long, invisible to every rule this compiler has, and strictly worse than
    // what the stop refused.
    //
    // The message alone was correct and useless: it says a `cstr` is not a `str`,
    // which the reader can see, and says nothing about the conversion that exists.
    // §4.17 is the standard — an error carries what is needed to fix the program
    // without opening another file.
    //
    // A **note and not a `Fix`**: the repair is `c.to_str()`, and building that
    // replacement needs the operand's own source text, which this module does not
    // have. A note that names the operation is the honest half; inventing a `Fix`
    // with a span and no text would be worse than none.
    if got == "cstr" {
        return diagnostic.with_note(
            "a `cstr` is C's string, not this language's — `c.to_str()` copies one into a `str`. Test `c == nullptr` first, because converting one aborts".to_string(),
        );
    }
    diagnostic
}

/// `sort` was handed an array of something the language does not order
/// (**panel 068 R2**, ratified 2026-08-16 — the refusal moved here from the
/// emitter).
///
/// **Three things this message must not do**, and each was a finding of that
/// sitting rather than a style preference.
///
/// It must not say the element *"has no `<`"*. That is **false of `str`**:
/// `"a" < "b"` is `bad_operand` and `sort(["b", "a"])` runs and is ordered. The
/// two questions are different — one is which operator the surface offers, the
/// other is which order the runtime can produce — and a message that conflates
/// them teaches a reader a rule that will mislead them the next time.
///
/// It must **name the ordered set**, because the reader's next act is to choose a
/// type, and §4.17 says the error carries the repair without opening another
/// file. The set is the twelve rows `hero_cmp_for` dispatches on and is written
/// out rather than summarised: the old emitter message said *"other than `i64`,
/// `f64` or `str`"* and had been stale since panel 042 added the seven widths.
///
/// It must leave **`sort_by` addable**. Panel 068 refused option C today and
/// priced it at +47…+55 with a stated return condition, so this message says what
/// `sort` orders and never that ordering *this* is impossible — a sentence the
/// language would have to take back.
///
/// The `reached` argument is what the transitive descent found. Where the element
/// is itself unordered the two coincide; where a record or a variant case is what
/// the author wrote, this names the member inside it that decided the answer, so
/// the reader is not left comparing a type name against a list it is not on.
/// **A type parameter takes a third note, and §4.17 is why** (panel 084). Telling
/// a reader to *"build an array of one of those"* is unanswerable when the element
/// is spelled `A`: there is no value in scope to build it from. design.md
/// §4.12:1593 has always named the route — *"If an operation on `T` is needed,
/// pass it as a parameter"* — so the note names it, and the route is measured
/// rather than promised: a full generic insertion sort taking
/// `(function(T, T) -> bool)` runs today at `i64` and at a user record.
pub(in crate::types) fn unordered_element(
    element: &str,
    reached: Option<String>,
    generic: bool,
    span: Span,
) -> Diagnostic {
    let diagnostic = Diagnostic::new(
        "unordered_element",
        format!("`sort` orders arrays of numbers, `str` and `bool` — `[{element}]` is none of those"),
        span,
    );
    if generic {
        return diagnostic.with_note(format!(
            "`{element}` is a type parameter, so this line cannot know whether it has an \
             order — take the comparison as a parameter instead, \
             `less: (function({element}, {element}) -> bool)`, and call `less(a, b)`"
        ));
    }
    match reached {
        Some(inside) => diagnostic.with_note(format!(
            "{inside}, which has no order the language can produce. Sort a key you can name \
             instead — build `[i64]`, `[str]` or `[bool]` from the values and order that"
        )),
        None => diagnostic.with_note(
            "the ordered types are `i8` `i16` `i32` `i64`, `u8` `u16` `u32` `u64`, `f32` `f64`, \
             `str` and `bool`. Sort a key you can name instead — build an array of one of those \
             from the values and order that"
                .to_string(),
        ),
    }
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

/// A line computed a value and nothing received it (panel 003).
///
/// **`receiver` is the name the call was written on when the call returns that
/// name's own type**, and it decides whether `_ = …` is `certain` or a `guess`
/// (panel 071, measured). `push` returns the new array and leaves its receiver
/// alone, so `_ = xs.push(4)` compiles at exit 0 and loses the element — a
/// `certain` fix converting a caught mistake into a silent wrong answer, which is
/// CLAUDE.md §8's definition failing on its own terms and §1.12's rule that a
/// check must surface a defect rather than hide it.
pub(in crate::types) fn discarded_value(
    got: &str,
    receiver: Option<String>,
    span: Span,
) -> Diagnostic {
    let Some(name) = receiver else {
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
        return diagnostic;
    };
    // Both repairs are offered and **neither is `certain`**, because the compiler
    // genuinely cannot tell them apart: the value has `{name}`'s own type, so
    // assigning it back and throwing it away are both well-formed programs. Only
    // a `certain` fix is machine-applicable (CLAUDE.md §8), so `--apply` now
    // declines this line instead of silently picking the losing one.
    let mut diagnostic = Diagnostic::new(
        "discarded_value",
        format!(
            "this line's value has type `{got}` and nothing receives it — every value here is a copy, so `{name}` is unchanged: write `{name} @ …` to keep the result, or `_ = …` to discard it on purpose"
        ),
        span,
    );
    diagnostic.fixes.push(Fix {
        title: format!("assign it back to `{name}`"),
        replacement: format!("{name} @ "),
        span: Span { start: span.start, end: span.start },
        certainty: Certainty::Guess,
    });
    diagnostic.fixes.push(Fix {
        title: "discard it explicitly".to_string(),
        replacement: "_ = ".to_string(),
        span: Span { start: span.start, end: span.start },
        certainty: Certainty::Guess,
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
