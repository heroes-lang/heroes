//! Writing the tree back out as text (design.md §4.15, Part 10 step 3).
//!
//! Part 10 puts the printer early on purpose: error messages, `???` output
//! and the mandatory canonical formatter are the same job, so it is written
//! once and used three times.
//!
//! | file        | idea |
//! |-------------|------|
//! | `types.rs`  | a type in surface syntax — the piece diagnostics need first |
//! | `bodies.rs` | statements and expressions, the latter fully parenthesised |
//! | `dump.rs`   | `--dump-ast`: the tree seen, one node per line |
//!
//! `heroes fmt` — whole declarations in canonical form — lands in M2 step 4
//! beside these.

mod bodies;
mod dump;
mod types;

pub use bodies::render_expr;
pub use dump::dump_ast;
pub use types::render_type;
