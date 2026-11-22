# Panel 254, llm-ergonomist (the blind seat): the paid run, scored, and R9's fourth arm

Written 2026-11-22 by the session that scored the blind seat, started by the coordinator of
M-rebel-library after the paid run. Every number below is a command's, named beside it; the seats had no
compiler, so a session's *accepted* or *refused* is its own reading of its arm's text, never a build,
except where this report says it built one.

## How it was run

- **The first run**: 26 fresh `claude -p` sessions, started 2026-11-22 09:27:34 and ended 10:03:16
  (`run.log`), by `run.sh` as registered (`docs/panel/254-briefs/blind/run.sh`, the same bytes by `cmp`),
  from the session that holds the author's word. Each ran in a folder of its own under
  `.claude/worktrees/p254-scratch/254-llm-ergonomist/s/`, an area that is the top of an empty git
  repository of its own (`.gitignore` `*`, local user `reader` <`reader@invalid`>, no commit), ignored by
  this repository and holding no `CLAUDE.md`: `env -i`, `HOME`, `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the
  area, `< /dev/null`, `--bare --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`,
  `--tools "Read,Write" --allowedTools "Read,Write"`, `--disallowedTools` the skill's list,
  `--max-budget-usd 3`, `--output-format json`; Claude Code 2.1.323. The free preflight ran first: cost 0,
  its scan clean (`run.log` 09:27:34).
- **The model**: `claude-opus-5-5` alone in every `run.json`'s `modelUsage`, 26 of 26.
- **The sessions**: 26, 25 graded and the A/B verdict; every one `subtype` `success`, `is_error` false,
  `terminal_reason` `completed` (`254-briefs/blind/read_runs.py` over the 26 `run.json`).
- **The cost**: **6.445551 USD** (`run.log`'s last line; the same sum read again from the 26 `run.json`).
- **The leak check**: `tools/scan.py`, 27 transcripts (the 26 and the preflight): **prompt-alone=NO 0,
  git-status=LEAK 0, claude-md-or-memory=YES 0**; `tools/reads.py`, run by the scoring session after the
  run (the runner does not run it): 27 transcripts, **0 sessions with a path outside their folder**, 0
  without a cwd. **0 sessions voided.** Every session's `context` answer says nothing but its folder's
  files and the agent's system setup reached it; two name the words of their folder's path (`t1-q-2`:
  *"the working-directory path (which contains `254-llm-ergonomist`)"*, `t2-p-5`: *"whose names mention
  "heroes-lang" and "llm-ergonomist""*), which the scan does not count, the area standing under the
  repository's root by CLAUDE.md § Hard stops.
- **No session wrote `report.md`**: 26 of 26 open their answer saying they have no tool that writes a file
  (`grep` over the 26 `result.md`, 25 by one pattern and `t2-p-5` read by hand), and gave the report as
  their final answer, which `tools/after.py` kept as `result.md`. Why `--tools "Read,Write"` with
  `--bare` gave them no working `Write` on this CLI is unread; the brief's fallback held.
- **The grader's compiler**: the brief registered `.claude/worktrees/lane-p254/heroes-next`; that worktree
  was removed before the scoring. t1 `now` was built by `.claude/worktrees/p254-scratch/spec-warden/heroes-next`,
  whose `--version` reads the same `sources 6e891e681c14bca0`.
- **The blinding, as it was for the scorer**: the outcomes were written by letter to a file (sha256
  `1151936f8a3139188d153dc77463a3f5482cf1a0216e7810d4bbc181fbea7bb0`, 10:41:57) before `key.txt` was opened,
  but the scorer was not blind in fact: the seat's prepared report names the draw (`now=q m=y m0=p`), the
  sitting's `evidence.tar.xz` holds `key.txt` as drawn, and the design itself names `y` (t3 runs on `m`
  alone). What kept the grading honest is the rows, written before the draw.

## The draw, opened after the outcomes were written by letter

`key.txt`'s first line: `now=q m=y m0=p`; its 26 rows give each folder's arm and its files' hashes, and
each folder's `heroes-spec.md` reads the arm's registered hash (`ffd687d6...` for `q`, `b008c307...` for
`y`, `1dc1446d...` for `p`). The key is published whole in `254-reports/blind-key.md`.

## t1, arm `now` (letter `q`): four documents counted on threads through the doors of § 13

Graded by the seven reader's rows, and mechanically: each answer's `main.hero` (the longest fenced block
under that name in its `result.md`) built by the stage 3 named above and run under `timeout -s KILL 20`
five times, then S1 to S4, each one edit asserted to match once in a copy, built and run once
(`254-briefs/blind/grade-t1-now.py`; every edit matched once; every build exit 0; every run exit 0).

| folder | cost USD | builds, as it reasoned | how a document crosses, and back | reader's rows | mechanical (5 runs, S1, S2, S3, S4) | choice points it reported, in short |
|---|---|---|---|---|---|---|
| `t1-q-1` | 0.244637 | accepted | `<length>:<bytes>` fields both ways | right | right: 5 of 5, and all four | the encoding (a separator would break on `\|`); `to_i64` of a `u8` as a `T?`; `slice` on a `str`; no `link` on `hero_os.h`; empty pieces skipped; a list across lines |
| `t1-q-2` | 0.231019 | accepted | `<length>:<bytes>` lists both ways | right | right | the encoding; `to_i64` of a `u8` (*"the shakiest point"*); named arguments; empty pieces; two loops, start then wait |
| `t1-q-3` | 0.235409 | accepted | `<length>:<bytes>` in; `<words> <distinct> <name>` back, the name last | right | right | the encoding; the answer's order, the name last; digits by hand; `len`'s type; join everything before printing |
| `t1-q-4` | 0.220004 | accepted | `<length>:<bytes>` both ways | right | right | the encoding; `len`'s type; the declarations copied from § 13; empty pieces; loop names against shadowing |
| `t1-q-5` | 0.240589 | accepted | `<length>:<bytes>` in; the name last back | right | right | no `link`; the signatures copied; the encoding; digits by hand; a zero byte across the boundary (unsaid) |

All five predicted the four lines and a normal end, and wrote them: **5 of 5 right by the reader's rows,
5 of 5 right mechanically**.

## t1, arm `m` (letter `y`): the same task through `spawn` and `wait`

No compiler reads `m`, so the reader's rows alone.

| folder | cost USD | builds, as it reasoned | how a document crosses, and back | reader's rows | choice points it reported, in short |
|---|---|---|---|---|---|
| `t1-y-1` | 0.202299 | accepted | a `Job` record in, a `Count` record back | right | one argument to `spawn`, so a record; **the threads in four names, not a `[(thread Count)]`: copying a handle into an array is unsaid**; `.must()` on `spawn`; `len`'s type; empty pieces |
| `t1-y-2` | 0.194419 | accepted | records both ways | right | a record, not a joined text; bytes and `slice`; **an array or a `for` variable of threads may be refused**; `len`'s type; `print` with several values |
| `t1-y-3` | 0.188959 | accepted | records both ways | right | a record; **a `(thread T)` copied into an array or a loop variable is unsaid**; `.must()` on `spawn`; `i == n \|\|` guarding the index |
| `t1-y-4` | 0.216591 | accepted | records both ways | right | records, not `name\|text`; **an array or a loop variable of handles is unsaid**; `keys(seen).len()` into an `i64`; `len`'s type |
| `t1-y-5` | 0.198039 | accepted | records both ways | right | a record; `wait` read as giving `Count`, not `Count?`; **waiting on an array's copy is unsaid**; `len`'s type |

**5 of 5 right.** Every one of the five kept its four threads in four names and named the array of threads
as the choice it avoided (a `grep` of their `choice_points` for an array or a `for` variable of threads:
5 of 5; the same command over `t1-q`: 0 of 5).

## t2, arm `m` (letter `y`): five programs at a mailbox's edges

Graded by the proposal's rows (`docs/panel/254-briefs/llm-ergonomist.md` § The rows).

| folder | cost USD | p1 | p2 | p3 | p4 | p5 | choice points it reported, in short |
|---|---|---|---|---|---|---|---|
| `t2-y-1` | 0.213744 | right | right | right | right | right | the inline `sum @ sum + v` arm; a closed mailbox's values received; `stopped` over a queued value (*"even with values in it"*); `_ =` on a thread; `free(b)` after a send on one path (the emptied cell read as `nullptr` the other reading) |
| `t2-y-2` | 0.235529 | right | right | right | right | right | the same five, and a stop not ending the process |
| `t2-y-3` | 0.227224 | right | right | right | right | right | the same, and a second close doing nothing; `empty it` read as an ended life, `nullptr` the other |
| `t2-y-4` | 0.219204 | right | right | right | right | right | the same, and whether a mailbox of handles crosses `spawn` without `@` (yes: a shared address) |
| `t2-y-5` | 0.204034 | right | right | right | right | right | the same, and what wakes a receive waiting at the close |

What each wrote: p1 `first: timeout`, `send: closed`, `got 1`, `got 2`, `last: closed`, `done`, normally; p2
`total 12`; p3 `receive: stopped`; p4 `main ends`, then an abort at `main`'s return saying one thread was
never waited for, no mailbox named; p5 `sent`, `freed 1`, then an abort at `free(b)` for a handle whose
life the send ended. **Per program: p1 5, p2 5, p3 5, p4 5, p5 5.** The send after the close (p1): `send:
closed` in all five; **no session predicted an abort**.

## t2, arm `m0` (letter `p`): panel 251's text, graded by the same rows

| folder | cost USD | p1 | p2 | p3 | p4 | p5 | what decided it |
|---|---|---|---|---|---|---|---|
| `t2-p-1` | 0.315528 | refused: `m` read after `close` ended its life | no line, an abort (the worker uses a mailbox `main` ended) | right | the abort names the thread **and the unclosed mailbox** | right | m0 makes a mailbox a handle `close` ends; it names no `closed` code |
| `t2-p-2` | 0.294089 | refused, as `t2-p-1` | refused: `.must()` on a `(thread i64)` | right | refused: `.must()` | refused: `.must()` | m0's `spawn` gives a `(thread T)`, not a `T?` (the one session of five to read it so) |
| `t2-p-3` | 0.345709 | refused | no line, never ends (`receive` waits after the close) | right | names the mailbox | right | m0 gives `receive` no `closed` |
| `t2-p-4` | 0.324224 | refused | no line, never ends | right | names the mailbox | right | the same |
| `t2-p-5` | 0.347129 | refused | no line, an abort | right | names the mailbox | right | the same, the abort by analogy with § 13's copy rule |

**Per program: p1 0, p2 0, p3 5, p4 0, p5 4.** On m0 the send after the close is a compile error in all
five sessions (a mailbox read after `close` ended its life as a handle's), never an abort.

## t3, arm `m` (letter `y`): a keeper thread holding a map, four workers asking it

| folder | cost USD | builds, as it reasoned | the keeper's stop | the reply | rows (1) to (7) | choice points it reported, in short |
|---|---|---|---|---|---|---|
| `t3-y-1` | 0.268892 | accepted | `main` closes the inbox after the four waits; `closed` returns the map | a reply mailbox inside each request | right | the close or a `stop` case; `close()` read as `()`; **four names, not an array of threads**; a mailbox in a record crosses without `@` |
| `t3-y-2` | 0.248214 | accepted | the close; any error breaks | the same | right | the close; a mailbox not a handle; `.err _ => break`; **an array of `(thread i64)`: copy or move unsaid** |
| `t3-y-3` | 0.221234 | accepted | a `stop` case of a `variant Request`, sent after the four waits | the same | right | the close or the case; **handles in an array may be refused or abort**; a case typed where it is bound |
| `t3-y-4` | 0.215334 | accepted | the close | the same | right | the close; `fold`'s `acc` and `item`; **waiting on copies from an array unclear** |
| `t3-y-5` | 0.225064 | accepted | the close | the same | right | the close; a mailbox not a handle; `close()`'s result unsaid; **an array and `for t in ts` would wait on copies** |

All five predicted `red: 402`, `green: 400`, `blue: 398`, `largest: 402` and a normal end: **5 of 5 right**.
All five name the array of threads as a choice they avoided (the same `grep`: 5 of 5).

## The cells, and the registered decision rules

| cell | right of 5 | voided |
|---|---|---|
| t1 `now`, reader's rows | 5 | 0 |
| t1 `now`, mechanical (beside, decides nothing) | 5 | 0 |
| t1 `m` | 5 | 0 |
| t2 `m`, p1 / p2 / p3 / p4 / p5 | 5 / 5 / 5 / 5 / 5 | 0 |
| t2 `m0`, p1 / p2 / p3 / p4 / p5 | 0 / 0 / 5 / 0 / 4 | 0 |
| t3 `m` | 5 | 0 |

- **D1**: `E1 = right(m) - right(now) = 5 - 5 = 0`; **D1 does not hold**: the record crossing is not read as
  serving the thesis by the registered count. On this task a reader of `now` wrote a framing no character
  confuses, five times of five, and it ran.
- **D2**: `d` per program 5, 5, 0, 5, 1; `d >= 3` on p1, p2 and p4, three programs: **D2 holds**, the
  proposal's sentences read as needed; **its text suffices (`right(m) >= 4`) for all five programs**.
- **D3**: `right(m) = 5`: **D3 holds**; no session not right, so no class to give.

## The registered predictions, scored

The spec-warden's W1 to W11 as registered at 02:11 (`warden-at-start.md`, sha256
`fd1418d769bd62b229d85114bab6ecac070b33f5ff8db23de681cfec678aee6b`, read again by `shasum -a 256`; the
resume copy `warden-at-resume-20261122-092734.md` holds the same bytes by `cmp`). *"The point is what
scores"*: **hit** where the point is the count, **miss** otherwise, the range said beside it.

| prediction | registered | counted | score | deciding sessions |
|---|---|---|---|---|
| W1, t1 `now` by the reader's rows | 2 of 5 (1 to 3) | 5 | **miss**, outside its range | `t1-q-1` to `t1-q-5` |
| W2, t1 `now` mechanical | 2 of 5 (1 to 3) | 5 | **miss**, outside its range | `t1-q-1` to `t1-q-5` (`grade-t1-now.py`) |
| W3, t1 `m` | 4 of 5 (4 to 5) | 5 | **miss** on the point, inside its range | `t1-y-1` to `t1-y-5` |
| W4, D1 | E1 = 2, D1 does not hold | E1 = 0, D1 does not hold | **hit** on the rule's verdict; its E1 missed (0, not 2) | `t1-q-*`, `t1-y-*` |
| W5, t2 `m` p1 to p5 | 5, 5, 4, 4, 2 (p5 1 to 3) | 5, 5, 5, 5, 5 | **miss**: p1 and p2 hit, p3 and p4 inside their ranges, p5 outside | `t2-y-1` to `t2-y-5` |
| W6, t2 `m0` p1 to p5 | 0, 0, 2, 0, 0 (p3 0 to 3, p5 0 to 1) | 0, 0, 5, 0, 4 | **miss**: p1, p2 and p4 hit, p3 and p5 outside | `t2-p-1` to `t2-p-5` |
| W7, D2 | holds, `d >= 3` on p1, p2, p4; d 2 on p3 and p5; the text suffices for p1 to p4, not p5 | holds on p1, p2, p4 (d 5, 5, 5); d 0 on p3, 1 on p5; suffices for all five | **hit** on the rule's verdict and its three programs; **miss** on *"not for p5"* | `t2-y-*`, `t2-p-*` |
| W8, t3 `m` | 4 of 5 (3 to 5) | 5 | **miss** on the point, inside its range | `t3-y-1` to `t3-y-5` |
| W9, D3 | holds | holds | **hit** | `t3-y-*` |
| W10, voided | 0 | 0 | **hit** | all 26 (`scan.py`, `reads.py`, `run.json`) |
| W11, the verdict | approve | approve | **hit** | `verdict` |

Counted: three hits on the point (W9, W10, W11), two on a rule's verdict with its number missed (W4, W7),
two misses inside their ranges (W3, W8), four outside them (W1, W2, W5, W6). **Every count of right sessions
the warden missed, it missed low** (W4's E1, a difference, it put at 2 and it read 0): readers did better than predicted on `now` (W1, W2: the length-prefixed framing
the warden called *"a long hand-written program"* was written by five of five and ran under all four
substitutions), on `m`'s p5 (W5) and on `m0`'s p3 and p5 (W6).

The historian's **P2** (*"the blind seat's t2 session on a send after the close will predict a run-time
abort (Go's rule) more often than the `closed` answer S6 states, on arm `m`"*,
`254-reports/historian.md:709-711`): on `m`, `send: closed` 5 of 5 and an abort 0 of 5. **Miss**,
`t2-y-1` to `t2-y-5`. (On `m0` the same send is a compile error in 5 of 5, never an abort.)

The seat's own prepared report (`254-reports/llm-ergonomist.md`) registers no count; its one predictive
sentence is that under the resolution's stop *"the same program prints `got queued`"*, which only the
fourth arm can score (below). Its decision rules D1 to D3, registered in
`docs/panel/254-briefs/llm-ergonomist.md`, are applied above.

## R9's fourth arm: the resolution's text, run 2026-11-22

**It had not run**: `order.txt` names 26 folders over the letters `q`, `y` and `p`, and `key.txt` three arms
(`now`, `m`, `m0`). So the scoring session ran it, bounded at 5.00 USD for the arm by the coordinator's
brief (the author's 20 USD for this seat; 6.445551 spent; the total never past 11.45).

- **The arm**: `spec-r.md`, the frozen spec with `254-reports/resolution/spec-r.diff` applied, made two
  ways that give the same bytes, `patch` on the frozen spec and `resolution/make_resolution.py` on the
  spec-warden's `spec-w2.md` (sha256 `0c06f838...`): sha256
  `c21c162d2612592f278ce80f40070e1716f2801b0191ffc7b414ac94c77dbb4b`, the hash R1 names.
- **Registered before any session of it**: `docs/panel/254-briefs/blind/arm-r-registration.md`, written
  10:45:21, sha256 `dda5c922fe4d85aaef5f3ac12e25c73f166a726c84aa87fb0720d91340260dfb`, when no session folder
  of the arm existed; the runner's gate copied it to `registration-at-start.md` at 10:45:51 and logged the
  same hash. It says plainly that its predictions were written after the first 26 sessions had been
  scored. t2's program 3 row re-registered as `got queued`; program 5's ending registered to count right
  only with the row's own reason (the handle's life ended, or the send emptied it), a reason of R3's alone
  (*"a thread that does not hold it"*) counted apart and not right.
- **The machinery**: a second area, `.claude/worktrees/p254-scratch/254-llm-ergonomist-4/`, made as the
  first; the first area's tools unchanged but three made from it by asserted edits
  (`254-briefs/blind/arm-r-adapt.py`): `arm-r-make_sessions.py` (one arm, three cells, no verdict session, the first
  draw's letters out of the pool), `arm-r-check_folders.py` (15 folders), `arm-r-run.sh` (each session
  capped at 0.60 USD, the arm at 5.00, the gate on the registration). Every flag, variable, the free
  preflight and the leak checks as in the first run.
- **The draw**: `r` drawn `w`; 15 folders, `check_folders.py` and `check_area.py` exit 0.
- **The run**: 10:45:51 to 11:04:36 (`run.log`); the free preflight cost 0, its listing of
  `/private/tmp/claude-501/` unchanged (0 lines differ), its scan clean; **15 sessions, every one
  `success`, `is_error` false, `terminal_reason` `completed`, `claude-opus-5-5` alone; 3.370710 USD**
  (`run.log`'s last line; the dearest session 0.268831, none near its 0.60 cap).
- **The leak check**: `scan.py`, 16 transcripts (the 15 and the preflight): **prompt-alone=NO 0,
  git-status=LEAK 0, claude-md-or-memory=YES 0**; `reads.py`: 16 transcripts, **0 with a path outside its
  folder**, 0 without a cwd. **0 voided.** No `context` answer names anything but its folder's files and
  the agent's setup; none names the path's words. 15 of 15 again had no working `Write`.

| folder | cost USD | t1: rows | t2: p1 / p2 / p3 / p4 / p5 | t3: rows | p5's stated reason, and what it chose that the text left open |
|---|---|---|---|---|---|
| `t1-w-1` | 0.213319 | right | | | the threads in four names (an array of threads: unsaid); `chars()` with a trailing space |
| `t1-w-2` | 0.182231 | right | | | the same, its point 6; `print` rather than an `f` literal (`to_str` of a `str` unsaid) |
| `t1-w-3` | 0.191471 | right | | | the same, its point 3; bytes and `slice` |
| `t1-w-4` | 0.194031 | right | | | the same, its point 8; `len`'s type |
| `t1-w-5` | 0.178551 | right | | | the same, its point 1 |
| `t2-w-1` | 0.249396 | | right / right / right / right / right | | the ended life (*"sent to another thread"*); p3 by *"finds it empty"* |
| `t2-w-2` | 0.245374 | | right / right / right / right / right | | the ended life; p3 because § 11 and § 13 make `stopped` only for a receive that finds it empty |
| `t2-w-3` | 0.248856 | | right / right / right / right / right | | both: § 13's ended life, *"also"* R3's sentence |
| `t2-w-4` | 0.234156 | | right / right / right / right / right | | both rules, named as two |
| `t2-w-5` | 0.226476 | | right / right / right / right / right | | both: *"the handle's life has already gone, and the main thread no longer holds it"* |
| `t3-w-1` | 0.268831 | | | right | the close; four names, not `[(thread i64)]` |
| `t3-w-2` | 0.231296 | | | right | the close; four names, its point 7 |
| `t3-w-3` | 0.226556 | | | right | the close; four names, its point 5 |
| `t3-w-4` | 0.212546 | | | right | the close; four names, its point 4 |
| `t3-w-5` | 0.267626 | | | right | the close; four names, its point 9; `close()` read as `()` |

What they wrote on t2: p1 `first: timeout`, `send: closed`, `got 1`, `got 2`, `last: closed`, `done`,
normally; p2 `total 12`; **p3 `got queued`, normally, 5 of 5**, each naming `receive: stopped` as the other
reading; p4 `main ends`, then an abort at `main`'s return for one thread never waited for, no mailbox named;
p5 `sent`, `freed 1`, then an abort at `free(b)`. **Per program: p1 5, p2 5, p3 5, p4 5, p5 5. t1 5 of 5,
t3 5 of 5.** The array of threads named as a choice in 10 of 10 t1 and t3 sessions (the first run's
command: 1 or 2 lines each).

### The fourth arm's registered predictions, scored

| prediction | registered | counted | score | deciding sessions |
|---|---|---|---|---|
| R-1, t1 `r` | 5 (4 to 5) | 5 | **hit** | `t1-w-1` to `t1-w-5` |
| R-2, t2 `r` p1 to p5 | 5, 5, 4, 5, 4 (p3 3 to 5, p5 3 to 5) | 5, 5, 5, 5, 5 | **miss** on two points, p3 and p5, each inside its range; p1, p2, p4 hit | `t2-w-1` to `t2-w-5` |
| R-3, t3 `r` | 5 (4 to 5) | 5 | **hit** | `t3-w-1` to `t3-w-5` |
| R-4, R9's condition | no row reopened | none reopened (below) | **hit** | the 15 |
| R-5, the array of threads named | 10 of 10 (8 to 10) | 10 of 10 | **hit** | `t1-w-*`, `t3-w-*` |
| R-6, p5 by R3's reason alone | 1 (0 to 2) | 0 (2 the ended life alone, 3 both) | **miss** on the point, inside its range | `t2-w-1` to `t2-w-5` |
| R-7, voided | 0 | 0 | **hit** | the 15 |
| R-8, the arm's spend | 3.40 (2.80 to 4.30) | 3.370710 | **miss** on the point, inside its range | `run.log` |
| R-9, D1, D2, D3 on `r` | E1 0, D1 does not hold; D2 holds; D3 holds | E1 = 5 - 5 = 0; d against `m0` 5, 5, 0, 5, 1, so D2 holds on p1, p2, p4; D3 holds | **hit** | the 15, with `t1-q-*` and `t2-p-*` |

The seat's own predictive sentence (`254-reports/llm-ergonomist.md`: under the resolution's stop *"the same
program prints `got queued`"*): **hit**, 5 of 5, `t2-w-1` to `t2-w-5`.

## R9's condition, row by row

*"A reading that shows a row of R2 read wrong by two or more of five readers reopens that row's text."* A
row is read wrong by a session when its answer is wrong on a task in the part the row decides (the map
registered in `arm-r-registration.md`). Arm `r` reads R2's own text; arm `m` reads the proposal, whose text
is R2's or near it on the rows marked so.

| R2 row | read by | wrong on `m` | wrong on `r` | reopened |
|---|---|---|---|---|
| S1 `spawn` | t1, t2 p2 p4 p5, t3 (`spawn(...).must()` in every one) | 0 of 15 | 0 of 15 | no |
| S2 `spawn_sized` | no task | | | unread |
| S3 `t.wait()` | t1, t2 p2 and p4, t3 | 0 | 0 | no; but see below: what a copy of a thread is, named by 10 of 10 on each arm and read wrong by none, since no graded program depends on it |
| S4 `mailbox()` | t2, t3 (each binds a mailbox with its type written) | 0 | 0 | no; R2's new form, a `mailbox()` typed by a constructor's field, written by none |
| S5 the life | t2 p4 (the abort names no mailbox) | 0 | 0 | no; the count and the queue's leak unread |
| S6 `m.send(v)` | t2 p1 (a send after the close: `closed`) | 0 | 0 | no; *"`v` then left as it was"* unread (p1 sends a literal) |
| S7 `m.receive` | t2 p1, p2, p3 | 0 on p1 and p2 (p3 was `m`'s own stop) | 0, p3 `got queued` 5 of 5 | no; the value after a `timeout` unread |
| S8 `m.close()` | t2 p1 | 0 | 0 | no |
| S9 what crosses | t1 (records), t3 (a mailbox in a record), t2 p5 (a mailbox of handles into `spawn`) | 0 | 0 | no |
| S10 never crosses | no task | | | unread |
| S11 a handle | t2 p5 | 0 (`m`'s text) | 0; R3's sentence cited beside the ended life by 3 of 5, alone by 0 | no |
| S12 the stop | t2 p3 | (`m`'s own stop) | 0 | no; *"from then on"*, a send after `stopped`, unread |
| S13 the names | every task (each operation written after a dot) | 0 | 0 | no; W-h's *"no function so named may take a thread or a mailbox first"* unread |
| S14 the doors | arm `now` only, which is not R2's text | | | not a reading of R2 |
| boundary | no task | | | unread |
| R5, no queue holds itself | t3 (a request carrying a `(mailbox i64)` through a `(mailbox Request)`, refused by none) | 0 | 0 | no |

**No row of R2 is reopened**: no row was read wrong by two readers, or by one, on either arm. Seven rows or
parts of rows were read by no task (marked *unread*), and R9's condition says nothing of them.

**The silence the run did find**: what copying a `(thread T)` means. 10 of 10 t1 and t3 sessions on `m` and
10 of 10 on `r` kept their threads in named bindings and said why: whether a thread pushed into an array, or
bound to a `for` variable, is a copy whose `wait` ends the thread, leaves the array's copy *"a thread nobody
waits for"*, is refused, or aborts, the text does not say. The verdict session named it in the same words
and wrote the shape. Neither arm's text answers it: both list `ptr`, `cstr` and `(mailbox T)` as copied
addresses and no thread (`spec-r.md:117`, `spec-m.md:116`), and the thread row says only *"a handle (section
13) with no null, which its `wait` ends"*. A search of the sitting, its briefs and its reports for an array,
a list or a copy of a `(thread T)` found only C handles copied across a send (R3's holder check); this is a
search with those words, not a proof that no seat considered it. R9's condition counts readings, not
silences, so it reopens nothing here; whether the text owes a sentence is the author's and the lane's
question.

## The appendix: every answer whole

`254-reports/llm-ergonomist-blind-answers.tar.xz` holds, byte for byte, every session's `result.md` and
`run.json` of both runs (41), each run's `run.log` and `runner.out`, the fourth arm's `key.txt`,
`order.txt`, `registration-at-start.md` and preflight `run.json`, the by-letter record named above, and
the mechanical grade's log (`mech-t1-now.log`: 25 lines `right`, 5 `RIGHT=True`, 0 `WRONG`). The answers
are kept in an archive rather than re-typed here, so that their bytes, the sessions' own, stay exactly what
the sessions wrote. The transcripts stay in the two areas under `home/.claude/projects/`, uncommitted.

## The A/B verdict, reproduced whole

The verdict session (`verdict/`, 0.368425 USD) was handed `spec-A.md` (the frozen spec, `now`), `spec-B.md`
(`m`, the proposal) and their `changes.diff`, which equals `docs/panel/254-briefs/arms/spec-m.diff` below its
two header lines (`diff`, run by the scoring session) and is not the resolution's `spec-r.diff`: **the verdict
judged the proposal, not the text R1 adopted**. Its answer, as `tools/after.py` kept it in `result.md`, byte
for byte:

````markdown
I couldn't write `report.md` because I have no file-writing tool here, only Read, so the whole report follows below as the brief allows. I also didn't run `heroes build`: whether each program is accepted, what it prints and how it ends are worked out from the specifications, not from a run. I read `spec-A.md`, `changes.diff` and `task.md` in full. I treated `spec-B.md` as `spec-A.md` with `changes.diff` applied, because the brief says that is the only difference, and did not open it on its own.

# report.md

## verdict

**approve**

## experiment

### Under spec-A: `main.hero`

```
extern "hero_os.h"
    function hero_thread_spawn_str(body: (function(str) -> str), arg: str) -> i64
    function hero_thread_join_str(handle: i64) -> str

