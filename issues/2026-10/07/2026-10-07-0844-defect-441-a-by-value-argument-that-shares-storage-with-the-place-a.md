---
kind: defect
area: ir
milestone: none
filed: 2026-10-07
commit: c1f87af5c071a6d6bd52d893c02b1a6812f450a8
github: none
---

- [x] **441 — a by-value argument that shares storage with the place a call writes reads the write** | a call writing a root through `@` while a by-value argument borrows the same one-reference array: the base printed `7 7` where copy in, copy out gives `1 7`, and one row ended in a heap use-after-free under `--sanitize` with a false message naming a null pointer (lane b13-land-addr's case `run/fixedbugs-413-*`'s by-value rows, red on the base) | ownership of an argument beside an `@` write, `selfhost/ir/held.hero` · **class: blocking**

    **Origin:** filed by the coordinator at 08:44 on 2026-10-07, from lane b13-land-addr's report of the night before, which found it while landing panel 196's R1 and repaired it in the same change; the lane's measurement.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a wrong value and a memory fault (design.md §1.12).

    Repaired at `c1f87af5`, 2026-10-07 (lane b13-land-addr), ownership rule 7 (`ir/held.hero`): when a call writes a root through `@`, a by-value argument borrowing that same array is retained before the call and released after it; gated by its cases and the compiler's own tests, the net owed at the batch's close; filed and its card filled by the coordinator.

## The repair

Repaired at `c1f87af5`, 2026-10-07 (lane b13-land-addr), ownership rule 7 (`ir/held.hero`): when a call writes a root through `@`, a by-value argument borrowing that same array is retained before the call and released after it; gated by its cases and the compiler's own tests, the net owed at the batch's close; filed and its card filled by the coordinator.

**Closed 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.
