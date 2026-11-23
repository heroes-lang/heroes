# Panel 258, the ffi-pragmatist's brief

Read `docs/panel/258-briefs/00-shared.md` first, whole. Your seat judges **the C boundary** (design.md §1.11, §4.19;
`.claude/rules/c-boundary.md`): what a real C library does to a Heroes callback's frames and whether (c) holds against
it. You have a veto on ABI breakage and on anything that makes a binding unsafe. You write and compile C.

## What you are asked to settle, in this order

1. **The doors, measured.** Write small C programs that abandon a callback's frames each way a real library does and
   run each against `docs/panel/258-briefs/leak/main_1k.hero`'s shape (a Heroes callback holding a string and an array):
   `longjmp`, `_longjmp`, `siglongjmp` (with and without the saved signal mask), `__longjmp_chk` (a fortified build),
   `__builtin_longjmp`, `setcontext`/`swapcontext` with the old context never resumed, a `pthread_exit` or a cancelled
   thread inside a callback, `exit` called inside a callback (the interim rule's own case), a C++ exception thrown
   through the callback's frames if a C++ compiler is at hand. **For each: which the runtime sees** (`hero_jmp_jump`,
   `runtime/parts/sigjump_save.c`, Linux glibc arm64 in the container `heroes-linux-arm64:latest`) **and which it does
   not**, by running it with a counter in a copy of the hook, never by reading alone. Darwin's route is
   `runtime/parts/rebind_jump.c`: say which of the same doors it sees on this Mac.
2. **A real library.** `libpng` 1.6.59 is installed on this Mac (`pkg-config --modversion libpng`); panel 257's `pngread`
   (real libpng, its error path over a Heroes read callback) is in `docs/panel/257-reports/evidence.tar.xz`. Run it
   in a loop of corrupt images (a truncated PNG, a bad CRC) and report the maximum resident size at 1,000 and at 100,000
   images, as `docs/panel/258-briefs/leak/time_100k.txt` does for the synthetic case. **Say what the leak really is in that program**
   (what the frames held, how many bytes an image) and so what (c) would buy a service that decodes untrusted images.
3. **Windows**, which the runtime does not interpose (`grep -l -i longjmp runtime/platform/*win*.c`, nothing): what a
   C `longjmp` over frames does there (MSVC's `longjmp` unwinds through SEH and runs termination handlers, which is a
   claim for you to check in Microsoft's documentation and on the box if a run is justified: the box answers `ssh win`,
   one run at a time under its lock `C:\w\box.lock` with an owner file; it is up to about 06:29 the same morning), and
   whether (c) has a Windows arm, a different one, or none. Anything you cannot run is *unrun* in your report.
4. **The hazard of where the release runs**, from the C side: a jump from inside a signal handler
   (`tests/golden/run/fixedbugs-2704-*`, the second case), a jump from a thread other than the main one, a jump whose
   target is in a frame above a coroutine switch. Show with a C program, under `--sanitize` where it applies, **what an
   `free` inside the hook does** when the signal interrupted `malloc` (a deadlock or a corruption, or say you could
   not provoke it and how hard you tried).
5. **ABI and what a binding sees.** (c) adds a chain and a call in the thunk; say whether it moves
   `HERO_RUNTIME_ABI` or any `hero_*.h` contract a library or a package of Rebel's platform layer compiles against
   (`runtime/hero_os.h` has a stability promise, decision 2228), and whether a C library that holds a Heroes value across
   the abandoned frames (a handle kept in C state) is made unsafe by releasing it.

## The report

Your report, a file of your copy's `docs/panel/258-reports/` folder named `ffi-pragmatist.md` (2026-11-24): your verdict, a table of the doors (door, platform, seen or not seen, the
measured counter), the `libpng` loop's numbers, what a veto would compel and what the conservative resolution would have
been (the interim: the audit stays off, the leak stays), and one falsifiable prediction with the measurement at which it
is checkable.
