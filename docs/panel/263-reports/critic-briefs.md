# Panel 263, the completeness critic's first reading: the briefs, before any seat starts

Written as it went, 2026-11-27 from 10:37 to about 11:00, in the detached worktree `.claude/worktrees/263-critic` of
`main` at `f9f42fcc4` (`f9f42fcc4ae54dc5e4b4305bb7eaec78cd72970e`, 10:36:09 +0100; `git worktree add --detach
.claude/worktrees/263-critic main`, `git status --porcelain` 0 lines), scratch `.claude/worktrees/263-critic-scratch/`,
`TMPDIR` there. **The seed's compiler** built there (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`,
`real 29.16 user 22.89 sys 0.98`), `sources 1e1eb23a1927d35b`, the clerk's; **stage 2** (`./heroes build
selfhost/main.hero -o heroes-next`, `real 311.81 user 204.83 sys 48.61`, load 13 to 16 on 8 cores, a gate running
beside it, so a duration nobody should quote), `sources 50e8403d5a885325`. Linux arm64: one container `c263` at a time,
`--rm --pull never`, the seed's compiler built inside (54 s), **not** stage 2 (said so in every Linux line below). The
coordinator's addition of about 10:45 (step 199j's stop, issue 2871) is § 9. **Ran** and **read** are kept apart; no
verdict on any route is given. Builds and runs: `.claude/worktrees/263-critic-scratch/` (`rs/`, `lx/`, `st/`, `img/`); the small programs and both `run.sh` outputs kept beside this report under `programs/critic/` (`static.hero` with `static-co.h`, kind 5; `adj.c`; `maps-co.h`, `co.h` with `CO_MAPS`; `linux.sh`).

## 1. What moved under the briefs since `a246032bf` (ran, 10:37 and 10:40)

- `git log --oneline a246032bf..main | wc -l`: **9 commits**. Under `runtime spec selfhost/emit`: `d8ab3094e` (the merge
  of lane-sig-family) and `7fefc7d1b`, **one line of `runtime/parts/rebind_image.c`** (a function-static array of two
  segment names made a local; Darwin only). `git diff --stat a246032bf main -- runtime spec selfhost tests/harness seed`:
  that line, `tests/harness/spec_refusals.hero` **+4 −1 (222 lines at the base, 225 on `main`; `CLAIMS` `:32` and
  `SENTENCE_OPENS` `:39` unmoved; the net's test now asserts the arm `PARKED`)** and `tests/harness/one_file_units.hero`.
  No `seed/`, no `selfhost/`, no `spec/`. Every line count of `00-shared.md` § 3 and `compiler-engineer.md`'s map reads
  the same on `main` (`wc -l` of the 18 files), every cited line number re-read and standing (but `rebind_entries.c:497`
  is the function's head and the loop `for (int k = own->left_n - 1; …)` is `:499`).
- **The lanes at 10:40**: `lane-sig-family` **no longer exists** (merged into `main` at `d8ab3094e`; `git rev-parse
  lane-sig-family` fails); **`lane-sig-family-3`** at `34ba67b79`, nothing beyond `main`; `lane-unwinder` at `9f5c59e6b`,
  nothing beyond `main`, container `unw` up; `lane-land262` **at `main`'s tip `f9f42fcc4`** (it took `main`), nothing of
  its own yet; `lane-win-paths` and **`lane-win-paths-h`** at `6363db2a4`; `lane-site-rebel` at `a388cb46b`. The briefs'
  never-touch lists name *sig-family* and not *sig-family-3* or *win-paths-h* (`git worktree list`).
- **The reproducer on `main`'s tip ends as the clerk's v2 build did** (§ 2): so the one-line runtime change moved no
  outcome I could see, but the seats' base (`a246032bf` in every brief) is now one Darwin runtime line behind `main`.

## 2. The reproducer re-run (ran)

### 2.1 This Mac, stage 2 at `f9f42fcc4` (`run.sh <heroes-next> rs 3 30`, `real 33.70`; `rs-mac.txt`)

| build | 3 runs each | the clerk's |
|---|---|---|
| `malloc-O2` | **`panic: malloc.deep ran on a thread this program did not start`, 134: 3 of 3** | v2's; v1 and runsh hung |
| `malloc-O0` | **exit 138 (Bus error), nothing said: 3 of 3** | v2's; v1 and runsh said `stack exhausted` |
| `malloc-san-O2` | `stack-overflow` in `hero_thread_guard`, 134: 1; `nested bug in the same thread`, 1: 2 | 1 and 5 of 6 |
| `malloc-san-O0` | `stack-overflow` in `h_malloc_deep`, 134: 3 of 3 (and `nested bug`, exit 1, once more under `CO_SAY`) | 6 of 6 |
| `guarded`, `bare`, `thread`, both levels | `panic: stack exhausted in <m>.deep`, 134: 3 of 3 each | the same |
| canary | the clerk's table to the byte (`-O0` n 1,500: 23,828 once, 23,964 twice) | the same |

`CO_SAY=1` once per build: `malloc` at both levels, the probe at `lo − 0xC000`; under `--sanitize` the stack is in ASan's
heap (`0x1011f4800`) and the probe at `0x62e000000400`, nowhere near: **the sanitized builds are another layout**.

### 2.2 Linux arm64, the seed's compiler at `f9f42fcc4` (`lx/linux-out.txt`; `run.sh` 63 s inside)

- `malloc`, `guarded`, `bare`, `thread`, both levels: **`panic: stack exhausted in <m>.deep`, 134, 3 of 3 each**;
  `malloc --sanitize` both levels: **`nested bug in the same thread`, exit 1, 3 of 3 each** (the draft's carried Linux
  line said nothing of `--sanitize`).
- Canary: `-O2` n 9,000 **19,445 to 19,453 bytes** written (lowest 25,936 below), n 10,000 **43,441** (lowest 57,936),
  `-O0` the Mac's numbers within 3 bytes; exit 0 every run. **The defect's widest form is on Linux too.**
- **Where glibc puts the block, read at last** (`/proc/self/maps` from inside the program, a copy of `co.h` in my scratch
  with `CO_MAPS`): the 256 KiB `malloc` is **its own anonymous mapping** (`ffffbc291000-ffffbc2d2000`, the stack at
  `+0x10`), **nothing mapped in the 8 MiB below it**, the main thread's TLS in another mapping **above** it
  (`ffffbc2d3000-ffffbc320000`). So Linux tells `stack exhausted` because the page under this block is free in this run:
  **a layout fact, like the Mac's** (§ 4, shape B, makes it otherwise with one more mapping).

### 2.3 The forms the briefs marked unrun, run on `malloc.hero` itself (this Mac, stage 2; `CO_N` overrides `n`)

- **`-O0`: the defect's own program returns the right answer at exit 0 with `CO_N` 1,600, 1,700, 1,800, 2,000, 2,600,
  3,000, 4,000, 6,000, 10,000, 20,000 to 24,000, 26,000 and 30,000** (1 to 3 runs each, every one exit 0; the answer
  equal to `thread-O0`'s where compared, n 1,700 to 4,000 and 20,000); at 192 bytes a frame that is about **70 KiB to
  5.2 MiB written below the stack**,
  through this thread's whole thread-local block and the allocator's region below it; first exit 138 at n 50,000 (1 of
  2), every run at 100,000 and up. `00-shared.md` § 2.2's *"nothing is mapped below that region"* is one `vmmap` of one
  run, and § 2.3's *"122,120 bytes"* is the canary's floor, not the defect's: **the briefs understate the widest form by
  a factor of about 40.**
- **`-O2`: n 9,600, exit 0, the right answer** (about 45 KiB below `lo`: run.c's thread-local buffers, § 3.1); **from n
  9,800 up, the false thread message inside the one callback**, 2 of 2 at each of 9,800, 10,000, 11,000, 14,000. So
  *"a recursion that ends inside the 48 KiB, then a second callback"* (§ 5 item 3) is not needed to see it: `deep`'s own
  entry guard refuses at the next recursive call.
- **`--sanitize` is no instrument of these writes**: `malloc-san-O2` with n 7,000, 9,000, 12,000 and `malloc-san-O0` with
  n 1,200, 1,500, 2,000 all **exit 0, nothing said**, 1 run each (n 12,000 at `-O2` passes the block by about 120 KiB at
  the plain build's 32-byte frame, more if ASan's frame is larger). A seat must
  not read a sanitized exit 0 as *0 bytes written*: only a count (the canary) or a watchpoint says it.
- **Where the stack lands, counted**: `CO_SAY=1 CO_N=1`, 30 runs of each `malloc` build: **58 of 60 at `lo − 0xC000`**,
  **2 of 60 with the stack at a 4 MiB boundary 4 MiB above the probe** (`lo % 4 MiB == 0`), and one more such run among
  my `-O0` sweeps. The clerk's *"eleven readings of eleven"* is true of eleven; the outcome is a fact of the build **and
  of the run**.

## 3. Readings the briefs left open, settled by a command

### 3.1 The thread-local block, measured (`otool -l`, `nm -m` of `rs/malloc-O2`)

`__thread_data` 8 bytes + `__thread_bss` `0xa5b0`: **42,424 bytes**, dyld's `malloc` of it the 48 KiB (`0xC000`) slot
below the stack; **40** TLV symbols in this image. Distance below `lo` in the 58-of-60 layout:

| below `lo` | word |
|---|---|
| 0 to 6,728 | the 48 KiB slot's slack above the block's 42,424 bytes |
| 6,728 to 39,504 | `hero_run_words` (the first word written), then `hero_run_variables` to 47,696 |
| 48,872 | `hero_signal_stack`, the whole signal record (to 47,696) |
| 48,880 | `hero_stack_telling` |
| 48,936 / 48,928 | `hero_stack_alt`, `hero_stack_alt_size` |
| 48,944 | `hero_callback_mine` (**a pointer**: the next callback's `hero_callback_in()` writes through what the recursion left) |
| 48,952 / 48,960 | `hero_stack_hi`, `hero_stack_lo` (what the guard's handler compares a fault with) |
| 49,111 | `hero_thread_is_home` (the clerk's lldb reading, confirmed) |
| 49,120 | `hero_alloc_inside` |
| 49,152 | `co_tls_probe` (offset 0) |

So § 2.2's *"which of them the recursion wrote in which build is unread"* is a table now, by distance; which bytes each
build wrote there stays the compiler-engineer's watchpoint. And **the count *43 in 13 files* is `runtime/parts/` alone**:
`grep -rnE "^\s*(static )?(extern )?_Thread_local" runtime/` gives **225 in 49 files**, **182 in `runtime/platform/`**
(`hero_sys_kind`, `hero_poll_kind`, … the layer's error words), linked into a program that binds the layer and lying in
the same block.

### 3.2 The hang's signal is a question with a cheap answer

`ground/sample-malloc-O2.txt` shows `hero_thread_guard + 56` twice above `hero_rebind_tramp + 384`; `hero_thread_refuse`
and `hero_signal_stop` are in the same translation unit and `static`, so `+56` may lie inside the inlined refusal's own
`raise(SIGABRT)`, or the signal may be the run-out's `SIGSEGV`/`SIGBUS` (read, not run, either way). The trampoline's
signal is its argument: one lldb `attach` to a hung binary and `frame variable` in `hero_rebind_tramp` settles it (the
compiler-engineer's item (ii) asks; my builds on `main` did not hang, so I could not).

## 4. Shapes nobody listed, run where seconds allowed

- **A. A stack inside a static array** (`st/`, a copy of `co.h` with kind 5, `static char co_static_stack[256 KiB]`; this
  Mac, stage 2): **`-O2` with n 9,600, 10,000, 12,000: prints the right answer, then hangs, exit 124 at 20 s**; n
  10,000,000 and `-O0` at every n: **hangs, exit 124, nothing said** (8 of 8 runs hang). **Below the array lie the
  runtime's process-wide words** (`nm -n st/static-O2`): `hero_run_live`/`hero_run_guard` (4,188), the saved previous
  handlers the guard **chains to**, `hero_lease_prev_*` (4,216 to 4,248) and **`hero_stack_prev_bus` / `hero_stack_prev_segv`
  (4,272 / 4,288)**, `hero_frame_held`, `hero_lend_slabs`, `hero_kept_key`, `hero_callback_counts` (6,464), the rebind
  region tables (6,496 to 22,888), `hero_rebind_records`, and **`hero_signal_stopping` 220,528 below** (which, written
  nonzero, sends every later stop into `pause()` with every signal held). A fifth outcome, a hang after the right answer,
  and the guard's chain target in reach of a few KiB of recursion. No route row names a stack in the image's data.
- **B. Neighbouring stacks are adjacent on both platforms, in opposite directions** (`img/adj.c`, two `malloc`s and two
  `mmap`s of each size): **Darwin**, a later 64 KiB or 256 KiB `malloc` and a later 256 KiB `mmap` lie **directly above**
  the earlier (the recursion on the later writes the earlier's top, where a suspended coroutine's frames are); **glibc**,
  `malloc` of 256 KiB, 1 MiB, 8 MiB and raw `mmap`s are top-down, the later **directly below** the earlier (4,096 bytes
  between mappings that include glibc's header page, 0 for raw `mmap`), and 16 KiB / 64 KiB come from the heap
  ascending. So **`bare.hero`'s told end and Linux's `malloc.hero` told end are each an accident of being the last
  mapping**: one more mapping made after the stack (a second coroutine, a thread, a library) puts writable memory under
  it. § 5 item 4 names the shape; it must name the direction per platform and add the `mmap`-without-a-guard case.
- **C. (read, unrun) A stack shared by two coroutines** (a copy-stack library: greenlet, libco's shared stack, which panel
  259's synthesis names): one range for two coroutines, the stack's contents copied out and in on each switch; it is the
  shape where (h) breaks C (the library copies its own block) and where (a1)'s guard inside the shared block survives
  every copy.
- **D. (read, unrun) A recursion THROUGH C**: Heroes → C → Heroes … unbounded (a tree walker in C calling the callback
  that calls the walker); § 5 item 6 has one nested entry, not the alternation; under (e) every level passes the thunk's
  probe, under (c) every Heroes level is checked and the C frames between are not.
- **E. (read) A Heroes heap value below the block**: an array allocated before the stack, read after the callback
  returns at exit 0 (§ 2.3's `-O0` runs write megabytes of the allocator's region): a wrong answer at exit 0, the worst
  form, unlisted; and **C `free`ing a neighbour** after such a run (the allocator's own words below written).
- **F. (read) The canary under `--sanitize`** is not in `run.sh`, and § 2.3 above shows why it matters.

## 5. Negative sentences and claims about the option set, searched

- **§ 3.2 *"whether anything is mapped or protected below `lo` … nothing"* and the ffi-pragmatist's item 4 *"a probe of
  the page below: none of them is in the runtime today"***: the grep is right word for word and the claim is not. **The
  runtime already reads memory through the kernel, failing rather than faulting where nothing readable is mapped**:
  `mach_vm_read_overwrite` (`rebind_jump.c:78`, `rebind_record.c:79`) and `process_vm_readv` (`sigjump_record.c:116`,
  `:159`), used today to read a context *"wherever it is mapped"*. Both answer *is the byte below `lo` readable*, which is
  the judgement (f) and (g) need (they cannot tell `PROT_NONE` from unmapped, and need not: both are *of no access*).
- **§ 3.3 *"No set of the functions reachable from a callback exists on `main`"***: true by `grep` and by a wider one
  (`callees`, `transitive`, `reachable`); but **`emit/members.hero:137-216` already walks the call graph from a frontier**
  (`emit/worklist.hero`, `instances.function_at`), and `callback_guard.hero`'s `Callbacks` is *"ONE computation with every
  consumer — the guard line, the thunks, the table and a unit's reach"*: the base (c2)'s set and R2's would be built on,
  which the compiler-engineer should be pointed at.
- **Route (b)'s *"every stack a callback is entered on"***: false by the thunk's own frames. `hero_callback_entered` →
  `hero_callback_asked` → `hero_sig_inside`/`hero_rebind_inside`, and on a refusal `hero_signal_stop`, run **on C's stack
  before any switch** (`sigguard.c:912-990`, the thunk's first line `callback_thunk.hero:137`); a callback entered within
  those frames' depth of C's lowest byte writes below it under (b) as written. The row should read *every stack, but for
  the thunk's own frames*, or the switch must come first.
- **Route (i)'s *"not the defect's layout"***: 2 of 60 runs put the stack at a region's first byte (§ 2.3), where (i)'s
  premise may hold; and on Linux the block is its own mapping with nothing below, (i)'s layout exactly (§ 2.2).
- **(d) *"the clerk finds no arrangement"*** and **(a)'s *"an allocator may write its own words when C frees it"***:
  questions, correctly marked; nothing I ran settles them.
- **Windows, *"no route of the signal guard stands there"*** (`sigguard.c:215-221`) and *"`CreateFiber` … finds
  nothing"*: re-run, standing (`grep -rln "CreateFiber\|SwitchToFiber\|ConvertThreadToFiber" runtime/ selfhost/`: none).
- **`-fsplit-stack`'s 0 `__morestack`**: re-run at `-O0`, 0 on Apple clang 21.0.0 and on Debian clang 22.1.8.
- **The image's absences**: re-run: no `/usr/bin/time`, `perf`, `valgrind`, `gdb`; `lldb`, clang 22.1.8, glibc 2.41,
  OpenSSL 3.5.7, no `/usr/include/boost/context`; image `338a1044ac90`.

## 6. Facts re-run that stand (one line each)

Spec 526 lines, 36,316 bytes, sha256 `736b534c…efd3`; § 9 at `:340-341`, § 13 at `:438`, `grep -c
"makecontext\|coroutine"` 1. Vendored 10,374 / 10,563 with both compilers; the seed's `STALE … 24de425e6c28e69b …
25b307910fc3ea19`, exit 1; **stage 2 prints `real 13849 claude-opus-5, 2026-11-26`** from the pin, no request. The six
cuts of `ground/parts/` measure 33/35, 90/91, 88/88, 348/350, 291/291, 27/27 as the table says; `s13-adopted-262.txt` is
`spec-adopted.txt` byte for byte. `SPEC_TOKENS` 10563 (`suite_spec.hero:114-115`), `REAL_TOKENS` 13849 (`:175-176`),
`SPEC_DIGEST` (`:227`), `pinned.hero:55-64`, the ledger's row 263. design.md `:255`, `:646`, `:3610-3612`, `:3642`,
`:4184-4189`. 43 thread-locals in `runtime/parts/` in 13 files; `mprotect` in four files; 2870's four goldens all on the
guarded `mmap` header; `fixedbugs-26/27/28` 41/95/39; the 17 numbers 126; the fibre grep 105; step 199's sixteen
subjects, 13 of 16 naming a stack word, every hash as listed; panel 104's numbers (`:47`, `:66-69`); panel 262 `:223`,
`:575-591`; panel 259 `:39-40`; OpenSSL 4.1.0 here, boost.context's three mangled `fcontext` symbols (branches to its
own assembly `_make_fcontext`, `_jump_fcontext`, `_ontop_fcontext`, `otool -tV`).

## 7. What a seat was handed and cannot check, and the clerk's unrun facts no seat may take as a premise

- **The real token count**: 13,849 is a pin read (`taken 2026-11-26`), not a count of today; and **the instrument's own
  line** says *"the FFI floor mortgages 60 of it (panel 030 R3), so what is measured against the ceiling is 13909"*:
  the headroom is **2,475**, not the spec-warden brief's 2,535 (`./heroes-next measure spec/heroes-spec.md`).
- **Every precedent** in `historian.md` and in the route rows (*"the route cgo takes"*, `g0`, issue 62440, Go 1.3,
  `stackguard`, Rust's guard, stacker, Lua, CPython, HotSpot, OCaml 5, GHC, Windows fibres, ASan's annotations): recalled.
- **OpenSSL's default fibre, *"32 KiB … `OPENSSL_malloc`"***: carried from panel 259's reading of Homebrew's libcrypto
  **3.5.9**; `pkg-config --modversion libcrypto` here is **4.1.0** (openssl@4; openssl@3 also installed), whose
  `nm -u` imports `_mmap` and `_mprotect` beside `_makecontext`: whether 4.1.0's default fibre is still a `malloc`'d
  32 KiB block, or guarded, is unrun. The ffi-pragmatist's prediction *"OpenSSL's default ASYNC fibre … writing 0 bytes
  below its stack"* rests on it.
- **glibc's 128 KiB threshold**: now read for 256 KiB (its own mapping) and for 16 and 64 KiB (the heap), § 4 B; the 1
  MiB and 8 MiB rows also mapped. Not read: the dynamic threshold after a `free`.
- **§ 2.2's *"That block holds every `_Thread_local` of the program's image"***: true of the executable's block (§ 3.1);
  a C library's own `_Thread_local` (a dylib's) is another block, so *"the program's image"* must not be read as *every
  image*.
- **The four outcomes as *"a fact of the build's layout"***: and of the run's (§ 2.3); my `main` builds give v2's two.
- **The draft's carried facts** (20 of 20, 4.1 MiB, *"a second region mapped below"*, Linux 3 of 3, the base hanging 3 of
  3): carried; my `-O0` runs agree with *"a second region mapped below"* and not with § 2.2's *"nothing is mapped below"*.
- **Route (b)'s *"the switch needs no knowledge of C's stack"***: § 5's thunk frames; and the nested entry (a callback
  inside a callback on the runtime's stack) does need to know whether it is already on its own.

## 8. Routes nobody listed (each a question; what would have to be true)

1. **(a4) (a) with `free`, `realloc` and `munmap` wrapped**, the guard taken off before the allocator writes there: the
   rebind (Darwin, ten names today) and the symbol wrappers (Linux) are the mechanism the runtime already uses for
   `makecontext`; it answers (a)'s main hole on the allocator path and leaves a pool allocator, `read` into the block and
   the shared cache's own `free`.
2. **(n) a static bound at the thunk**: (e)'s probe compared with the maximum stack the callback's call tree can use,
   computed at compile time (clang `-fstack-usage`, the call graph of § 5's second bullet), a recursion in the tree
   checked by (c) or refused, an indirect or C call in it making the bound unknown; one compare per callback where the
   tree is bounded.
3. **(o) the runtime's guarded allocator handed through a library's own door**: `ASYNC_set_mem_functions` (OpenSSL), a
   boost `stack_allocator`, libco's `co_create` attributes: closes OpenSSL's default fibre with no protection in C's
   memory and no swapped stack, reaches no library without such a door.
4. **(p) a hardware write watchpoint** on the bytes below `lo` (Linux `perf_event_open` breakpoint with `sigtrap`;
   no user API on Darwin; 8 bytes a register on arm64 Linux): likely ruled out, to be ruled out by name.
5. **(q) the process-wide words out of reach too**, beside (l): shape A writes `hero_stack_prev_segv`, the handler the
   guard chains to, and `hero_signal_stopping`; (l) as written moves only the thread-locals.

**And the reading nobody named**: Linux tells this program's run-out only because glibc gave the block its own mapping
with a free page under it (§ 2.2); the Mac's false message, hang or 138 and Linux's told end are the same defect under
two allocators, so *"Linux arm64 tells it `panic: stack exhausted` at both levels"* is no platform's immunity.

## 9. The coordinator's secondary question: step 199j's stop (issue 2871, `4ca0c34f3`), what the briefs must add

Read and run on `main`: the stop is **`hero_signal_switch_begin`, `sigguard.c:600-620`**, which calls `hero_signal_stop`
directly; **no arm of `hero_signal_refuse`** (so `tests/harness/spec_refusals.hero`, which reads that function's arms,
holds none of its words) and **no callback in it**: each delivery marks the contexts of the switch it interrupted
(`hero_signal_in_switch`, from Darwin's trampoline `rebind_entries.c:723` and Linux's `siginstall.c:169`), and the switch
wrappers (`rebind_jump.c:154`, `sigjump_save.c:249`) stop a handler's `swapcontext`/`setcontext` naming one. Its words open *"panic: a signal handler called `<call>` naming a context that the code
the signal interrupted was itself switching from or to"*; its seven cases (`tests/golden/run/fixedbugs-2871-*`, header
`…-a-handler-switching-contexts-inside-a-switch.h`) are four refused and three that run; the commit says C alone dies too
and that four switch-family cases were red in Linux arm64 on the base as well. The briefs owe:

- **`00-shared.md`**: a section stating the stop, its file and lines, its words, its cases, that it is no refusal arm and
  no callback's abort, and the two spec texts it is to be read against: § 13's handler sentence (panel 257's, `:438`)
  and panel 262's adopted R1 (`spec-adopted.txt`), with panel 258's stop sentence there (*"Where C jumps out of a signal
  handler whose signal interrupted this runtime's allocator … the program aborts"*) as the shape a sentence would take;
  and **that the stop reads the same record `own` the recursion overwrites** (`rebind_entries.c:722-723`, the hang's own
  line), so under this defect it may fire in its words falsely or loop: a shape to run, which ties the two questions.
- **`spec-warden.md`**: the sentence or the reason none is owed, its price, its home (§ 13 beside 258's), its
  instrument (none reads these words today), **and whether the one `--refresh` covers one text holding both answers or a
  second request is the coordinator's word**; the predictions heading covering the seven 2871 cases.
- **`ffi-pragmatist.md` and `compiler-engineer.md`**: the seven cases at both levels and `--sanitize`, both platforms
  (`heroes-next run tests/harness/main.hero -- heroes-next run fixedbugs-2871`), the four red Linux switch cases named,
  and **one shape each that a correct program could be stopped by** (a handler switching to a context the interrupted
  switch already finished saving into, say); x86-64 and Windows by reading.
- **§ 7**: this question can move § 13 whatever route the main question adopts, so it alone may register the blind
  reading. PARKED, as the coordinator says, is R1's first sentence's and not this sitting's.

## 10. § 7's reading of the lane

Right about (f) and about (b)'s held stacks. Missing: **(h)** holds a runtime stack per context `makecontext` named
until C frees its block, which the runtime does not see (the held-memory sentence's question, as (b)'s); **(g)** carries
(b)'s on (f)'s set; **(a), (c), (e) need § 9 to say what happens on a stack in the image's own data** (shape A) as well
as on one no wrapper saw; and **the secondary question** (§ 9 above). Not needed, as far as reading goes: a sentence for
(l) or (m) alone, which change no promise.

## 11. The historian's list, by reading only

Nothing in the brief's own words reads as recalled wrongly that I can say without the web. One framing to give the
historian and the seats: **cgo as recalled is (b) and (c) together**, the callback's Go code on a goroutine stack the
runtime owns *and* a check at each Go function's entry against `stackguard0` that grows it by copying; so *"the route
cgo takes"* is a question about both rows, not (b)'s alone.

## What the coordinator must repair before any seat starts

1. **Replace § 2.3's ceiling and § 2.2's *"nothing is mapped below"* with the defect's own runs**: `malloc-O0` returns
   the right answer at exit 0 up to `CO_N` 30,000 (about 5 MiB below the stack), `malloc-O2` at 9,600, the false message
   from 9,800. Settled by: `for n in 1600 2000 3000 10000 20000 30000 50000; do CO_N=$n timeout -k 5 20 <rs>/malloc-O0;
   echo $?; done` and the same with `malloc-O2` at 9600 9800, the answers held to `thread-O0`'s.
2. **Say that `--sanitize` cannot see these writes**, in `00-shared.md` § 2 and every seat's *"plain and `--sanitize`"*:
   `for n in 7000 9000 12000; do CO_N=$n <rs>/malloc-san-O2; done` and `for n in 1200 1500 2000; do CO_N=$n
   <rs>/malloc-san-O0; done`, exit 0 each.
3. **Add shape A, a stack in a static array, to § 5 and to every route row's *reaches***, with what lies below it:
   `nm -n <the binary> | grep -B40 co_static_stack`, and the runs (`st/static.hero`, kind 5 of `st/co.h`).
4. **Make § 5 item 4 directional per platform and add `mmap` without a guard with a mapping made after it** (shape B):
   `clang img/adj.c && ./a.out` on both platforms.
5. **Correct the negative on probing the page below** (§ 3.2, `ffi-pragmatist.md` item 4): `grep -n
   "mach_vm_read_overwrite\|process_vm_readv" runtime/parts/*.c`.
6. **Correct route (b)'s *"every stack"*** for the thunk's own frames on C's stack: read `sigguard.c:912-990` and
   `callback_thunk.hero:121-160`.
7. **Add the secondary question to `00-shared.md`, `spec-warden.md`, `ffi-pragmatist.md`, `compiler-engineer.md` and
   § 7** as § 9 above lists, including whether the spec-warden's one request covers it: `sed -n '600,620p'
   runtime/parts/sigguard.c`, `grep -n hero_signal_in_switch runtime/parts/*.c`, `ls tests/golden/run | grep 2871`.
8. **Put § 3.1's table of distances in § 2.2** and correct *"43 … in 13 files"* to `runtime/parts/` with the platform
   layer's 182 beside it: `otool -l <bin> | grep -A4 __thread`, `nm -m <bin> | grep 'tlv\$init'`, `grep -rnE
   "^\s*(static )?(extern )?_Thread_local" runtime/ | wc -l`.
9. **Re-base or say why not**: every brief names `a246032bf`; `main` is `f9f42fcc4`, one Darwin runtime line and the
   instrument's PARKED assert ahead (`git log --oneline a246032bf..main -- runtime spec selfhost tests/harness`); correct
   `spec-warden.md`'s *"222 lines"* and *"lane sig-family changed it at 10:33"*, and name `lane-sig-family-3` and
   `lane-win-paths-h` in every never-touch list (`git worktree list`, `git branch --list 'lane-*'`).
10. **Give the spec-warden the instrument's headroom, 2,475, and § 9's sentence's own ratification**: `./heroes-next
    measure spec/heroes-spec.md` (the FFI floor's 60), `git log -S"running out of stack aborts" -- spec/heroes-spec.md`
    (`641b43017`, panel 232) and `docs/panel/232-*.md:171-174` and `:244-251` (K1, *"It lands with defect 2253 repaired on
    Darwin and on Linux"*), which no brief cites.
11. **Mark OpenSSL's default fibre as unrun on 4.1.0** in `ffi-pragmatist.md`: `pkg-config --modversion libcrypto`,
    `nm -u $(pkg-config --variable=libdir libcrypto)/libcrypto.dylib | grep -E "mmap|mprotect|context"`.
12. **Give `run.sh` a kill after its timeout**: a binary in `hero_signal_stop`'s `pause()` holds every signal, SIGTERM
    included, and GNU `timeout` 9.12 then waits for ever (shape A writes `hero_signal_stopping`): `timeout -k 5 "$T"`.
13. **Add the routes nobody listed** (§ 8: (a4), (n), (o), (p), (q)) to § 4 as questions, and § 10's corrections to § 7.
14. **Say in `historian.md` that cgo is to be read as (b) and (c) together**, and in `00-shared.md` that the Windows box
    is up until about 12:50 for lane unwinder (`coord/unwinder-brief.md`): *by reading only* is then a choice, written
    as one.
