# Panel 251, llm-ergonomist: the blind seat's 51 sessions, run and graded

Written 2026-11-19 by the coordinator, who ran `run.sh` and graded the reports by the rows and the
decision rules `docs/panel/251-briefs/llm-ergonomist.md` registered before any run. Every number below
comes from a command run while this file was written; the commands are named beside them.

## How the sessions were run

- **The runner**: `bash .../rebel-scratch/251-llm-ergonomist/run.sh > run.out 2>&1`, started at
  10:08:24 (`run.out`'s first session line), ended after the verdict session, started 10:49:13. `run.out`
  printed `ANTHROPIC_API_KEY length 108` and `2.1.311 (Claude Code)`; the key's value was never printed.
- **Every session ran with the brief's guards**: `env -i` with `PATH`, `HOME`, `LANG`,
  `GIT_CEILING_DIRECTORIES` and the key alone; `--bare --restricted --safe-mode --strict-mcp-config
  --model claude-opus-5-5`; the tools `Read,Write`; a cap of 0.40 USD each.
- **51 of 51 ended `exit=0 subtype=success`** (`grep -c 'exit=0 subtype=success' run.log`: 51), every
  one on `claude-opus-5-5` alone (`run.log`'s `models=`). **None wrote `report.md`; every one's report is
  the CLI's `result`, saved as `result.md`** (`run.log`: `report.md=no result.md=yes` 51 times), as panel
  250's 24 sessions did; 44 of the 51 reports say the session could not write a file (`grep -l` over
  the reports for the phrases they use).
- **The spend: 8.690507 USD** (`tools/spent.py`), within the 10 USD the author approved for this sitting's
  blind seat (2026-11-19). 50 graded sessions 0.1322 to 0.2112 USD each, the verdict session 0.3772.
  The stop rule never fired: before the verdict session the spend read 8.313339, under its 9.00.

## What reached the sessions besides their folder, and why no session is voided

**No session answered `context: yes`**: every one of the 51 says no rule, contract, memory or project
document reached it (each answer is quoted whole in the appendix, under its session). So by the brief's
rule no session is voided and every cell is read with its five replicates.

**Two things did reach every session, and both are recorded here as the sitting's to weigh:**

1. **The 51 labels of `order.txt`, in every session's user message.** All 51 `context` answers name a
   list running from `t1-y-1` to `t4-h-5` and `verdict`, the last session's included (`t4-h-5`), which
   is `order.txt` whole and in its order. **The cause is a hypothesis, unrun**: `run.sh` reads
   `order.txt` as the loop's standard input (`done < order.txt`), each `claude -p` inherits that input,
   and the CLI appended it to the prompt; that the loop still reached all 51 folders says the CLI read
   the file from its start without moving the loop's position. Confirming it costs a paid session the
   brief does not name, so it is not run. **What it gives away**: each session learns it is one of 51 in
   a design of four tasks and several letters; the letters were drawn to name no arm (the brief's draw),
   and no label carries a specification, a task or an answer. **The repair for the next runner** is
   `claude -p ... < /dev/null`, owed before sitting 252's blind seat runs from its own `run.sh`.
2. **The folder's path**, which names `heroes-lang`, `rebel-scratch` and `251-llm-ergonomist` (four
   reports name `heroes-lang`: `t1-x-5`, `t2-f-4`, `t4-f-3`, `t4-f-4`), as the brief said it would.

**Corrected 2026-11-19, after the critic's second reading** (`critic-reports.md` finding 2): *two things
reached every session* is false; **three** did. The sessions' own transcripts, kept by the CLI under
`~/.claude/projects/` and read by the critic's script, show **the main checkout's git status rendered
into every one of the 51 conversations**, with the branch, the git user's name and five commit subjects
(the first *"Batch 39 round closed: a trap a library recovers goes on ..."*). `GIT_CEILING_DIRECTORIES`
did not stop it. **No answer of the 51 names it**, so *"no session answered `context: yes`"* is true and
is not evidence of a clean context: the answer missed a leak that was there. The same transcripts settle
the labels' cause as the inherited standard input, no longer a hypothesis. The reading kept, and why, is
the synthesis's.

## The grades

The arms by the draw (`key.txt`): `now` y, `q2-r` z, `q2-e` x, `q1-k` c, `proposal-b` f, `proposal-m` h.

| task | arm | right | how it was graded | the sessions that were not right |
|---|---|---|---|---|
| t1 | now | **0 of 5** | by the words, against today's runtime (an abort naming heap blocks) | all five predict a **normal** end, exit 0, no message |
| t1 | q2-r | **5 of 5** | by the words | none |
| t1 | q2-e | **5 of 5** | by the words | none |
| t2 | now | **5 of 5** | **built and run**: each answer's `main.hero` built by the frozen commit's stage 2 (`251-briefwriter/tree/heroes-next`, `--version` `sources 23c7d2a626b29fd5`) and run 5 times under `timeout -s KILL 20`; 25 of 25 runs wrote the four lines exactly, exit 0; each `main` waits for every thread it starts, no `return` before its joins | none |
| t2 | proposal-b | **5 of 5** | by the arm's words: **no compiler has the slice** (`compiler-engineer.md` § Q3.1, *"the compiler half NOT built"*) | none; none declares `spawn`, `spawn_sized`, `wait`, `mailbox`, `send`, `receive` or `close` |
| t2 | proposal-m | **5 of 5** | by the arm's words, as B | none; none declares `spawn`, `spawn_sized` or `mailbox` |
| t3 | now | **4 of 5** | by the words | `t3-y-3` predicts the **build refuses** the program |
| t3 | q1-k | **5 of 5** | by the words | none |
| t4 | proposal-b | **5 of 5** | by the arm's words | none |
| t4 | proposal-m | **5 of 5** | by the arm's words | none |

**`now`'s ending classes on task 1**, recorded beside Q2's rule: **normal 5**, abort 0, never 0, refused
0, hedged 0. What a reader of today's spec assumes is that the program ends normally; `t1-y-1` and
`t1-y-2` name C's `exit` as the model in a choice point.

## The decision rules, as registered

- **Q2, task 1. R's sentence is read as needed**: `q2-r` 5 right against `now`'s 0 re-graded by `q2-r`'s
  row (no `now` answer predicts an abort naming a thread), a difference of 5, at least 3. **E's sentence
  is read as needed** likewise: 5 against 0 (no `now` answer names a thread ended). The rule does not
  choose between them; beside it, the readers' default is a normal end, 5 of 5, which is E's ending
  class and not R's.
- **Q3, task 2. Neither spelling is read as serving the thesis**: B 5 and M 5 against `now`'s 5, a
  difference of 0, under 3. **B against M over tasks 2 and 4: 10 against 10, so the spelling does not
  decide.** B is not recorded as *taking a name readers write*: 0 of its 5 task 2 answers declares one.
- **Q3's mailbox, task 4. The address sentence is read as sufficing in both spellings**: 5 of 5 each.
- **Q1, task 3. Route K's sentence is not read as changing a reader's prediction**: `q1-k` 5 against
  `now`'s 4, a difference of 1, under 3.

## What the counts cannot see, recorded and not graded

- **Task 2's rule had a ceiling.** It counts a right answer on one input, and every one of the 15
  programs is right on it. **All five `now` readers name, in their first choice point, the hazard the
  encoding carries**: a separator that appears in the data compiles and gives wrong counts, or an index
  past the end of `split`'s answer and an abort (`t2-y-1` to `t2-y-5`, quoted in the appendix). That is
  the silent wrong program the thesis asks the compiler to refuse, and B and M remove it (no answer of
  theirs encodes anything); the registered count could not register it, because the task's input holds no
  separator. A task whose input holds one is what would measure it.
- **The proposal does not shorten the program.** `wc -l` of the 15 programs: `now` 52 to 61, B 59 to 71,
  M 61 to 67; B and M declare a `Job` and a `Count` record where `now` packs and unpacks a string.
- **A handle in a list.** The five `now` answers keep their `i64` handles in a list and join them in a
  `for` loop; **the ten B and M answers keep four named handles, `ta` to `td`, and none puts a
  `(thread T)` in a list.** The verdict session names exactly that as a condition that would turn its
  *approve* into *object*: *"An array of thread handles waited in a `for` loop is accepted and then aborts
  at exit."* What the proposal says of a handle inside a list is a question for the synthesis.
- **`t3-y-3`'s refusal** reads §13's *"No function of the program is a signal handler"* as a rule the
  build enforces; route K's sentence, which tells the residue, was read right 5 of 5. One answer cannot
  make the difference of three the rule asks.

## The spec-warden's predictions, scored for information

The brief scores the spec-warden's prediction *"if it registers one before `run.sh` starts"*. **It
registered W1 to W8 at 10:14:06** (`spec-warden.md`), **5 minutes 42 seconds after `run.sh` started**, when
six sessions had finished (`run.out`: `t1-y-1`, `t1-z-1`, `t1-x-1`, `t2-y-1`, `t2-f-1`, `t2-h-1`). Its
transcript holds no tool call into the blind area: the path appears only in an `ls` of `rebel-scratch`
and in the briefs it read (the coordinator's reading of the transcript, every line naming the folder
classified by a script). So the predictions stand **outside the rule's condition** and are scored here
for information, not as registered:

| prediction | as worded | the count | holds |
|---|---|---|---|
| W1 | R read as needed on task 1 (`q2-r` at least 4 of 5, `now` at most 1) | 5, and 0 | yes |
| W2 | likewise for E | 5, and 0 | yes |
| W3 | at least 3 of 5 `now` readers expect a normal ending | 5 | yes |
| W4 | each Q3 spelling beats `now` by 3 or more on task 2 | B 0, M 0 | **no** |
| W5 | B and M differ by less than 2 | 0 | yes |
| W6 | at most 1 of 5 B answers declares a taken name | 0 | yes |
| W7 | task 4 at least 4 of 5 in each spelling | 5 and 5 | yes |
| W8 | K changes predictions by 2 or fewer | 1 | yes |

## The verdict session, recorded whole and grading nothing

Its `verdict`, quoted: *"**Approve.** Spec B makes threads part of the language, with typed handles.
`spawn(f, x)` returns a `(thread T)` whose `T` comes from `f`, and `wait(t)` gives back that `T`. The
meaning of each line can be read from the line and its signature, so a veto does not apply."* Its
`experiment`, `choice_points`, `argument`, `condition` and `context` are in the appendix.

## Where the evidence is

The blind area, `.claude/worktrees/rebel-scratch/251-llm-ergonomist/` (`run.sh`, `run.out`, `run.log`,
`key.txt`, `order.txt`, every session's folder with its `run.json`, `run.err` and `result.md`); the
grading's own files, `.claude/worktrees/rebel-scratch/coord/251-grade/` (the 15 programs of task 2, the
five `now` builds and their 25 runs, `expected.txt`). Both are outside git and go into the sitting's
evidence archive.

## Appendix: every session's cost and grade, then its report whole

| session | arm | exit | subtype | cost, USD | grade |
|---|---|---|---|---|---|
| `t1-x-1` | q2-e | 0 | success | 0.1595 | right |
| `t1-x-2` | q2-e | 0 | success | 0.1677 | right |
| `t1-x-3` | q2-e | 0 | success | 0.1652 | right |
| `t1-x-4` | q2-e | 0 | success | 0.1710 | right |
| `t1-x-5` | q2-e | 0 | success | 0.2003 | right |
| `t1-y-1` | now | 0 | success | 0.1719 | wrong: a normal end, where today's runtime aborts naming heap blocks |
| `t1-y-2` | now | 0 | success | 0.1528 | wrong: a normal end, where today's runtime aborts naming heap blocks |
| `t1-y-3` | now | 0 | success | 0.1559 | wrong: a normal end, where today's runtime aborts naming heap blocks |
| `t1-y-4` | now | 0 | success | 0.1642 | wrong: a normal end, where today's runtime aborts naming heap blocks |
| `t1-y-5` | now | 0 | success | 0.1602 | wrong: a normal end, where today's runtime aborts naming heap blocks |
| `t1-z-1` | q2-r | 0 | success | 0.1525 | right |
| `t1-z-2` | q2-r | 0 | success | 0.1486 | right |
| `t1-z-3` | q2-r | 0 | success | 0.1467 | right |
| `t1-z-4` | q2-r | 0 | success | 0.1676 | right |
| `t1-z-5` | q2-r | 0 | success | 0.1602 | right |
| `t2-f-1` | proposal-b | 0 | success | 0.1798 | right, by the arm's words (no compiler slice) |
| `t2-f-2` | proposal-b | 0 | success | 0.1847 | right, by the arm's words (no compiler slice) |
| `t2-f-3` | proposal-b | 0 | success | 0.1818 | right, by the arm's words (no compiler slice) |
| `t2-f-4` | proposal-b | 0 | success | 0.1834 | right, by the arm's words (no compiler slice) |
| `t2-f-5` | proposal-b | 0 | success | 0.1825 | right, by the arm's words (no compiler slice) |
| `t2-h-1` | proposal-m | 0 | success | 0.1765 | right, by the arm's words (no compiler slice) |
| `t2-h-2` | proposal-m | 0 | success | 0.1693 | right, by the arm's words (no compiler slice) |
| `t2-h-3` | proposal-m | 0 | success | 0.2109 | right, by the arm's words (no compiler slice) |
| `t2-h-4` | proposal-m | 0 | success | 0.1926 | right, by the arm's words (no compiler slice) |
| `t2-h-5` | proposal-m | 0 | success | 0.1662 | right, by the arm's words (no compiler slice) |
| `t2-y-1` | now | 0 | success | 0.1958 | right: built, 5 of 5 runs |
| `t2-y-2` | now | 0 | success | 0.1950 | right: built, 5 of 5 runs |
| `t2-y-3` | now | 0 | success | 0.2092 | right: built, 5 of 5 runs |
| `t2-y-4` | now | 0 | success | 0.2112 | right: built, 5 of 5 runs |
| `t2-y-5` | now | 0 | success | 0.2081 | right: built, 5 of 5 runs |
| `t3-c-1` | q1-k | 0 | success | 0.1466 | right |
| `t3-c-2` | q1-k | 0 | success | 0.1527 | right |
| `t3-c-3` | q1-k | 0 | success | 0.1603 | right |
| `t3-c-4` | q1-k | 0 | success | 0.1549 | right |
| `t3-c-5` | q1-k | 0 | success | 0.1587 | right |
| `t3-y-1` | now | 0 | success | 0.1627 | right |
| `t3-y-2` | now | 0 | success | 0.1820 | right |
| `t3-y-3` | now | 0 | success | 0.1569 | wrong: predicts the build refuses the program |
| `t3-y-4` | now | 0 | success | 0.1610 | right |
| `t3-y-5` | now | 0 | success | 0.1727 | right |
| `t4-f-1` | proposal-b | 0 | success | 0.1413 | right |
| `t4-f-2` | proposal-b | 0 | success | 0.1506 | right |
| `t4-f-3` | proposal-b | 0 | success | 0.1459 | right |
| `t4-f-4` | proposal-b | 0 | success | 0.1508 | right |
| `t4-f-5` | proposal-b | 0 | success | 0.1432 | right |
| `t4-h-1` | proposal-m | 0 | success | 0.1385 | right |
| `t4-h-2` | proposal-m | 0 | success | 0.1360 | right |
| `t4-h-3` | proposal-m | 0 | success | 0.1367 | right |
| `t4-h-4` | proposal-m | 0 | success | 0.1322 | right |
| `t4-h-5` | proposal-m | 0 | success | 0.1384 | right |
| `verdict` | verdict (A: now, B: proposal-b) | 0 | success | 0.3772 | not graded |

The sum of the 51 costs: 8.690507 USD.

### `t1-x-1`, q2-e

~~~~~ markdown
I couldn't save `report.md` because I only had a file-reading tool, so the brief says to give the whole report here instead.

# report.md

## answer

**Files.** I wrote no new files. The only program is the one in the task, reproduced unchanged:

`main.hero`
```
extern "unistd.h"
    function usleep(useconds: u32) -> i32

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

function busy(n: i64) -> i64
    word = "held by a worker " + n.to_str()
    _ = usleep(useconds: 1000000)
    _ = usleep(useconds: 1000000)
    print("worker done: ", word)
    return word.len()

function main()
    handle = hero_thread_spawn(body: busy, arg: 7)
    _ = usleep(useconds: 200000)
    print("main done, worker ", handle, " not joined")
```

**1. Does `heroes build main.hero` accept the program?** Yes.

- **`usleep`:** C declares it as `int usleep(useconds_t)`, and `useconds_t` is a 32-bit unsigned int. So `u32` is the header's own width and sign, and the result `i32` is C's `int` (§13).
- **The literals:** `1000000` and `200000` become `u32` from their context and fit (§2).
- **The thread functions:** `hero_thread_spawn` and `hero_thread_join` are declared exactly as §13 gives them for `hero_os.h`.
- **Passing `busy`:** a function may be a callback parameter (§13). `busy` is a top-level function with no `@` or `lent` parameter, so it can be used as a value (§9). Its type is `(function(i64) -> i64)`.
- **Discarding results:** `_ = usleep(...)` throws away an `i32`, which is not fallible, so it is allowed (§5).
- **Building the string:** `"held by a worker " + n.to_str()` joins two `str` values (§7, §11). The literal has no braces, so the `f`-literal rule doesn't apply (§2).
- **Unused names:** every binding (`word`, `n`, `handle`) is read (§5).
- **`print`:** it takes `str` and the number `handle` (§11).
- **`busy` returns:** `busy` returns on its only path (§8).
- **`main`:** it takes and returns nothing (§1).

**2. What the program writes and how it ends.**

The worker starts with `arg: 7` (§13). It builds `"held by a worker 7"` and then sleeps twice for 1,000,000 µs, about 2 seconds in total, before it would print.

Meanwhile `main` sleeps 200,000 µs (0.2 s) and then writes one line (`print`: no separator, one trailing newline, §11):

1. `main done, worker H not joined`, where `H` is the `i64` that `hero_thread_spawn` returned. The specification does not say what that number is.

Then `main` returns. §13 says: "When `main` returns, the program ends, and a thread still running ends with it; the program says how many it ended." The worker is still in its second sleep, about 1.8 s from its `print`, so it is ended.

- **The worker's line:** `worker done: held by a worker 7` is **not** among the lines written.
- **Exit message:** the runtime then reports that it ended **1** thread. The specification does not give the wording of this message or say which stream it goes to.
- **How it ends:** normally, not by an abort. §13 describes a lease or handle left at the end of `main` as an abort ("aborts when `main` returns, saying how many"). For unfinished threads it uses only "says how many it ended", which is not called an abort. It never hangs: `hero_thread_join` is never called, so nothing waits for the worker.

## choice_points

1. **Type of `word.len()`.** §11 lists `len` but gives no result type. I took it to be `i64`, since `range`, indices and `slice` all use `i64`. If it is `u64`, `return word.len()` in a `-> i64` function is refused, because there are no implicit conversions (§3), and the program does not compile.
2. **Parameter names in function types.** `busy` is `(n: i64) -> i64`, and the parameter wants `(function(i64) -> i64)`. I read §9's "names are part of the type" as applying only to parameters that share a type, and `busy` has one parameter, so the types match. Under the other reading, the call is refused at compile time.
3. **Named argument on a one-parameter function.** I took `usleep(useconds: ...)` as allowed, because `Arg = [ ident ":" ] ...`. Under the other reading, the compiler refuses it.
4. **An extern function that is never called.** `hero_thread_join` is declared but unused. I assumed the unused-binding rule (§5) covers bindings and parameters, not extern declarations. Under the other reading, the compiler refuses the program.
5. **Whether ending a running thread is an abort.** I read it as a normal end with a count message. Under the other reading, the program aborts when `main` returns, saying 1 thread was ended. The printed line would be the same; only how it ends and probably the exit status would change.
6. **Timing.** I assumed the worker cannot print within 0.2 s, because its two sleeps total 2 s before the `print`. If somehow it did (it cannot under any normal scheduler), the worker line would appear before main's line.
7. **The value of `H`.** The specification does not say what a thread handle's value is, so I cannot predict the digits.

## prediction

- **Compiler:** accepts the program.
- **Lines written, in order:**
  1. `main done, worker H not joined`. `H` is the spawn handle, an `i64` whose value the specification does not give.
  2. A runtime notice that 1 running thread was ended when `main` returned. The specification gives neither its wording nor its stream.
- **The worker's line:** `worker done: held by a worker 7` is not written.
- **How it ends:** normally, about 0.2 s after it starts. It does not abort and does not hang.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md`, three other things were in my context:

- **System prompt:** it said I am a Claude agent built on the Claude Agent SDK, and gave the working directory path and the date.
- **The user's message:** it included a list of labels (`t1-y-1` … `t4-h-5`, `verdict`) whose meaning nobody stated.

No other rule, contract, memory or project document reached my context, and I read no other file.
~~~~~

### `t1-x-2`, q2-e

~~~~~ markdown
I only have a read tool here, so I couldn't create `report.md`. As the brief says to do in that case, the whole report is below.

# report.md

## answer

There are no files to write. The task asks me to predict the result of an existing program, `main.hero`, as given in `task.md`.

**1. Does `heroes build main.hero` accept the program?** Yes. I find no rule in the specification that refuses it:

- **Externs (§13):**
  - Grammar: two `extern` groups, each with a header string and `function` members. This matches the `Extern`/`Member` grammar.
  - `usleep(useconds: u32) -> i32`: C declares `int usleep(useconds_t)`. `useconds_t` is an unsigned 32-bit integer on glibc and macOS, and a parameter must be "declared at the header's own width and sign". The `int` result is `i32`.
  - Threads: `hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64` and `hero_thread_join(handle: i64) -> i64` are written exactly as §13 gives them.
  - The callback `body` is a parameter, which §13 allows ("A callback is a **parameter**, never a result…").
- **`busy` as a value (§9):** "Top-level functions are values, but not one with an `@` or `lent` parameter." `busy(n: i64) -> i64` has neither mark and matches `(function(i64) -> i64)`. Named arguments are only mandatory when two parameters share a type, and `busy` has one parameter, so the unnamed type argument is fine.
- **Calls:** The arguments are named (`body:`, `arg:`, `useconds:`), which `Arg = [ ident ":" ] …` allows (§9). The literals `7`, `1000000` and `200000` take their types from context (`i64`, `u32`) and fit (§2).
- **Discarding results (§5):** `_ = usleep(...)` throws away an `i32`, not a `T?` or `()`, so the discard is allowed.
- **`busy`'s body:**
  - `"held by a worker " + n.to_str()` is `str + str` (§7, §11).
  - `print("worker done: ", word)` prints two `str` values (§11).
  - `return word.len()` returns the length (an `i64`, see choice point 1).
  - `n` and `word` are both used.
- **`main` (§1, §11):** `main()` takes nothing and returns nothing. `handle` is used in `print`, and an `i64` is printable.
- **`hero_thread_join` is never called.** §5 only makes unused *bindings* and *parameters* errors. An extern member is a declaration, not a binding, so this is not refused.
- No string literal contains braces, so the rule about literals without `f` doesn't apply (§2). Indentation is 4 spaces per level (§1).

**2. Lines written, and how the program ends.**

What happens at run time:
- `main` starts the worker, then sleeps 0.2 s.
- The worker sleeps 2 × 1 s before it can print.
- After its sleep, `main` prints its line and returns.
- §13: "When `main` returns, the program ends, and a thread still running ends with it; the program says how many it ended." At about 0.2 s the worker is still in its first `usleep`, so it is ended before it prints.

Lines, in order:
1. `main done, worker <H> not joined`. `<H>` is the decimal `i64` handle that `hero_thread_spawn` returned. The specification does not say what that value is (choice point 3). `print` adds no separators and one trailing newline (§11).
2. The runtime's notice that it ended **1** thread. §13 says the program "says how many it ended" but does not give the wording or the stream, so I can't predict the exact text. It says the count is 1.

The line `worker done: held by a worker 7` is **not** among them. The worker is ended at about 0.2 s, while it is still in its first one-second sleep (§13).

How it ends: **normally.** `main` returns, and that ends the program (§13). Ending running threads is described as something the program reports, not as an abort. Compare the leases and handles in §13, which explicitly "abort when `main` returns, saying how many". The exit status is not stated, and it is presumably 0.

## choice_points

1. **Type of `str.len()`.** The specification never gives `len`'s result type.
   - My choice: `i64`, the default integer type of §2, which `range` and indices also use. So `return word.len()` matches `-> i64`.
   - The other reading: if `len` returned `u64` (as `repeat`'s `n` might suggest), the compiler would refuse the program because of a type mismatch with no implicit conversion (§3).
2. **Whether an unused extern member (`hero_thread_join`) is an error.**
   - My choice: no, because §5 lists only bindings, parameters, cells and `@` parameters.
   - The other reading: if it counted, the compiler would refuse the program for an unused declaration.
3. **The value of the spawn handle `<H>`.** It is unspecified (it could be 0, 1, a pointer-sized number, and so on).
   - My choice: leave it symbolic.
   - The other reading: any concrete number I chose could differ from the real output. That wouldn't change acceptance or how the program ends.
4. **Wording and stream of the "threads ended" notice.**
   - My choice: a message giving the count 1, probably on stderr. It is not an abort.
   - The other reading: if the implementation treats it as an abort, the program ends by an abort that gives the count 1 instead of ending normally. The lines before it are unchanged.
5. **`usleep(1000000)` on the target platform.** Old POSIX allowed `EINVAL` for 1,000,000 µs or more.
   - My choice: glibc or macOS, which sleep the full second.
   - The other reading: on a platform that returns `EINVAL` at once, the worker skips both sleeps. It prints `worker done: held by a worker 7` almost immediately, most likely before `main`'s line, and the program then ends with no running thread to report.
6. **Scheduling.** I assume the 0.2 s against 2 s margin holds, which it will except on a pathologically stalled system. If the worker did finish first, its line would appear and no thread would be reported as ended.

## prediction

- **Compiler:** accepts the program and builds a binary.
- **Output, in order:**
  1. `main done, worker <H> not joined`, where `<H>` is the unspecified `i64` handle
  2. a runtime notice that 1 running thread was ended (exact text unspecified)
- `worker done: held by a worker 7` is **not** written.
- **End:** normally, about 0.2 s after start, when `main` returns. The unfinished worker is ended with it. There is no abort.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory were read. Nothing else reached my context: no other file, memory or project document. The only extra items were the general system/agent instructions (a Claude agent with a file-read tool) and the user's message, which ended with a list of labels like `t1-y-1 … verdict` that I treated as not relevant to the task.
~~~~~

### `t1-x-3`, q2-e

~~~~~ markdown
I couldn't write `report.md` because I can only read files here, so the whole report follows, as the brief allows.

# report.md

## answer

**The file, unchanged** (path `main.hero`; the task's 4-space indent is read as markdown quoting, so the file starts at column 0):

```
extern "unistd.h"
    function usleep(useconds: u32) -> i32

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

function busy(n: i64) -> i64
    word = "held by a worker " + n.to_str()
    _ = usleep(useconds: 1000000)
    _ = usleep(useconds: 1000000)
    print("worker done: ", word)
    return word.len()

function main()
    handle = hero_thread_spawn(body: busy, arg: 7)
    _ = usleep(useconds: 200000)
    print("main done, worker ", handle, " not joined")
```

**1. Does `heroes build main.hero` accept it? Yes.**

- **`usleep`:** C declares `int usleep(useconds_t)`, and `useconds_t` is a 32-bit unsigned integer. §13 requires "A **parameter** … declared at the header's own width and sign", so `u32` is correct. The result is `i32` for `int`, which also matches ("a result may be wider than C's").
- **The thread functions:** both match §13's signatures word for word. `hero_os.h` is the language's own header, so it needs no `link`.
- **Literals:** `1000000` and `200000` are typed `u32` by their context and fit (§2). `7` is an `i64`.
- **`_ = usleep(...)`:** this discards an `i32`, which is a value and not fallible, so it is allowed (§5).
- **`busy` as a function value:** it has no `@` or `lent` parameter, so it may be passed as a value (§9). Its type `(function(i64) -> i64)` matches `body`, and it is a callback parameter of a C call, which §13 permits.
- **Unused-name rules (§5):** `n`, `word` and `handle` are all read, and there are no mutable cells.
- **Strings and printing:** `"..." + n.to_str()` joins two `str` values (§7). `print` takes `str` and `i64` (§11).
- **`return word.len()`:** the function's `-> i64` is satisfied, assuming `len` gives an `i64` (see choice point 2).
- **`hero_thread_join`:** declaring it without calling it is not an error. §5 only makes unused *bindings or parameters* errors.

**2. What the program writes, in order, and how it ends:**

1. `main done, worker <h> not joined`. Here `<h>` is the integer handle `hero_thread_spawn` returns. The spec does not fix its value. This line comes about 0.2 s after start.
2. A notice from the runtime that it ended one running thread. §13 says: "When `main` returns, the program ends, and a thread still running ends with it; the program says how many it ended." The spec gives neither the exact wording nor the stream.

**The line `worker done: held by a worker 7` is not among them.** The worker sleeps 2 s before printing, but `main` sleeps only 0.2 s and then returns. Under §13 that ends the program and the worker with it. Nothing calls `hero_thread_join`, which is the only thing that would wait.

**How it ends:** normally. §13 says the program ends and reports how many threads it ended. Unlike leases or unconsumed handles, which "abort when `main` returns", this case is not called an abort. The exit code is not specified; I expect 0.

## choice_points

1. **Indentation.** I read the task's 4-space indent as markdown quoting, so the file starts at column 0. If those 4 spaces were really in the file, every top-level line would start indented, which the grammar does not allow (§1, §4). The compiler would refuse the program.
2. **What type `len` returns.** The spec never states it. I assumed `i64`, based on how the examples use it (`xs.len() == 0`). If it were `u64`, `return word.len()` in a `-> i64` function would be a type error under §3's "No implicit conversions", and the compiler would refuse the program.
3. **The `usleep` parameter type.** I took `useconds_t` to be `u32`, as it is on macOS and Linux. If a platform defined it with another width, §13 would refuse the declaration.
4. **An extern function that is never called.** I read §5's unused-name rule as covering bindings and parameters only, so declaring `hero_thread_join` without calling it compiles. The other reading makes it a compile error.
5. **Is the thread-count notice an abort?** I read it as a normal end, because §13 does not say "aborts" here and does say it for leases and handles. The other reading would end the program with an abort that names one ended thread. The worker's line would still be missing.
6. **Where the notice goes and what it says.** The spec does not say, so I cannot give its exact text or say whether it goes to stdout or stderr.
7. **The worker finishing first.** POSIX says `usleep` *may* fail with EINVAL for 1,000,000 µs or more. If a platform did that, the worker's sleeps would return at once and `worker done: held by a worker 7` could print before `main`'s line. Neither macOS nor glibc does this, and the spec says nothing about it. I predict the normal behaviour.

## prediction

- **Compiler:** accepts the program.
- **Lines written, in order:**
  1. `main done, worker <h> not joined`, where `<h>` is an integer the spec does not fix.
  2. The runtime's notice that it ended 1 thread, in wording the spec does not give.
- `worker done: held by a worker 7` is **not** written.
- **End:** normal, about 0.2 s after start, when `main` returns. The worker is ended without finishing.

## context

Apart from `brief.md`, `task.md` and `heroes-spec.md` in this directory, only these reached my context:
- the system prompt, which says I am a Claude agent built on the Claude Agent SDK and gives the working directory and date;
- the user message, which ends with a list of bare labels (`t1-y-1` … `verdict`) and contains nothing else.

No other file, rule, contract or memory reached my context. My knowledge of POSIX `usleep` and `useconds_t` comes from general background, not from any file.
~~~~~

### `t1-x-4`, q2-e

~~~~~ markdown
I couldn't write `report.md` because I only have a file-reading tool. As the brief allows, the whole report follows here.

---

# report.md

## answer

**The file, unchanged, at `main.hero`:**

```
extern "unistd.h"
    function usleep(useconds: u32) -> i32

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

function busy(n: i64) -> i64
    word = "held by a worker " + n.to_str()
    _ = usleep(useconds: 1000000)
    _ = usleep(useconds: 1000000)
    print("worker done: ", word)
    return word.len()

function main()
    handle = hero_thread_spawn(body: busy, arg: 7)
    _ = usleep(useconds: 200000)
    print("main done, worker ", handle, " not joined")
```

**1. Does `heroes build main.hero` accept it? Yes.** Each check:

- **`usleep`:** C declares `int usleep(useconds_t)`. `useconds_t` is a 32-bit unsigned integer on both glibc and macOS, so `u32` matches its width and sign (§13: "A parameter … declared at the header's own width and sign"). The result `i32` matches `int`.
- **The `hero_os.h` group:** its two declarations are word for word the signatures §13 gives for threads. Neither group needs names from the other, so the order the groups are written in doesn't matter (§13).
- **Passing `busy` to the thread:** `busy` is a top-level function with no `@` or `lent` parameter, so it can be used as a value (§9). It takes one `i64` and returns one `i64`, which is what `(function(i64) -> i64)` asks for. Parameter names are only part of a function type when two parameters share a type (§9). With one parameter, `n` doesn't matter.
- **Literals:** `7` is an `i64`. `1000000` and `200000` take `u32` from the parameter's type and fit in it (§2).
- **Discarded results:** `_ = usleep(...)` throws away an `i32`, which is allowed because it isn't a `T?` (§5). `print(...)` returns `()` and stands alone on its line, as §5 requires.
- **Unused names:** `n`, `word` and `handle` are each read, so nothing is unused (§5).
- **`busy` returns on every path** (§8). `word.len()` returns an integer, taken to be `i64`.
- **The plain string has no braces**, so the rule about missing `f` prefixes doesn't apply (§2).
- **Indentation** is 4 spaces per level (§1).
- **`hero_thread_join` is declared but never called.** Only unused bindings and parameters are errors (§5). A declaration is neither.

**2. Lines the program writes, in order:**

1. `main done, worker H not joined`, where `H` is the decimal value of the `i64` handle that `hero_thread_spawn` returned. The spec doesn't fix that value. `print` writes its parts with no separator and one newline at the end (§11).
2. A notice from the runtime that it ended 1 thread. §13 says: "When `main` returns, the program ends, and a thread still running ends with it; the program says how many it ended." The spec gives neither the wording nor the stream (stdout or stderr).

**The line `worker done: held by a worker 7` is not written.** The worker sleeps two seconds (2 × `usleep(1000000)`) before it prints. `main` sleeps only 0.2 s, prints, and returns. Under the §13 rule above, returning from `main` ends the program and the still-sleeping worker with it, before the worker reaches its `print`.

**How it ends:** normally, after about 0.2 s, with the notice that 1 thread was ended. It does not abort. For leases and handles, §13 says the program "aborts when `main` returns, saying how many". For threads it says only that the program "ends" and "says how many". The exit code isn't specified; I expect 0.

## choice_points

1. **Unused extern declaration (`hero_thread_join`).**
   - My choice: accepted, because §5 makes only unused bindings and parameters errors.
   - The other reading: the compiler refuses the program for an unused declaration.
2. **A function type without parameter names (`(function(i64) -> i64)` against `busy(n: i64)`).**
   - My choice: they match, because names only count for a shared-type pair (§9).
   - The other reading: the compiler refuses it as a type mismatch.
3. **Unjoined thread at the end of `main`.**
   - My choice: the program ends normally with a count notice. §13 says "ends … says how many" and, unlike the lease rule, doesn't say "aborts".
   - The other reading: the program ends with an abort reporting 1 thread, and a non-zero exit status.
4. **Whether the worker is killed or waited for.**
   - My choice: killed ("a thread still running ends with it", §13).
   - The other reading: the program waits for the worker. It would then print `worker done: held by a worker 7` after about 2 s and end later. That is a different answer from the one intended.
5. **Where the thread-count notice goes, and its wording.** The spec says neither. I assume stderr and a free-form message. If it goes to stdout, it is a second stdout line.
6. **`usleep(1000000)`.** Older POSIX allows `EINVAL` for 1,000,000 µs or more. Some platforms may return −1 at once instead of sleeping. The result is discarded, and the worker still couldn't finish its 2 s of sleeping in main's 0.2 s window in the normal case. If both calls returned immediately, the worker might print `worker done: …` before main's line, so the output would change. I assume the sleeps happen (glibc and macOS both sleep).
7. **What `len()` returns.** I take it to be `i64`, as `busy`'s `-> i64` needs. If it were another width, the compiler would refuse `return word.len()` (§3: no implicit conversions).
8. **The handle's printed value.** The spec doesn't fix it, so I give it as `H`.

## prediction

- **Does it compile?** Yes, the compiler accepts the program.
- **Output, in order:**
  1. `main done, worker H not joined`, where `H` is the spawn handle's decimal value.
  2. The runtime's notice that 1 still-running thread was ended. The spec gives no wording or stream; I expect stderr.
- **`worker done: held by a worker 7` is not written.**
- **How it ends:** normally after about 0.2 s, not by an abort, with an exit status presumably 0.

## context

- **Other instructions that reached my context:**
  - A system preamble saying I am an agent built on Anthropic's Claude Agent SDK, with the working directory and the date 2026-11-19.
  - A list of identifiers added after the instruction in the user's message ("t1-y-1 … t4-h-5, verdict"). I treated it as having no meaning for this task.
- **Files read:** only `brief.md`, `task.md` and `heroes-spec.md` in this directory.
- **No other rule, contract, memory or document** (such as a CLAUDE.md or other files) reached my context.
~~~~~

### `t1-x-5`, q2-e

~~~~~ markdown
I couldn't write `report.md` because I only have a file-reading tool here. As `brief.md` allows in that case, the full report follows.

# report.md

## answer

**1. Does `heroes build main.hero` accept the program?** Yes. Every part of the program is allowed by the specification:

- **Extern groups (§13).** Threads come from the language's `hero_os.h`, and the two declared functions match the signatures §13 gives word for word. `usleep(useconds: u32) -> i32` matches C's `int usleep(useconds_t)`. Both `useconds_t` and `u32` are a 32-bit unsigned integer, and the result `int` is `i32`. Neither group uses the other's names, so their order doesn't matter.
- **Passing `busy` as a value (§9).** "Top-level functions are values, but not one with an `@` or `lent` parameter." `busy(n: i64) -> i64` has neither mark, so it has the type `(function(i64) -> i64)`, which is what `body` expects.
- **Named arguments (§9, `Arg = [ ident ":" ] ...`).** Labels are optional when parameter types differ, but writing them is allowed.
- **Literal types (§2).** `7` becomes `i64` and `200000` and `1000000` become `u32`, because the parameters ask for those types. All of them fit.
- **Discarding results (§5).** Each `_ = usleep(...)` throws away an `i32`, which is not a `T?`, so that's allowed. `word`, `n` and `handle` are each read, so nothing is unused.
- **Building the string (§7, §11).** `"held by a worker " + n.to_str()` joins `str` to `str`. `to_str` can't fail.
- **Printing (§11).** `print` accepts `str` and `i64` values.
- **Returning (§8).** `busy` reaches `return` on its only path. `word.len()` is taken to be an `i64` (see choice points).
- **`main` (§1).** It takes nothing and returns nothing.

**2. What the program writes, and how it ends.**

1. `main done, worker H not joined`, where `H` is the decimal `i64` that `hero_thread_spawn` returned. The specification doesn't say what that number is. This line appears about 0.2 s after start, from `usleep(useconds: 200000)` (§11 `print`: no separator, one newline).
2. A notice from the runtime that it ended 1 thread. §13 says: "When `main` returns, the program ends, and a thread still running ends with it; the program says how many it ended." The specification gives neither the wording nor the stream.

**`worker done: held by a worker 7` is NOT among the lines.** The worker needs about 2 s (two sleeps of 1,000,000 µs) before it reaches its `print`. `main` returns after about 0.2 s and never calls `hero_thread_join`. Under §13, the worker ends together with the program.

**How it ends:** normally, after about 0.2 s, with the thread-count notice. It is not an abort. In §13 a lease nobody ends "aborts when `main` returns, saying how many", but a still-running thread only "ends with it". The specification doesn't give the exit status.

File (unchanged; the task asks for no new files):

`main.hero`
```
extern "unistd.h"
    function usleep(useconds: u32) -> i32

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

function busy(n: i64) -> i64
    word = "held by a worker " + n.to_str()
    _ = usleep(useconds: 1000000)
    _ = usleep(useconds: 1000000)
    print("worker done: ", word)
    return word.len()

function main()
    handle = hero_thread_spawn(body: busy, arg: 7)
    _ = usleep(useconds: 200000)
    print("main done, worker ", handle, " not joined")
```

## choice_points

1. **Type that `len()` returns.** The specification never states it. I took it to be `i64`, the default integer type, which indices and `range` also use. If it is `u64` (note that `repeat` takes `n` as a `u64`), the compiler refuses `return word.len()` in a function returning `i64`, because §3 allows no implicit conversions.
2. **The declared but uncalled `hero_thread_join`.** I read the §5 rule against unused bindings and parameters as covering local bindings and function parameters, not extern declarations, so the program is accepted. Under the other reading, the compiler refuses the unused declaration.
3. **Width of `useconds_t`.** I took it to be `unsigned int`, which is `u32` on Linux and macOS, so clang accepts the declaration. On a platform where it is a different width, the parameter is refused (§13: "one that disagrees is refused").
4. **Parameter name `useconds` versus C's own parameter name.** §13 checks a parameter's width and sign, not its name, so I treated the name as free. If names had to match the header, the compiler would refuse it.
5. **Whether `usleep` is declared in `unistd.h` at all.** I assumed clang reads the header in its default mode, where glibc and macOS declare `usleep`. Under a strict-ISO mode, glibc hides it and the build is refused. The specification only mentions `_GNU_SOURCE` ordering.
6. **What `usleep(1000000)` does.** That is C's behaviour, not the specification's. I assumed glibc or macOS, which sleep for 1 s. On a strictly POSIX implementation that rejects one million or more microseconds with `EINVAL`, both sleeps return at once. The worker would then print `worker done: held by a worker 7` before main's line, finish, and the program would end with no running thread to report. That is a different output.
7. **Timing.** I relied on the 1.8 s gap between main returning and the worker's `print`. The specification guarantees nothing about scheduling, but under my assumptions the worker's line cannot come first.
8. **The thread-ended notice.** I read it as part of a normal end, not an abort, because §13 uses "aborts" for leases and handles but not for threads. If it were an abort, the program would end by an abort that reports 1 thread ended. Its wording, stream and exit status are unspecified either way.
9. **Value of `handle`.** It is unspecified, so the number in line 1 cannot be predicted.
10. **Linking `hero_os.h`.** I assumed it needs no `link`, since §13 says the language's headers are "linked where a group names one". If a `link` were required, the link step would fail.

## prediction

- **Compiler:** accepts the program, under choice points 1–5.
- **Lines, in order:**
  1. `main done, worker H not joined` (`H` = the handle's decimal value, which the specification doesn't give)
  2. the runtime's notice that it ended 1 thread (wording not given by the specification)
- `worker done: held by a worker 7` is not written.
- **Ending:** normally, after about 0.2 s, ending the one still-running worker thread. No abort.

## context

Apart from `brief.md`, `task.md` and `heroes-spec.md`, I read no file. Two other things did reach my context:

- **System prompt:** it said I am an agent built on the Claude Agent SDK and gave the working directory path, which contains `heroes-lang/.claude/worktrees/rebel-scratch/251-llm-ergonomist`, and the date.
- **User message:** after the instruction, it listed run identifiers (`t1-y-1` … `t4-h-5`, `verdict`) with no content behind them.

No other rules, memory, contracts or documents reached me, and none of this affected the analysis.
~~~~~

### `t1-y-1`, now

~~~~~ markdown
I couldn't write `report.md` because I only have a file-reading tool here, so the full report follows, as the brief allows.

# report.md

## answer

**1. Does `heroes build main.hero` accept the program?** Yes. Here is the check, line by line:

- **The `unistd.h` block.** Every group names its header, and "a parameter and a field are declared at the header's own width and sign" (§13). C declares `int usleep(useconds_t)`, and `useconds_t` is a 32-bit unsigned int, so `useconds: u32` matches. The result `i32` matches C's `int`, and clang checks it (§13).
- **The `hero_os.h` block.** Both signatures are exactly the ones §13 gives: `hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64` and `hero_thread_join(handle: i64) -> i64`. §13 says threads come "from the language's `hero_os.h` … linked where a group names one", and this group names it.
- **`busy`**
  - `n` and `word` are both read, so neither is unused (§5).
  - `"held by a worker " + n.to_str()` is `str + str`, which is concatenation (§7, §11).
  - In `_ = usleep(useconds: 1000000)` the literal takes `u32` from its context and fits (§2). The `i32` result is a value, not a `T?` and not `()`, so discarding it with `_ =` is allowed (§5).
  - `print("worker done: ", word)` prints two `str` values (§11).
  - `return word.len()` returns an `i64` (choice point 1).
- **`main`**
  - `busy` is a top-level function with no `@` or `lent` parameter, so it can be used as a value (§9).
  - Its one parameter shares a type with no other, so its name is not part of its type. That makes it a `(function(i64) -> i64)` (§9).
  - `7` becomes an `i64` (§2). Labels are allowed (`Arg = [ ident ":" ] …`, §9).
  - `handle` is read by `print`, which accepts an `i64` (§11).
- **`hero_thread_join` is never called.** No rule in the spec refuses an unused declaration. The rule about unused things covers bindings and parameters, and extern parameters have no body (§5).
- **Indentation** is 4 spaces per level and the file has no tabs (§1).

**2. What the program writes, and how it ends.**

The program writes exactly one line:

```
main done, worker H not joined
```

`H` is the `i64` that `hero_thread_spawn` returns. The spec doesn't say what number that is, so I can't predict it.

The line `worker done: held by a worker 7` is **not** among the lines written. Here is why:

- The worker starts in `busy(7)`, builds `word`, and sleeps for 1 s and then another 1 s before it would print.
- Meanwhile `main` sleeps 0.2 s, prints its line, and returns.
- `main` returning ends the program (§1: `main` "takes nothing and produces nothing"). The spec gives no rule that the process waits for unjoined threads. `hero_thread_join` is described as the call that "waits for its answer" (§13), and the program never calls it.
- So the worker is still in its first sleep (about 0.2 s into a 2 s wait) when the process ends.

**How it ends:** normally, exit status 0, after about 0.2 s. There is no abort:

- The only end-of-`main` aborts are for "a lease nobody ends, like a handle nobody consumes" (§13).
- The thread handle is a plain `i64`, not a `tag` handle record with an `acquires` mark. §13 forbids those marks except on a value that reaches a handle.
- Nothing overflows, and nothing is indexed (§7, §10).

## choice_points

1. **What type `len()` returns.** The spec lists `len` without a signature (§11). I took it as `i64`, since §6 compares `xs.len() == 0` with a bare literal. If it were `u64`, the compiler would refuse `return word.len()` in a function declared `-> i64`, because there are no implicit conversions (§3).
2. **Whether `main` returning waits for running threads.** The spec doesn't say. I took it that returning from `main` ends the process, worker included, as C's `exit` does. If the runtime joined live threads first, the program would also print `worker done: held by a worker 7` about 2 s in, after main's line, and then end normally. That gives a different answer from the one I predict.
3. **Whether an unjoined thread aborts at the end of `main`.** I took it that it doesn't, because the abort rule names only leases and handles, and a thread handle is an `i64`. If it did, the run would end with an abort naming the unjoined thread, after main's line.
4. **Whether an extern function that is never called is refused.** I took it that it isn't, because §5 covers only bindings and parameters. If it were, the compiler would refuse the program over `hero_thread_join`.
5. **Whether `usleep(1000000)` fails.** POSIX lets `usleep` reject values ≥ 1,000,000 with `EINVAL`, although glibc and macOS both sleep the full second. Either way the result is discarded and the worker never gets to print, so the output doesn't change.
6. **Whether the C environment declares `usleep`.** `usleep` is not in strict ISO C. If the build compiled the C side in a strict mode without `_DEFAULT_SOURCE`, the header check could fail. Then the compiler would refuse the program, and §13 would want a group that defines `_GNU_SOURCE` placed first. The spec doesn't say what mode the build uses, so I took it that the header declares `usleep`, as it does by default on Linux and macOS.

## prediction

- **Compiler:** accepts the program and produces a binary.
- **Output, in order:** `main done, worker H not joined`, where `H` is the unspecified `i64` handle. `worker done: held by a worker 7` is not written.
- **End:** normally, exit status 0, about 0.2 s after starting. There is no abort.

## context

- **Files read:** `brief.md`, `task.md` and `heroes-spec.md` from this directory, and nothing else.
- **Other context:** the system prompt said I am a Claude agent on the Claude Agent SDK and gave the working directory path. The user message also included a list of labels (`t1-y-1` … `t4-h-5`, `verdict`) that I did not use.
- **Not received:** no project instructions, rules, contracts, memory or other documents reached my context.
~~~~~

### `t1-y-2`, now

~~~~~ markdown
I can't write `report.md` because my only tool reads files. As the brief allows, here is the whole report as my answer.

# report.md

## answer

**Task's `main.hero`** (path `main.hero`; I used the file exactly as the task gives it, with the task's 4-space display indent removed so top-level lines start at column 0):

```
extern "unistd.h"
    function usleep(useconds: u32) -> i32

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

function busy(n: i64) -> i64
    word = "held by a worker " + n.to_str()
    _ = usleep(useconds: 1000000)
    _ = usleep(useconds: 1000000)
    print("worker done: ", word)
    return word.len()

function main()
    handle = hero_thread_spawn(body: busy, arg: 7)
    _ = usleep(useconds: 200000)
    print("main done, worker ", handle, " not joined")
```

**1. Does `heroes build main.hero` accept it?** Yes. Each check against the spec:

- **`hero_os.h` declarations.** §13 gives the thread functions with exactly these signatures, `hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64` and `hero_thread_join(handle: i64) -> i64`, and says `hero_os.h` is the language's own header, "linked where a group names one". No `link` is needed.
- **`usleep(useconds: u32) -> i32`.** C declares `int usleep(useconds_t)`, and `useconds_t` is a 32-bit unsigned int. §13 says a parameter must match the header's width and sign, and clang checks the result type. Both match.
- **Order of the two groups.** Neither header needs the other's names, so §13's order rule is met.
- **Passing `busy` as `body`.** §9 says top-level functions are values unless they have an `@` or `lent` parameter, and `busy` has neither. §13 says a callback must be a parameter, and here it is one. `busy` has type `(function(i64) -> i64)`. Parameter names would only be part of the type if two parameters shared a type (§9), and here there is just one.
- **Literals.** `1000000` and `200000` take the type `u32` from their context and fit it (§2). `7` is an `i64`.
- **Discarded values.** `_ = usleep(...)` drops an `i32`, which is neither `()` nor fallible, so §5 allows it.
- **Bindings.** `word` and `handle` are each read, and there are no cells, so nothing is unused (§5).
- **`print`.** It takes `str`, `i64` and `str`, all types it renders (§11).
- **Return paths.** `busy` returns on its only path. `main` takes nothing and has no `->` (§1, §8).
- **`unistd.h` group.** It declares `hero_thread_join` but never calls it. Nothing in §13 refuses an unused extern member.
- **`word.len()`.** I assume `len` gives an `i64` (see choice points).

**2. What the program writes, and how it ends.**

1. `main done, worker H not joined`. H is the decimal `i64` that `hero_thread_spawn` returned. The spec does not say what value a thread handle has, so I cannot give its digits. `print` writes its values with no separator and one trailing newline (§11).

The line `worker done: held by a worker 7` is **not** among them. Here is why:

- The worker sleeps 1 s twice (2 s in total) before it prints.
- `main` sleeps 0.2 s, prints its line, and returns.
- The spec does not say that `main` waits for unjoined threads. §13 makes waiting the job of `hero_thread_join`: it "waits for its answer". This program never calls it.
- When `main` returns, the process ends, as a C program's does. That kills the sleeping worker before it reaches its `print`.

The program **ends normally**, about 0.2 s after it starts, with no abort:

- The only abort at the end of `main` is for "a lease nobody ends, like a handle nobody consumes" (§13).
- The thread handle here is a plain `i64`, not a `record ... tag` handle with an `acquires` mark, so nothing is owed and there is nothing to report.
- No overflow, index or `.must()` abort is reachable.

## choice_points

1. **What happens to unjoined threads when `main` returns.** The spec only says that `hero_thread_join` waits (§13) and that `exit` ends the program (§11).
   - My choice: returning from `main` ends the process and kills the worker, as in C, so one line is printed.
   - The other reading: the runtime joins every thread before exiting. The program would then run and give a different answer. It would print a second line, `worker done: held by a worker 7`, about 2 s in, and still end normally.
2. **What type `len` returns.** §11 lists `len` without a signature.
   - My choice: `i64`, the language's default integer type. `range` gives `[i64]` and literals default to `i64` (§2).
   - The other reading: `len` returns `u64` (`repeat` takes a `u64`). Then `return word.len()` in a `-> i64` function breaks §3's "no implicit conversions" rule, and **the compiler refuses the program**.
3. **What value the handle has.** The spec does not say. Any digits I gave would be a guess, so the line is predicted only in form.
4. **How the file is laid out.** I read the task's indented block as the file with its display indent removed. Read literally, a top-level line starting at column 4 is an indentation error and **the compiler refuses the program** (§1).
5. **Whether `usleep` really takes 32 bits.** I took `useconds_t` to be a 32-bit unsigned int, as on Linux and macOS. On a platform where it differs, clang's width check would make **the compiler refuse the program** (§13).

## prediction

- **Compiler:** accepts the program.
- **Lines written, in order:**
  1. `main done, worker H not joined`, where H is the decimal `i64` handle, which the spec leaves unspecified.
- `worker done: held by a worker 7` is **not** written.
- **How it ends:** normally, after about 0.2 s, with no abort message. The exit status is presumably 0.

## context

Nothing reached my context except the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`) and the operator's instruction to follow the brief. That instruction gave the working directory path and a list of sibling run names (t1-y-1 … verdict), but none of their contents. No other rule, contract, memory or document reached me.
~~~~~

