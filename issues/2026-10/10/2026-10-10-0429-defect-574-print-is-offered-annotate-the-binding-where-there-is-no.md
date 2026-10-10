---
kind: defect
area: check
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **574 — `print({})` is offered *annotate the binding* where there is no binding** | defect 553's repair offers an empty map a map's annotation, `m: {str: i64} = {}`; for `print({})`, an empty map with nothing to type it and no binding, the fix still says *annotate the binding* | the empty-literal fix of `selfhost/check/` (defect 553's repair, `4156ee29`) · **class: adjacent**

    **Origin:** found by lane b18-close beside defect 465 (its final reply and notes, `.claude/worktrees/scratch-b15/b18-close/notes.txt`, ignored by git), filed by the coordinator at 04:29 on 2026-10-10.

    **Class: adjacent**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.
