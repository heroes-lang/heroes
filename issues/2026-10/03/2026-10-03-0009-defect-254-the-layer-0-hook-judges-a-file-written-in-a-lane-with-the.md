---
kind: defect
area: compiler
milestone: none
filed: 2026-10-03
commit: c821ef3832b8aecdb039058568a6123deaa6843f
github: none
---

- [x] **254 — the layer-0 hook judges a file written in a lane with the compiler of the session's directory, and skips the whole compiler's check there** | `.claude/hooks/fmt_check.py` takes the compiler as `<cwd>/heroes` and the file's path relative to `<cwd>`: a `selfhost/` module written under `.claude/worktrees/<lane>/` by a session whose directory is the trunk is formatted by the trunk's compiler and reads as `.claude/worktrees/<lane>/selfhost/...`, which is not `selfhost/`, so `heroes check selfhost/main.hero` never runs for it (read by the coordinator, 2026-10-04; batch 8's FFI lane, 2026-10-03, its report's finding 7) | `.claude/hooks/fmt_check.py:75` to `:119` · `.claude/rules/verification.md` § A suite is the last judge, layer 0 · **class: improvement**

    **Origin:** batch 8's FFI lane, 2026-10-03 (its report's finding 7); the hook read by the coordinator, 2026-10-04.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach; no program moves.

    Repaired at `c821ef38`, 2026-10-07 (lane b14-hooks), gated by its cases, the hooks' own tests, and `records`; the net is owed at the batch's close. `fmt_check.py` judges a written file in the tree it stands in, the nearest directory above it holding `seed/heroes.c` and a `.git` entry (`.claude/hooks/trees.py`), with that tree's compiler and its paths read from that tree's root; `.claude/hooks/test_hooks.py`, the hooks' first tests, holds seven cases, four red on the base and seven green, and a lane module calling an unknown name, written from a scratch trunk, read exit 0 on the base hook and exit 2 *does not check* on the repaired one.

## The repair

Repaired at `c821ef38`, 2026-10-07 (lane b14-hooks), gated by its cases, the hooks' own tests, and `records`; the net is owed at the batch's close. `fmt_check.py` judges a written file in the tree it stands in, the nearest directory above it holding `seed/heroes.c` and a `.git` entry (`.claude/hooks/trees.py`), with that tree's compiler and its paths read from that tree's root; `.claude/hooks/test_hooks.py`, the hooks' first tests, holds seven cases, four red on the base and seven green, and a lane module calling an unknown name, written from a scratch trunk, read exit 0 on the base hook and exit 2 *does not check* on the repaired one.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
