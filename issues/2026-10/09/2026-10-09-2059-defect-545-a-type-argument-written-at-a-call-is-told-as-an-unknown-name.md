---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: e5fa493904db40abeeacc7a74cfcf0cf9371598b
github: none
---

- [x] **545 — a type argument written at a call is told as an unknown name** | `ident<i64>(3)` is told *nothing named `i64` is in scope*, never that a generic function's type arguments are never written (spec § 9) (panel 201's spec-warden) | `selfhost/check/` or `selfhost/parse/` · spec § 9 · **class: adjacent**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a message that never names the rule.

    Repaired at `e5fa4939`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `resolve/type_argument.hero` tells a type word read as a name by the rule it broke, *a generic function's type arguments are never written at a call*, where the program's text around it is a list of types between a `<` after a name and its `>`, spaced as `heroes fmt` writes it or not; the code stays `unknown_name`. One case, `check/fixedbugs-545-…`, six shapes and the comparison `a < i64` beside them; `check` 641, `full` 28, `permissive` 16 and the compiler's 1,533 tests, 0 failed. Not reached: a record named as the type argument, `ident<Point>(p)`, which resolves and is told by the checker.

    Widened at `d3892d4a`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests: a record's or a variant's name standing as the type argument, `ident<Point>(p)`, the shape the line above left unreached, is told by the same rule, once; one more case, `check/fixedbugs-545-a-record-or-a-variant-…`, three shapes; `check` 644 and the compiler's 1,537 tests, 0 failed. The card keeps the first repair's commit.

## The repair

Repaired at `e5fa4939`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `resolve/type_argument.hero` tells a type word read as a name by the rule it broke, *a generic function's type arguments are never written at a call*, where the program's text around it is a list of types between a `<` after a name and its `>`, spaced as `heroes fmt` writes it or not; the code stays `unknown_name`. One case, `check/fixedbugs-545-…`, six shapes and the comparison `a < i64` beside them; `check` 641, `full` 28, `permissive` 16 and the compiler's 1,533 tests, 0 failed. Not reached: a record named as the type argument, `ident<Point>(p)`, which resolves and is told by the checker.

**Closed 2026-10-09** with batch 17 (lanes b17-fix, b17-emit and b17-check, landing panels 200 and 201 as ratified that evening, merged into one round tree made from the trunk), under the optimistic chain (`.claude/rules/verification.md` § The optimistic chain), its closing gate run on the round at `46b80b82`: the seed regenerated over two generations, the runtime's ABI moved to 30 by defect 470, 47,052,134 bytes, SHA-256 beginning `2b668ee89a3697cd`, its fixpoint by `cmp`, the committed seed of ABI 29 built against the runtime it was emitted for; the compiler's own tests 1,542, all passed; the net's own tests 326, all passed; the full net 7,306 passed over 29 suites, 0 failed, `run` in four shards of the harness's own (defect 537), 426 of 426 cases read, one skipped on this Mac (defect 437's case, `sys/prctl.h`); one floor told outgrown, `full`, raised in the closing commit as defect 536's rule asks. The census and panel 187's R2 run after the push, beside the CI.
