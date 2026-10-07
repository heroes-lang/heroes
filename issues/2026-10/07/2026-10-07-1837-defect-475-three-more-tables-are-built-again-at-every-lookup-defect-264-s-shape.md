---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **475 — three more tables are built again at every lookup, defect 264's shape** | on the compiler's own check: `widths.int_kinds` 22,187 calls, `widths.float_kinds` 16,103, `operators.binary_tokens` 3,467 (lane b14-check) | `selfhost/widths.hero`, `selfhost/operators.hero` · defect 264 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-check's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost per lookup.
