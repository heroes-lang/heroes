---
kind: defect
area: resolve
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **385 — the did-you-mean costs the product of the names in scope and the names it cannot find** | a function holding N locals and N unknown names, none near enough to offer: `check --brief` retires 3.74, 12.94 and 47.94 billion instructions at N of 200, 400 and 800, three and a half to three point seven times for each doubling (the round's compiler at `bead6e45`, measured by the coordinator from 11:23, its last count written at 11:23:37 on 2026-10-06); N functions each with a parameter of an unknown type, 0.92, 2.46 and 7.79 billion at 200, 400 and 800 (lane fit12's measurement, not re-run) | `selfhost/resolve/near_names.hero`, the candidates of a scope gathered again for each unknown name; `selfhost/resolve/types.hero`, `type_candidates` walking every declaration and copying its kind for each unknown type · defect 372 · **class: adjacent**

    **Origin:** lane b12-fit12, 2026-10-06 (its final reply's *Found beside*, finding 1), beside defect 372's second cause; reproduced by the coordinator at 11:23 and filed at 11:25 (the file's name says 1126, written before the clock was read) under the author's instruction of 2026-10-05, any defect found that is not an improvement goes into the batch.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost found beside the work, the product of two counts, no message wrong, as defect 372 was classed. Lane fit12's repair of 372 (`16af01c3`, its edit table read in a band) took the first shape from 8.87, 32.86 and 126.5 to 1.55, 5.38 and 20.15 billion on its own measurement, still a product.

    The reproducer, generated: `function main()`, then `locNNNN = NNNN` for N names, `total = loc0000 + ... ` over all of them, then `print(zqNNNNxw)` for N names no local is near, then `print(total)`.
