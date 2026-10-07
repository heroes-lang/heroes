---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **498 — `emit/builtins.hero`'s `reachable` copies its worklist at every push** | 192,132,578 elements copied on `--emit-c` of the compiler's own source, about 15% of the emitting thread: a local worklist popped with `work @ work.slice(...)` is still shared at its next push; 1,000 pushes copy 999,000 elements (lane b14-p409, `<scratchpad>/batch14/p409/probe/worklist.hero`) | `selfhost/emit/builtins.hero` · defect 409 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-p409's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the worklist's square.
