//! Bodies: blocks, statements, expressions and patterns (design.md §4.4
//! bindings, §4.6 `T?`, §4.7 control flow, §4.9 calls, §4.11 UFCS, §4.14
//! operators, §4.16 holes).
//!
//! Expressions and statements are recursive, so both live in arenas
//! (`Ast::exprs`, `Ast::stmts`) and link by index. A `Block` owns the list
//! of statement indices it contains.

use crate::source::Span;

use super::{ExprId, StmtId, TypeId};

/// An indented body: its statements, and the source it covers.
///
/// A block is also an *expression*: its value is its last statement, when
/// that statement is an expression (§4.7 — one rule seen in two places,
/// `if` as an expression and long `match` arms). Nothing in the parser
/// enforces that; it is the checker's judgment at M-typed-frontend.
pub struct Block {
    pub span: Span,
    pub stmts: Vec<StmtId>,
}

// --- statements --------------------------------------------------------

pub struct Stmt {
    pub kind: StmtKind,
    pub span: Span,
}

pub enum StmtKind {
    /// `x = 5`, or `xs: [i64] = []` where an empty literal needs the
    /// annotation (§4.5). Binds once, forever.
    Bind { name: Span, ty: Option<TypeId>, value: ExprId },
    /// `v: i64 @ 0` — declares a mutable cell. The type is **required**
    /// (§4.4): without it, `x @ 5` would not say whether it declares or
    /// mutates, and a typo would silently declare a new variable.
    Declare { name: Span, ty: TypeId, value: ExprId },
    /// `v @ v + 1`, `l.pos @ l.pos + 1` — the left side is a *place*: a
    /// name, or a field or index path rooted at one.
    Mutate { place: ExprId, value: ExprId },
    /// `return e`, or bare `return` in a function that returns nothing.
    Return(Option<ExprId>),
    Break,
    Continue,
    /// `assert e` (§4.18). The source text of `e` is its span, which is why
    /// the failure message can quote the expression.
    Assert(ExprId),
    /// `while cond` — the condition loop (§4.7; panel 018 gave it its own
    /// keyword).
    While { cond: ExprId, block: Block },
    /// `for x in xs` — iteration; sugar for a `while` with an index (Part 5).
    ForIn { name: Span, iterable: ExprId, block: Block },
    /// An expression alone on a line. Its type must be `()` (§4.14, panel
    /// 003) — a *type* judgment, not a parse error, so the parser accepts
    /// it and M-typed-frontend rejects `xs.push(4)` with the `_ =` fix.
    Expr(ExprId),
    Error,
}

// --- expressions ------------------------------------------------------

pub struct Expr {
    pub kind: ExprKind,
    pub span: Span,
}

pub enum ExprKind {
    /// Literals keep their text in `span`: `42`, `1.5`, `"hi"`, `'a'`,
    /// `true`. Decoding is the checker's job, and `escape::unescape` is
    /// already written for it.
    Int,
    Float,
    Str,
    Char,
    Bool,
    /// `nullptr` — the one `ptr` value the language can write (§4.19).
    NullPtr,
    /// A bare name: a local, a parameter, a top-level function or constant.
    /// Which one is the resolver's question (M-name-resolution).
    Name,
    /// `???` — the typed hole (§4.16). Not an error: the compiler reports
    /// what belongs here.
    Hole,
    Unary { op: UnaryOp, operand: ExprId },
    Binary { op: BinaryOp, left: ExprId, right: ExprId },
    /// `p.x` — a field read.
    Field { base: ExprId, name: Span },
    /// `xs[i]`, `m[k]`. Out of bounds aborts; a map yields `V?` (§4.9).
    Index { base: ExprId, index: ExprId },
    /// `f(x)`, `Point(x: 3, y: 4)`, `fail("code", "msg")` — record
    /// construction *is* a call with named arguments (§4.9), so it needs no
    /// node of its own.
    Call { callee: ExprId, args: Vec<Arg> },
    /// `x.f(y)` — UFCS, sugar for `f(x, y)` erased in the frontend (§4.11,
    /// Part 5). Kept here because errors must speak in the syntax written.
    Method { receiver: ExprId, name: Span, args: Vec<Arg> },
    /// `.plus`, `.num(v: 12)` — a variant case, its type coming from
    /// context (§4.5's ⇐ mode).
    Case { name: Span, args: Vec<Arg> },
    /// `[1, 2, 3]` — elements separated by commas on one line, or by
    /// newlines when the literal is written across several (§4.9).
    Array(Vec<ExprId>),
    /// `{ "mario": 30 }`
    Map(Vec<MapEntry>),
    /// `e?` — propagate the error to the caller (§4.6).
    Try(ExprId),
    /// `if c` / `else if c` / `else`, an expression like everything else
    /// (§4.7). `else` is absent when the `if` is used as a statement.
    If { branches: Vec<Branch>, otherwise: Option<Block> },
    /// `match e` with its arms — the only destructuring construct, and
    /// exhaustive or a compile error (§4.7).
    Match { scrutinee: ExprId, arms: Vec<Arm> },
    Error,
}

#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum UnaryOp {
    /// `-x`
    Neg,
    /// `!x` — `bool` only, there is no truthiness (§4.14).
    Not,
    /// `~x` — the bitwise complement, `i64` only (§4.14).
    BitNot,
}

#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum BinaryOp {
    Add,
    Sub,
    Mul,
    Div,
    Rem,
    Eq,
    Ne,
    Lt,
    Le,
    Gt,
    Ge,
    /// `&&` and `||` short-circuit and take `bool` only (§4.14).
    And,
    Or,
    /// The bitwise set, `i64` only and **not** short-circuiting (§4.14). `|` is the
    /// same token as the match-pattern join; position tells them apart.
    BitAnd,
    BitOr,
    BitXor,
    Shl,
    Shr,
}

/// One argument of a call. `name` is present when written `name: value`,
/// which §4.9 makes **mandatory** when two parameters share a type — the
/// rule that spends tokens exactly where argument inversion happens.
pub struct Arg {
    pub name: Option<Span>,
    /// `f(@l)` — the call site repeats the signature's `@` so the mutation
    /// is visible on the line (§4.8).
    pub mutable: bool,
    pub value: ExprId,
}

pub struct MapEntry {
    pub key: ExprId,
    pub value: ExprId,
}

/// One `if` or `else if` — a condition and its block.
pub struct Branch {
    pub cond: ExprId,
    pub block: Block,
}

/// One arm of a `match`: patterns joined by `|`, then an expression or a
/// block (§4.7).
pub struct Arm {
    pub patterns: Vec<Pattern>,
    pub body: ArmBody,
    pub span: Span,
}

pub enum ArmBody {
    /// One statement on the same line as `=>`. An expression statement's value
    /// is the arm's value, which is why this is not a second form: a block was
    /// already "its last expression" (panel 014).
    Stmt(StmtId),
    Block(Block),
}

pub struct Pattern {
    pub kind: PatternKind,
    pub span: Span,
}

pub enum PatternKind {
    /// `.num n`, `.num _`, `.plus` — a variant case, optionally binding its
    /// payload. `_` as a payload *name* is allowed; `_` as a whole arm is
    /// not, on a variant (§4.7).
    Case { name: Span, binding: Option<Span> },
    /// `_` — the catch-all, legal only where exhaustiveness is impossible
    /// (`i64`, `str`). Rejecting it on variants is the checker's job at
    /// M-data-declarations: the parser records what was written.
    Wildcard,
    /// An `i64` or `str` literal arm.
    Literal(ExprId),
    Error,
}
