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
//! | `scopes.rs` | `--dump-scopes`: the symbol table and every binding |
//!
//! `gallery.rs` (tests only) holds `examples/gallery/` to the properties this
//! module claims: every example parses clean, is canonical byte for byte, and
//! survives formatting with its tree intact.
//! | `fmt.rs`      | `heroes fmt`: declarations, comments, blank lines |
//! | `fmt_stmt.rs` | its statement half, including where a long line breaks |
//! | `fmt_expr.rs` | expressions with the *minimum* parentheses |
//!
//! Two renderings of the same tree, on purpose. The dump answers "what did
//! the parser understand?" and parenthesises everything; `fmt` answers "how
//! is this program written?" and parenthesises nothing it does not have to.

#[cfg(test)]
mod gallery;
#[cfg(test)]
mod tests;

mod bodies;
mod dump;
mod fmt;
mod fmt_expr;
mod fmt_stmt;
mod scopes;
mod types;

pub use bodies::render_expr;
pub use dump::dump_ast;
pub use fmt::format_file;
pub use scopes::dump_scopes;
pub use types::render_type;
