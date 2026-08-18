//! What the author sees when the program stops (design.md §4.18; spec § Failure,
//! spec line 163; panels 020, 042).
//!
//! Split out of `inst.rs` by the §11 sweep. The arms stay in that file's exhaustive
//! `match` — that exhaustiveness is what makes a new IR instruction a compile error
//! — and only the bodies moved here, which is the cut the sweep's item named in
//! advance.
//!
//! The concern is single and it is not "aborting": it is **the text a person reads
//! at the moment of the stop**. Everything here runs after the program has already
//! failed, so nothing it emits can be checked by a later pass — a wrong entry point
//! is a wrong message, at exit 1, with no diagnostic anywhere.
//!
//! Both bodies point at the **generated** file (`w.at_generated()`): the panic call
//! is housekeeping the author never wrote, and lldb must not blame their line for it.

use crate::ir::Arg;
use crate::types::Checked;

use super::mangle;
use super::writer::Writer;

/// `.must()` on an error.
///
/// The failure travels with the abort (`ir/fallible.rs`), so the panic names the
/// `code` and `msg` the author wrote rather than only that a `.must()` failed.
pub(super) fn must(w: &mut Writer, function: &crate::ir::Function, args: crate::ir::Args) {
    w.at_generated();
    match function.args_of(args).first() {
        Some(Arg::Value(value)) => {
            w.line(&format!("    hero_panic_must({});", mangle::value(value.0)))
        }
        _ => w.line("    hero_unreachable(); /* a must with no failure */"),
    }
}

/// `assert` (§4.18, spec line 163: "shows the source expression and both sides").
///
/// The IR carries three operands where the asserted expression is a comparison and
/// one where it is not (`ir/asserts.rs`), and each side is rendered through the same
/// `to_str` entry point `print` uses — so a side with no rendering (a record, an
/// array) falls back to the text alone rather than inventing one.
pub(super) fn assert(
    w: &mut Writer,
    checked: &Checked,
    function: &crate::ir::Function,
    args: crate::ir::Args,
) {
    w.at_generated();
    let operands = function.args_of(args);
    let rendered: Vec<String> = operands
        .iter()
        .skip(1)
        .filter_map(|arg| match arg {
            Arg::Value(value) => {
                let ty = checked.types.get(function.value_type(*value));
                // The same reader `to_str` uses. It was a copy of this match until
                // panel 052, and the copy in `builtins.rs` had a `_` arm that sent
                // `u64` to the signed entry point — so the two paths printed
                // different numbers for the same value (see `to_str_entry`).
                let entry = super::builtins::to_str_entry(&ty)?;
                Some(format!("{entry}({})", mangle::value(value.0)))
            }
            Arg::InOut(_) => None,
        })
        .collect();
    let text = match operands.first() {
        Some(Arg::Value(value)) => mangle::value(value.0),
        _ => return w.line("    hero_unreachable(); /* an assert with no text */"),
    };
    if rendered.len() == 2 {
        w.line(&format!(
            "    hero_panic_assert_sides({text}, {}, {});",
            rendered[0], rendered[1]
        ));
    } else {
        // A side with no rendering — a record, an array — is still computed: the
        // lowering does not know what C can print, and asking it to would put a
        // backend question in the IR. Discarding it explicitly is what keeps
        // `-Wunused-but-set-variable` at zero, and the cast says "deliberately"
        // where silence would say "forgotten".
        for arg in operands.iter().skip(1) {
            if let Arg::Value(value) = arg {
                w.line(&format!("    (void){};", mangle::value(value.0)));
            }
        }
        w.line(&format!("    hero_panic_assert({text});"));
    }
}
