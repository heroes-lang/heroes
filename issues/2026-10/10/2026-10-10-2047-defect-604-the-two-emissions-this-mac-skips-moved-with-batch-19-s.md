---
kind: defect
area: harness
milestone: none
filed: 2026-10-10
commit: cf5d79586c9357f0ab4fc82b02a483ee4fdf9aa8
github: none
---

- [x] **604 — the two emissions this Mac skips moved with batch 19's emitter and were not blessed on Linux** | batch 19's push (`b98016b5`): Linux x86-64 read 8,010 passed and 2 failed, Linux arm64 8,009 and 3, both red on `emission/run/fixedbugs-437-a-program-whose-runner-is-killed-dies-with-it` (*no longer emits what it was blessed to emit*, its line 12 now `#pragma clang diagnostic ignored "-Wpragma-pack"`, defect 582's header-state lines) and `emission/run/fixedbugs-589-a-call-of-mktemp-links-silent-on-linux` (*nothing blessed*, a case skipped off Linux by name); defect 558's shape again, batch 19's close owing the Linux bless of every case this Mac skips and not running it | `tests/emission/`, blessed on Linux arm64 · **class: blocking**

    **Origin:** filed by the coordinator at 20:47 on 2026-10-10 from the CI's two Linux legs of batch 19's push, run 38069799820, their job logs read through the API (`.claude/worktrees/scratch-b15/ci-b980/`, ignored by git), as `.claude/rules/verification.md` § The optimistic chain item 5 asks.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a red CI.

    Repaired at `cf5d7958`, 2026-10-10 (the coordinator), gated by its cases on Linux arm64; the net is owed at the batch's close. In `heroes-linux-arm64:latest` (8 cores), the compiler built from `b98016b5`'s seed: `emission` whole read 1,207 passed and 2 failed before, the two these, so the shape beside them is these two alone; 1,209 and 0 after `UPDATE_EMISSION=1` and on a second run; only the two files copied back. 437's 65 moved lines are 582's header-state lines, 584's quiet lines in place of 571's spoken ones, and the `#line` renumbering. The CI's Linux legs judge it after the next push.

## The repair

Repaired at `cf5d7958`, 2026-10-10 (the coordinator), gated by its cases on Linux arm64; the net is owed at the batch's close. In `heroes-linux-arm64:latest` (8 cores), the compiler built from `b98016b5`'s seed: `emission` whole read 1,207 passed and 2 failed before, the two these, so the shape beside them is these two alone; 1,209 and 0 after `UPDATE_EMISSION=1` and on a second run; only the two files copied back. 437's 65 moved lines are 582's header-state lines, 584's quiet lines in place of 571's spoken ones, and the `#line` renumbering. The CI's Linux legs judge it after the next push.

**Closed 2026-10-11** with batch 20 (lanes b20-pragma, b20-check, b20-link and b20-tools, merged into the round `lane-round-b20` made from the trunk at `42656199`, M-inferred-cell landed), its closing gate run on the round at `d56e1442`: the seed regenerated over two generations, the runtime's ABI at 30, its fixpoint by `cmp`, SHA-256 beginning `d000037530dc63c5`; the compiler's own tests 1,607, all passed; the net's own tests 336, all passed; the full net 8,211 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after. A first gate B on the round at `1dd89075` read two reds the batch's own repairs owed, both repaired before the second: `probe` on defect 608's old-symbol case (608's repair had left `fmt` writing `@=` for the wildcard's old `@`, repaired in 608 at `18aa64b7`), and `emission` on defect 590's three new run cases, never blessed, blessed on this Mac and on Linux arm64, byte-identical (`cca795fe`). The emissions of the cases this Mac skips were run whole on Linux arm64 at `1dd89075` (1,220 passed and 590's 3 unblessed before, 1,223 and 0 after). The trunk's merge after the gate (panel 210's records and `records/verdicts`' dated-answer reading) read records 28, canonical 2, unseen 3, spec 23 and the net's own tests 337, all 0 failed. Under the optimistic chain the CI's legs judge it after the push, a red leg filing a new `blocking` defect naming it.
