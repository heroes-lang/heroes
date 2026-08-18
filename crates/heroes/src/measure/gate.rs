//! The delta gate: the spec cannot grow without a commit saying so (§1.6).
//!
//! Panel 024 raised the ceiling to 4096 and queued two instruments, because the
//! raise put the budget's veto out of reach: at the observed ~+30 per amendment,
//! 4096 binds after about sixty more panels where 3000 would have bound after
//! twenty-five. Its own precedent is PEP 8 — 79 columns is intact in the document
//! and irrelevant in practice, because Black's 88 decides. **A budget that is not
//! checked on every run is whatever the author's editor tolerates.**
//!
//! Panel 031 adopted the gate as a **test rather than a command**. CLAUDE.md §10's
//! stopping rule refuses it as a subcommand or a flag — it types no fixpoint
//! invocation, no golden harness and no Part 11 harness, which is the refusal that
//! struck `outline` and `explain` — and the golden harness types it here for free.
//!
//! The mechanism is one constant. A spec amendment turns this test red, and the
//! only way to green is to write the new number into the commit that made it true.
//! That is precisely the moment panel 012's rule asks for a named removal or a
//! registered falsifiable prediction, so the gate does not enforce the rule: it
//! makes the rule's question unavoidable, and the answer lands in the commit body
//! where a reader can check it.
//!
//! **What "a prediction" means here was narrowed by panel 046**, and the narrowing
//! came out of this very table: six of its rows were paid with a prediction and not
//! one was ever collected, because each named metric 2 — an instrument scheduled at
//! M-selfhost-fixpoint. A prediction now pays only if it names the instrument that
//! will score it *and* that instrument exists on the day it is registered; one that
//! does not is registered as an observation and pays nothing. The gate still checks
//! none of this — it reads no commit, calls no git, and any form that made it do so
//! is refused by the same §10 argument three paragraphs up. The rows below are the
//! record a reader audits it against.
//!
//! **The six prediction-bought rows — `2588`, `2627`, `2675`, `2745`, `2768`,
//! `2959` — were re-decided at M-program-corpus** (panel 046 R2, ratified
//! 2026-08-13): scored, or `lapsed` with the clause re-argued in `DECIDE.md` under
//! the removal branch, never renewed. The date is here because `/step` greps here.
//!
//! **Done, 2026-08-13. Two scored, four lapsed**, and the split is exactly the one
//! panel 046 R1 predicts: the two that named an instrument existing on the day of
//! registration were collectable, and the four that named metric 2 were not.
//!
//! | row | prediction named | outcome |
//! |---|---|---|
//! | `2588` | first-try rates (metric 2) | **lapsed** |
//! | `2627` | the next spec-only writing experiment | **scored — half held, half falsified** |
//! | `2675` | a `mutate --survivors` run over bitwise code | **scored — held, and vacuously** |
//! | `2745` | first metric-2 harness run | **lapsed** |
//! | `2768` | first-try failures (metric 2) | **lapsed** |
//! | `2959` | first-try failures (metric 2) | **lapsed** |
//!
//! The four lapsed rows keep their clauses in the spec — R1 governs what may be
//! *offered* as payment and is not retroactive — and each clause is back in
//! `DECIDE.md` to be re-argued under the removal branch. None is renewed with a
//! new milestone name, which is the one thing R2 forbids.
//!
//! ## The four, re-decided — 2026-08-14, by author decision
//!
//! **Not renewed. Re-registered**, which is the distinction R2 exists to draw: a
//! renewal differs from its registration only in a date or a milestone name, and
//! each of these differs in **what is claimed and what will answer it**. Every one
//! names an instrument that exists today *and has an arm for the question* (R1 as
//! amended the same day), and every one is countable with `grep` or `heroes
//! mutate` rather than with the metric-2 harness that does not run until the
//! fixpoint.
//!
//! The honest starting position is the one `DECIDE.md` stated: **three of the four
//! are §1.0 compiler-need** — the widths, `i64` and the bases are all on the road
//! to the port — so for those the re-decision is *keep, and here is what would
//! now falsify the clause*. `2588` is the one that is not, and it is the one whose
//! new prediction is written to be able to fail.
//!
//! | row | the clause | re-registered prediction | instrument, and its arm | scored at |
//! |---|---|---|---|---|
//! | `2588` | a group's `constant` has no body | at the next FFI rung, **≥12** `extern constant` declarations exist across `examples/` and `tests/golden/`, and every one names a value **no `.hero` file spells as a literal** | `grep -c "constant [A-Z_]*:"` beside a grep for the same names as literals — both arms exist; today the count is 107 declarations in total | M-ffi-ladder rung 5 |
//! | `2745` | the four bases and `_` | the corpus holds **44** non-decimal literals today; at the next milestone that touches bit manipulation it is **≥44**, and **0** of them are a decimal spelling of a mask | `grep -coE "0x[0-9a-fA-F_]+\|0b[01_]+\|0o[0-7_]+"` over `examples/`, `library/`, `tests/golden/run/` | next milestone touching masks |
//! | `2768` | `int` deleted, `i64` everywhere | `grep -c "\bint\b"` over every `.hero` in the repository stays **0** while `i64` stays **≥374**, and no diagnostic transcript in `tests/golden/` names a width the author did not write | two greps and the golden corpus, both live | every milestone close |
//! | `2959` | the eight widths | the corpus holds **50** narrow-width annotations today; **≥40 of them are at an FFI boundary**, and removing the widths would make each a silent truncation that `ffi_parameter_type` now refuses — countable by deleting the widths in a scratch tree and counting the diagnostics that stop firing | `heroes check` plus the `ffi_parameter_type` class built at M-binding-fidelity, which is the arm that did not exist when the row was written | M-ffi-ladder rung 5 |
//!
//! **What makes `2959`'s the strongest of the four**: its original prediction
//! named metric 2 because, on the day it was registered, nothing else could see
//! the widths at all. `ffi_parameter_type` can — it exists because of them — so the
//! re-registration is not a rewording, it is the first time the question has an
//! instrument. That is the shape R1's second half asks for, arriving on its own.
//!
//! The two scored rows are in `docs/measurements/007`, and both found something
//! the row itself did not predict. `2627` bought a reader who avoids `+` in a
//! loop and reaches for `push` in one instead — the clause names `push`'s copy
//! and the reader did it anyway, because with no lambda and no `repeat` the
//! document leaves no other way to build the array `join` needs. `2675` holds
//! because **no operator in `heroes mutate` makes the mistake it is about**: a
//! prediction can name a live instrument and still be uncollectable if the
//! instrument has no arm for the question.

