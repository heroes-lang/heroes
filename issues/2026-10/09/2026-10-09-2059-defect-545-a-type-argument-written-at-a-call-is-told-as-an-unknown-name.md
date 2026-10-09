---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **545 — a type argument written at a call is told as an unknown name** | `ident<i64>(3)` is told *nothing named `i64` is in scope*, never that a generic function's type arguments are never written (spec § 9) (panel 201's spec-warden) | `selfhost/check/` or `selfhost/parse/` · spec § 9 · **class: adjacent**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a message that never names the rule.
