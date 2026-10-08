---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: 1a6499ed2092008331ae601835c0e6d155403883
github: none
---

- [ ] **499 — `check/flow.hero`'s `ended` rebuilds the dead list at every end** | 23,994,000 copies at 4,000 handles ended: the filter that rebuilds the list, not a field push (lane b14-p409) | `selfhost/check/flow.hero:153` · defect 409 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-p409's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the handles' square.

    Repaired at `1a6499ed`, 2026-10-08 (lane b15-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The ended places are kept one group per local, each event asks its local's group, a group grows in place and is rebuilt only where an event takes a place out of it, and a meet joins each local with its twin; the end a read is told of is the one list's. `check` of 500 / 1,000 / 2,000 / 4,000 handles ended and read again reads 1.13 / 2.30 / 5.04 / 12.1 billion instructions where it read 2.51 / 7.78 / 26.8 / 99.4, copies of a `Dead` 2,000 and 4,000 at 1,000 and 2,000 where they were 5,866,404 and 23,478,704, and the rest is the walk's reports, defect 409's shape; the 229 tracked programs that end, keep or hand over a handle check unmoved. The same ends followed by n branches cost the cube on the base and the square now (3.24 and 11.3 billion at 500 and 1,000): each join still visits every local, reported to the coordinator.
