---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **494 — a run of stray carriage returns, tabs or foreign words is told once per character, past defect 282's bound** | on the base compiler: 4,000 stray carriage returns 4,000 messages and 193 MB and 432 billion instructions; 4,000 tabs 4,001 messages and 22.3 billion; 1,334 foreign words 1,334 `reserved_word` and 14.7 billion; defect 282 bounded the plain refusal only, and 294 shrinks the bytes, not the count (lane b14-text) | the lexer's refusals, `selfhost/scan.hero` and beside · defects 282 and 294 · **class: adjacent**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-text's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): one mistake told thousands of times.
