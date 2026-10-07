---
kind: defect
area: cli
milestone: none
filed: 2026-10-06
commit: f0404dac3c88eea570712ad45ded866cf7876ea5
github: none
---

- [x] **390 — `run --sanitize` builds at `-O2`, where AddressSanitizer misses a stack overwrite that `-O0` catches** | a 4096-byte overwrite of the stack through `memset` in a header's `static inline`: `heroes run --sanitize` exits 0, `heroes build --sanitize` at `-O0` aborts with exit 134 (lane b12-ffi13's measurement, 2026-10-06, while gathering defect 092's evidence; not re-run by the coordinator); plain C at `-O2` misses it too, so it is the toolchain's, and the `run` suite's sanitizer leg can be blind to this shape | `selfhost/cli/` (the optimisation `--sanitize` builds at) · `tests/harness/suite_run.hero` (its sanitizer leg) · defect 092 · **class: improvement**

    **Origin:** lane b12-ffi13, 2026-10-06 (its report on defect 094, *Found beside* 2); filed by the coordinator at 13:44.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach, no program judged wrong by the compiler; the lane recommends the sanitizer leg run at `-O0` as well. Outside the batch under the author's instruction of 2026-10-05.

    Repaired at `f0404dac`, 2026-10-07 (lane b14-cli), gated by its cases and the net's own tests; the net is owed at the batch's close. Each `run` case is run again with `run --sanitize -O0`, `-O0` and `--sanitize` composing with no new flag; its control, `memset` eight bytes past a record's last field from a header's `static inline`, passes plain `-O0`, `-O2` and `--sanitize` at exit 0 and fails the new leg alone on *stack-buffer-overflow*, its exact twin passing every leg; `run` whole 372 passed on this Mac, the two legs +34.6% instructions on a sample of 93 cases.

## The repair

Repaired at `f0404dac`, 2026-10-07 (lane b14-cli), gated by its cases and the net's own tests; the net is owed at the batch's close. Each `run` case is run again with `run --sanitize -O0`, `-O0` and `--sanitize` composing with no new flag; its control, `memset` eight bytes past a record's last field from a header's `static inline`, passes plain `-O0`, `-O2` and `--sanitize` at exit 0 and fails the new leg alone on *stack-buffer-overflow*, its exact twin passing every leg; `run` whole 372 passed on this Mac, the two legs +34.6% instructions on a sample of 93 cases.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
