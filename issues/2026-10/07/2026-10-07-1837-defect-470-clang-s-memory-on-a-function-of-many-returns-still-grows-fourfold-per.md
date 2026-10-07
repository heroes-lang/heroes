---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **470 — clang's memory on a function of many returns still grows fourfold per doubling** | under `-g` and under line tables alike, 3.7 to 4.0 times per doubling of the returns at `-O2` (panel 197's compiler-engineer, 400 and 800 returns); panel 197's (C) lowers the constant only, so defect 322 closing does not record the growth | the emitted C of a function of many returns · defect 322 · panel 197's R7 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from panel 197's R7.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a growth on extreme shapes, no program wrong.
