# Panel 250, llm-ergonomist: the blind seat's 24 sessions, graded by the registered rule

Graded on 2026-11-15 by a grader that ran no model session and made no paid call. The plan, the
arms, the tasks, the grading rule and the decision rules are `250-briefs/llm-ergonomist.md`,
registered before any session ran; this report applies them and decides nothing they leave open.
`S/` below is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p250-scratch/`.

## How the seat was run

- **The command**, `S/run.sh`, from each session's folder `S/s/<folder>/`, one session at a time in
  `S/order.txt`'s order:

  ```
  env -i PATH="$PATH" HOME="$HOME" LANG=en_US.UTF-8 \
      GIT_CEILING_DIRECTORIES="$(cd .. && pwd)" ANTHROPIC_API_KEY="$ANTHROPIC_API_KEY" \
    claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
      --bare --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5 \
      --tools "Read,Write" --allowedTools "Read,Write" \
      --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
      --max-budget-usd "$CAP" --output-format json > run.json 2> run.err
  ```

  with `CAP=0.40`, the key loaded from the repository's `.env` and printed by its length alone.
- **The CLI**: `claude --version` printed `2.1.311 (Claude Code)`, as `S/run.log`'s first line records.
- **The model**: `claude-opus-5-5`, the one key of `modelUsage` in each of the 24 `run.json` files
  (read with `python3 -I`).
- **The sessions**: 24, t1, t2 and t3 under arm d (p0) and arm h (p1r), four each. Every one ended
  exit 0 with `subtype success` (`run.log`; `run.json`), each `run.err` empty. The `run.json` files
  were last written between 07:58:53 (t1-d-1) and 08:25:44 (t1-d-4) on 2026-11-15 (`stat`); their
  `duration_ms` sum to 1,246,048 ms.
- **The spend**: **3.139598 USD** for the 24, as `run.log` prints and as the 24 `total_cost_usd`
  values sum when read again; median 0.130788, smallest 0.119682, largest 0.142985, every session
  inside its 0.40 cap and the whole inside the author's 10 USD.
- **Where the answers came from**: **no session wrote `report.md`** (`run.log` says `report.md no` 24
  times) and **each put its whole report in the `result` field of its `run.json`**, saying in its first
  line that it had no tool that writes files. `permission_denials` is empty in all 24, so the Write
  tool was not offered rather than refused; why `--tools "Read,Write"` gave the sessions no Write tool
  is not traced here, a question for the coordinator. Each `result` was copied byte for byte to
  `S/grade/answers/<folder>.md` (`python3 -I`, each file read back equal to its `result`), and the
  grading reads those files.
- **The instruments**, their sha256 read again before grading and equal to the brief's:
  `tools/run-answer.sh` `8145d0e2193cdd92`, W `w/heroes` `6dbc55c8d1ce611d` (`--version`: `sources
  5bac791363384945`), M `m/heroes` `7fb56c9a07b8f1ab` (`sources 1593fa56260f5ec3`), `src/t1-ref.hero`
  `33f840132d807a4b`, `src/t3-ref.hero` `183662d587130d78`; the two start catalogues equal by `diff`.
- **The runs**: 80 of them, every one by `tools/run-answer.sh`, each from a fresh copy of
  `start/<world>/` into its own folder `S/grade/runs/<folder>-<world>[-c|-p]`, none inside another,
  one after another (1 min 58 s of wall in all). The driver, which records each answer's commands as
  read below, is `S/grade/tools/grade-sessions.sh`; its log is `S/grade/runs/grade.log`; the
  programs, cut from each answer's `main.hero` block, are `S/grade/programs/<folder>.main.hero`.
  Every program here ends; each compile ran under the instrument's `timeout -s KILL 300`, each binary
  under `timeout -s KILL 60`, every stream through `head -c 65536`, and `pgrep` found no binary still
  running after the last run.

### How the rule was read where it needed reading

1. **A command written as a list item's inline code**, ``1. `heroes build main.hero` ``, is read as
   that code's text. Every answer writes its steps this way.
2. **`./main`** stands as a step in 13 answers (t1-d-1 to 4, t1-h-1, t1-h-3, t1-h-4, t2-d-3, t3-d-1 to
   4, t3-h-4; `grep` over each `answer` section). It does not begin with `heroes `, so by rule 1 it is
   not a command; the brief says it decides nothing, since the binary is at `build/<key>/main`.
3. **t2, question 2's commands**: every arm d answer says *none* and then shows `heroes build
   main.hero` in a code block of `answer`; every arm h answer's block holds `heroes fetch`, `heroes
   build main.hero`. By rule 1 those blocks are the commands, so a t2 run is the first build, those
   lines, then the rule's last build. Reading arm d's block as no command at all changes no grade: in
   W the first build is accepted and part (ii) does not apply, in M question 1's word is already
   wrong.
4. **No answer holds a conditional command** (rule 2): the alternatives the sessions name stand in
   `choice_points`, never as steps of `answer`. No answer is hedged.
5. **Predicted outcomes** (rule 3): every command of every t1 and t3 answer is predicted accepted,
   in `answer` and in `prediction` alike. t2's words are question 1's.
6. **No case the rule leaves undecided was met.** One observation the rule does not reach, set down
   and not decided: 18 sessions report a list of sibling folder names in their prompt (§ The context
   answers).

## The grades

`C` the commands, `P` the program, `right` both (t2 has no P, its grade is C). Each run's line is in
`S/grade/runs/grade.log`.

### t1, the edit, written

The programs: seven answers write one file (`use <text>/strings` then `use <text>/strconv`, `total @=
0`, `strconv.atoi(w).must()`), t1-h-1 the same with the two `use` lines swapped; the reference writes
`total: i64 @= 0` and `.default(0)`. Every program, run with `heroes fetch`, `heroes build main.hero`,
built at exit 0 and printed `3/12/` with exit 0 in both worlds.

| session | cost USD | commands (rule 1) | W: C / P / right | M: C / P / right | class of a wrong C | M: what repaired the refusal |
|---|---|---|---|---|---|---|
| t1-d-1 | 0.125578 | build | right / right / **right** | wrong / right / **wrong** | unforeseen: build exit 1 `module_unplaced` | none |
| t1-d-2 | 0.119682 | build | right / right / **right** | wrong / right / **wrong** | unforeseen: build exit 1 `module_unplaced` | none |
| t1-d-3 | 0.122725 | build | right / right / **right** | wrong / right / **wrong** | unforeseen: build exit 1 `module_unplaced` | none |
| t1-d-4 | 0.131830 | build | right / right / **right** | wrong / right / **wrong** | unforeseen: build exit 1 `module_unplaced` | none |
| t1-h-1 | 0.127125 | fetch, build | right / right / **right** | right / right / **right** | | `heroes fetch` |
| t1-h-2 | 0.127476 | fetch, build | right / right / **right** | right / right / **right** | | `heroes fetch` |
| t1-h-3 | 0.130550 | fetch, build | right / right / **right** | right / right / **right** | | `heroes fetch` |
| t1-h-4 | 0.128066 | fetch, build | right / right / **right** | right / right / **right** | | `heroes fetch` |

W's `heroes fetch` printed `kept text 0.5.0 ...`, exit 0; M's printed `placed text 0.5.0 ...`, exit 0,
and the build after it printed `3/12/`.

### t2, the edit, predicted

| session | cost USD | question 1 | question 2's commands | W: first build / right | M: first build / last build / right | class of a wrong C | M: what repaired the refusal |
|---|---|---|---|---|---|---|---|
| t2-d-1 | 0.120007 | accepted, prints `3`, `12` | build | exit 0, `3/12/` / **right** | exit 1 `module_unplaced` / exit 1 / **wrong** | unforeseen (M) | none |
| t2-d-2 | 0.129097 | accepted, prints `3`, `12` | build | exit 0, `3/12/` / **right** | exit 1 `module_unplaced` / exit 1 / **wrong** | unforeseen (M) | none |
| t2-d-3 | 0.135752 | accepted, prints `3`, `12` | build | exit 0, `3/12/` / **right** | exit 1 `module_unplaced` / exit 1 / **wrong** | unforeseen (M) | none |
| t2-d-4 | 0.131027 | accepted, prints `3`, `12` | build | exit 0, `3/12/` / **right** | exit 1 `module_unplaced` / exit 1 / **wrong** | unforeseen (M) | none |
| t2-h-1 | 0.132490 | refused, the `use <text>/strconv` line, not placed | fetch, build | exit 0, `3/12/` / **wrong** | exit 1 `module_unplaced` / exit 0, `3/12/` / **right** | invented (W) | `heroes fetch` |
| t2-h-2 | 0.125210 | refused, the `use <text>/strconv` line, not placed | fetch, build | exit 0, `3/12/` / **wrong** | exit 1 `module_unplaced` / exit 0, `3/12/` / **right** | invented (W) | `heroes fetch` |
| t2-h-3 | 0.127010 | refused, the `use <text>/strconv` line, not placed | fetch, build | exit 0, `3/12/` / **wrong** | exit 1 `module_unplaced` / exit 0, `3/12/` / **right** | invented (W) | `heroes fetch` |
| t2-h-4 | 0.139284 | refused, the `use <text>/strconv` line, not placed | fetch, build | exit 0, `3/12/` / **wrong** | exit 1 `module_unplaced` / exit 0, `3/12/` / **right** | invented (W) | `heroes fetch` |

Recorded, not graded: every arm d answer predicted the output `3`, `12`, which W printed. Every arm h
answer gave as its reason spec § 1's p1r sentence, quoted, and predicted that the refusal would name
the `use <text>/strconv` line and point to `heroes fetch`, which M's message does (*"error[module_unplaced]:
the module `strconv` of the package `text` is in the version the catalogue names and not placed under
...: `heroes fetch` places every module the program's `use` lines reach"*, at `main.hero:1:5`).

### t3, a second package

The programs: six answers bind `count = words.len()` and two (t3-d-3, t3-h-1) bind `n`; each prints
it and `metric.km_to_m` of it. Every program, run with `heroes add units --index file://../../index`,
`heroes fetch`, `heroes build main.hero`, printed `3/3000/` with exit 0 in both worlds.

