---
kind: defect
area: process
milestone: none
filed: 2026-10-05
commit: 79064ed118c110c5b4a1353b0a6e393c6f61317c
github: none
---

- [x] **348 — the commit guard judges `./heroes` against the session's directory, not the tree the command runs in** | lane b11-misc, 2026-10-05: with the session's working directory on the trunk, `.claude/hooks/guard_bash.py` refused the lane's harness runs, naming the trunk's `selfhost/resolve/state.hero` (written 02:05) as newer than the trunk's `./heroes` (built 2026-10-04 21:24), while the lane's own compiler (02:41) was newer than every file of the lane; calling the compiler by its absolute path passed (the lane's report) | `.claude/hooks/guard_bash.py` (the compiler-age check, the directory it reads) · `.claude/rules/verification.md` § A suite is the last judge, layer 1 · **class: improvement**

    **Origin:** lane b11-misc, 2026-10-05 (its final report, *Found beside*).

    **Class: improvement**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a guard that refuses a run it should allow, and could allow one it should refuse where the two trees disagree the other way; no program is judged wrong by it, and the absolute path is the workaround.

    Repaired at `79064ed1`, 2026-10-07 (lane b14-hooks), gated by its cases, the hooks' own tests; the net is owed at the batch's close. `.claude/hooks/guard_bash.py` reads each segment in the directory the command's own `cd` leaves it in and judges a harness run's compiler against the tree that directory stands in (`.claude/hooks/trees.py`), a directory the text cannot tell giving no opinion; eight cases in `.claude/hooks/test_hooks.py`, six red on the base, and a scratch trunk older than its sources beside a fresh nested lane read the lane's run refused on the base guard and allowed on the repaired one.

## The repair

Repaired at `79064ed1`, 2026-10-07 (lane b14-hooks), gated by its cases, the hooks' own tests; the net is owed at the batch's close. `.claude/hooks/guard_bash.py` reads each segment in the directory the command's own `cd` leaves it in and judges a harness run's compiler against the tree that directory stands in (`.claude/hooks/trees.py`), a directory the text cannot tell giving no opinion; eight cases in `.claude/hooks/test_hooks.py`, six red on the base, and a scratch trunk older than its sources beside a fresh nested lane read the lane's run refused on the base guard and allowed on the repaired one.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
