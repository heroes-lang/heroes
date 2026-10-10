---
kind: defect
area: harness
milestone: none
filed: 2026-10-10
commit: cf5d79586c9357f0ab4fc82b02a483ee4fdf9aa8
github: none
---

- [ ] **604 — the two emissions this Mac skips moved with batch 19's emitter and were not blessed on Linux** | batch 19's push (`b98016b5`): Linux x86-64 read 8,010 passed and 2 failed, Linux arm64 8,009 and 3, both red on `emission/run/fixedbugs-437-a-program-whose-runner-is-killed-dies-with-it` (*no longer emits what it was blessed to emit*, its line 12 now `#pragma clang diagnostic ignored "-Wpragma-pack"`, defect 582's header-state lines) and `emission/run/fixedbugs-589-a-call-of-mktemp-links-silent-on-linux` (*nothing blessed*, a case skipped off Linux by name); defect 558's shape again, batch 19's close owing the Linux bless of every case this Mac skips and not running it | `tests/emission/`, blessed on Linux arm64 · **class: blocking**

    **Origin:** filed by the coordinator at 20:47 on 2026-10-10 from the CI's two Linux legs of batch 19's push, run 38069799820, their job logs read through the API (`.claude/worktrees/scratch-b15/ci-b980/`, ignored by git), as `.claude/rules/verification.md` § The optimistic chain item 5 asks.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a red CI.

    Repaired at `cf5d7958`, 2026-10-10 (the coordinator), gated by its cases on Linux arm64; the net is owed at the batch's close. In `heroes-linux-arm64:latest` (8 cores), the compiler built from `b98016b5`'s seed: `emission` whole read 1,207 passed and 2 failed before, the two these, so the shape beside them is these two alone; 1,209 and 0 after `UPDATE_EMISSION=1` and on a second run; only the two files copied back. 437's 65 moved lines are 582's header-state lines, 584's quiet lines in place of 571's spoken ones, and the `#line` renumbering. The CI's Linux legs judge it after the next push.
