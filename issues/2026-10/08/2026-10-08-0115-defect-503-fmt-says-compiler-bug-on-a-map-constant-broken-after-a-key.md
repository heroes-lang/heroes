---
kind: defect
area: print
milestone: none
filed: 2026-10-08
commit: none
github: none
---

- [ ] **503 — `fmt` says *compiler bug* on a map constant broken after a key** | `constant AGES: {str: i64}` with the body `{"ziggy":` on one line and `42, "mars": 7}` on the next: `check` exit 0; `fmt` exit 2, *`fmt` produced source that does not parse: …:3:1: error[indentation_jump]* and *this is a compiler bug — `b.hero` was NOT changed* (run by the coordinator before 01:15 on 2026-10-08 on the trunk's compiler at `56def9b4`, `<scratchpad>/batch15/fmt503/b.hero`); found by the formatter's probe by hand after batch 14's push, the `bracket` family over `tests/golden/ir/fixedbugs-394-a-literal-s-arguments-grow-in-place.hero`, 51 of 276 variants refused by the guard, the same 51 with batch 13's compiler | `selfhost/print/`, a map literal's break inside a constant's body · **class: blocking**

    **Origin:** filed by the coordinator at 01:15 on 2026-10-08 from the formatter's probe by hand owed by batch 14's push (`<scratchpad>/batch14/probe/`), reduced to four lines and run before filing.

    **Class: blocking**, 2026-10-08 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 on a correct program, told as a compiler bug.
