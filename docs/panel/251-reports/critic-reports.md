# Panel 251, the completeness critic's second reading: the five reports against the world

Read 2026-11-19 from 13:09 CET (`date`), after every seat had finished (the compiler-engineer's status
line reads 13:06, final) and before the synthesis. Read in the order the coordinator's prompt names:
`.claude/skills/panel/SKILL.md`, the six briefs, my first reading (`critic-briefs.md`), then the five
reports, the blind seat's body and its appendix where a grade is checked.

Every command ran in my own copy, `.claude/worktrees/rebel-scratch/251-critic/tree/` (a `git archive`
of `3a5ee269117a31780ef1f96d0305452630ac4d83`, stage 2 `sources 23c7d2a626b29fd5` by `./heroes-next
--version`, read 13:09), or against the git objects of `.claude/worktrees/rebel-round`, never its
working tree. A seat's file I needed to run was **copied** into `251-critic/work2/` first and run there;
no seat's folder was written, built in or run in. No paid run, no `claude -p`, no API call, no
credential printed, no GitHub write, no sub-agent, no commit. At most one build or run at a time; a
program that may not end ran as a built binary under `timeout -s KILL`, its output to `/dev/null` or
through `head -c`. Load at the start: 1.86 (`uptime`).

Kinds: **wrong** (a seat's or the coordinator's fact the command contradicts), **unrun** (a claim with no
command behind it, or a negative not searched), **route** (an option nobody listed), **rule** (a rule of
`SKILL.md` or CLAUDE.md a step did not keep), **contradiction** (two seats, and which side the command
takes), **question** (what the sitting should have asked and did not). Findings are numbered in the
order found; the summary at the foot puts first those that change what the synthesis may adopt.

## Findings

### 1. `5c5e6f811`, committed during the sitting, moved nothing any seat measured (rule; settled by command)

