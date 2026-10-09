---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **549 — `rest: zero` on a header record past 32 deep makes clang die on its `(T){0}`** | `rest: zero` on a header record 1,000 deep writes `t = (G999){0};` (`selfhost/emit/zeroed.hero`), and clang dies (Illegal instruction: 4) on the base and with defect 539's repair; without that line, or with `(G999){}`, it compiles; panel 194 chose `{0}` there on C11 grounds (lane b17-emit; reproducer `.claude/worktrees/scratch-b15/b17-emit/rz/`, ignored by git) | `selfhost/emit/zeroed.hero` · defect 539 · panel 194 · **class: blocking**

    **Origin:** filed by the coordinator at 22:34 on 2026-10-09 from lane b17-emit's final report (its notes `.claude/worktrees/scratch-b15/b17-emit/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 on a correct program the emitter can avoid, defect 539's cause in another writer.
