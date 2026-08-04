//! `heroes measure [file]` — the spec budget, measured (design.md §1.6).
//!
//! Defaults to `spec/heroes-spec.md`, since that is the file the ceiling is
//! about. Prints every instrument's reading, their spread, and the verdict
//! against the 3000-token ceiling; exits non-zero on a breach, so the rule
//! is enforceable rather than merely stated.

use std::process::ExitCode;

use heroes::measure::{measure, spec_path, vendor_dir};

/// design.md §1.6, as raised by panel 012 and measured, not estimated.
const CEILING: usize = 3000;
/// panel 012's soft threshold: above it, an addition needs a named removal
/// or a pre-registered prediction.
const SOFT: usize = 2000;

const USAGE: &str = "usage: heroes measure [file]   (default: spec/heroes-spec.md)";

pub fn run(args: &[String]) -> ExitCode {
    let mut file: Option<&String> = None;
    for a in args {
        if a.starts_with('-') || file.is_some() {
            eprintln!("error: unexpected argument `{a}`\n{USAGE}");
            return ExitCode::FAILURE;
        }
        file = Some(a);
    }
    let path = match file {
        Some(f) => std::path::PathBuf::from(f),
        None => spec_path(),
    };
    let text = match std::fs::read_to_string(&path) {
        Ok(t) => t,
        Err(e) => {
            eprintln!("error: cannot read `{}`: {e}", path.display());
            return ExitCode::FAILURE;
        }
    };
    let m = match measure(&text, &vendor_dir()) {
        Ok(m) => m,
        Err(e) => {
            eprintln!("error: {e}");
            return ExitCode::FAILURE;
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
        return ExitCode::FAILURE;
    }
    if max > SOFT {
        println!("\nAbove the soft {SOFT}: an addition needs a named removal or a");
        println!("pre-registered falsifiable prediction (panel 012). Headroom: {}.", CEILING - max);
    } else {
        println!("\nUnder the soft {SOFT}: Principle 0 alone. Headroom: {}.", CEILING - max);
    }
    ExitCode::SUCCESS
}

fn spread_percent(m: &heroes::measure::Measurement) -> usize {
    let max = m.max();
    if max == 0 {
        0
    } else {
        m.spread() * 100 / max
    }
}
