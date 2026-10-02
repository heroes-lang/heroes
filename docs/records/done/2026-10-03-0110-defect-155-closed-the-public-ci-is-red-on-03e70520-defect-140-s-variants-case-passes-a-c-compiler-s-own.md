# Defect 155 closed: the public CI is red on `03e70520`: defect 140's variants case passes a C compiler's own limit

- [x] **155 — the public CI is red on `03e70520`: defect 140's variants case passes a C compiler's own limit** | run 36939966148: Darwin arm64's clang (Apple clang 21.0.0, Xcode 26.6) crashes, *Illegal instruction: 4*, on `run/fixedbugs-140-variants-a-thousand-deep-build`, red in `run`, `determinism` and `emission`; Linux x86-64 and arm64 time out on it at `-O2` (exit 124); Linux arm64 also times out on `probe/selfhost, multi`; Windows green | `tests/golden/run/fixedbugs-140-variants-a-thousand-deep-build.hero` and its trace until 2026-10-02, `tests/golden/run/fixedbugs-140-variants-through-arrays-a-thousand-deep-build.hero` since · `tests/harness/suite_run.hero:151` · **class: blocking** · **closed 2026-10-03**

    **Origin:** the author, 2026-10-02 at 02:31 (*la ci è rotta*), on the push
    of 01:17; read by the coordinator from `gh run view 36939966148
    --log-failed`. This Mac and the Linux containers passed the same case at
    every gate of 2026-10-01.

    **Why it is a defect.** The trunk's own instrument is red on every push
    until the case witnesses defect 140 without depending on a C compiler's
    recursion limit or speed; lane ci140 is on it.

    **2026-10-02, lane ci140, the variant chain is built through arrays,
    where no C type nests, and kept by value in three `check/` cases, where
    no C compiler reads it**: repaired at `0aa055a9`, gated by its cases and
    the compiler's own tests; the net is owed at the batch's close.

    **2026-10-02, lane ci-probe, the second half: a probe row over a tree
    probes its files one at a time, and the files together must read what
    the probe reads of the whole root**: repaired at `efe2fea2`, gated by
    the net's own tests and `probe` whole; the net is owed at the batch's
    close, and Linux arm64 at the push.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): the public CI is red;
    both halves repaired, closes on a green CI run.

    **Closed 2026-10-03** after the round of 2026-10-02's third gate, `6bec7c8c` (lanes bounded, h158 and land186), and the push's platform legs. The gate, on this Mac: the seed regenerated once, 38,648,442 bytes, SHA-256 beginning `568bce290b6ed3bb`, its fixpoint by `cmp`; the compiler's own tests 1,056 and the net's own 197, all passed; the full net, 26 suites, 4,752 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,835 files and of `--emit-c` over 570, every move attributed to its lane. Linux arm64 on `6bec7c8c`, in the arm64 container with Debian clang 22.1.8: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed. The Windows box on `6bec7c8c`: the compiler's own tests 1,056, all passed, and 20 suites, 0 failed, by 22:44. Linux x86-64, the CI's job on `07ccb72a` (run 37065944766, Ubuntu clang 18.1.3; the three commits between touch only `docs/`): the compiler's own tests 1,056, all passed, and the net 4,711 passed and 1 failed over 26 suites, the one `unsupported/fixedbugs-145-a-typedef-of-an-opaque-struct-bound-with-fields`, defect 191's (filed in lane cb4 at `039d1906`, which the round's next gate carries to the trunk). A suite prints totals and not cases, and a case whose library a machine lacks is skipped and counted in neither, so this defect's cases were read one by one on 2026-10-03 with a compiler built from the trunk's seed at `2620bed1`: on this Mac (00:30 to 00:42 by `date`), `run/fixedbugs-140-variants-through-arrays-a-thousand-deep-build` 1 passed of 1 file and the three `check/fixedbugs-140-*-in-variants-a-thousand-deep-is-refused` 3 of 3, then `probe` 27 passed and 0 failed and the net's own three probe tests of `efe2fea2` each `ok` (in the copy, whose one records test that asks git fails for want of `.git`, 196 of 197); in the Linux arm64 container, one run from 00:36 to 00:54, the same 4 of 4, `probe` 27 and 0, and the three tests `ok`. The Windows box did not answer that night (`ssh` timed out at 00:27 and 01:03 by `date`, the tailnet reading it offline since about 00:20), so its leg stands for it, read through the harness's code: its `run` 234, `check` 415 and `probe` 27, each 0 failed, and the run case binds no C at all, so no skip reaches it, and `check` never skips: the four passed there. The public CI's run 37065944766 on `07ccb72a`, for the two causes: Darwin arm64, with the Apple clang that crashed on `03e70520` (21.0.0, clang-2100.1.1.101, Xcode 26.6), green, `run` 241, `determinism` 271 and `emission` 696, each 0 failed, the three suites red then; Linux x86-64 and arm64, `run` 237 and 0 each, the timeouts at `-O2` gone; Linux arm64, `probe` 27 and 0, where `8b98bcc7`'s job read 21 and 1 on `probe/selfhost, multi`; Darwin arm64 and Windows x86-64, the net's own 197, all passed, the three probe tests among them. The run's conclusion is `failure`, from one case in its two Linux jobs, defect 191's above, and none of this defect's; the item asked *a green CI run*, and this run is red by that case alone. It closes after the push's platform legs ran its cases (`.claude/rules/verification.md` § The batch).
