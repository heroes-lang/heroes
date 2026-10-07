---
kind: defect
area: compiler
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **456 — `heroes probe` panics on two comments fixtures in the `paren` family** | `heroes probe tests/golden/surface-fixtures/comments101/ascending.hero --family paren` and the same of `closeparen.hero` each stop with *panic: array index out of range*, exit 134; every other family passes on them (lane b14-harness-b's reading on `dad2da47`'s compiler; the two runs reproduced by the coordinator before 16:52 on the trunk's compiler at `00e2eda6`, `<scratchpad>/batch14/probe456/`) | `selfhost/probe/`, the `paren` family's mutation of a line that holds a comment · **class: blocking**

    **Origin:** filed by the coordinator at 16:52 on 2026-10-07, from lane b14-harness-b's final report (*found beside*, the lane's reading *blocking*); reproduced before filing.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a crash, exit 134, in one of the compiler's own commands, which the formatter's probe by hand before a push that touches `selfhost/print/` runs (`.claude/rules/verification.md` § The formatter's probe).
