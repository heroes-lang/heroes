---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **406 — an unused binding hides `fixed_outside_a_group` until it is fixed** | a program with an unused binding and fixed arrays outside a group was told 2 messages, then 8 once the binding was removed (the lane's `shapes.hero`) | `selfhost/check/`, the order the checker stops in · **class: adjacent**

    **Origin:** lane b13-front, 2026-10-06 (its report, *found beside, not filed*), the lane's measurement on its branch from `7001dfb3`, not re-run by the coordinator; filed by the coordinator at 20:41.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only once another is fixed, class (b) of defect 212's reading.
