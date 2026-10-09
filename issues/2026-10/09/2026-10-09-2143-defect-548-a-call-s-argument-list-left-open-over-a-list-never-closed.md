---
kind: defect
area: parse
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **548 — a call's argument list left open over a list never closed is told twice for one edit** | `x = g(1, [2, 3` over `print(x)` over `y = 3 )` gets the lexer's *`[` never closed* and `expected_args_close` at `print`, two messages for one `])` edit; it goes through the call-argument reader, not the binding path defect 459's repair carries its openers through (lane b17-fix) | the call-argument reader of `selfhost/parse/` · defect 459 · **class: adjacent**

    **Origin:** filed by the coordinator at 21:43 on 2026-10-09 from lane b17-fix's final report (its notes `.claude/worktrees/scratch-b15/b17-fix/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.
