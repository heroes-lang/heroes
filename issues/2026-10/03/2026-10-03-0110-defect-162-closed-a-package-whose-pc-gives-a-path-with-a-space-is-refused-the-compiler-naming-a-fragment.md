# Defect 162 closed: a package whose `.pc` gives a path with a space is refused, the compiler naming a fragment of the path as the flag

- [x] **162 — a package whose `.pc` gives a path with a space is refused, the compiler naming a fragment of the path as the flag** | a `.pc` with `Cflags: -I"<dir>/inc with space"`: `pkg-config --cflags` prints the path with its spaces escaped by backslashes, and `build` exit 1, `ffi_package`, *the package `spaced` answered with* the fragment `with` and its backslash, *which this compiler does not pass on* | `selfhost/cli/libraries.hero` (the word splitter before `filter_words`) · **class: blocking** · **closed 2026-10-03**

    **Origin:** lane h158, 2026-10-02, beside defect 160
    (`scratchpad/lane-h158/d160/pc/spaced.pc`); reproduced by the
    coordinator at 16:57 by `date` on the trunk at `0bcd442c`
    (`docs/panel/186-briefs/probes/coordinator/spaced/`, run with
    `PKG_CONFIG_PATH=<that>/pc`).

    **Why it is a defect.** A correct package is refused, and the message
    names a piece of a path as a flag (design.md §4.17).

    **2026-10-02, lane h158, pkg-config's answer is read as a shell reads
    it, so a word with a space stays one word**: repaired at `b37bfce1`
    (`selfhost/cli/shell_split.hero`, and the compile caches keyed by each
    word's length, `libraries.key_text`), gated by its cases, unit tests in
    `shell_split` and `libraries`, and the reproducer by hand, `spaced`
    printing 8; the net is owed at the batch's close.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` §
    Bounded discovery): a correct program refused, with a false message.

    **Closed 2026-10-03** after the round of 2026-10-02's third gate, `6bec7c8c` (lanes bounded, h158 and land186), and the push's platform legs. The gate, on this Mac: the seed regenerated once, 38,648,442 bytes, SHA-256 beginning `568bce290b6ed3bb`, its fixpoint by `cmp`; the compiler's own tests 1,056 and the net's own 197, all passed; the full net, 26 suites, 4,752 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,835 files and of `--emit-c` over 570, every move attributed to its lane. Linux arm64 on `6bec7c8c`, in the arm64 container with Debian clang 22.1.8: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed. The Windows box on `6bec7c8c`: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed, by 22:44. Linux x86-64, the CI's job on `07ccb72a` (run 37065944766, Ubuntu clang 18.1.3; the three commits between touch only `docs/`): the compiler's own tests 1,056, all passed, and the net 4,711 passed and 1 failed over 26 suites, the one `unsupported/fixedbugs-145-a-typedef-of-an-opaque-struct-bound-with-fields`, defect 191's (filed in lane cb4 at `039d1906`, which the round's next gate carries to the trunk). A suite prints totals and not cases, and a case whose library a machine lacks is skipped and counted in neither, so this defect's cases were read one by one on 2026-10-03 with a compiler built from the trunk's seed at `2620bed1`: its cases are three tests, `b37bfce1`'s *a word pkg-config escaped is judged whole, and a key keeps it one* and *pkg-config's answer is read as a shell reads it, and nothing in it is expanded*, and `77cb114a`'s *a package's words reach the layout check each one whole, and its verdict is keyed by them (defects 161 and 162)*, and its reproducer by hand, `docs/panel/186-briefs/probes/coordinator/spaced/`. On this Mac (00:30 to 00:42 by `date`) the three `ok` among 1,056 all passed and `spaced` prints `8`; in the Linux arm64 container, one run from 00:36 to 00:54, the three `ok` and `spaced` prints `8`. The Windows box did not answer that night (`ssh` timed out at 00:27 and 01:03 by `date`, the tailnet reading it offline since about 00:20), so its leg stands for it: the three passed among the leg's 1,056, `heroes test` running every test and skipping none (`selfhost/cli/verbs.hero:135-147`), and the reproducer, which needs `pkg-config`, was not run there. On Linux x86-64 the three are `ok` in the job's log, as in the run's Darwin arm64 and Windows x86-64 jobs. It closes after the push's platform legs ran its cases (`.claude/rules/verification.md` § The batch).
