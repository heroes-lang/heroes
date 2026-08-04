//! `heroes parse <file.hero> [--dump-ast]` — read the file and say what the
//! parser understood (M2).
//!
//! Without the flag it is a syntax check: silent on success, diagnostics on
//! stderr and a failing exit code otherwise. With `--dump-ast` it prints the
//! tree, but only when the file is clean — a tree built out of recovery
//! guesses would read like a fact and is not one.

use std::process::ExitCode;

use heroes::printer::dump_ast;
use heroes::source::Source;
use heroes::syntax::parse;

const USAGE: &str = "usage: heroes parse <file.hero> [--dump-ast]";

pub fn run(args: &[String]) -> ExitCode {
    let mut file: Option<&String> = None;
    let mut dump = false;
    for arg in args {
        match arg.as_str() {
            "--dump-ast" => dump = true,
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

    for diagnostic in &out.diagnostics {
        eprintln!("{}", diagnostic.render_line(&src));
    }
    if !out.diagnostics.is_empty() {
        return ExitCode::FAILURE;
    }
    if dump {
        print!("{}", dump_ast(&out.ast, &src));
    }
    ExitCode::SUCCESS
}
