# Panel 236, compiler-engineer's brief

Read `00-shared.md` and `proposal.md` first, then the record they name, and the critic's first pass,
`236-reports/critic-briefs.md`, on which these briefs were repaired. Your worktree is
`.claude/worktrees/236-compiler-engineer/`, your scratch `.claude/worktrees/236-compiler-engineer-scratch/`,
your report `docs/panel/236-reports/compiler-engineer.md` in your worktree, written as you go.

**Your task: build the routes and measure them.** In your worktree, under `selfhost/emit/` and
wherever else a route needs (cite every file and its line count before and after):

1. **The set** (question 1). Decide by measurement which group records the convention reaches, and
   build that rule: today's off-frame set, named fields past `LARGE`, or another; for each, the
   emission corpus's programs it moves (`./heroes run tests/harness/main.hero -- ./heroes-next
   emission` on the route, every moved file named and read), and what the union's double count
   (`probe/union/unioncount.hero`) does to it.
2. **Between Heroes functions** (question 2): on the base and on the route, at `-O0` and `-O2`, the
   run (`timeout 60`, output bounded) and `-Wframe-larger-than=65536` on the unit (each `stack frame`
   warning, function and bytes) for `grpnoext`, `grpgen`, `grpfix`, `grpopt`, `grpwrap`, `grpret`,
   and a program whose result is discarded and one taking the record by value.
3. **Two routes for function values** (question 3), both built far enough to run `extval`,
   `cbparam`, `cbresult` and `cbnamed`: **the proposal as written** (a value takes the out-parameter;
   an `extern` stored as a value gets an adapter; a Heroes function handed to C gets a C-convention
   adapter beside `emit/callback_thunk.hero`'s thunk) and **R7b** (values and thunks keep C's
   convention, only a call by name takes `hero_out`); in each, the per-thunk
   `_Static_assert(__builtin_types_compatible_p(...))` of panel 233's veto (a), and the C typedef C
   sees, byte for byte against the base's `--emit-c`. Say which one you would adopt and what the other
   costs, measured.
4. **The room** (question 5): the regions each route's prologue takes for one 128 MiB value
   (`grpopt` takes 9 on the base), and whether a temporary copied straight into a slot can be written
   in the slot's region; the peak, and what fits under the 1 GiB.
5. **Part (a)** (question 6): construction in place for group records (`emit/construct.hero:137`
   reaches own records alone), and clang `-O2` on `gn12`, `ga12` and one 1 MiB chain (`gen2.sh 16`,
   the array halves; you alone run it, base then route, one at a time, about 7 GB each), measured as
   `00-shared.md` measured the 64 KiB ones; against panel 233's form of bound, *each unit under 8
   billion instructions and 100 MB*.
6. **What else the route must hold**: `--sanitize` at both levels on every shape above; `wholes`,
   `determinism`, `descriptors`, `emit` and `lldb` (`./heroes run tests/harness/main.hero --
   ./heroes-next <suite>`) on the route; the emission corpus's programs with no such value byte for
   byte the base's but for `hero_program_sources`; panel 215's R1, the padding zero where C reads a
   record that crossed through `hero_out` or a pointer.
7. **The cost**: lines added per module and directory against each file's ceiling (`./heroes run
   tests/harness/main.hero -- ./heroes-next layout`), the compiler's own tests on the route, and the
   instructions of `./heroes-next build selfhost/main.hero` on the base and on the route.

Give **one falsifiable prediction** about the landing (a number with the instrument that reads it),
your verdict (adopt, adopt with conditions, or veto, a veto naming the soundness failure and the
program that shows it), the route you recommend between question 3's two, and the cases the landing
owes, named as `run/fixedbugs-2286-<slug>`. A route that needs a new diagnostic or spec text is
reported as such, for the full panel.
