---
kind: defect
area: harness
milestone: none
filed: 2026-10-04
commit: d11f4d84c5fc1e8998170dd1cd14094c114fe6f0
github: none
---

- [ ] **330 — the net's own test that the captures are removed can never fail: it looks for whole paths with the prefix `stdout-`** | `shell.hero`'s test *the captures are removed ...* compares each listed path, whole, with the prefix `stdout-`; with two capture files planted it counts 0 and passes; the `captures_left` helper lane b10-harness added reads them right | `tests/harness/shell.hero` (the test) · defect 320's capture names · **class: improvement**

    **Origin:** lane b10-harness, 2026-10-04, measured on its worktree at `bd1f168c` (its final reply's *Found beside*).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): an instrument that cannot fire, hardening of the net.

    Repaired at `d11f4d84`, 2026-10-07 (lane b14-harness-a), gated by its cases and the net's own tests; the net is owed at the batch's close. The test counts the capture names through `captures_left`, asserts 0 where its ceiling was 2, and plants two captures and asserts the count reads 2: measured on the base with 2 and with 24 captures planted, the old count read 0 and passed both, and the repaired assertion with two planted fails, `left: 2, right: 0`; the net's own tests 289, all passed.
