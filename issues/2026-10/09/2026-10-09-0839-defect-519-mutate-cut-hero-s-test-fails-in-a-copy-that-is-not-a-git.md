---
kind: defect
area: process
milestone: none
filed: 2026-10-09
commit: 0e9bd4cdc807227dae500589e94b8b12cdceac23
github: none
---

- [x] **519 — `mutate/cut.hero`'s test fails in a copy that is not a git checkout** | it asks `.` for a git checkout, so the compiler's own tests read one failure in any `git archive` snapshot or copy without `.git`, the base the same (lanes b15-runtime and b15-emit) | `selfhost/mutate/cut.hero` · defect 448 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-runtime's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a test asking the world where it could ask the value.

    Repaired at `0e9bd4cd`, 2026-10-09 (lane b16-tools, batch 16), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The test makes two checkouts of its own under `build/`, a clone's `.git` directory and a worktree's `.git` file, and asks the cut of each: a `git archive` of the base read 1504 tests and 1 failed, the same archive with the repair 1504 passed, the lane 1504 passed.

## The repair

Repaired at `0e9bd4cd`, 2026-10-09 (lane b16-tools, batch 16), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The test makes two checkouts of its own under `build/`, a clone's `.git` directory and a worktree's `.git` file, and asks the cut of each: a `git archive` of the base read 1504 tests and 1 failed, the same archive with the repair 1504 passed, the lane 1504 passed.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
