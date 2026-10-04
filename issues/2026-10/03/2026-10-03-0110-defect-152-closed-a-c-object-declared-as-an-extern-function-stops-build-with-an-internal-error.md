# Defect 152 closed: a C object declared as an `extern` `function` stops `build` with an internal error

- [x] **152 — a C object declared as an `extern` `function` stops `build` with an internal error** | `extern "errno.h"` with `function errno() -> i32`: `build` exit 2, *internal error: compiling the generated C failed: ... called object type 'int' is not a function or function pointer* at the result probe; the same for `stdin` and `optarg`, with or without parameters, on macOS and Linux (the seat's), where the same names declared `constant` get a clean exit 1 | `selfhost/emit/extern_probe.hero` (the result probe) · the `extern` member's kind check · **class: blocking** · **closed 2026-10-03**

    **Origin:** panel 185's ffi-pragmatist, 2026-10-02
    (`scratchpad/185-ffi-pragmatist/p185/`); reproduced by the critic and by the
    coordinator at 02:42 on `03e70520` (`scratchpad/file-0250/errno.hero`).

    **Why it is a defect.** Exit 2 is the compiler blaming itself for a program
    the author can be told about (`.claude/rules/c-boundary.md`).

    **2026-10-02, lane ffi-macro, a name the header has as an object, a
    value or a type is told on its declaration, and so is a field the header
    lacks whatever the header calls its type** (the second found in the
    lane's first pass, at the field assertion): repaired at `2d5240a0` and
    `6559facf`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): exit 2 where the
    author can be told; repaired and gated, closes after the push's platform
    legs.

    **Closed 2026-10-03** after the round of 2026-10-02's third gate, `6bec7c8c` (lanes bounded, h158 and land186), and the push's platform legs. The gate, on this Mac: the seed regenerated once, 38,648,442 bytes, SHA-256 beginning `568bce290b6ed3bb`, its fixpoint by `cmp`; the compiler's own tests 1,056 and the net's own 197, all passed; the full net, 26 suites, 4,752 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,835 files and of `--emit-c` over 570, every move attributed to its lane. Linux arm64 on `6bec7c8c`, in the arm64 container with Debian clang 22.1.8: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed. The Windows box on `6bec7c8c`: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed, by 22:44. Linux x86-64, the CI's job on `07ccb72a` (run 37065944766, Ubuntu clang 18.1.3; the three commits between touch only `docs/`): the compiler's own tests 1,056, all passed, and the net 4,711 passed and 1 failed over 26 suites, the one `unsupported/fixedbugs-145-a-typedef-of-an-opaque-struct-bound-with-fields`, defect 191's (filed in lane cb4 at `039d1906`, which the round's next gate carries to the trunk). A suite prints totals and not cases, and a case whose library a machine lacks is skipped and counted in neither, so this defect's cases were read one by one on 2026-10-03 with a compiler built from the trunk's seed at `2620bed1`: on this Mac (00:30 to 00:42 by `date`), `run/fixedbugs-152-*` 1 passed of 1 file and `unsupported/fixedbugs-152-*` 16 of 16, and the five tests `2d5240a0` and `6559facf` added each `ok` among 1,056 all passed; in the Linux arm64 container, one run from 00:36 to 00:54, the same 17 of 17 and the five `ok`. The Windows box did not answer that night (`ssh` timed out at 00:27 and 01:03 by `date`, the tailnet reading it offline since about 00:20), so its leg stands for it, read through the harness's code: a case is skipped only on `ffi_package`, `ffi_missing_header` or a missing `pkg-config` (`tests/harness/shell.hero:582-590`), the seventeen name their own headers and `errno.h`, `math.h`, `stdint.h`, `stdio.h`, `stdlib.h` and `string.h`, which `runtime/runtime.c` itself includes there, and no package, and `run` and `unsupported` read 0 failed there, so each passed; the five tests passed among the leg's 1,056, `heroes test` running every test and skipping none (`selfhost/cli/verbs.hero:135-147`). On Linux x86-64 the five tests are `ok` in the job's log, and which of its cases the job ran its log does not say: Linux arm64, which ran all seventeen, is the nearest measurement, an inference for x86-64. It closes after the push's platform legs ran its cases (`.claude/rules/verification.md` § The batch).
