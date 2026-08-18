//! The diagnostics about what may cross the **FFI boundary** (design.md §4.19;
//! panels 036, 038, 060).
//!
//! Split out of `data.rs` on 2026-08-15, along the same seam `types/ffi_decls.rs`
//! was cut on the same day and for the same reason: these three are about the C
//! **type vocabulary**, while everything left in `data.rs` is about records,
//! variants and `match`. Cutting the checker one way and its diagnostics another
//! would make the pair harder to read than either file was long.
//!
//! Three codes and not one, because the reader\'s mistake differs in each. A
//! signature names a type C cannot spell; a `constant` names a type that is a
//! Heroes *value* rather than something a header holds; a **field** is inside a
//! struct whose layout is the header\'s, where the repair depends on which Heroes
//! type was written and is carried in the note rather than guessed at.

use crate::diagnostics::Diagnostic;
use crate::source::Span;

/// A type in an `extern`'s signature that no C header can declare (§4.19).
///
/// The message names what the boundary *does* carry, because the repair is
/// almost always a different signature rather than a different design: a C
/// function that returns many values returns a pointer, and the shim that turns
/// one into a Heroes container is Heroes code on this side of the boundary.
pub(in crate::types) fn ffi_type(name: &str, what: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "ffi_type",
        format!(
            "`{name}` cannot cross the FFI boundary, and it is {what} — a C header can declare a number, `bool`, `str`, `ptr`, `cstr`, and a `record` declared in this same group (§4.19)"
        ),
        span,
    )
}

/// A type that *can* cross the boundary but cannot be a **value a header holds**
/// (§4.19, panel 038).
///
/// Separate from `ffi_type` because the reader's mistake is different: they did
/// not reach for a type C has never heard of, they reached for one that works
/// perfectly in a signature and cannot be a constant. So the message carries the
/// reason rather than the list — the list would say the type is allowed.
pub(in crate::types) fn ffi_constant_type(name: &str, why: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "ffi_constant_type",
        format!("an `extern constant` cannot be declared `{name}` — {why}"),
        span,
    )
}

/// A type that can cross the boundary in a **result** and cannot be an
/// **argument** (§4.19; panel 062's follow-up, measured 2026-08-16).
///
/// `ffi_constant_type`'s shape one position further, and the reasoning is the
/// same: the reader did not reach for a type C has never heard of, so the list
/// `ffi_type` prints would say their type is allowed — and it *is* allowed, just
/// not here. So this carries the reason instead.
///
/// **It reuses `ffi_parameter_type` deliberately rather than opening a code.**
/// That class already means *this parameter's declared type is wrong*; it is
/// where a reader looking for the answer will already be; and CLAUDE.md §4 makes
/// a new diagnostic *class* a panel path, which a second name for one question
/// would be. The existing instances are recovered from clang's output in
/// `emit/ffi_narrowed.rs`; these two are decidable in the frontend, because they
/// are facts about **Heroes** rather than about any particular header — no
/// header anywhere takes either, so no header needs reading.
pub(in crate::types) fn ffi_parameter_position(
    parameter: &str,
    function: &str,
    name: &str,
    why: &str,
    span: Span,
) -> Diagnostic {
    Diagnostic::new(
        "ffi_parameter_type",
        format!("`{parameter}` of `{function}` cannot be declared `{name}` — {why}"),
        span,
    )
}

/// A field of a group's `record` whose type no C header can hold (§4.19, panel
/// 060).
///
/// Separate from `ffi_type` because the reader's mistake is different and so is
/// the repair: `ffi_type` is about a *signature*, where the answer is often `ptr`;
/// here the author has written a Heroes type inside what is really a C struct, and
/// the repair depends on which one.
pub(in crate::types) fn ffi_field_type(name: &str, why: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "ffi_field_type",
        format!(
            "`{name}` cannot be a field of a `record` in an `extern` group — the header owns the layout, and it can hold a number, `bool`, `ptr`, `cstr`, or another record of the group (§4.19)"
        ),
        span,
    )
    .with_note(why.to_string())
}

/// A `record` its group marked `partial`, reached by an operation that reads the
/// fields nobody named (panel 061).
///
/// **The message names the record REACHED and the type in hand**, because they are
/// often not the same and §4.17 asks the diagnostic to carry the repair. Saying
/// *"`[Font]` names only some of its C struct's members"* is false — `[Font]` is an
/// array and has no C struct — and it sends the reader looking at the array.
pub(in crate::types) fn partial_operation(
    record: &str,
    shown: &str,
    what: &str,
    span: Span,
) -> Diagnostic {
    let through =
        if record == shown { String::new() } else { format!(" — reached through `{shown}`") };
    Diagnostic::new(
        "ffi_partial_operation",
        format!(
            "`{record}` is `partial`, so it cannot be {what}{through}: the fields it does not name are part of the answer, and this program cannot see them"
        ),
        span,
    )
    .with_note(format!(
        "a `partial` record may be read, copied, passed to C and returned from it — what it gives up is `==`, `hash` and being a map key. Name every field of the header's struct to get them back, and drop `partial` from `{record}`"
    ))
}

