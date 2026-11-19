# Panel 251, the completeness critic's first reading: the briefs against the world

Read 2026-11-19 from 09:34 CET, before any seat started. Every command ran in the critic's own copy,
`.claude/worktrees/rebel-scratch/251-critic/tree/` (a `git archive` of
`3a5ee269117a31780ef1f96d0305452630ac4d83`, 20,022 files by `find -type f | wc -l` and by the archive's
`tar -t`), or against the git objects of the worktree `.claude/worktrees/rebel-round`, never its working
tree. Seed built there: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0, `sources
5aae00294f7cd6fa`. No paid run, no `claude -p`, no `heroes test`, no credential printed, no sub-agent.
Temporary files under `.claude/worktrees/rebel-scratch/251-critic/`.

Kinds: **wrong** (a fact a brief hands a seat that the command contradicts), **unrun** (a claim with no
command behind it, or a negative or an *only* not searched), **route** (an option nobody listed),
**rule** (a rule of `SKILL.md` § 3, 3b, 3c or CLAUDE.md a brief does not hand its seat).

Written as it goes; the findings are numbered in the order they were found.

## Findings

### What held, so no seat re-pays for it

Each by its command at the frozen commit, in the critic's copy or `git -C .claude/worktrees/rebel-round`:
the commit line, `origin/main` `0becec5b6`, the merge base `f6c49d84c` and the three commits
`HEAD..origin/main`; `cc83b4d91..HEAD` 68 commits, `runtime/` 38 files 39,270 and 71, 30 added (the
list matches), `selfhost/` 7 files 289 and 14; spec and seed unmoved; the spec 492 lines, 29,407 bytes,
`1d532d51...`; all 26 "unchanged" files unchanged (`git diff --quiet`, exit 0 each) at the line counts
given; `alloc.c` 1,548 to 1,585 with one hunk `@@ -618,0 +619,37 @@` and every cited line; `stack.c`,
`os.c`, `runtime.c`, `hero_net.c` line counts and cited lines; the install and mask counts by 249's two
commands, 27 to 33 and 18 to 27; the `atexit` line, no `flockfile`; the setjmp grep's six lines; 811
`run` cases, 5 and 14 plus the folder case; `ev249` 3,232 files, 28M, the ten hashes, 9 + 3 depth
programs; the overlays' patch logs (one hunk each, 159, 158 for A), empty syntax files, no `.orig`;
every issue state in the briefs (`gh issue view`), both repair commits ancestors of the frozen commit;
the CI run, job and its three `FAIL` lines (the log re-fetched, `cmp`-equal to `bw/ci/`); every spec,
design.md, CLAUDE.md:136 and panel-file line cited in `00-shared.md` that I opened (249 R3 and the
ratification, 248:306-309, 246 historian 422, 246 reader 1766 and 2110, 248 reader 834, 111:182-188,
232:272-274); `hero_os.h`'s doors, `spawn.c`'s states and calls, `spawn_str.c:29-37`; the 38 door files
by the brief's grep (`cmp`-equal to `bw/spawners.txt`) and the 22 Rebel callers. **Stage 2 in my copy
printed `sources 23c7d2a626b29fd5`**, as the brief says, at `real 237.88`, `user 170.85`, `sys 45.65`
(load 3.6 to 6.6, not the 151 s the briefs quote; a seat budgeting its clock should read 151 to 240 s).

### 1. Three of "the six new installs" install nothing, and two run in a forked child (wrong fact, and an unrun safety question; changes what Q1's seats run)

