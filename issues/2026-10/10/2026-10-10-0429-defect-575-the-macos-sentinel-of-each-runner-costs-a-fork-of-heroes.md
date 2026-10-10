---
kind: defect
area: runtime
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **575 — the macOS sentinel of each runner costs a fork of `heroes`, and a runner killed between its sentinel's death and its next launch leaves one child unguarded** | defect 465's repair (`5a2d7cf1`): one sentinel process per runner on macOS; `heroes run hello` goes from 540.3M to 578.8M instructions retired (+7.1%), a warm build of the compiler unchanged; if the sentinel is killed and then the runner before its next launch or reap, that one child is not ended with it; a sentinel started by `posix_spawn` would remove both, the lane's recommendation, for a sitting | `runtime/parts/run.c`, the macOS arm of defect 465's repair · **class: improvement**

    **Origin:** found by lane b18-close beside defect 465 (its final reply and notes, `.claude/worktrees/scratch-b15/b18-close/notes.txt`, ignored by git), filed by the coordinator at 04:29 on 2026-10-10.

    **Class: improvement**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): hardening nobody needs to be right; the batch's question 1, answered the recommended way: the cost accepted.
