---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: f02eac4b7d44ead2e1f7881aa962b92b862d345d
github: none
---

- [x] **494 — a run of stray carriage returns, tabs or foreign words is told once per character, past defect 282's bound** | on the base compiler: 4,000 stray carriage returns 4,000 messages and 193 MB and 432 billion instructions; 4,000 tabs 4,001 messages and 22.3 billion; 1,334 foreign words 1,334 `reserved_word` and 14.7 billion; defect 282 bounded the plain refusal only, and 294 shrinks the bytes, not the count (lane b14-text) | the lexer's refusals, `selfhost/scan.hero` and beside · defects 282 and 294 · **class: adjacent**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-text's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): one mistake told thousands of times.

    Repaired at `f02eac4b`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A run of adjacent tabs or carriage returns, and of one foreign word's refusal through the spaces between, is told once at its first, *the first of N in a row*, tabs and carriage returns eight times a text and the rest counted (`refused_runs.told_once`, each kind its own count): 4,000 carriage returns 4,000 messages to 1 and 0.55 to 0.14 billion instructions retired, 4,000 tabs 4,000 to 1 and 0.50 to 0.14, 1,333 `fn` 1,333 to 2; the card's 193 MB and 432 billion did not reproduce on these shapes, its counts did.

## The repair

Repaired at `f02eac4b`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A run of adjacent tabs or carriage returns, and of one foreign word's refusal through the spaces between, is told once at its first, *the first of N in a row*, tabs and carriage returns eight times a text and the rest counted (`refused_runs.told_once`, each kind its own count): 4,000 carriage returns 4,000 messages to 1 and 0.55 to 0.14 billion instructions retired, 4,000 tabs 4,000 to 1 and 0.50 to 0.14, 1,333 `fn` 1,333 to 2; the card's 193 MB and 432 billion did not reproduce on these shapes, its counts did.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
