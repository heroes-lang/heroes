---
kind: defect
area: parse
milestone: none
filed: 2026-10-04
commit: 23cbe6fd015c6138bb321ebe3d372d03e2c3f1ae
github: none
---

- [ ] **306 — two stray closers on one line are told one at a time, the second only once the first is removed** | a line holding two closers no opener asked for: the first is told, the second only after the first is deleted (lane b9-recovery's compiler, 2026-10-04) | `selfhost/parse/` (the stray closer's report) · defects 198 and 199, a closer past a head · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`).

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only after another is fixed.

    Repaired at `23cbe6fd`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
