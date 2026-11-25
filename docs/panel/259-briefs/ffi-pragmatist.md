# Panel 259, the ffi-pragmatist's brief

Read `00-shared.md` first, whole, then the critic's first reading (`docs/panel/259-reports/critic-briefs.md`, written at
the first base `2cf85ea64`; the briefs were repaired from its items 1 to 9 and re-frozen at the base `3ec775b66`,
`00-shared.md` § 2) and the two drafts it names (§ 4.6). Your seat judges **the C boundary** (design.md §1.11, §1.12,
§4.19, reached by `grep -n`; `.claude/rules/c-boundary.md`): what real C coroutine libraries do to a Heroes callback's
stack, and whether a resolution holds against them. You have a veto on ABI breakage and on anything that makes a binding
unsafe. You write and compile C, and **you measure with a stage 2 compiler** built in your own copy,
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/259-ffi-pragmatist`, a worktree of `3ec775b66`. **You hold this
Mac, Linux arm64 and Linux x86-64** (`heroes-linux:latest`, amd64, emulated; run it with `docker run --pull never` and
no `--platform`, `00-shared.md` § 3). `own.h`'s x86-64 arm has been run, by the critic alone: `own_carved`,
`unseen_carved` and `below` in the x86-64 image **at `2cf85ea64`, emulated** (`critic-briefs.md` § I, F22 to F24; the
repair re-ran the arm64 cells at this base and stopped its x86-64 run at the coordinator's word), so **their re-run at
`3ec775b66` is yours** (§ 5). **Every Linux cell you write names the base it was run on** (§ 2: the `dlsym-make` lane
will move the `dlsym` rows). **You have no page reader** (`.claude/agents/ffi-pragmatist.md`: Read, Write, Bash) and the
sitting forbids a download: what only a page can answer is the historian's (questions 1 and 6).

## What you are asked to settle, in this order

1. **Which real libraries put a coroutine's frames inside the thread's own stack range, and by which switch.** The
   shapes are the lane's, written for the defect (`shapes/own.h`, `shapes/cr.h`), and the critic's `below`. The sitting
   needs the world's: for each library you can build or bind here, say where its stacks come from (the caller's memory,
   `malloc`, `mmap`, a shared stack it copies into and out of) and whether its switch is one the runtime sees
   (`makecontext`, `swapcontext`, `setcontext`, which the runtime wraps; or its own code). **Start from panel 257's
   evidence** (`docs/panel/257-reports/evidence.tar.xz`, extract into an empty folder of your own copy; `00-shared.md` §
   4.7): its census of this Mac's 422 dylibs (`docs/panel/257-reports/ffi-pragmatist.md:563-572`, `nm -u` of each),
   **`programs/ffi-pragmatist/asyncjob.h`** (OpenSSL's ASYNC fibers: libcrypto imports `makecontext`, `swapcontext` and
   `setcontext`, its head says, by `nm -u`; whether this runtime sees those switches is yours to measure), **`sched.c`,
   `sched.h`** (a preemptive green-thread scheduler, `swapcontext` from a timer's handler), **`bctx.h`, `bctx.cpp`,
   `bctx_plain.hero`, `bctx_signal.hero`** (its binding of Boost.Context) and **`programs/critic/mine/n6.h`** (a
   `makecontext` task on a helper's local array, the carved shape). **Boost.Context is on this Mac alone** (Boost
   1.92.0_1, `libboost_context`, `fcontext.hpp`; **neither Linux image holds Boost**, and **this Mac holds no Boost
   assembly source**, § 4.7, so every Boost cell is this Mac's and its switch is read as a symbol, never as source):
   bind `make_fcontext` and `jump_fcontext` through a C header, give it a stack carved from a frame of the thread's own
   (an array in a C helper, as `own.h`'s `ow_host` does, and in the extern call's own frame, as `below.h`'s `ow_below`
   does) and one from `malloc`, hold a large value across a jump in a Heroes callback, and run both three times, plain
   and `--sanitize`. A library you name and cannot build here (libco, corosensei, greenlet, and any other) is **the
   historian's to read**: cite `docs/panel/259-reports/historian.md` for it, or write *unread*; a sentence that a class
   of library does not exist goes out as a question naming what was searched.
2. **Does any real binding need a carved stack?** The sitting's cost of a refusal (route A of the shared brief § 6)
   is the correct programs it stops. Which libraries hand the choice of stack memory to the caller, and does a
   real program put that memory on the thread's stack (a stack array, `alloca`, a stack-copying library's shared
   region)? A program the defect reaches only by the lane's own shape is a different weight from one a real
   library's documentation shows (the documentation is the historian's to read).
3. **The doors, again, with switches.** Panel 258's door table (`docs/panel/258-reports/ffi-pragmatist.md`, Darwin's
   at lines 51 to 67, Linux arm64's at 81 to 96): re-read it as `00-shared.md` § 4.3 reads it (8, 12 and 13 abandon
   frames unseen with the process running on; 9 and 10 end the thread with no event; 11 is `exit`; door 6 unrun on
   x86-64; *"3 of 13 doors with no event"* is a reading, not a count), and add the switches: a library's own switch
   onto heap, onto a carved stack, onto a stack it copies; **`makecontext` reached through `dlsym`, by each of the
   three handles** (`RTLD_DEFAULT`, `RTLD_NEXT`, the C library's own `dlopen("libc.so.6", RTLD_NOLOAD)` handle):
   on this Mac `RTLD_DEFAULT` and `RTLD_NEXT` answer the program's own (`critic/probes/dlh.hero` prints `state 1111`;
   the C library's own handle is unprobed on Darwin), and on Linux
   `RTLD_DEFAULT` answers the runtime's `makecontext`, which the executable exports, while `RTLD_NEXT` and the C
   library's handle answer the C library's (§ 4.2, the critic's probes re-run by the repair at this base in the arm64
   image; the x86-64 probes are unrun by anyone: yours); **in both Linux images `unseen_carved`, which asks `RTLD_NEXT`,
   is refused (arm64 at this base, x86-64 at `2cf85ea64`, the critic's), exit 134, in
   `HERO_SIGNAL_REENTERED`'s words about a handler the program does not have**, and the critic **traced** why (§ 5:
   rule 6 judged coroutine 0's yield, a `swapcontext` the runtime saw, landing below the frame making it on the one
   range it knew; re-run by the repair at this base in the arm64 image, the same lines and values): run it in the x86-64
   image at this base, and say whether the words can be true and whose class they are
   (2705, 2755, § 4.6); through `dlvsym`; from inside an image of Darwin's dyld shared cache. **For each: seen or not,
   by which route, measured by a counter in a copy of the runtime**, never by reading alone. The `dlsym-make` lane is
   repairing the `RTLD_NEXT` and C-library-handle doors now: name the base of every row.
4. **Route D, a switch the binding tells, from the C side.** Write the binding a library like Boost.Context would
   need if the runtime offered an entry point at each switch (its arguments: the new stack's bounds, as
   `__sanitizer_start_switch_fiber(void **fake_stack_save, const void *bottom, size_t size)` takes; the tree already
   calls that under AddressSanitizer, `tests/golden/run/fixedbugs-coroutines-switched-by-hand.h:22-23` and `:45-49`).
   Can a binding call it from its own header around `jump_fcontext`, without patching the library? What happens to a
   binding that forgets? **A runtime that learned switches from the sanitizer's calls would learn them in
   `--sanitize` builds alone** (§ 6 D, by `grep`: `fiber_fcontext.hpp` and `continuation_fcontext.hpp`, Boost's
   default backends, make no sanitizer call; `fiber_ucontext.hpp:154-167` makes it under `BOOST_USE_ASAN` around a
   `::swapcontext` the runtime already sees). **Does it move `HERO_RUNTIME_ABI` or `runtime/hero_os.h`'s contract**
   (*"a stability promise, decision 2228"*, carried from panel 258's ffi brief; `gh issue view 2228` reads CLOSED,
   *"May a program bind `hero_os.h`, the runtime's own header, which the specification never names?"*: read what it
   settled)? `runtime/heroes_runtime.h` moved at this base (+86 −2, `00-shared.md` § 2): read it here.
5. **Route B from the C side**: healing only on a jump the runtime saw. Which correct binding shapes that run on
   `main` today, a C jump over a callback the runtime does not see (assembly, a C++ exception through a callback's
   frames built with `clang++`, a `dlvsym`'d `longjmp`, **`__builtin_longjmp`**), would then leak, abort or be
   refused, and in which words? **`__builtin_longjmp` is yours**: it compiles on x86-64 alone (the repair, 07:44:
   clang refuses `__builtin_setjmp` for this Mac's target and for `aarch64-linux-gnu`, and writes an object for
   `x86_64-linux-gnu`), you hold that image, and panel 258 left the door **unrun** there (its report's line 407): run
   it in `heroes-linux:latest`, three runs, plain and `--sanitize`, against today's runtime and against the
   compiler-engineer's prototype runtime if it hands you its path by then; say which.
6. **Windows** (read, unless the coordinator authorises the box): a fiber's switch changes the thread block
   (`SwitchToFiber`), which `runtime/parts/stack_lane.c:89-100` reads; does Boost.Context's Windows switch
   (`make_fcontext` and `jump_fcontext` for `x86_64_ms_pe`) update the thread block's stack fields? **You cannot read
   it here** (no Boost assembly on this Mac, no Boost in either image, no page reader, no download: § 4.7): the answer
   is **the historian's** (its question 2); cite its report, or write *unread*. The shapes' `own.h` has no `_WIN32` stop
   where its golden twin does (its line 49 is the only `#error`, § 4.7; defect 2811 is the harness's half): a binding of
   that switch on Windows is a wrong program before any of this.
