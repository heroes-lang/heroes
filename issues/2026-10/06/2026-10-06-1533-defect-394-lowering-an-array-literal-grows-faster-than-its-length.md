---
kind: defect
area: ir
milestone: none
filed: 2026-10-06
commit: 2c5f1a868b728e0e9443c08767513d076166a695
github: none
---

- [ ] **394 — lowering an array literal grows faster than its length** | one `constant BIG: [i64]` of N elements: `--dump-ir` retires 2.6, 6.3 and 18.3 billion instructions at N of 5,000, 10,000 and 20,000 while `check` stays linear; building 70,000 elements retires about 190 billion, its per-read route 350,593 lines of C (lane b13-c382's measurement, 2026-10-06; not re-run by the coordinator) | `selfhost/ir/lower.hero` and the lowering of a container literal · panel 195 · **class: improvement**

    **Origin:** lane b13-c382, 2026-10-06 (its report, *found beside* 2); filed by the coordinator at 15:33.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost at a literal's size no program in the tree reaches, every output right; outside the batch under the author's instruction of 2026-10-05.

    Repaired at `2c5f1a86`, 2026-10-07 (lane b14-ir), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The step was `build.args_run`, which pushed every element, entry and field of a literal onto the function's argument table through a field of the builder, copying the table at each push; it is lent to `append_arg` now, and `--dump-ir` of one `constant BIG: [i64]` retires 1.93, 3.38 and 6.38 billion instructions at 5,000, 10,000 and 20,000 elements where the base retired 2.70, 6.49 and 18.77, its IR and its C byte-identical.
