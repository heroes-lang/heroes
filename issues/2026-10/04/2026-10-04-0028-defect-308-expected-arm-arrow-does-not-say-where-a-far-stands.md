---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: 35b67762974796762524f09ae4ab37e23f0ce935
github: none
---

- [ ] **308 — `expected_arm_arrow` does not say where a far `=>` stands** | an arm whose `=>` stands further along the line than the reader looked: `expected_arm_arrow` is told without naming where the `=>` is (lane b9-recovery's compiler, 2026-10-04) | `selfhost/grammar_expr.hero` (the arm reader) · defect 195 · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), beside 195.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `35b67762`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
