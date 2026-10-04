---
kind: task
area: design
milestone: M-guide-book
filed: 2026-09-08
commit: none
github: none
---

- [ ] **M-guide-book** | the held-out tasks must never be committed, now that the repository is public | `harness/tasks/README.md` · `design.md` Part 11

    **Origin:** M-open-repository, 2026-09-08, and nobody had written it down.

    Metric 2 is held out: 15 of the 20 tasks must be author-written, and the
    whole point is that the model reading them has not seen them. **A public
    repository makes committing them the act that burns them** — not only for a
    scraped training set, but for anybody who reads the file before taking the
    test. Nothing is burned today, because `harness/tasks/` holds **0 tasks**
    and the five assistant drafts were pruned on 2026-08-03.

    Owed at the moment the first task is written: a home outside this
    repository, and a line in `harness/tasks/README.md` naming it. What can be
    committed is the grading and the provenance, which say nothing about the
    task's content.

    **Why it matters:** a measurement whose instrument is public measures
    something else.

    **Re-verified 2026-09-10: STILL OPEN, and its factual premise is true
    today.** `harness/tasks/` holds one file and **zero** tasks, so nothing is burned
    yet, which is exactly the window the item exists to use. What is owed is still
    absent: `grep -in "outside this repository" harness/tasks/README.md` finds
    nothing, so no home outside this repository is named anywhere.
