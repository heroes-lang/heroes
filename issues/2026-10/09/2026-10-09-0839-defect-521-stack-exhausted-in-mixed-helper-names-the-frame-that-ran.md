---
kind: defect
area: compiler
milestone: none
filed: 2026-10-09
commit: 7e761a991d0589f257ca1d84692dfd0e90adf87d
github: none
---

- [ ] **521 — `stack exhausted in mixed.helper` names the frame that ran out, not the recursion** | `climbs(helper(n))`: the panic names `helper`, the last frame, where the recursion is `climbs` (panel 199's compiler-engineer, route (M)) | the stack guard's message, `runtime/parts/stack.c` · panel 199 · **class: adjacent**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from panel 199's R7 and its compiler-engineer's report.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `7e761a99`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net, `run` whole and, as a runtime item, the push's platform legs are owed. At the abort the guard walks the frames again, a function met twice being a recursion, and says it beside the frame that ran out: *panic: stack exhausted in behind.helper, inside the recursion of behind.climbs*, a mutual one in call order from the first by name; where nothing repeats or the cycle is the frame named, the first sentence stands alone, and Windows, which walks no frames, is unchanged. Run on this Mac and Linux arm64 at both levels, every line true; three surface checks over `recursion521/`, two red on the base runtime.
