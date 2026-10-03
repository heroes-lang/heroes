# Panel 184's R4 landed: a statement after a jump is refused, and three statements end a path

2026-10-03 at 00:50 by the clock, lane flow4, defect 174. Panel 184's R4
(`docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md`,
ratified 2026-10-01, *R4 lands as it stands* after the task-2 reading)
landed whole at `2a03b5e6`, with panel 185's R5, the seal it lands with
(its own entry, beside this one). Neither half stood at `07ccb72a`, measured
before the first edit: `return 1` over `print(2)` ran at exit 0, and a `-> i64`
ending on `exit(code:)`, `assert false` or a `while true` was told
`missing_return`.

**What landed.** A statement after `return`, `break` or `continue`, in its
block, is `unreachable_statement`, a thesis rule, told once per block at the
first line that never runs, naming the jump and its line; a statement after
anything else that leaves stays legal, as the ratified route (2d) says.
`exit(code:)` called by its name, `assert false` and a `while true` with no
`break` of its own end a path, read by their syntax: `assert OFF`, a `break`
under `if false`, `n.exit()` and a record's field named `exit` end nothing
(`tests/golden/check/fixedbugs-174-a-path-ends-only-by-its-syntax.hero`). The
checker's `join.Leaves` gains `ends` beside `jumps`, so no message calls an
`exit` a jump; `missing_return`'s first note names the three ends.

**Where it is written.** Spec § 8, R4's own sentence, and design.md §4.17's
list of what a mistake costs (R8), in the lane's spec commit: +117 legacy and
+121 cl100k vendored with panel 185's R4 sentence beside it, +143 on
`claude-opus-5` (9164 to 9307), one `--refresh`.

**What it cost the compiler, measured**: the first build put the new arms in
`check/walk.statement`, whose frame grew 1,680 bytes, and the `if` statements
`check` survives nested fell from 206 to 195; with the arms moved out of the
recursion path, `check` reads 231 (`if`), 184 (`match`), 151 (value `if`), and
`build --emit-c` 149, 125, 140, against the base's 141, 119, 125.

**Owed at the round's gate**: the full net, the census of `check` (panel
185's engineer predicted R4 changes exit for 5 tracked files, all under
`docs/panel/184-briefs/`), the seed, the compiler's own tests, and panel
184's prediction for R4, *silent `over-indent` mutants fall from 68 to 49 of
2,000*, the coordinator's instrument run.
