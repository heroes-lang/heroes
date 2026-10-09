# Panel 202, the shared brief: two groups that bind one C name with different types (550, 453)

Written by the coordinator on 2026-10-09 from 22:53 (`date`), on the tree
frozen at `46b80b82` (worktree `lane-panel-202`: batch 17's round, panels 200
and 201 landed, its gate running beside it). Convened on the author's yes of
about 22:47 (the question widget: *both sittings now, 6 USD between them*),
under the author's instruction of about 19:35, *every defect closed*. Every
fact names its command or file; a number marked **carried** is a report's,
not re-run by the coordinator, a question for the seat.

**Lane: full**: what `check`, `build`, `test` and `--emit-c` say of a program
is a diagnostic class (CLAUDE.md § 4). The compiler-engineer, the
ffi-pragmatist, the spec-warden, the historian, the blind seat run by the
coordinator as fresh `claude -p` sessions outside the repository (the
author's exception of 2026-10-09; 6 USD shared with panel 203), the critic
before the seats and after them.

## The question

Two modules of one program bind `a.h` and `b.h`, each defining a `static
inline twice` of another type (the case is
`.claude/worktrees/scratch-b15/200-critic2/fp/`, and `fp2/` with the `use`
lines swapped; read only). Measured by panel 200's critic (carried): `check`
and `build` exit 0 and the program prints 6 and 8 (`build` compiles module by
module); `heroes test` compiles the whole program as one unit
(`selfhost/cli/assemble.hero:99`, `site.is_tests`) and clang refuses it.
**Defect 538's repair** (batch 17, `selfhost/cli/headers_together.hero`, 65
lines by `wc -l`) now tells `test` *these cannot be compiled in one unit*,
naming both headers, order-free. **Defect 550**: when the conflicting
definition sits in a file another group's header includes, clang's note names
that file, which no group names, so one order is still told the old false
message (the reproducer `.claude/worktrees/scratch-b15/b17-emit/inc/`, read
only). **Defect 453** (panel 200 R1): `--emit-c` should compile the
whole-program rendering as its one unit, so the file written is what clang
read, and it waits on this sitting, since it would then refuse the same
program.

**What should `check`, `build`, `test` and `--emit-c` say of a program whose
groups bind one C name with different types?** Routes to weigh, a list to
widen: (a) `check` refuses it, the checker asking clang of each group's
declarations (a new class; how does it know the names two headers share?);
(b) `build` compiles the whole program as one unit too, so all four agree;
(c) `test` and `--emit-c` compile per module as `build` does; (d) the true
message in `test` and `--emit-c` only, with the include graph for 550.
Measure each route's cost, which correct programs it refuses, and what each
says at which exit code (`.claude/rules/cli-surface.md:41`: *exit 1 the input
has diagnostics*). `.claude/rules/c-boundary.md` § A clang failure that the
author's own extern caused (`:47` on) holds the six classes a clang failure
may be told as exit 1.

## The rules every seat works under

As panel 200's shared brief states them (`docs/panel/200-briefs/00-shared.md`,
§ The rules every seat works under), with your folder
`.claude/worktrees/scratch-b15/202-<seat>/`, your copy rsynced from
`.claude/worktrees/lane-panel-202/`, and **two more**: the round's committed
seed stamps ABI 29 and its runtime 30 (defect 470), so build your compiler in
three stages, the seed against `541d9595`'s runtime
(`.claude/worktrees/scratch-b15/b17-rt29/runtime`, read only), then
`selfhost` with `HEROES_RUNTIME` pointing there, then `selfhost` again against
the copy's runtime; and lane b18-close (`lane-b18-close`) and panel 203
(`lane-panel-203`) work beside you: never touch them. **Time box**: report
within 60 minutes of starting, the unmeasured said plainly.

## Corrections and additions from the critic's first pass, binding

Read by 23:06 (`date`): `docs/panel/202-reports/completeness-critic-pass1.md`,
every one from a command it ran, applied before any seat starts and binding
over the text above where they disagree. Read it whole: its repairs 1 to 13
are panel 202's, 14 to 19 panel 203's, 20 both; its unlisted routes and
questions are part of your brief. In short:
- **The question widens** from *groups that bind one C name* to **headers two
  groups name that cannot share one unit** (repair 3's table, the critic's
  cases in `.claude/worktrees/scratch-b15/critic-202-203/p202/`, read only):
  `clash` (one C symbol declared `long twice(long)` and `double
  twice(double)`, defined by a third module) **prints a wrong value at exit
  0**; `onedef` and `extdef` (non-static definitions) make `build` exit 2,
  *duplicate symbol*; `macro` gets a second false message, its verdict
  depending on `use` order; `nobind` conflicts on a name no group binds. Each
  is filed as a defect by the coordinator after batch 17's gate; this sitting
  rules on all of them with 550 and 453.
- `check` asks clang nothing today (repair 4): route (a) would be its first
  clang question. Whether defect 538's message is a seventh class landed
  without a sitting is a question for this sitting (repair 5). Name `heroes
  run`. 550's reproducer as a program is `p202/inc1` and `inc2`.
- Of panel 200's rules, these no longer bind: lane b17-fix and the 21:00
  time box; the Windows box is shared with lane b18-close.
