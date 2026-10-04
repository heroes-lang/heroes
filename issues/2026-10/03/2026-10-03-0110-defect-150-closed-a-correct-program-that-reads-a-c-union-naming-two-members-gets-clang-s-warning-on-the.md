# Defect 150 closed: a correct program that reads a C union naming two members gets clang's warning on the author's line

- [x] **150 — a correct program that reads a C union naming two members gets clang's warning on the author's line** | `extern "w.h"` with `record W` (two members over a union) and `function make_w() -> W`, `print(w.i)`: `build` exit 0, the program prints 7, and the build prints *warning: excess elements in union initializer* at `read.hero:2:81` | the completeness probe's `{0,0}` for a union record (`selfhost/emit/`, beside defect 140's `{}`) · **class: blocking** · **closed 2026-10-03**

    **Origin:** lane emit, 2026-10-01 (`scratchpad/lane-emit/pass1/union2/read.hero`
    and its `w.h`), left open in defect 140's closed record; reproduced by
    the coordinator at 00:35 on 2026-10-02 on `b9fdb0a3`.

    **Why it is a defect.** The emitted C is the compiler's, and a warning
    about it reaches the author at a line they wrote correctly
    (`.claude/rules/generated-c.md`; design.md §4.17).

    **2026-10-02, lane land186, completeness asked of the header's layout
    and the positional probe gone** (panel 186 R4): repaired at `f2a08f13`,
    gated by its cases and the compiler's own tests; the net is owed at the
    batch's close, and the platform legs before the push.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): a clang warning on a
    correct program reaches the author's line.

    **Closed 2026-10-03** after the round of 2026-10-02's third gate, `6bec7c8c` (lanes bounded, h158 and land186), and the push's platform legs. The gate, on this Mac: the seed regenerated once, 38,648,442 bytes, SHA-256 beginning `568bce290b6ed3bb`, its fixpoint by `cmp`; the compiler's own tests 1,056 and the net's own 197, all passed; the full net, 26 suites, 4,752 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,835 files and of `--emit-c` over 570, every move attributed to its lane. Linux arm64 on `6bec7c8c`, in the arm64 container with Debian clang 22.1.8: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed. The Windows box on `6bec7c8c`: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed, by 22:44. Linux x86-64, the CI's job on `07ccb72a` (run 37065944766, Ubuntu clang 18.1.3; the three commits between touch only `docs/`): the compiler's own tests 1,056, all passed, and the net 4,711 passed and 1 failed over 26 suites, the one `unsupported/fixedbugs-145-a-typedef-of-an-opaque-struct-bound-with-fields`, defect 191's (filed in lane cb4 at `039d1906`, which the round's next gate carries to the trunk). A suite prints totals and not cases, and a case whose library a machine lacks is skipped and counted in neither, so this defect's cases were read one by one on 2026-10-03 with a compiler built from the trunk's seed at `2620bed1`: on this Mac (00:30 to 00:42 by `date`), `run/fixedbugs-150-a-union-read-through-two-members-builds-silently` 1 passed of 1 file, the seven tests of `f2a08f13`, the commit it shares with 151, each `ok` among 1,056 all passed, and `docs/panel/186-briefs/probes/coordinator/read.hero` builds with no warning and prints `7`; in the Linux arm64 container, one run from 00:36 to 00:54, the same 1 of 1, the seven `ok` and `read.hero` the same. The Windows box did not answer that night (`ssh` timed out at 00:27 and 01:03 by `date`, the tailnet reading it offline since about 00:20), so its leg stands for it, read through the harness's code: a case is skipped only on `ffi_package`, `ffi_missing_header` or a missing `pkg-config` (`tests/harness/shell.hero:582-590`), the case names a header beside it over `stdint.h` and `string.h` and no package, and `run` read 0 failed there, so it passed; the seven tests passed among the leg's 1,056, `heroes test` running every test and skipping none (`selfhost/cli/verbs.hero:135-147`). On Linux x86-64 the seven tests are `ok` in the job's log, and which of its cases the job ran its log does not say: Linux arm64, which ran the case, is the nearest measurement, an inference for x86-64. It closes after the push's platform legs ran its cases (`.claude/rules/verification.md` § The batch).
