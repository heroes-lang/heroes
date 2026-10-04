- [ ] **M-thesis-harness** | what it depends on, and the author's decision on the seed tasks | `design.md` Part 11 · `docs/measurements/007` · `docs/panel/011`

    **Origin:** author instruction 2026-09-03, scheduled `DESIGN-LOG.md:539`.

    **Metrics 2 and 4 have never run** (`docs/measurements/007:24-25`;
    M-publication-gate's checklist), and the instrument that runs them is a
    Heroes program over HTTPS, so it waits for M-core-packages' `net/http`
    client (libcurl, step 11) and reads its API key from the environment, one of
    §10's three input classes. Part 11's protocol binds it whole: spec-only
    context, single turn, hashed frozen prompts, two gradings, Wilson intervals,
    one non-Anthropic model, provenance (spec sha, compiler sha, model id,
    prompt sha, suite sha). **Author decision 2026-09-03**: a small seed of
    tasks written by the author at this milestone's opening, the bulk still at
    M-guide-book as decided 2026-08-24 (the M-guide-book item above stands).
    Corpus material never enters the held-out set (CLAUDE.md §9).

    **Where to look also:** `design.md` Part 11 (`:2823-2860`) ·
    `harness/tasks/README.md`.
    **Why it matters:** a thesis with one of two factors audited is an opinion
    with a decimal point, which §12 forbids.

    **Re-verified 2026-09-10: STILL OPEN, and it points at a contradiction the
    ROADMAP now has with itself.** `harness/tasks/README.md` still says *"Status: 0
    tasks"* and still keys the bulk to M-guide-book. Part 11 is `design.md:3114-3166`,
    not `:2823-2860`. **And the dependency's number moved**: the `net/http` client is
    M-core-packages **step 5** after the 2026-09-10 reorder, while
    `docs/ROADMAP.md` § M-thesis-harness still says *"step 11"* — which is now the
    byte buffer. Whoever opens either milestone repairs that sentence to name the
    package rather than a number, on §14's own rule.
