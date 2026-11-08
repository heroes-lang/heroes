# Panel 237's blind seat and reader test

Designed 2026-11-08 by the coordinator, committed with the sitting before any paid session runs.
**Paid, and waiting for the author's yes**: the coordinator asked for at most 10 USD in all for
this sitting (the seat, the reader test and the spec-warden's `--refresh` requests together). No
session starts before that yes is recorded here, under § The runs.

## What it measures

Question 7: which of three readings of spec § 13's first fence makes a model bind a close that SQLite
can refuse so that a refused close is never counted as the connection given back in silence. The
three arms differ only in the fence's two `sqlite3` lines (`00-shared.md` gives their vendored
prices):

- **A**, as they stand: `acquires sqlite3_close` and `function sqlite3_close(db: Db consumes) -> i64`;
- **B**, `function sqlite3_close(db: Db consumes) -> i64 when 0`;
- **C**, `acquires sqlite3_close_v2` and `function sqlite3_close_v2(db: Db consumes) -> i64`.

And question 4 for the blind seat alone: whether `shadowed_header`'s message and note, read with no
other file, lead to the right repair.

## The blind seat (one session)

A folder of its own under the seat's worktree's `build/` (ignored by git, inside the repository's
root, as panel 235's was), holding `brief.md` (this folder's), the specification at `163912031`
unamended as `heroes-spec.md`, `variant-1.md` to `variant-3.md` (the arms in the order
`variants.tsv` records, drawn with Python's `random.Random(2373)` over `A B C`: C, A, B), and
`task-1.md` and `task-2.md`. Run from that folder with `/panel`'s command:
`claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in
this directory only. Write report.md here. Answer in English." --bare --restricted --safe-mode
--strict-mcp-config --tools Read,Write --allowedTools Read,Write --disallowedTools
Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage --model claude-opus-5-5
--max-budget-usd 1.00 --output-format json`, its output kept as `run.json` and `run.err`.

## The reader test (18 sessions)

Six sessions per arm, first turn only, `claude-opus-5-5`, each in a folder of its own holding
`heroes-spec.md` (the arm's specification: A unamended, B and C with the two lines changed),
`task.md` (`task-reader.md` here) and nothing else, run with the seat's flags and
`--max-budget-usd 0.45`. Each answer's last fenced block is the program.

**Grading, mechanical**, with lane `p237`'s compiler (`sources 0d7a6741345899d8`) and this Mac's
SQLite 3.54.0, each program written to a folder of its own and built with `heroes build`, its binary
run under `timeout 20`:

- `binding`: the close the program binds, read from its `extern` group: `when` (a close marked
  `when 0`), `v2` (`sqlite3_close_v2` bound), or `bare` (neither);
- `outcome`: `refused` (does not build), `loud` (builds, exits non-zero with the runtime's panic),
  `quiet` (builds and exits 0).

A program is **silent** when it binds `bare` and its outcome is `quiet`: a refused close counted as
the connection given back with nothing said, defect 2322's report.

## The decision rule, registered before any session

- An arm is **preferred on the reader** over A when its silent programs number at least two fewer
  than A's, of six.
- Between B and C, the arm with fewer silent programs; a tie goes to the cheaper on the real
  tokeniser (the spec-warden's count).
- No arm two below A: the reader test decides nothing, and the sitting rules on the other seats.
- A session that fails (no fenced block, the CLI's error) is counted as `refused` and named.

## The runs

**Dry run, 2026-11-08, no paid call**: `blind237.py prep` wrote the seat's folder and the 18 reader
folders under lane `p237`'s `build/b237/`; `diff` against the specification prints the one line of
B and the two of C, and `cmp` prints nothing for A. Four reference programs, planted as answers and
graded by `blind237.py grade`, read: a `bare` binding closing once, `bare/quiet` (it prints `close 5`
and exits 0, the connection still open: **silent**); a `bare` binding that tries to close again,
`bare/refused` (`error[handle_used_after_end]`, whose notes do not name `when 0`); fence B's binding
retrying, `when/quiet` (`close 5`, `finalize 0`, `close 0`); fence C's, `v2/quiet` (`close 0`,
`finalize 0`). The folders were then written again from scratch.

The paid sessions: unrun. The author's yes, the CLI's reported cost per session and the sum go here
when they exist.
