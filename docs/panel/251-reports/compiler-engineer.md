# Panel 251 (sitting A of M-rebel-library), compiler-engineer's report

Written as the runs land, 2026-11-19, from 10:08 (`date`). Folder
`.claude/worktrees/rebel-scratch/251-compiler-engineer/` (called `ce/` below), a `git archive` of
`3a5ee269117a31780ef1f96d0305452630ac4d83`, 20,022 files (`find -type f | wc -l`, the brief's count),
`.git` the file the brief names. Seed: `clang -I runtime seed/heroes.c runtime/runtime.c`, exit 0,
`real 53.58` (load 7 to 11, other seats building). Stage 2: exit 0, `real 559.68`, `user 303.64`,
`sys 66.70` (waiting, by the ratio), **`sources 23c7d2a626b29fd5`**, the frozen commit's key
(`ce/stage2.log`). Binaries are `ce-heroes` and `ce-heroes-next` (hard links).

## How to read this report

Written in the order the runs landed, so each question's sections are not contiguous. **Q1**: § Question 1
(J built, Linux arm64), § Linux arm64, the follow-up (the stale record and `bjv`), § B'', A and C, § Verdict,
Question 1, then § The emulated x86-64 image and what lands after it. **Q2**: § Question 2 (R, E, robustness,
route C, cost), § Q2.1 and Q2.5 (the census), § Verdict, Question 2. **Q3**: § Question 3 (Q3.1 to Q3.5),
§ Q3's question 6 (inside Q2's part), § Q3.5 addendum, § Verdict, Question 3.

## Status (13:06, final)

- Q1: done in Linux arm64 (`ce/q1/out-arm64/`, `-arm64b/`, `-arm64c/`) and in the emulated x86-64 image
  for the CI's three cases, the 2494 run forms, candidate C and every hole row (`ce/q1/out-amd64a/`,
  `-amd64b/`). **The rest of the x86-64 rows are unrun here, owed to the CI's Linux x86-64 leg after the
  push**, listed with their command under § Verdict, Question 1. Part three (`ce-251-7`) was stopped at
  13:04 on the coordinator's word before it logged a row. Verdict written.
- Q2: done on this Mac; verdict written.
- Q3: done (the runtime half built, the compiler half measured by probes); verdict written.
- Nothing of this seat runs: no container (`docker ps`), no build, no program, no `caffeinate` (`pgrep`,
  13:05).

## Premises of the briefs found false, each run

1. *"J's Linux definitions are three names"* (`compiler-engineer.md` § Question 1): **ten** (`setjmp`,
   `_setjmp`, `__sigsetjmp`, `getcontext`, `longjmp`, `_longjmp`, `siglongjmp`, `__longjmp_chk`, `setcontext`,
   `swapcontext`), beside B''s ten; `nm` of the B'+J object.
2. *"`__builtin_setjmp` / `__builtin_longjmp` by the image's own clang"* as a hole on both images: **clang
   22 refuses `__builtin_longjmp` on aarch64** (*"not supported for the current target"*), so in Linux arm64
   that hole cannot be written; it is x86-64's alone.
