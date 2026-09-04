# 015 — The exit sweep's own temporaries, removed: what half a frame buys

Date: 2026-09-04 · M-corpus-depth, the step after panel 106 ·
**the repair the sitting adopted instead of the proposal it was convened for.**

## Provenance

| what | value |
|---|---|
| compiler before | `5145169` (the commit that landed `nbody/` and `spectral/`), rebuilt from its own seed |
| compiler after | the same tree plus `decref_slot`, rebuilt through the fixpoint |
| machine | the author's Mac, arm64, Apple clang 21.0.0; the frame numbers are `clang -c -O0 -fstack-usage` over the emitted C |
| the program measured | `examples/interpreter/` — eleven modules, the corpus's recursive-descent grammar and tree-walking evaluator |
| instruments | `-fstack-usage`; a binary search over nesting depth, one probe per configuration; `wc -l`; `cmp` for the fixpoint |

## What changed, in one sentence

The ownership pass used to release a slot by **loading it into a fresh
temporary and decrefing that**; it now names the slot. `selfhost/ir.hero`
gains one case, `ir/own.hero` mints it, `emit/inst.hero` emits it, and
`ir/uses.hero` reports it so the slot stays counted as read. Panel 106's own
words for why the old form existed, quoted from the file that is now
withdrawing them: *"releasing a slot is a load followed by a decref, which
keeps one form instead of two and makes the dominance check cover it free."*
Both halves were true. The sitting measured the price.

## The frames

| | before | after | change |
|---|---|---|---|
| interpreter, all 342 functions | 639,792 bytes | **294,128** | **−54.0%** |
| `h_runexpr_combined`, the worst | 112,704 | **17,296** | −84.7% |
| one nesting level of the grammar | 36,896 | **20,608** | −44.1% |
| — `synexpr.compared` | 9,680 | 4,720 | |
| — `synexpr.named` | 10,288 | 4,656 | |
| — `synexpr.primary` | 7,232 | 6,096 | |
| — `synexpr.unary` | 6,800 | 3,328 | |
| — `synexpr.grouped` | 2,896 | 1,808 | |
| **the compiler's own** 2,911 functions | 6,563,376 | **4,227,600** | **−35.6%** |

## The ceilings — the number the whole exercise was for

Binary-searched, one probe per configuration, all of them stopping with
`panic: stack exhausted` and the guard naming the function (panel 104):

| | before | after |
|---|---|---|
| nesting depth, `--sanitize` | 104 | **166** |
| nesting depth, `-O0` | 191 | **314** |
| nesting depth, `-O2` | 349 | **585** |
| script recursion, `--sanitize` | 40 | **76** |
| script recursion, `-O0` | 50 | **107** |
| script recursion, `-O2` | 80 | **239** |

**The milestone asked for 500 and 500 is still out of reach**, and this is the
half of the result worth stating first. `--sanitize` tops out at 166, the
corpus runs every program in all three configurations, and the rule
`examples/interpreter/` has followed since it was written is half the tightest
measured ceiling — so its nesting case moves **60 → 83** and its evaluator's
own guard moves **24 → 38**. Both numbers are re-derived from the measurement
above rather than chosen, and both are written into the programs beside the
table that produced them. A case that passed at 500 on two configurations and
was flaky on the third would be worse than the 83 that holds everywhere.

## The emitted C, and the seed

| | before | after | change |
|---|---|---|---|
| `examples/interpreter/` emitted | 69,052 lines | **54,610** | −20.9% |
| `seed/heroes.c` | 846,804 lines | **719,955** | **−15.0%** |
| blessed traces that changed | — | **135 of 189** | |
| zeroed declarations in one trace (`examples-query-main.c`) | 2,968 | **1,770** | −40.4% |

## Correctness, all run rather than argued

- **The fixpoint holds byte for byte.** The repaired compiler emitted
  `seed/heroes.c`; clang built a compiler from it; that compiler re-emitted the
  seed and `cmp` is silent.
- The compiler's own tests: **560, all passed**, before and after.
- Every blessed trace re-blessed with `UPDATE_EMISSION=1` and the diff read
  (§9): declarations removed, `#line` renumbered, and the release lines change
  from `h_..._release(&t203)` to `h_..._release(&h3_s0)`. **112 `_release(&t…)`
  remain in that trace on purpose** — the store expansion's `old` temporary,
  which panel 106 filed rather than fixed because the store genuinely sits
  between the load and the decref.
- `examples/interpreter/`: **57 tests, all passed** with both limits raised.
- Three files needed their `suite_layout.hero` ceilings raised, with the reason
  written into the table: `ir.hero` new at 310, `emit/inst.hero` 324 → 350,
  `ir/print.hero` 460 → 470. In all three the code is small and the comment is
  the argument, because a file that asserted a rule for a year is the file that
  has to withdraw it in writing.

## Predictions scored

| origin | prediction | result |
|---|---|---|
| panel 106, compiler-engineer | with `decref_place` landed, the interpreter's `-O0` ceiling is **≥ 300** and `--emit-c` emits **≤ 60,000 lines** (it measured 311 and 56,148 in its own copy) | **HOLDS**, and on the better side of both: **314** and **54,610** |
| panel 106, coordinator | the interpreter's own nesting case moves from 60 to **at least three times** it, measured in all three configurations | **FALSE.** It moves to 83, which is 1.4×. The ceilings roughly doubled and the case follows the tightest configuration, not the mean — a prediction written from the `-O0` number would have been right and the one that was written was not |

The second row is the useful one. It was registered by the seat that wrote the
brief, and it was wrong for the reason briefs are usually wrong: it took the
number it had in mind (the `-O0` ceiling, which did roughly double) and stated
it about a case that is governed by the tightest configuration. The measurement
was never in doubt; the arithmetic between the measurement and the claim was.

## What is still owed, with its number

**The store's `old` temporary — 9.1% more of declared bytes** (panel 106's
measurement, not re-taken here). It needs a generated per-type
`assign(&slot, &value)` because the store sits between the load and the decref,
which is why the sitting filed it rather than folding it into this step.

**And defect 008 must be re-measured on the Windows box.** That defect is the
documented one-clang-line build producing a compiler that cannot check
`examples/query/main.hero` there — `panic: stack exhausted`, exit 127, against
CI's own line which carries `-Wl,/STACK:67108864`. The compiler's frames are
35.6% smaller than when that was measured, so the size at which Windows' default
1 MB stack stops being enough has moved a long way out. Whether the plain line
now works there decides whether the defect's repair is a permanent Windows arm
in § Commands or a note about a version, and nobody should guess it.
