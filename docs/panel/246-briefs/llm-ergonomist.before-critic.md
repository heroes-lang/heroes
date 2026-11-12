# Panel 246, llm-ergonomist: the blind seat, registered before any session

Written 2026-11-12 for the coordinator, who builds the folders, runs the sessions and grades them;
**unrun** when written. The seat judges **Part B only** (Part A changes no spec token). It is the
sitting's one paid run, inside the author's ceiling of 10 USD for the whole sitting (*3a*,
00-shared.md). Everything below the line *The seat's brief* is copied verbatim into the session
folders and carries none of the project's names; everything above it is for the coordinator.

## What it measures

Whether a reader holding only the specification writes a correct thread program and a program that
stops cleanly, on the first try, with each candidate sentence and without it; and, for the stop,
which spelling a reader gets right. Six arms, each a whole specification, label-stripped (every
session folder holds it as `heroes-spec.md`):

| arm | the specification | for task |
|---|---|---|
| x | `git show b8c8240d4:spec/heroes-spec.md`, unchanged | t1, t2, t3 |
| y | x with **T1** in place | t1, t2 |
| z | x with **T2** in place | t1, t2 |
| p | x with **S1** in place | t3 |
| q | x with **S2** in place | t3 |
| r | x with **S3** in place | t3 |

T1, T2, S1 and S2 are inserted, each with one leading space, right after the one occurrence of
``threads, processes and files from the language's `hero_os.h`.`` (`spec/heroes-spec.md:377`); S3
replaces the one occurrence of `one written as a number is refused).` with `one written as a number
is refused)` followed by S3's text, which ends with its own `.`. The sentences are 00-shared.md's, word
for word. The coordinator's own drafts, made that way and priced by the spec-warden's table, hashed
with `shasum -a 256` (first 16 digits): x `fa765470cf65ac41` (equal to `git show
b8c8240d4:spec/heroes-spec.md | shasum -a 256`), y `5943625fca746d85`, z `9feb9c4cd1af64b9`, p
`e7f845cc476f8d0e`, q `5d739d1f1c7e57af`, r `7874df3e1b62d9ee`; an arm built again must hash the same
or the difference is said before any session.

## The sessions, their order and their bound

