# Panel 250, llm-ergonomist: the blind seat, registered before any session

Written 2026-11-15 between about 02:00 and 02:30 for the coordinator, who runs the sessions and grades
them. The author approved this seat on 2026-11-15 with a ceiling of **10 USD for all its paid sessions
together** (as the coordinator relays it). **Unrun when written**: the writer ran no session and made no
paid call (no `claude -p`, no API call, no `heroes measure --refresh`); every number below comes from a
command run while writing, named beside it. Everything below the line *The seat's brief* is copied
verbatim into the session folders and carries none of the project's names; everything above it is for
the coordinator. Every path is under `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/`; `S/`
below is its `p250-scratch/`.

## What it measures

**Does a reader holding only the specification write, build and predict a program correctly when the
program adds a `use` of a module of a package it already holds, with p0 and with p1r; and does p1r's
sentence suffice for a reader to get it right?** Panel 244's R1 set the bar a package sentence of the
spec must clear (*"where a reader holding only the specification would otherwise write a wrong program
or be unable to predict a refusal, measured"*, `244-*.md:22-25`), and the spec-warden's report names
this reading as owed (`250-reports/spec-warden.md` § Q2, *"The measurement 244's bar asks is a blind
reading"*, and § Q10's registered prediction).

**Two worlds, both run here before any session** (§ Grading has the setups and every output):

- **W, today**: the base's stage 2. A `use <text>/strconv` written after the package was placed builds
  at once; `heroes fetch` before it is harmless (`kept text 0.5.0 ...`, exit 0).
- **M, a program places only the modules it reaches**: the compiler-engineer's prototype `ce-m2` v2
  (`250-reports/compiler-engineer.md` § The prototype's second build). The same edit is refused, exit 1:
  *"error[module_unplaced]: the module `strconv` of the package `text` is in the version the catalogue
  names and not placed under `heroes.packages/text@0.5.0-554b61376b526680/`: `heroes fetch` places every
  module the program's `use` lines reach, and `build`, `check` and `run` never fetch"*, at the `use`
  line; after `heroes fetch` it builds and prints `3`, `12`. **Running `heroes add text` again also
  repairs it under this prototype** (exit 0, *"wrote heroes.toml"*, the catalogue unchanged by `diff`,
  `strconv.hero` and `strconv/internal/shown.hero` then placed): measured, and a property of the
  prototype, not of any ratified route.

**So p0 is right in W and p1r is meant for M.** Every answer is graded in both worlds, four cells per
task (arm d in W and in M, arm h in W and in M), and the rules below read named cells.

**Two arms**, each a whole specification, label-stripped (every session folder holds it as
`heroes-spec.md`). The labels and the assignment were drawn at random before the folders were made,
`python3 -I -c` with `secrets.choice` over the letters `cdfhjkuv` (none an arm of an earlier blind seat,
none naming a route or draft here), which printed *"labels ['d', 'h'] p0 -> d p1r -> h"*:

| arm | the specification | sha256 | vendored (legacy / cl100k / max) |
|---|---|---|---|
| d | **p0**: `git show 48f53b3437c6cd46aba5923f4f4780ac7b347283:spec/heroes-spec.md`, unchanged | `7dc8bdaa3692af173dbdfcfd94751cca023bce029668fbfb192db0b3b68630ef` | 8483 / 8634 / 8634 |
| h | **p1r**: p0 with the spec-warden's p1r sentence | `2bba46630042d0af95b30376578de174fdb8944f27574819a3f9c4041d7e98bb` | 8513 / 8664 / 8664 (+30 / +30) |

**How each arm is made**: `S/tools/make-sessions.py` (sha256 first 16 `52a37ba22573cc72`, run once with
`python3 -I`) takes p0 from `git show` and makes p1r by replacing the one occurrence of
`` `heroes add json` writes that entry; `` (asserted once) with *"`heroes add json` writes that entry, and
`heroes fetch` places the modules the `use` lines reach, so a `use` written after it is refused until it
runs again;"*, as the spec-warden's `scripts/drafts.py` applies its drafts. The script asserts p0 equal to
the spec-warden's `250-spec-warden/drafts/x.md` and p1r equal to its `drafts/p1r.md`, byte for byte, and
both held. `diff S/arms/spec-d.md S/arms/spec-h.md` prints one change, line 35, the sentence alone.
`grep -c fetch`: 0 lines in arm d, 1 in arm h; `grep -c 'heroes add'`: 1 in each. **So a reader of p0
meets no verb named `fetch`; whatever it writes for the edit comes from `heroes add` or its own guess.**
Counts by the base's stage 2, `heroes measure S/arms/spec-<arm>.md` from `S/grade/measure/` (the base's
`vendor/tokenizers`, `git archive`), vendored, a lower bound, the spec-warden's figures exactly.

## The tasks, and why each can go wrong under one arm

The package is the base's `core` 0.5.0 published under the neutral name **`text`** (its files, `list`
and sum unchanged), and a second package **`units`** 0.1.0, one module, published for t3, both in the
grading index (§ Grading). The texts are below, verbatim; the program and catalogue each task shows are
asserted by `make-sessions.py` to be the grading fixtures' own, byte for byte.

- **t1, the edit, written**: the program uses `strings`; add the sum of the numbers through `strconv`, a
  module of the same package; give `main.hero` and every command until it has run. Under M a reader of
  p0 plausibly writes `heroes build main.hero` alone, which is refused.
- **t2, the edit, predicted**: the edited program is given; does `heroes build main.hero` succeed, and
  if not, which commands make it? The bar's other half, *"unable to predict a refusal"*, with no program
  to write.
- **t3, a second package**: add a call into `units`, not yet in the catalogue. Both worlds need `heroes
  add units` (`heroes fetch` alone is refused `unknown_entry` in both, measured). A reader of p1r, told
  that *"`heroes fetch` places the modules the `use` lines reach"*, plausibly runs `fetch` instead: the
  task where the sentence can hurt, so the comparison is two-sided.

**Every reference and trap answer, run in both worlds before any session**
(`S/grade/tools/run-answer.sh`, outputs in `S/grade/ref/<world>-<case>/`):

| case | commands, in order | W | M |
|---|---|---|---|
| t1 reference | `heroes fetch`, `heroes build main.hero` | right, `3/12/` | right, `3/12/` |
| t1 trap | `heroes build main.hero` | right, `3/12/` | **build exit 1, `module_unplaced`** |
| t1 | `heroes check`, `heroes fetch`, `heroes build` | check 0, right | **check exit 1 `module_unplaced`**, then right |
| t1 | `heroes add text --index file://../../index`, build | right | right |
| t1 | `heroes add text`, build | right | right |
| t1 | `heroes fetch`, `heroes run main.hero` | right, `3/12/` | right, `3/12/` |
| t2 reference | build, `heroes fetch`, build | first build 0 | **first build exit 1 `module_unplaced`**, last 0, `3/12/` |
| t2 | build, `heroes add text`, build | first build 0 | first exit 1, last 0 |
| t3 reference | `heroes add units --index file://../../index`, build | right, `3/3000/` | right, `3/3000/` |
| t3 | `heroes add units`, build | right | right |
| t3 | `heroes add units --index ...`, `heroes fetch`, build | right | right |
| t3 trap | `heroes fetch`, build | **build exit 1 `unknown_entry`** | **build exit 1 `unknown_entry`** |
| t3 trap | build alone | **exit 1 `unknown_entry`** | **exit 1 `unknown_entry`** |

`unknown_entry`'s text in W: *"`<units>` is no entry of the catalogue `heroes.toml`, whose entries are
`text` — `heroes add units` writes one"*. **The instrument separates every trap from its reference**, so
no right answer is graded wrong by it (the lesson of panel 247's critic, its item 15).

## The sessions, their folders, their order and their bound

**24 sessions**: t1, t2 and t3 under each arm, four each. One folder per session, **`S/s/<task>-<arm>-<n>/`**
(inside the repository's root and ignored by git, `git check-ignore -v` naming `.gitignore:34:.claude/worktrees/`,
as panels 244, 246 and 247 kept theirs, CLAUDE.md § Hard stops forbidding any file outside the root),
each holding `brief.md`, `task.md` and `heroes-spec.md` and nothing else (`find S/s -type f | wc -l`: 72).
The folder's name carries the arm's letter, which says nothing; **the key is `S/key.txt`, beside `s/` and
never inside it**, one row per folder: its arm, p0 or p1r, and the sha256 of its three files; the order
is `S/order.txt`. Each session's working directory, which the session's system prompt shows, is
`.../heroes-lang/.claude/worktrees/p250-scratch/s/<folder>`: the language's name and the sitting's
scratch folder are visible, as `p247-scratch/s/<hex>` was to panel 247's 96 sessions.

Files, sha256 first 16: `brief.md` `8754f103cd4caccd` in all 24; `task.md` t1 `30c06606416eb4de`, t2
`10944eb2284e42cb`, t3 `0b367a43df7d74f9`; `heroes-spec.md` `7dc8bdaa3692af17` in the 12 folders of arm
d and `2bba46630042d0af` in the 12 of arm h (`shasum -a 256 S/s/*/heroes-spec.md | ... | uniq -c`).

**The order**, registered: four rounds over the six cells `t1-d t1-h t2-d t2-h t3-d t3-h`, odd rounds
forward and even reversed, round k carrying replicate k, so a stop leaves every cell represented:

`t1-d-1 t1-h-1 t2-d-1 t2-h-1 t3-d-1 t3-h-1 t3-h-2 t3-d-2 t2-h-2 t2-d-2 t1-h-2 t1-d-2 t1-d-3 t1-h-3
t2-d-3 t2-h-3 t3-d-3 t3-h-3 t3-h-4 t3-d-4 t2-h-4 t2-d-4 t1-h-4 t1-d-4`

**One session at a time, never two**, by `S/run.sh` (sha256 first 16 `5f8229ec34d39497`; `bash -n` clean;
**not run**). Before spending anything it checks that every folder not yet run holds its three registered
files and no other, against `key.txt`'s hashes (refused, *"nothing run"*, when a copy of `s/` with an
extra `report.md` was tried), that `order.txt` names 24 folders, and holds `S/run.lock` so two runs cannot
overlap; it asks `pmset -g assertions` and takes `caffeinate -i -w` only where nothing holds the machine
awake; a folder that already holds `run.json` is skipped and its cost counted, so a stopped run resumes.

**The command**, the skill's (`.claude/skills/panel/SKILL.md:160-166`, its prompt and every flag) with
panels 246's and 247's additions, run from each session's folder:

```
env -i PATH="$PATH" HOME="$HOME" LANG=en_US.UTF-8 \
    GIT_CEILING_DIRECTORIES="$(cd .. && pwd)" ANTHROPIC_API_KEY="$ANTHROPIC_API_KEY" \
  claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
    --bare --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5 \
    --tools "Read,Write" --allowedTools "Read,Write" \
    --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
    --max-budget-usd 0.40 --output-format json > run.json 2> run.err
```

**Why the additions, said so the coordinator can strike them**: the skill asks for a folder *"with no
`CLAUDE.md` in or above it"* and for the sitting's model (*"The `claude` command must support the
sitting's model, or the seat runs on another"*). A folder inside the repository has the repository's
`CLAUDE.md` above it, and the user's auto-memory is keyed to the project's path; `claude --help` of the
installed CLI (`claude --version`: `2.1.311 (Claude Code)`, its help kept in `S/grade/claude-help.txt`)
says `--bare` skips *"auto-memory ... and CLAUDE.md auto-discovery"*, `--safe-mode` disables *"CLAUDE.md,
skills, installed plugins, hooks, MCP servers"*, `--restricted` *"ignores user, project and local settings
files"* and confines the file tools to the working directory, and lists `--model`.
`GIT_CEILING_DIRECTORIES` keeps git from finding the repository above the folder, and `env -i` hands the
session nothing of the runner's environment but those five. Panel 247 ran this command 96 times; its
report reads *"40 of 40 answered `no`"* to the context question for its first forty
(`247-reports/llm-ergonomist.md:26`).

**The key**: `run.sh` loads it with `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a`,
prints its length alone, never its value, and passes it through `env -i` as above. The writer of this
brief neither read nor sourced `.env`.

**The bound, twice.** Each session is capped at **0.40 USD**, so **the 24 are bounded at 9.60 USD by their
caps alone** (`python3 -I -c`: 24 × 0.40 = 9.6), inside the author's 10. **A cap can be passed**: panel
247's session `62e9c1` ended `error_max_budget_usd` at **0.4959816 USD on a 0.40 cap**, 0.0959816 over,
with no report, its 21,249 output tokens 16,897 of them thinking (its `run.json`, read with `python3 -I`).
So `run.sh` starts a session only while the spend so far, summed from every `run.json` under `s/` (one
that cannot be read counted as 0.50), **plus 0.40 plus a margin of 0.10 is at most 10.00**: it starts at
9.50 spent and stops at 9.51 (tested). The total stays inside 10 USD while no session passes its cap by
more than 0.10. What earlier blind seats cost, read from their `run.json` files (`python3 -I` over each
glob): panel 247, 96 sessions, median 0.2060, largest that finished 0.3564, sum 21.0131; panel 246, 20,
median 0.1678, largest 0.3258; panel 244, 8, median 0.1294, largest 0.1347. At those medians the 24 would
cost about 4.03 to 4.94 USD (24 × 0.1678, 24 × 0.2060): an inference, not a measurement. Cost follows
the output and thinking tokens, not the spec's length (the sessions read the same spec at about 14,000
cache-written tokens). **No other paid run is named for this seat**; a retry of a session that ends at
its cap with no report is a new paid session, the coordinator's call, inside the same 10.

