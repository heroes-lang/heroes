//! Written types (design.md §4.3 the type table, §4.6 `T?`, §4.13 function
//! types). Recursive, so they live in `Ast::types` and link by `TypeId`.

use crate::source::Span;

use super::TypeId;

/// A written type. `span` covers the whole type as written (`[int]` includes
/// its brackets); for `Named` the span *is* the name, so the text never
/// needs copying.
pub struct TypeNode {
    pub kind: TypeKind,
    pub span: Span,
}

pub enum TypeKind {
    /// `int`, `str`, `Point`, `A` — a bare name. Whether it is built-in,
    /// user-declared or a generic parameter is the resolver's question
    /// (M3a); the parser only records that a name stood here.
    Named,
    /// `()` — no value (§4.3). Also the result of a function written
    /// without `->`.
    Unit,
    /// `[T]`
    Array(TypeId),
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
