# Panel 190, the ffi-pragmatist's brief

Read `00-shared.md` first; it binds you. You judge the C boundary (your
charter, `.claude/agents/ffi-pragmatist.md`): the emitted C a route writes,
compiled and run on the platforms.

## What to measure, in `<scratchpad>/190-ffi-pragmatist/`

1. **The emitted C of an exit, by hand first**: write the C the proposal
   implies for one small function (two strings, an early return, an `@`
   parameter), compile it under the compiler's flag list
   (`selfhost/cli/flags.hero`'s `flags()`) on Apple clang, Debian clang 22.1.8
   and 18 in the `heroes-linux-arm64` image, and the box's clang 23.1.1, under
   ASan, UBSan and LeakSanitizer where each exists; then, once the
   compiler-engineer's route exists (the coordinator sends you its path), the
   route's own C for Q1's cases, on the same platforms.
2. **The handles at an exit**: a function holding a C handle that `acquires`
   (`.claude/rules/c-boundary.md`; the handle set's leak gate) and returning
   early on several paths: is each path's handle released exactly once under
   one exit, on every platform, and does the leak gate still fire for a path
   that forgets one.
3. **What clang does with the shapes**: whether clang finishes
   `slots-returns-800` today on each platform you reach (no timing beside
   four lanes: a finish or not within a bound you name, never a duration),
   and on the route.

Report as the shared brief says.
