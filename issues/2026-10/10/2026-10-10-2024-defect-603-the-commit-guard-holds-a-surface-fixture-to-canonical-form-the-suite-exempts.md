---
kind: defect
area: process
milestone: none
filed: 2026-10-10
commit: 9d493742be4433bdb24d55a3a039ef96d13ee942
github: none
---

- [ ] **603 — the commit guard holds a surface fixture to canonical form that the `canonical` suite exempts** | `git commit -- tests` carrying `tests/golden/surface-fixtures/comments101/qualified.hero` and `types.hero`, two probe fixtures that are not canonical on purpose (comments after a type's `.` and inside a parameter's brackets, the shapes the probe reads) and were not canonical at HEAD either, is refused by `.claude/hooks/staged.py`: *is not canonical (`heroes fmt … --in-place`)*; the `canonical` suite reads only `SOURCE_DIRS`' seven directories (`tests/harness/suite_canonical.hero:66-75`: `selfhost`, `tests/harness`, `examples`, `run`, `ir`, `emit`, `fixedbugs`) and is green over the same tree (2 passed, 0 failed at 20:12), so the guard refuses what the judge accepts, the shape defect 581 repaired this morning for a marked golden case and left standing for a fixture that carries no `#~` mark. Found when M-inferred-cell's step 1 rewrote one cell in each (`Balance] @ g(n: n)` to `=`) and could not commit the tree | `.claude/hooks/staged.py` (the *not canonical* refusal, its 581 exemption through `marks.held_to_marks` and `marks.in_run_roots`), `.claude/hooks/test_hooks.py`; the suite's `SOURCE_DIRS` is the list the guard should read, as `ceiling.py` reads the `layout` suite's `DECIDED` from disk · **class: blocking**

    **Origin:** found by the landing lane of panel 209 at 20:22 on 2026-10-10, at the commit of M-inferred-cell step 1; the two files' HEAD versions were judged with `heroes fmt <copy>` and are not canonical either, so the guard would have refused them whenever any commit named them.

    **Class: blocking**, 2026-10-10: the current work's acceptance fails, a commit the suites accept cannot be made; repaired in the same lane, the hook's canonical refusal scoped to the directories the `canonical` suite reads, with a test in `test_hooks.py`.

    Repaired at `9d493742` (lane `lane-inferred-cell`, 2026-10-10 20:30; on the trunk's local `main` as the cherry-pick `55bcd8f2`, since the guard that judges a lane's commit is the trunk's copy), gated by the hooks' own tests, 155 passed, five of them the new class `ProbeFixtures`; the net is owed at the batch's close.
