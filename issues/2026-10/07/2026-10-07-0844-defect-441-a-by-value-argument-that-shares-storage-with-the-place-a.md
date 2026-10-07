---
kind: defect
area: ir
milestone: none
filed: 2026-10-07
commit: c1f87af5c071a6d6bd52d893c02b1a6812f450a8
github: none
---

- [ ] **441 — a by-value argument that shares storage with the place a call writes reads the write** | a call writing a root through `@` while a by-value argument borrows the same one-reference array: the base printed `7 7` where copy in, copy out gives `1 7`, and one row ended in a heap use-after-free under `--sanitize` with a false message naming a null pointer (lane b13-land-addr's case `run/fixedbugs-413-*`'s by-value rows, red on the base) | ownership of an argument beside an `@` write, `selfhost/ir/held.hero` · **class: blocking**

    **Origin:** filed by the coordinator at 08:44 on 2026-10-07, from lane b13-land-addr's report of the night before, which found it while landing panel 196's R1 and repaired it in the same change; the lane's measurement.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a wrong value and a memory fault (design.md §1.12).

    Repaired at `c1f87af5`, 2026-10-07 (lane b13-land-addr), ownership rule 7 (`ir/held.hero`): when a call writes a root through `@`, a by-value argument borrowing that same array is retained before the call and released after it; gated by its cases and the compiler's own tests, the net owed at the batch's close; filed and its card filled by the coordinator.
