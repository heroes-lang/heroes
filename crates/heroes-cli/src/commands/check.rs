//! `heroes check <file.hero> [--dump-scopes]` — read the file and say what is
//! wrong with it (M3a: names; M3b–M3d: types).
//!
//! This is the command the golden `check/` cases run through, and it is where
//! every frontend stage's diagnostics come out from now on. The stages are
//! ordered, not interleaved: **the resolver runs only if lexing and parsing
//! produced nothing.** After a parse error the tree is made of recovery
//! guesses, and a name error about a line the author did not write costs more
//! than it buys — the same rule that keeps `--dump-ast` quiet on a broken file.

use std::process::ExitCode;

use heroes::printer::dump_scopes;
use heroes::resolve::resolve;
use heroes::source::Source;
use heroes::syntax::parse;

const USAGE: &str = "usage: heroes check <file.hero> [--dump-scopes]";

pub fn run(args: &[String]) -> ExitCode {
    let mut file: Option<&String> = None;
    let mut dump = false;
    for arg in args {
        match arg.as_str() {
            "--dump-scopes" => dump = true,
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
    let parsed = parse(&src);
    for diagnostic in &parsed.diagnostics {
        eprintln!("{}", diagnostic.render_line(&src));
    }
    if !parsed.diagnostics.is_empty() {
        return ExitCode::FAILURE;
    }
    let resolved = resolve(&parsed.ast, &src);
    for diagnostic in &resolved.diagnostics {
        eprintln!("{}", diagnostic.render_line(&src));
    }
    if !resolved.diagnostics.is_empty() {
        return ExitCode::FAILURE;
    }
    if dump {
        print!("{}", dump_scopes(&parsed.ast, &resolved, &src));
    }
    ExitCode::SUCCESS
}
