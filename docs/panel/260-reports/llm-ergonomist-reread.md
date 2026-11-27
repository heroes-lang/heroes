# Panel 260, llm-ergonomist (the blind seat): the narrowed sentence, re-read as registered, scored

Written 2026-11-27 by the session the coordinator of M-rebel-library launched to run the registered re-read (its brief:
`.claude/worktrees/rebel-scratch/coord/reread260-brief.md`), on the author's authorisation of 2026-11-27 at about 08:25
(the answer *2a*, the last paragraph of `260-briefs/blind/registration.md`: *"the re-read registered above, 20
sessions, ceiling 12.00 USD"*). The run is that registration's section *Registered 2026-11-26: the narrowed text,
re-read*, the file as it stands on `main` at `9f5c59e6b` (sha256 `f9fcd8a8...f8a7`, copied by the runner as it
started), and the runner `260-briefs/blind/registration-runner-reread.md` registers, written before any session. Every
number below is a command's, named beside it; the run's notes are `.claude/worktrees/reread260-scratch/notes.md`. A
reader's *builds* or *refused* is its reading of its arm's text; the **built** grade and t2's key are today's `main`
build's.

## In one paragraph

**20 sessions ran, every one `success`, `is_error` false, `completed`: 4.135307 USD spent** of the author's 12.00
(`tools/read_runs.py` over the 20 JSONs; the free preflight 0). The draw: `adopted=s`, `narrowed=m`. **N1 holds**:
under `narrowed`, **0 of 5** readers write `layer` in W2, every one declared-right, where at most 1 may declare.
**N2 holds**: under `narrowed`, **5 of 5** readers are build-right on t2's (4). **By the registration's rule, the
narrowing stands.** And it cost nothing measured: under `narrowed` W1, W3 and W4 are declared-right 5 of 5 each, and
t2's other five programs build-right 5 of 5 each, no reader naming the importer's file for (2) or (5). **`adopted`, read
again the same day, reads exactly as on 2026-11-26**: W2 0 of 5 (5 of 5 write `extern "hero_os.h" layer`), (4) 0 of 5
(5 of 5 answer *refused, `main.hero`*), every other grade 5 of 5. So the day's model and runner did not move the
reading of the old text, and the new text moved W2 and (4) from 0 of 5 to 5 of 5. **Today's `main` build now tells
the `adopted` readers their mistake**: their five W2 are refused `ffi_layer_unreached` at `ident/me.hero:1:20`, on this
Mac and in Linux arm64 alike, where on 2026-11-26 they built. **What the narrowed text opens, reported for the
sitting**: 5 of its 10 readers name as a choice whether `hero_os.h`'s own library *"calls the layer"* (the clause
*"says no `layer` unless its library calls the layer"*); each chose *it does not*, and none went wrong.

## How it was run

