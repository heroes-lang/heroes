---
kind: defect
area: runtime
milestone: none
filed: 2026-10-11
commit: 14e53aabd05afcf6055f6d7ef2939f5ac93b18a7
github: none
---

- [ ] **641 — the Windows leg's sanitized run of six recursion cases does not say `stack-overflow`** | `run/fixedbugs-508-*`, `fixedbugs-520-*` (2) and `fixedbugs-547-*` (3) at `--sanitize` on Windows x86-64: *must be caught by the sanitiser saying `stack-overflow`, and it said:* nothing; the same cases pass at -O0 and -O2 there and on the three other legs; AddressSanitizer on Windows reports a stack overflow differently or not at all, a question for the box | `tests/harness/suite_run.hero`'s sanitizer expectation and the runtime's stack guard under ASan on Windows; the Windows box · **class: blocking**

    **Origin:** filed by the coordinator at 04:41 on 2026-10-11 from the CI's Windows x86-64 leg of batch 20's push (`4fb8bd98`, run 38101444166, job 114358017019, its log through the API at `.claude/worktrees/scratch-b15/ci-4fb8/`, ignored by git), the first run of that leg to reach the net past the compiler's own tests (1,607 passed there), as `.claude/rules/verification.md` § The optimistic chain item 5 asks.

    **Class: blocking**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): a red CI.

    Repaired at `14e53aabd05afcf6055f6d7ef2939f5ac93b18a7`, 2026-10-11 (lane b21-win), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Measured on the Windows box with the CI's clang 20.1.8 and link.exe: the box's clang 23 said ASan's `stack-overflow` every time, but `run/fixedbugs-547-a-self-call-through-map-aborts` built `--sanitize -O0` under clang 20 died 0xC0000005 with nothing on stderr in 3 runs of 8 (the trunk's runtime) and 4 of 8 (a 64 KiB stack guarantee), 4 of 8 at 256 KiB and 1 of 8 at 1 MiB, ASan reporting from its unhandled-exception filter on what is left of the stack; the harness's `run self-call` read it red once in two runs. Under ASan on Windows the runtime now handles `EXCEPTION_STACK_OVERFLOW` alone (`runtime/parts/stack.c`), `panic: stack exhausted` in 12 runs of 12 (547 `-O0`, 508 `-O2`), every other fault ASan's (171's null call, 396's overrun: `on unknown address` 12 of 12), the guarantee asked on every thread; and the suite judges a Windows `!sanitizer: stack-overflow` run by the case's ending (`platform_skips.runtime_tells`), as its `-O0` and `-O2` runs. Box: `run self-call` 6/0, `a-cycle-with` 1/0, `fixedbugs-171` 5/0, `fixedbugs-396` 11/0, `dead-handle` 15/0, `c-frees-a-lease` 3/0; Mac: `runtime` 8/0, the six cases 7/0 narrowed, POSIX unchanged.
