---
kind: defect
area: parse
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **410 — a line of ten thousand openers is told ten thousand `unclosed_bracket`** | a line of 10,000 opening brackets gets one `unclosed_bracket` per bracket, on the base and on lane front alike | `selfhost/parse/`, the pairing of brackets · **class: improvement**

    **Origin:** lane b13-front, 2026-10-06 (its report, *found beside, not filed*), the lane's measurement on its branch from `7001dfb3`, not re-run by the coordinator; filed by the coordinator at 20:41.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a pathological input, no program a writer means; at most an improvement.
