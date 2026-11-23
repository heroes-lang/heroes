# Panel 256, llm-ergonomist (the blind seat): the paid run, scored

Written 2026-11-23 by the session that scored the blind seat, started by the coordinator of M-rebel-library
after the paid run of 2026-11-22 and 23. Every number below is a command's, named beside it; the seats had no
compiler, so a session's *accepted* or *refused* is its own reading of its arm's text, never a build. **The
scoring session ran no session, no `claude -p` and no API call: its own spend is 0.**

## How it was run

- **The run**: 46 fresh `claude -p` sessions, started 2026-11-22 23:16:32 and ended 2026-11-23 00:12:56
  (`run.log`), by `run.sh` as registered (`docs/panel/256-briefs/blind/run.sh`, the same bytes by `cmp`), from
  the session that held the author's word. Each ran in a folder of its own under
  `.claude/worktrees/p256-scratch/256-llm-ergonomist/s/` (the verdict in `verdict/`), an area that is the top of an
  empty git repository of its own, ignored by this repository and holding no `CLAUDE.md`: `env -i`, `HOME` inside
  the area, `< /dev/null`, `--bare --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`,
  `--tools "Read,Write" --allowedTools "Read,Write"`, `--max-budget-usd` 1.00 (the verdict 1.50)
  (`run.sh:43-46,106-113`); Claude Code 2.1.323. The free preflight ran first, three times (23:02:41, 23:03:05,
  23:16:32): cost 0, its scan clean. **All 46 ran**: the runner's ceiling (19.50 USD, `run.sh:46`) was never near.
- **What was registered before, and read again**: the spec-warden's predictions (22:14, S26 to S28 added 22:23),
  `warden-at-start.md`, sha256 `2bd2ae1ccdc6be5ebcd5eb0c70c50ca2952b8b34d4733b8be505c51855dcf91f` (`shasum -a 256`;
  `cmp` against `256-reports/spec-warden.md` in the lane: the same bytes); the coordinator's `registration.md`,
  sha256 `fe6e3cb39fe73024671587b11cbf666dc06b6e32a24efc4dd96fc0b2ec326f92`, re-written and re-hashed at
  23:03:04, before any paid session (`run.log`; `RUN.md` says why: three clock times removed, predictions
  unchanged). **All eleven input hashes of that registration** (the four arms, both briefs, the four tasks,
  `make_sessions.py`) **re-read by the scoring session with `shasum -a 256`: 11 of 11 equal.** The tools in the
  area equal the ones committed under `256-briefs/blind/` (`cmp`, no output for the 8 scripts and the 7 texts
  compared).
- **The model**: `claude-opus-5-5` alone in every `run.json`'s `modelUsage`, 46 of 46 (`tools/read_runs.py .`).
- **The sessions**: 46, 45 graded and the A/B verdict; every one `subtype` `success`, `is_error` false,
  `terminal_reason` `completed` (`read_runs.py`, 46 lines).
- **The cost**: **10.775607 USD**: `run.log`'s last line (*"spent 10.775607 USD in all"*), `tools/spent.py`, and the
  sum of `read_runs.py`, three readings of the same 46 `run.json`. By cell, summed from the `run.json` (the
  scoring session's own python, kept in the archive): t1 `w` 1.248751, t1 `m` 1.421076, t2 `now` 1.249158, t2
  `gc` 1.049957, t2 `gi` 1.109787, t2 `gr` 0.877442, t3 `gc` 1.181202, t3 `gi` 1.133097, t3 `gr` 1.210372, the
  verdict 0.294765; the graded sessions 0.171964 to 0.377992 each (the dearest `t1-s-2`, its cap 1.00). Of the
  author's 20 USD for this seat, 9.224393 stand unspent.
- **The leak check**: `tools/scan.py`, 47 transcripts (the 46 and the preflight): **prompt-alone=NO 0,
  git-status=LEAK 0, claude-md-or-memory=YES 0**; `tools/reads.py`, run by the scoring session: 47 transcripts,
  **0 sessions with a path outside their folder**, 0 without a cwd; `tools/check_folders.py` and
  `tools/check_area.py`, run again after the run: exit 0 and 0. **0 sessions voided.** Every session's `context`
  answer says nothing but its folder's files and the agent's setup reached it; two name the words of their
  folder's path (`t1-j-1` and `t1-s-4`: *"llm-ergonomist"*), which the scan does not count, the area standing
  under the repository's root by CLAUDE.md § Hard stops; `t2-k-1`'s *"memory files"* is a negation.
- **No session wrote `report.md`**: 46 of 46 (`run.log`'s `report.md=no`, 46 lines; a pattern over the first 400
  bytes of each `result.md`, 46 of 46) open their answer saying they cannot write a file and give the report as
  their final answer, which `tools/after.py` kept as `result.md`. Why `--tools "Read,Write"` with `--bare` gives
  no working `Write` on this CLI is unread, as in panel 254; the brief's fallback held.
- **The blinding, as it was for the scorer**: **not blind in fact.** `RUN.md` says the draw is not blind to the
  scorer, and the scorer's orientation command listed `key.txt`'s rows (folder and arm) before any answer was
  read. The outcomes were written by folder's letter to `outcomes-by-letter.txt` (sha256
  `4e1a113503527f74dfeb099222052d4119c1b9208cefabd32e9a0be862f0fd69`, 2026-11-23 00:50:32) after reading the
  answers; what keeps the grading honest is the rows and the predictions, both written before the draw's
  sessions ran.
