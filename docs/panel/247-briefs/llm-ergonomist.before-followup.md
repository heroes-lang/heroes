# Panel 247, llm-ergonomist: the blind seat, registered before any session

Written 2026-11-13 for the coordinator, who builds the folders, runs the sessions and grades them;
repaired the same night on the critic's first pass (`docs/panel/247-reports/critic-briefs.md`, items
12 to 17); **extended at about 10:25 by the coordinator's decision of that hour**, on the author's
answer to the paid run, *"può spendere fino a 25"* (meant as: *it may spend up to 25*, USD), given in
the coordinator's conversation. The text before the extension stands beside this file as
`llm-ergonomist.before-extension.md`. **Every change below was written before any session ran**; the
sitting's one paid run, inside **the author's ceiling of 25 USD**. Everything below the line *The
seat's brief* is copied verbatim into the session folders and carries none of the project's names;
everything above it is for the coordinator.

## What it measures

Whether a reader holding only the specification writes a program that stops cleanly on a signal, and
one that counts signals while it works, and which handler it writes: a function of the program given
to `signal`, a handler written in C in a header the program ships, or something else; with a
candidate sentence and without it. R4, ratified, says no Heroes code runs in a handler (00-shared.md),
so what a reader of today's spec writes is recorded beside each grade. **Five arms**, each a whole
specification, label-stripped (every session folder holds it as `heroes-spec.md`):

| arm | the specification | hash (`shasum -a 256`, first 16) | vendored (legacy / cl100k / max) | the route its sentence describes |
|---|---|---|---|---|
| x | the base's, unchanged (`git -C .claude/worktrees/b34-round show ae5e2cedd:spec/heroes-spec.md`) | `41ad6690571bd27b` | 8340 / 8490 / 8490 | none: today |
| r | x with **hrr** appended | `819cb24d7e9c0ec0` | 8383 / 8533 / 8533 (+43) | C, the run-time refusal, and its remedy |
| e | x with **he** appended | `1868dce1e004ad46` | 8365 / 8515 / 8515 (+25) | E, the deferred handler on the runtime's thread |
| m | x with **hm6** merged | `3335350f71b16ce0` | 8349 / 8499 / 8499 (+9) | the rule itself, held by a refusing route (C) |
| s | x with **hs** appended | `f116bb024d10085f` | 8375 / 8525 / 8525 (+35) | S, the deferred handler at a safe point |

**How each arm is made**: r, e and s append their sentence, with one leading space, after the one
occurrence of `` `atexit(f: (function() -> ()))`. `` (spec line 404), word for word as
`spec-warden.md`'s table gives it, by `.claude/worktrees/p247-scratch/price/drafts.py`. **m is the
spec-warden's counter-proposal hm6** (`docs/panel/247-reports/spec-warden.md`, its table and § hm6):
the one occurrence of *"A callback is a **parameter**, never a result; its parameters follow the same
rule"* (spec line 403) becomes *"A callback is a **parameter**, never a result or a signal handler,
nor called by one; its parameters follow the same rule"*. Made by the coordinator from the base's spec
with that one replacement asserted to occur once (`python3 -I`, into `p247-scratch/price/spec-hm6.md`),
it is byte for byte the spec-warden's `p247-scratch/247-spec-warden/drafts/spec-hm6.md` (`cmp`: equal)
and priced by the base's stage 2 (`./heroes-next measure .../spec-hm6.md` from
`p247-scratch/base/tree`: claude-legacy 8349, cl100k_base 8499, maximum 8499). An arm built again
must hash the same, or the difference is said before any session.