**20 sessions**: t1 and t2 under x, y and z, two each (12); t3 under x, p, q and r, two each (8). One
folder per session, `<task>-<arm>-<n>`, under **`.claude/worktrees/p246-scratch/246-llm-ergonomist/`**
(inside the repository's root and ignored by git, as panels 239 and 244 kept theirs, CLAUDE.md § Hard
stops; the coordinator's instruction named the session scratchpad, outside the root, and 00-shared.md
§ Where each seat works says why it is not used), each holding `brief.md`, `task.md` (the task's text
below) and `heroes-spec.md` (the arm's). **The order**, registered: first replicate `t1-x-1 t1-y-1
t1-z-1 t2-x-1 t2-y-1 t2-z-1 t3-x-1 t3-p-1 t3-q-1 t3-r-1`, then the second in reverse, `t3-r-2 ... t1-x-2`,
so a stop leaves every arm with one session.

**The command**, the `/panel` skill's with `--bare` (panels 239 and 244) and `--model`, run from each
session's folder, the key loaded from the trunk's `.env` and never printed (`${#ANTHROPIC_API_KEY}`
alone, to know it loaded), the environment holding only these:

```
env -i PATH="$PATH" HOME="$HOME" LANG=en_US.UTF-8 \
    GIT_CEILING_DIRECTORIES="$(cd .. && pwd)" ANTHROPIC_API_KEY="$ANTHROPIC_API_KEY" \
  claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
    --bare --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5 \
    --tools "Read,Write" --allowedTools "Read,Write" \
    --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
    --max-budget-usd 3 --output-format json > run.json 2> run.err
```

`claude --version` read `2.1.311 (Claude Code)` while this was written, and `claude --help` lists
every flag above; panel 239 recorded that version accepting `claude-opus-5-5` (`grep -n 'Model'
docs/panel/239-reports/llm-ergonomist-blind.md`).

**The bound.** Each session is capped at 3 USD by `--max-budget-usd 3`, so 20 sessions are not
bounded by their caps alone. The runner sums `total_cost_usd` over every `run.json` written so far and
**starts the next session only while that sum is at most 6.50 USD**: a session then starting at its
cap ends near 9.50, and the last 0.50 is room for one request past the cap, which the CLI may or may
not allow (a question, unrun). At that guard at least three sessions run whatever they cost. What
earlier blind seats cost, from their records and not measured here: panel 244's eight sessions 1.0231
USD in all, 0.1209 to 0.1347 each (`grep -n 'What it cost' docs/panel/244-reports/llm-ergonomist.md`),
panel 239's twelve 1.5671 USD, 0.1158 to 0.1887 each (`grep -n '1.5671'
docs/panel/239-reports/llm-ergonomist-blind.md`); at those figures twenty sessions would cost about 2.4
to 3.8 USD, an inference. No other paid run is named for this sitting; a `--refresh` the spec-warden
asks for comes out of the same 10 USD and is the author's to allow.

**The reports**: at panel 244 every session gave its report as its final message rather than writing
`report.md` (`grep -n 'Where the reports came from' docs/panel/244-reports/llm-ergonomist.md`); take
`result` from `run.json` where `report.md` is missing, and say which. The coordinator copies them into
`docs/panel/246-reports/llm-ergonomist.md` under a header saying how they were run, with each
session's cost and every `context` answer; a `yes` voids that session until it is re-run.

## Grading, registered before any session

Each answer's files are copied into a fresh folder under `246-llm-ergonomist/grade/` and built; a
program that may not end is run as its binary under `timeout`, its output through `head -c 65536`.

- **t1** (expected: `170666346666800000` on both lines, `python3 -I -c` over the task's sum):
  **right** if it builds at exit 0, starts at least two threads that run at once, and prints the
  number twice; else **refused** (build exit 1), **stopped** (built, ends with an error when run, for
  instance a thread the runtime did not start), **silent** (built, prints something else) or **no
  threads** (the right lines on one thread, which the task forbids). Grading compiler: stage 2 at
  `b8c8240d4`, `sources 017930ae6061c8b4`, copied and pinned with its own `runtime/` named by
  `HEROES_RUNTIME` (panel 239's note: a copied compiler otherwise reads the trunk's).
- **t2** (expected lines `4`, `2`, `6`, `2`, `14`): **right** if it builds at exit 0, counts each
  sentence on its own thread, and prints those five lines; the same four wrong classes. Same compiler.
  Recorded beside the class, for B3: where the program keeps the sentences (a constant, rebuilt inside
  the thread, or anything else) and what crosses into each thread.
- **t3**: the binary started, `kill -INT <pid>` at 2.5 s, then the same with `kill -TERM`. **Right**
  if it prints `tick 1` to `tick N` in order, then `stopped after N ticks` with the same N, N at least
  2, and exits 0 within 2 s of the signal; **refused**, **stopped** (killed by the signal, or an
  error), **silent** (another output or exit code). Graded with a compiler that holds the arm's exact
  spelling: S1 and S2 in a seat's runtime (the compiler-engineer's brief asks for both), S3 where a
  seat built the built-in, x with the stage 2 of the base. **An arm no seat built is graded by reading
  against this rubric and marked provisional.**
- **The decision rule**: for B1, T1 or T2 enters only if its arm gives at least two more `right`
  answers than x over t1 and t2 together (four sessions each); for B2, the spelling with the most
  `right` answers of its two t3 sessions is the blind seat's reading, a tie said as a tie, and arm x's
  two answers are recorded as what a reader writes today.

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

Write a program, `main.hero`, that computes the sum of `i * i` for every whole number `i` from 0 up
to and not including 800,000, in two ways, and prints each result on a line of its own:

- first, split the numbers into eight bands of 100,000 consecutive numbers, compute each band's sum
  on a thread of its own, the eight threads running at the same time, and add the eight results;
- then compute the same sum on the program's own thread alone.

Both lines must show the same number. Give every file, whole, with its path, and say whether the
compiler accepts the program when built with `heroes build main.hero` and what it prints.
```

**t2**

```markdown
# Task

Write a program, `main.hero`, that holds these four sentences, in this order:

- `the quick brown fox`
- `jumps over`
- `the lazy dog sleeps all day`
- `and wakes`

and counts the words of each sentence (a word is a run of characters between single spaces) on a
thread of its own, the four threads running at the same time. It prints the four counts, one per
line, in the order of the sentences, then their sum on a line of its own. Give every file, whole,
with its path, and say whether the compiler accepts it when built with `heroes build main.hero` and
what it prints.
```

**t3**

```markdown
# Task

Write a program, `main.hero`, that runs a loop of steps for as long as it is left running: each step
waits one second, by calling C's `sleep` from `unistd.h`, and then prints `tick N`, N counting the
steps from 1. When the program is asked to stop, by Ctrl-C at the terminal or by `kill -INT` or
`kill -TERM` from another process, it must not end in the middle of a step: it finishes the step in
hand, prints `stopped after N ticks` with N the number of ticks it printed, and ends with exit code 0.
Give every file, whole, with its path, and say whether the compiler accepts it when built with
`heroes build main.hero` and what it prints when sent `kill -INT` about 2.5 seconds after it starts.
```
