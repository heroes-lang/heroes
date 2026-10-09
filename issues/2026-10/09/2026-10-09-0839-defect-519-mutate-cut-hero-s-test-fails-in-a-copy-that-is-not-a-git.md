---
kind: defect
area: process
milestone: none
filed: 2026-10-09
commit: 0e9bd4cdc807227dae500589e94b8b12cdceac23
github: none
---

- [ ] **519 — `mutate/cut.hero`'s test fails in a copy that is not a git checkout** | it asks `.` for a git checkout, so the compiler's own tests read one failure in any `git archive` snapshot or copy without `.git`, the base the same (lanes b15-runtime and b15-emit) | `selfhost/mutate/cut.hero` · defect 448 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-runtime's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a test asking the world where it could ask the value.

    Repaired at `0e9bd4cd`, 2026-10-09 (lane b16-tools, batch 16), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The test makes two checkouts of its own under `build/`, a clone's `.git` directory and a worktree's `.git` file, and asks the cut of each: a `git archive` of the base read 1504 tests and 1 failed, the same archive with the repair 1504 passed, the lane 1504 passed.
