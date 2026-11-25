# Panel 259, the ffi-pragmatist's report

Started 2026-11-25 08:21 CET by this Mac's clock (`date`). My copy is `.claude/worktrees/259-ffi-pragmatist` at
`3ec775b664c9cf0d6b8bae8c94f407f8a934e3a2` (`git rev-parse HEAD`); long output in
`.claude/worktrees/259-ffi-pragmatist-scratch/`. Programs and scripts in
`docs/panel/259-reports/programs/ffi-pragmatist/` of this copy. Written as I go; **run** and **read** are said apart.

## Status

- 08:21 Mac compilers building (seed, then stage 2) in my copy; x86-64 container `p259-ffi-pragmatist-1` started
  (`uname -m` x86_64, Debian GLIBC 2.41-12+deb13u3, clang 22.1.8, emulated), compilers building inside it from a
  `git archive 3ec775b66`.
- Measured so far: `/usr/include/openssl/async.h` present in the x86-64 image, `/usr/include/boost` absent.
- 09:17 both compilers built: Mac `sources 05186f2b38e77a97` (seed) and `sources 117362877b136f67` (stage 2); the
  x86-64 container the same two keys (07:23 to 07:51 UTC, `build.log`). The repair's keys.

## Facts of the brief found wrong or short (run)

1. **libcrypto does not import `swapcontext` on this Mac.** § 4.7 and the ffi brief carry panel 257's `asyncjob.h`
   head (*"libcrypto imports makecontext, swapcontext and setcontext: nm -u"*). `nm -u
   /opt/homebrew/opt/openssl@3/lib/libcrypto.3.dylib | grep -E 'context|jmp|longjmp'` (OpenSSL 3.5.9, `brew list
   --versions`) prints `__longjmp`, `__setjmp`, `_getcontext`, `_makecontext`, `_setcontext`: **no `_swapcontext`**.
   So OpenSSL's ASYNC fibers are made by `makecontext`, entered first by `setcontext`, and switched afterwards by
   `_setjmp`/`_longjmp`, a jump of the setjmp family. And `openssl/async.h:82-88` declares
   `ASYNC_set_mem_functions(ASYNC_stack_alloc_fn, ASYNC_stack_free_fn)`, `typedef void *(*ASYNC_stack_alloc_fn)(size_t
   *num)`: **the library hands the choice of a fiber's stack memory to the program** (question 2).
2. **Boost 1.92's fcontext functions are C++ names here, not C ones.** `nm -gU libboost_context.dylib` prints
   `__ZN5boost7context6detail13jump_fcontextEPvS2_`, `...13make_fcontextEPvmPFvNS1_10transfer_tEE`,
   `...14ontop_fcontextEPvS2_PFNS1_10transfer_tES3_E`, each a single `b` to a local `_jump_fcontext`,
   `_make_fcontext`, `_ontop_fcontext` (`otool -tv`); `fcontext.hpp:30-34` declares them in
   `boost::context::detail` with no `extern "C"`. `nm -a` names the objects `jump_arm64_aapcs_macho_gas.o` and
   `make_arm64_aapcs_macho_gas.o`. **The switch is the library's own code**: `_jump_fcontext` stores the callee-saved
   registers, `mov x4, sp`, `mov sp, x0`, reloads, `ret x4` (`otool -tv`, read from the shipped machine code, not
   from source, which this Mac does not hold). `nm -u libboost_context.dylib` imports no context or jump function.
