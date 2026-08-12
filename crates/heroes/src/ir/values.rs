//! What a value must satisfy: assigned once, and **defined on every path that reads
//! it** (design.md Part 10 step 6; panel 020, R4.10).
//!
//! Split out of `verify.rs` when the dominance check pushed it past CLAUDE.md §11's
//! ceiling, and the seam is real rather than convenient: everything left in that file
//! is about a block's *structure* (one terminator, `preds` agreeing, copy-out on every
//! exit edge), and everything here is about a value's *lifetime*. M-strings-ownership's ownership pass
//! reads the second question and not the first.
//!
//! Why the emitter needs dominance. Every temporary is hoisted to the C function's
//! prologue and nothing is initialised there, so a value read in a block its
//! definition does not dominate is a read of an uninitialised C local.
//! `-Werror=uninitialized` catches it — measured — but what it prints is `variable 't3'
//! is used uninitialized`, `#line`-mapped into the author's `.hero` file: a compiler
//! bug wearing a user diagnostic, which is design.md §8's known wart 13 happening to
//! somebody who did nothing wrong. Failing here instead means the message says the
//! compiler is wrong, in the IR's own words.
//!
//! The first version of the invariant was **"a temporary is read only in the block
//! that defines it"**, and the `assert` lowering falsified it immediately and
//! correctly: it computes both sides of a comparison in the test block and reads them
//! in the abort block, which the test block dominates. The weaker, true statement is
//! the one worth asserting.
//!
//! The algorithm is the textbook fixpoint, unadorned: `dom(entry) = {entry}` and
//! `dom(b) = {b} ∪ ⋂ dom(p)` over `b`'s predecessors, iterated until nothing changes.
//! O(blocks²) on a bitset per block, which for a function whose largest recorded block
//! count is 226 across a 320-line program is not worth a Lengauer–Tarjan.

use super::inst::ValueId;
use super::uses::{operands, terminator_operands};
use super::{Block, BlockId, Function};

/// Both value invariants, on one function.
pub(super) fn check(
    function: &Function,
    where_: &dyn Fn(String) -> String,
    problems: &mut Vec<String>,
) {
    let defined_in = definitions(function, where_, problems);
    dominance(function, &defined_in, where_, problems);
}

/// **A definition dominates every use of it** (panel 020, R4.10).
///
/// The check above says a value is produced *somewhere*; this one says it is produced
/// on **every** path that reads it. Nothing asserted that until M-scalars-run, and the emitter
/// is what makes it matter: temporaries are hoisted to the C prologue and left
/// uninitialised, so a use its definition does not dominate reads an uninitialised
/// local. `-Werror=uninitialized` catches it and reports `variable 't3' is used
/// uninitialized` at a `#line`-mapped position in the author's file — a compiler bug
/// wearing a user diagnostic (design.md §8's wart 13). Failing here instead means the
/// message says the compiler is wrong, in the IR's own words.
///
/// It lands before M-generics-library, because monomorphisation is the first pass that copies blocks.
fn dominance(
    function: &Function,
    defined_in: &[Option<usize>],
    where_: &dyn Fn(String) -> String,
    problems: &mut Vec<String>,
) {
    let dom = dominators(&function.blocks);
    for (index, block) in function.blocks.iter().enumerate() {
        let mut read: Vec<ValueId> = Vec::new();
        for inst in &block.insts {
            read.extend(operands(function, inst.op));
        }
        read.extend(terminator_operands(&block.term));
        for value in read {
            // `$t0` is the unit, defined by the function rather than by a block.
            if value.0 == 0 {
                continue;
            }
            let Some(Some(home)) = defined_in.get(value.0 as usize) else { continue };
            if !dom[index][*home] {
                problems.push(where_(format!(
                    "bb{index} reads ${}, defined in bb{home}, which does not dominate it",
                    value.0
                )));
            }
        }
    }
}

/// Every value a function names must be produced by some instruction in it — or be
/// `$t0`, the unit, which the function itself defines (`build.rs`).
fn definitions(
    function: &Function,
    where_: &dyn Fn(String) -> String,
    problems: &mut Vec<String>,
) -> Vec<Option<usize>> {
    let mut home: Vec<Option<usize>> = vec![None; function.values.len()];
    let mut defined = vec![false; function.values.len()];
    if !defined.is_empty() {
        defined[0] = true;
    }
    for (index, block) in function.blocks.iter().enumerate() {
        for inst in &block.insts {
            if let Some(dest) = inst.dest {
                if (dest.0 as usize) < defined.len() {
                    if defined[dest.0 as usize] {
                        problems.push(where_(format!("${} is assigned twice", dest.0)));
                    }
                    defined[dest.0 as usize] = true;
                    home[dest.0 as usize] = Some(index);
                }
            }
        }
    }
    for block in &function.blocks {
        for inst in &block.insts {
            for value in operands(function, inst.op) {
                if (value.0 as usize) < defined.len() && !defined[value.0 as usize] {
                    problems.push(where_(format!("${} is used and never produced", value.0)));
                }
            }
        }
        for value in terminator_operands(&block.term) {
            if (value.0 as usize) < defined.len() && !defined[value.0 as usize] {
                problems.push(where_(format!("${} is used and never produced", value.0)));
            }
        }
    }
    home
}

/// `dominators[b][d]` — whether block `d` dominates block `b`.
fn dominators(blocks: &[Block]) -> Vec<Vec<bool>> {
    let count = blocks.len();
    let mut dom: Vec<Vec<bool>> = vec![vec![true; count]; count];
    if count == 0 {
        return dom;
    }
    // The entry is dominated by itself alone; everything else starts maximal and
    // shrinks, which is what makes the fixpoint a greatest one.
    for (index, row) in dom.iter_mut().enumerate() {
        if index == 0 {
            row.fill(false);
            row[0] = true;
        }
    }
    let mut changed = true;
    while changed {
        changed = false;
        for index in 1..count {
            let preds: Vec<BlockId> = blocks[index].preds.clone();
            let mut next = vec![preds.is_empty(); count];
            for (position, pred) in preds.iter().enumerate() {
                let row = &dom[pred.0 as usize];
                if position == 0 {
                    next.copy_from_slice(row);
                } else {
                    for (slot, dominated) in next.iter_mut().zip(row) {
                        *slot = *slot && *dominated;
                    }
                }
            }
            next[index] = true;
            if next != dom[index] {
                dom[index] = next;
                changed = true;
            }
        }
    }
    dom
}
