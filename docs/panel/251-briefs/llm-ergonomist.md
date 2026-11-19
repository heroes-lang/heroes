# Panel 251, llm-ergonomist: the blind seat's sessions, its runner and its grading, registered before any run

Written 2026-11-19 by the coordinator's brief-writer, for the coordinator, who runs `run.sh` **after the
critic's reading of these repairs** and grades the reports. **Rewritten after the critic's first reading**
(`docs/panel/251-reports/critic-briefs.md`, findings 4, 5 and 11) and the coordinator's decision (1): the
first design, one session holding both specifications and a diff, is superseded (its five files are kept
under `superseded-0931/` in the blind area). **Unrun when written**: the writer started no `claude -p`
session and made no paid call; `run.sh` was checked by `bash -n` alone. Every number below comes from a
command run while writing, named beside it. Nothing in this file reaches a session: each reads its own
folder alone. **The frozen commit** is `3a5ee269117a31780ef1f96d0305452630ac4d83` (`00-shared.md` § The
frozen commit), whose `spec/heroes-spec.md` is the arm `now`.

## What it measures

Four readings, each a reader holding one specification and one task, under the frozen spec and under
the sitting's arms, label-stripped (design.md Part 11's question for a sentence of the spec, and the
spec-warden's payment, design.md §1.6):

- **task 1 (question 2)**: does a reader predict what a program that leaves a thread running does at the
  end of `main`, under the frozen spec, R's sentence and E's sentence?
- **task 2 (question 3)**: does a reader write a program that hands each of four threads a document and a
  list and reads back three facts, under the frozen spec (one `str` each way), the proposal spelled B
  (built-ins) and the proposal spelled M (operations after a dot)?
- **task 3 (question 1)**: does a reader predict that a function of the program called from a C signal
  handler is refused, under the frozen spec and route K's told-residue sentence?
- **task 4 (question 3's mailbox)**: does a reader predict that `m2 = m` then a send through `m2` is seen
  by a receive through `m`, under the proposal's two spellings (the critic's finding 7)?

## The blind area

