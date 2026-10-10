---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: cac4fc0b607b583f04653138d5bf6e86a8710600
github: none
---

- [x] **561 — defect 538's note says *whichever comes first*, false, and its remedy led two readers to edit a correct program** | `rlmath1` (raymath.h bound first, raylib.h second) fails `test` with the note *whichever comes first*, and `rlmath2`, the same two swapped, passes `test`, and so `tasn1a` and `tasn1b` (libtasn1 and OpenSSL); the blind seat's two readers given the message on `fp` both edited a correct program, one writing `use left` and `left.twice(...)`, which `check` refuses `extern_across_modules`, and both argued its second remedy, *a header of your own*, cannot work where the other header defines the name | `selfhost/cli/headers_together.hero` and `header_refused.hero` · panel 202 R2 · **class: blocking**

    **Origin:** filed by the coordinator at 00:25 on 2026-10-10 from panel 202 (`docs/panel/202-every-verb-cuts-a-program-s-c-by-module-and-what-two-modules-headers-disagree-on-is-told-before-the-link.md`): the ffi-pragmatist's `rlmath` and `tasn1` cases, and the blind seat's `d3-a` and `d3-b` as the critic's second pass read them, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a false message.

    Repaired at `cac4fc0b607b583f04653138d5bf6e86a8710600`, 2026-10-10, gated by its cases and the compiler's own tests; the net is owed at the batch's close. 538's note no longer says *whichever comes first* nor offers *a header of your own*: where the two groups are one module's it says to bind them from two modules, whose units each read their own groups' headers, and where they are two modules' already, which unit reads both (`cli/headers_together.hero`); `rlmath1` and `tasn1a` are told so in `test`, `rlmath2` and `tasn1b` pass it, measured with `CPATH=/opt/homebrew/include`. The four `fixedbugs-538` and `fixedbugs-550` expectations moved by the note; unsupported 203 and 0, the compiler's own tests 1,548 passed. Whether an order of one module's groups compiles where the written one does not is panel 204's R3, defect 563's.

## The repair

Repaired at `cac4fc0b607b583f04653138d5bf6e86a8710600`, 2026-10-10, gated by its cases and the compiler's own tests; the net is owed at the batch's close. 538's note no longer says *whichever comes first* nor offers *a header of your own*: where the two groups are one module's it says to bind them from two modules, whose units each read their own groups' headers, and where they are two modules' already, which unit reads both (`cli/headers_together.hero`); `rlmath1` and `tasn1a` are told so in `test`, `rlmath2` and `tasn1b` pass it, measured with `CPATH=/opt/homebrew/include`. The four `fixedbugs-538` and `fixedbugs-550` expectations moved by the note; unsupported 203 and 0, the compiler's own tests 1,548 passed. Whether an order of one module's groups compiles where the written one does not is panel 204's R3, defect 563's.

**Closed 2026-10-10** with batch 18 (lanes b18-close, b18-infer, b18-ffi and b18-guard, merged into the round `lane-round-b18` with the trunk), its closing gate run on the round at `f6528c53`: the seed regenerated over two generations, the runtime's ABI at 30, 50,640,450 bytes, SHA-256 beginning `3bfbddd0f618b118`, its fixpoint by `cmp`; the compiler's own tests 1,577, all passed; the net's own tests 332, all passed; the full net 7,872 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after, its one red `order` on a walk of defect 570's `cli/pragma_ask.hero` with no `# ORDER:` mark, the mark written on its function's doc line (no line moved, the fixpoint re-checked by `cmp`) and `order` 3 and 0 after; eight floors told outgrown and raised in the closing commit, `order`, `runtime`, `emit`, `unsupported` and `probe` 3, 8, 12, 237 and 27, all 0 failed, after it. Defect 558's case, the one emission this Mac skips, was blessed and read green on Linux arm64 at the same commit (1,169 and 0). Under the optimistic chain the census and panel 187's R2 run after the push beside the CI, and a CI leg red on a closed defect's case files a new `blocking` defect naming it.
