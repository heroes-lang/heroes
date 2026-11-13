# Panel 247, llm-ergonomist: the blind seat, registered before any session

Written 2026-11-13 for the coordinator, who builds the folders, runs the sessions and grades them;
repaired the same night on the critic's first pass (`docs/panel/247-reports/critic-briefs.md`, items
12 to 17); **unrun** when written, and **awaiting the author's yes**: the sitting's one paid run, with
a **ceiling of 5 USD** for the whole seat if the author gives it (a paid run is the author's every
time, `.claude/rules/verification.md` § The batch). Without the yes the synthesis records the seat as
unrun and no sentence enters on its reading. Everything below the line *The seat's brief* is copied
verbatim into the session folders and carries none of the project's names; everything above it is for
the coordinator.

## What it measures

Whether a reader holding only the specification writes a program that stops cleanly on a signal, and
one that counts signals while it works, and which handler it writes: a function of the program given
to `signal`, a handler written in C in a header the program ships, or something else; with a
candidate sentence of `spec-warden.md` and without it. R4, ratified, says no Heroes code runs in a
handler (00-shared.md), so what a reader of today's spec writes is recorded beside each grade. Three
arms, each a whole specification, label-stripped (every session folder holds it as
`heroes-spec.md`):

| arm | the specification | hash (`shasum -a 256`, first 16) | the route its sentence describes |
|---|---|---|---|
| x | the base's, unchanged (`git -C .claude/worktrees/b34-round show ae5e2cedd:spec/heroes-spec.md`) | `41ad6690571bd27b` | none: today |
| r | x with **hrr** in place | `819cb24d7e9c0ec0` | C, the run-time refusal, and its remedy |
| e | x with **he** in place | `1868dce1e004ad46` | E (or F with E's delivery), the deferred handler |

**How each arm is made**: the sentence appended, with one leading space, after the one occurrence of
`` `atexit(f: (function() -> ()))`. `` (spec line 404), word for word as `spec-warden.md`'s table
gives it; the coordinator's copies are `.claude/worktrees/p247-scratch/price/spec-x.md`,
`spec-hrr.md` and `spec-he.md`, made by `p247-scratch/price/drafts.py`, which printed the hashes above
(re-run after the safe-point draft `hs` was added, every hash the same). An arm built again must hash
the same, or the difference is said before any session. **Not in the twelve**: arm a (**ha**, route
A's sentence, `7e1a25c601977d20`) and arm s (**hs**, route S's, `f116bb024d10085f`), the coordinator's
call; four more sessions at the cap raise the bound by caps alone past the ceiling, so adding one
means dropping the second replicate of an arm.

## The sessions, their folders, their order and their bound

**12 sessions**: t1 and t2 under x, r and e, two each. **The folders' names say nothing of the seat,
the task or the arm** (panel 246's open question, `docs/panel/246-*.md:515-517`: *"a name saying less
than `246-llm-ergonomist/` is a question for the next sheet"*): each session is a folder of six random hexadecimal digits under
**`.claude/worktrees/p247-scratch/s/`** (inside the repository's root and ignored by git, as panel 246
kept its own, CLAUDE.md § Hard stops), holding `brief.md`, `task.md` (the task's text below) and
`heroes-spec.md` (the arm's). **The map, the coordinator's alone**, made by
`p247-scratch/tools/sessions.py`, which wrote it to `.claude/worktrees/p247-scratch/s-map.txt`, never inside
a session's folder (each hash `shasum -a 256`, first 16; the t1 `task.md` and `brief.md` hashes are
the ones the critic read in the first folders, and t2's changed with its count, below):

| session | folder under `s/` | `heroes-spec.md` | `brief.md` | `task.md` |
|---|---|---|---|---|
| t1-x-1 | `be41ed` | `41ad6690571bd27b` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| t1-x-2 | `dfc1e6` | `41ad6690571bd27b` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| t1-r-1 | `0fcbe1` | `819cb24d7e9c0ec0` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| t1-r-2 | `97c336` | `819cb24d7e9c0ec0` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| t1-e-1 | `8e485e` | `1868dce1e004ad46` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| t1-e-2 | `fdde25` | `1868dce1e004ad46` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| t2-x-1 | `830aaf` | `41ad6690571bd27b` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| t2-x-2 | `16000e` | `41ad6690571bd27b` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| t2-r-1 | `cc68b7` | `819cb24d7e9c0ec0` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| t2-r-2 | `1f4aad` | `819cb24d7e9c0ec0` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| t2-e-1 | `2d4e11` | `1868dce1e004ad46` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| t2-e-2 | `8699fb` | `1868dce1e004ad46` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |

The folders are prepared and nothing has run in them. The first prepared folders, named
`p247-scratch/247-llm-ergonomist/<task>-<arm>-<n>/`, which the critic read, were removed once these
were made: they held the three files per session and nothing else (`find ... -type f | wc -l`: 36). **The order**, registered: first one of each,
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

**The bound.** Each session is capped at **0.40 USD**, so **the twelve are bounded at 4.80 USD by
their caps alone**, inside the 5. The runner also sums `total_cost_usd` over every `run.json` written
so far and starts the next session only while that sum is at most **3.60 USD**; that bounds when a
session starts, not what the last one costs, so the total is at most 3.60 plus the last session's
cost, which its cap holds to 0.40 plus whatever one request passes the cap by. **Panel 246's twenty
run files show no whole session above 0.3258 USD** (`p246-scratch/246-llm-ergonomist/*/run.json`,
read with `python3 -I`: 20 files, 3.8216 USD in all, the largest `t2-x-1` at 0.3257576 over 5
turns, the smallest 0.1418676), so no single request of that run cost more: at that size the total
stays at most 3.60 + 0.40 + 0.33 = 4.33 USD, inside 5. **A single request above 1.00 USD**
(5.00 − 3.60 − 0.40) **would break the 5 USD bound**: whether the CLI can pass its cap by that much is
a question, unrun. At 246's per-session figures twelve would cost about 1.70 to 3.91 USD
(`python3 -I -c 'print(12*0.1419, 12*0.3258)'`), an inference.

**The reports**: take `result` from `run.json` where `report.md` is missing, and say which. The
coordinator copies them into `docs/panel/247-reports/llm-ergonomist.md` under a header saying how
they were run, with each session's cost and every `context` answer; a `yes` voids that session until
it is re-run.

## Grading, registered before any session

Each answer's files are copied into a fresh folder under `p247-scratch/grade/` and built; every
binary is run under `timeout -s KILL 60` or the drivers below, never a plain `timeout`, since t1 asks
the program to catch SIGTERM and a plain `timeout` sends SIGTERM (panel 246 graded so,
`docs/panel/246-*.md:205-207`); its output kept to 65,536 bytes a stream. **Three grading runtimes**,
each named by `HEROES_RUNTIME` beside the stage 2 of the base with D applied (`sources
560c95206c908459`, 00-shared.md § D, the floor; copied and pinned; `heroes doctor` shows which
runtime it reads):

- **today**: the base's `runtime/` with D, as the round stands;
- **C**: that runtime with protoC's change applied (`diff -u` of
  `b34-scratch/b34-signals/protoC/runtime/parts/thread.c` and `os.c` against `git -C
  .claude/worktrees/b34-round archive 62ce17563 runtime`, applied with `patch`), its two files'
  sha256 recorded; or the compiler-engineer's thunk form if it has built one by then, said which;
- **E**: a seat's built deferring runtime if one exists by then; otherwise **graded by reading against
  this rubric and marked provisional**.

Every answer is graded under all three, every Darwin death read in its crash report
(`p247-scratch/tools/ips.py`, 00-shared.md), and its **handler** is recorded beside its class: a
function of the program given to `signal` (formatting, allocating or neither), a handler written in C
in a header the program ships, the platform layer's waiting calls, or none.

- **t1**, run twice with `python3 -I t3run.py <binary> SIGINT` and `... SIGTERM`
  (`b34-scratch/b34-signals/t3x1/t3run.py`, copied: the signal at 2.5 s, the exit waited 10 s, then
  SIGKILL to the group, each stream kept to 65,536 bytes): **right** if both runs print `step 1` to
  `step N` in order, then `stopped after N steps` with the same N, N at least 2, and exit 0 within 2 s
  of the signal; else **refused** (build exit 1), **stopped** (killed by the signal, or an error),
  **silent** (another output or exit code).
- **t2**, run three times with the shared storm driver, `python3 -I storm.py <binary> SIGUSR1 4 0.3`
  (`p247-scratch/tools/storm.py`, copied, run unwrapped): **right** if every run prints `built
  20000000 items` and then `received M signals` with M at least 1 and at most the count the driver
  says it sent, and exits 0; else the same three wrong classes.
  - **The reference, run before any session** (the critic's item 15): the coordinator's answer
    (`p247-scratch/coord/t2ref/`, a C handler in its own header counting into a `volatile
    sig_atomic_t`), built with the base's stage 2. Under the task's first count, one list of
    2,000,000 items, five runs of the grading command (`grade-2000000.log`) read M = 90,629, 0, 0, 0
    and 0, the driver sending 120,973, 0, 0, 4,925 and 0: **four of five right answers graded wrong**,
    the program done before or around the driver's 0.3 s arm (with no signal, `/usr/bin/time -p`, three
    runs each: real 0.36 to 0.37 s under the first count, 2.39 to 2.68 s under the second). So t2's count changed before any
    session, to twenty rounds of a fresh list of 1,000,000 (`main.hero` there, `t2ref20`), and five
    runs of the same command (`grade-20000000.log`) read M = 1,273,492, 1,201,389, 1,226,803,
    1,218,662 and 1,215,182, each at most what was sent (1,284,383, 1,220,366, 1,242,409, 1,229,841,
    1,229,071), every one `built 20000000 items` and exit 0: five of five right.
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

Write a program, `main.hero`, that does a long piece of work: twenty times over, it builds the texts
`item 1` to `item 1000000` into a fresh list, and when the twenty rounds are done it prints `built
20000000 items`. While it works, other processes may send it the signal SIGUSR1 (with `kill -USR1`),
as often and as fast as they like. The program must count the SIGUSR1 signals it receives while it
works and, after the line above, print `received M signals` with M that count. It must not crash or
stop early however the signals come, and it ends with exit code 0. Give every file, whole, with its
path, and say whether the compiler accepts it when built with `heroes build main.hero` and what it
prints.
```