**The reports**: `run.sh` logs each session to `S/run.log` (exit, `subtype`, `total_cost_usd`, the model,
whether `report.md` was written). Take `result` from `run.json` where `report.md` is missing, and say
which (panels 244 and 246 found every report there, carried from panel 246's and 247's briefs, not
re-read here). The coordinator copies them into
`docs/panel/250-reports/llm-ergonomist.md` under a header saying how they were run, with each session's
cost and every `context` answer; **a `yes` voids that session until it is re-run**.

## Grading, registered before any session

### The setups, the index, the starting states, the instrument

All under `S/grade/`, made and run by the writer between about 02:10 and 02:20:

| setup | compiler (sha256, 16) | `--version` | runtime | what it is |
|---|---|---|---|---|
| **W** | `w/heroes`, `6dbc55c8d1ce611d`, copied from `S/briefs/base48/tree/heroes-next` | `sources 5bac791363384945`, the base's | `w/runtime`, copied from that tree | today, whole placement |
| **M** | `m/heroes`, `7fb56c9a07b8f1ab`, copied from `S/250-compiler-engineer/routeM/ce-m2` (dated 01:58) | `sources 1593fa56260f5ec3`, the report's v2 | `m/runtime`, copied from `routeM/runtime` | route M with F1, placement by reach |

