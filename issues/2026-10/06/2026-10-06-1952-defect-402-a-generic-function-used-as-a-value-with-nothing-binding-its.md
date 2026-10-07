---
kind: defect
area: ir
milestone: none
filed: 2026-10-06
commit: 6a181ef0116be0f13406a8a336f6cdc726b053fb
github: none
---

- [x] **402 — a generic function used as a value with nothing binding its type stops the build** | `function ident<A>(x: A) -> A` and `_ = ident` in `main`: `check` 0, then `run` exit 2 with *internal error: monomorphisation produced an ill-formed program* (the trunk's compiler at `6a03c488`, run by the coordinator at 19:52 on 2026-10-06; lane b13-unit measured the same on Linux arm64 and x86-64) | a function value's type parameters left unbound, `selfhost/check/` and `selfhost/ir/mono.hero` · spec § 9 (function values, generics) · **class: blocking**

    **Origin:** lane b13-unit, 2026-10-06 (its report, *found beside*), beside defect 398; reproduced by the coordinator.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told; `check` accepts what the build cannot make. Into batch 13.

    Repaired at `6a181ef0`, 2026-10-06 (lane b13-gen402), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Spec § 9 decided it: a type parameter takes its type from the arguments, else from the type the context asks for, so a generic function used as a value is the instance its position's type names, copied by monomorphisation and named by the emitter, and where the position asks for none the name is `cannot_infer`, every unbound letter named.

## The repair

Repaired at `6a181ef0`, 2026-10-06 (lane b13-gen402), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Spec § 9 decided it: a type parameter takes its type from the arguments, else from the type the context asks for, so a generic function used as a value is the instance its position's type names, copied by monomorphisation and named by the emitter, and where the position asks for none the name is `cannot_infer`, every unbound letter named.

**Closed 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.
