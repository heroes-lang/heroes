# Panel 253, llm-ergonomist: the blind seat's sessions, its runner and its grading, registered before any run

Written 2026-11-19 by the coordinator's brief-writer, for the coordinator, who runs `run.sh` **after the
spec-warden's report holds its registered predictions** (`spec-warden.md` § FIRST; the runner checks it,
below). **Repaired the same afternoon after the critic's first reading**
(`docs/panel/253-reports/critic-briefs.md`, findings 1 to 6 and 15 to 17, and the coordinator's
decisions (c) and (d)): the three tasks were re-worded, the labels drawn a third time, the runner given a
temporary directory inside the area, a free preflight, a copied and checked predictions gate and the
verdict's own cap, and `tools/scan.py` taught to skip what a session wrote itself. **Unrun when
written**: the writer started no `claude -p` session and made no paid call; `run.sh` was checked by
`bash -n` alone, and the read-only checks it makes before any spend were run by the writer
(`tools/check_folders.py` and `tools/check_area.py`, exit 0 each; `tools/spent.py` prints `0.000000`).
Every number below comes from a command run while writing, named beside it. Nothing in this file reaches
a session: each reads its own folder alone. **The frozen commit** is
`181555e9a32d0b18878f9f10e48d2eadd939c285` (`00-shared.md` § The frozen commit), whose
`spec/heroes-spec.md` is the arm `now`.

## What it measures

Three readings, each a reader holding one specification and one task, under the frozen spec and under one
robust sentence of this sitting, label-stripped (design.md Part 11's question for a sentence of the spec,
and the spec-warden's payment, design.md §1.6):

- **task t2 (question 2)**: asked to document `count_words` so that a reader sees what it answers, each
  parameter and an example, **what does a reader write by default** under the frozen spec, and does a
  reader of `doc-r` write the shape `heroes doc` reads (one `` - `name`: `` line per parameter in order,
  after a line holding `#` alone; the example a documented `test` titled `count_words:`)?
- **task t4 (question 4)**: asked for a test that connects where a server is named and *"cannot check the
  connection"* where none is, **how many readers of today's spec write a test that reads `ok` having
  checked nothing** (the hazard: Rebel holds 106 such tests, `00-shared.md` § Question 4), and does a
  reader of `skip-r` call `skip` there and predict the outcome?
- **task t6 (question 6)**: does a reader write the commands and the catalogue for a program over two of
  Rebel's packages the arm's example never names (`yaml`, `template`, version `0.4.0`), and predict what a
  third (`time`) used later does to the build, under the frozen spec (where it builds) and under `umb-r`
  (where it is refused with the `heroes add` that places it)?

**Questions 1, 3 and 5 have no cell**: question 1 moves design.md and no spec sentence, and the blind seat is
never given design.md (`.claude/agents/llm-ergonomist.md`); question 3 moves no sentence a reader reads;
question 5's report is the runtime's words, and a task that fixes a program from an abort's report sees the
difference only where the program hides which call aborted (none was designed: a seat may propose one,
with its size, for the coordinator).

## The blind area

**`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/253-llm-ergonomist/`**, inside the
repository's root (CLAUDE.md § Hard stops) and ignored by the repository's git (`.gitignore`'s
`.claude/worktrees/`). **Every session takes four repairs**, three of panel 251's leaks (its critic's
finding 2, defect 2589, measured by the coordinator on 2026-11-19 in `rebel-scratch/coord/blindtest/` with
a key the API refuses) and the fourth way out panel 253's critic found:

1. **The area is the top of an empty git repository of its own**, made by the writer: `git init -q .`,
   `.gitignore` holding `*` alone, `git config --local user.name reader`, `user.email reader@invalid`; `git
   rev-parse --show-toplevel` from the area prints the area, `git status --porcelain --ignored=no` prints
   nothing, no commit and no ref. It stands above `s/`, `verdict/` and `preflight/` (the coordinator's probe
   `coord/blindtest/s/probe4` rendered *"Git user: reader ... Status: (clean) ... Recent commits:"* and
   nothing under it, read from its transcript by the writer).
