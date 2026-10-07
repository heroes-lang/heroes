---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: da2d22ae36452d387f6098d4168972c794f50ad4
github: none
---

- [ ] **293 — the scanner costs about a million instructions a refused character on a line of 4,000, growing with the line** | refused characters cost about 74 thousand instructions each at 79 on a line (400 to 800 lines doubling), and about one million each at 4,000 on one line, what is left after defect 282 bounded the telling (lane b9-notext, instruction counts, 2026-10-04) | `selfhost/scan.hero` and the layout it calls · **class: improvement**

    **Origin:** lane b9-notext, 2026-10-04, reproduced on its compiler (`<scratchpad>/batch9/notext/report.md`, *Found beside* 4).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the line on a refused file; no program refused or wrong.

    Repaired at `da2d22ae`, 2026-10-07 (lane b14-text), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The question each refused character and foreign word asked, whether a quote left out moved it, walked its line each time; its two halves are read once a line now and kept in the text's outline (`selfhost/lost_reading.hero`), every message the walk's: 4,000 U+0001 on one line 4.34 billion instructions to 0.43, U+200B 12.28 to 0.54, 80,000 in 7.86 where the base ran past 300 s, the harness's check 13.139 billion both.
