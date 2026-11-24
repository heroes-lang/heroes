# Panel 258, the compiler-engineer's brief

Read `docs/panel/258-briefs/00-shared.md` first, whole: it carries the question, the table of what a jump leaves behind
(five defects, 2752 to 2756), the proposals (a), (a'), (c) to (h), the hazards the critic found and the corrected facts. Your
seat judges **implementation cost, soundness and what the emitted C owes** (design.md §1.1, §1.7, Part 5;
`.claude/rules/generated-c.md`). You have a veto. Your files are `selfhost/` and `runtime/`; **you measure with a stage 2
compiler** (stage 1 emits no `hero_callback_left()`), built in your own copy, which takes minutes on this loaded Mac.

## What you are asked to settle, in this order

1. **Is a slot's state "owned or zero" at every C call site, and what does an abandoned frame hold besides slots?** Read the
   emitted C of programs that hold each shape in a function a callback runs: a `str`, an array, a map, a record with
   refcounted fields, an optional, a variant payload, a function value, an `@` parameter, a `for` variable, the `own` slots
   (`h6_own6` to `h11_own11` in `docs/panel/258-briefs/leak/main_1k.emitted.c`), a value moved into a record or a callee, an
   early `return`, a `.must()` path; **and** the large-value room (`runtime/parts/frame.c`), a handle with `acquires`, a
   lease, a lent buffer, a scratch listing. Find a program where a slot holds a borrowed or half-moved value at a call
   site, or say you looked and found none, and how. **Under which events would (c) release a value that is still live**
   (shared hazard 3: an unseen jump leaves records in reused stack memory, a hand-written stack switch interleaves two
   chains)? A slot released twice is memory corruption, which outranks every other cost (CLAUDE.md § Precedence, rank 3), and
   the interim cannot do it.
2. **Which mechanism, judged against the table's seven rows**: (a') the interim widened (every balance gate forgives or
   tells the truth), (c) an extent record chain released by the hook, (d) forced unwinding with `__attribute__((cleanup))`
   and `_Unwind_ForcedUnwind`, (e) lazy detection by stack pointer with no hook, (f) the hybrid, (g) a told refusal, (h) a
   release point after the landing. For each: which rows it repairs, which it leaves, what it costs on every call of every
   program, what it needs of the emitter (`selfhost/emit/callback_thunk.hero`, the prologue the emitter writes) and of the
   runtime. **(c) must unwind every runtime stack, not only slots**: say how the count, the room (`hero_frame_held`), the
   lend depth, the handle and lease obligations come back to balance, and what a `spawn` thread's body, which has no thunk,
   needs.
3. **Which functions need a record** (for any mechanism that has one): every function; only functions reachable from a
   function handed to C as a callback; a record pushed only while the thread's callback count is above zero (the count is
   `static _Thread_local` at `callback.c:58`: price the export); or **callback-reachable and live across a call into C** (a
   frame that calls no C cannot be on the stack at a synchronous jump). Say what each does with indirect calls, function
   values, generics and a callback that calls a closure over a module boundary, and whether it can be unsound.
4. **Where may a release run?** The hook runs before the real jump, possibly inside a signal handler; `free`, the
   allocator's locks and a drop that reaches `free` are not async-signal-safe; the abandoned stack memory is reused by the
   code that lands; the handler-side record is reached through the thread key, never a thread-local access
   (`runtime/parts/sigentries.c:60-63`). Decide: in the hook; copy the pointers out and release at the next safe point; or
   (h) at the return of the extern call into Heroes code. Say whether an **asynchronous jump** (the `SIGALRM` row: any
   instruction, inside the allocator) is in scope at all.
5. **Cost, measured**: in your copy hand-edit the emitted C of a function that **holds refcounted slots** (`fib` holds none,
   so it measures only the push: use a recursion that builds and returns a string or an array) three ways (no record; a
   record pushed on every entry; a thread-local flag test), several runs, the first discarded, the ratio `real` to `user`
   plus `sys` read and a loaded run discarded with the reason. For the "every function" route, **the contract's instrument
   for compiler speed is `heroes run tests/harness/latency.hero -- compare`** (`.claude/rules/verification.md` § The latency
   ceiling's instrument): say what it would read. Then **prototype the mechanism you would build by hand on the leak test**
   (edit `leak/main_1k.emitted.c`'s callback) and report `leaks --atExit` counts at 1,000 jumps (7,000 leaks and 3,952,000
   bytes today) and the room and lend programs' exits.
6. **Does any resolution need a spec sentence?** Spec § 13 has one (panel 257's) about the exits of a signal handler a
   callback may abort after; say whether a promise about what happens to an abandoned frame's values has to be stated, and
   if so say so first: it sends the sitting to the full panel.

## The report

Your report, a file of your copy's `docs/panel/258-reports/` folder named `compiler-engineer.md` (2026-11-24): your verdict,
the mechanism you would build and its cost, the prototype's counts, the counterexamples you found or the reading that found
none, **what a veto would compel**, what the conservative resolution would have been ((a'), not (a)), and one falsifiable
prediction with the measurement at which it is checkable.