record Doc
    name: str
    text: str

record Tally
    name: str
    words: i64
    distinct: i64

function escape(s: str) -> str
    parts: [str] @= []
    for c in s.chars()
        if c == "\\"
            parts @ parts.push("\\\\")
        else if c == "|"
            parts @ parts.push("\\|")
        else
            parts @ parts.push(c)
    return join(parts, "")

function pack(fields: [str]) -> str
    parts: [str] @= []
    for f in fields
        parts @ parts.push(escape(f))
    return join(parts, "|")

function unpack(s: str) -> [str]
    fields: [str] @= []
    cur: [str] @= []
    escaped @= false
    for c in s.chars()
        if escaped
            cur @ cur.push(c)
            escaped @ false
        else if c == "\\"
            escaped @ true
        else if c == "|"
            fields @ fields.push(join(cur, ""))
            cur @ []
        else
            cur @ cur.push(c)
    fields @ fields.push(join(cur, ""))
    return fields

function split_words(text: str) -> [str]
    words: [str] @= []
    cur: [str] @= []
    for c in text.chars()
        if c == " "
            words @ words.push(join(cur, ""))
            cur @ []
        else
            cur @ cur.push(c)
    words @ words.push(join(cur, ""))
    return words

function contains(xs: [str], x: str) -> bool
    for e in xs
        if e == x
            return true
    return false

