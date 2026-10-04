# Defect 157 closed: `ffi_tag_is_a_union` says a union has no `record` spelling, which a record binding a union falsifies

- [x] **157 — `ffi_tag_is_a_union` says a union has no `record` spelling, which a record binding a union falsifies** | `extern "tu.h"` over `union utag { int32_t i; float f; };` with `record UI tag utag` naming `i`: `build` exit 1, `ffi_tag_is_a_union`, its note *a group's `record` is the header's STRUCT (§4.19). A union has no `record` spelling in this language: reach it as a `ptr` and read it through C functions, or bind the one member you need as its own type*, while `record W` over the typedef'd union `W` builds and prints `7` | `selfhost/emit/ffi_tag.hero:200-218` (landed in `8fdd2a3c`) · panel 077's ratified item 4 (*spell `union T` where the header says union*) · panel 073's *one record per arm by `tag`* · **class: blocking** · **closed 2026-10-03**

    **Origin:** panel 186's ffi-pragmatist and compiler-engineer (both
    measured the refusal, on SDL3's `SDL_Event` and on a minimal `union
    utag`) and its completeness critic (who read the note against
    `read.hero` and panel 077, and found no golden holding the code),
    2026-10-02; reproduced by the coordinator at 14:20 on the trunk at
    `43520193` (`docs/panel/186-briefs/probes/coordinator/tu1.hero` and
    `read.hero`).

    **Why it is a defect.** design.md §4.17: a diagnostic is true, and its
    note sends the author to a `ptr` and C functions for a union the
    language binds; the refusal itself, of a TAGGED union where a typedef'd
    one binds, is the question panel 186's R9 leaves open with it.

    **2026-10-02, lane land186, the note says what is true, its two routes
    built and run**: repaired at `24d3d23d`, gated by its own cases; the
    rest is owed at the round's gate, and the platform legs before the push.

    **Widened 2026-10-02** by lane land186's first pass, the same cause (a
    union reached by `tag`): a handle, `record UH tag utag` with no fields
    over `union utag { int32_t i; float f; };`, is told `ffi_unknown_name`,
    *`tags.h` declares no `utag`*, which is false (reproduced by the
    coordinator on `9faf7462` before 16:49, the widening's commit,
    `docs/panel/186-briefs/probes/coordinator/handle_utag.hero` over
    `tags.h`).

    **2026-10-02, lane land186, a tag naming a union's or an enum's tag told
    its own kind, a handle's and a bare name's included, with what reaches
    it** (the widening and three shapes beside it with its cause): repaired
    at `669fc846`, gated by its own cases; the rest is owed at the round's
    gate, and the platform legs before the push.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): a false note, against
    panel 077's ratified item 4.

    **Closed 2026-10-03** after the round of 2026-10-02's third gate, `6bec7c8c` (lanes bounded, h158 and land186), and the push's platform legs. The gate, on this Mac: the seed regenerated once, 38,648,442 bytes, SHA-256 beginning `568bce290b6ed3bb`, its fixpoint by `cmp`; the compiler's own tests 1,056 and the net's own 197, all passed; the full net, 26 suites, 4,752 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,835 files and of `--emit-c` over 570, every move attributed to its lane. Linux arm64 on `6bec7c8c`, in the arm64 container with Debian clang 22.1.8: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed. The Windows box on `6bec7c8c`: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed, by 22:44. Linux x86-64, the CI's job on `07ccb72a` (run 37065944766, Ubuntu clang 18.1.3; the three commits between touch only `docs/`): the compiler's own tests 1,056, all passed, and the net 4,711 passed and 1 failed over 26 suites, the one `unsupported/fixedbugs-145-a-typedef-of-an-opaque-struct-bound-with-fields`, defect 191's (filed in lane cb4 at `039d1906`, which the round's next gate carries to the trunk). A suite prints totals and not cases, and a case whose library a machine lacks is skipped and counted in neither, so this defect's cases were read one by one on 2026-10-03 with a compiler built from the trunk's seed at `2620bed1`: on this Mac (00:30 to 00:42 by `date`), `run/fixedbugs-157-*` 2 passed of 2 files and `unsupported/fixedbugs-157-*` 6 of 6, `669fc846`'s test `ok` among 1,056 all passed, and `docs/panel/186-briefs/probes/coordinator/tu1.hero` and `handle_utag.hero` each told `ffi_tag_is_a_union` at exit 1, *declares `utag` as a `union`, not a struct*, where the handle was told *declares no `utag`*; in the Linux arm64 container, one run from 00:36 to 00:54, the same 8 of 8, the test `ok` and the two reproducers the same. The Windows box did not answer that night (`ssh` timed out at 00:27 and 01:03 by `date`, the tailnet reading it offline since about 00:20), so its leg stands for it, read through the harness's code: a case is skipped only on `ffi_package`, `ffi_missing_header` or a missing `pkg-config` (`tests/harness/shell.hero:582-590`), the eight name their own headers over `stdint.h` and no package, and `run` and `unsupported` read 0 failed there, so each passed; the test passed among the leg's 1,056, `heroes test` running every test and skipping none (`selfhost/cli/verbs.hero:135-147`). On Linux x86-64 the test is `ok` in the job's log, and which of its cases the job ran its log does not say: Linux arm64, which ran all eight, is the nearest measurement, an inference for x86-64. It closes after the push's platform legs ran its cases (`.claude/rules/verification.md` § The batch).
