---
kind: defect
area: resolve
milestone: none
filed: 2026-10-06
commit: 03123b3141162fdaf42c8b262a5fe05657483d34
github: none
---

- [x] **401 — a binding named `zero` hides what the words `rest: zero` mean** | with `zero = 0` bound, `add(a: 1, rest: zero)` is told only `unused_binding` for `zero`, and `rest_not_a_construction` appears only once the binding is removed; `.circle(r: 2, rest: zero)` on a variant case is told only `unknown_name: zero`, never naming the words (lane b13-bs's measurement on its branch, which lands panel 194's R1; not re-run by the coordinator) | the resolver's reading of the stripped `zero`, `selfhost/resolve/` · panel 194 R1 · **class: adjacent**

    **Origin:** lane b13-bs, 2026-10-06 (its report, *found beside* 2); filed by the coordinator at 19:52.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only after another is fixed, and a message that does not name the words; R1's own surface, landing in this batch. The lane recommends that `rest: zero` be refused at the words whenever a binding named `zero` is visible, since a reader cannot tell the two meanings apart.

    Repaired at `03123b3141162fdaf42c8b262a5fe05657483d34`, 2026-10-06, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `03123b3141162fdaf42c8b262a5fe05657483d34`, 2026-10-06, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

**Closed 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.
