//! **The gate and the emitter agree, on every program the compiler accepts.**
//!
//! The corpus is free, for the second time: `heroes mutate` makes one plausible
//! mistake per site across the gallery to measure the thesis, and every mutant it
//! does *not* catch is a program that type-checks and lowers. `ir/tests/mutants.rs`
//! runs the verifier over those; this file runs the **backend** over them, and asserts
//! the one thing that cannot be checked case by case.
//!
//! The invariant: **the emitter never falls through to `hero_unreachable()` because
//! of a form the gate let past.** Every arm in `inst.rs` and `term.rs` that this
//! milestone does not implement emits a marked `hero_unreachable()`, so that adding an
//! instruction to the IR is a compile error rather than a silent omission. If the gate
//! misses a row, that marker reaches real generated C — and what it produces is not a
//! crash at compile time but a program that *builds*, links, runs, and aborts at
//! whatever point the missing form was reached. Exactly the silent-wrong-answer class
//! the gate exists to prevent, one row of a table away.
//!
//! Checked over hundreds of programs nobody wrote, which is the argument
//! `llvm-opt-fuzzer` makes with `verifyModule` and Csmith makes in its own verdict:
//! "fixed test suites … are an inadequate mechanism for quality control".

use crate::emit::emit;
use crate::ir::{lower, verify};
use crate::mutate::{corpus, mutants, OPERATORS};
use crate::resolve::resolve;
use crate::source::Source;
use crate::syntax::parse;
use crate::types::check;

#[test]
fn no_accepted_program_emits_c_the_gate_should_have_refused() {
    let mut emitted = 0;
    let mut refused = 0;
    for (name, text) in corpus() {
        for operator in OPERATORS {
            for mutant in mutants(operator.id, &name, &text) {
                let src = Source::new(name.clone(), mutant);
                let parsed = parse(&src);
                if !parsed.diagnostics.is_empty() {
                    continue;
                }
                let resolved = resolve(&parsed.ast, &src);
                if !resolved.diagnostics.is_empty() {
                    continue;
                }
                let checked = check(&parsed.ast, &resolved, &src);
                if !checked.diagnostics.is_empty() || !checked.holes.is_empty() {
                    continue;
                }
                let lowered = lower(&parsed.ast, &resolved, &checked, &src);
                if !lowered.diagnostics.is_empty() {
                    continue;
                }
                if !verify(&lowered.program, &checked).is_empty() {
                    continue; // `ir/tests/mutants.rs` owns that invariant.
                }
                let out = emit(&lowered.program, &parsed.ast, &resolved, &checked, &src);
                if out.diagnostics.is_empty() {
                    emitted += 1;
                    assert!(
                        !out.c.contains("the gate refuses"),
                        "{name}: the gate accepted a program whose C falls through to \
                         hero_unreachable(). That C compiles, links, runs and aborts — the \
                         silent wrong answer the gate exists to prevent.\n{}",
                        out.c
                    );
                    // And whatever it did emit is a translation unit, not a fragment.
                    assert!(out.c.contains("#include \"heroes_runtime.h\""), "{name}");
                } else {
                    refused += 1;
                    assert!(out.c.is_empty(), "{name}: a refused program emitted C anyway");
                }
            }
        }
    }
    // Both must be non-zero or this test is asserting nothing: all-refused would mean
    // the emitter never ran, and all-emitted would mean the gate never did. The counts
    // are small because the *frontend* catches 96% of mutants before this point
    // (measurement 002) — which is the thesis working, and it means this invariant runs
    // over the residue rather than over the corpus. It will grow with every gate row
    // M5b and M5c delete.
    assert!(refused >= 10, "only {refused} mutants were refused — is the gate running?");
    assert!(emitted >= 3, "only {emitted} mutants were emitted — is the emitter running?");
}
