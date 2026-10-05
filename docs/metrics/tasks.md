# Task suite — metric 2

Target: 20 frozen tasks, author-confirmed. Format: one file per task,
`NN-slug.md`, **held outside this repository**, which is public since
2026-09-08: committing a held-out task is the act that burns it
(`issues/2026-09/08/2026-09-08-0000-the-held-out-tasks-must-never-be-committed-now-that-the.md`,
which also owes the line naming that home). A task file holds only the task
prose a model would receive (no solutions there — graders live in the goldens
once the compiler exists).

Guidelines (panel 000): tasks must be solvable from the spec alone, span the
surface (records, variants+match, `T?`+`?`, `@` parameters, loops, maps,
generics use, tests), and include at least five designed to *tempt* the
plausible mistakes the mutation operators encode (same-typed args, missing
variant case, empty-container inference).

Status: 0 tasks. The 5 unreviewed assistant drafts were pruned 2026-08-03
(process simplification — they can be redrafted in minutes when the suite
becomes runnable). Of the 20: up to 5 may be assistant-drafted and ratified
in `/decide`; **15 must be author-written** (the held-out set — the
assistant must NOT write them all, or the suite measures the assistant's
priors). The live item is in `docs/work/SCHEDULED.md (retired 2026-09-12)`, keyed to **M-guide-book**
(author decision 2026-08-24, `/decide` answer `8b`) — it said `QUEUE.md` and
`/debrief` until 2026-08-26, naming the record and a skill deleted 2026-08-12.