`git diff --name-only 3a5ee2691 5c5e6f811`: 28 files, every one under `packages/rebel/0.1.0/files/`
(`bytes`, `cbor`, `compress`, `hash`, `http`, `poll`) and `examples-rebel/README.md`; no `selfhost/`,
`runtime/`, `seed/`, `spec/` or `tests/` path. At both commits, by `git grep` over the git objects:
B's declarations **124 in 98 files** both, the door files **38** both, Rebel's `threads.*` callers **22**
both; the brief's ledger census (`python3 -I 251-briefwriter/tools/ledger_threads2.py` over a `git
archive` of `packages/rebel/0.1.0/files` at each commit, `work2/c53/`): **86 blocks, 38 refused, 9
pending, 39 kept** both, and the two outputs equal once line numbers are stripped (`diff`, exit 0). The
one move: `http.ledger` gains a 7-line block at 298, so its server rows stand +7 lower, exactly the
*"459-464 and 515-520 at the frozen commit, 466-471 and 522-527 beside it"* the brief already named
(`00-shared.md:478-480`); `poll.ledger`'s change is one line rewritten in place (122), so its cited rows
(100-104, 140-145, 159-168) stand. Every seat copied the frozen commit by `git archive`, and the
coordinator graded the blind seat on the brief-writer's frozen stage 2. **So no reading moves.** The
letter of `SKILL.md:234-240` (*"the working tree is frozen from the moment the briefs go out until the
synthesis is written"*, binding the coordinator) was not kept, and the synthesis should say so with
this measurement beside it; the brief had already left the working tree out of every measurement
(28 uncommitted Rebel files when it was written, `00-shared.md:46-49`), which is what made the breach
harmless.

### 2. A third thing reached all 51 blind sessions, which the coordinator's account omits and every session's `context` answer missed: the main checkout's git status, five commit subjects included (wrong; the leak's cause settled with no paid run)

The coordinator's report (`llm-ergonomist.md:24-43`) names two things that reached every session, the
labels of `order.txt` and the folder's path, and rests *"why no session is voided"* on 51 answers of
`context: no`. **The sessions' own transcripts settle both the cause it left as a hypothesis and a
third leak.** With `env -i HOME="$HOME"` the CLI kept each session's transcript under
`~/.claude/projects/-Users-joseph-Temp-heroes-heroes-lang--claude-worktrees-rebel-scratch-251-llm-ergonomist-<folder>/<session_id>.jsonl`,
the `session_id` each `run.json` names. Read by a script (`work2/blind/scan.py`, which prints only
entry types and counts; nothing was written there):

- **The labels**: the first user entry of every one of the 51 is the runner's prompt, a newline, and
  `order.txt` byte for byte (`queue-operation` and `user` entries of `t1-y-1`, `09:08:25.491Z`). `run.sh`
  passes the CLI nothing else that holds the list, and its loop reads `done < order.txt`
  (`251-llm-ergonomist/run.sh`, read whole), so **the cause is the inherited standard input, no longer
  a hypothesis**. The loop reached all 51 folders because the CLI read the file from its start without
  moving the shared offset (every session, the last included, holds the list from `t1-y-1`); how it read
  it is the CLI's and unread.
- **The git status, not in the coordinator's account**: every one of the 51 transcripts holds an
  `attachment` of type `session_context` whose `gitStatus` is **rendered into the conversation** as a
  user-role `<system-reminder>` (*"As you answer the user's questions, you can use the following
  context: # gitStatus ..."*): `Current branch: main`, `Status: (clean)`, the git user's name, and five
  commit subjects of the main checkout, the first *"0becec5b6 Batch 39 round closed: a trap a library
  recovers goes on, a placed tree written into by nothing, a number past an i64 told by name, every gate
  read green"*, the others naming defects 2567, 2572, 2573 and 2576 and *"Part 6 item 9"*. Counted:
  `51 ('order', 'git', '-', '-', '-')`, so 51 of 51 hold the labels and the rendered git status, **none
  any text of `CLAUDE.md` or of the memory index**, and no attachment but `session_context` and `date`.
  `GIT_CEILING_DIRECTORIES`, the brief's guard, did not stop it (it stops `git rev-parse` from the
  folder, as the brief measured, and not this attachment); `claude --help` (2.1.311) names no switch
  that removes it (`--exclude-dynamic-system-prompt-sections` moves the git status into the first user
  message, by its own words). This is the shape panel 183 recorded as `context: yes` (*"a git status
  whose commit subjects named the sitting's own area"*, `SKILL.md:149-151`), and my first reading's
  finding 4 asked it as a question (*"whether a session ... is handed the repository's git status"*).
  The answer is yes.
- **What the sessions said**: the brief asked *"whether any instruction, rule, contract, memory or
  document other than the files in this directory reached your context, and which"*
  (`tools/brief-session.md:29-30`). The answers read name the system prompt, the date and the list of
  labels (`t1-y-1`, `t3-c-2`, `verdict`, quoted from their `result.md`), and **no answer of the 51 names
  the git status or a commit**: `grep -l -i` over every `result.md` prints 0 files for `commit`,
  `Batch`, `branch`, `trap a library`, `2567`, `Recent`, `snapshot`, and the 12 files `git` matches say
  *digits* or the spec's `git` package. **So `context: no` was false of 51 sessions out of 51, and is
  not evidence of a clean context in this set-up.**

**Should either leak void a reading?** Not by the letter: the rule voids on an answer of `yes`, and
none was given. On the substance, read from the answers (an inference, no session re-run): no answer
uses a label or a commit subject (the greps above, and the labels named only in `context` sections); the
subjects name no thread, mailbox or end of `main`, so they reach task 3's area (a trap) alone, and they
reached both of task 3's arms alike, a common-mode exposure that the 4-against-5 reading does not turn
on. **What the synthesis may not do** is write *"no session is voided because none answered yes"* as
if the answer measured the context: it records the three leaks by the transcripts, the guard that
failed, and the reading it keeps on that basis. **The repair owed before sitting 252's runner**: `<
/dev/null` on the `claude -p` line (the coordinator's), and a route that keeps the git status out,
**unrun** (no flag found; a folder with no `.git` above it is outside the root, which the hard stop
forbids): a question for the author before the next blind seat spends.

### 3. The blind seat's grading holds cell by cell; the ceiling the coordinator inferred on task 2 is now measured: all five `now` programs are silently wrong once a document holds their separator (settled by command; a measurement that informs Q3 and pays nothing)

**Re-graded by the rows `llm-ergonomist.md` (brief) registered**, reading every session's `prediction`
section in its `result.md` (`awk` over the blind area, read-only):

- task 1: `now` **0 of 5** (all five predict a normal end, one `main done` line, the worker's not
  written; `t1-y-2` hedges the status, *"presumably 0"*, not the ending), `q2-r` **5 of 5** (each an
  abort naming one thread never joined), `q2-e` **5 of 5** (each a normal end with a notice of one thread
  ended); `now` re-graded by either row: 0. The ending classes of `now`: **normal 5**.
- task 3: `now` **4 of 5**, `t3-y-3` predicting *"`heroes build main.hero` refuses the program"*;
  `q1-k` **5 of 5**. Task 4: **5 of 5** in each spelling, `received: 1`, a normal end.
- task 2, `now`, **rebuilt by my stage 2** (`sources 23c7d2a626b29fd5`, not the brief-writer's): each
  answer's `main.hero` taken as the longest fenced block of its `result.md` is `cmp`-equal to the
  coordinator's extraction (`coord/251-grade/t2/<s>/main.hero`, all 15), and the five `now` programs
  build (exit 0) and write the four lines exactly, exit 0, stderr empty, **25 of 25 runs** under
  `timeout -s KILL 20` (`work2/t2/`).
- the rules' arithmetic: Q2's two differences 5 and 5, Q3's 0 and 0, B against M 10 against 10, task 4 5
  and 5, Q1's 1; the spend **8.690507 USD**, the 50 graded **0.1322 to 0.2112**, sum **8.313339**, the
  verdict **0.377167** (`python3 -I` over every `run.json`). Every count the coordinator wrote holds.

**The ceiling, measured** (the coordinator's *"a task whose input holds one is what would measure
it"*, an inference in its report): each `now` program with one document's text changed to hold the
separator its own encoding uses, everything else byte-equal (`work2/t2/sep-<s>/`, one edit asserted to
match once by `python3 -I`, built by my stage 2, run under `timeout -s KILL 20`). The task's own rule,
words separated by single spaces, gives `a: 4 words, 4 distinct` for `the cat|dog sat on the mat` and
`a: 3 words, 3 distinct` for `the cat` + newline + `sat on the mat`:

| program | its separator | built | exit | line `a` written | stderr |
|---|---|---|---|---|---|
| `t2-y-1` | `\|` | 0 | 0 | `a: 1 words, 1 distinct` | empty |
| `t2-y-2` | `\|` | 0 | 0 | `a: 1 words, 1 distinct` | empty |
| `t2-y-3` | newline | 0 | 0 | `a: 1 words, 1 distinct` | empty |
| `t2-y-4` | newline | 0 | 0 | `a: 1 words, 1 distinct` | empty |
| `t2-y-5` | newline | 0 | 0 | `a: 1 words, 1 distinct` | empty |

(My first run of the last three put a backslash and an `n` into the Heroes string instead of a newline
and read right, which tested nothing; discarded and re-run as above.) **Five of five `now` answers are
accepted programs that print a wrong count and exit 0** on an input the task's own words allow: the
silent wrong program the thesis exists to refuse. The B and M answers encode nothing (read; none
splits a joined text), and **no compiler builds them**, so whether they are right on that input is
**unrun**. What this does and does not do: it does **not** move the registered reading (Q3's rule
reads *neither spelling serves the thesis*, 0 against 0, and stands as registered), and a measurement
taken after the run pays nothing under design.md §1.6 (a payment is a prediction registered before it
is scored). It is the measured basis for **a reading nobody registered**: task 2 with a separator in
its input, its rule written before it runs (finding 4 says what it would decide).

### 4. The W-predictions' timing: the brief's condition was unmeetable by the coordinator's own launch order, the predictions were blind in fact, and "for information" has consequences for R's sentence and for Q3 that the coordinator's report does not draw (rule; settled by the seat's transcript)

**The brief's rule**, in its words: *"The spec-warden's registered prediction, if it registers one
before `run.sh` starts, is scored on these counts as it is worded"* (`llm-ergonomist.md` brief, § The
decision rules), and *"Register yours in your report before the coordinator starts `run.sh`"*
(`spec-warden.md` brief, item 4). So scoring W1 to W8 *"for information"* is the brief's rule applied
to its letter. **But the letter could not be met**: the spec-warden's transcript (the coordinator's
session's `subagents/agent-aeb09f5a90c6de983.jsonl`, its `meta.json` naming `agentType: spec-warden`)
opens at **10:08:11.945 CET**; `run.sh`'s first session started **10:08:24** (`run.out`, and the first
blind session's own `queue-operation` at `09:08:25.491Z`): **13 seconds after the seat was launched**.
The same brief planned `run.sh` *"after the critic's reading of these repairs"* (`llm-ergonomist.md`
brief, line 3-4); no file of the sitting records a critic's reading of the repairs (mine ends 09:51,
before them). **The predictions were blind in fact**, which is what the condition is for: the seat
wrote them by `cat >>` at **10:14:05.698 CET** (the transcript's tool call, its text before it: *"My
prediction must be registered before `run.sh` starts, so I write it into the report now"*), and a
script over every tool call of that transcript finds **none naming the blind area, `run.out`,
`run.log` or a `result.md`** but its own two edits of its report's heading (10:27, which replaced
*"written before `run.sh` starts"* by the honest *"Whether the coordinator's `run.sh` had started by then
I did not read"*). The coordinator's reading of the same transcript said the same.

**What "for information" does to what the synthesis may adopt, which the coordinator's table does not
say**:

- **R's sentence has no payment if W1 is not admitted.** design.md §1.6 (lines 323-335): *"Every
  amendment owes a named removal or a registered prediction"*, unconditionally; the spec-warden's
  payment table pays R's +9 real tokens with W1 and nothing else (`spec-warden.md:322-327`), and the
  brief's decision rules are rules, not predictions. So either the synthesis **admits W1 to W8 as
  registered**, on the transcript (blind, and late only by the coordinator's 13 seconds), or **R's
  sentence waits** for a reading registered before it runs, R's runtime half landing alone (the
  spec-warden's own fallback, *"the runtime's true message carries the rule"*). Admitting them is a
  departure from the brief's letter the synthesis must write down as one; which way is the author's.
- **Q3 is unpaid and the spec-warden's veto condition is met either way.** W4 is falsified, B 0 and M 0
  against `now` (re-counted, finding 3), and the brief's own registered rule reads the same (*"neither
  spelling serves the thesis"*). The spec-warden's Q3 verdict reads *"It becomes a veto if W4 fails for
  both spellings"*, and its definition gives it that veto (*"or if Principle 0's burden of proof is
  unmet"*, `.claude/agents/spec-warden.md:45-46`). Whether W4 was registered in time does not change a
  count, so **the spec-warden's Q3 objection stands as a veto by its own stated condition**, and the
  coordinator's report, which scores W4 *"no"* and stops, does not say so. Finding 3's measurement is a
  measured argument for the thesis effect, taken after the run; it lifts nothing by itself, and a
  reading registered first (task 2 with a separator) is the route to it. **Its cost is a paid run**:
  the bound has 1.31 USD left (10 less 8.690507), about seven sessions at this sitting's median, so a
  five-replicate reading of three arms does not fit (an inference from the costs above): the author's.

### 5. The compiler-engineer's stale record reaches J on Darwin as the ffi-pragmatist built it, 40 of 40; the validated record carries to Darwin and closes it, every other result of D-rebind+J kept on the cases run (contradiction, settled by building and running)

**The contradiction**: the compiler-engineer vetoes J *as worded* on `ijstale.c` (a recorded buffer
refilled by `memcpy` inside the handler, then jumped to, the callback twelve frames deep) and writes
*"and so does Darwin's entry rule"* in its argument, but its own last line says *"unrun on Darwin by
me: a question for that seat"*; its evidence for Darwin's rule is `bje`, the entry rule re-written for
Linux. The ffi-pragmatist approves J on Darwin and ran no stale record. **Run on this Mac**, in
`work2/q1/`, each runtime a copy of the ffi-pragmatist's folder (`rt/drebindj/parts/rebind.c` is
`#define HERO_ROUTE_J 1` followed by `q1/rebind-j.c` byte for byte, `74f666a3998d6012`, the file its
report names; `rt/drebind/` the same file with `0`), the program the compiler-engineer's
(`q1/ship/ij/ijstale.c`, `224beb66ee02fe33`, with its `ij.h` and `ij.hero`), its library built as that
seat's `run3.sh:135` builds it (`clang -std=gnu11 -c -O2 -fno-omit-frame-pointer`), each program built
by my stage 2 with `HEROES_RUNTIME` naming the variant, 20 runs per cell under `timeout -s KILL 20`:

