# Panel 263, the ffi-pragmatist's report

Written as it went, 2026-11-27 from 11:13 to about 12:12, when the API limit stopped this seat; resumed at 15:36 on
the coordinator's word, only to finish this file and copy it (no run after the stop), in the detached worktree `.claude/worktrees/263-ffi-pragmatist` at
`f9f42fcc4` (`git worktree add --detach … f9f42fcc4`), scratch `.claude/worktrees/263-ffi-pragmatist-scratch/`, `TMPDIR`
there. The seed's compiler built there, `real 57.40 user 23.83 sys 1.03`, `sources 1e1eb23a1927d35b` (the brief's).
The load on this Mac: `uptime` 58.89 at 11:15, 78.67 at 11:17, on 8 cores: every duration below is said with its load.
Programs: `docs/panel/263-reports/programs/ffi-pragmatist/`. **Ran** and **read** are kept apart.

## 0. Where the brief was false or silent (first)

- `/opt/homebrew/opt/openssl@3`, `@3.3`, `@3.4`, `@3.5` and `@3.6` are **one install**: every one is a link to
  `../Cellar/openssl@3/3.5.9` (`readlink`, `OPENSSL_VERSION_STR` of each `opensslv.h`). So "3.5.x here" is 3.5.9 alone,
  and the brief's "`@3.3` to `@3.6` and `@4` are all in `/opt/homebrew/opt`" names two libcrypto releases, not five.
- **Silent**: route (a)'s row says nothing of **Darwin's `makecontext` zeroing the whole stack it is handed** (§ 2,
  `lldb`), which makes a guard placed in the runtime's wrapper as built (a tail branch, `rebind_jump.c:418`) fault inside
  `makecontext` itself.
- **Silent**: route (a)'s row reads a page as a small thing; **on this Mac a page is 16 KiB**, half of OpenSSL's default
  fibre (§§ 2, 6).
- **Silent**: the routes' rows ask what C does with its block (free, pool, `read`) and not **what C does with its stack
  from the stack pointer**: a library measuring or copying its coroutine's stack from `sp` (libtask's `needstack`,
  libco's shared stacks, as recalled) is what decides (b) and (h) (§ 5).
- **Partly false**: *"Linux tells it `panic: stack exhausted`"* is the reproducer's layout; **OpenSSL 3.5.7's default
  fibre on Linux comes from the heap, and a Heroes job 6 KiB past it dies of `SIGBUS` with nothing said** (§ 16).
- `adj.c`'s 16 KiB row: **128 KiB apart** in my run on this Mac, not 4 MiB (a layout of the run, § 11).
- **Not done by me**: the four Linux switch-family cases red at step 199j's base (§ 10).

## 1. The defect in a real binding: OpenSSL's ASYNC job on the library's DEFAULT fibre (ran)

`ofib.h` + `ofib_main.c`: **C alone, no Heroes code.** `ASYNC_get_mem_functions` reads the library's default stack
allocator; a pass-through calls it unchanged and records the size it asks and the block it gets, and copies the 512 KiB
under the block through the kernel (`mach_vm_read_overwrite`, `process_vm_readv`) before the job runs; after the job, the
bytes changed are counted. The job's function recurses `n` deep in C frames of about 256 bytes (`volatile char[224]`).
`clang -O0 -g -Wall -Wextra`, accepted with no warning, against each release's own `async.h`.

