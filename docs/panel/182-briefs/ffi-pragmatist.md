# Panel 182, brief for the ffi-pragmatist

Read `00-shared.md` first, in this directory. Your seat judges the founding
constraint (design.md §1.11, §4.19: everything comes from C), with a veto on
ABI breakage.

**Your directory is `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-182/ffi-pragmatist/`,
and it is yours alone.** Make it with `git -C /Users/joseph/Temp/heroes/heroes-lang
archive 0fc98107 | tar -x -C <your directory>`, then `rm -rf build` inside it,
and build your own compiler there from the seed: `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, about 3 s. Never build, run or edit anything in
the trunk or in another seat's directory, and kill only processes you started,
by PID. Set `HEROES_RUNTIME=<your directory>/runtime` when a binary outside the
tree's root runs `run` or `build`.

## What you decide

1. **The C each route implies, compiled.** Take the emitted
   `h_keywords_keyword` from your seed (`00-shared.md` gives its shape) and
   write by hand the C that routes (a), (b) and (c) of `compiler-engineer.md`
   would emit for it: (a) the same function with `= {0}` only on the 23 owned
   variables and `h1_s0`, `h2_r0`; (b) the arms storing into one slot; (c)
   each arm's slot released at its join. Compile each with the runtime under
   `clang -std=c11 -Wall -Wextra -Wuninitialized -Wsometimes-uninitialized`,
   and under `-fsanitize=address,undefined`, and drive it with a harness that
   calls it on every keyword and on a non-keyword a million times; report the
   warnings, the sanitizer's verdict, and the time of each against today's
   function, at `-O0` and `-O2`.
2. **What crosses the C boundary.** A value returned to C, a value handed to
   an extern, a callback's arguments, a record passed by pointer to C: say
   whether any route changes what C sees at a call, and whether any changes a
   declaration in `runtime/heroes_runtime.h` (ABI 26). An uninitialised value
   that C could read, through a pointer an `extern` was handed, is the case to
   look for: an `@` argument, a `link` parameter, a callback thunk
   (`selfhost/emit/callback_thunk.hero`).
3. **The FFI goldens**: build and run the `tests/golden/run/` cases that bind C
   (`grep -l extern tests/golden/run/*.hero`) with each route's emission if a
   seat's prototype exists by then, or say which you ran with today's; and
   whether a C compiler other than clang, the Windows box's `clang` with
   `-Wl,/STACK:67108864`, is owed a run for a route (say so; the coordinator
   runs the box).

## Prediction

Register one falsifiable prediction with an instrument that exists today,
scored at this milestone's close (M-agreed-retention).

Write your report to `<your directory>/report.md`, and copy it to
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/182-reports/ffi-pragmatist.md`
(the only file you write in the trunk). English, no em dashes.
