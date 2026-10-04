---
kind: defect
area: parse
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **306 — two stray closers on one line are told one at a time, the second only once the first is removed** | a line holding two closers no opener asked for: the first is told, the second only after the first is deleted (lane b9-recovery's compiler, 2026-10-04) | `selfhost/parse/` (the stray closer's report) · defects 198 and 199, a closer past a head · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`).

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only after another is fixed.
