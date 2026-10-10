---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: 5a2d7cf11e8d7d8520874ce542b36d12761f7719
github: none
---

- [x] **465 — macOS has no way to end a program whose `heroes` was killed with SIGKILL** | defect 437 closed it on Linux with `PR_SET_PDEATHSIG`; on the Mac lane b14-runtime built and measured a sentinel process per runner holding a pipe, which killed the child 3 of 3 for about 0.85 million instructions a fork (`<scratchpad>/batch14/runtime/sentinel/sentinel.c`); it adds a long-lived process to every `heroes` that launches anything | `runtime/parts/run.c` · defect 437 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-runtime's final report (*decisions* 2); the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a design question, a sitting's.

    Repaired at `5a2d7cf1`, 2026-10-10 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 200's R2, route A with every item the critic owed it: a sentinel per runner on Darwin (`defined(__APPLE__)`, `runtime/parts/run.c`) holding a pipe only the runner writes, each child named before exec and unnamed before its reap, the one past 1,024 included; the write end `F_SETNOSIGPIPE` and `O_NONBLOCK`, a sentinel gone restarted under a lock with the table named again. On this Mac the child ends 10 of 10 (0 of 5 on the base); `kill -9` of the sentinel leaves its runner alive 5 of 5 and the next sentinel ends the child; `yes` 3 of 3, a chain 4 of 4, a raylib program with a hidden window 3 of 3 with no fork-safety message; Linux arm64 and the Windows box compile it unchanged. Case `run/fixedbugs-465-…`; `runtime` 8, `run` 426, `cache` 7, the compiler's 1,546 tests, 0 failed. Cost: `heroes run hello` 540.3M to 578.8M instructions, one fork of the runner.

## The repair

Repaired at `5a2d7cf1`, 2026-10-10 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 200's R2, route A with every item the critic owed it: a sentinel per runner on Darwin (`defined(__APPLE__)`, `runtime/parts/run.c`) holding a pipe only the runner writes, each child named before exec and unnamed before its reap, the one past 1,024 included; the write end `F_SETNOSIGPIPE` and `O_NONBLOCK`, a sentinel gone restarted under a lock with the table named again. On this Mac the child ends 10 of 10 (0 of 5 on the base); `kill -9` of the sentinel leaves its runner alive 5 of 5 and the next sentinel ends the child; `yes` 3 of 3, a chain 4 of 4, a raylib program with a hidden window 3 of 3 with no fork-safety message; Linux arm64 and the Windows box compile it unchanged. Case `run/fixedbugs-465-…`; `runtime` 8, `run` 426, `cache` 7, the compiler's 1,546 tests, 0 failed. Cost: `heroes run hello` 540.3M to 578.8M instructions, one fork of the runner.

**Closed 2026-10-10** with batch 18 (lanes b18-close, b18-infer, b18-ffi and b18-guard, merged into the round `lane-round-b18` with the trunk), its closing gate run on the round at `f6528c53`: the seed regenerated over two generations, the runtime's ABI at 30, 50,640,450 bytes, SHA-256 beginning `3bfbddd0f618b118`, its fixpoint by `cmp`; the compiler's own tests 1,577, all passed; the net's own tests 332, all passed; the full net 7,872 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after, its one red `order` on a walk of defect 570's `cli/pragma_ask.hero` with no `# ORDER:` mark, the mark written on its function's doc line (no line moved, the fixpoint re-checked by `cmp`) and `order` 3 and 0 after; eight floors told outgrown and raised in the closing commit, `order`, `runtime`, `emit`, `unsupported` and `probe` 3, 8, 12, 237 and 27, all 0 failed, after it. Defect 558's case, the one emission this Mac skips, was blessed and read green on Linux arm64 at the same commit (1,169 and 0). Under the optimistic chain the census and panel 187's R2 run after the push beside the CI, and a CI leg red on a closed defect's case files a new `blocking` defect naming it.
