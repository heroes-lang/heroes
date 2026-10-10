---
kind: defect
area: check
milestone: none
filed: 2026-10-10
commit: 53df435ee240025c8a7853a612c75123e3a38ca4
github: none
---

- [ ] **578 — an operator over literals passed to a generic function's parameter takes no type from the call's context** | on batch 18's round (`e0aeb991`, defects 541 and 564 landed), `y: u8 = id(a: 2 + 3)` with `id<T>(a: T) -> T` is refused *expected `u8`, found `i64`*, while `y: u8 = id(a: 5)` builds and prints 5; panel 206's critic reads the same of `first(a: 2 + 3, b: 1)` and `first(a: 1, b: 2 + 3)` while `first(a: 2, b: 1)` is accepted; spec § 9's V3T, landed in the round, says a type parameter takes its type *else from the type the context asks for, else from a generic call or a literal among them*, so a reader accepts all three; reproduced by the coordinator at 05:01 with the round's compiler (`.claude/worktrees/scratch-b15/r578/`) | defect 541's waiting forms (`selfhost/check/waiting.hero` in batch 18's round, 2026-10-10) and 564's operand typing: an operator whose operands are all literals waits as a literal does · defects 541 and 564 · **class: blocking**

    **Origin:** found by panel 206's completeness critic, first pass (its shapes `u01` to `u03` and `t01`, `.claude/worktrees/scratch-b15/critic-206/shapes/`, ignored by git), reproduced and filed by the coordinator at 05:02 on 2026-10-10.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a correct program refused, against the spec the same batch lands.

    Repaired at `53df435e`, 2026-10-10 (lane b18-infer), gated by its cases and the compiler's own tests; the net is owed at the batch's close. An arithmetic operator or a minus whose every operand waits now waits too, weighs what its operands weigh, takes a context only where its literals can, and is checked against its parameter with defect 564's operand typing. `y: u8 = id(a: 2 + 3)`, `first(a: 2 + 3, b: 1)` and its swap, `first(a: 2 + 3, b: b8)` and its swap, `(2 + 3) * 4`, `-(2 + 3)`, a generic call among the operands and a receiver are accepted at the settled type; `300 + 1` at a `u8` is `int_out_of_range`, `2 + 3` beside `2.5` refused in both orders, `2 + 3.0` keeps `mixed_arithmetic`, a context the literals cannot take is told once at the call. `id(a: 200 + 100)` at a `u8` aborts when it runs, as `y: u8 = 200 + 100` does since 564, the ground panel 206 rules on (577), and no case pins it. Cases `run/fixedbugs-578-…` (12 values) and `check/fixedbugs-578-…`; `check` 654, `full` 34, `annotations` 946, `fixes` 941, the compiler's 1,548 tests, 0 failed; no file moves in the census and the IR of the 1,643 both accept is identical; `check selfhost/main.hero` unchanged in instructions retired.
