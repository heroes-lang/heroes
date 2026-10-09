---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 1460cc0ab081700fdd148d4150d8fcd2a8e93f39
github: none
---

- [ ] **551 — `ok(1).h()` is told to pass its value where a `T?` is expected, which it did** | `ok(1).h()` with a non-generic `h(n: i64?)` is refused with *pass it where a `T?` is expected*: a UFCS receiver gets no type from its position, a cause apart from defect 544's (lane b17-check) | the receiver of a UFCS call in `selfhost/check/` · defect 544 · **class: blocking**

    **Origin:** filed by the coordinator at 22:35 on 2026-10-09 from lane b17-check's final report (its notes `.claude/worktrees/scratch-b15/b17-check/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a false message on a reachable shape.

    Repaired at `1460cc0a`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The message is made true, the receiver not given its parameter's type (that would change what `check` accepts, a sitting's question on spec § 9's UFCS sentence): `check/generic_argument.hero` tells *`ok(…)` takes its type from the type the context asks for, and the value written before `.h(…)` asks for none: its type is read first, to know what the call names*, its guess binding the program's own text first. One case, `check/fixedbugs-551-…`, four shapes red on the base; the 433 case's `[].first()` moved to the same message; `check` 647, `full` 30, `permissive` 16 and the compiler's 1,542 tests, 0 failed.
