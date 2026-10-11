---
kind: defect
area: resolve
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **630 — panel 209's certain fix for a cell nothing re-binds leads away from the repair when the cell's method result is dropped** | `xs @= [1]` then `xs.push(4)` as a statement: `never_rebound` offers a `certain` `=`; once applied, the next message's first `guess` is refused `not_mutable` and its second compiles and prints 1 where the intended repair, `xs @ xs.push(4)`, prints 2; no wrong program compiles from the `certain` fix alone (the critic's class); the critic's unlisted route, a value line rooted at the local counted as a meant write, closes it | `selfhost/resolve/` (`never_rebound`'s fix) and the discarded-value message; panel 210 R4 · **class: adjacent**

    **Origin:** filed by the coordinator at 03:05 on 2026-10-11 from panel 210's completeness critic (`docs/panel/210-reports/critic.md`), its builds under `.claude/worktrees/scratch-b15/210-critic/` (ignored by git), the seat's measurement on the tree frozen at `1dd890751`, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): a fix that leads away from the repair, no wrong program compiled from it alone.
