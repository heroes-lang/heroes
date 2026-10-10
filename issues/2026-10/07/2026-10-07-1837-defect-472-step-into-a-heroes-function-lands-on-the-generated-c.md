---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: a3ebe9eb33051f073861bbbeafc5e0f886258e9d
github: none
---

- [x] **472 — `step` into a Heroes function lands on the generated C** | `step` into `square` lands on `dbg.c:34:5` under every debug word (panel 197's compiler-engineer); design.md `:682`'s attribution of prologue code, which an author still sees as C | the emitted prologue's `#line` · design.md §3.1 `:682` · panel 197's R7 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from panel 197's R7 and its critic.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a debugger step less exact than it could be.

    Repaired at `a3ebe9eb`, 2026-10-10 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 200's R4: the thread guard, the prologue and the `goto` to the first block are one physical line under the function's own `#line` (`emit/prologue.hero`, out of `emit/body.hero`), the exit and cleanup staying generated (defect 335); design.md §3.1's sentence amended with its date. lldb on this Mac at `-O0`: `step` into `greet` and a breakpoint on it, `dbg.c:19:13` before and `dbg.hero:1:13` after; at `-O2` both on line 2, no line-1 row mid-function. The `lines` suite now asks every entry of the `run/` corpus to be its signature's line (425 of 426 red with the compiler before, 426 and 0 after); every emission re-blessed and read by kind; the compiler's C 48,311,651 to 47,091,837 bytes and 1,535,984 to 1,207,560 lines, where route D's line per `#line` grew the seed 28%.

## The repair

Repaired at `a3ebe9eb`, 2026-10-10 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 200's R4: the thread guard, the prologue and the `goto` to the first block are one physical line under the function's own `#line` (`emit/prologue.hero`, out of `emit/body.hero`), the exit and cleanup staying generated (defect 335); design.md §3.1's sentence amended with its date. lldb on this Mac at `-O0`: `step` into `greet` and a breakpoint on it, `dbg.c:19:13` before and `dbg.hero:1:13` after; at `-O2` both on line 2, no line-1 row mid-function. The `lines` suite now asks every entry of the `run/` corpus to be its signature's line (425 of 426 red with the compiler before, 426 and 0 after); every emission re-blessed and read by kind; the compiler's C 48,311,651 to 47,091,837 bytes and 1,535,984 to 1,207,560 lines, where route D's line per `#line` grew the seed 28%.

**Closed 2026-10-10** with batch 18 (lanes b18-close, b18-infer, b18-ffi and b18-guard, merged into the round `lane-round-b18` with the trunk), its closing gate run on the round at `f6528c53`: the seed regenerated over two generations, the runtime's ABI at 30, 50,640,450 bytes, SHA-256 beginning `3bfbddd0f618b118`, its fixpoint by `cmp`; the compiler's own tests 1,577, all passed; the net's own tests 332, all passed; the full net 7,872 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after, its one red `order` on a walk of defect 570's `cli/pragma_ask.hero` with no `# ORDER:` mark, the mark written on its function's doc line (no line moved, the fixpoint re-checked by `cmp`) and `order` 3 and 0 after; eight floors told outgrown and raised in the closing commit, `order`, `runtime`, `emit`, `unsupported` and `probe` 3, 8, 12, 237 and 27, all 0 failed, after it. Defect 558's case, the one emission this Mac skips, was blessed and read green on Linux arm64 at the same commit (1,169 and 0). Under the optimistic chain the census and panel 187's R2 run after the push beside the CI, and a CI leg red on a closed defect's case files a new `blocking` defect naming it.
