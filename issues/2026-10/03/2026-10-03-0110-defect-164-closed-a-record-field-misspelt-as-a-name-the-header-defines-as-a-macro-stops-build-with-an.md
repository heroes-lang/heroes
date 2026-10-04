---
kind: defect
area: emit
milestone: none
filed: 2026-10-02
commit: 1cd675868318030a46c9970d6b0ad1e71281bc6c
github: none
---

# Defect 164 closed: a record field misspelt as a name the header defines as a macro stops `build` with an internal error and clang's text

- [x] **164 — a record field misspelt as a name the header defines as a macro stops `build` with an internal error and clang's text** | `extern "macro_field.h"` over `#define size 4` and `typedef struct { int32_t len; int32_t cap; } BUF;`, `record BUF` naming `len` and `size`: `build` exit 2, *internal error: compiling the generated C failed: ... error: expected identifier ... note: expanded from macro 'size'*, at the field assertion; the same with `stdin`, which a libc defines as a macro | `selfhost/emit/` (the field assertion, `heroes-ffi-field`, and the unknown-field mapper) · **class: blocking** · **closed 2026-10-03**

    **Origin:** lane land186's second batch, 2026-10-02, beside panel 186's
    macro-reached fields (`scratchpad/lane-land186/pass2/macro_field.hero`,
    `macro_field_stdio.hero`); reproduced by the coordinator at 17:43 by
    `date` on the trunk at `20652888`
    (`docs/panel/186-briefs/probes/coordinator/macrofield/`).

    **Why it is a defect.** Exit 2 is the compiler blaming itself for a
    misspelling the author can be told about, and clang's text reaches the
    author (`.claude/rules/c-boundary.md`).

    **2026-10-02, lane land186, a test the repair left stale**: `25b96332`,
    gated by its own cases, left one of the compiler's own tests stale,
    `emit/layout_check.hero`'s count of the questions a misspelt field puts
    under the preprocessor, which the member probe made five where it asked
    four (1027 passed and 1 failed on that commit's sources, with its
    compiler); fixed at `ba162242`, 1027 and 0, the rest owed at the round's
    gate.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` §
    Bounded discovery): exit 2. Repaired in lane land186 at `25b96332`,
    gated by its own cases; the rest is owed at the round's gate.

    **Closed 2026-10-03** after the round of 2026-10-02's third gate, `6bec7c8c` (lanes bounded, h158 and land186), and the push's platform legs. The gate, on this Mac: the seed regenerated once, 38,648,442 bytes, SHA-256 beginning `568bce290b6ed3bb`, its fixpoint by `cmp`; the compiler's own tests 1,056 and the net's own 197, all passed; the full net, 26 suites, 4,752 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,835 files and of `--emit-c` over 570, every move attributed to its lane. Linux arm64 on `6bec7c8c`, in the arm64 container with Debian clang 22.1.8: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed. The Windows box on `6bec7c8c`: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed, by 22:44. Linux x86-64, the CI's job on `07ccb72a` (run 37065944766, Ubuntu clang 18.1.3; the three commits between touch only `docs/`): the compiler's own tests 1,056, all passed, and the net 4,711 passed and 1 failed over 26 suites, the one `unsupported/fixedbugs-145-a-typedef-of-an-opaque-struct-bound-with-fields`, defect 191's (filed in lane cb4 at `039d1906`, which the round's next gate carries to the trunk). A suite prints totals and not cases, and a case whose library a machine lacks is skipped and counted in neither, so this defect's cases were read one by one on 2026-10-03 with a compiler built from the trunk's seed at `2620bed1`: on this Mac (00:30 to 00:42 by `date`), `unsupported/ffi-a-field-named-after-a-header-macro` 1 passed of 1 file, `25b96332`'s test *a field named after a header macro that names no member is told on the field* and the layout check's own test `ba162242` corrected, *a field the dump does not show puts the record's every question under the preprocessor*, each `ok` among 1,056 all passed, and `docs/panel/186-briefs/probes/coordinator/macrofield/macro_field.hero` told `ffi_unknown_field` on its field at exit 1; in the Linux arm64 container, one run from 00:36 to 00:54, the same 1 of 1, the two `ok` and the reproducer the same. The Windows box did not answer that night (`ssh` timed out at 00:27 and 01:03 by `date`, the tailnet reading it offline since about 00:20), so its leg stands for it, read through the harness's code: a case is skipped only on `ffi_package`, `ffi_missing_header` or a missing `pkg-config` (`tests/harness/shell.hero:582-590`), the case names its own header over `stdint.h` and no package, and `unsupported` read 0 failed there, so it passed; the two tests passed among the leg's 1,056, `heroes test` running every test and skipping none (`selfhost/cli/verbs.hero:135-147`). On Linux x86-64 the two tests are `ok` in the job's log, and which of its cases the job ran its log does not say: Linux arm64, which ran the case, is the nearest measurement, an inference for x86-64. It closes after the push's platform legs ran its cases (`.claude/rules/verification.md` § The batch).
