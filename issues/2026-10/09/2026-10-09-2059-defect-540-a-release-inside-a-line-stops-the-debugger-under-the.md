---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **540 — a release inside a line stops the debugger under the generated file** | with the prologue carrying the function's line, `step`, `next`, `next` stops at `dbg.hero:1`, `dbg.hero:2:10`, then `dbg.c:70:5`, a release inside line 2 mapped to the generated file (panel 200's compiler-engineer, lldb on this Mac) | the release's `#line` in `selfhost/emit/` · panel 200 R4 · defect 472 · **class: improvement**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 200 (`docs/panel/200-a-counted-slot-is-released-by-its-address-and-the-emitted-program-s-other-routes-are-ruled.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a debugger stop less exact than it could be.
