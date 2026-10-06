---
kind: defect
area: print
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **393 — `heroes fmt` costs the square of an array literal's length** | one `constant BIG: [i64]` of N elements: `fmt` retires 10.4, 37.3 and 141.0 billion instructions at N of 5,000, 10,000 and 20,000, about four times for each doubling, and 1.65 trillion at 70,000; `heroes probe --family multi` 141 billion at 5,000 and 515 billion at 10,000 (lane b13-c382's measurement, 2026-10-06, `<scratchpad>/batch13/c382/scale/`; not re-run by the coordinator) | `selfhost/print/fmt.hero` and the probe's reader, a literal's elements laid out · panel 195 R3, whose 70,000-element case waits on this · **class: adjacent**

    **Origin:** lane b13-c382, 2026-10-06 (its report, *found beside* 1), landing panel 195; filed by the coordinator at 15:33.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost, the product of a literal's length with itself, every output right; it keeps panel 195's 70,000-element golden out of the tree, since `canonical` would format it and `probe` nine times. Into batch 13 under the author's instruction of 2026-10-05.
