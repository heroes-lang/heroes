# Panel 258, the compiler-engineer's brief

Read `docs/panel/258-briefs/00-shared.md` first, whole: it carries the proposal (c), the measured leak and the hazard of
where a release may run. Your seat judges **implementation cost, soundness and what the emitted C owes**
(design.md §1.1, §1.7, Part 5; `.claude/rules/generated-c.md`). You have a veto. Your files are `selfhost/` and `runtime/`
(never `crates/`, which is archived: nothing builds it); the cheap route is the seed built with clang in a few seconds and
stage 2 from it, the Mac being loaded so budget minutes, not seconds, for stage 2 (`./heroes build selfhost/main.hero -o
heroes-next`).

## What you are asked to settle, in this order

1. **Is a slot's state "owned or zero" at every C call site?** (c) is exact only if it is. Read the emitted C of programs
   that hold each shape in a function a callback runs: a `str`, an array, a map, a record with refcounted fields, an
   optional, a variant payload, a function value, an `@` parameter, a `for` variable, the `own` slots the emitter makes
   (`h6_own6`, `h10_own10` in `docs/panel/258-briefs/leak/main_1k.emitted.c`), a value moved into a record or a callee, an
   early `return`, a `.must()` panic path. For each, say where a call into C can happen between a store into a slot and
   its release, and **find a program where a slot holds a borrowed or half-moved value at a call site, or say you looked
   and found none, and how**. A slot released twice is memory corruption, which outranks every other cost
   (CLAUDE.md § Precedence, rank 3).
2. **Which functions need a record?** Three routes, and you may find a fourth: every function (cost on every call of
   every program); only functions reachable from a function handed to C as a callback (a call-graph question: say what
   it does with indirect calls, function values, generics and a callback that calls a closure over a module boundary,
   and whether it can be unsound); or a record pushed only while the thread's callback count
   (`hero_callbacks_inside`, `runtime/parts/callback.c:61`) is above zero, tested in the prologue (a thread-local load on
   every call). Name the one you would build and the cost of each.
3. **Where may the release run?** The hook `hero_jmp_jump` runs before the real jump, with the abandoned frames still on
   the stack; it may be running inside a signal handler (the second golden `fixedbugs-2704-*`). `free`, the allocator's
   locks and refcount decrements that reach `free` are not async-signal-safe, and the abandoned stack memory is reused by
   the code that lands. Decide: release in the hook, or **copy the slots' pointers out in the hook (reading memory, no
   call) and release them at the next safe point** (the landing frame's next callback entry or exit, the next
   allocation, `hero_callback_left`, the thread's end), and what an orphaned value costs while it waits.
4. **Cost, measured**: in your copy, hand-edit the emitted C of a hot function (a `fib`-like recursion, 35 deep) three
   ways (no record; a record pushed on every entry; a thread-local flag test) and time each with `/usr/bin/time -p`,
   several runs, the first discarded, the ratio of `real` to `user` plus `sys` read (this Mac is loaded: a timing whose
   ratio is far above 1 is discarded and the reason written down). Then **prototype (c) by hand on the leak test**:
   edit the callback of `docs/panel/258-briefs/leak/main_1k.emitted.c` to register its slots and make the hook release them, build and run the
   100,000-jump variant and report the maximum resident size against the 352.9 MB it reads today (the shared brief gives all eight readings).
5. **What a coroutine, a thread and a nested callback do to the chain**: a callback entered on one thread and left on
   another (the module doc of `callback.c`); a thread that ends with abandoned frames; a jump that lands inside an outer
   callback's extent (nesting); `setcontext` and `swapcontext` as the other doors.
6. **Does (c) need a spec sentence?** Spec § 13 has one (panel 257's) about the exits of a signal handler a callback may
   abort after. Say whether a promise that *what an abandoned frame held is released at the jump, where the runtime sees
   the jump* has to be stated, and if so say so first: it sends the sitting to the full panel.

## The report

Your report, a file of your copy's `docs/panel/258-reports/` folder named `compiler-engineer.md` (2026-11-24): your verdict, the build you would choose and its cost, the prototype's
number, the counterexamples you found or the reading that found none, **what a veto would compel** and what the
conservative resolution would have been (it is the interim: the audit stays off, the leak stays), and one falsifiable
prediction with the measurement at which it is checkable.
