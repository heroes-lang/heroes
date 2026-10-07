---
kind: defect
area: resolve
milestone: none
filed: 2026-10-06
commit: 33d48dd81c15a903e967a1a4b85d24048a4071bc
github: none
---

- [x] **422 — a local named `args` is told three times for one mistake** | `args: i64 @ 1` then two writes `args @ 2`, `args @ 3`: `check --brief` tells `builtin_name_taken` at the binding and `no_mutable_globals` at each later write, three messages for the one name (the coordinator's re-run on round b13's source at `3bc3b2a5` with lane fixed405 merged, 23:12) | `selfhost/resolve/`, the reading of a binding whose name a built-in holds · **class: adjacent**

    **Origin:** lane b13-fixed405, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:12; reproduced by the coordinator.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): one mistake told as three, each later write read as the built-in's.

    Repaired at `33d48dd8`, 2026-10-07 (lane b13-zero401): a local named for a built-in or a top-level declaration is told once at its binding and then bound, its later writes, `@` arguments and field writes the binding's (`resolve/state.hero`), the unused sweep skipping such a name (`resolve/unused_sweep.hero`), and `args @ 2` with no binding told *built-in* (`resolve/writes.hero`); cases `check/fixedbugs-422-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; the card filled by the coordinator.

## The repair

Repaired at `33d48dd8`, 2026-10-07 (lane b13-zero401): a local named for a built-in or a top-level declaration is told once at its binding and then bound, its later writes, `@` arguments and field writes the binding's (`resolve/state.hero`), the unused sweep skipping such a name (`resolve/unused_sweep.hero`), and `args @ 2` with no binding told *built-in* (`resolve/writes.hero`); cases `check/fixedbugs-422-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; the card filled by the coordinator.

**Closed 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.
