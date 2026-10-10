---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 1460cc0ab081700fdd148d4150d8fcd2a8e93f39
github: none
---

- [x] **551 — `ok(1).h()` is told to pass its value where a `T?` is expected, which it did** | `ok(1).h()` with a non-generic `h(n: i64?)` is refused with *pass it where a `T?` is expected*: a UFCS receiver gets no type from its position, a cause apart from defect 544's (lane b17-check) | the receiver of a UFCS call in `selfhost/check/` · defect 544 · **class: blocking**

    **Origin:** filed by the coordinator at 22:35 on 2026-10-09 from lane b17-check's final report (its notes `.claude/worktrees/scratch-b15/b17-check/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a false message on a reachable shape.

    Repaired at `1460cc0a`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The message is made true, the receiver not given its parameter's type (that would change what `check` accepts, a sitting's question on spec § 9's UFCS sentence): `check/generic_argument.hero` tells *`ok(…)` takes its type from the type the context asks for, and the value written before `.h(…)` asks for none: its type is read first, to know what the call names*, its guess binding the program's own text first. One case, `check/fixedbugs-551-…`, four shapes red on the base; the 433 case's `[].first()` moved to the same message; `check` 647, `full` 30, `permissive` 16 and the compiler's 1,542 tests, 0 failed.

## The repair

Repaired at `1460cc0a`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The message is made true, the receiver not given its parameter's type (that would change what `check` accepts, a sitting's question on spec § 9's UFCS sentence): `check/generic_argument.hero` tells *`ok(…)` takes its type from the type the context asks for, and the value written before `.h(…)` asks for none: its type is read first, to know what the call names*, its guess binding the program's own text first. One case, `check/fixedbugs-551-…`, four shapes red on the base; the 433 case's `[].first()` moved to the same message; `check` 647, `full` 30, `permissive` 16 and the compiler's 1,542 tests, 0 failed.

**Closed 2026-10-09** with batch 17 (lanes b17-fix, b17-emit and b17-check, landing panels 200 and 201 as ratified that evening, merged into one round tree made from the trunk), under the optimistic chain (`.claude/rules/verification.md` § The optimistic chain), its closing gate run on the round at `46b80b82`: the seed regenerated over two generations, the runtime's ABI moved to 30 by defect 470, 47,052,134 bytes, SHA-256 beginning `2b668ee89a3697cd`, its fixpoint by `cmp`, the committed seed of ABI 29 built against the runtime it was emitted for; the compiler's own tests 1,542, all passed; the net's own tests 326, all passed; the full net 7,306 passed over 29 suites, 0 failed, `run` in four shards of the harness's own (defect 537), 426 of 426 cases read, one skipped on this Mac (defect 437's case, `sys/prctl.h`); one floor told outgrown, `full`, raised in the closing commit as defect 536's rule asks. The census and panel 187's R2 run after the push, beside the CI.
