---
kind: defect
area: check
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **564 — the operands of an arithmetic operator get no type from the context, so `y: u8 = 2 + 3` is refused** | `y: u8 = 2 + 3`, `show(v: 2 + 3)` with `show(v: u8)`, `e: u8 = 2 * (3 + 4)` and `d: f32 = 0.5 * 2.0` are refused *expected `u8`, found `i64`* where spec § 2 and design.md `:968-969` give a literal the width its context asks for; `1 + b` and `(2)` are accepted; panel 042 (ratified 2026-08-12) named this residual at its lines 251 and 252, *`b @ 1 + 2` still fails, because `Binary` is not a ⇐ form; another ~15 lines. Total ~30*, the price of its ruling 3 (*full adoption*, line 286), and it never landed | the checker's binary operator, `selfhost/check/`; the shapes beside it to attack in one pass: unary `-`, `%`, the shifts, a comparison where neither side has a type, an `if` or `match` value in an annotated position · panel 203 R3 · **class: blocking**

    **Origin:** filed by the coordinator at 00:25 on 2026-10-10 from panel 203 (`docs/panel/203-an-argument-with-no-type-of-its-own-waits-for-the-call-s-others-and-a-self-call-through-a-parameter-aborts-at-run-time.md`): found by the spec-warden, its ruling traced by the critic's second pass, each the seat's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a correct program refused, against a ratified ruling.