**`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/251-llm-ergonomist/`**, moved
there by the coordinator from the session scratchpad, **inside the repository's root** (CLAUDE.md § Hard
stops), ignored by git (`git check-ignore -v .` there: `.gitignore:34:.claude/worktrees/`), **under the
repository's `CLAUDE.md`, `.claude` and git tree** (`git -C <area> rev-parse --show-toplevel` prints
`/Users/joseph/Temp/heroes/heroes-lang`), as sittings 246, 247, 250 and 252 kept theirs. So **every session
runs with panel 252's guards** (`p252/docs/panel/252-briefs/llm-ergonomist.md:105-125`, carried), in
`run.sh`: `env -i` handing the session `PATH`, `HOME`, `LANG`, `GIT_CEILING_DIRECTORIES` and
`ANTHROPIC_API_KEY` alone; `GIT_CEILING_DIRECTORIES` set to the session folder's parent, with which `git
rev-parse --show-toplevel` from `s/t1-y-1` prints *"fatal: not a git repository"* (run); `--bare`, which
skips auto-memory and `CLAUDE.md` discovery and takes its auth strictly from `ANTHROPIC_API_KEY`
(`claude --help`, read by the critic, carried); `--restricted --safe-mode --strict-mcp-config --model
claude-opus-5-5`; the tools as `SKILL.md:160-166` names them. `claude --version` printed `2.1.311
(Claude Code)` and its help names every flag (`grep -o -E`, run). **What still reaches a session is the
question its `context` answer measures**: the folder's path names `heroes-lang`, `rebel-scratch` and
`251-llm-ergonomist`, as 252's and 250's paths named theirs; **a `context: yes` voids that session's
reading until it is re-run**.

**Its layout** (`find`, `ls`):

- `arms/`: the six specifications by arm name, and `proposal-b.diff` (outside every session folder);
- `tools/`: `brief-session.md`, `brief-verdict.md`, the five task files, `make_sessions.py` (which made
  the folders), and the runner's three helpers;
- `s/<task>-<label>-<n>/`: **50 session folders**, each holding `brief.md`, `task.md` and `heroes-spec.md`
  and nothing else (150 files, `find s -type f | wc -l`; none holds `run.json`);
- `verdict/`: the one verdict session: `brief.md`, `task.md` (task 2), `spec-A.md` (the frozen spec),
  `spec-B.md` (the proposal spelled B) and `changes.diff`, **never graded**;
- `key.txt` (the label of each arm, then each folder's arm and its three or five files' sha256) and
  `order.txt` (51 folders), beside `s/`, never inside it; `run.sh`.

**The draw**, made by `tools/make_sessions.py` before it made any folder, `secrets.SystemRandom().sample`
over `cfghjsuvwxyz` (no letter naming a route or an arm): *"draw now->y q2-r->z q2-e->x q1-k->c
proposal-b->f proposal-m->h"*. Every session's specification is named `heroes-spec.md`, and no folder holds
a diff, so no session can tell which arm it reads.

| arm | label | the specification (`bw/arms2/`, `make_arms2.py`) | sha256, first 16 | lines | vendored delta |
|---|---|---|---|---|---|
| `now` | y | the frozen spec | `1d532d514831aa70` | 492 | 0 |
| `q2-r` | z | Q2's R, merged into the lease sentence | `49cf54954dcb84e5` | 492 | +5 |
| `q2-e` | x | Q2's E, sitting 252's `pE` after T1 | `92c44fc0cb999390` | 492 | +27 |
| `q1-k` | c | Q1's route K, the told residue | `f7484374bdec1123` | 492 | +26 |
| `proposal-b` | f | Q2's R and Q3 spelled B | `9d30660e27e983a4` | 496 | +168 |
| `proposal-m` | h | Q2's R and Q3 spelled M | `5b2d48d35e0607d2` | 498 | +191 |

**The cells**, 10, each with 5 replicates (`ls s | grep -c '^t<n>-'`: 15, 15, 10, 10):

| task | its file (sha256, first 16) | arms |
|---|---|---|
| t1 | `task-t1.md` `535f581d23de4178`, sitting 252's t1, the same bytes: the unjoined program through the doors | now, q2-r, q2-e |
| t2 | `task-t2.md` `6a3916a749e9be37`: four documents counted on four threads | now, proposal-b, proposal-m |
| t3 | `task-t3.md` `a8226a017c2e2e95`: the C handler calling the program's function | now, q1-k |
| t4 | `task-t4-b.md` `484164c78f2ab998` and `task-t4-m.md` `1be98cf7de9ff794`: `m2 = m`, a send through `m2`, a receive through `m`, the close, in each spelling | proposal-b, proposal-m |

Every graded session's `brief.md` is `tools/brief-session.md`, sha256 `724e8e0d656bb7b0...`, **panel 252's
seat brief, the same bytes**: the method, `choice_points` and never hesitations (`SKILL.md:180-186`),
`prediction` with how a program ends, and `context`. The verdict session's brief (`brief-verdict.md`,
`52d37bb04ac8ab20...`) asks for the agent definition's `verdict`, `experiment`, `choice_points`,
`argument`, `condition` and `context` over task 2 written under both. **What the folders say of the
project**: `grep -l -i -E
'panel|defect|2522|2552|sitting|coordinator|claude|anthropic|design\.md|rebel|seat|ledger|worktree|joseph|heroes-lang|golden|fixedbugs|selfhost|runtime/|spec-warden|ergonomist|thesis|blind'`
over every session's and the verdict's `brief.md` and `task.md` and the verdict's `changes.diff` printed
nothing (0 files); the specifications and the tasks name the language and its command `heroes`.

## The order, the runner and the bound

**`order.txt`**: five rounds over the ten cells, odd rounds forward and even rounds reversed, round k
carrying replicate k, so a stop leaves every cell within one replicate of every other; **the verdict
session last**, so a stop spends the bound on the graded cells first.

**`run.sh`** (sha256 `964d7d3bcd3413c9...`, `bash -n` clean, **not run**), run by the coordinator alone:

```
bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/251-llm-ergonomist/run.sh
```

It holds `run.lock`, so one runner at a time; before spending anything it asks `tools/check_folders.py`
that `order.txt` names 51 distinct folders, each in `key.txt`, and that every folder not yet run holds
exactly its registered files at their registered sha256 (run by the writer, read-only: exit 0), and stops
otherwise; it loads the key with `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a`
and prints its length alone; it takes `caffeinate -i -w` for its own life (the machine's holds today are
the display's and the sound's, the critic's finding 10); then for each folder of `order.txt`, one at a time
and never two: **a folder holding `run.json` is skipped and its cost counted**, so a stopped run resumes;
**a session starts only while the spend so far** (`tools/spent.py`: every `run.json`'s `total_cost_usd`
summed, an unreadable one counted as 0.50; it prints `0.000000` today) **plus the 0.40 cap plus a 0.10
margin is at most 9.50**; the session runs from its folder as

```
env -i PATH="$PATH" HOME="$HOME" LANG=en_US.UTF-8 \
    GIT_CEILING_DIRECTORIES="$(cd .. && pwd)" ANTHROPIC_API_KEY="$ANTHROPIC_API_KEY" \
  claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
    --bare --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5 \
    --tools "Read,Write" --allowedTools "Read,Write" \
    --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
    --max-budget-usd 0.40 --output-format json > run.json 2> run.err