/// A literal built for a fixed array whose length it does not match (panel 062).
///
/// **Both numbers are in the message**, because the reader's mistake is a count and
/// a count is invisible in a type name: someone who read `float lensDistortionValues[4]`
/// as three cannot see their own error in *"expected `f32[4]`"* alone.
pub(in crate::types) fn fixed_array_length(
    want: &str,
    expected: u32,
    got: usize,
    span: Span,
) -> Diagnostic {
    Diagnostic::new(
        "fixed_array_length",
        format!(
            "`{want}` holds exactly {expected}, and this literal has {got} — a fixed array's length is part of its type, so the two must agree (§4.19)"
        ),
        span,
    )
}

/// A literal index past the end of a fixed array (panel 062).
///
/// **A compile error and not an abort**, which is the one place a fixed array is
/// safer than `[T]` rather than merely different: the length is in the type, so the
/// mistake is visible without running the program. §1.12 asks a check to surface a
/// defect rather than hide it, and a diagnostic surfaces it earlier than a trap.
pub(in crate::types) fn fixed_index_out_of_range(
    shown: &str,
    n: u32,
    got: i128,
    span: Span,
) -> Diagnostic {
    Diagnostic::new(
        "fixed_index_out_of_range",
        format!(
            "`{shown}` holds {n}, so its last index is {}, and this is {got} — a fixed array's length is part of its type, so the index is checked here rather than at run time",
            n - 1
        ),
        span,
    )
}

/// `i32[4]` in an ordinary `record` (panel 062).
pub(in crate::types) fn fixed_outside_a_group(name: &str, span: Span) -> Diagnostic {
    Diagnostic::new(
        "fixed_outside_a_group",
        format!(
            "`{name}` is a C array member, so it belongs to a `record` inside an `extern` group — this record's layout is this language's, and a fixed array exists to match one a C compiler chose (§4.19)"
        ),
        span,
    )
    .with_note(
        "use `[T]` here: it grows, it is compared and hashed like any other value, and it is what every Heroes record holds".to_string(),
    )
}

/// A map key that reaches a float (**panel 069 R4**, provisional 2026-08-16).
///
/// **Why this is a compile error and `==` on a float is not.** The sitting vetoed
/// refusing `==` on float-bearing types twice, on measured grounds: it breaks a
/// divide guard and a raylib assertion, and the hand-written replacement compiles,
/// is equally wrong, and can forget a field. A map key is the half that *can* be
/// decided statically, because key-eligibility is a property of the map's declared
/// type rather than of a value flowing through it — so the error points at the
/// annotation the author wrote, which is as local as a diagnostic gets.
///
/// **The precedent is two languages in this one's own machinery class.** Rust
/// requires `K: Eq + Hash` and `f64` is `PartialEq` but not `Eq`, so
/// `HashMap<f64, V>` does not compile; Zig's `std.hash.autoHash` answers a float
/// with `@compileError`. Neither needs traits to state it and neither does this.
/// Before this rule Heroes was, as far as the historian could source, the **only**
/// language that made a nan key a runtime event — Go's proposal to do the same has
/// been open since 2017 and Go shipped the `clear` builtin to route around its own
/// NaN keys instead.
///
/// **The note answers the generic case, and that was a condition of the sitting**
/// (llm-ergonomist, who would otherwise have vetoed on non-locality). Inside
/// `function count<K>(ks: [K])`, `m: {K: i64}` cannot be judged here — only
/// monomorphisation knows what the call chose, and `count([1.5, 2.5])` is a
/// working program. So the runtime guard stays behind this rule and the message
/// says so, rather than implying a guarantee the type system does not give.
pub(in crate::types) fn float_map_key(key: &str, reached: String, span: Span) -> Diagnostic {
    // *is* a float, versus *reaches* one — the reader's next act differs. For a
    // bare `f64` the key type is the whole answer; for a record they have to be
    // told which field, or they are left comparing a name against a rule.
    let because = if reached.is_empty() {
        "it is a float".to_string()
    } else {
        format!("`{reached}` is a float")
    };
    Diagnostic::new(
        "float_map_key",
        format!("`{key}` cannot be a map key, because {because}"),
        span,
    )
    .with_note(
        "a `nan` is not equal to itself, so it can be stored and never found again. Key the map \
         on something exact — an `i64` of the value scaled to the precision you need, or a `str` \
         — and keep the float in the value. Reached through a type parameter this cannot be \
         checked, and the runtime aborts instead"
            .to_string(),
    )
}
