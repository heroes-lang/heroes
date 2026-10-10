---
kind: defect
area: harness
milestone: none
filed: 2026-10-10
commit: 877f7867c447ce1e6a5d4e2a4954325f46e103d1
github: none
---

- [ ] **605 — defect 539's thousand-deep case under `--sanitize` ends exit 124 on the CI's Linux arm64 leg** | batch 19's push (`b98016b5`): Linux arm64 read `FAIL run/fixedbugs-539-records-a-thousand-deep-holding-a-str-build at --sanitize`, *printed something else*, nothing printed, *it ended: exit 124*, its stderr empty; Linux x86-64 read the same case green; in `heroes-linux-arm64:latest` on this Mac (8 cores) the coordinator's `run fixedbugs-539` read 3 passed and 0 failed, the sanitized build 7.47 s real (7.29 user) and its binary 0.05 s, printing `x` and `yz`: unreproduced, so the CI runner's speed, its load or the watchdog's bound is a question, not a premise (defect 566 was the same case past the watchdog at `-O2` on Windows) | the `run` suite's watchdog and the case's build cost under `--sanitize`, `tests/harness/suite_run.hero`; the CI's arm64 runner · **class: blocking**

    **Origin:** filed by the coordinator at 20:47 on 2026-10-10 from the CI's two Linux legs of batch 19's push, run 38069799820, their job logs read through the API (`.claude/worktrees/scratch-b15/ci-b980/`, ignored by git), as `.claude/rules/verification.md` § The optimistic chain item 5 asks.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a red CI.

    Repaired at `877f7867`, 2026-10-11 (lane b20-tools), gated by its case and the net's own tests; the net is owed at the batch's close. The case's `run --sanitize` builds at `-O2`, where the timing at filing was `build --sanitize`, at `-O0`: in the CI's own clang (Ubuntu 24.04's 18.1.3, arm64) in a container of four CPUs on this Mac it took 59.5 s and 2.77 GB, the 875-deep 51.3 s, the seed's build beside them 9.7 s; the runner built the seed in 21.7 s, and the CI's own two verdicts, the 875 under 120 s and the thousand over, bracket the factor at 2.02 to 2.34, so 120 to 139 s; the CI runs the net in one process, nothing beside the case. Reproduced with the container at 0.45 CPU (its seed 23.8 s): the trunk's harness read `FAIL run/fixedbugs-539-records-a-thousand-deep-holding-a-str-build at --sanitize`, exit 124 and stderr empty, the CI's words, and the lane's PASS. The same image's seed build read 9.7 s and 20.8 s an hour apart, so `shell.WATCHDOG_SECONDS` is 600 s, about four times the upper estimate. The cost is defect 622. `run` narrowed to `fixedbugs-539` 3 and 0 on this Mac, the net's own tests 336 and 0.
