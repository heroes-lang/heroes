//! Lowering: the checked tree becomes a three-address IR with explicit basic
//! blocks (design.md Part 10 step 6 — "this is the heart"; Part 5; panel 019).
//!
//! **Two jobs, one pass.** Part 5's sugar table is erased *on the way in* — there
//! is no desugared tree — and the result is flat: every expression becomes a
//! sequence of instructions naming temporaries, and every control-flow construct
//! becomes labelled blocks and jumps. Panel 019 costed the alternative: a second
//! tree would be ~450 lines and would invalidate the two dense side tables the
//! frontend already produces (`Checked::expr_types` and `Resolved::uses`, both
//! indexed by `Ast::exprs`). GHC is the precedent usually cited for the extra
//! tree, and what Core actually bought GHC was **Core Lint** — so this module
//! buys the check instead: `verify.rs` runs on every lowered function in tests.
//!
//! **Slots, not SSA** (panel 019 point 2, unanimous). A local, a parameter and a
//! mutable cell are all *slots*, read with `load` and written with `store`;
//! temporaries are assigned once by construction; there are no phi nodes. A
//! branching *expression* — `x = if c` — writes a synthetic slot from each arm and
//! loads it at the join. The decisive precedent is LLVM's own advice to frontend
//! authors ("we strongly recommend … alloca + load/store … unless there is an
//! extremely good reason not to"; clang does it for local mutable variables),
//! and the local reason is M5b: with slots, "release live locals and copy out `@`
//! parameters on every exit edge" is a walk over a fixed table, where phi would
//! make "who owns this value on this edge" a dataflow problem.
//!
//! | file | idea |
//! |------|------|
//! | `inst.rs`   | what an instruction is, and the five things C proved it must carry |
//! | `build.rs`  | the pass's state: blocks, instructions, terminators |
//! | `slots.rs`  | the other half of it: slots, runs, the `$` names |
//! | `decls.rs`  | one declaration at a time: signature, body, `test`, `extern` |
//! | `stmts.rs`  | statements, and the loop stack `break`/`continue` need |
//! | `exprs.rs`  | expressions → a temporary holding the value |
//! | `places.rs` | the left side of a mutation, and an `@` argument's root |
//! | `control.rs`| `if`, `while`, `for`, `match` — control flow's sugar, erased |
//! | `fallible.rs`| `?`, `.must()`, `.default()`, `.is_err()`, `ok`, `fail` |
//! | `calls.rs`  | UFCS, named arguments, construction, `.case` |
//! | `asserts.rs`| `assert` and `test` (§4.18), with both sides carried |
//! | `matches.rs`| `match`: a `switch` on a tag, or a chain of comparisons |
//! | `layout.rs` | where a field lives, and which case a name is — by index |
//! | `print.rs` · `print_inst.rs` · `print_names.rs` | the dump: the shape, one instruction, and the names |
//! | `uses.rs`   | what an instruction reads — the one def-use table |
//! | `verify.rs` | the invariants, checked on every function in tests |
//!
//! **Lowering runs only on a tree that checked clean**, for the same reason the
//! resolver runs only on one that parsed clean: after a type error the checker's
//! answers are guesses, and `expr_types` would be `Error` in places the lowering
//! reads as fact. It is the caller that declines to ask.

use crate::diagnostics::Diagnostic;
use crate::resolve::Resolved;
use crate::source::{Source, Span};
use crate::syntax::Ast;
use crate::types::{Checked, TyId};

mod asserts;
mod build;
mod calls;
mod control;
mod decls;
mod exprs;
mod fallible;
mod inst;
mod layout;
mod matches;
mod places;
mod slots;
mod print;
mod print_inst;
mod print_names;
mod stmts;
mod uses;
mod verify;

#[cfg(test)]
mod tests;

pub use inst::{
    Abort, Arg, Args, BinOp, BlockId, Callee, CastKind, Const, Inst, Op, Place, Shape, SlotId, Step,
    Steps, StrId, Term, UnOp, ValueId,
};
pub use print::dump;
pub use verify::verify;

/// What a lowered function *is*: a `constant`'s body, a `function`, an `extern`
/// with no body, or a `test` block.
///
/// A `constant` becomes a zero-argument function because its body is a block
/// whose value is its last expression (§4.2) — there are no mutable globals to
/// initialise, so the only shape that needs no new machinery is a call. M5a may
/// emit a C initialiser where the body is a literal; that is the emitter's
/// optimisation, not the IR's concern.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum FnKind {
    Function,
    Constant,
    /// §4.19: no body, and the name reaches C **unmangled**.
    Extern,
    /// §4.18: entered only by `heroes test` (M6). Ordinary builds ignore it.
    Test,
}

