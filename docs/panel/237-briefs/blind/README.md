# Panel 237's blind seat and reader test

Designed 2026-11-08 by the coordinator and **redesigned the same day on the completeness critic's
first pass** (`docs/panel/237-reports/critic-briefs.md`, findings 12 to 22), before any paid session
ran; both designs are committed, the first at `e6e692868`. **Paid, and authorized**: the author's
yes of 2026-11-08 at 11:44, meant as *set a cap of up to another 25 dollars*, covers this sitting's
paid runs in all (the seat, the reader test and the spec-warden's `--refresh` requests).

## What it measures

Question 7: which reading of spec § 13's first fence makes a model bind a close that can refuse so
that a refused close is never counted as the resource given back in silence, **and whether that
lesson carries to a library with no `_v2`**. Four arms, differing only in the fence's `sqlite3` lines
(`00-shared.md` gives their vendored prices):

- **A**, as they stand: `acquires sqlite3_close` and `function sqlite3_close(db: Db consumes) -> i64`;
- **B**, the close bound `-> i64 when 0`;
- **C**, `acquires sqlite3_close_v2` and `function sqlite3_close_v2(db: Db consumes) -> i64`;
- **D**, `acquires sqlite3_close | sqlite3_close_v2`, the close `when 0`, and a `sqlite3_close_v2`
  line: the form the specification's prose teaches at line 459 (the critic, finding 12).

And question 4 for the blind seat alone: whether `shadowed_header`'s message and note, read with no
other file, lead to the right repair.

## The blind seat (one session)

A folder of its own, `build/b237/seat/` in lane `p237` (ignored by git, inside the repository's
root, as panel 235's was), holding `brief.md`, the specification at `163912031` **with the fence's two
`sqlite3` lines replaced by one marker line** (the critic, finding 16: otherwise the seat reads which
variant is today's), `variant-1.md` to `variant-4.md` (the arms in the order `variants.tsv` records,
drawn with Python's `random.Random(2374)` over `A B C D`: D, A, B, C), and `task-1.md` (the SQLite
task), `task-2.md` (the refusal) and `task-3.md` (the store task). `claude-opus-5-5`,
`--max-budget-usd 2.00`.

## The reader test (144 sessions)

Two tasks: **R1**, `task-reader-1.md`, SQLite's close with a statement still prepared; **R2**,
`task-reader-2.md`, a store library of one header (`store.h` here) whose close refuses while a cursor
is open and which has no `_v2`. Each task under each arm, **six sessions for each of three models**,
`claude-sonnet-5-5`, `claude-opus-5-5` and `claude-haiku-5-5` (the thesis instrument registers four
rows, `tests/thesis/held-out/prediction.md`; the fourth, Fable 5.1, is its ceiling and is not run
here), so **18 sessions per task and arm**, first turn only, each in a folder of its own holding
`heroes-spec.md` (the arm's specification: A unamended, B, C and D with the lines changed) and
`task.md`, and nothing else. Each session caps at `--max-budget-usd 0.45`; the runner stops starting
sessions once the reported sum passes **20.00 USD**.

**Every session runs with git blind and the key loaded**: its environment holds `PATH`, `HOME`,
`LANG`, `GIT_CEILING_DIRECTORIES` set to the session folder's parent, so `git` finds no repository
there, and `ANTHROPIC_API_KEY`, which `--bare` needs (the critic, finding 17), read from the trunk's
`.env` by the runner and never printed; the runner refuses to start without it. The flags are
`/panel`'s: `--bare --restricted --safe-mode --strict-mcp-config`, `--tools Read,Write` and the same
in `--allowedTools`, the rest disallowed, `--output-format json`.

## Grading, on behaviour (the critic, finding 18)

Each answer's last fenced block is the program, written to a folder of its own and built with lane
`p237`'s compiler (`sources 0d7a6741345899d8`) under `timeout 120`; its binary runs under `timeout
20`, its output through `head -c 65536`. R1's binary runs with `DYLD_INSERT_LIBRARIES` set after
`timeout` to `shim.dylib`, built from the critic's `shim.c` (kept beside the runner), which counts
the SQLite connections still open at exit into `connections.txt`; R2's folder holds `store.h`,
whose destructor writes the stores still open into `stores.txt`. Each session is one of:

- `failed`: the CLI reported an error (`run.json`'s `is_error`): run once more, then named;
- `refused`: no fenced block, or the program does not build;
- `loud`: it builds and exits non-zero;
- `silent`: it exits 0 with a connection or a store still open, a refused close counted as the
  resource given back with nothing said (defect 2322's report);
- `correct`: it exits 0 with nothing open.

Beside the class, the close each program binds is read for the record: `when` (a close marked
`when 0`), `v2` (`sqlite3_close_v2` bound and called), or `bare`.

## The decision rule, registered before any session

Per task, the 18 sessions of an arm pooled over the three models:

- **An arm is preferred over A** on a task when its `silent` count is lower than A's with a one-sided
  Fisher exact p below 0.05, **and** its `correct` count is not lower than A's with a one-sided
  Fisher exact p below 0.05 (an arm whose readers fail to build does not win by having nothing run).
- **An arm wins the reader test** when it is preferred over A on both tasks; among several, the one
  with fewer `silent` over both tasks, then more `correct`, then the cheaper on the real tokeniser
  (the spec-warden's count, or the vendored count if the real one is missing, said so).
- An arm preferred on R1 and not on R2 is reported as **a lesson that does not carry**, and does not
  win.
- No arm preferred on both: the reader test decides nothing beyond what each task shows, and the
  sitting rules on the other seats.
- The p values are computed by the runner's `grade` from the hypergeometric distribution, exactly.

**Task 2's criterion for the seat**: the coordinator applies the seat's repair to a copy of task 2's
folder; it is **right** when the program then builds and prints `125000` with no file named
`stdint.h` left in the folder, and wrong otherwise.

## The runs

**Dry run of the first design, 2026-11-08, no paid call**: four reference programs graded by the
first runner read `bare/quiet` for a bare binding closing once (`close 5`, exit 0, the connection
open), `bare/refused` for a bare binding closing again (`error[handle_used_after_end]`, whose notes do
not name `when 0`), `when/quiet` for fence B's (`close 5`, `finalize 0`, `close 0`) and `v2/quiet`
for fence C's (`close 0`, `finalize 0`). The critic then found three shapes that grader misread,
which grading on behaviour answers.

The paid sessions: unrun. The CLI's reported cost per session and the sum go here when they exist.
