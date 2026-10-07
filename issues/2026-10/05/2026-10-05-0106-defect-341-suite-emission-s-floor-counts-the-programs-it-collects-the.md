---
kind: defect
area: harness
milestone: none
filed: 2026-10-05
commit: 470de4b8a0fabcd62ac451a455041ea0a2a3a1d8
github: none
---

- [ ] **341 — `suite_emission`'s floor counts the programs it collects, the 30 `--emit-c` refuses among them** | `LEAST` in `tests/harness/suite_emission.hero` counts the cases collected, and 30 of the 38 `fixedbugs/` cases exit 1 under `build --emit-c` with no C to bless, so the floor counts programs the suite passes without an emission (lane b11-misc, 2026-10-05, the lane's report) | `tests/harness/suite_emission.hero` (`LEAST`) · defect 298 · **class: improvement**

    **Origin:** lane b11-misc, 2026-10-05, beside defect 298's repair (its final report, *Found beside*).

    **Class: improvement**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): hardening of an instrument's floor; no program is judged wrong for it.

    Repaired at `470de4b8`, 2026-10-07 (lane b14-harness-a), gated by its cases and the net's own tests; the net is owed at the batch's close. `LEAST` counts the programs whose emission is blessed, 467 of the 502 collected that day, and `LEAST_REFUSED` the 35 with nothing blessed (`fixedbugs/` 32 of 40, `ir/` 3), each still read as a refusal; its case, one blessed program and one refused under a floor of two, read 0 failed over the base's `sweep` where 1 is owed; `emission` 972 passed, 0 failed (971 on the base, the one more the refusals' floor), the net's own tests 290, all passed.
