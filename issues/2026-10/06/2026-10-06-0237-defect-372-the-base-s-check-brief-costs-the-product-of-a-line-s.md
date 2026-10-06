---
kind: defect
area: compiler
milestone: none
filed: 2026-10-06
commit: 17f3f1a4d0b8bd3755de3dd0952f63fb88aefb2e
github: none
---

- [ ] **372 — the base's check --brief costs the product of a line's diagnostics and its length** | one line of 14,039 bytes holding 2,000 refused shapes: `check --brief` retires 148.5 billion instructions; 500 on a line of 3,508 bytes, 25.1 billion, five point nine times as much for four times the input (the base's compiler at `00217c39`, measured by the coordinator at 02:35 on 2026-10-06, `<scratchpad>/batch12/filing/` `longline*.hero`, the critic's files) | `selfhost/source.hero`, `locate` counting a column from its line's start for each place · defect 256's one-walk technique · panel 193's R6 · **class: adjacent**

    **Origin:** panel 193's completeness critic, its second pass (`docs/panel/193-reports/completeness-critic.md`), 2026-10-06; reproduced by the coordinator at 02:35 and filed into batch 12 under the author's instruction of 2026-10-05, any defect found that is not an improvement goes into the batch.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost found beside the work, the product of two counts, no message wrong.

    Repaired at `17f3f1a4`, 2026-10-06 (lane cli12), gated by its cases and the compiler's own tests; the net is owed at the batch's close. **What the entry measured is two causes, and this is one**: on the critic's file the line's length costs 11.5 of the 148.5 billion (the same 2,000 names with no rename to try cost 16.98 billion on one line and 5.49 on 2,000 lines), and a `Source` now carries the offsets of the bytes that continue a character, so a column is two binary searches: 16.98 to 5.45 billion, and 20.15 to 5.46 behind 6,000 bytes of wide characters. The rest, 137.0 billion after, is `rename_fit.judged`'s sixteen trial readings of the whole program, 24.4 billion for 500 names on 500 lines against 0.8 with no rename to try: another cause, reported to the coordinator with its reproducer.
