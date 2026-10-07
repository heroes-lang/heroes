---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: 5a019922a881fa6b569fc5504117cb82de7eeff9
github: none
---

- [x] **294 — an excerpt prints its line whole, 32 KB under each message for a line of 4,000 refused characters** | a line of 4,000 refused characters is printed whole under each message, eight of them since defect 282, about 32 KB each (lane b9-notext, 2026-10-04) | `selfhost/diag_render.hero` (the excerpt) · **class: improvement**

    **Origin:** lane b9-notext, 2026-10-04, reproduced on its compiler (`<scratchpad>/batch9/notext/report.md`, *Found beside* 5).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a message larger than what it names; no program refused or wrong.

    Repaired at `5a019922`, 2026-10-07 (lane b14-text), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A line, or the stretch of it before the span, wider than 120 columns is cut to a window of 120 around the span, `…` on each side the line goes on, the caret under it (`selfhost/excerpt_window.hero`); a line within 120 is printed as before, no golden of `full` or `unsupported` moving: 4,000 alternating control characters 257,579 bytes to 2,483, 4,000 trailing tabs 8,477,148 to 951,888.

## The repair

Repaired at `5a019922`, 2026-10-07 (lane b14-text), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A line, or the stretch of it before the span, wider than 120 columns is cut to a window of 120 around the span, `…` on each side the line goes on, the caret under it (`selfhost/excerpt_window.hero`); a line within 120 is printed as before, no golden of `full` or `unsupported` moving: 4,000 alternating control characters 257,579 bytes to 2,483, 4,000 trailing tabs 8,477,148 to 951,888.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
