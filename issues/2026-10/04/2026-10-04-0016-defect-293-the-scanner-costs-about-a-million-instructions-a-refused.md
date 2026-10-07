---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: da2d22ae36452d387f6098d4168972c794f50ad4
github: none
---

- [x] **293 — the scanner costs about a million instructions a refused character on a line of 4,000, growing with the line** | refused characters cost about 74 thousand instructions each at 79 on a line (400 to 800 lines doubling), and about one million each at 4,000 on one line, what is left after defect 282 bounded the telling (lane b9-notext, instruction counts, 2026-10-04) | `selfhost/scan.hero` and the layout it calls · **class: improvement**

    **Origin:** lane b9-notext, 2026-10-04, reproduced on its compiler (`<scratchpad>/batch9/notext/report.md`, *Found beside* 4).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the line on a refused file; no program refused or wrong.

    Repaired at `da2d22ae`, 2026-10-07 (lane b14-text), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The question each refused character and foreign word asked, whether a quote left out moved it, walked its line each time; its two halves are read once a line now and kept in the text's outline (`selfhost/lost_reading.hero`), every message the walk's: 4,000 U+0001 on one line 4.34 billion instructions to 0.43, U+200B 12.28 to 0.54, 80,000 in 7.86 where the base ran past 300 s, the harness's check 13.139 billion both.

## The repair

Repaired at `da2d22ae`, 2026-10-07 (lane b14-text), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The question each refused character and foreign word asked, whether a quote left out moved it, walked its line each time; its two halves are read once a line now and kept in the text's outline (`selfhost/lost_reading.hero`), every message the walk's: 4,000 U+0001 on one line 4.34 billion instructions to 0.43, U+200B 12.28 to 0.54, 80,000 in 7.86 where the base ran past 300 s, the harness's check 13.139 billion both.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
