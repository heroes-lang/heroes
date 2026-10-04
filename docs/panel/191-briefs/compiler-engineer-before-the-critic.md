# Panel 191, the compiler-engineer's brief

Read `00-shared.md` beside this file first, whole; it binds you.

**Your questions are Q2 and Q3, and Q1 where a route is yours to build.**

1. **Route (a), the manifest, built far enough to be run.**
   - Produce its resource the way the compiler would on every Windows link
     (`llvm-rc` or another tool on the box; say which exists there and on the
     CI's Windows runner, `.github/workflows/ci.yml`).
   - Carry it into `selfhost/cli/flags.hero`'s `link_flags()`, or wherever a
     Windows link is made, and into the compiler's own Windows build line
     (`seed/README.md`).
   - Count the lines, against each touched file's room in `layout`'s unit.
2. **Route (b), the wide calls.** Build it far enough to compare, or price it
   door by door from the shared brief's table, refined: which of those lines
   compile on Windows at all. Say which you did.
3. **Routes (c), (d) and (e)**, priced. For (d), where the start-up check
   would live (the generated `main`, or the runtime's `hero_args_set`), and
   the message a program prints where the manifest is not honoured.
4. **Q3.** The cases that would pin each row of 238, in which golden form
   each lives, and which run on Windows only. The net's own tests that a
   route moves.
5. **What moves elsewhere**: `tests/emission/`, the compiler's own tests and
   the seed, on Linux and macOS (expected: nothing; run it, do not assume
   it).

**The box** is the ffi-pragmatist's main instrument and yours too once the
coordinator frees it. Coordinate through your reports: the coordinator carries
one seat's built route to the other through a neutral folder,
`<scratchpad>/191-shared/`, never your copy. When you have a route built,
export it there:
- the route's diff against a pristine `7f4c0cc5` extract;
- the compiler built from it;
- its sha256.

Then say so in your report.
