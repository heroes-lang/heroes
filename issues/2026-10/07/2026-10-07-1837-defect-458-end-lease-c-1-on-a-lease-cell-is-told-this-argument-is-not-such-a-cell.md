---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **458 — `end_lease(@c, 1)` on a lease cell is told *this argument is not such a cell*, which is false of `@c`** | `end_lease(@c, 1)` where `c` is a lease cell gets `builtin_shape` and `end_lease_of_no_lease`, the second saying the argument is not such a cell, false of `@c`; the mistake is the extra argument (lane b14-resolve, read on the base) | `selfhost/check/leasing.hero` · **class: blocking**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-resolve's final report; the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a false message beside the true one.
