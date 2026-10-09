---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 2549a0b744fe82eee5044600d0c5b30339c2b4d5
github: none
---

- [ ] **544 — `cannot_infer` tells a program to pass a value where a `T?` is expected when it did** | `g(x: 1, n: ok(2))` with `n: i64?` is refused `cannot_infer` with *pass it where a `T?` is expected*, which is what the program did (panel 201's spec-warden) | the `cannot_infer` message of `selfhost/check/` · panel 201 · **class: blocking**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a false message.

    Repaired at `2549a0b7`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `check/generic_argument.hero` tells again the `cannot_infer` said at exactly a generic function's argument when the argument is `ok(…)`, `fail(…)`, a bare case, `[]` or `{}`: *`ok(…)` takes its type from the type the context asks for, and a generic function's parameter asks for none, `n` of `g` included*, its guess binding the program's own text first with the parameter's own type. One case, `check/fixedbugs-544-…`, seven shapes red on the base; `check` 639, `full` 28, `permissive` 16 and the compiler's 1,532 tests, 0 failed. Left as found beside it: `ok(1).h()` with `h(n: i64?)` keeps the old message, a UFCS receiver asking for none by another cause.
