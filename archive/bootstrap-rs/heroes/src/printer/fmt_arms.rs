//! Aligning a `match`'s arms: where every `=>` goes (design.md §4.7).
//!
//! **One column per run of arms, never one per `match`.** A run ends at a blank line or
//! at an arm whose body is a block, so one very long pattern widens its immediate
//! neighbours and nothing further. The alternative — one column for the whole construct —
//! is how a formatter turns a one-line edit into a diff across twenty untouched lines.
//!
//! Only an arm that fits on one line is aligned: an arm that must break carries no `=>`
//! to line up.

use crate::source::Source;
use crate::syntax::{ArmBody, Ast};

use super::fmt::WIDTH;
use super::fmt_fits::fits_on_one_line;

/// The left side of every arm — its patterns, joined by `|`.
pub(super) fn arm_heads(ast: &Ast, src: &Source, arms: &[crate::syntax::Arm]) -> Vec<String> {
    arms.iter()
        .map(|arm| {
            let patterns: Vec<String> = arm
                .patterns
                .iter()
                .map(|pattern| super::bodies::render_pattern_public(ast, src, pattern))
                .collect();
            patterns.join(" | ")
        })
        .collect()
}

/// How wide each arm's left side is printed, so that `=>` lines up.
///
/// A `match` is the language's most distinctive construct and its arms are a
/// table: the patterns are the keys and the bodies are the values. Lining up the
/// arrow is the same policy gofmt applies to adjacent trailing comments, and it
/// is what design.md's own appendix does by hand — so the reference aesthetic
/// and the canonical form are now the same thing.
///
/// A run stops at a **block-bodied arm** (its `=>` ends the line, so there is
/// nothing to align with) and at a **blank line** between arms, because blank
/// lines are content in this formatter and a group the author separated is two
/// groups. Every other arm in the run is padded to the widest left side in it.
pub(super) fn arm_alignment(
    ast: &Ast,
    src: &Source,
    arms: &[crate::syntax::Arm],
    lefts: &[String],
    indent: usize,
) -> Vec<usize> {
    let mut pads = vec![0usize; arms.len()];
    // (index, the one-line length with this arm's OWN head) — the second is
    // what `close_run` needs to know whether widening it stays inside the margin.
    let mut run: Vec<(usize, usize)> = Vec::new();
    let mut previous_end = 0u32;
    for (index, arm) in arms.iter().enumerate() {
        let line = src.line_of(arm.span.start);
        let end = src.line_of(arm.span.end.saturating_sub(1));
        // **Will this arm print on one line?** — the printer's own question,
        // asked with the printer's own test, and not "were these two adjacent in
        // the source". `spans_lines`'s twin, three hundred lines up in this
        // file: an arm whose body the 88-column rule is about to break has
        // nothing to line up with, and deciding from source adjacency made
        // `fmt(fmt(x)) != fmt(x)` — pass one aligned three arms because the
        // source had them consecutive, pass two did not because the first now
        // occupied six lines (2026-08-12, sweep 001 audit S3).
        //
        // The answer survives the round trip, which is what makes it stable: an
        // arm that gets broken reads back as a multi-line value, and
        // `spans_lines` then says the same thing the width test said the first
        // time.
        let inline = matches!(arm.body, ArmBody::Stmt(_));
        let width = if inline {
            fits_on_one_line(ast, src, arm, lefts[index].chars().count(), indent)
        } else {
            None
        };
        let joined = !run.is_empty() && line == previous_end + 1;
        if width.is_none() || !joined {
            close_run(&run, lefts, indent, &mut pads);
            run.clear();
        }
        if let Some(len) = width {
            run.push((index, len));
        }
        previous_end = end;
    }
    close_run(&run, lefts, indent, &mut pads);
    pads
}

/// Pad every member to the widest left side — **unless doing so pushes one of
/// them past the margin**, in which case the run gets no padding at all.
///
/// Without that clause the alignment could break the very line it was widening,
/// and the next format would see a broken value and drop the arm from the run:
/// the same non-idempotence one door down.
fn close_run(run: &[(usize, usize)], lefts: &[String], indent: usize, pads: &mut [usize]) {
    let widest = run.iter().map(|(i, _)| lefts[*i].chars().count()).max().unwrap_or(0);
    let fits = run.iter().all(|(index, len)| {
        let grown = widest - lefts[*index].chars().count();
        indent + len + grown <= WIDTH
    });
    if !fits {
        return;
    }
    for (index, _) in run {
        pads[*index] = widest;
    }
}