### `t1-y-3`, now

~~~~~ markdown
I couldn't create `report.md` because I only have a file-reading tool here. As the brief allows, here is the whole report.

# report.md

## answer

**1. Does `heroes build main.hero` accept the program?**

Yes. Every line checks against the specification:

- **`usleep` declaration.** `function usleep(useconds: u32) -> i32` declares the parameter at C's own width and sign (`useconds_t` is a 32-bit unsigned int) and the result as `int` → `i32` (§13: "A **parameter** and a **field** are declared at the header's own width and sign"). The literal `1000000` takes the type its context asks for, `u32`, and it fits (§2). The label `useconds:` is allowed by `Arg = [ ident ":" ] …` (§9).
- **Discarding the `usleep` result.** `_ = usleep(...)` throws away an `i32` on purpose. That is allowed because the result is neither `()` nor a `T?` (§5).
- **Thread declarations.** `hero_thread_spawn` and `hero_thread_join` are declared exactly as §13 gives them for `hero_os.h`.
- **`busy`.** `"held by a worker " + n.to_str()` joins two `str` values (§7). `to_str` cannot fail (§11). `n.to_str()` is UFCS sugar (§9). `print("worker done: ", word)` prints `str` values (§11). `word.len()` gives the `i64` that `return` needs. Every binding is used.
- **`main`.** `busy` is a top-level function with no `@` or `lent` parameter, so it is a value (§9). Its type `(function(i64) -> i64)` matches `body`, and `arg: 7` is an `i64`. `handle` is used by `print`, which accepts numbers. `main` takes nothing and returns nothing (§1).
- **Unused declaration.** `hero_thread_join` is declared but never called. §5 makes only an unused *binding or parameter* an error, and an extern member is a declaration, not a binding (see choice_points).
- **Unjoined thread.** Not joining the thread is not a compile error. The thread handle is a plain `i64`, not a handle record (a handle is "one with a `tag` and no fields", §13), so no `acquires`/`consumes` obligation applies.

**2. Lines written and how the program ends**

1. After about 0.2 s, `main` writes one line: `main done, worker <H> not joined`. Here `<H>` is the decimal `i64` that `hero_thread_spawn` returned. The specification doesn't say what that number is (§11: `print` writes its values with no separator and one trailing newline).
2. Then `main` returns. Section 13 says threads come from the C library `hero_os.h`, so they follow C rules: returning from `main` ends the process and every thread in it. At that moment the worker is still in its first one-second `usleep`.
3. So the line `worker done: held by a worker 7` is **not** written. The worker would reach its `print` only after about 2 s, and nothing joins it (the specification says only that `hero_thread_join` "waits for its answer", §13).
4. The program ends **normally**, with exit status 0 and no abort. The only rule about aborting when `main` returns covers unended leases and unconsumed handles (§13: "a lease nobody ends, like a handle nobody consumes, aborts when `main` returns, saying how many"). Neither exists in this program.

There is only one file, and it is unchanged:

`main.hero`
```
extern "unistd.h"
    function usleep(useconds: u32) -> i32

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

function busy(n: i64) -> i64
    word = "held by a worker " + n.to_str()
    _ = usleep(useconds: 1000000)
    _ = usleep(useconds: 1000000)
    print("worker done: ", word)
    return word.len()

function main()
    handle = hero_thread_spawn(body: busy, arg: 7)
    _ = usleep(useconds: 200000)
    print("main done, worker ", handle, " not joined")
```

## choice_points

1. **What happens to a running thread when `main` returns.** The specification doesn't say.
   - *My choice:* C rules apply, because threads come from `hero_os.h`. Returning from `main` exits the process and kills the worker. The output is one line and the end is normal.
   - *The other reading:* the runtime waits for all threads, or joins them, before exiting. Then the program also writes `worker done: held by a worker 7` about 2 s in and ends normally, which is a different answer.
   - *A third reading:* the runtime treats an unjoined thread like an unconsumed handle and aborts at `main`'s return. Then the program writes the main line and stops with an error.
2. **An extern function that is declared but never called (`hero_thread_join`).**
   - *My choice:* allowed, because §5's "unused" rule covers bindings and parameters only.
   - *The other reading:* the compiler refuses the program for an unused declaration, and nothing runs.
3. **Is the `i64` thread handle a "handle" in the §13 sense?**
   - *My choice:* no, because a handle is a fieldless record with a `tag`. So nothing aborts at `main`'s return.
   - *The other reading:* it counts as an unended life, and the program aborts when `main` returns, "saying how many".
4. **`usleep`'s parameter as `u32`.**
   - *My choice:* this matches `useconds_t` (a 32-bit unsigned int on Linux and macOS).
   - *The other reading:* on a platform where `useconds_t` has another width, clang refuses the parameter (§13), and the build fails.
5. **The value `<H>` that `main` prints.** The specification doesn't say what `hero_thread_spawn` returns, so I predict only that it is some integer, not which one.

## prediction

- **Compiler:** accepts the program and builds a binary.
- **Output, in order:**
  1. `main done, worker <H> not joined`, where `<H>` is the integer `hero_thread_spawn` returned.
- **Not written:** `worker done: held by a worker 7`.
- **How it ends:** normally, after about 0.2 s, with exit status 0 and no abort message. The worker thread is ended along with the process.

## context

- **Files I read:** only `brief.md`, `task.md` and `heroes-spec.md` in this directory.
- **Other material that reached my context:** the system prompt, which says I'm an agent built on the Claude Agent SDK and gives the working directory and the date 2026-11-19. The user message also contained a list of labels after the instruction (`t1-y-1`, `t1-z-1`, … `t4-h-5`, `verdict`). Those labels told me nothing about the language, and I didn't use them.
- **Anything else:** no other instruction, rule, contract, memory or document.
~~~~~

### `t1-y-4`, now

~~~~~ markdown
I couldn't write `report.md` because I only have a file-reading tool, so the whole report is below, as the brief says to do in that case.

# report.md

## answer

**1. Does `heroes build main.hero` accept the program?** Yes. I found no rule in the specification that it breaks:
- **The C declarations match their headers.** C's `usleep` takes a `useconds_t`, which is a 32-bit unsigned int, and returns an `int`. So `useconds: u32` and `-> i32` are at the header's own width and sign (§13). The two `hero_thread_*` declarations are copied exactly from §13.
- **`busy` can be passed as a function value.** "Top-level functions are values, but not one with an `@` or `lent` parameter" (§9). `busy` has neither. Its type is `(function(i64) -> i64)`, which is what `body` asks for, and it is passed as a parameter, which is where a callback may go (§13). Its one parameter means it doesn't need named parameters in the type (§9).
- **The literals fit their types.** `1000000` and `200000` take the type `u32` from the parameter and fit in it. `7` is an `i64` (§2).
- **The unused results are discarded correctly.** Each `_ = usleep(...)` throws away an `i32`, which is neither `()` nor fallible, so the line is allowed (§5). `print` returns `()` and stands alone, which is correct.
- **Every binding is used.** `n`, `word` and `handle` are all read (§5).
- **`"..." + n.to_str()` is valid.** It joins a `str` with a `str` (§7, §11).
- **The return type matches.** `word.len()` is an `i64`, matching `-> i64`. This is choice point 1.
- **No rule counts against the unused declaration or the unjoined thread.** No rule makes an extern function that is never called (`hero_thread_join`) an error, and none does so for a thread that is never joined.

**2. What the program writes, and how it ends:**
- **Line 1:** `main done, worker <H> not joined`, where `<H>` is the `i64` that `hero_thread_spawn` returned. `print` writes its values with no separator and one trailing newline (§11). The specification never says what number a thread handle is, so the exact digits can't be predicted from it.
- **`worker done: held by a worker 7` is not among the lines.** The worker sleeps for about 2 s, in two `usleep(1000000)` calls, before it prints. `main` sleeps 0.2 s, prints, and returns. §1 says `main` "takes nothing and produces nothing". Nothing in §13 says that returning from `main` waits for threads that are still running. Only `hero_thread_join` "waits for its answer", and the program never calls it. So the process ends with `main`, about 1.8 s before the worker would print.
- **Ending:** the program ends normally, with no abort. The only rule about things left over when `main` returns is §13's "a lease nobody ends, like a handle nobody consumes, aborts when main returns". That rule doesn't apply here. A "handle" in that sense is a group `record` with a `tag` and no fields, whose life an `acquires` mark began. The thread handle is a plain `i64` with no mark. No `exit` call is made, so the exit status is the normal one (0).

Files: the task asks me to write no file. The only file involved is `main.hero`, unchanged, as given in `task.md`.

## choice_points

1. **The type of `len`.** §11 lists `len` but never gives its result type. I took it to be `i64`. Integer literals and `range` default to `i64`, and `repeat` flags its `n` as a `u64` as if that were the exception. If `len` returned a `u64`, `return word.len()` in an `-> i64` function would be refused, because §7 allows no implicit conversions.
2. **What happens to running threads when `main` returns.** The specification doesn't say. I assumed C's behaviour: returning from `main` ends the process and every thread in it, so the worker never prints. There are two other readings:
   - The runtime joins outstanding threads first. The program would then also print `worker done: held by a worker 7` after about 2 s and then end normally.
   - The runtime treats an unjoined thread like an unconsumed handle. The program would then abort after line 1 with a count of live threads.
3. **Whether an extern function that is never called is an error.** §5 makes unused *bindings and parameters* errors, not unused declarations. I took it to be accepted. If it were an error, the compiler would refuse `hero_thread_join`.
4. **Whether `usleep(1000000)` really sleeps.** POSIX lets `usleep` fail with `EINVAL` for values of 1,000,000 or more. I assumed glibc or macOS behaviour, where it sleeps 1 s. On a system that rejects the value, both calls return at once. The worker would then print `worker done: held by a worker 7`, probably before `main` wakes from its 0.2 s sleep. Its order relative to line 1 would come down to thread timing.
5. **The value of the thread handle.** Nothing in the specification fixes it. Any reading gives the same line, with only the printed number differing.

## prediction

- **Compiler:** accepts the program.
- **Output:** exactly one line, `main done, worker <H> not joined`, where `<H>` is an unspecified `i64` handle.
- **The worker's line:** `worker done: held by a worker 7` is not written.
- **Ending:** normally, about 0.2 s after start, with no abort.

## context

- **Files read:** only `brief.md`, `task.md` and `heroes-spec.md` in this directory. I read no other file.
- **Other things in my context:**
  - The agent system prompt, which identifies me as a Claude agent built on the Claude Agent SDK and gives the working-directory path and date.
  - The user's message. After the instruction it listed names (`t1-y-1` … `t4-h-5`, `verdict`), which look like sibling run names. They carried no content, and I didn't use them.
- **Not in my context:** no other rules, contracts, memories or documents.
~~~~~

### `t1-y-5`, now

~~~~~ markdown
I could not write `report.md` because I only have a file-reading tool here. As the brief allows, the whole report follows.

# report.md

## answer

**1. Does `heroes build main.hero` accept the program?** Yes. Every line follows the specification:

- **The `unistd.h` group.** `usleep(useconds: u32) -> i32` matches C's `int usleep(useconds_t)`. `useconds_t` is a 32-bit unsigned integer on macOS and on glibc. §13: "A **parameter** and a **field** are declared at the header's own width and sign … `i32` where C says int". The result `i32` is C's `int`. The literals `1000000` and `200000` take the type `u32` from their context and fit it (§2: "A literal takes the type its context asks for … a literal must fit its type").
- **The `hero_os.h` group.** `hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64` and `hero_thread_join(handle: i64) -> i64` are word for word the signatures §13 gives. A callback is allowed as a parameter (§13: "A callback is a **parameter**"). The groups come in a valid order, because neither needs the other's names.
- **Passing `busy` as a value.** `busy` takes one plain `i64` and returns `i64`, so it is a function value (§9: "Top-level functions are values, but not one with an `@` or `lent` parameter"). Its type is `(function(i64) -> i64)`. Parameter names only enter a function type when two parameters share a type (§9), and `busy` has one parameter.
- **Discarding `usleep`'s result.** `usleep` returns an `i32`, which is not fallible. `_ = usleep(...)` discards it on purpose, which §5 allows.
- **`print`.** Every `print` call stands alone as a `()` line. Each one takes only `str` or number values (§11).
- **`busy`'s body.** `word` and `n` are both read. `return word.len()` returns an `i64`, so the function returns on every path (§8).
- **`main`'s body.** `handle` is read by `print`. The string literals contain no braces, so the §2 rule against unmarked holes does not apply.
- **`hero_thread_join`.** It is declared but never called. §5 makes only unused *bindings and parameters* compile errors, not unused declarations.

**2. What the program writes, and how it ends.**

The program writes exactly one line:

```
main done, worker <H> not joined
```

`<H>` is the `i64` that `hero_thread_spawn` returned. The specification does not say what value that is. `print` writes its values with no separator and one trailing newline (§11).

**`worker done: held by a worker 7` is NOT among the lines written.** The timing explains why:

1. `main` starts the worker. The worker builds its string and then sleeps 1 s, then another 1 s.
2. `main` sleeps only 0.2 s, prints its line and returns.
3. The worker is still about 1.8 s away from its `print` when `main` returns.

Nothing in the program joins the worker. §13 says only that `hero_thread_join` "waits for its answer". Nothing in the specification says that `main`'s return waits for threads. `main`'s return ends the process, and the sleeping worker dies with it.

**The program ends normally**, about 0.2 s after starting, with exit status 0. It does not abort:

- The abort when `main` returns covers only "a lease nobody ends, like a handle nobody consumes" (§13).
- A handle in that sense is a group's `record` with a `tag` and no fields, whose life an `acquires` mark began.
- The thread handle here is a plain `i64` with no mark, so no ownership is tracked and nothing is owed at exit.

No file is changed. The folder still holds only `main.hero`, plus whatever the build produced.

## choice_points

1. **What happens at `main`'s return while a thread is still running.**
   - The specification says nothing about this.
   - **My choice:** the process ends and the worker is killed, as when C's `main` returns. Result: one line, normal end.
   - **The alternative:** if the runtime joined outstanding threads, the program would also print `worker done: held by a worker 7` after about 2 s, then end normally. That is a different answer from the one I predict. If the runtime instead aborted on unjoined threads, the program would print the main line and then abort. The specification gives no basis for that.
2. **Whether the `i64` thread handle counts as a "handle nobody consumes".**
   - **My choice:** no. The specification defines handles as tagged, fieldless records with lives begun by `acquires`.
   - **The alternative:** the program would print the main line and then abort at `main`'s return, saying one handle was not consumed.
3. **Whether an unused `extern` declaration (`hero_thread_join`) is a compile error.**
   - **My choice:** no. §5 covers only bindings and parameters.
   - **The alternative:** the compiler refuses the program.
4. **Whether the extern parameter `handle` clashes with `main`'s binding `handle`.**
   - **My choice:** no. A parameter's scope is its own declaration, so neither name is in scope where the other is.
   - **The alternative:** the compiler refuses the program for shadowing (§5).
5. **The width of `useconds_t`.**
   - **My choice:** `u32`, which is true on macOS and glibc.
   - **The alternative:** on a platform where it has another width, the compiler refuses the parameter (§13).
6. **The value of the handle.** The specification does not define it, so I left it as `<H>`. A different value would change only the digits in the one line.

## prediction

- **Compiler:** accepts the program.
- **Lines written, in order:**
  1. `main done, worker <H> not joined`, where `<H>` is the `i64` returned by `hero_thread_spawn`.
- `worker done: held by a worker 7` is not written.
- **How it ends:** normally, after about 0.2 s, with exit status 0. The worker thread is ended by the process exit while it is still in its first `usleep`.

## context

I read only `brief.md`, `task.md` and `heroes-spec.md` in this directory. The only other things in my context were:

- the system prompt, which says I am an agent built on the Claude Agent SDK and gives the working directory and date;
- the user's message, which includes a list of names (`t1-y-1` … `t4-h-5`, `verdict`) whose meaning is not stated and which I did not use.

