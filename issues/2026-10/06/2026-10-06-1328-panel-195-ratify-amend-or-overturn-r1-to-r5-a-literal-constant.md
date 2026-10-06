---
kind: decision
area: emit
milestone: none
filed: 2026-10-06
commit: self
github: none
---

- [x] **panel 195** | ratify, amend or overturn R1 to R5 (a literal constant is one `HERO_ARRAY_STATIC` block whose count is never written, the guard returning on exactly -1 and panicking by name below 1, `HERO_RUNTIME_ABI` 27 to 28; a sanitized build keeps the block per read; the landing's cases on three platforms; `onceheld`, the static writable block, the per-thread copy, the static table and hoisting refused, the element route's inlined read filed apart; the stamp's move in a batch of its own) | `docs/panel/195-a-literal-constant-is-one-static-block-whose-count-is-never-written-and-a-sanitized-build-keeps-the-block-per-read.md` § The resolution

    **Origin:** panel 195's synthesis, 2026-10-06 from 13:28, on lane b12-ir12's branch head `96f3a588`, convened by the coordinator on lane ir12's recommendation under the author's goal of that day.

    **Recommendation: ratify R1 to R5**, the robust route at the one disagreement, on what was built and run: the static block built by the compiler-engineer and rebuilt by the critic on four platforms, meeting each veto condition the ffi-pragmatist stated but the last, which R2 answers in the sanitized build; `onceheld` refused on the critic's measurement that a release too many lets a reader write the shared constant.

    **The conservative alternative, the author's to choose instead**: `f7576a01` alone, a constant's read 39 times a name's at `-O0`, 382 closed with the cost written down.

    **Ratified by delegation, 2026-10-06**, on the author's answer at 12:49 by the clock read then, given before the synthesis existed, meant as: *OK, ratify* (`issues/2026-10/06/2026-10-06-1249-the-author-answers-1-2-3-m-issue-files-to-this-session-github.md`, point 3). **What it settles**: R1 to R5 as the synthesis states them. **What it does not settle**: what only the landing measures, R2's switch above all.
