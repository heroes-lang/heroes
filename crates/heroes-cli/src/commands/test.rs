//! `heroes test <file.hero>` — run the file's `test` blocks (§4.18).
//!
//! **A subcommand, not a flag**, and CLAUDE.md §10's stopping rule says why
//! mechanically: it answers a different question about the same input, and its
//! artifact class is different — `build` produces a binary, `run` produces a
//! process, `test` produces a verdict per test. The end-to-end verification list
//! in `docs/ROADMAP.md` has written `heroes test <the calculator>` since
//! M-day-zero.
//!
//! **One process per test**, which is the whole reason this file is not a loop
//! inside the generated `main`. An `assert` failure is a panic (§4.18), so a
//! runner that called the tests in sequence would stop at the first failure and
//! hide every test after it — the one thing a test runner must not do. So the
//! program is compiled once and executed once per test, with the index as the
//! only argument.
//!
//! The report goes to **stdout** because it is the artifact (CLAUDE.md §10);
//! what the failing test itself printed goes to stderr, where the program put it.

use crate::cli::{Exit, Invocation};
use heroes::emit::Target;

use super::compile::{compile_with_tests, Options};
use super::contract::level_from;

pub fn run(path: &str, args: &Invocation) -> Exit {
    let level = match level_from(args, "-O0") {
        Ok(level) => level,
        Err(exit) => return exit,
    };
    let options = Options {
        level,
        dump_ir: false,
        emit_c: false,
        output: None,
        sanitize: args.has("--sanitize"),
        target: Target::Tests,
    };
    let (binary, titles) = match compile_with_tests(path, &options) {
        Err(exit) => return exit,
        Ok(None) => return Exit::Ok,
        Ok(Some(pair)) => pair,
    };
    if titles.is_empty() {
        println!("no tests in {path}");
        return Exit::Ok;
    }
    let mut failed = 0;
    for (index, title) in titles.iter().enumerate() {
        let status = std::process::Command::new(&binary).arg(index.to_string()).status();
        let ok = match status {
            Ok(status) => status.success(),
            Err(e) => {
                eprintln!("error: cannot execute {}: {e}", binary.display());
                return Exit::Failed;
            }
        };
        if ok {
            println!("ok   {title}");
        } else {
            println!("FAIL {title}");
            failed += 1;
        }
    }
    let total = titles.len();
    let unit = if total == 1 { "test" } else { "tests" };
    if failed == 0 {
        println!("{total} {unit}, all passed");
        return Exit::Ok;
    }
    println!("{total} {unit}, {failed} failed");
    // A failing test is the *program* being wrong, which is exit 1 — the same
    // code a diagnostic gets, and for the same reason: the tool worked.
    Exit::Diagnostics
}