- **The compiler**: none read a `w`, `m`, `gc`, `gi` or `gr` arm, for those texts name forms no compiler has. The
  `now` t2 rows are the frozen compiler's (stage 2, `sources 031aaabb358218fd`, built and run by the sitting's
  coordinator 2026-11-22 before 23:00, `256-briefs/blind/t2-programs/coordinator-run.txt`), not re-run here.

## The draw

`key.txt`'s first line: `now=h gc=f gi=b gr=k w=j m=s`; its 46 rows give each folder's arm and its files'
hashes, and each folder's `heroes-spec.md` reads the arm's registered hash (`15804f3f...` for `now`, `w` and
`m`, `99e080b0...` for `gc`, `2d14c084...` for `gi`, `32b0504b...` for `gr`). **The `gi` arm is the frozen spec
with R1's sentence and nothing else; it is not `spec-r.md`** (`256-reports/resolution/spec-r.md`, sha256
`6c905ad57c68d63c...`): a word-level comparison of the two files (python `difflib`, run by the scoring session)
finds the one difference, R6's sentence (`m.number()`) and `number` in § 9's list of words written after a dot.
**So no session read R6's text** (§ What the run could not show). The key is published whole in
`256-reports/blind-key.md`.

## t1, the wake: arms `w` (letter `j`) and `m` (letter `s`), one task

Graded by the registered reader's rows (`256-briefs/llm-ergonomist.md` § The rows, rows (1) to (7)), the answers
read one by one: each `main.hero` (the longest fenced block holding `function main`) read for the wait, the wake,
the drain, the close and the language.

| folder | cost USD | arm | how a result reaches the wait | reader's rows | what it reported as a choice, in short |
|---|---|---|---|---|---|
| `t1-j-1` | 0.257132 | `w` | `poll.wake(job.waker)` after each send | right | **ring after the send, not before**; the drain with `receive(within_ms: 0)` until `timeout`; an empty event list a failure |
| `t1-j-2` | 0.231384 | `w` | after each send | right | the same; the event keys ignored; one record per worker |
| `t1-j-3` | 0.251244 | `w` | after each send | right | *"order of `send` and `wake` in the worker"*; the listener's key checked |
| `t1-j-4` | 0.243759 | `w` | after each send | right | send first, then wake; *"results that arrive with no ring of their own"* |
| `t1-j-5` | 0.265230 | `w` | after each send | right | send first, then wake; a failed `send` in a worker |
| `t1-s-1` | 0.268069 | `m` | the mailbox added to the set before any thread starts | right | one mailbox for four workers; the drain; a listener event a failure |
| `t1-s-2` | 0.377992 | `m` | added before the first wait | right | one shared mailbox; any other key a failure |
| `t1-s-3` | 0.255857 | `m` | added before the first wait | right | one shared mailbox; the drain in a function |
| `t1-s-4` | 0.267709 | `m` | added before the first wait | right | *"when the mailbox joins the set"*; the drain on the mailbox's key |
| `t1-s-5` | 0.251449 | `m` | added before the first wait | right | one shared mailbox, not four; `assert` at the drain's end |

**Counts: right(w) 5 of 5, right(m) 5 of 5, `lost(w)` 0, `lost(m)` 0** (no session built a wait that can sleep with
a result unseen; no session was classed otherwise). All ten wait only in `poll.wait` with `time.seconds(30)`,
drain by `receive(within_ms: 0)` until it fails, print `done 100` after the hundredth, wait for the four threads once
and close the set and the listener. All five `w` programs ring after the send (a `grep` of their choice points for
the order: 5 of 5 name it; the task text, *"Rings made before that wait are told once together"*, carries the trap,
as the coordinator's C5 said). **10 of 10 keep their four threads in a `[(thread i64)]` or `[(thread i64?)]` and wait
through `for t in threads`** (`grep -c '\[(thread'`, 1 in each of the 10), none naming it a choice: the sentence on a
copy of a `(thread T)` that `spec-now.md` carries (its line 383, the sentence panel 254's author's verdict records the lane writing on 2026-11-22) was read as written, 10
of 10; panel 254's blind seat had named that silence in the same words (its report, § The silence the run did find).

## t2, seven programs at a generic's edges: arms `now` (`h`), `gc` (`f`), `gi` (`b`), `gr` (`k`)

Graded by each arm's rows (`256-briefs/llm-ergonomist.md` § The rows, t2's table, and `blind/registration.md`):
a session is right on a program iff its acceptance, every line and its ending match, **and a refusal is right
only where its reason names the rule the row's arm refuses by**.

| arm, folder | cost USD | p1 | p2 | p3 | p4 | p5 | p6 | p7 |
|---|---|---|---|---|---|---|---|---|
| `gi`, `t2-b-1` | 0.236309 | right | right | right | right | right | right | right |
| `gi`, `t2-b-2` | 0.215949 | right | right | right | right | right | right | right |
| `gi`, `t2-b-3` | 0.235189 | right | right | right | right | right | right | right |
| `gi`, `t2-b-4` | 0.212709 | right | right | right | right | right | right | right |
| `gi`, `t2-b-5` | 0.209629 | right | right | right | right | right | right | right |
| `gc`, `t2-f-1` to `t2-f-5` | 0.202929, 0.236519, 0.183689, 0.228749, 0.198069 | 5 of 5 | 5 | 5 | 5 | 5 | 5 | 5 |
| `gr`, `t2-k-1` to `t2-k-5` | 0.172424, 0.177004, 0.179504, 0.176544, 0.171964 | 5 | 5 | 5 | 5 | 5 | 5 | 5 |
| `now`, `t2-h-1` to `t2-h-5` | 0.319481, 0.229539, 0.204724, 0.263489, 0.231924 | see below | | | | | | |

