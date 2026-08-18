//! Every command, as data (panel 016) — the table that both parses argv and
//! prints `--help`, so the two cannot disagree.
//!
//! Split out of `cli.rs` at M-scalars-run, when the file passed CLAUDE.md §11's ~300 lines.
//! The split is along the seam the design already had: this file is *what the
//! surface is*, `cli.rs` is *how a line is read against it*, and `help.rs` is how
//! it is shown. A new command touches exactly one of the three.
//!
//! The order is the pipeline's — stage by stage, then the tools — because that is
//! the order someone learning the compiler meets them in.

use super::{Command, Flag, Operand, Tag};

fn flag(spelling: &str, what: &str) -> Flag {
    Flag { spelling: spelling.to_string(), what: what.to_string(), value: false, repeatable: false, directory: false }
}

/// A flag that takes the next argument. `-o` is the first one the surface has, and
/// it is here because the fixpoint invocation cannot be written without it: `build
/// … -o A`, `--emit-c -o B.c`, `--emit-c -o C.c`, `diff B.c C.c`. The C leg would
/// compose from a shell redirect; the *binary* leg cannot be redirected at all.
fn valued(spelling: &str, what: &str) -> Flag {
    Flag { spelling: spelling.to_string(), what: what.to_string(), value: true, repeatable: false, directory: false }
}

/// A valued flag that may be written more than once, each time meaning one more.
///
/// **Only search paths are like this, and the decision is the author's**
/// (2026-08-14, settling the split panel 055 recorded). Three judges held that
/// §10's stopping rule yields *nothing*, because `CPATH`, `LIBRARY_PATH` and
/// `PKG_CONFIG_PATH` already compose to it — measured, all three live, because
/// `Command` inherits the environment. The judge who built it held that an
/// environment variable is **not an artifact a program can carry**: asked for a
/// program that binds SDL2, a model produces something that cannot be built, and
/// the missing half is an unversioned shell line.
///
/// What the author's yes buys is that half. **What it cost was written here as a
/// premise about the world, and panel 056 measured the premise false** — CLAUDE.md
/// §11's class exactly, where the argument stays valid, only the premise dies, and
/// the paragraph goes on reading as correct. It said `-I` and `-L` named
/// *independently* were the only route to panel 055's skew. They are not.
///
/// **One header path is enough, because the library side is never empty.** With
/// this machine's SDK SQLite on the default path, `--include` alone at Homebrew's
/// keg-only prefix builds a program whose header answers `3053004` and whose
/// library answers `3.51.0` — clean, exit 0. `CPATH` reaches the same state, and
/// that is the channel `machine_locked_path`'s own repair note recommends, so the
/// hazard is older than these flags and is not created by them.
///
/// And the outcome is not a wrong number. Two builds of one library, same symbol
/// and same signature, skewed: **exit 134 with no output**, and under `--sanitize`
/// a heap-buffer-overflow writing 64 bytes into a 16-byte region (design.md §1.12).
/// `package` stays the shape to reach for first — it asks one question of one `.pc`
/// and cannot skew — and these are for the library that has no `.pc`.
///
/// **This constructor was called `repeated`, and the name was the second defect**
/// (panel 056's findings; author decision 2026-08-15). A search path is repeatable
/// *because* a search list has more than one entry, and its value must name a
/// **directory** *because* that is what a search path is — two properties of one
/// thing. Naming the constructor after the first made the second invisible, and
/// nothing checked it: `clang -Wall -I/no/such/dir` says **nothing at all**, so a
/// committed invocation pinning one machine's prefix builds clean on another
/// **against a different header than the declarations were checked against** —
/// §4.19's oracle swapped rather than weakened, and the half of panel 055's skew
/// that survives being committed.
fn search_path(spelling: &str, what: &str) -> Flag {
    Flag {
        spelling: spelling.to_string(),
        what: what.to_string(),
        value: true,
        repeatable: true,
        directory: true,
    }
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
                search_path("--include", "search this directory for an `extern` group's header; may repeat"),
                search_path("--library", "search this directory for a `link` library; may repeat"),
                flag("--sanitize", "compile with -fsanitize=address,undefined"),
                flag("-O0", "compile at -O0 (the default here)"),
                flag("-O2", "compile at -O2"),
            ],
            summary: "compile to a native binary".to_string(),
        },
        Command {
            tag: Tag::Run,
            name: "run".to_string(),
            operand: Operand::File,
            flags: vec![
                valued("-o", "keep the binary here as well as running it"),
                search_path("--include", "search this directory for an `extern` group's header; may repeat"),
                search_path("--library", "search this directory for a `link` library; may repeat"),
                flag("--sanitize", "compile with -fsanitize=address,undefined"),
                flag("-O0", "compile at -O0"),
                flag("-O2", "compile at -O2 (the default here)"),
            ],
            summary: "compile and execute (the dev loop, -O2); `-- a b` passes a and b to the program".to_string(),
        },
        Command {
            tag: Tag::Test,
            name: "test".to_string(),
            operand: Operand::File,
            flags: vec![
                search_path("--include", "search this directory for an `extern` group's header; may repeat"),
                search_path("--library", "search this directory for a `link` library; may repeat"),
                flag("--sanitize", "compile with -fsanitize=address,undefined"),
                flag("-O0", "compile at -O0 (the default here)"),
                flag("-O2", "compile at -O2"),
            ],
            summary: "run the file's `test` blocks".to_string(),
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
            flags: vec![
                flag("--survivors", "print the mutants the compiler accepted, not only the rates"),
                valued("--operator", "score this operator alone, by its id"),
            ],
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
        Command {
            tag: Tag::This,
            name: "this".to_string(),
            operand: Operand::None,
            flags: Vec::new(),
            summary: "print The Zen of Heroes".to_string(),
        },
    ]
}

/// A retired spelling names its replacement — the same treatment the *language*
/// gives a foreign keyword (`fn` → `function`).
///
/// Empty since M-scalars-run: `run` was the only entry, and it has stopped being retired and
/// become the dev loop. The function stays because the *next* rename needs somewhere
/// to live, and because its emptiness is the record of that one having completed.
pub fn retired(word: &str) -> Option<&'static str> {
    let _ = word;
    None
}

/// `--write` shipped from M-syntax-tree to M-typed-frontend; panel 016 renamed it because "write" is read as
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
