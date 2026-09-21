# Panel 173 — brief for the ffi-pragmatist

Read `docs/panel/173-briefs/00-shared.md` first. You judge the founding
constraint at the boundary — design.md §1.11, §4.19 — and the platform facts:
`.claude/rules/platforms.md` says a platform fact is run on a platform or it is
an inference, and `.claude/rules/c-boundary.md` says Linux is where a leak in a
binding is visible. You hold a veto on ABI breakage, and here the ABI question
is: **does a signal handler in the runtime change anything a C library
observes?**

## Build

Fresh copy as the shared brief says; `git apply docs/panel/173-briefs/prototype.diff`;
rebuild; confirm the ten-run table on `lease070.hero` and the control.
Foreground only, generous timeouts, no polling loops.

## Questions you must answer, each with a command

1. **Linux, on the platform.** `docs/ref/environment/linux/LINUX-MACHINE.md`
   says how to start Docker Desktop and enter `heroes-linux-arm64` (native on
   this Mac). Inside it, from a copy of your copy (the file says how the tree
   gets in): build the seed compiler, build `lease070.hero` with the stock
   runtime and with the prototype, run each ten times, and report exit code,
   stderr bytes and the FIRST line of stderr. glibc 2.41 prints
   `free(): invalid pointer` and calls `abort()` for an interior free — measure
   whether that is so, which signal arrives (SIGABRT, or SIGSEGV if glibc's
   check does not fire), whether the prototype's line prints beside glibc's or
   instead of it, and whether `--sanitize` there (LeakSanitizer exists on that
   leg, CL-055) composes: ASan lines, ours, exit code. If Docker cannot start,
   say so and mark every Linux row UNRUN in those words — never infer it.
2. **The two paths on Darwin.** Panel 172's critic measured 133 = SIGTRAP on
   the interior-free and double-free paths and 134 = SIGABRT on the others,
   from `docs/panel/173-briefs/variants.c`. Re-run `variants.c` and say which
   allocator paths raise which signal, so the goldens can pin what they see.
3. **Real libraries, not `static inline`.** Build the filed shape against a
   library that actually frees: `sqlite3_free` on a lease
   (`extern "sqlite3.h" link "sqlite3"`, `function sqlite3_free(p: ptr)` — a
   `cstr` lease needs a `ptr` parameter; say how you spelled it or what refused
   you), and `free` from `stdlib.h` directly. Does the report print through a
   dynamic library's `free`, and is SQLite's own allocator (`sqlite3_free`
   checks its header) a different signal or a different message? Ten runs each.
4. **What C observes.** A handler installed for SIGTRAP and SIGABRT is
   process-wide. A C library that installs its own SIGABRT handler (rare) or
   relies on `abort()` reaching the default disposition (a test harness, a
   crash reporter) sees the runtime's first. Write the smallest C program that
   installs a SIGABRT handler through the header, bind it, install the
   prototype, and measure whose handler runs and whether the prototype's
   `SIG_DFL`-then-`raise` loses the library's. Say whether that is a veto
   (design.md §1.11: everything comes from C, and a runtime that eats a
   library's signal handling is a boundary the library did not agree to) or a
   documented limit.
5. **The message's claim.** The three lines say *a C function freed bytes this
   program still leases*. Is that TRUE on every path that raises SIGTRAP or
   SIGABRT while a lease is live — a Heroes panic with a live lease says it
   falsely today (question 1 of the engineer's brief). List the paths where the
   sentence would be false with the prototype as written, so the landing's
   message says exactly what is known: *a lease was live when the process was
   killed by the allocator* is weaker and true.

## Report

`docs/panel/173-reports/ffi-pragmatist.md`: verdict · section · cost · one
falsifiable prediction naming an instrument that exists (the Linux leg of
`.github/workflows/ci.yml` under `--sanitize`, or a `tests/golden/run/` case) ·
veto condition if any · the C you compiled and every table, with commands and
exit codes. English only. About 25 minutes of work; mark the rest UNRUN in
those words.
