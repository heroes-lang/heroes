---
kind: defect
area: mutate
milestone: none
filed: 2026-10-11
commit: feb161cd25a8cbadefdfe79916dc1dc294094d63
github: https://github.com/heroes-lang/heroes/issues/2003
---

- [x] **665 — R2 never judges APPLY-OTHER where the program carries its own message** | `mutate/applied.judged_apply` reads APPLY-OTHER's `clean` on what `check --apply` wrote as told, while defect 663's repair takes the unmutated program's own messages out of the same arm before `fresh` and `stands` are read; so a mutant of a corpus program told a message of its own (the frozen `examples/ledger/db/sqlite.hero` and `examples/ledger/main.hero`, told `never_rebound` at 322 by panel 210's rule) is never clean after `--apply`, and a certain fix that writes another program there is never flagged APPLY-OTHER, the one flag of the four that names defect 172's `blocking` class | `selfhost/mutate/applied.hero` (`judged_apply`'s `clean`, `less_own`), `selfhost/mutate/own.hero` (the subtraction) · **class: adjacent**

    **Origin:** found by lane b22-663 at 12:22 on 2026-10-11, reading `judged_apply` while repairing defect 663: its `clean` was left on the unsubtracted arm, the three flags then reading two measures; filed as 665 on the coordinator's word at 12:24.

    **Class: adjacent**, 2026-10-11: real, found beside the work, an instrument blind where it could see; no program is wrongly accepted and no message is false.

    Repaired at `feb161cd25a8cbadefdfe79916dc1dc294094d63`, 2026-10-11 (lane b22-663), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `clean_of` reads APPLY-OTHER's first question on the arm less the program's own messages, the measure `fresh` and `stands` read since defect 663: exit 0 with nothing left, or exit 1 whose every message was the program's with no other stderr line; an exit past 1 never. Not re-measured by an R2: that is the batch's, after the push.

## The repair

Repaired at `feb161cd25a8cbadefdfe79916dc1dc294094d63`, 2026-10-11 (lane b22-663), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `clean_of` reads APPLY-OTHER's first question on the arm less the program's own messages, the measure `fresh` and `stands` read since defect 663: exit 0 with nothing left, or exit 1 whose every message was the program's with no other stderr line; an exit past 1 never. Not re-measured by an R2: that is the batch's, after the push.

**Closed 2026-10-11** with batch 22 (lanes b22-662, b22-663, b22-site, b22-deps and b22-codeql, merged into the round `lane-round-b22` made from the trunk at `2edee78b`, with the trunk's `80c66b00`, `b6accb55` and `e9284f11` merged in), the closing commit carrying VERSION 0.3.0 for release v0.3.0. Its closing gate ran on the round at `4bc0bebc`: the seed regenerated over two generations with VERSION 0.3.0, the runtime's ABI at 30, its fixpoint by `cmp`, SHA-256 beginning `4882373884cc9fa3`, 52,119,283 bytes; the compiler's own tests 1,620, all passed; the net's own tests 341, all passed; the full net 8,306 passed over 29 suites and 1 failed, `records/counts` alone, on the ROADMAP's open count this closing moves, `run` in four shards and `cache` alone after. The emissions were blessed on this Mac for the version line alone (597 files, one line each), and the two cases this Mac skips, 437's and 589's, on Linux arm64 at the same tree (emission 1,225 passed and 2 failed before the bless, those two; 1,227 and 0 after); the other 597 read byte for byte the same on both. Lane b22-codeql's repairs and the trunk's `b6accb55` and `e9284f11` touch no compiler file and were judged by their own cases after the gate. Under the optimistic chain the CI's legs judge it after the push, a red leg filing a new `blocking` defect naming it.