`00-shared.md:102-104`, `:243-244`; `compiler-engineer.md:54-55`; `ffi-pragmatist.md:59-62`.
`git grep -n -E '(^|[^_a-z])(sigaction|signal)\(' 3a5ee2691 -- runtime/platform/hero_proc.c runtime/platform/hero_term.c`
and the code around each: `hero_proc.c:1628`, `hero_term.c:247` and `:272` are **reads**
(`sigaction(sig, NULL, &now)`); only `:1630`, `:258` and `:278` install. And `hero_proc.c:1628-1630`
are inside `hero_proc_child` (from `:1603`), **between `fork()` (`:1671`) and `execve` (`:1633`), in a
loop over every signal from 1 to `NSIG`**, a read and possibly an install per signal: the same
async-signal-safety question the briefs ask of `run.c:1805` alone, at up to twice `NSIG - 1` calls. That
child is reached only where `hero_proc_spawn_enters()` (`:1438-1449`) answers false and a folder is
named (`:1777-1779`): Darwin before 10.15, or a C library that is neither Darwin's nor glibc 2.29 or
later (musl, a BSD). So the two Docker images and this Mac do not take it.
**Repair**: say "three installs and three reads"; name `hero_proc.c`'s forked child beside `run.c:1805`
as a second forked-child path under any route that defines or rewrites `sigaction`; ask the building
seats to say whether J's `sigaction` (B' defines it, D-rebind rewrites its slot) is safe there, and to
reach it in code (a build with `hero_proc_spawn_enters` forced false, a second thread holding a lock the
route takes at the fork) since no image here takes that path by itself.

### 2. The five proposed built-ins are names the language then takes everywhere: 46 declarations in 39 files would be refused (wrong framing by omission; a route nobody listed; changes what Q3's seats price and what the blind task grades)

