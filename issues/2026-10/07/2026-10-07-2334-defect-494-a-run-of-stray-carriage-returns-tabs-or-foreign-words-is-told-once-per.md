---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: f02eac4b7d44ead2e1f7881aa962b92b862d345d
github: none
---

- [ ] **494 — a run of stray carriage returns, tabs or foreign words is told once per character, past defect 282's bound** | on the base compiler: 4,000 stray carriage returns 4,000 messages and 193 MB and 432 billion instructions; 4,000 tabs 4,001 messages and 22.3 billion; 1,334 foreign words 1,334 `reserved_word` and 14.7 billion; defect 282 bounded the plain refusal only, and 294 shrinks the bytes, not the count (lane b14-text) | the lexer's refusals, `selfhost/scan.hero` and beside · defects 282 and 294 · **class: adjacent**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-text's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): one mistake told thousands of times.

    Repaired at `f02eac4b`, 2026-10-09 (lane b15-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A run of adjacent tabs or carriage returns, and of one foreign word's refusal through the spaces between, is told once at its first, *the first of N in a row*, tabs and carriage returns eight times a text and the rest counted (`refused_runs.told_once`, each kind its own count): 4,000 carriage returns 4,000 messages to 1 and 0.55 to 0.14 billion instructions retired, 4,000 tabs 4,000 to 1 and 0.50 to 0.14, 1,333 `fn` 1,333 to 2; the card's 193 MB and 432 billion did not reproduce on these shapes, its counts did.
