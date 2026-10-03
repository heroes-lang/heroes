# Panel 184's R7 landed: `fmt` holds on the thread, the depth of types is repaired apart, and nothing pins a boundary

2026-10-03 at 12:28 by the clock, lane depth, beside R5 (its own entry,
`docs/records/log/2026-10-03-1130-panel-184-r5-landed-every-command-runs-on-a-thread-whose-stack-the-compiler-chooses.md`)
and defect 170, at `6c95f44a`. Panel 184's R7
(`docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md`,
ratified 2026-10-01) asked three things to land with R5 and R6.

**`fmt`, measured under the thread** (the critic's C8, `printparens.render`
dying on a 600-term chain). Before the thread `fmt` aborted every chain at
1,000 and an `if`, a `match` and a record literal nested 300 deep, on the
lane's compiler built from the seed of `02e507bc`. Under R5 it formats every
one of the 25 shapes at 1,000 and 2,000 on this Mac and on Linux arm64, every
one run at 5,000 on both, and every one run at 10,000 on this Mac but the
record literal (the four block nests are not written past 2,000, where one is
8 MB of indentation; `scratchpad/lane-depth/r5mac/`,
`scratchpad/lane-depth/linux/out/`).

**The depth of types, filed apart**: defect 170, repaired in the same lane at
`6c95f44a`. Clang converts a struct the first time a function needs it and
recurses into what it holds by value, so 3,000 variants nested by value
stopped `build` at exit 2 under *internal error* and clang's crash report;
past 32 of a unit's structs the emitter now hands clang the chain from its
bottom, and 3,000 variants build and run on this Mac and on Linux arm64. A
clang that still dies (its debug information, between 3,000 and 5,000 on
this Mac; Debian clang 22.1.8 killed at 10,000 with nothing written) is told
in this compiler's words at exit 2, with the program's deepest chain and the
fix of holding one level through an array. Exit 2 and not a diagnostic at
exit 1: the program is correct, R6 refuses none for its depth, and a clang
failure the author answers at exit 1 is the C boundary's class of five
members, which only a sitting widens.

**No golden, test or spec sentence pins a panic's function name or a depth
one level from the boundary** (the critic's C6), read on the lane's tree: no
`.expected` under `tests/golden/` holds *stack exhausted*; the four rows of
`tests/harness/suite_surface.hero` that assert it name no function and run
programs ten million frames deep; spec § 9 says *Recursion too deep aborts*
and no number. The lane's own cases sit at 2,000, where the first abort is
between 3,000 and 3,500 on both platforms measured, and at 32 and 100, the
emitter's threshold, a constant of this compiler and no boundary of a stack.