| session | cost USD | commands (rule 1) | W: C / P / right | M: C / P / right | which verb made the entry |
|---|---|---|---|---|---|
| t3-d-1 | 0.131145 | add units --index, build | right / right / **right** | right / right / **right** | `add` |
| t3-d-2 | 0.142985 | add units, build | right / right / **right** | right / right / **right** | `add` |
| t3-d-3 | 0.123957 | add units, build | right / right / **right** | right / right / **right** | `add` |
| t3-d-4 | 0.138001 | add units --index, build | right / right / **right** | right / right / **right** | `add` |
| t3-h-1 | 0.133466 | add units --index, fetch, build | right / right / **right** | right / right / **right** | `add` |
| t3-h-2 | 0.140380 | add units --index, fetch, build | right / right / **right** | right / right / **right** | `add` |
| t3-h-3 | 0.136980 | add units --index, fetch, build | right / right / **right** | right / right / **right** | `add` |
| t3-h-4 | 0.139779 | add units --index, fetch, build | right / right / **right** | right / right / **right** | `add` |

**No session ran `heroes fetch` in place of `heroes add units`**, the trap the brief registered for
arm h; each arm h answer ran `fetch` after `add`. Arm d's four ran no `fetch` and are right in M
because this prototype's `add units` places the reach of the `use` line already written: after `heroes
add units` alone, `runs/t3-d-2-m-c/heroes.packages/` holds `units@0.1.0-08a7dc4dde3fd007/metric.hero`
and its `.list` (`find`), and the build printed `3/3000/`. Under a route whose `add` placed nothing,
those four would be refused at the build and arm h's four would not: an inference from the commands,
unrun, since no such build exists.

### The cells, counted

| task | arm d in W | arm d in M | arm h in W | arm h in M |
|---|---|---|---|---|
| t1, C right | 4 of 4 | 0 of 4 | 4 of 4 | 4 of 4 |
| t1, right (C and P) | 4 | 0 | 4 | 4 |
| t2, right (C) | 4 of 4 | 0 of 4 | 0 of 4 | 4 of 4 |
| t3, C right | 4 of 4 | 4 of 4 | 4 of 4 | 4 of 4 |
| t3, right (C and P) | 4 | 4 | 4 | 4 |
| **over twelve, C right** | **12** | **4** | **8** | **12** |
| **over twelve, right** | **12** | **4** | **8** | **12** |

Every P is right in both worlds, so C decides every cell.

## The decision rules, as registered

- **Q1, is p1r needed if M lands** (read in M): arm h is C-right in **12** of its 12 sessions (t1 4,
  t2 4, t3 4), arm d in **4** of 12 (t1 0, t2 0, t3 4). The difference is **8, at least the registered
  4: p1r enters with M.** Read net, t3 counts 4 against 4: the sentence cost nothing on the task where
  it could hurt.
- **Q2, does p1r suffice** (read in M): arm h is C-right in **8 of its 8** t1 and t2 sessions (at least
  6 asked) and in **4 of its 4** t3 sessions (at least 3 asked): **p1r suffices as worded.** No arm h
  answer failed in M, so there are no failing `choice_points` to quote.
- **Recorded, not a gate: what each route costs a first-try reader** (`right`, C and P):
  - arm d in W (today: route W with p0) **12 of 12**, against arm h in M (route M with p1r) **12 of
    12**: a gap of **0**, smaller than four, so **within chance** by the brief's words; per task 4
    against 4, 4 against 4, 4 against 4.
  - arm d in M **4 of 12** against arm d in W **12 of 12**: M costs a reader of today's spec with no
    sentence **8** of 12 first-try answers, every one of t1 and t2 (each a build predicted accepted and
    refused `module_unplaced`), none of t3.
  - arm h in W, what a reader of p1r does in a world where the sentence would be false: **8 of 12**,
    the four t2 answers wrong, each predicting a refusal that W does not make (invented).
- **Re-grading under another M** (the brief's request, from the record and with no new session): in
  M **every refusal of t1 and t2 that was repaired was repaired by `heroes fetch`** (eight answers,
  all arm h), arm d's eight had no repair, and **no answer ran `heroes add text` again**. So an M
  whose `add` places nothing over an existing entry grades t1 and t2 exactly as here: read from the
  commands, not run, since no such build exists. t3 under such a route is the inference written under
  its table.

## The spec-warden's registered prediction

Registered at `250-reports/spec-warden.md:433-435`: *"with p1r, at least 7 of 10 blind runs given the
measured edit under a built M run `heroes fetch` before their first `check`; with p0, at most 3 of
10"*, scored as the brief says on t1 and t2, eight sessions per arm, an answer counting when in t1 a
`heroes fetch` stands before its first `heroes check`, `build` or `run`, and in t2 question 1 says
refused and question 2's commands hold `heroes fetch`.

| arm | t1 | t2 | of 8 | registered bound | held |
|---|---|---|---|---|---|
| h (p1r) | 4 of 4 | 4 of 4 | **8** | at least 6 | **yes** |
| d (p0) | 0 of 4 | 0 of 4 | **0** | at most 2 | **yes** |

**The prediction holds on both halves.** No t1 answer of either arm runs `heroes check`; the first
compile command each runs is `heroes fetch` (arm h) or `heroes build` (arm d). No arm d report writes
`heroes fetch` anywhere (`grep -l` over the twelve, none), a verb p0 does not contain.

## The context answers

**24 of 24 answer `no`**: none says an instruction, rule, contract, memory or document other than
`brief.md`, `task.md` and `heroes-spec.md` reached it. **No session is voided.** 21 name the
harness's own system prompt or setup (the working directory, the date, the tools) as another input,
and t3-d-3, t3-d-4 and t3-h-4 name only the user's message (`grep` over each `## context` section);
t3-h-2 adds that the working directory's path names `heroes-lang` and a worktree, which the brief
registered as visible.

