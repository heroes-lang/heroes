---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: 1ef2ccfe0e7035b514372ac5d3da048730e97e7c
github: none
---

- [ ] **490 — `cli/deep_types.hero`'s walk without recursion has no witness** | defect 189's shape in the cli's own walk over the deepest by-value chain: a mutant restoring its recursion is caught by nothing (lane b14-emit's reading) | `selfhost/cli/deep_types.hero` · defect 189 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-emit's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): coverage.

    Repaired at `1ef2ccfe`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The walk takes its graph as data (`held_graph` by a queue, `deepest_of` with its own stack), and a test holds it to a chain of 100,000: the mutant restoring the recursion fails it, `panic: stack exhausted in clideeptypes.measured`, defect 170's test passing; the tree passes.
