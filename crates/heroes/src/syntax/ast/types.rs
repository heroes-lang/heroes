//! Written types (design.md §4.3 the type table, §4.6 `T?`, §4.13 function
//! types). Recursive, so they live in `Ast::types` and link by `TypeId`.

use crate::source::Span;

use super::TypeId;

/// A written type. `span` covers the whole type as written (`[i64]` includes
/// its brackets); for `Named` the span *is* the name, so the text never
/// needs copying.
pub struct TypeNode {
    pub kind: TypeKind,
    pub span: Span,
}

pub enum TypeKind {
    /// `i64`, `str`, `Point`, `A` — a bare name. Whether it is built-in,
    /// user-declared or a generic parameter is the resolver's question
    /// (M-name-resolution); the parser only records that a name stood here.
    Named,
    /// `()` — no value (§4.3). Also the result of a function written
    /// without `->`.
    Unit,
    /// `[T]`
    Array(TypeId),
    /// `i32[4]` — a **fixed-size** array, and the length is part of the type
    /// (§4.19; panel 062, author instruction 2026-08-15).
    ///
    /// **Not an `Array`, and the difference is where the elements live.** A `[T]`
    /// is a heap block with a reference count in a header before the bytes; this
    /// is the bytes themselves, inline, at an offset a C compiler chose. They can
    /// no more share a representation than a `str` and a `cstr` can.
    ///
    /// The bracket comes **after** the element type, which is what makes the two
    /// unambiguous to parse and — the reason it was chosen over `[T; N]` and
    /// `[T N]` — what lets a reader comparing `raylib.h` to a `.hero` file read
    /// `int params[4]` and `params: i32[4]` without translating.
    Fixed(TypeId, u32),
    /// `{K: V}`
    Map(TypeId, TypeId),
    /// `T?` — a `T` or an error (§4.6).
    Fallible(TypeId),
    /// `(fn(A, B) -> C)` — the parentheses are mandatory (§4.13: without
    /// them `f: fn A -> B -> [B]` would be ambiguous).
    Func { params: Vec<TypeId>, result: TypeId },
    /// Recovery: the type could not be read. A diagnostic says why, and the
    /// tree keeps its shape so the errors after this one are still found.
    Error,
}
