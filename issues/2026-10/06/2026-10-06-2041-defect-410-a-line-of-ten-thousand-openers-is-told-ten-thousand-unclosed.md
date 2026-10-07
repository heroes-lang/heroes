---
kind: defect
area: parse
milestone: none
filed: 2026-10-06
commit: 5d5a2943aeda36ff8cbf4566e44f21e7f9fcfc78
github: none
---

- [x] **410 — a line of ten thousand openers is told ten thousand `unclosed_bracket`** | a line of 10,000 opening brackets gets one `unclosed_bracket` per bracket, on the base and on lane front alike | `selfhost/parse/`, the pairing of brackets · **class: improvement**

    **Origin:** lane b13-front, 2026-10-06 (its report, *found beside, not filed*), the lane's measurement on its branch from `7001dfb3`, not re-run by the coordinator; filed by the coordinator at 20:41.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a pathological input, no program a writer means; at most an improvement.

    Repaired at `5d5a2943` (2026-10-07, lane b14-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Reproduced on `dad2da47`: 10,000 reports, the full form 152,573,925 bytes. The openers of one line the lexer named open at one place are one report at the first now, saying how many more stand after it on its line, in both arms (`unclosed_runs`, after `parse/unclosed.withdrawn`): 10,000 reports to 1, the full form to 10,290 bytes; eight `check` goldens moved, 11 runs of two reports to one each. Openers on lines of their own, and openers with another place the lexer named openers at between them, keep a report each.

## The repair

Repaired at `5d5a2943` (2026-10-07, lane b14-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Reproduced on `dad2da47`: 10,000 reports, the full form 152,573,925 bytes. The openers of one line the lexer named open at one place are one report at the first now, saying how many more stand after it on its line, in both arms (`unclosed_runs`, after `parse/unclosed.withdrawn`): 10,000 reports to 1, the full form to 10,290 bytes; eight `check` goldens moved, 11 runs of two reports to one each. Openers on lines of their own, and openers with another place the lexer named openers at between them, keep a report each.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
