//! `heroes check <file.hero> [flags]` — names and types (M3).
//!
//! The command the golden `check/` cases run through, and where every frontend
//! stage's diagnostics come out. The stages are ordered, not interleaved: **each
//! runs only if the one before it said nothing.** After a parse error the tree is
//! recovery guesses, and a name error about a line the author did not write costs
//! more than it buys; after a name error the checker would be typing expressions
//! whose meaning is unknown.
//!
//! Five flags, and each answers a different question about the same input:
//!
//! | flag | question |
//! |------|----------|
//! | *(none)* | is this program wrong, and where? (§4.17's full form) |
//! | `--brief` | the same, one line each — what a terminal scans and the goldens pin |
//! | `--json` | the same, for another program (schema 1) |
//! | `--permissive` | how wrong would it be *without* the thesis? (Part 11's control arm) |
//! | `--apply` | what would it look like repaired? (only `certain` fixes) |
//! | `--dump-scopes` | what did the resolver decide? |
//!
//! A file with `???` in it is **not** wrong: it type-checks everything else and
//! the holes are reported on stdout with the exit code left at 0 (§4.16).

use heroes::diagnostics::{render, Certainty, Diagnostic};
use heroes::printer::dump_scopes;
use heroes::resolve::resolve;
use heroes::source::Source;
use heroes::syntax::parse;
use heroes::types::{check, report_holes};

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
    // Stage by stage: the next one runs only on silence.
    let parsed = parse(&src);
    if let Some(exit) = report(&parsed.diagnostics, &src, args) {
        return exit;
    }
    let resolved = resolve(&parsed.ast, &src);
    if let Some(exit) = report(&resolved.diagnostics, &src, args) {
        return exit;
    }
    let checked = check(&parsed.ast, &resolved, &src);
    if let Some(exit) = report(&checked.diagnostics, &src, args) {
        return exit;
    }
    if args.has("--dump-scopes") {
        print!("{}", dump_scopes(&parsed.ast, &resolved, &src));
    }
    // §4.16: holes are output, not diagnostics. The program is fine; it is
    // unfinished, and the compiler says what belongs in the gaps.
    if !checked.holes.is_empty() {
        print!("{}", report_holes(&parsed.ast, &resolved, &checked, &src));
    }
    Exit::Ok
}

/// Print a stage's diagnostics in the form the flags asked for, and say whether
/// to stop. `None` means "nothing to report, carry on".
fn report(diagnostics: &[Diagnostic], src: &Source, args: &Invocation) -> Option<Exit> {
    let kept: Vec<&Diagnostic> = if args.has("--permissive") {
        diagnostics.iter().filter(|d| !d.is_thesis_rule()).collect()
    } else {
        diagnostics.iter().collect()
    };
    if kept.is_empty() {
        return None;
    }
    if args.has("--apply") {
        // The repair is the artifact here, so the diagnostics stay quiet: a
        // caller that wants both runs the command twice.
        print!("{}", apply(&kept, src));
        return Some(Exit::Ok);
    }
    if args.has("--json") {
        eprint!("{}", json(&kept, src));
    } else if args.has("--brief") {
        for diagnostic in &kept {
            eprintln!("{}", diagnostic.render_line(src));
        }
    } else {
        eprint!("{}", rich(&kept, src));
    }
    Some(Exit::Diagnostics)
}

/// §4.17's form, for the filtered list. `render_all` takes owned diagnostics and
/// what is in hand here is a filtered slice of references, so the join happens
/// here rather than cloning every diagnostic to satisfy a signature.
fn rich(diagnostics: &[&Diagnostic], src: &Source) -> String {
    let rendered: Vec<String> = diagnostics.iter().map(|d| render(d, src)).collect();
    rendered.join("\n")
}

/// Schema 1. Versioned because a schema is a promise: a consumer that reads
/// `schema` can refuse a version it does not know, which is the whole reason the
/// field is first.
fn json(diagnostics: &[&Diagnostic], src: &Source) -> String {
    let mut out = String::from("{\n  \"schema\": 1,\n  \"diagnostics\": [\n");
    let last = diagnostics.len() - 1;
    for (index, diagnostic) in diagnostics.iter().enumerate() {
        let (line, col) = src.line_col(diagnostic.span.start);
        out.push_str("    {\n");
        out.push_str(&format!("      \"code\": \"{}\",\n", diagnostic.code));
        out.push_str(&format!("      \"message\": {},\n", quote(&diagnostic.message)));
        out.push_str(&format!("      \"file\": {},\n", quote(&src.name)));
        out.push_str(&format!("      \"line\": {line},\n      \"col\": {col},\n"));
        let notes: Vec<String> = diagnostic.notes.iter().map(|n| quote(n)).collect();
        out.push_str(&format!("      \"notes\": [{}],\n", notes.join(", ")));
        out.push_str("      \"fixes\": [");
        let fixes: Vec<String> = diagnostic
            .fixes
            .iter()
            .map(|fix| {
                format!(
                    "{{\"title\": {}, \"replacement\": {}, \"certainty\": \"{}\"}}",
                    quote(&fix.title),
                    quote(&fix.replacement),
                    match fix.certainty {
                        Certainty::Certain => "certain",
                        Certainty::Guess => "guess",
                    }
                )
            })
            .collect();
        out.push_str(&fixes.join(", "));
        out.push_str("]\n");
        out.push_str(if index == last { "    }\n" } else { "    },\n" });
    }
    out.push_str("  ]\n}\n");
    out
}

fn quote(text: &str) -> String {
    let escaped = text
        .replace('\\', "\\\\")
        .replace('"', "\\\"")
        .replace('\n', "\\n");
    format!("\"{escaped}\"")
}

/// Applies every `certain` fix, back to front so the earlier spans stay valid.
///
/// Only `certain`, and that is CLAUDE.md §8 rather than caution: a `guess` is
/// prose for the reader, and a tool that applied one would be writing the
/// program's meaning on the author's behalf.
fn apply(diagnostics: &[&Diagnostic], src: &Source) -> String {
    let mut edits: Vec<(u32, u32, String)> = Vec::new();
    for diagnostic in diagnostics {
        for fix in &diagnostic.fixes {
            if fix.certainty == Certainty::Certain {
                edits.push((fix.span.start, fix.span.end, fix.replacement.clone()));
            }
        }
    }
    edits.sort_by_key(|(start, _, _)| *start);
    let mut text = src.text.clone();
    for (start, end, replacement) in edits.into_iter().rev() {
        text.replace_range(start as usize..end as usize, &replacement);
    }
    text
}
