---
kind: defect
area: resolve
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **386 — a rename's sixteen trial readings each ask the did-you-mean again for every unknown name** | after defect 372's repairs the critic's file, one line of 2,000 `totl` with `total` in scope, still costs 68.5 billion instructions against 3.9 with no rename to try: each of `rename_fit.judged`'s sixteen trial readings resolves the whole program, and its did-you-mean queries repeat the first reading's; a memo of the last query in `resolve/state.near_names`, exact because `nearest` depends on the name and the candidate list alone, read 24.5 billion, the verdicts byte-identical (lane fit12's measurement, scratch only, not re-run by the coordinator) | `selfhost/rename_fit.hero`, `selfhost/resolve/state.hero` · panel 187, defect 220 · defect 372 · **class: improvement**

    **Origin:** lane b12-fit12, 2026-10-06 (its final reply, decision 1); filed by the coordinator at 11:26.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): the sixteen readings are panel 187's cost, linear in the program since 372's repairs; a cost no rule promises against, outside the batch under the author's instruction of 2026-10-05. Fewer readings per rename would change which renames are made certain, which is a sitting's question, never a lane's.
