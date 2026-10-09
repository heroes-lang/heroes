---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **531 — a wrong-arity call through a function value names `this function` as if it were a name** | the message reads *`this function` takes 1 argument(s), found 2*: a placeholder printed in backticks where a name stands (lane b16-compiler) | the arity diagnostic of a call through a function value, `selfhost/check/` · defect 510 · **class: adjacent**

    **Origin:** filed by the coordinator at 12:14 on 2026-10-09 from lane b16-compiler's final report (its notes `.claude/worktrees/scratch-b15/compiler/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.
