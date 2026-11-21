# Panel 251, compiler-engineer

Read `00-shared.md` first, whole: the frozen commit (`3a5ee269117a31780ef1f96d0305452630ac4d83`, stage 2 `sources
23c7d2a626b29fd5`), what moved since the 2026-11-15 briefs (the platform layer's fourteen members, three
new signal installs and three reads, a second forked child, new runtime threads, `alloc.c` +37, `stack.c` +2, `os.c` +121, `runtime.c` +34),
the three proposals and their conservative alternatives, the record of each question, Rebel's
witnesses, and the rules. You judge from the compiler's and the runtime's side, with a veto on
soundness: a veto names the program that dies in silence, is refused while correct, or corrupts memory
under the route vetoed, **built and run**.

**Your folder** is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/251-compiler-engineer/`,
made by you as `00-shared.md` § Where each seat works shows (a `git archive` of the frozen commit, the
`.git` file, the seed and stage 2: 12.08 s and 151 s for the writer, 238 s for the critic). Binaries `ce-...`, containers
`ce-251-<n>`, one at a time. **Your report** is `.claude/worktrees/rebel-round/docs/panel/251-reports/compiler-engineer.md`,
written as you go, in `.claude/agents/compiler-engineer.md:36-44`'s structure (`verdict`, `section`,
`implementation_cost` with file citations, `needed_for_self_hosting`, `argument` of at most 120 words,
`prediction`, `condition`), **one block per question**, each after the runs it rests on. No paid run, no
credential, no `heroes test`, nothing written outside your folder and your report.

## The cheap route to a compiler with a change

Edit your copy's `runtime/` and hand it to the stage 2 you built through `HEROES_RUNTIME`
(`selfhost/cli/doctor.hero:7`, the compiler's own channel; the runtime's key moves, the compiler's does
not), or edit `selfhost/` and rebuild stage 2 from your seed (151 s for the writer, 238 s for the critic), whose `sources` key
then moves. Say which you used for each row. A route's cases run through the `run` form narrowed by the
harness's third word, `./heroes-next run tests/harness/main.hero -- ./heroes-next run <substring>`, its
output whole into a file (the guard refuses a gate piped into `head` or `tail`).

## Question 1: route J into B' on Linux, and the escape

By 249's R3 (`docs/panel/249-*.md:253-282`), unchanged: **you build J into B' on Linux arm64 and the
emulated x86-64 image**; the ffi-pragmatist builds it into D-rebind on Darwin; **J is one rule on both**,
or each of you says where and why it differs (read its report as it lands).

- **Start from `bw/routes/bprime/`** (`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/251-briefwriter/routes/bprime/`,
  the overlay ported to the frozen `runtime.c`, `00-shared.md`): `runtime/parts/siginstall.c` 311 lines
  (the prune 113-122, the wrapper 124-153 pushing its frame at 135-139, `hero_sig_inside` 157-166, the
  definition of `sigaction` from 168), the thunk's one line at `parts/sigguard.c:345`; `bsecond/` is
  B'' (339 lines); `a-sigrtmax/` A with the vetoed marker (`siginstall.c` 255 lines); `c-word/` C. The
  2026-11-15 brief's description of each stands, its files byte-equal (`bw/routes/routes-manifest.txt`).
- **Build J**: the runtime defines glibc's exported save points `setjmp`, `_setjmp`, `__sigsetjmp`
  (`sigsetjmp` is glibc's macro for the last, carried) and `getcontext`, `swapcontext`, each recording
  its buffer and its caller's frame and tail-calling glibc's own; the jumps `longjmp`, `_longjmp`,
  `siglongjmp`, `__longjmp_chk`, `setcontext`, `swapcontext` drop only the entries whose wrapper frame
  lies deeper than the target's recorded frame. The proposal's rule for a buffer with no record: **keep
  every entry** (`00-shared.md` § The three proposals): run it and the other way, and say which program
  each refuses or misses. Say where the record lives, its size and what a full one does; how *deeper* is
  read across the thread's stack and an alternate one; **where the C library's own pointers are
  resolved** (before `main`, never by `dlsym` inside a handler; A resolves at every jump,
  `bw/routes/a-sigrtmax/runtime/parts/siginstall.c:113`, `:119`, `:126`); and the lines per file, the
  compiler's included.
- **The runtime's own calls now go through it** (`00-shared.md` § Question 1): `run.c:1805`'s `sigaction`
  in a forked child (async-signal-safe there?), `stack.c:1220`'s install and the prior it calls at
  `stack.c:743-746`, `os.c:392`'s prior call, the three new installs (`platform/hero_proc.c:1630`,
  `platform/hero_term.c:258`, `:278`) and three reads (`hero_proc.c:1628`, `hero_term.c:247`, `:272`).
  Name what each does under B'+J. **And `hero_proc.c`'s forked child** (`hero_proc_child`, 1603): a loop
  over every signal, a read at 1628 and an install at 1630 each, between `fork()` (1671) and `execve`
  (1633), taken only where `hero_proc_spawn_enters()` (1438) answers false and a folder is named
  (1777-1779), which no image here does by itself: **reach it in code** (a build with
  `hero_proc_spawn_enters` forced false; a second thread holding the lock your `sigaction` takes, if it
  takes one, at the fork) and say whether B'+J's `sigaction` is safe there.
- **Run**, Linux arm64 first and then the emulated image, base, B' and B'+J side by side, `-O0` and
  `-O2`, the cases with their kept paths under `.claude/worktrees/p251-scratch/ev249/` (copied into your
  folder): **must be refused** (exit 134, R1's words): `injump` 20 of 20, the three shapes and `j0f`,
  `j1f` 20 of 20, `injump` with `SA_ONSTACK`, `kmiss` 20 of 20, 247's `flib` static and shared 20 of 20,
  `fliblocale` storms (20 armed in Linux arm64, 5 emulated, counted only where `armed` printed), the deep
  library's program; **must not be refused**: the twelve depth programs; **the goldens**: `run
  fixedbugs-2558`, `run recovers-leaves-no-line`, `run fixedbugs-2494` with your stage 2 over B'+J, every
  move named; `--sanitize` and `--sanitize -O0` on `flib`, `injump`, one depth program and a correct
  program that jumps out of a deep frame and then uses its stack (clang 22's static ASan archive defines
  the jumps as weak interceptors, carried); the holes (a `memcpy`'d buffer, `pthread_exit` and a
  cancellation out of a handler, `__builtin_setjmp` / `__builtin_longjmp` by the image's own clang).
- **The CI's three reds** (`00-shared.md`: run 43769808533, job 132982964177, `run: 830 passed, 3 failed`,
  the three 2494 cases at `-O2`): does B'+J turn them green in the emulated image, and does C?
- **The escape, in those words**: J's Linux definitions are three names `ffi_defined_elsewhere`
  (`selfhost/cli/supplied_words.hero`) would refuse a library for defining. Build the widening or say it
  is not needed, with the program that shows which; a widened diagnostic is a class this sitting rules.
- **Then B'', C and A as the 2026-11-15 brief owed** (`p251/docs/panel/251-briefs/compiler-engineer.md:109-143`,
  uncommitted, in the worktree `.claude/worktrees/p251`), **only where J leaves them a row**: whether
  B'' is still needed once J stands, A's frame under J, C's `run` whole and `cache` alone. Say which you
  ran and which you left owed, and why.

## Question 2: the end of `main`, built

The code, every line `grep -n` at the frozen commit (`00-shared.md` § What moved for the moved files):
`runtime/parts/spawn.c` 664 (the slot's state comment `:222`, `state = 1` at `:508`, `= 0` at `:574` and
`:650`: no line writes 2; `hero_spawn_start` `:478`, `hero_spawn_await` `:311`, the thread's end
`:434-457`, `hero_spawn_join` `:620`); `spawn_str.c` 107; `runtime/hero_os.h:441-495` (the doors);
`alloc.c` (the premise `:79-80`, `hero_live_blocks` `:85`, the gate `:306`, its block line `:311-312`,
scratch `:356`, leases `:371`, handles' pass under `hero_handle_lock` from `:442`, `hero_alloc_block`
`:673`, the layer's uncounted memory `:619-655`); `thread.c` 144; `lend.c:962-963` (the per-thread audit
at a thread's end); `selfhost/emit/c_main.hero:102` (the gate after `main`); `hero_exit` at
`os.c:1126`.

1. **The census**: instrument your runtime (state 2 written at a thread's end; at the gate, the slots in
   state 1 and 2 counted to stderr) and run the `run` form narrowed to the thread cases, the eleven
   `examples/` door programs, Rebel's `examples-rebel/reach_threads.hero` (its catalogue's `path`
   entry) and the compiler building itself. Name every program that leaves a thread unjoined at the end
   of `main` on some path: each is a program route R moves. **Not `heroes test`.**
2. **R, built**: state 2 written; the gate asks the table first, under its lock, and reads no balance
   while any slot is 1 or 2; its message written out whole, naming each thread and **where it was started**
   (the handle machinery records an acquiring call's place, `<rebel>/threads/internal/lives.hero:18:12`
   in the writer's run; the door passes none today: say what it costs to pass one); the runtime's own
   threads (`00-shared.md`: `hero_net.c`, `run.c`, `hero_proc.c`, `hero_watch.c`, `hero_tls.c`) not
   counted, and say how you showed it. Run on `bw/repro/` (copy it): `unjoined`, `unjoined_str`,
   `unjoined_noheap`, `cr-late`, `cr-lease`, `finished_unjoined`, `exit_running`, `t1`, `t3`, and the
   writer's two Rebel programs (`bw/tree/examples-rebel/bw_unwaited*.hero`), each 20 runs, every outcome
   counted.
3. **E, built beside it**, the conservative, in sitting 252's words (`bw/arms2/q2-e.diff`, +27): *"When
   `main` returns, the program ends, and a thread still running ends with it; the program says how many
   it ended."* Today's ending, the gate telling running threads by count as the program's; say where
   the count is written, the spec naming no error stream. Same cases, same counts.
4. **Robustness at the end, under base, R and E**: a worker in a tight allocate-and-free loop, one in
   `print`, one holding a lease, while `main` returns, 200 runs each: count every exit other than 0 and
   134 and every partial line (sitting 252's critic read 2 of 20 ending mid-line under today's ending,
   carried).
5. **Is every counted block born and freed on one thread?** A per-slot balance not zero at a thread's
   end, over the thread cases and the compiler building itself, is a block that crossed: name any.
6. **Route C, priced or refused**: a compile-time check that every spawn is joined on every path, given a
   handle stored in an array (`selfhost/cli/link.hero:348`, a test), a record, a return value
   (`selfhost/check/acquiring.hero:23-28`, `moved.hero`, `consuming.hero`): lines and module, or the case
   that makes it unsound.
7. **Cost**: a spawn and a join whole (10,000 of each), and the gate at exit, base against R, in
   instructions.

## Question 3: any value across, the cost of the robust route

The proposal (`00-shared.md` § The three proposals, repaired after the critic's first reading;
`bw/arms2/proposal-b.diff` and `proposal-m.diff`): `spawn(f, x)` and `spawn_sized(f, x, bytes: n)`, a
`(thread T)` its wait consumes, a `(mailbox T)` that is a **handle with a life** (`mailbox()` begins it, a
close ends it) whose copies reach one queue, **§ 3's address sentence amended** to class it with `ptr` and
`cstr`; every value copied whole; a handle crossing only by moving its life. **Two spellings**: B, seven
built-ins (`spawn`, `spawn_sized`, `wait`, `mailbox`, `send`, `receive`, `close`); M, three built-ins
(`spawn`, `spawn_sized`, `mailbox`) and the four operations written only after a dot (`t.wait()`,
`m.send(v)`, `m.receive(within_ms:)`, `m.close()`), their names free. Where it lands (`wc -l`, `grep
-n`): the built-ins' table `selfhost/inventory.hero` (370 lines; `Builtin(name: "print", tier: .runtime)`
at 115), the emitter's `selfhost/emit/builtins.hero` (391), the types' parser `selfhost/parse/type.hero`
(660; `prefix` at 73, `unit_or_function` at 193), the release descriptors
`selfhost/emit/descriptors.hero` (221), `descriptor_set.hero` (311), `release.hero` (152),
`byte_copy.hero` (120), the checker's `crosses_the_boundary` (`selfhost/check/ffi.hero:79`) and its
handle lives (`consuming.hero` 245, `handle_built.hero` 40); in the runtime, the `str` door's copy
(`spawn_str.c:29-37`), `hero_spawn_start` (`spawn.c:478`), copy-on-write's `refcount == 1` tests
(`runtime/parts/cow.c:44`, `:88`, `:93`), `HeroDesc` (`runtime/heroes_runtime.h:617`).

1. **Build the narrowest slice that runs**: `spawn` and `wait` of one record type holding a `str` and a
   `[str]`, deep-copied by a descriptor at `spawn` (on the spawning thread) and at `wait` (on the worker),
   in your folder; **the type form's arm in `unit_or_function`** (`parse/type.hero:193`): today `(` in a
   type admits only `)` and `function`, so `function take(t: (thread i64))` is
   `error[expected_function_type]` at the frozen commit (`bw/names/p_thread.hero`, and the critic's
   `work/b/t1.hero`), and `thread` and `mailbox` can be contextual words in that one place, the shape of
   `tag`, `partial`, `lent` (`selfhost/parse/marks.hero:32`, *"A contextual word and never a keyword"*,
   re-read); say what the
   `grammar` and `surface` suites and the formatter's probe owe for it. Run the writer's task 2
   reference written for the slice (`bw/t2/ref-x.hero` is the `str`-door version, 5 of 5 right at the
   frozen commit) and a stress of 32 threads each handed a record while `main` rewrites its own, plain and
   `--sanitize`, 20 runs. **Your slice is the blind seat's grader** for task 2 under the proposal where an
   answer stays inside it (`llm-ergonomist.md` § The grading): keep its compiler and say where it is.
2. **The names, priced** (the critic's finding 2, re-run by the writer): a built-in's name is refused
   everywhere, `error[builtin_name_taken]`, declarations, `extern` members and locals alike, and **today's
   method-form built-ins take theirs too** (`lease`, `cstr`, `validated`, `slice`, `must`, `default`,
   `is_err`, `bw/names/`). B takes 124 declarations in 98 files and 30 `extern` members named `close`
   (C's `close(2)`); M takes 3 (`00-shared.md` gives each count and its command). Price B's renames, and
   **M's new kind of name**, one resolved by its receiver's type alone: where `selfhost/resolve/` (the refusal
   `builtin_name_taken` lives in `resolve/state.hero` and `resolve/errors.hero`, `grep -rln`) and the
   checker would hold it, whether *"`x.f(y)` is sugar for `f(x, y)`"* still holds for the four, and what a
   program's own `wait` beside a thread's `t.wait()` reads as.
3. **The mailbox, two shapes priced**: the proposal's (a handle the runtime owns, a lock, a condition, a
   bounded wait, woken by the platform layer's stop, a count of copies or a close) and **design.md 7.13's
   own** (one queue per thread, reached through its `(thread T)`, received inside the body, no value
   aliasing anything; `docs/design.md:4119-4123`), lines per module and instructions per `send`.
4. **What the proposal still needs from the doors**: `selfhost/main.hero` binds `hero_thread_spawn_sized`
   (87), sets `STACK_BYTES` to 268,435,456 (96-97), runs every command on it (101) and exits with its join
   inside `hero_exit` (112): does `spawn_sized` serve it, and **is a program that binds the old doors
   refused or left unnamed** (a refusal is a diagnostic class: say so in those words)? The 38 door files
   are what either moves.
5. **Defect 2582 under Q3**: a worker that prints becomes the common shape once threads are the
   language's own. Price and build a runtime lock that keeps one `print` whole (`panic.c:131-135`,
   `printing.hero:67`: each value then `hero_print_end`'s `putchar` as separate calls), and run sitting
   252's `t2-ref` (`bw/repro/t2-ref.hero`, 0 of 100 torn this morning, 27 of 200 for 252's critic,
   carried) under it, 200 runs.
6. **Say what Q3 does to Q2**: a `(thread T)` is a handle, so does the handle machinery tell an unjoined
   thread with no table, once the gate asks it first?

## What your verdict owes

Per question: built or not, lines per file and the compiler's lines; each case's outcome per image and
level with its run count; what moves (the `run` form narrowed, the `runtime` suite where `runtime/`
moved, `run` whole and `cache` alone where `flags.hero` moved); the cost in instructions where a counter
exists; whether it needs a spec sentence, a new or widened diagnostic, new refusal words or a change a
binding's author must learn, in those words; every premise of these briefs you found false; and a veto,
if you cast one, with its program.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/251-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out. `p251-scratch/ev249/` is read from
`docs/panel/249-reports/evidence.tar.xz`, whose 3,232 members are the 3,232 files named above.