2. **`HOME` is the area's `home/`**, so the CLI writes its transcripts under `home/.claude/projects/`, never
   under the user's `~/.claude/projects/` (the probe's stood under `coord/blindtest/home/.claude/projects/`).
3. **Every session's standard input is `/dev/null`**, so no session reads `order.txt` from the loop, as all
   51 of panel 251's did (finding 2).
4. **`TMPDIR` and `CLAUDE_CODE_TMPDIR` name the area's `tmp/`** (the critic's finding 5: panel 251's
   session `t4-f-2` left `cache-break-state-5ddf4b53-....json` in `/private/tmp/claude-501/`, outside the
   root; the CLI 2.1.311 names `CLAUDE_CODE_TMPDIR` in its own error text, the critic's `strings`).
   **Whether these two variables move that file into the area is unrun**: no session has run with them. So
   the runner's **free preflight** asks it (below), and after every session the runner looks under
   `/private/tmp/claude-<uid>/` for a file naming that session's id and stops at the first.

`tools/check_area.py` asks all four of the area before any spend and the runner stops on its refusal.
**What still reaches a session**, recorded and not repaired: the folder's path
(`.../heroes-lang/.claude/worktrees/rebel-scratch/253-llm-ergonomist/s/<task>-<label>-<n>`) and the CLI's
own system prompt; the label names no arm.

**`tools/scan.py`, repaired** (the critic's finding 4: two of panel 251's 51 transcripts, `t1-x-4` and
`t3-c-2`, flagged `CLAUDE.md` from their own `context` answers, an `assistant` entry): it reads only what
reached the session, never what the session wrote: every `assistant` entry is skipped, so is a request
blob whose message's role is `assistant` (251's transcripts hold 102 such blobs beside 51 `system` and 50
`user` ones, counted by `python3 -I`), and of a `user` entry it reads `message` and never `toolUseResult`,
the CLI's copy that for a write holds the written text. **Its controls, run by the writer at no cost**:

- **panel 251's 51 transcripts** (`python3 -I tools/scan.py
  .../rebel-scratch/251-llm-ergonomist/transcripts`, `bw/scan-control-251.txt`): **51 of 51
  `prompt-alone=NO`, 51 of 51 `git-status=LEAK`, 0 `claude-md-or-memory=YES`** (the old reading,
  every entry searched, flags `t1-x-4` and `t3-c-2`: reproduced by `python3 -I` over the files); so the two
  known leaks are seen and the false flag is gone;
- **the coordinator's probe** (`coord/blindtest/home/.claude/projects/`): `prompt-alone=NO` (its prompt was
  *"Say hi."*), `git-status=reader, no commit`, `claude-md-or-memory=no`;
- **a transcript holding the real prompt and no leak has never been read**: the runner's preflight makes
  the first, and refuses to spend unless the scan reads it `prompt-alone=yes git-status=reader, no commit
  claude-md-or-memory=no`.

**Its layout** (`find . -path ./.git -prune -o -type f -print`, `find s -type f | wc -l`):

- `arms/`: `spec-now.md`, `spec-doc-r.md`, `spec-skip-r.md`, `spec-umb-r.md`, `spec-all-r.md`, copied from
  `bw/arms/`, their sha256 in `tools/arms.sha256`, asserted by `make_sessions.py`; **unchanged by the
  repairs**;
- `tools/`: `brief-session.md` and `brief-verdict.md` (**panel 251's, the same bytes**: sha256
  `724e8e0d656bb7b0...` and `52d37bb04ac8ab20...`), the three tasks, `make_sessions.py`, `check_folders.py`
  (panel 251's, its count 51 made 31), `check_area.py`, `scan.py`, `spent.py` and `after.py` (panel 251's);
- `s/<task>-<label>-<n>/`: **30 session folders**, each holding `brief.md`, `task.md` and `heroes-spec.md`
  and nothing else (90 files; none holds `run.json`);
- `verdict/`: `brief.md`, `task.md` (task t6), `spec-A.md` (the frozen spec), `spec-B.md` (`all-r`: the three
  robust sentences together) and `changes.diff` (headers `spec-A.md` and `spec-B.md`, so it names no arm),
  **never graded**;
- `preflight/`, made by the runner; `home/` and `tmp/` (empty), `.gitignore`, the area's `.git/`;
  `key.txt` and `order.txt` beside `s/`, never inside it; `run.sh`.

| file | sha256, first 16 |
|---|---|
| `tools/task-t2.md` | `2fcb237358b4530c` |
| `tools/task-t4.md` | `3200048e07a92841` |
| `tools/task-t6.md` | `9c2f39400c2fad28` |
| `arms/spec-now.md` | `1d532d514831aa70` |
| `arms/spec-doc-r.md` | `656b8364f33abaeb` |
| `arms/spec-skip-r.md` | `a47a36c1f157cd29` |
| `arms/spec-umb-r.md` | `07acf83d2ec062f9` |
| `arms/spec-all-r.md` | `48d807010b254507` |
| `run.sh` | `389486a082cb464f` |
| `key.txt` | `65ddcb38e1a3c107` |
| `order.txt` | `6ac50bfb3db4e96a` |
| `tools/scan.py` | `41aae8419d682a6d` |
| `tools/check_area.py` | `083089c60775136c` |

**The draw**, made by `tools/make_sessions.py` before it made any folder, `secrets.SystemRandom().sample`
over `cfghjkmpqvwxyz` (no letter naming a route or an arm): **`now->g doc-r->m skip-r->f umb-r->q`**. **Two
earlier draws were discarded with their folders before any run**: *"now->x doc-r->p skip-r->y umb-r->f"*,
when task t6's first wording asked about a `use` line nothing in the program read, which every build
refuses (`error[unused_binding]`, exit 1, `bw/ref/t6/unused.hero`, `unused.txt`); and *"now->h doc-r->v
skip-r->x umb-r->m"*, when the critic found t6's packages printed in the arm's own example, t4's wording
forbidding the hazard its row counted and t2's wording deciding its own `now` cell (findings 1 to 3).
Every session's specification is named `heroes-spec.md`, and no session folder holds a diff. The
project-name `grep` (`panel|defect|sitting|coordinator|claude|anthropic|design\.md|seat|ledger|worktree|
joseph|heroes-lang|golden|selfhost|spec-warden|ergonomist|thesis|blind|253|robust`, case folded) over
every session's `brief.md` and `task.md` and the verdict's files prints 0 files.

**The cells**, 6, each with 5 replicates (`ls s | grep -c '^t<n>-'`: 10, 10, 10):

| task | arm | label | what the reader holds |
|---|---|---|---|
| t2 | `now` | g | the frozen spec: *"A comment directly above a declaration documents it; `##` is a section heading"* |
| t2 | `doc-r` | m | § 1's comment bullet with the shape, the example rule and `heroes doc` (`spec-doc-r.diff`) |
| t4 | `now` | g | the frozen spec: a test passes or fails |
| t4 | `skip-r` | f | § 12 with `skip(reason:)` (`spec-skip-r.diff`) |
| t6 | `now` | g | the frozen spec: one catalogue entry per package, nothing said of what is placed |
| t6 | `umb-r` | q | § 1 with an umbrella's `packages` (`spec-umb-r.diff`) |

**The tasks' shapes, chosen so the input tells the arms apart and dictates no answer** (panel 251's task
2 held no separator and could not show its hazard, `critic-reports.md` finding 3; this sitting's first
wordings dictated three answers, the critic's findings 1 to 3):

- **t2** asks for documentation that shows *"what it answers, what each parameter is, and an example"*,
  naming no form, so a reader of `now` writes its default and a reader of `doc-r` has the form in its spec;
- **t4**'s `test_server()` names no server *"on this machine, as written above"*, so every answer's test
  meets the absent case when run, and the task says only that there *"the test cannot check the
  connection"*: it neither asks nor forbids a pass;