Each pinned by `HEROES_RUNTIME`, and `heroes doctor` names each copy's own runtime. `diff -rq w/runtime
m/runtime` prints nothing; the sha256 of each runtime's sorted per-file sha256 list is `b08d8cf8329de7ec`
for both. If the compiler-engineer rebuilds its prototype, the grading copy stays v2.

- **The index**, `index/`: `text/` is `git archive 48f53b343 packages/core` renamed, its `versions` line
  `0.5.0 sha256-38e6194310472ac1e5779c6bf0fd30c0b725db0db8bcf149cb44f97cbde04204`, its `list` rebuilt by
  README § 10's two lines byte for byte (`cmp`); `units/` holds 0.1.0, one file, `metric.hero`
  (`efc3b5496f0dfe5a`, canonical by `heroes fmt`), published by the same two lines, `0.1.0
  sha256-936fb12b07b0b3981add2747cbe1b9c617cf63666ce632a0fc395b1ae0f7df07`.
- **The starting states**, `start/w/` and `start/m/`: `main.hero` = `src/start.hero`, then each world's
  own `heroes add text --index file://../../index` (both printed *"added text 0.5.0"*, *"wrote
  heroes.toml"*, exit 0, the two catalogues equal), then `heroes build main.hero` exit 0 and its binary
  printing `3`, then `build/` removed. W placed 30 files; M placed 9 (`strings.hero`,
  `strings/internal/scan.hero`, seven cards) and the listing beside the folder, 10 files under
  `heroes.packages/` (`find heroes.packages -type f | wc -l` in each). This is the history each
  task tells, in the order it tells it (the `use` line written before `heroes add`: under the prototype,
  an `add` run before any `use` names the entry places the whole version, the report's `addfirst`).
- **The instrument**, `tools/run-answer.sh` (`8145d0e2193cdd92`): `run-answer.sh <w|m> <start> <work>
  <main.hero to write, or -> <command> ...` copies the starting state, writes the file, runs each command
  beginning `heroes ` with the setup's compiler in order (any other recorded and skipped), runs the binary
  a build at exit 0 wrote, every compile under `timeout -s KILL 300`, every binary under `timeout -s KILL
  60`, every stream through `head -c 65536`, and prints one line per command: exit, first `error[...]`
  code, the binary's exit and its lines joined by `/`. **The binary is at `build/<key>/main`**, never
  `./main` (measured), so a reader's `./main` line decides nothing. Work folders go under `runs/`, two
  levels below `grade/`, where the catalogue's `file://../../index` resolves.

### What is read from a report, the same way by anyone

1. **The commands** are the lines of the report's `answer` that stand in a code block or as an item of a
   numbered or bulleted list of steps and begin, once leading white space and a `$ ` or `% ` prompt are
   taken off, with `heroes `; in the order they stand; each as written. `prediction` is read for them only
   where `answer` gives none. A command named in prose, not as a step, is not one.
2. **A conditional command** (*"if it is refused, run ..."*) is run only where its condition holds when
   graded, and its outcome is not checked; the answer is recorded **hedged**.
3. **A command's predicted outcome** is what the report says the tool does with it, accepted or refused,
   anywhere in the report, `prediction` winning where two places differ; a command with no stated outcome
   counts as predicted accepted, since each task asks for every outcome.
4. **`main.hero`** is the file the answer gives under that name (t1, t3); none, and P is wrong.
5. For **t2**: question 1's word, accepted or refused (a report that says neither is wrong in both
   worlds), and question 2's commands by rule 1.

