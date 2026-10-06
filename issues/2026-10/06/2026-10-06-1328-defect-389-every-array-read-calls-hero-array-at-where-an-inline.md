---
kind: defect
area: emit
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **389 — every array read calls `hero_array_at` where an inline index would do** | a loop reading a name bound to a 32-element array a million times, `local.hero`, retires 116.5 million instructions; the same loop with the read's call replaced by an inline index, 85.5 million (panel 195's completeness critic, its second pass, 2026-10-06, its `second/measure/`; not re-run by the coordinator) | `selfhost/emit/`, the read of an array element · `runtime/heroes_runtime.h`, `hero_array_at` · panel 195 R4 · **class: improvement**

    **Origin:** panel 195's completeness critic, 2026-10-06: 84 to 86% of the element route's gain over the floor came from this, which every array read could have; filed by the coordinator at 13:28 under that sitting's R4.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost no rule promises against, every value right; outside the batch under the author's instruction of 2026-10-05.