/// The spec's measured size, `max` over both vendored instruments — the binding
/// number, never an estimate.
///
/// Every change to this constant belongs in the same commit as the spec change
/// that caused it, with the delta and its justification in the commit body.
///
/// **THE LEDGER IS NOT HERE ANY MORE.** Its 38 rows — one per spec amendment,
/// each naming the delta and what paid for it — moved verbatim to
/// `docs/measurements/010-spec-budget-ledger.md` on 2026-08-19, ahead of this
/// crate's archival (panel 086 R1). What enforces the number is
/// `tests/harness/suite_spec.hero`, which re-derives it with `heroes measure` on
/// every run and checks the record's newest row and row count against its own pin
/// (R2 — the agreement this file only ever had by adjacency).
///
/// The forwarding line is CPython's practice rather than an invention:
/// `Misc/stable_abi.toml` opens by naming `PC/python3dll.c` and `PC/python3.def`
/// as its own previous homes, so a reader who arrives at the old address is sent
/// on instead of reading a fossil (panel 086's historian).
///
/// The constant below stays because this crate's own `cfg(test)` checks read it,
/// and it is frozen history the moment `crates/` moves.
pub const SPEC_TOKENS: usize = 3512;

/// The reserved-word registry's own size, gated separately — **not** part of
/// §1.6's budget, and that is the ruling rather than an omission (panel 035 D).
///
/// §1.6 says the spec "is **the prompt**", singular, and
/// `harness/prompts/first-try.md` makes it operational: the context is the
/// template plus `spec/heroes-spec.md` plus one task. The registry is §4.17's
/// *compiler output*, delivered at the moment of the mistake; it can never be in
/// a prompt, so it cannot spend a prompt's budget.
///
/// It gets a number anyway, because it grew **+85 across eight milestones with
/// no commit ever naming a delta** — 831 at M-day-zero, 916 today — for the plain reason
/// that `heroes measure` defaults to the other file. Gate it, do not merge it.
pub const RESERVED_WORDS_TOKENS: usize = 916;

