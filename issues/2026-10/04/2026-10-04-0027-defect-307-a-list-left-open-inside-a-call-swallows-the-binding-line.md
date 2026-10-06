---
kind: defect
area: parse
milestone: none
filed: 2026-10-04
commit: afde4848915dbcbc5d43e9fa5f81294bb27a454c
github: none
---

- [ ] **307 — a list left open inside a call swallows the binding line below it, its mistake told only once the list is closed** | the shape of defect 204 inside a call's arguments: the list's recovery takes the binding line below it, and that line's own mistake waits for the list's repair (lane b9-recovery's compiler, 2026-10-04) | `selfhost/parse/unclosed.hero` · `selfhost/grammar_expr.hero` (a call's arguments) · defect 204 · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), beside 204.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only after another is fixed.

    Repaired at `afde4848`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
