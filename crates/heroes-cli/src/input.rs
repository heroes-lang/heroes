//! Reading the file, once.
//!
//! Five commands used to carry their own copy of "cannot read `{path}`", their
//! own `USAGE` string and their own idea of what to exit with. The path is
//! validated by `cli.rs`, the read happens here, and every failure to *run*
//! (rather than to check) leaves through `Exit::Failed`.

use crate::cli::Exit;
use heroes::source::Source;

/// The file as a `Source`, or the message to print and the code to exit with.
pub fn read(path: &str) -> Result<Source, (String, Exit)> {
    match std::fs::read_to_string(path) {
        Ok(text) => Ok(Source::new(path.to_string(), text)),
        Err(e) => Err((format!("cannot read `{path}`: {e}"), Exit::Failed)),
    }
}
