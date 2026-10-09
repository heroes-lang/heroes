---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: 8ac4e17dc59c42a04369e10011dc3c12ab27602e
github: none
---

- [x] **471 — `-gno-column-info` would drop a column that means nothing under `#line`** | a column under `#line` is the C file's column printed against a `.hero` line (lldb draws its caret under the wrong character); (K) cuts objects 5.95% at 400 returns, `-O0`; UBSan's false column comes from the front end and survives it; Linux ASan's column and CodeView's unmeasured (panel 197's route (K)) | `selfhost/cli/flags.hero` · panel 197's R7 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from panel 197's R7.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a debugger's display, less exact than it could be.

    Measured at `8ac4e17d`, 2026-10-09 (lane b16-compiler), gated by the compiler's own tests; the net is owed at the batch's close. The measurement is the repair: `-gno-column-info` drops the false `.hero` column and moves no file or line on any platform, and it drops TRUE columns with it, a binding's header (`bind.h:15:24` in lldb, `bind.h:15:12` in Linux arm64's AddressSanitizer), the runtime (`panic.c:49:60`) and the emitted C (`uaf.c:78:5`), while UBSan keeps its own (`bind.h:19:14`); CodeView writes column 0 with and without it, so on Windows it is a no-op. The column a tool shows is worse, so the word is not landed; what lands is a test in `selfhost/cli/flags.hero` that each level's words keep a column and that a `#line` carries the C file's, red on a compiler carrying the word. The gain it would have bought at 100, 200 and 400 returns, `-O0`: objects 9.4% to 9.5% smaller, clang 3.0% to 4.6% fewer instructions retired.

## The repair

Measured at `8ac4e17d`, 2026-10-09 (lane b16-compiler), gated by the compiler's own tests; the net is owed at the batch's close. The measurement is the repair: `-gno-column-info` drops the false `.hero` column and moves no file or line on any platform, and it drops TRUE columns with it, a binding's header (`bind.h:15:24` in lldb, `bind.h:15:12` in Linux arm64's AddressSanitizer), the runtime (`panic.c:49:60`) and the emitted C (`uaf.c:78:5`), while UBSan keeps its own (`bind.h:19:14`); CodeView writes column 0 with and without it, so on Windows it is a no-op. The column a tool shows is worse, so the word is not landed; what lands is a test in `selfhost/cli/flags.hero` that each level's words keep a column and that a `#line` carries the C file's, red on a compiler carrying the word. The gain it would have bought at 100, 200 and 400 returns, `-O0`: objects 9.4% to 9.5% smaller, clang 3.0% to 4.6% fewer instructions retired.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
