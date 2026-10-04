---
kind: decision
area: none
milestone: none
filed: 2026-08-10
commit: dcb22e4a2aca7a646a7000f2b4033498a4d93e08
github: none
---

2026-08-10 | **`/` and `%` truncate toward zero, stated: `-7 / 3` is `-2`, `-7 % 3` is `-1`** (+26, replacing the parenthetical `(integer division truncates)`). This is the **only genuinely silent gap** the panel found, and the artifact is a program rather than an argument: the same clock-arithmetic task written from the unamended spec prints `-1:0` while its own comment says it expects `23:0` — compiles, runs, wrong, no instrument firing. Worse than a missing rule, the ergonomist's diagnosis: the old spec's `%` was **extra-textual**, decided by the reader's prior language, and the document's Python-shaped surface primes Python's `2` while the arithmetic is C's. *"A worse condition than non-locality, and one I cannot veto, only measure."* Verified on all six sign combinations | §4.14 mandates specifying arithmetic edge cases in its own heading, "because unspecified means the model invents" | §4.14, spec line 129 | 025 |
