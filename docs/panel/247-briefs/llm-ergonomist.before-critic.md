# Panel 247, llm-ergonomist: the blind seat, registered before any session

Written 2026-11-13 for the coordinator, who builds the folders, runs the sessions and grades them;
**unrun** when written, and **awaiting the author's yes**: the sitting's one paid run, with a
**ceiling of 5 USD** for the whole seat if the author gives it (a paid run is the author's every time,
`.claude/rules/verification.md` § The batch). Without the yes the synthesis records the seat as unrun
and no sentence enters on its reading. Everything below the line *The seat's brief* is copied verbatim
into the session folders and carries none of the project's names; everything above it is for the
coordinator.

## What it measures

Whether a reader holding only the specification writes a program that stops cleanly on a signal, and
one that counts signals while it works, and which handler it writes: a function of the program given
to `signal`, a handler written in C in a header the program ships, or something else; with each
candidate sentence of `spec-warden.md` and without it. Three arms, each a whole specification,
label-stripped (every session folder holds it as `heroes-spec.md`):

| arm | the specification | hash (`shasum -a 256`, first 16) | the route its sentence describes |
|---|---|---|---|
| x | the base's, unchanged (`git -C .claude/worktrees/b34-round show ae5e2cedd:spec/heroes-spec.md`) | `41ad6690571bd27b` | none: today |
| r | x with **hrr** in place | `819cb24d7e9c0ec0` | C, the run-time refusal, and its remedy |
| e | x with **he** in place | `1868dce1e004ad46` | E or F, the deferred handler |

**How each arm is made**: the sentence appended, with one leading space, after the one occurrence of
`` `atexit(f: (function() -> ()))`. `` (spec line 404), word for word as `spec-warden.md`'s table
gives it; the coordinator's copies are `.claude/worktrees/p247-scratch/price/spec-x.md`,
`spec-hrr.md` and `spec-he.md`, made by `p247-scratch/price/drafts.py`, which printed the hashes above.
An arm built again must hash the same, or the difference is said before any session. **Not in the
twelve**: arm a (**ha**, route A's sentence, `7e1a25c601977d20`), the coordinator's call after the
critic's first pass; four more sessions at the cap raise the bound by caps alone past the ceiling,
so adding it means dropping the second replicate of an arm.

## The sessions, their order and their bound

**12 sessions**: t1 and t2 under x, r and e, two each. One folder per session, `<task>-<arm>-<n>`,
under **`.claude/worktrees/p247-scratch/247-llm-ergonomist/`** (inside the repository's root and
ignored by git, as panel 246 kept its own, CLAUDE.md § Hard stops), each holding `brief.md`,
`task.md` (the task's text below) and `heroes-spec.md` (the arm's); the twelve folders are prepared,
and nothing has run in them. **The order**, registered: first one of each,
`t1-x-1 t1-r-1 t1-e-1 t2-x-1 t2-r-1 t2-e-1`, then the second in reverse, `t2-e-2 t2-r-2 t2-x-2
t1-e-2 t1-r-2 t1-x-2`, so a stop leaves every arm with one session.

**The command**, panel 246's (`docs/panel/246-briefs/llm-ergonomist.md`), run from each session's
folder, the key loaded from the trunk's `.env` and never printed (`${#ANTHROPIC_API_KEY}` alone, to
know it loaded), the environment holding only these:

```
env -i PATH="$PATH" HOME="$HOME" LANG=en_US.UTF-8 \
    GIT_CEILING_DIRECTORIES="$(cd .. && pwd)" ANTHROPIC_API_KEY="$ANTHROPIC_API_KEY" \
  claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
    --bare --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5 \
    --tools "Read,Write" --allowedTools "Read,Write" \
    --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
    --max-budget-usd 0.40 --output-format json > run.json 2> run.err
```

`claude --version` read `2.1.311 (Claude Code)` at 03:49, and `claude --help` lists every flag above
(`--bare`, `--restricted`, `--safe-mode`, `--strict-mcp-config`, `--model`, `--tools`,
`--allowedTools`, `--disallowedTools`, `--max-budget-usd`, `--output-format`).

**The bound, twice.** Each session is capped at **0.40 USD**, so **the twelve are bounded at 4.80 USD
by their caps alone**, inside the 5. The runner also sums `total_cost_usd` over every `run.json`
written so far and starts the next session only while that sum is at most **3.60 USD**, a second
bound that holds whether or not the CLI can pass its cap by one request (a question, unrun). What the
last blind seat cost, from its record and not measured here: panel 246's twenty sessions, 3.8216 USD
in all, 0.1419 to 0.3258 a session, every one `context: no` (`docs/panel/246-*.md:553-555`); at those
figures twelve would cost about 1.7 to 3.9 USD, an inference.

