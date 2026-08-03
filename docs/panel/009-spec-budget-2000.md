# Panel 009 — the spec budget rises to 2000 tokens (retro-record)

Date: 2026-08-04. **Decision already taken by the author**; this session
records real consequences and proposes governance, per the retro-record
rule (`/panel` step 1: record objections, do not stage dissent). Two judges
convened — the two whose mandate the number defines: the spec-warden (whose
veto threshold *is* the budget) and the llm-ergonomist (for whom the spec is
the prompt). The other three judges have no standing on a budget figure.

## The decision

design.md §1.6's budget rises from **~1500 to 2000 tokens**. Applied to
§1.6, §"The name" (L25), Part 10 (L1817), CLAUDE.md, both judge briefs, and
the site. **spec/heroes-spec.md is NOT touched** — v0 stays frozen as the
pre-amendment baseline (standing rule), so the raise changes the *rule*, not
the artifact.

## Verdicts

| Judge | Verdict | Cost/delta | Prediction | Condition |
|---|---|---|---|---|
| spec-warden | retro-record — consequences + governance | the raise buys +504 est. tokens on the heuristic, but the estimator spread on the *same unchanged file* is 1496 / 1646 / 1829 / 2058: **the raise is the same size as the measurement uncertainty that justified it**. What it costs is not tokens — it retires "every addition requires a removal", the only mechanism that forced a named cut | (1) v0 measures 1650–1900 at M3; (2) the **closure-complete** spec (v1 + §1.0's mortgage + the three deferred sentences) measures ≥1950 — the 500 is consumed in full by debt booked before it was granted, leaving <50 for anything new | measured-only · soft 1500 / hard 2000 · the 500 is a pre-allocated purse, not headroom · no second raise without a measurement |
| llm-ergonomist | retro-record — consequences | under full-resend accounting, +500 recurring tokens **cannot** repay themselves in round-trips if the first-try rate already exceeds ~77%; the raise is justified by the *silent-error* class, which sits outside that arithmetic entirely | spending the 500 on **under-specified corners** raises first-try ≥6pp and cuts compiles-but-wrong by ≥50% relative; spending it on an **error-message gallery** raises first-try ≤2pp and moves compiles-but-wrong <1pp | the harness must report a **compiles-but-wrong-output bucket** separate from compile errors — without it the prediction is untestable and the raise unjustified on this axis |

## Findings worth keeping

**1500 was never derived.** The warden quotes §1.6: no prompt-cost
arithmetic, no context economics, no comparison to other specs — except one
calibration, Wirth's Oberon report at *sixteen pages*, which supports a
number 5–8× larger, not smaller. 1500 was "about two pages": a round,
ergonomic proxy for one-person implementability. The proxy survives at 2000
(still far under Oberon); the phrase "about two pages" did not, and was
corrected in the same commit along with a stale "1400-token language" at
L823 and four claims on the site.

**The instruction predates the number.** design.md Part 10 has always read
"count it with a real tokeniser, not by estimation — the BPE vocabulary
contains arbitrary choices nobody predicts." The repo has been defending a
budget with a 1.33 tokens/word prose heuristic for two days, in explicit
contradiction of its own instruction. The warden notes for honesty that the
"~1500" inside *that very sentence* was changed to 2000 in this diff: the
instruction predates the raise, the number in it does not.

**Round-trips were never the argument.** The ergonomist's arithmetic: with
S spec tokens, a correction round re-sends everything (R ≈ S + 720), so at a
50% failure rate the raise breaks even only if failures drop 28 points, and
above a ~77% first-try rate **no** improvement can repay it. The escape
route is not caching — it is that a compiles-but-wrong program never
triggers a correction at all. Its cost is not 2200 tokens; it is a wrong
program shipped. **Tokens that close a silent corner are worth buying at
2000; tokens that prevent a loud compile error are not.** That sentence is
the allocation rule, and it is sharper than the budget it governs.

**An anti-pattern section is a confession.** The ergonomist, ranking what
the 500 could buy, put "do NOT write X" fourth of five: if X compiles, the
fix is the language, not the prompt; if X does not compile, the section buys
one round-trip at a recurring price. Last of five is an error-message
gallery — a stale, lossy copy of an artifact the compiler already delivers
free, exactly when needed (§4.17).

**A new silent corner, found in passing.** `shift(a @ n, b @ n)` — the same
variable passed twice as a mutable argument — compiles under both readings
of §4.8 and yields *different answers*: copy-in/copy-out says last-write
wins, a reference reading says the writes see each other. Both are plausible;
mainstream languages install the reference model. Recorded as an open
question (below); the ergonomist's proposed fix is not spec prose but a
compile error on repeated `@` arguments.

## Resolution — governance, adopted provisionally

The number is the author's; the rules that keep it meaningful are the
panel's. Adopted as put by the warden, `provisional — author ratification
pending` (queued in `docs/debrief/QUEUE.md`):

1. **Measured-only.** From M3 the budget is a measured number
   (`heroes measure`, Anthropic `count_tokens`). No estimate may authorise a
   spec commit; the 1.33 heuristic is retired as an authority. Where only an
   estimate exists, both bounds are stated and the **pessimistic** one binds.
2. **Soft 1500 / hard 2000.** Under 1500: §1.0 alone. Between 1500 and 2000:
   §1.0 **plus** a named removal **or** a pre-registered falsifiable
   error-prevention prediction, scored at Part 11. Over 2000 measured: veto.
3. **The 500 is a pre-allocated purse, not headroom.** Earmarked in order:
   (a) §1.0's mortgaged closure items (modules, file I/O, `args()`,
   `exit(code)`), (b) panel 008's escape line, 007-bis's layout sentence,
   003's `()` type-table row. Anything off that list needs a removal at any
   size. **"There is headroom" is never a reason.**
4. **Allocation rule** (ergonomist): spend on corners where a wrong guess is
   *silent*; do not spend on preventing errors the compiler already makes
   loud.
5. **No second raise without a measurement**, a panel, and a named
   alternative (cut features instead).

## Predictions to score

- warden: v0 measures 1650–1900 — **when `heroes measure` runs (M3)**;
  closure-complete spec ≥1950 — **at the M6 audit**.
- warden: v1 additions clear a 14pp rewrite-rate drop on their own task
  classes but move the flat 20-task aggregate <10pp — so Part 11 must report
  **class-weighted** numbers — **at the baseline**.
- ergonomist: corners +≥6pp first-try and −≥50% relative compiles-but-wrong;
  error gallery ≤2pp — **at the baseline, and only if the
  compiles-but-wrong bucket exists**.
