---
kind: defect
area: print
milestone: none
filed: 2026-10-08
commit: 76b5fb0ae2eecf334f96cff0cf8bd2ba6d64d4f2
github: none
---

- [ ] **503 — `fmt` says *compiler bug* on a map constant broken after a key** | `constant AGES: {str: i64}` with the body `{"ziggy":` on one line and `42, "mars": 7}` on the next: `check` exit 0; `fmt` exit 2, *`fmt` produced source that does not parse: …:3:1: error[indentation_jump]* and *this is a compiler bug — `b.hero` was NOT changed* (run by the coordinator before 01:15 on 2026-10-08 on the trunk's compiler at `56def9b4`, `<scratchpad>/batch15/fmt503/b.hero`); found by the formatter's probe by hand after batch 14's push, the `bracket` family over `tests/golden/ir/fixedbugs-394-a-literal-s-arguments-grow-in-place.hero`, 51 of 276 variants refused by the guard, the same 51 with batch 13's compiler | `selfhost/print/`, a map literal's break inside a constant's body · **class: blocking**

    **Origin:** filed by the coordinator at 01:15 on 2026-10-08 from the formatter's probe by hand owed by batch 14's push (`<scratchpad>/batch14/probe/`), reduced to four lines and run before filing.

    **Class: blocking**, 2026-10-08 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 on a correct program, told as a compiler bug.

    Repaired at `76b5fb0ae2eecf334f96cff0cf8bd2ba6d64d4f2`, 2026-10-08, gated by its cases and the compiler's own tests; the net is owed at the batch's close. The repair is defect 504's, `25d458aa520756810270827e5a58759ee81545ad` in lane parse: `fmt` printed the map in the spread form of design.md §4.9, its `{` alone over one entry a line, and the lexer read that `{` as a body's brace, so `fmt` refused its own output; nothing in `selfhost/print/` moved, and this commit is the two compiler tests that pin it, red on the tree before 504 (1,434 tests, 2 failed) and green after (1,436 passed). `heroes probe` over `tests/golden/ir/fixedbugs-394-a-literal-s-arguments-grow-in-place.hero`, every family: 1,001 variants, 923 parsing, 87 refused by the guard on the base (51 `bracket`, 36 `paren`) and 0 with 504, on the 504-only compiler and on the lane's alike; the reproducer `fmt` prints spread at exit 0. Beside it, the same cause refused `fmt` on a map in a match arm's block or an `if` branch, past the width on one line, and alone as a statement; 31 shapes (`fmt` on each): 16 exit 2 and 3 exit 1 on the seed's compiler, all 31 exit 0 now.
