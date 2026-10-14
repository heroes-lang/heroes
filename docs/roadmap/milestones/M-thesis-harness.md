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

**Re-ordered and re-scoped 2026-10-14, by the author's instruction**: this row
moves from 73 to 69, ahead of the packages (`docs/roadmap/scheduling.md`). The
sentence above that makes it depend on M-core-packages' HTTP client no longer
holds: `selfhost/cli/refresh.hero` already reaches an HTTPS API by spawning
`curl`, and `examples/curl` binds libcurl with no language change. Four
findings of the re-ordering's reviews change what it measures, each to be
settled by a sitting on Part 11:

- **One turn decides the control arm by design.** At a single turn
  `heroes check` and `--permissive` judge the same samples, so the strict arm
  can only tie or lose (design.md Part 11: *a stricter checker lowers
  first-try rate*). The primary outcome becomes hidden tests passed within k
  turns of the compiler's feedback, k = 1 being metric 2; the tokens spent to
  green and the silent failures are secondary.
- **The control arm is not what Part 11 says**: it lists a non-exhaustive
  `match` among the checks `--permissive` disables, and `non_exhaustive` is
  not in `selfhost/diag.hero`'s `is_thesis_rule`. Settled in
  M-rechecked-issues, before any task is frozen.
- **The strongest claim is the gap between models**: whether the rules narrow
  the distance between a small model and a large one (Haiku 5.5 at $0.10 and
  $0.50 per million tokens against Opus 5.5 at $4 and $20, Anthropic's pricing
  page read 2026-10-14). Two arms across three tiers, one more model as the
  ceiling and one non-Anthropic model co-primary, since the measured model
  co-designed the language; 40 to 60 tasks at three samples each, so the
  constraint is the author's tasks, not money. The instrument is tried first
  on the assistant's tasks, labelled not held-out.
- **Contamination**: the current models' training stops in June 2026 and the
  repository is public since 2026-09-08, so the baseline is taken on these
  pinned snapshots and never mixed with later ones; the tasks' hashes and the
  prediction are committed when the tasks are frozen; one arm with no
  specification is the canary.

And the message itself, measured: for each diagnostic code, cases from the
planted mistakes of `heroes mutate`'s frozen plan, given to a model in three
conditions, the message with its fix, the code and line alone, and nothing, so
that the message is measured rather than the program's readability. A mistake
models make for real becomes a candidate rule only if it repeats across two
model families or tiers and on held-out tasks, and then goes to a sitting.
