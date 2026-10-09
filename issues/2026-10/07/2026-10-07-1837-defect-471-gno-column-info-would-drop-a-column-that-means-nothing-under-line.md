---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: 8ac4e17dc59c42a04369e10011dc3c12ab27602e
github: none
---

- [ ] **471 — `-gno-column-info` would drop a column that means nothing under `#line`** | a column under `#line` is the C file's column printed against a `.hero` line (lldb draws its caret under the wrong character); (K) cuts objects 5.95% at 400 returns, `-O0`; UBSan's false column comes from the front end and survives it; Linux ASan's column and CodeView's unmeasured (panel 197's route (K)) | `selfhost/cli/flags.hero` · panel 197's R7 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from panel 197's R7.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a debugger's display, less exact than it could be.

    Measured at `8ac4e17d`, 2026-10-09 (lane b16-compiler), gated by the compiler's own tests; the net is owed at the batch's close. The measurement is the repair: `-gno-column-info` drops the false `.hero` column and moves no file or line on any platform, and it drops TRUE columns with it, a binding's header (`bind.h:15:24` in lldb, `bind.h:15:12` in Linux arm64's AddressSanitizer), the runtime (`panic.c:49:60`) and the emitted C (`uaf.c:78:5`), while UBSan keeps its own (`bind.h:19:14`); CodeView writes column 0 with and without it, so on Windows it is a no-op. The column a tool shows is worse, so the word is not landed; what lands is a test in `selfhost/cli/flags.hero` that each level's words keep a column and that a `#line` carries the C file's, red on a compiler carrying the word. The gain it would have bought at 100, 200 and 400 returns, `-O0`: objects 9.4% to 9.5% smaller, clang 3.0% to 4.6% fewer instructions retired.
