//! Reading the file, once.
//!
//! Five commands used to carry their own copy of "cannot read `{path}`", their
//! own `USAGE` string and their own idea of what to exit with. The path is
//! validated by `cli.rs`, the read happens here, and every failure to *run*
//! (rather than to check) leaves through `Exit::Failed`.

use crate::cli::Exit;
use heroes::source::Source;

/// The whole compilation as one `Source`: the file named, every module it
/// reaches through `use`, and the library last (§1.11, `heroes::library`).
///
/// Doing it here rather than per command is what keeps `lex`, `parse`, `check`,
/// `build`, `run`, `test` and `fmt` looking at the same text — and every offset
/// and line number in the file the author named is exactly what it would have
/// been if that file were alone, because nothing is ever prepended to it
/// (`heroes::source`).
///
/// A module named by a `use` and missing from disk is **not** an error here:
/// `heroes::modules::errors` reports it against this `Source`, with the caret on
/// the `use` line. Failing to read the *root* is a different thing — the tool
/// could not run at all — and leaves through `Exit::Failed`.
pub fn read(path: &str) -> Result<Source, (String, Exit)> {
    heroes::modules::load(path).map_err(|message| (message, Exit::Failed))
}
