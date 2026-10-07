---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **491 — `hero_array_at_mut` and `hero_str_byte` are still runtime calls where defect 389 inlined reads** | 389's cost shape; `at_mut` sits nested inside place expressions, so the read's inline form would duplicate calls there (lane b14-emit) | `selfhost/emit/`, `runtime/heroes_runtime.h` · defect 389 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-emit's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost per write and per byte read, unmeasured.