No other instruction, rule, contract, memory or document reached my context.
~~~~~

### `t1-z-1`, q2-r

~~~~~ markdown
I couldn't write `report.md` because my only tool reads files and cannot create them. Following the brief's fallback, the whole report is below.

# report.md

## answer

**Files.** The task gives one file and asks for no new ones. Here it is whole and unchanged.

`main.hero`:
```
extern "unistd.h"
    function usleep(useconds: u32) -> i32

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

function busy(n: i64) -> i64
    word = "held by a worker " + n.to_str()
    _ = usleep(useconds: 1000000)
    _ = usleep(useconds: 1000000)
    print("worker done: ", word)
    return word.len()

function main()
    handle = hero_thread_spawn(body: busy, arg: 7)
    _ = usleep(useconds: 200000)
    print("main done, worker ", handle, " not joined")
```

**1. Does `heroes build main.hero` accept the program?** Yes.
- **The two `extern` groups are valid.**
  - The thread functions are declared exactly as §13 gives them: `hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64` and `hero_thread_join(handle: i64) -> i64`.
  - `usleep` takes a `u32`, which is C's `useconds_t` (unsigned int). A parameter must match "the header's own width and sign" (§13), and it does.
  - The `i32` result matches C's `int`.
  - Neither header needs the other's names, so the order of the groups doesn't matter (§13).
  - The program never calls the declared `hero_thread_join`. Nothing in the spec makes an unused extern declaration an error; the unused-binding rule (§5) covers bindings and parameters.
- **`busy` can be passed as a value.** It is a top-level function with no `@` or `lent` parameter (§9), and its type `(function(i64) -> i64)` is the one `body` asks for.
- **Labels and literals are fine.** Labels on arguments are allowed even where they aren't required (`Arg = [ ident ":" ] ...`, §9), and every label here matches its parameter name.
  - `7` is an `i64` (§2).
  - `1000000` and `200000` take the type `u32` from context and fit it (§2).
- **Results are discarded correctly.** Each `_ = usleep(...)` throws away an `i32`, which is neither `()` nor fallible, so `_` may drop it (§5).
- **Every binding is used.** `word` is read twice, `handle` is printed, and `n` is read (§5).
- **`print` gets only printable values.** It receives `str` and `i64` values, both types it renders (§11).
- **`busy` returns on its only path (§8).** `word.len()` is treated as an `i64` (see choice points).

**2. What the program writes, and how it ends.**

The program writes one line:

1. `main done, worker <H> not joined`

`<H>` is the handle that `hero_thread_spawn` returned. The spec doesn't say what value a handle has, so I can't predict the number.

The line `worker done: held by a worker 7` is **not** among the output:
- Main spawns the worker, sleeps 0.2 s, prints its line, and returns.
- The worker would only reach its `print` after about 2 s of `usleep`.
- §13 says "a thread nobody joins, aborts when `main` returns, saying how many." The program never calls `hero_thread_join`, so the program aborts as soon as `main` returns.
- §6 says "An abort ends the program at once, saying why", so the worker never reaches its `print`.

**How it ends:** by an abort, about 0.2 s after starting. The abort reports one thread that was never joined (count = 1). The spec doesn't give the exact wording, only that the abort says how many.

## choice_points

1. **The type of `word.len()`.** The spec never states what `len` returns; examples like `xs.len() == 0` only suggest `i64`.
   - My choice: `i64`, so `return word.len()` matches `-> i64`.
   - The other reading: if `len` returned `u64`, the compiler would refuse `return word.len()` because there are no implicit conversions (§3). The author would then need `word.len().to_i64().must()`.
2. **`usleep`'s parameter type.** I read `useconds_t` as `u32`, which holds on Linux/glibc and macOS.
   - The other reading: on a platform where `useconds_t` has a different width or sign, clang would refuse the parameter (§13), and the compiler would refuse the program.
3. **Whether `usleep(1000000)` actually sleeps.** Old POSIX allowed `usleep` to fail with EINVAL for values of 1,000,000 or more. I assume glibc or macOS, where it sleeps the full second.
   - The other reading: if each call failed with -1 at once, the worker could finish before main's 0.2 s sleep ends. The output would then be `worker done: held by a worker 7` followed by the main line, and the program would still abort when `main` returns, because the worker was never joined.
4. **When the unjoined-thread rule applies.** §13 places it "when `main` returns", which is at run time, so I read it as an abort and not a compile error.
   - The other reading: if the compiler checked joins statically, it would refuse the program and no binary would be built.
5. **I made no edits.** The task asks for a prediction, not a fix. The fix the author probably meant is `_ = hero_thread_join(handle: handle)` before the `print` in `main`. That version would print the worker's line after about 2 s, then the main line, and end normally.

## prediction

- **Compiler:** accepts the program.
- **Lines written, in order:**
  1. `main done, worker <H> not joined` (`<H>` is an unspecified `i64` handle)
- **Ending:** an abort when `main` returns, saying one thread was never joined. The worker's line is never printed.

## context

- My context held the Claude Agent SDK system prompt, which names the working directory and the date.
- The user's message also contained a list of labels after the instruction (`t1-y-1` … `t4-h-5`, `verdict`). The labels carry no content, and I did not use them.
- I read only `brief.md`, `task.md` and `heroes-spec.md` in this directory. No other instruction, rule, contract, memory or document reached my context.
~~~~~

### `t1-z-2`, q2-r

~~~~~ markdown
I couldn't write `report.md` because I only have a file-reading tool, so the brief tells me to give the whole report here.

# report.md

## answer

**Files (unchanged from the task).** The task asks for a prediction, not new code. The only file is the author's own:

`main.hero`
```
extern "unistd.h"
    function usleep(useconds: u32) -> i32

extern "hero_os.h"
    function hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64
    function hero_thread_join(handle: i64) -> i64

function busy(n: i64) -> i64
    word = "held by a worker " + n.to_str()
    _ = usleep(useconds: 1000000)
    _ = usleep(useconds: 1000000)
    print("worker done: ", word)
    return word.len()

function main()
    handle = hero_thread_spawn(body: busy, arg: 7)
    _ = usleep(useconds: 200000)
    print("main done, worker ", handle, " not joined")
```

**1. Does `heroes build main.hero` accept the program?** Yes. Nothing in it breaks a rule of the specification:
- **The `usleep` group:** §13 says a parameter is declared "at the header's own width and sign". C's `useconds_t` is an unsigned 32-bit int on Linux and macOS, so `u32` matches. The `int` result as `i32` passes clang's check. The literals `1000000` and `200000` take the type `u32` from their context and fit in it (§2).
- **The `hero_os.h` group:** its declarations copy the exact signatures §13 gives for `hero_thread_spawn` and `hero_thread_join`. `hero_thread_join` is declared and never called, but §5 only makes an unused binding or parameter an error, not an unused extern declaration.
- **`busy` passed as a value:** `busy` is a top-level function with no `@` or `lent` parameter, so it may be used as a value (§9). Its type `(function(i64) -> i64)` matches `body`.
- **Discarded results:** `_ = usleep(...)` drops an `i32`, which is not a `T?` and not `()`, so the discard is allowed (§5).
- **Bindings:** `word` and `handle` are both read, and `n` is used. There is no shadowing and no unused binding (§5).
- **`print`:** it gets `str` and `i64` values, which are types it accepts (§11).
- **`return word.len()`:** this assumes `len` gives an `i64` (see choice_points).
- **Not joining the thread:** §13 makes this a runtime abort when `main` returns, not a compile error.

**2. What the program writes and how it ends.**
1. `main done, worker <H> not joined`

   `<H>` is the decimal `i64` that `hero_thread_spawn` returned. The specification does not say what value that is.

That is the only line. The line `worker done: held by a worker 7` is **not** among them.

Why:
- The worker sleeps twice for 1 second, so about 2 s, before it prints.
- `main` sleeps 0.2 s, prints its line, and returns without calling `hero_thread_join`.
- §13: "a lease nobody ends, like a handle nobody consumes or a thread nobody joins, aborts when `main` returns, saying how many."
- §6: "An abort ends the program at once, saying why". The worker is still asleep at that point, so it never reaches its `print`.

**How it ends:** by an abort, about 0.2 s after starting, when `main` returns. The abort reports one thread that was never joined. The specification does not give the exact wording, only that the abort says how many (1). The process does not end normally.

## choice_points

1. **What type `len` returns.** The specification never says. I took it to be `i64`, the default integer type. If it were `u64`, `return word.len()` in a `-> i64` function would be a mismatch with no implicit conversion (§3), and the compiler would refuse the program.
2. **Whether `usleep(1000000)` really sleeps.** POSIX allows `usleep` to fail with EINVAL when the value is 1,000,000 or more. I assumed glibc or macOS behaviour, where it sleeps 1 s. On a libc that fails at once, the worker would return straight away and print `worker done: held by a worker 7` before `main`'s 0.2 s sleep ends. The program would still abort on return for the unjoined thread, but it would write two lines, with the worker's line first.
3. **Whether the abort is caught at compile time or at run time.** §13 words it as "aborts when `main` returns", so I treated it as runtime and the build is accepted. If it were read as something the compiler checks, the build would be refused.
4. **Whether output printed before an abort survives.** I assumed `print` output written before the abort appears, since `print` writes a whole line with its newline. If the output stayed in a buffer and was lost, nothing would be printed and the program would show only the abort message.
5. **The handle's value.** The specification does not say, so it is left as `<H>`.

## prediction

- **Compiler:** accepts the program.
- **Output, in order:** `main done, worker <H> not joined`, then nothing else. In particular, there is no `worker done: held by a worker 7`.
- **Ending:** an abort when `main` returns, reporting 1 thread never joined (§13), which ends the process at once (§6). It does not end normally, and it does not hang.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md`, two other things reached my context:
- My system prompt, which says I am a Claude agent built on the Claude Agent SDK and gives the working-directory path and the date.
- A list of labels in your message (`t1-y-1` … `t4-h-5`, `verdict`), with no instructions attached.

No other rule, contract, memory or document reached my context, and I read no other file.
~~~~~

### `t1-z-3`, q2-r

~~~~~ markdown
I can't write `report.md`: the only tool I have reads files. As the brief allows, the whole report is below.

# report.md

## answer

**1. Does `heroes build main.hero` accept the program?**

Yes. Each part checks against the specification:

- **`extern "unistd.h"` / `usleep(useconds: u32) -> i32`:**
  - §13 says a parameter must be declared at the header's own width and sign. C declares `int usleep(useconds_t)`, and `useconds_t` is a 32-bit unsigned integer on both macOS and glibc, so `u32` matches.
  - The result `i32` matches `int`, and §13 also allows a result wider than C's.
  - Nothing in the specification ties the Heroes parameter name to C's.
- **`extern "hero_os.h"`:**
  - The `hero_thread_spawn` and `hero_thread_join` signatures are copied word for word from §13.
  - `hero_thread_join` is declared but never called. §5 makes an unused *binding or parameter* a compile error. An extern declaration is neither, so it is not refused.
- **`busy`:**
  - `"held by a worker " + n.to_str()` adds a `str` to a `str` (§7, §11). The literal has no braces, so it has no `f` problem (§2).
  - `_ = usleep(useconds: 1000000)` throws away an `i32`, which is not a `T?`, so §5 allows the `_ =` line. The literal takes the type `u32` (§2) and fits in it.
  - `print("worker done: ", word)` prints two `str` values (§11).
  - `return word.len()` returns the length, which I take to be `i64` to match `-> i64`.
  - The parameter `n` and the binding `word` are both used.
- **`main`:**
  - `busy` takes no `@` or `lent` parameter, so §9 lets it be used as a value, and its type is exactly `(function(i64) -> i64)`.
  - The named arguments `body:` and `arg:` are allowed.
  - `7` takes the type `i64`, and `200000` takes `u32` and fits.
  - `handle` is used, and it is an `i64`, which `print` accepts (§11).
  - The specification describes no compile-time check for a thread that is never joined. It names only a run-time abort when `main` returns (§13).

**2. The lines the program writes, and how it ends**

1. `main done, worker <H> not joined`. `<H>` is the decimal `i64` handle that `hero_thread_spawn` returned. The specification does not say what that value is (§13), so I cannot give the number.
2. Then the program aborts. §13 says: "a lease nobody ends, like a handle nobody consumes or a thread nobody joins, aborts when `main` returns, saying how many."
   - The abort says one thread was never joined.
   - The specification does not give the exact wording.

**Is `worker done: held by a worker 7` among the lines?** No.

- The worker sleeps 1 s twice, so it reaches its `print` after about 2 s.
- `main` sleeps 0.2 s, prints, and returns at about 0.2 s.
- On that return the program aborts, and §6 says an abort "ends the program at once". The worker never prints.

**How it ends:** by an abort reporting 1 unjoined thread, not normally. The output line shown above is written to standard output before the abort.

There is no program file to write. The task asks only for a prediction about the given `main.hero`.

## choice_points

1. **Type of `len()`.** §11 lists `len` but gives no result type. I chose `i64`, which is how the language types an integer when nothing else decides it.
   - If it were another width, `return word.len()` would not match `-> i64` and the compiler would refuse the program (§3: no implicit conversions).
2. **An unused extern function.** I chose that declaring `hero_thread_join` without calling it is not an error, because §5 speaks only of bindings and parameters.
   - If the rule covered unused declarations, the compiler would refuse the program.
3. **Whether `useconds_t` is `u32`.** I chose yes, as it is on the macOS and glibc headers.
   - On a platform where it differs, §13 says clang refuses the parameter, so the compiler would refuse the program.
4. **When the unjoined thread is caught.** I chose run time, when `main` returns, because §13 says so.
   - If the compiler checked this statically, it would refuse the program.
5. **Does the abort wait for the worker?** I chose no: "ends the program at once" (§6).
   - If the abort waited, or if the program waited for running threads at exit, `worker done: held by a worker 7` would print first, at about 2 s.
6. **Whether the `print` reaches the output before the abort.** I chose yes: `print` writes the line (§11).
   - If the abort dropped buffered standard output, there would be no output lines at all.
7. **The file's indentation.** I took the task's 4-space block indent as markdown formatting, so the file's top-level lines start at column 0.
   - Taken literally, every top-level line would be indented, and the compiler would refuse the program (§1).
8. **Handle value and abort wording.** Both are unspecified. I left them as `<H>` and "says 1 thread not joined".

## prediction

- **Compiler:** accepts the program.
- **Output:**
  1. `main done, worker <H> not joined`
- **Ending:** an abort when `main` returns, saying one thread was never joined. The line `worker done: held by a worker 7` is not written.

## context

- **Files read:** only `brief.md`, `task.md` and `heroes-spec.md` from this directory. No other language document, rule, contract or memory was read.
- **Other context that reached me:**
  - The agent system prompt, which says I am a Claude agent built on the Claude Agent SDK, and gives the working directory and date.
  - The user message, which besides the instruction listed labels: t1-y-1 … t4-h-5 and "verdict".
- None of this carried information about the language.
~~~~~

### `t1-z-4`, q2-r

~~~~~ markdown
I couldn't write `report.md` because I have no file-writing tool. As the brief says to do in that case, the whole report is below.

# report.md

## answer

**1. Does `heroes build main.hero` accept the program?** Yes. Each part checks out against the specification:

- **`usleep` declaration.** `usleep(useconds: u32) -> i32` is declared at the C header's width and sign. In `unistd.h`, `usleep` takes a `useconds_t`, which is a 32-bit unsigned type, and returns `int`. §13 requires this: "A parameter … declared at the header's own width and sign … `i32` where C says int", and "clang checks every result type".
- **Thread functions.** `hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64` and `hero_thread_join(handle: i64) -> i64` match the signatures §13 gives for `hero_os.h` word for word. The two `hero_thread_spawn` parameters have different types, so §9's named-argument rule doesn't come into play. Naming them anyway is allowed (`Arg = [ ident ":" ] …`).
- **The literals.** In `usleep(useconds: 1000000)` and `usleep(useconds: 200000)`, each literal takes the type its context asks for, `u32`, and fits it (§2).
- **`_ =` lines.** `_ = usleep(...)` discards an `i32` on purpose. It is neither `()` nor a `T?`, so §5 allows it.
- **`busy` as a value.** `busy` is a top-level function with no `@` or `lent` parameter, so it is a value (§9). Its type `(function(i64) -> i64)` matches `body`. It has only one parameter, so there is no same-type pair whose names would have to match the function type (§9).
- **Inside `busy`.** `"held by a worker " + n.to_str()` is `str + str`, which is concatenation (§7, §11; `to_str` cannot fail). `word` and `n` are both used. `word.len()` is returned as `i64` (see choice points). `print("worker done: ", word)` prints only `str` values (§11). A `return` ends the only path (§8).
- **Inside `main`.** `handle` is an `i64` and is used in `print`, which takes numbers and `str` (§11). `main` takes nothing and returns nothing (§1).
- **Unjoined thread.** §13 makes a thread nobody joins a runtime abort "when `main` returns", not a compile error. The thread handle is a plain `i64`, not a handle record, so no compile-time handle rule applies.

**2. Lines written, and how the program ends.**

1. `main done, worker <H> not joined`. `<H>` is the `i64` that `hero_thread_spawn` returned. The specification doesn't say what number that is. The line comes after main's 0.2 s sleep. `print` puts no separator between its values and adds one trailing newline (§11).
2. Then `main` returns, and the program **aborts**. §13 says: "a lease nobody ends, like a handle nobody consumes or a thread nobody joins, aborts when `main` returns, saying how many." The abort message reports one unjoined thread. The specification doesn't give its exact wording, only that it says how many. §6: "An abort ends the program at once, saying why."

**`worker done: held by a worker 7` is NOT among the lines.** The worker sleeps two seconds in total (two `usleep(1000000)` calls) before it prints. Main returns after about 0.2 s, and the abort ends the whole process at once (§6), worker thread included. So the worker never reaches its `print`.

No files to write: the task asks for a prediction, not a new program.

## choice_points

- **What `len()` returns.** The specification never states `len`'s result type. I took it as `i64`, which fits the literal default and the `xs.len() == 0` example. If it were `u64` (as `repeat`'s `n` is), `return word.len()` in a `-> i64` function would be refused at compile time (§3: no implicit conversions).
- **Declared but never called: `hero_thread_join`.** §5 makes an unused *binding or parameter* an error, not an unused extern declaration. I took it as accepted. The other reading means the compiler refuses the program.
- **When the unjoined thread is caught.** I followed §13 literally: a runtime abort when `main` returns. If it were a compile-time check, the program would be refused. If it weren't checked at all, the program would end normally after one line, with no worker output, because process exit kills the thread.
- **Whether the abort waits for the worker.** I followed §6, "ends the program at once", so the worker is killed and "worker done" never appears. A reading where the abort waits for, or joins, live threads would give "worker done: held by a worker 7" at about 2 s, and then the abort.
- **`usleep` with one million microseconds.** This depends on the platform, not the specification. POSIX lets `usleep` refuse values of 1,000,000 or more with EINVAL. On glibc and macOS it sleeps the full second, which is what I predicted. On a platform that refuses it, both worker sleeps return −1 immediately. The worker would then print `worker done: held by a worker 7` *before* main's line, and the program would still abort when `main` returns.
- **The value of `<H>`.** The specification doesn't fix it, so I can't predict the exact text of line 1.

## prediction

- **Compiler:** accepts the program and produces a binary.
- **Lines, in order:**
  1. `main done, worker <H> not joined`, where `<H>` is an unspecified `i64` thread handle.
- **How it ends:** by an **abort** when `main` returns, saying that one thread was never joined (exact wording unspecified). It ends at about 0.2 s. `worker done: held by a worker 7` is never written.

## context

Apart from `brief.md`, `task.md` and `heroes-spec.md`, three things reached my context:

- **System prompt:** a line saying I am an agent built on the Claude Agent SDK, with the working directory and date.
- **The request itself:** the user's message, which also listed labels (`t1-y-1` … `t4-h-5`, `verdict`) that I didn't use.
- **Tool descriptions:** the description of the Read tool.

No other rule, contract, memory or document reached me, and I read no other file.
~~~~~

### `t1-z-5`, q2-r

~~~~~ markdown
I only have a file-reading tool, so I could not write `report.md`. As the brief says to do in that case, the whole report follows.

# report.md

## answer

**Question 1. Does `heroes build main.hero` accept the program?** Yes. Each part matches the specification:

- **The `usleep` declaration.** The `unistd.h` group declares `usleep(useconds: u32) -> i32`. In C this is `int usleep(useconds_t)`. `useconds_t` is a 32-bit unsigned integer on glibc and on Darwin, so the parameter is "declared at the header's own width and sign". The result `i32` matches `int` (§13).
- **The thread functions.** The `hero_os.h` group declares `hero_thread_spawn` and `hero_thread_join` exactly as §13 writes them.
- **Declaring `hero_thread_join` without calling it.** No rule refuses this. The "unused" rule in §5 covers bindings, parameters, cells and `@` parameters, not extern declarations.
- **Passing `busy` as a value.** `busy` is a top-level function with no `@` or `lent` parameter, so it is a value (§9). Its type `(function(i64) -> i64)` is the one the `body` parameter asks for.
- **Labelled arguments.** `body:`, `arg:` and `useconds:` are optional labels, which the grammar allows (`Arg = [ ident ":" ] …`, §9).
- **Literals.** `7` is an `i64`. `1000000` and `200000` take the type their context asks for, `u32`, and they fit (§2).
- **Discarding results.** `_ = usleep(...)` throws away an `i32`, which is neither `()` nor fallible, so it is allowed (§5).
- **Every binding is used.** `word` is read twice and `n` once. `handle` is read by `print`, which accepts an `i64` (§11).
- **`word.len()`** is UFCS on the built-in `len`, and it returns the `i64` that `busy` promises.
- **String literals.** None of them contains braces or escapes (§2).
- **Joining is not checked at compile time.** The specification makes a thread nobody joins a run-time abort when `main` returns, not a compile error (§13: "a lease nobody ends, like a handle nobody consumes or a thread nobody joins, aborts when `main` returns, saying how many").

**Question 2. What does the run write, and how does it end?**

1. At time 0, `main` starts the worker (§13). The worker builds `word = "held by a worker 7"` and goes into its first one-second `usleep`.
2. `main` sleeps for 0.2 s.
3. At about 0.2 s, `main` writes one line: `main done, worker <handle> not joined`. Here `<handle>` is the `i64` that `hero_thread_spawn` returned, printed as a decimal number (§11: `print` writes its values with no separator and exactly one trailing newline). The specification does not say what number a handle is, so the exact digits cannot be predicted.
4. `main` reaches its end and returns. The one thread nobody joined makes the program abort at that moment, saying how many threads were left unjoined: one (§13). An abort "ends the program at once, saying why" (§6). The specification does not give the exact wording, so I expect something like "1 thread not joined".
5. When this happens the worker is about 0.2 s into a two-second sleep. The process ends before the worker wakes up.

**The line `worker done: held by a worker 7` is NOT among the lines written.** The program ends by an abort, not normally, and the abort reports one unjoined thread.

No other files exist. `main.hero` is used as given:

```
main.hero  (unchanged, as given in task.md)
```

## choice_points

- **`useconds_t` taken as a 32-bit unsigned integer.** It is on glibc and on Darwin, so `u32` is accepted. If a platform made it 64 bits, the parameter would disagree with the header and the compiler would refuse it (§13).
- **`usleep` assumed to be declared by `unistd.h` under the compiler's default feature macros.** If the build used a strict mode with no default source, `usleep` would be undeclared and clang would refuse the program.
- **`usleep(1000000)` assumed to sleep a full second.** glibc and macOS do this. Old POSIX allows `EINVAL` for one million or more. If both calls failed at once, the worker would finish within microseconds and print `worker done: held by a worker 7` before `main`'s line. The program would still abort at the end of `main`, because the worker was never joined.
- **The unjoined thread read as a run-time abort, not a compile error,** because §13 says so explicitly. A compiler that checked this statically would refuse the program instead.
- **The abort read as immediate.** It does not wait for the worker, because "ends the program at once" (§6). If the runtime waited for running threads, the worker's line would appear at about 2 s and the program would then exit, normally or by the abort depending on when the check runs.
- **Unused extern declaration accepted.** I took the unused `hero_thread_join` declaration as allowed. If it counted as an unused binding, the compiler would refuse the program.
- **`len` taken to return `i64`.** If it returned some other width, `return word.len()` would be refused, because the specification has no implicit conversions (§3).
- **Where the abort message goes.** I assumed it goes to stderr and is not a line the program prints itself. The specification only says the abort is "saying why".

## prediction

- **Compiler:** accepts the program.
- **Output, in order:** one line, `main done, worker <handle> not joined`, where `<handle>` is the number `hero_thread_spawn` returned. The specification leaves that number unspecified.
- **The worker's line:** `worker done: held by a worker 7` is not written.
- **How it ends:** by an abort about 0.2 s after starting, when `main` returns, saying that 1 thread was not joined (exact wording unspecified).

## context

Besides the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), my context held:
- the general system instructions that come with my tools and agent setup, including the working directory and the date;
- a list of directory-like tokens in your message (`t1-y-1` … `t4-h-5`, `verdict`), which I did not use.

No other rule, memory or document about Heroes reached my context. The facts about `useconds_t` and `usleep` come from my general knowledge of C, not from these files.
~~~~~

### `t2-f-1`, proposal-b

~~~~~ markdown
I only have a Read tool, so I couldn't create `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

**`main.hero`**

```
record Doc
    name: str
    text: str

record Job
    doc: Doc
    stop: [str]

record Count
    name: str
    words: i64
    distinct: i64

function split_words(text: str) -> [str]
    parts: [str] @= []
    current @= ""
    for c in text.chars()
        if c == " "
            parts @ parts.push(current)
            current @ ""
        else
            current @ current + c
    parts @ parts.push(current)
    return parts

function is_stop(word: str, stop: [str]) -> bool
    for s in stop
        if s == word
            return true
    return false

function count_doc(job: Job) -> Count
    total @= 0
    distinct @= 0
    seen: {str: bool} @= {}
    for w in split_words(job.doc.text)
        if !is_stop(w, job.stop)
            total @ total + 1
            if seen[w].is_err()
                distinct @ distinct + 1
                seen[w] @ true
    return Count(name: job.doc.name, words: total, distinct: distinct)

function main()
    stop = ["the", "a", "to"]
    doc_a = Doc(name: "a", text: "the cat sat on the mat")
    doc_b = Doc(name: "b", text: "a dog and a cat")
    doc_c = Doc(name: "c", text: "to be or not to be")
    doc_d = Doc(name: "d", text: "x")
    t_a = spawn(count_doc, Job(doc: doc_a, stop: stop))
    t_b = spawn(count_doc, Job(doc: doc_b, stop: stop))
    t_c = spawn(count_doc, Job(doc: doc_c, stop: stop))
    t_d = spawn(count_doc, Job(doc: doc_d, stop: stop))
    r_a = wait(t_a)
    r_b = wait(t_b)
    r_c = wait(t_c)
    r_d = wait(t_d)
    print(f"{r_a.name}: {r_a.words} words, {r_a.distinct} distinct")
    print(f"{r_b.name}: {r_b.words} words, {r_b.distinct} distinct")
    print(f"{r_c.name}: {r_c.words} words, {r_c.distinct} distinct")
    print(f"{r_d.name}: {r_d.words} words, {r_d.distinct} distinct")
```

**Does `heroes build main.hero` accept it?** I predict yes.

**Lines written:**
```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```

**How it ends:** normally. All four threads are waited for, so the "thread nobody waits for" abort when `main` returns doesn't happen.

## choice_points

1. **Passing two things to a thread.** `spawn(f, x)` takes exactly one argument, so I bundle the document and the stop list into a `Job` record. Passing two arguments, as in `spawn(count_doc, doc, stop)`, doesn't match the one form the spec gives, and I expect the compiler to refuse it.
2. **How to call `wait`.** I wrote `wait(t_a)` on an immutable binding, reading "its wait consumes" as something `wait` does on its own. The other choice is `wait(@t_a)`, which needs `t_a @= spawn(...)`. If the compiler wanted `@`, it would refuse my plain call. If I wrote `@` and it wasn't wanted, it would refuse that instead. The spec never gives `wait`'s signature, so this is the main risk.
3. **Named handles instead of an array of handles.** I bound four handles by name rather than pushing them into a `[(thread Count)]` and looping over it. Whether copying a handle into an array and out again in a `for` is allowed isn't clear. It might be refused. Or the copy left in the array might count as never waited for, so the program aborts when `main` returns with a "threads not waited for" count.
4. **Splitting words.** There's no split built-in, so I build each word from `chars()` with `+`. The other way is byte indexing with `s[i] == ' '` and `slice(from:, to:)`. It should work too, but it depends on the character literal taking the type `u8` and on `slice` working on a `str`, which the spec only implies.
5. **Counting distinct words.** I keep a counter and test `seen[w].is_err()`, rather than using `keys(seen).len()`. The spec never says what type `len` returns. If it's `u64`, putting it in the `i64` field `distinct` would be a refused mixed-width program.
6. **Checking the stop list.** I use a loop in `is_stop`. Using `any`/`find` would need a closure over `w`, and the language has no anonymous functions or closures, so that version wouldn't compile.
7. **Names.** I avoided local names that match a function or built-in (`count`, `keys`, `len`), and avoided `words` as a local in `count_doc` (I used `total`). Shadowing is a compile error, and the spec doesn't say whether a local can share a top-level function's name. Reusing such a name might be refused.
8. **Named arguments.** I used them everywhere. They're required for `Doc(name:, text:)` and `Count(words:, distinct:)`, where two fields share a type. Leaving them out there would be refused.
9. **Grammar of `1 words`.** I kept the task's exact format, `<words> words`, so `d` prints `1 words`. Writing "word" for one would not match the format the task asks for.

## prediction

- **Compiler:** accepts (with the uncertainty about `wait` in choice point 2).
- **Output:**
  - `a: 4 words, 4 distinct` (cat, sat, on, mat)
  - `b: 3 words, 3 distinct` (dog, and, cat)
  - `c: 4 words, 3 distinct` (be, or, not, be)
  - `d: 1 words, 1 distinct` (x)
- **End:** normal exit, no abort.

## context

- **What reached me besides this directory:** the agent harness's system prompt (it names the Claude Agent SDK and gives the working-directory path and date) and the user's instruction. That instruction also carried a list of labels (`t1-y-1`, `t1-z-1`, … `t4-h-5`, `verdict`), which look like names of other experiment runs. None of these affected the program.
- **What I didn't read:** no other file, rule, memory or document.
- **Files read:** only `brief.md`, `task.md` and `heroes-spec.md` in this directory.
~~~~~

### `t2-f-2`, proposal-b

~~~~~ markdown
I couldn't create `report.md` because I only have a tool for reading files, not one for writing them. As the brief allows, the whole report is below.

# report.md

## answer

**Question 1 — `main.hero`, whole:**

```
# A document: its name and its text.
record Doc
    name: str
    text: str

# What one thread is handed: a document and the words to leave out.
record Job
    doc: Doc
    skip: [str]

# What one thread gives back.
record Count
    name: str
    words: i64
    distinct: i64

# The words of a text, separated by single spaces.
function split_words(text: str) -> [str]
    words: [str] @= []
    current @= ""
    for ch in text.chars()
        if ch == " "
            words @ words.push(current)
            current @ ""
        else
            current @ current + ch
    words @ words.push(current)
    return words

# Whether a word is one of the words to leave out.
function is_skipped(word: str, skip: [str]) -> bool
    for s in skip
        if s == word
            return true
    return false

# Counts one document's kept words and its distinct kept words.
function count_doc(job: Job) -> Count
    seen: {str: bool} @= {}
    n @= 0
    distinct @= 0
    for word in split_words(job.doc.text)
        if !is_skipped(word: word, skip: job.skip)
            n @ n + 1
            if seen[word].is_err()
                distinct @ distinct + 1
                seen[word] @ true
    return Count(name: job.doc.name, words: n, distinct: distinct)

# Prints one result line.
function show(r: Count)
    print(f"{r.name}: {r.words} words, {r.distinct} distinct")

function main()
    skip = ["the", "a", "to"]
    a = Doc(name: "a", text: "the cat sat on the mat")
    b = Doc(name: "b", text: "a dog and a cat")
    c = Doc(name: "c", text: "to be or not to be")
    d = Doc(name: "d", text: "x")
    ta = spawn(count_doc, Job(doc: a, skip: skip))
    tb = spawn(count_doc, Job(doc: b, skip: skip))
    tc = spawn(count_doc, Job(doc: c, skip: skip))
    td = spawn(count_doc, Job(doc: d, skip: skip))
    ra = wait(ta)
    rb = wait(tb)
    rc = wait(tc)
    rd = wait(td)
    show(ra)
    show(rb)
    show(rc)
    show(rd)
```

**Question 2 — does `heroes build main.hero` accept it?** Yes, I predict it does.

**Question 3 — every line it writes:**
```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```

**Question 4 — how it ends:** Normally. All four threads are waited for, and there are no leases or handles left open.

## choice_points

1. **One argument to `spawn`.** `spawn(f, x)` runs `f(x)`, so the function gets exactly one argument. I wrapped the document and the words to leave out in a `Job` record. The other choice was a two-argument thread function, such as `spawn(count_doc, doc, skip)`. The compiler would refuse it because the arity doesn't match.
2. **Where the thread handles live.** I kept them in four separate `=` bindings, each waited for exactly once. The other choice was to put them in a `[(thread Count)]` array and wait inside a loop. Handles may only cross "into spawn or a send", and a wait consumes its handle, so copying a handle into an array or loop variable risks being refused. If it were accepted, it might end with an abort when `main` returns, reporting threads nobody waited for.
3. **Order of starting and waiting.** All four `spawn` calls come before any `wait`, as the task requires. Waiting right after each spawn would give the same output but break the task's rule that all threads start first.
4. **Splitting words.** The spec has no split built-in. I walk `text.chars()`, which I read as giving a `[str]` of one-character strings, and compare each with `" "`. The other choice was indexing bytes with `s[i]` (a `u8`), comparing with `' '` and slicing. That works too, but needs `u8`/`i64` index typing, and mixing them would be refused.
5. **Saving the last word.** After the loop I push `current` once more. Leaving that out still compiles and runs, but gives wrong answers: `a: 3 words, 3 distinct`, `b: 2 words, 2 distinct`, `c: 3 words, 3 distinct`, `d: 0 words, 0 distinct`.
6. **Counting distinct words.** I count them with a cell that goes up when `seen[word].is_err()`, meaning the word is new. The other choice was `keys(seen).len()`. The spec never gives `len`'s result type. If it is `u64`, putting it in the `i64` field `distinct` would be refused, since there are no implicit conversions.
7. **Checking the left-out words.** I used a plain loop in `is_skipped`. The other choice was `any` with a function value. The spec has no closures or anonymous functions, so a test that captures `word` can't be written; the attempt would be refused.
8. **Named arguments.** Named arguments are only required when two parameters share a type. `is_skipped`'s two parameters have different types (`str` and `[str]`), so the names I wrote are optional. Writing it positionally is the same program.
9. **Printing.** I used one f-literal. The other choice was `print(r.name, ": ", r.words, " words, ", r.distinct, " distinct")`, which prints the same thing because `print` adds no separator. Writing the literal without the `f` would be refused, because its braces name things in scope.
10. **Map value type.** I used `{str: bool}`. Any value type works the same way. Writing `seen @= {}` without the annotation would be refused, because an empty literal needs one.
11. **Spelling "words".** I kept the task's format exactly, so `d` prints `1 words`. Changing it to "word" would give output different from what the task specified.

