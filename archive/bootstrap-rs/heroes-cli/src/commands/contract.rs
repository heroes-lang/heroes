//! CLAUDE.md §10's contract, for the three verbs that compile (panels 016, 019).
//!
//! Split out of `compile.rs` by the §11 sweep. What is here is not pipeline: it is
//! **how this command talks** — which is a separate concern with a separate rule,
//! and the rule is one sentence. *The artifact on stdout, diagnostics on stderr;
//! exit 0 clean, 1 the input has diagnostics, 2 the tool could not run.*
//!
//! Two consequences of that sentence live here and are easy to lose if they sit
//! among the compiler passes:
//!
//! - **a flag that asked two questions is an error, not an answer to one of
//!   them.** `-O0 -O2` is refused rather than resolved by precedence, the same
//!   reading `build` gives `--dump-ir --emit-c`.
//! - **`build` and `run` take no diagnostic-shaping flags.** `check` already
//!   answers every question about how a diagnostic prints, and two commands with
//!   two renderings of one diagnostic is the surface panel 016 exists to prevent.

use heroes::diagnostics::{render, Diagnostic};
use heroes::source::Source;

use crate::cli::Invocation;
use crate::Exit;

/// The optimisation level this invocation asked for, or the verb's default.
///
/// **Two levels and no others**, which is CLAUDE.md §10's stopping rule applied to
/// a flag rather than to a verb: the `run/` golden harness types `-O0` and `-O2`
/// (every case runs at both), and nothing types `-O1`, `-O3` or `-Os`. A level the
/// harness does not need is surface nobody must type.
///
/// Spelled attached, as clang spells it, so the flag a reader already knows is the
/// flag that works. Both at once is refused rather than resolved by precedence —
/// the same reading `build` gives `--dump-ir --emit-c`: an invocation that asked
/// two questions gets an error, not an answer to one of them.
pub fn level_from(args: &Invocation, default: &'static str) -> Result<&'static str, Exit> {
    match (args.has("-O0"), args.has("-O2")) {
        (true, true) => {
            eprintln!("error: `-O0` and `-O2` are one choice — ask for one level at a time");
            Err(Exit::Failed)
        }
        (true, false) => Ok("-O0"),
        (false, true) => Ok("-O2"),
        (false, false) => Ok(default),
    }
}



pub(super) fn write(path: &str, text: &str) -> Result<(), Exit> {
    if let Some(parent) = std::path::Path::new(path).parent() {
        if !parent.as_os_str().is_empty() {
            let _ = std::fs::create_dir_all(parent);
        }
    }
    std::fs::write(path, text).map_err(|e| {
        eprintln!("error: cannot write {path}: {e}");
        Exit::Failed
    })
}

/// §4.17's rich form, on stderr, and `Exit::Diagnostics`.
///
/// `build` and `run` take no diagnostic-shaping flags: `check` already answers every
/// question about how to print them, and two commands with two renderings of one
/// diagnostic is the kind of surface panel 016 exists to prevent.
pub(super) fn report(diagnostics: &[Diagnostic], src: &Source) -> Result<(), Exit> {
    if diagnostics.is_empty() {
        return Ok(());
    }
    // A diagnostic pointing into the library is the COMPILER being wrong, not the
    // program: the line it names is in a file the author cannot open, so the
    // message is unactionable however good it is. Exit 2 and say so
    // (CLAUDE.md §10's contract; the class was panel 028 R5's).
    if let Some(what) = heroes::library::misplaced(diagnostics, src) {
        eprintln!("internal error: {what}");
        return Err(Exit::Failed);
    }
    let rendered: Vec<String> = diagnostics.iter().map(|d| render(d, src)).collect();
    eprint!("{}", rendered.join("\n"));
    Err(Exit::Diagnostics)
}