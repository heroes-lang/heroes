---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: be205ad0ae9e8a1172891ee2378e3626298b3929
github: none
---

- [x] **496 — a broken type in a head is told four messages for one mistake** | defect 456's shapes: the broken-type head is told 4 messages; with two such types, the second one's `expected_function_type` is told twice at one place (8:9); the 456 cases pin it as it stands (lane b14-text) | `selfhost/parse/brace_habit.hero` and the head's recovery · defect 456 · **class: adjacent**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-text's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a mistake told four times.

    Repaired at `be205ad0`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The line below a parameter's type broken after its `(` is the head's own and no spilled body: a spill's refused word stands in no bracket opened after the head's list, and its closer ends its line (`spill_reading.hero`); the broken-type head told 4 messages to 1, two such types 6 to 2 (the second's twice at 8:9 gone), defect 456's four cases moved and read, the new case 13 to 4.

## The repair

Repaired at `be205ad0`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The line below a parameter's type broken after its `(` is the head's own and no spilled body: a spill's refused word stands in no bracket opened after the head's list, and its closer ends its line (`spill_reading.hero`); the broken-type head told 4 messages to 1, two such types 6 to 2 (the second's twice at 8:9 gone), defect 456's four cases moved and read, the new case 13 to 4.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