| | this Mac, libcrypto **3.5.9** | this Mac, libcrypto **4.1.0** | Linux arm64 image, libcrypto **3.5.7** |
|---|---|---|---|
| the default stack, read from the run | **32,768 bytes asked and given**, from `malloc` (a `rw-` 4 MiB region of the allocator, the block at its first byte) | **the same: 32,768, `malloc`'s** | **32,768, from a 132 KiB `rw-` mapping at `0xaaaa…`** (`/proc/self/maps`), the block about 11 KiB above its start: the address range of glibc's `brk` heap, an inference from the address, the mapping's name unread |
| the page below the block | **`rw-`**, the region below | **`rw-`** | **`rw-`**, the same mapping, 8 KiB of it read before the hole |
| `n` 110 (ends inside, the answer right) | 81 bytes below changed, lowest 1,064 | 97, lowest 1,064 | 192 changed, lowest 5,296 (the library's own heap traffic: `n` 10 changes 191) |
| `n` 130 / 150 / 200 (a few KiB past) | 668 / 1,283 / 3,013 changed, lowest 7,144 / 13,224 / 28,424; **exit 0, the right answer** | 808 / 1,543 / 3,479; **exit 0, right** | `n` 130 exit 0 right; **`n` 150 and up: exit 139, nothing said** (the mapping's start about 11 KiB below) |
| `n` 1,000 | **28,043 changed, lowest 271,624 below a 32 KiB stack; exit 0, right** (253577) | 33,343, lowest 271,624; exit 0, right | 139 |
| `n` 4,000 / 20,000 | **58,155 / 58,132 changed through the whole 512 KiB read; exit 0, right** | `n` 4,000 exit 0 right; `n` 20,000 **exit 138, nothing said** | 139 |
| `n` 100,000 | exit 138, nothing said | exit 138 | 139 |

The right answers by `python3 -I` (`r = r*3 % 1000003 + n % 7`, the reproducer's): 434904, 148188, 761615, 435110,
253577, 538369, 532635 for 110 … 20,000, each printed by the run. **So the critic's item 11 is settled: 4.1.0's default
fibre is still a 32 KiB `malloc` block with writable memory under it; its `_mmap` and `_mprotect` imports guard nothing
of the fibre.** And **the defect is C's own before it is Heroes'**: a correct-looking OpenSSL program with a recursing
job, no Heroes code anywhere, returns the right answer at exit 0 having written a quarter of a megabyte under its stack on
this Mac (3.5.9 and 4.1.0), and on Linux writes the heap's earlier chunks, every object `malloc`'d before the fibre, then
dies of `SIGSEGV` saying nothing.

## 2. Route (a), and (a4), in C alone (ran, both platforms)

`ra.c`: the runtime's `makecontext` wrapper modelled as `ra_makecontext`, which makes the lowest whole page inside
`[ss_sp, ss_sp + ss_size)` `PROT_NONE`; (a4) as `ra_free` / `ra_realloc`, which take the guard off before the
allocator sees the block. Then what a **correct** C program does with that memory after its coroutine ended, one mode
each (the file's head lists them). A fault prints whether it landed inside a guard the wrapper placed.
`sweep.sh` runs every mode at 16 KiB, 64 KiB, 256 KiB, 1 MiB and 8 MiB, `-O0` and `-O2`. Clang accepted it on both
(`-Wall -Wextra`; Darwin's three `ucontext` deprecations aside).

**First finding, by lldb: on this Mac the guard cannot be placed in the wrapper as the runtime's wrappers are built.**
Darwin's `makecontext` **zeroes the whole stack it is handed**: `bzero(uc_stack.ss_sp, uc_stack.ss_size)` at
`libsystem_platform.dylib`'s `makecontext + 68` (`lldb`: the fault `EXC_BAD_ACCESS (code=2)` in `__bzero + 32`, `lr` =
`makecontext + 72`, `x0` the guard's first byte; the disassembly loads `x0` from the context's `+0x8` and `x1` from its
`+0x10`, `ss_sp` and `ss_size`). So a guard placed before the real `makecontext` faults inside it, and **the runtime's
Darwin wrapper (`rebind_jump.c:418`) tail-branches to the real one** (`br x16`) because Apple's arm64 ABI passes a
variadic argument on the stack: a guard placed *after* it needs the wrapper to call and return, copying `argc` stack
words, or the guard deferred to the first switch into the context. The rest of this section places it after.

| mode (a correct C program) | this Mac (16 KiB pages), 64 KiB to 8 MiB | Linux arm64 (4 KiB pages) |
|---|---|---|
| `deep`: a recursion past the stack, canary below | **stops in the guard, 0 bytes below written** (`-O0`) | **stops in the guard, 0 bytes below** (`-O0`, every size) |
| 16 KiB stack | **no whole page inside with one left: no guard at all**, `deep` wrote 3,942 / 4,092 bytes below | guard placed |
| `free` after the coroutine ended | ok | **faults in the guard at 16 and 64 KiB** (glibc's heap chunks) |
| `reuse`: free, `malloc` the same size, write it | **faults in the guard** (`-O0`; `-O2` deleted the dead `malloc`) | **faults** at 16 and 64 KiB |
| `realloc` to twice | **faults in the guard** (it copies the old block) | **faults in the guard at 256 KiB and up** (`mremap` carries the `PROT_NONE` page into the new block) |
| `read()` from `/dev/zero` into the block | **`-1`, `EFAULT`**, no fault: a correct program handed an error | **a short read: 4,080 of 262,144 bytes**, no error |
| `pool`: the block a slice of C's pool, `memset` for its next use | **faults** | **faults** |
| `paint`: high-water mark read from the bottom after | **faults** | **faults** |
| `carved`: the stack an array in a frame that has returned; the thread then recurses | **faults in the guard** (`-O0` 256 KiB): the guard stays in the thread's own stack | **faults in the guard** (`-O0` 256 KiB) |
| `static`: the stack a static array, `memset` for the next coroutine | **faults** | **faults** |
| `free4`, `reuse4`, `realloc4` (a4) | ok | ok |

**So (a1) makes correct C programs fault, on both platforms, in every way C goes on owning its memory, and (a4) repairs
only the three that pass through the allocator's own entry points.** `pool`, `paint`, `read`, `carved` and `static` stay
broken under (a4), and `read` breaks with no fault at all: an `EFAULT` on this Mac, a short read on Linux. `carved` is the
shape of the tree's own goldens (`fixedbugs-2853-*-carved-*`, panel 262's `mj_carved`): the guard outlives the frame
and lands in the thread's own stack. **That is a soundness veto on (a1) and (a4) (CLAUDE.md § Precedence rank 3,
design.md §1.12: the runtime's protection makes a correct program fault).**

**(a3), the page against C's stack on this Mac**: a page is 16 KiB here (`sysctl hw.pagesize` 16384). OpenSSL's default
fibre is 32 KiB (§ 1): a guard inside it takes **half the stack C sized** when the block is page-aligned, and is
impossible when it is not (no whole page inside with a page left). Either a correct job using 17 to 32 KiB of stack now
faults (a correct program refused), or OpenSSL's default fibre keeps no guard (the defect stays on the one real binding
this sitting names). Measured on the library's own block in § 1: 3 of 4 runs page-aligned (the 4 MiB region's first
byte), 1 at `+0x8000`.

## 3. The defect in a real binding, with a Heroes job (ran, this Mac; Linux in § 16)

`ofib.hero` + `ofib.h`, built by my stage 2 (`sources 50e8403d5a885325`, `real 465.10` at load 50 to 78), linked by
`pkgconfig "libcrypto"` to **4.1.0** (`otool -L`: `openssl@4/lib/libcrypto.4.dylib`); the job's function is the
program's `deep` (the reproducer's), on the library's default fibre, the pass-through counting the bytes below. Two runs
each; the right answers by `python3 -I`.

| build | `CO_N` inside | a few KiB past | far past | 10,000,000 |
|---|---|---|---|---|
| `-O0` | 100: 0 changed, right | **200: exit 0, right (435110), 1,409 / 1,442 bytes below changed, lowest 5,976** | 2,000: `panic: stack exhausted in ofib.deep`, 134 | told |
| `-O2` | 600: 0, right | **1,200: exit 0, right (280890), 2,330 / 2,511 changed, lowest 5,792** | **20,000: exit 0, right (532635), 243,318 bytes changed, through the whole 512 KiB read, 2 of 2** | told |
| `--sanitize -O0` | 100: right | 200: ASan's `stack-overflow`, 134 | ASan | ASan |
| `--sanitize -O2` | 600: right | **1,200: exit 0, right, 1,693 / 1,880 changed below: ASan silent** | 20,000: ASan's `stack-overflow` | ASan |
| **route (o)**, `OFIB_GUARDED=1`: `ASYNC_set_mem_functions` with 256 KiB `mmap`ed over a `PROT_NONE` page, every build | right | right (the stack is larger) | **`panic: stack exhausted in ofib.deep`, 134** (ASan's `stack-overflow` sanitized) | **told** |

**So the defect is in the one real binding this sitting names, at both levels, with the sanitizer silent at `-O2`**: a
Heroes job on OpenSSL 4.1.0's default fibre returns the right answer at exit 0 having changed a quarter of a megabyte
below a 32 KiB stack. Whether the told end at 2,000 / 10,000,000 is told is the layout's: in these runs 16 KiB below the
block was readable and then a hole (`16384 readable bytes` in the snapshot of most runs, 524,288 in one at 600 and 1,200).

## 4. Route (b) from the C ABI's side (ran, both platforms)

`cbs.c`: the switch in arm64 assembly for both systems (`cbs_call_on`: `stp x29, x30`, `mov x29, sp`, `mov sp, <new
top>`, `blr`, `mov sp, x29`, with CFI saying the CFA is `x29 + 16` throughout, so a frame-pointer walk and a DWARF
unwinder cross the switch); thunks written as an emitter would, each with the callback's own C signature, packing its
arguments into a record on C's stack and running the body on an 8 MiB `mmap`ed stack over a `PROT_NONE` page; a nested
entry runs where it is; a per-thread free list. Clang accepted it on both, `-Wall -Wextra`, no warning (Darwin's
`ucontext` deprecations aside).

| test | this Mac, `-O0` / `-O2` | Linux arm64, `-O0` / `-O2` |
|---|---|---|
| `wide`: 10 integers (two on the stack), 2 doubles, a 24-byte record by value, a 24-byte record returned through `x8` | **55.50 2.50 789.00, right**; the body's frame on the runtime stack | right |
| `alt 10`, Heroes → C → Heroes … on a 256 KiB `malloc`'d coroutine, canary below | ended right, **0 bytes below** | the same |
| `alt 1,000,000` | **`-O0`: stops in the runtime stack's guard, 0 bytes below**; `-O2`: ends, right, 0 below (8 MiB was room) | **stops in the guard, 0 below**, both levels |
| `deep 100,000` / `1,000,000` (the defect) | told in the guard, **0 below** (`-O0`); `-O2` 100,000 ends right (969147) in 8 MiB | told in the guard, 0 below |
| `yield 100`: the callback `swapcontext`s away inside it | **100 held** while paused; **0** once resumed; **100 held for 100 never resumed** | the same |
| `jump`: `longjmp` out of the callback to C's `setjmp`, 100 times | **100 held, hook not told; 0, hook told** (the hook frees every held stack entered below the target frame) | the same |
| the thunk's own frames on C's stack, below the caller's `sp` | **≤ 256 bytes warm** (the probe's floor), **1,408 / 1,392 at the thread's first callback** (the `mmap` that makes its stack) | ≤ 256 warm; 1,168 / 1,120 first |

**Under `--sanitize`** (ASan, `-O1`, both platforms): with no fiber annotation the switch draws **no false report** on
`qsort`, `wide`, `alt`, `deep` or `yield` (Linux prints its generic `makecontext` warning, and LeakSanitizer reports the array my `qsort` test never frees, exit 1, the test's own leak); **a jump out of the callback
does**: this Mac, `ASan is ignoring requested __asan_handle_no_return` then `CHECK failed: asan_thread.cpp:369`, exit
134; Linux, the warning, exit 0. Annotated (`__sanitizer_start_switch_fiber` / `finish` both ways): every test clean but
the jump, which on this Mac reports a **false `stack-buffer-overflow`**, exit 134; and the annotation costs 3.7 times
(`qsort` 100,000: 41.55 ns a comparison plain-switched, 154.94 annotated). **So (b) under the sanitizer owes the jump
hook the annotation's end**, as the runtime's hook already clears shadow today (steps 199e to 199o): the
compiler-engineer's side.

**The cost per callback** (best of five after the first, the program's own `clock_gettime`):

| | this Mac, load 85 to 93 on 8 cores | Linux arm64 (the VM's load 0.7) |
|---|---|---|
| 20,000,000 calls of a trivial callback, `-O2`: direct / switched | 4.70 / 12.85 ns (**+8.2**) | 4.08 / 8.44 ns (**+4.4**) |
| the same, `-O0` | 10.73 / 50.41 ns (+39.7) | 10.44 / 22.66 ns (+12.2) |
| `qsort` of 1,000,000 `i64`, a comparison, `-O2` | 10.96 / 20.42 ns (**+9.5**) | 12.73 / 18.52 ns (**+5.8**) |
| the same, `-O0` | 21.87 / 52.80 ns | 19.65 / 37.54 ns |
| **today's Heroes thunk**, this Mac, my stage 2: `cost_cb.hero` 20,000,000 calls `user` | `-O2` 0.24 s (**12 ns a call**), `-O0` 0.81 to 0.88 s | unrun |
| **today's**, `hq.hero`: `qsort` 1,000,000 `i64` through a Heroes comparator | `-O2` **20.92 to 22.83 ns** a comparison, `-O0` 87 to 108 | unrun |

So the switch, as I built it (a `_Thread_local` list, which Darwin reaches through `tlv_get_addr`), adds about **a third
to two thirds of today's whole callback** at `-O2` (8.2 ns on 12; 9.5 on 21). A cost, written, not a reason.

## 5. The binding that reads its own stack block, under (b) and (h) (ran)

**`csh.hero` + `csh.h`, a Heroes program, today's runtime, this Mac**: a shared-stack coroutine library modelled on
Tencent libco's shared-stack mode **as recalled** (the coroutine's stack pointer taken from the stack at its yield,
`[stack_sp, bp)` of ONE 256 KiB `malloc`'d block copied out before the other coroutine runs there and back before it
resumes), and libtask's `needstack` **as recalled** (`(char*)&t - (char*)t->stk < 256 + n` → *"task stack overflow"*,
abort). The coroutine's body is the program's function; it asks `needstack(1024)`, then yields twice **from inside the
callback**, keeping a value across each yield.

| | `-O0` | `-O2` | `--sanitize -O2` | `--sanitize -O0` |
|---|---|---|---|---|
| **today's runtime** | **exit 0, `both: 10022002`, right, 3 of 3** | **right, 3 of 3** | right, 3 of 3 | 134, ASan `SEGV`/`BUS`, 3 of 3 (my model's 1 KiB margin below `sp` is a guess; unread) |
| **the same binary, the function entered through (b)'s switch** (`CSH_B=1`: the header calls it on an 8 MiB `mmap`ed stack with a guard, the switch made before the program's code runs, as (b)'s thunk would) | **134, `task stack overflow`, 3 of 3** | **134, 3 of 3** | | |

The same library in C alone (`cs.c`, three runs each, both levels, this Mac): **today's shape right 3 of 3; (b) and (h)
crash 3 of 3** (139 and 138: the library computes the coroutine's stack as **about 500 GB**, `[stack_sp, bp)` with
`stack_sp` on the runtime's stack, and `memcpy`s it), and `needstack` **aborts, 3 of 3, under both**. The tree's own
copy-stack goldens (`fixedbugs-coroutines-copied-out-and-in-of-one-shared-region.h`, panel 259's `cpo.h`) copy the
**whole** region, never `[sp, bp)`, so they would not see this (read; not run under (b)).

**So route (b), applied to every callback, refuses a correct program this runtime runs today**: any C a callback calls
that measures or copies its own coroutine's stack from the stack pointer runs on a stack that is not its block. That is
the brief's veto, *"a correct program refused"*. And (h) breaks the same two shapes for every coroutine, a callback or
not.

**Against it, a binding with its own stack check that (b) does not break**: CPython, embedded (`py_on.c`, the
library's `repr()` of a list nested 300 / 3,000 / 30,000 deep, a recursion through C that CPython guards itself), this
Mac, 3.13.18, 3.14.10, 3.15.2:

| | the thread's own stack | **(b)'s 8 MiB runtime stack** | a 256 KiB `malloc`'d coroutine (today's shape) |
|---|---|---|---|
| 3.13.18 | 602 / 6002 / `RecursionError` at 30,000 | **the same** | 602 / **exit 139** at 3,000 / **exit 138** at 30,000 |
| 3.14.10 | 602 / 6002 / 60002 | **the same** | 602 / 6002 / **exit 139** at 30,000 |
| 3.15.2 | 602 / 6002 / 60002 | **the same** | 602 / 6002 / **exit 139** at 30,000 |

CPython's own C-stack check neither refuses the runtime's stack nor protects a coroutine's: on the `malloc`'d block it
dies of `SIGSEGV`/`SIGBUS` saying nothing, a second binding with the defect's shape and no Heroes code in it.

## 6. (a2), on OpenSSL's default fibre (ran, this Mac, C alone)

`ofib.h` with `OFIB_A2=1`: at the job's entry (the callback's), the lowest whole page inside the library's own 32 KiB
block made `PROT_NONE`, given back when it returns, as (a2) would at a callback's first entry onto a range. `ofib_main.c`'s
job recursing `n` deep in about 272-byte frames, three runs each, 3.5.9 and 4.1.0:

| `n` (about) | without the guard | with (a2)'s guard |
|---|---|---|
| 40 (10 KiB) | right | right, 3 of 3 each release |
| **60 (15 KiB)** | **right** (§ 1: `n` 110 ends inside the 32 KiB) | **exit 138, 3 of 3 each release** |
| 80 (21 KiB), 100 (26 KiB) | right | **138, 3 of 3** |

Every run: `a2 guard [lo, lo + 16384): 16384 of the stack's 32768 bytes left above it` (the block 16 KiB-aligned in all
24). **A correct job that fits the stack OpenSSL sized faults under (a2) on this Mac**, at both releases: the page is
half the stack. Under the runtime it would end in its guard's words, *stack exhausted*, on a program whose stack did not
run out: §4.17's false message as well as §1.12's correct program refused. **(a2) is vetoed with (a1)**, on this
binding alone; its other holes (a yield or a jump inside the callback leaving the guard standing, `cbs.c`'s `yield` and
`jump` shapes) are not needed.

## 7. (g)'s judgement priced, and (i)'s premise counted (ran, this Mac; Linux in § 16)

`probe_cost.c`, 200,000 questions each, `-O2`, three runs, load about 68: **`mach_vm_read_overwrite` of the aligned word
below a stack, 1.5 to 2.9 µs a question** (a one-byte read at an odd address fails on Darwin whatever is mapped: the
first version answered *unreadable* 200,000 of 200,000 times, so the runtime's word-aligned reads are the right shape);
`mincore` 0.3 to 1.4 µs and **useless**: it says *mapped* of a `PROT_NONE` page, 200,000 of 200,000. So (g) at every
callback is **about a hundred times today's whole callback** (12 ns, § 4), or a cache keyed by the range, which a
later mapping falsifies: the judgement is of the layout at one instant (§ 2.6 of the shared brief).

**(i) on this Mac** (`fi.c`, 30 fresh processes a size, the page under a `malloc`'d block, then after one more `malloc` and
one `mmap`): **never free**, 30 of 30 *mapped* at 16 KiB, 64 KiB, 256 KiB, 1 MiB and 8 MiB, with 0 or 4 blocks of the
size made first: so `MAP_FIXED_NOREPLACE`'s Darwin twin has nowhere to put a page. What is below is the allocation
history's: readable in 30 of 30 at 64 KiB and up, readable 22 to 25 of 30 at 16 KiB and of no access in 5 to 8; and
`dbg.c`, a program whose first act is one 256 KiB `malloc`, finds that block at a 4 MiB region's first byte over **a
reservation of about 12 GiB of protection 0**, 4 of 4: the same size, a guard below for free in one program and
writable memory in the next. **(i) is not a route on this Mac.**

## 8. (f) counted, by a run of the tree's own goldens (ran, this Mac)

**The instrument**: a runtime copy (`.claude/worktrees/263-ffi-pragmatist-scratch/rt-f`, never my copy's `runtime/`)
whose `hero_callback_entered` first asks, under `P263_PROBE`, which stack the frame lies on (the range `makecontext` was
handed, through the runtime's own `hero_rebind_live_range` / `hero_sig_live_range`; else the thread's own, from
`pthread_get_stackaddr_np` / `pthread_getattr_np`; else none) and whether the aligned word below that stack's lowest byte
is readable through the kernel (`mach_vm_read_overwrite` / `process_vm_readv`), one line a callback. `count.sh` builds
each of the **105** `run` goldens the brief's `ls … | grep -iE "fibre|fiber|coroutine|openssl-async"` names, at `-O0`,
against it, runs it once (`timeout -k 5 30`, `head -c 400000`): `real 258.00`. The table per case:
`programs/ffi-pragmatist/f-count-mac.txt`.

- **50 of 105 end 134 today** (their own expected panics and refusals: not counted).
- **55 end 0 today**; of them **26 enter a callback on a range whose byte below is readable, or on no known stack**,
  and route (f) refuses each: **26 goldens red**, among them `fixedbugs-openssl-async-jobs-on-heap-fibres-…`,
  `fixedbugs-2853-openssl-async-jobs-on-carved-fibres-…`, the six `…-on-carved-stacks-made-by-a-makecontext-…-found-…`,
  `fixedbugs-coroutines-on-stacks-carved-from-the-threads-own-…`, two `…copied-out-and-in-of-a-heap-region…`,
  `fixedbugs-coroutines-switched-by-a-librarys-own-switch-…`, `fixedbugs-2848-…-twelve-arguments-…`. **A carved stack is
  refused too**: below a range carved from the thread's own stack lies more of the thread's stack, readable, though the
  thread's guard is further down: (f)'s question cannot tell *a guard further below* from *none*.
- 29 run with every entry on a stack of no access below (the thread's own, or a guarded range), or with no entry.

**(f) is vetoed**: 26 correct programs the tree runs today, red. (g), (b) on (f)'s set, inherits (b)'s refusal of § 5's
binding on exactly the set where that binding lives (a `malloc`'d block, readable below).

## 9. (d), `sigaltstack` (read)

**No arrangement stops the first write below a stack, by reading**: the write is an ordinary store into a page the MMU
lets the process write, so no signal is raised, and `sigaltstack` only chooses where a handler runs once one is. The
runtime's alternate stacks are already in place (`stack.c:1324`); they decide where the guard's handler runs after the
fault the defect never produces. Nothing to run: the claim is about the absence of a signal, which the canary counts of
§ 1 and § 3 already are (bytes written, exit 0, no handler entered).

## 10. Step 199j's stop (issue 2871), the secondary question (ran, both platforms)

**Its seven cases**, `./heroes-next run tests/harness/main.hero -- ./heroes-next run fixedbugs-2871`, my stage 2:
**`run (only fixedbugs-2871): 7 passed, 0 failed`**, `real 242.93` (the form judges each at `-O0`, `-O2`, `--sanitize`
and `--sanitize -O0`, `suite_run.hero:239-256`). **Linux arm64**, my stage 2 built in the image, the same command:
**`7 passed, 0 failed`**, 352 s. **The four switch-family cases red on Linux at the step's base: not named by me.** The
harness's `fixedbugs-27` (95 cases) and `fixedbugs-28` (39) take about 35 to 50 s a case there (2871's seven took 352 s),
an hour and more in the one container a seat may hold, and I stopped that run before it began to give the container to
§§ 16's runs; the compiler-engineer's list stands alone in the synthesis, said so.

**A binding shape a correct program might be stopped by, built** (`gt.h`, `gt_main.c`, `gt.hero`): a **preemptive
green-thread scheduler**, three threads on `malloc`'d stacks yielding cooperatively by `swapcontext`, and a `SIGALRM`
every 50 µs whose handler switches to the next thread by `swapcontext` (the textbook preemption); each thread checks a
magic word on its frame and that the scheduler names it after every switch. Three disciplines, two seconds, three runs,
both levels, C alone and the same header in a Heroes program (no callback):

| | **mode 0**: the handler switches, nothing masked | **mode 1**: the yields mask `SIGALRM` around their switch, the handler still switches | mode 2: the handler only counts |
|---|---|---|---|
| **C alone**, `-O0` / `-O2` | **dies: exit 138 / exit 133, 3 of 3 each** | **0 checks failed, 3 of 3** | 0 failed |
| **Heroes**, `-O0` / `-O2` | **`panic: a signal handler called \`swapcontext\` naming a context that the code the signal interrupted was itself switching from or to …`, 134, 3 of 3** | **0 failed, 3 of 3: the stop lets the correct discipline through** | 0 failed |

**Linux arm64**, the same three runs: C alone mode 0 **exit 139 (`-O0`) / hung, 124 (`-O2`)**, 3 of 3; Heroes mode 0
the stop's panic, 3 of 3; modes 1 and 2 right in C alone; **Heroes mode 1 at `-O0` stopped once in three**. That run
sent me back to mode 1, which leaves a window open: the threads' first contexts carry an unblocked mask, and glibc's
`swapcontext` installs the target's mask before it loads the target's registers, so a `SIGALRM` can land inside a switch
into a fresh thread. **Mode 3 closes it** (every thread's first context made with `SIGALRM` in its `uc_sigmask`); ten
runs each, both levels, both platforms:

| | mode 1 (the window open) | **mode 3 (the window closed)** |
|---|---|---|
| C alone, this Mac | 10 of 10 right at each level | 10 of 10 right |
| C alone, Linux | `-O0` 10 of 10; **`-O2` 9 right, 1 exit 139** | **10 of 10 right, each level** |
| Heroes, this Mac | `-O0` 10 of 10; **`-O2` 9 right, 1 the stop's panic** | **10 of 10 right, each level** |
| Heroes, Linux | 10 of 10 each level (the three-run batch: 1 of 3 stopped at `-O0`) | **10 of 10 right, each level** |

So mode 1's race is real (C alone dies of it, 1 in 10 on Linux at `-O2`), the stop tells it where it lands, and **the
correct discipline runs 40 of 40 under the stop**.

**So *"C alone dies too"* holds for the shapes I could build, on both platforms**, and the stop tells, in words, the program C
alone dies of with a bus error or a trap; the correct discipline (a scheduler masking its signal across its own
switches, which is what makes a preemptive scheduler correct at all) runs. **The shape the brief names, a handler switching
to a context the interrupted switch has already finished saving into**, I could not build as a correct program: the
handler would have to know how far libc's own `swapcontext` got (its `pc` past the save, a fact of each libc's assembly);
no binding I know reads it. A question, not a finding.

## 11. The reproducer itself, my builds (ran)

- **This Mac**, `run.sh <my heroes-next> <scratch>/rs 3 30` (`real 342.80`, load 50 to 90): `malloc-O2` **hung, 124, 3
  of 3**; `malloc-O0` **138, 3 of 3**; `malloc-san` ASan's `DEADLYSIGNAL` (134 five, 1 once); `guarded`, `bare`, `thread`
  told, 3 of 3 each level; `static` **hung, 3 of 3 each level**; the canary the brief's table to the byte (`-O2` 9,000:
  25,941; 10,000: 57,937; `-O0` 1,500: 23,964; 2,000: 112,450; sanitized `-O0` 2,000: 232,195, one run of three ending
  in ASan's `DEADLYSIGNAL` instead). `CO_SAY=1 CO_N=1` once per build: `malloc` at both levels `lo = tls + 0xC000`
  (48 KiB), `static` in the image's data (`0x10502a8c0`), the sanitized builds in ASan's heap. `adj.c`: 64 KiB to 8 MiB and
  the 256 KiB `mmap`, the second **directly above** the first, as the brief; 16 KiB **128 KiB apart** in my run (the
  brief's *4 MiB apart*: a layout of the run).
- **Linux arm64**, the seed's compiler built in the image (`real 72 s`), **stage 2 built in the image (808 s, `sources
  50e8403d5a885325`, the Mac's)**, `run.sh` (82 s): `malloc`, `guarded`, `bare`, `thread` told 3 of 3 each level;
  `malloc --sanitize` exit 1, ASan, 3 of 3; **`static` exit 139, 3 of 3 each level**; the canary as the brief's within
  640 bytes (`-O0` 2,000: 112,451 twice, 111,814 once).

## 12. What a C binding needs under each route, and today's thunk on C's stack (ran)

`td.hero` + `td.h`: 256 KiB painted below C's `sp`, a trivial Heroes callback called through its pointer: **today's
thunk, its question and the function reach 548 bytes below C's `sp` at `-O0`, ≤ 256 (the probe's floor) at `-O2`,
1,528 / 384 under `--sanitize`**, the first call and the second the same. So the frames the brief's item 6 asks after
(the question run on C's stack before any switch) are about half a KiB; under (b) they stay on C's stack unless the
switch comes first, and under (c) they are part of the reserve.

`cdepth.c`, how far below the caller's `sp` a C library's own frames reach for calls a callback commonly makes (this Mac,
first call / again): `snprintf` 1,552; `%.17g %Lf` 2,000 / 1,952; `qsort` 1,528; `regcomp`+`regexec` 1,824;
**`getaddrinfo` 6,128 / 3,280; `dlopen` 15,712 / 8,640**; `strftime` 2,592 / 2,112; `fopen`+`fprintf` 1,440. That is the
total depth of a call, not one frame's size: a reserve (c) leaves below a Heroes entry's check must hold the deepest C a
callback calls before the next Heroes entry, **16 KiB on this Mac for a callback that `dlopen`s**.

## 13. Route (c2) from the C ABI's side, modelled (ran, this Mac)

`cbs.c`'s `deepc`: no switch; at each entry of the recursing function a compare of `sp` with the range the runtime would
know (the coroutine's `malloc`'d block, as a range `makecontext` was handed), refusing only where `sp` lies **inside the
range's lowest 32 KiB** (both bounds, so a range left stale by a switch the wrappers never saw refuses nothing outside
it). Both levels: `n` 100 and 1,000 **end right** (636733, 253577), 0 bytes below, one check a call; 100,000 and
10,000,000 **told, 0 bytes below the stack written**. **Nothing C sees moves**: the thunk keeps its signature, every
frame stays where C put it, no page of C's is protected; so § 2's eleven shapes and § 5's two are today's runs, which
are right (an inference from where the frames are, the `plain` columns of §§ 2 and 5 being its measurement). Its price
per call is the compiler-engineer's to measure (panel 104's ground); what it leaves is a stack no wrapper saw (a
library's own switch, boost.context's `fcontext`, libco's `coctx_swap`), and C's own frames between two Heroes entries,
which § 12 sizes: 16 KiB for a callback that `dlopen`s on this Mac.

## 14. Windows, by reading (a choice, written as one)

`ssh win "cmd /c ver & where clang & …"` at about 11:44 answered `Not enough memory resources are available to process
this command.`, the box held by lane unwinder until about 12:50: I ran nothing more there. By reading only:
`grep -rln "CreateFiber\|SwitchToFiber\|ConvertThreadToFiber" runtime/ selfhost/` finds nothing (re-run), and no route
of the signal guard stands on Windows (`sigguard.c:215-221`, *"none on Windows"*). **Recalled, unrun**: a fibre from
`CreateFiber` gets a stack the system reserves like a thread's, with a guard page, so a recursion there ends in
`STATUS_STACK_OVERFLOW` rather than writing below; a library switching by hand onto a heap block has no guard below, and
on x64 its large frames meet `__chkstk`, which probes against the thread information block's `StackLimit`, so such a
library must move `StackBase`/`StackLimit` at each switch. **What that means per route, by reading**: (b) on Windows owes
the same update of the thread information block at its own switch, or `__chkstk` and structured exception handling
misjudge the runtime's stack; (c2) has no range to check there (no `makecontext`), the system's fibres being guarded
already and a hand switch unseen; (a) and (h) have no `makecontext` to hook. Each a question for a run on the box.

## 15. Verdicts, per route

Every one below rests on a run named in §§ 1 to 13 unless it says *read*. The sections: design.md §1.11 (`:484`, the FFI
is every package's door), §1.12 (`:646`, *"must not segfault and must not corrupt memory"*, *"any C library must be
bindable"*), §4.19 (`:2672`). CLAUDE.md § Precedence rank 3 makes the most robust route win at any cost in size and speed
(the amendment of 2026-11-22), so every cost below is written and none is a reason.

### (a1), (a3), (a4): **veto**
- `section`: §1.12, *"any C library must be bindable"*: a protection placed in memory C owns makes correct C fault.
- `experiment`: `ra.c` (clang: accepted, both platforms, `-Werror` clean); § 2's table: `pool`, `paint`, `carved`,
  `static` fault in the guard on both platforms, at both levels, under (a4) too; `read` gets `EFAULT` (this Mac) or a
  short read of 4,080 of 262,144 bytes (Linux) with no fault; `realloc` and `reuse` fault without (a4). Darwin's
  `makecontext` zeroes the whole stack (`lldb`), so the runtime's tail-branching wrapper cannot place it as built. (a3):
  the page is 16 KiB on this Mac, half of OpenSSL's 32 KiB fibre (§ 6).
- `argument`: the guard outlives the runtime's knowledge of the memory's use; every way C keeps owning its block after a
  coroutine ends trips it, and three of those five never pass through an allocator (a4) could wrap.
- **And in the runtime, on the tree's own goldens** (Linux arm64, ran): a runtime copy whose `hero_sig_made_context`
  (`sigjump_make.c`) makes the lowest whole page inside the range `PROT_NONE` under `P263_A1`, before glibc's
  `makecontext`, which writes only at the top (`linux3.sh`; table `programs/ffi-pragmatist/a1-count-linux.txt`); the
  105 goldens built once at `-O0` and run without and with it: **51 stay 0, 47 stay 134, and 7 move: 4 correct programs
  from exit 0 to exit 139 with nothing said** (`fixedbugs-2706-…-the-frames-that-reuse-an-ended-coroutines-stack-runs`,
  `fixedbugs-2850-…-reuse-a-uc-link-started-coroutines-ended-stack-runs`, `fixedbugs-2851-…-reuse-a-heap-context-
  coroutines-ended-stack-runs`, `fixedbugs-coroutines-copied-out-and-in-of-one-heap-region-over-makecontext-each-keep-
  their-large-values`: frames that reuse an ended coroutine's stack, and a copy-stack library copying its whole region),
  **and 3 told ends become silent** (the three `fixedbugs-2870-…` on a guarded `mmap` stack, from `panic: stack
  exhausted` to exit 139: the fault now lands inside the range, where step 199i's arm does not look). The carved
  OpenSSL golden stayed green in that run (its thread never recursed through where the guard was left).
- `prediction`: landed as modelled, (a1) turns those four goldens red on Linux arm64 at `-O0`, exit 139, and the three
  `fixedbugs-2870-…` cases red by their message.
- `condition`: a run of the five shapes where none faults and `read` returns its full count, on both platforms.

### (a2): **veto**
- `section`: §1.12 (a correct program refused), §4.17 (it would be told *stack exhausted* on a stack that was not).
- `experiment`: `ofib.h` with `OFIB_A2=1` (§ 6): **a correct OpenSSL job using 15 to 26 KiB of its 32 KiB default fibre
  faults, 3 of 3 at every size, libcrypto 3.5.9 and 4.1.0**; without the guard every one is right.
- `argument`: on 16 KiB pages a guard inside a 32 KiB stack takes half of what the library sized for its own jobs.
- `prediction`: OpenSSL 4.1.0's default ASYNC fibre, a job recursing 60 deep in 272-byte frames, ends with a fault on
  this Mac under (a2) and at exit 0, right, without it.
- `condition`: a guard smaller than a page, which the MMU does not offer.

### (b), every callback on a runtime stack, its C calls too: **veto as stated**; its mechanics **approved from the ABI's side**
- `section`: §1.12 (a correct program refused), §1.11 (a library that reads its own stack is a C library that must stay
  bindable).
- `experiment`: `cbs.c` (assembly switch, both systems, clang accepted, `-Werror` clean): **the ABI holds**: 10 integers
  with two on the stack, 2 doubles, a 24-byte record by value and one returned through `x8`, right on both platforms;
  nested entries and the unbounded alternation stop in the runtime stack's guard with 0 bytes below C's coroutine; a
  yield holds one stack per paused callback, 100 for 100, 0 once resumed; a jump out holds 100 without the hook and 0 with
  it; CPython 3.13 to 3.15 run on the runtime's stack exactly as on the thread's. **And it refuses a correct program**:
  `csh.hero`, a Heroes program on a shared-stack library that copies `[sp, bp)` and asks libtask's `needstack`, **right 3
  of 3 at both levels today and `task stack overflow`, 134, 3 of 3, with the function entered on (b)'s stack**, on this Mac
  and on Linux; in C alone the copy computes a 500 GB (this Mac) or 263 KB (Linux) stack and dies, 3 of 3 each.
- `argument`: a C call a callback makes then runs on a stack that is not the coroutine's; a library that measures or
  copies its coroutine's stack from the stack pointer, as shared-stack libraries do, breaks. Cost: +4.4 to +8.2 ns a call,
  `qsort` +5.8 to +9.5 ns a comparison, against today's 12 and 21; ASan needs the jump hook to end its annotation.
- `prediction`: a Heroes program binding Tencent libco in shared-stack mode, whose coroutine body yields from inside its
  callback, ends 0 and right today and aborts or crashes under (b) as stated (libco's `save_stack_buffer` as recalled:
  the historian's to verify).
- `condition`: (b) with **every C call from a callback's Heroes code made back on the C stack the callback was entered
  on** (the shape cgo is recalled to take for C calls: a question for the historian), re-run on `csh.hero` and on the
  unbounded alternation; I have not built it, and in it the C frames of an alternation pile up on C's unguarded block
  again.

### (c2), with (n) as its bound where a call tree is closed: **approve with conditions**
- `section`: §1.11 and §4.19: it changes nothing a C binding sees; §1.12 for what it closes.
- `experiment`: `cbs.c deepc` (§ 13): told with 0 bytes below, right inside, both levels; C's view untouched by
  construction (no switch, no protection, the thunk's signature as today).
- `argument`: the only route measured here that closes the defect on every stack the runtime saw made (`malloc`'d,
  static, carved, OpenSSL's default fibre, a shared-stack library over `makecontext`) and moves nothing C reads.
- `prediction`: under (c2), `csh.hero` ends 0 and right at both levels, and `ofib.hero` with `CO_N` 20,000 at `-O2` ends
  told with 0 of the bytes below OpenSSL 4.1.0's fibre changed.
- `condition`s: (1) the check refuses only where `sp` lies inside the current range's lowest reserve, the range
  re-derived on the slow path, never a bare `sp < limit` (a switch the wrappers never saw leaves a stale limit, and a bare
  compare would refuse a correct program on another stack); (2) the reserve holds the thunk's question (548 bytes at
  `-O0`, 1,528 sanitized, § 12) and the deepest C a callback calls between two Heroes entries (16 KiB for `dlopen` on this Mac, 20 KiB for `regcomp`+`regexec` on Linux, §§ 12, 16);
  (3) a stack no wrapper saw stays outside it, and § 9 says so (§ 17).

### (f): **veto**
- `section`: §1.12 (correct programs refused).
- `experiment`: the instrumented runtime over the 105 goldens (§ 8): **26 of the 55 that run today red**, carved stacks
  among them.
- `argument`: *readable below* cannot tell *a guard further below* from *none*.
- `prediction`: (f) landed turns `fixedbugs-openssl-async-jobs-on-heap-fibres-run-under-the-sanitizer` red on this Mac.
- `condition`: a judgement that tells a carved range from a `malloc`'d one, which (f) as written is not.

### (g): **veto** (it is (b) on (f)'s set, where `csh.hero`'s `malloc`'d block lives), and its probe costs 1.5 to 2.9 µs a callback (§ 7)

### (h): **veto**
- `experiment`: `cs.c h` (§ 5): the shared-stack library crashes 3 of 3, `needstack` aborts 3 of 3, for every coroutine,
  a callback or not; and the runtime would hold a stack per context until a `free` it never sees.

### (o): **approve, as a binding's practice, not as the runtime's route**
- `section`: §1.11: the runtime may not know OpenSSL; Rebel's or a program's OpenSSL binding may.
- `experiment`: `ofib.hero` with `OFIB_GUARDED=1` (§ 3): told at every size past, right inside, every build.
- `prediction`: a binding of OpenSSL's ASYNC that calls `ASYNC_set_mem_functions` with `mmap`ed, guarded stacks before
  its first job turns § 3's exit 0 at `CO_N` 20,000 into `panic: stack exhausted`, on 3.5.7, 3.5.9 and 4.1.0.
- `condition`: none; it reaches only libraries with such a door.

### (i): **not a route** (this Mac: the page below never free, 30 of 30 at every size, § 7; Linux: the word below readable 30 of 30 at every size in `fi.c`, and a free page filled by the next `mmap`, 3 of 3, § 16)
### (d): **none exists** (§ 9, read); (j): unavailable (the brief's); (k): panel 104's ground stands; (l), (q), (m): no objection from the C side, they move nothing C sees; (p): read only, not mine

## 16. Linux arm64 (ran in `heroes-linux-arm64:latest`, `--rm --pull never`, my stage 2 built inside)

**§ 3 on Linux, a Heroes job on OpenSSL 3.5.7's default fibre** (`pkgconfig "libcrypto"`), two runs each:

| build | inside | a few KiB past | far past / 10,000,000 |
|---|---|---|---|
| `-O0` | 100: right (the library's own heap traffic changes about 200 bytes below in the 16 KiB read) | **200: exit 135 (`SIGBUS`), nothing said, 2 of 2** | 2,000 and 10,000,000: `panic: stack exhausted in ofib.deep` |
| `-O2` | 600: right | **1,200: exit 135, nothing said, 2 of 2** | 20,000 and 10,000,000: told |
| `--sanitize` | right | ASan's `stack-overflow`, exit 1 (the fibre in ASan's heap with its redzones) | ASan |
| route (o), the guarded door | right | **right** | **told**, every build |

So on Linux the same binding dies **silently at 6 KiB past**: a `SIGBUS` no handler of the runtime's tells, the
recursion having gone through the heap's first chunks into what lies under it.

**§ 5 on Linux**: `cs.c` plain right 3 of 3; **(b) and (h) crash, 139, 3 of 3** each level (the copied "stack" 263 KB
here: the runtime's mapping just below the block, an inference from that length, unread); `needstack` under (b) and (h) **aborts 3 of 3**; **`csh.hero`
today right 3 of 3 at both levels, `CSH_B=1` (entered on (b)'s stack) `task stack overflow`, 134, 3 of 3 at both**.

**CPython 3.13.5** (the image's `libpython3.13.so.1`, declared by hand): on (b)'s stack exactly as on the thread's
(602, 6002, the same `RecursionError` at 30,000); on a 256 KiB `malloc`'d coroutine **exit 139 at 3,000 and at 30,000**.

**§ 7 on Linux**: `process_vm_readv` of the word below a stack **0.6 to 1.3 µs a question**; `mincore` *mapped* for a
`PROT_NONE` page, 200,000 of 200,000. **A lone 256 KiB `mmap`: the byte below unreadable at the judgement, readable
after one more `mmap`** (the second lands directly below), 3 of 3: (g)'s and (i)'s judgement falsified by one mapping,
measured. `fi.c`: the word below a `malloc`'d block **readable, 30 of 30, at every size, with 0 or 4 blocks made
first**, so (i) has nowhere to put a page in that program either (the reproducer's 256 KiB block had a free page under
it: § 2.3 of the shared brief; the layout of the program, not the size). My `fi.c`'s Linux *mapped* column contradicts
its *readable* one (a bug of my `/proc/self/maps` reading, unread): the readable column is the measure.

**§ 12 on Linux** (`cdepth.c`, first / again): `snprintf` 2,016; `%.17g %Lf` 3,584; `qsort` 2,528; **`regcomp`+`regexec`
19,968 / 19,984**; `getaddrinfo` 7,616; `dlopen` 4,880; `strftime` 1,056 / 544; `fopen`+`fprintf` 2,848. **(c2)'s
reserve on Linux: 20 KiB for a callback that compiles a regular expression.**

**`CO_SAY` and `adj.c` on Linux**: `malloc` stacks at `0xffff…010` with the thread's TLS above them; `static` in the
image's data at `0xaaaa…`; `adj.c` as the brief: 256 KiB to 8 MiB the second **4,096 bytes below**, raw `mmap` **0 bytes
below**, 16 and 64 KiB from the heap ascending.

## 17. Does a sentence of the spec move (the shared brief's § 7)

- **The route I support, (c2) (with (n) where a call tree is closed), needs one.** It makes § 9's *"running out of stack
  aborts"* true on the thread's own stack and on every stack C hands `makecontext` (a static array's and a carved one's
  included), and leaves it false on a stack C switches to by means the runtime never sees (a library's own switch,
  boost.context's `fcontext`, libco's `coctx_swap`, hand assembly), where nothing of no access lies below. Either § 9
  names that edge, or the spec says what Heroes promises there (nothing beyond C's own); the spec-warden prices which.
  Nothing it holds needs § 13's held-memory sentence (no stack is held).
- **(b)**, if it were adopted with the condition of § 15, holds a runtime stack per callback paused or abandoned (§ 4:
  100 for 100) and needs § 13's held-memory sentence to name it, as the brief says.
- **(o)** needs none: it is a binding's choice.
- **Step 199j's stop needs one**, beside panel 258's in § 13: it aborts a program class (a handler's switch naming a
  context of the switch its signal interrupted), no sentence states it, and every shape I built that it stops is one C
  alone dies of or races in (§ 10), so the sentence is the honesty of an abort, not a new refusal of a correct program.

## 18. What a veto here compels

My vetoes ((a1), (a2), (a3), (a4), (f), (g), (h), and (b) as stated) leave the synthesis **(c2), with (n) where it can
bound a tree, as the runtime's route**; **(o) as the practice of a binding with a door** (Rebel's OpenSSL binding first:
its ASYNC jobs on stacks it `mmap`s with a guard); **(l) and (q)** for the honesty of the message on a stack (c2) cannot
reach; **and a § 9 sentence for that stack**. Or (b) rebuilt so that a callback's C calls run on the stack C entered it
on, run against `csh.hero` and the unbounded alternation before it is adopted.
