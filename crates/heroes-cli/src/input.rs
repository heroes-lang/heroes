//! Reading the file, once.
//!
//! Five commands used to carry their own copy of "cannot read `{path}`", their
//! own `USAGE` string and their own idea of what to exit with. The path is
//! validated by `cli.rs`, the read happens here, and every failure to *run*
//! (rather than to check) leaves through `Exit::Failed`.

use crate::cli::Exit;
use heroes::source::Source;

/// The file as a `Source`, **with the library attached** — every command that
/// resolves names needs Tier 2's source in scope (§1.11, `heroes::library`), and
/// attaching it here rather than per command is what keeps `lex`, `parse`,
/// `check`, `build`, `run` and `fmt` looking at the same text.
///
/// The library is appended, so every offset and line number the author can see
/// is exactly what it would have been without it.
pub fn read(path: &str) -> Result<Source, (String, Exit)> {
    match std::fs::read_to_string(path) {
        Ok(text) => Ok(heroes::library::attach(path.to_string(), text)),
        Err(e) => Err((format!("cannot read `{path}`: {e}"), Exit::Failed)),
    }
}
