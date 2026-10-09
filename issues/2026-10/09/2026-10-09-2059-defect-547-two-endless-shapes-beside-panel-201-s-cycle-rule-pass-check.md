---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **547 — two endless shapes beside panel 201's cycle rule pass `check`** | a function calling itself or another that calls it back, found by following first calls (`selfloop_first`), and a self-call through a parameter (`go(n: n, f: step)` inside `step`) pass the rule panel 201 R3 lands; each aborts at run time (panel 201's compiler-engineer) | `selfhost/check/`, the cycle rule panel 201 R3 lands · defect 520 · **class: improvement**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a refusal the rule does not yet reach, the run still aborting.