**The resolution's arm, `gi`, p1 to p7: 5, 5, 5, 5, 5, 5, 5.** What each wrote: p1 accepted, `false`, `7`, normal;
p2 accepted, `false`, `true`, normal; **p3 refused at the call** (line 9, `print(relay(m, d))`), p4 refused at the
call (line 7), p5 refused at the call (line 13), **p7 refused at `main`'s call** (line 12, `print(outer(m, d))`) with
the chain `outer` then `inner` named in the answer, 5 of 5 on each, and p6 accepted, `42`. The reasons, quoted
(`t2-b-4`): *"This call gives `T` = `db.Db`, which holds a handle. `relay` sends with `m.send(v)`, and that form is
allowed only for types that hold neither a handle nor a thread"* (p3); *"`hand` sends with `m.send(@v)`, and that
form is allowed only for types that hold a handle or a thread. `i64` holds neither"* (p4); *"`spawn(body, setup)`
would send a handle to a thread through `spawn`. A handle may cross only through a mailbox with `send(@v)` or as the
answer `wait` gives"* (p5); *"`outer` passes `A` on as `inner`'s `T`, and `inner` sends with `m.send(v)`, which is
not allowed for a type that holds a handle. The rule is judged on the concrete types each call gives"* (p7). **The
choice every `gi` reader reported**: where a generic's refusal is told. 5 of 5 name it and chose the call in `main`
(*"told at that call"*), saying the other reading, the `send` or `spawn` inside the generic or, for p7, the inner
call at line 7, is *"still a refusal; only the line differs"*. 0 of 5 predicted a program that builds and aborts.

**`gc`, p1 to p7: 5 each.** p1 and p3 and p7 refused for *"a value whose type names a type parameter ... sent only as
`m.send(@v)`, and `m.send(v)` of it is a compile error"* (`t2-f-1`: *"That is true even though `T` is `i64` at the
only call. The rule is judged where the generic is written, not at each call"*); p2 and p4 accepted, `false` then
`true` and `false` then `7`; p5 refused for the handle through `spawn`; p6 accepted. **`gr`, p1 to p7: 5 each**: all
seven refused, p1 to p4 and p7 for a `(mailbox T)` naming a type parameter and a `T` sent (`t2-k-1`: *"`(mailbox T)`
in a generic function; line 2 sends a `T`"*), p5 and p6 for `setup: S` handed to `spawn`.

**`now`, the frozen text, read two ways:**

| `now` | p1 | p2 | p3 | p4 | p5 | p6 | p7 |
|---|---|---|---|---|---|---|---|
| the frozen compiler's row | accepted, `false` `7` | **refused** `marker_mismatch` | accepted, `false` `true`, then an abort | **refused** `marker_mismatch` | accepted, `true`, then an abort | accepted, `42` | accepted, `false` `true`, normal |
| right of 5, **by the compiler** | 5 | **0** | **0** | **0** | **0** | 5 | **0** |
| right of 5, **by the frozen text as written** | 5 | 5 | 5 | 5 (undecided; accepted, its choice quoted by 5 of 5) | 5 | 5 | 5 |

p3, p5 and p7 refused in 5 of 5 sessions at the body's line (line 4), *"as instantiated by"* the call (*"`relay` as it
is instantiated with `T = db.Db` by the call on line 9"*, `t2-h-1`): **all five readers read § 11 as judged at each
call, which the frozen compiler does not do**; p2 and p4 accepted by 5 of 5, none predicting `marker_mismatch`
(`grep -c -i 'does not change\|marker_mismatch'` over the five: 0), and 5 of 5 naming *"whether `m.send(@v)` is
allowed when `v` is not a handle"* as the choice. **The gap between the compiler and the text is 25 of 25 readings
on the five programs where the two differ (p2, p3, p4, p5, p7)**: defect 2695 as a reader meets it; and it is the
`@` rule's missing half (the spec-warden's T0) that R1's first sentence now states, `gi` reading it 5 of 5 on p4.

## t3, a generic `deliver` for an `i64` and a `db.Db`: arms `gc` (`f`), `gi` (`b`), `gr` (`k`)

Graded by the arm's rows (§ The rows, t3), widened to every program the arm's text allows; each `main.hero` read,
and its receive limits, waits, spawns, generics and sends counted by `score-tools/t3-mech.py` (in the archive):
**15 of 15 receive with `within_ms: 5000` in both workers, wait twice, spawn twice.**

| folder | cost USD | arm | the program's shape | its stated reason | rows |
|---|---|---|---|---|---|
| `t3-b-1` | 0.264379 | `gi` | two generics, `deliver` (`m.send(v)`) and `deliver_owned` (`@v`), each used with its kind | one body carries one form of send | right |
| `t3-b-2` | 0.213279 | `gi` | `deliver<T>` with `m.send(v)` for the `41`, the handle sent in `main` as `closer_box.send(@conn)` | same | right |
| `t3-b-3` | 0.214439 | `gi` | the same shape | same | right |
| `t3-b-4` | 0.211659 | `gi` | the same shape | same | right |
| `t3-b-5` | 0.229339 | `gi` | the same shape | same (choice point 1) | right |
| `t3-f-1` | 0.236119 | `gc` | one `deliver<T>(box, @v: T)` for both; `n @= 41` | `m.send(@v)` is the only form in a generic | right |
| `t3-f-2` | 0.220859 | `gc` | the same | the same | right |
| `t3-f-3` | 0.241949 | `gc` | the same | the same | right |
| `t3-f-4` | 0.229794 | `gc` | the same | the same | right |
| `t3-f-5` | 0.252479 | `gc` | the same | the same | right |
| `t3-k-1` | 0.242354 | `gr` | no generic; each send written in `main` | `(mailbox T)` and a sent `T` are refused in a generic | right |
| `t3-k-2` | 0.264674 | `gr` | no generic; adds that a function value cannot take an `@` parameter | same | right |
| `t3-k-3` | 0.213374 | `gr` | `deliver_i64` and `deliver_db`, no generic | same | right |
| `t3-k-4` | 0.267994 | `gr` | `deliver_i64` and `deliver_db` | same | right |
| `t3-k-5` | 0.221974 | `gr` | `deliver_count` and `deliver_db` | same | right |

