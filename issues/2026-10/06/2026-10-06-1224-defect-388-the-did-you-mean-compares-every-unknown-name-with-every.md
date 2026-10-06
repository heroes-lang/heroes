---
kind: defect
area: resolve
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **388 — the did-you-mean compares every unknown name with every candidate near it in length** | after defect 385's repair the scope reproducer, N locals and N unknown names none near enough to offer, still grows about 3.35 times for each doubling of N, 7.89 billion instructions at 800, about 12 thousand instructions a comparison by division (an inference); a sample at 1,600 names puts 1,590 of the name stage's 1,711 in `nearest_in` (lane fit12's measurement, not re-run by the coordinator) | `selfhost/resolve/near_names.hero`, `nearest_in` and `within` · an index keyed by every deletion of up to two letters, unbuilt · defects 372 and 385 · **class: improvement**

    **Origin:** lane b12-fit12, 2026-10-06 (its final reply on defect 385, decision 1); filed by the coordinator at 12:24.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): the comparisons are the floor of a did-you-mean over every candidate, no message wrong; what is left grows only with many unknown names among many names close to them in length, and an index would cost something at every binding in every program. Outside the batch under the author's instruction of 2026-10-05.
