# The held-out tasks of the thesis instrument, sealed

This folder holds the author-written tasks design.md Part 11's metrics 2 and 4 are measured on,
**encrypted**, in this public repository. Nothing here is closed source: this page says what is
inside, why it is sealed, under which licence, how to read it today and when it opens by itself.
The rules the tasks are written by are `docs/metrics/tasks.md`; the sitting that decided this shape
is panel 226 (2026-10-20; `docs/panel/`, the file opening `226-`), on the author's decision of
2026-10-19 that the held-out tasks live here sealed rather than in a private repository.

## What is inside

- `suite.enc`: 40 tasks in the shape of the ten assistant-written tasks in clear one folder up
  (`tests/thesis/01` to `10`): a `task.md` of English prose a model receives, 4 to 8 hidden cases
  (`cases/K.args`, `cases/K.files/`, `cases/K.expected`), a `reference.hero` the instrument's `dry`
  word grades and no model sees, and the hand mutants each task tempts. **It lands at the
  milestone's freeze** (M-thesis-harness step 12; this page was written at step 4, 2026-10-20, and
  until the freeze the folder holds this page alone).
- `reserve.enc`: 20 more of the same shape, drawn in order when a task is burned or found
  ambiguous (`docs/metrics/tasks.md` § Adjudication).
- `suite.enc.sha256`, `reserve.enc.sha256`: the sha256 of each archive's text, checked before it is
  opened; AES-CBC does not authenticate, so an archive that moved is refused as moved.
- `hashes.txt`, `hashes-reserve.txt`: what `seal` prints for each archive's suite as sealed, its
  canaries written (a canary moves its task's digest, so never what `tasks` printed before), and
  what `tasks --sealed` prints again from the archive: one line per task
  (its digest over prose and cases, its prose digest, its number, its case count) and the suite's
  line, by task number and never by a telling name, so a measurement names the suite it ran and
  the CI checks the opened archive against it byte for byte. `traps.txt` lists by number the tasks
  that tempt a plausible mistake on purpose, since the results are reported stratified, and
  `constructs.txt` each task's construct classes by number, since they are reported per class and
  class-weighted; `report <out> --labels tests/thesis/held-out` reads both.
- `prediction.md`: the pre-registration, written in the freeze's commit before any model saw a
  task: the primary contrast, its three outcomes and the rule for the inconclusive one, the expected
  numbers, n, the adjudication, and the digests of the compiler, the template, the grader and the
  statistics code the run will use. The digests and the run's protocol stand in one fenced
  `preregistered` block, what `preregister <folder>` prints for the run's flags, which `report`
  sets against each run's `provenance.json` field by field, every difference a row of its
  deviations.
- `runs/`: the transcripts of the held-out runs, each sealed under its own key, since a transcript
  holds the task's prose.

## Why it is sealed, and what the seal is

The tasks are a test. A test a model has read in training measures memory, not the language: on
HumanEval, programs seen in training scored 42 points above unseen ones in one measurement the
sitting's historian verified (Riddell et al. 2024). This repository is public since 2026-09-08, so
a task committed in clear would enter the next crawl. The seal is `tar` of the task folder, then

    openssl enc -aes-256-cbc -pbkdf2 -iter 600000 -a -pass env:<VAR>

so the archive is base64 text the tree can hold and a key that is never on a command line, never
in a file of this repository and never in the instrument's own code: `-pass env:` has openssl read
it from the environment of the process that runs it. **Three archives, three keys**:
`HEROES_TASKS_KEY` for `suite.enc`, `HEROES_RESERVE_KEY` for `reserve.enc`, `HEROES_RUNS_KEY` for
`runs/`, so that one key given away burns one archive and not the others. Each key is 32 random
bytes the author generated and keeps in a password manager; a key that leaks burns what it covers,
for ever, and the README's register below says so the day it happens.

**What the seal does and does not hold against.** It holds against a crawler and against a model
without tools, and against a model with web search for exactly as long as the key is nowhere
public: the sealed BrowseComp set, whose key travelled with the data, was decrypted whole by a model
that found the evaluation's source code on GitHub (Anthropic, 2026-03-06, verified by the
sitting's historian). So the key is **not** committed until the opening below, and no model is ever
run against these tasks with web access.

## How to read it today

**Open it in one shell, grade it in another.** `heroes run tests/harness/thesis.hero -- unseal
--sealed <archive> --into build/opened`, the archive being `suite.enc` beside this page once the
freeze has written it, with the key in that shell's environment; then every `dry` or `run` over `build/opened` in a shell where no `HEROES_*_KEY` is
set, since the grader refuses to run a program while one is: inside the sandbox a program can
read another process's environment (the sitting's ffi-pragmatist measured it), so the key must be
in no process that runs one.