### t1 and t3, each answer in each world S (W and M)

- **C, the commands**: `run-answer.sh S start/S runs/<folder>-S-c <the task's reference main.hero>
  <the answer's commands>`, the reference being `src/t1-ref.hero` (t1) or `src/t3-ref.hero` (t3), which
  reach exactly the modules the task asks for, so the commands alone decide. **C is right** iff (i) every
  command whose outcome the answer states unconditionally gets it, accepted being exit 0 and refused any
  other, and (ii) the last `heroes build` or `heroes run` of the list exits 0 and the program prints
  exactly the reference's lines, `3/12/` for t1 and `3/3000/` for t3. A list with no build or run: C wrong.
- **P, the program**: `run-answer.sh S start/S runs/<folder>-S-p <the answer's main.hero> <the reference
  commands>`, which are `heroes fetch`, `heroes build main.hero` for t1 and `heroes add units --index
  file://../../index`, `heroes fetch`, `heroes build main.hero` for t3 (both right in both worlds, table
  above). **P is right** iff the last build exits 0 and its binary exits 0 printing exactly those lines.
- **right** = C and P.

### t2, each answer in each world S

`run-answer.sh S start/S runs/<folder>-S src/t1-ref.hero "heroes build main.hero" <question 2's commands>
"heroes build main.hero"` (`src/t1-ref.hero` is t2's edited program, asserted equal to the task's).
**Right** iff (i) question 1's word matches the first build, accepted at exit 0 (W, measured) or refused
(M, measured exit 1 `module_unplaced`), and (ii) where the first build was refused, the last build exits
0 and prints `3/12/`. t2 has no P; C is its grade. The output predicted where the answer says accepted,
and the reason given where it says refused, are recorded, not graded.

