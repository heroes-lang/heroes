---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: 7aa1f95f1a4229af0e134807a341ef576893dd71
github: none
---

- [ ] **491 — `hero_array_at_mut` and `hero_str_byte` are still runtime calls where defect 389 inlined reads** | 389's cost shape; `at_mut` sits nested inside place expressions, so the read's inline form would duplicate calls there (lane b14-emit) | `selfhost/emit/`, `runtime/heroes_runtime.h` · defect 389 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-emit's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost per write and per byte read, unmeasured.

    Repaired at `7aa1f95f`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A string's byte and the element a nested place descends through are read in place, every failing read handed to the runtime's own function for its words and exit code, and a base that holds a call keeps the runtime's call, so none is written twice: over 1,600,000 steps a byte read retires 18.8% fewer instructions at -O0 and 32.5% fewer at -O2, a nested place 9.0% and 5.4%, an `@` element 14.2% and 14.3%, and the compiler checks itself 4.5% faster from its own C built at -O2; 49 emissions moved, each only by the two forms.
