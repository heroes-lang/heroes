# Panel 263, the compiler-engineer's report (written as it goes)

Seat: compiler-engineer. Copy `.claude/worktrees/263-compiler-engineer`, detached at `f9f42fcc4`, scratch
`.claude/worktrees/263-compiler-engineer-scratch/` (`TMPDIR` there). Started 11:13. Load at the start: `uptime` 44.26 /
44.30 / 33.18 at 11:14 on 8 cores, 60.62 at 11:15: **every duration below carries the load beside it**, and no clock
reading is taken while the load is above 8 unless said.

**Ran** and **read** are kept apart: *ran* names the command and its output file; *read* names the line.

## Where the briefs were false or silent (first, as `00-shared.md` § 8's contract asks; each with its run)

1. **The canary cannot count a run a route stops** (`00-shared.md` § 2.2, § 5 item 2, and my brief's *"does the
   canary write 0 bytes"*): it reads its block after the coroutine returns. A counting copy that reads it at the
   runtime's `raise` and `abort` too is § 1.1's; every *0 bytes* below is its count.
2. **The compiler DOES hand C a function**: `hero_thread_spawn_sized` (`selfhost/main.hero:88`), so it opens the
   callback question (20 guards in its own C) and **(c2) moves its own C, 7,782 functions checked** (§ 10);
   `callback_guard.hero:28-30` (*"the compiler itself ... guards nobody"*) is stale, and my brief's *"every `examples/`
   program with no callback `extern`"* misses that eleven examples have one (`hero_thread_spawn`, § 7, § 10).
3. **Panel 104's *"the counter forbids the recursion-to-loop transformation"* has nothing to forbid today**: the base
   at `-O2` keeps `down` a call (`bl _h_costrec_down`, the overflow check after it, `-fno-optimize-sibling-calls`). What
   the brief did not name is what (c) does instead: **the check doubles a small function's frame on both platforms**
   (the TLV call on Darwin, the miss's arguments on Linux, § 3, § 11), and a correct program 300,000 deep on `main`'s
   own stack is refused (§ 3.1).
4. **Route (c)'s row is silent on the reserve's price**: a quarter of a stack under 64 KiB is cut off (§ 1.2).
5. **Panel 258's stop by address (`CFA > target`, the model my brief points at) releases 0 of 3 slots across a switch
   where C's stack lies below the runtime's**, CFI or not (§ 2): R2's stop must stop by the frame's identity under (b).
6. **(b)'s *"but for the thunk's own frames"* is measured**: at most 768 bytes of C's stack (§ 2).
7. **(b) as the row reads it would move the compiler off its 256 MiB thread** (§ 10): a condition owed.
8. **My brief's (ii) assumed the `-O2` build hangs**: my seed-compiler build gave the false message 2 of 2; the hang
   read is `static.hero`'s (§ 4).
9. **`parts/drop.c`'s *"`==` and `hash` still recurse"*** is stale for `==` on a 100,000-deep chain (§ 1.4).
10. **Shape D under (c), *"C frames ... unguarded"*, measured**: 81,432 bytes below at exit 0 on this Mac, 41,957 on
    Linux (§ 6), identical to the base: (c) changes nothing there, (b) closes it.

## 0. Builds

- Seed's compiler (ran): `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real 56.77 user 23.67 sys
  1.03` (load 44 to 60: the ratio says it waited), `heroes 0.6.0`, `sources 1e1eb23a1927d35b` as the brief says.
- Stage 2 (ran): `./heroes build selfhost/main.hero -o heroes-next`, `real 472.60 user 216.75 sys 47.47` (load 65 at
  11:24: a duration nobody should quote), `sources 50e8403d5a885325`, the critic's.

## 0.1 The defect on my stage 2, this Mac (ran: `run.sh <heroes-next> rs 3 30`, `real 238.61`, `rs-run.txt`)

The critic's outcomes exactly: `malloc-O2` **the false thread message, 134, 3 of 3**; `malloc-O0` **exit 138, nothing
said, 3 of 3**; `malloc --sanitize` `AddressSanitizer:DEADLYSIGNAL`, exit 1 or 134; `guarded`, `bare`, `thread` told, 3 of
3 each level; **`static` hung, exit 124, 6 of 6**; the canary as the clerk's table (n 9,000 `-O2` 25,941 bytes, one run
25,130; n 2,000 `-O0` 112,450; `--sanitize` `-O0` n 2,000 232,194, the lowest at 262,144, the whole half).

## 1. Route (c), the hand prototype (ran, this Mac, the seed's compiler's emission)

The runtime copy `rt-c/` (scratch): `heroes_runtime.h` gains `HeroStackWindow`, a `_Thread_local` pair `{base, span}`,
and `hero_stack_check(what)`, inline: the stack pointer read by one `mov` (`sp - base >= span`, unsigned, one compare),
the miss out of line. `parts/stackcheck.c` (new, 58 lines with its notes): the miss looks up the range holding `sp` in
the ranges the runtime already keeps (`hero_rebind_known_range`, `rebind_entries.c:192`; `hero_sig_known_range`,
`sigentries.c:293`), else the thread's own (`hero_stack_lo/hi`, `stack.c:467-468`), refuses inside the reserve, else
sets the window. **The window is two-sided**, so a switch to any other stack, seen or unseen, misses on the next check
and re-derives: the switch wrappers need touch it only to reset a window set wide on an unknown stack.

First run (`proto/malloc-c1-O{0,2}`, `deep`'s prologue given the one line `hero_stack_check("malloc.deep");` after
`hero_thread_guard`): both levels `panic: stack exhausted in malloc.deep (...)`, exit 134, two misses each (the
first check, then the reserve).

### 1.1 The instrument: the canary counted at the stop (ran)

**The clerk's canary cannot count a run a route stops**: it reads its block after the coroutine returns, and a stopped
program never returns. So `proto-cc/co.h` (in `programs/compiler-engineer/`) is a copy that also counts at the stop: the
runtime's stop ends `raise(SIGABRT); _exit(134);` (`sigguard.c:171-172`), and a `raise` defined in the program's own
header is what the static link binds the runtime's call to, so the block is read and its count written by `write(2)`
before the signal goes. Kind 4 is the clerk's (`malloc`, twice the stack, the lower half 0xA5); **kind 6 is the same
block in a function-static array, shape A**; `CO_KB` sizes the stack. Every *0 bytes* below is this count.

### 1.2 Route (c1) on the defect's programs, this Mac (ran; `c1.sh`, `runc1.sh`, `count.sh`; `c1-mac/`, `cc-c1/`)

`c1.sh` emits a program with the seed's or stage 2's compiler, puts `hero_stack_check("<m.f>");` after every
`hero_thread_guard("<m.f>");` (that set is (c1)'s), and builds plain and `--sanitize` at both levels against `rt-c/` by
the compiler's own words (`hb.sh`, `flags.hero:136-154`). 106 configurations of the counting canary, base (stage 2)
against (c1), every one run once (the `run.sh` sizes three times, `c1-mac/runs.txt`):

| | base | (c1) |
|---|---|---|
| `malloc`, `guarded`, `bare`, `thread`, `static`, default n, both levels, 3 runs each | false message, 138, hang 124 (as § 0.1) | **told, 134, every run** |
| `malloc` by `CO_N` 1,600 to 50,000 `-O0`, 9,600 to 14,000 `-O2`, plain and `--sanitize` | exit 0 with up to 5 MiB written (§ 2.2 of the shared brief) | **told, 134, every one** |
| canary kind 4, 256 KiB, `-O0` n 1,500 / 2,000; `-O2` n 9,000 / 10,000 / 14,000 | 23,964 / 112,450; 25,941 / 57,937 / 185,905 bytes below, exit 0 | **0 bytes, told** |
| the same `--sanitize` (`-O0` n 1,000 to 3,000; `-O2` n 9,000 to 50,000) | 8,909 to 262,080 bytes below, exit 0 | **0 bytes, told** |
| **canary kind 6 (shape A)**, the same sizes, plain and sanitized | the same counts at exit 0, then **hang 124** or **132** past them | **0 bytes, told** |
| kind 4 at 16 KiB, 64 KiB, 1 MiB, `-O2`, `-O0`, sanitized | up to 1,048,064 bytes below (1 MiB `--sanitize` n 1,000,000), or hang | **0 bytes, told** |

**What (c1) refuses that the base ran correctly** (exit 0, 0 bytes below): 16 KiB `-O2` n 500 (16,000 of 16,384 bytes
used; the reserve is a quarter of a stack under 64 KiB, so 4 KiB there); 256 KiB `-O0` n 1,200 and 64 KiB `-O0` n 300,
**because the check grows `-O0`'s frame from 192 to 224 bytes** (`otool -tV`, `sub sp, sp, #0xe0`), so 1,200 frames
need 268,800 bytes, more than the stack: those runs would now overrun, and are stopped. `-O2`'s frame stays 32 bytes; its
thread-local read is a call through the TLV thunk (`ldr x8, [x0]; blr x8`).

### 1.3 A shape of my own that separates (c1) from (c2) and (c3): `helper.hero` (ran; `hp/`)

The function C is handed, `enter`, calls `walk`, which recurses; `walk`'s address is never taken. **The emission
confirms the brief's reading**: `walk` carries no `hero_thread_guard` (`hp/helper-c1.c:247`), so (c1) checks `enter`
alone.

| `helper.hero`, counting canary kind 4 | base | **(c1)** | (c2): `walk` checked too | (c3): every function |
|---|---|---|---|---|
| `-O2` n 10,000 | 57,921 bytes, exit 0 | **57,921 bytes below, exit 0, the answer printed** | 0, told 134 | 0, told 134 |
| `-O2` n 30,000 | told | **hang, exit 124** | 0, told | 0, told |
| `-O2 --sanitize` n 10,000 / 30,000 | not run | **57,937 / 262,080 bytes below, exit 0** | 0, told | not built |
| `-O0` n 10,000 | hang 124 | told (the guard's handler, no count printed) | 0, told | 0, told |

**(c1) is vetoed on soundness**: it leaves 57,921 bytes written below a `malloc`'d stack at exit 0, `helper.hero`, this
Mac, `-O2`, plain (57,937 `--sanitize`), by the counting canary.

### 1.4 Another shape of my own, run and closed today: `deepeq.hero` (ran; `eq/`)

`parts/drop.c`'s head says *"`==` and `hash` still recurse, and still crash"*: a recursion in C the compiler EMITS (a
record's `_eq`), which no set of route (c) checks, being no Heroes function. Built: a callback on the canary's stack
builds two `record Node { kids: [Node] }` chains n deep **with a loop** and compares them. n 100 to 100,000, both
levels, base, (c1) and (c1) with the check in every emitted `_eq` too: **0 bytes below, `same: 1`, exit 0, every run**.
So `==` no longer recurses per level on this shape, and the note is stale for `==` (`hash` not run).

### 1.5 (c2) built in the compiler (ran; `sh-c2/`, `heroes-c2`, `sources 3b93a6590f98b9d9`)

A copy of `selfhost/` in my scratch, built by my stage 2 (`real 437.45`, load about 60). **`diff -r` against the base:
109 lines `<`/`>`**: `emit/callback_guard.hero` (207 lines at the base) gains the field `checked` (4 lines with its
note), `reached`, the address-taken set closed under `.call` (27 lines with its note: a pass over the program per
level of the call tree, keyed by declaration as the guard's map is, so every instance of a generic is checked; a call
through a function value needs no edge, its target being a root), and `entry` writes the check after the guard (11
lines for 5); `emit/body.hero` (402) two signatures, one call and three tests; `emit/unit.hero` two calls and a line;
`emit.hero` a call and a line. **No new construct, no checker, no lowering, no descriptor**: the emitter's prologue
line and one walk. Runtime: `heroes_runtime.h` +22 lines (the pair and the inline check), `parts/stackcheck.c` 58
lines, `runtime.c` +1 include; the switch wrappers' reset of a wide window, not written (about 2 lines in each of
`rebind_jump.c` and `sigjump_save.c`).

Emitted by it (`hp/helper-real-c2.c`): `walk` `hero_stack_check("helper.walk");` alone, `enter` guard and check,
`setup` and `main` nothing. Built by it, plain and `--sanitize`, both levels, n 1,000 to 10,000,000: **0 bytes below
every run**, `panic: stack exhausted in helper.walk`, 134, past the stack.

## 2. Route (b), the hand prototype (ran, this Mac; `rt-b/`, `b.sh`, `bb/`)

`rt-b/parts/cbstack.c` (new, 98 lines): `hero_on_stack`, an AArch64 trampoline (file-scope assembly, Mach-O and
ELF spellings) whose CFI names the frame it came from by three `DW_CFA` expressions; `hero_callback_run(fn, arg)`,
which runs `fn` where it is when the thread is already on one of its stacks (a nested entry), else on a free one of 16
per thread (8 MiB `mmap`ed, a `PROT_NONE` page below), the thread's `hero_stack_lo/hi` naming it while `fn` runs so the
guard tells its run-out; refuses past 16 held. `b.sh` rewrites each thunk: a struct of its arguments and result, a
static function that calls the real one, and `hero_callback_run` between the question and `hero_callback_left`. **The
question stays first, on C's stack.**

- **The defect's shapes**: canary kinds 4 and 6 and `helper.hero`, both levels, plain and `--sanitize`, n 1,000 to
  10,000,000: **0 bytes below C's stack, every run**, counted at return, at the stop and at the guard's `abort`
  (`hero_abort` ends in `abort()`, `panic.c:77-81`, which the counting header now defines too). Past 8 MiB `panic:
  stack exhausted in <m.f>` (the guard) or ASan's `stack-overflow`, 134. **And n 9,000 to 100,000 at `-O2` now RETURN
  THE RIGHT ANSWER at exit 0** where the base wrote bytes and (c) refuses: the callback's depth no longer rests on C's
  stack's size.
- **What (b) leaves on C's stack, measured** (`CO_LEAVE`: the coroutine's entry uses its stack down to that room before
  calling, `CO_N` 0; the smallest room with 0 bytes below): **base 128 / 512 / 512 bytes (`-O2` / `-O0` / `--sanitize
  -O2`), (b) 320 / 512 / 768**. The question, `hero_callback_run` and the trampoline need under 1 KiB of C's stack
  however deep the callback goes; the base's figure holds `deep(0)`'s frame as well.
- **Lane unwinder's R2 across the switch** (`fu/fu.c`, built `-fexceptions`; panel 258's ffi-pragmatist's
  `fu_main.c` the model): three cleanup slots, two below the switch and one above it on C's stack, released by
  `_Unwind_ForcedUnwind` from the bottom, this Mac:

