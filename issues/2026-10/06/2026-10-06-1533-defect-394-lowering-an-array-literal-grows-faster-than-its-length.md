---
kind: defect
area: ir
milestone: none
filed: 2026-10-06
commit: 2c5f1a868b728e0e9443c08767513d076166a695
github: none
---

- [x] **394 — lowering an array literal grows faster than its length** | one `constant BIG: [i64]` of N elements: `--dump-ir` retires 2.6, 6.3 and 18.3 billion instructions at N of 5,000, 10,000 and 20,000 while `check` stays linear; building 70,000 elements retires about 190 billion, its per-read route 350,593 lines of C (lane b13-c382's measurement, 2026-10-06; not re-run by the coordinator) | `selfhost/ir/lower.hero` and the lowering of a container literal · panel 195 · **class: improvement**

    **Origin:** lane b13-c382, 2026-10-06 (its report, *found beside* 2); filed by the coordinator at 15:33.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost at a literal's size no program in the tree reaches, every output right; outside the batch under the author's instruction of 2026-10-05.

    Repaired at `2c5f1a86`, 2026-10-07 (lane b14-ir), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The step was `build.args_run`, which pushed every element, entry and field of a literal onto the function's argument table through a field of the builder, copying the table at each push; it is lent to `append_arg` now, and `--dump-ir` of one `constant BIG: [i64]` retires 1.93, 3.38 and 6.38 billion instructions at 5,000, 10,000 and 20,000 elements where the base retired 2.70, 6.49 and 18.77, its IR and its C byte-identical.

## The repair

Repaired at `2c5f1a86`, 2026-10-07 (lane b14-ir), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The step was `build.args_run`, which pushed every element, entry and field of a literal onto the function's argument table through a field of the builder, copying the table at each push; it is lent to `append_arg` now, and `--dump-ir` of one `constant BIG: [i64]` retires 1.93, 3.38 and 6.38 billion instructions at 5,000, 10,000 and 20,000 elements where the base retired 2.70, 6.49 and 18.77, its IR and its C byte-identical.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
