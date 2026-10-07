---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: c057f4584241d2e0574619bea702dff41aa47735
github: none
---

- [x] **424 — the fixed-array note at a function's result still says no binding, constant or parameter** | at a function-result position the `fixed_outside_a_group` note keeps 399's wording, *no binding, constant or parameter*, and does not name the result | `selfhost/ffi_errors.hero` · **class: improvement**

    **Origin:** lane b13-fixed405, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:12; the lane's reading, not re-run by the coordinator.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a message less exact than it could be at one position.

    Repaired at `c057f458`, 2026-10-07 (lane b13-fixed405): the note names a function's result among the places that hold no fixed array; case `full/fixedbugs-424-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; merged into round b13 by the coordinator, the card filled by the coordinator since the lane was told not to edit it.

## The repair

Repaired at `c057f458`, 2026-10-07 (lane b13-fixed405): the note names a function's result among the places that hold no fixed array; case `full/fixedbugs-424-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; merged into round b13 by the coordinator, the card filled by the coordinator since the lane was told not to edit it.

**Closed 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.
