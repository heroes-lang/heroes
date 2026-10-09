---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **520 — mutual recursion and a self-call through a function value pass `check`** | panel 199's R4 refuses a function every path of which calls itself, and lets through `ping` calling `pong` calling `ping` and `f = go` then `f(n)`; since defect 508's repair both abort 134 at every level (panel 199's R7, route (I)) | `selfhost/check/self_call.hero` · panel 199 · **class: improvement**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from panel 199's R7 and lane b16-land199's final report.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a refusal a sitting filed apart, owing a sitting of its own.
