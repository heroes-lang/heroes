//! Every command, as data (panel 016) — the table that both parses argv and
//! prints `--help`, so the two cannot disagree.
//!
//! Split out of `cli.rs` at M5a, when the file passed CLAUDE.md §11's ~300 lines.
//! The split is along the seam the design already had: this file is *what the
//! surface is*, `cli.rs` is *how a line is read against it*, and `help.rs` is how
//! it is shown. A new command touches exactly one of the three.
//!
//! The order is the pipeline's — stage by stage, then the tools — because that is
//! the order someone learning the compiler meets them in.

use super::{Command, Flag, Operand, Tag};

fn flag(spelling: &str, what: &str) -> Flag {
    Flag { spelling: spelling.to_string(), what: what.to_string(), value: false }
}

/// A flag that takes the next argument. `-o` is the first one the surface has, and
/// it is here because the fixpoint invocation cannot be written without it: `build
/// … -o A`, `--emit-c -o B.c`, `--emit-c -o C.c`, `diff B.c C.c`. The C leg would
/// compose from a shell redirect; the *binary* leg cannot be redirected at all.
fn valued(spelling: &str, what: &str) -> Flag {
    Flag { spelling: spelling.to_string(), what: what.to_string(), value: true }
}

pub fn commands() -> Vec<Command> {
    vec![
        Command {
            tag: Tag::Lex,
            name: "lex".to_string(),
            operand: Operand::File,
            flags: vec![
                flag("--dump-tokens", "print the token stream"),
                flag("--json", "print it as JSON instead of text"),
            ],
            summary: "check the token stream".to_string(),
        },
        Command {
            tag: Tag::Parse,
            name: "parse".to_string(),
            operand: Operand::File,
            flags: vec![flag("--dump-ast", "print the syntax tree")],
            summary: "check the syntax".to_string(),
        },
        Command {
            tag: Tag::Check,
            name: "check".to_string(),
            operand: Operand::File,
            flags: vec![
                flag("--dump-scopes", "print the symbols and every binding"),
                flag("--json", "print the diagnostics as JSON (schema 1) instead of text"),
                flag("--brief", "one line per diagnostic instead of the full form"),
                flag(
                    "--permissive",
                    "drop the diagnostics that exist for the thesis — the control arm of Part 11's measurement",
                ),
                flag("--apply", "print the program with every certain fix applied"),
                flag("--in-place", "with --apply, rewrite the file instead of printing it"),
            ],
            summary: "check names and types".to_string(),
        },
        Command {
            tag: Tag::Build,
            name: "build".to_string(),
            operand: Operand::File,
            flags: vec![
                flag("--dump-ir", "print the lowered three-address IR and stop"),
                flag("--emit-c", "print the generated C11 and stop, instead of compiling it"),
                valued("-o", "write the artifact here instead of under build/"),
                flag("--sanitize", "compile with -fsanitize=address,undefined"),
            ],
            summary: "compile to a native binary".to_string(),
        },
        Command {
            tag: Tag::Run,
            name: "run".to_string(),
            operand: Operand::File,
            flags: vec![
                valued("-o", "keep the binary here as well as running it"),
                flag("--sanitize", "compile with -fsanitize=address,undefined"),
            ],
            summary: "compile at -O2 and execute (the dev loop)".to_string(),
        },
        Command {
            tag: Tag::Fmt,
            name: "fmt".to_string(),
            operand: Operand::File,
            flags: vec![flag(
                "--in-place",
                "rewrite the file instead of printing it; prints nothing, and refuses a file it cannot parse",
            )],
            summary: "print the canonical form".to_string(),
        },
        Command {
            tag: Tag::Doctor,
            name: "doctor".to_string(),
            operand: Operand::None,
            flags: Vec::new(),
            summary: "check the toolchain (clang, CLT, arch, runtime, cache)".to_string(),
        },
        Command {
            tag: Tag::Mutate,
            name: "mutate".to_string(),
            operand: Operand::Optional,
            flags: Vec::new(),
            summary:
                "metric 3: make one plausible mistake per site and count what the compiler catches (default: examples/)"
                    .to_string(),
        },
        Command {
            tag: Tag::Measure,
            name: "measure".to_string(),
            operand: Operand::Optional,
            flags: Vec::new(),
            summary: "count a spec file against its ceiling (default: spec/heroes-spec.md)"
                .to_string(),
        },
    ]
}

/// A retired spelling names its replacement — the same treatment the *language*
/// gives a foreign keyword (`fn` → `function`).
///
/// Empty since M5a: `run` was the only entry, and it has stopped being retired and
/// become the dev loop. The function stays because the *next* rename needs somewhere
/// to live, and because its emptiness is the record of that one having completed.
pub fn retired(word: &str) -> Option<&'static str> {
    let _ = word;
    None
}

/// `--write` shipped from M2 to M3; panel 016 renamed it because "write" is read as
/// "write the output somewhere", i.e. the *non*-destructive meaning, which makes the
/// plausible misreading the one that overwrites a file.
///
/// `--no-line` is here rather than in the table: panel 020 refused to give it a
/// place on the surface (CLAUDE.md §10's stopping rule — not the fixpoint
/// invocation, not typed by either harness, no measured Part 11 effect), and a flag
/// somebody plausibly reaches for should say so rather than print the whole list.
pub fn retired_flag(command: &str, arg: &str) -> Option<&'static str> {
    match (command, arg) {
        ("fmt", "--write") | ("fmt", "-w") => Some("--in-place"),
        ("lex", "--tokens") => Some("--dump-tokens"),
        ("build", "--no-line") => {
            Some("--emit-c` alone (the `#line` directives are the artifact; the emitter's own tests are what read C without them")
        }
        _ => None,
    }
}
