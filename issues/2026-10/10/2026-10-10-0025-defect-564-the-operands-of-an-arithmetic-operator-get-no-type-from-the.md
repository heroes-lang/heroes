---
kind: defect
area: check
milestone: none
filed: 2026-10-10
commit: ce9c980b755e8a49c8e97d9e89e7a1c8b72ed658
github: none
---

- [ ] **564 — the operands of an arithmetic operator get no type from the context, so `y: u8 = 2 + 3` is refused** | `y: u8 = 2 + 3`, `show(v: 2 + 3)` with `show(v: u8)`, `e: u8 = 2 * (3 + 4)` and `d: f32 = 0.5 * 2.0` are refused *expected `u8`, found `i64`* where spec § 2 and design.md `:968-969` give a literal the width its context asks for; `1 + b` and `(2)` are accepted; panel 042 (ratified 2026-08-12) named this residual at its lines 251 and 252, *`b @ 1 + 2` still fails, because `Binary` is not a ⇐ form; another ~15 lines. Total ~30*, the price of its ruling 3 (*full adoption*, line 286), and it never landed | the checker's binary operator, `selfhost/check/`; the shapes beside it to attack in one pass: unary `-`, `%`, the shifts, a comparison where neither side has a type, an `if` or `match` value in an annotated position · panel 203 R3 · **class: blocking**

    **Origin:** filed by the coordinator at 00:25 on 2026-10-10 from panel 203 (`docs/panel/203-an-argument-with-no-type-of-its-own-waits-for-the-call-s-others-and-a-self-call-through-a-parameter-aborts-at-run-time.md`): found by the spec-warden, its ruling traced by the critic's second pass, each the seat's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a correct program refused, against a ratified ruling.

    Repaired at `ce9c980b`, 2026-10-10 (lane b18-infer, panel 203 R3), gated by its cases and the compiler's own tests; the net is owed at the batch's close. An arithmetic operator, or a minus, whose every operand has no type of its own takes the type its position asks for and hands it to each operand where every literal can take it; an operand with none takes the other operand's, a generic call, `ok(…)` and an operator of literals among them, the operator tree asked once so a long chain stays linear. The four shapes, `b8 + (2 + 3)` on either side, an `if` and a `match` value are accepted at their widths; `300 + 1` in a `u8` is `int_out_of_range`; `1 + 2.0` in an `f32` keeps `mixed_arithmetic`; the shifts and the bitwise set stay `i64`; `m2` aborts 134 on the overflow. `repeat("-", 0 - 1)` now checks and aborts when it runs, its count's operands taking `u64`. Cases `run/fixedbugs-564-…` (two) and `check/fixedbugs-564-…`, two goldens corrected beneath; `check` 652, `full` 33, `permissive` 16, `annotations` 943, `fixes` 936, the compiler's 1,547 tests, 0 failed; the census moves 2 files against 541's route, none to refused, and the IR of the 1,636 both accept is identical; `check selfhost/main.hero` +0.36% in instructions retired.
