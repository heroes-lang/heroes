---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **550 — two groups whose headers conflict through an included file are told falsely in one order** | when the conflicting definition sits in a file another group's header includes, clang's note names that file, which no group names, so defect 538's repair tells one `use` order of both headers and the other still with the old false message; telling it order-free needs the include graph (lane b17-emit; reproducer `.claude/worktrees/scratch-b15/b17-emit/inc/`, ignored by git) | `selfhost/cli/headers_together.hero` · defect 538 · panel 200 R1 · **class: blocking**

    **Origin:** filed by the coordinator at 22:34 on 2026-10-09 from lane b17-emit's final report (its notes `.claude/worktrees/scratch-b15/b17-emit/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a false message, the shape beside defect 538, for the sitting panel 200 R1 names.
