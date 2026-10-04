---
kind: decision
area: none
milestone: none
filed: 2026-10-03
commit: f72905b76350d2c25d6715f279539743b406520d
github: none
---

# Panel 185's R5 landed: one predicate for where a path ends, and the seal where a value is read

2026-10-03 at 00:51 by the clock, lane flow4, in the same commit as panel
184's R4 (`2a03b5e6`), as R5 says it lands
(`docs/panel/185-a-macro-is-named-as-a-macro-an-arm-takes-a-statement-a-leaving-block-leaves-and-a-spaced-sign-has-two-readings.md`,
ratified 2026-10-02).

**What landed.** One predicate, the walk's `Outcome.leaves`, decides a
function's end (`missing_return`), a value arm (the join skips it) and a
value block (it owes no value), so the three cannot disagree. The checker
records where each body it judged leaving ends, `Checked.ends`, keyed by the
statement: a block's last, an arm's one. The lowering seals a value block, a
value arm and the end of a function with a result there with `unreachable`
where the IR would run on (`ir/emissions.seal`), and nowhere else, so a
function with no result keeps its `return` and clang still refuses a value
read from nothing wherever the checker did not say *leaves*. Lane flow's c1,
c2, c3 and c6, which R4 as prototyped checked clean and stopped at `build`
exit 2, build and run (`tests/golden/run/fixedbugs-174-a-value-arm-or-block-that-ends-its-path.hero`).

**Its `emission` run**, owed by the ruling, was made once in the lane on the
landing's compiler, read-only: 702 programs emit their blessed bytes, and the
2 that fail are the lane's two new `fixedbugs-174-*` run cases, which have
nothing blessed yet. So the seal moves no blessed emission; the two new
blessings, and the third for `fixedbugs-175-*`, are owed at the round's gate.

**What it cost, measured.** `Checked` grows 128 to 136 bytes (clang's record
layout of the emitted C); `Checker` stays 184, its four flags packed into the
padding of its last word. The seal added 512 bytes to each level of nested
`if`s in the lowering (`clang -fstack-usage`), and moving `flatten.statement`'s
mutation and `return` arms out of the recursion path left that frame 3,744
bytes under the base's: `build --emit-c` survives 149 nested `if`s where the
base did 141. The
decided ceilings: `check/walk.hero` stays under its 1870, `write_target`
moving to `check/access.hero` and `declared_labels` to `check/labels.hero`;
`ir/flatten.hero` reads 1150 of its 1150.