- **What the registration assumed, read on the day before any session** (each command's, 08:28 to 08:40):
  1. **Every input at its registered hash** (`shasum -a 256`): `c3885b81b`'s spec `c915f0f2...2bee`;
     `synthesis-adopted.diff` `78b15a71...`, `synthesis-adopted.txt` `142445a4...`; `task-t1.md` `3d71ca51...` (2,849
     bytes), `task-t2.md` `93d91f72...` (1,658 bytes); t2's 20 program files out of `260-reports/evidence-blind.tar.xz`,
     20 of 20 equal to the registration's table (its lines 225 to 244 parsed and `diff`ed); the session brief
     `ca8e2077...8beb` (274 words), pinned in `run.sh`; the warden's section `818747c9...7dd7ea`, as the first run
     pinned it. **All hold.**
  2. **The two arms rebuilt to their registered sha256**: `adopted`, `now` with the diff by `patch -p1`,
     **`70b8e497...f1a7`**, 34,855 bytes; `narrowed`, `adopted` with its line 401 replaced by § 13's line as step 208b
     wrote it (line 411 of `spec/heroes-spec.md` at `750d2540d` and at `9f5c59e6b`, `cmp`-equal, 910 bytes),
     **`6874087d...74cc`**, 35,051 bytes, `diff` printing one `<` and one `>` (`401c401`); its two sentences alone, 834
     bytes, **`cdd29e79...624d`**. **Both hold.**
  3. **The key, re-run under today's `main`** (§ The key, below): t2's six, and the W2 cell step 208a moved, on this
     Mac and in the Linux arm64 image. **No cell moved.**
  4. **`claude --version`: `2.1.326 (Claude Code)`** (08:36:07), where the first run read `2.1.323`; `claude --help`
     lists all ten flags of the command. The registration pins no version, and reads `adopted` again the same day
     *"so that a change of the model's or the runner's between the two days is not read as the text's"*. **Not a stop;
     reported**, and `adopted`'s reading did not move (above).
  5. **The free preflight** (08:39:44 to 08:39:47): a key the API refuses, `run.json` `subtype` `success`, `is_error`
     true, `terminal_reason` `api_error`, `total_cost_usd` 0, `api_error_status` 401; no file naming its id under
     `/private/tmp/claude-501/`; its transcript `prompt-alone=yes git-status=reader, no commit claude-md-or-memory=no`;
     **the tools offered: `Read` alone**, as in the first run.
- **The runner**: the first run's, as it ran (its archive's `run.sh`, `preflight.sh` and `tools/`, each at its registered
  hash and `cmp`-equal to the first run's area), adapted, **every change a `diff -u` in
  `260-briefs/blind/registration-runner-reread.md`, written before any session, the free preflight included** (placed at
  08:38, sha256 `2a74857d...d554`, the area's copy those bytes still; at 09:03 a dated correction was appended beneath
  them on `main`'s tree, one time it gave without a clock read, so that file now reads `2d50f5d1...48be`): two arms and two tasks, 20 folders, no verdict;
  the cap 0.60 a session, margin 0.10, **ceiling 12.00**; the registration pinned at `main`'s bytes; the arms checked by a
  new `tools/check_arms.py`; the tools the preflight was offered logged; the label pool without the first run's `q`,
  `f` and `c`. **Static preflight** (`bash preflight.sh`): **78 checks `ok`, 0 `FAIL`**, spent 0.000000.
- **The area**: `.claude/worktrees/260-reread-llm-ergonomist/`, drawn at 08:35 as the first run's was, the top of an empty
  git repository of its own (`.gitignore` `*`, `reader <reader@invalid>`, no commit; this repository's `.gitignore:34`
  ignores it), `home/` and `tmp/` inside. **The draw** (08:35:42): `order.txt` `4708a527...`, `key.txt` `4178bfd9...`, 260
  files under `s/`, `check_folders.py` exit 0; `key.txt`'s first line read only after the 20, for grading.
- **Every session**: `claude -p "<the registered prompt>" --bare --restricted --safe-mode --strict-mcp-config --model
  claude-opus-5-5 --tools "Read,Write" --allowedTools "Read,Write" --disallowedTools
  "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" --max-budget-usd 0.60 --output-format json
  < /dev/null` under `env -i` with `HOME`, `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the area and
  `GIT_CEILING_DIRECTORIES` above it, the key from the trunk's `.env` (`ANTHROPIC_API_KEY length 108`, its value never
  printed), one at a time, from its own folder. The runner started at 08:40:38 in a session of its own (`python3`'s
  `os.setsid()` then `exec bash run.sh`; this Mac has no `setsid` command), so a cut of the shell that launched it could
  not kill a session in flight.
- **The run**: first session 08:40:38, the last ended 08:58:22, `run.log`'s line *"spent 4.135307 USD in all"*, the
  runner's lock removed; no cut, no network stop, no session run twice. `tools/read_runs.py`: **20 lines, every one
  `subtype=success is_error=False terminal=completed`, the model `claude-opus-5-5` alone**; every t1 session 4 turns,
  every t2 session 24.
- **After the run**: every folder's inputs read again by sha256 against `key.txt`: **260 files, 0 differences**, each
  folder holding beyond them only `result.md`, `run.err` (0 bytes in all 20) and `run.json`. `tools/scan.py`: **21
  transcripts** (the preflight and the 20), **prompt-alone=NO 0, git-status=LEAK 0, claude-md-or-memory=YES 0**;
  `tools/reads.py`: **0 sessions with a path outside their folder**, 0 without a cwd. **0 sessions voided.**
- **No session wrote `report.md`** (20 of 20 `report.md=no`), as on 2026-11-26: each was offered `Read` alone and gave
  its report as its final answer, kept by `tools/after.py` as `result.md`, the brief's fallback. Every t1 file graded
  below is the one the reader wrote **in its answer**, under its path, extracted by the first run's `score/t1_extract.py`
  (8 files per reader, 80 in all, none on disk).
- **The scorer was not blind**: the key's first line was read before grading. Every grade is mechanical against the
  registered table or the key, by the first run's scorers (`t1_declared.py` reading `narrowed` by `adopted`'s rows, its
  one change, registered before any session and checked first on the first run's own files: its 40 `adopted` and `now`
  rows came out unchanged); every group line the readers wrote was read again by hand against the grades (80 files),
  and every t2 row against the report's own words (60 of 60, no correction).

## The spend, to the cent

| cell | sessions | USD | each |
|---|---|---|---|
| t1 `adopted` | 5 | 1.02 (1.015266) | 0.18 to 0.23 |
| t1 `narrowed` | 5 | 0.94 (0.943758) | 0.18 to 0.19 |
| t2 `adopted` | 5 | 1.10 (1.101702) | 0.21 to 0.23 |
| t2 `narrowed` | 5 | 1.07 (1.074581) | 0.20 to 0.23 |
| **all** | **20** | **4.14** (4.135307) | |

The most any session spent, 0.233196 (`t2-s-4`), under its 0.60 cap; the free preflight 0. **7.86 USD of the author's
12.00 unspent.** The registration's estimate, *"about 4.1 USD for the 20, an inference, unrun"*, held. The cells sit
beside the first run's (`adopted` t1 0.99 and t2 1.06 for five sessions each).

## The key, as run on the day

**Today's `main` build**: `git archive 9f5c59e6b seed runtime selfhost`, clang on the seed (`sources 1e1eb23a1927d35b`),
then stage 2, **`sources 1803fd0e58b66d44`**, on this Mac (Apple clang 21.0.0, Darwin arm64) and in the Linux arm64
image (`heroes-linux-arm64:latest`, no pull, one container `rr260`, Debian clang 22.1.8, glibc 2.41, aarch64; removed
at 09:01, `docker ps -a` naming none after). The two stage 2 print the same `sources`.

**t2's six**, the first run's `key.sh` unchanged (sha256 `80c144b4...`), three builds of each program in a fresh copy,
every binary run three times (this Mac 07:37:14 to 07:39:13 UTC, Linux 07:38:01 to 07:39:22 UTC; the 20 program files
in the container at their registered hashes); each platform's log read against the registered key by a script that
reads the first run's two logs as holding it (0 moved):

| t2 | the registered key | this Mac | Linux arm64 |
|---|---|---|---|
| (1) `p1` | **builds** | builds 3 of 3; 9 runs `true`, exit 0 | the same |
| (2) `p2` | **refused, `ticks/clock.hero`** | exit 1, 3 of 3, `ffi_layer_undeclared` at `ticks/clock.hero` | the same |
| (3) `p3` | **builds** | builds 3 of 3; 9 runs `true`, exit 0 | the same |
| (4) `p4` | **builds** | builds 3 of 3; 9 runs `true` `true`, exit 0 | the same |
| (5) `p5` | **refused, `ticks/clock.hero`** | exit 1, 3 of 3, `ffi_layer_undeclared` at `ticks/clock.hero` | the same |
| (6) `p6` | **refused, `num/ops.hero`** | exit 1, 3 of 3, `ffi_layer_unreached` at `num/ops.hero` | the same |

**No cell moved on either platform**; every program has its key and both rules read on it.

**W2's cell**, which the registration says step 208a moved (*"`extern "hero_os.h" layer` is now refused
`ffi_layer_unreached` at `ident/me.hero`"*): the first run's 15 W2 jobs, from its archive's `score/t1/`, built once each
by today's `main` with the first run's `score/t1_job.sh`, each binary run three times: the 10 that write `extern
"hero_os.h" layer` (its `adopted` and `rival` readers) **refused `ffi_layer_unreached` at `ident/me.hero:1:20`, 10 of
10**, and the 5 that do not (its `now` readers) **built, 15 runs `true`**, on this Mac and in Linux arm64 alike. The
registered cell holds. t1's key is otherwise each reader's own files, built after the sessions (below).

## t1, scored

**Declared-right** (`score/t1_declared.py`, the registration's table, file by file, `narrowed` by `adopted`'s rows;
`score/t1-declared.tsv`). Every reader wrote one group per job, in the file the task gives (`score/t1/`); no catalogue of
any reader holds the word:

| arm | W1 | W2 | W3 | W4 |
|---|---|---|---|---|
| `adopted` | **5 of 5** | **0 of 5** (5 write `extern "hero_os.h" layer` in `ident/me.hero`) | **5 of 5** | **5 of 5** |
| `narrowed` | **5 of 5** | **5 of 5** (5 write `extern "hero_os.h"`, no word) | **5 of 5** | **5 of 5** |

The groups, as written: under both arms `extern "hero_time.h" layer` in `ticks/clock.hero`, `extern "hero_net.h"
layer` and `extern "hero_activation.h" layer` in the programs, no group in any `main.hero` of W1 or W2.

**Why W2, in the readers' own words.** Under `adopted`, as on 2026-11-26, the exception list read as naming
`hero_compiler.h` alone: *"For `hero_os.h` I also chose `layer`. The text describes it apart from the "platform layer",
but the only header it exempts is `hero_compiler.h`"* (`t1-s-2`); *"The spec names "the language's `hero_os.h`" apart
from the platform layer, but it then says every other `hero_` header counts except `hero_compiler.h`"* (`t1-s-4`);
`t1-s-1` and `t1-s-5` name it a choice as well, `t1-s-3` writes the word without naming it one. Under `narrowed`, the
exclusion read at its word: *"Adding `layer` after `"hero_os.h"`: `hero_os.h` is not part of the layer, and no other
group may say `layer`"* (`t1-m-2`); `t1-m-1`, `t1-m-3` and `t1-m-4` name another choice, whether `hero_os.h`'s library
calls the layer (*"If the library behind `hero_os_pid` called the layer, the group would have to say `layer` and the
compiler would refuse `w2/ident/me.hero`. The spec doesn't say whether it does"*, `t1-m-4`), and each chose that it does
not.

**Built** (`score/t1_job.sh` unchanged, one build per job and platform with today's `main`, each binary run three
times; a job *built* where its build exits 0 and its three runs print what the task asks; `score/t1_built.py`,
`score/t1-built-mac.txt`, `score/t1-built-linux.txt`):

| arm | W1 | W2 | W3 | W4 | on |
|---|---|---|---|---|---|
| `adopted` | **built 5** | **refused 5**, `ffi_layer_unreached` at `ident/me.hero:1:20` | **built 5** | **built 5** | this Mac and Linux arm64 alike |
| `narrowed` | **built 5** | **built 5** | **built 5** | **built 5** | this Mac and Linux arm64 alike |

This Mac 07:58:48 to 08:00:48 UTC, six jobs side by side; Linux arm64 07:58:49 to 08:00:23 UTC, four side by side, in
`rr260`; the two platforms' grades agree job for job (`diff` empty). Every built job's three runs print `true`, `true`,
`stop asked: false` or `handed: 0` at exit 0 (105 runs of 105 on each platform). No job fell to *other*. The five
refusals carry step 208a's message, *"`hero_os.h` is the runtime's own header, none of the platform layer's, and
includes none of them, so nothing it binds is the layer's: take the word out"*, `fix (certain): take `layer` out`.

## t2, scored (`score/t2-read.tsv`, every row read again in the report)

Readers build-right of 5, per program (for a refusal, the file named too):

| arm | (1) | (2) | (3) | (4) | (5) | (6) | programs at 4 of 5 or more | the importer's file for (2) or (5) |
|---|---|---|---|---|---|---|---|---|
| `adopted` | 5 | 5 | 5 | **0** | 5 | 5 | 5 | 0 |
| `narrowed` | 5 | 5 | 5 | **5** | 5 | 5 | **6** | 0 |

Every `adopted` reader answers (4) *refused, `main.hero`*, for W2's reason: *"`hero_os.h` is one of the language's own
headers, so the group must write `layer`"* (`t2-s-1`), *"`hero_os.h` is a `hero_` header and is not `hero_compiler.h`,
so I read it as part of the layer"* (`t2-s-4`). Every `narrowed` reader answers (4) *builds*, quoting the new clause:
*"§13 says `hero_os.h` is not one of the layer's headers, and "a group over `hero_os.h` ... says no `layer` unless its
library calls the layer". The group names no library, so not saying `layer` is correct."* (`t2-m-1`); `t2-m-1` and `t2-m-4`
name whether `hero_os.h`'s library calls the layer as a choice, `t2-m-2` and `t2-m-3` whether `hero_os.h` counts as the
layer, `t2-m-5` whether the group may stand beside a package's `layer` group; all five choose the key's answer.

## The decision rules (`registration.md` § Registered 2026-11-26: the narrowed text, re-read)

- **N1, D2 on the narrowed text: holds.** Under `narrowed`, **0 of 5** readers declare in W2 (declared-right **5 of
  5**; at most 1 may declare). **The narrowing stands.**
- **N2, D3 on (4): holds.** Under `narrowed`, **5 of 5** readers build-right on t2's (4) (at least 4 needed). **The
  narrowing stands.**
- **Reported, `adopted` this day**: W2 **0 of 5** and (4) **0 of 5**, the first run's 0 and 0 again; and its W1, W3, W4
  and t2's other five 5 of 5 each, as on 2026-11-26. The day's model and runner, `2.1.326` against `2.1.323`, did not
  move the old text's reading.
- **Reported, the narrowing's cost under `narrowed`**: W1, W3 and W4 declared-right **5 of 5** each; t2's (1), (2),
  (3), (5) and (6) build-right **5 of 5** each; no reader naming the importer's file for (2) or (5). **None below 4 of
  5: no cost measured.**

## The `context` question

**20 of 20 answers say no instruction, rule, contract, memory or document outside their folder reached them** (each
`result.md`'s `context` read whole, `score/context.txt`, by a small extractor `score/context.py`, new, the first run's
not being in its archive). Three name the working directory's path, which the system prompt gives: `t1-s-1` and
`t1-s-4` (*"260-reread-llm-ergonomist"*, *"reread-llm-ergonomist"*, *"only a path name"*) and `t2-m-3`, which adds that
the path *"hints at what the experiment is for"*; a path and not a rule, as panels 257, 259, 261 and 262 and the first
run read theirs. **0 sessions voided.**

## What is unrun, and what the run could not do as registered

1. **No verdict session**, as registered; nothing here says what a reader handed both texts would choose.
2. **The x86-64 legs, Windows and MSVC** are not graded; nothing here says anything of them.
3. **The scorer was not blind** (the key read before grading), as in the first run; every grade is mechanical and its
   scripts and logs stand in the area.
4. **No session could write `report.md`**: 20 answers are the fallback `result.md`, as on 2026-11-26.
5. **The first run's static check** *"the worst case stays under the ceiling, margin included"* cannot hold at the
   registered numbers (20 x 0.60 + 0.10 = 12.10 > 12.00), so the re-read's preflight checks the registration's own
   *"20 sessions at the cap"* within the ceiling; the stop rule was the first run's, and the 20 spent 4.135307.
6. **The runner's changes beyond the arms' list**: the registration names the first run's runner *"with the arms' list
   read as `adopted` and `narrowed`"*; running it as registered also needed the ceiling, the absence of a verdict and
   the preflight's arm check changed, and `read_runs.py` taught a run without a verdict. Each is a `diff -u` in
   `registration-runner-reread.md`, written before any session; the label pool's leaving out `q`, `f` and `c` is this
   session's choice, recorded there.
7. **The narrowed sentence's real token count** is step 208b's (*+73 real*, the sitting's amendment); not measured again.

## Found beside, for the coordinator (unfiled: no GitHub write here)

- **The narrowed clause *"says no `layer` unless its library calls the layer"* opens a doubt it does not close**: 5 of
  the 10 `narrowed` readers (`t1-m-1`, `t1-m-3`, `t1-m-4`, `t2-m-1`, `t2-m-4`) ask whether *"its library"* means the
  library behind `hero_os.h` itself, the runtime's, rather than a library the group names with `link`; the sentence's
  own R3 meaning is the second. All ten chose the answer the key gives, so it cost nothing measured here; a reader
  who resolved it the other way would have written `layer` over `hero_os.h` and been refused `ffi_layer_unreached` at
  build. A question for the sitting, not a measurement of harm.
- **The first run's `preflight.sh` stops silently on two of its own failures**: its checks *"the arms are the frozen
  spec ..."* and *"every arm's full sha256 stands in the registration's own table"* put `|| exit 1` inside the string
  `chk` hands to `eval`, which ends the script itself at exit 1 with no `FAIL` line and no later check (run between the
  clock readings 08:33:57 and 08:35:23, `bash -c` of that shape: nothing printed, `shell exit 1`). Fail closed, so no session could have run past it, but the
  reason would not have been printed. The re-read's copy runs the one such check it keeps in a subshell.
- **A blind area's name reaches its sessions**: the system prompt gives the working directory, and `t2-m-3` reads
  *"260-reread"* in it as a hint of the experiment. The arms are not in the name, and the scan counts it as no leak, as
  every sitting since 257 has; whether a neutral area name is owed is a question for the runner, unrun.

## Where the answers are

`llm-ergonomist-reread-answers.tar.xz` beside this file (sha256
`07d9c57b4cffc8ea2c8b2887fe010e895fb5566ff4ba033bff8e2f212906e6fc`, 32,088 bytes, 40 entries): each of the 20 sessions'
`result.md` and `run.json`, under `260-reread-llm-ergonomist/s/<session>/`, never a key or a spec (every file `text` or
`JSON data` by `file`; no credential, `grep` for the key's prefix and name empty). The runner's registration is
`260-briefs/blind/registration-runner-reread.md`. The area (`.claude/worktrees/260-reread-llm-ergonomist/`: `key.txt`,
`order.txt`, `run.log`, `runner.out`, `read_runs.out`, `scan.out`, `reads.out`, `after-hashes.out`, the preflight's
outputs, the transcripts under `home/`, `score/` with the extracted files, the grades and every build's log) and the
scratch (`.claude/worktrees/reread260-scratch/`: `notes.md`, the rebuilt inputs, both stage 2 builds' logs, `key/` with
both platforms' key logs and `compare.py`, `w2check/`, the runner's diffs) stay uncommitted.
