---
kind: defect
area: runtime
milestone: none
filed: 2026-10-10
commit: 408330b319f40cf065b413aa445b654b93edb988
github: none
---

- [ ] **575 — the macOS sentinel of each runner costs a fork of `heroes`, and a runner killed between its sentinel's death and its next launch leaves one child unguarded** | defect 465's repair (`5a2d7cf1`): one sentinel process per runner on macOS; `heroes run hello` goes from 540.3M to 578.8M instructions retired (+7.1%), a warm build of the compiler unchanged; if the sentinel is killed and then the runner before its next launch or reap, that one child is not ended with it; a sentinel started by `posix_spawn` would remove both, the lane's recommendation, for a sitting | `runtime/parts/run.c`, the macOS arm of defect 465's repair · **class: improvement**

    **Origin:** found by lane b18-close beside defect 465 (its final reply and notes, `.claude/worktrees/scratch-b15/b18-close/notes.txt`, ignored by git), filed by the coordinator at 04:29 on 2026-10-10.

    **Class: improvement**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): hardening nobody needs to be right; the batch's question 1, answered the recommended way: the cost accepted.

    Repaired at `408330b3`, 2026-10-10 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The cost as filed did not reproduce: on this tree `heroes run hello` read 543.95M instructions with no sentinel, 546.74M with defect 465's fork (+0.5%) and 551.53M spawned (+1.4%), the same source built three ways; what the fork costs grows with the runner instead, a program holding 1 GB before its first launch 25.89e9, 26.23e9 forked and 25.89e9 spawned. So the sentinel is the system's `/bin/sh`, started by `posix_spawn` with exactly its descriptors and ten lines of POSIX sh (`runtime/parts/run.c`), and a thread of the runner reading the sentinel's life line replaces it the moment it ends, naming the table to the new one. `kill -9` of the sentinel and then of `heroes`: the child ended 0 of 5 with defect 465's sentinel, 5 of 5 now; every trial 465 passed passes again (10 of 10, the runner alive 5 of 5, `yes` 3 of 3, the chain 4 of 4, raylib 3 of 3); `runtime` 8, `run` 436, `cache` 7, the compiler's 1,546 tests, 0 failed; Linux arm64 and the Windows box compile it unchanged.