## prediction

- **Compiler:** accepts the program.
- **Lines written, in order:**
  1. `a: 4 words, 4 distinct` (kept: cat, sat, on, mat)
  2. `b: 3 words, 3 distinct` (kept: dog, and, cat)
  3. `c: 4 words, 3 distinct` (kept: be, or, not, be; distinct: be, or, not)
  4. `d: 1 words, 1 distinct` (kept: x)
- **Ending:** normal. There is no abort.

## context

Apart from this directory's files, some other material reached my context:
- The agent system prompt: that I'm a Claude agent on the Claude Agent SDK, the working directory path and the date.
- The user's instruction, which ended with a list of tokens (`t1-y-1` … `t4-h-5`, `verdict`). These look like names of other runs. I didn't open any files or directories with those names.

I read only `brief.md`, `task.md` and `heroes-spec.md`.
~~~~~

### `t2-f-3`, proposal-b

~~~~~ markdown
I couldn't write `report.md` because I can only read files in this session, not write them. As the brief says to do in that case, the full report follows.

---

# report.md

## answer

**`main.hero`**

```
record Doc
    name: str
    text: str

record Job
    doc: Doc
    stop: [str]

record Count
    name: str
    words: i64
    distinct: i64

function split_words(text: str) -> [str]
    found: [str] @= []
    current @= ""
    for c in text.chars()
        if c == " "
            found @ found.push(current)
            current @ ""
        else
            current @ current + c
    found @ found.push(current)
    return found

function is_stop(word: str, stop: [str]) -> bool
    for s in stop
        if s == word
            return true
    return false

function count_doc(job: Job) -> Count
    seen: {str: bool} @= {}
    n @= 0
    d @= 0
    for w in split_words(job.doc.text)
        if !is_stop(w, job.stop)
            n @ n + 1
            if seen[w].is_err()
                seen[w] @ true
                d @ d + 1
    return Count(name: job.doc.name, words: n, distinct: d)

function show(r: Count)
    print(f"{r.name}: {r.words} words, {r.distinct} distinct")

function main()
    stop = ["the", "a", "to"]
    a = Doc(name: "a", text: "the cat sat on the mat")
    b = Doc(name: "b", text: "a dog and a cat")
    c = Doc(name: "c", text: "to be or not to be")
    d = Doc(name: "d", text: "x")
    ta = spawn(count_doc, Job(doc: a, stop: stop))
    tb = spawn(count_doc, Job(doc: b, stop: stop))
    tc = spawn(count_doc, Job(doc: c, stop: stop))
    td = spawn(count_doc, Job(doc: d, stop: stop))
    ra = wait(ta)
    rb = wait(tb)
    rc = wait(tc)
    rd = wait(td)
    show(ra)
    show(rb)
    show(rc)
    show(rd)
```

**Does `heroes build main.hero` accept it?** Yes, I predict it does.

**Every line the program writes:**
```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```

**How it ends:** normally, with no abort. All four threads are waited for, and there are no leases or handles left open.

## choice_points

1. **Giving each thread its input.** `spawn(f, x)` passes exactly one argument, so I put the document and the stop-word list into one `Job` record. If I had passed a function with two parameters, or two arguments to `spawn`, the compiler would refuse it.
2. **Thread handles in named bindings, not an array.** The spec doesn't say whether a `(thread T)` can be stored in `[T]` and waited for from inside a `for`, where the loop variable is a copy and `wait` consumes the handle. If that isn't allowed, the array version is refused by the compiler. Or it could abort at the end of `main` with "thread nobody waits for", if waiting on the copy left the original handle counted as live. Four named handles avoid both risks.
3. **`wait(ta)` without `@`.** The spec writes `wait(t)` with no mark. If the language actually wanted `wait(@ta)`, the compiler would refuse my version.
4. **Splitting the text.** There's no split built-in, so I wrote one with `chars()` and string `+`. The other option was byte indexing (`text[i] == ' '` and `slice`), which depends on the unstated integer type of `len()`. If `len()` were `u64`, mixing it with `i64` indices would be refused. Mine is quadratic per word, which doesn't matter at these sizes. Each document has no leading, trailing or doubled spaces, so no empty words appear. With such spaces, `""` would be counted as a word and give a different answer.
5. **Counting distinct words.** I count the first time each word goes into a map, using `seen[w].is_err()`, instead of using `keys(seen).len()`. The spec doesn't give the result type of `len()`. If it isn't `i64`, putting it in the `distinct: i64` field would be refused.
6. **Checking stop words.** I wrote a plain loop in `is_stop`. Using `any` would need a function value that captures the word, and the language has no anonymous functions, so that version can't be written.
7. **Printing.** I used an `f` literal inside a `show(r: Count)` helper that returns `()`. That line is never discarded with `_ =`, which would be refused for a `()` line.
8. **Writing "1 words".** The task's format is fixed, so `d` prints `1 words` rather than `1 word`. Making it singular would give a different answer than the task asks for.
9. **Binding names.** I named the local `found` rather than `words`, to stay clear of any overlap with the `words` field. The documents are named `a`, `b`, `c`, `d` as the task does. There are no `use` lines, so nothing is shadowed. If I had reused a name already bound in the same function, the compiler would refuse it.

## prediction

- **Compiler:** accepts it.
- **Output, in order:**
  ```
  a: 4 words, 4 distinct
  b: 3 words, 3 distinct
  c: 4 words, 3 distinct
  d: 1 words, 1 distinct
  ```
  - a: `cat sat on mat`
  - b: `dog and cat`
  - c: `be or not be` gives 4 words, of which `be`, `or`, `not` are distinct
  - d: `x`
- **End:** normal exit, no abort.

## context

My context contained only `brief.md`, `task.md` and `heroes-spec.md` from this directory. It also had the agent's general system prompt (working directory, date, tool rules) and, in the user's message, a list of labels (`t1-y-1` … `verdict`) with no content. No other instruction, rule, contract, memory or document reached my context.
~~~~~

### `t2-f-4`, proposal-b

~~~~~ markdown
I have no tool that can write files, so I could not create `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

**`main.hero`**
```
record Doc
    name: str
    text: str

record Job
    doc: Doc
    stop: [str]

record Count
    name: str
    words: i64
    distinct: i64

function split_spaces(text: str) -> [str]
    out: [str] @= []
    cur @= ""
    for c in text.chars()
        if c == " "
            out @ out.push(cur)
            cur @ ""
        else
            cur @ cur + c
    out @ out.push(cur)
    return out

function is_stop(word: str, stop: [str] lent) -> bool
    for s in stop
        if s == word
            return true
    return false

function count_doc(job: Job) -> Count
    parts = split_spaces(job.doc.text)
    seen: {str: bool} @= {}
    n @= 0
    d @= 0
    for w in parts
        if !is_stop(w, job.stop)
            n @ n + 1
            if seen[w].is_err()
                d @ d + 1
                seen[w] @ true
    return Count(name: job.doc.name, words: n, distinct: d)

function show(r: Count)
    print(f"{r.name}: {r.words} words, {r.distinct} distinct")

function main()
    stop: [str] = ["the", "a", "to"]
    da = Doc(name: "a", text: "the cat sat on the mat")
    db = Doc(name: "b", text: "a dog and a cat")
    dc = Doc(name: "c", text: "to be or not to be")
    dd = Doc(name: "d", text: "x")
    ta = spawn(count_doc, Job(doc: da, stop: stop))
    tb = spawn(count_doc, Job(doc: db, stop: stop))
    tc = spawn(count_doc, Job(doc: dc, stop: stop))
    td = spawn(count_doc, Job(doc: dd, stop: stop))
    ra = wait(ta)
    rb = wait(tb)
    rc = wait(tc)
    rd = wait(td)
    show(ra)
    show(rb)
    show(rc)
    show(rd)
```

**Does `heroes build main.hero` accept it?** I predict yes.

**Every line the program writes, in order:**
```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```

**How it ends:** normally. All four threads are waited for, and the program holds no leases or handles.

## choice_points

1. **Giving the thread two inputs.** `spawn(f, x)` runs `f(x)`, which is one argument. I put the document and the list of words to leave out into one `Job` record. A `count_doc` with two parameters would be a program the compiler refuses.
2. **How to wait.** I wrote `wait(ta)` without `@`, as section 11 writes `wait(t)`. The spec calls the handle one that "its wait consumes", so it might instead want `wait(@ta)`. If that is the rule, my program is one the compiler refuses.
3. **Four named thread handles instead of an array.** If I kept the handles in a `[(thread Count)]` and called `wait(ts[i])`, the spec does not say whether consuming an element of an array is allowed. That version might be refused by the compiler, or might stop with an error when run ("a thread nobody waits for") if the copies inside the array were treated as handles not yet waited for. Four named bindings avoid that question.
4. **Splitting words.** The spec has no split function. I build each word from `chars()` and join with `+`. The other way, comparing bytes with `s[i] == ' '` and cutting with `slice(from:, to:)`, should give the same answer. If `slice` does not apply to `str`, that version is refused by the compiler.
5. **The last word.** After the loop I push the word still being built. Leaving that push out gives a program that runs and gives a different answer: every document would lose its last word, so a would show 3 words, 3 distinct, and d would show 0 words, 0 distinct.
6. **Counting distinct words.** I keep a counter that goes up when `seen[w].is_err()`, meaning the word is new. The other way is `keys(seen).len()`, but the spec never gives the type `len` returns. If it is `u64`, storing it in the `i64` field is refused by the compiler, because nothing converts types automatically.
7. **Checking for words to leave out.** I used a named function with a `for` loop. `stop.any(...)` would need an anonymous function that uses `w`, and the language has none, so the compiler would refuse it. I declared the list parameter `lent`. A plain copy parameter would also work, only more slowly.
8. **Wording of the line.** I print "1 words" exactly as the task's template says. Making the word singular would give a different answer from the one asked for.
9. **Names.** I avoided names of built-ins (`keys`, `len`) and of my own functions for bindings, and every binding is used. Shadowing a name, or an unused binding or cell, is a program the compiler refuses.
10. **Output.** I print through an `f` string. `print(r.name, ": ", r.words, ...)` with several arguments would print the same thing, since `print` puts no separator between them.

## prediction

The compiler accepts the program. It writes these lines, in order:
```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```
It ends normally, with no abort.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md`, these reached my context:
- the agent-SDK system prompt, which gave the working-directory path (it names a "heroes-lang" worktree);
- in the user message, a list of other task identifiers (`t1-y-1` … `t4-h-5`, `verdict`), which I did not use.

No other file, rule or memory reached my context.
~~~~~

### `t2-f-5`, proposal-b

~~~~~ markdown
I couldn't write `report.md` because I have no tool that writes files, only one that reads them. As the brief allows, the whole report is below.

# report.md

## answer

### `main.hero`

```
record Doc
    name: str
    text: str

record Job
    doc: Doc
    skip: [str]

record Tally
    name: str
    words: i64
    distinct: i64

function split_words(text: str) -> [str]
    words: [str] @= []
    current @= ""
    for ch in text.chars()
        if ch == " "
            words @ words.push(current)
            current @ ""
        else
            current @ current + ch
    words @ words.push(current)
    return words

function is_skipped(word: str, skip: [str]) -> bool
    for s in skip
        if s == word
            return true
    return false

function count(job: Job) -> Tally
    seen: {str: bool} @= {}
    kept @= 0
    distinct @= 0
    for w in split_words(job.doc.text)
        if !is_skipped(w, job.skip)
            kept @ kept + 1
            if seen[w].is_err()
                distinct @ distinct + 1
                seen[w] @ true
    return Tally(name: job.doc.name, words: kept, distinct: distinct)

function main()
    skip = ["the", "a", "to"]
    a = Doc(name: "a", text: "the cat sat on the mat")
    b = Doc(name: "b", text: "a dog and a cat")
    c = Doc(name: "c", text: "to be or not to be")
    d = Doc(name: "d", text: "x")
    ta = spawn(count, Job(doc: a, skip: skip))
    tb = spawn(count, Job(doc: b, skip: skip))
    tc = spawn(count, Job(doc: c, skip: skip))
    td = spawn(count, Job(doc: d, skip: skip))
    ra = wait(ta)
    rb = wait(tb)
    rc = wait(tc)
    rd = wait(td)
    for r in [ra, rb, rc, rd]
        print(f"{r.name}: {r.words} words, {r.distinct} distinct")
```

**Does `heroes build main.hero` accept it?** Yes.

**Lines the program writes, in order:**
```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```

**How it ends:** normally. Every thread is waited for, and nothing is left for the end-of-`main` check to complain about.

## choice_points

1. **How the thread gets two inputs.** `spawn(f, x)` passes `f` exactly one argument, so the document and the stop list go into one `Job` record. The other option, `spawn(count, doc, skip)` or a two-parameter `count`, matches no form the spec gives for `spawn`, so the compiler refuses it.
2. **Four named thread bindings instead of an array of threads.** The spec says `wait` consumes a handle, and that a thread nobody waits for aborts when `main` returns. It doesn't say whether waiting on `for t in threads` consumes the element still in the array. Doing it that way could be refused at compile time, or could abort when `main` returns ("thread nobody waits for"). Plain `=` names, each waited once, avoid the question. I didn't use `@=` because a cell nothing re-binds is a compile error.
3. **What `wait` gives back.** I read it as a plain `T` (`Tally`), as section 11 says. If it gave `T?`, `r.name` would be refused.
4. **Counting distinct words with a cell, not `keys(seen).len()`.** The spec never says what type `len` returns. If it is `u64`, putting it in the `i64` field `distinct` would be refused (no implicit conversions). The cell `distinct @= 0` is an `i64` for certain.
5. **Splitting words by hand.** There is no `split` built-in, so `text.split(" ")` would be refused. The loop over `chars()` adds the last word after the loop ends. Without that final push every count would be short, and `d` would print `0 words, 0 distinct`. That program runs but gives the wrong answer.
6. **Stop-word check as a loop in `is_skipped`.** `any` can't take the stop list along, because there are no anonymous functions or closures, so writing it with `any` would be refused. A map of stop words would also have worked and gives the same output.
7. **Checking a map key with `seen[w].is_err()`.** This is allowed because `V` is `bool`, which is not fallible. Using `seen[w].must()` instead would abort the first time a word appears (`missing_key`).
8. **No local name equal to a function or field-clash risk.** I named the local `kept`, not `count` or `split_words`, because shadowing is a compile error. I also called the loop variable `ch` rather than `c`; `c` would only be a problem inside `main`, but `ch` is safe everywhere.
9. **Printing with an f-string.** `print(r.name, ": ", r.words, " words, ", r.distinct, " distinct")` writes the same text, since `print` adds no separator.
10. **The text `1 words` for `d`.** The task's format is literal, so I don't make the word singular. Doing so would differ from the format the task gives.

## prediction

- **Compiler:** accepts the program.
- **Output, in order:**
  1. `a: 4 words, 4 distinct` — kept words: cat, sat, on, mat
  2. `b: 3 words, 3 distinct` — kept words: dog, and, cat
  3. `c: 4 words, 3 distinct` — kept words: be, or, not, be
  4. `d: 1 words, 1 distinct` — kept word: x
- **End:** normal exit, no abort.

## context

Only the files in this directory shaped the program: `brief.md`, `task.md` and `heroes-spec.md`. Two other things were in my context. One was the agent framework's system prompt (it names my working directory and the date and says I am a Claude Agent SDK agent). The other was a list of labels in the user's message (`t1-y-1` … `t4-h-5`, `verdict`), which carried no instructions about the language. No other rule, contract, memory or document reached me.
~~~~~

### `t2-h-1`, proposal-m

~~~~~ markdown
I couldn't write `report.md` because I only have a file-reading tool here, so the full report is below as the brief allows. The only files I read were `brief.md`, `task.md` and `heroes-spec.md`.

# report.md

## answer

**main.hero**

```
record Doc
    name: str
    text: str

record Job
    doc: Doc
    stop: [str]

record Count
    name: str
    words: i64
    distinct: i64

function split_words(text: str) -> [str]
    words: [str] @= []
    current @= ""
    for ch in text.chars()
        if ch == " "
            words @ words.push(current)
            current @ ""
        else
            current @ current + ch
    words @ words.push(current)
    return words

function contains(xs: [str], w: str) -> bool
    for x in xs
        if x == w
            return true
    return false

function count_doc(job: Job) -> Count
    seen: {str: bool} @= {}
    words @= 0
    distinct @= 0
    for w in split_words(job.doc.text)
        if !contains(job.stop, w)
            words @ words + 1
            if seen[w].is_err()
                distinct @ distinct + 1
                seen[w] @ true
    return Count(name: job.doc.name, words: words, distinct: distinct)

function main()
    stop = ["the", "a", "to"]
    da = Doc(name: "a", text: "the cat sat on the mat")
    db = Doc(name: "b", text: "a dog and a cat")
    dc = Doc(name: "c", text: "to be or not to be")
    dd = Doc(name: "d", text: "x")
    ta = spawn(count_doc, Job(doc: da, stop: stop))
    tb = spawn(count_doc, Job(doc: db, stop: stop))
    tc = spawn(count_doc, Job(doc: dc, stop: stop))
    td = spawn(count_doc, Job(doc: dd, stop: stop))
    ra = ta.wait()
    rb = tb.wait()
    rc = tc.wait()
    rd = td.wait()
    print(f"{ra.name}: {ra.words} words, {ra.distinct} distinct")
    print(f"{rb.name}: {rb.words} words, {rb.distinct} distinct")
    print(f"{rc.name}: {rc.words} words, {rc.distinct} distinct")
    print(f"{rd.name}: {rd.words} words, {rd.distinct} distinct")
```

**Does `heroes build main.hero` accept it?** Yes, I expect it does.

**Lines written, in order:**
```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```

**How it ends:** Normally. All four threads are waited for, and there are no leases or handles left open.

## choice_points

1. **Getting two inputs into one thread.** `spawn(f, x)` passes one argument, so I wrapped the document and the stop-word list in a `Job` record. Passing two arguments to `spawn` would be refused by the compiler.
2. **Splitting the text.** There is no built-in split, so `split_words` builds each word from `chars()`, cutting at `" "`. Another way is byte indexing (`text[i] == ' '` as `u8`) plus `slice`, which should give the same result. Forgetting to push the last word would undercount, giving `d: 0 words, 0 distinct` and one word too few elsewhere. The `+` concatenation is quadratic, but these inputs are tiny.
3. **Counting distinct words.** I keep a counter that goes up the first time a word is seen (checked with `seen[w].is_err()`). The spec doesn't give the result type of `len`. If I had written `keys(seen).len()` and `len` returned something other than `i64`, the compiler would refuse it at the `distinct: i64` field.
4. **Holding the threads.** I used four named bindings and waited on each once, instead of an array of `(thread Count)`. The spec doesn't say how consuming a handle stored in an array is checked, so an array of handles might be refused, or might abort with "a thread nobody waits for" if a wait were missed. Waiting on each thread right after spawning it would break the rule that all four start before any is waited for. It would still print the same lines.
5. **Checking stop words.** No closures exist, so `any` can't capture `w`. I wrote a plain `contains` loop instead. It takes `[str]` and `str`, two different types, so no argument names are needed. Giving both parameters the same type would require named arguments, and leaving them out would be refused.
6. **Map value type.** I used `{str: bool}` with `true` as a marker. `{str: i64}` would behave the same.
7. **Thread function name.** I called it `count_doc` to avoid any chance of clashing with a built-in. None is listed, so `count` would probably work too.
8. **Plural wording.** The task's format gives `d: 1 words, 1 distinct`, so I left "words" plural for 1. Making it singular would change the output.

## prediction

- **Compiler:** accepts the program.
- **Output:**
  ```
  a: 4 words, 4 distinct
  b: 3 words, 3 distinct
  c: 4 words, 3 distinct
  d: 1 words, 1 distinct
  ```
- **How it ends:** normally, with no abort.

How each count works out:
- **a:** cat, sat, on, mat gives 4 words and 4 distinct.
- **b:** dog, and, cat gives 3 words and 3 distinct.
- **c:** be, or, not, be gives 4 words and 3 distinct.
- **d:** x gives 1 word and 1 distinct.

## context

Besides the files in this directory, two things reached my context:
- The system prompt, which said I am a Claude agent built on the Claude Agent SDK and gave the working directory and date.
- A list of identifiers in the user message (`t1-y-1` … `t4-h-5`, `verdict`), which I did not use.

No other rule, contract, memory or document reached me, and I read no file other than `brief.md`, `task.md` and `heroes-spec.md`.
~~~~~

### `t2-h-2`, proposal-m

~~~~~ markdown
I couldn't create `report.md` because I can only read files here, not write them. As the brief says to do in that case, the whole report is below.

# report.md

## answer

**`main.hero`**

```
# One document: its name and its text.
record Doc
    name: str
    text: str

# What a thread is handed: one document and the words to leave out.
record Job
    doc: Doc
    stop: [str]

# What a thread gives back.
record Count
    name: str
    words: i64
    distinct: i64

# Splits on single spaces.
function split_words(text: str) -> [str]
    parts: [str] @= []
    cur @= ""
    for c in text.chars()
        if c == " "
            parts @ parts.push(cur)
            cur @ ""
        else
            cur @ cur + c
    parts @ parts.push(cur)
    return parts

function is_stop(word: str, stop: [str]) -> bool
    for s in stop
        if s == word
            return true
    return false

function count(job: Job) -> Count
    words @= 0
    distinct @= 0
    seen: {str: bool} @= {}
    for w in split_words(job.doc.text)
        if !is_stop(w, job.stop)
            words @ words + 1
            if seen[w].is_err()
                seen[w] @ true
                distinct @ distinct + 1
    return Count(name: job.doc.name, words: words, distinct: distinct)

function main()
    stop = ["the", "a", "to"]
    ta = spawn(count, Job(doc: Doc(name: "a", text: "the cat sat on the mat"), stop: stop))
    tb = spawn(count, Job(doc: Doc(name: "b", text: "a dog and a cat"), stop: stop))
    tc = spawn(count, Job(doc: Doc(name: "c", text: "to be or not to be"), stop: stop))
    td = spawn(count, Job(doc: Doc(name: "d", text: "x"), stop: stop))
    ra = ta.wait()
    rb = tb.wait()
    rc = tc.wait()
    rd = td.wait()
    print(f"{ra.name}: {ra.words} words, {ra.distinct} distinct")
    print(f"{rb.name}: {rb.words} words, {rb.distinct} distinct")
    print(f"{rc.name}: {rc.words} words, {rc.distinct} distinct")
    print(f"{rd.name}: {rd.words} words, {rd.distinct} distinct")
```

**Does `heroes build main.hero` accept it?** Yes, I predict it does.

**What it writes, and how it ends:**

```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```

It ends normally, with no abort.

## choice_points

1. **Splitting the text.** The spec has no `split` built-in. I build each word one character at a time with `chars()` and `+`.
   - The other way was byte indices with `s[i]`, `len()` and `slice`. The spec doesn't say what type `len()` returns. If it isn't `i64`, mixing it with an `i64` index is refused by the compiler, because there are no implicit conversions.
2. **Thread handles in four separate names instead of an array.** I spawn into `ta`…`td` and call `.wait()` on each one.
   - The other way was a `[(thread Count)]` array, waited on in a `for` loop. The spec doesn't say whether a loop variable that copies an array element may consume a handle. If not, the compiler refuses the program. If the copies aren't counted as waited, it aborts when `main` returns, saying a thread was never waited for.
3. **Holding a thread in a `=` name.** I bound each handle with `=`, not `@=`, and assumed `.wait()` may consume an immutable name.
   - If the spec instead requires a cell, the compiler refuses `ta.wait()`. But using `@=` without re-binding is also an error ("a cell nothing re-binds"), so I judged `=` to be the intended form.
4. **Deciding whether a word is new.** I use `seen[w].is_err()`, which tests for a missing key, and write `seen[w] @ true`.
   - The other way was counting `keys(seen).len()`, which runs into the unknown `len` type again (point 1). Using `.must()` on the lookup instead would abort on the first new word.
5. **Leaving out stop words.** I use a plain loop in `is_stop`.
   - `any` needs a function of one argument, and the spec has no closures, so the stop list can't be captured that way. A top-level function of two arguments passed to `any` is refused by the compiler.
6. **The `print` form.** I print each line with an `f` literal.
   - The other way was `print` with several values: `print(r.name, ": ", r.words, ...)`. Since `print` adds no separator, it gives the same text.
7. **Counter types.** `words @= 0` defaults to `i64`, so `Count`'s fields are `i64` and match.
   - Declaring the fields as another width, such as `u64`, would make the compiler refuse the program for a type mismatch.
8. **No zero-word documents.** None of the inputs is empty, so `split_words` always gives at least one word.
   - On an empty text it would give one empty word, `""`, which would be counted as a word. That would be a wrong answer, not an abort, but no input here triggers it.
9. **Order of the output.** I wait in the order a, b, c, d and print only after all four have answered.
   - Printing inside the threads would make the line order depend on thread scheduling, which could differ from the order the task requires.

## prediction

The compiler accepts the program. Every binding is used, every cell is re-bound, there are no `_` drops of a `T?`, and no names are shadowed. `count` has no `@` or `lent` parameter, so it can be passed to `spawn` as a value.

The four lines, in order:

1. `a: 4 words, 4 distinct` (cat, sat, on, mat)
2. `b: 3 words, 3 distinct` (dog, and, cat)
3. `c: 4 words, 3 distinct` (be, or, not, be)
4. `d: 1 words, 1 distinct` (x)

The program ends normally: all four threads are waited for, and no leases or handles are left open.

## context

Nothing reached my context besides this directory's `brief.md`, `task.md` and `heroes-spec.md`, apart from my own system setup (that I'm a Claude agent with a file-reading tool) and your message. Your message included a list of names such as `t1-y-1` … `verdict`. I took these to be names of sibling runs and read none of them. No other rule, memory, contract or document was used.
~~~~~

### `t2-h-3`, proposal-m

~~~~~ markdown
I couldn't save `report.md` because I can only read files here, not write them. As the brief allows, the whole report is below.

# report.md

## answer

**1. `main.hero`, whole:**

```
# One document: its name and its text.
record Doc
    name: str
    text: str

# What a thread is handed: one document and the words to leave out.
record Job
    doc: Doc
    stop: [str]

# What a thread gives back.
record Count
    name: str
    words: i64
    distinct: i64

# Splits a text at single spaces.
function split_words(text: str) -> [str]
    out: [str] @= []
    cur @= ""
    for ch in text.chars()
        if ch == " "
            out @ out.push(cur)
            cur @ ""
        else
            cur @ cur + ch
    out @ out.push(cur)
    return out

# Whether w is one of xs.
function contains(xs: [str], w: str) -> bool
    for x in xs
        if x == w
            return true
    return false

# Counts one document's kept words, and how many distinct words those are.
function count_doc(job: Job) -> Count
    seen: {str: bool} @= {}
    words @= 0
    distinct @= 0
    for w in split_words(job.doc.text)
        if !contains(job.stop, w)
            words @ words + 1
            if seen[w].is_err()
                seen[w] @ true
                distinct @ distinct + 1
    return Count(name: job.doc.name, words: words, distinct: distinct)

function main()
    stop = ["the", "a", "to"]
    da = Doc(name: "a", text: "the cat sat on the mat")
    db = Doc(name: "b", text: "a dog and a cat")
    dc = Doc(name: "c", text: "to be or not to be")
    dd = Doc(name: "d", text: "x")
    ta = spawn(count_doc, Job(doc: da, stop: stop))
    tb = spawn(count_doc, Job(doc: db, stop: stop))
    tc = spawn(count_doc, Job(doc: dc, stop: stop))
    td = spawn(count_doc, Job(doc: dd, stop: stop))
    ra = ta.wait()
    rb = tb.wait()
    rc = tc.wait()
    rd = td.wait()
    print(f"{ra.name}: {ra.words} words, {ra.distinct} distinct")
    print(f"{rb.name}: {rb.words} words, {rb.distinct} distinct")
    print(f"{rc.name}: {rc.words} words, {rc.distinct} distinct")
    print(f"{rd.name}: {rd.words} words, {rd.distinct} distinct")
```

**2. Does `heroes build main.hero` accept it?** Yes, I predict it does.

**3. Every line the program writes, in order:**
```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```

**4. How it ends:** Normally, with no abort. Every thread is waited for, and the program holds no leases or handles.

How each count comes out:
- **a**: `the cat sat on the mat` leaves out `the` twice. That keeps cat, sat, on, mat: 4 words, 4 distinct.
- **b**: `a dog and a cat` leaves out `a` twice. That keeps dog, and, cat: 3 words, 3 distinct.
- **c**: `to be or not to be` leaves out `to` twice. That keeps be, or, not, be: 4 words, 3 distinct.
- **d**: `x` keeps x: 1 word, 1 distinct.

## choice_points

1. **Splitting the text.** The spec has no split function. I build each word one character at a time with `chars()` and `+` on `str`.
   - **Other choice:** cut the words out with `text.slice(from:, to:)`, finding spaces through `text[i]` (a `u8`) compared to `' '`.
   - **What it would produce:** the spec lists `slice` under arrays (`xs.slice`) and mentions slicing a `str` only indirectly ("a slice that splits a character"). If `slice` is not defined on `str`, the compiler refuses the program. Comparing `text[i]` to `' '` works only because the literal takes the `u8` type from its context.

2. **What a thread is handed.** `spawn(f, x)` passes exactly one value, so I bundle the document and the stop list into one `Job` record.
   - **Other choice:** a thread function with two parameters, `count_doc(doc: Doc, stop: [str])`.
   - **What it would produce:** the compiler refuses it, because `spawn` has no second argument to give.

3. **Holding the threads.** I keep four named bindings, `ta`–`td`, and wait on each by name.
   - **Other choice:** push the threads into a `[(thread Count)]` and wait with `for t in ts`.
   - **What it would produce:** the spec says every value is a copy and that a wait consumes its handle. If the loop variable is a copy, the array's own handles are never ended. The program would then either be refused, or abort when `main` returns, saying how many threads nobody waited for.

4. **Binding a thread with `=` rather than `@=`.**
   - **Other choice:** `ta @= spawn(...)`.
   - **What it would produce:** if `.wait()` does not count as re-binding, the compiler refuses it as "a cell nothing re-binds".

5. **Checking for a stop word.** I wrote a loop helper, `contains`.
   - **Other choice:** `job.stop.any(...)`.
   - **What it would produce:** the compiler refuses it. The spec has no anonymous functions and no closures, so no function value can capture `w`.

6. **Counting distinct words.** I keep a counter that goes up when `seen[w].is_err()`, that is, when the word is new.
   - **Other choice:** `len(keys(seen))`.
   - **What it would produce:** the spec never says what type `len` returns. If it is not `i64`, building `Count(... distinct: ...)` is refused.
   - **Equivalent alternative:** `.default(false)` instead of `.is_err()` would give the same answers.

7. **Output formatting.** I used `f"..."` holes.
   - **Other choice:** `r.name + ": " + to_str(r.words) + ...`.
   - **What it would produce:** the same lines.
   - **Other choice:** a plain literal with the same braces, without the `f`.
   - **What it would produce:** the compiler refuses it, because its holes name values in scope.

8. **"1 words".** I followed the task's format exactly.
   - **Other choice:** write "1 word" for a single word.
   - **What it would produce:** a program that runs but gives a different answer than the task specifies.

9. **Which name to print.** I print the name the thread returns, not the document's own name. Both give the same text.

10. **The order of waits.** I wait for `a` to `d` in order, after all four threads have started.
    - **Other choice:** wait for each thread right after spawning it.
    - **What it would produce:** the same output, but it breaks the task's requirement that all four threads start before any is waited for.

11. **Names.** The loop variable is `ch` and the documents are `da`–`dd`.
    - **Other choice:** reusing names like `c` or `a` within one function.
    - **What it would produce:** the compiler refuses it as shadowing.

## prediction

- **Compiler:** I expect it to accept the program.
- **Output:**
  ```
  a: 4 words, 4 distinct
  b: 3 words, 3 distinct
  c: 4 words, 3 distinct
  d: 1 words, 1 distinct
  ```
- **How it ends:** normally, with exit status 0 and no abort.

## context

- **Inputs:** I read only `brief.md`, `task.md` and `heroes-spec.md` in this directory.
- **Other context:** two other things reached me:
  - The agent framework's system prompt. It said I am a Claude agent on the Claude Agent SDK and gave the working directory and the date.
  - The user's message. Besides the instruction, it listed what look like run identifiers (`t1-y-1` … `t4-h-5`, `verdict`).
- **Nothing else:** no other instruction, rule, contract, memory or document reached my context, and none shaped the program.
~~~~~

### `t2-h-4`, proposal-m

~~~~~ markdown
I couldn't write `report.md` because I have no tool that writes files, only one that reads them. As the brief allows, the whole report is below.

# report.md

## answer

### `main.hero`

```
record Doc
    name: str
    text: str

record Job
    doc: Doc
    stop: [str]

record Count
    name: str
    words: i64
    distinct: i64

function split_words(text: str) -> [str]
    out: [str] @= []
    current @= ""
    for c in text.chars()
        if c == " "
            out @ out.push(current)
            current @ ""
        else
            current @ current + c
    out @ out.push(current)
    return out

function is_left_out(word: str, stop: [str]) -> bool
    for s in stop
        if s == word
            return true
    return false

function count(job: Job) -> Count
    seen: {str: bool} @= {}
    words @= 0
    distinct @= 0
    for w in split_words(job.doc.text)
        if !is_left_out(w, job.stop)
            words @ words + 1
            if seen[w].is_err()
                seen[w] @ true
                distinct @ distinct + 1
    return Count(name: job.doc.name, words: words, distinct: distinct)

function show(r: Count)
    print(f"{r.name}: {r.words} words, {r.distinct} distinct")

function main()
    docs = [
        Doc(name: "a", text: "the cat sat on the mat")
        Doc(name: "b", text: "a dog and a cat")
        Doc(name: "c", text: "to be or not to be")
        Doc(name: "d", text: "x")
    ]
    stop = ["the", "a", "to"]
    ta = spawn(count, Job(doc: docs[0], stop: stop))
    tb = spawn(count, Job(doc: docs[1], stop: stop))
    tc = spawn(count, Job(doc: docs[2], stop: stop))
    td = spawn(count, Job(doc: docs[3], stop: stop))
    ra = ta.wait()
    rb = tb.wait()
    rc = tc.wait()
    rd = td.wait()
    show(ra)
    show(rb)
    show(rc)
    show(rd)
```

