---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **433 — a value reached through `ok`, a UFCS receiver or a call's result keeps its second message beside a refused fixed array** | defect 423's withdrawal follows a binding's value, a store, a constant, a `return`, an argument, literals' elements and `if`/`match` branches, and not a value inside `ok(...)`, a UFCS call's receiver, or a call's result, which keep the walk's `type_mismatch` beside `fixed_outside_a_group` | defect 423's withdrawal, `fixed_judged.hero` on lane b13-fixed405 (unmerged on 2026-10-07) · **class: adjacent**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-fixed405's report of the evening before (*found beside*); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): one mistake told twice at the positions 423 does not follow.
