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
