//! Metric 3, offline: the silent-error rate (design.md Part 11).
//!
//! The thesis says every plausible LLM mistake should be a compile error. This
//! pass is the falsifier: take a program that compiles, make one plausible
//! mistake in it, and ask whether the compiler notices. A mutant the compiler
//! **rejects** is KILLED — the design worked. One it **accepts** is SURVIVED, and
//! that is a silent wrong program, which the thesis says should not exist.
//!
//! Three rules from `harness/mutations/operators.md`, and they are what makes the
//! number mean anything:
//!
//! 1. **The operators are data, not code.** They are listed in that file with the
//!    plausible-mistake class each imitates and the §-rule that should kill it;
//!    this module implements the list and adds nothing to it.
//! 2. **A mutant that fails to parse is excluded.** It measures the lexer, not the
//!    thesis. Reported separately so the exclusion is visible rather than
//!    convenient.
//! 3. **Two arms.** `check` and `check --permissive`, the second with the thirteen
//!    thesis rules dropped (`Diagnostic::is_thesis_rule`). The difference between
//!    the two columns *is* the measured effect of the design — a language that
//!    rejects everything maximises kill rate and proves nothing, which is why
//!    panel 011 made the control arm mandatory.
//!
//! One mutant per site, sites found in a deterministic order (the arena's), so a
//! run is reproducible from the compiler sha alone.

use crate::resolve::resolve;
use crate::syntax::parse;
use crate::types::check;

mod edits;
mod operators;
mod survivors;
#[cfg(test)]
mod tests;

pub use operators::OPERATORS;
pub use survivors::{Survivor, report as survivor_report};

/// Every mutant one operator makes of one source.
///
/// Exported because the *surviving* mutants are a free corpus for anything that
/// must hold on **every program the compiler accepts**: they type-check by
/// definition, so lowering them must produce a well-formed IR. `llvm-opt-fuzzer`
/// does the same thing with `verifyModule` over a mutated module; the mutants here
/// were built to measure the thesis and cost nothing to reuse as its corpus.
pub fn mutants(operator: &str, name: &str, text: &str) -> Vec<String> {
    operators::apply(operator, name, text)
}

/// What became of one mutant.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Fate {
    /// The compiler rejected it: the mistake is loud.
    Killed,
    /// The compiler accepted it: a silent wrong program.
    Survived,
    /// It did not parse, so it measures the lexer and not the thesis.
    Excluded,
}

/// One operator's score over a corpus, in both arms.
pub struct Score {
    pub operator: String,
    pub mutants: usize,
    pub excluded: usize,
    pub killed_strict: usize,
    pub killed_permissive: usize,
}

impl Score {
    /// Kill rate over the mutants that count, as a percentage.
    pub fn rate(killed: usize, mutants: usize, excluded: usize) -> f64 {
        let counted = mutants - excluded;
        if counted == 0 {
            return 0.0;
        }
        100.0 * killed as f64 / counted as f64
    }
}

/// Run every operator over every source, and score them.
///
/// `sources` are programs that **check clean today** — the gallery and the
/// acceptance program. Mutating a program that is already wrong would measure
/// nothing: the compiler would reject the mutant for the mistake that was already
/// there. That sentence was a convention until 2026-08-12 and is now enforced by
/// the caller, over `base_checks_clean` below.
pub fn run(sources: &[(String, String)]) -> Vec<Score> {
    scored(sources, None).0
}

/// The same run, keeping the survivors and optionally narrowed to one operator.
///
/// **Two returns rather than two passes.** A survivor is discovered by the loop
/// that scores it, so re-deriving the list would mean running the whole frontend
/// over every mutant a second time — and, worse, would let the printed survivors
/// and the printed rate come from two different traversals. They must not be able
/// to disagree, which is `mod.rs`'s reason for having one `fate` for both arms.
///
/// `only` is the operator id from `--operator`; `None` runs the twelve.
pub fn scored(sources: &[(String, String)], only: Option<&str>) -> (Vec<Score>, Vec<Survivor>) {
    let mut scores: Vec<Score> = Vec::new();
    let mut survived: Vec<Survivor> = Vec::new();
    for operator in OPERATORS {
        if only.is_some_and(|id| id != operator.id) {
            continue;
        }
        let mut score = Score {
            operator: operator.id.to_string(),
            mutants: 0,
            excluded: 0,
            killed_strict: 0,
            killed_permissive: 0,
        };
        for (name, text) in sources {
            for mutant in operators::apply(operator.id, name, text) {
                score.mutants += 1;
                match fate(name, &mutant, false) {
                    Fate::Excluded => score.excluded += 1,
                    Fate::Killed => {
                        score.killed_strict += 1;
                        if fate(name, &mutant, true) == Fate::Killed {
                            score.killed_permissive += 1;
                        }
                    }
                    Fate::Survived => {
                        survived.extend(Survivor::locate(operator.id, name, text, &mutant));
                    }
                }
            }
        }
        scores.push(score);
    }
    (scores, survived)
}

