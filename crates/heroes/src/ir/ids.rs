//! How the IR names things — every reference an index, every run a range
//! (design.md §4.10, Part 5; CLAUDE.md §5; panel 019).
//!
//! Split out of `inst.rs` by the §11 sweep. `inst.rs` says what an instruction
//! *is*; this says what an instruction can *refer to*, and the two separate cleanly
//! because everything here is a consequence of one rule that has nothing to do with
//! instructions: **no struct in this compiler may hold a reference** (the Cyclone
//! rule), and §4.10 makes the array Heroes' only indirection.
//!
//! So an id is a `u32` index into a dense table, and anything of variable length is
//! a **run** — a start and a length into an arena the `Function` owns. That is what
//! keeps `Op` `Copy` even though `Call` and `Construct` are n-ary, and it is the
//! same trick `Ty` uses for a function's parameters. The port reads all of it as
//! two arrays.
//!
//! `Place` is here rather than with the store that writes it because §4.8's own
//! sentence is its specification — *"every place has exactly one root"* — which
//! makes it a slot plus a path, never an expression.

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
