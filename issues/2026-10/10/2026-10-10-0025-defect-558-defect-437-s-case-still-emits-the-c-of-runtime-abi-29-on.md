---
kind: defect
area: golden
milestone: none
filed: 2026-10-10
commit: 4bd4adc85e672224d1fbd61bdefc6b996f54ad91
github: none
---

- [x] **558 — defect 437's case still emits the C of runtime ABI 29 on Linux, and the CI's arm64 leg is red** | batch 17's push (`65b78f2e`), run 37993565819: Linux arm64's net reads 7,275 passed and 1 failed, `FAIL emission/run/fixedbugs-437-a-program-whose-runner-is-killed-dies-with-it`, its blessed C at line 13 `HERO_RUNTIME_ABI == 29` where the compiler now writes 30, and at lines 253 and 421 `hero_str_decref(...)` where defect 470 writes `hero_str_release_at(&...)`; the case binds `sys/prctl.h`, so this Mac skips it and batch 17's gate, which read every other emission, never compared it | `tests/emission/run-fixedbugs-437-a-program-whose-runner-is-killed-dies-with-it.c`, blessed on Linux with `UPDATE_EMISSION=1` (`tests/harness/suite_emission.hero:37`); and the shape beside it, every blessed emission of a case this Mac skips, after an emitter change · **class: blocking**

    **Origin:** filed by the coordinator at 00:25 on 2026-10-10 from the CI's Linux arm64 leg of batch 17's push, its job log read through the API (`gh api .../actions/jobs/114033692047/logs`, kept under `.claude/worktrees/scratch-b15/gate17/post/`, ignored by git), as `.claude/rules/verification.md` § The optimistic chain item 5 asks: a red CI leg is a new `blocking` defect, the next batch's first item.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a red CI.

    **Widened 2026-10-10**, read at 00:31: the CI's Linux x86-64 leg of the same run is red on the same case and only on it, 7,275 passed and 1 failed (job log through the API, kept beside the arm64 one); Darwin arm64 is green, the case skipped there as on this Mac.

    Repaired at `4bd4adc8`, 2026-10-10 (the coordinator, at batch 18's close), gated by its case and the round's gate; the net is owed at the batch's close. The case's emission blessed in `heroes-linux-arm64:latest` (Debian clang 22.1.8) with the round's compiler built from its regenerated seed at `f6528c53`: `emission` whole there read 1168 passed and 1 failed before, the one red this case, so the shape beside it (every emission of a case this Mac skips) is this case alone; 1169 and 0 after the bless and on a second run; only this file copied back. The 860 moved lines are batch 17's and 18's repairs (defect 470's `hero_str_release_at`, 472's one-line prologue, the `#line` renumbering). The CI's two Linux legs judge it after the push.

## The repair

Repaired at `4bd4adc8`, 2026-10-10 (the coordinator, at batch 18's close), gated by its case and the round's gate; the net is owed at the batch's close. The case's emission blessed in `heroes-linux-arm64:latest` (Debian clang 22.1.8) with the round's compiler built from its regenerated seed at `f6528c53`: `emission` whole there read 1168 passed and 1 failed before, the one red this case, so the shape beside it (every emission of a case this Mac skips) is this case alone; 1169 and 0 after the bless and on a second run; only this file copied back. The 860 moved lines are batch 17's and 18's repairs (defect 470's `hero_str_release_at`, 472's one-line prologue, the `#line` renumbering). The CI's two Linux legs judge it after the push.

**Closed 2026-10-10** with batch 18 (lanes b18-close, b18-infer, b18-ffi and b18-guard, merged into the round `lane-round-b18` with the trunk), its closing gate run on the round at `f6528c53`: the seed regenerated over two generations, the runtime's ABI at 30, 50,640,450 bytes, SHA-256 beginning `3bfbddd0f618b118`, its fixpoint by `cmp`; the compiler's own tests 1,577, all passed; the net's own tests 332, all passed; the full net 7,872 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after, its one red `order` on a walk of defect 570's `cli/pragma_ask.hero` with no `# ORDER:` mark, the mark written on its function's doc line (no line moved, the fixpoint re-checked by `cmp`) and `order` 3 and 0 after; eight floors told outgrown and raised in the closing commit, `order`, `runtime`, `emit`, `unsupported` and `probe` 3, 8, 12, 237 and 27, all 0 failed, after it. Defect 558's case, the one emission this Mac skips, was blessed and read green on Linux arm64 at the same commit (1,169 and 0). Under the optimistic chain the census and panel 187's R2 run after the push beside the CI, and a CI leg red on a closed defect's case files a new `blocking` defect naming it.
