---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **552 — `[ok(1)]` in a generic function's argument keeps the old `ok` message** | defect 544's repair tells `ok(...)` at a generic function's parameter by its rule, and the same `ok(1)` one level down, inside an array literal argument, keeps the message before it (lane b17-check) | `selfhost/check/generic_argument.hero` · defect 544 · **class: adjacent**

    **Origin:** filed by the coordinator at 22:35 on 2026-10-09 from lane b17-check's final report (its notes `.claude/worktrees/scratch-b15/b17-check/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than its sibling's.
