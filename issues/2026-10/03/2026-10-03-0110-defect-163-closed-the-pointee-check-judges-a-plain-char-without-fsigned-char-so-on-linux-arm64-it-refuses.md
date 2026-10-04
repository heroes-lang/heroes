---
kind: defect
area: cli
milestone: none
filed: 2026-10-02
commit: 1cd675868318030a46c9970d6b0ad1e71281bc6c
github: none
---

# Defect 163 closed: the pointee check judges a plain `char` without `-fsigned-char`, so on Linux arm64 it refuses the binding the program's own compile agrees with

- [x] **163 — the pointee check judges a plain `char` without `-fsigned-char`, so on Linux arm64 it refuses the binding the program's own compile agrees with** | `extern "c.h"` over `void fill(char *p)`, bound `function fill(@p: i8)`: on this Mac `build` exit 0, prints `-1`; in the Linux arm64 container on the trunk at `02b29536`, `build` exit 1, `ffi_parameter_type`, *`p` of `fill` is declared `i8`, and the header's `char *` points at a different width or sign*, while `@p: u8` is refused on this Mac and built on arm64; the program itself is compiled with `-fsigned-char` on both (`selfhost/cli/flags.hero:108`, panel 161) | `selfhost/cli/pointee.hero` (its two clang runs, without the compiler's own flags) · panel 161 (*a plain `char` meaning the same thing on four legs*) · **class: blocking** · **closed 2026-10-03**

    **Origin:** lane h158, 2026-10-02, a question raised beside defect 161
    (the pointee probe's compiles do not use the compiler's own flag list);
    measured by the coordinator at 17:33 by `date`, on this Mac and in the
    Linux arm64 container (`docs/panel/186-briefs/probes/coordinator/signchar/`,
    `s8.hero` and `u8.hero` over `c.h`). A result is held by the program's
    own compile and agrees on both (a `char` result bound `-> u64` refused on
    both, 17:34): only the pointee check diverges.

    **Why it is a defect.** A correct binding is refused on one platform and
    accepted on another, against panel 161's ruling that a plain `char` means
    one thing on every leg; no value read through it is wrong, the bytes
    being the same (measured: `u8` on arm64 prints `255`).

    **2026-10-02, lane h158, a probe compiles under the compiler's own flags
    but its diagnostics, so a plain `char` is signed there too**: repaired at
    `0235b942` (`pointee.probe_flags`, put on every probe's line by
    `selfhost/cli/pointee.hero`'s `with_search`), gated by its cases on this
    Mac, a run case printing -1, an unsupported case refusing `@p: u8` and
    two unit tests asking clang for Linux arm64's target; its Linux arm64
    leg, where the defect shows, is owed before the push with the net.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` §
    Bounded discovery): a correct program refused, on Linux arm64.

    **Closed 2026-10-03** after the round of 2026-10-02's third gate, `6bec7c8c` (lanes bounded, h158 and land186), and the push's platform legs. The gate, on this Mac: the seed regenerated once, 38,648,442 bytes, SHA-256 beginning `568bce290b6ed3bb`, its fixpoint by `cmp`; the compiler's own tests 1,056 and the net's own 197, all passed; the full net, 26 suites, 4,752 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,835 files and of `--emit-c` over 570, every move attributed to its lane. Linux arm64 on `6bec7c8c`, in the arm64 container with Debian clang 22.1.8: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed. The Windows box on `6bec7c8c`: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed, by 22:44. Linux x86-64, the CI's job on `07ccb72a` (run 37065944766, Ubuntu clang 18.1.3; the three commits between touch only `docs/`): the compiler's own tests 1,056, all passed, and the net 4,711 passed and 1 failed over 26 suites, the one `unsupported/fixedbugs-145-a-typedef-of-an-opaque-struct-bound-with-fields`, defect 191's (filed in lane cb4 at `039d1906`, which the round's next gate carries to the trunk). A suite prints totals and not cases, and a case whose library a machine lacks is skipped and counted in neither, so this defect's cases were read one by one on 2026-10-03 with a compiler built from the trunk's seed at `2620bed1`: on this Mac (00:30 to 00:42 by `date`), `run/fixedbugs-163-a-plain-char-pointee-bound-as-i8` 1 passed of 1 file and `unsupported/fixedbugs-163-a-plain-char-pointee-bound-as-u8-is-refused` 1 of 1, `0235b942`'s two tests, which ask clang for Linux arm64's target, each `ok` among 1,056 all passed, and `docs/panel/186-briefs/probes/coordinator/signchar/s8.hero` prints `-1` while `u8.hero` is told `ffi_parameter_type`; in the Linux arm64 container, where the defect showed, one run from 00:36 to 00:54, the same 2 of 2, the two `ok`, `s8.hero` printing `-1` and `u8.hero` refused, as on the Mac. The Windows box did not answer that night (`ssh` timed out at 00:27 and 01:03 by `date`, the tailnet reading it offline since about 00:20), so its leg stands for it, read through the harness's code: a case is skipped only on `ffi_package`, `ffi_missing_header` or a missing `pkg-config` (`tests/harness/shell.hero:582-590`), the two name their own headers over `stdint.h` and no package, and `run` and `unsupported` read 0 failed there, so each passed; the two tests passed among the leg's 1,056, `heroes test` running every test and skipping none (`selfhost/cli/verbs.hero:135-147`). On Linux x86-64 the two tests are `ok` in the job's log, and which of its cases the job ran its log does not say: Linux arm64, which ran both, is the nearest measurement, an inference for x86-64. It closes after the push's platform legs ran its cases (`.claude/rules/verification.md` § The batch).
