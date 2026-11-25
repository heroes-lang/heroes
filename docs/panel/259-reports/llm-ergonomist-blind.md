# Panel 259, llm-ergonomist (the blind seat): the paid run, as registered, scored

Written 2026-11-25 by the session the coordinator of M-rebel-library launched to run the registered blind reading
(its brief: `.claude/worktrees/rebel-scratch/coord/blind259-brief.md`), on the author's authorisation of 09:25 (*the
blind seat of 259, paid: 20 dollars*, recorded in `259-briefs/llm-ergonomist.md`). The run is
`259-briefs/llm-ergonomist.md` and `259-briefs/blind/registration.md` as the synthesis left them (both `cmp` equal to
`main` at `6da1ff4d7`). Every number below is a command's, named beside it. The readers had no compiler: a session's
*prints* or *aborts* is its reading of its arm's text, never a build.

## In one paragraph

**31 sessions ran, 31 count, 10.393155 USD spent** of the author's 20.00. **Every registered decision rule holds for
`adopted`**: D1 (q4 build-right 4 of 5 under `adopted`, 2 of 5 under `now` giving that verdict, both on the
boundary), D2 (no rise on q1, q2, q3, q6, q7), D2w (`NOT PROMISED` falls, 3 to 1 on each), D3 (design A 4 of 5, no
reader answers a wrong value or an abort for B or C). So, by the registration's own rules, **the resolution stays on
`adopted`, the sentence is not withdrawn and its class is not narrowed**. `rival` holds all four as well, by wider
margins on t1 (q4 5 of 5, no `NOT PROMISED`). **What the run found that the registration did not ask**: the handler
sentence of § 13 already in `now` (*"A callback called after C left a handler of C's ... may abort"*) is read by 3 of
5 `now` t1 readers and 4 of 5 `now` t2 readers as covering **every** C switch of a callback's frames, so they answer
`NOT PROMISED` for programs the *may* classification registered as **out**; under `adopted` 1 of 5 in each task read
it so, under `rival` 0 of 5 (t1) and 1 of 5 (t2).

## How it was run

- **The key first, because the registration waited on it.** `registration.md` § What the synthesis added: *"Owed
  before any session, and the run waits for them: the Linux arm64 rows above at these inputs, under `now` and under
  `rt-e` or the landed build, three runs each"*. No one had run them (the synthesis's own § What is open). This
  session ran them, free, before any paid session, **on both graded platforms** (and this Mac's rows again, so the
  key is one session's): a tree of `3ec775b66` (`git archive` of `seed runtime selfhost vendor spec`), clang on the
  seed, stage 2 from it, `heroes-next --version` reading `sources 117362877b136f67` on both (the synthesis's stage 2
  key); `rt-e` built as the base runtime with the compiler-engineer's `runtime-rt-e.diff` applied by `patch -p1`
  (from `259-reports/evidence.tar.xz`; `diff -r` against the synthesis's `panel259-scratch/synth/rt-e` prints
  nothing); the session's programs (below) and t2's design C (`blind-e4/design-c-q3m.hero` beside
  `blind/design-c/sw.h`), each built and run three times under `timeout 120`, output to a file. This Mac (Darwin
  arm64, Apple clang 21.0.0) 16:48 to 16:50 UTC; the Linux arm64 image (`heroes-linux-arm64:latest`, image id
  `338a1044ac90`, Debian clang 22.1.8, `docker run --rm`) 16:49 to 16:53 UTC. **Every cell's three outputs are
  byte-identical** (`shasum`, 32 cells: two platforms, two builds, eight programs), and the table is § The key. The x86-64 image, recorded and never graded by
  the registration, was **not run**.
- **The inputs, by hash, before any session** (`shasum -a 256`): `spec-now.md` is `git show
  3ec775b66:spec/heroes-spec.md`, `c915f0f2...2bee`; `spec-adopted.md` is it with `synthesis-adopted.diff` applied by
  `patch -p1`, `aaf23b9e...16f3`, 34,828 bytes; `spec-rival.md` with `synthesis-rival.diff`, `0461f1d3...4ac1`,
  34,915 bytes; the two texts `c90282af...c39b` and `c7e2341a...570b`; `task-t1.md` `f71c5447...52f7`; `task-t2.md`
  the E4 one, `28c1309c...e097`; `co.h`, `sw.h`, `jump.h`, `q5.hero`, `q7.hero` the first table's; `q1`, `q2`, `q3`,
  `q4`, `q6` the E4 table's (`2dd061ae`, `3da8917e`, `180fac47`, `5b9351ef`, `aad2c4d0`); `design-c/sw.h`
  `20260c76...9b`; `to-e4.pl` `e027a215...8ed3`. **Every one equals the registration's.** `to-e4.pl` run on the five
  registered E5 programs gives the five E4 files (`cmp`) and leaves q7 unchanged (`cmp`).
- **The runner**: panel 256's (`docs/panel/256-briefs/blind/`), adapted as panel 257 adapted it, adapted again for
  259, every change written as a `diff -u` against panel 256's committed bytes **before any session** in
  `259-briefs/blind/registration-runner.md` (sha256 `6a7f993c...6d98`, written 17:46; the first paid session started
  17:53:30). Panel 257's archive holds its `registration-runner.md` and `inputs.sha256` alone (92 files), not the
  runner, so its five changed files were rebuilt by applying its diffs to 256's bytes (clean; the rebuilt
  `brief-session.md` hashes `93761bfd...`, the hash panel 257's `key.txt` records for every folder's `brief.md`), and
  its two new files that were not diffs were written anew: `make_verdict.py` and `brief-verdict-reports.md`, the second
  now **listing the thirty reports' names** (panel 257's verdict could not list `reports/` and read none).
