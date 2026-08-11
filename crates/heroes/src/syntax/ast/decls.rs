//! The top level: the four entities and what they own (design.md §4.2,
//! §4.8 `@` parameters, §4.12 generics, §4.18 tests, §4.19 `extern`).
//!
//! Declarations are not recursive, so each one owns its members directly —
//! no arena, no indices.

use crate::source::Span;

use super::{Block, TypeId};

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
    /// `constant MAX: int` + body (§4.2).
    Constant { ty: TypeId, body: Block },
    Function(Function),
    /// `record Point` + one field per line.
    Record { fields: Vec<Field> },
    /// `variant Token` + one case per line, each optionally with fields.
    Variant { cases: Vec<Case> },
    /// `test "3-4-5 triangle"` + body (§4.18). `name` holds the string
    /// literal, quotes included — it is the test's title, not an identifier.
    Test { body: Block },
}

pub struct Function {
    /// `function map<A, B>(…)` — type parameters, no constraints, always
    /// inferred at the call site (§4.12).
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

/// `use geom` — the module named, and the whole line (panel 031).
///
/// **Not a `DeclKind`**, and the reason is what the later passes must never
/// see. A `use` declares nothing and lowers to nothing: the IR, the type
/// checker and the emitter have no arm for it and should not be given one, or
/// every exhaustive `match` in them grows a case that means "ignore me". It
/// lives beside `decls` instead, read by exactly the three passes that care —
/// discovery, the resolver, and the printers.
///
/// It still *binds* a name, which is the whole of panel 031 R3: `geom` enters
/// the ordinary namespace, so "shadowing is a compile error" and "an unused
/// binding is a compile error" cover it without a word of new specification.
pub struct Use {
    pub name: Span,
    pub span: Span,
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
