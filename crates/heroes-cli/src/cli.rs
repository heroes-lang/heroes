//! The command surface, as data (panel 016).
//!
//! One table describes every command: its name, whether it takes a file, which
//! flags it accepts, and one line of help. The table is what parses argv **and**
//! what prints `--help`, so the two cannot disagree — ripgrep's design, and the
//! opposite of docopt's (which generated the parser *from* the help text and has
//! not shipped a release since 2014).
//!
//! Three properties this file exists to guarantee, each of which was a guess
//! before it:
//!
//! 1. **Strict.** An unknown flag, a flag on the wrong command, a second
//!    operand — each is an error that *names what is accepted*. The
//!    llm-ergonomist made this a condition: a lenient shared parser would turn
//!    `check --json` (before it existed) into prose fed to a JSON reader, and a
//!    silent wrong answer is the one failure this project spends tokens to avoid.
//! 2. **One exit-code contract**, the same for every command: **0** clean · **1**
//!    the input has diagnostics · **2** the tool could not do its job. That is
//!    POSIX's own shape (`grep`, `diff`) and javac has shipped it since JDK 1.x.
//! 3. **`Tag`, not a name string.** Dispatch is an exhaustive `match` on this
//!    enum, so a table entry with no dispatch arm is a *compile* error rather
//!    than a runtime surprise — the compiler-engineer's condition, and one this
//!    compiler should hold itself to before it asks a language to.
//!
//! The Cyclone rule shapes the types here as much as anywhere: no `&'static
//! [Flag]` in a struct, so the table is built by a function returning owned data.

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
    Fmt,
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
}

pub struct Command {
    pub tag: Tag,
    pub name: String,
    pub operand: Operand,
    pub flags: Vec<Flag>,
    pub summary: String,
}

/// The exit-code contract, in one place. Printed in `--help`, asserted by the
/// golden harness, and the reason a wrapper can tell "your program is wrong"
/// from "I could not run".
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Exit {
    /// Nothing to report.
    Ok,
    /// The input has diagnostics. The program is wrong, the tool worked.
    Diagnostics,
    /// The tool could not do its job: an unreadable file, a bad flag, no clang.
    Failed,
}

impl Exit {
    pub fn code(self) -> std::process::ExitCode {
        match self {
            Exit::Ok => std::process::ExitCode::from(0),
            Exit::Diagnostics => std::process::ExitCode::from(1),
            Exit::Failed => std::process::ExitCode::from(2),
        }
    }
}

fn flag(spelling: &str, what: &str) -> Flag {
    Flag { spelling: spelling.to_string(), what: what.to_string() }
}

/// Every command, in the order `--help` prints them: the pipeline in stage
/// order, then the tools.
///
/// The `--dump-<stage>` spellings are design.md §3.5's own ("Inspection is flags
/// … **not subcommands**"), which the code had drifted from — `lex` used to print
/// its tokens by default and take `--json` as the only flag, so "which stages
/// print without being asked" was a table you had to memorise.
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
            flags: vec![flag("--dump-ir", "print the lowered three-address IR")],
            summary: "lower to the IR (code generation lands at M5a)".to_string(),
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
            summary: "check the toolchain (clang, CLT, arch, cache)".to_string(),
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

/// What a command line turned out to mean.
pub struct Invocation {
    pub tag: Tag,
    pub file: Option<String>,
    /// The flags that were present, in the order the table lists them.
    flags: Vec<String>,
}

impl Invocation {
    pub fn has(&self, spelling: &str) -> bool {
        self.flags.iter().any(|f| f == spelling)
    }
}

