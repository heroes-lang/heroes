//! `heroes fmt <file.hero> [--write]` — the canonical form (M2).
//!
//! Prints to stdout by default, like `gofmt`; `--write` replaces the file.
//! Either way it **refuses a file with diagnostics**: formatting a tree built
//! out of recovery guesses would rewrite the author's program into the
//! parser's guess about it, which is the one thing a formatter must never do.

use std::process::ExitCode;

use heroes::printer::format_file;
use heroes::source::Source;
use heroes::syntax::parse;

const USAGE: &str = "usage: heroes fmt <file.hero> [--write]";

pub fn run(args: &[String]) -> ExitCode {
    let mut file: Option<&String> = None;
    let mut write = false;
    for arg in args {
        match arg.as_str() {
            "--write" => write = true,
            _ if !arg.starts_with('-') && file.is_none() => file = Some(arg),
            _ => {
                eprintln!("error: unexpected argument `{arg}`\n{USAGE}");
                return ExitCode::FAILURE;
            }
        }
    }
    let Some(path) = file else {
        eprintln!("{USAGE}");
        return ExitCode::FAILURE;
    };
    let text = match std::fs::read_to_string(path) {
        Ok(text) => text,
        Err(e) => {
            eprintln!("error: cannot read `{path}`: {e}");
            return ExitCode::FAILURE;
        }
    };
    let src = Source::new(path.clone(), text);
    let out = parse(&src);
    if !out.diagnostics.is_empty() {
        for diagnostic in &out.diagnostics {
            eprintln!("{}", diagnostic.render_line(&src));
        }
        eprintln!("error: `{path}` is not formatted because it does not parse");
        return ExitCode::FAILURE;
    }
    let formatted = format_file(&out.ast, &out.comments, &src);
    if !write {
        print!("{formatted}");
        return ExitCode::SUCCESS;
    }
    if formatted == src.text {
        return ExitCode::SUCCESS; // nothing to do, and no needless mtime churn
    }
    match std::fs::write(path, &formatted) {
        Ok(()) => ExitCode::SUCCESS,
        Err(e) => {
            eprintln!("error: cannot write `{path}`: {e}");
            ExitCode::FAILURE
        }
    }
}
