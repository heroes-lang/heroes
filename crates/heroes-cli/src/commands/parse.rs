//! `heroes parse <file.hero> [--dump-ast]` — the syntax (M2).
//!
//! Without the flag it is a syntax check: silent on success. With it, the tree —
//! but only when the file is clean, because a tree built out of recovery guesses
//! reads like a fact and is not one.

use heroes::printer::dump_ast;
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
    for diagnostic in &out.diagnostics {
        eprintln!("{}", diagnostic.render_line(&src));
    }
    if !out.diagnostics.is_empty() {
        return Exit::Diagnostics;
    }
    if args.has("--dump-ast") {
        print!("{}", dump_ast(&out.ast, &src));
    }
    Exit::Ok
}
