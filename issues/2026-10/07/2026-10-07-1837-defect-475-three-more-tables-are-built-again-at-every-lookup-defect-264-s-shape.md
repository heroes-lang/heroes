---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: 6630fe83f5f641fed6e238da6c6cad1756731be1
github: none
---

- [ ] **475 — three more tables are built again at every lookup, defect 264's shape** | on the compiler's own check: `widths.int_kinds` 22,187 calls, `widths.float_kinds` 16,103, `operators.binary_tokens` 3,467 (lane b14-check) | `selfhost/widths.hero`, `selfhost/operators.hero` · defect 264 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-check's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost per lookup.

    Repaired at `6630fe83`, 2026-10-08 (lane b15-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The three lists are constants, `INT_KINDS`, `FLOAT_KINDS` and `BINARY_TOKENS`, each one static block (panel 195) that its function returns, so no caller changes and a call builds nothing: on the compiler's own check of one frozen input the calls stay 23,606, 17,048 and 3,663 and the instructions retired fall from 67.59 to 67.47 billion, every output byte-identical.
