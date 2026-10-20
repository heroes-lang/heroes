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

**Opened 2026-10-20, at row 70, in the lane `thesis`**, the author's instruction
of the night before meant as *start the loop and run until the step is done*,
the session left unattended. The decisions the author took on 2026-10-19,
in the session that planned the opening, and what the opening found, so that
the sitting and the steps start from them:

- **The instrument is a program of the harness**, `tests/harness/thesis.hero`, not yet written on 2026-10-20, run as `heroes run tests/harness/thesis.hero -- <word> ...` from that day's steps on: panel 212's
  reading of CLAUDE.md § 10 for `issues.hero`, extended here by the author.
  Nothing enters the seed, the help, the command table or the site.
- **The held-out tasks live in this repository, sealed**: a `tar` of the
  tasks encrypted with `openssl enc -aes-256-cbc -pbkdf2 -a`, the key read by
  name from the environment (`-pass env:`), base64 so the `unseen` suite reads
  it as clean text, decrypted only into the lane's scratch under `build/` and
  removed when the run ends; the ciphertext's sha256 beside it, since CBC does
  not authenticate. The author's idea, against the private repository the
  plan had proposed: one history, the archive's sha is the suite's sha, and the
  commit that carries it is the date the suite was frozen. Three archives and
  three keys, the suite, the reserve and the runs' transcripts, so one leak
  burns one. **What the author asked for in return is transparency**, so that
  nobody reads a sealed blob as a closed piece: a README beside it naming what
  it holds, its licence (the project's), the two ways to read it (the key from
  the author on request, never republished; the date it opens, when the key is
  committed), a canary string with a probing protocol, and a CI job that
  decrypts in the runner and prints counts and hashes alone. **When it opens,
  and whether the key also stands in a GitHub Actions secret, the author left
  to the sitting.**
- **Three transports**: the Anthropic Messages API, an OpenAI-compatible API
  (a commercial key or a local server), and a `harness`, `claude -p` and
  `codex exec`, the author's idea, since a ChatGPT subscription gives no API
  key but Codex's non-interactive mode runs on the plan's included usage.
  **Which number is the thesis measured, the API's with the harness rows in
  a table of their own (the coordinator's recommendation) or the harness's
  alone, the author left to the sitting.** A held-out task never travels on a
  consumer transport until its terms document that prompts do not train.
- **The order of the ceremony**: the open questions decided first, the
  sitting after, free to overturn (the author, 2026-10-19). The session being
  unattended, each question took its recommended answer (CL-002; the list and
  the direction each went are in the shared brief of the sitting this opening
  convenes), and the author's verdict is a reading of the sitting.

**Three facts of the tree, measured at the opening, that change the protocol.**
`--permissive` is a flag of `check` alone (`selfhost/cli/table.hero:94`, in
`check_flags`): nothing has ever built or run a permissive binary, so the
control arm can judge a program and cannot run its hidden cases, and the primary
outcome is not measurable there today; the route taken is `build --permissive`
(and `run`, `test`) with a runtime meaning pinned for each of the 27 codes
`is_thesis_rule` lists, each row motivated by what the most common permissive
language does, for the held-out run, and the judge-only control, declared
biased, for the trial. `check` speaks one stage at a time
(`selfhost/cli/check.hero:266-275`), so a program with a parse slip and a type
slip costs two turns in every arm: the stages spoken are recorded per sample
and never subtracted. And no hidden-case format exists (the run goldens hold
stdout and one ending line, `tests/harness/expectation.hero:12-17`), and the
harness's watchdog is fixed at 600 s (`tests/harness/shell.hero:262`, `:751`),
so the grader needs a `run_bounded` of its own.

**What a blind reading of the plan found** (an LLM-evaluation reader given the
plan alone, 2026-10-19), and the protocol now carries: the result is decided by
whoever writes the control's definition, so the control reproduces what the
user of a permissive language actually sees, a run-time panic included, and
the feedback loop is symmetric in every arm (the compiler's message on a
refusal; the visible example run and its observed-against-expected returned on
a program that compiles; the hidden cases never); failures come in three
classes, refused at compile time, failed loudly at run time, silent at exit 0
with a wrong output, and the thesis is measured on the third; there is one
pre-registered primary, `green@3` strict-full minus control on Sonnet 5.5 with a
cluster bootstrap over tasks and McNemar on the discordant pairs, named so
because Chen et al. 2021's pass@k is another quantity, everything else
exploratory, and three outcomes named before the freeze, confirmed, falsified,
inconclusive; the sample size comes from the trial's measured discordance and
intra-task correlation, never from the width of a single proportion's interval;
contamination has two layers, the language (the spec is public since
2026-09-08, each model's declared cutoff goes in the provenance) and the tasks
(the canary, the transports' terms); and the model's program runs in a sandbox,
never as the author's user.

**What the author owes, and when**: the 40 held-out tasks and 20 in reserve,
written on their own clock after the trial, read blind by a second reader,
checked against the difficulty target the trial sets; the three keys; a
spending cap on the console and an API key dedicated to the milestone before
the first paid run; `codex` installed and signed in if its row is wanted; the
sitting's ratification. **Every paid run is asked for, every time**, with the
request count and the token estimate the program's `plan` word prints in front
of the question; the program sends nothing without `--live`.

**The run order, each a gate on the next**: a dry run at no cost over the
reference solutions and hand-written mutants; the trial on the assistant's ten
tasks, with the harness rows beside it; the canary probe and the `none` context
on the same tasks; the freeze, one commit carrying the archives, the hashes,
the prediction with its numbers, the compiler's and the prompt's and the
grader's digests; the first held-out run; then issue 2080's three-condition
experiment on the recovery plan's planted mistakes.
