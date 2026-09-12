- [x] **M-interpolated-strings** | what a printed float guarantees, which the spec does not say — panel 121 R6 lands it WITH the interpolation clause, at +9 | `spec:184-185` · `runtime/parts/f64.c:26-37` · `docs/panel/021`, `027`

    **Origin:** `/decide` 2026-09-03, author instruction — the robust form is a
    stated guarantee, not a silence. Moved ahead of the packages late on
    2026-09-03, when every ruling on the language did, `DESIGN-LOG.md:539`. The
    four row numbers this item used to carry left on 2026-09-04, because
    CLAUDE.md §14 puts the order in the chain table and nowhere else — and four
    rows entered that day, which moved every one of them. A question the sitting
    MUST answer, not a maybe.

    **The spec says only *"A float prints a point or exponent"*; the runtime
    guarantees round-trip and promises nothing about shortest.** The sitting
    answers with a sentence at `spec:184-185` — what a printed float guarantees:
    it reads back as the same value — or with a design.md Part 6 row carrying
    its falsifier (CLAUDE.md §12). Measured 2026-09-03 with a C probe against
    `runtime.c`: `5e-324` → `5e-324`, `1e-323` → `1e-323`,
    `1.9999999999999998e23` → `2e+23`, `0.1` → `0.1`; `5.0e-324` is not
    writable in Heroes (`exponent_literal`), so any brief says so or a seat will
    try. `runtime/parts/f64.c:26`'s stale example was corrected at `830e416`.

    **Where to look also:** `DESIGN-LOG:92`, `:100`.
    **Why it matters:** a spec that is silent where the runtime is specific is
    §12's own case.

    **LANDED 2026-09-09 at M-interpolated-strings step 1**, with the interpolation
    clause, as panel 121 R6 said it would: `spec:193` now reads *and reads back as
    the same value*, at **+10** real tokens against R6's +9. The guarantee is the
    runtime's own (`runtime/parts/f64.c`, round-trip and nothing about shortest),
    stated where a reader of the spec meets it.
