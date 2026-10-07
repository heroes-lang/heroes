---
kind: defect
area: process
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **448 — the recovery plan's generator still lives only in the scratchpad, so a plan replays from the tree and cannot be cut again** | defect 210 brought the replay, the comparison and a frozen plan into `heroes mutate --recovery`; the plan's generator (`ops.py`, `model.py` and the rest under `<scratchpad>/instrument/tool/`, about 4,200 lines by the lane's count) did not move | `selfhost/mutate/` · defect 210 · panel 187's R2 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-mutate's final report (*found beside*, not repaired).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an instrument's home, the cause defect 210 named; no program moves.
