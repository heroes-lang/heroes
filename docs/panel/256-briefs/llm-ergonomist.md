# Panel 256, llm-ergonomist (the blind seat), prepared by the coordinator and run by the coordinator of M-rebel-library

Written 2026-11-22 from 21:45, **repaired from 22:20 after the completeness critic's first reading**
(`docs/panel/256-reports/critic-briefs.md`, findings 16, 19 to 27), **before any session is drawn or run**,
and **registered**: the arms, the
tasks, the rows each answer is graded by and the decision rules below are fixed here, and the report applies
them as written. The seat is not a subagent: each reading is one `claude -p` session run from a folder of its
own (`.claude/skills/panel/SKILL.md` § 2, as amended 2026-11-19 for defect 2589), started by the coordinator
of M-rebel-library on the author's word of 2026-11-22 (up to 20 USD), **never by this sitting's coordinator
or a seat**. The exact command and its folder are `blind/RUN.md`, written once the area stands. **The area is built
after the synthesis, and the run is one run** *(critic 24)*: its arms are the ones below, with the texts the
synthesis names (the spec-warden's repairs where the synthesis takes them), and, where the resolution adopts
a rule no arm states, the resolution's own arm added as one more cell; every text hashed in
`blind/registration.md` with the coordinator's predictions before any session is drawn, so the author's 20
USD covers the proposal's arms and the resolution's together.

## The area and the runner (panel 254's, its caps, paths and cells changed)

- **The area**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p256-scratch/256-llm-ergonomist/`,
  inside the repository's root, **the top of an empty git repository of its own** (`git init`, a
  `.gitignore` of `*`, local `user.name` `reader`, `user.email` `reader@invalid`, no commit), with `home/`
  and `tmp/`; `python3 -I tools/check_area.py` exit 0 before the run.
- **The runner**: `blind/run.sh`, panel 254's (`docs/panel/254-briefs/blind/run.sh`) with these changed:
  its header; `CAP=1.00` per graded session and `CAP_VERDICT=1.50`; `CEILING=19.50` with `MARGIN=0.10`, so
  the run stops before a session whose cap and margin would take the spend past 19.50 USD, under the
  author's 20; `WARDEN` naming this sitting's spec-warden report; **a second gate, the registration**: the
  runner refuses to start unless `blind/registration.md`'s sha256 equals the one `RUN.md` names, and copies
  it beside `run.log`; `check_folders.py` counting 46 folders. Every session: `env -i`, `HOME`, `TMPDIR`
  and `CLAUDE_CODE_TMPDIR` inside the area, `< /dev/null`, `--bare --restricted --safe-mode
  --strict-mcp-config --model claude-opus-5-5`, tools `Read,Write` alone, `--disallowedTools` the skill's
  list, `--max-budget-usd` the cap, `--output-format json`; one session at a time; the key read from the
  main checkout's `.env`, its length alone printed.
- **Before any spend**: the spec-warden's registered predictions (the runner's first gate, its report copied
  and hashed as it starts); the registration's hash; `tools/check_folders.py`; `tools/check_area.py`; **a
  free preflight** with a key the API refuses, which must cost 0, leave no file naming its session under
  `/private/tmp/claude-501/`, and read clean by `tools/scan.py`. The coordinator of this sitting runs the
  preflight alone (`blind/preflight.sh`, the runner cut before the key is read), and records it in RUN.md.
- **After the run**: `tools/scan.py` over every transcript; `tools/reads.py` (every path a session's own
  tool calls named outside its folder); a session counts only where `subtype` is `success`, `is_error`
  false and `terminal_reason` `completed`; every session's CLI-reported cost kept, summed by
  `tools/spent.py`.
- **The expected spend, an inference from panel 254's measured costs, not a measurement**: its 26 sessions
  cost 6.445551 USD, the 25 graded 0.188959 to 0.347129 each and the verdict 0.368434 by subtraction
  (`docs/panel/254-reports/llm-ergonomist-blind.md`, *critic 8*); 46 sessions at those rates, 8.7 to 16.0
  USD, and up to 15 more for a resolution's cell. t1 and t3 ask for whole programs beside a page of a package, so
  more is plausible; the ceiling, not the estimate, binds.

## The arms as they run, settled after the critic's second reading (2026-11-22, before 23:00)

**Settled after the seats and the critic's second reading** (`docs/panel/256-reports/critic-reports.md`):
`gc`, `gi` and `gr` run on the spec-warden's repaired texts, each of which states the half of the send's
`@` rule the frozen text lacks (its T0), with the critic's repair of *"these rules"* (its second reading,
finding 6) made by `docs/panel/256-reports/resolution/make_resolution.py`; **`gi`'s arm is the resolution's
§ 11 sentence, so the resolution is a cell of this run and no arm of its own is added**. Each arm's rule is
the one the table below names, as the spec-warden's predictions assume.

| arm | file (`256-reports/resolution/`) | sha256 (`arms.sha256`) |
|---|---|---|
| `now` | `256-briefs/arms/spec-now.md` | `15804f3fb52a62c2...` |
| `gc` | `arm-gc.md` (the warden's `spec-wc.md`, *"these rules"* repaired) | `99e080b00a996ad9...` |
| `gi` | `arm-gi.md` (the resolution's § 11 sentence on the frozen spec) | `2d14c084ed997ad3...` |
| `gr` | `arm-gr.md` (the warden's `spec-wr.md`, unchanged) | `32b0504b44f98852...` |

The table below is the arms as registered at 21:45, kept as the record of the proposal:

## The arms as proposed (`docs/panel/256-briefs/arms/`, and t1's two task files)

| arm | what it is | its spec | sha256 |
|---|---|---|---|
| `now` | the frozen spec | `spec-now.md` | `15804f3fb52a62c2...` |
| `gc` | Q2's proposal: judged at each call, a letter's value always sent `m.send(@v)` | `spec-gc.md` | `dda78a9c4100948e...` |
| `gi` | judged at each call, the `@` included | `spec-gi.md` | `fc53aaf54f51bc95...` |
| `gr` | refused in the body | `spec-gr.md` | `73d148550a923e86...` |
| `w` | t1 with the waker (`poll.add_waker`, `poll.wake`) | `spec-now.md` | task `task-t1-w.md` |
| `m` | t1 with the mailbox in the set (`poll.add_mailbox`) | `spec-now.md` | task `task-t1-m.md` |

t1's two task files are made from one template by `blind/make_t1.py` and differ only in the paragraph that
names how the waiting thread is woken (`diff task-t1-w.md task-t1-m.md`: lines 19-22 against 19-21). The
labels are drawn by `tools/make_sessions.py` from `bdfhjkpqsvxyz`, no letter that names an arm, written to
`key.txt` beside `s/`, never inside a session's folder.

## The tasks and the sessions

| task | what it asks | arms | sessions |
|---|---|---|---|
| t1 | a thread that waits only in `poll.wait` and is woken by four workers' results (`blind/task-t1-*.md`) | `w`, `m` | 5 + 5 |
| t2 | six programs, a generic that sends or spawns its letter's value: accepted? every line? how it ends? (`task-t2.md`) | `now`, `gc`, `gi`, `gr` | 20 |
| t3 | one generic `deliver` for an `i64` and a `db.Db`, where the language allows it (`task-t3.md`) | `gc`, `gi`, `gr` | 15 |
| verdict | `spec-A` = `now`, `spec-B` = `gi`'s arm, the resolution's text (settled after the critic's second reading; registered first as `gc`), `changes.diff`, task t3; panel 254's `brief-verdict.md`; never graded | both | 1 |

Each graded session's folder holds `brief.md` (panel 254's session brief, unchanged), `task.md` and
`heroes-spec.md`, nothing else. **46 sessions** in `order.txt`: five rounds over the nine cells, odd rounds
forward and even reversed, the verdict last.

## The rows each answer is graded by (no compiler reads `gc`, `gi`, `gr`, `w` or `m`: the reader's rows)

**t1, both arms.** **Right** iff all of: (1) `main`'s thread waits only in `poll.wait`, its limit 30 s or
more, never in a mailbox's `receive` with a limit above 0; (2) **no result can be left unseen while the
wait sleeps**: under `w`, each worker calls `poll.wake` after its send (after each send, or after its last
send provided every result is taken at each wake), never only before it; under `m`, the mailbox is added to
the set before the first wait; (3) at each wake the loop takes every result that has arrived, a
`receive(within_ms: 0)` repeated until it fails; (4) it prints `done 100` after the 100th; (5) every worker
waited for once, the set and the listener closed; (6) the prediction: accepted, 100 `got` lines, 25 from
each worker, then `done 100`, a normal end; (7) nothing the spec refuses (an unused binding, a missing `@`,
a handle crossing outside a mailbox's `m.send(@v)`, the set reached from a thread that does not hold it).
**Every session not right is classed**: *a wait that can sleep with a result unseen* (a ring forgotten, put
before its send, or one receive per ring where rings are told together), refused by the text, a thread not
waited for, numbers wrong, other.

**t2, by each arm's rows**, `now`'s **two ways** *(critic 16)*: by the frozen compiler, measured, and by the
frozen text as written, which refuses programs 3 and 5 and accepts 2 as `gi`'s rows do and leaves program 4
undecided (a session right on 4 under the text either way, its choice quoted). The compiler's rows: each program built by the
coordinator's stage 2 at the frozen commit (`sources 031aaabb358218fd`) beside `db.hero` and run once under
`timeout -s KILL 20`, 2026-11-22 21:35 (`blind/t2-programs/`, the programs and `coordinator-run.txt`):

| program | `now` (the compiler, measured) | `gc` | `gi` | `gr` |
|---|---|---|---|---|
| 1 `relay<T>` sends `v`, at `i64` | accepted; `false`, `7`; normal | refused (a letter's value sent without `@`) | accepted; `false`, `7`; normal | refused |
| 2 `hand<T>` sends `@v`, at `db.Db` | refused, `marker_mismatch` at `m.send(@v)` | accepted; `false`, `true`; normal | accepted; `false`, `true`; normal | refused |
| 3 `relay<T>` sends `v`, at `db.Db` | accepted; `false`, `true`; an abort at `main`'s second `db.shut`, a C handle a `consumes` call ended | refused | refused (at the call) | refused |
| 4 `hand<T>` sends `@v`, at `i64` | refused, `marker_mismatch` | accepted; `false`, `7`; normal | refused (at the call) | refused |
| 5 `start<S>` spawns `setup`, at `db.Db` | accepted; `true`; an abort at `main`'s `db.shut`, the same message | refused (at the call) | refused (at the call) | refused |
| 6 `start<S>` spawns `setup`, at `i64` | accepted; `42`; normal | accepted; `42`; normal | accepted; `42`; normal | refused |
| 7 `outer<A>` hands `v` to `inner<T>`, which sends it without `@`, at `db.Db` (added after the critic's second reading, finding 5) | accepted; `false`, `true`; normal | refused (`inner` sends a letter's value without `@`) | refused at `main`'s call, the chain `outer` -> `inner` named | refused |

A session is **right on a program** iff its acceptance, every line and its ending all match the row; **a
refusal is right only where the reason it gives names the rule the row's arm refuses by** (`gc`: a letter's
value sent without `@`, or a handle into `spawn`; `gi`: the kind the call gives against the send's `@`, or a
handle into `spawn`; `gr`: a letter's value sent, spawned or answered, or a `(mailbox T)` naming a letter;
`now` by the compiler: `marker_mismatch`'s *"`send` does not change this argument"*), the reason quoted in
the report *(critic 22)*.

**t3, by each arm's words**, widened to every program the arm's text allows *(critic 20)*. **Right** iff all
of: (1) the program's shape is one the arm allows: under `gc`, every send of a value whose type names a
letter is written `m.send(@v)` on a cell (the parameter `@`, or a local cell made from it), one `deliver`
serving both; under `gi`, either two functions, each used with its kind (two generics, or one generic and a
plain send), or one generic that sends no letter's value itself and takes the send as a parameter, the
session saying why one generic cannot send both kinds; under `gr`, sends written without a generic, or a
generic that takes the send as a parameter and writes no `(mailbox T)` naming its letter, the session saying
why; (2) the `db.Db` crosses
only through a mailbox written `m.send(@v)` and is closed once, by the closer; (3) every thread waited for
once; (4) each worker receives with a limit of 5000 ms; (5) the prediction: accepted, `counter: 42`,
`closer: true`, a normal end; (6) nothing the arm's text refuses.

## The decision rules, registered

- **D1, the wake (t1)**: `E1 = right(m) - right(w)`; and `lost(a)`, the sessions of arm `a` classed *a
  wait that can sleep with a result unseen*. **The waker admits, in practice, a mistake the build accepts
  iff `lost(w) >= 1`; the mailbox in the set closes it iff `lost(m) = 0`**; the mailbox in the set is read
  as the form readers get right more often iff `E1 >= 2`.
- **D2, the text predicts its rule (t2)**: for each of `gc`, `gi`, `gr`, **the text suffices for a program
  iff `right >= 4` of 5**, and **the arm's text is read as written iff it suffices for at least six of the
  seven programs**. For `now`, both counts are reported and decide nothing: right by the compiler, and right
  by the frozen text as written; the gap between them on programs 2, 3 and 5 measures defect 2695 as a
  reader meets it.
- **A cell with fewer than five counted sessions** *(critic 23)*: the bar is `right >= n - 1` of its `n`
  counted, with `n >= 3`; a cell under three counted decides nothing, and the report says so.
- **Chance at five** *(critic 23)*: a one-sided Fisher exact test gives p = 0.22 for 5 of 5 against 3 of 5,
  0.26 for 4 against 2, 0.08 for 5 against 2. So `E1 >= 2` is reported as a direction with its p beside it,
  never as a finding; **D1's robustness reading rests on `lost`, an existence**: one session of `w` whose
  program the build accepts and whose wait can sleep with a result unseen is the mistake the waker admits.
- **D3, the text lets a reader write it (t3)**: per arm, **iff `right >= 4` of 5**; every session not right
  is classed: wrong shape for the arm, the handle crossing otherwise than by `m.send(@v)`, closed twice or
  never, a thread not waited for, prediction wrong, other.
- **Voiding**: a session the scan reads with a leak (`prompt-alone=NO`, `git-status=LEAK`,
  `claude-md-or-memory=YES`) is voided and not counted; so is a session whose transcript the scan does not
  find, a session served by any model but `claude-opus-5-5` (`run.json`'s `modelUsage`), and a session one
  of whose tool calls named a path outside its folder (`tools/reads.py`); a session not a success by the
  three fields is recorded and not counted. A cell with fewer than five counted sessions is read on what it
  has, and the report says so.
- **What the run does to the resolution** (the critic's second reading, finding 5), registered: **a row of
  the resolution that two or more of five `gi` readers read wrong reopens that row's text** (panel 254's
  R9 condition), and the row comes back to the author alone; **if D2 fails for `gi` where it holds for `gr`**,
  the synthesis's Q2 is reopened toward `gr` (the spec-warden's condition); **if `lost(w) = 0` and `right(w)
  >= right(m)`**, the waker's case against it rests on the stress runs alone, and the record says so. Every
  other outcome leaves the resolution as ratified.
- **t1's row (7) grades the text** (the spec-warden's report, § 5): a worker's `m.send(@r)` of a record that
  holds no handle is refused by the compiler and by no sentence of `spec-now.md`, so it is not counted
  against a t1 session.
- **The predictions registered before the run**, scored on these counts as worded: the spec-warden's, in
  its report under `## Predictions registered before the blind run`, copied and hashed by the runner as it
  starts; the coordinator's, in `blind/registration.md`, its sha256 in `RUN.md`; and any resolution arm the
  synthesis adds, registered the same way before any session of it is drawn.

## Reference programs *(critic 26)*

Before the registration, the coordinator writes, for t1 under each arm and t3 under each arm, one program
that the row calls right, against stubs in Heroes for the functions no package has (`poll.add_waker`,
`poll.wake`, `poll.add_mailbox`), and checks the language half with the frozen stage 2 (`heroes check`),
in `blind/reference/`; a row no reference program meets is repaired before the run.

## What reaches the report

The draw (`key.txt`); every session's cost, exit, the three fields and its grade, its `context` answer
quoted; the scan's and `reads.py`'s tallies; the spend; the cells; the rules applied; each answer whole in
an archive. Written by whoever scores the run at `docs/panel/256-reports/llm-ergonomist-blind.md`.
