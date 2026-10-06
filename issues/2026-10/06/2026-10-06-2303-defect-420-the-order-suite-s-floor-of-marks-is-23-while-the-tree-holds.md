---
kind: defect
area: harness
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **420 — the order suite's floor of marks is 23 while the tree holds 28** | `# ORDER:` marks counted 28 against the `order` suite's floor of 23; one more mark asks the floor raised; lane b13-run400 rewrote its draft to avoid adding one | `tests/harness/suite_order.hero` · **class: improvement**

    **Origin:** lane b13-run400, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a floor behind its count, no program judged wrong.