**`gi` 5 of 5, `gc` 5 of 5, `gr` 5 of 5.** **0 of 5 `gi` readers wrote the single `deliver` that the text refuses**
(5 of 5 name it, and say which call the compiler would refuse); 5 of 5 `gc` readers wrote it and said the cost, a
cell for the `41`. Each program closes the database once, in the closer, by the closer's `db.shut(@conn)`.

## The cells, and the registered decision rules

| cell | right of 5 | voided |
|---|---|---|
| t1 `w`, reader's rows / `lost` | 5 / 0 | 0 |
| t1 `m`, reader's rows / `lost` | 5 / 0 | 0 |
| t2 `now`, by the compiler, p1 to p7 | 5, 0, 0, 0, 0, 5, 0 | 0 |
| t2 `now`, by the frozen text, p1 to p7 | 5, 5, 5, 5, 5, 5, 5 | 0 |
| t2 `gc`, p1 to p7 | 5, 5, 5, 5, 5, 5, 5 | 0 |
| t2 `gi`, p1 to p7 | 5, 5, 5, 5, 5, 5, 5 | 0 |
| t2 `gr`, p1 to p7 | 5, 5, 5, 5, 5, 5, 5 | 0 |
| t3 `gc` / `gi` / `gr` | 5 / 5 / 5 | 0 |

- **D1, the wake**: `E1 = right(m) - right(w) = 5 - 5 = 0`; `lost(w) = 0`, `lost(m) = 0`. *The waker admits, in
  practice, a mistake the build accepts iff `lost(w) >= 1`*: **does not hold**. *The mailbox in the set closes it iff
  `lost(m) = 0`*: holds, as it is 0 beside a `lost(w)` of 0. *The mailbox in the set is read as the form readers get
  right more often iff `E1 >= 2`*: **does not hold**. **The registered consequence applies**: `lost(w) = 0` and
  `right(w) >= right(m)`, so **the waker's case against it rests on the stress runs alone** (the ffi-pragmatist's
  320,799 to 397,379 of 400,000 values lost to a loop taking one per wake), and this record says so.
- **D2, the text predicts its rule (t2)**: the text suffices for a program iff `right >= 4` of 5, and the arm is read
  as written iff it suffices for at least six of the seven. `gc` 7 of 7, **`gi` 7 of 7**, `gr` 7 of 7: **D2 holds for
  all three.** For `now` both counts are reported and decide nothing.
- **D3 (t3)**: `gc` 5, `gi` 5, `gr` 5: **D3 holds for all three**; no session was not right, so no class to give.
- **Chance**: five readers per cell, one model, one run. Every cell of `gc`, `gi`, `gr` at 5 of 5 cannot tell the
  three arms' readability apart; D2 and D3 pass all three, and they do not choose between them.

## The registered predictions, scored

The spec-warden's S1 to S28 as registered at 22:14 and 22:23 (`warden-at-start.md`, sha256 above; programs 1 to
6, its table's width), and the coordinator's C1 to C18 as registered (`registration.md`, sha256 above). *"The
point is what scores"*: **hit** where the point is the count, **miss** otherwise, the range said beside it.

