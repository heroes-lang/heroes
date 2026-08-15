//! Every type error, in one place — one diagnostic class, judged together
//! (design.md §4.17: the error carries what is needed to fix the program).
//!
//! Two shapes of repair, and the line is §4.17's: a `Certain` fix preserves
//! meaning and the compiler knows it does (`_ = expr` to discard a value on
//! purpose, a field label the position already determines); a `Guess` is prose,
//! because the repair depends on intent (which of two numeric types was meant).
//!
//! | file | subject |
//! |------|---------|
//! | `values.rs` | mismatches, operators, and where a value may stand |
//! | `data.rs`   | fields, labels, cases, patterns, exhaustiveness |
//! | `ffi.rs`    | what may cross the C boundary — a signature, a `constant`, a field |
//! | `flow.rs`   | `return`, jumps, `?`, and branches without a value |
//! | `sizes.rs`  | a type that would have to contain itself at every depth |
//!
//! The rich renderer is M-rich-diagnostics's deliverable. These messages are written so the
//! one-line form already carries the two types and the repair — a message that
//! only said "type mismatch" would be the anti-thesis.

mod data;
mod ffi;
mod flow;
mod sizes;
mod contextless;
mod values;

pub(in crate::types) use contextless::*;
pub(in crate::types) use data::*;
pub(in crate::types) use ffi::*;
pub(in crate::types) use flow::*;
pub(in crate::types) use sizes::*;
pub(in crate::types) use values::*;
