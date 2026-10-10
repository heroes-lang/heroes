---
kind: defect
area: process
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **581 — the commit guard refuses a golden case that is non-canonical on purpose even when its annotations are green** | `.claude/hooks/staged.py` refuses any commit carrying a `tests/golden/` case that parses but is not canonical, even where its `#~` marks hold and a narrowed `annotations` run is green; defect 576's two cases are non-canonical on purpose (`heroes fmt` writes `-(-128)` as `--128` and drops the parentheses the case tests), so a lane's merge of them was refused and lane b18-close merged lane b18-infer at an older commit to get past it; the marks exemption covers only cases `fmt` refuses | `.claude/hooks/staged.py` and `.claude/hooks/marks.py`: a marked golden case is judged by its marks through `annotations`, whatever `fmt` returns · **class: blocking**

    **Origin:** found by lane b18-close in its landing of panel 206 (its final reply and notes, `.claude/worktrees/scratch-b15/b18-close/notes.txt`, ignored by git), filed by the coordinator at 11:25 on 2026-10-10, the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a correct commit refused, the work's acceptance blocked by its own guard.
