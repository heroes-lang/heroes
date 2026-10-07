---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: e5b7a318b7bd7dc8d89f34100a0425fb0672d2d0
github: none
---

- [ ] **431 — an `@` argument to a parameter without one is told `not_mutable` before `marker_mismatch`** | `f(x: @p)`, `p` bound with `=` and `f`'s parameter not `@`: told first `not_mutable` (make `p` a cell), and `marker_mismatch` only once that is done, which was never the repair | `selfhost/resolve/` or `selfhost/check/`, the order of the two refusals · **class: adjacent**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-zero401's report of the evening before (*found beside*); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only after another is fixed, the first message pointing away from the repair.

    Repaired at `e5b7a318`, 2026-10-07 (lane b13-zero401): an `@` the resolved callee takes none for is set aside and told once as `marker_mismatch`, before any `not_mutable` (`resolve/built_marks.hero`, `walk.hero`), a UFCS name resolved before its arguments; case `check/fixedbugs-431-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; the card filled by the coordinator.