| prediction | registered | counted | score | deciding sessions |
|---|---|---|---|---|
| S1, t1 `w` right | 4 (3 to 5) | 5 | miss on the point, inside its range | `t1-j-1` to `t1-j-5` |
| S2, t1 `m` right | 5 (4 to 5) | 5 | **hit** | `t1-s-1` to `t1-s-5` |
| S3, `lost(w)` | 0 (0 to 1) | 0 | **hit** | `t1-j-*` |
| S4, `lost(m)` | 0 (0 to 0) | 0 | **hit** | `t1-s-*` |
| S5, E1 | 1 (0 to 2) | 0 | miss on the point, inside its range | `t1-j-*`, `t1-s-*` |
| S6, t2 `now` by the compiler, p1 to p6 | 5, 0, 0, 0, 0, 5 | 5, 0, 0, 0, 0, 5 | **hit on all six** | `t2-h-1` to `t2-h-5` |
| S7, t2 `now` by the text, p1 to p6 | 5, 5, 4, 5, 4, 5 | 5, 5, 5, 5, 5, 5 | hit on p1, p2, p4, p6; miss on p3 and p5, each inside its range (3 to 5) | `t2-h-*` |
| S8, t2 `gc`, p1 to p6 | 4, 5, 4, 5, 5, 5 | 5, 5, 5, 5, 5, 5 | hit on p2, p4, p5, p6; miss on p1 (2 to 5) and p3 (3 to 5), inside | `t2-f-*` |
| S9, t2 `gi`, p1 to p6 | 5, 5, 5, 4, 5, 5 | 5, 5, 5, 5, 5, 5 | hit on five; miss on p4 (3 to 5), inside | `t2-b-*` |
| S10, t2 `gr`, p1 to p6 | 5, 5, 5, 5, 5, 5 | 5, 5, 5, 5, 5, 5 | **hit on all six** | `t2-k-*` |
| S11, t3 `gc` | 5 (3 to 5) | 5 | **hit** | `t3-f-*` |
| S12, t3 `gi` | 4 (3 to 5) | 5 | miss on the point, inside its range | `t3-b-*` |
| S13, t3 `gr` | 4 (2 to 5) | 5 | miss on the point, inside its range | `t3-k-*` |
| S14, D1: the waker admits a mistake | does not hold | does not hold | **hit** | `t1-j-*` |
| S15, D1: the mailbox closes it | holds | holds | **hit** | `t1-s-*` |
| S16, D1: `E1 >= 2` | does not hold | does not hold (E1 = 0) | **hit** | `t1-*` |
| S17, D2 for `gc` | holds, 6 of 6; its weakest p1 | holds, 7 of 7 (6 of 6 on p1 to p6) | hit on the rule; *"weakest p1"* not borne out (all at 5) | `t2-f-*` |
| S18, D2 for `gi` | holds, 6 of 6 | holds, 6 of 6 on p1 to p6, 7 of 7 | **hit** | `t2-b-*` |
| S19, D2 for `gr` | holds, 6 of 6 | holds, 7 of 7 | **hit** | `t2-k-*` |
| S20, D3 for the three | holds; `gr` the likeliest to fall | holds for all three at 5 | hit on the rule; *"likeliest to fall"* not borne out | `t3-*` |
| S21, voided | 0 (0 to 1) | 0 | **hit** | all 46 |
| S22 to S25, the arms' real deltas | | | scored in the sitting (four hits), not by this run | |
| S26, a resolution arm on `spec-wadopt.md`, t2 p1 to p6 5 each, t3 5, D2 and D3 hold | | the resolution's cell `gi` (`arm-gi.md`, **not** `spec-wadopt.md`, which adds § 9's sentence R5 queues): 5 each on p1 to p6, t3 5, D2 and D3 hold | **hit on the resolution's cell**; its own text did not run | `t2-b-*`, `t3-b-*` |
| S27, a resolution arm on `spec-wcq.md`, `gc` t2 p1 | 5 (4 to 5), above S8's 4 | the cell `gc` (`arm-gc.md`, not `spec-wcq.md`) p1 5 | **hit on the neighbour text**; the object text did not run | `t2-f-*` |
| S28, either arm against its unrepaired one, p4 | no lower | no unrepaired arm ran | **unscored** | |
| C1, t1 `w` | 4 (2 to 5) | 5 | miss on the point, inside | `t1-j-*` |
| C2, t1 `m` | 5 (4 to 5) | 5 | **hit** | `t1-s-*` |
| C3, `lost(w)` | 1 (0 to 2) | 0 | miss on the point, inside | `t1-j-*` |
| C4, `lost(m)` | 0 (0 to 0) | 0 | **hit** | `t1-s-*` |
| C5, D1's robustness reading holds (*"yes, weakly"*) | holds | does not hold (`lost(w) = 0`) | **miss** | `t1-j-*` |
| C6, t2 `gi` p1 to p7 | 5, 5, 5, 4, 5, 5, 4 (p4 3 to 5; p7 2 to 5) | 5, 5, 5, 5, 5, 5, 5 | hit on five; miss on p4 and p7, each inside its range | `t2-b-*` |
| C7, `now` p7: by the compiler / by the text | 0 (0 to 1) / 4 (3 to 5) | 0 / 5 | **hit** / miss on the point, inside | `t2-h-*` |
| C8, `gc` p7 | 4 (3 to 5) | 5 | miss on the point, inside | `t2-f-*` |
| C9, `gr` p7 | 5 (4 to 5) | 5 | **hit** | `t2-k-*` |
| C10, D2 holds for `gi`, `gc`, `gr` (six of seven at 4 or more) | holds | holds, 7 of 7 each | **hit** | |
| C11, t3 `gc` | 4 (3 to 5) | 5 | miss on the point, inside | `t3-f-*` |
| C12, t3 `gi` | 4 (3 to 5) | 5 | miss on the point, inside | `t3-b-*` |
| C13, t3 `gr` | 5 (4 to 5) | 5 | **hit** | `t3-k-*` |
| C14, D3 holds for the three | holds | holds | **hit** | |
| C15, 46 sessions, 0 voided | 46, 0 | 46, 0 | **hit** | `run.log`, `scan.py`, `reads.py` |
| C16, the spend | 11.5 (8.5 to 16.0) | 10.775607 | miss on the point, inside | `run.log` |
| C17, no row of the resolution read wrong by two of five `gi` readers | none | none (below) | **hit** | the `gi` cells |
| C18, D2 holds for `gi`, so Q2 is not reopened toward `gr` | holds | holds | **hit** | the `gi` cells |

**Counted.** The warden's S1 to S21: 14 hits whole (S2, S3, S4, S6, S10, S11, S14, S15, S16, S18, S19, S21, and S17
and S20 on their rules), two misses inside their ranges on t1 (S1, S5), two on t3 (S12, S13), and S7, S8, S9 with
five misses of their 18 program points, every one inside its range. **Every count of right sessions the warden
missed, it missed low** (S1, S7 p3 and p5, S8 p1 and p3, S9 p4, S12, S13: each counted above its point), the same
finding panel 254 recorded; **E1 (S5), a difference, missed the other way (1 predicted, 0 read), a consequence of
S1's miss.** The coordinator's C1 to C18, made knowing that finding: nine whole hits (C2, C4, C9, C10, C13, C14,
C15, C17, C18), C6 and C7 partly (five of seven and one of two points), six misses inside their ranges (C1, C3, C8,
C11, C12, C16), and C5 a miss.

