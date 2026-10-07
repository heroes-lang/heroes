---
kind: defect
area: check
milestone: none
filed: 2026-10-02
commit: e45fc34bdee7a3ad5b1913430cd4d3ea81bd50e5
github: none
---

- [x] **188 — two texts still state the arm rule panel 185's R3 replaced** | `selfhost/check/lending.hero:108-109` quotes the spec as *An arm that does nothing is a block holding `_ = 0`*, where the spec now reads *An arm that does nothing holds `_ = 0`*; `tests/golden/check/fixedbugs-135-a-discard-that-is-the-line-s-one-reading.hero:8` reasons *`_ = ` on the arm's own line is `declaration_in_arm`*, false since R3 | the two lines · **class: improvement**

    **Origin:** lane arm's report, 2026-10-02; read by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/stale-arm-texts/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). design.md §4.7 owes nothing: R3 brought the spec to it.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): no program moves.

    Repaired at `e45fc34b`, 2026-10-07 (lane b14-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `lending.hero` quotes spec § 8 as it stands, *An arm that does nothing holds `_ = 0` — `continue` is not one*, and the golden carries a dated correction below its program, in its `.hero` and `.fixed`, so no annotated line moved; the same premise, `_ = ` refused on an arm's line, was corrected beside it in `discard_errors.hero` and `statement_front.hero`, comments only, after the base compiler checked `0 => _ = compute(0)` and `1 => n @ compute(1)` at exit 0.

## The repair

Repaired at `e45fc34b`, 2026-10-07 (lane b14-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `lending.hero` quotes spec § 8 as it stands, *An arm that does nothing holds `_ = 0` — `continue` is not one*, and the golden carries a dated correction below its program, in its `.hero` and `.fixed`, so no annotated line moved; the same premise, `_ = ` refused on an arm's line, was corrected beside it in `discard_errors.hero` and `statement_front.hero`, comments only, after the base compiler checked `0 => _ = compute(0)` and `1 => n @ compute(1)` at exit 0.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