/// Parse argv against the table. The error is a message ready to print: it names
/// what was wrong *and* what would have been accepted, because a model reading
/// "unknown flag" learns nothing.
pub fn parse(args: &[String]) -> Result<Invocation, String> {
    let Some(word) = args.first() else {
        return Ok(Invocation { tag: Tag::Help, file: None, flags: Vec::new() });
    };
    match word.as_str() {
        "--version" | "-V" => {
            return Ok(Invocation { tag: Tag::Version, file: None, flags: Vec::new() })
        }
        "--help" | "-h" | "help" => {
            return Ok(Invocation { tag: Tag::Help, file: None, flags: Vec::new() })
        }
        _ => {}
    }
    let table = commands();
    let Some(command) = table.iter().find(|c| c.name == *word) else {
        // A retired spelling names its replacement, which is the same treatment
        // the *language* gives a foreign keyword (`fn` → `function`).
        if let Some(replacement) = retired(word) {
            return Err(format!("`{word}` is no longer a command — use `{replacement}`"));
        }
        let names: Vec<String> = table.iter().map(|c| c.name.clone()).collect();
        return Err(format!(
            "unknown command `{word}` — the commands are {}",
            names.join(", ")
        ));
    };
    let mut file: Option<String> = None;
    let mut flags: Vec<String> = Vec::new();
    for arg in &args[1..] {
        if arg.starts_with('-') {
            if let Some(known) = command.flags.iter().find(|f| f.spelling == *arg) {
                if !flags.contains(&known.spelling) {
                    flags.push(known.spelling.clone());
                }
                continue;
            }
            if let Some(replacement) = retired_flag(&command.name, arg) {
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
        return Err(format!("`{}` needs a file: `heroes {} <file.hero>`", command.name, command.name));
    }
    Ok(Invocation { tag: command.tag, file, flags })
}

/// The enumerating error the llm-ergonomist asked for: every wrong flag is one
/// round trip, because the message carries the list.
fn unknown_flag(command: &Command, arg: &str) -> String {
    if command.flags.is_empty() {
        return format!("`{}` takes no flags, and `{arg}` was given", command.name);
    }
    let accepted: Vec<String> =
        command.flags.iter().map(|f| f.spelling.clone()).collect();
    format!(
        "`{}` does not accept `{arg}` — it accepts {}",
        command.name,
        accepted.join(", ")
    )
}

fn retired(word: &str) -> Option<&'static str> {
    match word {
        // `build` arrived at M4 — earlier than this table said, because the ROADMAP
        // gives it `--dump-ir` there and the fixpoint invocation types the verb.
        "run" => Some("build` for now (`run` lands at M5a"),
        _ => None,
    }
}

/// `--write` shipped from M2 to M3; panel 016 renamed it because "write" is read
/// as "write the output somewhere", i.e. the *non*-destructive meaning, which
/// makes the plausible misreading the one that overwrites a file.
fn retired_flag(command: &str, arg: &str) -> Option<&'static str> {
    match (command, arg) {
        ("fmt", "--write") | ("fmt", "-w") => Some("--in-place"),
        ("lex", "--tokens") => Some("--dump-tokens"),
        _ => None,
    }
}

/// `heroes --help`: the table, plus the contract. The exit codes are here
/// because a wrapper script has nowhere else to read them.
pub fn help() -> String {
    let mut out = String::from("heroes — the Heroes compiler\n\nusage: heroes <command> [file] [flags]\n\n");
    for command in commands() {
        let operand = match command.operand {
            Operand::None => "",
            Operand::File => " <file.hero>",
            Operand::Optional => " [file]",
        };
        out.push_str(&format!("  {}{}\n      {}\n", command.name, operand, command.summary));
        for flag in &command.flags {
            out.push_str(&format!("      {}  {}\n", flag.spelling, flag.what));
        }
    }
    out.push_str(
        "\n  --version · --help\n\nstreams: the artifact on stdout, diagnostics and progress on stderr.\nexit:    0 nothing to report · 1 the input has diagnostics · 2 the tool could not run.\n\nMore commands arrive with each milestone: run (M5a) · test (M6)\nlsp outline explain (M6+) · cc doc (M7)\n",
    );
    out
}
