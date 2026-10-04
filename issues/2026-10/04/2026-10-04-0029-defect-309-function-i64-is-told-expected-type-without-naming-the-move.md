---
kind: defect
area: parse
milestone: none
filed: 2026-10-04
commit: 3416238a14755718d1b992a0c42fe338d0943b90
github: none
---

- [ ] **309 — `function( -> i64)` is told `expected_type` without naming the move it needs** | a function type with its parameters' `)` moved past the arrow and no parameter, `function( -> i64)`: `expected_type`, which says nothing of the `)` (lane b9-recovery's compiler, 2026-10-04) | `selfhost/parse/type.hero` · defect 197 · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), beside 197.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `3416238a`, 2026-10-05 (lane b11-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
