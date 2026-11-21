# Panel 252, llm-ergonomist: the blind seat's sessions, its runner and its grading, registered before any run

Written 2026-11-19 by the coordinator's brief-writer, for the coordinator, who runs `run.sh` **only after
the spec-warden's report holds its registered predictions** (`run.sh` copies the report, hashes the copy
and refuses an empty heading, § The runner) and grades the reports. **Repaired the same afternoon** after
the completeness critic's first reading (`docs/panel/252-reports/critic-briefs.md`, findings 1, 2, 4, 5,
9 and 11), panel 253's critic's findings 4, 5 and 6 on the same runner
(`docs/panel/253-reports/critic-briefs.md`), and the coordinator's decisions (a) to (e), each written in
below as a decision. **Unrun when written**: the writer started no paid session; `run.sh` was checked by
`bash -n` alone; its instruments were run against panel 251's 51 transcripts and the coordinator's free
probes, at no cost (§ The controls). Every number below comes from a command run while writing, named
beside it. Nothing in this file reaches a session. **The frozen commit** is
`181555e9a32d0b18878f9f10e48d2eadd939c285`, whose `spec/heroes-spec.md` is the arm `now`.

## What it measures

Five readings, each a reader holding one specification and one task, under the frozen spec and one arm
of the sitting, label-stripped (design.md Part 11's question for a sentence of the spec, and the
spec-warden's payment, design.md §1.6):

- **task 1 (Q1)**: does a reader writing a face and its private module write the ring the compiler
  refuses, under the frozen spec (silent on cycles) and Q1-C's sentence? Graded by building with the
  frozen stage 2 and again with the probe that lifts the refusal, so `now` reads Q1-R as well;
- **task 2 (Q2)**: does a reader predict that `nodes({"leaf": []})` is refused and `nodes({"a": ["b"],
  "b": []})` accepted, under the frozen spec and Q2-C's sentence (repaired to say what the compiler does,
  critic's finding 1)? A reader of `now` predicting both accepted reads the spec as Q2-R does;
- **task 3 (Q3)**: does a reader write a ring for any element type right, under the frozen spec
  (functions only) and Q3-R (records with a type parameter, its example now `Pair<A, B>`, never the task's
  answer, decision (c)); **both arms graded by one written rubric** (decision (c));
- **task 4 (Q4)**: does a reader writing each word's first letter put a `u8` in a hole and print a number,
  under the frozen spec and Q4-R (the refusal said)?
- **task 5 (Q5)**: does a reader given a tolerance of `1e-9` write an exponent literal the compiler
  refuses, under the frozen spec (silent on a float's form) and Q5-C (*"no exponent"* said)? Regraded
  with each exponent written out, so `now` reads Q5-R as well, by a registered count (critic's finding 9).

## The blind area, and every leak shut

**`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/252-llm-ergonomist/`**, inside
the repository's root (CLAUDE.md § Hard stops), ignored by git. What leaked before and what shuts it here:

- **panel 251's three leaks** (`docs/panel/251-reports/critic-reports.md:45-97`, finding 2; defect 2589):
  the labels by the inherited standard input (every `claude -p` line ends `< /dev/null`); the main
  checkout's git status rendered into each session (**`s/.git`**, an empty repository above every session
  folder, the verdict's included: `.gitignore` `*`, user `reader`, `reader@invalid`, no commit; from
  `s/t4-y-1` under the runner's environment, `git rev-parse --show-toplevel` prints `.../252-llm-ergonomist/s`,
  `git status --short` 0 lines, *"does not have any commits yet"*); the transcripts under the real
  `~/.claude/projects/` (**`HOME` set to `home/`** in this area; `ls ~/.claude/projects | grep -c
  252-llm-ergonomist` prints 0);
- **the system's temporary folder** (panel 253's critic's finding 5: panel 251's runner left a
  `cache-break-state-*.json` holding `t4-f-2`'s session id in `/private/tmp/claude-501/`; that file no
  longer stands there, `find /private/tmp/claude-501 -maxdepth 2 -name 'cache-break*'` printing nothing at
  15:40): **`TMPDIR` and `CLAUDE_CODE_TMPDIR` set to `tmp/` in this area** for every session (decision
  (e); the CLI names `CLAUDE_CODE_TMPDIR`, panel 253's critic by `strings`; whether it moves every file
  there is unrun, and is what the preflight and the scan read);
- `GIT_CEILING_DIRECTORIES` set to the area's root, so git may reach `s/.git` and never the repository
  above;
- **what still reaches a session is its folder's path**: the stored request's second system block is
  `CWD: <its folder>` (read in probe4 and in panel 251's transcripts), which names `rebel-scratch`, the
  area, the task, the label and the replicate. The critic offered neutral folder names as a route of small
  value (finding 11); the area's path is the contract's (`coord/brief-writer-common.md`), the label is drawn
  at random, and the brief already tells the reader it is a model judging a specification: kept, and the
  scan records it.

**The preflight, before any spend** (`tools/preflight.py`, repaired, decision (e)): a session in
`s/preflight/` runs the paid sessions' very command with a key the API refuses (the coordinator's four
probes spent 0, `total_cost_usd 0` each), and its transcript is read **only in the entries the session did
not write itself** (`tools/transcript_checks.py`: the user entries, the attachments, the queue's enqueued
content, the stored request's system blocks at `api-request-shape.shape.system`, the request blobs of role
`system` and `user`; never an `assistant` entry or blob, panel 253's critic's finding 4). It fails **on
silence** (no session id, no git status, no stored system blocks) and on: a git status naming a commit, a
person other than `reader` or a dirty tree; an attachment kind beyond `session_context` and `date` (51 of
51 of panel 251's held exactly those two, the critic's count, reproduced by the scan below); system blocks
other than *"You are a Claude agent, built on Anthropic's Claude Agent SDK."* and `CWD: <its folder>\nDate:`;
a first user entry or enqueued content other than the runner's prompt alone; another session folder's
name; any of `CLAUDE.md`, `MEMORY.md`, `Memory index`, `AGENTS.md`, `auto-memory`, `.claude/rules`; a
transcript not under the area's `home/` exactly once, or a folder for the area under the real `HOME`; a
file under `/private/tmp/claude-<uid>` modified since `tmp-marker` whose path names the area or a session
id, or whose bytes hold the session id outside a `tasks/` folder (a coordinating session's background-task
logs, which print ids and the area's name: three of them did for panel 251, read at 15:44); a cost other
than 0. It records the reader's `model`, `output_config` and `thinking`.

**The scan, after the run** (`tools/scan.py`, the same checks over every session's transcript, then the
temporary folder since `tmp-marker`): its output is `scan.txt`, its last line into `run.log`; a session
failing any check is voided (§ The decision rules).

## The controls, at no cost

Each run by the writer, 2026-11-19, its output under `bw/` (`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/252-briefwriter/`):

- **The scan over panel 251's 51 transcripts** (`rebel-scratch/251-llm-ergonomist/transcripts/`, its
  `order.txt`, its area; `python3 -I tools/scan.py --control ...`, `bw/control-scan-251.txt`), a positive
  control, since every one of them leaked: **51 of 51 fail**, each on the git status (51), the first user
  entry (51), the enqueued content (51) and other folders' names (50 sessions name 49 others, 1 names 50);
  **0 name project text** (the two sessions panel 253's scan flagged on their own `context` answers,
  `t1-x-4` and `t3-c-2`, are clean here); every one holds the two attachment kinds and two system blocks,
  effort `medium`. The temporary folder: 606 files modified since 10:00, **none** naming a 251 session or
  its area, once the coordinating session's task logs are told apart (three before that rule).
- **The temporary check's positive control**: two planted files in `bw/tmp/fake-claude-tmp/`, one holding a
  251 session id in its bytes, one naming the area in its path: **2 of 2 caught**.
- **The preflight on the coordinator's probe4** (`coord/blindtest/`, its prompt *"Say hi."*):
  **held**, every check: one session id, `date` and `session_context`, git `reader`, clean, no commit, two
  system blocks, the prompt alone, no other folder, no project text; `claude-opus-5-5`, `{"effort":
  "medium"}`, thinking `adaptive` (`bw/control-preflight-probe4.txt`).
- **The preflight on panel 251's `t4-f-2`**: **failed**, on the git status and the standard input twice
  (`bw/control-preflight-251.txt`).
- **The preflight on silence**: probe4's transcript with its `session_context` attachment and its request
  shape removed: **failed**, *"no git status rendered"* and *"no stored request shape"*
  (`bw/control-preflight-silent.txt`).
- **The classifier** (`tools/classify.py`): panel 251's 51 `run.json` read `success` 51; the coordinator's
  four probes read `failed` 4, though each records `subtype success` (with `is_error true`,
  `terminal_reason api_error`): **a subtype alone does not tell a failure**, measured.
- **The gate** (`tools/gate.py`, in `bw/tmp/gatetest/`): a report whose heading has nothing under it,
  refused; one line under it, passed with its copy's sha256; a resumed run's copy kept apart; a missing
  report, refused.

## Its layout

- `arms/`: the seven specifications by arm name and `proposal.diff`, outside every session folder; the
  four the critic's reading replaced kept under `arms/superseded-before-critic/`;
- `tools/`: `brief-session.md` and `brief-verdict.md` (**panel 251's, the same bytes**: sha256
  `724e8e0d656bb7b0...`, `52d37bb04ac8ab20...`), the five task files, `make_sessions.py`,
  `check_folders.py`, `spent.py`, `after.py` (panel 251's), `transcript_checks.py`, `preflight.py`,
  `scan.py`, `gate.py`, `classify.py`, `set_aside.py`;
- `s/<task>-<label>-<n>/`: **50 session folders**, each holding `brief.md`, `task.md` and `heroes-spec.md`
  and nothing else; `s/verdict/`: `brief.md`, `task.md` (task 3), `spec-A.md` (the frozen spec), `spec-B.md`
  (the `proposal` arm) and `changes.diff`, **never graded**; 155 files in all (`find s -type f -not -path
  's/.git/*' | wc -l` prints 156, `.gitignore` among them);
- `key.txt`, `order.txt` beside `s/`; `run.sh`; `home/`; `tmp/` and `failed/` (made by the runner).

**The draw**, made by `tools/make_sessions.py` before it made any folder, `secrets.SystemRandom().sample`
over `cfghjsuvwxyz`: *"draw now->y q1-c->j q2-c->g q3-r->f q4-r->w q5-c->x"*; **kept** when the folders
were remade after the critic's reading (`make_sessions.py --keep-draw`, which refuses where any `run.json`
exists). `grep -l -i -E` for
`panel|defect|sitting|coordinator|claude|anthropic|design\.md|rebel|seat|ledger|worktree|joseph|heroes-lang|golden|fixedbugs|selfhost|runtime/|spec-warden|ergonomist|thesis|blind`
over every session's `brief.md` and `task.md` and the verdict's `changes.diff`: 0 files.

| arm | label | the specification (`bw/arms/`, `make_arms.py`) | sha256, first 16 | lines | vendored delta |
|---|---|---|---|---|---|
| `now` | y | the frozen spec | `1d532d514831aa70` | 492 | 0 |
| `q1-c` | j | Q1-C: *"Modules may not form a cycle, so a module under `internal` cannot use the module that names it."* (§ 1) | `8448edd80708be52` | 494 | +26 |
| `q2-c` | g | Q2-C, **repaired**: *"`ok(...)`, `[]`, `{}` and a case name give none, nor does a literal whose first element or value gives none,"* (§ 9) | `4313f5f3bd0efd53` | 492 | +14 |
| `q3-r` | f | Q3-R, **repaired**: records and variants take type parameters, the example *"`record Pair<A, B>` holding `first: A` and `second: B` is written `Pair<i64, str>` in a type"* (§ 9, § 3, § 4) | `dfad79aa38933f85` | 495 | +63 |
| `q4-r` | w | Q4-R: a `u8` refused in a hole and in `print` (§ 2, § 11) | `1fddf579d07fd044` | 493 | +56 |
| `q5-c` | x | Q5-C: a float literal has no exponent; § 11 names `strtod` (§ 2, § 11) | `7dab16286fad68ad` | 493 | +35 |
| `proposal` | (verdict only) | Q3-R, Q4-R and Q5-R (**repaired**) together | `c7885b36d17a3bd0` | 498 | +160 |

**Why Q2-C changed** (critic's finding 1): its first wording, *"nor does a literal holding one"*, was
false of task 2's own line 6 (`{"a": ["b"], "b": []}` holds a `[]` and the compiler takes `K` from it), so
its readers would have been graded wrong for reading it exactly. Measured on 20 programs (`bw/q2/`): the
compiler types a container literal from its **first** element or value, and when that one gives no type it
reads nothing of the literal, its keys included (`{"a": [], "b": ["c"]}` and `[[], ["a"]]` refused;
`{"a": ["b"], "b": []}` and `[["a"], []]` accepted). The repaired sentence says that.

**The cells**, 10, each with 5 replicates (`ls s | grep -c '^t<n>-'`: 10 each):

| task | its file (sha256, first 16) | arms | the shape its input holds, run by the writer (`bw/ref/`) |
|---|---|---|---|
| t1 | `task-t1.md` `43fe94490564c63c` | now, q1-c | the ring (`area.hero` using `shapes` for `Rect`): `error[module_cycle]` under the frozen stage 2, `26` under the probe; without the ring (`of(w:, h:)`): `26` under both |
| t2 | `task-t2.md` `24e0f2c006fe933a` | now, q2-c | line 7 refused `cannot_infer` (column 26), line 6 accepted; with `{"leaf": []}` bound to a `{str: [str]}` the program prints `2`, `1` |
| t3 | `task-t3.md` `21651d32fa1ee3a9` | now, q3-r | under the frozen spec a shape record beside the program's list, `add<T>(@r: Ring, @slots: [T], item: T)`, prints `3 4 5 green blue` (`bw/ref/t3/main.hero`); an API in which a ring's state with another ring's list compiles. Neither the task nor this reference holds the word *pair* (`grep -c -i`: 0) |
| t4 | `task-t4.md` `becb4e82615f5ebf` | now, q4-r | `f"{word[0]} {word.len()}"` builds and prints `117 3`, `116 3`, `102 5`, `108 4`, exit 0; with a slice, `u 3`, `t 3`, `f 5`, `l 4` |
| t5 | `task-t5.md` `536a8a474e5aa2ba` | now, q5-c | `1e-9` in the code: `error[exponent_literal]`; written out (`bw/tools/t5_digits.py`): `true`, `false` |

The task files did not change in the repair. The verdict session reads task 3 alone under a `spec-B`
holding Q3-R, Q4-R and Q5-R: **its `verdict` speaks for Q3's form alone** (critic's finding 11), and is
recorded, never graded.

## The order, the runner and the bound

**`order.txt`** (sha256 `430f7064c1dfa490...`, unchanged by the remake): five rounds over the ten cells,
odd rounds forward and even rounds reversed, round k carrying replicate k; **the verdict session last**.
Round 5 runs forward and ends `t5-y-5 t5-x-5`.

**`run.sh`** (sha256 `3973292aa8ee107e...`, `bash -n` clean, **not run**), run by the coordinator alone:

```
bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/252-llm-ergonomist/run.sh
```

In order, each step stopping the runner with *"nothing run"* where it fails: `run.lock`; **0.** the gate
(`tools/gate.py`, panel 253's critic's finding 6, decision (e)): the spec-warden's report
(`.claude/worktrees/rebel-round/docs/panel/252-reports/spec-warden.md`) **copied as it stands** into
`warden-at-start.md` (a resumed run's copy `warden-at-resume-<time>.md`, the first never overwritten), the
**copy's** sha256 written into `run.log` with the time, and the run refused unless the copy holds `##
Registered predictions` once with at least one non-empty line under it; **1.** `tools/check_folders.py`;
**2.** `s/` its own repository's root, user `reader`, no commit, `.gitignore` `*`; `tmp-marker` touched
at the first start; **3.** the preflight and `tools/preflight.py`; **4.** the key from `.env`, its length
alone printed; **5.** `caffeinate -i -w` for its own life; then each folder of `order.txt`, one at a time;
**6.** after the loop, `tools/scan.py` into `scan.txt`. Each session runs from its folder as

```
env -i PATH="$PATH" HOME="$HERE/home" LANG=en_US.UTF-8 \
    TMPDIR="$HERE/tmp" CLAUDE_CODE_TMPDIR="$HERE/tmp" \
    GIT_CEILING_DIRECTORIES="$HERE" ANTHROPIC_API_KEY="$ANTHROPIC_API_KEY" \
  claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
    --bare --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5 \
    --tools "Read,Write" --allowedTools "Read,Write" \
    --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
    --output-format json --max-budget-usd <cap> < /dev/null > run.json 2> run.err
```

and `tools/after.py` saves the CLI's `result` as `result.md` and appends a line to `run.log` (the folder,
the exit, the `subtype`, the cost, the models, whether `report.md` and `result.md` exist), with the
session's outcome. `claude --version` printed `2.1.311 (Claude Code)`.

**The bound, decision (a)**: **CEILING 9.90 USD**, under the author's 10 for the sitting (the 9.50 the
first brief carried was the coordinator's margin, not the author's word). A session starts only while the
spend so far (`tools/spent.py`: every `s/*/run.json` and every set-aside `failed/*/*/run.json`, the
preflight's left out, an unreadable one counted 0.50) **plus its cap plus a 0.10 margin is at most the
ceiling**. **The cap**: 0.40 USD a session, **0.60 for the verdict session alone** (panel 251's verdict cost
0.3772 of its 0.40 cap, panel 253's critic's finding 15 for the same shape). No cell is cut: `t2-g` is
repaired, not dropped. **The spec-warden's refreshes count 0**: *"Token counting is **free to use** but
subject to requests per minute rate limits based on your usage tier"*
(`platform.claude.com/docs/en/build-with-claude/token-counting.md`, § Pricing and rate limits, read
2026-11-19 by the writer, decision (d)).

**A session that fails, decision (b)**, registered here and in `run.sh` before any run, by
`tools/classify.py`: **success** is `subtype` `success` with `is_error` false and `terminal_reason`
`completed`; **capped** is `subtype` `error_max_budget_usd` (the name the CLI 2.1.311's binary holds, by
`strings`; how it reaches `run.json` is unrun); **failed** is anything else, an unreadable `run.json`
included. A **failed** session has every file its folder's key does not register (its `run.json`,
`run.err`, `result.md`, any `report.md`) moved under `failed/<folder>/attempt-1/` (`tools/set_aside.py`) and
is **run again once**, under the same stop rule; a **second failure is kept, recorded** in `run.log`, and
its cell read without it. A **capped** session is **graded on what it wrote**, recorded as capped. A
resumed run gives a folder whose one failure was not yet set aside its retry.

**What it will cost, an inference from a measurement**: panel 251's 51 `run.json` (`python3 -I` in its
area): its prediction tasks a median 0.1602 and 0.1595 USD (max 0.2003), its writing task 0.1847 (max
0.2112), its verdict 0.3772; the critic's fit of its 50 graded costs to their `usage` is exact at 4, 5,
0.20 and 20 USD per million input, cache-write, cache-read and output tokens, each session's non-output
part 0.072 to 0.076 (carried, `critic-briefs.md:74-76`). This sitting holds four writing tasks and one
prediction task. **Simulated over `order.txt` with the stop rule above** (`python3 -I`): at 251's medians
all 51 start, **9.3672 USD**; every graded session starts up to a uniform rise of **6.8%** per session; at
251's maxima **46 run**, the stop coming before `t4-y-5`, so tasks 4 and 5 each short one replicate and the
verdict unrun. A retried failure costs a session more and is not in the simulation. The decision rules
read the replicates every cell of a task reached.

## The grading, registered before any run

What is read, the same way by anyone: the answer the session gave, **`result.md`** (panel 251's 51
sessions wrote no `report.md` and 51 `result.md`, `grep -c` over its `run.log`), or `report.md` where one
stands; the files of `answer`, each under the path it names (the longest fenced block where one file is
asked and no path is named); the build's verdict from `answer` or `prediction`, `prediction` winning where
they differ; the lines written, in order, a line given as *maybe* or *depending* recorded as **hedged**;
the ending: **normal**, **abort** (and what it names), **never**, **refused**. **Building**: by the frozen
stage 2, `bw/tree/heroes-next` (`sources 23c7d2a626b29fd5`), and for task 1 also by the probe
`bw/q1/heroes-nocycle` (`sources 7023dd011002b5d5`, its patch `bw/q1/nocycle.diff`), each binary run 3
times under `timeout -s KILL 20`. A **capped** session is graded on what it wrote; a session the scan voids
is not graded.

| task | arm | right iff |
|---|---|---|
| t1 | now, q1-c | the three files build under the **frozen** stage 2 and print `26`, exit 0, 3 of 3. **Recorded**: whether `area.hero` uses `shapes` (the ring); the reader's own prediction. **Regraded for Q1-R** (`now` alone): the same files under the **probe**: build, `26`, exit 0 |
| t2 | now, q2-c | **refused**, naming line 7 (the second `print`, `nodes({"leaf": []})`) or the `[]` in it, and **line 6 not refused**. **Regraded for Q2-R** (`now` alone): **accepted**, printing `2` then `1`, a normal end. **Recorded**: every report's verdict line by line |
| t3 | now, q3-r | **one written rubric for both arms, read from the answer's text** (decision (c)), right iff all four rows hold: **R1** the program holds nothing its arm's specification refuses: under `now` no type parameter on a record or variant; under either, no `@` parameter called without `@`, no `[]` or `{}` with no type to take, no two same-typed parameters called without their labels, no method, closure, default argument, tuple or mutable global; **R2** one ring written once serves both element types (no two copies of its code differing only in the type); **R3** an add moves no held item: it pushes while not full, then overwrites the oldest slot and moves the oldest's position, never rebuilding the list; **R4** the lines it says it prints are `3`, `4`, `5`, `green`, `blue`, in order, a normal end. **Recorded beside the grade, never graded**: the `now` answer's build and run under the frozen stage 2 (no compiler holds generic records, so `q3-r`'s cannot be built); its shape (a shape record beside the program's list, a `[T]` and an index passed beside it, a generic record, other); whether its API lets one ring's state be handed another ring's list and still compile |
| t4 | now, q4-r | the file builds under the frozen stage 2 and prints `u 3`, `t 3`, `f 5`, `l 4`, exit 0, 3 of 3. **Recorded**: whether a `u8` stands in an `f"..."` hole or as a `print` argument; **silent wrong** where it builds, exits 0 and prints a number where the letter goes. Under `q4-r` an answer holding a `u8` in a hole or `print` is graded **refused** (the arm says so), wrong on the first try and recorded as *loud* |
| t5 | now, q5-c | the file builds under the frozen stage 2 and prints `true`, `false`, exit 0, 3 of 3. **Recorded**: whether it writes an exponent literal (`error[exponent_literal]`). **Regraded for Q5-R** (`now` alone): each exponent literal in its code written out by `python3 -I bw/tools/t5_digits.py <in> <out>` (exactly, by Python's `Decimal`; strings and comments left; one inside an `f"..."` hole written out by hand), built and run the same way |

## The decision rules, counted and registered before any run

With 5 replicates per cell; where the stop rule leaves a cell short, each rule reads the replicates every
cell of its task reached, and a difference it names stays a count of sessions.

- **Q1, task 1.** **The refusal meets readers** when `now`'s right answers under the probe exceed its right
  answers under the frozen stage 2 by **at least 3**. Q1-C's sentence **carries** when `q1-c`'s right
  answers (frozen) exceed `now`'s (frozen) by at least 3. Neither, and R6's ruling stands on this reading.
- **Q2, task 2.** **The spec reads as Q2-R** when at least 4 of `now`'s 5 predict both lines accepted;
  Q2-C's sentence **carries** when at least 4 of `q2-c`'s 5 are right by its row (line 7 refused, line 6
  not).
- **Q3, task 3.** Q3-R **serves the thesis** when `q3-r`'s right answers by the rubric exceed `now`'s by
  the rubric by **at least 3**. Recorded beside it, not a gate: how many `now` answers hold an API that
  accepts a mismatched pair; 3 or more of 5 is *"readers write the hazard `containers` wrote"*.
- **Q4, task 4.** **The trap meets readers** when at least 2 of `now`'s 5 are silent wrong. Q4-R **serves
  the thesis** when `q4-r`'s right answers exceed `now`'s by at least 3; and **by construction**, each of
  `now`'s silent wrong answers would be a refusal under Q4-R's compiler, recorded as the count turned from
  silent to loud.
- **Q5, task 5.** **The refusal meets readers** when at least 3 of `now`'s 5 write an exponent literal.
  Q5-C's sentence **carries** when `q5-c`'s right answers exceed `now`'s by at least 3. **Q5-R is read as
  needed** when `now`'s right answers once regraded exceed its right answers under the frozen stage 2 by
  **at least 3** (critic's finding 9).
- **The spec-warden's registered predictions** (under `## Registered predictions` in its report, as the
  gate's copy holds them, `warden-at-start.md`) are scored on these counts as they are worded.
- **The verdict session** is recorded whole in the report, its `verdict` quoted, and grades nothing.
- **Voided**, recorded and its cell read without it: any session answering `context: yes`, and any whose
  transcript fails a check of the scan.

## After the run

`run.log`, `scan.txt`, `warden-at-start.md`, each `run.json`'s cost, model, `subtype` and outcome, every
`result.md` (or `report.md`), every set-aside attempt under `failed/`, and what each transcript holds of
attachments and system blocks (kinds and counts, never its text) are copied into
`.claude/worktrees/rebel-round/docs/panel/252-reports/llm-ergonomist.md` under a header saying how the
sessions were run, with every `context` answer quoted and each session's grade by the rows above.

Written by the coordinator's brief-writer, 14:52 CET on 2026-11-19; repaired after the critics' readings
until the time at the foot of `00-shared.md` (`date`); `run.sh` not run.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/252-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out. `coord/blindtest/`, the coordinator's preflight of a blind session, is in no archive and
is gone.