### Recorded beside every grade

The class of a wrong C: **unforeseen** (a command predicted accepted was refused), **invented** (one
predicted refused was accepted), **unrepaired** (the last build or run failed with neither), **program**
(P wrong); **hedged** where rule 2 applied. For t1 and t2 in M, **which command repaired the refusal**
(`heroes fetch`, `heroes add`, or none), so the synthesis can re-grade, with no new session, under an M
whose `add` places nothing over an existing entry (unbuilt: the prototype's re-placing `add` is measured,
not ratified). For t3, **which verb made the entry** (`add`, `fetch` alone, none). Each answer's
`choice_points` that name placement, `heroes add` or `heroes fetch`, quoted.

### The decision rules, registered before any session

- **Q1, is p1r needed if M lands** (244's bar): read in M. **p1r enters with M only if arm h gives at
  least four more C-right answers than arm d over its twelve sessions** (t1, t2 and t3 together);
  otherwise the reading is that a reader of p0 meets M no worse than a reader of p1r, within what chance
  makes, and the sentence waits, the refusal's message carrying the fix (M's names `heroes fetch`,
  measured above). **Its reason**: over three tasks of four sessions per cell, one changed answer per task
  makes a difference of three, so the sentence enters only on a difference no single answer per task can
  produce (panel 247's reason for *three* over two tasks, carried one task further). t3 counts with its
  sign: a sentence that helps t1 and t2 and hurts t3 is read net.
- **Q2, does p1r suffice**: read in M. **p1r suffices as worded if arm h is C-right in at least 6 of its
  8 t1 and t2 sessions and in at least 3 of its 4 t3 sessions**; otherwise the synthesis says it does not,
  quotes the failing answers' `choice_points`, and a rewording owes its own reading.
- **The spec-warden's registered prediction** (`250-reports/spec-warden.md:433-435`, *"with p1r, at least
  7 of 10 blind runs given the measured edit under a built M run `heroes fetch` before their first
  `check`; with p0, at most 3 of 10"*), scored on t1 and t2, eight sessions per arm, its 7 and 3 of 10
  read as fractions: **at least 6 of 8 for arm h, at most 2 of 8 for arm d** (0.7 × 8 = 5.6, 0.3 × 8 =
  2.4, `python3 -I -c`). An answer counts when, in t1, a `heroes fetch` stands before its first `heroes
  check`, `build` or `run`, and in t2, question 1 says refused and question 2's commands hold `heroes
  fetch`. `heroes add` run again counts for C (it repairs under the prototype) and not for this
  prediction, which names `fetch`.
- **Recorded, not a gate: what each route costs a first-try reader.** `right` (C and P) for arm d in W
  (today: route W with p0 standing, as 244 ruled) against arm h in M (route M with its sentence), per task
  and over twelve; and arm d in M against arm d in W (what M costs a reader of today's spec with no
  sentence). Arm h in W is recorded as what a reader of p1r does in a world it would be false in. A gap of
  four or more over twelve between the first two is written as M with p1r costing (or saving) that many
  first-try answers; a smaller one as within chance.

## Said by the writer, against itself

- **One file was written outside the repository's root**, at 02:06: the base spec, by a redirection into
  the session scratchpad (`/private/tmp/claude-501/...scratchpad/p0.md`) to hash it; removed at once, `ls`
  of the folder showing it gone and three files that predate this session, not touched. Nothing else.
- **The grading of M is one prototype's.** Its `add` places the whole version when no `use` names the
  entry and re-places the reach when run over an existing entry, the order of the verbs deciding what is
  placed (the compiler-engineer's own words: *"placement under M is a function of the program's
  history"*). A route that pruned on `fetch` or left `add` alone would grade some answers otherwise; the
  record above lets the synthesis re-grade.
- **What a session cannot be told**: under p0 the verb `fetch` exists nowhere in its inputs, so Q1's arm
  d in M measures whether readers guess a verb or re-run `heroes add`, not whether they read a sentence.
  That is the bar's own question, *"a reader holding only the specification"*.

---

## The seat's brief (`brief.md`, verbatim)

```markdown
# Your brief

You judge one thing: whether the language's specification, read with no other file, lets a language
model predict what the compiler and its tool do with a program, and write the program and the
commands its author meant, on the first try. Your verdict is an experiment, not an opinion: **write
the code, don't opine.**

## Your inputs, and nothing else

The files in this directory are your whole input. Read no other file, and do not ask for one.

- `heroes-spec.md`: the language's specification, the whole language.
- `task.md`: one task.

## Your method

Read the task and do what it asks. Write every file you write or change whole, with its path, and
every command in the order you would run it. For every place where the specification left you a
choice, say the choice you made and what the other would produce: a program or a command the tool
refuses, a program that stops with an error when run, or a program that runs and gives a different
answer than intended.

## Your report, `report.md` in this directory, in exactly this structure

- `answer`: what the task asks: every file, whole, each under its path, and every command, in the
  order you would run it.
- `choice_points`: every place the specification left you a choice, the choice you made, and what
  the other would produce.
- `prediction`: for each command, whether the tool accepts it and what it prints, and what the
  program prints when it runs.
- `context`: whether any instruction, rule, contract, memory or document other than the files in
  this directory reached your context, and which.

If you cannot write `report.md`, give the whole report as your final answer instead.
```

It is panels 246's and 247's brief with the agent definition's method (*"write the code, don't opine"*,
the choices and what each other choice would produce, `context`) and two changes for tasks that are
commands as well as code: *"and the commands its author meant"*, *"and every command in the order you
would run it"*, and `prediction` asking for each command's outcome. It asks for `choice_points`, never for
hesitations, as the skill says (`SKILL.md:180-186`).

## The tasks (`task.md`, verbatim, one per session)

**t1**

```markdown
# Task

A folder holds a program that builds and runs. Its author wrote `main.hero`:

    use <text>/strings

    function main()
        words = strings.fields("3 4 5")
        print(words.len())

then, in that folder, ran `heroes add text --index file://../../index`, which wrote this
`heroes.toml`:

    index = "file://../../index"

    [dependencies.text]
    version = "0.5.0"
    sum = "sha256-38e6194310472ac1e5779c6bf0fd30c0b725db0db8bcf149cb44f97cbde04204"

and then ran `heroes build main.hero`, which accepted it; the program printed `3`. Nothing in the
folder has changed since.

The package `text` holds, among others, these two modules:

- `strings`, with `fields(s: str) -> [str]`, called `strings.fields(s)`: the words of `s`, the
  pieces between runs of white space;
- `strconv`, with `atoi(s: str) -> i64?`, called `strconv.atoi(s)`: the integer `s` writes in
  decimal, failing where `s` is not one.

Change the program so that, after the count, it prints on a line of its own the sum of the numbers
the words write (here `12`). Give `main.hero` whole, then every command you would run in that
folder, in order, until the program has been built and has printed its two lines, and for each
command say whether it is accepted and what it prints.
```

**t2**

```markdown
# Task

A folder holds a program. Its author wrote `main.hero`:

    use <text>/strings

    function main()
        words = strings.fields("3 4 5")
        print(words.len())

then, in that folder, ran `heroes add text --index file://../../index`, which wrote this
`heroes.toml`:

    index = "file://../../index"

    [dependencies.text]
    version = "0.5.0"
    sum = "sha256-38e6194310472ac1e5779c6bf0fd30c0b725db0db8bcf149cb44f97cbde04204"

and then ran `heroes build main.hero`, which accepted it; the program printed `3`.

Today the author changed `main.hero` to this, and ran nothing else:

    use <text>/strconv
    use <text>/strings

    function main()
        words = strings.fields("3 4 5")
        print(words.len())
        total: i64 @= 0

        for w in words
            total @ total + strconv.atoi(w).default(0)

        print(total)

`strconv` is a module of the package `text`: `strconv.atoi(s)` answers the integer `s` writes in
decimal, failing where `s` is not one.

The author now runs `heroes build main.hero` in that folder.

1. Does the tool accept it? If it does, say what the program prints when it runs. If it does not,
   say what it refuses and why, citing the specification.
2. If it does not, give the fewest commands, in order, after which `heroes build main.hero` is
   accepted, with no file changed by hand.
```

**t3**

```markdown
# Task

A folder holds a program that builds and runs. Its author wrote `main.hero`:

    use <text>/strings

    function main()
        words = strings.fields("3 4 5")
        print(words.len())

then, in that folder, ran `heroes add text --index file://../../index`, which wrote this
`heroes.toml`:

    index = "file://../../index"

    [dependencies.text]
    version = "0.5.0"
    sum = "sha256-38e6194310472ac1e5779c6bf0fd30c0b725db0db8bcf149cb44f97cbde04204"

and then ran `heroes build main.hero`, which accepted it; the program printed `3`. Nothing in the
folder has changed since.

The same index also publishes the package `units`, which the program does not use yet. Its module
`metric` holds `km_to_m(km: i64) -> i64`, called `metric.km_to_m(km)`: the metres in `km`
kilometres.

Change the program so that, after the count, it prints on a line of its own the count read as
kilometres and given in metres (here `3000`). Give `main.hero` whole, then every command you would
run in that folder, in order, until the program has been built and has printed its two lines, and
for each command say whether it is accepted and what it prints.
```
