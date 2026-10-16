# Panel 217, the compiler-engineer's brief

Read `docs/panel/217-briefs/00-shared.md` first, whole.

## Your seat's question

**What a read-only lend costs the compiler, and build it.** Prototype F1 in your copy, far enough
that a reader's program using it builds and runs: a Heroes parameter declared `c: Counter lent` (the
spelling is the spec-warden's to settle; build this one unless your report says why another), the
caller's place passed by address, the body allowed to read it and to hand it on to another `lent`
parameter or to a C parameter declared `c: Counter lent` with no `@` (the header's `const struct
counter *`), and refused anywhere it would be written, `@`-lent, returned or copied. Say what each
refusal is told, and which existing checks it touches: `selfhost/resolve/written_nowhere.hero` (an
`@` parameter nothing writes), `selfhost/check/lock_copies.hero` (R2's positions),
`selfhost/check/lending.hero` and `leasing.hero` (where a lend may stand), the parser's `Param`
(`selfhost/parse/`), the formatter (`selfhost/print/fmt.hero`) and every tool that re-prints a
program (`.claude/rules/diagnostics-and-goldens.md` § A new surface form, the walk a new form owes).
Then: lines by directory, whether any `DECIDED` row in `tests/harness/suite_layout.hero` moves, a
cold `--emit-c` of the compiler before and after (machine still, `real` against `user + sys`), and
the `check`, `run` and `emission` forms on your copy.

**Build F1's key**: task C's program written with the form (`docs/panel/215-reports/spec-warden-work/q3/kit/key/c.hero` is F0's), which must build and print `6`, `4`, `5` under your prototype, and refuse the aliasing the shared brief names (`f(a: @p, b: p)` with `b` read-only) or say why not. Put it at `<scratchpad>/217-compiler-engineer/key-f1.hero`.

**Leave the compiler that builds it at a path the coordinator can use**, `<scratchpad>/217-compiler-engineer/heroes-f1`, and say so in your report as soon as it builds: the
blind readers' F1 arm is judged with it.

F2 and F3: say in a paragraph each what they would cost and what they leave, from the code; build
them only if F1 turns out unsound.

## What to return

`verdict` per form, `section` (design.md's; §4.8 and §4.10 are the nearest, read them),
`cost` (lines, time), `prediction` with its instrument, `condition`.