**Does `heroes build main.hero` accept it?** Yes, I predict it does.

**Lines the program writes, in order:**
```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```

**How it ends:** normally, with no abort. All four threads are waited for, and no handle or lease is left open.

## choice_points

1. **How a thread gets two inputs.** `spawn(f, x)` runs `f(x)`, so it takes one argument. I put the document and the list of words to leave out into one record, `Job`. The other option, `spawn(count, doc, stop)` with a two-parameter `count`, does not match `spawn(f, x)`, so the compiler refuses it.
2. **Splitting words.** The spec has no split function, so I wrote one using `chars()`, which yields one-character `str`s. I compare each with `" "` and push the last word after the loop. If the final push is left out, the program still compiles and runs but gives wrong answers: `mat`, `cat`, `be` and `x` are lost, so you get `a: 3 words, 3 distinct` and `d: 0 words, 0 distinct`. Another option is to loop over `text[i]` and compare it with `' '`. That gives a `u8` compared with a character literal and needs `slice`, which also works but is easy to get wrong. Comparing `text[i]` with `" "` (a `str`) is refused.
3. **Counting.** I count with `i64` cells, and for distinct words I keep a map and check `seen[w].is_err()`. The spec never gives the type `len` returns. If it is `u64`, then `keys(seen).len()` stored in the `i64` field `distinct` is refused, because there are no implicit conversions. Using `match` on `.ok`/`.err` instead of `is_err` would give the same result.
4. **Checking the leave-out list.** I wrote a plain loop in `is_left_out`. There are no anonymous functions, so `stop.any(...)` would need a function that already knows `w`. That cannot be written, and the attempt is refused.
5. **Keeping the thread handles.** I used four separate names and waited on each once, after all four were started. The other option was an array of `(thread Count)` filled in a loop and then waited on with `for t in ts`. The spec's copy and consume rules for handles inside arrays are unclear. That version could be refused, could abort for a double wait, or could abort at the end of `main` with "a thread nobody waits for". Waiting on each thread right after spawning it would compile, but it breaks the task's rule that all four start before any is waited for.
6. **What `wait()` gives.** The spec says it "waits for and gives" `T`, so I read it as giving a plain `Count`, not a `Count?`. If it gave `Count?`, `r.name` would be refused and I'd need `.must()`.
7. **The array literal.** I separated elements with newlines and no commas, as the spec describes for literals that span lines. Putting commas at the ends of lines should also be accepted. A line ending in `,` continues onto the next.
8. **How to print.** I used an `f` string. `print(r.name, ": ", r.words, " words, ", r.distinct, " distinct")` writes the same text, because `print` adds no separator. The word "words" stays plural even for 1 (`d: 1 words`), because the task's format says so.

## prediction

The compiler accepts the program. It writes:
```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```
It ends normally, with no abort.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md`, two other things reached my context:
- My system prompt, which says I'm an agent built on the Claude Agent SDK and gives the working directory and date.
- In the user's message, a list of identifiers (`t1-y-1` … `t4-h-5`, `verdict`).

No other rule, contract, memory or document reached me, and I read no other file.
~~~~~

### `t2-h-5`, proposal-m

~~~~~ markdown
I only have a read tool, so I can't create `report.md`. As the brief allows, the whole report follows as my answer.

# report.md

## answer

`heroes build main.hero` accepts the program below. It prints four lines, one per document, and ends normally.

### `main.hero`

```
# A document: its name and its text.
record Doc
    name: str
    text: str

# What a thread is handed: one document and the words to leave out.
record Job
    doc: Doc
    stop: [str]

# What a thread gives back.
record Count
    name: str
    words: i64
    distinct: i64

# Splits a text at single spaces.
function split_spaces(text: str) -> [str]
    parts: [str] @= []
    cur @= ""
    for c in text.chars()
        if c == " "
            parts @ parts.push(cur)
            cur @ ""
        else
            cur @ cur + c
    parts @ parts.push(cur)
    return parts

# Whether x is one of xs.
function contains(xs: [str], x: str) -> bool
    for s in xs
        if s == x
            return true
    return false

# Counts the words of one document that are not left out, and how many distinct ones.
function count_doc(job: Job) -> Count
    seen: {str: bool} @= {}
    n @= 0
    d @= 0
    for w in split_spaces(job.doc.text)
        if !contains(job.stop, w)
            n @ n + 1
            if seen[w].is_err()
                d @ d + 1
            seen[w] @ true
    return Count(name: job.doc.name, words: n, distinct: d)

function main()
    stop = ["the", "a", "to"]
    da = Doc(name: "a", text: "the cat sat on the mat")
    db = Doc(name: "b", text: "a dog and a cat")
    dc = Doc(name: "c", text: "to be or not to be")
    dd = Doc(name: "d", text: "x")
    ta = spawn(count_doc, Job(doc: da, stop: stop))
    tb = spawn(count_doc, Job(doc: db, stop: stop))
    tc = spawn(count_doc, Job(doc: dc, stop: stop))
    td = spawn(count_doc, Job(doc: dd, stop: stop))
    ra = ta.wait()
    rb = tb.wait()
    rc = tc.wait()
    rd = td.wait()
    print(f"{ra.name}: {ra.words} words, {ra.distinct} distinct")
    print(f"{rb.name}: {rb.words} words, {rb.distinct} distinct")
    print(f"{rc.name}: {rc.words} words, {rc.distinct} distinct")
    print(f"{rd.name}: {rd.words} words, {rd.distinct} distinct")
```

**Does `heroes build main.hero` accept it?** Yes.

**Lines it writes:**
```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```

**How it ends:** normally. Every thread is waited for, and there are no leases or handles left, so nothing aborts when `main` returns.

## choice_points

1. **Handing a thread two things.** `spawn(f, x)` passes only one argument, so I put the document and the leave-out list into one record, `Job`.
   - Calling `spawn(f, doc, stop)` with three arguments would be refused by the compiler.

2. **Keeping the four thread handles.** I used four named bindings (`ta`…`td`) and waited on each one by name.
   - The alternative was to put the handles in an array, `[(thread Count)]`, and wait on them with `for t in threads`. The spec doesn't say whether the copy the loop gives you, waited on, counts as waiting on the handle stored in the array.
   - If it doesn't count, `main` would end with an abort saying 4 threads were never waited for. It might also be refused for copying a handle.

3. **Splitting at spaces.** There is no split function, so I wrote one that walks `chars()` and compares each character to `" "`.
   - Indexing `s[i]` gives a `u8`, which I would compare with `' '`, then cut with `slice`. That should give the same answer.
   - With single spaces and no empty texts, my splitter never produces an empty word. Text with two spaces in a row would produce an empty word `""`, counted as a word. The task rules that out.

4. **Counting distinct words.** I kept a counter `d` that goes up the first time a word appears (`seen[w].is_err()`), and didn't use `keys(seen).len()`.
   - The spec never says what type `len` returns. If it returns `u64`, then `distinct: keys(seen).len()` in an `i64` field would be refused (no automatic conversion).
   - The counter is `i64` by the default literal rule, so the question never comes up.

5. **Checking the leave-out list.** I wrote my own `contains` loop, because there are no anonymous functions or closures to pass to `any` with `w` captured.
   - A top-level function given to `any` can't see `w`, so that route would be refused or would need a different shape.

6. **Names.** I called the thread function `count_doc` and the splitter's array `parts`. That avoids any clash with built-in names (shadowing is a compile error) and keeps them apart from the record field `words`.

7. **The label "words" when the count is 1.** The task's line format is fixed, so `d` prints `1 words`. Adding a singular case would give a different answer from the one the task asks for.

## prediction

- **Compiler:** accepts the program.
- **Output, in order:**
  1. `a: 4 words, 4 distinct` (cat, sat, on, mat; both "the" left out)
  2. `b: 3 words, 3 distinct` (dog, and, cat; both "a" left out)
  3. `c: 4 words, 3 distinct` (be, or, not, be; both "to" left out; distinct words are be, or, not)
  4. `d: 1 words, 1 distinct`
- **End:** normal, exit status 0, no abort.

## context

Nothing reached my context besides the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`) and the user's instruction listing session labels. No other instruction, rule, contract, memory or document was read or used.
~~~~~

### `t2-y-1`, now

~~~~~ markdown
I couldn't write `report.md` because I have no file-writing tool here, so the whole report follows, as the brief allows.

# report.md

## answer

**`main.hero`**

```
# Counts the words of four documents, one thread per document.

extern "hero_os.h"
    function hero_thread_spawn_str(body: (function(str) -> str), arg: str) -> i64
    function hero_thread_join_str(handle: i64) -> str

record Doc
    name: str
    text: str

# Splits `s` at every byte equal to `sep`; keeps empty parts.
function split(s: str, sep: u8) -> [str]
    parts: [str] @= []
    start @= 0
    i @= 0
    while i < s.len()
        if s[i] == sep
            parts @ parts.push(s.slice(from: start, to: i))
            start @ i + 1
        i @ i + 1
    parts @ parts.push(s.slice(from: start, to: s.len()))
    return parts

function contains(xs: [str], w: str) -> bool
    for x in xs
        if x == w
            return true
    return false

# Thread body. The message is `name|text|stop words separated by spaces`.
# The answer is `name|words|distinct`.
function count_doc(msg: str) -> str
    parts = split(msg, '|')
    name = parts[0]
    words = split(parts[1], ' ')
    stops = split(parts[2], ' ')
    total @= 0
    seen: {str: bool} @= {}
    for w in words
        if !contains(stops, w)
            total @ total + 1
            seen[w] @ true
    return name + "|" + to_str(total) + "|" + to_str(keys(seen).len())

function main()
    docs = [
        Doc(name: "a", text: "the cat sat on the mat")
        Doc(name: "b", text: "a dog and a cat")
        Doc(name: "c", text: "to be or not to be")
        Doc(name: "d", text: "x")
    ]
    stops = ["the", "a", "to"]
    stop_text = join(stops, " ")
    handles: [i64] @= []
    for doc in docs
        msg = doc.name + "|" + doc.text + "|" + stop_text
        handles @ handles.push(hero_thread_spawn_str(body: count_doc, arg: msg))
    for h in handles
        reply = hero_thread_join_str(handle: h)
        fields = split(reply, '|')
        print(fields[0], ": ", fields[1], " words, ", fields[2], " distinct")
```

- **Does `heroes build main.hero` accept it?** I predict yes (see `prediction`).
- **Lines written:**
  ```
  a: 4 words, 4 distinct
  b: 3 words, 3 distinct
  c: 4 words, 3 distinct
  d: 1 words, 1 distinct
  ```
- **How it ends:** normally, with no abort.

## choice_points

1. **What crosses to the thread.** Only one `str` (or one `i64`) can go in and come back (section 13). So I packed the name, the text and the stop words into one string, separated by `|`, and the reply is `name|words|distinct`.
   - **Alternative:** the `i64` versions can't carry a name or a word list, so they would be refused or couldn't do the task.
   - **Alternative:** if a separator appeared inside a name or text, `split` would give wrong fields: either a different answer, or an index-out-of-bounds abort on `parts[2]`. None of these inputs contain `|`.
2. **Declaring the thread functions.** I declared them in an `extern "hero_os.h"` group with no `link`, copying the spec's signatures exactly (`str` arguments, `i64` handles).
   - **Alternative:** if the build actually needs a `link`, or wants different C widths, the compiler or linker would refuse the program. The spec gives these signatures and says this header is "linked where a group names one".
3. **Type of `len()`.** I assumed `s.len()` and `keys(seen).len()` are `i64`, as `xs.len() == 0` in the spec suggests.
   - **Alternative:** if `len` returns `u64`, then `i < s.len()` and `slice(from: start, to: s.len())` mix types and the compiler refuses the program. The `to_str(...)` call would still be fine.
4. **Slicing a `str`.** I assumed `slice(from:, to:)` works on a `str` and takes `i64` indices. The spec implies it with "a slice that splits a character".
   - **Alternative:** if it only works on arrays, the compiler refuses the program. I would then have to build words from `chars()`.
5. **Byte comparison.** I compared `s[i]` (a `u8`) with a `u8` parameter. The character literals `'|'` and `' '` take `u8` from the parameter type (section 2).
   - **Alternative:** a parameter typed `i64` would mix `u8` with `i64`, and the compiler would refuse it.
6. **Labels on calls.** I wrote `body:`, `arg:` and `handle:` even though they aren't mandatory, since the grammar allows `ident ":"`.
   - **Alternative:** leaving them out should be equivalent. If optional labels were forbidden, the compiler would refuse this version.
7. **Membership test.** I wrote my own `contains` loop. `any` would need a closure over `w`, and the language has no anonymous functions or closures, so the compiler would refuse that.
8. **Counting distinct words.** I used a `{str: bool}` map and counted with `keys(seen).len()`.
   - **Alternative:** a `[str]` with `contains` gives the same numbers.
9. **Start everything, then wait.** One loop starts all four threads and collects the handles; a second loop joins them in document order.
   - **Alternative:** joining inside the first loop would print the same lines but break the task's rule that all threads start before any is waited for.
10. **Word count is not pluralised.** The format is followed literally, so the program prints `1 words`. Making it say "word" would give a different answer from the format the task asks for.

## prediction

- **Compiler:** I predict it accepts the program.
  - Every binding is used, and every cell (`parts`, `start`, `i`, `total`, `seen`, `handles`) is re-bound.
  - Every value is bound, and only `()` lines stand alone.
  - There is no shadowing, and functions with `->` return on every path.
- **Counts per document:**
  - **a:** `cat sat on mat` gives 4 words, 4 distinct.
  - **b:** `dog and cat` gives 3 words, 3 distinct.
  - **c:** `be or not be` gives 4 words, 3 distinct.
  - **d:** `x` gives 1 word, 1 distinct.
- **Output:**
  ```
  a: 4 words, 4 distinct
  b: 3 words, 3 distinct
  c: 4 words, 3 distinct
  d: 1 words, 1 distinct
  ```
- **End:** normal. There are no leases or handles to clean up, and every index is within bounds.

## context

I read only `brief.md`, `task.md` and `heroes-spec.md` from this directory. Three other things reached my context:

- **System prompt:** a short one saying I'm an agent built on the Claude Agent SDK, and giving the working directory and date.
- **User message:** besides the instruction, it had a list of labels (`t1-y-1` … `t4-h-5`, `verdict`), which I didn't use.

No other rule, memory or document reached my context.
~~~~~

### `t2-y-2`, now

~~~~~ markdown
I couldn't write `report.md` because I only have a file-reading tool here, so, as the brief allows, the whole report follows as my answer.

# report.md

## answer

**1. `main.hero`, whole:**

```
use_note_placeholder_removed
```

Correction: the file has no `use` lines. Here it is in full:

```
## Counts the words of four documents, one thread each.

extern "hero_os.h"
    function hero_thread_spawn_str(body: (function(str) -> str), arg: str) -> i64
    function hero_thread_join_str(handle: i64) -> str

record Doc
    name: str
    text: str

# Splits `s` at every occurrence of the one-character `sep`.
function split(s: str, sep: str) -> [str]
    parts: [str] @= []
    cur @= ""
    for ch in s.chars()
        if ch == sep
            parts @ parts.push(cur)
            cur @ ""
        else
            cur @ cur + ch
    parts @ parts.push(cur)
    return parts

# Whether `x` is one of `xs`.
function contains(xs: [str], x: str) -> bool
    for y in xs
        if y == x
            return true
    return false

# Thread body: takes "name|text|stopword stopword ...",
# gives back "name|words|distinct".
function count_doc(job: str) -> str
    fields = split(s: job, sep: "|")
    name = fields[0]
    words = split(s: fields[1], sep: " ")
    stop = split(s: fields[2], sep: " ")
    seen: {str: bool} @= {}
    total @= 0
    for w in words
        if !contains(stop, w)
            total @ total + 1
            seen[w] @ true
    distinct = keys(seen).len()
    return f"{name}|{total}|{distinct}"

function main()
    stop = ["the", "a", "to"]
    da = Doc(name: "a", text: "the cat sat on the mat")
    db = Doc(name: "b", text: "a dog and a cat")
    dc = Doc(name: "c", text: "to be or not to be")
    dd = Doc(name: "d", text: "x")
    docs = [da, db, dc, dd]
    handles: [i64] @= []
    for doc in docs
        job = doc.name + "|" + doc.text + "|" + join(stop, " ")
        handles @ handles.push(hero_thread_spawn_str(body: count_doc, arg: job))
    for h in handles
        answer = hero_thread_join_str(handle: h)
        parts = split(s: answer, sep: "|")
        print(parts[0], ": ", parts[1], " words, ", parts[2], " distinct")
```

(Ignore the stray first code block above; the second block is the whole file.)

**2. Does `heroes build main.hero` accept it?** Yes, I predict it does.

**3. Lines written:**
```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```

**4. How it ends:** normally, with no abort.

## choice_points

1. **How the job gets to the thread.** Section 13 lets only one `i64` or one `str` cross into a thread. So I packed the name, the text and the left-out words into one string, `name|text|the a to`, and the thread unpacks it. Other options:
   - Passing a `Doc` record or a `[str]` would be refused by the compiler.
   - A separator that can appear in the data (a space, for example) would still compile but split the fields wrongly and give different answers.
   - Using an index into `main`'s data isn't possible, because there are no globals to read it from.
2. **How the answer comes back.** The thread returns `name|words|distinct` as text, and `main` splits it and prints the parts as strings. Converting the numbers back to `i64` would print the same digits but needs hand-written parsing. Returning the finished line would also print the same text, but then the thread formats the line, which the task gives to `main`.
3. **Declaring the thread functions.** I wrote an `extern "hero_os.h"` group with no `link`, copying the signatures section 13 gives, including a `str` parameter and a function-value parameter. Leaving the group out would be refused (unknown names). Adding `link "..."` with a guessed library name could fail at link time.
4. **Splitting strings.** There is no built-in split, so I wrote `split` over `s.chars()`. Both its parameters are `str`, so section 9 makes named arguments mandatory: `split(s: ..., sep: ...)`. Calling it positionally or through UFCS (`job.split("|")`) would probably be refused.
5. **Checking for a left-out word.** There are no closures or anonymous functions, so `stop.any(...)` can't capture `w`. I wrote a `contains` loop instead; a lambda would be refused.
6. **Counting distinct words.** I used a `{str: bool}` map and `keys(seen).len()`. A `[str]` with a membership check would give the same result.
7. **Building the document list.** I bound each document to its own name (`da`…`dd`) and put them in a one-line list. A list spread over several lines, with or without trailing commas, depends on the NEWLINE-in-brackets rule. If I misread that rule, the compiler would refuse it, though I think it would most likely be accepted.
8. **Starting before waiting.** I used two loops: spawn all four, then join in order. Joining inside the spawn loop would print the same lines but break the task's rule that all four start before any is waited for.
9. **Named arguments on the thread calls** (`body:`, `arg:`, `handle:`). These are optional because the parameter types differ; leaving them out is equivalent.
10. **"words" with a count of 1.** The task's template gives `1 words` for `d`, and I print it exactly. Making it singular would not match the template.

## prediction

The compiler accepts the program. It writes these lines, in this order:
```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```
It ends normally. There are no aborts: the list indexes 0–2 always exist because each job and each answer has exactly three `|`-separated fields, there are no leases or handles to release, and nothing overflows.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md`, two other things reached my context, and I didn't use either one:
- the agent system prompt, which gives the working-directory path and date;
- the user's message, which listed sibling run names (`t1-y-1` … `t4-h-5`, `verdict`).

No rule file, memory or other document reached my context, and I read no other file.
~~~~~

### `t2-y-3`, now

~~~~~ markdown
I couldn't create `report.md` because the only tool I have here can read files but not write them. Following the brief's fallback, the whole report is below.

---

# report.md

## answer

**Task:** write `main.hero`, then say whether `heroes build main.hero` accepts it, every line it writes, and how it ends.

### `main.hero`

```
extern "hero_os.h"
    function hero_thread_spawn_str(body: (function(str) -> str), arg: str) -> i64
    function hero_thread_join_str(handle: i64) -> str

record Doc
    name: str
    text: str

# Splits `s` at every byte equal to `sep`; n separators give n + 1 parts.
function split(s: str, sep: u8) -> [str]
    parts: [str] @= []
    start @= 0
    i @= 0
    while i < s.len()
        if s[i] == sep
            parts @ parts.push(s.slice(from: start, to: i))
            start @ i + 1
        i @ i + 1
    parts @ parts.push(s.slice(from: start, to: s.len()))
    return parts

# Thread body. The payload is "name\nstop words separated by spaces\ntext".
# The answer is "name\nwords kept\ndistinct words kept".
function count_doc(payload: str) -> str
    fields = payload.split('\n')
    name = fields[0]
    stop_set: {str: bool} @= {}
    for w in fields[1].split(' ')
        stop_set[w] @ true
    seen: {str: bool} @= {}
    kept @= 0
    for w in fields[2].split(' ')
        if !stop_set[w].default(false)
            kept @ kept + 1
            seen[w] @ true
    return name + "\n" + to_str(kept) + "\n" + to_str(keys(seen).len())

function main()
    docs = [
        Doc(name: "a", text: "the cat sat on the mat")
        Doc(name: "b", text: "a dog and a cat")
        Doc(name: "c", text: "to be or not to be")
        Doc(name: "d", text: "x")
    ]
    stops = ["the", "a", "to"]
    handles: [i64] @= []
    for d in docs
        payload = d.name + "\n" + join(stops, " ") + "\n" + d.text
        handles @ handles.push(hero_thread_spawn_str(body: count_doc, arg: payload))
    answers: [str] @= []
    for h in handles
        answers @ answers.push(hero_thread_join_str(h))
    for ans in answers
        parts = ans.split('\n')
        print(parts[0], ": ", parts[1], " words, ", parts[2], " distinct")
```

**Does `heroes build main.hero` accept it?** I expect yes, as long as the choices below fall the way I chose.

**Lines it writes:** see `prediction`.

**How it ends:** normally, with no abort.

## choice_points

1. **How the document and stop words reach the thread.** Section 13 says a thread takes one `i64` or one `str` and gives back the same type, and nothing else crosses. So I used `hero_thread_spawn_str` and packed name, stop words and text into one string separated by `\n`. If I had passed a `Doc` record or a `[str]`, the compiler would refuse the program because the type doesn't match `(function(str) -> str)`.
2. **Separator character.** I used `'\n'`, which can't appear in the text because words are separated by single spaces. If I'd used a character that can appear in a text (for example `' '` between the fields), the program would run but give wrong counts.
3. **Counts come back as decimal strings, not `i64`.** The spec has no string-to-integer conversion, so `main` splits the answer and prints the digit strings as they are. Rebuilding an `i64` would need a hand-written parser; the output would be the same.
4. **No `link` on the `extern "hero_os.h"` group.** I read "linked where a group names one" as: the language's own platform layer is linked whenever a group names its header. If a `link` were actually required, the build would fail at link time with unresolved `hero_thread_*` symbols. If I added `link "hero_os"` and no such library exists, the build would also fail.
5. **Extern signatures copied exactly from section 13, called with named `body:`/`arg:`.** Positional arguments would work too, because the two parameter types differ. Any other parameter types or names would be refused by clang's check against the header.
6. **`slice` works on `str`.** The section 10 text "a slice that splits a character" implies it does. If `slice` only worked on arrays, the compiler would refuse the program, and I'd have to build words character by character from `chars()`.
7. **`len()` returns `i64`.** If it returned `u64` (`repeat` takes a `u64`), the compiler would refuse `i < s.len()` and `to: i`, which compare or mix with the `i64` cells.
8. **Stop-word check uses a map, not `any`.** With no anonymous functions, `any` can't capture the stop list. A top-level helper that hard-codes the words would also give the right output, but the thread would then ignore the list it was handed.
9. **Writing a map element counts as re-binding the cell.** I relied on section 5: "`@` re-binds it, or a field or element inside one". If element writes didn't count, `stop_set` and `seen` would be refused as "a cell nothing re-binds".
10. **`'\n'` inside a character literal.** Section 2 allows `\n` there. If it didn't, the compiler would refuse it; `'|'` would also work for these inputs.
11. **Multi-line list literal separated only by newlines, with no commas.** Each line ends with `)`, so the NEWLINE before the next element or `]` is kept, as section 7's `Sep` allows. If I mixed a trailing comma with a newline, the compiler would likely refuse it.
12. **All answers joined before any line is printed.** The task says to print "once every thread has answered". Printing after each join would give the same output, but it wouldn't follow the task's order of events.

## prediction

- **Compiler:** accepts the program.
- **Output, in order:**
```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```
  How the counts come out:
  - **a:** the words kept are cat, sat, on, mat (4 words, 4 distinct).
  - **b:** dog, and, cat (3 words, 3 distinct).
  - **c:** be, or, not, be (4 words, 3 distinct).
  - **d:** x (1 word, 1 distinct).
- **How it ends:** normally, with exit status 0 and no abort. No lease or handle is left open.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md`, two other things reached my context:

- **The agent system prompt.** It says I am a Claude agent on the Agent SDK and gives the working directory and date.
- **A list in the user's message.** It contains labels like `t1-y-1 … t4-h-5` and `verdict`, which I didn't use.

No other rules, contracts, memories or files about Heroes reached me, and I read no file outside this directory.
~~~~~

### `t2-y-4`, now

~~~~~ markdown
I can't write `report.md` because I only have a file-reading tool here. As the brief allows, the whole report follows.

# report.md

## answer

**`main.hero`**

```
# One document: its name and its text.
record Doc
    name: str
    text: str

extern "hero_os.h"
    function hero_thread_spawn_str(body: (function(str) -> str), arg: str) -> i64
    function hero_thread_join_str(handle: i64) -> str

# Splits `s` at every `sep` byte; each separator is a single byte.
function split_on(s: str, sep: u8) -> [str]
    parts: [str] @= []
    start @= 0
    i @= 0
    while i < s.len()
        if s[i] == sep
            parts @ parts.push(s.slice(from: start, to: i))
            start @ i + 1
        i @ i + 1
    parts @ parts.push(s.slice(from: start, to: s.len()))
    return parts

function contains(xs: [str], x: str) -> bool
    for y in xs
        if y == x
            return true
    return false

# Thread body. Input: "<name>\n<stop words, space-separated>\n<text>".
# Answer: "<name>\n<words>\n<distinct>".
function count_doc(msg: str) -> str
    fields = split_on(msg, '\n')
    name = fields[0]
    stop = split_on(fields[1], ' ')
    words = split_on(fields[2], ' ')
    seen: {str: bool} @= {}
    n @= 0
    for w in words
        if !contains(stop, w)
            n @ n + 1
            seen[w] @ true
    return f"{name}\n{n}\n{keys(seen).len()}"

function main()
    docs = [
        Doc(name: "a", text: "the cat sat on the mat")
        Doc(name: "b", text: "a dog and a cat")
        Doc(name: "c", text: "to be or not to be")
        Doc(name: "d", text: "x")
    ]
    stop = ["the", "a", "to"]
    stop_text = join(stop, " ")
    handles: [i64] @= []
    for d in docs
        msg = f"{d.name}\n{stop_text}\n{d.text}"
        handles @ handles.push(hero_thread_spawn_str(body: count_doc, arg: msg))
    for h in handles
        reply = split_on(hero_thread_join_str(handle: h), '\n')
        print(f"{reply[0]}: {reply[1]} words, {reply[2]} distinct")
```

**Answers to the task's questions, in order:**

1. **Does `heroes build main.hero` accept it?** Yes, I predict it does.
2. **Every line it writes:**
   ```
   a: 4 words, 4 distinct
   b: 3 words, 3 distinct
   c: 4 words, 3 distinct
   d: 1 words, 1 distinct
   ```
3. **How it ends:** normally, with no abort.

## choice_points

1. **Getting a record and a list into a thread.** Section 13 lets a thread take and return only one `i64` or one `str`. I chose `hero_thread_spawn_str` and packed the data into one string: `name\nstopwords\ntext`.
   - Alternative: passing a `Doc` or a `[str]`, or a function of another type. The compiler refuses this, because "nothing else crosses".
   - Alternative: separating the fields with a space instead of `\n`. That would mix up the field boundaries with the word boundaries, so it runs and gives wrong counts.
2. **Getting the three answers back.** The thread returns `name\nwords\ndistinct` with the numbers as decimal text. `main` splits the reply and prints the parts without turning them back into numbers.
   - Alternative: parsing the numbers into `i64` by hand. This gives the same output with more code. Note that nothing in the spec says `to_i64` accepts a `str`; using it on one would most likely be refused.
3. **Extern declaration.** I copied the signatures exactly as section 13 gives them, with `arg: str` not marked `lent`, and named only the header, with no `link`, because the spec says the header is "linked where a group names one".
   - Alternative: adding `link "hero_os"`. This could fail at link time if no such library exists.
   - Alternative: changing `str` to `cstr`. This would be refused, because outside a group nothing answers `cstr` and the types would not match the spec's signature.
4. **Splitting.** There is no built-in split, so I wrote `split_on` using `s[i]` (a `u8`) and `slice`. The task says words are separated by single spaces, so a byte-by-byte split is exact.
   - Alternative: splitting with `chars()`. This also works, but is slower.
   - Alternative: comparing `s[i]` with a `str` such as `" "`. The compiler refuses this as a type mismatch.
5. **Checking stop words.** I wrote a `contains` loop because there are no anonymous functions or closures.
   - Alternative: `stop.any(...)` with a lambda. The compiler refuses this.
6. **Counting distinct words.** I used a `{str: bool}` map and `keys(seen).len()`.
   - Alternative: `seen.len()` on the map. The spec only shows `len` on arrays and strings, so this might be refused.
7. **Thread order.** All four `spawn` calls happen in the first loop, and all `join` calls in the second, as the task requires. Joining in `handles` order keeps the printed order the same as the documents.
   - Alternative: joining inside the spawn loop. This runs the threads one after another, against the task. The output would be the same, but the program would be wrong.
8. **Named arguments.** I named the extern call arguments (`body:`, `arg:`, `handle:`) even though their types differ and naming them is optional. I did not name the `split_on` and `contains` arguments, because their two parameter types differ.
   - Alternative: giving two parameters the same type without naming them at the call. The compiler refuses this.
9. **Plural.** The format line says `words` for every document, so `d` prints `1 words`. That is what the task's format asks for.
   - Alternative: special-casing the singular. This runs and gives output that doesn't match the asked format.

## prediction

- **Compiler:** accepts the program.
- **Output, in order:**
  ```
  a: 4 words, 4 distinct
  b: 3 words, 3 distinct
  c: 4 words, 3 distinct
  d: 1 words, 1 distinct
  ```
  - a: `cat sat on mat` gives 4 words, 4 distinct.
  - b: `dog and cat` gives 3 words, 3 distinct.
  - c: `be or not be` gives 4 words, 3 distinct.
  - d: `x` gives 1 word, 1 distinct.
- **Ending:** normally. No lease or handle is left owed, so nothing aborts when `main` returns.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory were read. Other things that reached my context:

- The agent-SDK system framing, which gave the working directory and the date.
- A list of labels in the user message (`t1-y-1` … `t4-h-5`, `verdict`). These carry no instructions or content.

No other rule, contract, memory or document reached my context.
~~~~~

### `t2-y-5`, now

~~~~~ markdown
I couldn't write `report.md` because I have no file-writing tool here, only Read. Following the brief's fallback, the whole report is below as my answer. I haven't built or run the program; the prediction comes from reading the spec only.

# report.md

## answer

**`main.hero`**
```
extern "hero_os.h"
    function hero_thread_spawn_str(body: (function(str) -> str), arg: str) -> i64
    function hero_thread_join_str(handle: i64) -> str

record Doc
    name: str
    text: str

function split(s: str, sep: u8) -> [str]
    parts: [str] @= []
    start @= 0
    for i in range(from: 0, to: s.len())
        if s[i] == sep
            parts @ parts.push(s.slice(from: start, to: i))
            start @ i + 1
    parts @ parts.push(s.slice(from: start, to: s.len()))
    return parts

function count_doc(input: str) -> str
    fields = input.split('\n')
    name = fields[0]
    stop = fields[1].split(' ')
    words = fields[2].split(' ')
    skip: {str: bool} @= {}
    for sw in stop
        skip[sw] @ true
    seen: {str: bool} @= {}
    total @= 0
    for w in words
        if !skip[w].default(false)
            total @ total + 1
            seen[w] @ true
    distinct = keys(seen).len()
    return f"{name}\n{total}\n{distinct}"

function main()
    docs = [
        Doc(name: "a", text: "the cat sat on the mat")
        Doc(name: "b", text: "a dog and a cat")
        Doc(name: "c", text: "to be or not to be")
        Doc(name: "d", text: "x")
    ]
    skip_words = ["the", "a", "to"]
    stop_line = join(skip_words, " ")
    handles: [i64] @= []
    for d in docs
        input = f"{d.name}\n{stop_line}\n{d.text}"
        handles @ handles.push(hero_thread_spawn_str(body: count_doc, arg: input))
    for h in handles
        reply = hero_thread_join_str(h)
        fields = reply.split('\n')
        print(f"{fields[0]}: {fields[1]} words, {fields[2]} distinct")
```

1. **Does `heroes build main.hero` accept it?** Yes, I expect so.
2. **Every line the program writes:**
   - `a: 4 words, 4 distinct`
   - `b: 3 words, 3 distinct`
   - `c: 4 words, 3 distinct`
   - `d: 1 words, 1 distinct`
3. **How does it end?** Normally, with no abort.

## choice_points

1. **Packing the input into one string.** Only one `str` can cross into a thread, so I joined name, stop words and text into one string with `\n` between them; none of the inputs contains a newline.
   - Other choice: using a space as the separator would mix the name and stop words into the text. The program would run but give a different answer.
2. **Packing the answer.** The thread returns `name\nwords\ndistinct`, and `main` splits it and prints the counts as the text the thread wrote. I did not parse them back into `i64`.
   - Other choice: parsing digits by hand (`s[i] - '0'`, then `to_i64(...).must()`) prints the same lines but adds code that can go wrong. Using `to_i64` directly on a `str` is refused by the compiler, because the spec only gives it number arguments.
3. **Declaring the thread functions.** The spec says threads come from `hero_os.h`, so I declared them in an `extern "hero_os.h"` group, with the signatures copied exactly from the spec.
   - Other choice: leaving the declarations out means the compiler refuses the program for unknown names.
4. **No `link` on that group.** The header belongs to the language, and the spec says linking happens "where a group names one."
   - Other choice: adding `link "hero_os"` may fail at link time if no such library exists.
5. **No `lent` on `arg: str`.** I copied the spec's signature as written, since the spec says the string is copied anyway.
   - Other choice: adding `lent` changes the declared signature and might be refused.
