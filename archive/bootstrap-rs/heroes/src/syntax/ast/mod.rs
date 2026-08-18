//! The syntax tree: what the parser builds and every later pass reads
//! (design.md §4.1 file structure, §4.2 the four entities, §4.13 function
//! types, §4.18 tests, §4.19 `extern`).
//!
//! Data definitions only — no parsing, no rendering. Split by what the node
//! *is*:
//!
//! | file       | nodes |
//! |------------|-------|
//! | `types.rs` | written types: `i64`, `[T]`, `{K: V}`, `T?`, `(function(A) -> B)` |
//! | `decls.rs` | the top level: the four entities, their parameters, fields, cases |
//! | `exprs.rs` | bodies: blocks, statements, expressions, patterns |
//!
//! Two shapes of link, both forced by the Cyclone rule (CLAUDE.md §5) and by
//! what the port to Heroes will need — design.md §4.10: "the array is the
//! only indirection":
//!
//! - **Types are recursive** (`[{str: i64}?]`), so they all live in one
//!   arena, `Ast::types`, and point at each other with `TypeId` — an index,
//!   never a pointer. The port reads `types: [TypeNode]` unchanged.
//! - **Declarations are not recursive**, so each one owns its parameters,
//!   fields and cases directly.
//!
//! No node stores text. Every node carries a `Span` and the `Source` stays
//! the single owner of every byte, so the printer and the diagnostics read
//! the same characters the author typed.

/// Index into `Ast::types`. The tree's only link.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub struct TypeId(pub u32);

/// Index into `Ast::exprs`.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub struct ExprId(pub u32);

/// Index into `Ast::stmts`.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub struct StmtId(pub u32);

/// One parsed file. `decls` is in source order — declaration order carries
/// no meaning in Heroes (§4.2), but the tree preserves it so that errors,
/// `fmt` and `outline` can speak in the order the author reads.
#[derive(Default)]
pub struct Ast {
    pub decls: Vec<Decl>,
    /// `use geom` lines, in source order (panel 031). Separate from `decls`
    /// because a `use` lowers to nothing — see [`Use`].
    pub uses: Vec<Use>,
    /// The type arena. Indexed directly (`ast.types[id.0 as usize]`) rather
    /// than through an accessor: a getter would have to return a reference,
    /// and the Cyclone rule keeps references to parameters only.
    pub types: Vec<TypeNode>,
    pub exprs: Vec<Expr>,
    pub stmts: Vec<Stmt>,
}

impl Ast {
    /// Adds a node to the arena and hands back its index.
    pub fn push_type(&mut self, node: TypeNode) -> TypeId {
        self.types.push(node);
        TypeId(self.types.len() as u32 - 1)
    }

    pub fn push_expr(&mut self, node: Expr) -> ExprId {
        self.exprs.push(node);
        ExprId(self.exprs.len() as u32 - 1)
    }

    pub fn push_stmt(&mut self, node: Stmt) -> StmtId {
        self.stmts.push(node);
        StmtId(self.stmts.len() as u32 - 1)
    }
}

mod decls;
mod exprs;
mod types;

pub use decls::{Case, Decl, DeclKind, Field, Function, Library, Param, Use};
pub use exprs::{
    Arg, Arm, ArmBody, BinaryOp, Block, Branch, Expr, ExprKind, MapEntry, Pattern, PatternKind,
    Stmt, StmtKind, UnaryOp,
};
pub use types::{TypeKind, TypeNode};
