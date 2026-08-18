//! `heroes run <file.hero> [-o <path>]` — compile at `-O2` and execute (M-scalars-run).
//!
//! The dev loop, and the verb CLAUDE.md §6 sanctions by name (`nim r` → `heroes
//! run`). It is a **subcommand** rather than a flag because it answers a different
//! question with a different artifact class: `build` produces a file, `run` produces
//! a process. Panel 020 struck the warrant originally offered for it — the M-selfhost-fixpoint
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

use heroes::emit::Target;

use super::compile::{compile, Options};
use super::contract::level_from;

pub fn run(path: &str, args: &Invocation) -> Exit {
    let level = match level_from(args, "-O2") {
        Ok(level) => level,
        Err(exit) => return exit,
    };
    let options = Options {
        level,
        dump_ir: false,
        emit_c: false,
        output: args.value_of("-o"),
        sanitize: args.has("--sanitize"),
        search: super::compile::Search {
            include: args.values_of("--include"),
            library: args.values_of("--library"),
        },
        target: Target::Program,
    };
    let binary = match compile(path, &options) {
        Err(exit) => return exit,
        Ok(None) => return Exit::Ok,
        Ok(Some(binary)) => binary,
    };
    // No line of our own before the program's output: stdout is the program's from
    // here, and a progress line on stderr would still interleave in a terminal.
    let status = match std::process::Command::new(&binary).args(&args.program_args).status() {
        Ok(status) => status,
        Err(e) => {
            eprintln!("error: cannot execute {}: {e}", binary.display());
            return Exit::Failed;
        }
    };
    match status.code() {
        Some(code) => Exit::Program(code as u8),
        // **Killed by a signal: `128 + signal`, which is what the comment here has
        // claimed since it was written and what the code did not do** (defect, panel
        // 070, found by the compiler-engineer while measuring something else).
        //
        // It returned a hardcoded `134` for *every* signal. 134 is `128 + SIGABRT`,
        // so a `hero_panic` reported correctly and a **SIGSEGV reported as an
        // abort** — 134 where the shell says 139. That is not a cosmetic difference:
        // an abort is this runtime working (§1.12's loud direction) and a segfault is
        // this runtime failing, and for the length of that constant `heroes run`
        // could not tell them apart.
        //
        // It cost exactly what a wrong instrument costs. Panel 070's brief carried a
        // table showing `heroes run -O0` at 134 and the same binary at 139, and the
        // convener read the pair as "aborts cleanly through `run`" — so the sitting
        // was briefed that the crash depended on the optimisation level, which the
        // compiler-engineer then refuted with measurements. The instrument hid the
        // defect from the person holding it.
        //
        // `Exit::Program` is a `u8` and `128 + signal` fits: signals are 1..=64 on
        // every platform this compiler targets, so the sum is 129..=192.
        None => Exit::Program(signal_exit(&status)),
    }
}

/// `128 + signal`, the convention every shell prints and every harness reads.
///
/// **Unix only, and the `cfg` is the honest half.** `ExitStatus::signal()` is a
/// `std::os::unix` extension; Windows has no signals, and a process that dies
/// there without an exit code did so some other way — `134` stays as the fallback
/// there because that is what this function returned everywhere before, and
/// changing Windows' answer is a claim this panel measured nothing about.
fn signal_exit(status: &std::process::ExitStatus) -> u8 {
    #[cfg(unix)]
    {
        use std::os::unix::process::ExitStatusExt;
        if let Some(signal) = status.signal() {
            // Saturating rather than wrapping: a signal number above 127 would
            // otherwise wrap into the range a *program's* exit code occupies, which
            // is the one confusion this repair exists to remove.
            return 128u8.saturating_add(u8::try_from(signal).unwrap_or(u8::MAX - 128));
        }
    }
    #[cfg(not(unix))]
    let _ = status;
    134
}