function digit(c: str) -> i64?
    return match c
        "0" => ok(0)
        "1" => ok(1)
        "2" => ok(2)
        "3" => ok(3)
        "4" => ok(4)
        "5" => ok(5)
        "6" => ok(6)
        "7" => ok(7)
        "8" => ok(8)
        "9" => ok(9)
        _ => fail(code: "not_a_digit", msg: c)

function to_count(s: str) -> i64?
    n @= 0
    for c in s.chars()
        n @ n * 10 + digit(c)?
    return ok(n)

function job_fields(d: Doc, stop: [str]) -> [str]
    fields @= [d.name, d.text]
    for s in stop
        fields @ fields.push(s)
    return fields

function count_packed(packed: str) -> str
    f = unpack(packed)
    stop = f.slice(from: 2, to: f.len())
    words @= 0
    seen: {str: bool} @= {}
    for w in split_words(f[1])
        if w != "" && !contains(stop, w)
            words @ words + 1
            seen[w] @ true
    return pack([f[0], to_str(words), to_str(keys(seen).len())])

function main()
    docs = [
        Doc(name: "a", text: "the cat sat on the mat")
        Doc(name: "b", text: "a dog and a cat")
        Doc(name: "c", text: "to be or not to be")
        Doc(name: "d", text: "x")
    ]
    stop = ["the", "a", "to"]
    handles: [i64] @= []
    for d in docs
        handles @ handles.push(hero_thread_spawn_str(count_packed, pack(job_fields(d, stop))))
    for h in handles
        r = unpack(hero_thread_join_str(h))
        t = Tally(name: r[0], words: to_count(r[1]).must(), distinct: to_count(r[2]).must())
        print(f"{t.name}: {t.words} words, {t.distinct} distinct")
