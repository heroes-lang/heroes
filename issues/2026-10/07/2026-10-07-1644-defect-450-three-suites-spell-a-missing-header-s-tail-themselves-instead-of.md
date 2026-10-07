---
kind: defect
area: harness
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **450 — three suites spell a missing header's tail themselves instead of reading it from `absence`** | `suite_annotations`, `suite_corpus` and `suite_special` spell *clang looked and did not find it* themselves; it matches the compiler today, and `absence.HEADER_LOOKED` holds it since defect 328 (lane b14-harness-a, 2026-10-07) | the three suites · defect 328 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-harness-a's final report (*found beside*).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): copies that can lag the compiler, defect 328's own cause; no case reads wrong today.
