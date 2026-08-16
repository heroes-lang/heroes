//! The command surface: how one argv line is read against the table (panel 016).
//!
//! The table itself is `cli/table.rs` and the help text is `cli/help.rs` — one
//! table describes every command, and it is what parses argv **and** what prints
//! `--help`, so the two cannot disagree. That is ripgrep's design, and the opposite
//! of docopt's, which generated the parser *from* the help text and has not shipped
//! a release since 2014.
//!
//! Three properties this file exists to guarantee, each of which was a guess before
//! it:
//!
//! 1. **Strict.** An unknown flag, a flag on the wrong command, a second operand —
//!    each is an error that *names what is accepted*. The llm-ergonomist made this a
//!    condition: a lenient shared parser would turn `check --json` (before it
//!    existed) into prose fed to a JSON reader, and a silent wrong answer is the one
//!    failure this project spends tokens to avoid.
//! 2. **One exit-code contract**, the same for every command: **0** clean · **1**
//!    diagnostics were reported and no artifact was produced · **2** the tool could
//!    not do its job. That is POSIX's own shape (`grep`, `diff`) and javac has
//!    shipped it since JDK 1.x. The gloss on 1 widened at M-scalars-run: an unsupported form
//!    is a diagnostic about the *compiler*, and it exits 1 because 2 sends an agent
//!    to reinstall its toolchain (panel 020, measured).
//! 3. **`Tag`, not a name string.** Dispatch is an exhaustive `match` on this enum,
//!    so a table entry with no dispatch arm is a *compile* error rather than a
//!    runtime surprise — the compiler-engineer's condition, and one this compiler
//!    should hold itself to before it asks a language to.
//!
//! The Cyclone rule shapes the types here as much as anywhere: no `&'static [Flag]`
//! in a struct, so the table is built by a function returning owned data.

mod help;
mod table;

pub use help::help;
pub use table::commands;

/// Which command was asked for. Exhaustive dispatch lives in `main.rs`.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Tag {
    Doctor,
    Lex,
    Measure,
    Mutate,
    Parse,
    Check,
    Build,
    Run,
    Test,
    Fmt,
    This,
    Version,
    Help,
}

/// What a command does with a positional argument.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Operand {
    None,
    /// A `.hero` file, required.
    File,
    /// A file with a documented default (`measure`).
    Optional,
}

pub struct Flag {
    pub spelling: String,
    pub what: String,
    /// Whether the next argument belongs to this flag (`-o path`).
    pub value: bool,
    /// Whether writing it twice means two things rather than one.
    ///
    /// **A search path is a list, and the default was to drop the second
    /// silently** (panel 055): the parse below kept the first occurrence and
    /// discarded the rest without a word, which for `-o` is right — a second
    /// output path is a mistake and the first is what the reader meant — and for
    /// `--include A --include B` is a **wrong build** rather than a failed one,
    /// which is what design.md §1.12 forbids.
    pub repeatable: bool,
    /// Whether the value must name a directory that exists.
    ///
    /// **The one place this parser looks at the machine**, and it is deliberate.
    /// Everything else here is a fact about argv, but a search path that names
    /// nothing is the failure clang is *silent* about (panel 056's findings), and
    /// silence is what makes it survive being committed to a script or a CI file.
    /// It is checked here rather than nearer clang because a missing directory is
    /// **a bad flag** — §10's exit 2, *the tool could not run* — and not a
    /// diagnostic about the program, which keeps §7's exit-1 narrowing intact: every
    /// exit-1 class recovers a name *this program declared* `extern`, and a
    /// directory is not a declaration.
    pub directory: bool,
}

pub struct Command {
    pub tag: Tag,
    pub name: String,
    pub operand: Operand,
    pub flags: Vec<Flag>,
    pub summary: String,
}

/// The exit-code contract, in one place. Printed in `--help`, asserted by the
/// golden harness, and the reason a wrapper can tell "your program is wrong" from
/// "I could not run".
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Exit {
    /// Nothing to report.
    Ok,
    /// Diagnostics were reported and no artifact was produced. The tool worked.
    Diagnostics,
    /// The tool could not do its job: an unreadable file, a bad flag, no clang.
    Failed,
    /// `run` only: the program itself exited with this status. The program is the
    /// artifact, so its verdict is the command's.
    Program(u8),
}

impl Exit {
    pub fn code(self) -> std::process::ExitCode {
        match self {
            Exit::Ok => std::process::ExitCode::from(0),
            Exit::Diagnostics => std::process::ExitCode::from(1),
            Exit::Failed => std::process::ExitCode::from(2),
            Exit::Program(status) => std::process::ExitCode::from(status),
        }
    }
}

/// What a command line turned out to mean.
pub struct Invocation {
    pub tag: Tag,
    pub file: Option<String>,
    /// Everything after `--`, handed to the program verbatim (`run` only).
    ///
    /// **A separator rather than a trailing operand**, because the alternative is
    /// unreadable in exactly the case that matters: `heroes run f.hero --sanitize`
    /// would have to mean the flag, and `heroes run f.hero -o x` the flag's value,
    /// so a program taking `-o` could never be run. `--` is what every tool that
    /// hit this settled on, and it is the only spelling with no ambiguity left in
    /// it (author instruction 2026-08-12).
    pub program_args: Vec<String>,
    /// The flags that were present, in the order the table lists them, each with
    /// its value if it takes one.
    flags: Vec<(String, Option<String>)>,
}

