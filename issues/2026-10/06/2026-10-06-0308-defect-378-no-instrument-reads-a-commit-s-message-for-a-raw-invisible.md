---
kind: defect
area: records
milestone: none
filed: 2026-10-06
commit: 0a4b0340317d3011aab4e126b64d4da062e8851b
github: none
---

- [x] **378 — no instrument reads a commit's message for a raw invisible character** | the body of `fe3f788e` holds one raw U+200B, the write tool having decoded a written `\u200b`, defect 356's hazard; the `unseen` suite that 356 added reads files, not commit messages (counted by the coordinator at 03:08 on 2026-10-06; found by lane b12-str192) | `tests/harness/suite_unseen.hero` · the commit guard, `.claude/hooks/guard_bash.py` · **class: improvement**

    **Origin:** lane b12-str192, 2026-10-06, found beside panel 192's landing (its report's *found beside*); filed by the coordinator at 03:08.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): hardening: a commit's body is outward-facing and append-only, and nothing reads it.

    Repaired at `0a4b0340`, 2026-10-07 (lane b14-hooks), gated by its cases, the hooks' own tests and the net's own tests; the net is owed at the batch's close. The `unseen` suite's list is data, `REFUSED` in `tests/harness/suite_unseen.hero`, and the commit guard reads it from there (`.claude/hooks/unseen.py`) to refuse a commit's, a merge's or a tag's message, `-m`, `-F` or a heredoc, holding such a character, by its code point, line and column; six cases in `.claude/hooks/test_hooks.py`, five red before, `fe3f788e`'s own message refused at line 17, column 28, the net's own tests 290 and 0, and `unseen` 3 and 0 at 23.92 billion instructions against 23.87.

## The repair

Repaired at `0a4b0340`, 2026-10-07 (lane b14-hooks), gated by its cases, the hooks' own tests and the net's own tests; the net is owed at the batch's close. The `unseen` suite's list is data, `REFUSED` in `tests/harness/suite_unseen.hero`, and the commit guard reads it from there (`.claude/hooks/unseen.py`) to refuse a commit's, a merge's or a tag's message, `-m`, `-F` or a heredoc, holding such a character, by its code point, line and column; six cases in `.claude/hooks/test_hooks.py`, five red before, `fe3f788e`'s own message refused at line 17, column 28, the net's own tests 290 and 0, and `unseen` 3 and 0 at 23.92 billion instructions against 23.87.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
