---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: 50bcbcb297d39983f5f81010deea8c090b8543c8
github: none
---

- [ ] **412 — an `@` argument inside a construction passes `check` and stops the build** | `record Point` of two `i64`, `n: i64 @ 1` and `p = Point(x: @n, y: 2)`: `check` 0, then `run` exit 2 with *internal error: compiling the generated C failed* and *clang refused the generated C*, the emission writing `.f_x = &h0_n` (the coordinator's re-run on round b13's compiler at `4f251f6e`, 22:46) | the checker's reading of a construction's arguments, `selfhost/check/` · **class: blocking**

    **Origin:** lane b13-zero401, 2026-10-06 (its report, *found beside it*), beside defect 401; reproduced by the coordinator. Measured by the lane, not re-run by the coordinator: `.circle(r: @n)` and `.ring(r: 2, rest: @zero)` also check at 0.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told, `check` accepting a program the build cannot make.

    Repaired at `50bcbcb2`, 2026-10-07 (lane b13-zero401): an `@` handed to a construction refused at the name stage (`resolve/built_marks.hero`), `marker_mismatch` quoting the argument as written, its guess fix dropping the mark; cases `check/fixedbugs-412-*` and `full/fixedbugs-412-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; merged into round b13 by the coordinator, the card filled by the coordinator since the lane was told not to edit it.
