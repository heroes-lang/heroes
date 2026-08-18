//! The two names one aggregate answers to (design.md §4.19; panels 060, 074).
//!
//! Split from `typedefs.rs` by CLAUDE.md §11, on the seam this project has cut
//! twice before — `resolve/resolved.rs` and `types/checked.rs`, both times
//! separating *what the answer is* from *how it is computed*. `typedefs.rs` is
//! the table: every name in the unit, built in one walk of the AST. This is the
//! **value** that table stores, and it has an invariant of its own that a reader
//! should meet before the walk that fills it: **the prefix is never the header's**.
//!
//! Get that wrong and the emitter writes `_eq` for a struct it did not declare —
//! or, since panel 074, tries to spell a function name `struct stat_eq`, which is
//! not a C identifier at all.

/// The two names one aggregate answers to: the prefix its generated functions
/// carry, and the C type it *is*.
///
/// **They differ for exactly one kind of declaration** — a `record` inside an
/// `extern` group, whose type is the header's and whose `_eq` must still be this
/// compiler's — and agree for every other, which is what made a single name look
/// right for as long as no header was involved. Owned rather than borrowed
/// (CLAUDE.md §5), and one value rather than two parameters, because the
/// functions that need it already carry nine and ten.
#[derive(Clone)]
pub(super) struct Aggregate {
    /// This compiler's mangled name — what `_eq`, `_hash`, `_desc`, `_retain` and
    /// `_release` are spelled with. Never the header's, or the emitter writes a
    /// global unmangled `Color_eq` beside the library that declared `Color`.
    pub(super) prefix: String,
    /// The C type — the header's own spelling for a group's `record`.
    pub(super) c_type: String,
    /// Whether the struct is the **header's** rather than this compiler's, which
    /// decides how its members are spelled as well as what it is called.
    pub(super) foreign: bool,
    /// Whether the field list names only **some** of that struct — which decides
    /// what the generated `_eq` and `_hash` are allowed to do (panel 061).
    pub(super) partial: bool,
}
