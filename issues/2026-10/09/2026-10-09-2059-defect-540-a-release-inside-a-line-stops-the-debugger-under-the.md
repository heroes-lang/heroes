---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: 74e3d457befd161819214c5fadeae970a32fbe1f
github: none
---

- [x] **540 — a release inside a line stops the debugger under the generated file** | with the prologue carrying the function's line, `step`, `next`, `next` stops at `dbg.hero:1`, `dbg.hero:2:10`, then `dbg.c:70:5`, a release inside line 2 mapped to the generated file (panel 200's compiler-engineer, lldb on this Mac) | the release's `#line` in `selfhost/emit/` · panel 200 R4 · defect 472 · **class: improvement**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 200 (`docs/panel/200-a-counted-slot-is-released-by-its-address-and-the-emitted-program-s-other-routes-are-ruled.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a debugger stop less exact than it could be.

    Repaired at `74e3d457`, 2026-10-10 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A retain or a release inside a line is written under that line (`emit/release.hero`'s `named`), and the exit keeps the generated file as defect 335 ruled: a merged exit's load, the retain of what a way out returns (`term.retains_returned`) and the sweep. lldb on this Mac, `next` through `greet`: before 2:10, dbg.c, 2:11, dbg.c, 2:12, dbg.c, 3:10; after 2:10, 3:10, 5:10, 7:11, then the exit. Case `emit/fixedbugs-540-…`; 313 blessed emissions and six `emit` goldens moved, `#line` lines only; `emission` 1086, `emit` 12, `lines` 426, the compiler's 1,545 tests, 0 failed; the compiler's C 47,130,136 to 48,295,204 bytes.

## The repair

Repaired at `74e3d457`, 2026-10-10 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A retain or a release inside a line is written under that line (`emit/release.hero`'s `named`), and the exit keeps the generated file as defect 335 ruled: a merged exit's load, the retain of what a way out returns (`term.retains_returned`) and the sweep. lldb on this Mac, `next` through `greet`: before 2:10, dbg.c, 2:11, dbg.c, 2:12, dbg.c, 3:10; after 2:10, 3:10, 5:10, 7:11, then the exit. Case `emit/fixedbugs-540-…`; 313 blessed emissions and six `emit` goldens moved, `#line` lines only; `emission` 1086, `emit` 12, `lines` 426, the compiler's 1,545 tests, 0 failed; the compiler's C 47,130,136 to 48,295,204 bytes.

**Closed 2026-10-10** with batch 18 (lanes b18-close, b18-infer, b18-ffi and b18-guard, merged into the round `lane-round-b18` with the trunk), its closing gate run on the round at `f6528c53`: the seed regenerated over two generations, the runtime's ABI at 30, 50,640,450 bytes, SHA-256 beginning `3bfbddd0f618b118`, its fixpoint by `cmp`; the compiler's own tests 1,577, all passed; the net's own tests 332, all passed; the full net 7,872 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after, its one red `order` on a walk of defect 570's `cli/pragma_ask.hero` with no `# ORDER:` mark, the mark written on its function's doc line (no line moved, the fixpoint re-checked by `cmp`) and `order` 3 and 0 after; eight floors told outgrown and raised in the closing commit, `order`, `runtime`, `emit`, `unsupported` and `probe` 3, 8, 12, 237 and 27, all 0 failed, after it. Defect 558's case, the one emission this Mac skips, was blessed and read green on Linux arm64 at the same commit (1,169 and 0). Under the optimistic chain the census and panel 187's R2 run after the push beside the CI, and a CI leg red on a closed defect's case files a new `blocking` defect naming it.