- **The area**: `.claude/worktrees/259-llm-ergonomist/`, the top of an empty git repository of its own (`.gitignore`
  `*`, `reader <reader@invalid>`, no commit), `home/` and `tmp/` inside, ignored by this repository (`git
  check-ignore`). The static preflight (`bash preflight.sh`): 66 checks `ok`, 0 `FAIL` (the caps 0.60, 1.20, margin
  0.10, ceiling 20.00; the command's every flag; the arms equal to the frozen spec plus the one-line diffs; panel
  256's seven unchanged tools by hash; the warden's predictions section pinned; 30 folders at their registered hashes,
  5 per task and arm). **The free preflight** (17:46:24): a key the API refuses, `run.json` `total_cost_usd` 0,
  `api_error_status` 401, its transcript `prompt-alone=yes git-status=reader, no commit claude-md-or-memory=no`, no
  file naming its id under `/private/tmp/claude-501/`. `claude --version`: **2.1.323 (Claude Code)**, the version the
  registration names.
- **Every session**: `claude -p "<the registered prompt>" --bare --restricted --safe-mode --strict-mcp-config --model
  claude-opus-5-5 --tools "Read,Write" --allowedTools "Read,Write" --disallowedTools
  "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" --max-budget-usd <cap> --output-format
  json < /dev/null` under `env -i` with `HOME`, `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the area, one at a time, from
  its own folder, 0.60 USD a graded session and 1.20 the verdict, the runner stopping before any session where `spent
  + cap + 0.10` would pass 20.00 (`run.sh`). The spec-warden's report was copied as the runner started
  (`warden-at-start.md`, sha256 `7f4cba24...48f9`, `cmp` equal to `docs/panel/259-reports/spec-warden.md` on `main`;
  its predictions section `5be9f7b3...a82a`, pinned in `run.sh`); the registration likewise (`4d685809...bc44`, `cmp`
  equal to `main`'s).
- **The draw**: `key.txt`'s first line, `now=h adopted=u rival=z` (`secrets.SystemRandom().sample` over the letters in
  none of the arms' names). **The scorer was not blind**: `order.txt`'s cells run in a fixed sequence whose first is
  `now`, and the key's first line was read before grading. The grading is mechanical against the key, and the
  extractors and the grades stand in the archive so that anyone can count again.
- **The run**: first session 17:53:30, the verdict ended 18:45:01, `run.log`'s last line *"spent 10.393155 USD in
  all"*; `tools/read_runs.py`: 31 lines, every one `subtype=success is_error=False terminal=completed`, the model
  `claude-opus-5-5` alone in every `modelUsage`.
- **After the run**: every file of the 31 folders read again by sha256 against `key.txt` and, for the arm's spec, the
  task and the programs, against `tools/inputs.sha256`: **31 folders, 315 files, 0 differences**, and each folder holds
  beyond them only `result.md`, `run.err`, `run.json`. `tools/scan.py`: **32 transcripts, prompt-alone=NO 0,
  git-status=LEAK 0, claude-md-or-memory=YES 0**; `tools/reads.py`: **0 sessions with a path outside their folder**, 0
  without a cwd. **Every `context` answer says no rule, contract or memory reached it** (read whole, 31 of 31); seven
  name words of the working directory's path, which the system prompt gives (`t1-u-1`, `t2-h-5`, `t2-u-3`, `t2-z-1`,
  `t2-z-3`, `t2-z-5`: *"259-llm-ergonomist"*; `t1-z-4`: *"heroes-lang"*), a path and not a rule, the area standing
  under the repository's root by CLAUDE.md § Hard stops, as panel 257's four did. **0 sessions voided.**
- **No session wrote `report.md`** (31 of 31 `report.md=no` in `run.log`), as in panel 257: under the registered flags
  the sessions say they hold `Read` alone (*"I couldn't create `report.md` because the only tool I have reads files"*,
  `t1-h-1`), and each gave its report as its final answer, which `tools/after.py` kept as `result.md`, the brief's
  fallback. **Whether `--tools "Read,Write"` under `--bare --restricted --safe-mode` yields a Write tool at all is a
  question for the runner's next sitting, unrun here**.

## The spend, to the cent

| cell | sessions | USD | each |
|---|---|---|---|
| t1 `now` | 5 | 2.14 | 0.39 to 0.47 |
| t1 `adopted` | 5 | 2.15 | 0.40 to 0.47 |
| t1 `rival` | 5 | 1.81 | 0.34 to 0.40 |
| t2 `now` | 5 | 1.16 | 0.22 to 0.25 |
| t2 `adopted` | 5 | 1.21 | 0.23 to 0.26 |
| t2 `rival` | 5 | 0.99 | 0.19 to 0.22 |
| the verdict | 1 | 0.93 | |
| **all** | **31** | **10.39** (10.393155) | |

The 30 graded 9.467460 (mean 0.315582; the most any spent 0.471656, under its 0.60 cap), the verdict 0.925695, of the
author's 20.00: **9.61 unspent**. The free preflight cost 0. The registration's estimate, *"about 6.3 to 8.5 USD"*,
*an inference, unrun*, was low: each t1 session cost 0.34 to 0.47, where every one of panel 257's 30 cost 0.16 to
0.24 (its report, § How it was run).

## The key, as run (three runs each, every three byte-identical)

The session's programs: q1, q2, q3, q4, q6 at `E4`, q5 at `E5`, q7 at `E2`. t2's design A is q4, B is q3, C is q3
with `design-c/sw.h`.

| | q1 | q2 | q3 | q4 | q5 | q6 | q7 | t2 A | t2 B | t2 C |
|---|---|---|---|---|---|---|---|---|---|---|
| `now`, this Mac | `PRINTS` | `PRINTS` | `PRINTS` | **`WRONG`** | `ABORTS` | `PRINTS` | `PRINTS` | a wrong value | yes | yes |
| `now`, Linux arm64 | `PRINTS` | `PRINTS` | `PRINTS` | **`WRONG`** | `ABORTS` | `PRINTS` | `PRINTS` | a wrong value | yes | yes |
| `adopted` (`rt-e`), this Mac | `PRINTS` | `PRINTS` | `PRINTS` | **`PRINTS`** | `ABORTS` | `PRINTS` | `PRINTS` | yes | yes | yes |
| `adopted` (`rt-e`), Linux arm64 | `PRINTS` | `PRINTS` | `PRINTS` | **`PRINTS`** | `ABORTS` | `PRINTS` | `PRINTS` | yes | yes | yes |

- `PRINTS`, both builds, both platforms: q1 and q3 `order 0 1: 30 32`, `order 1 0: 30 32`; q2, q7 (and q4 under
  `rt-e`) `body holds 7` twice, `body holds 7 coroutines 30 32`, `carved: 7`; q6 `jumped inside a coroutine: 10 35021`;
  design C q1's lines. Exit 0.
- q4 under `now`: `body holds 11` twice, then `panic: internal error: a large value's room was given back out of order
  (compiler bug)`, exit 134, **on both platforms** (Linux adds `Aborted`). At `E4` **q4 no longer splits**: the
  critic's § F1 said so, and this is its three-run reading on both. So under `now` q4 has a key, `WRONG`.
- q5, both builds, both platforms: `panic: stack exhausted in q5.e5: the room large values take off the stack is
  full, 1 GiB for the whole program`, exit 134. R2's words (*"... while N callbacks C called have not returned"*) are
  unbuilt, so the build says no count; the registration grades no wording.
- **No program splits between the graded platforms under either build**, so every rule has its key and none waits.
  `rt-e` stands for the landed build, as `registration.md` allows (*"under `rt-e` or the landed build"*): the landing
  (lane `land259`) had not built R1 when the run started, and no program of t1 or t2 lends (the synthesis's reading).

## t1, scored (`score/grades-t1.tsv`, counted by `score/score.py`)

Each answer's verdict word, its printed lines compared with the key's by string, and q5's abort sentence, read from the
session's own `result.md` (`score/t1-read.tsv`); no reader split the Mac from Linux (every verdict line read: one
verdict for both, 15 of 15). **build-right** is against the build of the session's arm: the base for `now`, `rt-e`
for `adopted` and `rival`.

| arm | q1 | q2 | q3 | q4 | q5 | q6 | q7 |
|---|---|---|---|---|---|---|---|
| `now` | `PRINTS` 2, `NP` 3 | `PRINTS` 2, `NP` 3 | `PRINTS` 2, `NP` 3 | `PRINTS` 2, `NP` 3 | `ABORTS` (stack) 4, `NP` 1 | `PRINTS` 2, `NP` 3 | `PRINTS` 2, `NP` 3 |
| `now` build-right | 2 | 2 | 2 | **0** (key `WRONG`) | 4 | 2 | 2 |
| `adopted` | `PRINTS` 4, `NP` 1 | `PRINTS` 4, `NP` 1 | `PRINTS` 4, `NP` 1 | `PRINTS` 4, `NP` 1 | `ABORTS` (room) 5 | `PRINTS` 4, `NP` 1 | `PRINTS` 4, `NP` 1 |
| `adopted` build-right | 4 | 4 | 4 | **4** | 5 | 4 | 4 |
| `rival` | `PRINTS` 5 | `PRINTS` 5 | `PRINTS` 5 | `PRINTS` 5 | `ABORTS` (room) 5 | `PRINTS` 5 | `PRINTS` 5 |
| `rival` build-right | 5 | 5 | 5 | **5** | 5 | 5 | 5 |

`NP` is `NOT PROMISED`. Every `PRINTS` gives the key's lines exactly; **no reader answered `WRONG`, and none `ABORTS`
for any program but q5**. **text-right**: none possible, every cell of the *may* table being **out**; every `NOT
PROMISED` is width (D2w).

- **Who said `NOT PROMISED`, and why**: `t1-h-1`, `t1-h-2`, `t1-h-4` (`now`) and `t1-u-5` (`adopted`), each for q1
  to q4, q6 and q7, each by the same reading: the handler sentence's *"may abort"* covers any frame C leaves by a
  `swapcontext`, an assembly jump or a `setcontext`, whatever a handler is (`t1-u-5`'s choice point 2: *"I took it to
  mean a callback frame: a function of the program that C called"*). The two `now` readers who print (`t1-h-3`,
  `t1-h-5`) read *handler* as a signal handler. `t1-h-1` also gives q5 `NOT PROMISED`.
- **q5's two causes**: the four `now` readers who abort say it runs **out of stack in its first iteration** (a 256 KiB
  `E5` on a 256 KiB coroutine stack: the confound the synthesis registered for q5 alone); all ten `adopted` and
  `rival` readers say it aborts **near the 4,096th pass, past 1 GiB of values held for callbacks C left by
  `setcontext`, saying how many have not returned** (4,095 to 4,097), which is the build's cause and the adopted text's
  words. Both are graded right, the registration grading an `ABORTS` *"by whether it names the switch or the stack"*:
  the first names the stack, the second the room and the `setcontext` that left the frames. A reading that took only
  the build's cause as right would leave `now` at 0 of 5 on q5.

## t2, scored (`score/grades-t2.tsv`)

| arm | A | B | C | Q3 | Q2 (the design written) |
|---|---|---|---|---|---|
| `now` | `NP` 4, yes 1 | `NP` 5 | `NP` 5 | no 5 | C 4, A 1 |
| `adopted` | yes 4, `NP` 1 | yes 4, `NP` 1 | yes 4, `NP` 1 | no 5 | C 5 |
| `rival` | yes 4, `NP` 1 | yes 4, `NP` 1 | yes 4, `NP` 1 | no 5 | C 4, B 1 |

Build-right: `now` A 0 (key: a wrong value; `t2-h-2` alone said *yes*), B 0, C 0 (key yes). `t2-h-1`, `t2-h-3`,
`t2-h-4` and `t2-h-5` answer `not promised` for all three on the handler reading above; `t2-h-2` reads *handler* as
a signal handler and answers `not promised` for B and C on another ground, a stack outside the thread's that the
runtime's *"running out of stack aborts"* may misjudge. `adopted` and `rival` A 4, B 4, C 4. **No reader
answered *a wrong value* or *the program aborts* for any design.** The `not promised` answers under `adopted` and
`rival` are `t2-u-5` and `t2-z-2`, again on the handler sentence. **Question 3**: all fifteen answer *no*, which is
right against the adopted text (it names no function). **Question 2** is not scored: under `rt-e` the three designs
give the same verdict, so the landed build does not separate them; 13 of 15 chose C, for its guard page.

## The decision rules (`259-briefs/llm-ergonomist.md` § The decision rules)

| rule | `adopted` | `rival` |
|---|---|---|
| **D1**: q4 build-right under the arm at least 4 of 5; under `now` at most 2 of 5 give that verdict | **holds**: 4 of 5; `now` 2 of 5 (`t1-h-3`, `t1-h-5`); **both halves on their boundary** | **holds**: 5 of 5; `now` 2 of 5 |
| **D2**: on q1, q2, q3, q6, q7, `ABORTS`, `WRONG` or other lines rise by at most 1 against `now` | **holds**: 0 against 0 on each | **holds**: 0 against 0 on each |
| **D2w**: `NOT PROMISED` rises by at most 1 against `now` on those five | **holds**: 1 against 3 on each (−2) | **holds**: 0 against 3 on each (−3) |
| **D3**: design A build-right at least 4 of 5; at most 1 answers a wrong value or an abort for B or C | **holds**: 4 of 5; 0 | **holds**: 4 of 5; 0 |

**What the run does to the resolution**, by the registration's last bullet: D1 and D3 hold under `adopted`, so the
resolution does not move to `rival`; D2 holds, so the sentence is not withdrawn; D2w holds, so its class is not
narrowed. **D1's margin is nil on both halves**: one more `now` reader printing q4's lines, or one fewer `adopted`
reader doing so, would have failed it, and `t1-u-5`'s `NOT PROMISED` is the handler sentence's, not the adopted
sentence's.

## The spec-warden's predictions, scored (`warden-at-start.md`, registered 09:25)

Registered for the `E5` inputs, before the `E4` change, and for `now`, `arm-p`, `w1`, `arm-r`, `w2`, `arm-t`, `w3`;
`rival` is `w1` (byte for byte, the synthesis), and **no row is the adopted text**, so only `now` and `w1` are scored.

| prediction | registered (range) | measured | |
|---|---|---|---|
| `now` t1 build-right q1, q2, q3, q6 | 3 (1-5) each | 2 each | in range |
| `now` q4 | no key; `PRINTS` 3 (1-4) | key `WRONG` at `E4`: 0; `PRINTS` 2 | `PRINTS` in range |
| `now` q5 | 2 (0-4) | 4 | in range, at its top |
| `now` q7 | 4 (2-5) | 2 | in range, at its bottom |
| `now` `NOT PROMISED` q1, q2, q3, q6, q7 | 0 (0-1), 1 (0-2), 1 (0-2), 1 (0-2), 0 (0-2) | 3 each | **out, all five** |
| `w1` t1 build-right q1 to q4, q6 | 3 (1-5) each | 5 each | in range |
| `w1` q5 | 2 (0-4) | 5 | **out** |
| `w1` q7 | 4 (2-5) | 5 | in range |
| `w1` `NOT PROMISED`, the five | 0 (0-1) | 0 | in range |
| `now` t2 B, C | 3 (2-5) each | 0 each | **out** |
| `now` t2 Q3; `w1` t2 Q3 | 5 (4-5) *no* | 5, 5 | in range |
| `w1` t2 A, B, C | 3 (1-5), 4 (2-5), 4 (2-5) | 4, 4, 4 | in range |
| `w1` D1 | **fails** (q4 3 of 5; `now` 3 `PRINTS`) | **holds** (5 of 5; `now` 2) | **wrong** |
| `w1` D2, D2w | hold | hold | right |
| `w1` D3 | fails narrowly (A 3 of 5) | holds (A 4 of 5) | **wrong** |
| the spend | 8.3 (6.0 to 11.5) USD | 10.39 | in range |
| at most 1 of 30 stops at its cap | | 0 | right |

The synthesis's prediction to score (*"the warden's `now` arm giving q4 `PRINTS` at least 3 of 5"*): **2 of 5,
not met**. The warden's reading that *"`now` already teaches q4's value (§ 3), so a promise has nothing to move"* is
what D1's `now` half measured: 2 of 5 `now` readers printed q4's lines, 5 of 5 `w1` readers.

## The verdict session

`verdict/` (0.925695 USD, 47 `Read` calls) was handed `spec-A.md` (`now`), `spec-B.md` (`adopted`), `changes.diff`,
`task.md` (t1 then t2), t1's programs, `reports/` (the 30, by label, no arm) and a brief listing their names. **It read
all thirty** (its transcript: 30 distinct `reports/` paths) and **never opened `spec-B.md`**, reading the change
through `changes.diff` alone (its own `context`). From the reports' quotations it inferred `h` = spec-A, `u` = spec-B
and `z` = a third version. **Its verdict: approve**: *"It turns a case the spec left undefined (frames C abandons)
into a defined result ... It also makes readers' answers agree on q5, q6 and t2. It does not fix the larger ambiguity
in the same paragraph, the 'handler of C's' sentence. That defect is in both versions, so it does not count against
this change."* Its argument names the price: *"a program that abandons frames still builds and fails only after about
4,096 passes, so a short test run will not catch it."* Its conditions for *object*: spec-B readers predicting wrong
output because they assumed large values sit off the stack; an implementation unable to keep the 1 GiB count on
`malloc` stacks C frees; a rerun where spec-B readers' signal-handler reading of *handler* falls back to spec-A's 2 of
5. For a stronger approve: the third version's *"switch of stacks"* sentence making every reader agree on q1 to q4 and
q7. Its own answers: under spec-A it reads the handler sentence as covering every switch (`NOT PROMISED` for q1 to q4,
q6, q7; t2 all *not promised*); under spec-B `PRINTS` and *yes*.

## What the registration could not do as written, and what was done instead

1. **The key under `adopted` is `rt-e`, not the landed build**, which did not exist when the run began; the
   registration allows it. R1's lend bound and R2's abort words are unbuilt in it: no program here lends, and the
   abort's words are not graded.
2. **The Linux arm64 rows the run waited for were run by this session** (free, local, before any paid session), and
   this Mac's rows again; nothing split. The x86-64 rows were not run (recorded, never graded, by the registration).
3. **Panel 257's runner is not in its archive**: rebuilt from 256's bytes and 257's diffs; `make_verdict.py` and
   `brief-verdict-reports.md` written anew, the second listing the thirty report names. Recorded before any session in
   `259-briefs/blind/registration-runner.md`. (The coordinator's brief said the archive held `make_sessions.py`,
   `read_runs.py`, `spent.py`, `brief-session.md`, `brief-verdict.md`; it holds none of them.)
4. **The verdict's folder holds `task.md` and `programs/`** beside *"the two specs, a diff and the reports"*, as panel
   257's did, since the registered `brief-verdict.md` (panel 256's bytes) asks for a task done under each spec.
5. **No session could write `report.md`**: 31 answers are the fallback `result.md`.
6. **The warden's predictions name no row for the adopted text** and were registered at `E5`: scored for `now` and
   `w1` only.
7. **The scorer was not blind** (§ How it was run). **q5's `ABORTS` is graded by the registration's lenient rule**,
   both causes right; the stricter reading is given beside it.

## Where the answers are

`259-reports/llm-ergonomist-blind-answers.tar.xz`, text only: the area's `key.txt`, `order.txt`, `run.log`,
`runner.out`, `read_runs.out`, `scan.out`, `reads.out`, `warden-at-start.md`, `registration-at-start.md`, `run.sh`,
`preflight.sh`, `tools/`, `preflight/run.json` and its `cli-tmp-diff.txt`; every session's `result.md`, `run.json` and
`run.err` and the verdict's `brief.md`; `key/` (`key.sh`, `key.md`, the two platforms' logs and every run's output
file); `score/` (`t1_read.py`, `t2_read.py`, `score.py`, `t1-read.tsv`, `grades-t1.tsv`, `grades-t2.tsv`,
`score.out`). The transcripts, the binaries and the session folders' inputs (reproducible by hash) stay in the area,
`.claude/worktrees/259-llm-ergonomist/`, uncommitted.
