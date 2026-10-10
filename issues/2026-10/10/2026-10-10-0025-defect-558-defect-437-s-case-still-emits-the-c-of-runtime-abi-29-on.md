---
kind: defect
area: golden
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **558 — defect 437's case still emits the C of runtime ABI 29 on Linux, and the CI's arm64 leg is red** | batch 17's push (`65b78f2e`), run 37993565819: Linux arm64's net reads 7,275 passed and 1 failed, `FAIL emission/run/fixedbugs-437-a-program-whose-runner-is-killed-dies-with-it`, its blessed C at line 13 `HERO_RUNTIME_ABI == 29` where the compiler now writes 30, and at lines 253 and 421 `hero_str_decref(...)` where defect 470 writes `hero_str_release_at(&...)`; the case binds `sys/prctl.h`, so this Mac skips it and batch 17's gate, which read every other emission, never compared it | `tests/emission/run-fixedbugs-437-a-program-whose-runner-is-killed-dies-with-it.c`, blessed on Linux with `UPDATE_EMISSION=1` (`tests/harness/suite_emission.hero:37`); and the shape beside it, every blessed emission of a case this Mac skips, after an emitter change · **class: blocking**

    **Origin:** filed by the coordinator at 00:25 on 2026-10-10 from the CI's Linux arm64 leg of batch 17's push, its job log read through the API (`gh api .../actions/jobs/114033692047/logs`, kept under `.claude/worktrees/scratch-b15/gate17/post/`, ignored by git), as `.claude/rules/verification.md` § The optimistic chain item 5 asks: a red CI leg is a new `blocking` defect, the next batch's first item.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a red CI.

    **Widened 2026-10-10**, read at 00:31: the CI's Linux x86-64 leg of the same run is red on the same case and only on it, 7,275 passed and 1 failed (job log through the API, kept beside the arm64 one); Darwin arm64 is green, the case skipped there as on this Mac.
