# Writing a held-out task

The order of work for whoever writes the sealed suite's tasks (40, and a reserve of 20), from an
empty folder to the archive the freeze commits. **What a task must be is `docs/metrics/tasks.md`**,
the rules' one home; this page is the workflow and cites them. The instrument is
`tests/harness/thesis.hero`, run from the repository's root as `./heroes-next run
tests/harness/thesis.hero -- <word> ...` (`heroes-next` being the stage-2 compiler of CLAUDE.md
§ Commands); the kit's two words, `scaffold` and `lint`, are `tests/harness/thesis_kit.hero`
(issue 2156, 2026-10-21).

## The one rule that comes before the others

**No model reads a held-out task**: no chat, no editor's assistant, no review bot, and not this
project's assistant. A task a model has read is held out no longer, and a transport whose terms do
not document that prompts are not trained on is barred to these tasks by the protocol
(`docs/roadmap/milestones/M-thesis-harness.md`). The runs send them to models through the APIs; a
person writing them shows them to none.

What holds it mechanically, inside this repository: the folders `held-out-draft/` (where the tasks
are written) and `build/opened/` (where an archive is opened) are ignored by git; a Claude Code
session here is denied both by `.claude/settings.json` for its file tools, and refused by
`.claude/hooks/guard_bash.py` for any shell command naming them, save the instrument's words that
print no word of a task (below). **Nothing holds a paste**, nor a tool outside this repository's
settings, which is why this section is first.

## Where the tasks are written

At the repository's root, never outside it:

    held-out-draft/suite/      LABEL (held-out), 01 to 40, traps.txt, constructs.txt
    held-out-draft/reserve/    LABEL (held-out), 41 to 60, traps.txt, constructs.txt

The reserve numbered **41 to 60**, so that a number names one task for ever: a reserve task drawn
to replace a burned one keeps its number in every report (`docs/metrics/tasks.md`
§ Adjudication). The folder is the only copy in clear until the freeze seals it; keep a backup of
it where you keep your other private files, never in a repository and never in a shared drive a
model indexes.

## One task, from skeleton to green

1. **The skeleton**: `scaffold held-out-draft/suite 07` writes `07/` with a `task.md` to replace,
   `cases/0.args` and `cases/0.expected` empty, a `reference.hero` that is a hole, and
   `mutants/trap.hero`, whose three opening lines say what a trap must declare. It makes the folder
   and its `LABEL` the first time, and never writes over a task that exists.
2. **The prose**, `task.md`: English, naming no construct, every choice point pinned
   (`docs/metrics/tasks.md` § The rules a task is written by), ending with case 0, the visible
   example, its output shown exactly as `cases/0.expected` holds it.
3. **The cases**: `cases/K.args` one argument a line, `{dir}` standing for the folder a case's
   files are copied into, `cases/K.files/` at most three input files, `cases/K.expected` the stdout
   then `!exit: N` or `!panic: <text>` where the program does not end at 0; four to eight beside
   case 0, one empty, one malformed with its exit code, one large.
4. **The reference**, `reference.hero`: 25 to 80 lines of code, accepted by `check`.
5. **The mutants**, `mutants/*.hero`: the reference with one mistake made, one file a mistake,
   opening `# mistake:`, `# strict:` and `# control:` (`docs/metrics/tasks.md`, the hand mutants).
   **At least one is the trap** every held-out task carries (decision 2172): `# strict:
   compile_error <code>` and `# control: compiled_wrong case K`, `K` a hidden case and never `0`.
   A file's name may say its mistake: no word that can be shown prints it.
6. **The two lists**, one line a task: the number in `traps.txt` (every held-out task is a trap),
   and in `constructs.txt` the number then its classes among `record`, `variant`, `propagation`,
   `at-parameter`, `map` and `function-value`.
7. **`lint held-out-draft/suite`** reads the rules no other word checks and prints, per task,
   `NN holds every rule lint checks` or one line a rule broken, then the suite's lines and
   `lint: N tasks, label held-out, F findings`; exit 0 at no finding, 1 otherwise. The spread is
   the page's numbers for forty scaled to the folder's count and rounded up, so a suite half
   written knows where it stands and the reserve keeps the suite's proportions.
8. **`dry held-out-draft/suite`**, then again with `--arm control`: every reference passes every
   case and every mutant ends as its opening lines say, in that arm. In your own terminal run it
   without `--mute`, which shows a failure's detail; anywhere a model might read the output, with
   `--mute`, which prints a task's digest and its outcome alone. **No `HEROES_*_KEY` may be set in
   the shell that runs `dry`**: the grader refuses to run a program while one is, since a program
   inside the sandbox can read another process's environment (`tests/thesis/held-out/README.md`).

`lint` and `dry --mute` print task numbers, digests, rules and counts and never a line of a task,
a case, a reference or a mutant's name, so either can be shown to anyone, an assistant included,
to ask what a finding means. `tasks`, `seal`, `canary`, `scaffold` and `unseal` print nothing of a
task either; every other word and command reads it.

## The suite, before the freeze

- **`lint` at exit 0 on both folders**, `dry` green in the strict and the control arm.
- **The difficulty target** (`docs/metrics/tasks.md`): judged by the references' shape and the
  trial's rates, never by running a model on a held-out task.
- **The blind second reader**: a person given `task.md` and the cases alone, not the reference, the
  mutants or the classes, who says what the prose leaves open; each finding pinned by a sentence or
  covered by a case, then `lint` and `dry` again.

## The freeze

1. **The keys**, once: two values of 32 random bytes, `HEROES_TASKS_KEY` for the suite and
   `HEROES_RESERVE_KEY` for the reserve, made in your own terminal (`openssl rand -hex 32`), kept
   in your password manager and in `.env`, which git ignores. Never on a command line, never in a
   file of the repository, never pasted anywhere a model reads.
2. **The seal**, in a shell where the key is set, writing the two archives the freeze commits
   (neither exists on 2026-10-21, the day this page was written):

       ./heroes-next run tests/harness/thesis.hero -- seal held-out-draft/suite -o tests/thesis/held-out/suite.enc --key HEROES_TASKS_KEY  # none until the freeze, 2026-10-21
       ./heroes-next run tests/harness/thesis.hero -- seal held-out-draft/reserve -o tests/thesis/held-out/reserve.enc --key HEROES_RESERVE_KEY  # none until the freeze, 2026-10-21

   Each writes the canaries where they are missing (a canary moves a task's digest), seals, and
   prints the suite's digests as sealed: those lines are `hashes.txt` and `hashes-reserve.txt`,
   never what `tasks` printed before the canaries were in.
3. **The check**, none before the freeze (2026-10-21): `unseal --sealed tests/thesis/held-out/suite.enc
   --into build/opened` in the keyed shell, then in a shell with no key, `tasks build/opened` printing the same lines as
   `hashes.txt` and `dry build/opened --mute` green; the same for the reserve.
4. **The commit**: the archives, their `.sha256`, the hash files, `traps.txt`, `constructs.txt` and
   `prediction.md`, in the freeze's commit (M-thesis-harness step 12), the draft folder never;
   `traps.txt` and `constructs.txt` name tasks by number and say nothing a model could use.

## Afterwards

The draft folder stays on your machine, ignored, until the archive opens
(`tests/thesis/held-out/README.md` § When it opens); a key given to anybody is written in that
README's register the day it is given. A task found ambiguous after a run is replaced from the
reserve by number, never edited in place: an edit is a new task and a new digest.
