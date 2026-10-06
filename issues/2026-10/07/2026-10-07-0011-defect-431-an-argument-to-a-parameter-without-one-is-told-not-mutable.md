---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **431 — an `@` argument to a parameter without one is told `not_mutable` before `marker_mismatch`** | `f(x: @p)`, `p` bound with `=` and `f`'s parameter not `@`: told first `not_mutable` (make `p` a cell), and `marker_mismatch` only once that is done, which was never the repair | `selfhost/resolve/` or `selfhost/check/`, the order of the two refusals · **class: adjacent**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-zero401's report of the evening before (*found beside*); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only after another is fixed, the first message pointing away from the repair.
