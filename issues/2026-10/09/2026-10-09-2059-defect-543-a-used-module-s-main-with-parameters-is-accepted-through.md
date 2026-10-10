---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: bc189f941d4df59daa84148a3ddb3744cbf6b500
github: none
---

- [x] **543 — a used module's `main` with parameters is accepted through `use` and refused checked alone** | `main(n: i64) -> i64` in a module another file uses passes `check` of the program, and `check` of that file alone refuses it `main_parameters` and `main_returns`: one file, two verdicts (`selfhost/check/decls.hero:89` applies the rules to the file compiled only; panel 201's compiler-engineer) | `selfhost/check/decls.hero` · panel 201 R2 · **class: adjacent**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): one file told two different things.

    Repaired at `bc189f94`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `check/decls.hero` holds every module's `main` the program writes to `main_parameters` and `main_returns`, never the root's alone, as spec § 1 says of `main`; a widened refusal that `git grep` finds refusing nothing tracked (three mains with parameters, every one a root its case already refuses). Its case is a test of the compiler's own over two files, the reproducer `check` 0 on the base and 1 now; `check` 641, `corpus` 55 and the compiler's 1,534 tests, 0 failed.

## The repair

Repaired at `bc189f94`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `check/decls.hero` holds every module's `main` the program writes to `main_parameters` and `main_returns`, never the root's alone, as spec § 1 says of `main`; a widened refusal that `git grep` finds refusing nothing tracked (three mains with parameters, every one a root its case already refuses). Its case is a test of the compiler's own over two files, the reproducer `check` 0 on the base and 1 now; `check` 641, `corpus` 55 and the compiler's 1,534 tests, 0 failed.

**Closed 2026-10-09** with batch 17 (lanes b17-fix, b17-emit and b17-check, landing panels 200 and 201 as ratified that evening, merged into one round tree made from the trunk), under the optimistic chain (`.claude/rules/verification.md` § The optimistic chain), its closing gate run on the round at `46b80b82`: the seed regenerated over two generations, the runtime's ABI moved to 30 by defect 470, 47,052,134 bytes, SHA-256 beginning `2b668ee89a3697cd`, its fixpoint by `cmp`, the committed seed of ABI 29 built against the runtime it was emitted for; the compiler's own tests 1,542, all passed; the net's own tests 326, all passed; the full net 7,306 passed over 29 suites, 0 failed, `run` in four shards of the harness's own (defect 537), 426 of 426 cases read, one skipped on this Mac (defect 437's case, `sys/prctl.h`); one floor told outgrown, `full`, raised in the closing commit as defect 536's rule asks. The census and panel 187's R2 run after the push, beside the CI.