**Arm a (route A's sentence, `7e1a25c601977d20`) stays out**: route A was vetoed alone by the
compiler-engineer (`247-reports/compiler-engineer.md:432`, *"**A alone** | **veto** | `wraplocale`
passes A's `check` (exit 0) and dies 5 of 5 in silence under what A leaves, D"*), objected to as the
only enforcement by the historian (`historian.md:79`, *"object as the only enforcement: the
precedents' own hole is `wrap`"*), and refused for hm6 by the spec-warden (`spec-warden.md:305`,
*"ha | no, hm6 instead"*), so no resolution is left for its sentence to describe.

## The sessions, their folders, their order and their bound

**40 sessions**: t1 and t2 under each of the five arms, four each. **The folders' names say nothing
of the seat, the task or the arm** (panel 246's open question, `docs/panel/246-*.md:515-517`: *"a
name saying less than `246-llm-ergonomist/` is a question for the next sheet"*): each session is a
folder of six random hexadecimal digits under **`.claude/worktrees/p247-scratch/s/`** (inside the
repository's root and ignored by git, CLAUDE.md § Hard stops), holding `brief.md`, `task.md` and
`heroes-spec.md` (the arm's), every arm's hash checked in every folder by the script that made them.
**The map, the coordinator's alone**, made by `p247-scratch/tools/sessions.py`, which wrote it to
`.claude/worktrees/p247-scratch/s-map.txt` and the order to `s-order.txt`, never inside a session's
folder (each hash `shasum -a 256`, first 16):

| order | session | folder under `s/` | `heroes-spec.md` | `brief.md` | `task.md` |
|---|---|---|---|---|---|
| 1 | t1-x-1 | `3830e4` | `41ad6690571bd27b` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 2 | t1-r-1 | `98c872` | `819cb24d7e9c0ec0` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 3 | t1-e-1 | `707099` | `1868dce1e004ad46` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 4 | t1-m-1 | `fcce6e` | `3335350f71b16ce0` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 5 | t1-s-1 | `14aeb3` | `f116bb024d10085f` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 6 | t2-x-1 | `3b5cc9` | `41ad6690571bd27b` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 7 | t2-r-1 | `304e74` | `819cb24d7e9c0ec0` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 8 | t2-e-1 | `ba6229` | `1868dce1e004ad46` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 9 | t2-m-1 | `a19d3a` | `3335350f71b16ce0` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 10 | t2-s-1 | `8b4305` | `f116bb024d10085f` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 11 | t2-s-2 | `47763b` | `f116bb024d10085f` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 12 | t2-m-2 | `36e5b3` | `3335350f71b16ce0` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 13 | t2-e-2 | `daaef5` | `1868dce1e004ad46` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 14 | t2-r-2 | `e46eba` | `819cb24d7e9c0ec0` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 15 | t2-x-2 | `e63d43` | `41ad6690571bd27b` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 16 | t1-s-2 | `f302f2` | `f116bb024d10085f` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 17 | t1-m-2 | `3b3338` | `3335350f71b16ce0` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 18 | t1-e-2 | `597f7e` | `1868dce1e004ad46` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 19 | t1-r-2 | `a4d252` | `819cb24d7e9c0ec0` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 20 | t1-x-2 | `b91072` | `41ad6690571bd27b` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 21 | t1-x-3 | `38180e` | `41ad6690571bd27b` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 22 | t1-r-3 | `2e62c9` | `819cb24d7e9c0ec0` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 23 | t1-e-3 | `87efae` | `1868dce1e004ad46` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 24 | t1-m-3 | `8be6f8` | `3335350f71b16ce0` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 25 | t1-s-3 | `2dde04` | `f116bb024d10085f` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 26 | t2-x-3 | `1bf6d1` | `41ad6690571bd27b` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 27 | t2-r-3 | `056a0c` | `819cb24d7e9c0ec0` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 28 | t2-e-3 | `0d036a` | `1868dce1e004ad46` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 29 | t2-m-3 | `ff7f62` | `3335350f71b16ce0` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 30 | t2-s-3 | `5c1359` | `f116bb024d10085f` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 31 | t2-s-4 | `86745c` | `f116bb024d10085f` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 32 | t2-m-4 | `41fe36` | `3335350f71b16ce0` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 33 | t2-e-4 | `313ff5` | `1868dce1e004ad46` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 34 | t2-r-4 | `ec3f30` | `819cb24d7e9c0ec0` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 35 | t2-x-4 | `d9a2db` | `41ad6690571bd27b` | `1d0b6a53cddcb0a0` | `a6fc886f62c47711` |
| 36 | t1-s-4 | `3fb6c6` | `f116bb024d10085f` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 37 | t1-m-4 | `51d552` | `3335350f71b16ce0` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 38 | t1-e-4 | `027658` | `1868dce1e004ad46` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 39 | t1-r-4 | `86b0cd` | `819cb24d7e9c0ec0` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |
| 40 | t1-x-4 | `216c39` | `41ad6690571bd27b` | `1d0b6a53cddcb0a0` | `0fd42e1c700f113f` |

The twelve folders of the registration before the extension, which nothing had run in (36 files, the
three per session), were removed before these were made. **The order**, registered: four rounds over
the ten cells, round 1 in the order `t1-x t1-r t1-e t1-m t1-s t2-x t2-r t2-e t2-m t2-s` with replicate
1, round 2 the same cells in reverse with replicate 2, round 3 forward with 3, round 4 reverse with 4,
so a stop leaves every cell represented; **four sessions run at a time**, each started in that order
as a slot frees (`xargs -P 4` over `s-order.txt`).

**The command**, panel 246's (`docs/panel/246-briefs/llm-ergonomist.md`), run from each session's
folder by `p247-scratch/tools/blind-one.sh`, the environment holding only these:

```
env -i PATH="$PATH" HOME="$HOME" LANG=en_US.UTF-8 \
    GIT_CEILING_DIRECTORIES="$(cd .. && pwd)" ANTHROPIC_API_KEY="$ANTHROPIC_API_KEY" \
  claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
    --bare --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5 \
    --tools "Read,Write" --allowedTools "Read,Write" \
    --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
    --max-budget-usd 0.40 --output-format json > run.json 2> run.err
```

**The key**: loaded in the runner's own shell by `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env;
set +a`, its length alone printed (`${#ANTHROPIC_API_KEY}`), never its value, and passed to each
session through `env -i` as above.

**The bound.** Each session is capped at **0.40 USD**, so **the forty are bounded at 16.00 USD by
their caps alone**, inside the 25. The runner also sums `total_cost_usd` over every `run.json` written
so far and starts a session only while that sum is at most **22.00 USD**; with four in flight, the
total is at most 22.00 plus four sessions' costs, each held by its cap to 0.40 plus whatever one
request passes the cap by: 23.60 USD before any overrun, and inside 25 while no session passes its cap
by more than 0.35 USD. Panel 246's twenty run files show no whole session above 0.3258 USD
(`p246-scratch/246-llm-ergonomist/*/run.json`), an inference about these, not a measurement; whether
the CLI can pass its cap by a request is a question, unrun. At 246's per-session figures forty would
cost about 5.67 to 13.03 USD (`python3 -I -c 'print(40*0.1418676, 40*0.3257576)'`).

**The reports**: take `result` from `run.json` where `report.md` is missing, and say which (every
session of panel 246 gave its report so, its runner's log reading `report.md no` twenty times). The
coordinator copies them into `docs/panel/247-reports/llm-ergonomist.md` under a header saying how
they were run, with each session's cost and every `context` answer; a `yes` voids that session until
it is re-run.

## Grading, registered before any session

Each answer's files are written from its report into a fresh folder under `p247-scratch/grade/<folder>/`
and built three times; every binary is run under the drivers below, never a plain `timeout`, since t1
asks the program to catch SIGTERM and a plain `timeout` sends SIGTERM (panel 246 graded so,
`docs/panel/246-*.md:205-207`); each stream kept to 65,536 bytes. **Three grading setups**, each a
compiler and a runtime copied into `p247-scratch/grade/` and pinned by `HEROES_RUNTIME` (`heroes
doctor` names each copy's own runtime, read for all three):

| setup | compiler (sha256, 16) | `--version` | runtime (sha256 of its files' sorted sha256 list, 16) | what it is |
|---|---|---|---|---|
| **today** | `grade/today/heroes`, the base's stage 2 with D (`c8a7586f07ffccac`) | `sources 560c95206c908459` | `grade/today/runtime` (`a4c5b1a7d7d3200d`) | the round as it stands: D's guard |
| **C** | `grade/c/heroes`, the compiler-engineer's thunk form, its `tree/heroes-c2` (`c06b1956714cebef`) | `sources 1f83fdf86f0d2855` | `grade/c/runtime`, its `tree/runtime` (`65c2849ff4b253cf`; `parts/thread.c` `80e8f1bb64fd5528`, `#define HERO_SIGNAL_DEFER 0` at 135) | route C, refusing at the delivery, D kept |
| **E** | `grade/f/heroes`, the same `heroes-c2` (`c06b1956714cebef`) | `sources 1f83fdf86f0d2855` | `grade/f/runtime`, that runtime with line 135 `#define HERO_SIGNAL_DEFER 1` (`c9cdff4b7eefda8e`; `parts/thread.c` `56fb184ffbf383cc`) | route F, E's delivery on the runtime's thread with C's detection, the one deferring runtime built (`compiler-engineer.md` § 3, *"the runtime switched by one line"*) |

**Checked before any session** (`p247-scratch/grade/check/`, the lane's `storm.hero` built by each
setup and run once under the storm driver): **today** stopped with D's words (exit -6), **C** refused
with *"storm.on_signal ran as a signal handler ..."* (exit -6), **E** ran to *"built 20000000 words"*,
exit 0, after 757,653 signals: each as the compiler-engineer's report reads its route. E is built with
`heroes-c2` and the switched runtime rather than the report's `heroes-f` (`sources
6fe8841f73943f3c`), since the emitted thunk is the same and only the runtime's one line differs.
**No safe-point runtime is built** (`compiler-engineer.md` § 7: its polls built, the per-thread
deferral S needs not), so arm s is graded under **E** and marked: S's sentence promises *"on the thread
the signal interrupted"*, which F does not keep, and neither task's rubric can see the thread.

Every answer is graded under all three setups, **every Darwin death read in its crash report**
(`p247-scratch/tools/ips.py`, 00-shared.md), and its **handler** is recorded beside its class: a
function of the program given to C as a handler (formatting, allocating or neither), a handler written
in C in a header the program ships, the platform layer's waiting calls, or none.

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
    and 0, the driver sending 120,973, 0, 0, 4,925 and 0: **four of five right answers graded wrong**
    (with no signal, `/usr/bin/time -p`, three runs each: real 0.36 to 0.37 s under the first count,
    2.39 to 2.68 s under the second). So t2's count changed before any session, to twenty rounds of a
    fresh list of 1,000,000 (`main.hero` there, `t2ref20`), and five runs of the same command
    (`grade-20000000.log`) read M = 1,273,492, 1,201,389, 1,226,803, 1,218,662 and 1,215,182, each at
    most what was sent (1,284,383, 1,220,366, 1,242,409, 1,229,841, 1,229,071), every one `built
    20000000 items` and exit 0: five of five right.
- **The decision rule, rescaled before any session**: an arm's sentence enters only if its arm gives
  at least **three** more `right` answers than arm x over its **eight** sessions (t1 and t2 together),
  both graded under the route its sentence describes: r and m under **C**, e and s under **E** (marked
  provisional for s, above). Arm x's eight answers are recorded under all three setups as what a reader
  writes today and what each route does to it. **Its reason**: with four sessions in each of two cells,
  a difference of two right answers is what one changed answer per task can make by chance, so a
  sentence enters only on a difference no single answer per task can produce.

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
