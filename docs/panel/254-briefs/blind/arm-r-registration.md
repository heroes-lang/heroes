# Panel 254, the blind seat's fourth arm (R9): the resolution's text, registered before any session of it runs

Written 2026-11-22 by the session that scores panel 254's blind seat, started by the coordinator of
M-rebel-library. **Registered before any session of this arm is drawn or run**, which is what R9 asks.
Said plainly: these predictions are written AFTER the first 26 sessions (arms `now`, `m`, `m0`) ran and were
scored, so they are made knowing those results; they are registered before the arm they predict.

## The arm

`spec-r.md`: the frozen spec (`89f155330`'s `spec/heroes-spec.md`, sha256 `ffd687d6dd6b6b54...`) with
`docs/panel/254-reports/resolution/spec-r.diff` applied, made two ways that give the same bytes: `patch`
on the frozen spec, and `resolution/make_resolution.py` on the spec-warden's `spec-w2.md` (sha256
`0c06f838c0841643...`); both read sha256 `c21c162d2612592f278ce80f40070e1716f2801b0191ffc7b414ac94c77dbb4b`,
the hash R1 names. Its label is drawn from `cfghjkvwxz` (the first draw's pool without `q`, `y`, `p`).

## The area and the runner

`.claude/worktrees/p254-scratch/254-llm-ergonomist-4/`, made as the first area was: the top of an empty git
repository of its own (`.gitignore` `*`, local user `reader` <`reader@invalid`>, no commit), `home/`,
`tmp/`. Its tools are the first area's, unchanged (`brief-session.md`, the three tasks, `scan.py`,
`reads.py`, `after.py`, `spent.py`, `check_area.py`), with three adapted by asserted edits:
`make_sessions.py` (one arm, the three cells t1, t2, t3, no verdict session), `check_folders.py` (15
folders) and `run.sh`: its gate reads THIS file under the heading below and copies it to
`registration-at-start.md`, hashing it in `run.log`; **each session capped at 0.60 USD** (the first run's
dearest session cost 0.347129, `t2-p-5`); **the arm bounded at 5.00 USD**, the runner stopping before a
session whose cap and a 0.10 margin would pass it (the author's 20 USD for this seat; 6.445551 spent by the
first run; the total never past 11.45). Every other line of the runner as it ran: the free preflight with
a refused key, `env -i`, `HOME`, `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the area, `< /dev/null`,
`--bare --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5`, `Read,Write` alone, the
look under `/private/tmp/claude-<uid>/` after each session, `scan.py` at the end; `reads.py` after it.

## The sessions

15: t1, t2 and t3 on arm `r`, five each; five rounds over the three cells, odd rounds forward and even
reversed. No verdict session (R9 asks an arm, and the first run's verdict judged `m`).

## How each answer is graded, registered

By the rows of `docs/panel/254-briefs/llm-ergonomist.md` as registered, read against arm `r`'s own text,
with these changes and no others:

- **t1**: the seven reader's rows; no compiler reads `r`, so no mechanical grade.
- **t3**: its seven rows.
- **t2**: the proposal's rows, but **program 3's row is `got queued`, ending normally** (R9: under R4 a
  receive after a stop still gives what is queued), and **program 5's ending, an abort at `free(b)`,
  counts right only where the session's stated reason is that `b`'s life ended or that the send emptied
  it** (the row's own words); an abort at `free(b)` whose only stated reason is that `main`'s thread does
  not hold the handle (R3's sentence) is graded not right and counted apart. Programs 1, 2 and 4 keep their
  rows: under `r` the first `receive` of program 1 finds an open, empty mailbox and fails `timeout`, the
  send after the close fails `closed`, the two queued values are received and the last receive fails
  `closed`; program 2's thread drains 5 and 7 and fails `closed`; program 4's thread is never waited for.
- **Voiding** as registered for the first run: a leak by `scan.py`, a transcript the scan does not find,
  another model, a path outside the folder by `reads.py`; a session not a success by the three fields is
  recorded and not counted.

## R9's condition, applied

A row of R2 is read wrong by a session when that session's answer is wrong on a program in the part the
row decides; **two or more of five sessions reopen that row's text**. The rows each program reads:

| program | the R2 rows it reads |
|---|---|
| t2 p1 | S6 (the send after the close), S7 (`timeout` on an empty open mailbox, the oldest value, `closed` when closed and empty), S8 (values still received after the close; a second close does nothing) |
| t2 p2 | S1 (`spawn`'s `?`), S3 (`wait` gives the answer), S7 (`closed` after the drain), S9 (a mailbox crosses as itself) |
| t2 p3 | S7 and S12 (the drain: a stop over a queued value) |
| t2 p4 | S1, S3 (a thread nobody waits for aborts at `main`'s end, saying how many, naming no mailbox) |
| t2 p5 | S9 (a mailbox of handles crosses `spawn` as itself), S11 (`m.send(@b)` empties `b`), S6 (emptied only on a success) |
| t1 | S1, S3, S9 (records cross whole), S13 (the four operations after a dot) |
| t3 | S1, S3, S4, S7, S8, S9, S13, and R5 (a request carrying a reply mailbox of another type is not refused) |

## Predictions registered before the fourth arm runs

Each a count out of five counted sessions per cell; a range beside each point, the point what scores.

- R-1, t1 `r`, the seven rows: **5 of 5** (range 4 to 5).
- R-2, t2 `r`, per program: **p1 5, p2 5, p3 (`got queued`) 4, p4 5, p5 4** (ranges p1 4-5, p2 4-5, p3 3-5,
  p4 4-5, p5 3-5). p3's likely miss: `stopped ... and from then on` read without its *"on an empty
  mailbox"*; p5's: the holder reason given alone.
- R-3, t3 `r`: **5 of 5** (range 4 to 5).
- R-4, R9's condition: **no row of R2 read wrong by two or more of five readers**; the row most at risk is
  S7 with S12 (p3).
- R-5, the question the first run's verdict named (what copying a `(thread T)` means, in an array or a
  `for` variable, which `r`'s copied-ADDRESS sentence leaves out as `m`'s did): **named as a choice point
  in 10 of the 10 t1 and t3 sessions** (range 8 to 10), and no session's program depends on an answer to it.
- R-6, t2 p5 sessions whose only stated reason is the holder rule: **1** (range 0 to 2).
- R-7, voided: **0**.
- R-8, the arm's spend: **3.40 USD** for 15 sessions (range 2.80 to 4.30).
- R-9, the registered decision rules read on `r` (descriptive, beside the first run's): D1 on `r`,
  `E1 = right(r, t1) - right(now, t1)` = **0**, so D1 does not hold; D2 on `r` against `m0`: **holds** (d >= 3
  on p1, p2 and p4); D3 on `r`: **holds**.