| C's stack | switch | CFI on the switch | stop by address (258's `CFA > target`) | stop by the frame (`CFA == guard's`) |
|---|---|---|---|---|
| main thread's | none | | 3 of 3 | 3 of 3 |
| main thread's (above the runtime's) | yes | **none** | **2 of 3** (`_UA_END_OF_STACK` at the switch) | **2 of 3** |
| main thread's | yes | yes | 3 of 3 | 3 of 3 |
| a coroutine's in the image's data (below the runtime's) | none | | 3 of 3 | 3 of 3 |
| the same | yes | none | **0 of 3** (stops at the first frame) | 2 of 3 |
| the same | yes | yes | **0 of 3** | **3 of 3** |

**So (b) costs R2 two things a lane can build**: CFI on the switch (three `DW_CFA` expressions per architecture, per
platform's assembler), and **a stop function that stops at the save point's frame by identity, never by address
order**, since frames on two stacks are not ordered by address. Without the first R2 releases nothing above the switch;
without the second, nothing at all where C's stack lies below the runtime's.

## 3. Costs (ran, this Mac; `cost/`, `costbin/`, `costrun.sh`; instructions retired by `/usr/bin/time -l`, six rounds
alternated base / (c2) / (b), the first discarded, the best of five; the load 29 to 48 throughout, so the program's
own `ns` is written beside and not judged)

| program | level | base | (c2) | (b) |
|---|---|---|---|---|
| `cost_cb`, 20,000,000 callbacks (panel 259's) | `-O2` | 3,370,579,731 | 3,711,011,021 (**+10.1%**, +17 a callback) | 9,043,524,319 (**+168%**, +284 a callback) |
| | `-O0` | 8,448,765,277 | 8,949,519,064 (+5.9%) | 22,707,997,819 (+169%) |
| `qsort` of 1,000,000 `i64` through a callback | `-O2` | 3,943,755,912 | 4,300,224,218 (+9.0%) | 9,905,607,249 (+151%) |
| | `-O0` | 9,657,517,806 | 10,185,061,540 (+5.5%) | 24,654,680,269 (+155%) |
| one callback, 20,000,000 calls of a small function | `-O2` | 242,313,309 | 342,698,255 (**+41%**, +5 a call) | 242,466,972 (+0.06%) |
| | `-O0` | 1,367,532,065 | 1,827,945,327 (+34%, +23 a call) | 1,368,145,450 |
| one callback, `down(10000)` 1,000 times (panel 104's shape) | `-O2` | 124,466,616 | 325,364,859 (**+161%**, +20 a call) | 124,650,552 |
| | `-O0` | 497,763,025 | 737,465,987 (+48%) | 497,145,066 |

Every answer equal across the three. (b)'s +284 instructions a callback is this prototype's (a scan of 16 slots, the
thread-locals read through the TLV thunk four times, the bounds swapped); a landing would keep a per-thread index, an
estimate unrun.

**Panel 104's shape at `-O2`, read** (`otool -tV costbin/cost_rec-{base,c2}-O2`): **the base does not turn `down` into
a loop today**: `bl _h_costrec_down` stands, held by the overflow check after the call and by
`-fno-optimize-sibling-calls` (`flags.hero:151`). So the check blocks no transformation. **What it does on Darwin**:
the window's read is a call through the TLV thunk (`ldr x8, [x0]; blr x8`), and **`down`'s frame grows from 16 to 32
bytes** (`stp x20, x19` to keep `n` across it): a stack holds half as many of its frames, on `main`'s stack too,
wherever the function is in the set. `deep`'s `-O0` frame grows 192 to 224 (§ 1.2).

### 3.1 What the frame growth costs a correct program (ran; `cost/rec_main.hero`, `costbin/rm*`)

`down` is reachable from a callback, so (c2) checks it; `main` also recurses through it on the thread's own 8 MiB
stack. `-O2`, this Mac: depth 200,000 both exit 0; **depth 300,000, 400,000 and 500,000: base exit 0, (c2) `panic:
stack exhausted in rmN.down`, 134**. Nothing is corrupted, it is told; but **a program that ran no longer does**, on
Darwin, because the window's read through the TLV thunk doubles a small function's frame (§ 3). Linux's thread-local
read is inline (`mrs tpidr_el0`): its frame, unread, is the next landing's to read.

## 4. The readings left (ran, this Mac)

**(i) Which instruction writes the runtime's words** (stage 2's `rs/malloc-O2`, the false message 3 of 3; lldb, the
per-thread address by the TLV descriptor's thunk as the clerk's `lldb2` did, then `watchpoint set expression -w write`;
`lldb/watch.py`, `lldb/watch-malloc-O2.txt`):

- `hero_callback_mine` (48,944 below `lo`): first written by **`h_malloc_deep`'s own prologue, `stp x20, x19, [sp,
  #-0x20]!`**, `sp` exactly on the word, the value 0x100022524, an address inside the image's text.
- `hero_thread_is_home` (49,111 below `lo`): first written by **`hero_thread_guard`'s own prologue, `stp x29, x30,
  [sp, #-0x10]!`**: the word's byte is byte 1 of the saved return address `0x10002239c`, **0x23**. The guard then reads
  it `ldrb w9, [x0]; cmp w9, #0x1; b.ne refuse` (`otool -tV`): **the false message is the guard reading its own return
  address**, which its own frame, pushed below the stack by the recursion, laid over the word it tests.

**(ii) Which signal the trampoline was called for in the hang**: my seed-compiler `malloc-O2` build gave the false
message 2 of 2, not the hang (the outcome is the build's and the run's), so the hang read is `static.hero`'s (stage
2, hung 6 of 6): lldb with `EXC_BAD_ACCESS` passed to the program and a breakpoint at `hero_rebind_tramp` filtered on
the signal (the runtime's own probe is signal 16, `hero_signal_learn`): **`SIGBUS` (10), the recursion's fault, the
catcher `hero_stack_handler` (`stack.c:1145`)**; inside it, `hero_rebind_relived` (`rebind_entries.c:722`) faults
again, **`SIGSEGV` (11)**, whose delivery enters the trampoline nested (`-O0`, read with variables,
`lldb/tramp-static-O0.txt`); at `-O2` lldb's `continue` never returned (exit 124, `lldb/tramp-static-O2.txt`).
**Added 2026-11-27 by the coordinator**: that timeout left lldb's debuggee behind, stopped (state `T`, pid 79351), found at the end of the sitting by `ps` and stopped by its id.

## 5. Step 199j's stop (ran)

- **Its seven cases, this Mac, stage 2**: `heroes-next run tests/harness/main.hero -- heroes-next run fixedbugs-2871`:
  **7 passed, 0 failed**, each judged at `-O0`, `-O2`, `--sanitize` and `--sanitize -O0` by the `run` form
  (`suite_run.hero:239-256`). Linux arm64: § 6.
- **Under this defect** (`proto-stop/stop.hero`, a header of mine: the clerk's kind 1, then a `SIGUSR1` handler that
  swaps to a second context and back, outside any switch, three times; the shape
  `fixedbugs-2871-a-handler-swapping-contexts-outside-any-switch-runs` holds): `-O2`, `CO_N` up to 9,660 (the recursion
  45 KiB below `lo`, short of the record): `handled: 33`, exit 0. **`CO_N` 9,680 to 9,720 (the record `hero_signal_stack`
  written, 47,696 to 48,872 below `lo`, the guard's word at 49,111 not): the callback's answer printed, then the
  handler's switch: hang, exit 124, 3 runs, or exit 138, 3 runs; nothing said.** The stop neither fires falsely nor
  holds: it loops or dies, as the trampoline does (§ 4 (ii)). From 9,730 the false thread message comes first, inside
  the recursion. **So the stop's honesty, like the guard's, rests on words this defect writes**: (l) is owed to it too.
- **A correct program the stop might refuse, by reading** (not built; § 9 says why): every window in which the stop
  fires lies between a wrapper's `switch_begin` and its `switch_back` (`sigguard.c:600-640`), that is inside the
  program's own call of `swapcontext` or `setcontext`, which is no async-signal-safe function: a handler's switch
  there is a program POSIX leaves undefined, so the windows I read refuse no POSIX-defined program. The one shape I
  cannot rule out by reading: a coroutine carved from the thread's own stack below the noted frame (panel 262's
  `mj_carved`), whose frame walk (`hero_signal_in_switch`, bounded by the noted stack's range, `sigguard.c:670-685`)
  runs inside that range; unrun.

## 6. Shape D, a recursion in C under a callback (ran, this Mac; `proto-d/`, `dd/`): what separates (b) from (c)

The function C is handed calls `c_walk`, a C function of the header recursing n deep with 64 bytes of locals, on the
canary's 256 KiB `malloc` stack. Base (stage 2), (c2) built by its compiler, (b) by `b.sh`:

| | `-O0` n 2,500 / 3,000 / 4,000 | `-O0` n 6,000 / 100,000 | `-O2` n 1,000 to 6,000 | `-O2` n 100,000 |
|---|---|---|---|---|
| base | 7,952 / 32,442 / **81,424 bytes below, exit 0** | 77,226 below, then `stack exhausted in c_deep` | 0, exit 0 | **exit 138, nothing said** |
| **(c2)** | 7,960 / 32,450 / **81,432 bytes below, exit 0** | 77,218 below, then told | 0, exit 0 | **exit 138, nothing said** |
| **(b)** | **0, exit 0, the answer** | 0 at n 6,000; **0, told**, at 100,000 | 0, exit 0 | **0, exit 0, the answer** (8 MiB holds it) |

**(c) leaves exactly the hole its row names**: C's frames below the last check. (b) closes it, because every frame the
callback makes, Heroes or C, is on the runtime's guarded stack; and (b) needs to have seen no stack at all, so it
closes item 11 (a stack no wrapper saw, boost.context, hand assembly) as well. This is the run my verdict turns on.

## 7. The parallel examples pay (c2) too (ran)

`hero_thread_spawn` takes a function-typed parameter (`examples/nqueens/main.hero:18`; the eleven examples that declare it,
by § 10's run: `collatz`, `dotproduct`, `firsthit`, `histogram`, `mandelbrot`, `matmul`, `montecarlo`, `nqueens`, `server`,
`threads`, `wordbands`), so each of them opens the callback question
and under (c2) every function a worker reaches is checked (`heroes-c2 --emit-c`: `nqueens` 4 functions, among them
the recursive `solutions_from`; `mandelbrot` 7). `-O2`, `/usr/bin/time -l`, best of five alternated, the first
discarded, the outputs' `md5` equal: **`nqueens` 26,016,793 → 26,842,576 instructions retired (+3.2%), `mandelbrot`
28,201,154 → 31,910,499 (+13.2%)**. Under (b) each worker's body would run on a runtime stack: one switch per spawn,
unmeasured.

## 8. The other routes

- **(e), a probe at the thunk** against the space left on the range: **ruled out by the runs above**: the canary's
  thunk is entered at the top of its 256 KiB stack with all of it left (`CO_LEAVE` unset), passes any probe, and the
  recursion after it writes 25,941 to 262,080 bytes (§ 1.2, base). Alone it closes nothing of this defect.
- **(n), a static bound of the callback's tree** (`-fstack-usage` and § 1.5's walk): every tree in the tree's own
  programs that hands C a function is a `hero_thread_spawn` worker or a server's handler (§ 7); `nqueens`' worker is
  recursive (`solutions_from`), so its tree has no static bound and falls to (c) or (b); the others I did not read
  function by function. (n) is (c) with fewer checks on bounded trees, and inherits (c)'s hole in C (§ 6) and its frame
  growth wherever it checks. Not built.
- **(l), the runtime's per-thread words out of `_Thread_local`**, and **(q), the process-wide words too**: by reading
  and by § 4 and § 5's runs. They change the WORDS, never the writes: the false message is the guard reading its own
  return address (§ 4 (i)), the hang and 138 the trampoline reading its own overwritten records (§ 4 (ii), § 5); moved
  out of reach behind a guard page, those words would stay true and the recursion would write on into whatever else
  lies below (the program's and C's thread-locals, the allocator) until a fault the guard may not recognise (past its 1
  MiB window, `stack.c:517`). **Owed beside the route, in my reading, only where the route leaves a write**: under (b)
  no frame of a callback is below a stack it did not choose (§ 2, § 6), so nothing reaches the words; under (c2) C's
  frames do (§ 6), so (l) and (q) are owed for honesty (design.md §4.17, line 2,395 by `grep -n "^### 4.17" docs/design.md`). Not built
  (225 thread-locals in 49 files, the shared brief's § 2.3, re-read by `grep`, not re-run).
- **(p), a hardware watchpoint**: **ruled out by count** (ran: `lldb/wcount.txt`). lldb on this Mac sets four 8-byte
  write watchpoints and refuses the fifth (*"Watchpoint creation failed (addr=0x16fdfdf60, size=8)"*): 4 words, against
  2,048 in a stack's lowest 16 KiB page here, and every stack of every thread at once. Linux's `perf_event_open`
  breakpoint takes `bp_len` 1 to 8 (`HW_BREAKPOINT_LEN_8`), by reading, unrun in the image. Darwin gives a process no
  user API for one but a debugger's `thread_set_state` of the debug state (its use on a task's own thread unrun).
- **(k), a depth counter**: a count is not a size: one function's frame measured here is 32 bytes at `-O2`, 192 at
  `-O0`, 224 checked at `-O0`, larger under ASan (§ 1.2), a factor of 7 for one count.
- **(m), a canary below the range read at return or switch**: my counting header is (m) in miniature, and it read 1 MiB
  written at exit 0 (§ 1.2): it tells the corruption after it, never prevents it.

## 9. Linux arm64 (ran in `p263-compiler-engineer`, `--rm --pull never`, one at a time; `lx/linux{1,2,3}.txt`)

- **Builds**: the seed's compiler 65 s, `sources 1e1eb23a1927d35b`; **stage 2 inside the image, 810 s, `sources
  50e8403d5a885325`**, the Mac's. (`linux1.sh` forgot `HEROES_RUNTIME` and every program build failed, *cannot find
  the Heroes runtime*: rerun as `linux2.sh`.)
- **The defect on stage 2** (`run.sh` 3 runs): the repair's table: `malloc`, `guarded`, `bare`, `thread` told at both
  levels; `malloc --sanitize` `AddressSanitizer:DEADLYSIGNAL`, exit 1; **`static` exit 139, nothing said, 6 of 6**; the
  canary at `-O2` n 9,000 19,453 bytes, n 10,000 41,630 to 43,441, `-O0` n 2,000 112,451, `--sanitize -O0` n 2,000
  219,700, exit 0 each.
- **(c1)'s counting canaries (kinds 4 and 6, every size and level, plain and sanitized): 0 bytes in every
  configuration where it checks the recursing function**. **`helper.hero` under (c1): 43,441 bytes below at `-O2` n
  10,000, exit 0, the answer printed (`--sanitize` the same); 99,619 bytes at `-O0` n 10,000, then `stack exhausted`.
  (c2) and (c3): 0 bytes, every run, plain and sanitized.** The veto stands on both platforms.
- **(b)**: 35 counted runs at 0 bytes; the 5 `--sanitize` runs past 8 MiB end in ASan's `stack-overflow`, told, with no
  count printed (ASan's report reaches neither `raise` nor `abort`): told, uncounted. The thunk's room on C's stack:
  **base 64 / 448 / 384 bytes (`-O2` / `-O0` / `--sanitize -O2`), (b) 192 / 448 / 512**.
- **The forced unwind across the switch**: the Mac's table line for line (§ 2): 2 of 3 without CFI, **0 of 3 by
  address where C's stack lies below the runtime's**, 3 of 3 only with CFI and the stop by the frame.
- **(iii), a 64 KiB `malloc` stack** (the clerk's `co.h`, `CO_STACK` changed; `CO_SAY`): **in the `brk` heap**
  (`0xaaaa…`), the thread-local block far above (`0xffff…`); `-O2` n up to 2,100 exit 0 (n 2,100 is 67,200 bytes of
  frames on 65,536), past it `stack exhausted`. Counted (the counting canary, `CO_KB` 32, a 64 KiB block from the
  heap): **base `-O2` n 2,000 23,484 bytes below at exit 0; `-O0` 12,454 bytes, then told**: the recursion writes the
  heap's earlier chunks and is told only once it runs off the heap's bottom. (c1) (the same set as (c2) for this
  program): 0 bytes every run; (b): 0 bytes, and n 10,000 returns the right answer.
- **199j's seven cases on stage 2: 7 passed, 0 failed** (549 s).
- **The four switch-family cases red in Linux arm64 on 199j's base, named** (ran, `lx/linux4.sh` inside `linux5.sh`,
  `runs/linux-27-28-exits.txt`; one harness process a case, four at a time, a red to be re-run alone): **`fixedbugs-27`
  and `fixedbugs-28`, 134 cases (2871's 7 among them), 134 passed, 0 failed, every exit 0, 474 s**. So at `f9f42fcc4`
  none of the four reproduces in these two sets: repaired since the step's base, or in a set I did not run
  (`fixedbugs-26`'s 41, and the commit's *"ten sets"* of 166 are not these 134). Unnamed, then, and not red here.

## 10. (c2) moves the compiler's own C (ran; `ident.sh`, `ident/result.txt`)

`selfhost/main.hero`'s emission **differs: 7,782 functions checked, 20 guards**. The compiler declares
`hero_thread_spawn_sized` with a function-typed `body` (`selfhost/main.hero:88`; also `cli/link.hero:234`,
`mutate/cut.hero:45`, `mutate/recovery.hero:35`): every command runs on a thread whose 256 MiB stack the compiler
chooses (`main.hero:70-102`, panel 184's R5), so it opens the callback question, and `callback_guard.hero`'s note
*"A program that binds no callback guards nobody — the compiler itself and all 45 corpus programs"* (`:28-30`) is
stale. **44 examples emitted the same bytes but the `hero_program_sources` line; 12 programs differ, the compiler and the 11
examples that declare `hero_thread_spawn`** (`collatz` 3 checks, `dotproduct` 4, `firsthit` 4, `histogram` 3,
`mandelbrot` 7, `matmul` 6, `montecarlo` 4, `nqueens` 4, `server` 50, `threads` 3, `wordbands` 5; `ident/result.txt`,
`ident.sh`, stage 2 against the (c2) compiler). Every program with no callback `extern` is byte-identical.

**And (b) as I prototyped it would move the compiler's work off that 256 MiB stack** onto an 8 MiB runtime one: the
spawned body is reached through the thunk. **An inference, unrun** (it needs a compiler built with (b)'s thunks), but
the floor is measured: stage 2 checks a one-line sum of 12,000 terms and stops at 16,000, `panic: stack exhausted in
walk.expr` (`nest/`), on 256 MiB; on 8 MiB panel 184's own reading was 300 terms. **So (b) is owed a condition: the
runtime's own doors, `hero_thread_spawn` and `_sized`, whose stacks the runtime makes and guards, run the body where
it is.**

## 11. Costs on Linux arm64, and the check's code there (ran, `lx/linux3.txt`, about 12:15)

By the program's own `CLOCK_MONOTONIC`, base / (c2) / (b) each built by `hb.sh` from the image's stage 2 emission
((c2)'s sets from the (c2) compiler's emissions here, `costbin/c2-sets.txt`, put in by `addcheck.py`), six rounds
alternated, the first discarded, the best of five, the answers equal. **Read while this Mac's load was about 60 (the
container's own 2.6 to 4.3), so the per-call rows (-13.9% to +0.5%) are noise, and only (b)'s per-callback row stands
clear of it**: `cost_cb` `-O2` 342,323,166 ns base, 355,930,000 (c2), **1,005,563,292 (b), +194%**; `qsort` `-O2`
(b) **+117%**; `-O0` (b) +93% and +66%. A quiet re-run: § 13.

**The check, Linux arm64, `-O2`, `down`** (`llvm-objdump -d`): inline, `mrs x9, TPIDR_EL0`, `ldp`, `sub`, `cmp`,
`b.hs`, 8 instructions and no call; **and the frame still grows from 16 to 32 bytes** (`str x19`), the out-of-line
miss taking `n`'s register for its arguments. So the halved depth of § 3.1 is both platforms' cost. A miss taking no
argument under a convention that keeps every register (clang's `preserve_all`) might avoid it: an option, unrun.

## 12. The compiler under (c2) (ran, this Mac, load 1.4 to 5 from 15:36)

- **Its nesting floor** (`nest/`, a one-line sum of n terms, `check`): stage 2 checks 13,000 and stops at 14,000
  (`panic: stack exhausted in walk.expr`); **the compiler built by the (c2) compiler (`heroes-c2c2`, `sources
  11b1ee4b15d12e8e`, its own C checked) checks 14,000 and stops at 15,000** with its check's words. No loss: its
  recursion's frames are large. Spec § 9's 2,000 stands. (The check's words name the module run together,
  `resolvewalk.expr`: my prototype's naming, the existing guard's `who`, to be read at a landing.)
- **Its speed, `check selfhost/main.hero`** (`/usr/bin/time -l`, alternated, `check-time3.txt`, `check-three.txt`):
  **base stage 3 (`heroes-next3`, selfhost built by stage 2, `sources 50e8403d5a885325`) 149.1e9 to 149.5e9
  instructions retired; `heroes-c2c2` 129.2e9 to 129.7e9, 13% FEWER**; and `heroes-c2` (the (c2) sources, its C emitted
  without checks) 149.7e9, the base's. Traced (ran, `unitcmp/`): `resolve/walk`'s unit differs between the two builds
  by the check lines and the paths alone (`diff`, 44 lines), and **compiled by hand with identical words, the unit with
  the checks gives `walk.expr` a `-O0` frame of 18,464 bytes against 20,368**: the always-inline check changes how
  clang lays out a large `-O0` function's frame. A side effect of code generation, not the check's price, and the
  reason the checked compiler retires fewer. The compiler is not slowed on `check`; the clocks of these rounds (load 5
  to 34, `real` up to 67 s against `user` 19) are discarded by the rule. Its size: **26,980,312 → 29,507,320 bytes
  (+9.4%)**. The warm build, before and after: § 13.

## 13. The compiler's warm build, before and after, and the Linux costs on a quiet machine

**The warm build** (ran, `runs/warm-build.txt`): before, base stage 3 building `selfhost/main.hero`; after, the (c2)-built
compiler building the (c2) sources (its C checked: the compiler a landing would ship), alternated, `/usr/bin/time -l`,
round 1 discarded (the (c2) side's units cold, `user 181.90`), round 4 stopped by its ids once two warm pairs agreed;
**the load 31 to 47 from other sessions throughout, so every `real` is discarded** (161 and 158 s against `user` 102
and 101) **and the judge is `user` and instructions retired**:

| round | base `user` / instructions | (c2) `user` / instructions |
|---|---|---|
| 2 | 102.18 s / 781,867,744,449 | 89.18 s / 657,361,084,353 |
| 3 | 100.85 s / 781,985,313,567 | 87.96 s / 656,808,138,735 |

**(c2) does not slow the compiler down: 16% fewer instructions, 13% less user time**, the `-O0` frame side effect of
§ 12; and its binary is 9.4% larger. Read with § 12, what (c2) costs the compiler is its size; what it would cost a
compiler built `-O2` is unmeasured.

**Linux arm64 again** (ran, `lx/linux5.sh`, `runs/linux5.txt`, from 16:14; the container's load 1.1 to 1.8, this Mac's
about 43 from other sessions; the programs' own clock, six rounds alternated, the first discarded, the best of five,
every answer equal): per callback **(c2) +6.2% (`cost_cb` `-O2`), +4.9% (`qsort`); (b) +206% and +107%** (`-O0`
+97%, +33%). The per-call rows wander from -28% to +11% between two such runs: **this clock on a loaded host cannot
resolve them**, and this Mac's instructions retired (§ 3: +41%, +161% for (c2), +0.1% for (b)) stay the judge there.

## 14. Verdicts (per route; design.md §1.12 at line 646 by `grep -n "^### 1.12" docs/design.md`)

### (c1), the address-taken set: **veto** (soundness)

- `section`: §1.12 (*"must not corrupt memory"*).
- `implementation_cost`: `callback_guard.hero` `entry` one line; runtime as (c2)'s.
- `needed_for_self_hosting`: no.
- `argument`: it leaves bytes written below a stack in a shape it claims (a recursion a callback reaches): `helper.hero`,
  the recursion in a helper whose address is never taken: **this Mac `-O2` 57,921 bytes below at exit 0 (`--sanitize`
  57,937), hang at n 30,000; Linux arm64 `-O2` 43,441 at exit 0, `-O0` 99,619 then told**, by the counting canary
  (§ 1.3, § 9).
- `prediction`: any landing of the address-taken set alone reads a nonzero count on `helper.hero` at `-O2` n 10,000 on
  both arm64 platforms.
- `condition`: a run of `helper.hero` under it at 0 bytes, which only a wider set gives.
- **What the veto compels**: a written answer to `helper.hero`'s count.

### (c2), every function a callback reaches: **object** (sound for Heroes frames; not the most robust route)

- `section`: §1.12; CLAUDE.md § Precedence (the author's amendment of 2026-11-22: the most robust wins).
- `implementation_cost`: **selfhost +93 −16 lines** (`selfhost-c2.diff`): `emit/callback_guard.hero` 207 → 249
  (+41 −6: `checked`, `reached` 27 lines, `entry`), `emit/body.hero` 402 → 405 (+10 −7), `emit/unit.hero` 262 → 283
  (+23 −2) and `emit.hero` 422 → 433 (+12 −1), most of these two the formatter breaking each widened call across lines;
  no checker, lowering or descriptor change. **Runtime +81**: `heroes_runtime.h` +22, `parts/stackcheck.c` 58,
  `runtime.c` +1; the switch wrappers' reset, about 4 more. **Per callback +17 instructions (+10.1% `cost_cb`, +9.0%
  `qsort`); per call +5 (small function) to +20 (panel 104's recursion, +161%)**, this Mac; Linux per callback +6.2% and +4.9%, per call
  in the clock's noise (§ 11, § 13); the parallel examples +3.2% (`nqueens`) and +13.2% (`mandelbrot`); **the compiler's own C moves, 7,782
  functions checked** (§ 10), its binary +9.4%, its `check` not slowed (a code-generation side effect makes it
  retire 13% fewer, § 12), its nesting floor unmoved (§ 12), its warm build § 13.
- `needed_for_self_hosting`: no.
- `argument`: 0 bytes in every Heroes-frame shape I ran, both platforms, plain and sanitized, shape A included. But it
  leaves shape D exactly as the base (81,432 bytes below at exit 0 here, 41,957 on Linux, § 6) and every stack the
  runtime never saw (its window goes wide there); it refuses correct programs the base ran (a small function's frame
  doubles on both platforms, § 3.1, § 11: 300,000 deep on `main` refused), and cuts a quarter of a small stack. (b)
  closes all of that.
- `prediction`: under (c2) `proto-d/dshape.hero` at `-O0` n 4,000 reads a count above 40,000 bytes on both platforms.
- `condition`: (b) failing its conditions below (the signal guard's cases red on (b), or R2 unable to cross the switch);
  then (c2) with (l) and (q) is the fallback, and I would approve it.

- **Where (c)'s word lives** (my brief's item 2): a `_Thread_local` pair, so on Darwin in the thread-local block, in
  the common placement 48 KiB below a coroutine's `lo` (`00-shared.md` § 2.3), **the memory the recursion writes**.
  Heroes frames never reach it under (c2) (0 bytes, every run); C's frames under a callback do (§ 6), and then the next
  check reads what C left: a miss re-derives the window from the ranges, never from the word, but a false pass goes
  unchecked. Under (b) no frame of a callback is below a stack it did not choose.

### (c3), every function of a program with a callback `extern`: **object**

- `argument`: no run of mine where it closes anything (c2) does not; it adds the checks to `main` and every function off
  the callbacks' trees. One function each gets that the other does not: (c3) `helper.setup` (and `main`), (c2) none
  that (c3) lacks; (c2) ⊇ (c1) by construction (its roots are (c1)'s set).
- `condition`: a function a callback reaches that (c2)'s walk misses (a call it does not follow).

### (b), the callback on a stack the runtime owns: **approve, with conditions** (the most robust)

- `section`: §1.12; CLAUDE.md § Precedence (robustness, rank 3; the author's amendment of 2026-11-22).
- `implementation_cost`: runtime **`parts/cbstack.c` 98 lines** (an AArch64 trampoline with CFI for Mach-O and
  ELF, a per-thread pool of 16 guarded 8 MiB stacks, nested entries in place, the guard's bounds swapped), **+2 lines**
  in `heroes_runtime.h`, +1 in `runtime.c`; to write: the x86-64 trampoline (about 25), ASan's fibre annotations
  (about 10), the jump hook's release (about 10), the doors' exemption (about 5). Compiler: **`emit/callback_thunk.hero`
  `written` (`:121`, 472 lines) gains the argument record, the runner and the call: about 30 lines** (the prototype is
  a regex over `i64` signatures, `b.sh`). **Per callback +284 instructions this prototype (+168% `cost_cb`, +151%
  `qsort`); per call 0 (+0.06%, +0.15%)**; Linux per callback +194% to +206% (`cost_cb`), +107% to +117% (`qsort`), two runs (§ 11, § 13).
- `needed_for_self_hosting`: no.
- `argument`: 0 bytes below C's stack in every shape I ran, both platforms, both levels, plain and sanitized: the
  defect's program, shape A (static), the heap stack, `helper.hero`, **shape D** (§ 6), and it needs to have seen no
  stack, so a stack no wrapper saw is closed too; the thunk's own frames need at most 768 bytes of C's stack; a
  recursion that fits 8 MiB now RETURNS where the base wrote and (c) refuses; no per-call cost, no frame growth.
- `prediction`: at (b)'s landing the counting canary (kinds 4 and 6, 16 KiB to 1 MiB, n to 10,000,000) and
  `dshape.hero` read 0 bytes in every run on both arm64 platforms, plain and `--sanitize`.
- **The strongest case against (b)**, which its conditions answer: it moves frames off C's stack, so every rule of the
  signal guard that asks which stack a frame is on (steps 199a to 199o, `00-shared.md` § 3.7) meets a stack it has
  never seen, and none of the 175 cases has run on it; R2 crosses the switch only with CFI and a stop by identity (§ 2:
  0 of 3 otherwise); the compiler's own 256 MiB thread would move (§ 10); C that the callback calls and that compares
  `sp` with its own block (a library's own overflow check, ASan's fibre bounds) sees a stack not its own (unrun); and
  each callback suspended inside C holds a stack (8 MiB of address space and two mappings each: Linux's
  `vm.max_map_count`, **262,144 in the image**, read there by `cat /proc/sys/vm/max_map_count`, bounds them near
  131,000; a host's own default may be lower, unread).
- `condition` (each a run at the landing; any one red turns me to (c2) with (l) and (q)): (1) **the runtime's own doors
  (`hero_thread_spawn`, `_sized`) do not switch**, so the compiler keeps its 256 MiB thread (§ 10): its nesting floor
  13,000 to 14,000 terms unchanged; (2) **CFI on every switch, and R2's stop by the frame's identity**, 3 of 3 slots in
  `fu.c`'s six layouts on both platforms (§ 2); (3) **`fixedbugs-26`, `-27`, `-28` (175 cases, 2871's 7 among them)
  green on (b)'s runtime at four levels, both platforms** (unrun here: it needs a compiler emitting (b)'s thunks); (4) a
  jump out of a callback releases its stack and restores the thread's bounds; (5) ASan's fibre annotations on the
  switch; (6) the pool's bound, and its sentence (§ 15); (7) the per-callback cost brought under the prototype's,
  measured.

### Step 199j's stop: **approve as it stands, with a condition** (the secondary question)

- `section`: design.md §4.17 (a stop says why, in words a reader can act on); §1.12.
- `implementation_cost`: none new (`sigguard.c:600-622` stands).
- `argument`: its seven cases pass at four builds each on both platforms (§ 5, § 9); every window I read in which it fires
  lies inside the program's own switch call, which POSIX leaves undefined in a handler. Under this defect it neither
  holds nor lies: it reads the record the recursion overwrote and the program hangs or dies at 138 (§ 5), which (b)
  removes for a callback's frames and (l) would remove for C's.
- `prediction`: `fixedbugs-2871`, 7 passed, on both arm64 platforms at (b)'s landing.
- `condition`: a POSIX-defined program it refuses (the carved-stack shape of § 5, unrun).
- Its sentence: § 15.

### (e) object (closes nothing alone, § 8); (n) object ((c) with fewer checks, (c)'s holes); (p) ruled out (4 registers);
### (k) ruled out; (m) ruled out (tells after); (l) and (q): owed beside (c2), not beside (b) (§ 8)

## 15. Does a sentence of the spec move? (`00-shared.md` § 7; read, the measurements cited)

- **(b)**: makes § 9's *"running out of stack aborts"* (`spec/heroes-spec.md:340-341`) true on every stack a callback
  is entered on, the thunk's own frames aside (at most 768 bytes of C's stack, measured, like any C function's). **It
  holds a runtime stack for every callback entered from a stack not the runtime's and not yet returned** (suspended
  by a yield, or abandoned by a jump the hook does not see), a resource panel 259's held-memory sentence (§ 13, `:438`)
  does not name, and a bound on them is a new abort: **a sentence of § 13 is owed**, the spec-warden's to price.
- **(c2)**: § 9 stays false for C's frames under a callback (§ 6) and on a stack the runtime never saw: it would need
  the sentence the shared brief's § 7 describes, or (b).
- **Step 199j's stop**: its words reach the reader of a program whose Heroes code is correct, and no sentence states
  them, beside panel 258's that one does: **a sentence of § 13 is owed in my reading**. Every window I read in which
  it fires lies inside the program's own call of `swapcontext` or `setcontext` (§ 5).

## 16. Windows and Linux x86-64, by reading (a choice: the box was free until about 12:50 and nothing of mine ran there)

- **Windows**: no fibre is wrapped (`grep -rln "CreateFiber\\|SwitchToFiber\\|ConvertThreadToFiber" runtime/
  selfhost/`: nothing, the shared brief's § 3.2, re-run by the critic). (c2) there finds no range for a fibre's stack
  and its window goes wide: today's behaviour. (b) needs a trampoline per Windows architecture whose unwind data
  (`.pdata`/`.xdata`, not DWARF's CFI) describes the switch for SEH: unread, unrun, a cost of its own.
- **Linux x86-64**: (b)'s trampoline with the same three `DW_CFA` expressions over `rbp` (about 25 lines); (c2)'s
  window read through `%fs`, inline. Both unrun (the emulated image is the CI's leg).

## 17. Ran and read, apart

**Ran**: everything with a command and a file in §§ 0 to 13. **Read, not run**: (n) function by function; (l) and (q)
built; the correct program 199j's stop might refuse (§ 5); (b) against `fixedbugs-26`/`-27`/`-28` (needs a compiler
emitting (b)'s thunks); a compiler built with (b)'s thunks (§ 10's inference); Windows; Linux x86-64; OpenSSL 4.1.0's
fibre; `hash` on a deep value.
