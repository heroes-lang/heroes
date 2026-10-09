---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: 7aa1f95f1a4229af0e134807a341ef576893dd71
github: none
---

- [x] **491 — `hero_array_at_mut` and `hero_str_byte` are still runtime calls where defect 389 inlined reads** | 389's cost shape; `at_mut` sits nested inside place expressions, so the read's inline form would duplicate calls there (lane b14-emit) | `selfhost/emit/`, `runtime/heroes_runtime.h` · defect 389 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-emit's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost per write and per byte read, unmeasured.

    Repaired at `7aa1f95f`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A string's byte and the element a nested place descends through are read in place, every failing read handed to the runtime's own function for its words and exit code, and a base that holds a call keeps the runtime's call, so none is written twice: over 1,600,000 steps a byte read retires 18.8% fewer instructions at -O0 and 32.5% fewer at -O2, a nested place 9.0% and 5.4%, an `@` element 14.2% and 14.3%, and the compiler checks itself 4.5% faster from its own C built at -O2; 49 emissions moved, each only by the two forms.

## The repair

Repaired at `7aa1f95f`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A string's byte and the element a nested place descends through are read in place, every failing read handed to the runtime's own function for its words and exit code, and a base that holds a call keeps the runtime's call, so none is written twice: over 1,600,000 steps a byte read retires 18.8% fewer instructions at -O0 and 32.5% fewer at -O2, a nested place 9.0% and 5.4%, an `@` element 14.2% and 14.3%, and the compiler checks itself 4.5% faster from its own C built at -O2; 49 emissions moved, each only by the two forms.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
