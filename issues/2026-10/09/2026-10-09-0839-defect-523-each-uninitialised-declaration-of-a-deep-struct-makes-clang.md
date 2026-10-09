---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **523 — each uninitialised declaration of a deep struct makes clang walk its nesting again** | 2,000 declarations at depth 1,000, 2,000 and 4,000 cost +0.57, +1.10 and +2.16 billion instructions over the same declarations with `= {0}`; the one emitted shape that avoids it reverses panel 182's ruling that a temporary gets no initialiser, so it is a sitting's (lane b15-parse) | the prologue in `selfhost/emit/` · panel 182 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost a ruling stands in front of.
