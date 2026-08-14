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
/// | 2963 | `docs/panel/043` | the cost paragraph restated declaratively and `join`'s shape given, **+7 net** — and this is a **removal disguised as an addition**. Out: the imperative *"Build a long string with `join`, not repeated `+`"*, and *"a long array in chunks"*, which **advised the impossible** (`[i64] + [i64]` is `bad_operand`; the language has no array concatenation and never had). In: `join(xs, sep)`'s shape, which panel 043's llm-ergonomist had to guess in **both** blind variants and named as the paragraph's real payload. The rule the sitting established: the spec may state **what an operation costs** — design.md §4.10:1327 already models it, *"Price, declared: mutating a shared array copies it, O(n)"* — and may **not** instruct a reader to prefer a construct for speed, because that binds the author on a premise about the implementation, which is CLAUDE.md §11's world-premise. C++17 removed `register` for being the second kind and broke real builds |
/// | 2974 | author decision, `docs/panel/045` | **`fit_<width>` becomes `to_<width>`**, +11, and the delta is bought rather than spent: the family joins the scheme panel 017 established when it renamed `.str()` to `to_str` *"so the three conversions share one scheme and a model can derive the third from the two the spec already lists"*. `fit_` broke exactly that — a model knowing `to_i64`, `to_f64` and `to_str` cannot derive `fit_u8` — and panel 044's blind judge proved it by reaching for `to_i64(text[i])` and noting its own miss. `to_i64` absorbs the old `f64` conversion and becomes fallible with the rest, because one name cannot carry two failure models; the clause now states the rule (*the name says whether it can fail*) instead of listing two families, which is what the +11 buys |
/// | 2983 | panel 048 | **a false sentence repaired**, +9 and the cheapest kind of amendment there is. § FFI said *"A group names its header and its library"* — and **13 of 20 `extern` groups in this repository name no library**, including `library/source.hero`'s `extern "hero_os.h"`, which is the **closure list's only `extern` group**. Read under §12, the spec made the port's own library source illegal and the compiler buggy for accepting it. Now: *"A group names its header, and `link` a library when the symbols need one."* **Paid as §1.0 compiler-need, not with a prediction and not with a removal** — the warden's removal search over six candidates returned nothing, and four of the six are themselves compiler-need. Three judges found this sentence independently while judging something else entirely: the two clauses the sitting was convened about (+38 and +54) were both **refused**, and the only thing that landed in the spec is the repair of what was already there |
/// | 3106 | author decision, `docs/panel/050` | **`package`**, +123, and the author's instruction was to spare no expense: *"the FFI is central, spend the tokens needed, make it work fully."* Paid as **§1.0 compiler-need** on the strictest available reading — §4.19's own acceptance ladder had a rung that could not be climbed, measured four ways at panel 049. One spelling replaces a whole platform axis: `pkg-config` answers with frameworks on macOS and `-lGL -lX11 -lm -ldl` on Linux, so the +144 platform proposal panel 049 refused is subsumed at less cost and without putting a machine's name in a program. The clause names its allow-list in the spec because the allow-list *is* the feature's safety: Go shipped this idea without one and it became CVE-2018-6574 |
/// | 3140 | `docs/panel/051` | **the assertion checks results and never parameters**, +34. The sentence said *"clang checks every signature … so a wrong FFI type is a compile error"*, and two judges falsified it with running programs: `putchar(c: i64)` given `4294967361` prints `A` at exit 0, and a C `int narrow(int)` given `4294967301` prints `5` under this project's exact flags, silently. The half that is true is the result type, which `_Generic` checks; the half that is false is every parameter, and **267 of 643 bindable entry points across `sqlite3.h`, `curl/curl.h` and `raylib.h` — 42% — take one that is not 64 bits**. §12 says the compiler has the bug, so the repair is §1.0 compiler-need. The asymmetry the clause now states is **principled and was measured, not assumed**: a result may be wider than C's because an `unsigned int` always fits an `i64`, and a parameter may not be, because that narrowing is the one clang performs in silence. `examples/sqlite/` moved to the header's own widths in two edits and no shim; `examples/curl/` moved its parameters and **could not move its results**, because `CURLcode`'s compatible type is unsigned and `HERO_RET_I32` refuses it correctly. Panel 012's registered-prediction branch: the llm-ergonomist's first-try width rates, scored at the next ladder rung. The warden's `−8` removal is **not** spent here — it belongs to panel 037's live clause and funding this sitting from it would repeat what panel 041 refused |
/// | 3141 | `docs/panel/052` | **and sign**, +1 — the cheapest amendment in this table, and it exists because the compiler grew a check the sentence did not describe. Panel 051 wrote *"at the header's own width"* and `examples/curl/` immediately produced the other half: `curl_easy_setopt` takes `CURLoption` and `curl_easy_strerror` takes `CURLcode`, **two enums in one header whose compatible integer types have different signedness**, so `i32` is right for one parameter and wrong for the next. `-Werror=sign-conversion` catches it and `ffi_parameter_type` reports it as one class with the width, because the repair is one sentence. Paid as §1.0 compiler-need under §12: the compiler now refuses what the spec permitted |
pub const SPEC_TOKENS: usize = 3141;

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