6. **Named arguments on the spawn call (`body:`, `arg:`).** These are optional because the two parameters have different types. Positional arguments would behave the same.
7. **Stop words held in a map.** I used a `{str: bool}` map with `.default(false)`. A hand-written `contains` loop gives the same result. The built-in `any` doesn't work here, because it would need the stop list captured in a function, and the language has no anonymous functions; the compiler would refuse it.
8. **Distinct counted after removing stop words.**
   - Other choice: counting distinct words including stop words would run but print different numbers, for example `c: 4 words, 4 distinct`.
9. **Splitting on single spaces with no trimming.** The task says words are separated by single spaces. Trimming or merging repeated spaces would make no difference for these inputs.
10. **All spawns first, then joins in document order.** This matches the task and fixes the output order.
    - Other choice: joining inside the spawn loop would print the same lines but break the rule that all four threads start before any is waited for.
11. **Order in an expression.** `!skip[w].default(false)` relies on calls and `.` binding more tightly than unary `!`, as the spec states. Without that rule, `!` would be applied to a `bool?` and the compiler would refuse it.
12. **The 4-document list written across several lines.** Each line ends in `)`, so the line break separates the items. Commas would also work.

## prediction

- **Compiler:** accepts the program.
- **Output, in order:**
  - `a: 4 words, 4 distinct` (cat, sat, on, mat)
  - `b: 3 words, 3 distinct` (dog, and, cat)
  - `c: 4 words, 3 distinct` (be, or, not, be; distinct: be, or, not)
  - `d: 1 words, 1 distinct`
- **Ending:** normal, with exit status 0 and no abort. No leases or handles are left open.

## context

Apart from the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), I received:
- the agent's system prompt, which says it runs on the Claude Agent SDK and gives the working directory path and the date;
- the user's message, which also lists run labels (`t1-y-1` … `verdict`). Those labels carried no content about the language.

No other rules, memories or documents reached me.
~~~~~

### `t3-c-1`, q1-k

~~~~~ markdown
I couldn't write `report.md` because I can only read files here, not write them. As `brief.md` allows, the whole report is below.

# report.md

## answer

**Files** (unchanged from `task.md`; the task asks for a prediction, not new code):

`signal_lib.h`: exactly as given in `task.md`.

`main.hero`:
```
extern "signal_lib.h"
    function called_install(sig: i32, answer: (function(i64) -> i64)) -> i32
    function called_raise(sig: i32) -> i32
    function called_seen() -> i64

extern "signal.h"
    constant SIGUSR1: i32

function answer(sig: i64) -> i64
    print("the answer ran for signal ", sig)
    return sig * 2

function main()
    print("installed: ", called_install(sig: SIGUSR1, answer: answer))
    print("raised: ", called_raise(sig: SIGUSR1))
    print("seen: ", called_seen())
```

**1. Does `heroes build main.hero` accept the program?** Yes.
- The C parameters are declared at C's own width and sign: `int32_t` becomes `i32` and `int64_t (*)(int64_t)` becomes `(function(i64) -> i64)`. The results and the constant `SIGUSR1: i32` also match the header (§13: "clang checks every result type, constant and record field against that header"; "A parameter … declared at the header's own width and sign").
- The callback is a parameter, which is the only place a callback may be (§13: "A callback is a **parameter**, never a result…"). `answer` is a top-level function with no `@` or `lent` parameter, so it may be used as a value (§9).
- Each group's header is a separate file, so header order doesn't matter (§13).
- No binding goes unused (§5). Every value printed is a number or `str` (§11). `sig * 2` is `i64` arithmetic (§7).
- Named arguments are allowed even where they are not required (§9, `Arg = [ ident ":" ] …`).
- Whether a C function stores the callback inside a signal handler can't be seen from its signature. So the compiler has nothing to refuse.

**2. What the program writes, and how it ends:**
1. `installed: 0`. `sigaction` succeeds and returns 0. At this point it only stores a function pointer; no Heroes function is called yet.
2. Nothing else. `called_raise` runs while the arguments of the second `print` are being evaluated. That happens before the `print` writes anything (§7: "An expression's parts are evaluated left to right"; §11: `print` writes its values only once it has them). `raise` delivers SIGUSR1 at once, so `called_handler` runs and calls `called_answer`, which is the program's `answer`. §13 says a callback is never "called by one [a signal handler], which aborts where the handler was installed and left through the C library's own names". Here the handler was installed through libc's `sigaction` and runs through libc's `raise`, so the call aborts.

- **Is `the answer ran for signal 30` among the lines?** No. The abort happens when the signal handler calls the callback, before the body of `answer` runs.
- **`raised: …` and `seen: …`:** never written.
- **How it ends:** by an abort (§6: "An abort ends the program at once, saying why"). The message says a program function (`answer`) was called from a signal handler (SIGUSR1). The spec doesn't give the exact wording.

## choice_points

1. **Whether the abort happens before the body of `answer` runs.** I chose that the check happens at the call boundary, so the body never runs. The other reading is that the body runs first and the abort comes on return. That run would also print `the answer ran for signal 30` after `installed: 0`, then abort. That is a different answer from the one intended.
2. **Whether the compiler refuses the program because the callback reaches a signal handler.** I chose acceptance: the extern signature can't show it, and §13 describes the case with "aborts" rather than as a compile error. The other choice is a program the compiler refuses.
3. **Whether "past them is the binding's word" covers this program.** I chose that it doesn't: installation (`sigaction`) and delivery (`raise`) both go through the C library's own names. The other reading makes the behaviour undefined. The run might then print `the answer ran for signal 30`, `raised: 0` and `seen: 60`, and end normally, which is a different answer.
4. **Whether the extern parameter name `answer` counts as shadowing the top-level function `answer` (§5).** I chose that it doesn't: an extern member has no body, so its parameter names bind nothing in scope. The other choice is a program the compiler refuses for shadowing.
5. **The abort's text and exit status.** The spec doesn't give them, so I described the content and didn't quote exact wording.

## prediction

- **Compiler:** accepts the program.
- **Output, in order:** one line:
  ```
  installed: 0
  ```
- **How it ends:** by an abort while `called_raise` is running. The message says a program function, `answer`, was called from a signal handler (SIGUSR1). `the answer ran for signal 30`, `raised: …` and `seen: …` are never written.

## context

The only directory files that reached my context were `brief.md`, `task.md` and `heroes-spec.md`. From outside the directory, two things did:
- The system prompt, which says I am a Claude agent built on the Claude Agent SDK and gives the working directory and the date.
- The user's message, which, after the instruction, listed labels such as `t1-y-1 … t4-h-5` and `verdict`. I didn't use them.

No other instruction, rule, contract, memory or document reached my context.
~~~~~

### `t3-c-2`, q1-k

~~~~~ markdown
I couldn't write `report.md` because I only have a file-reading tool here. As `brief.md` says to do in that case, the whole report follows.

# report.md

## answer

**1. Does `heroes build main.hero` accept the program?** Yes. Nothing in it breaks a compile-time rule:

- **Declarations match the header.** `called_install(sig: i32, answer: (function(i64) -> i64)) -> i32` matches `int32_t called_install(int32_t, int64_t (*)(int64_t))`. §13 says a parameter is declared "at the header's own width and sign" and that "a callback is a **parameter**", with parameters following the same rule. `called_raise` (int32_t → `i32`) and `called_seen` (int64_t → `i64`) also match. `constant SIGUSR1: i32` is a group constant with no body, which §13 allows: "the header holds the value".
- **`answer` can be passed as a value.** §9 says "Top-level functions are values, but not one with an `@` or `lent` parameter", and `answer` has neither.
- **Named arguments are allowed.** The two parameters of `called_install` have different types, so names aren't required (§9), but `Arg = [ ident ":" ] ...` permits them.
- **No unused bindings.** `sig` is read in `answer`, and every value is used by `print`, so §5 is satisfied.
- **`print` takes these types.** It accepts numbers and `str` (§11), and every argument here is one of those.
- **The signal-handler rule doesn't stop the build.** §13 says a callback is never "a signal handler, nor called by one, which aborts where the handler was installed … through the C library's own names". That is a runtime abort, not a compile error. The compiler can't see that `called_handler`, a C function, calls the callback.

**2. What the program writes and how it ends.**

```
installed: 0
```

Then the program aborts. Step by step:

- **Line 1.** `called_install` stores `answer` and installs `called_handler` with `sigaction`, which returns 0. `print` writes its values with no separator and one newline (§11), so the line is `installed: 0`.
- **The abort.** `called_raise(sig: SIGUSR1)` raises signal 30, and `called_handler` runs synchronously inside `raise`. The handler calls the program's callback `answer`. That is a callback "called by" a signal handler that was installed through the C library's own name `sigaction`. §13 says that case "aborts". §6 says "An abort ends the program at once, saying why". The spec doesn't give the exact wording; it will say that a callback was called from a signal handler.
- **`the answer ran for signal 30` is not printed.** The abort happens at the call, so the body of `answer` never runs.
- **The last two lines never appear.** `raise` never returns, so `raised: …` and `seen: …` are not written.

The program ends with an abort, not normally.

No files needed writing: the task asks for a prediction, and `main.hero` and `signal_lib.h` are given whole in `task.md`.

## choice_points

1. **Is an extern parameter named `answer` shadowing the top-level function `answer`?** (§5: "Shadowing is a compile error".)
   - **My choice:** no. An extern member has no body, so its parameter names bind nothing in any scope.
   - **The other reading:** the compiler refuses the program, citing §5.
2. **When does the §13 signal-handler rule fire?**
   - **My choice:** at runtime, when the handler calls the callback, and before the callback's body runs. The handler was installed through `sigaction`, a C library name, so this is the "aborts" case, not the "binding's word" case.
   - **Reading the abort as coming after `answer`'s body:** `the answer ran for signal 30` would be printed before the abort.
   - **Reading the call as allowed:** the program prints `the answer ran for signal 30`, `raised: 0`, `seen: 60` and ends normally.
   - **Treating it as a compile error:** the build refuses, but the compiler can't see inside C, so I rejected this.
3. **Is output written before the abort kept?** I took §6 ("ends the program at once, saying why") to mean that lines already printed stay printed. If the abort discarded buffered output, `installed: 0` could disappear when stdout isn't a terminal.
4. **What does `sigaction` return?** I assumed it succeeds and returns 0, as it does for SIGUSR1 on a normal system. A failure would print `installed: -1`, and the program would then be killed by SIGUSR1's default action.

## prediction

- **Compiler:** accepts the program.
- **Output:** one line, `installed: 0`.
- **Ending:** an abort during `called_raise`, saying that a callback was called from a signal handler (§13). The spec doesn't fix the exact text.
- `the answer ran for signal 30`, `raised: …` and `seen: …` are never written.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md`, three other things reached my context:

- **The system prompt.** It says I am a Claude agent built on the Claude Agent SDK and gives the working directory and date.
- **The user's message.** Besides the instruction, it held a list of identifiers that look like other test cases (`t1-y-1` … `t4-h-5`, `verdict`).
- **The tool limits.** My only tool can read files, not write them.

No other rules, memory files or documents (such as a CLAUDE.md) reached my context, and I read no files outside this directory.
~~~~~

### `t3-c-3`, q1-k

~~~~~ markdown
I can't create files with the tools I have (I can only read), so the full report is below instead of in `report.md`, as the brief allows.

# report.md

## answer

**1. Does `heroes build main.hero` accept the program?** Yes. Each part of `main.hero` meets the specification:

- **The extern groups are well-formed.** Each group names its header. C reads the headers in the order the groups are written (§13), so `signal_lib.h` comes before `signal.h`. `signal_lib.h` already includes `<signal.h>` itself, so the order causes no problem.
- **Parameter widths match the header.** §13 requires a parameter "at the header's own width and sign". `sig: i32` matches `int32_t sig`. The callback's own parameter `i64` matches `int64_t`.
- **Result types match.** The `-> i32` results match `int32_t`, and `called_seen() -> i64` matches `int64_t`.
- **`constant SIGUSR1: i32` passes.** A group's constant has no body (§13). clang checks it against the header, and C's `int` 30 fits in `i32`.
- **The callback is a parameter, which is allowed.** §13 says "A callback is a **parameter**, never a result", and its parameters follow the same width rule. So `answer: (function(i64) -> i64)` is a legal parameter type. The parentheses around a function type are mandatory (§3), and they are written.
- **`answer` can be passed as a value.** It is a top-level function with no `@` or `lent` parameter, so it is a value (§9). Its signature `(sig: i64) -> i64` matches the parameter type.
- **The arguments are fine.** Named arguments are optional here because no two parameters share a type, and writing them is allowed (`Arg = [ ident ":" ] ...`). `print` takes `str`, `i32` and `i64` (§11). Each `print(...)` call returns `()` and stands alone as a line, as §5 requires.
- **Nothing is unused.** `sig` is read in `answer`, and there are no cells.
- **`main` is correct.** It takes nothing and returns nothing (§1). `answer` returns on its only path (§8).

The specification forbids nothing else here. A callback being *called by* a signal handler is a run-time fact, not a compile-time one: the compiler cannot see that the C code in `called_handler` calls the function pointer.

**2. Lines written, and how the program ends.**

1. `installed: 0`. `called_install` stores the pointer and calls `sigaction`, which succeeds and returns 0. `print` writes its values with no separator and one trailing newline (§11).
2. Then `called_raise(sig: SIGUSR1)` runs `raise(30)`. The signal is delivered at once, and `called_handler` runs and calls `called_answer`, which is the Heroes function `answer`. §13 says a callback is "never … a signal handler, nor called by one, which aborts where the handler was installed and left through the C library's own names". The handler here was installed through libc's `sigaction`, and it runs and returns through libc's own signal path from `raise`. So this call aborts, and the "binding's word" exception ("past them") does not apply. Per §6, "An abort ends the program at once, saying why."

The line `the answer ran for signal 30` is **not** among the lines written: the call into `answer` aborts, so its body does not run. `raised: …` and `seen: …` are never written either.

**The program ends by an abort**, after the single line `installed: 0`. The abort says that a callback (`answer`) was called by a signal handler. The specification does not give the exact wording, only that an abort says why (§6).

No files were written. The task only asks for analysis, and the given `main.hero` is used unchanged.

## choice_points

1. **Reading "nor called by one, which aborts where the handler was installed and left through the C library's own names" (§13).**
   - **My choice:** a Heroes callback called from a signal handler that was installed with `sigaction` and entered through `raise` aborts at the call.
   - **The other reading:** this is merely forbidden and undefined, or allowed. Then the program would run and give a different answer: `the answer ran for signal 30`, `raised: 0`, `seen: 60`, ending normally.
   - **Also rejected:** treating it as a compile-time refusal. The compiler cannot see C calling the pointer, and the sentence says "aborts".
2. **Whether the abort happens before or after `answer`'s body runs.**
   - **My choice:** before. The call itself aborts, so `the answer ran for signal 30` is not written.
   - **The other reading:** the body runs and the program aborts on return. That would write `the answer ran for signal 30` before the abort.
3. **Whether the extern parameter named `answer` shadows the top-level function `answer` (§5, "Shadowing is a compile error").**
   - **My choice:** no. An extern member has no body, so its parameter names bind nothing in scope.
   - **The other reading:** the compiler refuses the program for shadowing.
4. **`constant SIGUSR1: i32` against C's `int` macro.**
   - **My choice:** accepted, since clang checks it and 30 fits.
   - **The other reading:** a constant must be `i64`, as in the sqlite example. Then the program is refused, or `called_install(sig: SIGUSR1…)` is refused for passing an `i64` where an `i32` is needed (§3, no implicit conversions).

## prediction

- **Compiler:** accepts the program.
- **Output, in order:**
  - `installed: 0`
  - then an abort saying that a callback was called by a signal handler (the specification does not fix the exact text).
- **Lines not written:** `the answer ran for signal 30`, `raised: …` and `seen: …` never appear.
- **How it ends:** by an abort, not normally.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md` in this directory, my context held:
- the agent-SDK system prompt, which gave the working directory path and the date;
- the user's instruction, followed by a list of tokens (`t1-y-1` … `t4-h-5`, `verdict`) that look like run labels. I did not use them.

No other file, rule, contract, memory or document reached my context, and I read no other file.
~~~~~

### `t3-c-4`, q1-k

~~~~~ markdown
I couldn't write `report.md` because I can only read files here, so the brief says to give the whole report as my final answer. Here it is.

# report.md

## answer

The task asks two questions about the existing files `signal_lib.h` and `main.hero`. It doesn't ask me to write a program, so I wrote no files.

**1. Does `heroes build main.hero` accept the program?** Yes.

- **The callback parameter is allowed.** §13 says "A callback is a **parameter**, never a result". `answer: (function(i64) -> i64)` is a parameter of `called_install`. Its parameter and result follow the same width rule ("its parameters follow the same rule"), and `i64` matches C's `int64_t (*)(int64_t)`.
- **`answer` can be passed as a value.** §9: "Top-level functions are values, but not one with an `@` or `lent` parameter". `answer` has neither.
- **The other parameters and results match the header.** §13: "A **parameter** … declared at the header's own width and sign". `sig: i32` matches `int32_t`. The results `-> i32` and `-> i64` match `int32_t` and `int64_t`, and clang checks result types against the header.
- **The constant is fine.** `constant SIGUSR1: i32` has no body (§13: "A group's `constant` has no body: the header holds the value"). It is an int macro, which clang checks.
- **Named arguments are optional here.** §9 makes them mandatory only when two parameters share a type. Here they don't, so writing them is allowed but not required.
- **No unused bindings.** `sig` is read in `answer` (§5).
- **`print` takes these values.** It accepts numbers, so `i32` and `i64` are fine (§11).
- **The compiler can't see the rule being broken.** The only rule that applies is that a callback is never "called by" a signal handler (§13). That depends on what C code does at run time, and §13 says it is enforced then ("which aborts"). It is not a compile error.

**2. What the program writes, and how it ends.**

1. `installed: 0`. `called_install` calls `sigaction`, which succeeds and returns 0. `main` prints it (§11: no separator, one trailing newline).
2. Then `called_raise` calls `raise(30)`. The C library runs `called_handler` at once, and the handler calls `called_answer`, which is the Heroes function `answer`. §13 says a callback is "never a result or a signal handler, **nor called by one**, which aborts where the handler was installed and left through the C library's own names". This handler was installed with the C library's `sigaction` and entered through the C library's `raise`. So calling the callback aborts, before the body of `answer` runs.

**The line `the answer ran for signal 30` is not among them.** The program ends **by an abort**. §6 says "An abort ends the program at once, saying why", so the message says a Heroes function was called from a signal handler. The spec doesn't give the exact wording. `raised: …` and `seen: …` are never printed.

## choice_points