```

and `tools/after.py` saves the CLI's `result` as `result.md` beside `run.json` and appends one line to
`run.log`: the folder, the exit, the `subtype`, the cost, the models of `modelUsage`, and whether
`report.md` and `result.md` exist (panel 250's 24 sessions wrote no `report.md`, carried).

**The bound**: 10 USD for the sitting (the author's, as the coordinator relays it); the caps alone bound
the 51 sessions at 20.40 USD, so **the stop rule is what holds the ceiling**: the total stays under 9.50
while no session passes its cap by more than 0.10, and under 10.00 while none passes it by more than 0.60;
the spec-warden's refreshes count at 0 inside the rest (`spec-warden.md`). **What it will cost, an
inference**: panel 250's 24 sessions, each one specification and one task, cost 0.120 to 0.143 USD, sum
3.139598, median 0.1308 (the critic's reading of `docs/panel/250-reports/evidence.tar.xz`, carried); 50 at
that median is 6.54 USD (`python3 -I -c`), and the verdict session, two specifications and a diff, about
twice one session's cache write.

## The grading, registered before any run

What is read from a report, the same way by anyone: the build's verdict (accepted or refused) from
`answer` or `prediction`, `prediction` winning where they differ; the lines written, in order, a line
given as *maybe* or *depending on timing* recorded as **hedged**; the ending, one of **normal**, **abort**
(and what it names), **never**, **refused**, a report giving two graded by the first it commits to.

| task | arm | right iff the report says |
|---|---|---|
| t1 | now | accepted; `main done, worker <n> not joined`, **any integer `<n>`** (the spec gives no handle's value, the critic's finding 11); the worker's line not written; an **abort** naming heap blocks (today's runtime: 10 of 10 at the frozen commit, `00-shared.md`). Expected right: none. **Recorded: the reader's ending class**, what a reader of today's spec assumes |
| t1 | q2-r | accepted; the main line, any integer; the worker's line not written; an **abort** naming **1** thread not joined |
| t1 | q2-e | accepted; the main line, any integer; the worker's line not written; **normal**, with a message naming **1** thread ended |
| t2 | now | the answer's `main.hero` built by a stage 2 of the frozen commit and run 5 times under `timeout -s KILL 20`: builds, **waits for every thread it starts on every path**, and writes the four lines exactly, in order, exit 0, 5 of 5 (the writer's reference, `bw/t2/ref-x.hero`, does: 5 of 5). Recorded: the encoding into text (it must encode), its lines (`wc -l`), its `choice_points` that name the encoding |
| t2 | proposal-b | **built and run on the compiler-engineer's Q3 slice** (`spawn` and `wait` of one record holding a `str` and a `[str]`, `compiler-engineer.md` § Question 3) **where the answer stays inside it**, as `now`'s row; elsewhere graded by the arm's words: `spawn` handed a value holding the document and the list, four started before any `wait`, every thread waited for, the four lines it states right, **no encoding into text**. **Wrong if it declares a `spawn`, `spawn_sized`, `wait`, `mailbox`, `send`, `receive` or `close` of its own** (each `error[builtin_name_taken]` under B, which the arm's text never says, the critic's finding 2); recorded either way |
| t2 | proposal-m | as `proposal-b`'s row, the operations after a dot; **wrong if it declares a `spawn`, `spawn_sized` or `mailbox` of its own** (a declared `wait`, `send`, `receive` or `close` is the program's own under M); recorded either way |
| t3 | now and q1-k | accepted; `installed: 0`; then an **abort** when the handler calls the function, naming a signal handler; `the answer ran for signal 30` not written (this Mac at the frozen commit: 5 of 5 at `-O0` and at `-O2`) |
| t4 | proposal-b and proposal-m | accepted; `received: 1`; **normal** (the mailbox closed). Wrong if the receive through `m` is predicted to fail (`timeout`) or abort, the reading of § 3's *"mutating `b` never changes `a`"* that the arm's address sentence answers. Not buildable today (no compiler has a mailbox): graded by the arm's words |

## The decision rules, counted and registered before any run

With 5 replicates per cell; where the stop rule leaves a cell short, each rule reads the replicates every
cell of its task reached, and a difference it names stays a count of sessions.

- **Q2, task 1.** R's sentence is read as **needed** when `q2-r`'s right answers exceed `now`'s, re-graded
  by `q2-r`'s row, **by at least 3**; E's likewise by `q2-e`'s row. Recorded beside it, not a gate: `now`'s
  five ending classes (normal, abort, never, refused, hedged), what a reader of today's spec assumes; a
  route that matches the reader's default surprises a reader least. Its reason, as panel 252's: one
  changed answer per cell cannot make a difference of three.
- **Q3, task 2.** A spelling is read as **serving the thesis** when its right answers exceed `now`'s by
  at least 3; **B against M**: the one with more right answers over tasks 2 and 4 together (10 sessions
  each) by at least 2 is the readers' spelling, and a difference under 2 says the spelling does not
  decide; a spelling with **2 or more of its 5 task 2 answers declaring a name it takes** is recorded as
  *"taking a name readers write"*.
- **Q3's mailbox, task 4.** The address sentence is read as **sufficing** in a spelling when at least 4 of
  its 5 answers are right; otherwise the synthesis quotes the failing answers' `choice_points`, and a
  rewording owes its own reading.
- **Q1, task 3.** Route K's sentence is read as **changing a reader's prediction** when `q1-k`'s and
  `now`'s right answers differ by at least 3.
- **The spec-warden's registered prediction**, if it registers one before `run.sh` starts, is scored on
  these counts as it is worded.
- **The verdict session** is recorded whole in the report, its `verdict` quoted, and grades nothing.
- **Any session answering `context: yes`** is voided, recorded, and its cell read without it.

## After the run

`run.log`, each `run.json`'s cost, model and `subtype`, and every `report.md` (or `result.md`) are copied
into `.claude/worktrees/rebel-round/docs/panel/251-reports/llm-ergonomist.md` under a header saying how
the sessions were run, with every `context` answer quoted and each session's grade by the rows above.