**The historian's sentence on the waker** (`256-reports/historian.md:456-460`, *"a P1.4 waker puts on every program
the order GLib's example keeps by hand (state first, then the ring), and some program will break it"*, not in the
sitting's table): **0 of the 5 `w` programs broke the order**, each named it as a choice. A miss on this run's five,
on a task whose text names the trap. The historian's other sentence there, on a `gi` with no signature showing the
obligation producing messages whose reason is a line inside the body, is about a compiler's messages, which no
session read: **unscored** by this run.

## The resolution's rows, one by one

*"A row of the resolution that two or more of five `gi` readers read wrong reopens that row's text"* (registered
in `llm-ergonomist.md`; the author's condition of 2026-11-23 is the same measure). A row is read wrong by a session
when its answer is wrong on a task in the part the row decides. Arm `gi` reads R1's sentence; t1 `m` reads R7's
sentence in the task's own words; the other rows are not in any arm's text or are not a reader's matter.

| row | read by | wrong | reopened |
|---|---|---|---|
| R1, *"a send is written `m.send(@v)` exactly where `v` holds a handle or a thread, and `m.send(v)` everywhere else"* | t2 `gi` p1 (`send(v)` at `i64`, accepted), p2 (`send(@v)` at `db.Db`, accepted), p3 (`send(v)` at `db.Db`, refused), p4 (`send(@v)` at `i64`, refused); t3 `gi`, the two forms | 0 of 5 on each of p1 to p4; 0 of 5 on t3 | no |
| R1, *"judged at each call, on the types that call gives its type parameters"*, and *"a compile error, told at that call"* | t2 `gi` p3, p4, p5, p7 | 0 of 5 on each; the call named is `main`'s in 20 of 20 | no |
| R1, a spawn in a generic | t2 `gi` p5 (a handle, refused), p6 (an `i64`, accepted) | 0 of 5 on each | no |
| R1, *"the chain of generics named"* (the message) | no session read a message; p7's answers name `outer` then `inner` in 5 of 5 | 0 of 5 | no; the message text is the compiler's and was not read |
| R1, a thread's answer, a mailbox carrying `()`, a queue holding itself, a `ptr` or `cstr`, in a generic | no task | | unread |
| R2, a generic no type can satisfy refused at its declaration | no task (`i9` unbuilt) | | unread |
| R3, the emitter's crossing walk | not a reader's row: no sentence of the spec names it | | unread |
| R4, `heroes doc` prints the needs; **its registered reopening condition, *"program 7 read wrong by two or more of five `gi` readers"*** | t2 `gi` p7 | **0 of 5** | **no**; its second condition, a need carried three calls deep, is unrun (p7 is two deep) |
| R5, queued | no task | | unread |
| R6, `m.number()` | **in no arm** (`arm-gi.md` lacks it, § The draw) | | **unread, and could not have been read** |
| R7, a mailbox an entry of a set (`poll.add_mailbox<T>`) | t1 `m`, through the task's sentence | 0 of 5: the mailbox added before the first wait 5 of 5, the drain until `timeout` 5 of 5 | no |
| R8, its mechanism; R9, the user interface's wait; R10, 2660; R11; R12, the Windows bell | no task; runtime, package and defect rows | | unread |

**No row of the resolution is reopened**: none was read wrong by two readers of five, nor by one, in any cell.
R2, R3, R5, R6, R8, R9, R10, R11 and R12 were read by no task, R7 only through the sentence a task states.

**Silences the run found** (a search of the sitting, its briefs and its reports for the words below found none
answering; a search with those words, a question rather than a premise):
- *Which call a chain's refusal is told at.* The sentence says *"told at that call"*; for p7 `outer` calls `inner`
  at line 7 and `main` calls `outer` at line 12. 5 of 5 `gi` readers chose `main`'s call and named the other as
  possible; the compiler-engineer's built messages tell it at `main`'s call with the chain named (the sitting's
  R1), so the readers agree with the build. The sentence leaves it to the reader to infer; R1's message line is
  where it is said.
- *What `m.send(v)` leaves behind.* Not tested.

## The A/B verdict, summarised (reproduced whole at the end)

`verdict/` (0.294765 USD, task t3) was handed `spec-A.md` (the frozen spec, sha256 `15804f3f...`), `spec-B.md`
(**`arm-gi.md`**, sha256 `2d14c084...`) and `changes.diff` (sha256 `4f241c0e...`), which equals `diff -u spec-A.md
spec-B.md` below its two header lines (`diff`, run by the scoring session). **It judged R1's sentence on the frozen
spec, not `spec-r.md`**, so it did not read R6 either. Its verdict: **approve**. Under spec-B it wrote two generics
(`deliver`, `hand_over`) and the right program; under spec-A it wrote the single `deliver<T>(box, @v)`, *"Probably,
but spec A does not settle it"* whether the build accepts it. **That spec-A program is program 2's `hand<T>` renamed
(the same `@v` parameter and `box.send(@v)` body), which the frozen compiler refuses (`marker_mismatch`, the row
measured at the frozen commit)**: an inference, the verdict's own program not built. Its three conditions, scored:
- *Toward object, "A's compiler is shown to already judge generic sends at each call and to accept `@` on
  non-handles"*: shown the other way, p2 and p4's rows (the compiler refuses an `@` send in a generic body).
- *Toward object, "a model given B is shown to write the single `@v` deliver anyway, and to miss that the build
  refuses it"*: **0 of 5 `gi` t3 readers did** (§ t3).
- *Toward veto, "B's 'judged at each call' made error messages point into the generic's body rather than at the
  call"*: **neither shown nor refuted**: no session read a message; the readers predicted the call line in 20 of 20
  refusals; the build's messages are the compiler-engineer's, in the sitting.

## What the run could not show

- **A build.** Every *accepted* and every line is a reader's reading; the `gi`, `gc`, `gr`, `w` and `m` texts name
  forms no compiler has, and only the `now` rows are the compiler's (measured before the run, not here).
- **A message.** R1 says the body's file and line and the chain are named. Readers predicted a line, never read a
  message; the historian's concern (a reason inside the body, a line the call's author cannot connect) is about
  messages and stays what the compiler-engineer's built ones answer in the sitting.
