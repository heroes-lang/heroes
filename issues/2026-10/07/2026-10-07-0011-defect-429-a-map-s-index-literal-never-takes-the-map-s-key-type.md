---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: 0bade8bc04dbf9cc38437fbe96d976e5102e2fb3
github: none
---

- [x] **429 — a map's index literal never takes the map's key type** | `names: {u8: str} = {11: "eleven"}`, then `names[11]`: `type_mismatch`, *expected `u8`, found `i64`* (the coordinator, 00:11); spec § 2 says a literal takes the type its context asks for; lane b13-gen402 reads it reaching `m[k] @ v` the same way, unrun | `access.index_type`, which synthesises the index, `selfhost/check/` · **class: blocking**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-gen402's report of the evening before (*found beside*); reproduced by the coordinator on round b13's compiler.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a correct program refused, § 2's rule not applied to an index.

    Repaired at `0bade8bc`, 2026-10-07 (lane b13-gen402): a map's index with no type of its own (a number literal, a `.case`, an empty literal) is checked against the map's key type, for a read and a store (`check/access.hero`'s `key_asked`, applied by `walk.hero`'s `index_of`); cases `run/` and `check/fixedbugs-429-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; the card filled by the coordinator.

## The repair

Repaired at `0bade8bc`, 2026-10-07 (lane b13-gen402): a map's index with no type of its own (a number literal, a `.case`, an empty literal) is checked against the map's key type, for a read and a store (`check/access.hero`'s `key_asked`, applied by `walk.hero`'s `index_of`); cases `run/` and `check/fixedbugs-429-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; the card filled by the coordinator.

**Closed 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.
