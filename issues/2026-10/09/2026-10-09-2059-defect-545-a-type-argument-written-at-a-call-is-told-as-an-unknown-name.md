---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: e5fa493904db40abeeacc7a74cfcf0cf9371598b
github: none
---

- [ ] **545 — a type argument written at a call is told as an unknown name** | `ident<i64>(3)` is told *nothing named `i64` is in scope*, never that a generic function's type arguments are never written (spec § 9) (panel 201's spec-warden) | `selfhost/check/` or `selfhost/parse/` · spec § 9 · **class: adjacent**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a message that never names the rule.

    Repaired at `e5fa4939`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `resolve/type_argument.hero` tells a type word read as a name by the rule it broke, *a generic function's type arguments are never written at a call*, where the program's text around it is a list of types between a `<` after a name and its `>`, spaced as `heroes fmt` writes it or not; the code stays `unknown_name`. One case, `check/fixedbugs-545-…`, six shapes and the comparison `a < i64` beside them; `check` 641, `full` 28, `permissive` 16 and the compiler's 1,533 tests, 0 failed. Not reached: a record named as the type argument, `ident<Point>(p)`, which resolves and is told by the checker.
