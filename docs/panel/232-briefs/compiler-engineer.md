# Panel 232, the compiler-engineer's brief

Read `00-shared.md` first: the frame, the tree, your worktree
`.claude/worktrees/232-compiler-engineer/`, the rules every seat keeps, and
the critic's first pass it cites (`docs/panel/232-reports/critic-briefs.md`,
whose programs are in `.claude/worktrees/232-critic/critic-scratch/`: read
them, never run inside that worktree; copy what you rerun). Your report is
`docs/panel/232-reports/compiler-engineer.md`, written as you go. You may
build edited compilers in your worktree (about 40 s a generation; two after a
library change), and a route you would adopt is built and its cases run
before you recommend it (the panel skill, § 3c).

**Item 1, the order of evaluation.** Name, with file and line, where the
lowering orders what it evaluates: an operator's and a call's in
`selfhost/ir/flatten.hero`, a record's construction at `flatten.hero:538`
(F17; not `selfhost/ir/containers.hero`), a place's `place`, an `f`
literal's holes. Then the two orders a one-line sentence would hide (F4,
F26, F31): a write evaluates its place's indices, then the value, and
checks the place as it writes; an `@` argument's place is checked after the
arguments after it. For each: is it the lowering's choice or a consequence
of where the check is emitted, and what moving the check before the value
would cost, built and run, or whether the sentence should say the two
phases. And the built-ins that call a function value (F5): where `map`,
`filter`, `any`, `all` and `find` are written (`selfhost/library_source.hero`
for those written in Heroes) and whether their order and their stopping are
the library's code, which a sentence would freeze. Is there any position
where left to right as written is NOT what the compiler does? Search, and
say what you searched.

**And the labels' order.** A call, a record or a variant case whose labels
stand in another order than declared is refused `wrong_label` (F5). Find
where (`selfhost/check/labels.hero`) and whether it is a rule of its own or
a consequence of matching by position.

**Item 2, `<<` and `>>`.** design.md § 4.14 rules both and gives its reason
(F8): read it whole before anything else. Then: what `selfhost/emit` writes
for each (`selfhost/emit/operator.hero`), and **the route nobody listed**, an
abort on a `<<` that loses a set bit, keeping `1 << 63` and `3 << 62` (F28):
build it in your worktree, run the shapes beside it (a negative operand,
`-3 << 62`, a mask's top flag, a count from a call), and measure its cost in
the emitted C and at run time on a program, never by estimate. A route that
aborts on a sign change is refused by § 4.14 by name.

**Item 4, the stack.** Reproduce d435ca009's chain in your worktree and say
where `stack exhausted` comes from (`runtime/parts/stack.c`); then the
shapes beside it (F10): a chain of small frames 2,000 deep, one call whose
frame alone passes the stack, and a thread's smaller stack (`thread_deep`).
Is *a call chain deeper than the stack aborts* true of every one?

**Item 5, `exit`.** Confirm F7's edges, run, and say which a sentence of
§ 11 would misdescribe: `-1` refused as a literal though the grammar makes it
a minus on one; a constant, a `=` name, `255 + 1` aborting at run time; a
report naming `.must()`; a call through a function value naming
`<heroes library>:232:1`.

**Item 7, a diagnostic's code.** Count the codes the compiler can emit
(`selfhost/`'s `code: "..."` in diagnostics, not in a `fail`), against the 242
a golden pins (measurement 060); F16 found `not_indexable` and
`if_without_else` reachable and pinned by none. And what `heroes check
--json`'s `"schema"` number promises today (design.md § 4.17).

**Item 8, the suite's corpus.** `tests/harness/suite_compat.hero` reads
`examples/` at each release tag. F23 and F33 counted the others (`run/` 544
at `v0.5.0`, 248 of 622 changed in 30 days; the spec's code blocks; the
thesis suite's references; `selfhost/` at each tag): what checking each
would cost the net per release, and which change with the language they
test, which is what makes a corpus a bad promise's.

Every number from a command you ran, with the command; a negative claim
says what you searched for.
