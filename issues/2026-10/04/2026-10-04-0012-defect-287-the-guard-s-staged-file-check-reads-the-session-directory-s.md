---
kind: defect
area: process
milestone: none
filed: 2026-10-04
commit: 2990e653df33ba43d3db92742e598d814026baa4
github: none
---

- [x] **287 — the guard's staged-file check reads the session directory's index, so a lane's commit is never checked** | `staged_hero_files(cwd)` runs `git diff --cached` in the session's directory (`.claude/hooks/guard_bash.py:247`), so a commit made in a lane's worktree is judged by the trunk's index: lane b9-harness's `a3fb46d5` staged three cases `heroes fmt` refuses and the guard accepted it (measured by the lane, 2026-10-04); in the trunk the same check refuses a golden case whose intended diagnostic makes `fmt` refuse it (defect 272's shape) | `.claude/hooks/guard_bash.py:247` to `:290` · defects 254 and 272, the layer-0 hook's two · **class: improvement**

    **Origin:** lane b9-harness, 2026-10-04 (its reply's *found beside*, measured).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach; no program moves.

    Repaired at `2990e653`, 2026-10-07 (lane b14-hooks), gated by its cases, the hooks' own tests; the net is owed at the batch's close. The staged-file check, now `.claude/hooks/staged.py`, reads the index of the directory the commit runs in, `git -C` or the command's own `cd`, its paths from that worktree's root, judged by that tree's compiler and told as that compiler's age where it is older than its tree; six cases in `.claude/hooks/test_hooks.py` over real repositories and a worktree, five red before, and a scratch lane's staged module `fmt` refuses passed the base guard from the trunk and is refused by the repaired one.

## The repair

Repaired at `2990e653`, 2026-10-07 (lane b14-hooks), gated by its cases, the hooks' own tests; the net is owed at the batch's close. The staged-file check, now `.claude/hooks/staged.py`, reads the index of the directory the commit runs in, `git -C` or the command's own `cd`, its paths from that worktree's root, judged by that tree's compiler and told as that compiler's age where it is older than its tree; six cases in `.claude/hooks/test_hooks.py` over real repositories and a worktree, five red before, and a scratch lane's staged module `fmt` refuses passed the base guard from the trunk and is refused by the repaired one.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
