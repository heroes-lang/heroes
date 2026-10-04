---
kind: defect
area: emit
milestone: none
filed: 2026-10-02
commit: 744cdc085fd84a79f50400838aa4c3bfe70e15b2
github: none
---

# Defect 158 closed: a group's header that includes a header this machine lacks stops `build` with an internal error and clang's text

- [x] **158 — a group's header that includes a header this machine lacks stops `build` with an internal error and clang's text** | `extern "outer.h"` over a header holding `#include <no_such_header_here.h>`: `check` exit 0, `build` exit 2, *internal error: compiling the generated C failed: In file included from ...: ./outer.h:2:10: fatal error: 'no_such_header_here.h' file not found*, where a missing header the group names itself is told `ffi_missing_header` at exit 1 | `selfhost/emit/ffi_build.hero` (where `ffi_missing_header` is told) · `tests/golden/run/fixedbugs-143-system-macros-through-functions-of-the-programs-own.hero`, red on the Windows box · **class: blocking** · **closed 2026-10-03**

    **Origin:** the coordinator, 2026-10-02, reading the Windows
    box's pre-push leg on `2bb45a96` (16 of 19 suites green; `run`,
    `emission` and `determinism` each 1 failed, all on lane ffi-macro's new
    run case, whose header includes `sys/wait.h` and `sys/select.h`, absent
    on Windows; the leg's log reads its exit at 15:52), then reproduced on
    this Mac before 15:55, the filing commit's time, on the trunk at
    `4d0f27a1` (`docs/panel/186-briefs/probes/coordinator/nested.hero` and
    `outer.h`).

    **Why it is a defect.** Exit 2 is the compiler blaming itself for the
    machine's fact (`.claude/rules/c-boundary.md`), and clang's text reaches
    the author; the run suite skips a case only on `ffi_missing_header`
    (`tests/harness/shell.hero`'s `machine_lacks_the_library`), so the same
    fact also turns a platform's correct skip into a red that would reach
    the CI's Windows leg at the next push.

    **2026-10-02, lane h158, a header the group's own header includes is
    told on the group from clang's line alone, and one reached through other
    headers from the include stack above it**: repaired at `744cdc08` (the
    line) and `7790f2f6` (the stack, the call in `selfhost/emit/ffi.hero`
    handing clang's whole stderr), gated by their cases, the first also by the
    compiler's own tests; the net is owed at the batch's close.

    **2026-10-03, its cases read one by one, held for the Windows box**: all fourteen `unsupported/fixedbugs-158-*` passed on this Mac (00:30 to 00:42 by `date`) and in the Linux arm64 container (one run, 00:36 to 00:54), and `docs/panel/186-briefs/probes/coordinator/nested.hero` is told `ffi_missing_header` on `outer.h`'s line 2 at exit 1 on both, with the ten tests of `744cdc08` and `7790f2f6` `ok` there and in the four jobs of the CI's run 37065944766. Every one of the fourteen EXPECTS `ffi_missing_header`, so on a machine where one's text differs from its `.expected` the harness skips it rather than failing it (`tests/harness/suite_golden.hero:210`), and the Windows box is the machine this defect was found on: its leg of `6bec7c8c` (`unsupported` 113 at 0 failed against 119) leaves exactly one skip between these fourteen and defect 156's curl case, and the box did not answer on 2026-10-03 to say which. The item waits for the box to read the fourteen one by one.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): exit 2 and clang's
    text for the machine's own fact; it would turn the CI's Windows leg red.

    **Closed 2026-10-03** after the push of `64befd50`. Repaired at `744cdc08` and `7790f2f6` in lane h158, it entered the trunk at the round of 2026-10-02's third gate, `6bec7c8c` (lanes bounded, h158 and land186): the seed regenerated once, 38,648,442 bytes, SHA-256 beginning `568bce290b6ed3bb`, its fixpoint by `cmp`; the compiler's own tests 1,056 and the net's own 197, all passed; the full net, 26 suites, 4,752 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,835 files and of `--emit-c` over 570, every move attributed to its lane. Its cases read one by one at `02e507bc`, whose compiler is the one `64befd50` carries (the seeds differ in `#line` numbers alone, and the compiler built from each is the same binary on this Mac and in the Linux arm64 container under clang 22.1.8 and 18.1.8 by `cmp`, the same assembly on the Windows box): on this Mac (Apple clang 21), in the Linux arm64 container under Debian clang 22.1.8 and again under 18.1.8, and on the Windows box (clang 23.1.1), all fourteen `unsupported/fixedbugs-158-*` passed on each, none skipped, the Windows box's reading included, which the paragraph held for it had left unread; `docs/panel/186-briefs/probes/coordinator/nested.hero` was told `ffi_missing_header` on `outer.h`'s line 2 at exit 1 on each; and the compiler's own tests, the ten of `744cdc08` and `7790f2f6` among them, all passed on this Mac and in both Linux arm64 runs. The push's legs at `02e507bc`: Linux arm64, the compiler's own tests 1,083 all passed and 20 suites at 0 failed under each clang; the Windows box, 20 suites at 0 failed. Linux x86-64, the CI's job on `64befd50` (run 37118626800, Ubuntu clang 18.1.3): the compiler's own tests 1,083 all passed and 26 suites at 0 failed; which of its golden cases that job ran, its log does not say, so Linux arm64 under the same clang major is the nearest measurement of them, an inference for x86-64. It closes after the push's platform legs ran its cases (`.claude/rules/verification.md` § The batch).
