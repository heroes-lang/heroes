---
kind: defect
area: ir
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **394 — lowering an array literal grows faster than its length** | one `constant BIG: [i64]` of N elements: `--dump-ir` retires 2.6, 6.3 and 18.3 billion instructions at N of 5,000, 10,000 and 20,000 while `check` stays linear; building 70,000 elements retires about 190 billion, its per-read route 350,593 lines of C (lane b13-c382's measurement, 2026-10-06; not re-run by the coordinator) | `selfhost/ir/lower.hero` and the lowering of a container literal · panel 195 · **class: improvement**

    **Origin:** lane b13-c382, 2026-10-06 (its report, *found beside* 2); filed by the coordinator at 15:33.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost at a literal's size no program in the tree reaches, every output right; outside the batch under the author's instruction of 2026-10-05.