3. **Debian's libcrypto switches by `swapcontext`.** In `heroes-linux:latest` (x86-64), `nm -D --undefined-only
   /usr/lib/x86_64-linux-gnu/libcrypto.so.3` (libssl3t64 3.5.7-1~deb13u2, `dpkg -l`) prints `getcontext`,
   `makecontext`, `swapcontext`, and no jump: one library, two switches, by build (Homebrew's 3.5.9 above jumps by
   `_longjmp`).

## Status, 09:22 (by the cells.log start, 08:23 UTC; the first heading said 09:40, a time not read from a clock)

- Programs written (`docs/panel/259-reports/programs/ffi-pragmatist/`): `bfc.h` (Boost.Context bound from C, no C++
  shim, and route D's tell prototyped), `cpo.h` (a stack-copying library on its own switch), `cpu.h` (the same over
  `makecontext`/`swapcontext`), `aj2.h` (OpenSSL ASYNC, two jobs paused, heap or carved stacks), `bl6.h` (door 6 with
  a large value held), `dl3.h` (the `makecontext` doors `dlh` does not ask), `gen.sh` (the `.hero` programs),
  `cells.sh` (every cell), `make-count-runtime.sh` (the counting runtime).
- First cell, run: `bfc_host` (Boost.Context 1.92 on a stack carved from a C helper's frame, this Mac, stage 2,
  `3ec775b66`): **exit 134, 3 of 3, equal by `cmp`, 158 bytes**: `body holds 11`, `body holds 11`, `panic: internal
  error: a large value's room was given back out of order (compiler bug)`, `called from 0x10002f757, in
  bfc_host.run`. The lane's `own_carved`, reproduced by a real library.

## Boost.Context 1.92 on this Mac (Darwin arm64, stage 2 at `3ec775b66`, `cells.sh`, 08:23 UTC on), run

Each cell three runs, each `timeout 120`, outputs in `259-ffi-pragmatist-scratch/mac/runs/`. Built `heroes-next build
<s>.hero --library /opt/homebrew/opt/boost/lib` (and `--sanitize`); `bfc.h` binds the two mangled symbols from C.

| shape | stack | plain | `--sanitize` |
|---|---|---|---|
| `bfc_host` | carved from a C helper's frame (as `own.h`'s `ow_host`) | **exit 134, 3 of 3, equal by `cmp`**: `body holds 11` twice, `panic: internal error: a large value's room was given back out of order (compiler bug)`, `called from 0x10002f757, in bfc_host.run` | **exit 134, 3 of 3**: `heap-use-after-free`, `READ of size 8`, `located 32 bytes inside of 1048608-byte region`, `freed by thread T0` |
| `bfc_below` | carved from the extern call's own frame (as `below.h`) | **exit 134, 3 of 3, equal**: the *compiler bug* panic, `called from 0x10002f78f, in bfc_below.run`, nothing else | **exit 134, 3 of 3**: `WRITE of size 262144`, `located 786464 bytes inside of 1048608-byte region`, freed |
| `bfc_heap` | `malloc` | exit 0, 3 of 3, equal: `body holds 7` three times, `coroutines 30 32`, `heap: 7` | exit 0, 3 of 3, the same |
| `bfc_host_told` | carved, **told** (route D's binding, `p259_stack_entered` before the first jump) | **exit 0, 3 of 3, equal**: `body holds 7` three times, `coroutines 30 32`, `carved, told: 7` | **exit 0, 3 of 3**, the same bytes |
| `bfc_below_told` | carved in the call's frame, **told** | **exit 0, 3 of 3**: `below, told: 3032 (3032 is right)` | **exit 0, 3 of 3**, the same |

So the lane's two carved shapes are **a real library's**, with the same words and the same sanitizer report, and a
binding that tells the runtime each stack's range before its first jump makes both right, plain and sanitized.

## Question 4: route D's binding, written and compiled

**The C a Boost.Context binding needs under route D** is `bfc.h`'s, and its whole cost is one call before the first
jump onto a stack the binding made (`bfc_start`):

```c
char *base = bfc_carved != NULL ? bfc_carved + k * BFC_STACK : (char *)bfc_heap[k];
if (bfc_told) p259_stack_entered(base, BFC_STACK);      /* route D: hero_stack_entered(low, bytes) */
void *c = bfc_make_fcontext(base + BFC_STACK, BFC_STACK, bfc_entry);
```

- **It is written in the binding's own header, around the library's call, without patching the library**: clang
  accepted it (Apple clang, `heroes-next build bfc_*.hero`, exit 0, twelve builds), and the told cells run right
  (above). For Boost's `fiber` API the same call goes in a `StackAllocator`'s `allocate()`, the library's documented
  hook (the historian's Q2, `stack.html`); for OpenSSL in `ASYNC_set_mem_functions`' allocator (`aj2.h`'s `aj_alloc`);
  for byuu's libco before `co_derive` (the historian's Q3). **All three hand the binding the stack's address before
  the first switch**, so a per-stack tell is writable for them. Read, not run, for Boost's `fiber` and libco.
- **The prototype is not route D's runtime**: `p259_stack_entered` calls `hero_rebind_known_stack` (Darwin) or
  `hero_sig_known_stack` (Linux), the functions the runtime's own `makecontext` wrappers call (`rebind_jump.c:253`,
  `sigjump_make.c:33`), with a `ucontext_t` holding the range alone. A binding today can reach them (external linkage
  in the program's link), and nothing names them: it is the experiment's door, never a binding's.
- **A binding that forgets** gets today's cells: `bfc_host`, wrong value then the false *compiler bug*; `bfc_below`,
  the false *compiler bug*; a use-after-free under `--sanitize`. Route D alone tells it nothing, unless some fact the
  runtime reads separates the told stack from the untold: none is shown in this report (the brief's route A question,
  asked, not answered).
- **ABI and the promised header**: a new declaration beside the others moves no existing one, and the runtime's own
  rule for that keeps `HERO_RUNTIME_ABI` (`heroes_runtime.h:1091-1093`: *"Added beside the declarations above and
  changing none of them ... so `HERO_RUNTIME_ABI` stands: a runtime without them is a compile error at the first unit
  that names one"*; the number is 32 at this base, `heroes_runtime.h:108`). **`hero_os.h` is the language's door**
  (decision 2228, CLOSED, read by `gh issue view 2228`: *"Answered a by the author on 2026-10-27: hero_os.h is the
  language's door ... its functions' stability is promised with the rest at M-compatibility-promise"*), and spec §
  13's line 428 already names `hero_os.h` functions by signature (`grep -n hero_os spec/heroes-spec.md`: lines 401
  and 428). So a route-D entry point there is additive for the ABI and **a promise for ever**, and owes a sentence
  (arm-t's shape).
- **What route D's tell needs that `makecontext`'s learning lacks**: an untell. A range learned stays known until a
  later one overlaps it (`sigentries.c:136-155`, `rebind_entries.c:74-93`, read), so after `bfc_host` returns, its
  array's memory is the thread's stack again and still a *carved range* to `stack_lane.c`: later frames there take
  their room aside. No wrong value follows from that in any cell here (unrun as a shape of its own).

## The stack-copying class (route G's second direction), this Mac at `3ec775b66`, run

`cpo.h` is the design the historian verified in Tencent's libco's copy-stack mode (its Q3: shared stacks from
`malloc`, the occupant's live part `memcpy`'d out before another runs there and back in before it resumes; its switch
`coctx_swap`, the library's own): here one shared region of 512 KiB, the whole region copied, switched by `own.h`'s
assembly. `cpu.h` is the same design over `makecontext` and `swapcontext` **as the program links them, every switch
one the runtime wraps**. Two coroutines' frames hold **the same addresses in turn**. Three runs a cell.

| shape | region | switch | plain | `--sanitize` |
|---|---|---|---|---|
| `cpo_heap` | `malloc` | own (unseen) | **exit 0, 3 of 3, equal**: `body holds 7` three times, `coroutines 30 32`, `copied, heap: 7` | **exit 0, 3 of 3**, the same |
| `cpo_heap_told` | `malloc`, **told** by route D's range | own | **exit 134, 3 of 3, equal, 412 bytes**: `body holds 7` twice, then `panic: a null pointer was read through, at offset 0x1c, called from hero_frame_aside_unlink`, `Two things put a null where C read it: a handle or ptr holding nullptr handed to a C function ...` `This runtime raised neither.` | **exit 134, 3 of 3**: `heap-use-after-free`, `WRITE of size 262144` in `__asan_memcpy` |
| `cpo_host` | carved from a helper's frame | own | exit 134, 3 of 3, equal: `body holds 11` twice, the *compiler bug* panic, `in cpo_host.run` | exit 134, 3 of 3: `READ of size 8`, freed |
| `cpo_host_told` | carved, **told** | own | **exit 134, 3 of 3, equal**: `body holds 7` twice (the host's value saved by the tell), then the *compiler bug* panic, `in cpo_host_told.run` | exit 134, 3 of 3: `WRITE of size 262144`, freed |
| `cpu_heap` | `malloc` | **`makecontext` + `swapcontext`, seen** | **exit 134, 3 of 3, NOT equal**: runs 1 and 3 the null-pointer panic in `hero_frame_aside_unlink` (412 bytes), run 2 `body holds 7` twice then the *compiler bug* panic (113 bytes) | (below) |

**What it says.**

1. **Route D's range tell makes a correct binding of a shipped design crash.** Untold, the copying library's
   coroutines run on a stack nobody named, tag 0, *"never judged gone"* (`frame_aside.c:99`), and the program is
   right. Told, the region becomes one named stack, both coroutines' takes stand under one tag, and the thunk of
   coroutine 1, entered at the address coroutine 0's thunk stood at, heals coroutine 0's live take
   (`hero_callback_heal`, `callback.c:257-263`, every entry at or below the thunk's frame): the runtime then walks a
   freed block (`hero_frame_aside_unlink`). That mechanism is read from the code, not traced; the run that names the
   cause is the no-heal table below. **A binding that obeys arm-t turns a running program into a corrupted
   one.**
2. **The class is wider than *"a switch the runtime cannot see"***: `cpu_heap` switches only by the functions the
   runtime wraps, onto the heap, and corrupts today. `makecontext`'s learning names the shared region, and the same
   heal follows. **No address and no range separates two coroutines that occupy the same addresses in turn**; only a
   fact that changes at each switch (an identity the switch carries) can.
3. **The words are false** in both: a runtime fault inside the runtime's own list is told as *"a handle or `ptr`
   holding `nullptr` handed to a C function"*, in a program that hands C no pointer: the class of defects 2705 and
   2755 (*words that name causes the program does not have*), on top of memory corruption (§1.12).

## Question 3: the doors, with switches, counted (this Mac first)

**Seen** is the counting runtime's word (`make-count-runtime.sh`, nine `perl` edits of a copy: one stderr line,
`COUNT <kind> <n>`, the first eight times the runtime learns a `makecontext` range (`made`), enters its
`swapcontext` (`swap`), its `setcontext` (`set`) or its setjmp-family jump funnel (`jump`)); a build is handed the copy
by `HEROES_RUNTIME=`. One plain run each, `cells.sh`, at `3ec775b66`.

### Darwin arm64, this Mac

| door: the switch | the program | made | swap | set | jump | what the program got |
|---|---|---|---|---|---|---|
| `makecontext` + `swapcontext`, carved | `carved` (the lane's) | 2 | 8 | 0 | 0 | exit 0, `body holds 7` |
| the same through `dlsym(RTLD_DEFAULT)` | `unseen_carved` | 2 | 8 | 0 | 0 | exit 0, `carved: 7`: **seen** (Darwin answers `dlsym`) |
| a library's own switch onto the heap | `own_heap` | 0 | 0 | 0 | 0 | exit 0, `order 0 1: 30 32` (tag 0, never judged) |
| a library's own switch, carved | `own_carved` | 0 | 0 | 0 | 0 | exit 134, `body holds 11` then *compiler bug* |
| **Boost.Context's `jump_fcontext`**, heap | `bfc_heap` | 0 | 0 | 0 | 0 | exit 0, right |
| **Boost.Context's `jump_fcontext`**, carved | `bfc_host` | 0 | 0 | 0 | 0 | exit 134, `body holds 11` then *compiler bug* |
| a stack-copying library, `makecontext` + `swapcontext`, heap | `cpu_heap` | 2 | 7 | 0 | 0 | **exit 134: every switch seen, and corrupted** |
| **OpenSSL 3.5.9 ASYNC**, 256 KiB fibers from `malloc` | `aj_heap` | 2 | 0 | 2 | 6 | exit 0 (`makecontext`, `setcontext` and `_longjmp` all seen) |

The `dlsym` doors, by probe (one run each, at `3ec775b66`, each compared with the program's own `makecontext`):
`dlh` prints `state 1111` (`RTLD_DEFAULT` and `RTLD_NEXT`, `makecontext` and `swapcontext`: the program's own, the
repair's reading again); `dl3` prints **`state 1111`**: `dlopen("/usr/lib/libSystem.B.dylib", RTLD_NOLOAD)`'s handle,
`dlopen("/usr/lib/system/libsystem_c.dylib", RTLD_NOLOAD)`'s (**the C library's own handle, unprobed on Darwin until
now**), `dlopen(NULL)`'s and `RTLD_SELF` all answer the runtime's `makecontext`. `dlh2` is Linux's probe (`libc.so.6`)
and does not build here (`ffi_header_refused`: `<ucontext.h>` without `_XOPEN_SOURCE`, its own header's omission).
**A pointer to the C library's own `makecontext` taken without `dlsym` inside the shared cache** (an export-trie walk
of libsystem_c's image, or a cache library that calls it): **unrun**; the question is what such a program would be,
since every `dlsym` handle here is answered.

## Question 2: does a real binding need a carved stack?

**Libraries that hand the choice of stack memory to the caller**, each by its own interface: **`makecontext`**
(POSIX: `uc_stack` is the caller's); **Boost.Context** (`make_fcontext(sp, size, fn)` takes any memory: `bfc.h`,
run; its `fiber` takes a `StackAllocator` or `preallocated(sp, size, sctx)`, the historian's Q2, read by it);
**OpenSSL 3's ASYNC** (`ASYNC_set_mem_functions`, `async.h:82-88`, run: `aj_carved` gives every fiber a quarter of
a helper's array and **runs right on this Mac, 3 of 3, plain and `--sanitize`**, because libcrypto's `makecontext`
is the runtime's: counter `made 2`); **byuu's libco** (`co_derive`, *"attempts to use the provided memory"*, the
historian's Q3, read by it, unbuilt here). **Tencent's libco** hands nothing: its shared stacks are its own
`malloc`'s (the historian's Q3).

**Does a real program put that memory on the thread's stack?** Searched: Boost's two documentation pages and libco's
usage document by the historian (*"Neither page says whether a stack may be memory the caller carves from its own
stack"*; libco's says nothing of where the memory may come from); this Mac holds no program's source that binds any of
them. So **no real program doing it was found, and the search was documents, not code**: a question, not a premise.
What is measured is the **class's reach by a real library**: Boost.Context's own switch onto a carved stack loses a
value exactly as the lane's assembly does (`bfc_host`, `bfc_below`), and OpenSSL's onto a carved stack does not
(`aj_carved`), because the one switch is learned and the other is not. **The real library whose frames lie inside
the thread's own stack by design** is a stack-copying one over the thread's stack (greenlet's design, named in my
`cpo.h` from memory): **unread by me**, the historian's to read, and the cells below say why it matters more than
the carved case.

## Question 6: Windows

**Unread by me** (no Boost assembly on this Mac, no Boost in either image, no page reader, no download). **The
historian read it** (`docs/panel/259-reports/historian.md`, its Q2, *"verified"*, at Boost's `develop` branch, not at
the 1.92.0 tag): `jump_x86_64_ms_pe_masm.asm` and its GNU twin save and restore the thread block's stack base, limit,
deallocation stack and fiber storage **unconditionally**, so `stack_lane.c:89-100`'s reading of the thread block is
told the truth by Boost's Windows switch for a stack `make_fcontext` prepared: an inference from the assembly, unrun on
Windows. **What that does not reach**: a stack-copying library on Windows (its frames on one region, the thread block
naming that region for every coroutine in turn) is the copying cells' shape, and the thread block, like a range,
cannot tell two coroutines on one region apart: an inference from the cells below, **unrun on Windows**. I do not ask
for the box: the copying cells decide the class on this Mac (Linux's rows below, by base), and a Windows run would price
a row, not move a route.

## The cause, run: the heal by address (this Mac, `cells2.sh`, 08:30 to 08:31 UTC)

Every corrupting cell built against `make-noheal-runtime.sh`'s copy (three lines: `hero_frame_heal_below`,
`hero_lend_heal_inside` and `hero_callback_heal` return at once; a give keeps its own rule), three plain runs each:

| cell | today | no heal by address |
|---|---|---|
| `own_carved` | 134, `body holds 11`, *compiler bug* | **0, 3 of 3, equal**: `body holds 7` three times, `carved: 7` |
| `below` | 134, *compiler bug* | **0, 3 of 3**: `below: 3032` |
| `bfc_host` (Boost) | 134, `body holds 11`, *compiler bug* | **0, 3 of 3**: `carved: 7` |
| `bfc_below` (Boost) | 134, *compiler bug* | **0, 3 of 3**: `3032` |
| `cpu_heap` (copying, every switch seen) | 134, null read or *compiler bug* | **0, 3 of 3**: `copied by ucontext, heap: 7` |
| `cpo_host` | 134, `body holds 11` | **0, 3 of 3** |
| `cpo_host_told` | 134, *compiler bug* | **0, 3 of 3** |
| `cpo_heap_told` | 134, null read in `hero_frame_aside_unlink` | **0, 3 of 3** |

**One cause for all eight, measured**: the room and the regions given back by a frame's address. Not the switch,
not the stack's place, not whether anybody was told.

OpenSSL, the corrected binding (256 KiB fibers through `ASYNC_set_mem_functions`; the first `aj_heap` overflowed the
library's default fiber stack, `stack-overflow ... in h_ajheap_e4` under `--sanitize`, my binding's error; the first
`aj_carved` count run died of SIGABRT at exit after right values, my `aj_free` freeing a carved stack, also mine):
`aj_heap` and `aj_carved` **exit 0, 3 of 3, equal, plain and `--sanitize`**, `jobs 1030 1032`; counted `made 2 swap
0 set 2 jump 6` each. Under `--sanitize` `aj_heap` prints ASan's *"False positive error reports may follow"* warning
(its 64 MiB heuristic, the historian's Q5) and exits 0.

## Question 5: route B from the C side, and what the heal buys (run)

**Door 13 with a large value**, this Mac at `3ec775b66` (`door13.sh`: panel 258's `cx258.cpp` built `clang++ -O2`,
`throw 13` from C++ through the callback's frames to `catch (...)`, the callback holding 262,144 bytes), three runs a
cell:

| runtime | `DOOR_N=3` | `DOOR_N=5000` |
|---|---|---|
| today's (R1 heals by address) | exit 0, `throws 21 last 5` | **exit 0, 3 of 3**, `throws 35000 last 5` |
| no heal by address (route B on a door no hook sees) | exit 0, `throws 21 last 5` | **exit 134, 3 of 3**: `panic: stack exhausted in cx13.e5: the room large values take off the stack is full, 1 GiB for the whole program`, `called from ... in cx13.body` |

So the heal by address is what keeps a correct C++ binding running past about 4,000 throws of a 256 KiB value
(1 GiB / 256 KiB = 4,096, arithmetic). **Route B stops that program**, told, never corrupted, **in words that name a
depth the program never reached**: the room is full of values whose frames C++ unwound, not of a deep call. That is
2705's class again, priced. The same holds of door 6 on x86-64 (below, when the container's cells land) and, by
the same mechanism, of 12 and 8 (unrun here with a large value).

**Route C's frame chain, one fact from a real library**: Debian's x86-64 libcrypto (`libssl3t64` 3.5.7) has `push
%rbp` among the first three instructions of **731 of its 9,228 functions** (`objdump -d --no-show-raw-insn`, two
`grep -c`s, in `p259-ffi-pragmatist-1`): a callback an OpenSSL fiber calls on x86-64 Linux is reached through frames
that mostly keep no frame record, so a chain from the thunk to the thread's outermost frame is not a fact the runtime
can read there. A crude count (a `push %rbp` can save the register without making a record), not a walk.

## Greenlet: a shipped library whose coroutines run inside the thread's own stack (read from its source on this disk)

`find / -name '_greenlet*.so' -not -path '*/System/*'` found greenlet in `~/.cache/uv/archive-v0/` (an `uv` cache,
already on disk: nothing downloaded), **version 3.5.6** (its `dist-info/METADATA`), with its C++ source shipped beside
the built extension. **Read, not run** (binding it from Heroes needs CPython embedded, unbuilt):

- **Where a greenlet's stack is**: `TUserGreenlet.cpp:175-185` declares `void* dummymarker;` and calls
  `real_target->g_initialstub(&dummymarker)`; `g_initialstub(void* mark)` (`:228`) makes the new greenlet's stack
  state from it, `this->stack_state = StackState(mark, ...)` (`:292`). **A new greenlet starts at the address of a
  local of the C frame that starts it: on the thread's own stack.**
- **Copying**: `StackState::copy_stack_to_heap_up_to` (`TStackState.cpp:106-130`) `memcpy`s the live slice into a
  `PyMem_Realloc`'d buffer; `copy_heap_to_stack` (`:86-104`) `memcpy`s it back. **Two greenlets' frames stand at the
  same addresses of the thread's stack in turn.**
- **The switch**: `platform/switch_aarch64_gcc.h:70-87`, `slp_switch`: `mov %0, sp`, then `add sp,sp,%0` and `add
  x29,x29,%0`, the library's own code: no context function, no jump of the setjmp family.

So the shape this report runs as `cpo_host` (a copying library on a region of the thread's own stack: `body holds
11`, *compiler bug*, 3 of 3 on this Mac) is **greenlet's design**, a library gevent and others carry (that use: the
historian's to read; I name none as read). **No range tell can express it**: the range would be the thread's own
stack. Route D cannot repair it; route A must refuse it (a correct program); route E repairs it (`cpo_host-noheal`,
right 3 of 3). Ruby 4.0.10's fibers, also on this Mac: `libruby.4.0.dylib` exports `_coroutine_transfer` (`nm
-gU`), imports `_mmap` and no context function (`nm -u`): its own switch, unread further, unbound.

## Provisional verdict (09:38 CET, before the Linux cells of my shapes; the final one closes this report)

**veto** on route D as a **range** tell (arm-t as drafted), **object** to arm-r and arm-p as worded and to any
resolution scoped to *"a switch the runtime cannot see"*. Reasons, each a cell above: the heal by address is the
single cause of eight corrupting cells (no-heal table); `cpu_heap` corrupts with every switch seen; a range tell
turns `cpo_heap` (Tencent libco's design, right untold) into a crash; greenlet's design (read) has no range to tell.

## Question 1: the libraries, where their stacks come from, their switch, seen or not

"Seen" is the counter's (`made`/`swap`/`set`/`jump`, this Mac at `3ec775b66`). Cells three runs each unless said.

| library, version | its stacks | its switch | seen (Mac counter) | shape run, Mac cells (plain / `--sanitize`) |
|---|---|---|---|---|
| **Boost.Context 1.92.0_1** (Homebrew; absent from both images) | the caller's: `make_fcontext(sp, size, fn)`; `fiber` by a `StackAllocator` or `preallocated` (historian) | its own assembly, `_jump_fcontext` (`otool -tv`) | **no**: 0/0/0/0 (`bfc_host`, `bfc_heap`) | `bfc_heap` 0 / 0; **`bfc_host` 134 wrong value / 134 UAF**; **`bfc_below` 134 / 134 UAF**; told: `bfc_host_told`, `bfc_below_told`, `bfc_heap_told` 0 / 0 |
| **OpenSSL 3.5.9 ASYNC** (Homebrew libcrypto) | `OPENSSL_malloc` by default; the caller's through `ASYNC_set_mem_functions` (`async.h:82-88`) | `makecontext`, `setcontext` to enter, `_setjmp`/`_longjmp` after (`nm -u`) | **yes**: made 2, set 2, jump 6 | `aj_heap` (256 KiB `malloc`) 0 / 0; **`aj_carved` (carved through the library's hook) 0 / 0** |
| **OpenSSL 3.5.7 ASYNC** (Debian libssl3t64, x86-64 image) | as above | `makecontext`, `swapcontext` (`nm -D --undefined-only`) | Linux cells below | |
| **greenlet 3.5.6** (uv cache, source read) | **the thread's own stack**, from a local's address (`&dummymarker`), slices copied to the heap and back | its own assembly, `slp_switch` | unbound (needs CPython) | its design run as `cpo_host`: 134 wrong value / 134 UAF |
| **Tencent libco**, copy-stack mode (the historian's reading; unbuilt) | its own `malloc`'d shared stacks, copied out and in | its own, `coctx_swap` (historian) | its design run as `cpo_heap`: 0/0/0/0 | `cpo_heap` 0 / 0; **told: `cpo_heap_told` 134 null read in the runtime / 134 UAF** |
| a copying design over `makecontext` + `swapcontext` (`cpu.h`, mine) | one `malloc`'d region | the C library's, as linked | **yes**: made 2, swap 7 | **`cpu_heap` 134, two different panics across three runs** / sanitizer cell void (shadow not copied) |
| **byuu's libco v20** (historian; unbuilt) | the caller's through `co_derive`, or its own | its own per backend | unread by me | none |
| **Ruby 4.0.10 fibers** (`libruby.4.0.dylib`) | `mmap` (imported; read as a symbol only) | `_coroutine_transfer` (exported) | unbound | none |
| corosensei, others named in the brief | | | **unread** (the historian's) | none |
| the C library's `ucontext` (libsystem_c; glibc 2.41) | the caller's (`uc_stack`) | `makecontext`/`swapcontext` | yes as linked; on Darwin through every `dlsym` handle (`dlh` 1111, `dl3` 1111); Linux by handle (below) | `carved` 0 (made 2 swap 8); `unseen_carved` 0 on Darwin (made 2 swap 8) |

`below` beside `own_carved` (the lane's assembly): Mac `own_carved` 134 `body holds 11` / 134 UAF READ; `below` 134
*compiler bug* / 134 UAF WRITE of 262,144 (this seat's runs at `3ec775b66`, equal to the repair's).

## What my veto would compel

1. **No resolution may make a binding's correctness depend on its telling the runtime anything**, and **none may ask
   it to tell a range**. Measured why: a range tell repairs separate stacks (`bfc_*_told`) and corrupts shared ones
   (`cpo_heap_told`, `cpo_host_told`); greenlet's stacks are the thread's own, so no range exists to tell; and a
   binding that forgets gets today's wrong value (`bfc_host`). §1.12's *"any C library must be bindable"* reads, at
   this boundary, as: **the binding of a correct C library is correct as written**.
2. **The runtime must stop giving back a callback-reachable frame's room or region by that frame's address**, where
   the frame may be a suspended coroutine's: every corrupting cell is right with that heal off (the no-heal table,
   eight of eight on this Mac; Linux below), whatever the switch, seen or not, the stack, carved, heap or shared, told
   or not; **and take it order-free**, a block of its own given back by its own give: with no heal alone, a coroutine
   that ends first gives back out of the room's order and the runtime aborts with the false *compiler bug*
   (`cpo_host_01`, `bfc_host_01`, 3 of 3 each, values right until then).
3. **What it then owes, and the price is measured**: the jumps the heal served. Seen jumps (the setjmp family, doors
   1 to 5 and 7, panel 258's table) can be healed where the hook decodes them (route B). The unseen ones keep their
   values: door 13 at 5,000 throws of a 256 KiB value goes from exit 0 to *"stack exhausted ... 1 GiB"* (run, this
   Mac). So the veto compels, beside the change, **true words for that abort** (not *stack exhausted in e5*, a depth
   the program never reached) and, for door 13, the unwinder made a seen event (panel 258's (d), its § 4 prototype:
   a C++ exception runs cleanups in frames that have them), which turns its row from a leak into a heal. Door 6 on
   x86-64 and door 12 stay a leak bounded by the room, told.
4. **If a tell is kept at all**, it is optional and carries **which coroutine runs now, at each switch** (an identity,
   as `__sanitizer_start_switch_fiber` carries a fiber's own `fake_stack_save`), never a range, and a binding that
   never calls it loses nothing. Untested here: no runtime keys its room by such an identity.
5. **Not compelled**: any ABI change. Every binding in this report compiled against today's headers; a new
   `hero_os.h` entry point would be additive (`heroes_runtime.h:1091-1093`'s rule keeps `HERO_RUNTIME_ABI` at 32).

## What the conservative resolution would have been

Keep R1 and step 181a as built; no spec sentence; file the lane's two drafts and this report's new defect (a
stack-copying library over `makecontext` and `swapcontext`, every switch seen, loses a callback's value and the
runtime faults in `hero_frame_aside_unlink` with words that blame a null handle: `cpu_heap`); and tell binding
authors, outside the spec, to give each coroutine a stack of its own from `malloc`. **Why it is not the robust one**:
it leaves greenlet's design and every copying library over `makecontext` corrupting memory with no binding able to
avoid it (greenlet chooses its own stacks), which §1.12 forbids; and it leaves `cpo_heap`-style programs right only
by the accident that a stack nobody named is never judged.

## The repaired arm-t, run: a tell of **which coroutine runs**, not of a range (this Mac, `cells-id.sh`, `cells-01.sh`)

`make-ident-runtime.sh` makes a copy of the runtime with two edits: `p259_coroutine_now(uintptr_t id)` sets a
thread-local, and `hero_stack_lane` answers *"not the thread's own, tag `id`"* for every frame while an id stands, so
those takes stand aside under that coroutine's tag and R1's heal runs among one coroutine's blocks alone (R1 itself
untouched). The bindings call it around each switch (`BFC_NOW`, `CPO_NOW`, `CPU_NOW`: `k + 1` before the switch into
coroutine `k`, `0` once it is back), two lines a switch, in their own headers (`bfc_id.h`, `cpo_id.h`, `cpu_id.h`
define the mode). Three runs a cell, plain and `--sanitize`:

| cell | shape | today | identity-told, identity runtime |
|---|---|---|---|
| `bfc_host_id` | Boost.Context, carved | 134, `body holds 11` | **0, 3 of 3, plain and `--sanitize`**: `carved: 7` |
| `cpo_host_id` | **greenlet's design** (copying on the thread's own stack) | 134, `body holds 11` | **0, 3 of 3, plain and `--sanitize`** |
| `cpo_heap_id` | Tencent libco's design | 0 (untold) / 134 (range-told) | **0, 3 of 3, plain and `--sanitize`** |
| `cpu_heap_id` | copying over `makecontext` + `swapcontext` | 134 | **0, 3 of 3 plain**; `--sanitize` 134, the same `stack-buffer-underflow` in `__asan_memcpy` as untold (ASan's shadow is not copied with the stack: the sanitizer's limit, not the runtime's) |
| `bfc_host_id_01` | Boost carved, coroutine 0 ended **first** | 134, `body holds 11` | **0, 3 of 3** |

**And the no-heal copy is not route E** (`cells-01.sh`, three plain runs each): with coroutine 0 ended before
coroutine 1, `cpo_host_01` and `bfc_host_01` print **`body holds 7`** (the value kept) and then abort with *"internal
error: a large value's room was given back out of order (compiler bug)"*, 3 of 3 each, where today they print `body
holds 11`; the identity runtime with untold bindings prints `body holds 11`, as today. So **turning the heal off
keeps the values and leaves the room's last-in-first-out give**, which a coroutine that ends first breaks: route E
needs a block of its own for every take under a callback, as the brief's § 6 E says, not only no heal. The eight
"right with no heal" cells above end in the order the room expects.

**What it says for route D.** The fact that separates coroutines is **which one runs**, an identity a switch carries;
a range carries it only where one coroutine owns the range. A binding can tell it (two lines a switch, compiled and
run here), and **a runtime can read it itself at every switch it wraps**: the context `swapcontext` or `setcontext`
switches into is a different object for each coroutine (`cpu_ctx[k]` in `cpu.h`), so `cpu_heap` needs no tell under a
runtime that keys its tags by the context switched into (an inference from the identity cells, **unrun**: no runtime
here keys by the context). For a library's own switch onto the thread's stack (greenlet, Boost carved) only the
binding knows; **a binding that forgets gets today's wrong value under every tell route**, so a tell can be an
improvement and never the safety.

## Question 7: Linux x86-64 at `3ec775b66`, emulated (`heroes-linux:latest`, container `p259-ffi-pragmatist-1`)

`uname -m` x86_64, *Debian GLIBC 2.41-12+deb13u3*, *Debian clang version 22.1.8*; both compilers built inside it
from a `git archive 3ec775b66` (07:23 to 07:51 UTC, exit 0, `sources 05186f2b38e77a97` and `sources
117362877b136f67`). **Emulated**, not the CI's native leg. `cells-linux.sh` (my first `cells.sh` there was stopped by
`kill` of its pids after one cell, the emulated sanitizer build taking over eleven minutes at a load of 175).

| shape | plain | `--sanitize` |
|---|---|---|
| `own_carved` | **exit 139, 3 of 3, equal**: `Segmentation fault` alone, 19 bytes | **exit 1, 3 of 3**: `heap-use-after-free`, `READ of size 8`, `located 32 bytes inside of 1048608-byte region` |
| `unseen_carved` | **exit 134, 3 of 3, equal, 823 bytes, equal by `cmp` to the repair's arm64 output at `3ec775b66`**: `HERO_SIGNAL_REENTERED`'s words, then the remedy | **exit 134, 3 of 3**, the same bytes (`cmp`) |
| `below` | **exit 139, 3 of 3, equal**, `Segmentation fault` alone | (below) |

So the critic's x86-64 cells at `2cf85ea64` hold at this base, and x86-64 is arm64's row.

**Whether `unseen_carved`'s words can be true**: they say *"called after C jumped into stack frames the thread had
already left (a handler left by a jump and jumped back into, or frames only a signal's delivery puts on the alternate
stack): the handler is held to be running again"*. The program installs no handler and makes no jump (`grep -n -i -E
'signal|sigaction|sigaltstack|raise|longjmp'` of `cr.h` and `unseen_carved.hero`: the repair's, nothing), and its
switches are `swapcontext`s (the Mac counter: `swap 8`). **They cannot be true of this program**: the class of
defects 2705 (closed) and 2755 (open), words naming causes the program does not have; and the third instance of the
class this report meets, after `cpu_heap`'s and `cpo_heap_told`'s *"a handle or `ptr` holding `nullptr`"* and door
13's *"stack exhausted"* under no heal.

x86-64 at `3ec775b66`, the rest (`cells-linux.sh`, 08:50 to 09:07 UTC; my library shapes plain only there, three runs
each; Boost absent):

| cell | x86-64 |
|---|---|
| `below --sanitize` | exit 1, 3 of 3: `WRITE of size 262144`, `located 786464 bytes inside of 1048608-byte region`, freed |
| **door 6**, `bl6_3` (3 `__builtin_longjmp`s over a 262,144-byte value) | **exit 0, 3 of 3, equal, plain and `--sanitize`**: `jumps 21 last 5` |
| **door 6**, `bl6_5000` (5,000 of them) | **exit 0, 3 of 3, equal, plain and `--sanitize`**: `jumps 35000 last 5` |
| `dlh` | `state 1011` (`RTLD_DEFAULT` `makecontext` the program's, `RTLD_NEXT`'s the C library's; both `swapcontext`s the program's) |
| `dlh2` | `state 1` (the C library's own handle: its `makecontext`, the program's `swapcontext`) |
| **`dl3`** | **`state 1`**: `dlvsym(RTLD_DEFAULT, "makecontext", "GLIBC_2.2.5")`, `dlvsym(RTLD_NEXT, ...)` and `dlvsym(libc, ...)` all answer **the C library's**; `dlsym(dlopen(NULL), ...)` the program's. `nm -D bin-dlh`: `T makecontext`, `T swapcontext` |
| `cpu_heap` (copying over `makecontext` + `swapcontext`) | **exit 139, 3 of 3, equal**: `body holds 7` twice, then `Segmentation fault` |
| `cpo_heap` (Tencent libco's design, untold) | **exit 0, 3 of 3, equal**: right |
| `cpo_heap_told` (the same, range-told) | **exit 139, 3 of 3, equal**: `body holds 7` twice, then `Segmentation fault` |
| `cpo_host` (greenlet's design) | exit 139, 3 of 3, `Segmentation fault` alone |
| `cpo_host_told` | exit 139, 3 of 3: `body holds 7` twice, then `Segmentation fault` |
| `aj_heap` (OpenSSL 3.5.7, `swapcontext` build, 256 KiB `malloc` fibers) | exit 0, 3 of 3, equal: `jobs 1030 1032` |
| `aj_carved` (the same, fibers carved through `ASYNC_set_mem_functions`) | **exit 0, 3 of 3, equal**: `jobs 1030 1032` |

**Door 6 on x86-64, the cell panel 258 left unrun**: today's heal by address carries 5,000 unseen jumps over a
256 KiB value; the no-heal cell (route B's) is below. **The range tell's crash and the copying class's corruption are
x86-64's too**, where Darwin panics and Linux segfaults: §1.12's *"must not segfault"*, on a program that is correct
C.

### The doors on Linux x86-64 at `3ec775b66`, counted (one plain run each, `cells-linux.sh`, 09:07 to 09:14 UTC)

| door: the switch | the program | made | swap | set | jump | what the program got |
|---|---|---|---|---|---|---|
| `makecontext` through `dlsym(RTLD_NEXT)`, `swapcontext` likewise | `unseen_carved` | **0** | 3 | 0 | 0 | exit 134, `HERO_SIGNAL_REENTERED`'s words (the `makecontext` unseen, the switches seen) |
| a copying library over `makecontext` + `swapcontext`, heap | `cpu_heap` | 2 | 7 | 0 | 0 | **exit 139: every switch seen, and a segfault** |
| **door 6**, `__builtin_longjmp` | `bl6_3` | **0** | **0** | **0** | **0** | exit 0, `jumps 21`: never seen, healed by address at the next thunk |
| OpenSSL 3.5.7 ASYNC (`swapcontext` build) | `aj_heap` | 2 | 8 | 0 | 0 | exit 0, right |

The `makecontext` doors by probe (above): `RTLD_DEFAULT` seen; `RTLD_NEXT`, the C library's handle and **`dlvsym` by
any of the three handles** unseen (`dl3` `state 1`); `dlsym(dlopen(NULL))` seen. **Every Linux row names the base
`3ec775b66`; the `dlsym-make` lane is repairing the `RTLD_NEXT` and C-library-handle answers, and `dlvsym` is a
fourth door beside them** (`sigjump.c:69-70` says `dlvsym` is not answered; for `makecontext` the cell is now run).

### Route B's price on x86-64: door 6 with no heal (`cells-linux.sh`, three plain runs each)

| cell | today | no heal by address |
|---|---|---|
| `bl6_5000` (5,000 `__builtin_longjmp`s over a 262,144-byte value) | exit 0, `jumps 35000` (plain and `--sanitize`) | **exit 134, 3 of 3, equal**: `panic: stack exhausted in bl6_5000.e5: the room large values take off the stack is full, 1 GiB for the whole program` |
| `cpu_heap` | exit 139 | **exit 0, 3 of 3**: `copied by ucontext, heap: 7` |
| `own_carved` | exit 139 | **exit 0, 3 of 3**: `carved: 7` |

So on x86-64 as on this Mac: the heal by address is what corrupts the coroutine cells and what carries the unseen
jump; door 6 is the one door no hook can ever see (`__builtin_longjmp` inline, no call to interpose: panel 258's
`objdump`, and now the counter's four zeros), and route B turns its program from running into a told abort whose
words name a depth it never reached.

### The identity read by the runtime itself, no binding telling (this Mac, `make-ctxid-runtime.sh`)

The identity copy above, plus two lines around the C library's call in each `swapcontext` wrapper the runtime
already has (`rebind_jump.c`'s and `sigjump_save.c`'s): before the switch, the context switched into names the
coroutine (0 where it is the one the thread's own code last switched away from); after it returns, the context it
returned into. **`cpu_heap` with `cpu.h` unchanged: exit 0, 3 of 3, `copied by ucontext, heap: 7`**; `carved` and
`unseen_carved` exit 0, 3 of 3 each, as today. So the copying class over `makecontext` and `swapcontext` needs no
binding change, only a runtime that keys a coroutine's room by the context it switched into rather than by a range:
**the inference above is now a cell, on this Mac**, and on Linux arm64 (`arm-extra.sh`: `cpu_heap` 0, 3 of 3; the
Linux arm64 section below), plain runs only.

## Linux arm64 at `3ec775b66` (`heroes-linux-arm64:latest`, container `p259-ffi-pragmatist-2`, native)

*Debian GLIBC 2.41-12+deb13u3*, *Debian clang version 22.1.8 (1~deb13u4)*; both compilers built inside it from the
same archive (09:19 to 09:36:50 UTC, `sources 05186f2b38e77a97`, `sources 117362877b136f67`); `arm-all.sh` then
`arm-extra.sh`, 09:36 to 10:00 UTC. Debian's arm64 libcrypto imports `_longjmp`, `_setjmp`, `getcontext`,
`makecontext`, `setcontext` (`nm -D --undefined-only`): the Mac's shape, not x86-64's `swapcontext` one. Three runs a
cell.

| cell | plain | `--sanitize` |
|---|---|---|
| `own_carved` | exit 139, 3 of 3, `Segmentation fault` alone | exit 1, 3 of 3, UAF `READ of size 8` |
| `unseen_carved` | exit 134, 3 of 3, 823 bytes, `HERO_SIGNAL_REENTERED`'s words | the same |
| `below` | exit 139, 3 of 3 | exit 1, 3 of 3, UAF `WRITE of size 262144` |
| door 6 (`bl6_*`) | cannot be written on arm64: the header answers −9 a try (`jumps -27`, `-45000`), exit 0 | the same |
| `dlh` / `dlh2` / **`dl3`** | `state 1011` / `state 1` / **`state 1`** (`dlvsym` by all three handles answers the C library's `makecontext`); `T makecontext`, `T swapcontext` exported | |
| `cpu_heap` | **exit 139, 3 of 3**: `body holds 7` twice, `Segmentation fault` | |
| `cpo_heap` / `cpo_heap_told` | **0, right** / **139, 3 of 3**, after `body holds 7` twice | |
| `cpo_host` / `cpo_host_told` / `cpo_host_01` | 139 / 139 after `body holds 7` twice / 139 | |
| `aj_heap` / `aj_carved` (OpenSSL, `_longjmp` build) | 0, 3 of 3 each, `jobs 1030 1032` | |
| counted | `unseen_carved` made 0 swap 3; `cpu_heap` made 2 swap 7; `bl6_3` 0/0/0/0; `aj_heap` made 2 set 2 jump 6 | |
| **no heal** | `cpu_heap` **0, 3 of 3**; `own_carved` **0, 3 of 3**; `cpo_host_01` `body holds 7` then the *compiler bug* (the room's order), 3 of 3 | |
| **identity told** | `cpo_host_id`, `cpo_heap_id`, `cpu_heap_id` **0, 3 of 3, plain and `--sanitize`**; `cpo_host_id_01` **0, 3 of 3** | |
| **identity read by the runtime** (`make-ctxid-runtime.sh`, no tell) | `cpu_heap` **0, 3 of 3**; `carved` 0; `unseen_carved` still 134 in the signal words (rule 6's refusal, not the room's) | |
| **door 13** (`cx13`, `clang++ -O2`) | today: `DOOR_N=5000` **0, 3 of 3**, `throws 35000`; no heal: `DOOR_N=3` 0, **`DOOR_N=5000` 134, 3 of 3**, *"stack exhausted in cx13.e5 ... 1 GiB"* | |

**So every row of this report holds on all three platforms at `3ec775b66`**, Boost's on this Mac alone (absent from
the images), door 6 on x86-64 alone (unwritable on arm64), and the sanitizer cell of `cpu_heap_id`, which ASan on
this Mac voids (its shadow not copied with the stack) and on arm64 passes.

## Run and read, apart

**Run** (each cell's command and output named above): every cell of this report's tables, on this Mac at
`3ec775b66` (stage 2 built in my copy) and in `heroes-linux:latest` (x86-64, emulated) at `3ec775b66`; the counting,
no-heal, identity and context-identity runtime copies (`make-*-runtime.sh`); `nm`, `otool`, `objdump` of the shipped
Boost, OpenSSL, libcrypto and libruby binaries; `gh issue view 2228`. **Read, not run**: greenlet 3.5.6's source on
this disk; the runtime's code at the lines cited; the historian's report (Boost's Windows switch, libco, Tencent
libco); design.md §1.11, §1.12, §4.19; panel 257's and 258's evidence. **Neither**: Windows; Boost's `fiber` API;
byuu's libco; a pointer to Darwin's `makecontext` taken inside the shared cache; a route-E runtime (no seat handed me
one); the `HELD` arm (§ 1 item 5) by a program of mine: OpenSSL's ASYNC on this Mac jumps by `_longjmp` onto stacks
`makecontext` was given, six times in `aj_heap` (counter `jump 6`), outside any handler, and runs right; `HELD` needs
the jump inside a handler, which no binding of mine makes.

## Verdict

- `verdict`: **veto**: on route D as a **range** tell (arm-t as drafted, and any tell a binding's correctness
  depends on), and on any resolution that keeps a callback-reachable frame's room or region given back by that
  frame's address for a coroutine nobody named; **object** to arm-r and arm-p as worded (false today, clause by
  clause, `ffi-clauses.md`).
- `section`: design.md **§1.12** (*"A Heroes program must not segfault and must not corrupt memory ... the boundary is
  complete: any C library must be bindable"*), **§1.11** (*"the FFI is every package's door to the platform"*),
  **§4.19** (a binding is its header and nothing beside it). design.md does not speak of a coroutine library's stacks
  at all (`grep -n -i coroutine docs/design.md`: Part 6's refusal and Part 7's *"do not build a scheduler"*, both
  about the language's own); the objection stands on §1.12's *complete*.
- `experiment`: `bfc.h` (Boost.Context 1.92's `jump_fcontext`/`make_fcontext` bound from C by `__asm__` labels to
  their mangled names, no C++ shim), `cpo.h` (Tencent libco's and greenlet's copy-stack design, its own switch),
  `cpu.h` (the copying design over `makecontext`/`swapcontext`), `aj2.h` (OpenSSL's ASYNC with
  `ASYNC_set_mem_functions`), `bl6.h` (`__builtin_longjmp`), `cx13.hero` + `cx258.cpp` (a C++ exception), `dl3.h`
  (`dlvsym` and the C library's handles), and route D's two tells (`p259_stack_entered`, `*_id.h`). **clang accepted
  every one** where its library exists: Apple clang on this Mac (all), Debian clang 22.1.8 in `heroes-linux:latest`
  x86-64 (all but Boost, absent; `bl6.h`'s `__builtin_setjmp` compiles there alone) and in `heroes-linux-arm64`
  (all but Boost; `bl6.h` compiles with the door left out, as it says).
- `argument`: One cause, measured: every corrupting cell, Boost.Context's own switch on a carved stack, greenlet's and
  Tencent libco's copy-stack designs, and a copying library over `makecontext` whose every switch the runtime sees,
  keeps its values once the room is not given back by frame address, on this Mac and both Linux images. So the defect is the
  heal by address, not an unseen switch. A range tell repairs Boost and turns libco's design, right untold, into a
  segfault; greenlet has no range to tell. An identity per switch repairs all, and the runtime can read it at every
  switch it wraps; a forgetting binding still loses values. A correct binding must stay correct as written.
- `prediction`: **Boost.Context 1.92's binding `bfc.h` needs no added line under the resolution §1.12 requires**:
  `bfc_host.hero` and `bfc_host_01.hero` print `body holds 7` three times and `carved: 7`, exit 0, 3 of 3, plain and
  `--sanitize`, on this Mac, built `HEROES_RUNTIME=<the sitting's runtime> heroes-next build bfc_host.hero --library
  /opt/homebrew/opt/boost/lib`; and **door 13 at `DOOR_N=5000` (`cx13.hero`) then aborts with the room full, unless
  the resolution makes a C++ unwind a seen event**. Checkable the day the sitting's runtime is built; falsified by
  one `11`, one abort of `bfc_host`, or `cx13` running without a seen unwind.
- `condition`: a runtime prototype under which the corrupting cells (`own_carved`, `below`, `bfc_host`, `bfc_below`,
  `bfc_host_01`, `cpo_host`, `cpo_host_01`, `cpo_host_told`, `cpo_heap_told`, `cpu_heap`, on this Mac and both Linux
  images) are right with every binding unchanged turns the veto into **approve**, whatever else it keeps; so would a
  detector that tells the untold carved and copying shapes in true words **before** any wrong value (route A),
  which nobody has shown a fact for. A real library that cannot run correctly unless it tells the runtime would make
  me accept a tell as a requirement for that library alone.

## One line per question

1. **Libraries**: Boost.Context 1.92 (caller's stacks, own switch, unseen: carved loses values), OpenSSL 3.5.9 and
   3.5.7 ASYNC (heap or the caller's stacks through its allocator; `makecontext` plus `_longjmp` or `swapcontext`,
   seen: right everywhere), greenlet 3.5.6 (the thread's own stack, copied; read), Tencent libco (heap, copied;
   historian); *changed by* a library whose own switch onto heap stacks loses a value today.
2. **A carved stack in a real program**: three libraries hand the stack's memory to the caller and none of the read
   documents forbids the thread's stack; no real program doing it was found (documents searched, not code); greenlet
   puts every coroutine on the thread's own stack by design; *changed by* a real program found carving.
3. **Doors**: Darwin answers every `dlsym` handle, the C library's included (`dl3` 1111); Linux x86-64 and arm64 at
   `3ec775b66` leave `RTLD_NEXT`, the C library's handle and `dlvsym` by all three handles unseen (`dl3` `state 1`);
   door 6 never seen (counter 0/0/0/0); a copying library is corrupted with every switch seen; *changed by* the
   `dlsym-make` lane's base.
4. **Route D's binding**: one call before the first jump, written and run; a range tell repairs separate stacks and
   breaks shared ones; an identity tell repairs all; no ABI move (`HERO_RUNTIME_ABI` 32 stands by
   `heroes_runtime.h:1091-1093`'s rule), a promise in `hero_os.h` (decision 2228); *changed by* a shared-stack cell
   right under a range tell.
5. **Route B from C**: door 13 (this Mac and arm64) and door 6 (x86-64) at 5,000 iterations over a 256 KiB value go from
   exit 0 to *"stack exhausted ... 1 GiB"*, a told abort in false words; *changed by* a route-B runtime that reclaims
   those without a seen jump.
6. **Windows**: unread by me; the historian read Boost's Windows switch keeping the thread block true at `develop`;
   a copying library is the thread block's blind spot (inference); *changed by* a run on the box.
7. **x86-64 at `3ec775b66`**: the critic's cells hold (`own_carved` 139 / UAF, `unseen_carved` 134 in the same 823
   bytes as arm64, `below` 139 / UAF); door 6 runs 5,000 jumps today; my shapes repeat the Mac's rows with
   segfaults where Darwin panics; *changed by* the CI's native leg disagreeing.
8. **Clauses**: arm-r's r1 to r4, arm-t's t1 to t3 and arm-p false today, each with its program
   (`ffi-clauses.md`); repaired arm-t (an identity) run true for the told; the untold half untested until a route-E
   runtime; *changed by* that runtime's cells.

Finished 2026-11-25 11:01 CET by this Mac's clock (`date`). No container of mine is left (`docker ps -a | grep -c p259-ffi` prints 0).
