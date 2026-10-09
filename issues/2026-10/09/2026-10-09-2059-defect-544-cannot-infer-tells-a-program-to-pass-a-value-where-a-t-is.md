---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 2549a0b744fe82eee5044600d0c5b30339c2b4d5
github: none
---

- [x] **544 — `cannot_infer` tells a program to pass a value where a `T?` is expected when it did** | `g(x: 1, n: ok(2))` with `n: i64?` is refused `cannot_infer` with *pass it where a `T?` is expected*, which is what the program did (panel 201's spec-warden) | the `cannot_infer` message of `selfhost/check/` · panel 201 · **class: blocking**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a false message.

    Repaired at `2549a0b7`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `check/generic_argument.hero` tells again the `cannot_infer` said at exactly a generic function's argument when the argument is `ok(…)`, `fail(…)`, a bare case, `[]` or `{}`: *`ok(…)` takes its type from the type the context asks for, and a generic function's parameter asks for none, `n` of `g` included*, its guess binding the program's own text first with the parameter's own type. One case, `check/fixedbugs-544-…`, seven shapes red on the base; `check` 639, `full` 28, `permissive` 16 and the compiler's 1,532 tests, 0 failed. Left as found beside it: `ok(1).h()` with `h(n: i64?)` keeps the old message, a UFCS receiver asking for none by another cause.

## The repair

Repaired at `2549a0b7`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `check/generic_argument.hero` tells again the `cannot_infer` said at exactly a generic function's argument when the argument is `ok(…)`, `fail(…)`, a bare case, `[]` or `{}`: *`ok(…)` takes its type from the type the context asks for, and a generic function's parameter asks for none, `n` of `g` included*, its guess binding the program's own text first with the parameter's own type. One case, `check/fixedbugs-544-…`, seven shapes red on the base; `check` 639, `full` 28, `permissive` 16 and the compiler's 1,532 tests, 0 failed. Left as found beside it: `ok(1).h()` with `h(n: i64?)` keeps the old message, a UFCS receiver asking for none by another cause.

**Closed 2026-10-09** with batch 17 (lanes b17-fix, b17-emit and b17-check, landing panels 200 and 201 as ratified that evening, merged into one round tree made from the trunk), under the optimistic chain (`.claude/rules/verification.md` § The optimistic chain), its closing gate run on the round at `46b80b82`: the seed regenerated over two generations, the runtime's ABI moved to 30 by defect 470, 47,052,134 bytes, SHA-256 beginning `2b668ee89a3697cd`, its fixpoint by `cmp`, the committed seed of ABI 29 built against the runtime it was emitted for; the compiler's own tests 1,542, all passed; the net's own tests 326, all passed; the full net 7,306 passed over 29 suites, 0 failed, `run` in four shards of the harness's own (defect 537), 426 of 426 cases read, one skipped on this Mac (defect 437's case, `sys/prctl.h`); one floor told outgrown, `full`, raised in the closing commit as defect 536's rule asks. The census and panel 187's R2 run after the push, beside the CI.
