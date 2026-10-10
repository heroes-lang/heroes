---
kind: defect
area: parse
milestone: none
filed: 2026-10-09
commit: 640810c38f19911fdb71d3d8d7e89930095fd7ec
github: none
---

- [x] **548 — a call's argument list left open over a list never closed is told twice for one edit** | `x = g(1, [2, 3` over `print(x)` over `y = 3 )` gets the lexer's *`[` never closed* and `expected_args_close` at `print`, two messages for one `])` edit; it goes through the call-argument reader, not the binding path defect 459's repair carries its openers through (lane b17-fix) | the call-argument reader of `selfhost/parse/` · defect 459 · **class: adjacent**

    **Origin:** filed by the coordinator at 21:43 on 2026-10-09 from lane b17-fix's final report (its notes `.claude/worktrees/scratch-b15/b17-fix/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

    Repaired at `640810c3`, 2026-10-09 (lane b17-fix, batch 17), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Where a call's `(` is closed further down and its last argument left openers the lexer named never closed on the `(`'s line, the call's report at the line below names them, the `(` and the openers it stands in, with the closers the one edit writes (`])`, `})`, `]))`), and carries the never-closed ones, whose lexer reports are withdrawn (`selfhost/args_left_open.hero`, asked by `grammar_expr.call_args`); the call ends on the line above and the line below is read as its own statement, its stray `)` told. Cases `check/fixedbugs-548-*` (five shapes) and `full/fixedbugs-548-*`, both red on the base; `check` 639, `full` 29, `permissive` 16, the compiler's own tests 1,532, all passed.

## The repair

Repaired at `640810c3`, 2026-10-09 (lane b17-fix, batch 17), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Where a call's `(` is closed further down and its last argument left openers the lexer named never closed on the `(`'s line, the call's report at the line below names them, the `(` and the openers it stands in, with the closers the one edit writes (`])`, `})`, `]))`), and carries the never-closed ones, whose lexer reports are withdrawn (`selfhost/args_left_open.hero`, asked by `grammar_expr.call_args`); the call ends on the line above and the line below is read as its own statement, its stray `)` told. Cases `check/fixedbugs-548-*` (five shapes) and `full/fixedbugs-548-*`, both red on the base; `check` 639, `full` 29, `permissive` 16, the compiler's own tests 1,532, all passed.

**Closed 2026-10-09** with batch 17 (lanes b17-fix, b17-emit and b17-check, landing panels 200 and 201 as ratified that evening, merged into one round tree made from the trunk), under the optimistic chain (`.claude/rules/verification.md` § The optimistic chain), its closing gate run on the round at `46b80b82`: the seed regenerated over two generations, the runtime's ABI moved to 30 by defect 470, 47,052,134 bytes, SHA-256 beginning `2b668ee89a3697cd`, its fixpoint by `cmp`, the committed seed of ABI 29 built against the runtime it was emitted for; the compiler's own tests 1,542, all passed; the net's own tests 326, all passed; the full net 7,306 passed over 29 suites, 0 failed, `run` in four shards of the harness's own (defect 537), 426 of 426 cases read, one skipped on this Mac (defect 437's case, `sys/prctl.h`); one floor told outgrown, `full`, raised in the closing commit as defect 536's rule asks. The census and panel 187's R2 run after the push, beside the CI.
