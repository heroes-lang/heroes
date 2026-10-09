---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: a9e557db3172ce97d15348e6fb601448f3e3a0b4
github: none
---

- [x] **488 — a generic function value handed to a generic function's parameter is refused `cannot_infer`, where spec § 9 read plainly would admit it** | `app(f: ident, x: 20)` with `ident<A>` and `app<A>(f: (function(A) -> A), x: A)`, `ns.map(ident)` and `ns.fold(19, keep)` are refused `cannot_infer`; the message names the narrowing itself (*hand it to a non-generic function's parameter*), defect 402's design; spec § 9 says a type parameter takes its type *from the type the context asks for* (lane b14-emit; reproduced by the coordinator at 20:01 on the trunk and the round) | `selfhost/check/`, the inference of a function value · spec § 9 · defect 402 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-emit's final report; the lane's measurement, not re-run by the coordinator. Reproduced by the coordinator before filing.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a narrowing the message itself states, so a sitting's question whether the spec's sentence or the compiler is the rule.

    Repaired at `a9e557db`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 201's R1, ratified: spec § 9's generics bullet takes the spec-warden's N1f word for word, *and a generic function's parameter asks for none*, and the compiler is unchanged. Priced on one `--refresh` at 21:02: 9,831 to 9,847 real (+16, the seat's P1 of +13 to +20 a hit), 7,467 to 7,479 vendored; the sentence is true of `check` on the seat's 23 probe programs (P2 a hit); `spec` 23, `special` 10, `grammar` 9, `fixes` 916, `unseen` 3, `records` 28, each 0 failed.

## The repair

Repaired at `a9e557db`, 2026-10-09 (lane b17-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 201's R1, ratified: spec § 9's generics bullet takes the spec-warden's N1f word for word, *and a generic function's parameter asks for none*, and the compiler is unchanged. Priced on one `--refresh` at 21:02: 9,831 to 9,847 real (+16, the seat's P1 of +13 to +20 a hit), 7,467 to 7,479 vendored; the sentence is true of `check` on the seat's 23 probe programs (P2 a hit); `spec` 23, `special` 10, `grammar` 9, `fixes` 916, `unseen` 3, `records` 28, each 0 failed.

**Closed 2026-10-09** with batch 17 (lanes b17-fix, b17-emit and b17-check, landing panels 200 and 201 as ratified that evening, merged into one round tree made from the trunk), under the optimistic chain (`.claude/rules/verification.md` § The optimistic chain), its closing gate run on the round at `46b80b82`: the seed regenerated over two generations, the runtime's ABI moved to 30 by defect 470, 47,052,134 bytes, SHA-256 beginning `2b668ee89a3697cd`, its fixpoint by `cmp`, the committed seed of ABI 29 built against the runtime it was emitted for; the compiler's own tests 1,542, all passed; the net's own tests 326, all passed; the full net 7,306 passed over 29 suites, 0 failed, `run` in four shards of the harness's own (defect 537), 426 of 426 cases read, one skipped on this Mac (defect 437's case, `sys/prctl.h`); one floor told outgrown, `full`, raised in the closing commit as defect 536's rule asks. The census and panel 187's R2 run after the push, beside the CI.
