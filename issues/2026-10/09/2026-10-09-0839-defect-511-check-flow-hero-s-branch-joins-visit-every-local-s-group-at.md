---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 87e49bae9c4256fa05eecec94422490be9f6ddd7
github: none
---

- [ ] **511 — `check/flow.hero`'s branch joins visit every local's group at each meet** | n handles ended, then n `if`s: 3.24, 11.3 and 41.2 billion instructions at 500, 1,000 and 2,000 after defect 499's repair, quadratic (cubic on the base, 93.9 and 742 billion at 500 and 1,000); shared or copy-on-write groups would answer it (lane b15-check) | `selfhost/check/flow.hero` · defect 499 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-check's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the square of the handles.

    Repaired at `87e49bae`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The walk splits the flow where control splits, each event logs its local once per open split (`check/flow_log.hero`), and a meet starts from the split and meets only the locals the paths logged, a local no path changed being the base's on both. `check` of n handles ended then n `if`s reads 0.90, 1.72 and 3.36 billion instructions at 500, 1,000 and 2,000 where it read 3.26, 11.39 and 41.35; every output byte-identical, and the 224 tracked programs that consume, transfer or retain check unmoved. Ends inside both branches of n `if`s go from 2.56, 8.16 and 27.99 to 1.49, 3.73 and 10.73, what is left being a path's first change copying the flat group array, a cause apart.
