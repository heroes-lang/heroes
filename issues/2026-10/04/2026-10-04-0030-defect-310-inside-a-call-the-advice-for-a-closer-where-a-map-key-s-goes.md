---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: 84181f646dbc5f8235acac3b051b3b84dd7fbeb2
github: none
---

- [ ] **310 — inside a call, the advice for a closer where a map key's `:` goes is true and incomplete** | defect 205's shape inside a call's arguments: the second reading's advice is true and leaves out the call (lane b9-recovery's compiler, 2026-10-04) | `selfhost/grammar_expr.hero`, `selfhost/parse/list_line.hero` · defect 205 · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), beside 205.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `84181f64`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
