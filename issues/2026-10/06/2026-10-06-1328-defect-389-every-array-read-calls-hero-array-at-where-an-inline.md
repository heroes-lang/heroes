---
kind: defect
area: emit
milestone: none
filed: 2026-10-06
commit: 1f5d9b70158b08a2f2ea325592ab21ba8fe46e0f
github: none
---

- [ ] **389 — every array read calls `hero_array_at` where an inline index would do** | a loop reading a name bound to a 32-element array a million times, `local.hero`, retires 116.5 million instructions; the same loop with the read's call replaced by an inline index, 85.5 million (panel 195's completeness critic, its second pass, 2026-10-06, its `second/measure/`; not re-run by the coordinator) | `selfhost/emit/`, the read of an array element · `runtime/heroes_runtime.h`, `hero_array_at` · panel 195 R4 · **class: improvement**

    **Origin:** panel 195's completeness critic, 2026-10-06: 84 to 86% of the element route's gain over the floor came from this, which every array read could have; filed by the coordinator at 13:28 under that sitting's R4.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost no rule promises against, every value right; outside the batch under the author's instruction of 2026-10-05.

    Repaired at `1f5d9b70`, 2026-10-07 (lane b14-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. An array's element and length are read in place by emitted C (`emit/array_read.hero`), every failing read handed to `hero_array_at` or `hero_array_len` for its own abort and the descriptor's size checked against the element's C type; the card's loop, re-run here, reads 116.1 to 117.0 million instructions before and 93.1 to 93.7 million after at -O0, 38.1 to 38.7 and 24.1 to 24.8 million at -O2, a `for` of a million steps 130.6 to 131.3 and 90.5 to 91.2 million, and the compiler built from it does a warm build of itself in 876.0 billion where it took 911.1 (-3.8%), its fused C 2.8% larger; 181 emissions moved and no header word or ABI with them.
