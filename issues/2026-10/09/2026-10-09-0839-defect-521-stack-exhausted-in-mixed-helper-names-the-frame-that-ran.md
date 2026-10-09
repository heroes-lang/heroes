---
kind: defect
area: compiler
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **521 — `stack exhausted in mixed.helper` names the frame that ran out, not the recursion** | `climbs(helper(n))`: the panic names `helper`, the last frame, where the recursion is `climbs` (panel 199's compiler-engineer, route (M)) | the stack guard's message, `runtime/parts/stack.c` · panel 199 · **class: adjacent**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from panel 199's R7 and its compiler-engineer's report.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.
