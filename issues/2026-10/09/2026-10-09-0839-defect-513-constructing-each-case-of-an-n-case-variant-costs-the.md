---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 47bc65f3dc901c53795c8d3f82d8026a05e640c4
github: none
---

- [ ] **513 — constructing each case of an N-case variant costs the square of N** | 0.62, 1.59 and 4.90 billion instructions at 400, 800 and 1,600 cases on defect 409's tree; read as `case_construct`'s name scan, unsampled (lane b15-check) | `selfhost/check/` (`case_construct`) · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-check's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the square of the cases.

    Repaired at `47bc65f3`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The checker's `case_construct` asks the table defect 512 keeps, and the lowering keeps its own (`ir/layout.case_index`), where each walked the variant's names at every construction, the lowering too. At 400, 800 and 1,600 cases `check` reads 0.34, 0.61 and 1.14 billion instructions where it read 0.56, 1.52 and 4.78, and `build --dump-ir` 0.69, 1.21 and 2.22 where it read 1.24, 3.39 and 10.92; the IR and the C byte-identical.
