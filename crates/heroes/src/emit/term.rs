//! How a block ends (design.md §3.1, CLAUDE.md §7).
//!
//! **One `goto` or one `return` per block, always.** The IR guarantees exactly one
//! terminator (`ir/verify.rs`'s first check), which is what makes the emitted C
//! never fall through a label — and falling through a label is the failure that
//! would be invisible in the source and obvious only in the output.
//!
//! A `Branch` is emitted with both edges written out, `if (c) goto a; else goto b;`
//! rather than a bare `if` with an implicit fallthrough. Physical adjacency then
//! carries no meaning, so blocks can be emitted in any order and unreachable ones
//! can be skipped entirely — which is what `decls.rs` does.
//!
//! `Unreachable` becomes `hero_unreachable()`, and the distinction it draws is the
//! point: an abort is something the program *does*, and this is something the type
//! system *claims*. Reaching it means the compiler is wrong, and finding that out
//! through undefined behaviour is not acceptable.

use crate::ir::{Function, Term};
use crate::types::Checked;

use super::ctype::is_unit;
use super::mangle;
use super::writer::Writer;

pub(super) fn emit(w: &mut Writer, function: &Function, checked: &Checked, term: &Term) {
    match term {
        Term::Jump(target) => {
            w.line(&format!("    goto {};", mangle::block(target.0 as usize)));
        }
        Term::Branch { cond, then, otherwise } => {
            w.line(&format!(
                "    if ({}) goto {}; else goto {};",
                mangle::value(cond.0),
                mangle::block(then.0 as usize),
                mangle::block(otherwise.0 as usize)
            ));
        }
        // A `match` on a variant. `gate.rs` refuses it until the tag has a
        // representation (M5c), and the arm stays here so that the day it lands is
        // a compile error rather than a silent gap.
        Term::Switch { .. } => {
            w.line("    hero_unreachable(); /* the gate refuses this terminator */");
        }
        Term::Return(None) => w.line("    return;"),
        Term::Return(Some(value)) => {
            // The unit rule from the other side: a function whose result is `()`
            // returns nothing, even where the IR names the unit value.
            if is_unit(checked, function.value_type(*value)) {
                w.line("    return;");
            } else {
                w.line(&format!("    return {};", mangle::value(value.0)));
            }
        }
        Term::Unreachable => w.line("    hero_unreachable();"),
        // `ir/verify.rs` rejects this before any backend sees it.
        Term::Open => w.line("    hero_unreachable(); /* a block was left open */"),
    }
}
