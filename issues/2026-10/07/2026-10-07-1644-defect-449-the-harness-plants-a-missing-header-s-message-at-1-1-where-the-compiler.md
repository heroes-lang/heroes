---
kind: defect
area: harness
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **449 — the harness plants a missing header's message at `:1:1`, where the compiler says the group's first declaration** | the harness's planted missing-header messages sit at `:1:1`, the compiler's at the group's first declaration (`:2:5` in the lane's case); left because moving it moves other suites' mark tests (lane b14-harness-a, 2026-10-07) | `tests/harness/absence.hero` · defect 328 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-harness-a's final report (*found beside*).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a planted message less exact than the compiler's; no case reads wrong today.
