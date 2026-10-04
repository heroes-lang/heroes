# Panel 191, the compiler-engineer's brief

Read `00-shared.md` beside this file first, whole; it binds you. It was
repaired after the critic's first pass; the text the critic read is
`compiler-engineer-before-the-critic.md`.

**Your questions are Q2 and Q3, and Q1 where a route is yours to build.**

1. **Route (a), built far enough to run, in the form you judge most robust**:
   (a′), the XML handed to the linker; (a″), the compiler writing the fixed
   `.res` itself; or (a1), a `.res` made by `llvm-rc`. Price the other two
   against it.
   - **Measure the linker first** (Q-b): `clang -###` on the box with and
     without `-fuse-ld=lld`. Run the route under each linker you can reach.
   - Carry it into `selfhost/cli/flags.hero`'s `link_flags()`, or wherever a
     Windows link is made, and into the compiler's own Windows build line
     (`seed/README.md`).
   - Count the lines against each touched file's room in `layout`'s unit.
2. **Route (b)**: build it far enough to compare, or price it over the 40
   lines of the shared brief's Windows view. Say which you did, and price its
   argument door in each form (Q-h).
3. **Routes (c), (d), (e) and (g)**, priced.
   - For (d): where the start-up check would live (the generated `main`, or
     `hero_args_set`), and the message's stream, exit code and ASCII text,
     legible under any console code page.
   - For (g): say on a run whether the UCRT's `.UTF8` locale reads a narrow
     name as UTF-8 at all, if you can reach one.
4. **Q-d**: the links `heroes` does not write (`--emit-c`, the seed's line,
   the CI's seed lines), and what each route means for them.
5. **Q3**: the cases that would pin each row, in which golden form each
   lives, which run on Windows only, and **each shown red at `7f4c0cc5`**
   (Q1-a). Also the net's own tests that a route moves.
6. **What moves elsewhere**: `tests/emission/`, the compiler's own tests and
   the seed, on Linux and macOS. Expected nothing: run it, do not assume it.

**The box** is free from 14:40, shared with the ffi-pragmatist: at most one
heavy build of yours there at a time. **When you have a route built**,
export it for the ffi-pragmatist to `<scratchpad>/191-shared/<route>/`:
- the diff against a pristine `7f4c0cc5` extract;
- the compiler built from it;
- its sha256.

Say so in your report. Never use another seat's copy.