- **t6** names `yaml` and `template` at version `0.4.0` and adds `time` with a call, none of which the
  arm's example or sentence names (`spec-umb-r.diff` names `csv`, `json`, `flags` and `0.1.0`; `grep` of the
  task for `csv`, `flags` and `0.1.0` prints nothing); it names `json.Value` as the type the two packages
  use, and the arm's example lists `json`, so a reader of `umb-r` who lists `json` is recorded, not graded
  wrong.

## The order, the runner and the bound

**`order.txt`**: five rounds over the six cells, odd rounds forward and even rounds reversed, round k
carrying replicate k, so a stop leaves every cell within one replicate of every other; **the verdict
session last**.

**`run.sh`**, run by the coordinator alone:

```
bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/253-llm-ergonomist/run.sh
```

In order, before spending anything:

1. it holds `run.lock`, so one runner at a time;
2. **the predictions gate** (the critic's finding 6): it refuses to start unless
   `.claude/worktrees/rebel-round/docs/panel/253-reports/spec-warden.md` holds the line `## Predictions
   registered before the blind run` **with at least one non-empty line under it before the next `## `
   heading**; it then **copies the report as it stands** to `warden-at-start.md` beside `run.log` (a resumed
   runner keeps that first copy and writes `warden-at-resume-<time>.md`) and logs **the copy's** sha256, so
   the predictions the rules score can be read later from a file that does not move;
3. it asks `tools/check_folders.py` that `order.txt` names 31 distinct folders, each in `key.txt`, and that
   every folder not yet run holds exactly its registered files at their registered sha256; and
   `tools/check_area.py` the four repairs;
4. **the free preflight**: one session in `preflight/` (a copy of the session brief alone), with the real
   prompt and every flag a graded session takes, **whose key is a string the API refuses**
   (`sk-ant-api03-refused-by-design-panel-preflight`; the coordinator's probes, made so, each logged
   `total_cost_usd: 0` and `terminal_reason: api_error`, read in `coord/blindtest/s/probe*/run.json`). It
   lists `/private/tmp/claude-<uid>/` before and after into `preflight/cli-tmp-before.txt`, `-after.txt`
   and their `diff` (other sessions on this Mac write there too: 29 entries at 15:39, `ls | wc -l`, so the
   listing is read by a person), and **refuses to go on** unless the preflight cost 0, no file naming its
   session id stands under `/private/tmp/claude-<uid>/`, and `tools/scan.py` reads its transcript clean;
5. it loads the key with `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a`, prints its length
   alone, and takes `caffeinate -i -w` for its own life.

Then for each folder of `order.txt`, one at a time and never two: a folder holding `run.json` is skipped
and its cost counted, so a stopped run resumes; **a session starts only while the spend so far**
(`tools/spent.py`: every `run.json`'s `total_cost_usd` summed, an unreadable one counted as 0.50) **plus
its cap plus a 0.10 margin is at most 9.50**, the cap 0.40 for a graded session and **0.60 for the
verdict** (the critic's finding 15: panel 251's verdict cost 0.3771674 of its 0.40); the session runs from
its folder as

```
env -i PATH="$PATH" HOME="$HERE/home" LANG=en_US.UTF-8 \
    TMPDIR="$HERE/tmp" CLAUDE_CODE_TMPDIR="$HERE/tmp" \
    GIT_CEILING_DIRECTORIES="$(dirname "$HERE")" ANTHROPIC_API_KEY="$3" \
  claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
    --bare --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5 \
    --tools "Read,Write" --allowedTools "Read,Write" \
    --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
    --max-budget-usd "$2" --output-format json < /dev/null > run.json 2> run.err
```

(`claude --version` printed `2.1.311 (Claude Code)`, and its `--help` names every flag above, `grep -o`,
run), then `tools/after.py` saves the CLI's `result` as `result.md` and appends one line to `run.log`, and
the runner stops if a file naming the session's id stands under `/private/tmp/claude-<uid>/`. After the
last session it prints the spend and runs `tools/scan.py` over `home/.claude/projects/`.

**The bound**: 10 USD for the sitting (the author's, as the coordinator relays it); the caps alone bound
the 31 sessions at 12.60 USD, so **the stop rule is what holds the ceiling**. **What it will cost, an
inference**: panel 251's 50 graded sessions, each one specification and one task, cost 0.1322 to 0.2112
USD, median 0.1619, sum 8.313339, and its verdict 0.377167 (`python3 -I` over
`rebel-scratch/251-llm-ergonomist/s/*/run.json` and `verdict/run.json`, run); 30 at that median and one
verdict is about **5.24 USD**, 30 at that maximum and one verdict **6.71 USD** (the critic's arithmetic,
finding 16, reproduced); a graded cell is lost only if the graded sessions average above 0.30 USD, 1.42
times panel 251's most expensive. The preflight costs nothing by construction, and the runner checks it.

## The grading, registered before any run

**Where the answer is read** (the critic's finding 16): every one of panel 251's 51 sessions logged
`report.md=no` (its `run.log`), one saying *"My only tool can read files, not write them"*, and this
runner's flags are the same; so each session is graded from its **`result.md`** (the CLI's `result`,
saved by `after.py`), and from `report.md` only where one exists and says more. The files of `answer` are
taken whole (the longest fenced block under each file's path), the predictions from `prediction`,
`prediction` winning where it and `answer` differ; a line given as *maybe* or *depending* recorded as
**hedged**. **Built with the writer's stage 2 of the frozen commit** (`bw/tree/heroes-next`, `sources
23c7d2a626b29fd5`) wherever the answer is buildable today, each run under `timeout -s KILL 20`; the
writer's references, each run so: `bw/ref/t2/words.hero` (`check` exit 0, `test` *"1 test, all passed"*),
`bw/ref/t4/shop.hero` (fails where no server is named: `FAIL`, *"1 test, 1 failed"*, exit 1),
`bw/ref/t4/trap.hero` (prints and ends: **`ok`, *"1 test, all passed"*, exit 0**), `bw/ref/t6b/page.hero`
over a `path` entry (builds, exit 0; with `data.yaml` and `page.tmpl` prints *"Ziggy sang on Low"*, exit
0, `build.txt`), and `bw/ref/t6b/page_time.hero` with `use <rebel>/time` and a call of `time.now` added
(builds, exit 0, `with-time-used.txt`).

| task | arm | right iff |
|---|---|---|
| t2 | now and doc-r | `words.hero` is accepted by `heroes-next check` (exit 0); a comment stands directly above `function count_words`, **its first line holding a sentence that ends with a period on that line**; after that line or lines, a line holding `#` alone, then exactly the two lines opening `` # - `text`: `` and `` # - `sep`: ``, in that order; a `test` whose title opens `count_words:` has a comment directly above it, and `heroes-next test words.hero` reports it `ok`. **Recorded either way, the `now` cell's measurement**: a code fence inside a comment (the doctest form panel 136 refused), a parameter written another way (`@param`, a table, prose), the example written anywhere but a `test`, and which spec rule the answer's part 2 names |
| t4 | now | `shop.hero` builds; **the two given functions' lines stand in it byte for byte, trailing spaces aside**; `main` prints `shop`; and **the prediction of part 2 matches what `heroes-next test shop.hero` reports when run 3 times**: the test's outcome (`ok` or `FAIL`) and the exit status (0 or 1). **Recorded, the hazard**: every answer whose test reports `ok` when run, having checked no connection |
| t4 | skip-r | graded by the arm's words, since no compiler has `skip`: the test calls `skip(reason: <a str>)` on the path where `test_server()` fails, and on the other connects to the server it names and asserts success; nothing declares a `skip` of its own; the given lines stand byte for byte as above; the prediction says the test is reported skipped with its reason, neither passed nor failed, and the run ends with status 0. **Recorded**: the answer built with each `skip(reason: R)` written as `print(R)`, so the rest is held to the compiler (exit recorded, not gated), and every answer that still ends the absent case in silence |
| t6 | now | the `heroes add` commands are `heroes add rebel` (with or without a version) and none of `yaml`, `template` or `rebel/<package>`; `heroes.toml` holds one entry `[dependencies.rebel]` with `version = "0.4.0"` and a `sum`, and no field naming packages; `page.hero` writes `use <rebel>/yaml` and `use <rebel>/template`; question 4 predicts **accepted**. **Any other verb** (`heroes fetch`, `heroes new`, `heroes build`) among the commands is recorded, neither right nor wrong. Today's answer to 4: accepted under a `path` entry (run, above); under a fetched entry accepted by panel 250's R1 (*"every version placed whole"*), **unrun**, since Rebel is not in the index. **Recorded**: `page.hero` built over a `path` entry (not gated) |
| t6 | umb-r | the `heroes add` commands are `heroes add rebel/yaml` and `heroes add rebel/template` (either order; one command naming both recorded and accepted), any other verb recorded as above; `heroes.toml` holds one entry `[dependencies.rebel]` with `version = "0.4.0"`, `packages` holding `yaml` and `template` in any order (and `json` too only where `page.hero` writes `use <rebel>/json`; a `json` listed with no such line recorded, not wrong), and a `sum`; the `use` lines as above; question 4 predicts **refused**, the build saying to run `heroes add rebel/time` |

## The decision rules, counted and registered before any run

With 5 replicates per cell; where the stop rule leaves a cell short, each rule reads the replicates every
cell of its task reached, and a difference it names stays a count of sessions.

- **Q2, task t2.** `doc-r`'s sentence **suffices** when at least 4 of its 5 answers are right; it is
  **needed** for a reader to write the shape when `doc-r`'s right answers exceed `now`'s by at least 3.
  Since the task no longer asks for a form the frozen spec lacks, `now`'s count is a measurement of the
  default (the critic's finding 3); its five defaults are recorded whole.
- **Q4, task t4.** `skip-r`'s sentence **suffices** when at least 4 of its 5 answers are right. **The
  hazard under today's spec**: the count of `now` answers whose test reports `ok` when run; **2 or more of
  5** is recorded as *"a reader of today's spec writes the silent pass the outcome exists to refuse"*, 0 or
  1 as *"a reader of today's spec fails the test, or never writes one that passes in silence"*. **The
  outcome reduces the hazard** when `now`'s silent passes exceed `skip-r`'s answers that still end the
  absent case in silence by at least 3.
- **Q6, task t6.** `umb-r`'s sentence **suffices** when at least 4 of its 5 answers are right. Recorded, not
  a gate: `now`'s five answers to question 4 (accepted, refused, or fetch needed), what a reader of today's
  spec assumes of placement, a silence of § 1.
- **The spec-warden's registered predictions** are scored on these counts as they are worded, and only
  those under its heading in `warden-at-start.md`, the copy the runner made when it started.
- **The verdict session** is recorded whole in the report, its `verdict` quoted, and grades nothing.
- **A session is voided**, recorded and its cell read without it, where it answers `context: yes`, **or
  where `tools/scan.py` reads a leak in its transcript** (its first user entry not the prompt alone, a git
  status naming a commit or a person other than `reader`, `CLAUDE.md` or a memory index in what reached it):
  panel 251's 51 sessions all answered `context: no` while each held the main checkout's status (finding 2
  of its critic), so the answer alone is not the measurement.

## After the run

`run.log`, `warden-at-start.md`, `preflight/` (its `run.json`, `cli-tmp-*.txt`), each `run.json`'s cost,
model and `subtype`, `tools/scan.py`'s lines, a listing of `tmp/` and every `result.md` (and `report.md`
where one exists) are copied into `.claude/worktrees/rebel-round/docs/panel/253-reports/llm-ergonomist.md`
under a header saying how the sessions were run, with every `context` answer quoted and each session's
grade by the rows above, the builds' commands and exits beside them.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/253-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out. `coord/blindtest/`, the coordinator's preflight of a blind session, is in no archive and
is gone.
