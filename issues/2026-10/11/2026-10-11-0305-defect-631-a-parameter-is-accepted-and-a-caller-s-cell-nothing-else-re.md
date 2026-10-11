---
kind: defect
area: resolve
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **631 — a parameter `@_` is accepted, and a caller's cell nothing else re-binds escapes panel 209's rule through it** | `function f(@_: i64)` checks and runs at exit 0, and a caller's `k @= 1` passed as `f(@k)` and re-bound nowhere else is not refused `never_rebound`, the `@` argument counting as its re-binding; defect 608's twin one level down (608 refused `_ @= e`) | `selfhost/resolve/` (`resolve/meant.declares_no_cell`, 608's repair); panel 210 R2 · **class: blocking**

    **Origin:** filed by the coordinator at 03:05 on 2026-10-11 from panel 210's completeness critic (`docs/panel/210-reports/critic.md`), its builds under `.claude/worktrees/scratch-b15/210-critic/` (ignored by git), the seat's measurement on the tree frozen at `1dd890751`, not re-run by the coordinator.

    **Class: blocking**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): a wrong program accepted: a cell nothing can re-bind.
