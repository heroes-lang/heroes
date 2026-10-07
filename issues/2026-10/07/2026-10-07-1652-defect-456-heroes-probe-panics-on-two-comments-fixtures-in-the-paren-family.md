---
kind: defect
area: compiler
milestone: none
filed: 2026-10-07
commit: 428a5cfd480123649703197394c3dca9d94cc01e
github: none
---

- [ ] **456 — `heroes probe` panics on two comments fixtures in the `paren` family** | `heroes probe tests/golden/surface-fixtures/comments101/ascending.hero --family paren` and the same of `closeparen.hero` each stop with *panic: array index out of range*, exit 134; every other family passes on them (lane b14-harness-b's reading on `dad2da47`'s compiler; the two runs reproduced by the coordinator before 16:52 on the trunk's compiler at `00e2eda6`, `<scratchpad>/batch14/probe456/`) | `selfhost/probe/`, the `paren` family's mutation of a line that holds a comment · **class: blocking**

    **Origin:** filed by the coordinator at 16:52 on 2026-10-07, from lane b14-harness-b's final report (*found beside*, the lane's reading *blocking*); reproduced before filing.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a crash, exit 134, in one of the compiler's own commands, which the formatter's probe by hand before a push that touches `selfhost/print/` runs (`.claude/rules/verification.md` § The formatter's probe).

    **Corrected 2026-10-07** (lane b14-text, measured on the base compiler at `dad2da47`): the panic is the parser's, and `heroes check` reaches it itself, not only `probe`'s variants: exit 134 on a program of three lines with no comment, `function f(a: (` over `        bool), b: bool)` over a body, in `parse/brace_habit.a_closer_too_many`, which read one token past the last of a body's view; the `paren` family writes that shape into `ascending.hero` and `closeparen.hero`, and every other family over the 40 comments101 fixtures exits 0.

    Repaired at `428a5cfd`, 2026-10-07 (lane b14-text), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The step past a run of stray closers takes the run's own closers only, a body's view laying its `dedent` and `eof` at its closing token's place, and `stray_run` asks for a token before reading it: five cases under `tests/golden/check/fixedbugs-456-*`, four red on the base at exit 134; the probe over comments101 under all seven families, 2 failures to 0.
