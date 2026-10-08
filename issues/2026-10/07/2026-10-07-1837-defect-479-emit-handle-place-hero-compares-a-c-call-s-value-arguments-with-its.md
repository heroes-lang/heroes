---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: e8aafc46515fed5b962fba0c1004db497c223baa
github: none
---

- [ ] **479 — `emit/handle_place.hero` compares a C call's value arguments with its `@` arguments pairwise** | `rewritten`, defect 260's cost shape (lane b14-resolve, read, not measured) | `selfhost/emit/handle_place.hero` · defect 260 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-resolve's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the arguments' square, unmeasured.

    Repaired at `e8aafc46`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The `@` places a C call writes are held once, their keys and every key a step boundary cuts one short at, and each value argument asks the two sets once per step of its own key, the pairwise `overlaps` its test's oracle over every pair of fourteen shapes: one call of 800 value and 800 `@` arguments read 164.502 billion instructions against 162.993, the rest of that shape's growth another function's.
