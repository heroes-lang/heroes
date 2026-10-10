---
kind: defect
area: runtime
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **573 — a program that leaves the launch builder's buffer live is told at exit that the runtime has a bug** | a program binding `hero_os.h`'s `hero_run_reset`, `hero_run_arg` and `hero_run_go` that never calls `hero_run_reset()` after its launch dies at exit 134 with *panic: 1 scratch buffers still live at exit (a runtime call kept what it borrowed) — this is a runtime bug*; the same program ending with `hero_run_reset()` exits 0; reproduced by the coordinator at 04:28 with the trunk's compiler (`.claude/worktrees/scratch-b15/r573/a.hero`, `c.hero`) | the runtime's exit check of its scratch buffers, `runtime/` · **class: blocking**

    **Origin:** found by lane b18-close beside defect 465 (its final reply and notes, `.claude/worktrees/scratch-b15/b18-close/notes.txt`, ignored by git), filed by the coordinator at 04:29 on 2026-10-10.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a false message: the program's mistake told as the runtime's.
