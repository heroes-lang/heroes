# Panel 258, the compiler-engineer's report (2026-11-24)

Seat: compiler-engineer, soundness lane. Base `a23ea157a`, own worktree
`.claude/worktrees/258-compiler-engineer` (it holds no commit; its `.ce/` folder holds every program, edited
unit and reading below), own compilers: stage 1 from the seed, stage 2 from `selfhost/`, every measurement
with stage 2. The seat was stopped once by a usage limit (about 01:20 to 02:03) and resumed from this file
and the worktree. Everything ran on this Mac (macOS arm64); **nothing ran on Linux or Windows**, and every
sentence about those two below says so.

## The seat's answer

- `verdict`: **veto** of (c) as the brief specifies it (an extent record per frame on a thread-local chain,
  walked by the jump hook and released there). **Approve with conditions** the layered resolution of
  § What I would build: the runtime's stacks heal themselves by frame (no hook, no per-call cost), the gates
  keep their aborts and say the truth, `spawn` and the asynchronous jump are told; and the values released by
  (d), a forced unwind through cleanup landing pads, scoped to functions reachable from a function handed to
  a **foreign** extern.
- `section`: design.md §1.12 (*"where two admissible forms disagree and one of them can be made to crash, the
  other wins"*), CLAUDE.md § Precedence rank 3; for the cost, §1.1 and §1.7 and Part 5 (no core construct is
  added by any option: every one is the backend's and the runtime's).
