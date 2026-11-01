# Panel 233, the ffi-pragmatist's brief

Read `00-shared.md` first; it binds you. Your worktree: `.claude/worktrees/233-ffi-pragmatist/`.
Your report: `docs/panel/233-reports/ffi-pragmatist.md`, written as you go.

Your seat's question is the C boundary. A group's `record` is the header's struct (spec § 13), passed to
and from C at C's own layout and ABI, so a route that changes how a large record is passed, returned or
held must leave every `extern` call's ABI as C declares it. Write and compile the binding C each route
implies: a header with a large struct passed by value to a C function and returned from one, a group
record held inside a Heroes record past the route's threshold, and a callback taking such a struct.
State, for each route of `00-shared.md`, whether any `extern` call's C would change, and compile it.

A carried claim, checked by the critic before you start: defect 2253's run cases hold a group record
`Held`, declared `partial`, 128 MiB in its C struct (`tests/golden/run/fixedbugs-2253-held.h`), whose
unit compiles at `-O2` in 191 million instructions. But the inference *the cost follows the fields a
record names, not its bytes* changes three things at once: that struct is an array member, its address
escapes to C, and there is no call chain to inline. Separate them: measure (instructions retired and
peak memory, the command in `00-shared.md`) a header struct of many named scalar fields, one of the same
size as an array, and the reproducer's shape, and say what that means for the threshold's unit.
