---
kind: defect
area: process
milestone: none
filed: 2026-10-10
commit: 5c2880e6f49b94d358a8eaa9e47fd55081ce7f15
github: none
---

- [ ] **581 — the commit guard refuses a golden case that is non-canonical on purpose even when its annotations are green** | `.claude/hooks/staged.py` refuses any commit carrying a `tests/golden/` case that parses but is not canonical, even where its `#~` marks hold and a narrowed `annotations` run is green; defect 576's two cases are non-canonical on purpose (`heroes fmt` writes `-(-128)` as `--128` and drops the parentheses the case tests), so a lane's merge of them was refused and lane b18-close merged lane b18-infer at an older commit to get past it; the marks exemption covers only cases `fmt` refuses | `.claude/hooks/staged.py` and `.claude/hooks/marks.py`: a marked golden case is judged by its marks through `annotations`, whatever `fmt` returns · **class: blocking**

    **Origin:** found by lane b18-close in its landing of panel 206 (its final reply and notes, `.claude/worktrees/scratch-b15/b18-close/notes.txt`, ignored by git), filed by the coordinator at 11:25 on 2026-10-10, the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a correct commit refused, the work's acceptance blocked by its own guard.

    Repaired at `5c2880e6`, 2026-10-10 (lane b18-close), gated by its cases and the hooks' own tests; the net is owed at the batch's close. `marks.held_to_marks` states the rule once: a `tests/golden/` case of the annotations suite's `DIRECTORIES` (the four the `canonical` suite leaves out) whose `#~` marks claim diagnostics is asked of a narrowed `annotations` run whatever `fmt` answers, and left to the gate where the index holds another version than its working tree; a case with no mark keeps `fmt`'s verdict, and so does a marked program of `run/` or `fixedbugs/`, which `canonical` reads. Seven tests in `test_hooks.py`'s `NotCanonicalOnPurpose`, four red on the old guard and three green on both; probed on a merge of lane-b18-infer's head in a worktree of its own, its compiler built there: the old guard refused 576's two cases and 578's as *not canonical*, the new concluded the merge in one `annotations` run of 21 s, and an unmarked case not canonical staged beside them was refused by both. The hooks' 147 tests, records 28, unseen 3, 0 failed. The write-time hook, `fmt_check.py`, still tells such a case *not canonical* when it is written, outside this item's files.
