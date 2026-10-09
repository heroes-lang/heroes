---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **510 — calling a constant that holds a function exits 2 with an internal error** | `DOUBLE(4)`, where a constant `DOUBLE` holds a function: the generated C calls the constant's zero-argument accessor with an argument and the build exits 2, *internal error*; binding it first, `h = DOUBLE` then `h(4)`, works (lane b16-land199; its reproducer `.claude/worktrees/scratch-b15/land199/beside/constcall.hero`, ignored by git) | the call of a constant of function type, `selfhost/ir/` and `selfhost/emit/` · **class: blocking**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b16-land199's final report; the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 on a correct program.
