---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: 7dd2ac34b91e3361ebefdab5c3c672befd22ab84
github: none
---

- [x] **437 — a `heroes` ended by SIGKILL still leaves its program running** | no process can catch SIGKILL, so a `heroes` killed that way still orphans its child after defect 425's repair; Linux offers `PR_SET_PDEATHSIG`, which the lane did not add, and macOS has no equal | `runtime/parts/run.c`, the child before exec · **class: improvement**

    **Origin:** filed by the coordinator at 04:40 on 2026-10-07, from lane b13-tmpl407's report (*found beside*); the lane's reading, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): hardening of the shape 425 closed for every catchable signal.

    Repaired at `7dd2ac34`, 2026-10-07 (lane b14-runtime), gated by its cases and the compiler's own tests; the net is owed at the batch's close. On Linux `hero_run_child` asks between fork and exec for SIGKILL when the thread that forked it ends (`prctl(PR_SET_PDEATHSIG)`) and kills itself where its parent is already gone, so a chain of runtime-started processes ends link by link: on Linux arm64 a `sleep 30` run through `hero_run_go` outlived its runner's `kill -9` 3 runs of 3 at `dad2da47` and was gone 3 of 3 after (one hand-written C over `runtime/`); `run/fixedbugs-437-*` binds `sys/prctl.h`, skipped on the Mac and on Windows, judged at the round's Linux leg, its emission owed a blessing there. The Mac's answer is none in the kernel: no request survives exec as a watcher, and a sentinel process per runner (its own session, a pipe's read end, the groups it was told killed on end-of-file, a start time against a reused id) ended a `sleep` after its runner's `kill -9` 3 runs of 3 as a prototype in hand-written C, about 0.85M instructions for its one fork, a design question left to the coordinator.

## The repair

Repaired at `7dd2ac34`, 2026-10-07 (lane b14-runtime), gated by its cases and the compiler's own tests; the net is owed at the batch's close. On Linux `hero_run_child` asks between fork and exec for SIGKILL when the thread that forked it ends (`prctl(PR_SET_PDEATHSIG)`) and kills itself where its parent is already gone, so a chain of runtime-started processes ends link by link: on Linux arm64 a `sleep 30` run through `hero_run_go` outlived its runner's `kill -9` 3 runs of 3 at `dad2da47` and was gone 3 of 3 after (one hand-written C over `runtime/`); `run/fixedbugs-437-*` binds `sys/prctl.h`, skipped on the Mac and on Windows, judged at the round's Linux leg, its emission owed a blessing there. The Mac's answer is none in the kernel: no request survives exec as a watcher, and a sentinel process per runner (its own session, a pipe's read end, the groups it was told killed on end-of-file, a start time against a reused id) ended a `sleep` after its runner's `kill -9` 3 runs of 3 as a prototype in hand-written C, about 0.85M instructions for its one fork, a design question left to the coordinator.

**Closed 2026-10-08**, after the push's platform legs, this defect being at the C boundary (`.claude/rules/verification.md` § The batch): batch 14 closed on this Mac alone and the CI's legs ran its cases afterwards. The CI's four legs on `ee95a6f0` (run 37735987684, created at 08:08 and its Windows leg finished at 10:22 on 2026-10-08) are all green: Darwin arm64 with the net at 6,809 passed and 0 failed, Linux arm64 and Linux x86-64 at 6,790 each, Windows x86-64 at 6,648, and on every leg the compiler's own tests 1,430, the module's 260 and the net's own tests 308, all passed. A case bound to one platform ran where it is bound, read from the legs' logs: the SDL3 event case of defect 213 and the `sys/prctl.h` case of defect 437 are not among the SKIP lines of either Linux leg (they are, as they must be, on Darwin and on Windows), and the Linux legs built SDL3 from source and checked that `pkg-config` answers 3.2.10 before the net started. The leg that had read red on `02256c1e`, Windows, did so on defect 505's test of the order of legs, repaired at `ee95a6f0`.
