---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **585 — a name hidden by the program's own stricter switch is told *declares no* without naming the switch** | under a header of the program's own defining `_POSIX_C_SOURCE 200112L` before `string.h`, binding `strlcpy` is refused *`string.h` declares no `strlcpy`*, true, where the message could name the switch that hid it (defect 568's message names `_GNU_SOURCE` when a name is declared only under a feature-test macro, the converse shape) | the hidden-name message of defect 568's repair, `selfhost/emit/` · **class: improvement**

    **Origin:** found by lane b18-guard beside panel 205's landing (its final reply and notes, `.claude/worktrees/scratch-b15/b18-guard/notes.txt`, ignored by git), filed by the coordinator at 13:56 on 2026-10-10.

    **Class: improvement**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.