```

### Under spec-B: `main.hero`

```
record Doc
    name: str
    text: str

record Job
    doc: Doc
    stop: [str]

record Tally
    name: str
    words: i64
    distinct: i64

function split_words(text: str) -> [str]
    words: [str] @= []
    cur: [str] @= []
    for c in text.chars()
        if c == " "
            words @ words.push(join(cur, ""))
            cur @ []
        else
            cur @ cur.push(c)
    words @ words.push(join(cur, ""))
    return words

function contains(xs: [str], x: str) -> bool
    for e in xs
        if e == x
            return true
    return false

function tally(job: Job) -> Tally
    words @= 0
    seen: {str: bool} @= {}
    for w in split_words(job.doc.text)
        if w != "" && !contains(job.stop, w)
            words @ words + 1
            seen[w] @ true
    return Tally(name: job.doc.name, words: words, distinct: keys(seen).len())

function main()
    docs = [
        Doc(name: "a", text: "the cat sat on the mat")
        Doc(name: "b", text: "a dog and a cat")
        Doc(name: "c", text: "to be or not to be")
        Doc(name: "d", text: "x")
    ]
    stop = ["the", "a", "to"]
    threads: [(thread Tally)] @= []
    for d in docs
        threads @ threads.push(spawn(tally, Job(doc: d, stop: stop)).must())
    for t in threads
        r = t.wait()
        print(f"{r.name}: {r.words} words, {r.distinct} distinct")
