---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: 2cd795f662a37bd12540653f9d65927f6719d041
github: none
---

- [x] **399 — a fixed array outside a group is told to use `[T]`, which written as said gives five more errors** | panel 194's blind readers of task 1 under two variants wrote a record's `u8[256]` fields where no group holds them; the `fixed_outside_a_group` note says *use `[T]` here*, and the program written as it says gives five `type_mismatch` (the completeness critic's second pass, compiling the readers' programs, `docs/panel/194-reports/blind/b194-t1-L.md` and `-t1-C.md`; not re-run by the coordinator) | the `fixed_outside_a_group` note, `selfhost/ffi_errors.hero` and `selfhost/check/ffi.hero` · design.md §4.17 · **class: adjacent**

    **Origin:** panel 194's completeness critic, its second pass, 2026-10-06.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a true refusal whose note points to a dead end, a message less exact than it could be. Into batch 13.

    Repaired at `2cd795f6`, 2026-10-06 (lane b13-front), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `2cd795f6`, 2026-10-06 (lane b13-front), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

**Closed 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.
