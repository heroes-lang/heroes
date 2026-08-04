//! `heroes run <file.hero> [-o <path>]` — compile at `-O2` and execute (M5a).
//!
//! The dev loop, and the verb CLAUDE.md §6 sanctions by name (`nim r` → `heroes
//! run`). It is a **subcommand** rather than a flag because it answers a different
//! question with a different artifact class: `build` produces a file, `run` produces
//! a process. Panel 020 struck the warrant originally offered for it — the M8c
//! fixpoint invocation does *not* contain `run` — and kept the verb on the honest
//! one: design.md §3.5 names it, and the `run/` golden harness needs the
//! compile-and-execute leg at `-O2`.
//!
//! **The program's exit status is this command's.** The program is the artifact, so
//! its verdict is the answer; the tool's own failures stay at 2. A Heroes program
//! that aborts therefore surfaces as the signal it really died of, which is what
//! makes `hero_panic`'s `abort()` visible to a harness instead of being flattened
//! into "the compiler failed".

use crate::cli::{Exit, Invocation};

use super::compile::{compile, Options};

pub fn run(path: &str, args: &Invocation) -> Exit {
    let options = Options {
        level: "-O2",
        dump_ir: false,
        emit_c: false,
        output: args.value_of("-o"),
    };
    let binary = match compile(path, &options) {
        Err(exit) => return exit,
        Ok(None) => return Exit::Ok,
        Ok(Some(binary)) => binary,
    };
    // No line of our own before the program's output: stdout is the program's from
    // here, and a progress line on stderr would still interleave in a terminal.
    let status = match std::process::Command::new(&binary).status() {
        Ok(status) => status,
        Err(e) => {
            eprintln!("error: cannot execute {}: {e}", binary.display());
            return Exit::Failed;
        }
    };
    match status.code() {
        Some(code) => Exit::Program(code as u8),
        // Killed by a signal — `hero_panic`'s `abort()` is the one that matters, and
        // the shell's own convention (128 + signal) is what a harness expects.
        None => Exit::Program(134),
    }
}
