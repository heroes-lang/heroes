//! `heroes check <file.hero> [--dump-scopes]` — names and types (M3).
//!
//! This is the command the golden `check/` cases run through, and it is where
//! every frontend stage's diagnostics come out. The stages are ordered, not
//! interleaved: **each runs only if the one before it said nothing.** After a
//! parse error the tree is recovery guesses, and a name error about a line the
//! author did not write costs more than it buys; after a name error the checker
//! would be typing expressions whose meaning is unknown.

use heroes::printer::dump_scopes;
use heroes::resolve::resolve;
use heroes::syntax::parse;
use heroes::types::check;

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
    let parsed = parse(&src);
    for diagnostic in &parsed.diagnostics {
        eprintln!("{}", diagnostic.render_line(&src));
    }
    if !parsed.diagnostics.is_empty() {
        return Exit::Diagnostics;
    }
    let resolved = resolve(&parsed.ast, &src);
    for diagnostic in &resolved.diagnostics {
        eprintln!("{}", diagnostic.render_line(&src));
    }
    if !resolved.diagnostics.is_empty() {
        return Exit::Diagnostics;
    }
    let checked = check(&parsed.ast, &resolved, &src);
    for diagnostic in &checked.diagnostics {
        eprintln!("{}", diagnostic.render_line(&src));
    }
    if !checked.diagnostics.is_empty() {
        return Exit::Diagnostics;
    }
    if args.has("--dump-scopes") {
        print!("{}", dump_scopes(&parsed.ast, &resolved, &src));
    }
    Exit::Ok
}
