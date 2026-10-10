---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 0411ec8feafe2543ecdc319e8d8d3439d31ea8b7
github: none
---

- [x] **547 — two endless shapes beside panel 201's cycle rule pass `check`** | a function calling itself or another that calls it back, found by following first calls (`selfloop_first`), and a self-call through a parameter (`go(n: n, f: step)` inside `step`) pass the rule panel 201 R3 lands; each aborts at run time (panel 201's compiler-engineer) | `selfhost/check/`, the cycle rule panel 201 R3 lands · defect 520 · **class: improvement**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a refusal the rule does not yet reach, the run still aborting.

    Narrowed 2026-10-09 (lane b17-check): its first shape, a function calling itself or another that calls it back (`selfloop_first`), is refused with a true headline by defect 520's landing (`912b2df3`); its second, a self-call through a parameter, still passes and is the item.

    Repaired at `0411ec8f`, 2026-10-10 (lane b18-infer, panel 203 R2, the author's *close 547 on the measurement*), gated by its cases; the net is owed at the batch's close. No change to the compiler: the run-time abort is the answer, panel 199 R2's at every level. Three `run` witnesses pin it, a self-call through a parameter, through a record's field and through `map`, each passing `check` and aborting `stack exhausted` (`stack-overflow` under the sanitiser) at every level the form runs; `run` filtered 3 passed, 0 failed, and their emissions blessed. Which frame the abort names varies between runs at one level (`paramvalue.go` or `paramvalue.step`, measured on this lane's probes), so the witnesses pin the abort and not the frame.

## The repair

Repaired at `0411ec8f`, 2026-10-10 (lane b18-infer, panel 203 R2, the author's *close 547 on the measurement*), gated by its cases; the net is owed at the batch's close. No change to the compiler: the run-time abort is the answer, panel 199 R2's at every level. Three `run` witnesses pin it, a self-call through a parameter, through a record's field and through `map`, each passing `check` and aborting `stack exhausted` (`stack-overflow` under the sanitiser) at every level the form runs; `run` filtered 3 passed, 0 failed, and their emissions blessed. Which frame the abort names varies between runs at one level (`paramvalue.go` or `paramvalue.step`, measured on this lane's probes), so the witnesses pin the abort and not the frame.

**Closed 2026-10-10** with batch 18 (lanes b18-close, b18-infer, b18-ffi and b18-guard, merged into the round `lane-round-b18` with the trunk), its closing gate run on the round at `f6528c53`: the seed regenerated over two generations, the runtime's ABI at 30, 50,640,450 bytes, SHA-256 beginning `3bfbddd0f618b118`, its fixpoint by `cmp`; the compiler's own tests 1,577, all passed; the net's own tests 332, all passed; the full net 7,872 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after, its one red `order` on a walk of defect 570's `cli/pragma_ask.hero` with no `# ORDER:` mark, the mark written on its function's doc line (no line moved, the fixpoint re-checked by `cmp`) and `order` 3 and 0 after; eight floors told outgrown and raised in the closing commit, `order`, `runtime`, `emit`, `unsupported` and `probe` 3, 8, 12, 237 and 27, all 0 failed, after it. Defect 558's case, the one emission this Mac skips, was blessed and read green on Linux arm64 at the same commit (1,169 and 0). Under the optimistic chain the census and panel 187's R2 run after the push beside the CI, and a CI leg red on a closed defect's case files a new `blocking` defect naming it.
