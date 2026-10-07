---
kind: defect
area: resolve
milestone: none
filed: 2026-10-06
commit: 7fbf2a43b2afe2cb3f64a09a6cea2453bb9b341a
github: none
---

- [ ] **388 — the did-you-mean compares every unknown name with every candidate near it in length** | after defect 385's repair the scope reproducer, N locals and N unknown names none near enough to offer, still grows about 3.35 times for each doubling of N, 7.89 billion instructions at 800, about 12 thousand instructions a comparison by division (an inference); a sample at 1,600 names puts 1,590 of the name stage's 1,711 in `nearest_in` (lane fit12's measurement, not re-run by the coordinator) | `selfhost/resolve/near_names.hero`, `nearest_in` and `within` · an index keyed by every deletion of up to two letters, unbuilt · defects 372 and 385 · **class: improvement**

    **Origin:** lane b12-fit12, 2026-10-06 (its final reply on defect 385, decision 1); filed by the coordinator at 12:24.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): the comparisons are the floor of a did-you-mean over every candidate, no message wrong; what is left grows only with many unknown names among many names close to them in length, and an index would cost something at every binding in every program. Outside the batch under the author's instruction of 2026-10-05.

    Repaired at `7fbf2a43`, 2026-10-07 (lane b14-resolve), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Reproduced on the base (`dad2da47`): the scope reproducer 0.67, 2.10, 7.42 and 27.91 billion instructions at 200 to 1,600, three point one to three point eight per doubling. The index the entry names, every deletion of up to two bytes (`resolve/near_index.hero`), keyed by hashes so that a collision adds a candidate and loses none, is built for a list only once the bands walked against it reach its entries (`resolve/asked.hero`). Against the compiler before it, every `check --json` byte-identical: 0.69, 2.13, 7.52 and 28.27 against 0.43, 0.84, 1.88 and 4.68 billion; names a slip from the locals 143.5 against 64.8 at 1,600; `check selfhost/main.hero` 66.29 and 66.23 against 66.24 and 66.25 over two rounds, so not dearer, the brief's condition for landing it. Keyed by strings, the index first made 200 far names of 15 bytes 9.5% dearer; hashed, 2.6% cheaper.