- **Depth.** p7 is two generics deep. R4's second reopening condition, *"a need carried three calls deep"*, has no
  program. **Unrun; one more program of t2 at `gi`, five sessions of it at most 1.11 USD (the five `t2-b` cost 1.109787
  with seven programs each; an inference for one program)**: a question for the author, not run here.
- **Rows no task read**: R2, R3, R5, R6, R8, R9, R10, R11, R12; and in R1 a thread's answer through a generic, a
  mailbox of `()`, a queue holding itself and a `ptr` or `cstr` in a generic. **R6's sentence is in no arm**: an
  arm on `spec-r.md` (sha256 `6c905ad57c68d63c...`) with a task that reaches `m.number()` would be needed;
  panel 254's fourth arm, 15 sessions, cost 3.370710 USD (`254-reports/llm-ergonomist-blind.md`), and this run's
  sessions cost 0.17 to 0.38 each, so 15 sessions would be about 3 to 4 USD, **an inference, unrun, not run here**.
- **Which arm.** `gc`, `gi` and `gr` all read at 5 of 5 on every cell; D2 and D3 cannot choose among them, and
  `gr`'s cell is easier by construction (critic 22; here its reasons were graded: 35 of 35 program sections name a type parameter, a `(mailbox T)` or `spawn`
  as the reason, by a pattern over each section, and the reasons of five sessions read by eye in part). The
  choice rests on the compiler-engineer's and the ffi-pragmatist's measurements, not on these readers.
- **Other readers.** One model, `claude-opus-5-5`, five per cell, one run; another model, a longer program or a
  reader with a compiler may read otherwise. The `w` task's own text names the trap, so t1 does not test whether a
  reader would forget a ring.

## The appendix: every answer whole

`256-reports/llm-ergonomist-blind-answers.tar.xz` (sha256 `2fe9b585161b3816cfb0f62aa7794bed7127a7cd1152090f644c294cb0a39ba5`)
holds, byte for byte, every session's `result.md` and `run.json` (46), the preflight's `run.json`, `run.log`,
`runner.out`, `key.txt`, `order.txt`, `registration-at-start.md`, `warden-at-start.md`, the scan's, `reads.py`'s and
`read_runs.py`'s outputs, `outcomes-by-letter.txt`, and the scoring session's extractors with `t3-mech.out`
(`score-tools/`). The transcripts stay in the area under `home/.claude/projects/`, uncommitted.

## The A/B verdict, reproduced whole

The verdict session's answer, as `tools/after.py` kept it in `verdict/result.md`, byte for byte:

````markdown
I could only read files, not write them, so `report.md` was not created. The whole report is below.

# report.md

## verdict

**Approve.**

Spec A says a handle crosses a thread "only through a mailbox, written `m.send(@v)`", and "anything else is a compile error". It does not say three things: whether `@` is allowed when sending a value that holds no handle, whether a generic function's body is checked once or at each call, and so what `m.send(v)` means when `v: T` and `T` may later be `db.Db`. Spec B settles all three. The `@` is written exactly where the value holds a handle or a thread. Generic bodies are checked at each call, against the types that call gives. A call with a type its send does not serve is a compile error at that call. Each send's meaning can now be read from the line and the signature around it, so a veto does not apply.

## experiment

### Under spec-A: `main.hero`

```
use db

# Counter worker: takes one i64 and answers it plus one.
function counter(inbox: (mailbox i64)) -> i64?
    n = inbox.receive(within_ms: 5000)?
    return ok(n + 1)

# Closer worker: takes one open database and answers whether it closed.
function closer(inbox: (mailbox db.Db)) -> bool?
    held @= inbox.receive(within_ms: 5000)?
    return ok(db.shut(@held))

# Sends one value into a mailbox and answers whether the send failed.
function deliver<T>(box: (mailbox T), @v: T) -> bool
    return box.send(@v).is_err()

function main()
    counter_box: (mailbox i64) = mailbox()
    closer_box: (mailbox db.Db) = mailbox()
    counter_thread = spawn(counter, counter_box).must()
    closer_thread = spawn(closer, closer_box).must()
    start @= 41
    if deliver(counter_box, @start)
        print("counter: send failed")
    conn @= db.open_memory()
    if deliver(closer_box, @conn)
        _ = db.shut(@conn)
    counted = counter_thread.wait()
    closed = closer_thread.wait()
    match counted
        .ok c => print("counter: ", c)
        .err e => print("counter: ", e.code)
    match closed
        .ok s => print("closer: ", s)
        .err e => print("closer: ", e.code)
```

**Does `heroes build main.hero` accept it?** Probably, but spec A does not settle it. Nothing in A says whether `counter_box.send(@v)` with `T = i64` is allowed ("empties it" has no stated meaning for an `i64`).

**If accepted, it writes:**

```
counter: 42
closer: true
```

**How it ends:** normally, with exit code 0. Both threads are waited for and the database is closed once, on the closer's thread, which holds it.

### Under spec-B: `main.hero`

Spec B does not let one generic `deliver` carry both sends. A body written `m.send(@v)` serves only types that hold a handle or a thread, and one written `m.send(v)` serves only types that hold neither. `i64` and `db.Db` fall on opposite sides, and the other call is a compile error at that call. What B does allow is two generic functions, one for each kind of value:

