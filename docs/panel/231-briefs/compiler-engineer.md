# Panel 231, the compiler-engineer's brief

Read `00-shared.md` beside this file first. You judge the ceiling (design.md § 1.1, § 1.7, Part 5):
what the generator, its oracles and its reducer cost, where they live, and whether any of it can be
made to crash or to lie. You hold a veto.

**Where you work**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/231-compiler-engineer`,
detached at `bd0c4477b`, its `heroes-next` built (`sources 5e0ca4c634086a7c`). Prototype there, never
in the trunk. Your report: `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/231-reports/compiler-engineer.md`,
written as you go.

**What you are asked, each answered with the command that settles it:**

1. **The value as it is built, and who computes it.** The generator is a Heroes program compiled by
   the compiler it tests. If the generator computes `a + b` with Heroes' own `+`, a miscompilation of
   that `+` writes the same wrong number into the program's output and into its `.expected`, and
   oracle 2 is blind to it. How large is that correlation in practice (the generator's arithmetic
   runs in the generator's binary, the program's in the program's, through different emitted C), and
   what breaks it: computing in another width, through `hero_` C helpers, by the three
   configurations' disagreement, by building the generator with the base compiler of a round? Say
   what you recommend and why, with a program that shows the risk or shows it absent.
2. **The declared arm**: the generator must know, before computing, whether an operation aborts:
   overflow at every width, division and `%` by zero and `i64` min by -1, a shift outside 0 to 63, an
   index or slice out of range, a slice splitting a character, `.must()` on an error, recursion too
   deep. Which can be decided before the operation without performing it, and which cannot (recursion
   depth: the stack the program is given); what the `.expected` says for each (`!panic: <message>`,
   its exact text taken from where in `runtime/` or `selfhost/emit/`).
3. **The core**: build in your worktree a generator far enough that the numbers are real: the clean
   arm over `i64` and `bool` expressions, `if`, `match` on a small variant, a `for` over `range`, a
   record, a function call, an array index; each program with its `.expected`; and judge a few
   hundred with your `heroes-next` through `build` and the binary run, oracles 1 and 2 at `-O0` and
   `-O2`. Report how many programs per second it makes and judges (an indication), and **any program
   that fails an oracle**, which is a finding of the sitting, reproduced and kept in your scratch.
4. **Where it lives and what it costs**: `tests/harness/` against `selfhost/mutate/` (CLAUDE.md § 10
   and § 4's trigger; `heroes mutate` is a verb, the harness is not), the modules and their lines
   against the 300-line ceiling, what it reuses (`mutate/child.hero`'s bounded child, `workers.hero`'s
   threads, `cli/process.hero`), and whether a module of the harness can `use` a module of `selfhost/`
   at all (measure it).
5. **Depth**: the dial's shapes (056 § 2.3 lists the clang limits measured by defects 539, 549, 219,
   468, 522), what depth the generator should reach in the net and in the hunt, and how oracle 1 tells
   the compiler falling over from clang dying on the program's C.
6. **The reducer** (task 934): from `selfhost/mutate/sites.hero`'s primitives, or a reducer over the
   generator's own tree (shrink the tree and regenerate, which keeps every candidate valid and its
   `.expected` known): which is sounder and cheaper, measured on one failing program if question 3
   found one, else on a planted one.
7. **The net** (P7): the fixed sample's size against the full net's wall clock (the `run` form is the
   critical path, journal 071 and `.claude/rules/verification.md` § The optimistic chain), the suite's
   name, and whether the witnesses go into `tests/golden/run/` as ordinary cases or into a form of
   their own.
8. **Task 2214**: whether coverage of the compiler (clang's `-fprofile-instr-generate` over the
   emitted C of `selfhost/`, or another route) is buildable here, what one run over `tests/golden/check/`
   costs, and whether the same instrument should steer the generator toward code no case reaches
   (coverage-guided policies). Measure only what is cheap; say what the rest would cost.

**Your output**, per proposal line P1 to P8: `verdict` (approve | object | veto) with the design.md
section you stand on; `implementation_cost` (lines per module, with file citations);
`needed_for_self_hosting` (yes | no); `argument`; `prediction` (falsifiable, checkable at a named
milestone or step); `condition` (what would change your verdict); every number with its command.
