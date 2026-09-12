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
two dead comments repaired, the contract moved onto `claude-opus-5` at a ceiling
of 12288 with a +50 delta gate, and every counting floor checked from both sides
with eight re-based. **Closed 2026-09-12**, journal
[052](../../records/journal/052-declared-thresholds.md).

Carried forward: `suite_runtime`'s three floors, which are read in composite
conditions over one sweep.

## What is closed

All three items this milestone opened were closed by it, on 2026-09-12, and the
record is
`docs/records/done/2026-09-12-2310-m-declared-thresholds-three-items-closed-by-their-own-milestone.md`.
The contract moved to the reader's tokeniser at a ceiling of 12288; the delta
gate landed in the same commit and fired on that commit's own edit; and the
floors are checked from both sides, with eight re-based to today's count.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
