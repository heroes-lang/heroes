# Defect 161 closed: a package's compile flags other than `-I` never reach clang, so a correct package is refused or stops `build` at exit 2

- [x] **161 — a package's compile flags other than `-I` never reach clang, so a correct package is refused or stops `build` at exit 2** | `extern "valued.h" package "dpkg"` over a `.pc` with `Cflags: -I<dir> -DHERO_PKG_VALUE=7` and a header returning `HERO_PKG_VALUE`: `build` exit 2, *internal error: ... use of undeclared identifier 'HERO_PKG_VALUE'*; a package's `-F` likewise never reaches the compile step, so a framework header it names is told missing | `selfhost/cli/units.hero:139` and `selfhost/cli/pointee.hero:243`, which keep only a package's `-I` · **class: blocking** · **closed 2026-10-03**

    **Origin:** lane h158, 2026-10-02, beside defect 160 (the framework
    reproducer, once its `-F` was no longer refused, still not built;
    `scratchpad/lane-h158/d160/dpkg/`); reproduced by the coordinator at
    16:57 by `date` on the trunk at `0bcd442c`
    (`docs/panel/186-briefs/probes/coordinator/dpkg/`, run with
    `PKG_CONFIG_PATH=<that>/pc`).

    **Why it is a defect.** Exit 2 is the compiler blaming itself for a
    package the author wrote correctly, and the flags a `.pc` gives are the
    reason `package` exists (design.md §4.19).

    **2026-10-02, lane h158, every compile is handed a package's `-I -D -U
    -F` by one rule, and its cache keyed by them**: repaired at `40bf84f2`
    (`libraries.compile_flags`, called by `selfhost/cli/units.hero` and
    `selfhost/cli/pointee.hero`'s `with_search`), gated by its cases, unit
    tests in the three modules, and the reproducers by hand, `dpkg` printing
    7 and defect 160's framework reproducer building; the net is owed at the
    batch's close.

    **2026-10-02, the round's third gate, lanes land186 and h158 reconciled:
    the layout check's cache keyed by the same words**: lane land186's
    `selfhost/cli/layout.hero` (`f2a08f13`), whose three clang runs
    `pointee.with_search` now hands a package's compile words and the
    probe's flags (161, 163), kept its verdicts under the package's whole
    answer joined by spaces (162's collision too); on one `build/` the
    merged tree replayed a verdict land186's compiler had reached without
    the package's `-D` and built, at exit 0, a record four bytes short of
    the header's struct. Keyed by `libraries.compile_flags` and
    `pointee.probe_flags` at `77cb114a`, with its compiler test; the round's
    gate is the commit carrying this line.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` §
    Bounded discovery): exit 2, a correct program refused.

    **Closed 2026-10-03** after the round of 2026-10-02's third gate, `6bec7c8c` (lanes bounded, h158 and land186), and the push's platform legs. The gate, on this Mac: the seed regenerated once, 38,648,442 bytes, SHA-256 beginning `568bce290b6ed3bb`, its fixpoint by `cmp`; the compiler's own tests 1,056 and the net's own 197, all passed; the full net, 26 suites, 4,752 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,835 files and of `--emit-c` over 570, every move attributed to its lane. Linux arm64 on `6bec7c8c`, in the arm64 container with Debian clang 22.1.8: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed. The Windows box on `6bec7c8c`: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed, by 22:44. Linux x86-64, the CI's job on `07ccb72a` (run 37065944766, Ubuntu clang 18.1.3; the three commits between touch only `docs/`): the compiler's own tests 1,056, all passed, and the net 4,711 passed and 1 failed over 26 suites, the one `unsupported/fixedbugs-145-a-typedef-of-an-opaque-struct-bound-with-fields`, defect 191's (filed in lane cb4 at `039d1906`, which the round's next gate carries to the trunk). A suite prints totals and not cases, and a case whose library a machine lacks is skipped and counted in neither, so this defect's cases were read one by one on 2026-10-03 with a compiler built from the trunk's seed at `2620bed1`: its cases are four tests, `40bf84f2`'s *a compile takes a package's -I -D -U -F, and only those, in their order*, *a package's -D reaches the pointee check, and its verdict is keyed by the words it was asked with (defect 161)* and *a package's -D reaches the unit's compile, and an object is keyed by the words it was compiled with (defect 161)*, and `77cb114a`'s *a package's words reach the layout check each one whole, and its verdict is keyed by them (defects 161 and 162)*, and its reproducers by hand, `docs/panel/186-briefs/probes/coordinator/dpkg/` and defect 160's `fw/`. On this Mac (00:30 to 00:42 by `date`) the four `ok` among 1,056 all passed, `dpkg` prints `7` and `fw` builds and prints `7`; in the Linux arm64 container, one run from 00:36 to 00:54, the four `ok`, `dpkg` prints `7` and `fw` `7`, clang there printing *argument unused during compilation* for the package's `-F` at exit 0, which the Mac does not print and which clang prints for a Linux target's link and not for its compile: a finding of its own beside this defect. The Windows box did not answer that night (`ssh` timed out at 00:27 and 01:03 by `date`, the tailnet reading it offline since about 00:20), so its leg stands for it: the four passed among the leg's 1,056, `heroes test` running every test and skipping none (`selfhost/cli/verbs.hero:135-147`), and the reproducers, which need `pkg-config`, were not run there. On Linux x86-64 the four are `ok` in the job's log, as in the run's Darwin arm64 and Windows x86-64 jobs. It closes after the push's platform legs ran its cases (`.claude/rules/verification.md` § The batch).
