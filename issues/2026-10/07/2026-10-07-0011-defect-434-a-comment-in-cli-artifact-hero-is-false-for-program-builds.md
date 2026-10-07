---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: 8b151864380455e5b280467fff56b30e41ed6b2d
github: none
---

- [x] **434 — a comment in `cli/artifact.hero` is false for program builds** | *`emit_maybe` with the learned tags is exactly what `assemble.fused` compiles* holds for `--emit-c`, whose artifact is the fused text, and not for a program build, whose rounds compile the per-module units | `selfhost/cli/artifact.hero` · **class: improvement**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-run400's report of the evening before (*found beside*); the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a false comment, no program judged wrong.

    Repaired at `8b151864`, 2026-10-07 (lane b14-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The comment now says that `--emit-c` is a program build, whose rounds compile one unit per module, so no clang run of that build reads the artifact's bytes, and that only `heroes test` compiles the fused unit; measured by hand to write it, clang read the artifacts of the 372 `run` programs under the build's flags with no error and no warning.

## The repair

Repaired at `8b151864`, 2026-10-07 (lane b14-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The comment now says that `--emit-c` is a program build, whose rounds compile one unit per module, so no clang run of that build reads the artifact's bytes, and that only `heroes test` compiles the fused unit; measured by hand to write it, clang read the artifacts of the 372 `run` programs under the build's flags with no error and no warning.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
