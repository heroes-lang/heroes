//! What an instruction *is* (design.md Part 10 step 6, Part 5; panel 019).
//!
//! **The invariant, exactly as panel 019 restated it: no instruction contains a
//! nested expression.** The proposal put to the judges said "at most two operands
//! and one destination" and the compiler-engineer refuted it in three places at
//! once — `print` is variadic, `slice` and `fold` are 3-ary, a record
//! construction is n-ary. So the rule is about *nesting*, not arity, and the
//! arity is variable in exactly two forms: `Call` and `Construct`. Both hold
//! their arguments in a run (`Args`) rather than a `Vec`, which keeps `Op` `Copy`
//! — the same trick, for the same reason, as `Ty` keeping a function's parameters
//! in `Types::params` (CLAUDE.md §5: a struct may not hold a reference, and the
//! port reads two arrays because §4.10 makes the array Heroes' only indirection).
//!
//! Five things here exist because compiling C found them missing (panel 019 § 1):
//!
//! | carried | what breaks without it |
//! |---|---|
//! | `Callee`'s linkage | an unmangled Heroes `function open` silently replaces libc's |
//! | `dest: Option` on a call | `dst = call print(x)` is a hard clang error; raylib is void-returning |
//! | `Place` as a store's target | §4.8's own `l.pos @ l.pos + 1` is unwritable |
//! | `Construct` | a record would have to be expanded by the emitter, not lowered |
//! | `Cast` | §4.3's no-implicit-conversions rule would be invisible in the dump |
//!
//! There is deliberately **no `And`/`Or`**: `&&` and `||` short-circuit (§4.14),
//! so they are control flow and lower to branches. And there is no `Match`: the
//! surface's only destructuring construct becomes a `Switch` on a variant tag or
//! a chain of `Branch`es on literals.

use crate::source::Span;
use crate::types::TyId;

/// A temporary. Assigned exactly once, by construction — the lowering never
/// re-uses one, which is what makes the dump readable without a def-use index.
#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Debug)]
pub struct ValueId(pub u32);

/// A slot: a parameter, a local, a mutable cell, or one lowering invented.
/// Slots are the reason there are no phi nodes (panel 019 point 2).
#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Debug)]
pub struct SlotId(pub u32);

#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Debug)]
pub struct BlockId(pub u32);

/// A decoded string literal, interned per program (`Program::strings`).
#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Debug)]
pub struct StrId(pub u32);

/// A run inside `Function::args`.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub struct Args {
    pub start: u32,
    pub len: u32,
}

/// A run inside `Function::steps`.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub struct Steps {
    pub start: u32,
    pub len: u32,
}

/// A place: where a `store` writes, and what an `@` argument names. §4.8's own
/// sentence is the specification — "every place has exactly one root" — so a
/// place is a slot plus a path of field and index steps, never an expression.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub struct Place {
    pub root: SlotId,
    pub path: Steps,
}

#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Step {
    /// `.x`, by *index* in the declaration — the name was resolved once, by the
    /// checker, and no later pass compares strings again.
    Field(u32),
    /// `[i]`, the index already in a temporary.
    Index(ValueId),
}

/// One argument of a call. `InOut` is `@l` at the call site: the callee copies
/// in and copies out, so the argument is a *place*, not a value (§4.8).
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Arg {
    Value(ValueId),
    InOut(Place),
}

/// What a call calls — and this enum is the panel's most expensive finding. The
/// resolver decided this once (`Resolved::uses`); recording it here is what stops
/// the emitter from re-deriving it and mangling an `extern`, or failing to mangle
/// a Heroes function whose name collides with libc's.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Callee {
    /// A Heroes function or constant: index into `Ast::decls`. Mangled.
    Heroes(u32),
    /// An `extern function`: index into `Ast::decls`. **Unmangled, by design**
    /// (CLAUDE.md §7) — which is exactly why the two cannot share a variant.
    Extern(u32),
    /// A built-in: index into `resolve::BUILTINS`. The runtime provides tier 1
    /// and M-generics-library's prelude provides tier 2.
    Builtin(u32),
    /// A function value (§4.13): a C function pointer in a temporary.
    Indirect(ValueId),
}

