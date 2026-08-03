# Panel 000 — didactics review (verdict: APPROVE-WITH-CHANGES)

## Top objections (all adopted in revision 2)

**1. The prediction was cued, so it was not retrieval practice.** Revision 1's
journal had the assistant write "Expected output: <the shape>" in the same
section, above the author's blank — recognition, not recall (the fluency
illusion), and unfalsifiable ("the shape" cannot diverge from anything).
**Fix:** the author's prediction lives in a separate file
(`docs/journal/NNN-prediction.md`), countable and falsifiable, committed
BEFORE any src/ change; the assistant states goal + input only.

**2. Nobody made the author produce anything.** Across M0–M8 the author wrote
zero compiler code and never diagnosed first — yet the generation effect is
the largest lever available. **Fix:** one author-written function per
milestone (assistant reviews); author-first diagnosis (assistant posts the raw
symptom and stops until the author writes a hypothesis and names the file).

**3. M3 was four lessons and the conceptual cliff.** Resolver + bidirectional
checking + variants/exhaustiveness + diagnostics-as-product in one milestone.
**Fix:** M3a–M3d, each with its own prediction and runnable artifact.

**4. The M0→M5 spike loop was weeks wide.** **Fix:** the M0 comparison stays
as motivation, but every lowering/emission step gets a same-day 5-line
hand-written expected output, diffed immediately.

**5. Golden authorship would degenerate into rubber-stamping.** **Fix:** the
author writes exactly 5 *adversarial* cases per milestone; bulk regression
cases are assistant-written and labelled; UPDATE_GOLDEN forbidden in `check/`.

## The three prerequisite concepts (adopted)

1. **Checking vs synthesis (bidirectional)** — lands days before M3, with a
   paper exercise (five expressions, two columns ⇐/⇒).
2. **Core vs sugar / elaboration** — before M4: the author hand-desugars
   `for x in xs`, `?`, and one UFCS chain first.
3. **Basic blocks** — spike 02 in M0: the author draws the CFG before seeing
   the goto version.

Everything else stays just-in-time, triggered by a missed prediction.

## Protocol upgrades (adopted)

Explain-it-back (≤10 lines from memory, then diffed against the code) replaces
the "annotated code" journal section; 3 spaced questions from ≥2 steps back at
each step's close; a mutation drill per milestone (bug injected into committed
*compiler* code, author diagnoses from the failing golden alone); a tag
exit-quiz (author re-implements one small function from scratch on a throwaway
branch).

## On the fixpoint

Right summit, wrong final lesson if reached unprepared: the Rust→Heroes port
is transcription (low-generative), and `diff` failing on nondeterminism should
not be the author's first encounter with it. **Fix (adopted):** byte-identical
double-emit is a golden-tested invariant from M5a; the fixpoint compares
generated C, not binaries.
