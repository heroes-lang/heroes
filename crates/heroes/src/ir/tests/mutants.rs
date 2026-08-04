//! **Every program the compiler accepts must lower to a well-formed IR.**
//!
//! The corpus is free: `heroes mutate` already makes one plausible mistake per site
//! across the gallery to measure the thesis (metric 3), and the mutants it does *not*
//! catch are, by definition, programs that type-check. Each of those is a test case
//! for this invariant, and nobody had to write it.
//!
//! The precedent for *this* shape — a mutated corpus checked against an **internal
//! invariant** rather than an output — is `llvm-opt-fuzzer`, which mutates a module,
//! runs the pass pipeline, then calls `verifyModule` and reports "Transformation
//! resulted in an invalid module". Csmith is the famous generator and it is
//! *output*-differential (compile with several compilers, run, compare); what Csmith
//! contributes here is its verdict, which is this test's whole justification: "our
//! results suggest that fixed test suites — the main way that compilers are tested —
//! are an inadequate mechanism for quality control". An invariant holds over inputs
//! nobody imagined; an expectation can be no stronger than whoever wrote it.
//!
//! Two invariants are asserted per surviving mutant, and the second is the one that
//! costs nothing and catches the most: the **dump is byte-identical twice**. Any
//! iteration order that leaked into the IR — a `HashMap`, a pointer, an address —
//! shows up here across hundreds of programs rather than in the fourteen goldens.

use crate::ir::{dump, lower, verify};
use crate::mutate::{mutants, OPERATORS};
use crate::resolve::resolve;
use crate::source::Source;
use crate::syntax::parse;
use crate::types::check;

/// The gallery, as `(name, text)` — the same corpus `heroes mutate` runs on.
fn corpus() -> Vec<(String, String)> {
    let dir = concat!(env!("CARGO_MANIFEST_DIR"), "/../../examples/gallery");
    let mut paths: Vec<std::path::PathBuf> = std::fs::read_dir(dir)
        .expect("examples/gallery must exist")
        .map(|entry| entry.expect("a readable entry").path())
        .filter(|path| path.extension().and_then(|e| e.to_str()) == Some("hero"))
        .collect();
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

#[test]
fn every_mutant_the_frontend_accepts_lowers_to_a_well_formed_ir() {
    let mut accepted = 0;
    let mut rejected = 0;
    for (name, text) in corpus() {
        for operator in OPERATORS {
            for mutant in mutants(operator.id, &name, &text) {
                let src = Source::new(name.clone(), mutant);
                let parsed = parse(&src);
                if !parsed.diagnostics.is_empty() {
                    rejected += 1;
                    continue;
                }
                let resolved = resolve(&parsed.ast, &src);
                if !resolved.diagnostics.is_empty() {
                    rejected += 1;
                    continue;
                }
                let checked = check(&parsed.ast, &resolved, &src);
                if !checked.diagnostics.is_empty() {
                    rejected += 1;
                    continue;
                }
                // The frontend accepted it, so lowering must produce something the
                // verifier is happy with. A wrong *program* is fine here — it is a
                // mutant, it is supposed to be wrong. A wrong *IR* is this
                // compiler's bug.
                accepted += 1;
                let out = lower(&parsed.ast, &resolved, &checked, &src);
                let problems = verify(&out.program, &checked);
                assert!(
                    problems.is_empty(),
                    "{} ({}): the frontend accepted this and the IR is malformed: {}\n{}",
                    name,
                    operator.id,
                    problems.join("; "),
                    src.text
                );
                // Determinism, over hundreds of programs instead of fourteen.
                let once = dump(&out.program, &parsed.ast, &checked, &src);
                let again = lower(&parsed.ast, &resolved, &checked, &src);
                let twice = dump(&again.program, &parsed.ast, &checked, &src);
                assert_eq!(once, twice, "{name} ({}) dumps differently twice", operator.id);
            }
        }
    }
    // The counts are asserted so that a change which stops generating mutants — or
    // stops accepting any of them — fails here instead of passing vacuously. Metric
    // 3 recorded 379 mutants over this corpus with 94% caught, so the surviving
    // population is small by design and must not be zero.
    assert!(accepted >= 15, "only {accepted} mutants survived the frontend");
    assert!(rejected >= 200, "only {rejected} mutants were rejected");
}

/// The acceptance program, mutated. It is 320 lines against the gallery's ~60 each,
/// and it holds the shapes the gallery does not — a recursive variant, nested
/// `match`, `?` chains — so its surviving mutants reach lowering paths the gallery's
/// never do.
#[test]
fn every_surviving_mutant_of_the_acceptance_program_lowers() {
    let design = std::fs::read_to_string(concat!(env!("CARGO_MANIFEST_DIR"), "/../../design.md"))
        .expect("design.md must exist");
    let appendix = design
        .find("## Appendix — A complete example program")
        .expect("the appendix must exist");
    let tail = &design[appendix..];
    let open = tail.find("```\n").expect("the appendix has a code fence") + 4;
    let close = tail[open..].find("\n```").expect("the fence closes") + open;
    let program = tail[open..=close].to_string();

    let mut accepted = 0;
    for operator in OPERATORS {
        for mutant in mutants(operator.id, "appendix", &program) {
            let src = Source::new("appendix".to_string(), mutant);
            let parsed = parse(&src);
            if !parsed.diagnostics.is_empty() {
                continue;
            }
            let resolved = resolve(&parsed.ast, &src);
            if !resolved.diagnostics.is_empty() {
                continue;
            }
            let checked = check(&parsed.ast, &resolved, &src);
            if !checked.diagnostics.is_empty() {
                continue;
            }
            accepted += 1;
            let out = lower(&parsed.ast, &resolved, &checked, &src);
            let problems = verify(&out.program, &checked);
            assert!(
                problems.is_empty(),
                "appendix ({}): {}",
                operator.id,
                problems.join("; ")
            );
        }
    }
    assert!(accepted >= 5, "only {accepted} mutants of the appendix survived");
}
