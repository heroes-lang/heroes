# A fallible case named twice is defect 128's class, not a defect found after the waiver

2026-09-28 | lane 123's finding that two `.ok` arms on a fallible draw no
`duplicate_arm` is read as defect 128's class and repaired in 128's commit
(`c6d62c7e`), not filed as a new defect carried by the author's waiver of 16:40
(`docs/records/log/2026-09-28-1640-m-agreed-retention-closes-over-the-defects-found-after-its-last-eight.md`) |
the same rule, `duplicate_arm`, an arm an earlier arm covers, in the
neighbouring branch of the same function, `check/walk.case_pattern`, whose
variant branch already refused a case named twice; landing in 128's commit, it
leaves the waiver's reason, commits left pending, untouched; CLAUDE.md § RUN IT
says the shapes beside a defect are where what it is becomes visible (CL-078).
Reproduced on the trunk's compiler at `d0f24496` before it was decided
(`w03_dup_ok.hero` in `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect127/`).
The author may overrule it, and the repair then stands as a defect of its own
closed early | design.md §4.17 | none: the coordinator's reading of the
author's instruction, put to the author at 18:58
