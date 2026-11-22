# Panel 254, llm-ergonomist (the blind seat), run by the coordinator

Written 2026-11-22 before any session is drawn or run, **repaired the same day after the completeness
critic's first reading** (`docs/panel/254-reports/critic-briefs.md`, findings 16, 18, 20, 22 to 26), and
**registered**: the arms, the tasks, the rows
each answer is graded by and the decision rules below are fixed here, and the report applies them as
written. The seat is not a subagent: each reading is one `claude -p` session run by the coordinator from
a folder of its own (`.claude/skills/panel/SKILL.md` § 2 as amended on `rebel-round` at `cb2ad6d7d`,
defect 2589's repairs).

## The area and the runner

- **The area**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p254-scratch/254-llm-ergonomist/`,
  inside the repository's root, **the top of an empty git repository of its own** (`git init`, a
  `.gitignore` of `*`, local `user.name` `reader`, `user.email` `reader@invalid`, no commit), with `home/`
  and `tmp/`; `python3 -I tools/check_area.py` exit 0 before 01:43.
- **The runner**: `run.sh`, panel 253's (from `docs/panel/253-reports/evidence.tar.xz`,
  `253-llm-ergonomist/run.sh`) with four things changed by asserted edits (its header, `CAP=3.00`,
  `CAP_VERDICT=3.00`, `CEILING=20.00`, and `WARDEN` naming this sitting's spec-warden report); `bash -n`
  exit 0. Every session: `env -i`, `HOME`, `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the area, `< /dev/null`,
  `--bare --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, tools `Read,Write` only,
  `--max-budget-usd 3` (the coordinator's word of 2026-11-22), `--output-format json`. One session at a
  time; **the run stops before a session whose cap would take the spend past 20 USD**, the author's bound
  for the sitting's paid runs in all (2026-11-22), with a margin of 0.10. The key is read from the main
  checkout's `.env`; its length alone is printed.
- **Before any spend**: the spec-warden's registered predictions (the runner's gate, copied and hashed as
  it starts); `tools/check_folders.py` (26 folders, each holding its registered files at their hashes);
  `tools/check_area.py`; **a free preflight** with a key the API refuses, which must cost 0, leave no file
  naming its session under `/private/tmp/claude-501/`, and read clean by `tools/scan.py` (the prompt
  alone, a git status naming `reader` and no commit, no `CLAUDE.md`, no memory).
- **After the run**: `tools/scan.py` over every transcript, and `tools/reads.py` (written for this sitting,
  the critic's finding 25), which counts every path a session's own tool calls named outside its folder; a session counts only where `subtype` is
  `success`, `is_error` false and `terminal_reason` `completed`; **every session's CLI-reported cost is
  kept in the report**, summed by `tools/spent.py`.
- **The expected spend, an inference from two sittings' measured costs, not a measurement**: panel 251's
  graded sessions cost 0.1322 to 0.2112 USD and its verdict 0.3772 (`docs/panel/251-reports/
  llm-ergonomist.md:21`); panel 253's 31 sessions 5.335998 USD in all (`253-reports/llm-ergonomist.md:23`).
  26 sessions at panel 251's rates: 3.4 to 5.7 USD (`26 x 0.1322`; `25 x 0.2112 + 0.3772`); t1 and t3 ask
  for whole programs, so more is plausible and unmeasured; the ceiling, not the estimate, binds.

## The arms (`docs/panel/254-briefs/arms/`)

| arm | what it is | sha256 (`arms.sha256`) |
|---|---|---|
| `now` | the frozen spec, `89f155330`'s `spec/heroes-spec.md` | `ffd687d6dd6b6b54...` |
| `m` | the proposal, `spec-m.diff` applied (repaired after the critic's first reading) | `b008c3078855228f...` |
| `m0` | panel 251's Q3 text, spelling M, ported (`spec-m0.diff`) | `1dc1446d4fc99a03...` |

The labels are drawn by `tools/make_sessions.py` from `cfghjkpqvwxyz`, letters that name no arm (`m`
removed from the pool after the critic's finding 24), written to `key.txt`
beside `s/`, never inside a session's folder.

## The tasks and the sessions (`tools/task-t1.md`, `task-t2.md`, `task-t3.md`; copied to `blind/`)

| task | what it asks | arms | sessions |
|---|---|---|---|
| t1 | panel 251's task 2 (four documents counted on threads, each thread giving back a name and two counts), **with one sentence added**: *"A document's name and text may hold any character, a newline and a `|` among them; these four are the ones this run holds, and the program must stay right for any others."* | `now`, `m` | 5 + 5 |
| t2 | five programs at a mailbox's edges, each: accepted? every line? how it ends? | `m`, `m0` | 5 + 5 |
| t3 | a keeper thread holding a map, four workers asking it while they run, a reply to each request | `m` | 5 |
| verdict | `spec-A` = `now`, `spec-B` = `m`, `changes.diff`, task t1 under each; panel 251's `brief-verdict.md`; never graded | both | 1 |

Each graded session's folder holds `brief.md` (panel 253's session brief, unchanged), `task.md` and
`heroes-spec.md`, nothing else. **26 sessions** in `order.txt`, five rounds over the five cells, odd
rounds forward and even reversed, the verdict last.

## The rows each answer is graded by

**t1, `now`: built and run.** The answer's `main.hero` is the longest fenced block under that name in its
report (or its result where the report is missing), built by the frozen commit's stage 3
(`.claude/worktrees/lane-p254/heroes-next`, `sources 6e891e681c14bca0`) and run under `timeout -s KILL 20`
five times on the task's own documents; then **four substitutions**, each one edit asserted to match once
in a copy of the answer, each built and run once:

| substitution | the edit | the line for `a` the program must write |
|---|---|---|
| S1 | `a`'s text becomes `the cat\|dog sat on the mat` | `a: 4 words, 4 distinct` |
| S2 | `a`'s text becomes `the cat` `\n` `sat on the mat` (a real newline, written `\n` in the literal) | `a: 3 words, 3 distinct` |
| S3 | `a`'s text becomes the Heroes literal `"the c,a:t; d\tog=x \"q\" \\ \u{1e}r\u{1f}s # mat mat"`, pasted as written | `a: 8 words, 7 distinct` |
| S4 | `a`'s name becomes `a\|b,c: d` `\n` `e` | `a\|b,c: d` newline `e: 4 words, 4 distinct` |

The expected lines are a reference implementation's (python, `str.split(" ")` and the leave-out list,
run 2026-11-22 before 01:43; the four lines of the task's own run are `a: 4 words, 4 distinct`, `b: 3 words, 3
distinct`, `c: 4 words, 3 distinct`, `d: 1 words, 1 distinct`). **Right** iff every build is accepted and
every run exits 0 writing exactly the expected lines. Where a substitution cannot be made by one
asserted edit (the text not written as one literal), the grader makes it by hand and says so.

**t1, both arms, also by the reader's rows** (the critic's finding 22: a compiler grades `now` and no
compiler can grade `m`, so a rubric for `m` alone tilts the difference toward `m` before any session
runs). **Every t1 answer, of either arm, is graded by the seven rows below**, reading the program against
its own arm's text; and `now`'s answers are graded mechanically as well, as above. **D1 reads the rows'
count for both arms**; the mechanical count of `now` is reported beside it and decides nothing. Row (1)
for `now`: the text a thread is handed may be one joined text only where the joining cannot be confused
by any character the task allows (an escape, a length prefix, a separator the task excludes, of which it
excludes none); row (2) the same for the answer. **Right** iff all of: (1) the document and the leave-out list reach `spawn` as values, never a joined text the thread
splits; (2) the thread's answer comes back as a record or values, never a joined text `main` splits;
(3) all four started before any wait; (4) each thread waited for once; (5) a failure the start can
answer is answered (`must`, `match`, `?`) where the arm's text gives one; (6) the prediction writes the four lines above and a normal end; (7)
nothing the arm's text refuses (an unused binding, a name it takes, a missing `@`).

**t2, both arms, by the proposal's rows** (the answers the proposal's text gives; `m0` is graded by the
same rows, as panel 251 graded `now` by its proposal's row):

| program | accepted | lines, in order | how it ends |
|---|---|---|---|
| 1 | yes | `first: timeout`, `send: closed`, `got 1`, `got 2`, `last: closed`, `done` | normally |
| 2 | yes | `total 12` | normally |
| 3 | yes | `receive: stopped` (the program now calls `hero_net_catch_stop()` first, the critic's finding 18) | normally |
| 4 | yes | `main ends` | an abort when `main` returns, saying one thread was never waited for (and naming no mailbox) |
| 5 | yes | `sent`, `freed 1` | an abort at `free(b)`, saying a handle whose life ended (emptied by the send) reached `free` |

Program 5 sends `b` inside `if args().len() < 100` and is run with no arguments (the task says so), so
the send happens and the checker, which refuses a read *"only where the life ended on every path to the
read"*, cannot (the critic's finding 16, its `p254/t2p5/` probes): today's runtime, on the same shape
with a `consumes` call in place of the send (`lane-p254/build/p254probe/`, run 2026-11-22), prints
*"panic: a dead C handle reached the argument `p` of `free`: a call marked `consumes` or `transfers` ended
this handle's life and emptied the binding ..."*, exit 134, which is the row's ending in today's words.

A session is **right on a program** iff its acceptance, every line and its ending all match the row.

**t3, `m`: by the arm's words.** **Right** iff all of: (1) the keeper alone holds the map; (2) a request
carries the way its reply comes back (a reply mailbox inside it, or one per worker given at `spawn`);
(3) the keeper's loop ends on the program's own act (a close, or a stop request) and gives the counts
back; (4) every thread started is waited for once; (5) no wait the program's own order cannot end; (6)
the prediction writes `red: 402`, `green: 400`, `blue: 398`, `largest: 402` (the reference's, run
2026-11-22 before 01:43) and a normal end; (7) nothing the arm's text refuses.

## The decision rules, registered

- **D1, the record crossing (t1)**: `E1 = right(m) - right(now)`. **The record crossing serves the
  thesis by the registered count iff `E1 >= 3`** (the effect R3 named where it lives).
- **D2, the semantics sentences (t2)**: for each program, `d = right(m) - right(m0)`. **The proposal's
  sentences are read as needed iff `d >= 3` on at least three of the five programs**; **its text suffices
  for a program iff `right(m) >= 4`**.
- **D3, the gateway shape (t3)**: **the text lets a reader write it iff `right(m) >= 4`**; every session
  not right is classed: a wait that cannot end, a thread not waited for, a keeper that never ends,
  numbers wrong, refused by the text, other.
- **Voiding**: a session the scan reads with a leak (`prompt-alone=NO`, `git-status=LEAK`,
  `claude-md-or-memory=YES`) is voided and not counted; so is **a session whose transcript the scan does
  not find** (*"a scan that read nothing being no clean reading"*, SKILL.md § 2), **a session served by
  any model but `claude-opus-5-5`** (`run.json`'s `modelUsage`), and **a session one of whose tool calls
  named a path outside its folder** (`tools/reads.py`); a session not a success by the three fields is
  recorded and not counted. A cell with fewer than five counted sessions is read on what it has, and the
  rule says so.
- **The spec-warden's predictions** in `warden-at-start.md` are scored on these counts as worded.

## What reaches the report

The draw (`key.txt`); every session's cost, exit, the three fields and its grade, its `context` answer
quoted; the scan's tally; the spend; the cells; the rules applied; and each answer whole in an appendix.
The coordinator writes it at `docs/panel/254-reports/llm-ergonomist.md`.
