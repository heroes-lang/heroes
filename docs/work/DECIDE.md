# DECIDE — the decisions the compiler is waiting on

Read by **`/decide`**. Every item here asks **what should be true**, and until it
is answered the compiler goes on behaving some way by default. That default is
the cost of leaving an item open, so each item names it.

**Only open items live here.** The moment one is answered it is ticked with the
verdict written into it and moved to `docs/work/DONE.md`. A file that keeps its
own closed items stops being a list of what is owed — this one held **138 ticked
items and zero open ones** on 2026-08-26, under eighteen headings still titled
`## Open`, which is why the rule now lives in `/decide` itself instead of in
prose about `/decide`.

**One notation: `- [ ]`.** No section headings, no prose sections, no
strikethrough. A finding written as a bare bullet is invisible to every count in
this project, and nine of them sat in exactly that shape under two panel
headings here until 2026-08-26 — **five already closed** and two of those
measurably stale, while the file reported itself empty.

Two rules bind this file, both learned the hard way:

- **Verify before asking.** An entry is a claim from the day it was written, and
  entries outlive their causes. Asking the author about a settled question is the
  one cost this list cannot pay. Of the nine findings recovered on 2026-08-26,
  five were closed and were ticked with what closed them rather than asked.
- **Rank by what it blocks**, never by age, and say the blocker in the question.

Format: `- [ ] <origin> | <what> | <where to look> | <why it matters>`

- [ ] **Should `heroes mutate` over `examples/` run in CI, and on which leg?** | M-corpus-depth step 0, 2026-09-03 — the first `mutate` over `examples/` since 2026-08-13 found it **refused at exit 2**, broken since 2026-09-02 (`c12de74`), and no instrument had said so | **The metric the thesis rests on runs nowhere automatically.** `suite_surface.hero:281` runs `mutate examples/gallery` (twelve single-file programs) on every leg; nothing runs it over the corpus the score is defined on, so a change that stops it reading its own corpus — as `examples/shapes/` did for a day — is seen only when somebody types the command. **The default while this is open**: the score is taken by hand, at milestone closes, and the record shows how that goes — 45 files on 2026-08-13, then nothing for three weeks while the corpus grew to 78. **The price, measured** (`docs/measurements/014-mutate-over-thirty-five.md`): the whole-directory run is refused in 2.10 s today; the 36 per-directory runs that stand in for it took the wall time recorded there, on the Mac. **Three shapes, one recommended.** (a) **A row in `suite_surface.hero` that runs `mutate examples/` and asserts exit 0** — cheap, every leg, and it catches exactly the failure step 0 met (a corpus the tool cannot read) without pinning a rate; it is the shape of every other surface row and needs no CI change. Recommended: the number that must not silently break is *"the tool reads its corpus"*, and a rate pinned in CI would go red on every program added, which is the corpus's job. (b) The full score on the **tag-triggered** run only, printed into the job log — Nim's out-of-band pattern (`important_packages` sharded, 60 min, `fail-fast: false`) — for the number without the gate. (c) Both. **What is NOT proposed**: pinning the kill rate as a CI assertion; the rate is a measurement (Part 11 metric 3) and moves by design | .github/workflows/ci.yml:464-470 · tests/harness/suite_surface.hero:281 · selfhost/cli/mutate.hero · docs/measurements/014-mutate-over-thirty-five.md | a measurement nobody is obliged to run is write-only, and this one was for three weeks
