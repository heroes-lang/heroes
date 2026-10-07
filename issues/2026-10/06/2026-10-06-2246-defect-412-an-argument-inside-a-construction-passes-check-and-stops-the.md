---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: 50bcbcb297d39983f5f81010deea8c090b8543c8
github: none
---

- [x] **412 — an `@` argument inside a construction passes `check` and stops the build** | `record Point` of two `i64`, `n: i64 @ 1` and `p = Point(x: @n, y: 2)`: `check` 0, then `run` exit 2 with *internal error: compiling the generated C failed* and *clang refused the generated C*, the emission writing `.f_x = &h0_n` (the coordinator's re-run on round b13's compiler at `4f251f6e`, 22:46) | the checker's reading of a construction's arguments, `selfhost/check/` · **class: blocking**

    **Origin:** lane b13-zero401, 2026-10-06 (its report, *found beside it*), beside defect 401; reproduced by the coordinator. Measured by the lane, not re-run by the coordinator: `.circle(r: @n)` and `.ring(r: 2, rest: @zero)` also check at 0.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told, `check` accepting a program the build cannot make.

    Repaired at `50bcbcb2`, 2026-10-07 (lane b13-zero401): an `@` handed to a construction refused at the name stage (`resolve/built_marks.hero`), `marker_mismatch` quoting the argument as written, its guess fix dropping the mark; cases `check/fixedbugs-412-*` and `full/fixedbugs-412-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; merged into round b13 by the coordinator, the card filled by the coordinator since the lane was told not to edit it.

## The repair

Repaired at `50bcbcb2`, 2026-10-07 (lane b13-zero401): an `@` handed to a construction refused at the name stage (`resolve/built_marks.hero`), `marker_mismatch` quoting the argument as written, its guess fix dropping the mark; cases `check/fixedbugs-412-*` and `full/fixedbugs-412-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; merged into round b13 by the coordinator, the card filled by the coordinator since the lane was told not to edit it.

**Closed 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.
