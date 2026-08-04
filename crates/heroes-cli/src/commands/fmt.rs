//! `heroes fmt <file.hero> [--in-place]` — the canonical form (M2, §4.15).
//!
//! Prints to stdout by default, like `gofmt`; `--in-place` rewrites the file and
//! prints nothing. The flag was `--write` until panel 016: "write" is read as
//! "write the output somewhere", the *non*-destructive meaning, which made the
//! plausible misreading the one that overwrites a file. The retired spelling
//! names its replacement, the same treatment the language gives `fn`.
//!
//! Either way it **refuses a file with diagnostics**: formatting a tree built out
//! of recovery guesses would rewrite the author's program into the parser's guess
//! about it, which is the one thing a formatter must never do.

use heroes::printer::format_file;
use heroes::syntax::parse;

use crate::cli::{Exit, Invocation};
use crate::input;

pub fn run(path: &str, args: &Invocation) -> Exit {
    let src = match input::read(path) {
        Ok(src) => src,
        Err((message, exit)) => {
            eprintln!("error: {message}");
            return exit;
        }
    };
    let out = parse(&src);
    if !out.diagnostics.is_empty() {
        for diagnostic in &out.diagnostics {
            eprintln!("{}", diagnostic.render_line(&src));
        }
        eprintln!("error: refusing to format a file with diagnostics");
        return Exit::Diagnostics;
    }
    let formatted = format_file(&out.ast, &out.comments, &src);
    if !args.has("--in-place") {
        print!("{formatted}");
        return Exit::Ok;
    }
    if formatted == src.text {
        return Exit::Ok; // already canonical: no write, no mtime change
    }
    match std::fs::write(path, &formatted) {
        Ok(()) => Exit::Ok,
        Err(e) => {
            eprintln!("error: cannot write `{path}`: {e}");
            Exit::Failed
        }
    }
}