3. *"`musttail` ... compiled and never run (carried)"*: now **run** on both Linux images (every injump shape
   goes through J's save points), each ending in a branch under `flags()`' `-fno-optimize-sibling-calls`
   (`br x1` on arm64, `jmp *%rax` on x86-64).
4. The `runtime` suite's state: **red at the frozen commit itself** in a copy (8 passed, 2 failed, the
   platform layer's own lines); no brief says so.
5. Q3's *"deep-copied by a descriptor"* as what makes a crossing safe: **a shallow crossing passed the same
   stress** (32 threads, ASan, TSan), the count being atomic; the deep copy is defence, priced.
6. *"`spawn_sized` serves `main.hero`"*: **only if it answers a refusal**; the compiler's exit 2 on a refused
   thread (`main.hero:101-110`) has no expression in the proposal's words.
7. The holes list (`00-shared.md` § Question 1) misses one: **a stale record**, J's own miss (below).

## Question 2: the end of `main`, built (this Mac, Darwin arm64; written 10:45)

**Route used**: runtime only, through `HEROES_RUNTIME` and the stage 2 above (`sources 23c7d2a626b29fd5`,
unmoved). Every variant is a copy of the frozen `runtime/` with asserted edits by one script,
`ce/q2/patch.py` (each edit asserted to match once): `rt-census`, `rt-R`, `rt-E`, `rt-H`, `rt-P`. Each
passes `clang @flags.txt -fsyntax-only` at `-O0` and `-O2` with no diagnostic. **No compiler line** for R,
E or P.

### Q2.2 and Q2.3: R and E, built, on the writer's repro, 20 runs each (`ce/q2/repro-20.txt`)

R, in code: `spawn.c` writes **state 2 at a thread's end** under the table's lock (after
`slot->result = slot->body(slot->arg)`, `spawn.c:433`); each door records its caller's return address
in the slot; the gate (`alloc.c:306`) **asks the table first, under `hero_spawn_lock`, before the lend
audit and before any balance**, and where any slot is 1 or 2 writes the count and one line per thread,
then `hero_abort()`; the function that started it is named by the stack guard's own reader
(`hero_stack_symbol`, `stack.c:585`; `hero_stack_say_heroes_name`, `:432`). Lines: `spawn.c` +10,
`spawn_str.c` +1, `alloc.c` +43 (the census helpers the script also adds are not R's).
E: the same state 2 and a gate that, finding any slot 1 or 2, writes the count to **stderr** (the spec
names no error stream: said) and returns before any balance, so `main`'s `return 0` exits 0; +13 in
`alloc.c`, +12 for the table read. **E as built counts finished-unjoined threads too**: its sentence
names only *"a thread still running"*, and a finished `str`-door thread left unjoined holds its
answer's block, which the balance would otherwise call a compiler bug.

| program | base | R | E |
|---|---|---|---|
| `unjoined` | 20 exit 134, *"2 heap blocks ... compiler bug"* | 20 exit 134, *"1 thread(s) never joined when `main` returned ... thread 0, still running, started in unjoined.main"* | 20 exit 0, *"1 thread(s) not joined ended with the program when `main` returned"* |
| `unjoined_str` | 20 exit 134, compiler-bug line | 20 exit 134, the R line, `unjoined_str.main` | 20 exit 0, the E line |
| `unjoined_noheap` | 20 exit 0, silent | 20 exit 134, R | 20 exit 0, E |
| `cr-late` | 20 exit 0, silent | 20 exit 134, R | 20 exit 0, E |
| `cr-lease` | 20 exit 134, *"1 lease(s) never ended ... taken at cr-lease.hero:12:16, in cr-lease.busy"* | 20 exit 134, R | 20 exit 0, E |
| `finished_unjoined` | 20 exit 0 | 20 exit 134, R, *"thread 0, finished"* | 20 exit 0, E |
| `exit_running` | 20 exit 0 | **20 exit 0** (`exit` skips the gate, kept) | 20 exit 0, silent |
| `t1` | 20 exit 134, compiler-bug line | 20 exit 134, R, `t1.main` | 20 exit 0, E |
| `t3` (never ends) | 20 exit 0 | 20 exit 134, R | 20 exit 0, E |
| Rebel `bw_unwaited` (running) | 20 exit 134, compiler-bug line | 20 exit 134, R, *"started in h_1rebelthreadsheroos_start"* | 20 exit 0, E |
| Rebel `bw_unwaited_done` | 20 exit 134, *"1 C handle(s) never given back ... lives.hero:18:12"* | 20 exit 134, R, the same symbol | 20 exit 0, E |

R's message, whole (`unjoined`, stderr, exit 134):

```
panic: 1 thread(s) never joined when `main` returned, a thread the program starts is joined before `main` returns, and this program is missing that many
  thread 0, still running, started in unjoined.main
```

E's, whole (stderr, exit 0): `1 thread(s) not joined ended with the program when `main` returned`.

Every outcome is deterministic in 20 of 20. **What R's *"where"* costs**: with no compiler line R names
the Heroes **function** that called the door; a Rebel thread is named by the package's door function,
`h_1rebelthreadsheroos_start`, which the reader does not spell (a module component opening with a digit;
`<rebel>/threads/hero_os.hero`), so a library's wrapper hides the program's call. **A line and column**
need the emitter to hand the door a `HeroPlace`, as every `_at` entry gets one (`heroes_runtime.h:482-495`):
one name-keyed case in `selfhost/emit/handle_traffic.hero`'s shape (about 20 lines) and a `_at` door, or
under Q3 nothing extra, `spawn` being a built-in whose call the emitter already places. Unbuilt.

**The runtime's own threads are not counted, shown**: they never take a spawn slot (each is a bare
`pthread_create`, `00-shared.md` § What moved), and a program that releases a still-running child so
`hero_proc`'s own thread waits for it past `main` (`ce/q2/own/own.hero`) reads **5 of 5 exit 0 under R**,
as under base.

### Q2.4: robustness at the end, 200 runs each (`ce/q2/rb-200.txt`, `ce/q2/rb/`)

Workers in a tight allocate-and-free loop (`rb_alloc`), in `print` (`rb_print`), taking and ending
leases (`rb_lease`), `main` returning after 20 ms. **No exit other than 0 and 134 in 1,800 runs, none
hung** (each under `timeout -s KILL 20`; nothing left running).

| | base | R | E |
|---|---|---|---|
| `rb_alloc` | 200 exit 134, compiler-bug line | 200 exit 134, R | 200 exit 0, E |
| `rb_lease` | 200 exit 134, **compiler-bug line** (the worker's text blocks) | 200 exit 134, R | 200 exit 0, E |
| `rb_print`, output ending mid-line | 14 of 200 | 6 of 200 | **35 of 200** |
| `rb_print`, a torn line inside | 150 of 200 | 128 of 200 | 144 of 200 |

The torn line is `main`'s written into the worker's at a value boundary (`worker line main done,
worker 0`, then `6417 of a thread that never stops`): **defect 2582's shape, under every route**, so it
is Q3's question 5 (below), not R's or E's. E ends mid-line most (its `exit` flushes stdout while the
worker is inside a `print`).

### Q2.6: route C, refused

A compile-time *"every spawn is joined on every path"* is a must-reach analysis; the checker's one flow
analysis, `check/moved.hero` (145 lines), asks the opposite (no read after an end). Today it has nothing
to track: a handle is an `i64` the doors return, which arithmetic and storage erase. Under Q3's
`(thread T)` it would be about the size of `moved.hero` plus the plumbing `consuming.hero` (245) holds,
and **the case that makes it unsound or refusing is the compiler's own**: `selfhost/cli/link.hero:348`
pushes six handles into `handles: [i64]` and joins them in a `for`; a per-variable analysis must either
refuse that correct test or accept *"stored in an array"* as joined, which then passes a program that
joins five of six. R catches that one at run time; C cannot. **Not built; refused.**

### Q2.7: cost, instructions retired (`/usr/bin/time -l`, best of five after the first discarded, load 15 to 60)

| | base | R |
|---|---|---|
| 10,000 spawns, each joined (`ce/q2/cost/spawn10k.hero`, `-O2`) | 1,969,651,252 | 1,957,651,301 |
| no thread at all (`none.hero`, the gate's cost) | 18,605,115 | 18,623,838 |

Both differences are inside the run-to-run spread (about 3.5 % and 4 %): R's extra lock per thread end
and the 256-slot scan at the gate are below this instrument's floor. About 197,000 instructions per
spawn-and-join pair either way.

### Q3's question 6 answered here: the handle machinery, asked first (`rt-H`)

With the gate asking `hero_handle_live` before any block (variant H, +6 lines in `alloc.c`), Rebel's
`threads` (a thread whose start is a handle's life) is told for the **running** thread too: `bw_unwaited`
**20 of 20 exit 134** *"1 C handle(s) never given back — every life a mark began, `acquires` or `retains`,
owes one release ... taken at <rebel>/threads/internal/lives.hero:18:12, in lives.begin"*, where base
said *"compiler bug"* 20 of 20; `bw_unwaited_done` the same. So a `(thread T)` that is a handle needs no
table to be TOLD, once the gate asks handles first, but the words are a C handle's (`acquires`,
`retains`, a program that wrote neither) and the place is where its life began. R's words are a
thread's.

## Question 3: any value across, the cost of the robust route (written 10:55)

### Q3.1: the slice. The runtime half built and run; the compiler half NOT built, measured instead

**Why not built in the compiler, measured**: the slice needs a type `(thread T)`, and a new type is a new
case of the checker's `Ty` (`selfhost/check/table.hero:35`). Added to a copy of the compiler
(`ce/q3/probe/`, one case `thread_ty { result: i64 }`), **`check selfhost/main.hero` reports 288
`error[non_exhaustive]` in 109 files** (`ce/q3/ty-check.txt`): `check/builtins.hero` 15, `check/generics.hero`
12, `check/walk.hero` 10, `emit/*` 30 files, `ir/print.hero` 4, the CLI's `pointee_wants.hero` 5. Each
is a decision (what a thread handle is to `==`, `hash`, `print`, a descriptor, a lend, an assert's
spelling, the C type). `(mailbox T)` is a second such case. **The parser arm alone** (`ce/q3/probe2/`):
`thread` and `mailbox` read as contextual words right after a type's `(`, `parse/type.hero` +12 and
`ast.hero` +6 (two `TypeKind` cases), leaves **24 matches on `TypeKind` non-exhaustive in 12 files**
(`ce/q3/tk-check.txt`): the formatter (`print/types.hero`), the resolver (`resolve/types.hero`),
`check/lower.hero` (which then needs the `Ty` case), `mutate/handles.hero`, two FFI emitters. So a
written `(thread i64)` cannot even reach the checker without the 288. **Core, not sugar** (design.md
§1.7): the checker, the lowering and the backend all match it. What the suites owe for the arm: the
`grammar` suite (the spec's `Prefix` production and `heroes grammar` must both gain
`"(" ( "thread" | "mailbox" ) Type ")"`), `surface` (a fixture per form), and the formatter's probe's
strided pass after the push (`print/types.hero` moves; `.claude/rules/verification.md`'s 2026-11-10 rule).

**The runtime half, built** (`ce/rt-Q3/`, a copy of the frozen runtime): `parts/spawn_value.c` (109
lines: the job, the worker's side, `hero_spawn_value`, `hero_wait_value`, the `str` and `[T]` deep
copies), `HeroDesc` gains a sixth member `deep` (`heroes_runtime.h` +1, **an ABI move**, `HERO_RUNTIME_ABI`
32 at `:108`), `parts/desc.c` +14 (each runtime descriptor's `deep`), `parts/array.c` +2. The argument
is copied whole on the spawning thread before the thread exists, the answer whole on the worker before
it ends, the waiter takes that copy. **C standing where the emitter's output would**: the compiler's own
`--emit-c` of `ce/q3/slice/item.hero` (`record Item { name: str, tags: [str] }`, `work(it: Item) -> Item`),
unchanged, plus what the emitter would add (the record's descriptor with its `deep`, one thunk per
spawned function, 30 lines) and a stress driver (`ce/q3/slice/slice.c`): **32 threads, each handed its own
`Item` of eight tags, while `main` rewrites every one of its own in place 1,000 times; every answer waited
for and checked, the leak gate at the end.**

| build | runs | outcome |
|---|---|---|
| `-O0` | 20 | 20 exit 0, *"right 32 of 32"*, gate silent |
| `-O2` | 20 | 20 exit 0, the same |
| `-O0 -fsanitize=address,undefined` | 20 | 20 exit 0, no report |
| `-O1 -fsanitize=thread` | 5 | 5 exit 0, **0 data races** |
| **control: the record's `deep` replaced by its shallow copy (an incref)** | 20 plain, 5 TSan | **20 exit 0 and 5 with 0 races: the same** |

**The control is the finding**: this stress cannot tell a deep crossing from a shallow one, because the
refcount is `_Atomic int64_t` (`heroes_runtime.h:183`) and copy-on-write never writes a block two counted
references reach. `cow.c:33-43`'s open hazard (*"two threads can both read 1"*) needs two threads on ONE
reference, which share-nothing excludes. So **a shallow crossing (an incref on the sending thread before
the handoff) may already be sound**, with no `deep` member and no ABI move: a question. **The nested
shape, run since** (`ce/q3/slice/gridx.c`, the compiler's own C for `record Grid { name: str, rows: [[str]] }`
with a worker writing `rows[0][0]`, 32 threads each handed a grid of 4 rows of 4 while `main` writes into
its own inner rows 500 times): **shallow 20 of 20 right, ASan 10 of 10 clean, TSan 5 of 5 with 0 races**,
as deep. Still unrun: a map, an answer that still shares a block with the spawner's value. Under the deep route a block is born on one thread and freed on another twice per crossing, by
design.

**The price per crossing, measured** (`ce/q3/slice/cost.c`, spawn and wait of one `Item`, 2,000 rounds,
`-O2`, instructions retired, best of five after the first, `ce/q3/slice/cost-runs.txt`):

| handed over each way | deep | shallow |
|---|---|---|
| nothing (empty name, no tag) | 212,549 | 208,794 |
| 100 tags of 100 bytes | 494,122 | 215,900 |
| 1,000 tags of 100 bytes | 2,979,027 | 282,930 |
| 100 tags of 10,000 bytes | 13,865,486 | 223,987 |

So the deep route costs **about 7 to 14 instructions per byte**, both ways, over a thread's own
~200,000; the shallow one nothing that grows with the value.

**The blind seat's grader**: no compiler of mine accepts `spawn` or `wait`, so task 2 under the proposal
is graded by spec words (`llm-ergonomist.md` § The grading's fallback). Kept for the record:
`ce/q3/slice/` (the C slice) and `ce/rt-Q3/`.

### Q3.2: the names, priced

Re-run at the frozen commit (`git grep -l -E '^function close\(' 3a5ee2691 -- '*.hero'`, vendored copies
out): `close` 77, the brief's counts stand (my copy reads 85, its 8 extra being my two probe copies).
**B**: 124 declarations in 98 files renamed and 30 `extern` members named `close` (C's `close(2)`)
refused, so a binding of `close(2)` under its own name stops compiling; a rename per file, mechanical,
in `packages/rebel/` mostly. **M**: 3 renames (Rebel's `spawn`, `spawn_sized`, `mailbox`), and **a kind
of name the compiler does not have**: the refusal lives in the resolver (`resolve/state.hero:351-355`,
`builtin_names.taken`), which runs before any type is known, so `t.wait()` cannot be resolved there;
the resolver must leave the four names after a dot unresolved and the checker dispatch them by the
receiver's type (priced, unbuilt: resolver about 15 lines, checker about 50 for the dispatch and the
refusal below, a sentence in § 9). **Spec § 9's** *"`x.f(y)` is sugar for `f(x, y)`"* (spec line 291)
**stops holding for the four**: `t.wait()` is not `wait(t)`. **A program's own `wait` beside it**: with
`function wait(t: (thread i64)) -> i64` declared, `t.wait()` read receiver-first is the built-in and the
program's function is never called, in silence; read name-first it is the program's and the built-in is
unreachable through a dot. Either way **one more refusal is owed** (a declared `wait`, `send`, `receive`
or `close` whose first parameter is a thread or a mailbox), **a new diagnostic class**, said in those
words.

### Q3.3: the mailbox, two shapes priced (`ce/q3/slice/mailbox.c`, `mbox.c`)

**The proposal's core, built**: a lock, a condition, a ring of copies, a bounded wait, a close (125
lines, `parts/mailbox.c` in `ce/rt-Q3/`). Two threads, a worker sending 20,000 records and `main`
receiving each: **5 of 5 "right 20000 of 20000", ASan 3 of 3 clean, TSan 3 of 3 with 0 races**. **Not
built, priced**: the handle's life (`hero_handle_acquired_at` at `mailbox()`, a release at the close,
about 15 lines in the emitter's `handle_traffic.hero` shape), the count of copies (an atomic count, about
10), and **the platform layer's stop waking a wait** (a condition wait is not a `poll`; it needs a registry
of mailboxes the stop broadcasts to, or every wait timed and the stop flag read, about 30 lines in the
runtime and a new edge in `hero_net.c`'s stop). **Instructions per send and receive pair**, one thread,
after the 18.6 million an empty program costs: an `i64` **about 904**; a one-tag `Item` deep **about
3,390**, shallow **about 1,039** (best of five after the first).
**Design.md 7.13's own** (one queue per thread, reached through `(thread T)`, received inside the body):
the same core, the queue living in the spawn slot (`spawn.c`'s `HeroSpawnSlot`), **no handle life, no
count of copies, no close, no aliasing value and no § 3 amendment**; its `receive` needs no argument
inside the body, its `send(t, v)` names the thread. About 90 of the 125 lines and the same instructions
per send; what it gives up is a mailbox several threads send to without holding each other's threads.

### Q3.4: what the proposal needs from the doors

`selfhost/main.hero:101-110` asks a sized thread and, **when the system refuses it, says the system's own
number and exits 2**. The proposal's `spawn_sized(f, x, bytes: n)` says nothing of a refusal: if it
panics, the compiler loses that exit-2 contract; **so `spawn_sized` must answer a refusal**
(`(thread T)?`, a failure naming the system's number). With that it serves `main.hero:87-112` (the join
inside `hero_exit` becomes `hero_exit(code: wait(t))`). **A program that binds the old doors**: if
`hero_os.h` stops declaring them, the existing class answers, `error[ffi_unknown_name]` at build
(*"`hero_os.h` declares no `hero_thread_spawn_gone`"*, `ce/q3/doors/`, check exit 0, build exit 1):
**no new diagnostic class**. A program's own header declaring the symbol would still link while the
runtime exports it, so the runtime must stop exporting the doors (they become `static`), and the 38
door files move.

### Q3.5: defect 2582, built (`rt-P`, runtime only, no compiler line)

The first printer of a statement takes stdout's own lock (`flockfile`, recursive) and `hero_print_end`
writes the newline and gives it back: `panic.c` +18, `f64.c` +2, `str.c` +1, since nothing but an emitted
`print` calls them (`printing.hero:67`, `hero_print_end` after every value). **`t2-ref`, 200 runs: base
18 torn, P 0 torn**; `rb_print` (a worker printing without end while `main` returns), 200 runs: base 150
with a torn line and 14 ending mid-line, **P 0 and 0** (`ce/q2/p5/`). Its price in instructions is the addendum below (about 209 per `print`).

### Q2.1 and Q2.5: the census (`ce/q2/census.sh`, `ce/q2/census/census.txt`, 1,461 lines; written 10:50)

`rt-census` writes state 2 at a thread's end and appends to the file `HERO_CENSUS` names: at the gate and
in `hero_exit` the slots running and finished-unjoined and the calling thread's own block balance; at
each spawned thread's very end its slot and its **own balance** (blocks born on it less blocks freed on
it, a `_Thread_local` moved beside `hero_live_blocks` in `hero_alloc_block` and `hero_release_block`,
`alloc.c:673`, `:679`). Its outputs are byte-identical to base's: the `run` form over it reads
**`thread` 18 passed, `fixedbugs-2277` 1, `fixedbugs-396` 15, `fixedbugs-417` 1, `the-platform-layer-ends`
4, `-runs-the-leak` 1, `-stops` 3, 0 failed** (those seven substrings cover the twenty `run` goldens that
start a thread); the eleven door `examples/` each **exit 0 and SAME** as `main.expected`; Rebel's
`reach_threads` exit 0; the compiler built over it (`sources 69fffe564a2af430`, its runtime's key) built
itself (exit 0, `real 185.90`) and checked itself (exit 0).

**Q2.1: no program left a thread unjoined at the end of `main`**: 134 gate lines, every one *"0 running,
0 finished"*. So **R moves none of these programs**; it moves only the writer's repro and Rebel's
two. The compiler ends through `hero_exit` after joining its command thread (`selfhost/main.hero:112`):
0 and 0 on `build` and on `check`. **But its `--version` ends by `exit` from INSIDE the command thread**
(the census line *"exit 1 0 7"*: one thread running, the caller itself, 7 of its blocks never freed),
the path R and E both leave alone, `exit` ending every thread with the process. Not run: the 22 Rebel
callers of `threads.spawn` beyond `reach_threads` (the brief named that one).

**Q2.5: every counted block was born and freed on one thread**: 1,307 thread ends over these programs
and the compiler's two, **every own balance 0**. No block crossed. (Q3's deep route makes blocks cross
twice per spawn by design: below.)

## Question 1: route J into B' on Linux (written 10:55, Linux arm64 first)

### J, built (`ce/routes/bj/`, laid over `bw/routes/bprime/`, whose bytes the manifest names)

- **`runtime/parts/sigjump.c`, new, 213 lines**, `#if !defined(_WIN32) && !defined(__APPLE__)`: the
  runtime defines **ten names**, the save points `setjmp`, `_setjmp`, `__sigsetjmp`, `getcontext` and the
  jumps `longjmp`, `_longjmp`, `siglongjmp`, `__longjmp_chk`, `setcontext`, `swapcontext` (`sigsetjmp` is
  glibc's macro for `__sigsetjmp`; `makecontext` is variadic and not defined, named below). **The brief's
  *"three names"* for the escape is false of the build: J's Linux definitions are ten, B''s ten beside
  them** (`nm` of the B'+J runtime object, `ce/q1/out-arm64/q1-arm64.log` § 0, twenty names).
- **The record lives in the thread's `HeroSignalStack`** (`parts/sigguard.c`, +9 lines:
  `HERO_SIG_JUMPS 32` pairs of a buffer's address and the frame of the call that filled it, 512 bytes and
  a counter per claimed thread). A buffer filled again takes its pair back; **a full record overwrites its
  oldest pair**, so a jump to a buffer older than 32 others has no pair. `runtime/runtime.c` +5 (the
  include after `parts/siginstall.c`). **The compiler: 0 lines.**
- **A save point** looks the thread up (`pthread_getspecific`, no lock), writes its pair with a signal
  fence between the address and the frame, and **branches** to the C library's own by
  `__attribute__((musttail))`, so glibc saves the caller's registers and return address: in Linux arm64
  under `flags()` (which carries `-fno-optimize-sibling-calls`) each save point ends `br x1` (`br x2` for
  `__sigsetjmp`) at `-O0` and at `-O2` (`objdump -d` of the runtime object, § 0), **compiled and run**
  (every injump shape below goes through it).
- **A jump** finds its buffer's pair and drops, from the top, the entries B''s own prune proves left
  (`hero_sig_prune`, `siginstall.c:113-122`): **deeper on the target's own stack, or on an alternate stack
  while the target is on the thread's own**; then calls the C library's jump (`__asan_handle_no_return()`
  first under ASan, the call ASan's own interceptor makes, since J's strong definition displaces ASan's
  weak one). **A jump to a buffer with no pair keeps every entry** (the proposal's rule); built the other
  way as `-DHERO_JMP_MISS_DROPS` (variant `bjd`).
- **The C library's own pointers are resolved before `main`** by a constructor beside B''s
  (`hero_jmp_find_early`, `dlsym(RTLD_NEXT, ...)` once each); a save point reached before that constructor
  (a shared library's constructor) resolves them there, once: **a `dlsym` outside `main`'s time, never in a
  handler after it**, said as the one residue of resolution.
- J differs from the ffi-pragmatist's Darwin build in WHAT it records: **the frame** here (glibc's saved
  SP is mangled, but the save point's own frame is readable), **the innermost trampoline entry** there
  (Darwin's is mangled and signed, `ffi-pragmatist.md` § 1.2). On one stack the two rules drop the same
  entries; the entry rule needs no address and is writable on Linux too (the depth index at the save),
  so **J can be one rule on both**: the entry rule.

### Linux arm64 (`heroes-linux-arm64:latest`, glibc 2.41, clang 22.1.8; stage 2 `sources 23c7d2a626b29fd5`)

Base, B' and B'+J side by side (and `bjd`, J with a miss dropping every entry), each built `-O0` and
`-O2`, base 3 runs and each route 20 (`ce/q1/out-arm64/q1-arm64.log`):

| must be refused (exit 134, R1's words) | base | B' | B'+J | bjd |
|---|---|---|---|---|
| `injump` (`ij.c`, 249's), both levels | exit 0, inside | 20/20 | **20/20** | 20/20 |
| its shapes `j1`, `j2`, `j3` (`ijv.c` HOW 1, 2, 3) | exit 0 | 20/20 each | **20/20 each** | 20/20 |
| fortified `j0f`, `j1f` (`__longjmp_chk`) | exit 0 | 20/20 | **20/20** | 20/20 |
| `injump` with `SA_ONSTACK` (`ijo.c`, new) | 3/3 refused | 20/20 | **20/20** | 20/20 |
| the control `jn` (a handler calling back directly) | exit 0 | 20/20 | **20/20** | 20/20 |
| `kmiss` | exit 0 (20/20 inside) | 20/20 | **20/20** | 20/20 |
| 247's `flib` static, and shared through `LD_LIBRARY_PATH` | exit 0 | 20/20, 20/20 | **20/20, 20/20** | — |

| must not be refused | base | B' | B'+J | bjd |
|---|---|---|---|---|
| the twelve depth programs, 3 runs each per level | 72/72 exit 0 | **`dp-0-0-ds`, `dp-1-0-ds`, `dp-2-0-ds` at `-O0`: 3/3 refused** (249's row, re-run); the rest exit 0 | **72/72 exit 0** | 72/72 |

**So on arm64 J removes B''s one false refusal and keeps every refusal**, at both levels and under the
miss rule either way (every buffer here had a pair). **`--sanitize`** (`-O2` and `-O0`, 3 runs each):
B'+J refuses `flib` and `injump` and runs `dp-0-0-ds`, where B' refuses `dp-0-0-ds` at `-O0` under ASan
too. My ASan program for J's displaced interceptor (`aj.hero`: a jump out of forty sanitised frames from
a library never sanitised, then the stack used again) ran clean under every runtime **and under J with
the ASan call taken out** (`bjna`): with clang 22's default fake stacks it has no teeth; re-run without
them in the follow-up.

**The forked child, reached in code** (`hero_proc_spawn_enters` forced false, each child saying `F` on
stderr; `ce/q1/ship/holes/fk/`): **200 of 200 children through `hero_proc_child`, 3 runs each, under
base, B' and B'+J, exit 0**, while a second thread opened and closed `libm` and allocated the whole time.
**B'+J's `sigaction` takes no lock**: B''s definition loads an atomic pointer resolved before `main`
and stores two atomics (`siginstall.c:168-207`); J adds nothing to it. So it is async-signal-safe in a
forked child, by reading and by those 600 children.

**The runtime's own calls under B'+J**: `run.c:1805` (`SIG_IGN` in a forked child) takes B''s
non-handler path, the C library's own call and an atomic store; `stack.c:1220`'s install goes through
B''s definition, so **the stack guard's own handler runs inside B''s wrapper with a depth entry**, and the
prior it calls (`stack.c:743-746`) is the program's handler as the table answers it, run inside that
entry; `os.c:392` the same for the lease handler's prior; `hero_proc.c:1630`, `hero_term.c:258`, `:278`
install through B' (the terminal member's handlers wrapped); the reads `hero_proc.c:1628`,
`hero_term.c:247`, `:272` answer the table's handler. **J changes none of these**: the runtime calls no
save point and no jump (`grep`: two comments, `stack.c:810`, `os.c:451`). The 2558 and recovery goldens
below are where a prior that jumps is exercised.

**The escape, run** (`ce/q1/ship/holes/esc/`, a library defining `longjmp` beside a function the program
binds): **base and B' build it and run it** (exit 0; the library's `longjmp` replaces the C library's
for the whole program, in silence); **B'+J refuses it, `error[ffi_defined_elsewhere]`, exit 1**: *"library
`esc`, which module `esc` links, defines `longjmp`, a name of the program's own runtime, and a program
holds one `longjmp`"*, both definitions named. **No compiler line**: panel 239's member reads the link's
own duplicate, so it widens by data to every name J defines. Built shared, the library is not judged and
builds under all three. **The widening is a ruling, not a build**: the class's words are already true
of it; which programs it refuses that built before (a library shipping its own `setjmp` family, as
libunwind's setjmp library does, carried from memory: a question for the historian) is unmeasured in the
tree.

### Q3.5 addendum: the print lock's price

`ce/q2/p5/lines.hero` (100,000 `print`s of three values, one thread, `-O2`), instructions retired, best
of five after the first: base 1,372,613,462, P 1,393,534,216: **about 209 instructions per `print`**,
1.5 % of a print's own 13,726.

## Verdict, Question 2

- `verdict`: **approve R** (and object to E as the conservative)
- `section`: design.md §1.12 (robustness: a false *"this is a compiler bug"* is the program misled), §1.1's
  ceiling (no compiler line), Part 5 (no construct)
- `implementation_cost`: runtime only, measured in `ce/rt-R/`: `runtime/parts/spawn.c` (664) +10
  (state 2 at `:433`, the door's caller in the slot), `spawn_str.c` (107) +1, `alloc.c` (1,585) +43 (the
  table asked first at `hero_runtime_check_leaks`, `:306`, the message naming each thread). **0 compiler
  lines**; a line and column for each thread: about 20 in `selfhost/emit/handle_traffic.hero`'s shape
  and a `_at` door, unbuilt (or none under Q3, `spawn` being a built-in the emitter places). Spec: +5
  tokens (`bw/arms2/q2-r.diff`).
- `needed_for_self_hosting`: **no** (the compiler joins its one thread before `hero_exit`, census 0 and 0)
- `argument`: R makes the gate's premise (`alloc.c:79-80`) true by asking the table before any balance:
  11 programs, 20 runs each, every unjoined thread told as the program's, 20 of 20, where base told two
  false compiler bugs and one false lease. It moves no program of the tree (134 gates, 0 unjoined), costs
  nothing measurable (inside the spread at 10,000 spawns and at an empty gate), and leaves `exit` and
  the runtime's own threads alone (both shown). E keeps the program running to exit 0 but turns the leak
  gate off for any program that leaves a thread, and ends mid-line most often (35 of 200).
- `prediction`: with R landed, `run` over `tests/golden/run/` reads the same count as without it on this
  Mac and Linux arm64 at M-rebel-library's close (no tracked program leaves a thread unjoined), and
  `heroes test selfhost/main.hero` reads the same failures as without it (unrun here: no seat runs it).
  (Measured since, `ce/rt-RP`, R with the print lock: `rb_print` 200 of 200 exit 134 with R's line,
  every output ending whole, no foreign line.)
- `condition`: a tracked program found leaving a thread unjoined at `main`'s return on some path the
  census did not take (the 22 Rebel callers unrun), or R's message costing more than the spread on the
  10,000-spawn program on Linux, would make me ask R's words before adopting it.

## Verdict, Question 3

- `verdict`: **object**
- `section`: design.md §1.7 and Part 5 (*"Only these need implementing in the type checker and the
  lowering and the backend"*), Principle 0 (CLAUDE.md § 2), and Part 7 item 13's own *"the first and
  probably only rung"*
- `implementation_cost`: **each type form is a new core case**: one `Ty` case leaves 288 matches
  non-exhaustive in 109 files (`ce/q3/ty-check.txt`), the parser arm alone 24 more in 12
  (`parse/type.hero` 660 +12, `ast.hero` +6); two forms (`thread`, `mailbox`). Built-ins in
  `inventory.hero` (370) and the emitter's `emit/builtins.hero` (391); a thunk per spawned function; the
  deep route adds a sixth `HeroDesc` member (`heroes_runtime.h:617`, an ABI move) and a generated `deep`
  per reachable type in `emit/descriptors.hero` (221) and `descriptor_set.hero` (311); runtime
  `spawn_value.c` 109 and `mailbox.c` 125 built, about 55 more priced (handle life, copies, the stop's
  wake). B: 124 renames in 98 files and 30 `extern` `close` bindings refused; M: a name kind the resolver
  cannot hold (`resolve/state.hero:351-355`), checker dispatch, and a new refusal class for a program's own
  `wait`.
- `needed_for_self_hosting`: **no**: the compiler needs one sized thread returning an `i64`
  (`selfhost/main.hero:87-112`), which today's door gives, and the proposal's `spawn_sized` must answer a
  refusal to keep its exit 2
- `argument`: The robust route adds two type constructors to the core, the class of change §1.7 says
  sets the compiler's size, for a capability the compiler does not need. Its most expensive half is not
  what makes it safe: a shallow crossing passed the same 32-thread stress, ASan and TSan, because the
  count is already atomic (`heroes_runtime.h:183`); the deep copy costs 7 to 14 instructions a byte. Land
  R and the print lock now (runtime only, measured); take the type forms when a program the closure needs
  asks for them.
- `prediction`: adding `(thread T)` to `selfhost/` as a `Ty` case will touch at least 100 of
  `selfhost/`'s files in its landing commit (`git show --stat`), checkable at the milestone that lands it.
- `condition`: a design that reaches `(thread T)` without a `Ty` case (the slot index an `i64` the checker
  tags, measured to leave under 30 matches non-exhaustive), or a program on the closure list that needs a
  record across a thread, would move me to approve, the shallow crossing first.

**Storms** (`fliblocale`, `-O2`, the 247 driver, `ce/q1/out-arm64/q1-arm64.log` § 3): **B' 20 of 20 and
B'+J 20 of 20 armed and ended by R1's refusal (SIGABRT)**; base 3 of 3 armed, exit 0, the Heroes function
run inside the signal.

**The goldens, with my stage 2 over each runtime** (`./heroes-next run tests/harness/main.hero --
./heroes-next run <group>`, `HEROES_RUNTIME` set, Linux arm64): base `fixedbugs-2558` 5 passed,
`recovers-leaves-no-line` 1, `fixedbugs-2494` **15 passed**; **B'+J `fixedbugs-2558` 5 passed,
`recovers-leaves-no-line` 1 passed, `fixedbugs-2494` 10 passed, 5 failed**. **Every move named**, and
the direct judge (each case built and run at `-O0` and `-O2` under base, B' and B'+J) shows **every one is
B''s: in no row do B' and B'+J differ**. Each stays exit 134, in other words:

| golden (`tests/golden/run/fixedbugs-2494-...`) | the golden says | B' and B'+J say |
|---|---|---|
| `a-handler-that-allocates-inside-the-allocator-stops-its-program-by-name` | D's *"a signal handler used this runtime's allocator while the code it interrupted was inside it"* | R1's *"on_signal was called from inside a signal handler: C called it"* |
| `a-handler-that-lends-inside-the-lend-tracking-stops-its-program-by-name` | D's lend words | R1's |
| `a-heroes-function-given-to-signal-is-refused-at-its-first-signal` | *"ran as a signal handler: the system called it"* | *"was called from inside a signal handler: C called it"* |
| `a-heroes-function-the-programs-own-sigaction-installs-is-refused` | the same | the same move |
| `a-heroes-function-with-siginfo-the-programs-own-sigaction-installs-is-refused` | the same | the same move |

The last three are **false of the program under B'**: the program installed its Heroes function itself
and the system delivered the signal; B''s wrapper is the C that calls it now. The thunk can tell (its
return address is the wrapper's), so the words are owed by B', not by J: **a change a reader of these
messages must learn**, said. The first two lose D's more exact sentence to R1's earlier one.

### Linux arm64, the follow-up (`ce/q1/out-arm64b/q1-arm64-run2.log`, `ce/q1/ship/run2.sh`; written 11:02)

Three of my own test defects repaired (the deep library's header was not on the include path; one
header held all four hole modes and **clang 22 refuses `__builtin_longjmp` on aarch64**, *"not supported
for the current target"*, so on arm64 the inline pair cannot exist and modes 3 and 4 are x86-64's; the
ASan program re-run without fake stacks), and two cases added: **the stale record**, and **`bjv`, J
with each record validated**, built after reading the stale case (below).

| case | base | B' | B'+J | J, a miss drops (`bjd`) | J validated (`bjv`) |
|---|---|---|---|---|---|
| the deep library's program (`deep.hero`, twelve frames with records), `-O0` and `-O2`, 20 each | **exit 0, inside**, 40/40 | refused 40/40 | **refused 40/40** | — | — |
| `kmiss`, both levels, 20 each | inside | refused | refused | refused | **refused 40/40** |
| a buffer copied by `memcpy`, a jump OUT of the handler, then a correct deep callback (`holes.h` mode 1), 5 each | runs | `-O0`: **refused, 5/5**; `-O2` runs | `-O0`: **refused, 5/5** (no record: every entry kept); `-O2` runs | runs, both levels | `-O0`: **refused, 5/5**; `-O2` runs |
| the same copy, a jump WITHIN the handler, then the callback (mode 2) | refused (the walk, the handler having a frame record) | refused | refused | refused | refused |
| **the stale record** (`ij/ijstale.c`, below), both levels, 20 each | **inside, 40/40** | **refused 40/40** | **INSIDE, exit 0, 40/40** | **inside, 40/40** | **refused 40/40** |
| a handler left by `pthread_exit`, and by a cancellation, on a C thread, then a deep callback on `main`, 5 each | 10/10 exit 0 | 10/10 | 10/10 | — | — |
| `aj` (a jump out of forty sanitised frames from an unsanitised library, then the stack used), `--sanitize` `-O2` and `-O0`, with and without fake stacks, 3 each | clean | clean | clean | — | clean **with the ASan call taken out too**: this program cannot see the call's absence; the call stays, as ASan's own interceptor makes it |

**The stale record is J's own miss, measured**: a global buffer filled once by a save point J sees, at a
shallow frame that returns (`ij_prime`, called from `ij_install`); later the handler fills a buffer of
its own, copies it into the global one with `memcpy`, and a helper jumps to the global one. The jump
stays INSIDE the handler; J holds only the old shallow frame for that address, so it drops the handler's
live entry; the handler then reaches the program through twelve frames, past the walk's eight. **B'
alone refuses it 40 of 40; B'+J runs the Heroes function inside the signal, exit 0, 40 of 40.** J as the
proposal writes it (*"drops only the depth entries whose wrapper frame lies deeper than its target's"*)
trusts a record the program can have overwritten. **The repair, built and run** (`ce/routes/bjv/`,
`sigjump.c` 236 lines, `sigguard.c` +4 more): each pair also keeps a fingerprint of its buffer's bytes,
read at the thread's next save point or jump (when the C library has filled it and returned); at a jump, a
pair whose buffer's bytes differ from its fingerprint is no record, so every entry is kept. **40 of 40
refused.** A pair still unread at its own jump is trusted as before: the residue is a buffer refilled,
between its save and its jump, from a save J never saw (the inline pair on x86-64, the C library's own),
with no other J event between. Both build clean under `flags()` in Linux arm64 at `-O0` and `-O2`.

**The keep rule's price, measured on Linux too**: the `memcpy`'d jump OUT of a handler is refused at
`-O0`, 5 of 5, under B'+J and `bjv`, a correct program told R1's words, which are false of it (the
ffi-pragmatist's same row on Darwin, 40 of 40); the drop rule runs it, and runs Heroes code inside a
signal in the frameless library's within-shape (its row). Mode 2's within-jump is refused under every
runtime here because the header's handler keeps a frame record and the walk finds the wrapper.

**The `runtime` suite** (`ce/q1/runtime-suite/`, each variant's `runtime/` in an APFS clone of my copy):
**base, B'+J, R and P each read 8 passed, 2 failed, 4 floors outgrown**, and the two failures
(`runtime/alloc`, `runtime/threads`) name **the same 114 lines in all four** (`diff`, empty). **A premise
the brief does not state: the suite is red at the frozen commit itself**, on the platform layer's own
allocation calls (`hero_keyring.c:1507`, `hero_sys.c:1897-1915`) and shared state (`hero_crypto.c:963-972`).
No variant adds an offender.

### B'', A and C: only where J leaves them a row

- **B''** (`bw/routes/bsecond/`, 339 lines) exists for B''s one false refusal, the depth row. **On arm64
  J leaves it no row**: B'+J runs the twelve depth programs 72 of 72 where B' refuses three at `-O0`.
  Not run; its owed rows (the `runtime` suite under it, 249's rows (4), (5), (7)) stay owed and need not
  be paid unless J is refused.
- **A** keeps no frame of its own around a handler: its marker rides the handler's mask
  (`a-sigrtmax/.../siginstall.c:63-67`) and its jumps release the marker before jumping (`:104-130`). **J
  has nothing to compare under A**; given a wrapper's frame to compare, A is B' with a marker. Its built
  form is the vetoed `SIGRTMAX` one (249's veto stands). Not built, not run; argued from the code.
- **C** (`bw/routes/c-word/`, `flags.hero` 584 to 602): its stage 2 and the three CI cases are in the
  emulated run (below). **`run` whole and `cache` alone over C's stage 2 are owed**: each moves every
  unit's key (`.claude/rules/verification.md`'s `flags.hero` row) and costs a full `run` form, which this
  sitting's load (15 to 65 on this Mac all morning) made me spend on J instead. Instructions on x86-64:
  neither image has a counter (`/usr/bin/time` absent, **carried** from 249's own logs,
  `ev249/249-compiler-engineer/linux/bp-arm64.log:2` and `amd64/base.log:3`, not run again here), owed.

**`bjv` through the whole arm64 set** (`ce/q1/out-arm64c/q1-arm64c-run2.log`, sections 1 and 4): the eight
injump shapes (`jn`, `j0`, `j0f`, `j1`, `j1f`, `j2`, `j3`, `jo`) **refused 20 of 20 at `-O0` and at `-O2`**,
the twelve depth programs **exit 0, 3 of 3 at each level**; with the follow-up's `kmiss` 40/40 and the stale
record 40/40 refused. So on arm64 **`bjv` keeps every result of B'+J and closes J's own miss.**

**Q3.1 addendum**: the writer's task-2 reference at the frozen commit (`bw/t2/ref-x.hero`, the `str`
door), run by my stage 2: **5 of 5 exit 0, one output** (`md5` equal, 4 lines), the carried 5 of 5 holds.
Under the proposal it has no compiler to run on (§ Q3.1 above).

## Verdict, Question 1 (Linux arm64 and the emulated x86-64 image; Darwin is the ffi-pragmatist's)

- `verdict`: **veto** of route J **as the proposal words it** (a record trusted as found), cast with its
  program, built and run on both images: `ce/q1/ship/ij/ijstale.c` bound by `ij.hero`. **Lifted by J
  validated** (`ce/routes/bjv/`), which I approve into B' on Linux.
- `section`: design.md §1.12 (robustness; a boundary complete and defended)
- `implementation_cost`: runtime only. `runtime/parts/sigjump.c` new, **236 lines** validated (213
  unvalidated); `runtime/parts/sigguard.c` (371; B''s 384) **+14** for the record in `HeroSignalStack`
  (32 pairs of address, frame, fingerprint, size, about 800 bytes per claimed thread); `runtime/runtime.c`
  **+5**. **Compiler: 0 lines. Spec: 0 tokens.** `ffi_defined_elsewhere` widens by data to J's ten names,
  no compiler line (run). B''s own `siginstall.c`, 311, unchanged.
- `needed_for_self_hosting`: **no**
- `argument`: J as written trusts a record the program can overwrite. `ijstale.c` refills a recorded
  buffer by `memcpy` inside its handler and jumps to it: B' alone refuses it 80 of 80 over both images;
  **B'+J runs the Heroes function inside the signal 80 of 80**, defect 2522 itself, and so does Darwin's
  entry rule. Fingerprinting each buffer once the C library has filled it refuses it 80 of 80 and keeps
  every arm64 row: all injump shapes, `kmiss`, `flib`, the deep library, the storms refused, the twelve
  depth programs run where B' refused three. In the emulated image the CI's three reds turn green (C
  leaves two red). No compiler line. The x86-64 injump and depth rows are owed.
- `prediction`: with `bjv` and 2522's repair landed, `ijstale.c` kept as a `run` golden is refused at
  `-O0` and `-O2` on the CI's Linux arm64 and x86-64 legs at M-rebel-library's close, and the CI's Linux
  x86-64 `run:` line moves from 3 failed to exactly the five B' word goldens until B' gives them their
  words (the move measured identical on three platforms).
- `condition`: I lift the veto on J unvalidated only for a run of `ijstale.c` refused under it; and I
  would prefer a record-free J (the target's frame read from the buffer itself, glibc's saved stack pointer
  demangled by its pointer guard) if a seat builds it and it refuses the stale and `memcpy`-within shapes
  without refusing the `memcpy`-out one.

**The emulated x86-64 rows NOT run, each unrun here, owed to the CI's Linux x86-64 leg after the push.**
Stopped at 13:04 on the coordinator's word, before its first row (`ce-251-7`, part three, had logged none).
Each runs with the frozen tree on stdin and my case folder mounted, from the repository's root (`F` is
`.claude/worktrees/rebel-scratch/251-compiler-engineer`); the script and every case are in `$F/q1/ship/`:

```
git -C .claude/worktrees/rebel-round archive 3a5ee269117a31780ef1f96d0305452630ac4d83 | timeout -s KILL 14400 \
  docker run -i --rm --name ce-251-8 -e TAG=amd64x -e SECTIONS="<sections>" -e RUNS=20 -e STORMS=5 -e DEPTHRUNS=2 \
  -e RTS="base bp bj bjv" -v $F/q1/ship:/ship:ro -v $F/q1/out-amd64x:/out heroes-linux:latest bash /ship/run3.sh
```

| row owed on x86-64 | `<sections>` |
|---|---|
| the injump family (`jn`, `j0`, `j0f`, `j1`, `j1f`, `j2`, `j3`, `jo`), base, B', B'+J, `bjv`, `-O0` and `-O2`, 20 runs each | `1` |
| `kmiss` and the deep library's program, 20 runs each | `2` |
| 247's `flib`, static and shared, 20 each; `fliblocale` storms, 5 armed | `3` |
| the twelve depth programs, both levels (B''s false refusal and J's repair of it, on x86-64) | `4` |
| `--sanitize` and `--sanitize -O0` on `flib`, `injump`, `dp-0-0-ds`, `aj` | `6` |
| `hero_proc`'s forked child with the fork path forced | `7` |
| the escape (a library defining `longjmp`, static and shared) | `8` |
| the 2558 and recovery goldens judged directly per level, and their run forms | `9` |
| candidate C's whole 2494 run form (its three cases ran, above) | `9q` with `-e WORD=1` |

On the CI's own leg, the goldens' share of these is the net's command over a stage 2 whose runtime carries
the route: `HEROES_RUNTIME=<route's runtime> ./heroes-next run tests/harness/main.hero -- ./heroes-next run
fixedbugs-2558` (and `recovers-leaves-no-line`, `fixedbugs-2494`). Candidate C's `run` whole and `cache`
alone stay owed as written above. What the emulated image DID run: the CI's three cases at each level
under base, B', B'+J and `bjv`; the 2494 run form under base and B'+J; C's stage 2 and its three cases;
J's save points' disassembly; and every hole row (modes 1 to 4, the stale record under six runtimes, the
thread exits).

### The emulated x86-64 image (`heroes-linux:latest`, glibc 2.41, clang 22.1.8, emulated; `ce/q1/out-amd64a/`; written 11:28)

Stage 2 `exit 0` at 754 s; B', B'+J, `bjd` and `bjv` each pass `clang @flags.txt -fsyntax-only` at `-O0`
and `-O2` here too. **The CI's three reds** (run 43769808533, job 132982964177, the three 2494 cases at
`-O2`), each built at each level and judged against its `.expected`:

| `tests/golden/run/fixedbugs-2494-...` | base | B' | B'+J | `bjv` |
|---|---|---|---|---|
| `a-heroes-atexit-function-a-c-handler-reaches-through-exit-is-refused` | **DIFF, exit 0, `-O0` and `-O2`** (*"the atexit function ran: 42"*) | SAME, 134, both | **SAME, 134, both** | SAME, both |
| `a-heroes-function-a-c-handler-calls-is-refused` | SAME `-O0`; **DIFF, exit 0, `-O2`** (*"the answer ran for signal 10"*) | SAME, both | **SAME, both** | SAME, both |
| `a-heroes-function-a-c-handler-reaches-through-fork-is-refused` | **DIFF, exit 0, both** (*"prepare ran: 42"*) | SAME, both | **SAME, both** | SAME, both |

**So the emulated image reproduces the CI's reds under base, and B'+J turns all three green at both
levels.** It is B''s wrapper that does it (B' alone reads the same); J changes nothing here. That the
emulated image is not the CI's leg (`00-shared.md` § Where each seat works) stands.

**The run form in the emulated image** (`./heroes-next run tests/harness/main.hero -- ./heroes-next run
fixedbugs-2494`, `HEROES_RUNTIME` set): **base 12 passed, 3 failed**, the CI's own three
(`...atexit...` at `-O0`, `...calls-is-refused` at `-O2`, `...through-fork...` at `-O0`; the CI read `run: 830
passed, 3 failed`, the same three at `-O2`, carried from the brief).
**B'+J 10 passed, 5 failed**: the CI's three gone, and the five failures are **exactly the five word moves
named in Linux arm64 and on Darwin** (`a-handler-that-allocates...`, `a-handler-that-lends...`,
`...given-to-signal...`, `...the-programs-own-sigaction...`, `...with-siginfo...`, each at `-O0`). So on three
platforms the 2494 count moves by those five and no other.

**Candidate C in the emulated image** (`bw/routes/c-word/` laid over the tree, its own seed and stage 2,
`sources 76206cef51dc949c`, the three CI cases built at each level by C's stage 2 over the frozen runtime
plus C's `sigguard.c` and `hero_compiler.h`):

| `fixedbugs-2494-...` | C `-O0` | C `-O2` |
|---|---|---|
| `...atexit-function-a-c-handler-reaches-through-exit-is-refused` | **DIFF, exit 0**, *"the atexit function ran: 42"* | **DIFF, exit 0** |
| `...a-c-handler-calls-is-refused` | SAME, 134 | **SAME, 134** (green where base is red) |
| `...a-c-handler-reaches-through-fork-is-refused` | **DIFF, exit 0**, *"prepare ran: 42"* | **DIFF, exit 0** |

**So C turns one of the CI's three green and leaves two running Heroes code inside the handler at both
levels; B'+J turns all three green.** C's 2494 run form was stopped before it ran (the direct rows answer
the question; its `run` whole and `cache` alone stay owed, above).

**J's save points on x86-64** (`ce/q1/out-amd64b/q1-amd64b-run3.log` § 0, `objdump -d` of the B'+J
runtime object under `flags()`): `setjmp`, `_setjmp`, `getcontext` end in **`jmp *%rax`** at `-O0` (18
instructions) and `-O2` (16), `__sigsetjmp` the same (20, 18): **a branch on both architectures**, so the
C library's own save records the caller. The same twenty names defined. B', B'+J, `bjd`, `bjv` and `bje`
(below) each pass `-fsyntax-only` at both levels here.

**J's holes on x86-64** (`ce/q1/out-amd64b/q1-amd64b-run3.log` § 5; `bje` is **the entry rule**, the
ffi-pragmatist's Darwin rule written for Linux: a save records how many entries were live, a jump restores
at most that many, `ce/routes/bje/`):

| case | base | B' | B'+J | `bjd` | `bjv` | `bje` |
|---|---|---|---|---|---|---|
| `memcpy`'d buffer, jump OUT, then a correct deep callback (mode 1), 5 each | runs | `-O0` refused, `-O2` runs | `-O0` **refused**, `-O2` runs | runs | `-O0` **refused**, `-O2` runs | `-O0` **refused**, `-O2` runs |
| `memcpy`'d buffer, jump WITHIN (mode 2) | refused | refused | refused | refused | refused | refused |
| **the inline pair**, `__builtin_longjmp` OUT, then a correct deep callback (mode 3) | runs | `-O0` **refused**, `-O2` runs | `-O0` **refused**, `-O2` runs | `-O0` **refused** | `-O0` **refused** | `-O0` **refused** |
| the inline pair WITHIN (mode 4) | refused | refused | refused | refused | refused | refused |
| **the stale record**, 20 each per level | **inside**, 40/40 | **refused** 40/40 | **INSIDE**, 40/40 | **inside**, 40/40 | **refused** 40/40 | **INSIDE**, 40/40 |
| a handler left by `pthread_exit` and by a cancel on a C thread, then a deep callback on `main` | 10/10 runs | 10/10 | 10/10 | — | — | — |

So on x86-64 as on arm64: **the stale record is J's miss under the frame rule and under the entry rule
alike** (the entry rule misses it 40 of 40 here), **and only the validated record refuses it**; the
inline pair, which calls no jump, leaves B''s stale entry and refuses a correct program at `-O0` under
every J (the error goes toward refusal, never inside); the keep rule refuses the `memcpy`'d jump-out at
`-O0` (a correct program told R1's false words), the drop rule runs it. **J is one rule on both
platforms only if the rule is validated**: Darwin's entry record (`ffi-pragmatist.md` § 1.2) is read by a
jump to a buffer the program refilled exactly as mine is, so its D-rebind+J owes the same fingerprint
(unrun on Darwin by me: a question for that seat).

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/251-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
