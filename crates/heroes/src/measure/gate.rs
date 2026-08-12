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

/// The spec's measured size, `max` over both vendored instruments — the binding
/// number, never an estimate.
///
/// Every change to this constant belongs in the same commit as the spec change
/// that caused it, with the delta and its justification in the commit body.
///
/// | value | when | what moved it |
/// |---|---|---|
/// | 2231 | panel 023 | the `[T]` clause §1.6 says must survive a raise |
/// | 2363 | M6 | `range`'s labels, the tier phrase, panel 028's prelude |
/// | 2434 | panel 031 | modules: `use`, qualification, the transitivity rule |
/// | 2422 | panel 035 | the thirteen silences, closed **net −12**: one line deleted, four added |
pub const SPEC_TOKENS: usize = 2422;

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
/// no commit ever naming a delta** — 831 at M0, 916 today — for the plain reason
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
             SAME commit, and put the delta plus either a named removal or a registered \
             falsifiable prediction in the commit body (panel 012). If it is not \
             intended, the spec grew by accident, which is the case this test exists for.",
            measured.max(),
            measured.max() as i64 - SPEC_TOKENS as i64,
        );
    }

    /// The one thing §1.6 says must survive every raise: the headroom is real, not
    /// notional. Panel 024's own arithmetic put the justified total at 3181.
    #[test]
    fn the_headroom_the_remaining_mortgages_need_is_still_there() {
        // M7's floor for file I/O, `args()` and `exit(code)` — panel 030 R3, which
        // refuted "they are plain externs" by compiling it.
        let m7_floor = 60;
        assert!(
            SPEC_TOKENS + m7_floor < 4096,
            "modules plus M7's floor is {} against 4096",
            SPEC_TOKENS + m7_floor
        );
    }
}
