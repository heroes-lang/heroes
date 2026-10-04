---
kind: defect
area: cli
milestone: none
filed: 2026-10-02
commit: 1cd675868318030a46c9970d6b0ad1e71281bc6c
github: none
---

# Defect 160 closed: a package whose `.pc` gives `-F <dir>` is refused, its flag read joined as `-F<dir>`, while the note says `-F` is accepted

- [x] **160 — a package whose `.pc` gives `-F <dir>` is refused, its flag read joined as `-F<dir>`, while the note says `-F` is accepted** | `extern "Fake/fake.h" package "fakefw"` over a `.pc` with `Cflags: -F ${pcfiledir}/../frameworks`: `pkg-config --cflags` prints `-F/<dir>`, and `build` exit 1, `ffi_package`, *answered with `-F/...`, which this compiler does not pass on*, its note listing `-F` among the flags accepted | `selfhost/cli/libraries.hero` (`filter_words`, which takes `-F` only as two words) · **class: blocking** · **closed 2026-10-03**

    **Origin:** lane h158, 2026-10-02, measuring framework headers for
    defect 158 (`scratchpad/lane-h158/shapes/fw/`); reproduced by the
    coordinator before 16:47, the filing commit's time, on the trunk at
    `545e0044`
    (`docs/panel/186-briefs/probes/coordinator/fw/`, run with
    `PKG_CONFIG_PATH=<that>/pc`).

    **Why it is a defect.** A correct package is refused and the message
    contradicts itself (design.md §4.17); macOS frameworks reach a program
    only through `-F`.

    **2026-10-02, lane h158, every accepted flag of a package is read in both
    its spellings, and a refused one refused in either**: repaired at
    `fb33a992`, gated by its case (a unit test in
    `selfhost/cli/libraries.hero` over pkg-config's measured output) and the
    reproducer by hand; the net is owed at the batch's close. The
    reproducer's package now passes the filter and its program still does
    not build: the compile line keeps only a package's `-I` words
    (`selfhost/cli/units.hero`), so `-F` never reaches clang, a second cause
    that lane h158's report gives for filing apart.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` §
    Bounded discovery): a correct program refused, with a false note.

    **Closed 2026-10-03** after the round of 2026-10-02's third gate, `6bec7c8c` (lanes bounded, h158 and land186), and the push's platform legs. The gate, on this Mac: the seed regenerated once, 38,648,442 bytes, SHA-256 beginning `568bce290b6ed3bb`, its fixpoint by `cmp`; the compiler's own tests 1,056 and the net's own 197, all passed; the full net, 26 suites, 4,752 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,835 files and of `--emit-c` over 570, every move attributed to its lane. Linux arm64 on `6bec7c8c`, in the arm64 container with Debian clang 22.1.8: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed. The Windows box on `6bec7c8c`: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed, by 22:44. Linux x86-64, the CI's job on `07ccb72a` (run 37065944766, Ubuntu clang 18.1.3; the three commits between touch only `docs/`): the compiler's own tests 1,056, all passed, and the net 4,711 passed and 1 failed over 26 suites, the one `unsupported/fixedbugs-145-a-typedef-of-an-opaque-struct-bound-with-fields`, defect 191's (filed in lane cb4 at `039d1906`, which the round's next gate carries to the trunk). A suite prints totals and not cases, and a case whose library a machine lacks is skipped and counted in neither, so this defect's cases were read one by one on 2026-10-03 with a compiler built from the trunk's seed at `2620bed1`: its case is a test of `fb33a992`, *every accepted flag in both its spellings, and a refused one refused in either*, and its reproducer by hand, `docs/panel/186-briefs/probes/coordinator/fw/` with `PKG_CONFIG_PATH` at its `pc/`. On this Mac (00:30 to 00:42 by `date`) the test `ok` among 1,056 all passed and the reproducer builds and prints `7`; in the Linux arm64 container, one run from 00:36 to 00:54, the test `ok` and the reproducer builds and prints `7`, clang there printing *argument unused during compilation* for the package's `-F` at exit 0, which the Mac does not print and which clang prints for a Linux target's link and not for its compile: a finding of its own beside this defect. The Windows box did not answer that night (`ssh` timed out at 00:27 and 01:03 by `date`, the tailnet reading it offline since about 00:20), so its leg stands for it: the test passed among the leg's 1,056, `heroes test` running every test and skipping none (`selfhost/cli/verbs.hero:135-147`), and the reproducer, which needs `pkg-config`, was not run there. On Linux x86-64 the test is `ok` in the job's log, as in the run's Darwin arm64 and Windows x86-64 jobs. It closes after the push's platform legs ran its cases (`.claude/rules/verification.md` § The batch).