/// Where a slot came from. It decides one thing beyond the dump: whether an exit
/// edge owes a `CopyOut` (§4.8).
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum SlotKind {
    Param { mutable: bool },
    /// `x = 5` or `v: int @ 0` — the surface distinction is the resolver's, and
    /// by here both are cells that hold a value.
    Local,
    /// One lowering invented: a loop index, a branch's join. Its name starts with
    /// `$`, a character no Heroes program contains, so it can collide with
    /// nothing the author wrote.
    Synthetic,
}

pub struct Slot {
    pub name: String,
    pub ty: TyId,
    pub kind: SlotKind,
}

/// One basic block: where control can arrive from, what happens, and the single
/// edge out.
///
/// `preds` and `note` are both there for the reader. The llm-ergonomist measured
/// the cost of their absence on a five-block function: answering "what is this
/// block's role" took a full pass over every block, looking for jumps to it —
/// O(blocks) for a question that should be local.
pub struct Block {
    pub preds: Vec<BlockId>,
    pub insts: Vec<Inst>,
    pub term: Term,
    /// What this block is for, in three words: `loop head`, `for: step`, `join`.
    pub note: String,
}

pub struct Function {
    /// The source name, unmangled. The mangler is M5a's, and it needs the module
    /// this came from, which does not exist until M8a.
    pub name: String,
    /// Index into `Ast::decls` — the link back to everything the frontend knows.
    pub decl: u32,
    pub kind: FnKind,
    /// The type-parameter letters, in order, so the dump prints what the author
    /// wrote. A body lowers **polymorphically**: `Ty::Generic(i)` survives into
    /// the IR and monomorphisation is an IR→IR pass at M6 (§4.12, amended by
    /// panel 019).
    pub generics: Vec<String>,
    pub params: Vec<SlotId>,
    pub result: TyId,
    pub slots: Vec<Slot>,
    pub blocks: Vec<Block>,
    /// The type of every temporary, dense over `ValueId`.
    pub values: Vec<TyId>,
    /// The argument pool. `Args` runs point in here — the arity of a call is
    /// variable, and `Op` stays `Copy`.
    pub args: Vec<Arg>,
    /// The field/index-step pool, for the same reason: `Place` stays `Copy`.
    pub steps: Vec<Step>,
    pub span: Span,
}

impl Function {
    pub fn args_of(&self, args: Args) -> Vec<Arg> {
        let start = args.start as usize;
        self.args[start..start + args.len as usize].to_vec()
    }

    pub fn steps_of(&self, steps: Steps) -> Vec<Step> {
        let start = steps.start as usize;
        self.steps[start..start + steps.len as usize].to_vec()
    }

    pub fn value_type(&self, value: ValueId) -> TyId {
        self.values[value.0 as usize]
    }
}

pub struct Program {
    /// In source order. Declaration order carries no meaning (§4.2), but the dump
    /// must be deterministic and the order the author reads is the one that costs
    /// nothing to preserve.
    pub functions: Vec<Function>,
    /// Decoded string literals, interned. `"a\nb"` is two lines here, not four
    /// characters: the escape rules (panel 008) are applied once, at lowering,
    /// and the emitter re-escapes for C.
    pub strings: Vec<String>,
}

pub struct Lowered {
    pub program: Program,
    /// Lowering is not a checking pass and produces almost nothing here. The one
    /// class it owns is a literal that cannot be represented — nothing before this
    /// pass ever needed an `int` literal's *value*, so an out-of-range one was
    /// invisible until now (panel 019 § Watch list, queued as its own trigger).
    pub diagnostics: Vec<Diagnostic>,
}

/// Lower one checked file.
pub fn lower(ast: &Ast, resolved: &Resolved, checked: &Checked, src: &Source) -> Lowered {
    let mut b = build::Lowering::new(resolved, checked.types.unit());
    for index in 0..ast.decls.len() {
        decls::declaration(&mut b, ast, resolved, checked, src, index as u32);
    }
    let mut out = b.out;
    out.diagnostics.sort_by_key(|d| d.span.start);
    out
}
