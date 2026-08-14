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

/// Where a group's symbols come from — **one of two, never both**, which is why
/// this is an enum and not two `Option`s.
///
/// The distinction is who answers the question. `link` is the program telling the
/// linker a name it already knows. `package` is the program asking the *machine*
/// where something is and what else it needs, and getting back a different answer
/// on every platform — frameworks on macOS, `-lGL -lX11` on Linux — from one
/// spelling that never changes (panel 049's measurements; author decision
/// 2026-08-14).
#[derive(Clone, Copy, PartialEq, Debug)]
pub enum Library {
    /// `link "sqlite3"` — reaches the linker as `-lsqlite3`, a name and never a
    /// flag, so no `.hero` file can hand clang an arbitrary argument.
    Link(Span),
    /// `package "raylib"` — the machine is asked. What comes back is validated
    /// against a closed set before it reaches clang, because a package file is
    /// input this program did not write: Go shipped the same idea without that
    /// check and it became CVE-2018-6574, a remote-code-execution vector through
    /// `-fplugin=`.
    Package(Span),
}

pub enum DeclKind {
    /// `constant MAX: i64` + body (§4.2), or — inside an `extern` group —
    /// `constant SQLITE_OK: i64` with **no** body, whose value is the header's
    /// (§4.19, panel 038).
    ///
    /// One variant rather than two, and the reason is a measurement rather than
    /// taste: `ir/exprs.rs`'s `is_constant` is a `matches!`, not an exhaustive
    /// `match`, so a sixth `DeclKind` would leave it answering `false` and lower a
    /// header constant to `Op::FuncRef` — a function pointer where an `i64` is
    /// wanted, silently. The variant that breaks more call sites at compile time
    /// is worse at the one site that decides correctness.
    ///
    /// `header`/`link` carry the group's head line exactly as `Function`'s do, and
    /// mean the same thing: the group is flattened in the parser and nowhere else.
    Constant { ty: TypeId, body: Option<Block>, header: Option<Span>, library: Option<Library> },
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
    /// The header this signature was declared under, quotes included, and the
    /// library to link — both from the group's head line, **copied onto every
    /// member** (§4.19, panel 036).
    ///
    /// The group is flattened here and nowhere else: one `Decl` per `extern
    /// function`, so a declaration's index stays function identity for `Ref::Top`,
    /// `ir::Function::decl`, `checked.result_type` and the mangler's name table.
    /// A `DeclKind::ExternGroup` holding N functions would have broken that in
    /// five modules, which is the whole reason the panel's condition was
    /// "flatten in the parser, or it is core".
    ///
    /// `None` on both for an ordinary `function`, and on `header` never for an
    /// `extern`: an `extern` without a header does not parse, because a signature
    /// with no `#include` behind it is the accidental-link hazard §4.19 names.
    pub header: Option<Span>,
    pub library: Option<Library>,
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
