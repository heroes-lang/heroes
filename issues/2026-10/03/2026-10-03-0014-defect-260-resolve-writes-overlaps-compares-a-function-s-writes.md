---
kind: defect
area: resolve
milestone: none
filed: 2026-10-03
commit: adf443f3349296397467501de8003704a469ac4c
github: none
---

- [ ] **260 — `resolve/writes.overlaps` compares a function's writes pairwise** | 319,600 calls on `many-params-800` (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/emit/shapes/` and `<scratchpad>/round1003d/shapes2000/`) | `selfhost/resolve/writes.hero:237` (`overlaps`) · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the parameters' square; no program refused or wrong.

    Repaired at `adf443f3`, 2026-10-07 (lane b14-resolve), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The `@` arguments of a call kept so far are one tree of their steps (`resolve/aliased_places.hero`), each node holding the first place that ends there, passes through, or goes on through a decided step, so a new place walks it once and is answered the earliest it overlaps, as the pairwise walk answered. One call of N `@` arguments, the name stage alone (one unknown name added so the checker never runs), instructions retired, base against the repair: 4.02 and 12.28 billion against 1.88 and 3.72 at 1,600 and 3,200 parameters, 13.74 against 3.74 for 3,200 fields, 12.91 against 2.94 for 3,200 literal indices; every output byte-identical from 100 to 3,200 in all three shapes; `check selfhost/main.hero` 66.06 against 65.96 billion, inside the noise. The rest of the whole shape's growth is the checker's `user_call`, reported apart.
