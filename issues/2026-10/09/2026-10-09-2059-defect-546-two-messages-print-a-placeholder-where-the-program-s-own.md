---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: c8b94d8589653055285d2cbefcc7fc43acd11f6d
github: none
---

- [ ] **546 — two messages print a placeholder where the program's own text belongs** | `needs_parameter_names` on `(function(A, A) -> ())` prints *a type parameter* for `A`, and `first(a: ident, b: double)` prints *expected `?`, found ...* (panel 201's compiler-engineer and critic) | `selfhost/check/` · panel 201 · **class: adjacent**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `c8b94d85`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `check/type_labels.hero` renders a function type with the letters of the declaration that holds it, so `(function(A, A) -> ())` is told with `A`; `check/generics.hero` lets a letter an earlier argument left poisoned take the next argument's type in silence, so `first(a: ident, b: double)` is told `ident`'s message alone, never *expected `?`*. One case, `check/fixedbugs-546-…`, four shapes, six messages on the base and four now; `check` 640, `full` 28, `permissive` 16 and the compiler's 1,532 tests, 0 failed.