| program | level | base | D-rebind | **D-rebind+J (`rebind-j.c`)** | D-rebind+J validated (mine) |
|---|---|---|---|---|---|
| `ijstale.c` (the stale record) | `-O0` | inside, 20/20 | inside, 20/20 | **inside, exit 0, *"answer ran for 30"*, 20/20** | refused, R1, 20/20 |
| | `-O2` | inside, 20/20 | inside, 20/20 | **inside, 20/20** | refused, 20/20 |
| `ij.c` (249's in-handler jump, built `-fomit-frame-pointer` as its header says): the control | `-O0` | inside, 20/20 | inside, 20/20 | refused, 20/20 | refused, 20/20 |
| | `-O2` | inside, 20/20 | inside, 20/20 | refused, 20/20 | refused, 20/20 |

The control shows J active in my build (it refuses what D-rebind alone runs inside), so **the stale
record is J's own miss on Darwin, 40 of 40, Heroes code run inside the signal**: defect 2522 itself,
on the route the ffi-pragmatist approves. Mechanism, read in `rebind-j.c:95-123`: `ij_prime`'s
`setjmp(ij_env)` records `ij_env` with the entry live at that save (none, outside every handler);
the jump to `ij_env` after the `memcpy` finds that record and `tramp_set`s it, so the thread reads as
outside every handler. (With the library built with frame records the walk of eight catches the
plain `ij.c` under every runtime, 20 of 20; that first control was not discriminating and is not
counted.) **So the ffi-pragmatist's Q1 approval stands on a route the compiler-engineer's veto reaches,
now run on Darwin; its condition list (miss words, the `dlsym` installer) does not name this case.**

**Does a validated record carry to Darwin? Yes, on the cases run.** `work2/q1/make_jv.py` applies five
asserted edits to a copy of `rebind-j.c` (`rt/drebindjv/`, 246 lines, `-fsyntax-only -Wall -Wextra`
clean at `-O0` and `-O2`): each record gains a fingerprint of its buffer's first 192 bytes (Darwin
arm64's `jmp_buf` is 48 `int`s), taken at the thread's next save point or jump once the C library has
filled it (a second pthread key holds the one record still unread, so nothing allocates, locks or calls
`dlsym`), and a jump whose buffer no longer reads as its fingerprint is a miss, which keeps every
entry: the compiler-engineer's `bjv` rule, written for Darwin. 20 runs per cell, 3 for each depth
program (`work2/q1/jv-matrix.txt`), every library the ffi-pragmatist's own, copied:

| case under the validated variant | `-O0` | `-O2` |
|---|---|---|
| `ijstale.c` | refused 20/20 | refused 20/20 |
| `ij.c`; `ijv.c` HOW 1, 2, 3 | refused 20/20 each | refused 20/20 each |
| `kmiss-darwin` | refused 20/20 | refused 20/20 |
| `miss.c` HOW 1 (a copy jumped to WITHIN the handler) | refused 20/20 | refused 20/20 |
| `miss.c` HOW 2 (a copy jumped to OUT of the handler, then a correct callback) | **refused 20/20, R1's false words** | **refused 20/20** |
| the twelve depth programs | **36/36 exit 0**, their lines | **36/36 exit 0** |

So on Darwin the validated record refuses the stale record, keeps every refusal and every non-refusal
of D-rebind+J on these cases, and keeps its one known price (the `memcpy`'d jump OUT told R1's false
words, the ffi-pragmatist's residue 1, which validation neither causes nor cures). **J can be one rule
on both platforms, and the rule is the validated one**; the compiler-engineer's *"J is one rule on both
platforms only if the rule is validated"* is now run on both. **Unrun under the validated variant**:
`flib` static and dylib, the `fliblocale` storms, the 2558, 2494 and recovery goldens through the `run`
form, Guard Malloc, the census libraries, ASan and TSan beside it, the forked children; and the
residue the compiler-engineer names (a buffer refilled between its save and its jump with no J event
between) on Darwin. **What the synthesis may adopt**: J only validated, on both platforms, with those
rows owed by the lane before 2522 closes; J as worded is vetoed on Darwin too by the run above.

### 6. Q2: the compiler-engineer's R hangs where the ffi-pragmatist's R2 ends; so "approve R" is two different runtimes, and only R2 keeps R's promise. The hang is today's too, and the new `hero_panic` defect reproduces on the frozen runtime (contradiction and defect, settled by running)

**The two approvals are not one route.** The compiler-engineer approves R as it built it; the
ffi-pragmatist approves R *"with its gate flushing stdout only where no thread holds it (R2)"*. The
compiler-engineer's gate calls `fflush(stdout)` before its message (`251-compiler-engineer/rt-R/parts/alloc.c:328`,
read), the shape the ffi-pragmatist measured hanging in its own first R; the compiler-engineer's
robustness runs (`rb_print`) wrote to a file through a bound, never to a pipe nobody reads. **Run**
(`work2/q2/`, both seats' runtimes copied, `fp-quiet.hero` copied from the ffi-pragmatist: a worker
printing without end, `main` returning after 0.2 s), each binary built by my stage 2, stdout a pipe
nobody reads, `pipe_run.py` waiting 8 s, then closing the pipe:

| runtime | 3 runs each |
|---|---|
| base (the frozen runtime) | **hung past 8 s, no line**; ended by SIGPIPE when the pipe closed (status -13, so 141) |
| **the compiler-engineer's R** (`rt-R`) | **hung past 8 s, no line**, then SIGPIPE, 3 of 3 |
| the ffi-pragmatist's R2 (`rt/R2`, `ftrylockfile` before the flush) | **ended by itself in 0.23 to 0.47 s, status -6 (134), its line on stderr**, 3 of 3 |

So **R as the compiler-engineer built it does not keep R's promise on this shape**, and the synthesis
that adopts *"R"* adopts R2's flush rule with it, said in those words. This also places the R-against-E
contradiction: **E's hang is today's hang** (base hangs the same way, above, because a normal end runs
C's `exit`, which flushes every stream and waits on the lock the blocked writer holds), so E inherits
it and cannot leave it short of `_exit`, which skips the `atexit` handlers spec § 13 shows (the
ffi-pragmatist's E2, 15.00 s, carried from its log, not re-run). The blind readers' default (normal 5
of 5, finding 3) is E's ending class; the two sentences are both read as needed by the registered
rule, which does not choose. **What chooses is CLAUDE.md § Precedence**: robustness (rank 3, a program
that cannot end, and a gate that turns every balance off, Rebel's documented rule among them, the
ffi-pragmatist's 2.4) above the reader's default (a Part 11 effect, rank 4), and the synthesis writes
down that it went that way. Unrun by anyone: R2 on Linux with a blocked pipe (the ffi-pragmatist's
R2 ran its Linux rows on other shapes).

**The new defect, reproduced on the frozen tree** (the ffi-pragmatist's defect 1, *"a runtime panic
never says its line while another thread is blocked writing stdout"*): its `fp-panic-blocked.hero`
(copied from `251-ffi-pragmatist/q2/rt/w-base/`; a worker printing without end, then `main` indexing
past the end), built by my stage 2 over the frozen runtime with no overlay: **stdout a pipe nobody
reads, 3 of 3 hung past 8 s with stderr empty**, ended only by SIGPIPE when the pipe closed; **stdout to
`/dev/null`, 3 of 3 status 134 with `panic: array index out of range` and its place**. The cause is
`runtime/parts/panic.c:59`, `fflush(stdout)` before `fprintf(stderr, ...)` (read in my copy). **A defect
of the frozen tree, not of a prototype**: every program that panics while another thread is inside a
`print` to a stalled pipe (a reader like `less` not yet scrolled) hangs with its message unsaid. Not
found on GitHub by `gh issue list --state all --search` for *fflush stdout panic*, *panic blocked
stdout*, *panic never says*, *stdout lock panic*, *hero_panic fflush* (read-only). Its repair is the
same try-lock as R2's, one site; the class is the coordinator's to file (a told failure that never
tells reads `blocking` by `.claude/rules/verification.md` § Bounded discovery, the ffi-pragmatist's
reading, which I share).

### 7. Q3: the shallow crossing's safety was measured on one direction of the promise; the other direction and an answer still sharing the spawner's blocks now run, and hold. The 288 is reproduced (unrun claim closed by running)

**What the compiler-engineer's drivers check, read**: `q3/slice/slice.c:316-357` hands each of 32
threads an `Item` while `main` rewrites its own tags 1,000 times, then checks each **answer** (its
first tag must still read `tag i 0`, the value before `main`'s rewrites); `gridx.c` checks each answer's
`rows[0][0]` and its row count. **Neither reads `main`'s own value after the workers wrote into their
copies** (`mine[i]` is released unchecked in both), so *"mutating `b` never changes `a`"* was measured
from `main` to the worker and not from the worker to `main`, except as TSan's 0 reports; and the
seat itself names *"an answer that still shares a block with the spawner's value"* unrun. Two premises
are true by reading: the hazard `runtime/parts/cow.c:35-49` names (*"two threads can both read 1 ...
both write"*) needs two threads on **one** reference, and `runtime/parts/thread.c`'s guard is for
callbacks on threads C made; per-thread references, which a crossing by increment gives, keep the count
at 2 or more while two threads hold the block.

**Run** (`work2/q3/`, the compiler-engineer's `rt-Q3/`, `flags.txt`, `slice.c` and `gridx.c` copied;
`add_checks.py` makes `slice2.c` and `gridx2.c` by asserted edits; built as its `cost.sh` builds, `clang
@flags.txt -O2 [-DSLICE_SHALLOW] -I rt-Q3 ... rt-Q3/runtime.c`, TSan at `-O1 -g -fsanitize=thread`):
`mine right` checks `main`'s every tag (each its own last rewrite, length 8) and every cell of its grid
(the diagonal `main`'s last write, the rest untouched, 4 rows of 4) after every `wait`; the `IDENTITY`
build's worker returns its argument as it came, so under the shallow route the answer **shares
`main`'s blocks**, and after the `wait` `main` writes the answer's first tag and its own, then checks
each.

| driver | crossing | plain, 20 runs | TSan, 5 runs |
|---|---|---|---|
| `slice2` | deep | `right 32 of 32`, `mine right 32 of 32`, 20/20 | the same, 0 reports, 5/5 |
| `slice2` | **shallow** | **the same, 20/20** | **the same, 0 reports, 5/5** |
| `slice2 -DIDENTITY` (the answer shares `main`'s blocks) | deep | 32/32 and 32/32, 20/20 | 0 reports, 5/5 |
| `slice2 -DIDENTITY` | **shallow** | **32/32 and 32/32, 20/20** | **0 reports, 5/5** |
| `gridx2` (nested) | deep | 32/32 and 32/32, 20/20 | 0 reports, 5/5 |
| `gridx2` | **shallow** | **32/32 and 32/32, 20/20** | **0 reports, 5/5** |

So **on these shapes the shallow crossing keeps the promise both ways, and with a shared answer**, as
the deep copy does, with no `deep` member and no ABI move. Limits, said: no negative control was built
(a crossing broken on purpose to show the check fails); `gridx2`'s check of cell `(0,0)` is weak,
since `main`'s own last write lands there after any worker's; a map, a mailbox's send of a value the
sender keeps writing, and Linux are unrun. **This narrows the compiler-engineer's objection to its
first half**: the 288, not the copy.

**The 288, reproduced**: `selfhost/` of my copy with the probe's one change (`check/table.hero` +3, a
`thread_ty` case, `diff` against the frozen file shows nothing else), `heroes-next check
selfhost/main.hero`: exit 1, **288 errors, every one `error[non_exhaustive]`, in 109 files**
(`work2/q3/probe/ty.txt`). The seat's number stands. The spec allows generics on functions only
(`spec/heroes-spec.md:303-306`), so no library can declare a thread type carrying its answer's type, and
the only route without a new `Ty` case named so far is the compiler-engineer's own (an `i64` slot the
checker tags), **unbuilt**; a generic `wait<T>(t: i64)` checked against a descriptor at run time would
need none either and would move the mistake from the compiler to the runtime, a route nobody listed
and nobody should adopt without the thesis seat's reading.

### 8. What nobody asked, three of them: the close of a mailbox (two built mailboxes disagree with each other and with the text), a `(thread T)` in a list (measured on today's handle machinery), and `spawn_sized` (question, partly settled by command)

**(a) A receive waiting at the close, messages queued at the close, a send after it.** The proposal's
text (`251-briefwriter/arms2/spec-proposal-b.md:366`, read): `receive` takes the oldest, *"failing
`timeout`, or `stopped` once a stop is asked"*, and nothing else; § 13's life rules give the after-close
abort only in C's words (the spec-warden's finding, +8 vendored to repair it for a send). **The two
mailboxes the seats built answer every one of these questions, and differently**:

| | the proposal's text | the compiler-engineer's (`rt-Q3/parts/mailbox.c`) | the ffi-pragmatist's (`q3/mailbox/fp-mailbox.c`) |
|---|---|---|---|
| a send through a copy after the close | aborts (`00-shared.md` § The three proposals; arm silent, spec-warden) | **answers a status**, *"1 closed (the value stays the sender's)"* (`:45-56`) | **aborts**, *"a mailbox was sent after it was closed: a copy of it outlived its life"* (`:100`) |
| a receive already waiting when another thread closes | **unsaid** (`timeout` or `stopped` only) | answers **closed** (*"2 closed and empty"*, `:78`, `:89`) | answers **closed** (`MBOX_CLOSED`, `:128`) |
| messages still queued at the close | unsaid | dropped (`:106-111`) | dropped and counted (`:131-138`) |
| a queued message carrying a handle whose life moved into the send | unsaid | dropped with the message, unrun | *"would leave that handle never released"*, the seat's inference, unrun |

The historian's pages split the same way: Go panics on a send after the close and a receive after it
*"will return the zero value ... without blocking"*; Rust's send to a dropped half gives `Err`; Erlang's
send *"never fails"* and has no close. **So `receive` needs a third failure the text lacks (`closed`),
or a rule that a close waits for the queue to drain; a send after the close must be either an abort
(the text and one prototype) or a told failure (the other) and the sitting has not chosen; and a
handle inside a dropped message needs its release named**, since its life moved into the send by the
proposal's own words and a dropped life is what *"a handle nobody consumes ... aborts when `main`
returns"* then blames on the program. With several senders an abort on a send after the close is also
a race the sender cannot test for first (no operation asks whether a mailbox is closed in the text);
which shape a correct program then has (every sender waited for before the close) is unsaid. None of
this was put to the blind seat (task 4 is one thread). **Unrun**, all of it, as a reading; the
synthesis may not adopt the mailbox without these sentences or a decision that defers them by name.

**(b) A `(thread T)` held in a list and waited in a `for`.** The verdict session's condition (*"An
array of thread handles waited in a `for` loop is accepted and then aborts at exit"*,
`verdict/result.md:169`) and the brief's own unanswered question (*"Does a `[(thread T)]` waited for in a
`for` meet spec § 13's handing C a copy made before ... aborts?"*, `00-shared.md:698-699`) were answered
by no seat. No compiler has `(thread T)`, but the proposal makes it *"a handle (section 13) its wait
consumes"*, and that machinery exists. **Run on it** (my copy's `examples-rebel/`, the catalogue's
`path`, my stage 2, 5 runs each): four `Life` handles (`tag void`, `malloc` `acquires free`, `free`
`consumes`) pushed into a list and consumed in a `for` (`cr_life_for.hero`): `check` exit 0, **exit 0,
`freed 4`, 5/5**; consumed by index: the same; four Rebel `threads.Thread`s waited by index,
`threads.wait(@ts[j])`: **exit 0, `sum 12`, 5/5**; the same in a `for`, `threads.wait(@t)`: **refused at
`check`**, `error[not_mutable]: t is the loop variable` (Rebel's `wait` takes `@`); the `Life` list
consumed **twice**, two `for` loops (`cr_life_twice.hero`): `check` and `build` exit 0, then **exit 134,
3/3**, *"a C handle reached the argument `p` of `free` at an address a call marked `consumes` ...
ended ... a copy made before the ending call"*. So on today's machinery a list of handles each consumed
once **ends cleanly**, and the verdict session's failure appears only for a handle consumed twice, which
is accepted and aborts at run time with a true message (the shape Rebel's `threads` documents, *"one
waited for twice, itself or a copy, stops it at the second wait"*). That `wait(t)` would be
`consumes`-shaped and not `@`-shaped is the proposal's word (*"its wait consumes"*); under an `@` wait
the `for` is refused at compile time. **What the synthesis may say**: the list question is answered by
analogy on the existing machinery, measured; on the proposal itself it is unrun, since its slice was
never built.

**(c) `spawn_sized` that cannot fail.** The compiler-engineer and the spec-warden agree, each by
reading the same lines (`selfhost/main.hero:100-110` exits 2 with the system's reason;
`runtime/hero_os.h:469-472` answers a refusal with -1 and the system's number), that the proposal's
`spawn_sized` must give a `(thread T)?`; the spec-warden prices the words, +16 vendored. **Read again
in my copy, both hold.** Nobody said the same of plain `spawn`: the compiler-engineer's own
`spawn_value.c` panics *"the operating system refused to start a thread"*, and the proposal's text
gives `spawn` no failure, so under it a refused thread is an abort the spec does not name. A question,
unrun: whether `spawn` too owes a `?`, or the text a sentence that a refusal aborts.

### 9. Q3's crossing, deep as built and shallow alike, departs from design.md Part 7.13's isolated heaps, and the one sentence that rules on the shallow route was cited by no seat (question; the sitting's own proposal calls 7.13 "made a ruling")

The brief presents Q3 as *"design.md Part 7 item 13's 'copying at the boundaries' ... made a ruling"*
(`00-shared.md:196-198`). Part 7.13 in my copy (`docs/design.md:4104-4148`, read): *"isolated per-thread
heaps, copying at the boundaries"*; *"each thread allocates from and frees into a region no other thread
touches"*; *"the sender copies into the receiver's heap"*; *"At a thread boundary, the copy is
unconditional and real ... Expect two distinct code paths and do not try to unify them"*; and of
Erlang's refc binaries, *"a cross-heap refcount — which is an atomic counter, i.e. the one thing this
design gets to avoid. That is a trade to make consciously if the copies ever hurt, not to
rediscover."* Two consequences no seat drew:

- **The shallow crossing the compiler-engineer recommends first** (*"take the type forms when a
  program the closure needs asks for them ... the shallow crossing first"*), and which finding 7 measures
  holding today, **is the trade 7.13 names and declines in so many words**; adopting it is a change to
  Part 7.13, which CLAUDE.md § 4 sends to a panel, and is not *"7.13 made a ruling"*. The historian's
  *"departure"* (Erlang and Dart share immutables, Heroes would copy a `str` too) is 7.13's own argued
  choice, already in design.md at 4142-4148 (the historian's brief pointed at 4980-4991, the appendix).
- **The deep copy as built is not 7.13's copy either**: `rt-Q3/parts/spawn_value.c` copies the argument
  on the spawning thread and the answer on the worker, so the receiver frees blocks another thread
  allocated, *"twice per crossing, by design"* (the compiler-engineer's words); 7.13 copies *into the
  receiver's heap*. Today one `malloc` serves every thread, so nothing breaks, and the compiler-engineer's
  census shows today's invariant holding (*"every counted block was born and freed on one thread"*,
  1,307 thread ends). **Under either built route that invariant ends**, and the `runtime` suite's
  `runtime/alloc` check exists for it (*"isolated per-thread heaps need one place to change"*, its
  message). So the synthesis owes a sentence on which of 7.13's two consequences Q3 keeps, whichever
  spelling or route it adopts or defers.

**The `runtime` suite at the frozen commit, the compiler-engineer's premise 4, reproduced**: in my copy,
`./heroes-next run tests/harness/main.hero -- ./heroes-next runtime` (whole output into
`work2/prem/runtime-suite.txt`, 48 s), **`runtime: 8 passed, 2 failed, 4 floors outgrown`**, `FAIL
runtime/alloc` and `FAIL runtime/threads`, the same set of `file:line` as the compiler-engineer's
`q1/runtime-suite/base.txt` (`diff` of the two sorted sets, empty): allocation calls outside `alloc.c`
in `hero_keyring.c`, `hero_sys.c` and others, and objects kept between calls that are neither
`_Thread_local`, `_Atomic` nor in `SHARED_BY_DECISION`, in `hero_crypto.c`, `hero_term.c`,
`hero_odbc.c` and 10 more members. **A defect of the branch at the frozen commit, not of a copy or a
prototype**: the suite walks the runtime's files and calls no git (`grep` of `suite_runtime.hero`: its
only `git` is in comments), and the members it names are the branch's (`git ls-tree`: 16 `.c` files
under `runtime/platform/` at `3a5ee2691`, 2 at `origin/main`). It is the lane's gate's to answer
before M-rebel-library's round closes, and it bears on Q3, since it is the instrument that holds 7.13's
invariants on the runtime.

### 10. The premises the seats say their briefs got wrong: each re-run where a command settles it (all but two held as the seat said)

| seat's premise found false | re-run by me | the seat is |
|---|---|---|
| compiler-engineer 1: J's Linux definitions are ten names, not three | `251-compiler-engineer/routes/bjv/runtime/parts/sigjump.c:173-230` defines `setjmp`, `_setjmp`, `__sigsetjmp`, `getcontext`, `longjmp`, `_longjmp`, `siglongjmp`, `__longjmp_chk`, `setcontext`, `swapcontext` (read); the brief's own § The three proposals already listed all ten as J's, so the error was its § escape's *"three"* alone | right |
| compiler-engineer 2: `__builtin_longjmp` cannot be written on aarch64 | Apple clang 21 on this Mac, `-target arm64-apple-macos` and `aarch64-linux-gnu`: *"`__builtin_setjmp` is not supported for the current target"*, error; `x86_64-linux-gnu` compiles (`work2/prem/bl.c`); not clang 22 | right (on this Mac's clang) |
| compiler-engineer 4: the `runtime` suite is red at the frozen commit | finding 9, reproduced | right |
| compiler-engineer 5: a shallow crossing passed the same stress | finding 7: right, and holds in the direction it did not check | right, narrower than stated before finding 7 |
| compiler-engineer 6 and spec-warden 2: `spawn_sized` must answer a refusal | `selfhost/main.hero:100-110`, `runtime/hero_os.h:469-472` (read) | right |
| ffi-pragmatist 2: `kmiss` does not build on Darwin | my stage 2, `build kmiss.hero -O2`: exit 1, `error[ffi_header_refused]: ... inline asm clobber list contains reserved registers: X18`; `kmiss-darwin.h` differs by `"x18"` alone (`diff`) | right |
| ffi-pragmatist 3.4 and spec-warden 1: 26 of the 30 `extern close` members are a sitting's evidence | `git grep -n -E '^ +function close\(' 3a5ee2691 -- '*.hero'`: 26 under `docs/panel/178-reports/`, 3 `tests/golden/check`, 1 `tests/golden/run` | right |
| spec-warden 3: `hero_os.h:481-482`'s *"whose type does not move (panel 232's promise)"* is false before 1.0.0 | `runtime/hero_os.h:482` and panel 232's lines 15-16, *"before `1.0.0` no form enters to keep an older program compiling, and the promise starts at `v1.0.0`"* (read) | right |
| spec-warden 4: `hero_os.h:485-486`, *"A thread never joined drops its answer with the process"*, is false of the runtime | the comment read at 485-486; the brief's `unjoined_str` 20 of 20 exit 134 with the compiler-bug line, re-run by four seats | right |
| spec-warden 6: proposal M's arm leaves UFCS standing | `251-briefwriter/arms2/spec-proposal-m.md:294`: *"`x.f(y)` is sugar for `f(x, y)` (UFCS). There are no methods but a thread's and a mailbox's"* | right: M's +282 real is short of its own text |
| spec-warden 5: `--refresh` runs in a copy with no `.git` | not re-run: it is an API call this reading may not make | unrun by me |
| ffi-pragmatist 1, 3, 4 (libSystem's handle under ASan, no crash report, D-rebind displacing TSan) | not re-run: each is a property of 249's prototype or of this Mac's crash reporter, not of the frozen tree, and none moves a resolution | unrun by me |

Two of the premises above **are defects of the frozen tree in comments a program reads**:
`hero_os.h:482` and `:485-486` sit in the header § 13 lets a program bind, and the spec-warden's Q2
item asks the second be corrected *"in the same change"* as R. The synthesis should carry both.

### 11. Spelling B against M: the ffi-pragmatist's veto holds by command, it already describes one name today, and the seats' four positions leave one spelling nobody priced (contradiction, settled where checkable; a route)

The positions: the ffi-pragmatist **vetoes B** (C's `close(2)`, `send(2)`, `wait(2)` would have no spelling
a binding can write); the spec-warden **objects to both** with repairs (B's `close` 77 of its 124
collisions, the compiler's own 4 among them; M's arm leaving UFCS standing); the historian finds no
page that decides and names Go's third spelling, a built-in a declaration may shadow; the blind seat
reads 10 against 10. **Checked**:

- **The veto's premises hold.** The grammar gives a member one name for Heroes and for C
  (`spec/heroes-spec.md:486`, *"Member = "function" ident ..."*; the spec names no rename, `grep -n -i -E
  'rename|another name|symbol'` prints only line 377, on another subject); a built-in's name is refused
  on a member (the ffi-pragmatist's `len`, the spec-warden's `exit`); the resolver takes 41 names
  (`Builtin(name: ...)` in `selfhost/inventory.hero`, the set `resolve/builtin_names.hero:37` asks), and
  of these **one is a C function in this Mac's SDK today, `exit`** (`_stdlib.h`), while **B adds three,
  `wait` (`sys/wait.h`), `send` (`sys/socket.h`), `close` (`unistd.h`)** (`python3 -I` over 17 headers,
  `work2/prem/`). So the boundary failure the veto names is not new in kind (C's `exit` has had no
  binding by its name since `exit(code:)` became the language's), and B **quadruples** it, with three
  calls a binding of a socket or a file descriptor needs and no built-in of the language replacing.
- **The historian's third spelling contradicts a ruling it did not cite.** Go's pages say what the
  historian quotes (`go.dev/ref/spec`, fetched by `curl` into `work2/hist/`: *"The scope of a
  predeclared identifier is the universe block"*, *"An identifier declared in a block may be
  redeclared in an inner block"*, `close` among the predeclared functions). Heroes refuses that on
  purpose: *"A built-in's name is taken everywhere ... a local `ok` makes `ok(x)` on the next line mean
  the local, which is precisely the bug the shadow ban exists to kill"* (`selfhost/resolve/state.hero:345-350`,
  defect 422 named there). So the third spelling is not open without reversing that rule.
- **The spelling nobody priced**: B's operations named so that none collides, each read by the blind
  seat. The spec-warden offers `end_mailbox` for `close` (0 declarations, 0 members, its grep) and
  leaves `wait`, `send`, `receive` colliding, which keeps the ffi-pragmatist's veto standing on `send` and
  `wait`; the ffi-pragmatist's lift conditions (a member rename in the grammar, or built-in names free for
  members alone) are each a language change of their own, unpriced. M keeps every name free but owes
  UFCS a sentence (+282 real counted short of it) and the compiler a new name kind plus a new refusal
  class (the compiler-engineer's Q3.2). **None of this decides the sitting**: Q3 is unpaid and the
  spec-warden's veto condition is met (finding 4), so the spelling is a question the deferred decision
  carries, with these four positions and the fact that B's veto rests on three names, not thirty.

### 12. Route J and the close of defect 2522: on the seats' own residue lists, no platform meets "closes only on no residue"; two of the historian's Darwin questions answered on this Mac; the escape is ten names (synthesis condition, settled where checkable)

**Residues measured or named, per platform**, as the reports and finding 5 leave them: **Linux**
(validated J, the compiler-engineer's): a buffer refilled between its save and its jump with no J event
between (unrun, named), the inline pair on x86-64 refusing a correct program at `-O0` (run, toward
refusal), the `memcpy`'d jump out refused with R1's false words (run, 5 of 5 at `-O0`), and the x86-64
rows owed to the CI. **Darwin** (D-rebind+J, the ffi-pragmatist's, and validated, mine): the `memcpy`'d jump
out refused with false words (40 of 40, both), a save reached by `dlsym` or made inside a shared-cache
framework (52 images import one, its `cache-family-counts.txt`), an installer reached by `dlsym` or
inside the cache where only the walk of eight guards (249's, carried, **toward silence past eight
frames**), and the J-as-built stale record (finding 5, toward silence, closed by validation). **So
2522 closes on neither platform by the proposal's own condition**, and two residues are the
`blocking` class's *"a false message"* (the false R1 words; the ffi-pragmatist owes them new words, a
change that needs no spec token). The synthesis may adopt validated J and keep 2522 open with each
residue filed by its class; closing it needs either the residues gone or route K's told residue, which
the author answered *no, for now*.

**The historian's Darwin objection, two questions answered here.** It asks *"whether
`dyld_hw_tpro_pagers` is passed on this Mac"* (its dyld-1378 reading: `enableTproDataConst =
(appleParam("dyld_hw_tpro_pagers") != nullptr)`). A program printing its `apple[]` parameters' names
(`work2/tpro/apple.c`, values withheld), built here and run plain and signed `codesign -o runtime`:
**12 parameters, `dyld_hw_tpro_pagers` absent, both** (macOS 27.2, Darwin 27.2.0). And it asks for *"a
run on macOS 27.2 showing D-rebind's write landing"*: every D-rebind and D-rebind+J row of the
ffi-pragmatist's and of finding 5 is that run (the rewrite changes what the program does, 20 of 20 per
cell). **What stays owed**: which dyld this macOS ships (not read), `macos-26` (the CI's leg, after the
push), and arm64e third-party slots (the ffi-pragmatist's veto condition). The historian's objection
is answered for this Mac and stands for the CI's runner.

**The escape is ten names, not three.** The brief and the spec-warden's Q6 say J's Linux definitions
widen panel 239's `ffi_defined_elsewhere` by three names; the compiler-engineer's build defines ten
(finding 10). Its one run is a library defining `longjmp`, refused statically (`error[ffi_defined_elsewhere]`,
exit 1) and **not judged when shared**; the golden census the spec-warden owes for a widened refusal,
and a library that ships its own setjmp family on purpose, are **unrun**: owed before landing, by
`.claude/rules/verification.md`'s *a new checker rule is judged by every golden tree*.

### 13. Two of the historian's readings re-read, and one precedent it missed in a page it read (unrun claim checked; a prior-art correction)

Fetched by `curl` into `work2/hist/` (public pages, no credential, no API): `tsan_interceptors_posix.cpp`
at llvm-project `main`, **113,959 bytes**, the historian's own count; line 593 prints *"ThreadSanitizer:
can't find longjmp buf"*, line 571 *"Find the saved buf with matching sp"*, line 565 saves
`in_signal_handler` at a `setjmp` and 587 restores it at the jump: **as the historian read it**. Go's
specification (341,473 bytes): the predeclared `close`, its universe scope, the inner redeclaration,
and *"A send on a closed channel proceeds by causing a run-time panic"*: **as read**. **Missed**: the
historian's Q1 says Heroes' R1 refusal *"has no ancestor among the pages read"* and conditions an
approval on *"a language runtime that detects its own code running inside a C handler"*; the same
tool it read has one. `tsan_flags.inc` (6,264 bytes), lines 33-35: `TSAN_FLAG(bool,
report_signal_unsafe, true, "Report violations of async signal-safety (e.g. malloc() call from a
signal handler).")`, the check riding the `in_signal_handler` depth the historian quotes (lines
2374-2390 of the interceptors file count it around the program's handler). **Not a language's
runtime, and it reports rather than refuses, so the condition stays unmet as worded; but the sentence
"no ancestor" must not reach a `## Prior art` section as written**: the decision issue's line for J
can name TSan once, for the depth it saves at a `setjmp`, restores at a jump, and checks inside a
handler. The rest of the historian's report (C++, Rust, Java, Python, Erlang, Dart, Swift, JavaScript,
Pony, Ubuntu, fishhook, dyld) I did not re-read; each line the synthesis keeps is re-read by the
session that files it (`.claude/rules/records.md` § And the record says whose idea it was).

### 14. Defect 2582 reproduced at the frozen commit; the seats' other new defects classed as the tree's or a prototype's (settled by running)

`t2-ref.hero` (the brief-writer's `repro/`, copied; a worker printing `sum 15` while `main` prints
`main done`, then a join), built by my stage 2, **200 runs at load 1.5: 165 `main done/sum 15/`, 14
`sum 15/main done/`, and 21 torn** (19 `main donesum 15//`, one `sum main done/15/`, one `sum 15main
done//`), every run exit 0 (`work2/p2582/`). The brief-writer's 0 of 100 at load 5.89 was the outlier;
the compiler-engineer's 18 of 200 and sitting 252's critic's 27 of 200 hold their order. **The tree's
defect**, open as `adjacent`; spec § 11's *"exactly one trailing newline"*, and the spec-warden's
reading that the runtime has the bug (CLAUDE.md § 12), make a torn line *"a wrong value"* by
`.claude/rules/verification.md` § Bounded discovery, so `blocking` is the class the reading supports;
the coordinator's to set. Its repair is measured (the compiler-engineer's P, 0 of 200, about 209
instructions a `print`), runtime only.

The other new defects the seats name, by whose they are:

| defect | the tree's or a prototype's | reproduced by me |
|---|---|---|
| `hero_panic` hangs, its line unsaid, behind a writer blocked on stdout (ffi-pragmatist) | **the tree's**, `panic.c:59` | yes, finding 6, 3 of 3 |
| the `runtime` suite red on the platform layer (compiler-engineer) | **the tree's** (the branch's) | yes, finding 9 |
| `hero_os.h:482` and `:485-486` false (spec-warden) | **the tree's**, comments in a header programs bind | read, finding 10 |
| J as worded misses the stale record (compiler-engineer) | J's, a prototype; **on both platforms** | yes on Darwin, finding 5 |
| J's miss tells a correct program R1's false words (ffi-pragmatist) | J's, a prototype; validated J keeps it | yes, `miss.c` HOW 2, 40 of 40 under validated J |
| D-rebind as 249 built it kills `--sanitize` programs, displaces TSan's handler (ffi-pragmatist) | 249's prototype, not in the tree | not re-run |
| two 2494 goldens' words move on Darwin under J, five under B' on Linux | the routes', owed by the lane that lands them | not re-run |

### 15. Smaller points, each with its command

- **The folder's path** (the coordinator's second leak) appears in four reports (`t1-x-5`, `t2-f-4`,
  `t4-f-3`, `t4-f-4`), each time inside its `context` section and nowhere else (`awk` printing the
  section of every line naming `heroes-lang`): no reading uses it. It voids nothing.
- **The counted prices of Q3 are short of the text Q3 needs**, every gap named by a seat or above:
  `spawn_sized` fallible (+16 vendored, spec-warden), the after-close abort (+8 vendored, spec-warden),
  M's UFCS sentence (unpriced), `receive`'s third answer and the queued messages (finding 8, unpriced),
  a plain `spawn`'s refusal (finding 8, unpriced), and the ffi-pragmatist's mark for a handle that may
  not cross (*"a change a binding's author must learn"*, unpriced, a new surface). So +260 and +292 real
  are lower bounds of the proposal as it would land; nothing near the ceiling (16,384), but the
  payment owed grows with them.
- **I ran `claude --version` and `claude --help`** (no session, no API call), to read the flags in
  finding 2; whether the CLI writes anything outside the root when asked for its help is unread.

## Not run by me, and why

The emulated x86-64 rows (the compiler-engineer's own list, owed to the CI's leg); Linux arm64 for any
finding above (one Docker run was not needed to settle a finding); the validated Darwin J's `flib`,
storms, goldens through the `run` form, Guard Malloc, census libraries, sanitizers and forked children
(finding 5's list); R2 with a blocked pipe on Linux; a negative control for the shallow-crossing checks;
the `--refresh` counts (an API call); any blind session (a paid run); the historian's pages beyond the
three re-read; Windows, which no seat ran either.

## Summary: what changes what the synthesis may adopt, then the rest

**Findings that change what the synthesis may adopt**, most binding first:

1. **Finding 5, Q1**: the compiler-engineer's stale record runs the Heroes function inside the signal
   on Darwin under the ffi-pragmatist's D-rebind+J, **40 of 40**, with a control showing J active. J as
   worded is vetoed on both platforms; **the validated record carries to Darwin** (stale refused 40 of
   40, every must-refuse case refused, the twelve depth programs 72 of 72 run). Adopt J only validated,
   with finding 5's Darwin rows owed. Commands: `work2/q1/stale/`, `work2/q1/ctl2/`,
   `work2/q1/make_jv.py`, `work2/q1/jv-matrix.txt`.
2. **Finding 6, Q2**: *"approve R"* is two runtimes; the compiler-engineer's R **hangs 3 of 3** with no
   line behind a writer blocked on stdout, as base does, and only the ffi-pragmatist's **R2** ends (134,
   0.23 to 0.47 s, its line). Adopt R2's flush rule with R. E inherits the same hang; CLAUDE.md §
   Precedence (robustness over the reader's default) chooses, and the synthesis says so. The
   `hero_panic` hang **reproduces on the frozen tree**, 3 of 3, `panic.c:59`: a defect to file.
   Command: `work2/q2/pipe_run.py`.
3. **Finding 4, the W-predictions**: scored *"for information"* is the brief's letter, but its condition
   was unmeetable (the seat launched 10:08:11, `run.sh` 10:08:24) and the predictions were blind by the
   seat's transcript. Consequences the coordinator's table does not draw: **R's sentence has no payment
   unless W1 is admitted** (or it waits, R2's runtime half landing alone), and **Q3 is unpaid and the
   spec-warden's veto condition (W4 failing for both spellings) is met either way**, so Q3 cannot be
   adopted in this sitting.
4. **Finding 2, the blind seat's context**: all 51 sessions held the main checkout's **git status with
   five commit subjects**, rendered into their context (transcripts under `~/.claude/projects/`, read
   only), and **all 51 answered `context: no`**; the `order.txt` leak's cause is **settled** as the
   inherited standard input. No reading uses either leak, so I would void none; but the synthesis may
   not rest *"no session is voided"* on the answers, and sitting 252's runner owes `< /dev/null` and a
   way to keep the git status out (none found; a question for the author).
5. **Finding 9, Q3 against design.md 7.13**: the shallow crossing is the *"cross-heap refcount"* Part
   7.13 declines in so many words, and the deep copy as built frees blocks across threads, which 7.13's
   heaps forbid; adopting either is a change to 7.13, not 7.13 *"made a ruling"*. The `runtime` suite is
   **red at the frozen commit** on the branch's platform layer (reproduced, 8 passed, 2 failed).
6. **Finding 8, what nobody asked**: the two built mailboxes disagree on a send after the close (an
   abort against a told status) and both give `receive` a third answer, closed, the text lacks; queued
   messages and a handle inside one are unsaid; plain `spawn`'s refusal is unsaid. A `(thread T)` in a
   list, measured on today's handle machinery: each consumed once in a `for` **ends cleanly 5 of 5**;
   consumed twice, accepted and **aborts with a true message 3 of 3**.
7. **Finding 12, 2522**: on the seats' own residue lists **2522 closes on neither platform**; two of the
   residues are false messages (`blocking`). `dyld_hw_tpro_pagers` is **absent** on this Mac, plain and
   hardened; `macos-26` owed. The escape widens `ffi_defined_elsewhere` by **ten** names, its golden
   census owed.
8. **Finding 7, Q3's copy**: the shallow crossing **holds in the direction the seat did not check** and
   with an answer that shares `main`'s blocks (20 of 20 plain, 5 of 5 TSan, 0 reports); the **288 in 109
   files reproduces**. The compiler-engineer's objection rests on the type form, not on the copy.
9. **Finding 11, B against M**: the ffi-pragmatist's veto holds; today one built-in (`exit`) already
   takes a C function's name and B would take three more (`wait`, `send`, `close`); Go's shadowable
   spelling contradicts Heroes' shadow ban (`resolve/state.hero:345-350`). A deferred question, since
   Q3 is not adoptable now.
10. **Finding 13, prior art**: TSan's `report_signal_unsafe` (*"Report violations of async
    signal-safety (e.g. malloc() call from a signal handler)"*) is in the tool the historian read; the
    sentence *"R1 has no ancestor"* must not reach a `## Prior art` section as written.

**The rest** (4): finding 1 (`5c5e6f811` moved nothing a seat measured; the freeze's letter not kept),
finding 3 (the blind grading holds cell by cell; the five `now` programs are **silently wrong 5 of 5**
on an input holding their separator, a measured basis for a reading nobody registered), finding 10
(the seats' false premises: every one re-run holds, two left unrun by me), finding 14 (defect 2582
**reproduces, 21 of 200 torn**; the seats' new defects classed the tree's or a prototype's).

Finished 2026-11-19 at 13:40 CET (`date` read 13:39:56), begun at 13:09. Nothing of mine runs (`pgrep -fl 251-critic`
empty, no container); every file I wrote is under `.claude/worktrees/rebel-scratch/251-critic/work2/`
but this report.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/251-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
