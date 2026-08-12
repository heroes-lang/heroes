//! The rich form: an error written to be read by a model (design.md §4.17).
//!
//! §4.17's whole argument is that a human has the project open and a model does
//! not, so a message that says "type mismatch at line 12" costs three
//! conversational turns for a comma. Every error therefore carries **what is
//! needed to fix the program without opening another file**: the line as written,
//! what is wrong with it, where the other half is, and the repairs — with the one
//! that preserves meaning marked as such.
//!
//! The shape, and every part of it is load-bearing:
//!
//! ```text
//! error[type_mismatch]: expected `int`, found `str`
//!   at examples/gallery/00-first.hero:5:12
//!    |
//!  5 |     return "x"
//!    |            ^^^
//!   note: `half` is declared `-> int` at line 1
//!   fix (certain): rename to `total`
//! ```
//!
//! - the **code** is stable and machine-readable (the same convention as `fail`
//!   codes), so a harness can count kinds without parsing prose;
//! - the **caret** is the span, not the line: a wrong argument in a long call is
//!   pointed at, which is the difference between reading a message and searching
//!   for what it meant;
//! - a **note** carries the other end — the declaration, the signature, the case
//!   that is missing — because that is the file the model would otherwise open;
//! - a **fix** is tagged, and only `certain` may ever be applied by a tool
//!   (CLAUDE.md §8). The tag is part of the output, not an internal flag: a model
//!   that cannot see which repair is safe will apply whichever one it read last.

use crate::source::Source;

use super::{Certainty, Diagnostic};

/// One diagnostic, in the rich form. No trailing blank line: the caller joins.
pub fn render(diagnostic: &Diagnostic, src: &Source) -> String {
    // Two line numbers, and they are not the same thing once a compilation has
    // more than one file: `line` is what the reader is told, `absolute` is where
    // the text actually is. Printing the second is a message that names a real
    // file and a line nobody can find in it.
    let (file, line, col) = src.locate(diagnostic.span.start);
    let (absolute, _) = src.line_col(diagnostic.span.start);
    let mut out = format!(
        "{}[{}]: {}\n  at {}:{line}:{col}\n",
        diagnostic.kind.word(),
        diagnostic.code,
        diagnostic.message,
        file
    );
    let number = format!("{line}");
    let gutter = " ".repeat(number.len());
    out.push_str(&format!("  {gutter} |\n"));
    let text = source_line(src, absolute);
    out.push_str(&format!("  {number} | {text}\n"));
    out.push_str(&format!("  {gutter} | {}\n", caret(src, diagnostic, col)));
    for note in &diagnostic.notes {
        out.push_str(&format!("  note: {note}\n"));
    }
    for fix in &diagnostic.fixes {
        let tag = match fix.certainty {
            Certainty::Certain => "certain",
            Certainty::Guess => "guess",
        };
        out.push_str(&format!("  fix ({tag}): {}\n", fix.title));
    }
    out
}

/// Every diagnostic, in source order, separated by a blank line.
pub fn render_all(diagnostics: &[Diagnostic], src: &Source) -> String {
    let rendered: Vec<String> = diagnostics.iter().map(|d| render(d, src)).collect();
    rendered.join("\n")
}

fn source_line(src: &Source, line: u32) -> String {
    src.text
        .lines()
        .nth(line as usize - 1)
        .unwrap_or("")
        .trim_end()
        .to_string()
}

/// The span, underlined. One byte is one column, and a span that runs past its
/// line is clamped, because a declaration's span covers its whole body and
/// pointing at forty lines of caret helps nobody.
///
/// **The padding copies the line's own tabs.** This used to say tabs cannot
/// appear because the lexer rejects them, which is true of every line except the
/// one the tab diagnostic is *about* — and that line is printed above the caret
/// like any other, so a terminal expanded the tab in the source line and not in
/// a caret padded with spaces. rustc's rule and rustc's reason: reuse the
/// whitespace rather than guess a tab width, and the two lines agree at any
/// setting (2026-08-12).
fn caret(src: &Source, diagnostic: &Diagnostic, col: u32) -> String {
    let (end_line, end_col) = src.line_col(diagnostic.span.end);
    let (start_line, _) = src.line_col(diagnostic.span.start);
    // Characters, because `line_col`'s column is characters — the underline was
    // one `^` per *byte* of the span, so `return "ààà"` was underlined eight
    // wide for five columns (2026-08-12, sweep 001 audit S5).
    let width = if end_line == start_line && end_col > col {
        (end_col - col) as usize
    } else {
        1
    };
    let start = src.line_start_of(diagnostic.span.start) as usize;
    let before = &src.text[start..diagnostic.span.start as usize];
    let padding: String =
        before.chars().map(|c| if c == '\t' { '\t' } else { ' ' }).collect();
    format!("{padding}{}", "^".repeat(width.max(1)))
}