/// What a `Construct` builds. Every one of these is a surface form Part 5 calls
/// sugar and the IR still needs a single instruction for, because expanding it
/// here would put the layout decision in the wrong pass (M-value-aggregates owns it).
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Shape {
    /// `Point(x: 3, y: 4)` — a `record`, fields in **declared** order, the named
    /// arguments already reordered (Part 5's row, erased here).
    Record(u32),
    /// `.num(v: 12)` — a variant case: `(declaration, case index)`.
    Case(u32, u32),
    /// `[1, 2, 3]`
    Array,
    /// `{ "a": 1 }` — arguments alternate key, value.
    Map,
    /// `ok(x)` — the success side of a `T?` (§4.6).
    Ok,
    /// `fail(code, msg)` — the error side, built from two `str`s.
    Fail,
    /// The error side, built from a failure that already exists. This is what `?`
    /// produces: the error propagates **unchanged** into the caller's `T?`, so
    /// nothing re-reads its code and msg on the way (§4.6).
    Err,
}

/// Why the program is stopping. Every one of these is design.md's word — "aborts"
/// — rather than an exception, because §4.6 says no exceptions exist. An `Abort`
/// instruction is always the last in its block, and the block's terminator is
/// `unreachable`: the C it becomes is a `_Noreturn` call.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Abort {
    /// `.must()` on an error (§4.6). The runtime has the failure and prints it.
    Must,
    /// `assert e` failed (§4.18). Carries the source text of `e`, and both sides
    /// when `e` is a comparison — the spec promises both, so lowering carries both
    /// rather than leaving M-generics-library to rewrite this.
    Assert,
}

#[derive(Clone, Copy, PartialEq, Debug)]
pub enum Const {
    Int(i64),
    Float(f64),
    Bool(bool),
    Str(StrId),
}

#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum UnOp {
    /// `-x`. Aborts on `INT64_MIN` (§4.3's overflow rule), which is why the dump
    /// marks it `!`.
    Neg,
    Not,
}

#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum BinOp {
    Add,
    Sub,
    Mul,
    Div,
    Rem,
    /// Structural, on everything, recursively (§4.3). One instruction carrying a
    /// `TyId`; M-value-aggregates's descriptor pass turns it into a `HeroDesc.eq` call rather
    /// than lowering expanding it field by field.
    Eq,
    Ne,
    Lt,
    Le,
    Gt,
    Ge,
}

/// The only conversion the language performs without a call, and it exists for
/// one boundary: C (§4.19). Heroes strings are NUL-terminated so this is free at
/// runtime (§1.11) — it is in the IR so that the dump *shows* it, because §4.3
/// promises no implicit conversions and an invisible one would be a lie.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum CastKind {
    StrToCstr,
}

