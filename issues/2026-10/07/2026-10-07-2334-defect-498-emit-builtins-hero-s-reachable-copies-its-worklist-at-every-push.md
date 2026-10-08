---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: 9b26e4b10077fbacf92fe43e3425ccc16765f4e7
github: none
---

- [ ] **498 — `emit/builtins.hero`'s `reachable` copies its worklist at every push** | 192,132,578 elements copied on `--emit-c` of the compiler's own source, about 15% of the emitting thread: a local worklist popped with `work @ work.slice(...)` is still shared at its next push; 1,000 pushes copy 999,000 elements (lane b14-p409, `<scratchpad>/batch14/p409/probe/worklist.hero`) | `selfhost/emit/builtins.hero` · defect 409 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-p409's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the worklist's square.

    Repaired at `9b26e4b1`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `emit/worklist.hero` drains a worklist from its top in place, in the slicing drain's own order, in `builtins.reachable` and the emitter's five other slice-popped worklists: on the compiler's own `--emit-c` the elements copied by a slice, a copying push or an unshare fell from 2,432,221,290 to 338,917,151, `reachable`'s 2,091,585,474 to none, and its instructions from 818.095 to 689.013 billion, the C byte-identical.
