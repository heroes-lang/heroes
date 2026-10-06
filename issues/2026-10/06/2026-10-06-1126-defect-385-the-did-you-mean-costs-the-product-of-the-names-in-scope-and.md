---
kind: defect
area: resolve
milestone: none
filed: 2026-10-06
commit: c58bee417bc007466a21707be6a09785adaf6caa
github: none
---

- [ ] **385 — the did-you-mean costs the product of the names in scope and the names it cannot find** | a function holding N locals and N unknown names, none near enough to offer: `check --brief` retires 3.74, 12.94 and 47.94 billion instructions at N of 200, 400 and 800, three and a half to three point seven times for each doubling (the round's compiler at `bead6e45`, measured by the coordinator at 11:25 on 2026-10-06); N functions each with a parameter of an unknown type, 0.92, 2.46 and 7.79 billion at 200, 400 and 800 (lane fit12's measurement, not re-run) | `selfhost/resolve/near_names.hero`, the candidates of a scope gathered again for each unknown name; `selfhost/resolve/types.hero`, `type_candidates` walking every declaration and copying its kind for each unknown type · defect 372 · **class: adjacent**

    **Origin:** lane b12-fit12, 2026-10-06 (its final reply's *Found beside*, finding 1), beside defect 372's second cause; reproduced and filed by the coordinator at 11:26 under the author's instruction of 2026-10-05, any defect found that is not an improvement goes into the batch.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost found beside the work, the product of two counts, no message wrong, as defect 372 was classed. Lane fit12's repair of 372 (`16af01c3`, its edit table read in a band) took the first shape from 8.87, 32.86 and 126.5 to 1.55, 5.38 and 20.15 billion on its own measurement, still a product.

    The reproducer, generated: `function main()`, then `locNNNN = NNNN` for N names, `total = loc0000 + ... ` over all of them, then `print(zqNNNNxw)` for N names no local is near, then `print(total)`.

    Repaired at `c58bee41`, 2026-10-06 (lane fit12), gated by its cases and the compiler's own tests; the net is owed at the batch's close. **The product was two, and the measurement says which is the floor.** A module's candidates were gathered again for each name nothing answers (the names sorted, the built-ins' table rebuilt, each declaration's kind copied), and that half is gone: they are gathered once per module and handed on unjoined, so the shapes whose candidates are a module's names grow with the program, 800 unknown types over as many functions 7.79 to 1.68 billion and 800 unknown names over as many functions 44.19 to 1.70. The other half is the comparisons, cheaper but still one per name and candidate where the two are near in length: the coordinator's scope 47.94 to 7.89 billion at 800, still growing 3.35 times per doubling, the comparisons nine tenths of it (a sample of 1,600 names at `c58bee41`: `nearest_in` 1,590 of 1,711 in the name stage, `within` 1,506). Bucketing by length changes no suggestion if each bucket's hits are merged back in order, and does not move that floor: every local there is within the band of the names it is compared with, and where none is, the walk now costs about 56 instructions a pair by the counts at 400 and 800, about 1% at 800. The locals in scope are still read for each name, 61 of the same 1,711, the same order as the comparisons.
