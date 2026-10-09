# Panel 198, llm-ergonomist (blind seat): the coordinator's prediction before the readings

Written by the coordinator at 10:47 on 2026-10-09 (`date`), before any of the six
sessions starts, so the readings score a prediction and not a story. The
folders are `<scratchpad>/readings-b/u<task>-<label>`, outside the repository
and outside any git tree (the author's exception of 2026-10-09 to the rule that
no file is written outside the repository's root, asked because the blind seat
must not see the project's `CLAUDE.md` or its git status), each holding
`brief.md`, `spec.md` (`56def9b4`'s, SHA-256 `75407a13724c3758`),
`main.hero` (t1 `faf2b7ec88ace6a7`, t2 `bb55e8ac2de3488b`), `machine.txt` and
`build-output.txt`. The labels' mapping, never in a folder: **M** is today's
message (the frozen compiler's); **K** is route (A)'s note, which K shows where
it still refuses (on both tasks' machines route K builds with no message, so
the brief's rule gives K the (A) note); **R** is route (G)'s note,
`--allow-word`. Each `build-output.txt` is the compiler-engineer's prototype
output, copied unchanged from `.claude/worktrees/scratch-b15/198-compiler-engineer/blind/run-{M,A,G}-t{1,2}/`.

**What I expect.**

1. **M**: both readers take `link` (`link "SDL3"` and `link "curl"`), the one way
   out the message names; on t1 at least one also writes a `--include` or a
   `CPATH` they need not (the headers sit on the default paths), and none
   edits a `.pc` file or writes a wrapper.
2. **K** (the (A) note): both follow the note as written, `link` with the
   named `--include`/`--library`, and the t1 reader keeps the note's warning
   about the loader in its `files` (an `ldconfig` or an `LD_LIBRARY_PATH`).
3. **R** (the (G) note): both take `--allow-word`, the shorter way the note
   offers, so both pass a refused word on: the list weakened by the reader,
   which the critic's first pass and the compiler-engineer's objection to (G)
   predicted.
4. Every program builds and runs where its world exists; `context` names only
   the folder and the harness's environment information.

**What would falsify it**: an M or K reader who edits a `.pc` or wraps
`pkg-config`; an R reader who takes `link` over the allowance; a `context`
naming a project rule, which voids that reading.
