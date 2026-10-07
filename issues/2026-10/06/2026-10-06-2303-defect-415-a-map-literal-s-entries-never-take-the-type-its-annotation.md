---
kind: defect
area: check
milestone: none
filed: 2026-10-06
commit: 23d00fbe0cc3777821c7850142cf805d48ce73b2
github: none
---

- [x] **415 — a map literal's entries never take the type its annotation asks for** | `m: {str: u8} = {"a": 1}` is refused `type_mismatch`, *expected `{str: u8}`, found `{str: i64}`*, where `xs: [u8] = [1, 2]` checks clean; spec § 2 says a literal takes the type its context asks for (the coordinator's re-run on round b13's compiler at `5fc393e3`, 23:03); measured by the lane, not re-run: `{"a": .plus}` is `cannot_infer`, and a generic function value inside a map literal is refused for the same reason | the checker's typing of a map literal against an expected type, `selfhost/check/` · **class: blocking**

    **Origin:** lane b13-gen402, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; reproduced by the coordinator.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a correct program refused, the spec's own rule (§ 2, line 43) not applied to a map's entries.

    Repaired at `23d00fbe`, 2026-10-07 (lane b13-gen402): a map literal's keys and values checked against the key and value types its context asks for, as an array literal's elements are; cases `run/` and `check/fixedbugs-415-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; merged into round b13 by the coordinator, the card filled by the coordinator since the lane was told not to edit it.

## The repair

Repaired at `23d00fbe`, 2026-10-07 (lane b13-gen402): a map literal's keys and values checked against the key and value types its context asks for, as an array literal's elements are; cases `run/` and `check/fixedbugs-415-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; merged into round b13 by the coordinator, the card filled by the coordinator since the lane was told not to edit it.

**Closed 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.
