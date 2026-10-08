---
kind: defect
area: check
milestone: none
filed: 2026-10-07
commit: c960261e89f76b04b1f5c711ff72b00382c771d9
github: none
---

- [ ] **477 — a call with thousands of arguments costs the checker faster than linear** | many-params-3200: `checkwalk.named_call` 1,935 of 6,385 samples (lane b14-ir), `check/walk.hero`'s `user_call` 3,757 of 3,946 on 3,200 fields (lane b14-resolve); samples, not counts | `selfhost/check/walk.hero` · defect 260 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-resolve's and lane b14-ir's final reports; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost on extreme shapes.

    Repaired at `c960261e`, 2026-10-08 (lane b15-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Whether a parameter's type is shared was asked of each position against every other at four sites, the call by name in a copy of its own; it is `type_fit.shared` now, each type counted once, and each argument asks a flag or a set where it scanned the call's lists, what every label at fault in one call asks of its signature made once (`labels.Asked`). `check` of one call of 800 / 1,600 / 3,200 / 6,400 `i64` arguments reads 0.68 / 1.44 / 3.49 / 9.56 billion instructions where it read 0.80 / 1.89 / 5.29 / 16.8, the rest `table.intern_function`'s pushes, defect 409's shape; with every label shifted, at 200 / 400 / 800 / 1,600, 0.23 / 0.40 / 0.76 / 1.63 where it read 0.54 / 1.65 / 5.79 / 21.7; with every second label missing, at 800, 3.26 where it read 29.3; every output byte-identical.
