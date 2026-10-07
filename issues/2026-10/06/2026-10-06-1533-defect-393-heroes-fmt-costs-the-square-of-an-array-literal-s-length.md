---
kind: defect
area: print
milestone: none
filed: 2026-10-06
commit: 5438d3d60863b71961214f9acdf5aaa1c55dcbc9
github: none
---

- [x] **393 — `heroes fmt` costs the square of an array literal's length** | one `constant BIG: [i64]` of N elements: `fmt` retires 10.4, 37.3 and 141.0 billion instructions at N of 5,000, 10,000 and 20,000, about four times for each doubling, and 1.65 trillion at 70,000; `heroes probe --family multi` 141 billion at 5,000 and 515 billion at 10,000 (lane b13-c382's measurement, 2026-10-06, `<scratchpad>/batch13/c382/scale/`; not re-run by the coordinator) | `selfhost/print/fmt.hero` and the probe's reader, a literal's elements laid out · panel 195 R3, whose 70,000-element case waits on this · **class: adjacent**

    **Origin:** lane b13-c382, 2026-10-06 (its report, *found beside* 1), landing panel 195; filed by the coordinator at 15:33.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost, the product of a literal's length with itself, every output right; it keeps panel 195's 70,000-element golden out of the tree, since `canonical` would format it and `probe` nine times. Into batch 13 under the author's instruction of 2026-10-05.

    Repaired at `5438d3d6`, 2026-10-06 (lane b13-front), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `5438d3d6`, 2026-10-06 (lane b13-front), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

**Closed 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.