Ask the author for the key. The one condition is not to republish it: the moment the key is
public the archive is open to the next crawl. Every delivery is written in the register at the end
of this page, with the day and the recipient. **Nothing here is visible to a human and hidden from
a crawler**: there is no such route without a key, because what a person can read on the web
without one is copied and crawled like anything else, and this page says so rather than pretending
otherwise.

**The CI attests without showing.** The job `thesis-attest` in `.github/workflows/ci.yml` (it lands
with the milestone's step 4b, dated 2026-10-20, when this page was written) decrypts the archives
inside the runner, under the key held as a GitHub Actions secret in an environment with a required
reviewer, runs `tasks --sealed` against `hashes.txt`, the prose lint and `dry --mute` on the
reference solutions and the hand mutants, and prints **counts and digests alone**: how many tasks,
how many cases, which digests, and on a failure a task's digest and never its text. Anyone can read
the job's log and see that the archive holds what this page says, without seeing a task. The job is
skipped on a fork, where the secret does not exist.

**Corrected 2026-10-24** (defect 2197): the job's first run with the keys, after that day's held-out
run, stopped at the suite's first task, because `seal` had packed the archives with macOS's `tar`,
which writes an AppleDouble member `._<name>` before a file carrying an extended attribute; a Mac
never shows it, and tar on Linux unpacks it as a file. The archives are not sealed again, since
their digests are the pre-registration's: from that day `unseal` removes such a member where it
opens an archive, asked of the file's bytes and not of its name, and `seal` packs without them.
The change is to `tests/harness/thesis_seal.hero`, which is inside the digest a run records as
`instrument`, so that digest differs from the pre-registration's from that day, for this change
alone.

## When it opens

**The archive opens by itself**: within 30 days of the first `v*` release tagged after the first
held-out run, or of the suite's being declared burned by the canary rule below, whichever is first,
the author commits the three keys into this folder as `KEYS.md`, and the tasks become readable from
git's history by anyone. From that day the suite is a public benchmark like any other, burned by
construction and declared so, and a new series needs a new sealed suite. The trigger is mechanical
so that nobody has to decide to open it; the register below records the opening when it happens.

## The canary

`LABEL`'s second line carries a suite GUID and every `task.md`'s last line a per-task GUID
(`canary heroes-task:<32 hex digits>`), in the prompt a model receives. Before a run the instrument's
`canary-probe` asks each model to complete each prefix and records the answer. **A per-task GUID
completed exactly means that task was in the model's training data**: it is burned, replaced by the
next of the reserve in order, and the register says so. **The suite GUID completed exactly stops the
run** and the author reads: the suite GUID stands in this repository and may be learnt from it, as
BIG-bench's canary can, so its completion is a warning and the per-task one the finding. Under the
`none` context, where a model is given the task and no specification, a Heroes-only spelling in the
model's code (`@=`, `???`, `.must()`, `fail(code:`) is counted as knowledge of the language and
reported with the rule of three, not as a burned task: all four listed models declare a training
cutoff of June 2026, before this repository was public.

## Licence

Everything in the archives is under this repository's licence, `LICENSE` (Apache License 2.0) with
`LICENSE-RUNTIME-EXCEPTION`, the same as every file in clear. Sealing changes nothing of the
licence; it changes when the text is readable. The programs a model writes in answer to a task
belong to whoever the provider's terms say; the instrument records which provider and which plan
answered, and the measurement cites the terms it read.

## Register

One line per event: the day, what happened, and who was given a key or which task was burned.

- 2026-10-20: this page written, at M-thesis-harness step 4, no archive yet; the suite and the
  reserve land at the freeze.
- 2026-10-24: the freeze. The suite, 40 tasks (`c24a5b81...`), and the reserve, 20 (`b27ea1a9...`),
  sealed and committed with their digests, `traps.txt`, `constructs.txt` and `prediction.md`; no
  key given to anybody.
