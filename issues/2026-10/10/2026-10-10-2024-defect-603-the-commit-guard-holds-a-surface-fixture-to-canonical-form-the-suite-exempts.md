---
kind: defect
area: process
milestone: none
filed: 2026-10-10
commit: 9d493742be4433bdb24d55a3a039ef96d13ee942
github: none
---

- [x] **603 — the commit guard holds a surface fixture to canonical form that the `canonical` suite exempts** | `git commit -- tests` carrying `tests/golden/surface-fixtures/comments101/qualified.hero` and `types.hero`, two probe fixtures that are not canonical on purpose (comments after a type's `.` and inside a parameter's brackets, the shapes the probe reads) and were not canonical at HEAD either, is refused by `.claude/hooks/staged.py`: *is not canonical (`heroes fmt … --in-place`)*; the `canonical` suite reads only `SOURCE_DIRS`' seven directories (`tests/harness/suite_canonical.hero:66-75`: `selfhost`, `tests/harness`, `examples`, `run`, `ir`, `emit`, `fixedbugs`) and is green over the same tree (2 passed, 0 failed at 20:12), so the guard refuses what the judge accepts, the shape defect 581 repaired this morning for a marked golden case and left standing for a fixture that carries no `#~` mark. Found when M-inferred-cell's step 1 rewrote one cell in each (`Balance] @ g(n: n)` to `=`) and could not commit the tree | `.claude/hooks/staged.py` (the *not canonical* refusal, its 581 exemption through `marks.held_to_marks` and `marks.in_run_roots`), `.claude/hooks/test_hooks.py`; the suite's `SOURCE_DIRS` is the list the guard should read, as `ceiling.py` reads the `layout` suite's `DECIDED` from disk · **class: blocking**

    **Origin:** found by the landing lane of panel 209 at 20:22 on 2026-10-10, at the commit of M-inferred-cell step 1; the two files' HEAD versions were judged with `heroes fmt <copy>` and are not canonical either, so the guard would have refused them whenever any commit named them.

    **Class: blocking**, 2026-10-10: the current work's acceptance fails, a commit the suites accept cannot be made; repaired in the same lane, the hook's canonical refusal scoped to the directories the `canonical` suite reads, with a test in `test_hooks.py`.

    Repaired at `9d493742` (lane `lane-inferred-cell`, 2026-10-10 20:30; on the trunk's local `main` as the cherry-pick `55bcd8f2`, since the guard that judges a lane's commit is the trunk's copy), gated by the hooks' own tests, 155 passed, five of them the new class `ProbeFixtures`; the net is owed at the batch's close.

## The repair

Repaired at `9d493742` (lane `lane-inferred-cell`, 2026-10-10 20:30; on the trunk's local `main` as the cherry-pick `55bcd8f2`, since the guard that judges a lane's commit is the trunk's copy), gated by the hooks' own tests, 155 passed, five of them the new class `ProbeFixtures`; the net is owed at the batch's close.

**Closed 2026-10-11** with batch 20 (lanes b20-pragma, b20-check, b20-link and b20-tools, merged into the round `lane-round-b20` made from the trunk at `42656199`, M-inferred-cell landed), its closing gate run on the round at `d56e1442`: the seed regenerated over two generations, the runtime's ABI at 30, its fixpoint by `cmp`, SHA-256 beginning `d000037530dc63c5`; the compiler's own tests 1,607, all passed; the net's own tests 336, all passed; the full net 8,211 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after. A first gate B on the round at `1dd89075` read two reds the batch's own repairs owed, both repaired before the second: `probe` on defect 608's old-symbol case (608's repair had left `fmt` writing `@=` for the wildcard's old `@`, repaired in 608 at `18aa64b7`), and `emission` on defect 590's three new run cases, never blessed, blessed on this Mac and on Linux arm64, byte-identical (`cca795fe`). The emissions of the cases this Mac skips were run whole on Linux arm64 at `1dd89075` (1,220 passed and 590's 3 unblessed before, 1,223 and 0 after). The trunk's merge after the gate (panel 210's records and `records/verdicts`' dated-answer reading) read records 28, canonical 2, unseen 3, spec 23 and the net's own tests 337, all 0 failed. Under the optimistic chain the CI's legs judge it after the push, a red leg filing a new `blocking` defect naming it.