7. **x86-64, your cells**: the carved shapes and `below` were run there by the critic at `2cf85ea64` (§ 5): **re-run
   them at this base** (`own_carved`, `unseen_carved`, `below`, plain and `--sanitize`, and the `dlsym` probes
   `critic/probes/dlh.hero` and `dlh2.hero`), then **your library shapes** (those that build in that image: no Boost)
   and **door 6** (question 5), each three runs, plain and `--sanitize`,
   with a compiler built inside the container from the seed and stage 2 (`00-shared.md` § 5's script, `runs/all.sh`,
   is the model). Say where an x86-64 cell differs from arm64's, and remember the image is **emulated**, not the CI's
   native leg.
8. **Each draft sentence's clauses** (`arms/arm-r`, `arm-t`, `arm-p`): mark each clause **true** (the program and its
   cell), **false** (the program that falsifies it) or **untested** (why). The critic's F18 marks clauses the runs
   already decide (`arm-r`'s *"given to `makecontext`, is followed"* against the Linux `RTLD_NEXT` door; its scope
   *"not by `swapcontext`"* against `unseen_carved`, which switches by the runtime's `swapcontext` and aborts; `arm-p`
   against `own_carved` and `below`): re-run them. A clause you find false: write the repaired clause in
   `docs/panel/259-reports/ffi-clauses.md` of your copy and run the repaired claim.

Keep every program you write, with the script that built and ran it, in
`docs/panel/259-reports/programs/ffi-pragmatist/` of your copy (sources, never a binary), so the critic can run them on
the other platform.

## The report

`docs/panel/259-reports/ffi-pragmatist.md` of your copy, written as you go: your verdict, the table of libraries (name,
version, where its stacks come from, its switch, seen or not, the shape run and its cells, each platform with the base
it ran on; `below` beside `own_carved`), the doors with switches and the three handles, the binding route D would need,
what a veto would compel, what the conservative resolution would have been, and one falsifiable prediction with the
measurement at which it is checkable. Then the verdict structure of `.claude/agents/ffi-pragmatist.md`, and one line
per numbered question.
