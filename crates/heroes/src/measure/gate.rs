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
/// | 2363 | M-generics-library | `range`'s labels, the tier phrase, panel 028's prelude |
/// | 2434 | panel 031 | modules: `use`, qualification, the transitivity rule |
/// | 2422 | panel 035 | the thirteen silences, closed **net −12**: one line deleted, four added |
/// | 2560 | panel 036 | the FFI group and the program's edges, **net +138** against two removals worth −31 |
/// | 2588 | panel 038 | a group's `constant`, **+28** with no removal available: the two the record still listed had already been spent by `6a58d47`, so this is panel 012's *other* branch — a registered prediction (the llm-ergonomist's first-try rates, scored at M-program-corpus). +1 of the 28 is the `@out: ptr` repair, which fixed an example that ran to completion doing nothing |
/// | 2627 | panel 037 | the cost of building a string or an array, **+39**, panel 012's registered-prediction branch again — the prediction is written into `docs/panel/037-array-growth.md` § Ratification and scored at M-program-corpus. The clause the ergonomist asked for was the `join` half at +23; the extra 16 buy `push`'s copy, without which the sentence would send a reader to build the `[str]` quadratically instead of the `str` |
/// | 2675 | author decision, `docs/panel/040` | the bitwise set becomes real, **+48** — a row in the operator fence and five levels in the precedence chain. Panel 012's registered-prediction branch: no removal was available, because panel 036 had already deleted the spec's own sentence naming the six reserved spellings. The prediction is in the panel file and scored at the next FFI rung |
/// | 2745 | author decision, `docs/panel/041` ratified | the four bases and the `_` separator, **+70** in three lines — `0x1f` `0o37` `0b11111` `31`, a leading zero refused, and the value reading stated so a reader does not have to guess whether `0xffffffffffffffff` is `-1`. Panel 012's registered-prediction branch, and the **removal was refused rather than unavailable**: the spec-warden priced spec lines 152–153 at −39 as stray performance advice, and those lines are `2627`'s own entry two rows above — panel 037 added them deliberately on the llm-ergonomist's ask, and their prediction is still unscored. Deleting them would have spent another sitting's live measurement to fund this one. The prediction registered instead is in `docs/panel/041` § Predictions scored, clause 7's live half: **≥25% of unprompted attempts at a mask or a copied header constant reach for a base other than decimal**, scored at the first metric-2 harness run, and under 10% the notation is a road nobody takes |
/// | 2768 | author decision, `docs/panel/042` ratified | **`int` is deleted**, `i64` everywhere, **+23** — and the delta is entirely the tokeniser rather than the language: cl100k spends one token on ` int` and two on `i64`, and the document names an integer 21 times. No rule was added, no rule removed. The author's ground is §1.1's: `int` is a word carrying forty years of conflicting widths and a reader must know the platform to know what it means, while `i64` is ambiguous to nobody — and the 545 occurrences across 168 `.hero` files that this cost to migrate were 545 places carrying a number nobody could read off the name. Panel 012's registered-prediction branch, and the prediction is `docs/panel/042`'s #4: integer-width mismatch diagnostics are **≤5%** of first-try failures, above **15%** the widths are a net §1.2 loss |
/// | 2959 | author decision, `docs/panel/042` ratified | **the eight widths**, `+191`: two table rows in place of one, the no-implicit-conversion rule extended to widths, the `fit_<width>` family, the literal's context rule with its example, and "overflow aborts at every width". That is the whole of design.md:833's *"most expensive spec item that exists"* — and it came in at less than the +189 first drafted because **half of it was already paid**: §4.3 forbids implicit conversions outright, so there is no promotion lattice to specify, which is the part of C's rules that does the damage. Panel 012's registered-prediction branch, no removal available and none proposed after the one on offer turned out to be another sitting's unscored deliverable (see the 2745 row). The predictions are `docs/panel/042` #4 and #5 |
/// | 2956 | same sitting, `docs/panel/042` | `s[i]` becomes a `u8` and a widening becomes infallible, **−3**: *"yields an `i64` in 0..255 (a byte)"* was a type and a range and a gloss where *"yields a `u8`"* is a type, and the conversion clause gains six words. The rule the author gave is *"the precise type, not the widest one that holds it"*, and the spec got shorter by following it |
pub const SPEC_TOKENS: usize = 2956;

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
