# Panel 260, llm-ergonomist (the blind seat): the paid run, as registered, scored

Written 2026-11-26 by the session the coordinator of M-rebel-library launched to run the registered blind reading
(its brief: `.claude/worktrees/rebel-scratch/coord/blind260-brief.md`), on the author's authorisation of 2026-11-25
(*ok a*, relayed at 10:37, a ceiling of 20.00 USD for the whole run, recorded in `260-briefs/llm-ergonomist.md`).
The run is `260-briefs/llm-ergonomist.md` and `260-briefs/blind/registration.md` as they stand on `main`, with the
section this session appended before any session (*Written 2026-11-26, after the landing*: t2's task and six
programs, their hashes and the key) and the runner's registration beside it (`blind/registration-runner.md`). Every
number below is a command's, named beside it. A reader's *builds* or *refused* is its reading of its arm's text; the
**built** grade and t2's key are the landed build's.

## In one paragraph

**31 sessions ran and count, plus one attempt a network outage cut and the runner ran again: 7.108556 USD spent** of
the author's 20.00 (the 31 counted 7.018539, the cut attempt 0.090017). The draw: `now=q`, `adopted=f`, `rival=c`.
**D1 holds** (W1 and W3 declared-right 5 of 5 under `adopted`), **D1c holds** (W4 5 of 5), **D2 fails** (5 of 5
`adopted` readers declare in W2, where at most 1 may), **D3 fails on one program of six** (t2's (4), 0 of 5
build-right; the other five 5 of 5; no reader names the importer's file for (2) or (5)). `rival` reads **exactly as
`adopted`** on every grade of both tasks, so by the registration's rules nothing moves the resolution to `rival`;
**D2's failure narrows the text** and D3's has the same cause. **The cause, in the readers' own words**: the sentence
*"processes and files from the language's `hero_os.h`, and sockets, clocks and the rest from its platform layer:
`hero_net.h`, `hero_time.h` and every other `hero_` header it provides but the compiler's `hero_compiler.h`"* is read
by every `adopted` and `rival` reader as putting `hero_os.h` among the layer's headers, the one exception it names
being `hero_compiler.h` (7 of the 10 t1 readers name `hero_os.h` in a choice point, 6 of them reading the exception
list so, the other 3 write the word on every group without naming it a choice; all 10 t2 readers predict (4)'s
`hero_os.h` group refused for lacking the word).
**And the landed build does not tell them**: `extern "hero_os.h" layer` builds and runs at exit 0, and `heroes
check` passes it, on this Mac and in the Linux arm64 image, while the text says *"no other group may"*; so every
reader's W2 is declared-wrong by the text and **built** by the compiler, a plausible mistake the build accepts
(found beside, below).

## How it was run

- **First, the clerk's owed work, before any session** (`registration.md` § What is still owed): t2's task and six
  programs, written by this session in the adopted spelling, label-stripped, in the detached worktree
  `.claude/worktrees/panel260-blind` (`main` at `1e56b5da5`, which holds `a60dfb77f`), and the key, both platforms,
  three builds of each program, every binary run three times; written into the registration as the section *Written
  2026-11-26, after the landing* (91 lines appended, none changed: `git diff` prints no `-` line). **The two platforms
  agree on all six, so all six are in D3.** The key's table is there and in § The key below.
- **The inputs, by hash, before any session**: the three arms rebuilt from `git show c3885b81b:spec/heroes-spec.md`
  with each registered diff applied by `patch -p1`: `now` `c915f0f2…2bee`, `adopted` `70b8e497…f1a7` (34,855 bytes),
  `rival` `abe96488…acc4` (34,740 bytes); both diffs and both texts at their registered hashes, `synthesis-rival.diff`
  `cmp`-equal to `spec-w1-group-mark.diff`; `task-t1.md` `3d71ca51…1b247b41a2`. **Every one equals the
  registration's.**
- **The runner**: panel 256's as panel 257 adapted it and panel 259 used it (read from panel 259's archive; panel
  257's `brief-session.md` rebuilt from its own diff and `cmp`-equal to 259's copy), adapted for 260, every change a
  `diff -u` against panel 256's committed bytes in `260-briefs/blind/registration-runner.md`, written before any
  session. **The session brief** is panel 257's with the two lines the registration rewrites and no other (sha256
  `ca8e2077…8beb`, 274 words), and **`run.sh` now pins it**, refusing to start on any other brief, as the registration
  asks and panel 259's runner did not check.
