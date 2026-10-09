---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: cb8c5704b35485dbcdd8b3b8e21e5ab91c9ec21d
github: none
---

- [x] **482 — `closers.close_to_its_kind` walks the whole stack for every closer of the wrong kind** | `(` N times then `]` N times: `lex` costs 0.34, 1.21 and 4.59 billion at 1,000, 2,000 and 4,000; the same line gets N `expected_group_close`, one per `]` (lane b14-parse) | `selfhost/closers.hero` · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the line's square, and a message per closer.

    Repaired at `cb8c5704`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A closer no opener of its kind answers is remembered until an opener is pushed (`lex_asked.hero`), the parser's drop of a failed line counts its stack, and a run of closers told alike is one report noting the rest (`closer_runs.hero`): `(` then `]` 4,000 each, `check` 14.36 to 1.85 billion instructions retired and 4,000 messages to 1, `lex` 4.22 to 1.15 (the growth left is defect 473's pop by a copy).

## The repair

Repaired at `cb8c5704`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A closer no opener of its kind answers is remembered until an opener is pushed (`lex_asked.hero`), the parser's drop of a failed line counts its stack, and a run of closers told alike is one report noting the rest (`closer_runs.hero`): `(` then `]` 4,000 each, `check` 14.36 to 1.85 billion instructions retired and 4,000 messages to 1, `lex` 4.22 to 1.15 (the growth left is defect 473's pop by a copy).

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
