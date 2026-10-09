---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: f0a126758de76004bb02ee7f7d4d40deb64350cc
github: none
---

- [ ] **523 — each uninitialised declaration of a deep struct makes clang walk its nesting again** | 2,000 declarations at depth 1,000, 2,000 and 4,000 cost +0.57, +1.10 and +2.16 billion instructions over the same declarations with `= {0}`; the one emitted shape that avoids it reverses panel 182's ruling that a temporary gets no initialiser, so it is a sitting's (lane b15-parse) | the prologue in `selfhost/emit/` · panel 182 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost a ruling stands in front of.

    Measured at `f0a12675`, 2026-10-09 (panel 200, ratified at 20:57): a known cost; most of clang's walk is a call result's assignment, the missing initialiser about 2.2e9 of 12.5e9 at depth 1,000, the seed's deepest chain 8, the corpus's 5, four stress cases past 32; panel 182 stands. Closed on its measurement (panel 200's R5); the deep record's crash under `= {0}` filed apart as defect 539.
