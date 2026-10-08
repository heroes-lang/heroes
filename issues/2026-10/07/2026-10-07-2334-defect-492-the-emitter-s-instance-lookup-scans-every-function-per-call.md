---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: 170db6ba3a83f345022ec9ecb29bbd99c026b10f
github: none
---

- [ ] **492 — the emitter's instance lookup scans every function per call** | after defect 418 made it one lookup, it still walks every function for each call (lane b14-emit, unmeasured) | `selfhost/ir/instances.hero` · defect 418 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-emit's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with functions times calls, unmeasured.

    Repaired at `170db6ba`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `instances.Held` holds the program with its functions by declaration, built once per program, and every lookup of a callee in the emitter reads it where it scanned the whole program, first match and last as before: N functions each called once, 500, 1,000 and 2,000, read 8.484, 16.581 and 40.540 billion instructions against 7.183, 11.391 and 19.862, and the compiler's own `--emit-c` 689.013 against 528.883, the C byte-identical.
