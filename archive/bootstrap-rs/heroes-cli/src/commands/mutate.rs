//! `heroes mutate [dir]` — metric 3, offline (design.md Part 11).
//!
//! Takes every program in the corpus that checks clean, makes **one plausible
//! mistake per site**, and counts how many the compiler catches — in two arms, so
//! the number means something. A language that rejects everything would score
//! 100% and prove nothing, which is why the control arm (`--permissive`, the same
//! compiler with the thesis rules dropped) is printed beside it.
//!
//! The corpus is the *clean* programs: `examples/gallery/` and `examples/`.
//! Mutating a program that is already wrong measures nothing, because the
//! compiler would reject the mutant for the mistake that was already there. The
//! acceptance program is included by the caller passing `design.md`'s fence — the
//! harness does that; this command's default is the directory.

use heroes::mutate::{
    OPERATORS, base_checks_clean, report, scored, survivor_report,
};

use crate::cli::Exit;

fn corpus(dir: &str) -> Result<Vec<(String, String)>, String> {
    let mut sources: Vec<(String, String)> = Vec::new();
    collect(std::path::Path::new(dir), &mut sources)?;
    sources.sort();
    Ok(sources)
}

fn collect(
    dir: &std::path::Path,
    sources: &mut Vec<(String, String)>,
) -> Result<(), String> {
    let entries = std::fs::read_dir(dir).map_err(|e| format!("cannot read {}: {e}", dir.display()))?;
    for entry in entries {
        let path = entry.map_err(|e| format!("{e}"))?.path();
        if path.is_dir() {
            collect(&path, sources)?;
            continue;
        }
        if path.extension().and_then(|e| e.to_str()) != Some("hero") {
            continue;
        }
        let text = std::fs::read_to_string(&path)
            .map_err(|e| format!("cannot read {}: {e}", path.display()))?;
        sources.push((path.display().to_string(), text));
    }
    Ok(())
}

/// Every program in the corpus must compile *before* anything is mutated.
///
/// The refusal is exit 2 and not a quiet exclusion, by author decision
/// 2026-08-12: a corpus this command cannot use is a mistake in the invocation,
/// which is what exit 2 means, and a run that silently dropped half its input
/// would print a rate over a corpus nobody named. Every offending file is listed
/// rather than the first, because a corpus is usually wrong in one way and fixing
/// it one error per run is the slowest possible loop.
fn refuse_a_corpus_that_does_not_compile(sources: &[(String, String)]) -> Option<String> {
    let refused: Vec<&String> = sources
        .iter()
        .filter(|(name, text)| !base_checks_clean(name, text))
        .map(|(name, _)| name)
        .collect();
    if refused.is_empty() {
        return None;
    }
    let mut message = format!(
        "this corpus does not compile, so its kill rate would be meaningless\n  \
         {} of {} programs are already refused by `heroes check`:\n",
        refused.len(),
        sources.len()
    );
    for name in refused {
        message.push_str(&format!("    {name}\n"));
    }
    message.push_str(
        "  every mutant of a program that is already wrong is killed by the\n  \
         diagnostic that was already there — run `heroes check` on each",
    );
    Some(message)
}

pub fn run(dir: Option<&str>, survivors: bool, operator: Option<&str>) -> Exit {
    let dir = dir.unwrap_or("examples");
    if let Some(id) = operator {
        if !OPERATORS.iter().any(|o| o.id == id) {
            let known: Vec<&str> = OPERATORS.iter().map(|o| o.id).collect();
            // **The count is read, never written.** It said "twelve" while there
            // were thirteen, from the day panel 040's own falsifier arrived as
            // `boolean-twin` — a number in prose is a premise about the world and
            // it expires in silence (CLAUDE.md §11).
            eprintln!(
                "error: no operator `{id}`\n  the {} are: {}",
                known.len(),
                known.join(", ")
            );
            return Exit::Failed;
        }
    }
    let sources = match corpus(dir) {
        Ok(sources) => sources,
        Err(message) => {
            eprintln!("error: {message}");
            return Exit::Failed;
        }
    };
    if sources.is_empty() {
        eprintln!("error: no `.hero` files under {dir}");
        return Exit::Failed;
    }
    if let Some(message) = refuse_a_corpus_that_does_not_compile(&sources) {
        eprintln!("error: {message}");
        return Exit::Failed;
    }
    println!("corpus: {} programs under {dir}\n", sources.len());
    let (scores, survived) = scored(&sources, operator);
    print!("{}", report(&scores));
    if survivors {
        print!("{}", survivor_report(&survived));
    }
    Exit::Ok
}
