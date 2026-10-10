---
kind: defect
area: harness
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **605 — defect 539's thousand-deep case under `--sanitize` ends exit 124 on the CI's Linux arm64 leg** | batch 19's push (`b98016b5`): Linux arm64 read `FAIL run/fixedbugs-539-records-a-thousand-deep-holding-a-str-build at --sanitize`, *printed something else*, nothing printed, *it ended: exit 124*, its stderr empty; Linux x86-64 read the same case green; in `heroes-linux-arm64:latest` on this Mac (8 cores) the coordinator's `run fixedbugs-539` read 3 passed and 0 failed, the sanitized build 7.47 s real (7.29 user) and its binary 0.05 s, printing `x` and `yz`: unreproduced, so the CI runner's speed, its load or the watchdog's bound is a question, not a premise (defect 566 was the same case past the watchdog at `-O2` on Windows) | the `run` suite's watchdog and the case's build cost under `--sanitize`, `tests/harness/suite_run.hero`; the CI's arm64 runner · **class: blocking**

    **Origin:** filed by the coordinator at 20:47 on 2026-10-10 from the CI's two Linux legs of batch 19's push, run 38069799820, their job logs read through the API (`.claude/worktrees/scratch-b15/ci-b980/`, ignored by git), as `.claude/rules/verification.md` § The optimistic chain item 5 asks.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a red CI.
