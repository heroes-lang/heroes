---
kind: defect
area: cli
milestone: none
filed: 2026-10-03
commit: 2cc846cc1f74f87ba7bdaa6bfd2b6e55e9c3eafa
github: none
---

- [ ] **210 — the recovery instrument lives in the scratchpad as a Python tool, where a reboot can lose it; its home is `heroes mutate`'s recovery arm, with its plan pinned** | `<scratchpad>/instrument/tool/` (3,618 code lines of Python, the compiler-engineer's count); panel 187's R2 reads it at every recovery round's gate, and the fourth round's reading had to subtract a corpus program the language changed under it by hand | `selfhost/cli/mutate.hero` and `selfhost/mutate/` · panel 187's R2 and R3 · **class: improvement**

    **Origin:** panel 187's R10 (filed beside the sitting), 2026-10-03, and the fourth round's differential reading (`scratchpad/inst-187/r4-differential.txt`, 2026-10-03): the port owes a frozen plan in the tree and the subtraction of an unmutated program's own messages.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an instrument's home; no program moves.

    **2026-10-04, batch 8's gate**: the comparator read beside the instrument, `<scratchpad>/inst-187/differential.py`, took a pair's *told* status for a boolean where the instrument writes a word, `same`, `other` or `hidden`, so a told second newly hidden, one of R2's three findings, could not be one. Repaired there and re-run: rounds 4 and 6 moved no told status, batch 8 moved one pair (defect 271). The port owes the comparison with the instrument, in the tree, with a test that makes each finding fire.

    Repaired at `2cc846cc`, 2026-10-07 (lane b14-mutate), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The plan, its 653-file corpus and the trunk's record of the corpus's own messages are pinned in `tests/golden/recovery/` (`1ce644cb`, `2cc846cc`), `heroes mutate --recovery --compiler <c> -o <run>` replays it (`90419ccc`) and `--compare <base>` reads two runs (`3d2b39f4`), and on the trunk's compiler the arm and the Python agree on every field of 13,594 of 13,594 singles and 16,041 of 16,041 pairs; the arm is reachable from argv once `selfhost/cli/table.hero` gains its four flag rows.
