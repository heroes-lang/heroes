---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: 7dd2ac34b91e3361ebefdab5c3c672befd22ab84
github: none
---

- [ ] **437 — a `heroes` ended by SIGKILL still leaves its program running** | no process can catch SIGKILL, so a `heroes` killed that way still orphans its child after defect 425's repair; Linux offers `PR_SET_PDEATHSIG`, which the lane did not add, and macOS has no equal | `runtime/parts/run.c`, the child before exec · **class: improvement**

    **Origin:** filed by the coordinator at 04:40 on 2026-10-07, from lane b13-tmpl407's report (*found beside*); the lane's reading, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): hardening of the shape 425 closed for every catchable signal.

    Repaired at `7dd2ac34`, 2026-10-07 (lane b14-runtime), gated by its cases and the compiler's own tests; the net is owed at the batch's close. On Linux `hero_run_child` asks between fork and exec for SIGKILL when the thread that forked it ends (`prctl(PR_SET_PDEATHSIG)`) and kills itself where its parent is already gone, so a chain of runtime-started processes ends link by link: on Linux arm64 a `sleep 30` run through `hero_run_go` outlived its runner's `kill -9` 3 runs of 3 at `dad2da47` and was gone 3 of 3 after (one hand-written C over `runtime/`); `run/fixedbugs-437-*` binds `sys/prctl.h`, skipped on the Mac and on Windows, judged at the round's Linux leg, its emission owed a blessing there. The Mac's answer is none in the kernel: no request survives exec as a watcher, and a sentinel process per runner (its own session, a pipe's read end, the groups it was told killed on end-of-file, a start time against a reused id) ended a `sleep` after its runner's `kill -9` 3 runs of 3 as a prototype in hand-written C, about 0.85M instructions for its one fork, a design question left to the coordinator.
