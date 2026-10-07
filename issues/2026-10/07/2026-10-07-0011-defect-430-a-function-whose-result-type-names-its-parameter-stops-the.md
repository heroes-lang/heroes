---
kind: defect
area: ir
milestone: none
filed: 2026-10-07
commit: b594a130dd5e83a003b0b3e95531ca869a4cfe65
github: none
---

- [x] **430 — a function whose result type names its parameter stops the build** | `function pick() -> (function(x: i64) -> i64)` returning `inc`: `check` 0, then `run` exit 2, *internal error: the lowered program is not well formed* and *the verifier refused* (the coordinator, 00:11); the unlabelled `(function(i64) -> i64)` runs, and so does a labelled binding type (the lane's) | the lowering of a function value returned against a labelled function type, `selfhost/ir/` · **class: blocking**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-zero401's report of the evening before (*found beside*); reproduced by the coordinator on round b13's compiler.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told, `check` accepting what the build cannot make.

    Repaired at `b594a130`, 2026-10-07 (lane b13-gen402): the verifier's two return checks compare function types that differ only in their parameters' names as one (`ir/agree.hero`, used by `ir/verify.hero` and `ir/one_exit.hero`), the checker untouched; cases `run/` and `check/fixedbugs-430-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; the card filled by the coordinator.

## The repair

Repaired at `b594a130`, 2026-10-07 (lane b13-gen402): the verifier's two return checks compare function types that differ only in their parameters' names as one (`ir/agree.hero`, used by `ir/verify.hero` and `ir/one_exit.hero`), the checker untouched; cases `run/` and `check/fixedbugs-430-*`; gated by its cases and the compiler's own tests, the net owed at the batch's close; the card filled by the coordinator.

**Closed 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.
