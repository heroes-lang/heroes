---
kind: defect
area: compiler
milestone: none
filed: 2026-10-06
commit: 16af01c36d27854f8f77520c244d5d557fc3a66b
github: none
---

- [ ] **372 — the base's check --brief costs the product of a line's diagnostics and its length** | one line of 14,039 bytes holding 2,000 refused shapes: `check --brief` retires 148.5 billion instructions; 500 on a line of 3,508 bytes, 25.1 billion, five point nine times as much for four times the input (the base's compiler at `00217c39`, measured by the coordinator at 02:35 on 2026-10-06, `<scratchpad>/batch12/filing/` `longline*.hero`, the critic's files) | `selfhost/source.hero`, `locate` counting a column from its line's start for each place · defect 256's one-walk technique · panel 193's R6 · **class: adjacent**

    **Origin:** panel 193's completeness critic, its second pass (`docs/panel/193-reports/completeness-critic.md`), 2026-10-06; reproduced by the coordinator at 02:35 and filed into batch 12 under the author's instruction of 2026-10-05, any defect found that is not an improvement goes into the batch.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost found beside the work, the product of two counts, no message wrong.

    Repaired at `17f3f1a4`, 2026-10-06 (lane cli12), gated by its cases and the compiler's own tests; the net is owed at the batch's close. **What the entry measured is two causes, and this is one**: on the critic's file the line's length costs 11.5 of the 148.5 billion (the same 2,000 names with no rename to try cost 16.98 billion on one line and 5.49 on 2,000 lines), and a `Source` now carries the offsets of the bytes that continue a character, so a column is two binary searches: 16.98 to 5.45 billion, and 20.15 to 5.46 behind 6,000 bytes of wide characters. The rest, 137.0 billion after, is `rename_fit.judged`'s sixteen trial readings of the whole program, 24.4 billion for 500 names on 500 lines against 0.8 with no rename to try: another cause, reported to the coordinator with its reproducer.

    Repaired at `16af01c3`, 2026-10-06 (lane fit12), gated by its cases and the compiler's own tests; the net is owed at the batch's close. **The second cause was two, measured apart** with compilers built to stop a trial at each stage (100 to 1,600 names, one line and one a line): of the critic's 137 billion, the sixteen readings are linear in the program, about 3.6 billion each at 1,600 names and nearly all of it the name stage, and the matching of what a trial said against the program's own was a walk and a copy of the list for each, `n` squared, 1.89 billion a trial at 1,600 names and growing four times per doubling. A count per `Said` answers the same at the same thing, and the did-you-mean reads a band of its edit table: the critic's file 122.87 to 68.54 billion, every verdict byte for byte over 7,107 programs. What stays is the sixteen readings themselves, panel 187's design and its `TRIED`, linear in the program, which `check --apply` repeats once per sixteen renames; the coordinator has the measurements and the options.
