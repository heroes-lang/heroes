//! The syntax tree: what the parser builds and every later pass reads
//! (design.md §4.1 file structure, §4.2 the four entities, §4.13 function
//! types, §4.18 tests, §4.19 `extern`).
//!
//! Two shapes of link, both forced by the Cyclone rule (CLAUDE.md §5) and by
//! what the port to Heroes will need — design.md §4.10: "the array is the
//! only indirection":
//!
//! - **Types are recursive** (`[{str: int}?]`), so they all live in one
//!   arena, `Ast::types`, and point at each other with `TypeId` — an index,
//!   never a pointer. The port reads `types: [TypeNode]` unchanged.
//! - **Declarations are not recursive**, so each one owns its parameters,
//!   fields and cases directly.
//!
//! No node stores text. Every node carries a `Span` and the `Source` stays
//! the single owner of every byte, so the printer and the diagnostics read
//! the same characters the author typed.

use crate::source::Span;

/// Index into `Ast::types`. The tree's only link.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub struct TypeId(pub u32);

/// One parsed file. `decls` is in source order — declaration order carries
/// no meaning in Heroes (§4.2), but the tree preserves it so that errors,
/// `fmt` and `outline` can speak in the order the author reads.
#[derive(Default)]
pub struct Ast {
    pub decls: Vec<Decl>,
    /// The type arena. Indexed directly (`ast.types[id.0 as usize]`) rather
    /// than through an accessor: a getter would have to return a reference,
    /// and the Cyclone rule keeps references to parameters only.
    pub types: Vec<TypeNode>,
}

impl Ast {
    /// Adds a node to the arena and hands back its index.
    pub fn push_type(&mut self, node: TypeNode) -> TypeId {
        self.types.push(node);
        TypeId(self.types.len() as u32 - 1)
    }
}

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

/// A top-level declaration. `span` covers the whole declaration, header and
/// body; `name` is just the name, which is what diagnostics point at.
pub struct Decl {
    pub name: Span,
    /// The comment lines directly above, with no blank line between
    /// (§4.1 — adjacency *is* the doc marker, Go's rule). A `##` section
    /// heading stops the run: a heading documents the section, not the
    /// declaration under it.
    pub doc: Vec<Span>,
    pub span: Span,
    pub kind: DeclKind,
}

pub enum DeclKind {
    /// `MAX = constant: int` + body (§4.2).
    Constant { ty: TypeId, body: Block },
    Function(Function),
    /// `Point = record` + one field per line.
    Record { fields: Vec<Field> },
    /// `Token = variant` + one case per line, each optionally with fields.
    Variant { cases: Vec<Case> },
    /// `test "3-4-5 triangle"` + body (§4.18). `name` holds the string
    /// literal, quotes included — it is the test's title, not an identifier.
    Test { body: Block },
}

pub struct Function {
    /// `function<A, B>:` — type parameters, no constraints, always inferred
    /// at the call site (§4.12).
    pub generics: Vec<Span>,
    pub params: Vec<Param>,
    /// The declared result. A signature without `->` gets a `Unit` node, so
    /// later passes never ask "was the arrow there?".
    pub result: TypeId,
    /// `None` for an `extern`: the implementation comes from C (§4.19).
    pub body: Option<Block>,
    pub is_extern: bool,
}

pub struct Param {
    pub name: Span,
    pub ty: TypeId,
    /// `@l: Lex` — in-out: copy in, copy out (§4.8). UFCS does not apply to
    /// a first parameter marked this way, and two `@` arguments of one call
    /// may not share a root binding (panel 010).
    pub mutable: bool,
}

/// A record field, or a variant case's payload field.
pub struct Field {
    pub name: Span,
    pub ty: TypeId,
    pub doc: Vec<Span>,
}

/// One case of a variant. A case with fields *is* a small record (§4.2),
/// which is why no separator syntax exists.
pub struct Case {
    pub name: Span,
    pub fields: Vec<Field>,
    pub doc: Vec<Span>,
}

/// An indented body.
///
/// M2 step 1 records its *shape* only: the span from the first token after
/// the `Indent` to the last one before the matching `Dedent`. Statements
/// land in M2 step 3 as a `Vec<StmtId>` beside this span — the body is
/// proven well-formed here, and read there.
pub struct Block {
    pub span: Span,
}