impl Invocation {
    pub fn has(&self, spelling: &str) -> bool {
        self.flags.iter().any(|(f, _)| f == spelling)
    }

    pub fn value_of(&self, spelling: &str) -> Option<String> {
        self.flags.iter().find(|(f, _)| f == spelling).and_then(|(_, v)| v.clone())
    }

    /// Every value a repeatable flag was given, in the order written — which is
    /// the order clang will search, so it is the order the author chose.
    pub fn values_of(&self, spelling: &str) -> Vec<String> {
        self.flags
            .iter()
            .filter(|(f, _)| f == spelling)
            .filter_map(|(_, v)| v.clone())
            .collect()
    }
}

/// Parse argv against the table. The error is a message ready to print: it names
/// what was wrong *and* what would have been accepted, because a model reading
/// "unknown flag" learns nothing.
pub fn parse(args: &[String]) -> Result<Invocation, String> {
    let Some(word) = args.first() else {
        return Ok(Invocation { tag: Tag::Help, file: None, flags: Vec::new(), program_args: Vec::new() });
    };
    match word.as_str() {
        "--version" | "-V" => {
            return Ok(Invocation { tag: Tag::Version, file: None, flags: Vec::new(), program_args: Vec::new() })
        }
        "--help" | "-h" | "help" => {
            return Ok(Invocation { tag: Tag::Help, file: None, flags: Vec::new(), program_args: Vec::new() })
        }
        _ => {}
    }
    let commands = commands();
    let Some(command) = commands.iter().find(|c| c.name == *word) else {
        if let Some(replacement) = table::retired(word) {
            return Err(format!("`{word}` is no longer a command — use `{replacement}`"));
        }
        let names: Vec<String> = commands.iter().map(|c| c.name.clone()).collect();
        return Err(format!("unknown command `{word}` — the commands are {}", names.join(", ")));
    };
    let mut file: Option<String> = None;
    let mut flags: Vec<(String, Option<String>)> = Vec::new();
    let mut program_args: Vec<String> = Vec::new();
    let mut rest = args[1..].iter();
    while let Some(arg) = rest.next() {
        if arg == "--" {
            // Only `run` executes anything, so only `run` has a program to
            // forward to. Anywhere else the separator is a mistake worth naming
            // rather than a no-op that silently drops what follows it.
            if command.tag != Tag::Run {
                return Err(format!(
                    "`--` passes arguments to the program, and `{}` does not run one — only `heroes run` does",
                    command.name
                ));
            }
            program_args.extend(rest.cloned());
            break;
        }
        if arg.starts_with('-') {
            if let Some(known) = command.flags.iter().find(|f| f.spelling == *arg) {
                let value = if known.value {
                    match rest.next() {
                        Some(next) if !next.starts_with('-') => Some(next.clone()),
                        _ => {
                            return Err(format!(
                                "`{arg}` needs a path: `heroes {} <file.hero> {arg} <path>`",
                                command.name
                            ))
                        }
                    }
                } else {
                    None
                };
                if let (true, Some(path)) = (known.directory, value.as_deref()) {
                    if !std::path::Path::new(path).is_dir() {
                        return Err(format!(
                            "`{arg} {path}` names no directory on this machine — clang searches it \
                             in silence and builds against whatever header it finds elsewhere, so \
                             this is refused here rather than discovered as a wrong answer"
                        ));
                    }
                }
                if known.repeatable || !flags.iter().any(|(f, _)| *f == known.spelling) {
                    flags.push((known.spelling.clone(), value));
                }
                continue;
            }
            if let Some(replacement) = table::retired_flag(&command.name, arg) {
                return Err(format!(
                    "`{arg}` is no longer a flag of `{}` — use `{replacement}`",
                    command.name
                ));
            }
            return Err(unknown_flag(command, arg));
        }
        if command.operand == Operand::None {
            return Err(format!("`{}` takes no file", command.name));
        }
        if file.is_some() {
            return Err(format!("`{}` takes one file, and two were given", command.name));
        }
        file = Some(arg.clone());
    }
    if command.operand == Operand::File && file.is_none() {
        return Err(format!(
            "`{}` needs a file: `heroes {} <file.hero>`",
            command.name, command.name
        ));
    }
    Ok(Invocation { tag: command.tag, file, flags, program_args })
}

/// The enumerating error the llm-ergonomist asked for: every wrong flag is one
/// round trip, because the message carries the list.
fn unknown_flag(command: &Command, arg: &str) -> String {
    if command.flags.is_empty() {
        return format!("`{}` takes no flags, and `{arg}` was given", command.name);
    }
    let accepted: Vec<String> = command.flags.iter().map(|f| f.spelling.clone()).collect();
    format!("`{}` does not accept `{arg}` — it accepts {}", command.name, accepted.join(", "))
}
