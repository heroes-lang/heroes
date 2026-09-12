# M-lsp-server — `heroes lsp`


**Scheduled, no warrant.** ~250 lines of JSON-RPC: diagnostics on save,
formatting, hover, documentSymbol. It blocks nothing and could land any time
after M-rich-diagnostics; it is here rather than earlier by the author's choice,
and M-vscode-extension is what consumes it.

**And the incremental frontend, since 2026-09-03.** The `SCHEDULED.md` item that
M-separate-compilation step 6 left open asked *which milestone does it*, and this
is the answer: a server that re-checks a program on every save cannot wait for
`heroes check` on the compiler's own source — about 8 s on 2026-08-26, after step
9 took it down from 88 — and the per-module build's warm 45.2 s against the fused
44.1 s says the emission half is architectural (panel 093 R4 puts the emitted
text in the cache key). The frontend half is the one an editor feels, so it lands
here; the emission half keeps its numbers in that item as its trigger.

*******************************************************************************
**OPEN: 1**

- [ ] **M-lsp-server** | the per-module build is slower, and the frontend is where the prize was | `docs/panel/093` · `selfhost/emit/unit.hero` · `selfhost/cli/units.hero`

    **Origin:** M-separate-compilation step 6, measured 2026-08-26. Its home
    since 2026-09-03, `DESIGN-LOG.md:539`: a server that re-checks on every save
    cannot wait for `heroes check` on the compiler's own source, so the
    incremental frontend is the tool's precondition rather than a build
    optimisation. This item was M-separate-compilation step 6, open, and it was
    the step's remaining question.

    **The per-module build is SLOWER than the fused one, and the reason is that
    the frontend is 83% of a build.** Measured with one binary on the compiler's
    own source, after the step's four repairs: fused `--emit-c` + clang + link
    **≈123 s** (frontend 88 s + fused emission 30 s + clang **4.3 s** on 788,406
    lines + link); per-module cold **261 s**, per-module fully warm — nothing
    edited — **250 s**. The cache works and is not the problem: the TU objects
    are reused (`ls -lT` shows the cold run's mtimes, 162 cache directories
    after two identical builds rather than 314), and what it saves is the
    **4.3 s** of clang. What it costs is 157 per-TU emissions where the fused
    path prints once. **So there is no invocation where per-module wins today**,
    which collides with the author's rule that a change must not slow the
    compiler down — and the collision is architectural rather than a defect:
    panel 093 R1 keeps the frontend whole-program, so a warm rebuild re-parses
    and re-checks all 158 modules whatever the cache holds. Two directions, and
    the decision is the author's: make the per-TU emission cheaper (it prints
    471k lines against the fused 788k, at 2.5x the cost per line — the per-TU
    setup), or make the FRONTEND incremental, which is the only one that changes
    the ratio.

    **RE-MEASURED 2026-08-26 at step 7 close, and the pair above is NOT
    REPRODUCIBLE on today's tree.** Four timings inside fifteen minutes, one
    machine, one source tree, with the pre-step-7 compiler rebuilt from the
    committed seed as the control: per-module **cold 142.29 s** before /
    **145.53 s** after, per-module **warm 133.08 s** before / **132.68 s**
    after. The fused path measured the same day is **≈124 s** (`--emit-c`
    120.45 s + one clang line over the 792,357-line seed 3.65 s). So the 261 s /
    250 s above is 1.9x today's number for the same command and the same binary
    lineage, and **no cause is asserted because none was measured** — the old
    pair keeps its date (CLAUDE.md §14) and this one is beside it, because the
    decision this item asks for was about to be taken on numbers almost twice
    too large. On the new numbers: per-module warm 132.7 s against fused 124 s,
    so per-module still does not win — but the gap is **8.7 s, not 127 s**,
    which is a different question. Step 7's own contribution is measured and
    small: +3.2 s cold for the header freshness check, nothing at all warm, and
    the `runtime_text` hoist it also landed (157 shell `cat` calls and 25 MB of
    hashing removed per build) does **not** show in wall clock at all.

    **ANSWERED 2026-08-26 by the author: make the FRONTEND incremental** — and
    then the premise the answer rested on stopped being true the same day, which
    is why the item is rewritten rather than actioned. Step 9 took `heroes
    check` on the compiler's own source from **88.0 s to about 8** by removing
    four instances of one quadratic; no architecture changed. Re-measured after
    it, one machine, one tree, new compiler: per-module **cold 57.93 s**,
    **warm 45.21 / 46.23 s**; fused **emit 40.41 s + one clang line 3.71 s =
    44.1 s**. **So the frontend is now ~18% of a build where this item measured
    83%, and the per-module/fused gap is ~1 s where it was 8.7** — the prize an
    incremental frontend could win is at most the 8 s the frontend now costs,
    and the 83% figure that made it the obvious direction is gone. **What
    dominates instead is the per-TU EMISSION, and that is architectural rather
    than a defect**: panel 093 R4 puts the emitted text IN the cache key, so a
    warm build must emit all 157 TUs to learn that it may reuse their objects.
    The cache can never skip the work that computes its own key. That is the
    shape the next sitting on this should argue about, with these numbers; an
    incremental frontend is now the smaller half. The item stays here rather
    than moving to `DECIDE.md` because what it asks is which milestone does it,
    not what should be true — answered 2026-09-03: M-lsp-server, for the
    frontend half an editor feels; the emission half, architectural by panel 093
    R4, stays measured here as the trigger.

    **Why it matters:** separate compilation was priced in LINES by the sitting
    and never in seconds, and the seconds say the payoff is behind a
    whole-program frontend.

    **Re-verified 2026-09-10: STILL OPEN.** No incremental frontend exists;
    `grep -rln incremental selfhost/` hits a comment about `starts_of`
    (`selfhost/source.hero:116`) and MSVC's incremental **linker**
    (`selfhost/cli/flags.hero:126-130`). **Its numbers were deliberately not
    re-run**: every one is a wall-clock build, and CL-025 forbids a clock while
    anything else moves, so they stand as the three dated triples the item already
    carries.

*******************************************************************************