#[derive(Clone, Copy, PartialEq, Debug)]
pub enum Op {
    Const(Const),
    Load(Place),
    Store { place: Place, value: ValueId },
    Unary { op: UnOp, operand: ValueId },
    Binary { op: BinOp, left: ValueId, right: ValueId },
    Cast { kind: CastKind, operand: ValueId },
    /// `variadic` is set for the one call shape clang verifies nothing about
    /// (`print` today, `TextFormat` and `sqlite3_mprintf` at M-ffi-ladder). The
    /// ffi-pragmatist measured it: a wrong argument count compiles, runs, and
    /// prints garbage, with no diagnostic anywhere.
    Call { callee: Callee, args: Args, variadic: bool },
    Construct { shape: Shape, args: Args },
    /// `p.x` as a *read*. The base is already a value; the field is an index.
    Field { base: ValueId, index: u32 },
    /// `xs[i]` and `s[i]`. Aborts out of bounds — hence `!` in the dump.
    Index { base: ValueId, index: ValueId },
    /// `m[k]`, which is **not** `xs[i]`: it yields a `V?` and cannot abort
    /// (§4.9). Same surface syntax, different instruction.
    MapGet { map: ValueId, key: ValueId },
    Len(ValueId),
    /// The case index of a variant value, or 0/1 for a `T?`'s `ok`/`err`. What a
    /// `Switch` reads.
    Tag(ValueId),
    /// The payload of a known case: `.num n` binds `n` to this. For a `T?`, case
    /// 0 is the `ok` value and case 1 the `Failure`.
    Payload { base: ValueId, case: u32 },
    /// A function as a value (§4.13) — a C function pointer, and no environment,
    /// because a function that captures nothing needs none. It carries a `Callee`
    /// rather than a declaration index for the same reason a call does: an
    /// `extern`'s name must not be mangled, and a built-in's implementation is
    /// somewhere else entirely. `Callee::Indirect` here would mean "the address of
    /// an address", which the verifier rejects.
    FuncRef(Callee),
    /// §4.8's second half, on an exit edge: write an `@` parameter's slot back to
    /// the caller's place. **"Copy-out happens always"** — including on early
    /// `return` and on `?` — so lowering emits one of these per `@` parameter
    /// before *every* `Return`, and the dump shows them. The callee works on a
    /// copy, which is why aliasing cannot exist even here.
    CopyOut { param: SlotId },
    /// The program stops here. What follows in the block is nothing: the
    /// terminator is `unreachable`.
    Abort { reason: Abort, args: Args },
    /// One reference more, one reference fewer (M-strings-ownership, panel 021). Inserted by the
    /// **ownership pass**, never by lowering — which is why they are real
    /// instructions rather than something the emitter does on its own: Swift's SIL
    /// has `strong_retain`/`strong_release` for the same reason, and LLVM D92808
    /// records the failure mode of the alternative, where ARC's pairing lived only
    /// in the backend and passes separated the calls from their markers.
    ///
    /// They name a **value**, not a place: releasing a slot is a `Load` followed by
    /// a `Decref`, which keeps one form instead of two and makes the dominance
    /// check cover it for free.
    ///
    /// `cow_check` is not here. It would have zero call sites until M-value-aggregates gives it
    /// `push`, and an arm in four exhaustive matches that nothing emits is the arm
    /// that rots (panel 021 R1).
    Incref(ValueId),
    Decref(ValueId),
    /// `???` (§4.16). A hole type-checks, so lowering must produce something; the
    /// verifier allows it and the emitter (M-scalars-run) refuses it, which is how "no
    /// binary" is enforced without making a hole an error.
    Hole,
    /// M-ir-lowering step 2 scaffolding: a form the lowering does not handle yet. The
    /// verifier rejects it, so it cannot reach a golden or a backend. Deleted
    /// when the last sugar row lands.
    Missing,
}

/// How a block ends. Exactly one, always — the verifier's first check, and the
/// reason the emitted C never falls through a label (CLAUDE.md §7).
#[derive(Clone, PartialEq, Debug)]
pub enum Term {
    Jump(BlockId),
    /// Polarity is in the *text* of the dump (`-> bb2 else bb4`), not in the
    /// reader's memory: the llm-ergonomist recorded inverting an unlabelled
    /// two-target branch as a silent wrong answer.
    Branch { cond: ValueId, then: BlockId, otherwise: BlockId },
    /// `match` on a variant: dense over the declaration's cases, exhaustive by
    /// the time it gets here (M-data-declarations), so there is no default edge.
    Switch { tag: ValueId, cases: Vec<BlockId> },
    Return(Option<ValueId>),
    /// A point the type system proves unreachable. Becomes `hero_unreachable()`
    /// (CLAUDE.md §7); it is not an abort, it is a claim.
    Unreachable,
    /// Only while a block is being built. The verifier rejects it.
    Open,
}

/// One instruction: at most one destination, an operation, and the span it came
/// from. The span is not decoration — `#line` on source-line change (CLAUDE.md
/// §7) is emitted from it, and R1 (panel 019) needs every message to be able to
/// point back at what the author wrote.
#[derive(Clone, Copy, PartialEq, Debug)]
pub struct Inst {
    pub dest: Option<ValueId>,
    pub op: Op,
    pub ty: TyId,
    pub span: Span,
}
