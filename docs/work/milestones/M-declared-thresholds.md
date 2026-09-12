# M-declared-thresholds — a threshold says which way it may move, and something notices when it does not


**Author instruction 2026-09-12**: analyse every number in this repository that
acts as a ceiling, say which of them make sense and which should be widened,
because sessions keep hitting their heads on them — then the same question asked
of the floors. (The words were the author's own, in Italian; CLAUDE.md §11 asks
for what they MEANT, in English, and leaves the original in the git history.)

**No language form, no built-in, no diagnostic.** What it touches is the
instruments and the contract, so Principle 0 binds nothing here; what binds it is
CLAUDE.md § Verification and § RUN IT.

## What it delivers

A threshold in this repository states **which direction it may move** and carries
**an instrument that notices when it should have moved and did not**. Today three
constants of twenty-five do that — `VERDICT_FLOOR` (*"moves DOWN with a line
saying which sitting moved it, never up"*), `clang_floor.hero`'s `FLOOR` (chases
the CI's oldest leg, zero slack by construction) and `suite_canonical.hero`'s
bracket (`assert LEAST < 371 && LEAST > 300`). The rest are bare numbers.

## The measurement that opened it, taken at `7d31838a`

**Ceilings sit at the value measured the day they were written, so the first
legitimate addition hits the wall.**

| ceiling | value | measured 2026-09-12 | room |
|---|---|---|---|
| `CONTRACT_CEILING`, CLAUDE.md | 5500 vendored | **5491** | **9 tokens** |
| `WHERE_CEILING`, § Where we are | 15 lines | **15** | **0 lines** |
| spec | 8192 real | 7531 (+60 FFI) | 601 |
| per file | 300 `code_lines` + 18 `DECIDED` rows | max 1870 | a ratchet |

**Floors sit at the value measured the day they were written too, and because
the tree grows they drift away from underneath: they stop catching, in silence,
with nothing ever going red.**

| floor | value | measured 2026-09-12 | slack |
|---|---|---|---|
| `LEAST_WALKS` (`suite_order`) | 10 | ~30 | +200% |
| `LEAST_MARKS` (`suite_order`) | 14 | 23 | +64% |
| `LEAST` (`suite_emission`) | 142 | 225 | +58% |
| `LEAST` (`suite_canonical`) | 340 | 521 | +53% |
| `LEAST` (`suite_layout`) | 140 | 202 | +44% |
| `least` check (`suite_golden`) | 65 | 112 | +42% |
| `LEAST` (`suite_lines`) | 87 | 111 | +28% |
| `LEAST` (`suite_corpus`) · `LEARNING_FLOOR` | 44 · 300 | 54 · 368 | +23% |
| keywords · operators · foreign · moved | 21 · 18 · 23 · 10 | unchanged | 0, sound |

**NOT MEASURED and said so**: `LEAST` in `suite_annotations.hero` (the criterion
sums extracted markers *and* diagnostics produced by running the compiler, so it
is machine-dependent and no grep replicates it) and the second threshold in
`suite_lines.hero`.

## The proof that a floor stops working, rather than the argument that it might

`suite_canonical`'s floor was written for a defect its own comment records: the
first draft used the flat walk and found **325 files of 371**, 88%, and the floor
at 340 caught it. The tree is **521** files now, so the same fault — a walk
reaching 88% — would find **458**, far above 340. **That floor would not catch
today the defect it was written for.**

This is the class `.claude/rules/module-shape.md` already names: *a fact about
the value cannot expire; a premise about the world expires silently, and the
comment justifying it goes on reading as correct*. A floor **is** a premise about
the world — *"there are 127 cases today"* — and that rule says what is owed when
one is unavoidable: **a test that fires when it dies**. None of the twenty has
one. It also corrects a distinction that looked sound: sanity floors decay too,
because what they must catch is a *fraction* of the tree, and a fraction of a
bigger tree is a bigger number.

## What is done, and what is open

Landed in this milestone's own steps: the site's ceiling derived rather than
typed, `suite_layout`'s ceiling table proved from the table instead of two
incised numbers, `WHERE_CEILING` 15 → 32 with its tautological assert removed,
and two dead comments repaired.

*******************************************************************************
**OPEN: 3**

- [ ] **M-declared-thresholds** | the contract is judged on the vendored tokenisers while §1.6 has ruled those are not the reader's — move CLAUDE.md to `claude-opus-5` through `count_tokens` and set the ceiling on that scale | `tests/harness/suite_spec.hero` `CONTRACT_CEILING`, `selfhost/measure/pinned.hero`, `selfhost/cli/measure.hero`

    **Origin:** author instruction 2026-09-12, to consider taking CLAUDE.md to
    8K as well. Changing the instrument without changing the number is a TIGHTENING,
    not a neutral move: the spec's real/vendored ratio is 7531/5655, so the
    contract's 5491 vendored is likely near 7300 real and 5500 would be a breach
    the day it landed. 8192 is the instructed number; it is confirmed against the
    measurement first, because a ceiling born violated is the error of 1500 and
    4096 both. Two constants at one value, never one shared constant, or the next
    spec raise moves the contract in silence. `run_refresh` today refuses every
    file but the spec, with a test pinning that refusal — extending it to two
    documents is the panel's question, not a quiet edit.
- [ ] **M-declared-thresholds** | the raise takes away the contract's only alarm, so the delta gate design.md §1.6 keeps queued should land with it | `docs/design/design.md:355`, `tests/harness/suite_spec.hero`

    **Origin:** 2026-09-12. The spec's ceiling has a forcing function behind it —
    every addition owes a named removal or a registered prediction. The contract
    has none: it is amended by author instruction and is deliberately a ceiling
    and not a pin, so today its 9 tokens of room are what does the work of a
    payment rule. At 8192 that alarm rings ~1700 tokens later. A delta gate is
    level-independent, so it survives every future raise, and it watches the
    movement instead of the level.
- [ ] **M-declared-thresholds** | nothing notices a floor that has stopped catching; a ratchet is owed in the direction opposite `DECIDED`'s | `tests/harness/suite_layout.hero` `DECIDED`, and the floors listed above

    **Origin:** 2026-09-12, from the census in this file. Raising the twenty
    numbers today repairs them until the tree grows again — the shape `golden.rs`
    already paid for, *"a floor at 60, which was a number from when there were
    sixty"*. The open design question is the REGISTER: a table like `DECIDED`, or
    discovery by reading the constants, or each floor declaring the function that
    re-measures it beside itself. That choice decides whether the mechanism
    survives or becomes another list nobody updates. `VERDICT_FLOOR`, `FFI_FLOOR`
    and `clang_floor.hero`'s `FLOOR` are excluded and the reasons are in the plan:
    they are an applicability threshold, a token reserve and a minimum version,
    none of them a count.
*******************************************************************************
