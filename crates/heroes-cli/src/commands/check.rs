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
    // The module graph, before any name is resolved: a missing module makes
    // every qualified name into it an unknown name, and reporting the cause
    // once beats reporting the consequence nine times (§4.17).
    if let Some(exit) = report(&heroes::modules::errors(&parsed.ast, &src), &src, args) {
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
    // A diagnostic pointing into the library is the COMPILER being wrong, not the
    // program: the line it names is in a file the author cannot open, so the
    // message is unactionable however good it is. Exit 2 and say so
    // (CLAUDE.md §10's contract; the class was panel 028 R5's).
    if let Some(what) = heroes::library::misplaced(diagnostics, src) {
        eprintln!("internal error: {what}");
        return Some(Exit::Failed);
    }
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
        // `Source::locate`, exactly as the two text renderers do. Assembling the
        // triple here is what made the machine-readable surface the last place
        // in the compiler still naming the root file for a diagnostic in another
        // module — the one consumer that cannot notice by eye (2026-08-12).
        let (file, line, col) = src.locate(diagnostic.span.start);
        out.push_str("    {\n");
        out.push_str(&format!("      \"code\": \"{}\",\n", diagnostic.code));
        out.push_str(&format!("      \"message\": {},\n", quote(&diagnostic.message)));
        out.push_str(&format!("      \"file\": {},\n", quote(file)));
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
    // **Fixes outside the file being written are skipped, and said out loud.**
    // A fix's span is an offset into the whole compilation, and the text below is
    // the root file alone — so a certain fix in a `use`d module indexed past the
    // end of the string and `replace_range` panicked: `assertion failed:
    // self.is_char_boundary(n)`, **exit 101**, a code CLAUDE.md §10 does not
    // have, on every multi-module program with a fixable mistake outside the root
    // (2026-08-12). Silently dropping them would be worse than the panic: this
    // flag's contract is *every* certain fix, and CI asserts `.fixed` files check
    // clean afterwards.
    let root_end = src.root_end();
    let mut elsewhere: Vec<String> = Vec::new();
    for diagnostic in diagnostics {
        for fix in &diagnostic.fixes {
            if fix.certainty != Certainty::Certain {
                continue;
            }
            if fix.span.end > root_end {
                let (file, line, _) = src.locate(fix.span.start);
                let at = format!("{file}:{line}");
                if !elsewhere.contains(&at) {
                    elsewhere.push(at);
                }
                continue;
            }
            edits.push((fix.span.start, fix.span.end, fix.replacement.clone()));
        }
    }
    if !elsewhere.is_empty() {
        eprintln!(
            "note: {} certain fix(es) are in another module and were not applied: {}",
            elsewhere.len(),
            elsewhere.join(", ")
        );
    }
    edits.sort_by_key(|(start, _, _)| *start);
    // **The author's file, not the compilation.** The library is appended to
    // every `Source` (§1.11, `heroes::library`), and `--apply` writes a program
    // back — with `--in-place` it writes it into their file. Emitting the whole
    // text would append the library to the user's source, once per invocation.
    let mut text = src.user_text().to_string();
    for (start, end, replacement) in edits.into_iter().rev() {
        text.replace_range(start as usize..end as usize, &replacement);
    }
    text
}
