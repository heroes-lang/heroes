---
kind: defect
area: compiler
milestone: none
filed: 2026-10-05
commit: f50b9102628108472a4ed31567c48403bc73fa2f
github: none
---

- [ ] **350 — five more sites push a diagnostic or a row through a copy of the whole list, defect 146's pattern, outside the parser** | `resolve/state.push_diagnostic` (`r.out.diagnostics @ ...push`), `check/state.hero:237`, `resolve/cycles.hero` (two) and `ir/interning.hero` grow their lists by a copy each push, so N reports cost the square of N (lane b11-parse, 2026-10-05, read in the code and named in its report; unmeasured per site) | the five sites · defects 146, 267 and 343, the same pattern repaired elsewhere · **class: adjacent**

    **Origin:** lane b11-parse, 2026-10-05, beside defects 267 and 343 (its final report, *Found beside*).

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a cost that grows as the square of the reports, no message or value wrong; unmeasured per site.

    Repaired at `f50b9102`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
