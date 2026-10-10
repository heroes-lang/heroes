---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: cac4fc0b607b583f04653138d5bf6e86a8710600
github: none
---

- [x] **557 — a macro in one header and a function of its name in another are told that one header does not compile** | the critic's `macro`: `a.h` `#define twice(x)`, `b.h` `static inline int twice`; `test` exits 1 with the old false message *`b.h` ... does not compile: expected identifier or '('*, and with the `use` lines swapped the program passes every tool: a false message, and a verdict that depends on `use` order | `selfhost/cli/headers_together.hero` · defects 538 and 550 · panel 202 · **class: blocking**

    **Origin:** filed by the coordinator at 23:11 on 2026-10-09 from panels 202 and 203's completeness critic, first pass (its report committed with the sitting, its cases under `.claude/worktrees/scratch-b15/critic-202-203/p202/`, ignored by git); the critic's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a false message, the shape beside defect 550.

    Repaired at `cac4fc0b607b583f04653138d5bf6e86a8710600`, 2026-10-10, gated by its cases and the compiler's own tests; the net is owed at the batch's close. A header clang refused in a unit is compiled alone on the failure path, and where it compiles, after each header its unit read before it (`cli/header_alone.hero`): the first it does not compile after is named, *`b.h`, the header this group names, compiles alone and not after `a.h`*, as 538's class, `ffi_header_refused`; the critic's `macro` in `test` is told so, and the other order passes, a one-unit `test`'s verdict until defect 560 makes it per module. Case `unsupported/fixedbugs-557-*` (one module, red on the base); unsupported 203 and 0, the compiler's own tests 1,548 passed.

## The repair

Repaired at `cac4fc0b607b583f04653138d5bf6e86a8710600`, 2026-10-10, gated by its cases and the compiler's own tests; the net is owed at the batch's close. A header clang refused in a unit is compiled alone on the failure path, and where it compiles, after each header its unit read before it (`cli/header_alone.hero`): the first it does not compile after is named, *`b.h`, the header this group names, compiles alone and not after `a.h`*, as 538's class, `ffi_header_refused`; the critic's `macro` in `test` is told so, and the other order passes, a one-unit `test`'s verdict until defect 560 makes it per module. Case `unsupported/fixedbugs-557-*` (one module, red on the base); unsupported 203 and 0, the compiler's own tests 1,548 passed.

**Closed 2026-10-10** with batch 18 (lanes b18-close, b18-infer, b18-ffi and b18-guard, merged into the round `lane-round-b18` with the trunk), its closing gate run on the round at `f6528c53`: the seed regenerated over two generations, the runtime's ABI at 30, 50,640,450 bytes, SHA-256 beginning `3bfbddd0f618b118`, its fixpoint by `cmp`; the compiler's own tests 1,577, all passed; the net's own tests 332, all passed; the full net 7,872 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after, its one red `order` on a walk of defect 570's `cli/pragma_ask.hero` with no `# ORDER:` mark, the mark written on its function's doc line (no line moved, the fixpoint re-checked by `cmp`) and `order` 3 and 0 after; eight floors told outgrown and raised in the closing commit, `order`, `runtime`, `emit`, `unsupported` and `probe` 3, 8, 12, 237 and 27, all 0 failed, after it. Defect 558's case, the one emission this Mac skips, was blessed and read green on Linux arm64 at the same commit (1,169 and 0). Under the optimistic chain the census and panel 187's R2 run after the push beside the CI, and a CI leg red on a closed defect's case files a new `blocking` defect naming it.
