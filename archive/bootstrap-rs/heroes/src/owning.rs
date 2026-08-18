//! Which operations hand back a **new** reference (design.md §4.10; panel 021,
//! panel 022).
//!
//! Split out of `own.rs` by the §11 sweep. `own.rs` decides *where* an incref and a
//! release go; this decides *what needs one at all*, and it is worth its own file
//! because it is the question the pass gets wrong silently. Every other mistake in
//! the ownership pass is a crash or a sanitiser report; a wrong answer here is a
//! **leak**, which nothing in an ordinary build reports — it took the leak counter
//! saying `3 heap blocks still live at exit` to find the first one.
//!
//! The list is written as an exhaustive `match` rather than an allow-list on
//! purpose: a new IR op has to be classified here, and `hero_unreachable()`'s own
//! rule (CLAUDE.md §11's loud direction) says the failure must be loud rather than
//! `false`.

use crate::ir::{Op, Shape};

/// Whether this operation hands back a **new** reference. Everything else either
/// borrows (a `Load`, a field read) or produces something uncounted.
///
/// A call is owning because a Heroes function increfs what it returns (rule 4), and a
/// built-in is owning because the runtime's constructors return +1. A `Const` of a
/// string literal is **not**: a literal is a static block whose refcount is negative,
/// so nothing owns it and decrefing it is a no-op.
/// **`+` on a `str` allocates**, and forgetting it was this pass's first defect: the
/// leak counter said `3 heap blocks still live at exit` on the first program with a
/// string in it, and all three traced to one missing row. The concatenation is a
/// `Binary`, which reads like arithmetic and is a constructor — the operator table
/// (§4.14) puts `+` on `str` in the same row as `+` on `i64`, and only the *result
/// type* tells them apart. This function is called only when the result is
/// refcounted, so `Op::Binary` here can be nothing else.
pub(crate) fn allocates(op: Op) -> bool {
    match op {
        Op::Call { .. } | Op::Construct { .. } | Op::Cast { .. } | Op::Binary { .. } => true,
        // **`m[k]` allocates**, and it is the only one of these four that does. The
        // other three hand back a *borrowed* view — a field read, an element read and a
        // payload read all copy bytes out of something that keeps its own reference — but
        // a map lookup builds a `V?` around the value, and building it copies the value
        // through its descriptor, which increfs. So the `V?` arrives owning something.
        //
        // The comment this replaces said "listed rather than defaulted so that switching
        // one on is a decision here", and this is that decision. It was found by the leak
        // counter: `{1: "one" + "!"}` then `.default(...)` leaked exactly one block, and
        // only with a heap-allocated value — a `str` literal has a negative refcount, so
        // the same missing decref is invisible.
        Op::MapGet { .. } => true,
        Op::Field { .. } | Op::Index { .. } | Op::Payload { .. } => false,
        Op::Const(_)
        | Op::Load(_)
        | Op::Store { .. }
        | Op::Unary { .. }
        | Op::Len(_)
        | Op::Tag(_)
        | Op::FuncRef(_)
        | Op::CopyOut { .. }
        | Op::Abort { .. }
        | Op::Incref(_)
        | Op::Decref(_)
        | Op::Hole
        | Op::Missing => false,
    }
}

/// Whether a constructor makes its result a **second owner** of every counted
/// argument (rule 6).
///
/// A record and a variant case do: they hold the field by value, so two places now
/// name one block. `ok(x)` and the `?`-propagating `err` do **not** — they *wrap*,
/// taking ownership of the argument rather than adding a reference — and an array or
/// a map literal is the same wrap repeated per element, because `push` moves. Listed
/// one shape at a time rather than defaulted, so that landing a container is a
/// decision here.
pub(crate) fn retains_fields(shape: Shape) -> bool {
    match shape {
        // A record, a variant case and both sides of a `T?` all hold their argument by
        // value, so the new value is a second owner. `ok(x)` was first written as a
        // *wrap* that consumes its argument, on panel 021's note — but rule 5 came
        // later and made every value reaching a constructor borrowed, so consuming one
        // is a release the program never made. The note is older than the rule.
        Shape::Record(_) | Shape::Case(_, _) => true,
        Shape::Ok | Shape::Err | Shape::Fail => true,
        // A container literal is built by `push`, whose `copy` already takes the
        // array's own reference per element.
        Shape::Array | Shape::Map => false,
    }
}

