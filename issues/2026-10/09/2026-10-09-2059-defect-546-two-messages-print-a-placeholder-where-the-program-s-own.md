---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: c8b94d8589653055285d2cbefcc7fc43acd11f6d
github: none
---

- [x] **546 — two messages print a placeholder where the program's own text belongs** | `needs_parameter_names` on `(function(A, A) -> ())` prints *a type parameter* for `A`, and `first(a: ident, b: double)` prints *expected `?`, found ...* (panel 201's compiler-engineer and critic) | `selfhost/check/` · panel 201 · **class: adjacent**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `c8b94d85`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `check/type_labels.hero` renders a function type with the letters of the declaration that holds it, so `(function(A, A) -> ())` is told with `A`; `check/generics.hero` lets a letter an earlier argument left poisoned take the next argument's type in silence, so `first(a: ident, b: double)` is told `ident`'s message alone, never *expected `?`*. One case, `check/fixedbugs-546-…`, four shapes, six messages on the base and four now; `check` 640, `full` 28, `permissive` 16 and the compiler's 1,532 tests, 0 failed.

## The repair

Repaired at `c8b94d85`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `check/type_labels.hero` renders a function type with the letters of the declaration that holds it, so `(function(A, A) -> ())` is told with `A`; `check/generics.hero` lets a letter an earlier argument left poisoned take the next argument's type in silence, so `first(a: ident, b: double)` is told `ident`'s message alone, never *expected `?`*. One case, `check/fixedbugs-546-…`, four shapes, six messages on the base and four now; `check` 640, `full` 28, `permissive` 16 and the compiler's 1,532 tests, 0 failed.

**Closed 2026-10-09** with batch 17 (lanes b17-fix, b17-emit and b17-check, landing panels 200 and 201 as ratified that evening, merged into one round tree made from the trunk), under the optimistic chain (`.claude/rules/verification.md` § The optimistic chain), its closing gate run on the round at `46b80b82`: the seed regenerated over two generations, the runtime's ABI moved to 30 by defect 470, 47,052,134 bytes, SHA-256 beginning `2b668ee89a3697cd`, its fixpoint by `cmp`, the committed seed of ABI 29 built against the runtime it was emitted for; the compiler's own tests 1,542, all passed; the net's own tests 326, all passed; the full net 7,306 passed over 29 suites, 0 failed, `run` in four shards of the harness's own (defect 537), 426 of 426 cases read, one skipped on this Mac (defect 437's case, `sys/prctl.h`); one floor told outgrown, `full`, raised in the closing commit as defect 536's rule asks. The census and panel 187's R2 run after the push, beside the CI.
