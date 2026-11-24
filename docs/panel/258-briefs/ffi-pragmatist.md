# Panel 258, the ffi-pragmatist's brief

Read `docs/panel/258-briefs/00-shared.md` first, whole. Your seat judges **the C boundary** (design.md §1.11, §4.19;
`.claude/rules/c-boundary.md`): what real C code does to a Heroes callback's frames and whether a resolution holds against
it. You have a veto on ABI breakage and on anything that makes a binding unsafe. You write and compile C. **You measure with
a stage 2 compiler** built in your own copy (stage 1 emits no `hero_callback_left()`).

## What you are asked to settle, in this order

1. **The doors, measured.** Write small C programs that abandon a callback's frames each way a real library does and run each
   against `docs/panel/258-briefs/jumps/` and `leak/`'s shape (a Heroes callback holding a string and an array): `longjmp`,
   `_longjmp`, `siglongjmp` (with and without the saved signal mask), `__longjmp_chk` (a fortified build),
   `__builtin_longjmp`, `setcontext`/`swapcontext` with the old context never resumed, `pthread_exit` and a cancelled thread
   inside a callback and in a `spawn` body, `exit` inside a callback, a handler that returns with an edited register file, a C++
   exception through the callback's frames if a C++ compiler is at hand. **For each: which the runtime sees**
   (`hero_jmp_jump`, `runtime/parts/sigjump_save.c`, Linux glibc arm64 in the container `heroes-linux-arm64:latest`), **by
   which route** (the target's stack pointer is decoded only where the key verified, otherwise route A's record judges it:
   `sigjump_save.c:43-59`) **and which it does not**, by running it with a counter in a copy of the hook, never by reading
   alone. Darwin's route is `runtime/parts/rebind_jump.c` (it rewrites only images outside the dyld shared cache): say which
   of the same doors it sees on this Mac.
2. **The binding shapes that put a Heroes frame above a library's jump.** The first brief's libpng example was wrong for
   panel 257's `pngread` (4,000 corrupt images, no frame abandoned: shared brief). **Write at least two shapes that do**: a
   libpng read callback that reports its own stream's failure by `png_error` from inside the Heroes callback, and a
   progressive reader's row callback that calls back into libpng; Lua's `lua_error` over a Heroes function and a watchdog if
   you can. For each, `leaks --atExit` counts and the exit code at 1,000 jumps, and say what a service that used the shape
   would lose. **No program in the tree jumps over Heroes frames today** (shared brief): say which real bindings Rebel or a
   user is likely to write that will.
3. **Windows**, which the runtime does not interpose: what a C `longjmp` over frames does there (clang targets
   `x86_64-pc-windows-msvc`, so it is the UCRT's `longjmp`; whether it unwinds through SEH and runs termination handlers is a
   claim for you to check in Microsoft's documentation and, if a run is justified, on the box under the lock protocol of the
   shared brief), and whether (d)'s Windows arm (`__try/__finally`) exists. Anything you cannot run is *unrun*.
4. **The hazard of where a release runs, from the C side**: a jump from inside a signal handler (`tests/golden/run/
   fixedbugs-2704-*`, the second case, and `jumps/alarm.hero`), from a thread other than the main one, whose target is above
   a coroutine switch. Show with a C program, under `--sanitize` where it applies, **what a `free` inside the hook does when
   the signal interrupted `malloc`** (a deadlock or a corruption, or say you could not provoke it and how hard you tried).
   And the critic's D7: a release that runs a binding's consumer in the hook frees the library's own `jmp_buf` (libpng keeps
   it in the `png_struct`): show it or refute it.
5. **ABI and what a binding sees.** Say whether any mechanism moves `HERO_RUNTIME_ABI` or a `hero_*.h` contract a library or
   a package of Rebel's platform layer compiles against (`runtime/hero_os.h` has a stability promise, decision 2228); whether
   a C library that holds a Heroes value across the abandoned frames is made unsafe by releasing it; and **which of the
   bindings of item 2 the told refusal (g) would break** and whether that is the right price. **Prior art for (g)** (Rust's
   `C-unwind`, cgo, OCaml's callbacks) is unverified and this lane has no historian: name it only as a question.

## The report

Your report, a file of your copy's `docs/panel/258-reports/` folder named `ffi-pragmatist.md` (2026-11-24): your verdict, a
table of the doors (door, platform, seen or not, route, the measured counter), the binding shapes' counts, what a veto would
compel, what the conservative resolution would have been ((a'), not (a)), and one falsifiable prediction with the
measurement at which it is checkable.