- **The area**: `.claude/worktrees/260-llm-ergonomist/`, the top of an empty git repository of its own (`.gitignore`
  `*`, `reader <reader@invalid>`, no commit), `home/` and `tmp/` inside. The static preflight (`bash preflight.sh`):
  **76 checks `ok`, 0 `FAIL`** (the caps 0.60 and 1.20, the margin 0.10, the ceiling 20.00; every flag of the command;
  the arms equal to `now` with each registered diff applied; panel 256's unchanged tools by hash; the warden's
  section pinned; the brief pinned; 30 folders at their registered hashes, 5 per task and arm). **The free
  preflight** (14:15:41): a key the API refuses, `run.json` `total_cost_usd` 0, `api_error_status` 401, its transcript
  `prompt-alone=yes git-status=reader, no commit claude-md-or-memory=no`, no file naming its id under
  `/private/tmp/claude-501/`. `claude --version`: **2.1.323 (Claude Code)**.
- **Every session**: `claude -p "<the registered prompt>" --bare --restricted --safe-mode --strict-mcp-config --model
  claude-opus-5-5 --tools "Read,Write" --allowedTools "Read,Write" --disallowedTools
  "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" --max-budget-usd <cap> --output-format
  json < /dev/null` under `env -i` with `HOME`, `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the area, one at a time,
  from its own folder; the runner started in a session of its own (`setsid`), so a cut of the shell that launched it
  could not kill a session in flight. The spec-warden's report was copied as the runner started (`warden-at-start.md`,
  sha256 `1ec3ca09…`, equal to `docs/panel/260-reports/spec-warden.md`; its predictions section `818747c9…7dd7ea`,
  pinned in `run.sh`); the registration likewise (`369e4fa1…e672`, `cmp`-equal to the worktree's).
- **Two cuts, and what each did.** (1) **The coordinator's restart at 14:07** cut this session before any area
  existed: no session had run, 0.00 spent; the Linux stage 2 build it cut was built again. (2) **A network outage at
  about 14:25** (DNS, `ENOTFOUND`) cut `t1-f-1`: its `run.json` reads `subtype` `success` but **`is_error` true,
  `terminal_reason` `api_error`**, 0.090017 USD, so it does not count; the runner stopped itself at 14:26:51 as
  written. Before the rerun the attempt moved to `cut/t1-f-1-attempt1/`, `spent.py` was amended to sum `cut/` too (the
  ceiling's arithmetic keeping that spend), and the amendment was appended to `registration-runner.md` with its
  diffs; the static preflight read 76 `ok` again; the runner restarted at 14:32:00, skipping the counted `t1-q-1`. (3)
  **The usage limit that stopped the agents at about 14:55 did not reach the runner**, which ran on the `.env` key in
  its own session and ended at 14:59:36.
- **The run**: first session 14:15:53, the verdict ended 14:59:36, `run.log`'s last line *"spent 7.108556 USD in
  all"*; `tools/read_runs.py`: 31 lines, every one `subtype=success is_error=False terminal=completed`, the model
  `claude-opus-5-5` alone in every `modelUsage`.
- **After the run**: every file of the 31 folders read again by sha256 against `key.txt`: **31 folders, 445 files, 0
  differences**, each folder holding beyond them only `result.md`, `run.err`, `run.json`. `tools/scan.py`: **33
  transcripts** (the preflight, both attempts of `t1-f-1`, the other 29 graded and the verdict), **prompt-alone=NO 0,
  git-status=LEAK 0, claude-md-or-memory=YES 0**; `tools/reads.py`: **0 sessions with a path outside their folder**, 0
  without a cwd. **Every `context` answer says no rule, contract or memory reached it** (read whole, 31 of 31); seven
  name words of the working directory's path, which the system prompt gives (`t1-c-4`, `t1-c-5`, `t1-f-5`, `t1-q-1`,
  `t1-q-3`, `t2-c-3`, `t2-q-1`: *"260-llm-ergonomist"* or *"heroes-lang"*), a path and not a rule, as panels 257 and
  259 read theirs. **0 sessions voided.**
- **No session wrote `report.md`** (31 of 31 `report.md=no`), as in panels 257 and 259: under the registered flags
  each session says it holds `Read` alone, and gave its report as its final answer, which `tools/after.py` kept as
  `result.md`, the brief's fallback. So every t1 file graded below is the one the reader wrote **in its answer**, under
  its path, extracted by `score/t1_extract.py` (8 files per reader, 120 in all, no path given twice, no file on disk).
- **The scorer was not blind**: the key's first line was read before grading. Every grade is mechanical against the
  registered table or the key, every t2 row was checked against the report's own words (90 of 90, no correction), and
  the extractors, grades and logs stand in the archive so that anyone can count again.

## The spend, to the cent

| cell | sessions | USD | each |
|---|---|---|---|
| t1 `now` | 5 | 0.97 | 0.19 to 0.21 |
| t1 `adopted` | 5 | 0.99 | 0.18 to 0.21 |
| t1 `rival` | 5 | 1.00 | 0.18 to 0.22 |
| t2 `now` | 5 | 1.13 | 0.22 to 0.23 |
| t2 `adopted` | 5 | 1.06 | 0.20 to 0.22 |
| t2 `rival` | 5 | 1.07 | 0.21 to 0.22 |
| the verdict | 1 | 0.79 | |
| **the 31 counted** | **31** | **7.02** (7.018539) | |
| `t1-f-1`'s cut attempt | (1) | 0.09 (0.090017) | |
| **all** | | **7.11** (7.108556) | |

The 30 graded 6.223946 (the most any spent 0.232410, under its 0.60 cap), the verdict 0.794593: **12.89 USD of the
author's 20.00 unspent**. The free preflight cost 0. The registration's estimate, *"about 6 to 8 USD"*, *an
inference, unrun*, held.

## The key, as run (`blind/registration.md` § Written 2026-11-26, after the landing)

The landed build on this Mac (`.claude/worktrees/gate-push4/heroes-next`, stage 3 of `a60dfb77f`, `sources
6e60cb9f65cc1298`, by its absolute path) and a stage 2 built from `a60dfb77f`'s seed in the Linux arm64 image (the same
`sources 6e60cb9f65cc1298`); three builds of each program in a fresh copy, every binary run three times:

| t2 | what it is | the key, both platforms |
|---|---|---|
| (1) `p1` | a package module binding the clock, its group saying `layer` | **builds**, prints `true`, 9 runs of 9 |
| (2) `p2` | `p1` without the word | **refused**, `ffi_layer_undeclared` at **`ticks/clock.hero`** |
| (3) `p3` | a module binding nothing over the package's module `raw`, whose group says `layer` | **builds** |
| (4) `p4` | `p1`'s package, and a program with a group of its own over `hero_os.h`, no word | **builds**, prints `true` `true` |
| (5) `p5` | a group over the package's own `myclock.h`, which includes `hero_time.h`, no word | **refused**, `ffi_layer_undeclared` at **`ticks/clock.hero`** |
| (6) `p6` | a group over `stdlib.h` saying `layer` | **refused**, `ffi_layer_unreached` at **`num/ops.hero`** |

## t1, scored

**Declared-right** (`score/t1_declared.py`, the registration's table, file by file; `score/t1-declared.tsv`). Every
reader wrote one group per job, in the file the task gives (`score/t1/`): under `now` none says the word; under
`adopted` and `rival` **every group says it, `hero_os.h`'s included**:

| arm | W1 | W2 | W3 | W4 |
|---|---|---|---|---|
| `now` | 0 of 5 (by construction) | **5 of 5** (no word written) | 0 of 5 (by construction) | 0 of 5 (by construction) |
| `adopted` | **5 of 5** | **0 of 5** (5 write `extern "hero_os.h" layer` in `ident/me.hero`) | **5 of 5** | **5 of 5** |
| `rival` | **5 of 5** | **0 of 5** (the same, 5 of 5) | **5 of 5** | **5 of 5** |

**Why W2**: in `choice_points`, `t1-f-1`, `t1-f-4`, `t1-f-5`, `t1-c-2`, `t1-c-3` and `t1-c-5` name it a choice and read
the exception list the same way (*"The only header named as an exception is `hero_compiler.h`"*, `t1-c-5`; *"I read
'every other `hero_` header it provides but the compiler's `hero_compiler.h`' as every `hero_` header the language
provides"*, `t1-c-3`); `t1-c-4` names it a choice and gives no reason but the rule it applied (*"`layer` on every
`hero_` header"*); `t1-f-2`, `t1-f-3` and `t1-c-1` write the word on every group without naming `hero_os.h` a choice
(*"`layer` on every group. The spec requires it here"*, `t1-c-1`). No reader under `now` wrote any word.

**Built** (`score/t1_job.sh`, one build per job and platform with the landed build, each binary run three times; a
job built where its build exits 0 and its three runs print what the task asks; `score/t1-built-mac.txt`,
`score/t1-built-linux.txt`):

| arm | W1 | W2 | W3 | W4 | on |
|---|---|---|---|---|---|
| `now` | refused 5, `ffi_layer_undeclared` at `ticks/clock.hero` | **built 5** | refused 5, at `main.hero` | refused 5, at `main.hero` | this Mac and Linux arm64 alike |
| `adopted` | **built 5** | **built 5** | **built 5** | **built 5** | this Mac and Linux arm64 alike |
| `rival` | **built 5** | **built 5** | **built 5** | **built 5** | this Mac and Linux arm64 alike |

This Mac 14:04:54 to 14:07:15 UTC, six jobs side by side (`.claude/worktrees/gate-push4/heroes-next`); Linux arm64
14:05:13 to 14:15:58 UTC, four side by side, one container, removed after (the stage 2 of the key, `sources
6e60cb9f65cc1298`). Every built job's three runs print what the task asks: `true`, `true`, `stop asked: false`,
`handed: 0`, exit 0.

No job of either platform fell to *other*: every refusal is the declaration's own, `ffi_layer_undeclared` at the
group's line, and every build prints what the task asks, three runs of three. **The `adopted` and `rival` W2, declared
wrong by the text, build and print `true`** on both platforms.

## t2, scored (`score/t2-read.tsv`, every row read again in the report)

Readers build-right of 5, per program (for a refusal, the file named too):

| arm | (1) | (2) | (3) | (4) | (5) | (6) | programs at 4 of 5 or more | the importer's file for (2) or (5) |
|---|---|---|---|---|---|---|---|---|
| `now` | 0 | 0 | 0 | 0 | 5 | 5 | 2 | 0 |
| `adopted` | 5 | 5 | 5 | **0** | 5 | 5 | 5 | **0** |
| `rival` | 5 | 5 | 5 | **0** | 5 | 5 | 5 | 0 |

Every `adopted` and `rival` reader answers (4) *refused, `main.hero`*, for the reason of W2: *"its group names
`hero_os.h`, a `hero_` header, without `layer`"* (`t2-c-1`), *"`hero_os.h` is the language's layer header for
processes and files, so that group must say `layer`"* (`t2-f-1`). Every `now` reader reads the word `layer` as outside
its grammar (*"`layer` is none of these"*, `t2-q-4`), so it answers (1), (3) and (4) refused at the package's file and
(2) built; its (5) right for another reason (no group names `hero_time.h`, so nothing links the clock, `t2-q-3`, its
confidence low), and its (6) right as a syntax error.

## The decision rules (`260-briefs/llm-ergonomist.md` § The decision rules)

- **D1, the text teaches the form: holds.** Under `adopted`, W1 **5 of 5** and W3 **5 of 5** declared-right (at least 4
  of 5 each). Under `now`, 0 and 0, by construction, and the landed build refuses every `now` W1, W3 and W4
  (`ffi_layer_undeclared`, 15 of 15 on each platform), the baseline.
- **D1c, a rule for a header the text does not name: holds.** W4 **5 of 5** under `adopted`: every reader applied
  *"every other `hero_` header"* to `hero_activation.h` (`t1-f-1`, `t1-f-2`, `t1-f-4`, `t1-f-5` say so in a choice
  point).
- **D2, the text never makes a reader declare what needs none: fails.** Under `adopted` **5 of 5** declare in W2,
  where at most 1 may; under `rival` 5 of 5 too. By the registration, **D2 failing narrows the text** (*"before it
  lands"*: the text has landed at `a60dfb77f`, so the narrowing is owed after it). What it must narrow, measured: the
  readers resolve *"every other `hero_` header it provides but the compiler's `hero_compiler.h`"* to include
  `hero_os.h`, the sentence naming `hero_compiler.h` alone as the exception. **Unrun**: no narrowed text was read by any
  reader; that a sentence naming `hero_os.h` beside `hero_compiler.h` in the exception closes it is a question for the
  sitting, not a measurement.
- **D1b, built (reported, not a gate)**: the table above; no *other*. A text that teaches the form did not break the
  rest of any reader's program, and the word over `hero_os.h` broke nothing either, because the landed build accepts
  it.
- **D3, a reader predicts the refusals and where they land: fails on (4).** Under `adopted` 5 of 5 build-right on (1),
  (2), (3), (5) and (6), **0 of 5 on (4)**, where at least 4 of 5 on each of the six are needed; the readers naming the
  importer's file for (2) or (5): **0** (at most 1 allowed). (4)'s failure is D2's cause, not a failure to place a
  refusal: every reader who predicted a refusal named the file the landed build names.
- **D4, locality (reported)**: the `choice_points` on the declaration cite **§ 13 and no other section**, in every
  arm; no reader cites two. Counted per reader as the sections named by number or quoted word for word: `adopted` 1, 1,
  0, 1, 1 (median **1**), `rival` 0, 1, 1, 0, 1 (median **1**), `now` (its points on linking the layer's headers) 1, 0,
  1, 1, 0 (median **1**); by number alone: `adopted` 1, 0, 0, 0, 1, `rival` 0, 0, 0, 0, 0, `now` 1, 0, 0, 1, 0 (each
  median **0**). The thesis prefers one; the readers never needed two.
- **What the run does to the resolution**: D1 holds; D3 fails, and `rival` fails it identically (0 of 5 on (4)), so it
  is **not** a rival that *"satisfies both and `adopted` does not"*: **the resolution stays on `adopted`**. **D2 fails:
  the text is narrowed** (the sitting's to write). D1c holds: no amendment for an unnamed header.

## The spec-warden's predictions, scored (`warden-at-start.md`, section `818747c9…`)

Read by `registration.md` § How a prediction is read: `rival` against `spec-w1-group-mark` as registered (its diff
byte for byte); `adopted` against `w1`'s, the lines its diff has and `w1`'s does not named in the registration (in line
401, *"includes a file of the layer at any depth, or whose library calls the layer"* for *"includes one at any depth,
or that binds a name one declares"*, the clause *"a name one of these declares is bound only by a group whose header
is or includes it"*, and *"holds a group saying `layer`"* for *"holds such a group"*); `now` as registered for every
text.

| rule | arm | predicted | measured | |
|---|---|---|---|---|
| D1 W1 | `adopted`, `rival` | 5 [4-5] | 5, 5 | right, right |
| D1 W3 | `adopted`, `rival` | 5 [4-5] | 5, 5 | right, right |
| D1c W4 | `adopted`, `rival` | 5 [4-5] | 5, 5 | right, right |
| D2 | `adopted`, `rival` | 0 [0-1] | **5, 5** | **wrong, wrong** |
| D3 programs | `adopted`, `rival` | 5 [4-6] | 5, 5 | right, right (and D3 fails as a gate, read apart, as the registration said it could) |
| D3 importer | `adopted`, `rival` | 0 [0-1] | 0, 0 | right, right |
| D4 | `adopted`, `rival` | 1 [1-2] | 1, 1 (number or quote); 0, 0 (number alone) | right by the first count, wrong by the second |
| D1 W1, W3; D1c | `now` | 0 [0-0] each | 0, 0, 0 | right |
| D2 | `now` | 0 [0-1] | 0 | right |
| D3 programs | `now` | 1 [0-2] | 2 ((5) and (6)) | right |
| D3 importer | `now` | 0 [0-1] | 0 | right |
| D4 | `now` | 0 [0-1] | 1 (number or quote); 0 (number alone) | right either way |
| the spend | the 31 | 6.8 USD [4.5-10.0] | 7.02 counted, 7.11 with the cut attempt | right |

The warden's *"D2 is near 0 for every text, since each says `hero_os.h` is the language's"* is the one premise the
run falsifies: each text does say it, and its list then names `hero_compiler.h` alone as the exception.

## The verdict session

`verdict/result.md` (0.794593 USD, 55 turns; `spec-A.md` `now`, `spec-B.md` `adopted`, `task.md` t1 then t2, t2's
programs, the 30 reports by label): **approve**, *"you can tell what the word means from the line it sits on"*. Its
argument names the same doubt the run measured and reads it as harmless: *"The remaining doubt, whether `hero_os.h`
counts as a layer header, can only lead to a refused build, never to a wrong program that builds"*. **Against the
landed build that premise is false**: neither spelling over `hero_os.h` is refused (W2 with the word builds, t2's (4)
without it builds), so the doubt leads to no refusal at all, and to a reader's prediction the build contradicts. Its
second condition, *"readers writing from spec-B often leaving out `layer` on `hero_os.h`, with a refusal message that
doesn't say which group or why"*, met its mirror: readers always write it, and nothing is said.

## What the registration could not do as written, and what was done instead

- **Its owed list**: t2's task and programs, the key and `registration-runner.md` were unwritten; this session wrote
  them before any session, in the detached worktree, appended and never rewritten. **t2's (6)**: `hero_os.h` was the
  obvious header for *"a package that declares and binds nothing"*, and the landed build accepts `extern "hero_os.h"
  layer`, a key the text contradicts; (6) names `stdlib.h` instead, and the registration says why. **t2's (4)**: a
  group of the program's own over `hero_os.h` was added so that the program holds a group it does not declare; it is
  the program D3 failed on.
- **t1's key before any session** (*"the landed build's run of t1's jobs"*): t1's key is the readers' own files, built
  after the sessions on both platforms (above); before any session only the `now` reference was built, once, on this
  Mac, as a baseline (W1, W3, W4 refused, W2 built). **The reference's sources are not on `main`**: the commit holding
  `blind/reference-t1/` holds its `results.txt` alone; the four jobs stand in `.claude/worktrees/panel260`.
- **The built grade's shape**, which the registration leaves open: one build per job and platform, three runs of each
  binary; the jobs six (this Mac) and four (Linux) side by side, no clock read. t2's key, three builds of each program
  and three runs of each binary (*"three runs of three"*).
- **`report.md`**: no session could write it; the brief's registered fallback carried every answer.
- **The cut session**: re-run after its attempt was moved aside, its spend kept in the ceiling by `spent.py`'s
  amendment, recorded with its diff in `registration-runner.md`.
- **D2's consequence**, *"narrows the text before it lands"*: the text landed before the run, which the registration
  made wait for the landed build; the narrowing is owed now.

## Found beside, for the coordinator (unfiled: no GitHub write here)

- **`extern "hero_os.h" layer` is accepted by the landed build, and the text refuses it** (*"and no other group may"*,
  `hero_os.h` being *"the language's"*): `heroes check` exit 0, `heroes build` exit 0, the binary printing `true` at
  exit 0, three builds of three on this Mac and in the Linux arm64 image (`sources 6e60cb9f65cc1298` on both;
  `panel260-blind-scratch/probe6/b`, `key/probe-os-layer`). The landed `told` in `selfhost/cli/layer_bound.hero` judges
  only a group whose header the walk read a reach for (*"A group with no reach read is not judged here"*), and
  `hero_os.h` gets none. A wrong program accepted is `blocking` by `.claude/rules/verification.md` § Bounded discovery;
  10 of 10 `adopted` and `rival` t1 readers wrote exactly this program. Whether the repair is the refusal, or the text
  narrowed so that a reader stops writing it (D2), or both, is the sitting's.

## Where the answers are

`llm-ergonomist-blind-answers.tar.xz` beside this file (text only; 1,276 files, 131,016 bytes, sha256
`82d4a90849ea9feb33241b46ec2473aea250b1f0d47828d2ca560033039c2050`; every file `text`, `JSON data` or empty by
`file`): the area's `run.sh`, `preflight.sh`, `tools/`,
`key.txt`, `order.txt`, `run.log`, the runner's outputs, `registration-at-start.md`, the warden's copies, the
preflight's `run.json`, every session's `result.md`, `run.json` and `run.err`, the cut attempt's, the verdict's folder
but its specs, `scan.out`, `reads.out`, `read_runs.out`, `score/` (the extractors, the grades, the readers' extracted
files, every build's and run's log, no binary and no `build/`), and the key's scripts and logs. The area itself stays
at `.claude/worktrees/260-llm-ergonomist/`, uncommitted.