- `implementation_cost` (estimates, by file; the prototypes' sizes are measured): **the healing stacks**,
  `runtime/parts/frame.c` (157 lines) +49 and −4 in the prototype, `runtime/parts/lend.c` (985) about +50,
  `selfhost/emit/large_value.hero` (242), `ffi_lend.hero` (177) and `lend_local.hero` (254) about +10 to pass
  the prologue's own frame; the gates' words, `runtime/parts/alloc.c` (1,767) about +25; `spawn`'s answer,
  `runtime/parts/spawn.c` (825) about +25; the asynchronous refusal, `sigjump_save.c` (224) and
  `rebind_jump.c` (239) about +20 each: **about 200 lines of runtime and 15 of emitter**. **(d)**: the scope
  (a new `selfhost/emit/` module beside `callback_guard.hero` (207), about 80 with tests), the cleanup word on
  an in-scope slot (a helper beside `prologue.hero`, which stands at 301 lines, its threshold, about 40) and
  the sweep it replaces (`release.hero` (158), about 15), the thunk's count as a cleanup
  (`callback_thunk.hero` (406), about 10), `-fexceptions` for a unit holding an in-scope function
  (`selfhost/cli/flags.hero` (610) and the unit's key, about 25), the lend site's cleanup (about 30); a new
  `runtime/parts/unwind.c` (about 120: the forced unwind, its stop at the target, the standing-handler split,
  the end-of-stack fallback) called from the two hooks: **about 200 lines of emitter and 140 of runtime**. The
  compiler is 232,734 lines in 896 modules and the runtime 129,914 (`find ... | xargs cat | wc -l`): no
  ceiling is breached, and the per-module threshold is where the lines must land with care.
- `needed_for_self_hosting`: **no** (Principle 0). The compiler's four externs that take a function are all the
  language's own `hero_thread_spawn*` (18 of its functions are guarded, the address-taken set), and no program, package or module of the tree hands a foreign C library a function
  (enumerated below). The author's rule of 2026-11-22 (the most robust solution wins, Principle 0 yields) is
  what carries the release half; the healing half closes five false messages and an invisible leak and stands
  on §1.12 alone.
- `argument` (at most 120 words): (c) walks records it cannot prove alive. A jump the runtime never sees
  (assembly, run here; `__builtin_longjmp`, C++ and Objective-C unwinding, a dyld-shared-cache image,
  `dlvsym`) leaves records in stack memory the landing code reuses, and the next seen jump releases through
  them: a correct program that exits 0 today exits 134, or dies silently by SIGBUS, three of three, and an
  address check at the push does not save it. §1.12 decides by which form can crash. (d) walks only live
  frames, by the platform's unwinder: the same programs exit 0, leaking only the unseen half. It costs no
  instruction on a call (−0.4 against +30.6), and the compiler, handing no foreign extern a function, pays
  nothing.
- `prediction`: **the commit that lands (d) leaves `heroes build selfhost/main.hero --emit-c` byte-identical**,
  the `hero_program_sources` line taken out of each (`cmp` prints nothing), because no function of the
  compiler is reachable from a function handed to a foreign extern; **`heroes run tests/harness/latency.hero --
  compare --clock all`** of the whole resolution against `a23ea157a`'s compiler reports no loss past the floor
  of effect; and with that stage 2, `leaks --atExit` reads **0 leaks** on `leak/main_1k.hero` and **exactly
  4,000 leaks with exit 0** on `.ce/d6/d6b.hero`. Checkable at the landing's gate. Falsified if a function of
  the compiler moves under (d), if `compare` declares a loss, if the leak test leaks, or if d6b exits anything
  but 0.
- `condition`: (1) the veto falls if (c) comes with a liveness proof for every record its walk releases,
  which on this reading is an unwinder walk checking each record's frame and function, and then (c) carries
  (d)'s dependency without (d)'s free normal path; (2) my approval of (d) falls if a forced unwind is shown to
  run a landing pad in a frame that is not live (wrong unwind information in a C library between, read not
  run), or if the box shows UCRT's `longjmp` runs no termination handler and Windows then needs a mechanism of
  its own; (3) if the author wants no release at all, the conservative floor below is my answer, and it still
  is not (a').

## What a veto compels

A written answer from (c)'s author to D6 as measured: which check makes the walk release only records whose
frames are live, after a jump the runtime did not see, including the case of d6b where both jumps happen
inside one C call and the seen one lands deeper than the stale record. *Every jump is seen* is false on this
Mac (assembly), on Linux (`sigjump.c:66-74`'s list) and on Windows (no hook). A veto is a refusal and not a
price: it does not block (d), the healing stacks or the gates' words.

## The conservative resolution, and why it is not (a')

(a') as the brief writes it (*every gate forgives or tells the truth*) leaves two correct programs wrong: the
room row still aborts (truer words do not make a correct program run), and the lend row stays an unbounded
leak that no gate sees. And its *forgives* half for handles and leases contradicts spec § 13: *"a lease nobody
ends, like a handle nobody consumes or a thread nobody waits for, aborts when `main` returns, saying how many"*
(line 473). **The conservative floor this seat accepts is the first layer alone**: the stacks heal, the gates
keep their aborts with true words, `spawn` and the asynchronous jump are told, and the values stay leaked
with the heap-block audit forgiven as the interim does. It needs no spec sentence. The robust resolution adds
(d).

## What I ran (each with its command), and what I only read

### Ran, on this Mac (macOS arm64), stage 2 unless said

- Stage 1: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real 24.21 user 20.03 sys 0.73`
  (load average 14.87 when it started). Stage 2: `./heroes build selfhost/main.hero -o heroes-next`, `real
  398.02 user 222.27 sys 52.40` under load averages of 36 to 86 (ratio 1.45: not a number this report leans
  on).
- The leak test, `leak/main_1k.hero` built by my stage 2, `leaks --atExit`: `jumped 1000 times, sum 7000`,
  `7000 leaks for 3952000 total leaked bytes`. The critic's count, reproduced.
- Q1's survey (`.ce/shapes/`), Q5's four variants (`.ce/cost/v/`), the prototypes of (c) and (d) on the leak
  test (`.ce/proto/`), D6 (`.ce/d6/`), Darwin's `pthread_exit` (`.ce/pexit/px.c`), the room prototype and its
  21 cases (`.ce/room/`), clang's price of (d) (`.ce/dcost/`). Every command and reading is in its section.
- Enumerations: `grep -h -o 'h_0cb_[a-zA-Z0-9_]*(' build/tu-*/*.c | sort -u | wc -l` (18) over the compiler's
  897 units; every extern declaring a function-typed parameter across `selfhost examples examples-rebel
  packages tests` (`grep -E '^    function [a-z_0-9]+\(.*\(function'`): outside `tests/` all are
  `hero_thread_spawn*` (and the runtime's `hero_net_when_stopped`), none foreign.

### Read, not run

- `runtime/parts/callback.c` (86 lines), `frame.c` (157), `lend.c` (985), `sigjump_save.c` (224),
  `sigjump.c` (166), `rebind_jump.c` (239), `rebind_image.c`, `alloc.c` (1,767; the gates at 343-520),
  `sigguard.c` (645; the thunk's entry and its walk, 563-627), `thread.c` (144); `selfhost/emit/prologue.hero`
  (301), `release.hero` (158), `callback_thunk.hero` (406), `callback_guard.hero` (207), `selfhost/ir/own.hero`
  (631), `selfhost/ir/held.hero` (244), `selfhost/cli/flags.hero` (the frame word), `selfhost/main.hero`
  (70-125), spec § 13 and the `spawn` paragraph of § 11, design.md §1.12. The lend repair, (h), the Linux and
  Windows behaviour and the latency instrument's reading are read or inferred, never run.

## What I would build

**First layer, the runtime's stacks and gates (closes 2752 to 2756's false messages; no hook; no instruction
on any call).**

1. **The room heals itself by frame** (prototype run, below). Each take remembers the frame that took it; a
   take or a give first gives back every take of a frame deeper than its own, since the frame asking is the
   deepest one alive on its stack. A jump seen or unseen, on every platform. The real repair passes the
   prologue's own `__builtin_frame_address(0)` (always defined; clang warns `-Wframe-address` on the
   prototype's `__builtin_frame_address(1)`, and Windows keeps no frame records), and compares two frames only
   on one stack: the signal layer already keeps known ranges (`hero_sig_known_range`), and a coroutine's takes
   live on a LIFO of their own range or are never popped from another's.
2. **The lend regions the same** (`hero_lend_push` records the lending frame, a push or a give first retires
   the regions of deeper frames; read, not prototyped).
3. **The gates keep their aborts and say why** where the count of unfinished callbacks is above zero: the
   handle, lease and scratch gates add that a callback's frames never finished, abandoned by a C jump or a
   thread's end, and that what they owed was never reached. Forgiving them would need a spec sentence (line
   473 promises the abort). **The hook never runs a binding's consumer** (shared hazard 4, D7): a release is
   of counted memory only.
4. **`spawn`'s answer** is marked put by the thread's trampoline, and `wait` on a thread that ended without
   one aborts naming it (C ended the thread, or jumped out of its function), never a null read.
5. **An asynchronous jump is out of release's scope**: where the hook runs inside a standing handler that
   interrupted this runtime's allocator or lend bracket, it stops the program saying so, since POSIX leaves
   everything after such a jump undefined; elsewhere in a handler it jumps and leaks.

**Second layer, the values: (d), scoped.** The scope is asked of the program as the guard's is, since the
emitter does not chase a function value to the extern it reaches (`callback_guard.hero`): **where the program
declares a foreign extern with a function-typed parameter** (a group not of the language's own `hero_os.h`,
`hero_net.h`, `hero_time.h`), every function reachable from an address-taken function (by direct call,
through every address-taken function for an indirect call, over monomorphised instances), and their thunks;
**where it declares none, nothing**, which is the compiler's case and every shipped program's. A program with
one foreign callback and `hero_thread_spawn` workers puts its workers in scope too: the safe direction, paid
in clang's time and never on a call. In that scope each counted
slot is declared with `__attribute__((cleanup(<its release>)))` instead of being swept by hand (every release
the emitter writes already takes the slot's address, defect 470), the thunk ends its count by a cleanup, and
the unit holding such a function is compiled with `-fexceptions`. The hook, on Linux glibc and Darwin, outside
a standing handler, calls `_Unwind_ForcedUnwind` with a stop that makes the real jump once a frame's CFA is
above the target, which is glibc's own route for cancellation; at `_UA_END_OF_STACK` it jumps at once and
leaks what was not reached. Today that scope is empty for every program, package and module of the tree.
**What of it ran**: the forced unwind called from the header's jump on this Mac (`.ce/proto/proto_unwind.h`),
not from the runtime's hook, and nothing on Linux; Windows has no hook, and whether UCRT's `longjmp` runs
termination handlers there, which would make (d) reach it with none, is the box's question, unrun.

## Q2. The seven rows against each mechanism

| row (issue) | (a') widened | (c) chain, hook | (d) forced unwind | (e) lazy by frame | (g) told refusal | (h) after landing |
|---|---|---|---|---|---|---|
| values (none) | leak, told | released at a seen jump; **crash after an unseen one** (D6, run) | released at a seen jump (run); unseen: leak | not reached (slots are on the stack) | breaks correct bindings | unsound unless copied at the hook (the landing code reuses the stack), which is (c)'s walk |
| room (2752) | still aborts, truer words | needs a marker per extent; unseen: unbalanced | the room's give as a cleanup (`hero_frame_give_at` exists); unseen: unbalanced | **heals, seen or unseen** (prototype, run) | n/a | n/a |
| handles (2753) | forgive = spec sentence; tell = none | cannot (no consumer may run, D7) | cannot | cannot | n/a | cannot |
| `spawn`, `pthread_exit` (2754) | tell | no hook for `pthread_exit` | glibc unwinds by force (documented, unrun); **Darwin runs no C cleanup** (run: 0 of 1 with and without `-fexceptions`) | n/a | the trampoline's own check | n/a |
| signal jump (2755) | tell the true cause | unsound mid-instruction | no landing pad at an arbitrary instruction; not async-signal-safe | n/a | **for a handler that interrupted the allocator** | never in a handler |
| lend regions (2756) | no gate sees it | needs a marker | a cleanup scope round the lending call | **heals** (read) | n/a | n/a |
| lease, scratch (read) | as handles | cannot | cannot | cannot | n/a | cannot |

Cost on every call (Q5's table, Darwin arm64): (c) +30.6 instructions, its thread-local test +27.0, (d) −0.4
(within the spread), (e) nothing on a call (a word per large take or lend), (a') nothing.

## Q4. Where a release may run

**Only in the hook, only outside a standing handler, only for a jump made by C at a C call site.** There the
abandoned frames are intact (the hook runs before the jump), every slot is owned or zero (Q1), and this
runtime's allocator is not inside a call (C called the jump; it did not interrupt anything). Inside a handler
`free` and the allocator's locks are not async-signal-safe and the forced unwind is not either, so there it
jumps and leaks (the (f) split: `hero_sig_standing(own)`), and where the handler interrupted this runtime's
allocator or lend bracket it stops, saying so (row 5's true cause). **Copying the pointers out and releasing
later** needs the hook's walk to be right first, which is D6's problem again. **(h), after the landing**, with
records on the stack: the landing C code runs and calls before control returns to Heroes code, and reuses the
stack the records and slots stood in (read, not run; d6b's pad is that reuse, measured in the hook's case). A
chain hung off the thread key (D8) is unneeded by (d), which keeps no chain; the thunk's count stays the plain
thread-local it is.

## Q6. A spec sentence?

**No, for what I support.** The release of an abandoned frame's values ends nothing C may keep: a lend *"lives
for its call and no longer"* (spec § 13), and leases and handles are never ended by the mechanism. The gates
keep the abort line 473 promises. `wait` on a thread C ended: the spec gives `wait` *"its answer"* and says
nothing of a thread that has none; an abort naming it is the loud reading of that silence, and the place a
spec-warden might disagree. **Yes, for (a')'s forgiving branch**, which contradicts line 473; this seat does
not support it.

## The room prototype, run

`.ce/room/rt/` is a copy of `runtime/` whose `parts/frame.c` remembers each take's frame (+49 lines, −4);
the runtime object compiled from it with the unit's level words, `room_order.hero` (`docs/panel/258-briefs/
jumps/`) built by stage 2 and relinked against it. Stage 2 as it is: `panic: internal error: a large value's
room was given back out of order (compiler bug)`, exit 134, 3 of 3 (row 2 reproduced). Healed: **exit 0, 3 of
3**, the same output (`jumped, sum 56 main holds 2`), `0 leaks for 0 total leaked bytes`. **Nothing that does
not jump moves**: the 21 run cases of defects 2278, 2286 and 2316 that build alone (the abort cases left aside;
three cases importing `fixedbugs2278/` modules were not copied with them and **not run**) print the same bytes
and exit the same against both runtimes, 21 of 21, the spawned-thread case and the C-calls-back case among
them.

## (d)'s price at compile time, measured

clang's `-cc1` on the compiler's largest unit (`checkwalk.c`, 8,990 lines, 40 functions with an exit sweep,
585 release lines made cleanups by `.ce/proto/unwind.py`), instructions retired, median of three (the
process counted is clang itself, `exec`ed, after two readings that counted the driver and the shell):

| level | as emitted | cleanups, no `-fexceptions` | cleanups, `-fexceptions` | `__text` as emitted, then (d) |
|---|---|---|---|---|
| `-O0` (the compiler's build) | 2,830.5 M | 2,783.7 M (−1.7%) | 3,043.0 M (**+7.5%**) | 240,452 to 277,684 B (+15.5%) |
| `-O2` (`heroes run`) | 10,760.1 M | 11,271.9 M (+4.8%) | 13,707.8 M (**+27.4%**) | 126,192 to 159,988 B (+26.8%) |

So (d) is paid in clang's time and code size by the functions in its scope, never on a call; this is why
the scope must be *a foreign extern's callback*, and why it must not be *every function*. Carried to the
compiler whole, which the broad scope would do (Q3), that is several percent of the warm-build clock: an
inference from one unit.

## Where the briefs were false or silent

1. **Q3's *callback-reachable* route is presented as a cheaper one; for the compiler it is every function**
   (`command` is handed to `hero_thread_spawn_sized`, `selfhost/main.hero:102`), and for any program that
   works through `hero_thread_spawn`. The briefs are silent on it; `callback_guard.hero:31-32`'s *"the
   compiler itself ... guards nobody"* is stale at `a23ea157a` (18 thunks in the compiler's units).
2. **Q5's *thread-local flag test* saves almost nothing on Darwin**: a `_Thread_local` there is a descriptor
   call at every access (`otool`), +27.0 instructions a call against +30.6 for the push.
3. **(a') is said to need no spec sentence**; its forgiving branch contradicts spec § 13 line 473.
4. **The rule-7 hold** (`ir/held.hero`) is a counted reference in a temporary across a call, absent from
   Q1's list; a release by slots leaks it.
5. **`frame.c:135-143`** describes a route removed by panel 236's landing (`658251be4`, 2026-11-08), whose R2
   made a large group record write through `hero_out`; the comment and `hero_frame_give_at` remain.
6. **The doors on this Mac**: clang refuses `__builtin_longjmp` on arm64 Darwin, and Darwin's route answers
   `dlsym` for the jump names (`rebind_image.c:84-97`), so neither is a door here; hand-written assembly is
   (run). **Darwin's `pthread_exit` runs no C cleanup** (run), which the critic's F5 left for Linux only.
7. Held, by my own run: the leak test's 7,000 leaks and 3,952,000 bytes; the room row's abort.

## Q1. What an abandoned frame holds at a C call site (the survey, run)

`.ce/shapes/shapes.hero`: a callback `body` reaching fourteen functions, each holding one shape across a no-op
extern `sh_mark()`; its `--emit-c` read at every `sh_mark()` site. **At every site, every counted slot holds
zero or exactly one owned reference**, as `ir/own.hero`'s rule 5 promises ("nothing is ever owned by a
temporary past its own instruction ... ownership lives in slots, and cleanup is a walk over a table"):
`str`, array, map, a record (`h_shapes_Pair h1_p`, held by value, its fields released by its own
`h_shapes_0Pair_release`), an optional and a variant (released by their own `_release` through
`hero_slot_escape`), the `for` variable (`h4_x`, incremented when stored), the `own` slots, a value built into
a record (`k_moved`: the construct increfs, the slot keeps its own), a callee's argument (`k_callee_of`
borrows; the caller's `h2_own2` owns), the early-return and `.must()` paths (the one exit sweeps every slot
whatever path ran; the failing `.must()` aborts). A function value (`k_fnvalue`) holds no count. Found and
not in the brief's list:

- **The rule-7 hold** (`ir/held.hero`, panel 196's R1): `f(@x, x.items)` lowers in `k_held` to
  `hero_array_incref(t6); t7 = h_shapes_k_held_of(&h1_p, t6); hero_array_decref(t6);`. A +1 lives in the
  TEMPORARY `t6` for the duration of a call that reaches C. A release by slots misses it: one block a jump,
  a **leak residue, never a double release**.
- **An `@` parameter is the caller's place** (`ph0_acc`, `ph0_x`): the callee has no slot for it and its
  caller's sweep releases it. A record of the callee must not name it, and the emitter's sweep already does
  not.
- **The release set is a per-function static table with a release function per slot**, not a kind tag: a
  record, an optional and a variant each have their own `_release`.
- **Between the instructions of one store** (`t32 = h6_own6; h6_own6 = t2; hero_str_decref(t32);`) the old
  value is owned by a temporary, and inside a runtime call (`hero_array_push_owned` growing an array) a slot
  may be mid-write. A synchronous jump never stops there; an asynchronous one (a signal) can. So a release
  by slots is sound for a jump made at a C call site only.

**Since defect 470 (panel 200's R3) every counted slot's address already escapes** into an opaque runtime
call at the exit (`hero_str_release_at(&h1_s)`, `runtime/heroes_runtime.h:274-285`), so clang already keeps
every counted slot in memory: a table of slot addresses costs no register allocation the code does not
already pay.

## Q3. Which functions need a record (measured)

**The compiler runs every command inside a callback.** `selfhost/main.hero:88` binds
`hero_thread_spawn_sized(body: (function(i64) -> i64), ...)` and `main` hands it `command` (line 102), so
`callback_guard.takes_a_callback` is true for the compiler, and its stage 2 units carry **18 thunks and 18
guarded functions** (`grep -h -o 'h_0cb_[a-zA-Z0-9_]*(' build/tu-*/*.c | sort -u | wc -l`, 897 units).
`h_0cb_main_command` is one of them. **Every function of the compiler is therefore reachable from a function
handed to C**, and so is every function of any program that runs its work through `hero_thread_spawn`,
which `runtime/parts/thread.c`'s own refusal tells programs to use. `callback_guard.hero:31-32` says *"A
program that binds no callback guards nobody ... the compiler itself and all 45 corpus programs"*: **false at
`a23ea157a`** for the compiler (it is the brief-adjacent stale sentence this seat found).

So the four routes of Q3 collapse to two in cost: *every function* and *callback-reachable* both put a record
on every call the compiler makes; *only while the count is above zero* needs a thread-local read at every
entry (priced below: on Darwin it costs what the push costs); *callback-reachable and transitively calling an
extern* still includes the compiler's driver and every function between it and a file read. Indirect calls
are covered by `callback_guard.addresses_taken` (a function value is a `func_ref`, v1 has no closures), and
generics by the monomorphised instances, so none of the routes is unsound by under-approximation; the
problem is that none of them is cheap for the compiler.

**A fifth route, the one this seat recommends** (§ What I would build): the address-taken set's closure only
where the program declares a *foreign* extern taking a function. The compiler's four such externs are all
`hero_os.h`'s thread start, whose trampoline is the runtime's and holds no save point a jump could return
to, so its scope is empty; and outside `tests/`, so is every program's, package's and module's in the tree.

## Q5. Cost, measured, and the two prototypes

### The per-call cost of a record (Q5's three ways, and a fourth)

`.ce/cost/walk.hero`: `walk(n)` holds six counted slots and builds and returns a string per call
(`f"{...}"`); `walk(33)` makes 11,405,773 calls. Its compiled unit (`-O2`, the unit's own words) hand-edited
four ways by `.ce/cost/v/mk.py`, each linked against the same runtime object, the thread-local variables a
runtime would own in a separate object (`v/ext.c`): **v0** as emitted; **v1** a record (prev, sweep) and the
six slots in one struct, pushed at every entry and popped before the exit sweep; **v2** the same struct and
a test of a thread-local count, never above zero here, so it never pushes; **v3** each slot
`__attribute__((cleanup(hero_str_release_at)))`, the explicit sweep removed, compiled `-fexceptions` (the
normal path of route (d)).

Wall time was **discarded**: eight alternating rounds under load averages 9.5 to 16.5 read `real` far above
`user` plus `sys` in most runs, and `user` spread ±10% inside each variant (v0 1.06 to 1.18 s). The
instrument used instead is the process's **instructions retired**, from `/usr/bin/time -l`, five rounds of
the four, median:

| variant | instructions (median of 5) | per call against v0 | cycles (median) | unit `__text` |
|---|---|---|---|---|
| v0 as emitted | 15,533,363,156 | | 3,325 M | 584 B |
| v1 record pushed on every entry | 15,881,965,542 | **+30.6 (+2.24%)** | 3,395 M | 748 B |
| v2 thread-local test, no push | 15,840,975,758 | **+27.0 (+1.98%)** | 3,406 M | 784 B |
| v3 cleanups, `-fexceptions` | 15,528,604,751 | **−0.4 (in the spread)** | 3,242 M | 720 B |

The spread of instructions inside one variant is about ±15 M (±1.3 a call); cycles move ±3% between rounds
and are reported, not leaned on. **Why the test costs what the push costs**: on Darwin arm64 a
`_Thread_local` is reached through its descriptor, `ldr x8,[x0]; blr x8` (`otool -tv v/walk_v2.o`), an
indirect call at every entry whether or not the record is pushed. On ELF a local-exec access is a
register read plus an offset: an inference from the ABI, **unrun** (no Linux container was started).
Exporting `callback.c:58`'s `static _Thread_local` for route 3 buys nothing on this Mac.

**What the latency instrument would read** (inference from the table, **unrun**: `heroes run
tests/harness/latency.hero -- compare` needs the machine still and two built compilers): the compiler is
callback-reachable whole (Q3), so routes v1 and v2 put about 2% more instructions on every function that
allocates, and `compare --clock all` judges a median loss past 0.5% (`.claude/rules/verification.md`
§ The latency ceiling's instrument) a breach to be paid before a tag. A record on every entry would be
one. v3 adds no instruction on the normal path; its price is code size and clang's time on landing pads,
priced below.

### The prototype of (c) on the leak test: the leaks go

`.ce/proto/chain.py` rewrites the compiled unit `main1k.c` of `leak/main_1k.hero` (stage 2, `-O0`): each
function with an exit sweep gets a record of its counted slots' addresses on a thread-local chain
(`proto_chain.h`), popped before the sweep, with a static sweep made from its own release lines; the thunk
gets a record whose sweep calls `hero_callback_left()`. The header's `ab_throw` walks the chain down to the
save point's frame before `longjmp` (the save point records its own frame: a prototype's shortcut for the
hook's decoding of the buffer). Result, `leaks --atExit`: **`0 leaks for 0 total leaked bytes`**, prints
`jumped 1000 times, sum 7000`, exit 0 (baseline 7,000 leaks, 3,952,000 bytes). The count comes back to
zero, so the heap-block audit ran and found nothing.

### D6, run: the chain turns a jump it did not see into a crash

`.ce/d6/`: `d6.h` adds a save and a jump **written in arm64 assembly** (`d6_save`, `d6_jump`: x19 to x30, sp,
d8 to d15), a door `runtime/parts/sigjump.c:66-74` names as unseen; `__builtin_longjmp`, the first choice,
is refused by clang on this target, and Darwin's route answers `dlsym` for the jump names
(`rebind_image.c:84-97`). `d6a.hero`: each round, a callback `body_u` holding a 1 KB string jumped over
unseen (`bj_try`), then `body` jumped over by `longjmp` (`ab_try`). `d6b.hero`: both inside ONE C call
(`d6_mix`), the seen one landing through a C frame of 4 KiB (`d6_pad`, filled with 0x41 as a library's buffer
would be), so it is deeper than the stale record. Three runs each:

| build | d6a | d6b |
|---|---|---|
| interim (stage 2 as it is) | exit 0, `rounds 1000, sum 10000`; 8,000 leaks, 5,264,000 bytes | the same |
| (c), the chain as specified | **exit 134**: `panic: a str's block has lost its mark ... the block was freed` | **exit 138 (SIGBUS), nothing said**: lldb, `EXC_BAD_ACCESS (code=257, address=0x4141414141414141)`, pc `0x0000414141414141` |
| (c) with the address check at a push (a record at or below the one pushed is dropped, never released) | exit 0 | **exit 138, nothing said** |
| (d), a forced unwind to the save point's frame | exit 0; **4,000 leaks, 2,640,000 bytes** | exit 0; **4,000 leaks, 2,640,000 bytes** |

In d6a the chain released a slot of a frame the unseen jump had abandoned, which the next frame's record had
overwritten; the runtime's string mark caught the double release this time. In d6b the walk read a sweep
pointer out of the pad's bytes and called it. **The check at a push does not save it**: a stale record that
lies above the landing frame looks like a live caller, and no comparison of addresses tells the two apart.
Nor would a check at the extern call's return: d6b makes both jumps inside one C call. **(d) leaks exactly the
unseen half (four blocks a round) and releases the seen half**: the unwinder walks the frames that are live,
so a frame nobody saw leave is never reached. The prototype of (d) on the leak test: `.ce/proto/unwind.py`
and `proto_unwind.h`, `_Unwind_ForcedUnwind` with a stop that makes the real `longjmp` once a frame's CFA is
above the save point's frame: **`0 leaks for 0 total leaked bytes`**, exit 0, three of three.
