---
kind: defect
area: harness
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **341 — `suite_emission`'s floor counts the programs it collects, the 30 `--emit-c` refuses among them** | `LEAST` in `tests/harness/suite_emission.hero` counts the cases collected, and 30 of the 38 `fixedbugs/` cases exit 1 under `build --emit-c` with no C to bless, so the floor counts programs the suite passes without an emission (lane b11-misc, 2026-10-05, the lane's report) | `tests/harness/suite_emission.hero` (`LEAST`) · defect 298 · **class: improvement**

    **Origin:** lane b11-misc, 2026-10-05, beside defect 298's repair (its final report, *Found beside*).

    **Class: improvement**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): hardening of an instrument's floor; no program is judged wrong for it.
