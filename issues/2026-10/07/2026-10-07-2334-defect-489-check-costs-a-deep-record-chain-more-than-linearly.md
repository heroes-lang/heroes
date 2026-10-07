---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: 88528669c22d5397f7d2d94262c0866e9ac58a80
github: none
---

- [ ] **489 — `check` costs a deep record chain more than linearly** | 16.5 billion instructions at 5,000 records, 60.7 billion at 10,000, about the square (lane b14-emit, while witnessing defect 189) | `selfhost/check/`, `selfhost/resolve/` · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-emit's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the chain's square.

    Repaired at `88528669`, 2026-10-08 (lane b15-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `check/sized.hero`'s walk lists each declaration's edges once and pops its stack by a height, where it scanned every edge at every step and popped by a slice (88% of the run at 8,000, sampled): `check` on a chain of 1,000 / 2,000 / 4,000 / 8,000 records reads 0.65 / 1.37 / 3.33 / 9.21 billion instructions where it read 1.09 / 3.10 / 10.2 / 36.7, every verdict byte-identical, and what still grows is `table.intern`'s push, defect 409's shape, repaired in lane b14-p409.
