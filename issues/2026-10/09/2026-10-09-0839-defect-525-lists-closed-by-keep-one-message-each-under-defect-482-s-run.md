---
kind: defect
area: parse
milestone: none
filed: 2026-10-09
commit: c7b3473f48d263691ff7c57d22c199e7df109555
github: none
---

- [ ] **525 — lists closed by `)` keep one message each under defect 482's run** | defect 482's repair tells a run of wrong closers once, and lists closed by `)` keep one message each, each naming its own `[` column (lane b15-parse) | `selfhost/parse/` · defect 482 · **class: adjacent**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

    Repaired at `c7b3473f`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Reports at adjacent closers alike but for the numbers in them are one run, told at its first with a note that the closers after it are each for the next opener out (`closer_runs.told_once`), so `[[[[1))))` is told once as `((((1]]]]` is; a mixed run stays apart. One new `full` case pinning the note, red on the base; `fixedbugs-482-a-run-of-closers-of-the-wrong-kind-is-told-once` moved from three reports to one.