/// Whether a corpus program is one this measurement may use: it compiles, today,
/// with nothing to report.
///
/// **The same pipeline that judges the mutants judges the base**, deliberately.
/// `heroes mutate` pointed at `crates/heroes/src/library/` reported 77 sites and
/// 100% killed while `heroes check` refused that very corpus (`range` is a
/// built-in, because the library *is* the built-ins) — every mutant killed by a
/// diagnostic that was already there. Metric 3's one number that can be wrong in
/// the flattering direction, found by a scaffold rather than by the harness
/// (panel 038; author decision 2026-08-12: refuse, do not silently exclude).
pub fn base_checks_clean(name: &str, text: &str) -> bool {
    fate(name, text, false) == Fate::Survived
}

/// One mutant through the real frontend. `permissive` drops the thesis rules,
/// which is exactly what `check --permissive` does — the same function, so the
/// arms cannot drift apart.
fn fate(name: &str, text: &str, permissive: bool) -> Fate {
    // **The whole compilation, not the one file.** `mutate` measures what the
    // compiler catches, so it has to see what the compiler sees: the library and
    // every module the file names. Measured on a bare `Source`, a module file
    // scores 100% caught on one unrelated complaint about a `use` line nobody
    // loaded — a number that is high and means nothing.
    let src = crate::modules::load_text(name, text.to_string());
    let parsed = parse(&src);
    if !parsed.diagnostics.is_empty() {
        // A parse failure is an exclusion, not a kill: it measures the lexer.
        return Fate::Excluded;
    }
    let resolved = resolve(&parsed.ast, &src);
    let checked = check(&parsed.ast, &resolved, &src);
    let mut reported = resolved
        .diagnostics
        .iter()
        .chain(checked.diagnostics.iter())
        .filter(|d| !permissive || !d.is_thesis_rule());
    if reported.next().is_some() {
        Fate::Killed
    } else {
        Fate::Survived
    }
}

/// The table, as text. Written to be pasted into `docs/measurements/NNN.md`.
pub fn report(scores: &[Score]) -> String {
    let mut out = String::from(
        "| operator | mutants | excluded | killed (check) | killed (--permissive) |\n|---|---|---|---|---|\n",
    );
    let (mut mutants, mut excluded, mut strict, mut permissive) = (0, 0, 0, 0);
    for score in scores {
        mutants += score.mutants;
        excluded += score.excluded;
        strict += score.killed_strict;
        permissive += score.killed_permissive;
        out.push_str(&format!(
            "| {} | {} | {} | {} ({:.0}%) | {} ({:.0}%) |\n",
            score.operator,
            score.mutants,
            score.excluded,
            score.killed_strict,
            Score::rate(score.killed_strict, score.mutants, score.excluded),
            score.killed_permissive,
            Score::rate(score.killed_permissive, score.mutants, score.excluded),
        ));
    }
    out.push_str(&format!(
        "| **total** | {mutants} | {excluded} | {strict} ({:.0}%) | {permissive} ({:.0}%) |\n",
        Score::rate(strict, mutants, excluded),
        Score::rate(permissive, mutants, excluded),
    ));
    out.push_str(
        "\nNever pool these into one headline (panel 011): a per-operator rate is\nthe measurement, and `forget-at-decl` is excluded from any summary because\nits catch rate is 100% by construction.\n",
    );
    out
}

/// The corpus every mutation invariant runs over: the gallery, plus the programs
/// that actually compile and run.
///
/// Test-only, and shared rather than copied — `emit/tests/mutants.rs` asserts the
/// gate and the emitter agree over it, `types/tests/sizes.rs` asserts the type
/// order is a real topological order over it, and two readers of one corpus must
/// not disagree about what the corpus is.
///
/// The second directory is the point. `examples/gallery/` is what `heroes mutate`
/// scores, and at M-scalars-run exactly one of its programs was inside the backend's subset,
/// so mutating it produced **zero** emitted mutants and the invariant asserted
/// nothing.
#[cfg(test)]
pub(crate) fn corpus() -> Vec<(String, String)> {
    let mut paths: Vec<std::path::PathBuf> = Vec::new();
    for dir in [
        concat!(env!("CARGO_MANIFEST_DIR"), "/../../examples/gallery"),
        concat!(env!("CARGO_MANIFEST_DIR"), "/../../tests/golden/run"),
    ] {
        paths.extend(
            std::fs::read_dir(dir)
                .unwrap_or_else(|e| panic!("{dir} must exist: {e}"))
                .map(|entry| entry.expect("a readable entry").path())
                .filter(|path| path.extension().and_then(|e| e.to_str()) == Some("hero")),
        );
    }
    paths.sort();
    paths
        .into_iter()
        .map(|path| {
            let name = path.file_name().expect("a file name").to_string_lossy().into_owned();
            let text = std::fs::read_to_string(&path).expect("a readable .hero file");
            (name, text)
        })
        .collect()
}