1. **Does `called_install` (the header's own wrapper around `sigaction`) count as installing "through the C library's own names"?**
   - **My choice:** yes. The actual installing call is libc's `sigaction`, and the signal is delivered through libc's `raise`. So the language's guarantee applies and the program aborts.
   - **The other reading:** the wrapper is "past them", so it is "the binding's word" (the language guarantees nothing). Then the program would most likely run and give a different answer: `installed: 0`, `the answer ran for signal 30`, `raised: 0`, `seen: 60`, and end normally. That still relies on `print` working inside a signal handler, which isn't guaranteed.
2. **Does the abort happen before the callback's body runs?**
   - **My choice:** yes. §13 says the call itself aborts, so `the answer ran for signal 30` is not printed.
   - **The other reading:** the check happens after the body. Then that line would appear just before the abort.
3. **Is `installed: 0` actually written before the abort?**
   - **My choice:** yes. §11 says `print` writes a line, and §6's abort doesn't take back earlier output.
   - **The other reading:** output still sitting in a buffer is lost when the program aborts. Then the program would show only the abort message.
4. **Can the compiler see this misuse?**
   - **My choice:** no. It can't see that the C handler calls the callback, and §13 says this is a run-time abort.
   - **The other reading:** the compiler refuses the program. That would make question 2 moot.

## prediction

- **Compiler:** accepts the program and builds it.
- **Lines written, in order:**
  1. `installed: 0`
- **How it ends:** by an abort when the signal handler calls the callback `answer`. The message says a program function was called from a signal handler; the spec doesn't fix its exact text. `the answer ran for signal 30`, `raised: 0` and `seen: 60` are not printed.

## context

- **Files I read:** only `brief.md`, `task.md` and `heroes-spec.md` in this directory.
- **Other things that reached my context:**
  - The environment's system prompt, saying I'm an agent on the Claude Agent SDK, with the working-directory path and the date.
  - The user's message, which held a list of labels (`t1-y-1` … `t4-h-5`, `verdict`) whose meaning isn't explained. I didn't use them.
- No other rules, project instructions, memories or documents reached my context.
~~~~~

### `t3-c-5`, q1-k

~~~~~ markdown
I couldn't save `report.md` because I can only read files here, not write them. The brief says to give the whole report as the final answer in that case, so here it is.

# report.md

## answer

No files are written: the task asks for a prediction, not a program. The two files are `signal_lib.h` and `main.hero`, both given whole in `task.md` and not changed.

**1. Does `heroes build main.hero` accept the program?** Yes.

- **The `signal_lib.h` group.** §13 says a parameter must be declared at the header's own width and sign.
  - `sig: i32` matches `int32_t`.
  - `answer: (function(i64) -> i64)` matches `int64_t (*)(int64_t)`. A callback is allowed as a **parameter** (§13: "A callback is a parameter, never a result…"), and its own parameter and result follow the same width rule (`int64_t` ↔ `i64`).
  - The results `i32` (`int32_t`) and `i64` (`int64_t`) match what clang checks against the header. `called_seen()` takes nothing.
- **The `signal.h` group.** `constant SIGUSR1: i32` has no body, as a group's constant must not (§13). The header holds the value as an `int` macro. The rule that a constant may not be read through an enumerator or object applies only "in a package named by `version` or `git`". This program uses no package.
- **Group order.** `signal_lib.h` includes `<signal.h>` itself, so neither group needs the other's names first (§13).
- **`answer` as a value.** It is a top-level function with no `@` or `lent` parameter, so it may be passed as a value (§9). Its type `(function(i64) -> i64)` matches the parameter.
- **Named arguments.** `sig: i32` and the function parameter have different types, so labels are not required. They are allowed, though (`Arg = [ ident ":" ] …`, §9), and they are the declared names in the declared order.
- **`print` arguments.** `print` takes a `str` and a number (§11). `i32` and `i64` are numbers.
- **Unused things.** `sig` is used in `answer`, and no binding goes unused (§5).
- **`main`.** It takes nothing and returns nothing (§1).
- **Why the compiler can't refuse it.** The forbidden use is C calling the callback from inside a signal handler. That happens inside C (`called_handler`), so the compiler can't see it. §13 calls it a runtime abort, not a compile error.

**2. What it writes and how it ends**

Lines written, in order:

1. `installed: 0`. `called_install` runs `sigaction(30, …)`, which succeeds and returns 0. The installed handler is `called_handler`, and it calls the program's function `answer`.
2. Nothing more. `called_raise(sig: SIGUSR1)` calls `raise(30)`, which runs `called_handler` at once on this thread. The handler then calls the callback `answer`. §13 says: "A callback is a parameter, never a result or a signal handler, **nor called by one, which aborts** where the handler was installed and left through the C library's own names". This handler was installed with `sigaction` and entered through `raise`, both the C library's own names. So the call into `answer` aborts before its body runs.

The line `the answer ran for signal 30` is **not** among them. Neither are `raised: …` and `seen: …`.

**How it ends:** by an abort, as soon as the signal handler calls the callback. §6 says "An abort ends the program at once, saying why". The spec doesn't give the exact wording, but it will say that a program function (`answer`) was called from a signal handler. It also cannot run the abort that §13 attaches to `main` returning (for a lease nobody ended), because `main` never returns. This program has no leases or handles anyway.

## choice_points

1. **How to read "nor called by one, which aborts where the handler was installed and left through the C library's own names and past them is the binding's word".**
   - **My choice:** a callback that a signal handler calls aborts whenever that handler was installed through libc (`sigaction`/`signal`) and entered through libc (`raise`/the kernel). That is the case here, so the program aborts during `raise`. Only handlers installed some other way are "the binding's word".
   - **The other reading:** the check covers only callbacks the program hands *directly* as the handler. Calling through a C trampoline like `called_handler` would then be "past them", the binding's word, with no check. The program would run and print `installed: 0`, `the answer ran for signal 30`, `raised: 0` and `seen: 60`, then end normally. That is the answer it was written to expect, but the spec forbids it, so it would be a different answer than the spec intends.
2. **Whether the abort comes before or after the callback's body runs.**
   - **My choice:** before. "never … called by one, which aborts" means the call itself aborts.
   - **The other reading:** the abort is noticed only when the callback returns into C. Then `the answer ran for signal 30` would print first, followed by the same abort.
3. **Whether a compiler that sees `called_handler` (clang reads the header) refuses the program at build time.**
   - **My choice:** no. §13 calls this an abort, which happens at runtime.
   - **The other reading:** the compiler refuses the build, and no binary or output exists.
4. **Whether `installed: 0` is visible before the abort.**
   - **My choice:** yes. `print` writes a complete line, and §6 lets the abort "say why", which suggests output written before it is kept.
   - **The other reading:** buffered output is lost on abort, and nothing appears except the abort message.
5. **Labels on calls whose parameters have different types.**
   - **My choice:** allowed, because the grammar allows them.
   - **The other reading:** if labels were refused there, the compiler would reject `called_install(sig: …, answer: …)` and `called_raise(sig: …)`.

## prediction

- **Does it compile?** Yes, the compiler accepts it.
- **Output:**
  ```
  installed: 0
  ```
- **How it ends:** an abort during `called_raise`, when the `SIGUSR1` handler calls the callback `answer`. The message says a program function was called from a signal handler (the spec doesn't fix the wording).
- **Lines that never print:** `the answer ran for signal 30`, `raised: …` and `seen: …`.

## context

- **The run environment.** The environment's own instructions reached my context: I was told I am an agent built on Anthropic's Claude Agent SDK, given the working directory path, given the date 2026-11-19, and given rules for using tools.
- **The user's message.** It contained, besides the instruction, a list of tokens (`t1-y-1` … `t4-h-5`, `verdict`). It did not come from these files, and I did not use it.
- **What I read.** Only `brief.md`, `task.md` and `heroes-spec.md` from this directory. No other file, memory or document.
~~~~~

### `t3-y-1`, now

~~~~~ markdown
I couldn't write `report.md` because I can only read files here, not create them. As the brief allows, the full report is below.

# report.md

## answer

**1. Does `heroes build main.hero` accept the program?** Yes. I don't think the spec gives the compiler any rule that would refuse it.

- **The callback's type matches the header.** C declares `called_install(int32_t sig, int64_t (*answer)(int64_t))`. The program declares `sig: i32` and `answer: (function(i64) -> i64)`, returning `i32`.
  - §13 says a parameter is "declared at the header's own width and sign".
  - A callback must be "a **parameter**, never a result". Here it is a parameter.
  - "its parameters follow the same rule": `i64` matches `int64_t`.
- **The function value is allowed.** `answer` is a top-level function with no `@` or `lent` parameter, so it may be used as a value (§9).
- **The other declarations match the header.** `called_raise(sig: i32) -> i32` and `called_seen() -> i64` match it. So does `constant SIGUSR1: i32`, which matches `int` (clang checks constants, §13).
- **Header order is fine.** `signal_lib.h` includes `<signal.h>` itself, so the `"signal.h"` group coming second needs no names from the first (§13).
- **The rest of the program is valid.**
  - `print` takes numbers and `str` (§11).
  - Nothing is unused: `sig` is read, and every result is printed (§5).
  - Labels at the call are optional because the two parameter types differ (§9).
  - `sig * 2 = 60` does not overflow (§7).

The rule this program breaks is in §13: "A callback is a parameter, never a result or a signal handler, **nor called by one**", and "No function of the program is a signal handler". The program never declares anything that is a signal handler. The breach exists only in C code: `called_handler` calls the stored pointer `called_answer` when `SIGUSR1` arrives. The spec gives no rule by which the compiler analyses what a header's C functions do with a pointer it was handed. So the build is accepted, and the rule can only take effect when the program runs.

**2. What the program writes and how it ends**

```
installed: 0
```

- `sigaction` succeeds and returns 0.
- `called_raise(sig: SIGUSR1)` sends signal 30. `called_handler` runs and calls `called_answer(30)`, which is the program's `answer`.
- §13 says no function of the program is called by a signal handler. The only way the language can keep that promise once C makes the call is to refuse to run `answer`. §6 says "An abort ends the program at once, saying why".
- So the program **ends with an abort**. The abort says a function of the program was called from a signal handler. The spec doesn't give the exact wording.
- **`the answer ran for signal 30` is not among the lines.** `raised: …` and `seen: …` are never written either.

## choice_points

1. **Does the compiler or the running program enforce "never … called by one"?**
   - I chose: the build accepts, and the program aborts when the callback is entered from the handler. The declaration is a legitimate callback parameter, and nothing in the spec makes the compiler inspect the C bodies.
   - The other reading: the compiler refuses. In that case the program is never built and writes nothing.
2. **What happens when the callback is actually entered from the handler?**
   - I chose: an abort, which is the only way the language can keep §13's promise.
   - The other reading: the spec's rule is only advice, and the call goes through. The program would then run normally and give a different answer than I predict. It would print `installed: 0`, `the answer ran for signal 30`, `raised: 0`, `seen: 60`, and end normally. That assumes the `print` inside the handler happens to work, which is not async-signal-safe.
3. **Does the name `answer` count as shadowing?** The extern parameter `answer` has the same name as the top-level function `answer`.
   - I chose: an extern member's parameter names bind nothing in file scope, so there is no shadowing (§5).
   - The other reading: the compiler refuses the program for shadowing.
4. **Is `installed: 0` actually written before the abort?**
   - I chose: yes. `print` writes its line (§11), and the line is out before the abort.
   - The other reading: if output were buffered and the abort dropped it, the program would write no lines at all.
5. **Labels at the call.** I wrote `called_install(sig: SIGUSR1, answer: answer)` as given. Labels are optional here because the two parameter types differ. Leaving them out would change nothing.

## prediction

- **Compiler:** accepts the program.
- **Lines written, in order:**
  1. `installed: 0`
- **How it ends:** by an abort while handling `SIGUSR1` (30). The callback `answer` was called by a signal handler, which §13 forbids. The spec doesn't give the abort's exact wording. `the answer ran for signal 30`, `raised: 0` and `seen: 60` are not written.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. Besides them I had only the system prompt and the user's message. The message held the instruction plus a list of labels (`t1-y-1` … `t4-h-5`, `verdict`) with no content behind them. No other rule, contract, memory or document reached me.
~~~~~

### `t3-y-2`, now

~~~~~ markdown
I couldn't create `report.md` because I only have a file-reading tool here, so the whole report follows instead, as the brief allows.

---

# report.md

## answer

**1. Does `heroes build main.hero` accept the program?** Yes, as far as anything the specification makes the compiler check.

- **The extern group matches the header.** "clang checks every result type, constant and record field against that header" (§13):
  - `called_install`, `called_raise` and `called_seen` return `int32_t`, `int32_t` and `int64_t`, declared as `i32`, `i32` and `i64`.
  - `SIGUSR1: i32` matches C's `int` macro. It is a plain macro: not a float, a record or a value "read through an enumerator or object", and no `version`/`git` package is involved.
- **The parameters have C's width and sign** ("i32 where C says int", §13). `sig: i32` matches `int32_t`. `answer: (function(i64) -> i64)` matches `int64_t (*)(int64_t)`.
- **A callback is allowed in parameter position.** "A callback is a **parameter**, never a result…; its parameters follow the same rule" (§13).
- **`answer` may be passed as a value.** "Top-level functions are values, but not one with an `@` or `lent` parameter" (§9), and `answer` has neither.
- **The labelled arguments are allowed.** `Arg = [ ident ":" ] …` (§9) permits labels even where they aren't required.
- **`print` accepts every result.** Each call's result is an `i32` or `i64`, and `print` takes "a number, `str` or `bool`" (§11).
- **Nothing is unused, nothing is dropped and nothing is fallible.** `main` takes nothing and returns nothing (§1). `answer` returns on its only path (§8). `sig * 2` = 60 does not overflow (§7).

The only thing wrong with the program is a runtime fact: C calls the callback from a signal handler. The Heroes source can't show that, so it is not something the specification says the compiler refuses.

**2. Lines written, in order, and how the program ends.**

1. `installed: 0`
   - Arguments are evaluated left to right (§7). `called_install` stores the function and calls `sigaction`, which returns 0 on success, and `print` writes `installed: ` followed by `0` with no separator and one newline (§11).
2. Next, `called_raise(sig: SIGUSR1)` raises signal 30, and C's `called_handler` runs.
   - The handler calls the Heroes function `answer` through the pointer it stored.
   - The specification forbids this: "A callback is a **parameter**, never a result or a signal handler, **nor called by one**", and "No function of the program is a signal handler" (§13).
   - The language has no undefined behaviour: "An abort ends the program at once, saying why" (§6). So this rule must be enforced when the callback is entered.
   - **The program therefore aborts at that point, before `answer`'s body runs.** The abort says that a function of the program was called from a signal handler. The specification does not give the exact wording.

- **Is `the answer ran for signal 30` among the lines?** No.
- **What about `raised: 0` and `seen: 60`?** They are not written either.
- **How does it end?** By an abort, after the single line `installed: 0`.

Files: I wrote no new file. The program is the task's `main.hero`, unchanged:

`main.hero`
```
extern "signal_lib.h"
    function called_install(sig: i32, answer: (function(i64) -> i64)) -> i32
    function called_raise(sig: i32) -> i32
    function called_seen() -> i64

extern "signal.h"
    constant SIGUSR1: i32

function answer(sig: i64) -> i64
    print("the answer ran for signal ", sig)
    return sig * 2

function main()
    print("installed: ", called_install(sig: SIGUSR1, answer: answer))
    print("raised: ", called_raise(sig: SIGUSR1))
    print("seen: ", called_seen())
```

## choice_points

1. **How the "never called by a signal handler" rule is enforced.**
   - **My choice:** the compiler accepts the program, and the callback aborts when a signal handler calls it. I chose this because the rule sits in §13 with no "compile error" attached, the compiler only checks types against the header, and the language allows only compile errors or aborts.
   - **Alternative A, refused at compile time:** if the build looked into the header and saw that `called_handler` calls the stored pointer, it would refuse the program.
   - **Alternative B, not enforced at all:** the program would run and print a different answer: `installed: 0`, `the answer ran for signal 30`, `raised: 0`, `seen: 60`, and end normally.
2. **The extern parameter named `answer` versus the top-level function `answer`** (§5: "Shadowing is a compile error").
   - **My choice:** a parameter of a bodiless extern member binds nothing in any scope, so it shadows nothing.
   - **Alternative:** if it counts as shadowing, the compiler refuses the program. The same question applies to the `sig` parameters.
3. **Labelled arguments where labels are not required.**
   - **My choice:** allowed, because of the `Arg` grammar.
   - **Alternative:** if labels were allowed only where two parameters share a type, the compiler would refuse the program.
4. **Whether `installed: 0` reaches the output before the abort.**
   - **My choice:** yes. `print` writes its line, and the abort comes later.
   - **Alternative:** if output were buffered and lost on abort, the program would write no lines at all and then abort.
5. **Where exactly the abort happens.**
   - **My choice:** on entry to the callback, so `answer`'s `print` never runs.
   - **Alternative:** if it were checked only when the callback returns, the line `the answer ran for signal 30` would appear first, then the abort.
6. **The order of the two groups.**
   - **My choice:** acceptable. `signal_lib.h` includes `<signal.h>` itself, and the `signal.h` group comes after it (§13: "one that needs another's names comes after it").
   - **Alternative:** none that changes the outcome.

## prediction

- **Does the compiler accept it?** Yes.
- **What does it write?** One line: `installed: 0`.
- **Is `the answer ran for signal 30` among the lines?** No.
- **How does it end?** By an abort when the signal handler calls the Heroes callback `answer`. The abort says that a function of the program was called from a signal handler, which §13 forbids. `raised: …` and `seen: …` are never written.

## context

- Besides `brief.md`, `task.md` and `heroes-spec.md`, two other things reached my context:
  - the agent system prompt, which gives the working directory path;
  - the user's message, which ends in a list of labels (`t1-y-1` … `t4-h-5`, `verdict`).
- No other rule, contract, memory or document reached my context, and I read no other file.
~~~~~

### `t3-y-3`, now

~~~~~ markdown
I couldn't save `report.md` because I only have a file-reading tool here, so the full report is below as the brief allows.

# report.md

## answer

**1. Does `heroes build main.hero` accept the program?**

No. Most likely the compiler refuses it, because the callback `answer` ends up being called by a signal handler. The spec does not say outright whether this is checked at compile time; see `choice_points`.

- **What breaks the rule.** `main` passes the program's function `answer` to `called_install` as a callback (`answer: (function(i64) -> i64)`). In the header, `called_install` stores it in `called_answer`. It then installs `called_handler` with `sigaction` as the handler for `sig`, which here is `SIGUSR1`. When the signal arrives, `called_handler` calls `called_answer`, so the program's own function runs inside a signal handler.
- **What the spec says.** Section 13: "A callback is a **parameter**, never a result or a signal handler, nor called by one". The next sentence makes it a guarantee of the language: "No function of the program is a signal handler". It points to the allowed alternatives for signals: `hero_signal_watch(sig: i32) -> i64` and `hero_signal_take(sig: i32) -> i64` from `hero_os.h`, or `hero_net_catch_stop` for Ctrl-C and SIGTERM.
- **Why it is a build-time refusal.** That sentence lists the forbidden roles together: being a result, being a signal handler, and being called by one. The first is plainly a refusal of the declaration, so the others read the same way. The spec says the compiler hands the header to clang ("clang checks every result type, constant and record field against that header"). Here the header holds the whole path in its own text: the stored pointer, the `sigaction` call that sets `sa_handler = called_handler`, and the call through `called_answer`. The spec names no run-time abort for breaking this rule, so the only enforcement it describes is refusing the program.

Everything else in the program is fine:
- The extern widths match the header: `int32_t` is `i32`, `int64_t` is `i64`, and the callback's parameter and result follow the same rule (section 13).
- `SIGUSR1: i32` is a group constant with no body (section 13).
- Named arguments are allowed even where they are not required (section 9).
- `print` accepts an `i32` and an `i64` (section 11).

Since the answer to question 1 is "no", I stop there, as the task says. Question 2 does not apply. The task asked for no new files, so I wrote none; the program is exactly the given `main.hero`.

## choice_points

1. **Is the "called by a signal handler" rule checked by the build, or is it only a promise?**
   - **My choice:** the build checks it and refuses the program (reasons above).
   - **If it is only a promise and the build accepts:** I expect the program to run and give an answer the spec does not promise. It would print `installed: 0`, then `the answer ran for signal 30` from inside the handler, then `raised: 0` and `seen: 60`, and end normally. Running Heroes code (including `print`) inside a signal handler is not async-signal-safe, so that output is not guaranteed.
   - **A third reading:** the runtime notices it is being called from a signal context and stops with an abort. The spec names no such abort or message.

2. **Does the extern parameter name `answer` shadow the top-level function `answer`?**
   - **My choice:** no. Parameter names in an extern declaration (which has no body) are labels, not bindings in a scope, so section 5's shadowing rule does not apply. The same goes for `sig`, which appears both in the extern declarations and as `answer`'s own parameter.
   - **The other reading:** the build refuses with a shadowing error (section 5), which is still a refusal, just for a different reason.

3. **Named arguments where the parameter types differ** (`called_install(sig:, answer:)`).
   - **My choice:** allowed. Section 9 makes names *mandatory* only when two parameters share a type, and the `Arg` grammar allows a name on any argument.
   - **The other reading:** the build refuses the labels. I see nothing in the spec that supports this.

## prediction

- **Does the compiler accept it?** No. `heroes build main.hero` refuses the program, because the callback `answer` is called by a signal handler (section 13: "never a result or a signal handler, nor called by one"; "No function of the program is a signal handler"). No binary is made.
- **Lines written:** none, because the program is never built or run. The line `the answer ran for signal 30` does not appear.
- **How it ends:** the build fails with a compile error, so there is no run and no abort.

## context

Nothing reached my context besides the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), the user's instruction, and the agent's system setup (working directory and date). The user's message also included a list of sibling directory names (such as `t1-y-1` and `verdict`); I did not read anything in them. No other rule, contract, memory or document was used.
~~~~~

### `t3-y-4`, now

~~~~~ markdown
I can only read files here, so I couldn't write `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

**Files** (as the task gives them, unchanged):

`signal_lib.h`
```c
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

static int64_t (*called_answer)(int64_t) = NULL;
static volatile sig_atomic_t called_got = 0;

static void called_handler(int sig) {
    (void)__builtin_frame_address(0);
    called_got = (sig_atomic_t)called_answer((int64_t)sig);
}

static inline int32_t called_install(int32_t sig, int64_t (*answer)(int64_t)) {
    struct sigaction sa;
    called_answer = answer;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = called_handler;
    sigemptyset(&sa.sa_mask);
    return sigaction(sig, &sa, NULL);
}

static inline int32_t called_raise(int32_t sig) { return raise(sig); }

static inline int64_t called_seen(void) { return (int64_t)called_got; }
```

`main.hero`
```
extern "signal_lib.h"
    function called_install(sig: i32, answer: (function(i64) -> i64)) -> i32
    function called_raise(sig: i32) -> i32
    function called_seen() -> i64

extern "signal.h"
    constant SIGUSR1: i32

function answer(sig: i64) -> i64
    print("the answer ran for signal ", sig)
    return sig * 2

function main()
    print("installed: ", called_install(sig: SIGUSR1, answer: answer))
    print("raised: ", called_raise(sig: SIGUSR1))
    print("seen: ", called_seen())
```

**1. Does `heroes build main.hero` accept it?** Yes, I predict it does.
- **Types match the header.** Section 13 says each parameter is declared at the header's own width and sign. `sig: i32` matches `int32_t`. The results `i32` and `i64` match `int32_t` and `int64_t`. `SIGUSR1: i32` is an int macro, and a group's `constant` has no body.
- **The callback is a parameter.** Section 13 says "a callback is a **parameter**, never a result". `(function(i64) -> i64)` matches `int64_t (*)(int64_t)`, and its parameters follow the same width rule.
- **`answer` may be passed as a value.** It is a top-level function with no `@` or `lent` parameter (§9).
- **Header order is fine.** The groups are read in the order written; `signal_lib.h` includes `<signal.h>` itself (§13).
- **Labels are in declared order** (§9).
- **Nothing is unused.** `answer` uses `sig`, and every value is printed (§5).
- **`print` accepts these values.** `print` takes numbers and `str` (§11), and here it gets `i32`, `i64` and `str`.

**2. What the program writes, and how it ends.**
1. `installed: 0`
   - Arguments run left to right (§7), so `called_install` runs before the line is printed.
   - `sigaction` succeeds and returns 0.
   - `print` writes its values with no separator and one newline (§11).
2. Then `called_raise(sig: SIGUSR1)` raises signal 30, and `called_handler` calls the program's function `answer` from inside a signal handler. §13 forbids exactly this: "A callback is … never a result or a signal handler, **nor called by one**", and "No function of the program is a signal handler". The supported route is `hero_signal_watch` / `hero_signal_take`.

The check can only happen when the program runs, because the build cannot know where C will call the function from. So I predict the program aborts when the handler enters `answer`, before `answer`'s body runs. §6 says "An abort ends the program at once, saying why". The message says that a function of the program was called from a signal handler. The specification does not give the exact wording.

- **Is `the answer ran for signal 30` among the lines?** No.
- `raised: …` and `seen: …` are never written either.
- **How it ends:** by an abort, after one line (`installed: 0`).

## choice_points

1. **When the "never called by a signal handler" rule is enforced.** §13 states the rule but does not say whether breaking it is a compile error or an abort.
   - **My choice:** an abort when the program runs, at the moment the handler calls `answer`. The header's `__builtin_frame_address(0)`, which keeps a frame that a stack walk can find, supports this reading.
   - **Other reading A:** the build refuses the program, because clang sees the inline header bodies and spots the callback reaching a `sigaction` handler. Then nothing is written.
   - **Other reading B:** nothing is enforced, and the program runs and gives a different answer than §13 allows. It would write four lines and end normally:
     - `installed: 0`
     - `the answer ran for signal 30`
     - `raised: 0`
     - `seen: 60`

     That would also mean `print`, which is not async-signal-safe, runs inside a handler.
2. **The extern parameter named `answer` and the function named `answer`.** §5 says shadowing is a compile error.
   - **My choice:** a parameter name in a member with no body binds nothing in the file, so the program compiles.
   - **Other reading:** this counts as shadowing, and the build refuses the program.
3. **Whether the abort comes before `answer`'s body.** If the check ran only after `answer` returned instead, the abort would come after a second line, `the answer ran for signal 30`.

## prediction

- **Compiler:** accepts the program.
- **Lines written, in order:**
  - `installed: 0`
- **How it ends:** an abort while `called_raise` runs, saying that a program function or callback was called from a signal handler (§13). The process does not end normally.
- **Not written:** `the answer ran for signal 30`, `raised: …` and `seen: …`.

## context

Nothing reached my context apart from the files in this directory: `brief.md`, `task.md` and `heroes-spec.md`. The only extra thing was the user's message, which held the instruction and a list of directory-like labels (t1-y-1 … verdict). I did not read anything those labels name.
~~~~~

### `t3-y-5`, now

~~~~~ markdown
I couldn't create `report.md` because I only had a file-reading tool here, so, as the brief allows, the whole report is below.

The main finding: the spec says a callback is never called by a signal handler, but it doesn't say whether the build or the running program enforces that. My answer depends on a choice I made about this.

# report.md

## answer

**1. Does `heroes build main.hero` accept the program?**

Yes, on my reading. Every line of `main.hero` follows the spec's rules:

- **The `extern "signal_lib.h"` group.** Each member fits the `Member` / `CParam` grammar (§13).
  - `called_install(sig: i32, answer: (function(i64) -> i64)) -> i32`: `sig` is `int32_t` and is declared `i32`, the header's own width and sign. `answer` is a callback, and §13 allows one only as "a **parameter**", which it is. Its own parameter and result follow "the same rule", and `int64_t (*)(int64_t)` matches `(function(i64) -> i64)`. The function-type parentheses are there, as §3 requires. The result `i32` matches `int32_t`.
  - `called_raise(sig: i32) -> i32` and `called_seen() -> i64` match the header the same way.
- **The `extern "signal.h"` group.** `constant SIGUSR1: i32` has no body, as a group constant must (§13). The value comes from a system header, not from a package named by `version` or `git`, and it is a plain macro integer that fits `int`.
- **Group order.** "One that needs another's names comes after it" (§13). Neither group needs the other's names. `signal_lib.h` includes `<signal.h>` itself.
- **`answer`.** It is a top-level function with no `@` or `lent` parameter, so it is a value and can be passed (§9). Its parameter `sig` is read, so it counts as used (§5). It returns on its only path (§8). `sig * 2` is `i64 * i64` (§7).
- **`main`.** It takes nothing and returns nothing (§1). Each line is a `print(...)` call, a `()` line standing alone (§5).
  - `print` accepts numbers (§11): `i32` from the first two calls, `i64` from the third.
  - The named arguments `sig:` and `answer:` are allowed (`Arg = [ ident ":" ] ...`, §9). They are in declared order, and since the two types differ they aren't required anyway.
- **What the compiler can check.** It checks C's side only through the header's types: "clang checks every result type, constant and record field against that header" (§13). Nothing in the declarations shows that C keeps `answer` and calls it from a signal handler. So the rule in the next point can't be a build-time refusal of this source.

**2. Lines written, and how the program ends**

My prediction is that the program writes exactly one line and then aborts:

```
installed: 0
```

1. **`installed: 0`.** `called_install` calls `sigaction(30, &sa, NULL)`, which succeeds and returns 0. `print` writes its values with no separator and one trailing newline (§11).
2. **The raise.** `called_raise(sig: SIGUSR1)` calls `raise(30)`. The C function `called_handler` runs as the signal handler, and it calls the program's function `answer` through the stored pointer.
   - §13 forbids exactly this: "A callback is a **parameter**, never a result or a signal handler, **nor called by one**", and "No function of the program is a signal handler."
   - The language's sanctioned route for signals is `hero_signal_watch` / `hero_signal_take` (`hero_os.h`), and this program doesn't use it.
   - The spec can't enforce this rule at build time (point 1). Its general rule is "An abort ends the program at once, saying why" (§6). So I predict the callback's entry aborts before `answer`'s body runs. The abort would say that a callback of the program was called by a signal handler (signal 30).
3. **Is `the answer ran for signal 30` among the lines?** No. The abort comes before `answer`'s first statement.
4. **The other lines.** `raised: ...` and `seen: ...` are never written, because an abort "ends the program at once" (§6).

**How it ends:** by an abort, after `installed: 0`, saying that a function of the program was called by a signal handler. The exit status is nonzero.

There are no files to write: the task asks for a prediction, not a program.

## choice_points

1. **How the "never called by a signal handler" rule is enforced** (§13 states the rule but not how it is enforced).
   - **My choice:** a runtime abort when the callback is entered from the handler.
   - **Alternative A, refused at build time:** the compiler would refuse the program, citing §13. It would have to analyse the header's function bodies, and §13 says clang checks only types, constants and fields.
   - **Alternative B, not enforced:** the program runs and gives a different answer than the reader expects. It would print:
     ```
     installed: 0
     the answer ran for signal 30
     raised: 0
     seen: 60
     ```
     It would then end normally, with heroes code running inside a signal handler, which the spec says never happens.
2. **Whether the `answer` parameter name in the extern shadows the top-level function `answer`** (§5: "Shadowing is a compile error").
   - **My choice:** it doesn't. An extern member's parameter names are labels with no body and no scope.
   - **Alternative:** the compiler refuses the program over that name.
3. **Whether `installed: 0` is visible before the abort.**
   - **My choice:** yes. `print` writes its line out before the next statement.
   - **Alternative:** if output were buffered and the abort didn't flush it, the program would print nothing and then abort.
4. **The return value of `sigaction`.**
   - **My choice:** 0 (success). It is a valid signal and a valid handler.
   - **Alternative:** -1 would print `installed: -1`. Nothing else changes, because the raise still delivers the signal to the default action or the handler.

## prediction

- **Does the compiler accept it?** Yes.
- **Lines written, in order:** `installed: 0`.
- **How it ends:** an abort, raised when the signal handler calls the program's callback `answer`. The abort says a callback of the program was called by a signal handler (signal 30). `the answer ran for signal 30`, `raised: ...` and `seen: ...` are never written.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md`, two other things reached my context:

- **My agent setup:** a system prompt saying I am a Claude agent built on the Claude Agent SDK, with a Read tool, the working directory path and the date.
- **The user's message:** it told me to follow `brief.md` and ended with a list of labels (`t1-y-1` … `t4-h-5`, `verdict`). I didn't use the labels.

No other rule, contract, memory or document reached my context, and I read no other file.
~~~~~

### `t4-f-1`, proposal-b

~~~~~ markdown
I only have a file-reading tool, so I can't create `report.md`. As the brief allows, here is the whole report.

# report.md

## answer

**The program, `main.hero`:**

```
function main()
    m: (mailbox i64) = mailbox()
    m2 = m
    send(m2, 1)
    print("received: ", receive(m, within_ms: 0).must())
    close(m)
```

**1. Does `heroes build main.hero` accept the program?** Yes.

- **Layout:** `main` is `function main()`, takes nothing and produces nothing, as §1 requires. Indentation is 4 spaces per level.
- **Mailbox type:** `m: (mailbox i64) = mailbox()` matches the grammar `"(" "mailbox" Type ")"` (§3). The annotation gives `mailbox()` its element type, which comes "from the type the context asks for" (§9, Generics).
- **The copy:** `m2 = m` is allowed. §3 says a `(mailbox T)` "is a copied ADDRESS, wherever it sits: two copies reach … one queue". The table row in §3 says the same: "a handle … whose copies reach one queue".
- **No unused names:** `m2` is read by `send`, and `m` is read by `receive` and `close`. Neither is unused (§5), and neither is a cell (`=`, not `@=`).
- **Literals:** in `send(m2, 1)` the literal `1` is an `i64`, from the context or by default (§2).
- **`receive`:** `receive(m, within_ms: 0)` uses the signature in §11. It gives an `i64?`, and `.must()` turns that into an `i64` (§6).
- **`print`:** it takes a `str` and a number, both kinds it can render (§11). The string `"received: "` has no braces, so the rule about `f` literals in §2 does not apply.
- **`send` and `close` as bare lines:** I take both to return `()`. §11 lists no failure for either, while it does list failures for `receive`. Under that reading they are `()` lines and "stand alone" (§5).
- **After the close:** nothing reads `m` or `m2` after `close(m)`, so the rule in §13 about reading an ended handle is not broken.

**2. Lines written, in order, and how the program ends:**

```
received: 1
```

- `send(m2, 1)` adds `1` to the one queue that both `m2` and `m` reach (§3, §11).
- `receive(m, within_ms: 0)` "takes the oldest" (§11). The queue already holds `1`, so the call succeeds and does not fail with `timeout`. `.must()` gives `1` (§6).
- `print` writes its values with no separator and one trailing newline (§11). The output is `received: 1`.
- `close(m)` ends the mailbox's life (§11, "as a handle's (section 13)"). There is only one life, because `m2` is a copy of the same address and not a second handle. So when `main` returns, no handle remains unconsumed, and the abort in §13 for a handle nobody consumes does not happen.
- The program ends normally, with exit status 0.

## choice_points

1. **Does `m2 = m` start a second handle life?**
   - **My choice:** no. It is a copy of the same address and the same queue (§3), so the single `close(m)` ends the one life.
   - **Other reading:** each copy is its own life. Then `m2` is never consumed, and the program would print `received: 1` and then abort when `main` returns, reporting 1 live handle.
2. **Does `send` return `()` or `()?`?**
   - **My choice:** `()`. §11 lists no failure for it.
   - **Other reading:** if it returned `()?`, the bare line `send(m2, 1)` would compute a fallible value that is not used, and a `_` cannot drop it (§5). The compiler would refuse the program. The author would then have needed `send(m2, 1).must()`.
3. **Does `close` return `()` or `()?`?**
   - **My choice:** `()`, for the same reason as `send`.
   - **Other reading:** the same compile refusal on the line `close(m)`.
4. **What does `within_ms: 0` do when a value is already waiting?**
   - **My choice:** the call returns the oldest value straight away, because the timeout only applies when the queue is empty.
   - **Other reading:** a 0 ms wait always fails with `timeout`. Then `.must()` aborts and the program writes no line; the abort reports the `timeout` error.
5. **Can the first argument of `receive` be passed by position, with only `within_ms:` named?**
   - **My choice:** yes. The two parameters have different types, so named arguments are not required (§9).
   - **Other reading:** if every label were required, the compiler would refuse the call.

## prediction

- **Compiler:** accepts the program.
- **Output:** one line, `received: 1`
- **Ending:** normal, with exit status 0. No abort.

## context

- **Files read:** only `brief.md`, `task.md` and `heroes-spec.md` from this directory.
- **Other context:**
  - My system prompt said I am an agent built on Anthropic's Claude Agent SDK and gave the working directory and date.
  - Your message listed some run labels (`t1-y-1` … `t4-h-5`, `verdict`). I read no files for them, and they did not affect the analysis.
  - No other instruction, rule, memory, project file or document reached my context.
~~~~~

### `t4-f-2`, proposal-b

~~~~~ markdown
I couldn't write `report.md`: the only tool I have here reads files and cannot create them. As the brief allows, the whole report is below.

# report.md

## answer

**1. Does `heroes build main.hero` accept the program?** Yes.

`main.hero`:
```
function main()
    m: (mailbox i64) = mailbox()
    m2 = m
    send(m2, 1)
    print("received: ", receive(m, within_ms: 0).must())
    close(m)
```

Why each line is accepted:
- **`function main()`**: takes nothing and produces nothing, as §1 requires of the file you compile. It uses no package, so it needs no `heroes.toml` (§1).
- **`m: (mailbox i64) = mailbox()`**:
  - `(mailbox T)` is a valid type, and its parentheses follow the `Prefix` production in §3.
  - `mailbox()` gets `T` from "the type the context asks for", which is the annotation (§9, Generics).
  - The call "begins a mailbox's life" (§11).
- **`m2 = m`**: an immutable binding (§5). §3 says a `(mailbox T)` "is a copied ADDRESS, wherever it sits: two copies reach one … queue". The type table adds "a handle … whose copies reach one queue". So copying a mailbox is explicitly allowed, and both names reach the same queue.
- **`send(m2, 1)`**:
  - It uses `m2`, so no binding is left unused (§5).
  - The literal `1` takes the type `i64` from the mailbox's `T` (§2).
  - The two parameters have different types, so the arguments need no names (§9).
  - `send` is taken to return `()`, so the line stands alone (§5); see choice points.
- **`print("received: ", receive(m, within_ms: 0).must())`**:
  - `receive` gives an `i64?` and `.must()` takes the value out (§6, §11).
  - `print` accepts a `str` and a number (§11).
  - `print` returns `()`, so the line stands alone.
- **`close(m)`**: ends the mailbox's life (§11, §13). Nothing reads `m` or `m2` after it, so the rule "reading it where every path has ended it is a compile error" (§13) is not triggered.
- **Layout**: 4-space indentation, ASCII only, no tabs (§1).

**2. What the program writes, and how it ends.**

It writes one line:
```
received: 1
```

1. `send(m2, 1)` adds `1` to the one queue that both `m` and `m2` reach (§3, §11).
2. `receive(m, within_ms: 0)` "takes the oldest" item (§11). An item is already waiting, so it does not fail with `timeout`, and no stop has been asked, so it does not fail with `stopped`. It gives `ok(1)`.
3. `.must()` gives `1` (§6).
4. `print` writes its values with no separator and exactly one trailing newline (§11), which gives `received: 1`.
5. `close(m)` ends the mailbox's one life. When `main` returns, no handle is left unconsumed, so the leak check in §13 ("a handle nobody consumes … aborts when `main` returns") does not fire.

The program ends normally.

## choice_points

1. **Whether the copy `m2` has a life of its own.**
   - *My choice:* a mailbox has one life, shared by all its copies (§3: "copies reach one queue"). `close(m)` ends that life, and nothing is owed for `m2`.
   - *The other reading:* each copy is its own handle to end. Then `m2` is never closed, and the program prints `received: 1` and then aborts when `main` returns, saying one handle was not consumed (§13).
2. **What `send` returns.** §11 does not give `send` a result type.
   - *My choice:* `()`, so `send(m2, 1)` can stand alone.
   - *The other reading:* if it returned `()?`, the bare line would be "a line that computes a value" and must use it, and a `_ =` may not drop a `T?` (§5). The compiler would refuse the program.
3. **What `within_ms: 0` does when an item is already queued.**
   - *My choice:* it takes the waiting item at once, because `timeout` only applies when nothing arrives in time.
   - *The other reading:* a zero wait always fails with `timeout`. Then `.must()` aborts after `send` and before `print` writes anything, with a message naming `timeout`.
4. **What `close` returns.**
   - *My choice:* `()`, so the line stands alone.
   - *The other reading:* if it returned `()?`, the compiler would refuse the program, as in point 2.
5. **Whether a mailbox may be copied inside one thread at all.** §11 says a handle crosses threads "only into `spawn` or a send, its life ending in the sender".
   - *My choice:* that rule is about crossing between threads, not about copying inside one. §3 explicitly allows copies.
   - *The other reading:* the compiler would refuse `m2 = m`.

## prediction

- **Compiler:** accepts the program and produces a binary.
- **Output:**
  ```
  received: 1
  ```
- **Ending:** normal, with no abort.

## context

- No document other than `brief.md`, `task.md` and `heroes-spec.md` in this directory reached my context, and I read no other file.
- Two other things reached my context:
  - The system prompt, which said I am an agent built on Anthropic's Claude Agent SDK and gave the working directory and date.
  - The user's message, which, after the instruction, listed names such as `t1-y-1` … `t4-h-5` and `verdict`. These look like the names of sibling task directories. I did not open them, and they gave no information about the language.
~~~~~

### `t4-f-3`, proposal-b

~~~~~ markdown
I only have a file-reading tool here, so I couldn't write `report.md`. As the brief says to do in that case, the whole report is below.

---

# report.md

## answer

**The file under its path, `main.hero`** (as the task gives it; I changed nothing):

```
function main()
    m: (mailbox i64) = mailbox()
    m2 = m
    send(m2, 1)
    print("received: ", receive(m, within_ms: 0).must())
    close(m)
```

**1. Does `heroes build main.hero` accept the program?** Yes.
- **Layout:** the file holds `function main()`, which takes and returns nothing (§1). It uses 4-space indentation and no `use`, so it needs no catalogue (§1).
- **Line 2:** `m: (mailbox i64) = mailbox()` matches `Binding = [":" Type] "=" Expression` (§5). `(mailbox i64)` matches `Prefix = "(" ("thread"|"mailbox") Type ")"` (§3). `mailbox()` takes its element type from the context's annotation (§9, generics take a type "from the type the context asks for").
- **Line 3:** `m2 = m` is allowed. §3 says a `(mailbox T)` is "a copied ADDRESS … two copies reach … one queue", and its table row says "a handle (section 13) whose copies reach one queue". Copying the handle is therefore legal and does not make a second queue.
- **Unused-binding rule (§5):** `m2` is read by `send`, and `m` is read by `receive` and `close`. Neither binding is unused, and neither is a cell (`@=`), so no "cell nothing re-binds" error applies.
- **Line 4:** in `send(m2, 1)`, the literal `1` takes the type the context asks for, `i64` (§2).
- **Line 5:** `receive(m, within_ms: 0)` gives an `i64?` (§11: it "takes the oldest, failing `timeout`…"). `.must()` extracts it (§6), and `print` takes a `str` and a number (§11). `"received: "` has no braces, so the rule against braces without `f` doesn't apply (§2).
- **Line 6:** `close(m)` ends the mailbox's life (§11: "`mailbox()` begins a mailbox's life, which `close(m)` ends, as a handle's (section 13)"). Nothing reads `m` or `m2` after the close, so the §13 rule ("reading it where every path has ended it is a compile error") is not triggered.

**2. Lines written, and how it ends:**
1. `received: 1`
   - `send` adds 1 to the one queue that both `m2` and `m` reach (§3, §11).
   - `receive(m, within_ms: 0)` takes the oldest item, 1, which is already there, so it gives `ok(1)` and not `timeout` (§11).
   - `.must()` gives `1` (§6).
   - `print` writes its values with no separator and one trailing newline (§11).
2. **It ends normally.** `close(m)` ended the mailbox's single life, so at `main`'s return no handle is unconsumed. The rule that "a handle nobody consumes … aborts when `main` returns" (§13) is satisfied. `m2` is a copy of the same handle, not a second life (§3: "copies reach one queue").

## choice_points

1. **Return type of `send` and `close`:** the spec doesn't say.
   - **My choice:** they return `()`, so standing alone as statements is legal.
   - **The other reading:** if either returned `()?`, the line would compute a fallible value that is never used. That would be a program the compiler refuses: §5 says "A line that computes a value must use it… A `_` never drops a `T?`… Answer the error."
2. **Whether the copy `m2` is a second handle life that must be closed separately:** the spec doesn't say directly.
   - **My choice:** one life, shared by the copies. This rests on §3 ("whose copies reach one queue", "a copied ADDRESS") and §11 (the mailbox's life is begun by `mailbox()` and ended by `close`).
   - **The other reading:** each copy needs its own `close`. Then the program would compile but abort when `main` returns, saying 1 handle was not consumed.
3. **Whether copying a handle by `=` is allowed at all:** §11 says "another handle [crosses] only into spawn or a send".
   - **My choice:** that sentence is about crossing between threads, not about a local copy. §3 explicitly allows copies of a mailbox.
   - **The other reading:** the copy is forbidden, and the compiler refuses line 3.
4. **Whether `close(m)` after sending through `m2` ends the right life:**
   - **My choice:** yes, since both reach one queue.
   - **The other reading:** the program would need `close(m2)` too, or instead. That leads to the same abort as point 2.
5. **`within_ms: 0` with an item already queued:**
   - **My choice:** it returns the item at once. §11 says receive "takes the oldest" and fails `timeout` only when nothing arrives in time.
   - **The other reading:** a zero wait always times out. Then `.must()` would abort the program with the `timeout` error, and nothing would be printed.
6. **The `within_ms:` label:** I used it, as §11 writes it (`receive(m, within_ms:)`). Leaving it out might be refused by the compiler. This didn't matter here because the task's file already has the label.

## prediction

- **Compiler:** accepts the program and produces a binary.
- **Output, in order:**
  ```
  received: 1
  ```
- **End:** normally, with no abort, exit status 0.

## context

- **From this directory:** `brief.md`, `task.md` and `heroes-spec.md`. These are the only files I read.
- **From outside this directory:**
  - The working-directory path (`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/251-llm-ergonomist/s/t4-f-3`) and the current date.
  - The fact that I run as a Claude agent on Anthropic's Claude Agent SDK.
  - A list of sibling run names in the user message (`t1-y-1` … `t4-h-5`, `verdict`).
  - None of these affected the answer.
- **Not received:** no other rule, contract, memory or document about the Heroes language.
~~~~~

### `t4-f-4`, proposal-b

~~~~~ markdown
I only have a file-reading tool here, so I couldn't create `report.md`. As the brief says to do in that case, the whole report is below.

---

# report.md

## answer

The task asks me to predict what an existing program does, not to write a new one. The only file involved is the one the task gives, unchanged:

`main.hero`
```
function main()
    m: (mailbox i64) = mailbox()
    m2 = m
    send(m2, 1)
    print("received: ", receive(m, within_ms: 0).must())
    close(m)
```

**1. Does `heroes build main.hero` accept the program?** Yes. Line by line:

- `function main()` takes nothing and produces nothing, as required (§1).
- **`m: (mailbox i64) = mailbox()`**
  - `(mailbox T)` is a valid type, and the parentheses match the grammar `"(" "mailbox" Type ")"` (§3).
  - `mailbox()` starts a mailbox's life (§11). Its `T` comes from the type annotation, which is "the type the context asks for" (§9).
- **`m2 = m`** is allowed. A `(mailbox T)` "is a copied ADDRESS … two copies reach … one queue" (§3), and "a handle (section 13) whose copies reach one queue" (§3 table). Nothing forbids copying a handle inside one thread.
- **`send(m2, 1)`**
  - `send(m, v)` "adds to it" (§11). The spec gives it no fallible result (only `receive` is said to fail), so this is a `()` line, which "stands alone" (§5).
  - The literal `1` becomes the mailbox's `i64` (§2).
  - `m2` is read here, so it is used (§5).
- **`print("received: ", receive(m, within_ms: 0).must())`**
  - `receive` gives an `i64?`, and `.must()` extracts the `i64` (§6). The `T?` is therefore not dropped.
  - `print` accepts a `str` and a number (§11).
  - `within_ms:` is the named argument the spec writes for `receive` (§11).
- **`close(m)`** ends the mailbox's life "as a handle's" (§11, §13). Nothing reads `m` or `m2` after the close, so the rule that "reading it where every path has ended it is a compile error" (§13) doesn't apply.
- **Other checks:**
  - No binding goes unused (§5), and no cell or `@` parameter is involved.
  - Nothing is shadowed (§5).
  - The indentation is 4 spaces (§1).
  - There is no `???` (§12), so the build produces a binary.

**2. What the program writes, and how it ends.**

```
received: 1
```

- **Why `received: 1`:** `send(m2, 1)` puts `1` on the queue that both `m2` and `m` reach (§3, §11).
  - `receive(m, within_ms: 0)` "takes the oldest" (§11). That is the `1` already waiting, so it neither waits nor fails `timeout`.
  - No stop was asked (§13), so it doesn't fail `stopped`.
  - `.must()` gives `1` (§6).
  - `print` writes its values with no separator and exactly one trailing newline (§11).
- **How it ends: normally.** `close(m)` ends the mailbox's one life. When `main` returns, no handle is left unconsumed, so the check that "aborts when `main` returns, saying how many" (§13) finds nothing. `m2` is a copy of the same handle, not a second life.

## choice_points

1. **Does `send` return `()` or `()?`?** The spec never gives `send` a result type.
   - **My choice:** `()`. The spec says only `receive` fails (`timeout`, `stopped`), and `send` "adds to it".
   - **The other reading:** if `send` returned `()?`, the bare line `send(m2, 1)` would compute an unused value. A `_` can't drop a `T?` (§5), so the compiler would refuse the program.
2. **Is `m2` a separate life or a copy of one life?**
   - **My choice:** one life. "Copies reach one queue" (§3), and `mailbox()` "begins a mailbox's life, which `close(m)` ends" (§11), so `close(m)` ends the life for both names.
   - **The other reading:** if each copy counted as its own handle, `m2` would never be closed. The program would print `received: 1` and then abort when `main` returns, reporting 1 live handle.
3. **What does `within_ms: 0` mean?**
   - **My choice:** take a message if one is already queued, and fail `timeout` only if the queue is empty.
   - **The other reading:** if 0 meant "fail at once regardless", `.must()` would abort on `timeout` and nothing would be printed.
4. **May a handle be copied with `=` at all?**
   - **My choice:** yes. §3 says copies reach one queue, and the "only into `spawn` or a send" restriction in §11 is about crossing threads.
   - **The other reading:** a stricter rule would refuse `m2 = m` at compile time.
5. **The type of `within_ms`** isn't given.
   - **My choice:** whatever integer type it is, the literal `0` takes it and fits (§2).
   - **The other reading:** if it were something a literal can't become, the compiler would refuse the call. I don't think that's a plausible reading.

## prediction

- **Compiler:** accepts the program and produces a binary.
- **Output, in order:**
  ```
  received: 1
  ```
- **Ending:** normally, with no abort, after `close(m)` and the return from `main`.

## context

- **Files I read:** only `brief.md`, `task.md` and `heroes-spec.md` from this directory.
- **Other things in my context:**
  - The agent-SDK system prompt, which gave the working directory path (it includes `heroes-lang` and `251-llm-ergonomist`) and the date.
  - A list of identifiers in the user message (`t1-y-1` … `t4-h-5`, `verdict`) with no content behind them.
- **What was not there:** no other instructions, rules, contracts, memory files or documents. Nothing in my context described Heroes beyond the spec.
~~~~~

### `t4-f-5`, proposal-b

~~~~~ markdown
I only have a file-reading tool, so I couldn't create `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

**1. Does `heroes build main.hero` accept the program?** Yes. Line by line:

- `m: (mailbox i64) = mailbox()`: the type `(mailbox i64)` follows the grammar `"(" ( "thread" | "mailbox" ) Type ")"` (§3). `mailbox()` gets its `T` from the type the context asks for (§9, Generics). This is an immutable `=` binding (§5).
- `m2 = m`: this is allowed. A `(mailbox T)` is "a copied ADDRESS … two copies reach … one queue" (§3), and the table calls it "a handle (section 13) whose copies reach one queue". `m2` is read later by `send`, so it is not an unused binding (§5).
- `send(m2, 1)`: the literal `1` takes the type `i64` from the context (§2). The line stands alone as a `()` call (§5, "a `()` line … stands alone"); see choice point 1.
- `print("received: ", receive(m, within_ms: 0).must())`: `receive(m, within_ms:)` gives an `i64?` (§11: "takes the oldest, failing `timeout`…"). `.must()` takes the `i64` out (§6). `print` accepts a `str` and a number (§11). The `T?` is consumed, not dropped (§5).
- `close(m)`: this ends the mailbox's life "as a handle's" (§11, §13). Nothing reads `m` or `m2` after the life ends, so the §13 compile error ("reading it where every path has ended it") does not apply.
- `main` takes nothing and produces nothing (§1). Indentation is 4 spaces (§1). Both `m` and `m2` are read (§5).

**2. What the program writes, and how it ends**

```
received: 1
```

- `send(m2, 1)` adds `1` to the one queue that both `m2` and `m` reach (§3, §11).
- `receive(m, within_ms: 0)` takes the oldest value. One is already waiting, so it gives `ok(1)` (§11). `.must()` takes out `1` (§6).
- `print` writes its values with no separator and one trailing newline (§11), so the output is `received: 1`.
- `close(m)` ends the mailbox's one life. When `main` returns there is no live handle, unended lease or unwaited thread, so the "aborts when `main` returns, saying how many" rule (§13) does not fire.
- The program ends **normally**.

No files were written; the task asks only for predictions.

## choice_points

1. **What `send` and `close` return.** The spec does not say whether they return `()` or `()?`.
   - My choice: `()`. Only `receive` is given failure codes (`timeout`, `stopped`) in §11.
   - The other reading: if `send` or `close` returned `()?`, the bare statements `send(m2, 1)` and `close(m)` would compute a fallible value and not use it. The compiler would refuse the program (§5: "A line that computes a value must use it … A `_` never drops a `T?`"). The author would have to write `.must()` after each call.

2. **What `within_ms: 0` means.** The spec does not say whether a 0 ms timeout first takes a value that is already waiting.
   - My choice: it does, so `receive` gives `ok(1)`.
   - The other reading: if a 0 ms timeout failed `timeout` before looking at the queue, `.must()` would abort before anything is printed. The spec does not give the wording of that abort; it would cite the code `timeout`.

3. **Whether `m2 = m` makes a second life.** The spec does not say whether copying a mailbox handle creates a second life that must also be closed.
   - My choice: no. A mailbox has one life per `mailbox()` call: "`mailbox()` begins a mailbox's life" (§11), and "copies reach one queue" (§3). So `close(m)` ends it for both names.
   - The other reading: if each copy were its own life, the program would print `received: 1` and then abort when `main` returns, saying 1 handle was never consumed (§13).

4. **The copy and the thread rule.** "Another handle only into `spawn` or a send, its life ending in the sender" (§11) could be read as limiting copies.
   - My choice: this rule only covers values crossing to another thread. A plain `m2 = m` on the same thread is allowed, because §3 explicitly speaks of copies.
   - The other reading: the compiler would refuse `m2 = m`.

5. **Naming the argument.** Writing `within_ms:` when it is not required is a style matter, not a choice point. The two parameters have different types, so names are not mandatory (§9), and §11 writes the call with that label.

## prediction

- **Compiler:** accepts the program.
- **Output:** one line, `received: 1`.
- **Ending:** normally, with no abort.

## context

- Besides `brief.md`, `task.md` and `heroes-spec.md`, two things reached my context:
  - the system prompt, which says I am a Claude agent built on the Claude Agent SDK, gives the working directory and today's date, and gives me only a file-reading tool;
  - the user's message, which repeated the instruction and listed labels (`t1-y-1` … `t4-h-5`, `verdict`) that look like names of other runs.
- Neither affected this analysis.
- I read no other file, rule, memory or document.
~~~~~

### `t4-h-1`, proposal-m

~~~~~ markdown
I can't write `report.md` because I only have a file-reading tool. As the brief allows, here is the whole report.

# report.md

## answer

**The program, `main.hero` (unchanged from the task):**

```
function main()
    m: (mailbox i64) = mailbox()
    m2 = m
    m2.send(1)
    print("received: ", m.receive(within_ms: 0).must())
    m.close()
```

**1. Does `heroes build main.hero` accept the program?** Yes. Line by line:
- **Line 1:** `function main()` takes nothing and returns nothing, as §1 requires.
- **Line 2:** `m: (mailbox i64) = mailbox()` uses the type syntax `"(" "mailbox" Type ")"` (§3 grammar). The annotation gives `mailbox()` its `T` (§9: a type parameter takes its type from "the type the context asks for"). `mailbox()` begins the mailbox's life (§11).
- **Line 3:** `m2 = m` is allowed. §3 says a `(mailbox T)` "is a copied ADDRESS … two copies reach one queue", and the table says it is "a handle (section 13) whose copies reach one queue". `m2` is read on the next line, so it is not an unused binding (§5).
- **Line 4:** `m2.send(1)`: the literal `1` takes the type `i64` from the context (§2). Mailbox operations are written after a dot (§11). Its result is `()` (see choice point 1), so the line can stand alone (§5).
- **Line 5:** `m.receive(within_ms: 0)` is an `i64?`, and `.must()` turns it into an `i64` (§6). `print` takes a `str` and a number (§11).
- **Line 6:** `m.close()` ends the mailbox's life (§11). Nothing reads `m` or `m2` after that, so the §13 compile error "reading it where every path has ended it" does not apply.
- **Bindings and layout:** no binding is unused, there is no shadowing, and the indentation is 4 spaces.

**2. What the program writes, and how it ends:**

```
received: 1
```

- **Line 4:** `m2.send(1)` puts `1` in the one queue that both `m` and `m2` reach (§3, §11).
- **Line 5:** `m.receive(within_ms: 0)` "takes the oldest" (§11). The queue already holds `1`, so it gives `ok(1)` instead of failing with `timeout`, and `.must()` gives `1` (§6). `print` writes its values with no separator and exactly one newline at the end (§11), so the output is `received: 1`.
- **Line 6:** `m.close()` ends the handle's life.
- **When `main` returns:** no handle is left unended, so the abort for "a handle nobody consumes" (§13) does not happen.
- **How it ends:** normally.

## choice_points

1. **What `send` and `close` return.** The spec doesn't say. I assumed `()`, because `send` is described only as "adds to it" and no failure code is listed. If `send` returned `()?`, the compiler would refuse line 4: "A line that computes a value must use it" and "A `_` never drops a `T?`" (§5). The same applies to `close`.
2. **Whether copying a handle splits its life.** I read the mailbox as having one life shared by all copies ("copies reach one queue"), so `m.close()` ends it for `m2` as well. If each copy had its own life, `m2` would never be ended. The program would then print `received: 1` and abort when `main` returns, reporting one live handle (§13). Another reading is that a copy must be ended too, which would make the compiler refuse the program.
3. **Whether `send` blocks without a receiver.** I took the mailbox to be an unbounded queue, so `send` returns at once. If `send` waited for a receiver on another thread, the program would never end at line 4 and print nothing.
4. **What `within_ms: 0` does when a message is already waiting.** I took it to mean "take the oldest, wait 0 ms if empty", so the message is returned. If 0 meant an immediate `timeout`, `.must()` would abort, and the abort would name the error `timeout` (§6, §11).
5. **The type of `within_ms`.** It isn't stated. I assumed an integer type that the literal `0` fits, which works for any integer type (§2).

## prediction

- **Does the compiler accept it?** Yes.
- **Lines written, in order:**
  1. `received: 1`
- **How it ends:** normally, with no abort.

## context

- **Inputs read:** only `brief.md`, `task.md` and `heroes-spec.md` from this directory.
- **Other context:** my system prompt (a generic agent description and the working-directory path) and the user's message. That message listed names like `t1-y-1` … `verdict` without contents.
- **Excluded:** no other file, project rule, memory or document reached my context.
~~~~~

### `t4-h-2`, proposal-m

~~~~~ markdown
I have only a read tool, so I couldn't write `report.md`. As the brief says to do in that case, here is the whole report.

# report.md

## answer

The task gives one file, `main.hero`. I add or change nothing.

`main.hero`:
```
function main()
    m: (mailbox i64) = mailbox()
    m2 = m
    m2.send(1)
    print("received: ", m.receive(within_ms: 0).must())
    m.close()
```

**1. Does `heroes build main.hero` accept the program?** Yes.
- **`main`:** `function main()` takes and returns nothing (§1).
- **`m`:** `m: (mailbox i64) = mailbox()` is an annotated binding, and the mailbox type is in the grammar (§3: `"(" ( "thread" | "mailbox" ) Type ")"`). `mailbox()` starts a mailbox's life (§11).
- **`m2 = m`:** this copies a handle. §3 says a `(mailbox T)` is "a handle (section 13) whose copies reach one queue". It also says "A `ptr`, a `cstr` or a `(mailbox T)` is a copied ADDRESS … two copies reach one … queue". The only limit on moving a handle is about crossing between threads ("another handle only into `spawn` or a send", §11), and nothing crosses here.
- **No unused bindings:** `m2` is read by `m2.send(1)`, and `m` is read by `receive` and `close`. §5 says "a read is a use", so neither binding is unused.
- **`m2.send(1)`:** mailbox operations are written after a dot (§11). The literal `1` takes the type `i64` from the mailbox's element type (§2). The line produces `()` and stands alone (§5).
- **`m.receive(within_ms: 0)`:** this is an `i64?`, because §11 says it can fail with `timeout` or `stopped`. `.must()` takes out the `i64` (§6). `print` accepts a `str` and a number (§11). Its result is `()` and stands alone.
- **`m.close()`:** this ends the mailbox's life (§11). Nothing reads `m` or `m2` after that, so the rule against reading a handle whose life has ended doesn't apply (§13).

**2. Lines written, and how the program ends:**
- It writes one line: `received: 1`
  - `m2` and `m` reach the same queue (§3), so the `1` sent through `m2` is the oldest item in `m`'s queue.
  - `receive` "takes the oldest" (§11), so it returns `ok(1)`, and `.must()` gives `1` (§6).
  - `print` writes its values with no separator and one trailing newline (§11), which gives `received: ` followed by `1`.
- It then ends normally with exit status 0.
  - `m.close()` ends the only life the mailbox has; its copies reach one queue, which has one life (§3, §11).
  - So the check for an unended handle when `main` returns (§13) finds nothing to report.

## choice_points

1. **Whether copying a handle copies its life.** I read §3 to mean `m2` and `m` are copies of one handle with one life, and that `m.close()` ends it.
   - The other reading treats `m2` as a second life that nobody closes. Then the program would print `received: 1` and abort when `main` returns, saying one handle was never ended.
   - A stricter reading forbids copying a handle at all. Then the compiler would refuse `m2 = m`.
2. **Whether `send` and `close` return `()` or `()?`.** §11 lists failures only for `receive` (`timeout`, `stopped`), so I took `send` and `close` to return `()`.
   - If either returned `()?`, the bare statement would leave a fallible value unused (§5), and the compiler would refuse the program.
3. **What `within_ms: 0` means when an item is already waiting.** I read it as "take the oldest item now", so it succeeds.
   - The other reading is that a zero timeout fails straight away with `timeout`. Then `.must()` would abort, the abort would name `timeout`, and the program would print nothing.
4. **Whether `send` through one copy reaches the queue the other copy receives from.** I took yes, from "whose copies reach one queue" (§3).
   - The other reading is "every value behaves as an independent copy", giving two separate queues. Then `m` would be empty, `receive` would fail with `timeout`, and `.must()` would abort with nothing printed. §3 explicitly excludes mailboxes from that rule.
5. **The type of `within_ms`.** I assumed an integer type that a bare literal `0` fits (§2). Other types for it would make no difference here.

## prediction

- **Compiler:** accepts the program.
- **Output, in order:**
  ```
  received: 1
  ```
- **How it ends:** normally, with exit status 0 and no abort.

## context

- **Other files:** none. Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context.
- **Other instructions:** my agent setup said I am a Claude agent on the Agent SDK and gave the working directory and date. The user's message also ended with a list of labels (`t1-y-1` … `t4-h-5`, `verdict`), which I didn't use.
- **Nothing else:** no other rules, contracts, memories or documents reached my context.
~~~~~

### `t4-h-3`, proposal-m

~~~~~ markdown
I can't write `report.md` because I only have a file-reading tool here, so the full report follows below, as the brief allows.

# report.md

## answer

**File:** `main.hero` (as given in the task, unchanged)

```
function main()
    m: (mailbox i64) = mailbox()
    m2 = m
    m2.send(1)
    print("received: ", m.receive(within_ms: 0).must())
    m.close()
```

**1. Does `heroes build main.hero` accept the program?** Yes. Line by line:

- `function main()` takes nothing and produces nothing, as §1 requires.
- **`m: (mailbox i64) = mailbox()`**
  - The type annotation matches the grammar `"(" "mailbox" Type ")"` (§3).
  - `mailbox()` gets its `T` from the type the context asks for (§9: "else from the type the context asks for").
- **`m2 = m`** is allowed.
  - §3: a `(mailbox T)` is "a handle (section 13) whose copies reach one queue".
  - §3 also says: "A `ptr`, a `cstr` or a `(mailbox T)` is a copied ADDRESS… two copies reach … one queue."
  - `m2` is read by the next line, so it is not an unused binding (§5).
- **`m2.send(1)`** is a mailbox operation written after a dot (§11).
  - The literal `1` takes the type `i64` from context (§2).
  - The line produces `()`, so it may stand alone (§5).
- **`print("received: ", m.receive(within_ms: 0).must())`**
  - `receive(within_ms:)` gives an `i64?` (§11: "takes the oldest, failing `timeout`…").
  - `.must()` takes the value out of that `i64?` (§6).
  - `print` accepts a `str` and a number (§11).
  - The string has no braces, so the rule about `f` literals in §2 does not apply.
- **`m.close()`** ends the mailbox's life (§11). The line gives `()` and stands alone.
- Nothing in the program is shadowed, unused, or left as a hole.

**2. Output and ending:**

The program writes one line:

```
received: 1
```

- `m2.send(1)` puts `1` into the single queue that both `m` and `m2` reach (§3).
- `m.receive(within_ms: 0)` takes the oldest value in that queue, which is `1`. The value is already there, so `timeout` does not happen (§11). `.must()` gives `1` (§6).
- `print` writes its values with no separator and exactly one trailing newline (§11), so the line is `received: 1`.
- `m.close()` ends the mailbox's one life, which began at `mailbox()` (§11).
- `main` then returns. There is no unended life to report: §13 says "a handle nobody consumes … aborts when `main` returns", and the only life has been ended. The program therefore **ends normally**.

## choice_points

1. **Does `m2 = m` start a second life that must also be closed?**
   - **My choice: no.** §11 says `mailbox()` begins *a* mailbox's life, and §3 says copies are addresses that "reach one queue". So copying does not begin a life, and `m.close()` ends the only one.
   - **The other reading:** each copy owes its own `close`. Then the program would abort when `main` returns, with a message like "1 handle not consumed".
   - A stricter reading of §13 would treat `m2 = m` as copying a handle outside `spawn`/send and refuse it at compile time. I rejected that because §3 explicitly allows copying a mailbox.
2. **What does `m.close()` return?**
   - **My choice: `()`.** The specification names no failure for `close`, so the bare line is legal.
   - **The other reading:** it returns `()?`. Then the bare statement would compute an unused fallible value. §5 says "a line that computes a value must use it", and a `_` may not drop a `T?`, so the compiler would refuse the program.
3. **The `within_ms:` label.**
   - **My choice: write the label**, as §11 does (`m.receive(within_ms:)`).
   - **The other reading:** leaving the label out might be refused, if the label is mandatory as written in §11.
4. **`within_ms: 0` when a message is already queued.**
   - **My choice: it returns the queued value at once.**
   - **The other reading:** a zero wait always fails with `timeout`. Then `.must()` would abort with a message naming `timeout`, and nothing would be printed.
5. **Whether `send` happens before `receive`.**
   - **My choice: it does.** There is one thread, and §7 evaluates parts in the order they are written.
   - No other reading seems possible.

## prediction

- **Compiler:** accepts the program.
- **Output:**
  ```
  received: 1
  ```
- **Ending:** normal, with exit status 0 and no abort.

## context

No instruction, rule, contract, memory or document other than `brief.md`, `task.md` and `heroes-spec.md` in this directory reached my context. The only other things I saw were the system prompt (which identifies me as an agent and gives the working directory and date) and a list of labels in the user message (`t1-y-1` … `verdict`). Those labels have no content and I did not use them.
~~~~~

### `t4-h-4`, proposal-m

~~~~~ markdown
I have no tool that can write files, so I couldn't create `report.md`. The brief says to give the whole report as the final answer in that case, so here it is.

# report.md

## answer

**1. Does `heroes build main.hero` accept the program?** Yes. Nothing in the specification refuses it, line by line:

- `m: (mailbox i64) = mailbox()` is a typed immutable binding. `Binding = [ ":" Type ] ( "=" | "@=" ) Expression` (§5), and `"(" "mailbox" Type ")"` is a valid `Prefix` (§3). The `i64` element type comes from the annotation, which is "the type the context asks for" (§9, Generics). `mailbox()` is a built-in (§11).
- `m2 = m` is allowed. A `(mailbox T)` is "a copied ADDRESS … two copies reach … one queue" (§3, and the table: "a handle whose copies reach one queue"). The handle limit ("another handle only into `spawn` or a send", §11) is about values crossing between threads, not copies made within one thread.
- `m2.send(1)`: mailbox operations are "written only after a dot" (§11), and this one is. The literal `1` takes the type `i64` from the mailbox. `send` is a statement that stands alone; the specification gives it no result that would have to be used (see choice points). This line also uses `m2`, so `m2` is not an unused binding (§5).
- `print("received: ", m.receive(within_ms: 0).must())`: `receive(within_ms:)` is the written form (§11). It gives an `i64?`, and `.must()` takes the value out (§6). `print` accepts a `str` and a number (§11). The string has no braces, so the rule about `f` literals doesn't apply (§2).
- `m.close()` ends the mailbox's life (§11, "as a handle's (section 13)") and stands alone as a `()` statement.
- `main` takes nothing and returns nothing (§1). The indentation is 4 spaces (§1). Every binding is used (§5).

**2. Lines written and how it ends:**

1. `received: 1`. `m2` and `m` reach the same queue (§3), so the `1` sent through `m2` is the oldest item in `m`. `receive` "takes the oldest" (§11), so it succeeds without a `timeout` and `.must()` gives `1` (§6). `print` writes its values with no separator and one trailing newline (§11).

After that, `m.close()` ends the one mailbox life. When `main` returns, no lease, handle, mailbox or thread is still open, so the abort for unended lives in §13 doesn't fire. **The program ends normally.**

There are no files to write: the task only asks for predictions about the given `main.hero`, which stays as given.

## choice_points

1. **What `send` and `close` return.** §11 doesn't give their result types. I assumed `()`, so each can stand alone as a line. If either returned a `T?`, the compiler would refuse the line: "A `_` never drops a `T?` … Answer the error" (§5), and a bare line that computes a value must use it.
2. **Does `close` through `m` end the life `m2` also reaches?** I read it as one life per mailbox: copies reach one queue, and one `close` ends it. If each copy counted as its own life, `m2` would never be closed, and the program would abort when `main` returns, reporting 1 unended life (§13). The output line would still be printed first.
3. **`within_ms: 0` when an item is already waiting.** I read "takes the oldest, failing `timeout`" as failing only when nothing arrives in time, so the waiting `1` is taken. The other reading, that a 0 ms limit always times out, would make `.must()` abort with a `timeout` error, and nothing would be printed.
4. **Copying a mailbox into `m2`.** I took §3 as allowing it explicitly. If copies were refused (like a moved handle), the compiler would reject `m2 = m`.
5. **The `i64` element type** comes from the annotation on `m` (§9). Without the annotation, `mailbox()` would have nothing to infer `T` from, and the compiler would refuse the call. This program has the annotation, so the question doesn't arise.

## prediction

- The compiler accepts the program and the build produces a binary.
- It writes one line:
  ```
  received: 1
  ```
- It ends normally.

## context

The only files I read were `brief.md`, `task.md` and `heroes-spec.md` in this directory. Besides those, my context had:

- the agent's system prompt (the Claude Agent SDK, the working directory path, the date);
- the user's message, which ended with a list of labels such as `t1-y-1 … t4-h-5` and `verdict`. These look like sibling experiment names; they had no content and I didn't use them.

No other rule, contract, memory or document reached my context.
~~~~~

### `t4-h-5`, proposal-m

~~~~~ markdown
I could not write `report.md` because I have no tool that writes files. As the brief allows, the whole report follows here.

# report.md

## answer

The task asks for no new files, so I wrote none. The program is the task's `main.hero`, unchanged:

`main.hero`
```
function main()
    m: (mailbox i64) = mailbox()
    m2 = m
    m2.send(1)
    print("received: ", m.receive(within_ms: 0).must())
    m.close()
```

**1. Does `heroes build main.hero` accept the program?** Yes.
- **Layout and the type:** The file holds `function main()` with no parameters and no result (§1). It uses 4-space indentation (§1). `(mailbox T)` is a valid type, written with its parentheses (§3 table and the `Prefix` production).
- **The copy `m2 = m`:** This is an immutable binding (§5). §3 says a `(mailbox T)` is "a copied ADDRESS … two copies reach … one queue". The table calls it "a handle (section 13) whose copies reach one queue". So copying a mailbox is allowed and does not start a second life.
- **No unused bindings (§5):** `m` is read by `.receive` and `.close`, and `m2` is read by `.send`. No cell exists, so no "cell nothing re-binds" error can arise.
- **Literals:** `1` in `m2.send(1)` takes `i64` from the mailbox's element type (§2). `within_ms: 0` is a named argument (§9 `Arg`) and fits its type.
- **The fallible result:** `m.receive(within_ms: 0)` is an `i64?` that fails `timeout` or `stopped` (§11). `.must()` takes out the `i64` (§6), so no `T?` is dropped (§5).
- **`print`:** It takes a `str` and a number (§11). The text `"received: "` holds no braces, so the rule about `f` literals does not apply (§2).
- **`m.close()`:** It is a mailbox operation written after a dot (§11). It ends the handle's life (§11, §13). It stands alone as a `()` line (§5).

**2. What the program writes, and how it ends:**
- **Line 1:** `received: 1`.
  - `mailbox()` begins the queue's life (§11).
  - `m2.send(1)` adds 1 to the same queue that `m` reaches (§3, §11).
  - `m.receive(within_ms: 0)` "takes the oldest" (§11). That is the 1 already waiting, so it neither times out nor sees a stop. `.must()` gives `1` (§6).
  - `print` writes its values with no separator and one trailing newline (§11).
- **How it ends:** normally.
  - `m.close()` ends the mailbox's one life before `main` returns.
  - Nothing is left alive that would trigger the abort for "a handle nobody consumes" when `main` returns (§13).
  - `m2` is never used after the close, so the abort for using an ended handle or a copy made before it (§13) is never reached.

## choice_points

1. **What `m.close()` returns.**
   - **The gap:** The spec never states it. It only says close ends the mailbox's life (§11).
   - **My choice:** It returns `()`, so a bare `m.close()` line is legal (§5: a `()` line "stands alone").
   - **The other reading:** If it returned `()?`, the line would compute a value that is never used. `_ =` cannot drop a `T?` either (§5). The compiler would refuse the program. That reading gives a refused program.
2. **Whether the copy `m2` is a second life that must be closed on its own.**
   - **My choice:** It is one life. §3 says copies are addresses reaching one queue, and §11 says `mailbox()` begins the life.
   - **The other reading:** If every copy needed its own close, `main` would return with one handle unended. The program would abort at the end, saying one handle was never consumed. It would still print `received: 1` first. That reading gives an abort at run time.
3. **Whether `receive(within_ms: 0)` returns a message that is already queued.**
   - **My choice:** Yes. It "takes the oldest" and fails `timeout` only when nothing arrives in time.
   - **The other reading:** If a 0 ms limit always timed out, `.must()` would abort with a `timeout` error before anything was printed. That reading gives an abort at run time with no output.
4. **Whether calling `close` on an immutable `=` binding is allowed.**
   - **My choice:** Yes. §11 writes `m.close()` with no `@`, and §9 says UFCS and `@` rules do not govern a mailbox's own operations.
   - **The other reading:** If ending a life required an `@` cell, the compiler would refuse the program.

## prediction

- **Compiler:** accepts the program and produces a binary.
- **Output, in order:**
  ```
  received: 1
  ```
- **Ending:** normally, with no abort.

## context

- **Only these files:** No instruction, rule, contract, memory or document other than `brief.md`, `task.md` and `heroes-spec.md` reached my context.
- **Session framing:** It told me I am a Claude agent built on the Claude Agent SDK and gave the working-directory path and the date.
- **Run labels:** The user's message ended with a list of run labels (`t1-y-1` … `t4-h-5`, `verdict`) that are not files in this directory. I did not use them.
~~~~~

### `verdict`, verdict (A: now, B: proposal-b)

~~~~~ markdown
I couldn't write `report.md` because I only have a file-reading tool, not a file-writing one. As `brief.md` allows, the full report is below.

# report.md

## verdict

**Approve.** Spec B makes threads part of the language, with typed handles. `spawn(f, x)` returns a `(thread T)` whose `T` comes from `f`, and `wait(t)` gives back that `T`. The meaning of each line can be read from the line and its signature, so a veto does not apply.

## experiment

**Spec B: `main.hero`**
```
record Job
    name: str
    text: str
    skip: [str]

record Count
    name: str
    words: i64
    distinct: i64

function split_words(text: str) -> [str]
    out: [str] @= []
    cur @= ""
    for c in text.chars()
        if c == " "
            out @ out.push(cur)
            cur @ ""
        else
            cur @ cur + c
    out @ out.push(cur)
    return out

function contains(xs: [str], x: str) -> bool
    for y in xs
        if y == x
            return true
    return false

function count(job: Job) -> Count
    words @= 0
    seen: {str: bool} @= {}
    for w in split_words(job.text)
        if !contains(job.skip, w)
            words @ words + 1
            seen[w] @ true
    return Count(name: job.name, words: words, distinct: len(keys(seen)))

function main()
    skip = ["the", "a", "to"]
    t1 = spawn(count, Job(name: "a", text: "the cat sat on the mat", skip: skip))
    t2 = spawn(count, Job(name: "b", text: "a dog and a cat", skip: skip))
    t3 = spawn(count, Job(name: "c", text: "to be or not to be", skip: skip))
    t4 = spawn(count, Job(name: "d", text: "x", skip: skip))
    r1 = wait(t1)
    r2 = wait(t2)
    r3 = wait(t3)
    r4 = wait(t4)
    print(f"{r1.name}: {r1.words} words, {r1.distinct} distinct")
    print(f"{r2.name}: {r2.words} words, {r2.distinct} distinct")
    print(f"{r3.name}: {r3.words} words, {r3.distinct} distinct")
    print(f"{r4.name}: {r4.words} words, {r4.distinct} distinct")
```

**Spec A: `main.hero`**
```
extern "hero_os.h"
    function hero_thread_spawn_str(body: (function(str) -> str), arg: str) -> i64
    function hero_thread_join_str(handle: i64) -> str

function split(text: str, sep: str) -> [str]
    out: [str] @= []
    cur @= ""
    for c in text.chars()
        if c == sep
            out @ out.push(cur)
            cur @ ""
        else
            cur @ cur + c
    out @ out.push(cur)
    return out

function contains(xs: [str], x: str) -> bool
    for y in xs
        if y == x
            return true
    return false

function encode(name: str, text: str, skip: [str]) -> str
    return name + "\n" + join(skip, " ") + "\n" + text

function count_job(job: str) -> str
    parts = split(text: job, sep: "\n")
    skip = split(text: parts[1], sep: " ")
    words @= 0
    seen: {str: bool} @= {}
    for w in split(text: parts[2], sep: " ")
        if !contains(skip, w)
            words @ words + 1
            seen[w] @ true
    return f"{parts[0]} {words} {len(keys(seen))}"

function report(line: str)
    p = split(text: line, sep: " ")
    print(f"{p[0]}: {p[1]} words, {p[2]} distinct")

function main()
    skip = ["the", "a", "to"]
    h1 = hero_thread_spawn_str(body: count_job, arg: encode(name: "a", text: "the cat sat on the mat", skip: skip))
    h2 = hero_thread_spawn_str(body: count_job, arg: encode(name: "b", text: "a dog and a cat", skip: skip))
    h3 = hero_thread_spawn_str(body: count_job, arg: encode(name: "c", text: "to be or not to be", skip: skip))
    h4 = hero_thread_spawn_str(body: count_job, arg: encode(name: "d", text: "x", skip: skip))
    r1 = hero_thread_join_str(handle: h1)
    r2 = hero_thread_join_str(handle: h2)
    r3 = hero_thread_join_str(handle: h3)
    r4 = hero_thread_join_str(handle: h4)
    report(r1)
    report(r2)
    report(r3)
    report(r4)
```

**Results (worked out by reading the specs; neither build was run):** I expect `heroes build main.hero` to accept both programs. Both should print the same lines and end normally when `main` returns:
```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```

## choice_points

**Both specs**
1. **How to split words.** The language has no `split`, so I wrote one over `chars()` that compares each character to `" "`.
   - Using `text[i] == ' '` with byte slices should also be accepted, because a `u8` from the context types the literal.
   - Forgetting the final `push` is still accepted, and the program then gives wrong answers (for example `d: 0 words`). Neither spec catches this.
2. **Counting distinct words.** I used `len(keys(map))`. Deduplicating through an array also works.
3. **Start all threads before waiting.** If a model waits right after each spawn, both specs accept it and print the same output. The concurrency requirement is broken silently.
4. **Checking skipped words.** Spec A allows no closures, so I wrote `contains` as a loop. Under both specs, trying `any` with a lambda is refused by the compiler.

**Spec B**
5. **How to hand over the arguments.** `spawn` takes one argument, so I packed the document and skip list into a `Job` record. Passing three arguments to `spawn` is refused (wrong arity).
6. **Where to keep the handles.** I used four named bindings.
   - An array `[(thread Count)]` waited in a `for` loop is a real gap in the spec. It doesn't say whether waiting on the copy made by iteration ends the life of the handle still in the array. The program could be refused, abort when `main` returns, or run fine.
7. **Forgetting a wait.** An unused `t4` binding is refused by the compiler. Writing `_ = spawn(...)` is accepted and then aborts when `main` returns. Both are caught.
8. **What `wait` returns.** I treated it as `T`, as the text says. If it were really `T?`, reading `r1.name` would be refused by the compiler, so the mistake is safe. The spec doesn't say what `wait` gives if the thread aborts.
9. **Collecting results through a mailbox instead.** That is accepted, but results arrive in finishing order, so the output order would be wrong. `receive` also needs a `within_ms` value whose type the spec never states. I used `wait`.
10. **Marking `count`'s parameter `lent`.** This is refused, because such a function is not a value. That's a good catch.

**Spec A**
11. **Encoding the input.** A thread carries only a `str`, so I invented a format: name, skip list and text separated by `\n` and spaces.
    - A wrong field index stops with an out-of-bounds abort.
    - A wrong separator gives wrong answers that the build accepts.
12. **Returning the counts.** I returned them as text (`"a 4 4"`) and printed the pieces. Turning them back into numbers would need a hand-written parser; `to_i64` from `str` isn't specified.
13. **Matching spawn and join.** Calling `hero_thread_join` (the `i64` version) on a handle from `spawn_str` type-checks, because the handle is just an `i64`. It's caught only if the result is later used as a `str`. Printed directly, the build accepts it and the output is undefined.
14. **Losing a handle.** `_ = hero_thread_spawn_str(...)` is accepted, and spec A says nothing about what happens at exit. Joining the same handle twice is also accepted, with undefined results.
15. **Linking.** I wrote the `extern` group with no `link`. Adding `link "hero_os"` names a library the spec never mentions, which could fail at link time.
16. **Named arguments.** My `split` has two `str` parameters, so named arguments are required. Leaving them out is refused (a catch).

## argument

Under B the task is direct: one record, `spawn`, `wait`. The thread's answer type comes from `f`, and a forgotten wait is refused or aborts. Under A the model must declare an FFI group and invent a text format for every value, and it gets back an untyped `i64` handle. Its main hazards (mismatched join variant, wrong format index, double join, lost handle) are mostly accepted by the build and fail at runtime or silently. B turns those into compiler refusals or a guaranteed abort. Two gaps remain: what happens to handles stored in arrays, and the type of `within_ms`. Neither touches this task. The handle sentence in §11 is dense but readable. Approve.

## condition

Several results would change my verdict to object:
- Building the B program fails, or `wait` actually returns `T?` and the spec doesn't say so.
- An array of thread handles waited in a `for` loop is accepted and then aborts at exit. Arrays are the idiomatic way to write this, so that would be a plausible mistake the build accepts.
- A task that uses mailboxes shows that `receive`'s `within_ms` type, or the rule that a mailbox's life ends "in the sender", can't be read well enough to write a correct program.

If the A program turned out not to build because `str` isn't allowed as an extern parameter, B's case would be stronger.

## context

- I read only `brief.md`, `task.md`, `changes.diff`, `spec-A.md` and `spec-B.md` in this directory.
- Outside these files, my context held the agent system prompt (working directory and date) and your message. That message ended with a list of labels (`t1-y-1` … `t4-h-5`, `verdict`), which I treated as having no meaning for this task.
- No other rules, memories or documents reached my context.
~~~~~

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/251-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