**The reports**: take `result` from `run.json` where `report.md` is missing, and say which. The
coordinator copies them into `docs/panel/247-reports/llm-ergonomist.md` under a header saying how
they were run, with each session's cost and every `context` answer; a `yes` voids that session until
it is re-run.

## Grading, registered before any session

Each answer's files are copied into a fresh folder under `247-llm-ergonomist/grade/` and built; every
binary is run under `timeout 60`, its output through `head -c 65536`. **Three grading runtimes**, each
named by `HEROES_RUNTIME` beside the base's stage 2 (`sources 7cc70d136b590d46`, copied and pinned;
`heroes doctor` shows which runtime it reads):

- **today**: the base's own `runtime/`;
- **C**: a copy of the base's `runtime/` with protoC's change applied (`diff -u` of
  `b34-scratch/b34-signals/protoC/runtime/parts/thread.c` and `os.c` against `git -C
  .claude/worktrees/b34-round archive 62ce17563 runtime`, applied with `patch`), its two files' sha256
  recorded; or the compiler-engineer's thunk form if it has built one by then, said which;
- **E**: a seat's built deferring runtime if one exists by then; otherwise **graded by reading against
  this rubric and marked provisional**.

Every answer is graded under all three, and its **handler** is recorded beside its class: a function
of the program given to `signal` (allocating or not), a handler written in C in a header the program
ships, the platform layer's waiting calls, or none.

- **t1** (sent `kill -INT` at 2.5 s; then a fresh run sent `kill -TERM` at 2.5 s): **right** if it
  prints `step 1` to `step N` in order, then `stopped after N steps` with the same N, N at least 2,
  and exits 0 within 2 s of the signal, in both runs; else **refused** (build exit 1), **stopped**
  (killed by the signal, or an error), **silent** (another output or exit code).
- **t2** (run three times under the lane's storm driver, `b34-scratch/b34-signals/r2494/storm.py`
  copied, `python3 -I storm.py <binary> 4 0.3`): **right** if every run prints `built 2000000 items`
  and then `received M signals` with M at least 1 and at most the count the driver says it sent, and
  exits 0; else the same three wrong classes.
- **The decision rule**: an arm's sentence enters only if its arm gives at least two more `right`
  answers than arm x over its four sessions (t1 and t2 together), both graded under the route the
  sentence describes (r under **C**, e under **E**). Arm x's four answers are recorded under all three
  runtimes as what a reader writes today and what each route does to it.

---

## The seat's brief (`brief.md`, verbatim)

```markdown
# Your brief

You judge one thing: whether the language's specification, read with no other file, lets a language
model write the program its author meant on the first try. Your verdict is an experiment, not an
opinion: **write the code, don't opine.**

## Your inputs, and nothing else

The files in this directory are your whole input. Read no other file, and do not ask for one.

- `heroes-spec.md`: the language's specification, the whole language.
- `task.md`: one task.

## Your method

Read the task and do what it asks. Write every file you write whole, with its path. For every place
where the specification left you a choice, say the choice you made and what the other would produce:
a program the compiler refuses, a program that stops with an error when run, or a program that runs
and gives a different answer than intended.

## Your report, `report.md` in this directory, in exactly this structure

- `answer`: what the task asks, and every file, whole, each under its path.
- `choice_points`: every place the specification left you a choice, the choice you made, and what
  the other would produce.
- `prediction`: what each program prints when built and run, and whether you expect the compiler to
  accept it.
- `context`: whether any instruction, rule, contract, memory or document other than the files in
  this directory reached your context, and which.

If you cannot write `report.md`, give the whole report as your final answer instead.
```

## The tasks (`task.md`, verbatim, one per session)

**t1**

```markdown
# Task

Write a program, `main.hero`, that runs a loop of steps for as long as it is left running: each step
builds the text `step N`, N counting the steps from 1, adds it to a list of the texts built so far,
waits one second by calling C's `sleep` from `unistd.h`, and prints the text. When the program is
asked to stop, by Ctrl-C at the terminal or by `kill -INT` or `kill -TERM` from another process, it
must not end in the middle of a step: it finishes the step in hand, prints `stopped after N steps`
with N the number of steps it printed, and ends with exit code 0. Give every file, whole, with its
path, and say whether the compiler accepts it when built with `heroes build main.hero` and what it
prints when sent `kill -INT` about 2.5 seconds after it starts.
```

**t2**

```markdown
# Task

Write a program, `main.hero`, that builds the texts `item 1` to `item 2000000` into a list and then
prints `built 2000000 items`. While it works, other processes may send it the signal SIGUSR1 (with
`kill -USR1`), as often and as fast as they like. The program must count the SIGUSR1 signals it
receives while it works and, after the line above, print `received M signals` with M that count. It
must not crash or stop early however the signals come, and it ends with exit code 0. Give every file,
whole, with its path, and say whether the compiler accepts it when built with `heroes build
main.hero` and what it prints.
```