**One observation, set down and not decided**: **18 sessions** (t1-d-2, t1-d-3, t1-d-4, t1-h-1,
t1-h-3, t2-d-2, t2-d-4, t2-h-1 to t2-h-4, t3-d-1 to t3-d-4, t3-h-2, t3-h-3, t3-h-4) report that
their prompt listed sibling folder names (*"t1-d-1 … t3-h-4"*) with no contents, and that they opened
none; 6 (t1-d-1, t1-h-2, t1-h-4, t2-d-1, t2-d-3, t3-h-1) mention no such list. The names carry each
folder's task and arm letter and nothing of the specifications; where the list came from is not
traced, since `run.json` keeps no transcript: a question for the coordinator, not a `yes` under the
rule. Counted by `grep` over each answer's `## context` section.

## Each session's `choice_points`, in short

Quoted where they name placement, `heroes add` or `heroes fetch`; the rest named.

**t1, arm d**: every one keeps `heroes.toml`, runs no `heroes add`, builds, then runs `./main`.
- **t1-d-1**: *"Running `heroes add text` again is unnecessary. The spec doesn't say whether the tool
  would refuse it or rewrite the same entry, and either way the program would be unaffected."* Others:
  `.must()` over `?`, `.default(0)`, `match`; a cell and `for` over `fold`; the binary's name guessed
  `./main`, or `build` runs the program.
