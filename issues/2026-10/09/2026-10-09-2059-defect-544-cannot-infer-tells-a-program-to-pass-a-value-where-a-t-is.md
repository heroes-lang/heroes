---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **544 — `cannot_infer` tells a program to pass a value where a `T?` is expected when it did** | `g(x: 1, n: ok(2))` with `n: i64?` is refused `cannot_infer` with *pass it where a `T?` is expected*, which is what the program did (panel 201's spec-warden) | the `cannot_infer` message of `selfhost/check/` · panel 201 · **class: blocking**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a false message.