`00-shared.md:181-201` (Q3), `compiler-engineer.md:118-142`, `spec-warden.md:78-88`,
`llm-ergonomist.md:130-131`. A built-in's name is refused as any other declaration's: in my copy,
`function print(s: str)` and `function exit(code: i64)` each print `error[builtin_name_taken]: ... is a
built-in of the language, so the name is taken everywhere — pick another name`, **an `extern` member
`function exit(status: i32)` inside `extern "stdlib.h"` likewise**, and a local `len = 3` likewise
(`heroes-next check`, `work/b/t4.hero` to `t6.hero`, exit 1 each). Today `thread`, `mailbox`, `spawn`,
`wait`, `send` and `receive` are plain names (`work/b/t2.hero` binds all six, exit 0).
`grep -rn -E '^function (spawn|wait|send|receive|mailbox)\(' --include='*.hero'` over the copy, the
vendored `heroes.packages/` copies left out: **46 declarations in 39 files** (`send` and `receive` most;
with the vendored copies, 103), among them `packages/core/0.6.0/files/net/hero_net.hero:74` `send` and
`net/http/internal/client.hero:20`, `:53`, Rebel's own `threads.hero:56` `spawn` and `:87` `wait`,
`process.hero:623` `wait`, `poll.hero:332` `wait`, and `send`/`receive` in `redis`, `grpc`, `http`,
`http2`, `kafka`, `ldap`, `mail/smtp`, `mqtt`, `nats`, `sql/postgres`, `sql/mysql`, `s3`, `websocket`.
A rough grep of bindings and parameters with those names outside comments
(`(^|[ (,])(spawn|wait|send|receive|mailbox)(: [^ ]| = | @= )`) prints 117 more lines, unread one by one.
And C's own `send(2)` and `wait(2)` could no longer be bound under their names. The spec states the rule
nowhere I found (`grep -n -i -E 'reserved|taken|shadow'` over it prints only line 167's `use` rule).
**Repair**: hand Q3's seats the count and the diagnostic, so the compiler-engineer prices the renames
(CLAUDE.md § 2 rules them allowed before `1.0.0`, but they are a cost), the spec-warden judges a rule
the spec does not state, and the historian is asked nothing it cannot read. **The route nobody listed**:
spell the five so they take no name a program uses: as methods on the two new types (`t.wait()`,
`m.send(v)`, `m.receive(within_ms:)`, the shape `xs.slice(from:, to:)` and `.lease()` already have), or
as words that are not common names. **The blind task's grading** (`llm-ergonomist.md:131`) should record
whether spec-B's answer declares a `send`, `receive` or `wait` of its own, since spec-B builds nothing
and a reader of spec-B is never told the name is taken.

### 3. Question (b): `(thread T)` and `(mailbox T)` do not parse today, and would parse with no new keyword (answered by reading and running the frozen parser)

`00-shared.md:544-545` (posed as a question, rightly). The frozen parser's type entry
(`selfhost/parse/type.hero:73-101`, `prefix`) sends a `(` to `unit_or_function` (`:193-213`), which
takes `)` or the keyword `function` and nothing else; stage 2 on `function take(t: (thread i64))` prints
``error[expected_function_type]: expected `function` — in a type, `(` opens either `()` or a
function type ..., found a name (`thread`)``, and on `x: [(mailbox str)] = []` the same at `mailbox`
(`work/b/t1.hero`, `t3.hero`, exit 1). **So the place is free**: after a type's `(` the grammar today
admits only `)` and `function`, so a name there is unambiguous with one token of lookahead, and the
parser can read `thread` and `mailbox` as contextual words in that one position, the shape it already
uses for `tag`, `partial`, `lent`, `consumes`, `borrows` (`selfhost/parse/marks.hero:32`, *"A contextual
word and never a keyword"*; `records.hero:59`, `:84`). Owed, unrun: the `grammar` suite reads the
contextual words in `selfhost/parse/**` and compares `keyword` arms with `heroes grammar`
(`.claude/rules/verification.md`'s map), so the two words must be registered where it reads them; the
formatter and `heroes grammar` must re-print them; and a user's type named `thread` (lowercase type names
parse: `record thread` raised nothing before the later error in `work/b/t3.hero`) stays legal, since
`(thread` and `thread` differ at their first token. **Repair**: the compiler-engineer's Q3 slice builds
the arm in `unit_or_function` and says what `grammar`, `surface` and the formatter's probe owe.

### 4. The blind seat's folder is inside the repository, under its `CLAUDE.md` and its git tree, and the command has none of the guards that case needs (wrong fact; missing rule; changes what the blind seat measures)

`llm-ergonomist.md:21-26`, `:45-58`, `:89-96`; `00-shared.md:17-18`, `:528-530`. The brief says the folder
is *"the session scratchpad of the coordinator's session, outside the repository and outside any git
tree (`git rev-parse --show-toplevel` there: "fatal: not a git repository"), with no `CLAUDE.md` ... in
any folder above it"*, and that its path holds the scratchpad's name `-Users-joseph-Temp-heroes-heroes-lang`.
The path it names is `.claude/worktrees/rebel-scratch/251-llm-ergonomist/`, and from it:
`git rev-parse --show-toplevel` prints `/Users/joseph/Temp/heroes/heroes-lang`; `git check-ignore -v .`
prints `.gitignore:34:.claude/worktrees/`; the walk of its ancestors (`ls -a` of each, as the brief did)
prints `.claude`, `.git` and `CLAUDE.md` in `/Users/joseph/Temp/heroes/heroes-lang` and `.claude` in
`/Users/joseph`. Its five files belong to group `wheel` (`ls -la`), which suggests they were made under
`/private/tmp` and moved in; that is an inference, not a measurement. The path names `heroes-lang`,
`rebel-scratch`, `251` and `llm-ergonomist`. Git from the folder sees the main checkout on `main` at
`0becec5b6`, whose subject begins *"Batch 39 round closed: a trap a library recovers goes on"*, which is
Q1's area (`git log --oneline -5` there). The command (`:89-96`) has none of panel 252's three guards for
exactly this case (`p252/docs/panel/252-briefs/llm-ergonomist.md:108-125`: `env -i`,
`GIT_CEILING_DIRECTORIES="$(cd .. && pwd)"`, `--bare`), and names no `--model`. `claude --help`
(2.1.311, `work/claude-help.txt`): `--bare` skips *"auto-memory ... and CLAUDE.md auto-discovery"*, and
Anthropic auth is then *"strictly ANTHROPIC_API_KEY"*; `--safe-mode` lists CLAUDE.md, skills, plugins,
hooks and MCP servers, and **auto-memory is not among the words it lists**. With
`GIT_CEILING_DIRECTORIES=<the folder's parent>`, `git rev-parse --show-toplevel` prints *"fatal: not a git
repository"* (run). **Unrun, and a question, not a premise**: whether a session started in this folder
with the brief's flags is handed the repository's git status or the project's memory index (the memory
this session was handed is keyed to the repository's root, and this session's own working directory is
a worktree under that root). The conflict the brief records for the author (CLAUDE.md's hard stop
against `SKILL.md`'s *outside the repository*) is not the one that now stands: the folder obeys the hard
stop and breaks the skill's premise, which is the case sittings 246, 247, 250 and 252 met with the
guards. **Repair**: correct the folder paragraph and `00-shared.md:528-530`; run the session as 252's
command does, `env -i PATH HOME LANG GIT_CEILING_DIRECTORIES ANTHROPIC_API_KEY`, `--bare`,
`--model claude-opus-5-5`, the key loaded from `.env` with its length alone printed. A `context: yes`
voids the reading, and here it would void the whole 10 USD.

### 5. Question (a): one session holding both arms measures less than one session per arm and per task, and costs a fraction of the bound (missing route; changes what the blind seat measures)

`llm-ergonomist.md:85-117`, `:134-146`. **Measured**: panel 250's evidence
(`docs/panel/250-reports/evidence.tar.xz` at the frozen commit, unpacked into `work/ev250/`, every
`run.json` read by `python3 -I`): **24 sessions, each one specification and one task, 0.120 to 0.143
USD, sum 3.139598, median 0.1308**, every `modelUsage` key `claude-opus-5-5`, 4 or 5 turns, 14,096 to
15,345 cache-write and 2,409 to 3,586 output tokens. Opus 5.5's price, from the `claude-api` reference
bundled with this Claude Code (model table, *cached 2026-11-03*): $4 input, $20 output per million; panel
250's `t1-d-1` implies $5.00 per million written to the cache, at $0.20 read. **What one session holding
both arms does to the readings**: (i) it reads `changes.diff`, whose last hunk adds *"or a thread nobody
waits for, aborts when `main` returns"*, the very answer Q2's rule reads from the reader of spec-A
(`:136-139`, *"task 1 under A's ending class"*), so that cell no longer measures what a reader of today's
spec assumes; (ii) task 3's control (`:143-144`) is two answers written by one reader one after the
other, so their agreement measures copying, not noise; (iii) one answer per cell, which the brief itself
says *"decides nothing"* (`:139`), with about 9.5 of the 10 USD left unspent. **By arithmetic, an
inference**: one session of both specifications, the diff and three tasks twice is roughly twice panel
250's cache write and six times its output, about 0.14 + 0.34 + reads, **about 0.5 USD**; six cells (three
tasks under two specifications) at panel 250's median cost about 0.78 USD per replicate, so **three
replicates about 2.4 USD, five about 3.9**. **Recommended, resting on those numbers**: panel 252's shape,
one folder per task and per specification (the spec as `heroes-spec.md`, no diff, so no label and no
direction to read), each task file holding the program for its specification alone, three to five
replicates, a cap of 0.40 per session and 252's stop rule at 9.50 (`p252/.../llm-ergonomist.md:130-139`),
the guards of finding 4; **plus one session holding both specifications and the diff for the `verdict`
field alone**, its answers recorded and never graded as a measurement. The decision rules then need
counts, as 252's had (*"at least 3 more right answers"*), registered before the run.

### 6. The spec-warden is told not to refresh, where panel 249's ruling puts its refreshes inside the same bound and they cost nothing by Anthropic's page (missing rule; changes what the spec-warden measures)

`spec-warden.md:56-61`; `00-shared.md:19-23`. Panel 249's ratification (`docs/panel/249-*.md:557-558`):
*"the blind seat of question 3 bounded at **10 USD** for all its paid sessions together, the
spec-warden's `--refresh` calls asked with it"*; its R3 (`:280-282`): the refresh calls *"are named with
their count in its brief and asked with the blind seat"*. `selfhost/cli/refresh.hero:37` names the
refresh's one address, `https://api.anthropic.com/v1/messages/count_tokens`. Panel 252's brief counted
each refresh at 0 USD on Anthropic's page, *"Token counting is free to use but subject to requests per
minute rate limits"* (`p252/docs/panel/252-briefs/00-shared.md:333-339`, carried), and the `claude-api`
reference bundled with Claude Code 2.1.326 calls the endpoint *"free"*
(`shared/preserved-thinking-migration.md:157`, read today). So every count the spec-warden is handed
stays vendored, a lower bound by about a third (`spec-warden.md:56-57`), and its verdict stays
`provisional` for want of free calls. **Repair**: name the refreshes in the brief with their count (the
six arms of `spec-warden.md:47-54`, each applied to the seat's copy, refreshed and reverted, at most eight
as in 252), with the key from `.env` and its length alone printed, each refresh's time and count logged
in the report; or say in the brief why the sitting declines them.

### 7. Q3's `(mailbox T)` is not Part 7.13's mailbox, and spec-B contradicts its own § 3 and its `@` rule (wrong framing; the question the sitting should ask and does not)

`00-shared.md:181-183`, `:191-195`; `bw/arms/proposal.diff`; the blind seat's `spec-B.md`. The brief calls
Q3 *"design.md Part 7 item 13's shape made a ruling"*, quoting *"The transport is a mailbox"*. Part 7.13
says *"one queue per thread; the sender copies into the receiver's heap and appends to the tail, the
receiver pops from the head"* (`docs/design.md:4119-4123`): a queue a thread owns. The proposal's
`(mailbox T)` is a free-standing value, *"its copies are one queue"* (spec-B line 96). spec-B keeps § 3's
*"Every value behaves as an independent copy: after `b = a`, mutating `b` never changes `a`. No aliasing
exists among the values this language owns."* (spec-B lines 99-100, untouched by `changes.diff`), and § 9's
*"Mutable parameters are marked `@` in the signature and at the call site"* (spec-A line 294), while
`send(m, v)` and `receive(m, within_ms:)` change through a plain argument a queue every copy of `m`
shares. Read in `spec-B.md` with `grep -n`, and in `design.md` with `sed`. **The question nobody is
asked**: is a mailbox the language's first owned value with reference semantics, and if so which
sentence of § 3 and § 9 changes, at what price; or does it take 7.13's shape. **Routes nobody listed**:
7.13's own (a thread's queue reached through its `(thread T)`, received inside the body, no aliasing
value); a mailbox that is a handle and moves; `send(@m, v)` with the `@` rule kept. **Repair**: state the
departure from 7.13 in the proposal; ask the spec-warden to judge the two contradictions, the
compiler-engineer to price 7.13's shape beside the proposal's, and the historian how Erlang addresses a
process's mailbox (a pid, copyable) against Go's channels (shared by reference), as questions; give the
blind seat a task that predicts what `m2 = m` then `send(m2, 1)` does to `m`.

