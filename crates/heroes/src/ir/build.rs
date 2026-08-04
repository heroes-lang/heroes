//! The pass's state, and the only thing that may add a block, a slot or a
//! temporary (design.md Part 10 step 6; panel 019 points 2 and 3).
//!
//! It owns everything and takes `&Ast`/`&Resolved`/`&Checked`/`&Source` as
//! parameters, which is the Cyclone rule (CLAUDE.md §5) and the same shape
//! `Resolver` and `Checker` have: nothing here may store a reference.
//!
//! Every invariant `verify.rs` checks is one this file makes hard to break:
//!
//! - **A temporary is assigned once.** `value()` hands out a fresh `ValueId` and
//!   records its type; nothing can name one twice.
//! - **A block ends exactly once.** `terminate` refuses a block that already
//!   ended, and it is the only place edges are recorded — so `preds` cannot
//!   disagree with the terminators.
//! - **Nothing is emitted after a terminator.** `push` drops it, and the
//!   statement loops stop: statements after a `return` cannot execute, so
//!   lowering them would put unreachable instructions in the dump.
//!
//! Slots, runs and the `$` counters live in `slots.rs` — the same state, split by
//! concern: this file is what *happens*, that one is what holds a value.

use std::collections::BTreeMap;

use crate::diagnostics::Diagnostic;
use crate::resolve::Resolved;
use crate::source::Span;
use crate::types::TyId;

use super::inst::{BlockId, Inst, Op, SlotId, StrId, Term, ValueId};
use super::{Block, FnKind, Function, Lowered, Program};

/// Where `break` and `continue` go. A stack, because loops nest; `break` leaves
/// and `continue` re-enters, and for a `for` loop `continue` must land on the
/// *step* block rather than the head — the increment has to happen on that path
/// too. That single fact is why a `for` needs four blocks and a `while` three.
pub struct Loop {
    pub break_to: BlockId,
    pub continue_to: BlockId,
}

pub struct Lowering {
    pub out: Lowered,
    /// The function being built. One at a time: `begin` resets it, `end` files it.
    /// Visible to `slots.rs` and to nothing else: those two files *are* the pass's
    /// state, and every other file in `ir/` goes through a method.
    pub(super) func: Function,
    pub current: BlockId,
    pub loops: Vec<Loop>,
    /// A resolver local index → the slot that holds it. The resolver already
    /// answered "which binding is this name"; this map is the only translation,
    /// and no pass here ever looks a name up again (`resolve/mod.rs`'s rule).
    pub slots_by_local: BTreeMap<u32, SlotId>,
    /// Where a local's name starts → its index in `Resolved::locals`. Built once,
    /// so lowering does not depend on walking the tree in the resolver's order —
    /// the checker keys the same way, for the same reason.
    pub locals_by_span: BTreeMap<u32, u32>,
    /// One counter per synthetic prefix, so two loops in a row are `$i0` and
    /// `$i1` rather than `$i0` and `$i4`.
    pub(super) counters: BTreeMap<String, u32>,
    /// `()`, interned once. Every function reserves `ValueId(0)` for it.
    unit: TyId,
}

impl Lowering {
    pub fn new(resolved: &Resolved, unit: TyId) -> Lowering {
        Lowering {
            out: Lowered {
                program: Program { functions: Vec::new(), strings: Vec::new() },
                diagnostics: Vec::new(),
            },
            func: empty(),
            current: BlockId(0),
            loops: Vec::new(),
            slots_by_local: BTreeMap::new(),
            locals_by_span: resolved
                .locals
                .iter()
                .enumerate()
                .map(|(index, local)| (local.name.start, index as u32))
                .collect(),
            counters: BTreeMap::new(),
            unit,
        }
    }

    // --- one function at a time ------------------------------------------

    pub fn begin(
        &mut self,
        name: String,
        decl: u32,
        kind: FnKind,
        result: TyId,
        generics: Vec<String>,
        span: Span,
    ) {
        self.func = empty();
        self.func.name = name;
        self.func.decl = decl;
        self.func.kind = kind;
        self.func.result = result;
        self.func.generics = generics;
        self.func.span = span;
        self.current = BlockId(0);
        self.loops.clear();
        self.slots_by_local.clear();
        self.counters.clear();
        // `ValueId(0)` is `()`, in every function: the value of everything that
        // produces none. It is defined by the function rather than by an
        // instruction, which is what lets a call to a `()`-returning function have
        // **no destination** — `dst = call print(x)` is a hard clang error, and
        // raylib is almost entirely void-returning (panel 019, ffi-pragmatist).
        self.func.values.push(self.unit);
    }

    /// The one value no instruction produces.
    pub fn unit_value(&self) -> ValueId {
        ValueId(0)
    }

    pub fn unit_ty(&self) -> TyId {
        self.unit
    }

