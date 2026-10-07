---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **477 — a call with thousands of arguments costs the checker faster than linear** | many-params-3200: `checkwalk.named_call` 1,935 of 6,385 samples (lane b14-ir), `check/walk.hero`'s `user_call` 3,757 of 3,946 on 3,200 fields (lane b14-resolve); samples, not counts | `selfhost/check/walk.hero` · defect 260 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-resolve's and lane b14-ir's final reports; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost on extreme shapes.
