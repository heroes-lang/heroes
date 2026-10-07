---
kind: defect
area: cli
milestone: none
filed: 2026-10-03
commit: 2cc846cc1f74f87ba7bdaa6bfd2b6e55e9c3eafa
github: none
---

- [x] **210 — the recovery instrument lives in the scratchpad as a Python tool, where a reboot can lose it; its home is `heroes mutate`'s recovery arm, with its plan pinned** | `<scratchpad>/instrument/tool/` (3,618 code lines of Python, the compiler-engineer's count); panel 187's R2 reads it at every recovery round's gate, and the fourth round's reading had to subtract a corpus program the language changed under it by hand | `selfhost/cli/mutate.hero` and `selfhost/mutate/` · panel 187's R2 and R3 · **class: improvement**

    **Origin:** panel 187's R10 (filed beside the sitting), 2026-10-03, and the fourth round's differential reading (`scratchpad/inst-187/r4-differential.txt`, 2026-10-03): the port owes a frozen plan in the tree and the subtraction of an unmutated program's own messages.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an instrument's home; no program moves.

    **2026-10-04, batch 8's gate**: the comparator read beside the instrument, `<scratchpad>/inst-187/differential.py`, took a pair's *told* status for a boolean where the instrument writes a word, `same`, `other` or `hidden`, so a told second newly hidden, one of R2's three findings, could not be one. Repaired there and re-run: rounds 4 and 6 moved no told status, batch 8 moved one pair (defect 271). The port owes the comparison with the instrument, in the tree, with a test that makes each finding fire.

    Repaired at `2cc846cc`, 2026-10-07 (lane b14-mutate), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The plan, its 653-file corpus and the trunk's record of the corpus's own messages are pinned in `tests/golden/recovery/` (`1ce644cb`, `2cc846cc`), `heroes mutate --recovery --compiler <c> -o <run>` replays it (`90419ccc`) and `--compare <base>` reads two runs (`3d2b39f4`), and on the trunk's compiler the arm and the Python agree on every field of 13,594 of 13,594 singles and 16,041 of 16,041 pairs; the arm is reachable from argv once `selfhost/cli/table.hero` gains its four flag rows.

## The repair

Repaired at `2cc846cc`, 2026-10-07 (lane b14-mutate), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The plan, its 653-file corpus and the trunk's record of the corpus's own messages are pinned in `tests/golden/recovery/` (`1ce644cb`, `2cc846cc`), `heroes mutate --recovery --compiler <c> -o <run>` replays it (`90419ccc`) and `--compare <base>` reads two runs (`3d2b39f4`), and on the trunk's compiler the arm and the Python agree on every field of 13,594 of 13,594 singles and 16,041 of 16,041 pairs; the arm is reachable from argv once `selfhost/cli/table.hero` gains its four flag rows.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