    /// Opens `bb0`. Called for a declaration that *has* a body: an `extern` has
    /// no blocks at all, and a function with zero blocks is how the dump and the
    /// emitter tell the two apart without consulting the tree.
    pub fn entry(&mut self) {
        self.current = self.block("entry");
    }

    pub fn end(&mut self) {
        let func = std::mem::replace(&mut self.func, empty());
        self.out.program.functions.push(func);
    }

    /// The span of the whole declaration being lowered — what an instruction the
    /// author did not write points at (a copy-out, for instance).
    pub fn declaration_span(&self) -> Span {
        self.func.span
    }

    /// The result type the enclosing function promised. `?` needs it: the error it
    /// propagates is wrapped into *this* function's `T?`.
    pub fn result_type(&self) -> TyId {
        self.func.result
    }

    // --- blocks ----------------------------------------------------------

    pub fn block(&mut self, note: &str) -> BlockId {
        self.func.blocks.push(Block {
            preds: Vec::new(),
            insts: Vec::new(),
            term: Term::Open,
            note: note.to_string(),
        });
        BlockId(self.func.blocks.len() as u32 - 1)
    }

    pub fn switch_to(&mut self, block: BlockId) {
        self.current = block;
    }

    /// Whether anything jumps here. A join with no predecessors is a join every arm
    /// diverged past, and lowering says `unreachable` rather than leaving a load
    /// nothing can reach.
    pub fn has_preds(&self, block: BlockId) -> bool {
        !self.func.blocks[block.0 as usize].preds.is_empty()
    }

    pub fn is_terminated(&self) -> bool {
        self.func.blocks[self.current.0 as usize].term != Term::Open
    }

    /// Ends the current block, and records the edges it creates. Silently
    /// declining to end an already-ended block is deliberate: the alternative is
    /// every caller asking `is_terminated()` first, and the one that forgets
    /// produces a block with two exits.
    pub fn terminate(&mut self, term: Term) {
        if self.is_terminated() {
            return;
        }
        for target in successors(&term) {
            let from = self.current;
            let preds = &mut self.func.blocks[target.0 as usize].preds;
            if !preds.contains(&from) {
                preds.push(from);
            }
        }
        self.func.blocks[self.current.0 as usize].term = term;
    }

    // --- instructions ----------------------------------------------------

    /// An instruction with a destination: the temporary it produces is returned,
    /// so a lowering rule reads `let v = b.emit(...)`.
    pub fn emit(&mut self, op: Op, ty: TyId, span: Span) -> ValueId {
        let dest = self.value(ty);
        self.push(Inst { dest: Some(dest), op, ty, span });
        dest
    }

    /// An instruction with no destination: a `store`, a `copyout`, a call to a
    /// function that returns nothing. The last one is not a nicety — `dst = call
    /// print(x)` is a hard clang error, which the ffi-pragmatist compiled.
    pub fn emit_void(&mut self, op: Op, ty: TyId, span: Span) {
        self.push(Inst { dest: None, op, ty, span });
    }

    fn push(&mut self, inst: Inst) {
        if self.is_terminated() {
            return;
        }
        self.func.blocks[self.current.0 as usize].insts.push(inst);
    }

    fn value(&mut self, ty: TyId) -> ValueId {
        self.func.values.push(ty);
        ValueId(self.func.values.len() as u32 - 1)
    }

    // --- program-wide ----------------------------------------------------

    /// Interns a decoded string. Linear search: a file has few distinct literals,
    /// and interning keeps the ids stable across two runs of the same input,
    /// which the determinism test asserts.
    pub fn intern(&mut self, text: String) -> StrId {
        if let Some(index) = self.out.program.strings.iter().position(|s| *s == text) {
            return StrId(index as u32);
        }
        self.out.program.strings.push(text);
        StrId(self.out.program.strings.len() as u32 - 1)
    }

    pub fn push_diagnostic(&mut self, diagnostic: Diagnostic) {
        self.out.diagnostics.push(diagnostic);
    }
}

fn empty() -> Function {
    Function {
        name: String::new(),
        decl: 0,
        kind: FnKind::Function,
        generics: Vec::new(),
        params: Vec::new(),
        result: TyId(0),
        slots: Vec::new(),
        blocks: Vec::new(),
        values: Vec::new(),
        args: Vec::new(),
        steps: Vec::new(),
        span: Span { start: 0, end: 0 },
    }
}

/// Every block a terminator can reach. One function, so `preds` and the
/// terminators cannot drift apart — `verify.rs` recomputes with this same list.
pub fn successors(term: &Term) -> Vec<BlockId> {
    match term {
        Term::Jump(target) => vec![*target],
        Term::Branch { then, otherwise, .. } => vec![*then, *otherwise],
        Term::Switch { cases, .. } => cases.clone(),
        Term::Return(_) | Term::Unreachable | Term::Open => Vec::new(),
    }
}
