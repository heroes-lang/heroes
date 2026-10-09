---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: 758f4ab288bd08de7df907c8f46db9f7bfe9a585
github: none
---

- [ ] **469 — clang's front end costs the options chain super-quadratically** | the options shape grows ×7.59 per doubling (exponent 2.92) while its C grows linearly; o5000 costs 27 times v5000; debug information is 1.0% of it (panel 197's critic, second pass, `o5000-E-unit.c`) | the emitted C of a chain through options · panel 197's R7 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from panel 197's R7.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost past the floor, no program wrong.

    Repaired at `758f4ab2`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. What grew is clang's `RecordType::hasConstFields`, 98.5% of a sample of the front end at 2,000 levels, which clang asks of every assignment of a struct at the square of the structs it holds by value, and every level of the chain had a descriptor whose copy was such an assignment; the copy is now `__builtin_memmove`, and the front end at 500, 1,000 and 2,000 levels reads 3.65, 19.24 and 134.92 billion instructions to 1.57, 2.94 and 5.70 (10,000 levels 27.85, `-fintegrated-cc1`, this Mac). The same cause in a function's own assignments, a chain whose every level's value is built in one function, is not reached: 3.39 and 18.75 billion at 250 and 500 levels after this repair, 1.47 and 3.40 with every assignment written as a byte copy by hand, a decision put to the coordinator.
