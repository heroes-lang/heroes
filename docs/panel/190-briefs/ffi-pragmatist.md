# Panel 190, the ffi-pragmatist's brief

Read `00-shared.md` first; it binds you. You judge the C boundary (your
charter, `.claude/agents/ffi-pragmatist.md`): the emitted C a route writes,
compiled and run on the platforms. Repaired after the critic's first pass
(the text it read is `ffi-pragmatist-before-the-critic.md`): its item 2
rested on a false premise, a handle released at an exit; the exit sweep
releases counted types only (`selfhost/check/counted.hero:58` to `:91`), and
handles and leases are checked once at the process's exit, from `main`
(`selfhost/emit/decls.hero:232` and `:248`).

## What to measure, in `<scratchpad>/190-ffi-pragmatist/`

1. **The emitted C of an exit, by hand first**: write the C the shared exit
   implies for one small function (two strings, an early return, an `@`
   parameter), compile it under the compiler's flag list
   (`selfhost/cli/flags.hero`'s `flags()`) on Apple clang, Debian clang 22.1.8
   and 18 in the `heroes-linux-arm64` image, and the box's clang 23.1.1,
   under ASan, UBSan and LeakSanitizer where each exists; then, once the
   compiler-engineer's routes exist (the coordinator sends you their paths),
   each route's own C for Q1's cases, on the same platforms.
2. **What an exit's order means for memory C still reads**: a `cstr` lent
   from a `str` to a C function that keeps the pointer (a retained lend,
   `.claude/rules/c-boundary.md`), on a function with early returns: is any
   `str` released earlier or later under a route than under the trunk, on any
   path, and does it matter to a library that reads the pointer after the
   call? Panel 106's carried warning applies: ASan does not see a C library
   reading freed memory, so say what instrument would, and run it.
3. **The 800 shape at `-O2` and under `--sanitize`**, today and on each
   route, on each platform you reach: a finish or not within a bound you
   name, never a duration beside the lanes.

Report as the shared brief says.
