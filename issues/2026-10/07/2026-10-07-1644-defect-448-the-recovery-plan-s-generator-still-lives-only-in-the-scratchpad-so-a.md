---
kind: defect
area: process
milestone: none
filed: 2026-10-07
commit: 7561d0b97efaaf61f07b23a6c92a76bf258ea429
github: none
---

- [ ] **448 — the recovery plan's generator still lives only in the scratchpad, so a plan replays from the tree and cannot be cut again** | defect 210 brought the replay, the comparison and a frozen plan into `heroes mutate --recovery`; the plan's generator (`ops.py`, `model.py` and the rest under `<scratchpad>/instrument/tool/`, about 4,200 lines by the lane's count) did not move | `selfhost/mutate/` · defect 210 · panel 187's R2 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-mutate's final report (*found beside*, not repaired).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an instrument's home, the cause defect 210 named; no program moves.

    Repaired at `7561d0b9`, 2026-10-08 (lane b15-mutate), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `heroes mutate --recovery --cut <tree> --compiler <c> -o <folder>` cuts a plan from a tree's programs: with 84430015's compiler over the c85bccb8 tree it writes the frozen `plan-singles.jsonl` (13,594) and `plan-pairs.jsonl` (16,041) and `corpus/` (653) byte for byte, and with the lane's compiler it and the Python write one plan, base and site count, byte for byte; the cut retires 1.69e12 instructions; one rule is a count (a use closure over 512 KiB) where the Python's was a duration (0.8 s), which leaves out the same nine programs; the case is `tests/golden/recovery/cut/`; `--cut` waits on its row in `selfhost/cli/table.hero`.
