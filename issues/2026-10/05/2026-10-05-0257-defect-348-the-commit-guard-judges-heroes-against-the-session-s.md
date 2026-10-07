---
kind: defect
area: process
milestone: none
filed: 2026-10-05
commit: 79064ed118c110c5b4a1353b0a6e393c6f61317c
github: none
---

- [ ] **348 — the commit guard judges `./heroes` against the session's directory, not the tree the command runs in** | lane b11-misc, 2026-10-05: with the session's working directory on the trunk, `.claude/hooks/guard_bash.py` refused the lane's harness runs, naming the trunk's `selfhost/resolve/state.hero` (written 02:05) as newer than the trunk's `./heroes` (built 2026-10-04 21:24), while the lane's own compiler (02:41) was newer than every file of the lane; calling the compiler by its absolute path passed (the lane's report) | `.claude/hooks/guard_bash.py` (the compiler-age check, the directory it reads) · `.claude/rules/verification.md` § A suite is the last judge, layer 1 · **class: improvement**

    **Origin:** lane b11-misc, 2026-10-05 (its final report, *Found beside*).

    **Class: improvement**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a guard that refuses a run it should allow, and could allow one it should refuse where the two trees disagree the other way; no program is judged wrong by it, and the absolute path is the workaround.

    Repaired at `79064ed1`, 2026-10-07 (lane b14-hooks), gated by its cases, the hooks' own tests; the net is owed at the batch's close. `.claude/hooks/guard_bash.py` reads each segment in the directory the command's own `cd` leaves it in and judges a harness run's compiler against the tree that directory stands in (`.claude/hooks/trees.py`), a directory the text cannot tell giving no opinion; eight cases in `.claude/hooks/test_hooks.py`, six red on the base, and a scratch trunk older than its sources beside a fresh nested lane read the lane's run refused on the base guard and allowed on the repaired one.