/// The ceiling panel 024 set, asserted at **compile time**: `SPEC_TOKENS` is a
/// constant, so a runtime `assert!` on it is optimised out and tests nothing.
/// Raising either number is a panel, and this line is what makes forgetting one
/// of them a build failure rather than a green suite.
const _: () = assert!(SPEC_TOKENS < 4096, "the spec is over panel 024's ceiling");

#[cfg(test)]
mod tests {
    use super::*;
    use crate::measure::{measure, spec_path, vendor_dir};

    /// The gate. It fails on any spec change, which is the point: the number
    /// below it is a record of every amendment this project has made.
    #[test]
    fn the_reserved_word_registry_is_gated_too() {
        let path = spec_path().parent().expect("spec/").join("reserved-words.md");
        let text = std::fs::read_to_string(&path).expect("the registry must exist");
        let measured = measure(&text, &vendor_dir()).expect("the tables load");
        assert_eq!(
            measured.max(),
            RESERVED_WORDS_TOKENS,
            "the registry measures {} and the record says {RESERVED_WORDS_TOKENS}. It is \
             not in §1.6's budget (panel 035 D: the spec is *the prompt*, and compiler \
             output can never be in one) — but it grew +85 across eight milestones with \
             no commit naming a delta, because `heroes measure` defaults to the other \
             file. Write the new number down.",
            measured.max()
        );
    }

    #[test]
    fn the_spec_measures_what_the_record_says_it_measures() {
        let text = std::fs::read_to_string(spec_path()).expect("the spec must exist");
        let measured = measure(&text, &vendor_dir()).expect("the tables load");
        assert_eq!(
            measured.max(),
            SPEC_TOKENS,
            "the spec measures {} and the record says {SPEC_TOKENS} — a delta of {}. \
             If the change is intended, write the new number into `SPEC_TOKENS` in the \
             SAME commit, and put the delta plus its payment in the commit body: either \
             a named removal, or a registered falsifiable prediction that names BOTH the \
             instrument that will score it and the milestone at which it is scored — and \
             the instrument has to exist today (panel 012, as amended by panel 046). A \
             prediction naming an instrument nobody has built is an observation, not a \
             payment, and six rows of this ledger were bought with one. If it is not \
             intended, the spec grew by accident, which is the case this test exists for.",
            measured.max(),
            measured.max() as i64 - SPEC_TOKENS as i64,
        );
    }

    /// The one thing §1.6 says must survive every raise: the headroom is real, not
    /// notional. Panel 024's own arithmetic put the justified total at 3181.
    #[test]
    fn the_headroom_the_remaining_mortgages_need_is_still_there() {
        // The floor for file I/O, `args()` and `exit(code)` — panel 030 R3, which
        // refuted "they are plain externs" by compiling it. The floor belongs to the
        // FFI, not to the milestone that pays it, so the name says so.
        let ffi_floor = 60;
        assert!(
            SPEC_TOKENS + ffi_floor < 4096,
            "modules plus the FFI floor is {} against 4096",
            SPEC_TOKENS + ffi_floor
        );
    }
}