- **t1-d-2**: *"I ran no new `heroes add`. The package is already listed, and a program holds one
  version of each package."*; running it again *"would most likely rewrite the same entry and change
  nothing."* Others: `.must()`; a cell over `fold`; qualified call over UFCS; `./main`.
- **t1-d-3**: *"My choice: don't run `heroes add`."*; *"The spec doesn't say what that does to an
  entry that already exists. It might refuse, or rewrite the same entry. Either way the build that
  follows is unaffected."* Others: `.must()`; a cell over `fold`; `./main`.
- **t1-d-4**: *"No `heroes add` for `strconv`."*; again, *"it might be refused as a duplicate or
  rewrite the same entry."* Its prediction: *"the build fetches and checks the same `text 0.5.0` as
  before"*, a build that fetches, which M's message says `build` never does. Others: `.must()`; a cell
  over `fold`; `./main`.

**t1, arm h**: every one runs `heroes fetch` before the build, citing the p1r sentence.
- **t1-h-1**: *"`use <text>/strconv` is a new `use` line, written after the modules were last placed.
  So I run `heroes fetch` first."*; the other choice, *"The tool would refuse the `use <text>/strconv`
  line because its module hasn't been placed."* Not `heroes add text` again; `fetch` with no `--index`.
  Others: `.must()`; the `use` order; `./main`.