### 8. *"The `hero_os.h` thread doors leave the program's surface"* has no text, no refusal and no answer for the compiler's own sized thread (unrun claim; Principle 0)

`00-shared.md:185-187`; `changes.diff` (line 377's hunk only stops naming them). spec § 13 lets a program
bind what a header declares, so under spec-B a program can still bind `hero_thread_spawn`: is that
refused (a new refusal, a diagnostic class this sitting would rule) or a door left unnamed? And the
compiler needs the sized door: `selfhost/main.hero:87` binds `hero_thread_spawn_sized`, `:96-97` sets
`STACK_BYTES` to 268,435,456, `:101` runs every command on it, `:112` exits with its join (`grep -n`,
`sed`); spec-B's `spawn(f, x)` takes no stack size. **Repair**: the proposal says, as a spec diff, either a
`spawn` that takes a size or the sized door kept and named, and whether the other doors are refused; the
compiler-engineer's Q3 item 2 already asks what `main.hero:101` needs, and the spec-warden should be asked
the same under CLAUDE.md § 2's closure-list clause.

### 9. Q2's conservative has no words (missing rule: a proposal touching the spec owes its alternative as a diff)

`00-shared.md:177-179`: E is *"no spec sentence or one saying so"*; `spec-warden.md:47-54` prices no E arm;
the blind seat grades no E reading. Panel 252 had E's words and their count, pE, *"When `main` returns,
the program ends, and a thread still running ends with it; the program says how many it ended."*, +27
vendored (`p252/.../llm-ergonomist.md:27`, carried). **Repair**: write E as a diff (252's pE, or "no
sentence" chosen and said), measure it with the other arms, and say whether the blind seat reads it.

### 10. Smaller facts to correct before the seats lean on them

- `00-shared.md:473` cites `.github/workflows/ci.yml:368`, `:373`, `:1434` for the Darwin leg; `:373` is the
  Linux legs' line (`sed -n 373p`: `ubuntu-26.04`, `ubuntu-26.04-arm`).
- `ffi-pragmatist.md:33-36`: the overlay `bw/routes/drebind/runtime/parts/rebind.c` (85 lines) is the 249
  source (86) **without its line 10, `#define _DARWIN_C_SOURCE`** (`diff` prints `9a10`), so every line the
  brief cites in the 249 source (23-28, 40-45, 49, 68-70, 76) stands one lower in the overlay (`grep -n`:
  22, 39, 48, 67, 75). Say which file J starts from.
- `00-shared.md:467-468`: what held the machine awake at 09:08 and still does (`pmset -g assertions`) is
  powerd's *"Prevent sleep while display is on"* and coreaudiod's audio output, both of which end with the
  display or the sound; neither is a hold for an unattended run of 1,800 to 3,000 s. `caffeinate -i -w
  <pid>` costs nothing (`.claude/rules/verification.md` § A long run holds the machine awake).
- The stage 2 time: 237.88 s here at load 3.6 to 6.6 against the brief's 151 s (above); the bound of
  1,800 s stands.
- The sitting reuses the number 251: the 2026-11-15 sitting's own `docs/panel/251-briefs/` and
  `251-reports/` stand uncommitted in the worktree `p251` (`ls`), so a later commit of either tree under
  that name would meet this one's. Worth a line in the synthesis.

### 11. The blind seat's grading cannot see two things it should (missing rule in a brief)

`llm-ergonomist.md:126-132`. Task 1 under spec-A grades the line `main done, worker 0 not joined`; the
`0` is the handle's value, which spec-A does not give (`grep -n -i handle` over it: no value named), so
grade any integer there, or a right ending class is lost to an unreadable number. Task 2 under spec-B is
*"unbuilt (no compiler has `spawn`)"*, while the compiler-engineer is asked to build exactly the slice it
uses (`compiler-engineer.md:132-136`, `spawn` and `wait` of a record holding a `str` and a `[str]`): grade
spec-B's answer on that build where it stays inside the slice. And record whether spec-B's answer
declares its own `send`, `receive`, `wait` or `spawn` (finding 2).

### 12. A question the sitting should ask and does not: defect 2582 under Q3

Defect 2582 (two threads' `print`s tear a line) reads `OPEN`, `class:adjacent` (`gh issue view 2582`).
Q3 makes threads the language's own, so a worker that prints becomes the common shape, and the blind
tasks are built so that only `main` prints after every join (`llm-ergonomist.md:74-75`), so no instrument
of this sitting can see it. Whether Q3 owes a sentence or a runtime lock that keeps a `print` whole is a
question for the compiler-engineer and the spec-warden, with sitting 252's critic's 27 of 200 torn
(carried) and this morning's 0 of 100 and 1 of 30 (`00-shared.md:296-297`) as its record.

## What each brief hands its seat of `SKILL.md` § 3 and 3c

Read in each brief, line by line:

| seat | where to copy and build | report written as it goes | no credential | no paid run |
|---|---|---|---|---|
| compiler-engineer | yes, `:11-14` and `00-shared.md:416-430` | yes, `:14-17` | yes, `:17-18` | yes, `:17` |
| ffi-pragmatist | yes, `:10-12` | yes, `:13-16` | yes, `:17` | yes, `:16-17` |
| spec-warden | yes, `:9-10` | yes, `:10-14` | yes, `:15` | yes, `:15`, but see finding 6 |
| historian | builds nothing (no Bash by its definition) | yes, `:5-7` | yes, `:9` | yes, `:9` |
| llm-ergonomist | the coordinator runs it; its folder is finding 4 | the coordinator copies `report.md` (`:114-117`) | the key's route is not said (finding 4) | one session, 10 USD (`:99-101`; finding 5) |

`00-shared.md:436-468` carries the common rules (own folder, no file outside the root, Go and `lldb`
inside the folder, `timeout -s KILL` with output bounded, Docker containers named and stopped, costs in
instructions, the adopted route built before the synthesis, every negative run or posed). Nothing missing
there but finding 10's machine-awake line.

## The proposals as spec diffs, with their conservatives

- **Q1**: robust, no spec token, said so (`00-shared.md:156-158`); conservative K as a diff,
  `bw/arms/q1-k.diff`, +26 vendored (reproduced by `measure`). Complete.
- **Q2**: robust as a diff, two wordings and a third placement, each measured (`spec-warden.md:50-53`,
  reproduced); **conservative E has no words** (finding 9).
- **Q3**: robust as a diff (`bw/arms/proposal.diff`; the blind seat's `changes.diff` reproduces `diff
  spec-A.md spec-B.md` byte for byte, and `patch` of `spec-A.md` by it gives `spec-B.md`, `cmp`-equal to
  `bw/arms/spec-proposal.md`; `spec-A.md` is `cmp`-equal to `git show 3a5ee2691:spec/heroes-spec.md`);
  conservative, no change, named; *plain data parallelism* named and not proposed. Its missing halves are
  findings 2, 7 and 8.

**The blind seat's folder** holds the five files the brief lists, at the sha256 and line counts it gives
(`shasum -a 256`, `wc -l`), and nothing else. `grep -n -i -E` over `brief.md`, `task.md` and
`changes.diff` for `panel|defect|2522|2552|sitting|coordinator|claude|anthropic|design\.md|rebel|seat|ledger|worktree|joseph|heroes-lang|golden|fixedbugs|selfhost|runtime/|spec-warden|ergonomist|thesis|blind`
printed nothing (exit 1). Its **contents** name nothing of the project's internals; its **place** does
(finding 4).

## Re-run by the critic, and each the same as the brief

The Q2 table's rows with my stage 2 (`work/repro/`, each binary under `timeout -s KILL 60`, stderr's
first 300 bytes): `unjoined` 20 of 20 exit 134 with the block line; `unjoined_noheap` 20 of 20 exit 0,
silent; `cr-late` 20 of 20 exit 0, silent; `cr-lease` 10 of 10 exit 134 with the lease line; `exit_running`
5 of 5 exit 0; `t3` 5 of 5 exit 0; `pgrep -fl 251-critic` empty after. The Q1 case (`work/t3/`, the
writer's `main.hero` and `signal_lib.h`): 5 of 5 exit 134 at `-O0` and 5 of 5 at `-O2`, `installed: 0`
then R1's words. `measure` of the frozen spec and of the six arms: every vendored count and every arm's
sha256 and line count as `spec-warden.md:26-54` gives them. T1: 64 words, 416 characters (`python3 -I`).
The census tool (`python3 -I bw/tools/ledger_threads2.py` over my copy): `TOTAL 86 refused 38 pending 9
kept 39`, 144 ledgers. **Every one of the 46 ledger rows** of `00-shared.md:356-408`, each read at its
cited lines and its quoted words found there, its capability and its ruling's first word as the brief
says (`work/ledger_rows.py`, 0 misses). Not re-run by me: the two Rebel programs
(`bw/tree/examples-rebel/bw_unwaited*.hero`), the torn-line counts, 249's measurements (carried by the
brief too).

## Tally

Eleven findings and one answer: **wrong fact** 4 (findings 1, 4, 7, 10); **unrun claim** 1 (8, and the
cost premise inside 2); **missing route** 2 (2, 5); **missing rule in a brief** 3 (6, 9, 11); **a question
the sitting does not ask** 1 (12, and the one inside 7). Finding 3 answers question (b) and faults no
brief.

**Those that change what a seat measures**: 4 and 5 (the blind seat's reading could be voided, and as
designed cannot fill its own decision rules); 6 (the spec-warden's counts stay lower bounds for want of a
free call); 2, 7 and 8 (Q3 priced without its renames, its aliasing value and the compiler's own sized
thread); 1 (a forked-child `sigaction` path no Q1 seat is asked to reach).

Finished 2026-11-19 at 09:51 CET (`date`), begun at 09:34; no seat had started.
