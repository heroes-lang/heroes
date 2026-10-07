---
kind: defect
area: emit
milestone: none
filed: 2026-10-06
commit: 6c87c041cf26ed32e4ddc795ae9d73e6c13570f2
github: none
---

- [x] **418 — two emitter modules keep their own copy of the instance lookup for calls** | `emit/inst.hero` and `emit/members.hero` each keep the rule that finds a generic call's instance, which `func_ref.instance_at` now names; one copy could serve both | `selfhost/emit/inst.hero`, `selfhost/emit/members.hero`, `selfhost/emit/func_ref.hero` · **class: improvement**

    **Origin:** lane b13-gen402, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cleaner form, no program judged wrong.

    Repaired at `6c87c041`, 2026-10-07 (lane b14-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The type arguments an instruction names and the function the program holds at them are each one function of `selfhost/ir/instances.hero`, `named_at` and `index_of`, where three emitter modules kept three copies of the first and three loops of the second (one keeping the last match, two the first); `emission` read 971 passed and 0 failed, every emission byte-identical, and the compiler's own tests 1,358, all passed.

## The repair

Repaired at `6c87c041`, 2026-10-07 (lane b14-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The type arguments an instruction names and the function the program holds at them are each one function of `selfhost/ir/instances.hero`, `named_at` and `index_of`, where three emitter modules kept three copies of the first and three loops of the second (one keeping the last match, two the first); `emission` read 971 passed and 0 failed, every emission byte-identical, and the compiler's own tests 1,358, all passed.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
