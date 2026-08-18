//! `heroes measure [file]` — the spec budget, measured (design.md §1.6).
//!
//! Defaults to `spec/heroes-spec.md`, since that is the file the ceiling is
//! about. Prints every instrument's reading, their spread, and the verdict
//! against the ceiling; exits non-zero on a breach, so the rule is enforceable
//! rather than merely stated.

use heroes::measure::{measure, spec_path, vendor_dir};

use crate::cli::Exit;

/// design.md §1.6, raised to 4096 by author decision (2026-08-10, panel 024
/// retro-record). Measured, never estimated — that is the point of the budget.
const CEILING: usize = 4096;
/// The soft threshold, and it is deliberately **not** rescaled with the ceiling.
///
/// What the soft line is *for* is when an addition starts owing a named removal or
/// a pre-registered falsifiable prediction (panel 012). Rescaling it in proportion
/// — 2:3 would put it near 2730 — would drop that burden overnight for a document
/// that measures 2231, which is the one thing §1.6 says must survive a raise:
/// "a spec that grows to fill the budget because it can has failed §1.2 just as
/// surely as one that breaches it". So the ceiling moved and the discipline did not.
const SOFT: usize = 2000;

pub fn run(file: Option<&str>) -> Exit {
    let path = match file {
        Some(f) => std::path::PathBuf::from(f),
        None => spec_path(),
    };
    let text = match std::fs::read_to_string(&path) {
        Ok(t) => t,
        Err(e) => {
            eprintln!("error: cannot read `{}`: {e}", path.display());
            return Exit::Failed;
        }
    };
    let m = match measure(&text, &vendor_dir()) {
        Ok(m) => m,
        Err(e) => {
            // **2, not 1.** Both of `measure`'s error arms are "the vendored
            // tokeniser tables could not be read" — the tool failing to run, not
            // the input having a mistake in it (CLAUDE.md §10). The two adjacent
            // paths in this one function used to disagree: an unreadable spec
            // file exited 2 and a missing table exited 1.
            eprintln!("error: {e}");
            return Exit::Failed;
        }
    };

    println!("{}", path.display());
    for r in &m.readings {
        println!(
            "  {:<16} {:>6}   ({} ranks)",
            r.instrument, r.tokens, r.vocabulary
        );
    }
    let max = m.max();
    println!("  {:<16} {:>6}   the binding number", "maximum", max);
    println!("  {:<16} {:>6}   the error bar ({}%)", "spread", m.spread(), spread_percent(&m));

    if max > CEILING {
        println!("\nBREACH: {max} over the {CEILING}-token ceiling (design.md §1.6).");
        println!("An amendment must be net-negative, or the ceiling moves — with a");
        println!("measurement, a panel, and a named alternative (panel 012).");
        return Exit::Diagnostics;
    }
    if max > SOFT {
        println!("\nAbove the soft {SOFT}: an addition needs a named removal or a");
        println!("pre-registered falsifiable prediction (panel 012). Headroom: {}.", CEILING - max);
    } else {
        println!("\nUnder the soft {SOFT}: Principle 0 alone. Headroom: {}.", CEILING - max);
    }
    Exit::Ok
}

fn spread_percent(m: &heroes::measure::Measurement) -> usize {
    let max = m.max();
    if max == 0 {
        0
    } else {
        m.spread() * 100 / max
    }
}