```

### Expected result for both programs (worked out from the specs, not run)

`heroes build main.hero` should accept either program. Each should print:

```
a: 4 words, 4 distinct
b: 3 words, 3 distinct
c: 4 words, 3 distinct
d: 1 words, 1 distinct
```

Each should end normally with exit code 0, since every thread is joined or waited for. Spec-A is about 70 lines longer: everything from `escape` to `job_fields` exists only to carry the data through a single `str`.

## choice_points

### spec-A

1. **How to fit a document and the left-out words into one `str`.** I escaped `\` and `|` and joined the fields with `|`.
   - Joining naively with `|` or `\n`, which is the obvious choice: the build accepts it, and it gives a different answer than intended when a name or text holds that character. The task warns about exactly this case. This is the enemy.
   - Length-prefixed fields: correct, but it needs number parsing and byte slicing, which aborts if a slice splits a character.
2. **`hero_thread_spawn` (i64) or `hero_thread_spawn_str`.** I chose `_str`.
   - The i64 version can't carry the data, so the compiler refuses it.
3. **Matching join to spawn.** I used `hero_thread_join_str`.
   - `hero_thread_join` on a thread started with `_str`: both handles are plain `i64`, so the build accepts it. Run-time behaviour is unspecified (garbage or a crash). This is the enemy.
4. **Spawn every thread before waiting on any.** I used two loops.
   - Joining inside the spawn loop: the build accepts it, the output is the same, and the work runs one at a time, which breaks the task without anything noticing. This is the enemy.
5. **Joining every handle.** I joined them all.
   - Forgetting one stops with an error when `main` returns. The handle is just an `i64` the compiler doesn't track, so nothing catches it at build time.
6. **The `extern "hero_os.h"` group, with or without `link`.** I wrote no `link`.
   - The spec doesn't say whether `link` is needed. A wrong guess fails at link time.
7. **Bringing the counts back as numbers.** I parsed them with `match` on digit strings.
   - Printing the strings as they come gives the same output, but the thread then doesn't really give back counts.
8. **Whether `len` gives `i64` or `u64` when passed to `slice` and stored in an `i64` field.** I assumed `i64`.
   - If it is `u64`, the compiler refuses. That is a win.
9. **Empty words, e.g. an empty text.** I skip them.
   - Counting `""` as a word gives a different answer, and the build accepts it.

### spec-B

1. **`spawn(f, x)` takes one argument, so how to hand over the document and the word list.** I bundled them into `record Job`.
   - Passing only `Doc` and writing the list inside the thread: the build accepts it and the output is right, but it breaks "handed the list". This is minor.
2. **Spawn every thread before waiting on any.** I used two loops.
   - Calling `.wait()` inside the spawn loop: accepted, the same output, and it breaks the task. This is the same enemy as spec-A point 4.
3. **Handling `spawn`'s `?`.** I used `.must()`.
   - Leaving it out means calling `.wait()` on a `T?`, which the compiler refuses. `_ = spawn(...)` is also refused. Wins.
4. **`t.wait()` or `wait(t)`.** I used the dot form.
   - `wait(t)` is refused. A win.
5. **Waiting on every thread.** I waited on all of them.
   - Forgetting one stops with an error when `main` returns. It is visible, but only at run time.
6. **Waiting through a `for` loop variable while `threads` still holds copies.** The spec doesn't say what copying a `(thread T)` means; the diff adds `mailbox` to the "copied ADDRESS" list but not `thread`. I waited once per thread.
   - Waiting twice could only end in an error or a refusal, never a silent wrong answer.
7. **The thread's function.** I wrote no `lent` on its parameter.
   - `job: Job lent` makes it something that can't be a function value, so the compiler refuses. A win.
8. **Empty words and the type of `len`.** Same as spec-A points 8 and 9.
9. **Data crossing the thread.** No choice is left: `|` and newlines cross unchanged inside the record.

## argument

Under spec-A the task forces me to pack the data into strings by hand. That creates the worst kind of mistake: a naive `|` or newline join that the build accepts and that gives wrong answers on the inputs the task warns about. It also allows a `join`/`join_str` mix-up that compiles, and an `i64` handle the compiler doesn't track. Spec-B removes all of these. Records cross the thread whole, the thread's type carries its answer type, a forgotten `?` is refused, and `wait(t)` is refused. The only plausible mistake left that the build accepts, waiting inside the spawn loop, exists under both specs. Every new construct (`spawn`, `t.wait()`, `(thread T)`) can be read from its line. The dot-only exception to UFCS is a wrinkle, but it fails loudly.

## condition

- **Toward object:** if a build showed that spec-B refuses `t.wait()` on a `for` loop variable, or a `(thread T)` inside an array, without spec-B saying so, a model would have to guess.
- **Toward object:** if waiting on a copy of an array entry stopped with an error at run time.
- **Toward veto:** if a task using `mailbox` showed that the rule for when `receive` gives `stopped` can't be followed from its line.
- **Toward reconsidering:** if spec-A's naive-separator program turned out to be refused by some rule I missed.

## context

Only my system setup reached me: that I am a Claude agent on the Agent SDK, the working directory, and the date. I read `brief.md`, `changes.diff`, `task.md` and `spec-A.md`, and nothing outside this directory. No rule, contract, memory or other document reached my context.
````