- **t1-h-2**: *"The new `use <text>/strconv` was written after the last fetch, so I fetch first. If I
  didn't, `heroes build main.hero` would refuse the program at `use <text>/strconv`."*; a second `add`
  *"wouldn't make the module available anyway, because only `fetch` does that"*, which this prototype
  contradicts (its `add` re-places, the brief's measurement). Takes `build` as running the program, so
  writes no run step.
- **t1-h-3**: *"`heroes build main.hero` would most likely refuse the program"* without the fetch; *"The
  original author never ran `fetch` by name, so `heroes add` probably placed the modules itself. If
  `add` placed every module of the package, building without `fetch` might work. The spec doesn't say,
  so running `fetch` is the safe choice."* Others: `.must()`; a cell over `fold`; `./main`.
- **t1-h-4**: *"The new line `use <text>/strconv` reaches a module that was never placed, because only
  `strings` was reached before."*; *"Running `fetch` costs nothing if they are already placed, so I run
  it."* Not `heroes add` again. Others: `.must()` over a `match` with a numeric exit code; `./main`.

**t2, arm d**: every one answers accepted, the catalogue keyed by package.
- **t2-d-1**: *"My choice: no. The catalogue lists packages with their version and `sum` (§1), and
  `strconv` belongs to `text`, which is already listed."*; `heroes add text` again *"is unnecessary"*.
  Others: `atoi`'s width; whether 0.5.0 holds `strconv`.
- **t2-d-2**: the other reading, *"`heroes build` would refuse `<text>/strconv` as not listed, and the
  fix would be to run `heroes add text --index file://../../index` again. The spec has no per-module
  entries, so I rejected this."*; *"the spec doesn't say whether the tool fetches from the index
  again."*
- **t2-d-3**: *"The other reading is that the tool refuses `use <text>/strconv` until you run `heroes
  add text` again. That reading has no basis in the spec"*; *"`heroes add text` already wrote whatever
  the whole package needs."*
- **t2-d-4**: the other reading, *"each new module would need its own entry, so the build would be
  refused for an unlisted `strconv`. The fix would then be running `heroes add text --index
  file://../../index` again"*; the build *"reads the same index and checks the same `sum`"*.

**t2, arm h**: every one answers refused and repairs with `heroes fetch`, naming `heroes add` again as
the other route.
- **t2-h-1**: *"'It' could mean `heroes fetch` or `heroes add`."*; *"I chose `heroes fetch`, because it
  is the command the spec names for placing modules."*; `add` again *"would probably also work"*.
  `fetch` with no `--index`.
- **t2-h-2**: *"The other reading is that the whole package is placed, so `strconv` is already there.
  Then the build would be accepted and print `3` and `12`."* (what W does); re-running `add` *"The spec
  doesn't say it places modules again, so the next build might still be refused."*
- **t2-h-3**: *"'it' is the last placement of modules ... So `heroes add` must place the modules the
  `use` lines reached at that moment"*; it rejects *"the build places modules on its own"* because the
  spec says such a `use` *"is refused until it runs again"*.
- **t2-h-4**: *"I read it as `heroes fetch`, so the fix is `heroes fetch`."*; the other reading, *"a
  listed package makes every one of its modules available. Then the build is accepted as it stands and
  prints `3` and `12`, with no commands needed."* (what W does).

**t3, arm d**: every one runs `heroes add units`, then builds; none runs `fetch`.
- **t3-d-1**: *"I used `heroes add`. If I skipped it, `use <units>/metric` would name a package missing
  from the catalogue, and the build would be refused."*; repeats `--index`. Others: `len`'s width;
  `./main`.
- **t3-d-2**: leaves `--index` off, *"expect the tool to use that line"*; the tool, never a hand edit.
  Others: `len`'s width; `as` aliases; `./main`.
- **t3-d-3**: *"I ran `heroes add units`. If I skip this step, `heroes build` refuses the program"*;
  `--index` left off. Others: `len`'s width; `./main`.
- **t3-d-4**: *"If I build without running `heroes add` first, `<units>` isn't in the catalogue and the
  tool refuses the build."*; repeats the author's `--index`. Others: `len`'s width; `./main`.

**t3, arm h**: every one runs `heroes add units --index ...`, then `heroes fetch`, then builds.
- **t3-h-1**: *"I wrote the new `use` line first, then ran `add`, then `fetch`."*; *"The task's own
  history ... suggests `add` may also place the modules. If so, `fetch` changes nothing"*. Takes
  `build` as running the program.
- **t3-h-2**: *"Running `heroes fetch`. I ran it."*; *"Leaving it out risks the build refusing `use
  <units>/metric`"*; editing after a `fetch` and building at once *"would get the new `use` refused."*
- **t3-h-3**: *"Running fetch before editing `main.hero` would not place `metric`, and build would
  refuse."*; `add` may place modules itself, *"In that case the fetch is redundant, not wrong."*
- **t3-h-4**: *"The author's first run in the task shows no `fetch`, so `add` may place the modules
  itself. Running `fetch` is the safe choice."*; repeats `--index`; `./main`.

## What this reading does not say

- **The grading of M is one prototype's**, `sources 1593fa56260f5ec3`, the brief's caveat: its `add`
  places the reach of a new entry's `use` lines and re-places over an existing one. The t1 and t2
  grades hold under an M whose `add` places nothing over an existing entry (§ The decision rules); t3
  arm d's four in M rest on the first property (§ t3).
- **Four sessions per cell**, every one of one model on one day. Arm h was uniform within each task,
  and so was arm d; whether that uniformity holds at more sessions is unmeasured.
