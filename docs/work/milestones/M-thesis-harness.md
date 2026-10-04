# M-thesis-harness — the thesis, measured


**Scheduled by author instruction 2026-09-03**, and it is the second milestone
after the fixpoint with a warrant: **§1.1**, comprehension measured rather than
asserted, and design.md Part 11, whose metrics 2 and 4 have never run
(`docs/measurements/007:24-25`; M-publication-gate's checklist says so in its own
words). Everything this project claims about first-try rates rests today on
metric 3 alone.

**What it delivers is the instrument**, written in Heroes and run by the one
command: Part 11's protocol — spec-only context, single turn, frozen and hashed
prompt templates, two gradings (compiles · tests pass), Wilson intervals, one
non-Anthropic model as robustness, provenance on every run (spec sha, compiler
sha, model id, prompt sha, suite sha) — and metric 4's turns-to-green over broken
programs, capped at 5. It reaches the model over HTTPS, so it depends on
M-core-packages' `net/http` client (libcurl, step 11) and reads its key from the
environment, one of §10's three input classes.

**The tasks, and a decision of the same night.** Metric 2's held-out tasks must
be author-written or they measure the assistant's priors (panel 011; author
decision 2026-08-24, the M-guide-book item in `SCHEDULED.md`). That decision
stands: the bulk of the set is written at M-guide-book, where the author's own
work *is* writing Heroes. What this milestone adds, by author decision
2026-09-03, is a **small seed of tasks written by the author at its opening**, so
that the instrument runs once on real input before the book grows the set, and
so that the number exists before the books state it.

**Why here.** After the packages that give it a client; before the tools, whose
value it does not need; before the books and the gate, which will print the
number. Corpus material is labelled and never enters the held-out set
(CLAUDE.md §9).
