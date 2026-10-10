---
kind: defect
area: runtime
milestone: none
filed: 2026-10-10
commit: d30b0176f8dbbdfadb1bf755651770b87edd0e9c
github: none
---

- [x] **573 — a program that leaves the launch builder's buffer live is told at exit that the runtime has a bug** | a program binding `hero_os.h`'s `hero_run_reset`, `hero_run_arg` and `hero_run_go` that never calls `hero_run_reset()` after its launch dies at exit 134 with *panic: 1 scratch buffers still live at exit (a runtime call kept what it borrowed) — this is a runtime bug*; the same program ending with `hero_run_reset()` exits 0; reproduced by the coordinator at 04:28 with the trunk's compiler (`.claude/worktrees/scratch-b15/r573/a.hero`, `c.hero`) | the runtime's exit check of its scratch buffers, `runtime/` · **class: blocking**

    **Origin:** found by lane b18-close beside defect 465 (its final reply and notes, `.claude/worktrees/scratch-b15/b18-close/notes.txt`, ignored by git), filed by the coordinator at 04:29 on 2026-10-10.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a false message: the program's mistake told as the runtime's.

    Repaired at `d30b0176`, 2026-10-10 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The launch list (`runtime/parts/run.c`) and, the shape beside with the same cause, a directory listing (`runtime/parts/dir.c`) each count in a process-wide atomic the scratch they hold between calls, and the exit check (`runtime/parts/alloc.c`) tells those as the program's: *a launch's argument list still holds 2 word(s) at exit — `hero_run_arg` keeps each word until `hero_run_reset()` gives the list back, and this program did not call it after its last `hero_run_arg`*, and the listing's alike with `hero_dir_release()`; only the scratch left after them is told as the runtime's bug. Cases `run/fixedbugs-573-…` (2, each told *a runtime bug* on the base runtime); `runtime` 8, `run` 436, `cache` 7, `warnings` 501, the compiler's 1,546 tests, 0 failed; the runtime compiles on Linux arm64 and the Windows box with -Wall -Wextra -Werror.

## The repair

Repaired at `d30b0176`, 2026-10-10 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The launch list (`runtime/parts/run.c`) and, the shape beside with the same cause, a directory listing (`runtime/parts/dir.c`) each count in a process-wide atomic the scratch they hold between calls, and the exit check (`runtime/parts/alloc.c`) tells those as the program's: *a launch's argument list still holds 2 word(s) at exit — `hero_run_arg` keeps each word until `hero_run_reset()` gives the list back, and this program did not call it after its last `hero_run_arg`*, and the listing's alike with `hero_dir_release()`; only the scratch left after them is told as the runtime's bug. Cases `run/fixedbugs-573-…` (2, each told *a runtime bug* on the base runtime); `runtime` 8, `run` 436, `cache` 7, `warnings` 501, the compiler's 1,546 tests, 0 failed; the runtime compiles on Linux arm64 and the Windows box with -Wall -Wextra -Werror.

**Closed 2026-10-10** with batch 18 (lanes b18-close, b18-infer, b18-ffi and b18-guard, merged into the round `lane-round-b18` with the trunk), its closing gate run on the round at `f6528c53`: the seed regenerated over two generations, the runtime's ABI at 30, 50,640,450 bytes, SHA-256 beginning `3bfbddd0f618b118`, its fixpoint by `cmp`; the compiler's own tests 1,577, all passed; the net's own tests 332, all passed; the full net 7,872 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after, its one red `order` on a walk of defect 570's `cli/pragma_ask.hero` with no `# ORDER:` mark, the mark written on its function's doc line (no line moved, the fixpoint re-checked by `cmp`) and `order` 3 and 0 after; eight floors told outgrown and raised in the closing commit, `order`, `runtime`, `emit`, `unsupported` and `probe` 3, 8, 12, 237 and 27, all 0 failed, after it. Defect 558's case, the one emission this Mac skips, was blessed and read green on Linux arm64 at the same commit (1,169 and 0). Under the optimistic chain the census and panel 187's R2 run after the push beside the CI, and a CI leg red on a closed defect's case files a new `blocking` defect naming it.
