# The task suite: metrics 2 and 4

Home of the rules a task is written by and of where the tasks live, since panel 226 (2026-10-20,
provisional pending the author's reading; design.md Part 11 items 2 and 4). The instrument that
reads a suite is `tests/harness/thesis.hero` (`heroes run tests/harness/thesis.hero -- tasks
<folder>` prints every task's digest and the suite's); what it measures and how is Part 11's, and
this page says only what a task is and where it stands.

## Two suites, two homes

- **The assistant's tasks, in clear**: `tests/thesis/`, labelled `assistant` in its `LABEL` file.
  They prove the instrument (`dry` grades their reference solutions, the trial runs on them) and
  **never enter a published number**: a suite the assistant wrote measures the assistant's priors
  (panel 011; the author's rule of 2026-08-24).
- **The author's tasks, held out**: `tests/thesis/held-out/` (its files land at the milestone's
  step 4, 2026-10-20), sealed in this public repository as `openssl enc -aes-256-cbc -pbkdf2`
  archives, three of them with three keys (`HEROES_TASKS_KEY` the suite, `HEROES_RESERVE_KEY` the
  reserve, `HEROES_RUNS_KEY` the runs' transcripts), base64 so the tree holds text; the
  ciphertext's sha256 beside each, since CBC does not authenticate; `hashes.txt` with the tasks'
  digests by number and never by a telling name; a README naming what is inside, why it is sealed,
  the licence (the project's), the two ways to read it (the key from the author on request, with
  the one condition of not republishing it, the delivery register beside; and the opening), and
  **the opening trigger**: within 30 days of the first `v*` release tagged after the first held-out
  run, or of the suite's being declared burned by the canary rule, whichever is first, the author
  commits the key and the tasks become readable in git's history (panel 226's R10). Until
  2026-10-20 this page said a held-out task lives outside the repository and that committing one
  burns it (issue 999); a sealed task is committed and not burned, and a key that is never public is
  what the seal rests on (BrowseComp's, which travelled with the data, was found by a model with
  search; the sitting's historian, `docs/panel/226-reports/historian.md`). **And no process holding
  a key runs a program**: the archive is opened by `unseal --sealed <file> --into <dir>` in a shell
  where the key is set, and graded, `dry <dir>` or `run <dir>`, in a shell where it is not; the
  grader refuses to run while a `HEROES_*_KEY` stands in its environment, because a program inside
  the sandbox reads another process's environment through `sysctl` (the sitting's ffi-pragmatist,
  measured). The CI does the same in two steps.

## The shape on disk

`<folder>/LABEL` (`assistant` or `held-out`), then one folder per task, `NN/`:

- `task.md`: the prose a model receives, and nothing else; a fixed comment line carrying the
  task's canary GUID (below);
- `cases/K.args`: one argument per line, the token `{dir}` standing for the folder the case's
  files are copied into; `cases/K.files/<name>`: the input files the arguments name, at most three;
  `cases/K.expected`: the expected stdout, then an ending line, `!exit: N` for a non-zero exit or
  `!panic: <text>` for an abort, in `tests/harness/expectation.hero`'s form; **`cases/0` is the
  visible example**, written into `task.md` too, and judged among the hidden ones;
- `reference.hero`: the author's or the assistant's solution, read by `dry` alone and never by the
  grader.

A task's digest is the sha256 of `task.md` and its cases in order; its prose digest the sha256 of
`task.md`; the suite's the sha256 of the task digests sorted.

## The rules a task is written by

- **One program**, `main.hero` with `main()`, no `use`, no `extern` (a program binding C would run
  as the grader's user; `refused_extern` is its bucket). Inputs arrive by `args()` and by at most
  three files whose paths are among the arguments; there is no stdin. Output is exact stdout and an
  exit code; stdout is compared byte for byte, `print` ending every line it writes with a line feed,
  and an output may be nothing, one empty line or any lines (`tests/harness/expectation.hero`);
  stderr is ignored.
- **4 to 8 hidden cases**: one empty, one malformed with the exit code the prose names, one large,
  sized so the reference runs in under a second; deterministic (a map's `keys` has no order, so a
  task that prints a map sorts it or says the order).
- **Prose in English, naming no construct**: no Heroes spelling, no rule of the thesis, no
  example of the specification, no shape of a solution; a sentence like *receives one argument* is
  the most a task says of the language.
- **Every choice point pinned or covered** (the blind reader of panel 226, who found 25 the three
  assistant tasks left open): the argument count and an unreadable input, with their exit codes;
  the trailing newline; CRLF; the character class of a *name* or a *word*; signs, `+5`, `007`
  and `--5`; overflow; the order of ties; an empty line in the middle; a case that is malformed
  only after good lines. Each is pinned by a sentence of the prose or covered by a hidden case, so
  a hidden failure is the language's and never a guess's.
- **A reference of 25 to 80 lines** accepted by `check`, passing every case under `dry` in the
  strict arm and in the control arm.
- **For every mistake the task tempts, a hand mutant and the case that kills it**, kept beside the
  reference under `mutants/` (step 6's `dry` runs them and each bucket must fire).
- **Spread over the surface**, counted over the held-out suite before the freeze: records in at
  least 6 tasks, a `variant` with at least 4 cases in at least 8, `T?` with propagation in at least
  8, `@` parameters in at least 5, maps in at least 6, a generic built-in taking a function value in
  at least 4; and at least 5 (ten is better) that tempt the plausible mistakes the mutation
  operators encode, labelled `trap` in `hashes.txt` and **reported stratified**, trap and not.
- **A difficulty target** from the trial: `green@1` in the strict arm on Sonnet 5.5 between 0.3
  and 0.7 for the median task; the held-out suite's distribution compared with it before the
  freeze, by the reference solutions' shape and the trial's rates, never by running a model on a
  held-out task.
- **A blind second reader** before the freeze: a reader given the prose and the cases alone, not
  the construct the task aims at and not the reference, says what the prose leaves open; what they
  find is pinned or covered.

## The canary

A suite GUID in `LABEL`'s second line and a per-task GUID in the fixed comment of every `task.md`.
Before a run, `canary-probe` asks each model to complete each prefix and records the answer. **A
per-task GUID completed exactly burns that task**, replaced by the next of the reserve in order;
**the suite GUID completed exactly stops the run** and the author reads, since the suite GUID can
be learnt from this repository as BIG-bench's can and the per-task one exists in one sealed file
alone. The Heroes-only spellings (`@=`, `???`, `.must()`, `fail(code:`) are grepped in a model's
code blocks under the `none` context, never in its prose, and a hidden case passed under `none`
is reported as knowledge of the language with the rule of three, not as a burned task: all four
listed models declare a training cutoff of June 2026, before this repository was public.

## Adjudication, pre-registered

- A task found ambiguous after the run (two correct readings of its prose that differ on a hidden
  case) is replaced by the next reserve task in order, the decision taken by a reader who has not
  seen the arms' verdicts, two readers and their agreement recorded.
- A sample with no answer after the transport's retries is excluded from every arm.
- A sample whose program `check` accepted and `build` refused (`build_failed`) is excluded,
  refiled as a compiler defect, and reported in a table of its own; panel 226's R1 expects the
  table empty once `build --permissive` performs every dropped rule's meaning.
- Nothing is resampled in silence: every exclusion is counted in the report's flow table,
  attempted → answered → extracted → compiled → ran → green, per arm.

## History

Until 2026-10-20 this page said: target 20 frozen tasks, author-confirmed, one file per task held
outside this repository; 0 tasks, the 5 unreviewed assistant drafts pruned 2026-08-03; up to 5
assistant-drafted and ratified in `/decide`, 15 author-written; the live item in
`docs/work/SCHEDULED.md` (retired 2026-09-12), keyed to M-guide-book (the author's `8b` of
2026-08-24). The author-written rule stands; the count, the home and the format are panel 226's.
