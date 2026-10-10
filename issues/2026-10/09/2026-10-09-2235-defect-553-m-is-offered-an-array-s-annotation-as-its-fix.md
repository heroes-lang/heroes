---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 4156ee2946729c46567e9abecd0462a0b5140541
github: none
---

- [x] **553 — `m = {}` is offered an array's annotation as its fix** | an empty map literal with nothing to type it gets the fix *annotate the binding: `xs: [i64] = []`*, an array's example for a map (lane b17-check) | the empty-literal fix of `selfhost/check/` · **class: adjacent**

    **Origin:** filed by the coordinator at 22:35 on 2026-10-09 from lane b17-check's final report (its notes `.claude/worktrees/scratch-b15/b17-check/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a fix less exact than it could be.

    Repaired at `4156ee29`, 2026-10-09 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Each empty literal has its own constructor in `contextless_errors.hero`, `empty_array_literal` and `empty_map_literal`, the map's guess *annotate the binding: `m: {str: i64} = {}`*. Case `full/fixedbugs-553-…`, five map shapes red on the base and the array's example standing; `check` 648, `full` 32, `permissive` 16, the compiler's 1,544 tests, 0 failed.

## The repair

Repaired at `4156ee29`, 2026-10-09 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Each empty literal has its own constructor in `contextless_errors.hero`, `empty_array_literal` and `empty_map_literal`, the map's guess *annotate the binding: `m: {str: i64} = {}`*. Case `full/fixedbugs-553-…`, five map shapes red on the base and the array's example standing; `check` 648, `full` 32, `permissive` 16, the compiler's 1,544 tests, 0 failed.

**Closed 2026-10-10** with batch 18 (lanes b18-close, b18-infer, b18-ffi and b18-guard, merged into the round `lane-round-b18` with the trunk), its closing gate run on the round at `f6528c53`: the seed regenerated over two generations, the runtime's ABI at 30, 50,640,450 bytes, SHA-256 beginning `3bfbddd0f618b118`, its fixpoint by `cmp`; the compiler's own tests 1,577, all passed; the net's own tests 332, all passed; the full net 7,872 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after, its one red `order` on a walk of defect 570's `cli/pragma_ask.hero` with no `# ORDER:` mark, the mark written on its function's doc line (no line moved, the fixpoint re-checked by `cmp`) and `order` 3 and 0 after; eight floors told outgrown and raised in the closing commit, `order`, `runtime`, `emit`, `unsupported` and `probe` 3, 8, 12, 237 and 27, all 0 failed, after it. Defect 558's case, the one emission this Mac skips, was blessed and read green on Linux arm64 at the same commit (1,169 and 0). Under the optimistic chain the census and panel 187's R2 run after the push beside the CI, and a CI leg red on a closed defect's case files a new `blocking` defect naming it.