```
use db

# Counter worker: takes one i64 and answers it plus one.
function counter(inbox: (mailbox i64)) -> i64?
    n = inbox.receive(within_ms: 5000)?
    return ok(n + 1)

# Closer worker: takes one open database and answers whether it closed.
function closer(inbox: (mailbox db.Db)) -> bool?
    held @= inbox.receive(within_ms: 5000)?
    return ok(db.shut(@held))

# Sends one value holding no handle or thread; answers whether the send failed.
function deliver<T>(box: (mailbox T), v: T) -> bool
    return box.send(v).is_err()

# Sends one value holding a handle or thread, emptying it; answers whether the send failed.
function hand_over<T>(box: (mailbox T), @v: T) -> bool
    return box.send(@v).is_err()

function main()
    counter_box: (mailbox i64) = mailbox()
    closer_box: (mailbox db.Db) = mailbox()
    counter_thread = spawn(counter, counter_box).must()
    closer_thread = spawn(closer, closer_box).must()
    if deliver(counter_box, 41)
        print("counter: send failed")
    conn @= db.open_memory()
    if hand_over(closer_box, @conn)
        _ = db.shut(@conn)
    counted = counter_thread.wait()
    closed = closer_thread.wait()
    match counted
        .ok c => print("counter: ", c)
        .err e => print("counter: ", e.code)
    match closed
        .ok s => print("closer: ", s)
        .err e => print("closer: ", e.code)
```

**Does `heroes build main.hero` accept it?** Yes. Every line follows a stated rule.

**It writes:**

```
counter: 42
closer: true
```

**How it ends:** normally, with exit code 0. Both threads are waited for and the database is closed once.

### What differs

- A has one `deliver` taking `@v` and sending with `@`.
- B has `deliver` (`v`, plain send) for the counter and `hand_over` (`@v`, `@` send) for the database.
- In `main`, A needs the cell `start @= 41` so it can pass `@start`. B passes the literal `41`.

## choice_points

### Spec-A

1. **How the generic `deliver` sends.**
   - Chose: `box.send(@v)` with an `@v: T` parameter.
   - Other choice: `box.send(v)` with `v: T`. A does not say whether a generic body is judged once or at each call. If at each call, the `db.Db` call is a compile error (a win). If judged once, the build accepts a handle crossing as a plain copy. `main` keeps a live copy, so the program would stop with an error at run time. That could happen at `sqlite3_close` on a thread that does not hold the handle, or when `main` returns with a handle nobody consumed. Which one is unspecified, and this is the enemy case.
2. **`@` when sending an `i64`.**
   - Chose: allowed, since A forbids nothing here.
   - Other reading: refused. Then A also allows no single `deliver`, and the build refuses the program (a win, but discovered only by trying the build).
3. **Whether there is one function or two.**
   - Chose: one, because A appears to allow it.
   - Other: two, as under B. That would work too.

### Spec-B

1. **What to write in place of the one generic `deliver`.**
   - Chose: two generic functions, `deliver` and `hand_over`.
   - Other: one generic `deliver` with a plain send for the counter, and a direct `closer_box.send(@conn)` for the database. That is equally correct. A single `deliver` in either form is refused at the call it does not serve, which is a win.
2. **`@` in a non-generic send.** `closer_box.send(conn)` without `@` is refused, as is `counter_box.send(@n)` with it. Both are wins.

### Both specs

1. **What a worker does when nothing arrives in time.**
   - Chose: `receive(...)?`, so the worker answers an error.
   - Other: `.must()`, which stops with an error at run time on a timeout. That does not happen here.
2. **What `main` does if `spawn` fails.**
   - Chose: `.must()`.
   - Other: a `match`. That is equivalent here: the thread already started is then never waited for, and the program stops with an error either way.
3. **What `main` does if the database send fails.**
   - Chose: `db.shut` it in `main`.
   - Other: discard the result with `_ =`. The build accepts that. On a failed send, the program would stop with an error when `main` returns, holding a handle nobody consumed.
4. **Variable names.** Naming any binding `db` is refused, because the name is taken by `use db` (a win).
5. **How the closer binds the database it receives.**
   - Chose: a cell, `held @= ...`.
   - Other: `held = ...`. The build refuses that, because `@held` needs a cell (a win).
6. **Waiting for the threads.** Leaving out a `wait` makes the program stop with an error when `main` returns. Waiting twice on the same name is refused by the build. Neither choice was taken.

## argument

Under spec A, the only line the task depends on, a send inside a generic body, has no meaning that can be read from it. A does not say whether `@` is allowed on a non-handle, or whether generic bodies are checked per call. One plausible program might be accepted and then stop with an error when run, or be refused. Under spec B, every send's spelling follows from what its type holds. Each misuse is a compile error at the call that causes it. My B program needed no guessing. B costs some expressiveness: one generic `deliver` cannot carry both kinds of value. But it says so plainly, and the build refuses the mistake instead of accepting it.

## condition

I would change my verdict to **object** if either of these turned out to be true:

- A's compiler is shown to already judge generic sends at each call and to accept `@` on non-handles. Then A's single `deliver` is correct, and B has only taken expressiveness away.
- A model given B is shown to write the single `@v` deliver anyway, and to miss that the build refuses it.

I would change it to **veto** if B's "judged at each call" made error messages point into the generic's body rather than at the call, so that the error could not be read from the call line.

## context

Apart from the files in this directory (`brief.md`, `changes.diff`, `task.md`, `spec-A.md`), what reached my context was the harness's system prompt. It says I am an agent built on the Claude Agent SDK, gives the working directory path and the date, and lists one Read tool. I did not read `spec-B.md` directly; I rebuilt it from `spec-A.md` and `changes.diff`. No other rule, memory or document reached my context.
````
