---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **405 — a nested fixed array outside a group is told twice** | a binding of `i64[2][3]` outside a group record is told `fixed_outside_a_group` for the outer type and again for its inner `i64[2]`, two messages for one mistake; the order is pinned by `check/nested-fixed-array-tiebreak` | `selfhost/check/ffi_sweep.hero` · **class: adjacent**

    **Origin:** lane b13-front, 2026-10-06 (its report, *found beside, not filed*), the lane's measurement on its branch from `7001dfb3`, not re-run by the coordinator; filed by the coordinator at 20:41.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a mistake told twice, where the thesis wants one message per mistake (design.md §4.17).
