---
kind: defect
area: cli
milestone: none
filed: 2026-10-06
commit: c41525aec367702a815ee4b69cc9ce7e97db2d26
github: none
---

- [x] **383 — `check --json` pays its SHA-256 digests in Heroes compiled without optimisation** | `check --json` on the compiler's own source retires 101.6 billion instructions against 86.4 billion before panel 193's R5 put a digest of every file read into the document, +17.5%, all of it the SHA-256 written in Heroes (lane cli12's measurement, its final report; not re-run by the coordinator) | `selfhost/cli/sha256.hero` · panel 193's R5 · **class: improvement**

    **Origin:** lane b12-cli12, 2026-10-06 (its final report); filed by the coordinator at 09:49.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost no rule promises against, the digest right; outside the batch under the author's instruction of 2026-10-05, the improvements stay out.

    Repaired at `c41525ae`, 2026-10-07 (lane b14-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Whole blocks are read where they lie, one schedule array is reused and rotations are written out: 2,729 to 2,737 instructions a byte before, 1,686 to 1,695 after, at 64 KiB, 512 KiB and 4 MiB, and `check --json` over `check` on the compiler's own source +24.2% before, +15.4% after, the documents byte-identical and every digest `shasum -a 256`'s on 1,226 files.

## The repair

Repaired at `c41525ae`, 2026-10-07 (lane b14-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Whole blocks are read where they lie, one schedule array is reused and rotations are written out: 2,729 to 2,737 instructions a byte before, 1,686 to 1,695 after, at 64 KiB, 512 KiB and 4 MiB, and `check --json` over `check` on the compiler's own source +24.2% before, +15.4% after, the documents byte-identical and every digest `shasum -a 256`'s on 1,226 files.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
