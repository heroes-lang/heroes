---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 0411ec8feafe2543ecdc319e8d8d3439d31ea8b7
github: none
---

- [ ] **547 — two endless shapes beside panel 201's cycle rule pass `check`** | a function calling itself or another that calls it back, found by following first calls (`selfloop_first`), and a self-call through a parameter (`go(n: n, f: step)` inside `step`) pass the rule panel 201 R3 lands; each aborts at run time (panel 201's compiler-engineer) | `selfhost/check/`, the cycle rule panel 201 R3 lands · defect 520 · **class: improvement**

    **Origin:** filed by the coordinator at 20:59 on 2026-10-09 from panel 201 (`docs/panel/201-the-spec-says-a-generic-function-s-parameter-types-no-value-and-a-cycle-that-cannot-end-is-refused.md`, its reports beside it); the seats' and the critic's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a refusal the rule does not yet reach, the run still aborting.

    Narrowed 2026-10-09 (lane b17-check): its first shape, a function calling itself or another that calls it back (`selfloop_first`), is refused with a true headline by defect 520's landing (`912b2df3`); its second, a self-call through a parameter, still passes and is the item.

    Repaired at `0411ec8f`, 2026-10-10 (lane b18-infer, panel 203 R2, the author's *close 547 on the measurement*), gated by its cases; the net is owed at the batch's close. No change to the compiler: the run-time abort is the answer, panel 199 R2's at every level. Three `run` witnesses pin it, a self-call through a parameter, through a record's field and through `map`, each passing `check` and aborting `stack exhausted` (`stack-overflow` under the sanitiser) at every level the form runs; `run` filtered 3 passed, 0 failed, and their emissions blessed. Which frame the abort names varies between runs at one level (`paramvalue.go` or `paramvalue.step`, measured on this lane's probes), so the witnesses pin the abort and not the frame.
