# Panel 263, the shared brief: a callback recursing past a stack C allocated writes below it before anything stops it

**Repaired 2026-11-27 from 10:58 to 11:15, by the coordinator's repair (the panel skill's step 3c), before any seat
starts**, on the completeness critic's first reading (`docs/panel/263-reports/critic-briefs.md`, its fourteen items under
*What the coordinator must repair before any seat starts*, cited below as **the critic's item N**, and its sections as
**critic § N**). **Re-based on `main` at `f9f42fcc4`** (below). Every sentence the repair changed was re-run there, on
this Mac with the seed's compiler built at that commit (`clang -I runtime seed/heroes.c runtime/runtime.c -o
../panel263-scratch/heroes-f9f`, `real 23.35`, `sources 1e1eb23a1927d35b`, `ground/f9f42fcc4/seed-build.txt`) and in
Linux arm64 with the seed's compiler built inside the image (30 s, `ground/f9f42fcc4/linux.txt`), the command named
beside it; its outputs are `ground/f9f42fcc4/`. Where the critic's run is the only one (its stage 2, `sources
50e8403d5a885325`), the sentence says so. What the repair changed, in order: the damage as measured on the defect's own
program and on both platforms (§ 2, the critic's items 1 and 2); the static-array stack and the adjacency of
neighbouring stacks (§ 2.5, § 2.6, § 5; items 3 and 4); the false negatives (§ 3.2, § 4 (b), § 2.3; items 5, 6 and 8);
the routes the critic added (§ 4; item 13); the clerk's unrun facts marked (§ 2.7); **the secondary question, step
199j's stop, issue 2871** (§ 3.9, § 7; item 7); the never-touch lists and the Windows box (§ 8; items 9 and 14); panel
232's R4 cited for § 9's sentence (§ 3.4; item 10); `timeout -k 5` in `run.sh` (item 12).

**Widened 2026-11-27, at about 10:33, by the coordinator**, on § 7 (the panel skill's *"When in doubt take the full
panel"*, and CL-023's choice of seats): **the sitting has four seats and the critic**, `compiler-engineer`,
`ffi-pragmatist`, **`spec-warden`** (`spec-warden.md`) and **`historian`** (`historian.md`), the critic reading all four
seats' briefs first. **The llm-ergonomist's blind reading is registered only if the adopted route moves a sentence of the
spec, and its paid run is the author's to authorise**: nothing paid now, no blind seat launched, no `claude -p` session.
The historian verifies precedents by web search; **a seat names a precedent as a fact only where the historian's report
marks it `verified`**, and until then as a question.

**Convened by the author**, 2026-11-27 at about 10:01, the answer *1*, meant as: *a sitting of the soundness lane now,
before the tag* (`.claude/worktrees/rebel-scratch/coord/agents-live.md:534`), on the defect lane sig-family-2 drafted
beside its step 199i; first written for the soundness lane (`.claude/skills/panel/SKILL.md` § Two lanes), widened as
above. **No paid run is part of the sitting** (no `claude -p` session, API call or cloud run by any seat or the critic,
but the spec-warden's one `--refresh`, `spec-warden.md`); **no credential printed** (`gh auth status` where a seat must
know, never `gh auth token`). Under the author's amendment of 2026-11-22 (CLAUDE.md § Precedence) **the most robust route
wins at any cost in size and speed**; a seat's soundness veto stands as ever, and a cost is measured and written, never a
reason by itself.

**The base is `main` at `f9f42fcc4`** (`f9f42fcc4ae54dc5e4b4305bb7eaec78cd72970e`, 2026-11-27 10:36:09 +0100, *merge
lane-win-paths*; `git -C .claude/worktrees/panel263 checkout --detach f9f42fcc4` at 10:59, the briefs untracked and
travelling with it, `git status --short` the two folders `263-briefs/` and `263-reports/`). **The clerk wrote at
`a246032bf`** (10:03 to 10:35); **from it to `f9f42fcc4`** under `runtime spec selfhost tests/harness seed`: one line of
`runtime/parts/rebind_image.c` (Darwin, a function-static array made a local, `7fefc7d1b`) and
`tests/harness/spec_refusals.hero` +4 −1 (225 lines; `CLAIMS` `:32` and `SENTENCE_OPENS` `:39` unmoved), and
`one_file_units.hero` (`git diff --stat a246032bf f9f42fcc4 -- runtime spec selfhost tests/harness seed`); no `seed/`, no
`selfhost/`, no `spec/`, so every line pointer below stands (the critic re-read all, critic § 1; one correction:
`rebind_entries.c:497` is `hero_rebind_relived`'s head and its loop is `:499`). **`main` moved again at 10:58** to
`0f9da0309` (lane-site-rebel's merge) with nothing under those five paths (`git diff --stat f9f42fcc4 main -- runtime
spec selfhost tests/harness seed`: empty, 11:03), so `f9f42fcc4` stands as the base. The load: `uptime` 18.73 / 25.51 /
25.01 at 10:58 on 8 cores; Amphetamine holds the machine awake (`pmset -g assertions`).

## 1. The question, in ten lines

1. A C library runs a coroutine on a stack it took from `malloc` and handed to `makecontext`; nothing of no access lies below it.
2. A Heroes function C calls back on that coroutine recurses past the stack's lowest byte.
3. Nothing faults there: the recursion writes on through whatever lies below, as far as memory stays mapped.
4. On this Mac that is this thread's thread-local block, 48 KiB below the stack (60 of 60 placements read by the repair, 58 of 60 by the critic), the runtime's own words among them, then the allocator's region (§ 2.3).
5. **The defect's own program returns the right answer at exit 0 having written up to about 5.2 MiB below its stack** (`-O0`, `CO_N` 30,000; § 2.2); **`--sanitize` sees none of it** (§ 2.2).
6. Past that, what it does is a fact of the build and the run: a hang, the false *"ran on a thread this program did not start"*, exit 138 with nothing said, or `panic: stack exhausted` after the writes (§ 2.1).
7. On Linux the 256 KiB block is its own mapping with nothing below it in the runs read, so it is told: **a fact of glibc's layout, not an immunity** (§ 2.3, § 2.6; critic § 8).
8. A stack in the image's own data runs into the runtime's process-wide words: a hang on this Mac, exit 139 in Linux arm64 (§ 2.5).
9. Spec § 9 says *"running out of stack aborts"* (panel 232's R4); design.md §1.12 says *"must not corrupt memory"*: both false here, rank 3 of CLAUDE.md § Precedence. Which mechanism stops the recursion before the first byte below the stack, on every stack a callback can run on, at what cost per callback and per call, and with what to lane unwinder's R2 and the signal guard of steps 199a to 199o?
10. Does the route adopted, **or step 199j's stop (§ 3.9)**, move a sentence of the spec (§ 7)?

## 2. The defect, and its reproducer

**The draft**: `.claude/worktrees/rebel-scratch/coord/issues/d-lane-sig-family-2-1.md`, class `blocking`, `github: none`
(not yet filed; the coordinator files it, and the sitting's resolution closes it). It names `co/co.h` and
`co/malloc.hero` in lane sig-family-2's scratch; **that scratch is gone** (`ls .claude/worktrees`: no
`lane-sig-family-2-scratch`). So the clerk **wrote the reproducer again from the draft's words** (a 256 KiB `malloc`'d
stack, `getcontext`, `makecontext`, `swapcontext`, `deep(n)` returning `deep(n - 1) * 3 % 1000003 + n % 7`, `n`
10,000,000), modelled on panel 262's `so` (`docs/panel/262-reports/evidence.tar.xz`,
`docs/panel/262-reports/programs/compiler-engineer/so.h`): **`reproducer/`** beside this file.

| program | header, `co_run(kind: …)` | the stack the callback recurses on |
|---|---|---|
| `malloc.hero` | `co.h`, 1 | 256 KiB from `malloc`, nothing of no access below it: **the defect** |
| `guarded.hero` | `co.h`, 0 | 256 KiB from `mmap` with a `PROT_NONE` page below it (defect 2870's golden shape) |
| `bare.hero` | `co.h`, 2 | 256 KiB from `mmap`, nothing protected below it |
| `thread.hero` | `co.h`, 3 | no coroutine, the thread's own stack (the control) |
| `canary.hero` | `co.h`, 4 | the upper half of a 512 KiB `malloc` block, the lower half filled with `0xA5` and read after the coroutine ends |
| **`static.hero`** | **`static-co.h`, 5** | **256 KiB in a function-static array of the image's own data** (the critic's shape A, item 3) |

`CO_N` in the environment replaces `n` in every kind; `CO_SAY=1` writes the stack's `[lo, hi)` and the address of
`co.h`'s own `_Thread_local` word. `maps-co.h` (the critic's) adds `CO_MAPS`, Linux's `/proc/self/maps` near the stack,
used as `co.h` in a copy; `adj.c` (the critic's) prints where two `malloc`s and two `mmap`s of one size land. **`run.sh
<compiler> <out> <runs> <seconds>`** builds every program at `-O0` and `-O2`, `malloc` and `canary` under `--sanitize`
at both, and runs each under **`timeout -k 5`** (the critic's item 12: a binary that wrote the stop word waits in
`pause()` with every signal held, and `timeout` alone waits for ever), output cut by `head -c 65536`. Run by the repair
at `f9f42fcc4` on this Mac (`run.sh <heroes-f9f> rs-f9f 1 20`, `real 91.03`, `ground/f9f42fcc4/run.sh-mac.txt`) and in
Linux arm64 (32 s inside, `ground/f9f42fcc4/linux.txt`). Every `.hero` passed `heroes fmt --in-place` and `heroes
check`, exit 0.

### 2.1 How each build ends, at the default `n` (10,000,000)

| build | `malloc -O2` | `malloc -O0` | `malloc --sanitize` | `guarded`, `bare`, `thread` | `static` |
|---|---|---|---|---|---|
| this Mac, seed's compiler, `f9f42fcc4`, the repair (1 run, `T` 20) | **hung, exit 124** (4 runs of 4 more at 15 s) | `panic: stack exhausted in malloc.deep`, 134 | `-O2`: ASan's `stack-overflow` begun, then hung, **killed, exit 137**; `-O0`: `nested bug in the same thread`, exit 1 | `panic: stack exhausted in <m>.deep`, 134, each level | **hung, exit 124**, each level |
| this Mac, the critic's stage 2, `f9f42fcc4` (3 runs; critic § 2.1) | `panic: malloc.deep ran on a thread this program did not start`, 134, 3 of 3 | exit 138, nothing said, 3 of 3 | `-O2` `stack-overflow` 1, `nested bug` 2; `-O0` `stack-overflow` 3 of 3 | told, 3 of 3 each | hung, 8 of 8 (critic § 4 A) |
| Linux arm64, seed's compiler, `f9f42fcc4`, the repair (1 run) | `panic: stack exhausted in malloc.deep`, 134 | the same | ASan's `stack-overflow`, exit 1, each level | told, each level | **exit 139, nothing said**, 4 runs of 4 each level |
| this Mac, the clerk at `a246032bf` (`ground/v1-runs/`, `v2-runs/`, `runsh/`) | hung 5 of 5 (v1); the thread's words 4 of 4 (v2); hung 3 of 3 (runsh) | told 3 of 3 (v1, runsh); exit 138 2 of 2 (v2) | `-O2` `nested bug` 5 of 6, `stack-overflow` 1; `-O0` `stack-overflow` 6 of 6 | told | not run |

**Two builds whose machine code is the same end differently** (the clerk's v2 and runsh, `otool -t … | shasum`
`4994fe30d364` both at `-O2`), each the same way at every run; and the critic saw the placement itself move in 2 of 60
runs (§ 2.3). **So the outcome at the default `n` is a fact of the build and of the run, never of the program**, and no
row above is the program working.

### 2.2 The defect's own program, sized by `CO_N` (the critic's item 1; the repair's runs, `ground/f9f42fcc4/sweep-mac.txt`)

`for n in 1600 2000 3000 10000 20000 30000 50000; do CO_N=$n timeout -k 5 20 rs-f9f/malloc-O0; CO_N=$n … thread-O0; done`,
and the same at `-O2` with 9600, 9800, 10000, this Mac, the seed's compiler at `f9f42fcc4`, one run each:

| `malloc -O0`, `CO_N` | 1,600 | 2,000 | 3,000 | 10,000 | 20,000 | 30,000 | 50,000 |
|---|---|---|---|---|---|---|---|
| ends | exit 0 | exit 0 | exit 0 | exit 0 | exit 0 | exit 0 | exit 138 |
| answer, against `thread-O0`'s | equal (811991) | equal (564721) | equal (210722) | equal (742238) | equal (532635) | equal (441055) | `thread-O0` 134, `stack exhausted` |
| bytes below the stack, by 192 a frame less 262,144 | about 44 KiB | about 119 KiB | about 306 KiB | about 1.58 MiB | about 3.41 MiB | **about 5.24 MiB** | |

`malloc -O2`: `CO_N` 9,600 exit 0, the answer equal to `thread-O2`'s (480257); 9,800 and 10,000 **hung, exit 124** in
this build, where the critic's stage 2 build gave **the false thread message inside the one callback** from 9,800 up
(critic § 2.3), so *"a recursion that ends inside the 48 KiB, then a second callback"* is not needed to see it. The
critic's own sweep reached exit 0 up to 30,000 as well, and wrote *"about 70 KiB to 5.2 MiB"*; the repair's arithmetic
gives about 44 KiB at 1,600 (the canary's count at `-O0` `CO_N` 2,000 is 122,120 below against the arithmetic's
121,856). **The clerk's *"up to 122,120 bytes"* was the canary's floor, not the defect's** (the critic: *"by a factor of
about 40"*).

**`--sanitize` is no instrument of these writes** (the critic's item 2; the repair's re-run): `CO_N=$n timeout -k 5 20
malloc-san-O2` with 7,000, 9,000, 12,000 and `malloc-san-O0` with 1,200, 1,500, 2,000: **exit 0 and the right answer,
every one, nothing said**. And the canary under `--sanitize` (now in `run.sh`) counts the bytes ASan let through:

| canary, bytes of the 262,144 below the stack written (lowest below) | `-O2` 7,000 | `-O2` 9,000 | `-O2` 10,000 | `-O0` 1,200 | `-O0` 1,500 | `-O0` 2,000 |
|---|---|---|---|---|---|---|
| this Mac, plain | 0 | 25,941 (25,952) | 56,126 (57,952) | 0 | 23,964 (26,120) | 112,450 (122,120) |
| this Mac, `--sanitize` | 0 | 25,957 (25,968) | 57,953 (57,968) | **57,097** (64,628) | **129,390** (146,228) | **232,195** (262,144: the whole half) |
| Linux arm64, plain | 0 | 19,453 (25,936) | 43,441 (57,936) | 0 | 23,967 (26,120) | 112,451 (122,120) |
| Linux arm64, `--sanitize` | 0 | 19,469 (25,968) | 43,465 (57,968) | 39,750 (45,412) | 107,240 (122,212) | 219,700 (250,212) |

Every run exit 0. **A seat never reads a sanitized exit 0 as *0 bytes written***: only a count (the canary) or a
watchpoint says it. ASan's frames are larger, so a sanitized build writes further for one `n`.

### 2.3 Where the recursion writes (this Mac; the critic's item 8 and critic § 2.2, § 3.1)

- **Placement**: `CO_SAY=1 CO_N=1`, 30 runs of each of the repair's `malloc-O2` and `malloc-O0`: **60 of 60 with the
  stack's `lo` 0xC000 above `co_tls_probe`** (`ground/f9f42fcc4/where-60.txt`); the critic's stage 2 builds: 58 of 60, and
  2 with the stack at a 4 MiB boundary 4 MiB above the probe. The clerk's eleven readings at `a246032bf`:
  `ground/addresses.txt`. **Under `--sanitize` the stack is in ASan's heap and the probe far from it** (critic § 2.1): the
  sanitized builds are another layout.
- **The thread-local block, measured** (`otool -l rs-f9f/malloc-O2 | grep -A5 "sectname __thread"`, `nm -m … | grep
  __thread_`; `ground/f9f42fcc4/tlv-malloc-O2.txt`): `__thread_data` 8 bytes and `__thread_bss` 0xa5b0, **42,424
  bytes**, and `__thread_vars` 0x3c0, **40** descriptors; dyld takes the block from `malloc` (an inference from where it
  lies, a `Malloc Small` region of `DefaultMallocZone`, `ground/vmmap-malloc-O2.txt`). By each word's offset in the
  template, below `lo` in the common placement:

| bytes below `lo` | word |
|---|---|
| 0 to 6,728 | the 48 KiB slot's room above the block's 42,424 bytes |
| 6,728 to 39,504 | `hero_run_words`, the first word the recursion reaches, then `hero_run_variables` from 47,696 |
| 48,872 | **`hero_signal_stack`**, the thread's whole signal record, the trampoline's `own` (§ 2.4) |
| 48,880 | `hero_stack_telling` |
| 48,928 / 48,936 | `hero_stack_alt_size`, `hero_stack_alt` |
| 48,944 | **`hero_callback_mine`, a pointer**: the next callback's `hero_callback_in()` writes through what the recursion left there (critic § 3.1) |
| 48,952 / 48,960 | **`hero_stack_hi`, `hero_stack_lo`**, what the guard's handler compares a fault with |
| 49,111 | **`hero_thread_is_home`** (the clerk's lldb reading, `ground/lldb2-malloc-O2.txt`, and the offset 0x29) |
| 49,120 | `hero_alloc_inside`, guard D's word |
| 49,152 | `co_tls_probe`, offset 0 |

- **What declares thread-locals** (`grep -rnE "^\s*(static )?(extern )?_Thread_local" runtime/ | wc -l`): **225 in 49
  files**, **43 in `runtime/parts/` and 182 in `runtime/platform/`** (the layer's words, linked into a program that binds
  the layer, in the same block). The clerk's *"43 in 13 files"* counted `runtime/parts/` alone. *"Every `_Thread_local`
  of the program's image"*: the executable's block; a C library's own (a dylib's) is another block (critic § 7).
- **Below the block**: the clerk's one `vmmap` found nothing mapped below the region (`ground/vmmap-malloc-O2.txt`), and
  the defect's own runs at `-O0` wrote megabytes and returned (§ 2.2), so **in those runs memory below was mapped and
  writable**: the allocator's region, as the draft's *"a second region mapped below"* says. One reading, one layout.
- **Linux arm64** (the repair, `maps-co.h` as `co.h`, `CO_SAY=1 CO_MAPS=1 CO_N=1`, `ground/f9f42fcc4/linux.txt`): the
  256 KiB block is **its own anonymous mapping** (`ffffb8221000-ffffb8262000`, the stack at `+0x10`), **the thread's
  TLS in another mapping above it** (`ffffb8263000-ffffb82b0000`), and **no other mapping within 8 MiB below**; the
  critic read the same shape (critic § 2.2). glibc's 16 and 64 KiB blocks come from the heap (§ 2.6).

### 2.4 The hang, and the false message

- **The hang is inside the runtime's own signal trampoline** (the repair, at `f9f42fcc4`: the `-O2` build run in the
  background, `lldb -p <pid> --batch -o "bt 8" -o "register read x0 x1 x2"`, `ground/f9f42fcc4/lldb-hang-2.txt`): frame
  0 `hero_rebind_relived`, inlined, `rebind_entries.c:500`, reading `own->left_slo[k]` in the loop `:499`, `for (int k =
  own->left_n - 1; …)`; frame 1 `hero_rebind_tramp` `:722`; frames 2 and 3 `hero_thread_guard` `thread.c:147`; then
  `h_malloc_deep`. `x0` read `0x…118`, the block's offset of `hero_signal_stack` (280); **a first attach found an
  `EXC_BAD_ACCESS` (code 2) pending at an address inside that loop** (`lldb-hang-1.txt`): the trampoline faults on the
  record the recursion overwrote. **Which signal the trampoline was called for, still unread**: `x2` read 10 (Darwin's
  `SIGBUS`) at an inlined frame whose registers the compiler may reuse, and the `-O2` build has no variables
  (`-gline-tables-only`); the clerk's `sample` read the same frames (`ground/sample-malloc-O2.txt`).
- **The false message's cause, read**: `hero_thread_guard` (`thread.c:145-148`) refuses wherever `hero_thread_is_home`
  reads false, and the recursion wrote it, 49,111 bytes below `lo`.

### 2.5 A stack in the image's own data (the critic's shape A, item 3)

`static.hero`: on this Mac **hung, exit 124 at 20 s, each level** (the repair's `run.sh`; the critic: 8 of 8, and at `-O2`
with `CO_N` 9,600 to 12,000 the right answer printed, then a hang); in Linux arm64 **exit 139, nothing said, 4 runs of 4
at each level** (the repair). **What lies below the array** (`nm -n rs-f9f/static-O2`, `ground/f9f42fcc4/nm-n-static-O2.txt`,
bytes below `co_static_stack`): `hero_run_sentinel_pid` 88, `hero_run_live` 4,188, `hero_run_guard` 4,192, the saved
previous handlers `hero_lease_prev_ill` / `_abrt` / `_trap` 4,216 / 4,232 / 4,248, **`hero_stack_prev_bus` 4,272 and
`hero_stack_prev_segv` 4,288, the handlers the guard chains to**, `hero_lend_slabs` 4,304, `hero_frame_held` 4,312,
`hero_kept_key` 4,328, `hero_callback_counts` 6,464, and **`hero_signal_stopping` 220,528**, the stop's latch: written
nonzero, every later stop waits in `pause()` with every signal held (`sigguard.c:146-160`). No route row named a stack in
the image's data until this repair.

### 2.6 Neighbouring stacks (the critic's shape B, item 4; `adj.c`, the repair on both platforms)

`clang -O0 adj.c && ./adj` (`ground/f9f42fcc4/adj-mac.txt`, `linux.txt`): two of each, the second's end against the
first's start.

| | 16 KiB `malloc` | 64 KiB | 256 KiB | 1 MiB | 8 MiB | 256 KiB `mmap` |
|---|---|---|---|---|---|---|
| this Mac | 4 MiB apart | **second directly above** | **directly above** | **directly above** | **directly above** | **directly above** |
| Linux arm64 | second above (heap, 16 bytes of header) | second above (heap) | **second directly below**, 4,096 bytes between | **below**, 4,096 | **below**, 4,096 | **below, 0 bytes** |

So **on this Mac a recursion on the later stack writes the top of the earlier, where a suspended coroutine's frames
lie, and on Linux a recursion on the earlier writes the later**; and **`bare.hero`'s told end and Linux's told
`malloc.hero` are each an accident of being the last mapping made**: one more mapping after the stack (a second
coroutine, a thread, a library) puts writable memory under it (critic § 4 B).

### 2.7 What the briefs carry and nobody re-ran (no seat takes these as premises)

- The draft's *20 of 20*, *4.1 MiB under the block*, *a second region mapped below*, Linux *3 of 3*, and the base before
  step 199i *hung 3 of 3*: carried.
- The real token count, **13,849**, is a pin read (`selfhost/measure/pinned.hero:55-64`, taken 2026-11-26), not a count
  of today; the headroom **2,475** is the instrument's, run by the critic's stage 2 (`./heroes-next measure
  spec/heroes-spec.md`, critic § 7: the FFI floor's 60, `selfhost/measure/judged.hero:86-87`, against 16,384); the
  repair's seed's compiler gives no real row (`STALE`).
- **Every precedent named in any brief is recalled** until the historian's report marks it `verified`.
- **OpenSSL's default ASYNC fibre, *"32 KiB … `OPENSSL_malloc`"***, is panel 259's reading of Homebrew's libcrypto
  **3.5.9**; this Mac's `pkg-config --modversion libcrypto` is **4.1.0**, whose `nm -u` imports `_mmap` and `_mprotect`
  beside `_makecontext`, `_getcontext` and `_setcontext`: whether 4.1.0's default fibre is a `malloc`'d block, or guarded,
  is unrun (the critic's item 11). The image holds 3.5.7.
- glibc's dynamic `mmap` threshold after a `free`: unread.

## 3. The ground on `main` (line counts `wc -l` at `f9f42fcc4`, unchanged from `a246032bf` but `spec_refusals.hero`; every pointer re-read before you cite it)

### 3.1 The stack guard, and step 199i's arm

`runtime/parts/stack.c`, **1,546** lines. One `SIGSEGV`/`SIGBUS` handler per process, `SA_SIGINFO | SA_ONSTACK`
(`hero_stack_guard_install`, `:1377`), saving and chaining what was installed before (panel 104's resolution, item 3);
an alternate stack per claimed thread, 256 KiB `mmap`ed with a `PROT_NONE` page below (`hero_stack_guard_enter`,
`:1324`, the size at `:1335`); the thread's bounds in two thread-locals (`:467-468`, **48,952 and 48,960 bytes below a
coroutine's `lo` in § 2.3's placement**); the window `HERO_STACK_WINDOW`, 1 MiB (`:517`). The two witnesses
(`hero_stack_ran_out`, `:1018`). **Step 199i** (`94c2266a4`, defect 2870): where the thread's own stack does not explain a
fault, the known range nearest above it is asked the same witnesses (`hero_stack_coroutine_ran_out`, `:1127`), and past
the window, the frame chain climbing into the range (`hero_stack_climbs`, `:1113`, at most `HERO_STACK_CLIMB` 2^22
records, `:1108`); the handler `:1145`. Its note (`:1061` on) says what it does not reach, *"what lay below a stack with
no page of no access is overwritten before the fault … which only C's choice of a guarded stack prevents"*; step 199j's
notes (`6e43e7977`) corrected its claim to *read from the code, the word's address unread*. **Only a fault reaches this
handler**: it can tell a stack ran out, never stop the writes before. Its four goldens,
`tests/golden/run/fixedbugs-2870-*.hero`, are all on the guarded `mmap` header; **none on a `malloc` stack.**

### 3.2 What the runtime knows of a coroutine's stack today

- **Its range, `[ss_sp, ss_sp + ss_size)`, read from the context `makecontext` is handed**: on Linux by the wrapper in
  `runtime/parts/sigjump_make.c` (135 lines: `hero_sig_made_context` `:44`; assembly on aarch64 and x86-64, a C wrapper
  `:120` elsewhere), on Darwin by the rewritten pointer (`hero_rebind_made_context`, `rebind_jump.c:403`, the assembly
  `:418` and `:433`). Kept in a ring of **256** slots a platform (`HERO_SIG_STACKS`, `sigentries.c:94`;
  `HERO_REBIND_STACKS`, `rebind_entries.c:31`), a slot cleared when a later range overlaps it and reused when the ring
  wraps (`hero_sig_known_stack`, `sigentries.c:186`; `hero_rebind_known_stack`, `rebind_entries.c:111`), with the
  context's address, its `uc_link` and a life (`sigentries.c:104-170`). The switches are wrapped: Linux `setcontext` and
  `swapcontext` (`sigjump_save.c:260`, `:289`), Darwin `hero_rebind_swapcontext` (`rebind_jump.c:241`). Step 199i's
  question of a fault: `hero_sig_range_above` (`sigentries.c:325`), `hero_rebind_range_above` (`rebind_entries.c:229`).
- **Whether the byte below `lo` is readable, the runtime can already ask** (corrected by the critic's item 5): it reads
  memory through the kernel, failing rather than faulting where nothing readable is mapped, by `mach_vm_read_overwrite`
  (`rebind_jump.c:78`, `rebind_record.c:79`) and `process_vm_readv` (`sigjump_record.c:116`, `:159`), today to read a
  context *"wherever it is mapped"* (`grep -n "mach_vm_read_overwrite\|process_vm_readv" runtime/parts/*.c`). Those
  answer *readable or not*, never *protected or unmapped*, and both are *of no access* to the judgement routes (f) and (g)
  need. **No code asks it of the page below a stack** (`grep -rn "mincore\|mach_vm_region\|vm_region\|/proc/self/maps\|
  MAP_FIXED_NOREPLACE\|msync" runtime/parts/*.c runtime/runtime.c`: nothing). Who owns the memory and when C frees it:
  no wrapper of `free` (a question; `mprotect` stands in four files, `grep -rln mprotect runtime/parts/*.c`).
- **A stack no wrapped `makecontext` named**, which the files list themselves: Darwin's *"`makecontext` so held on this
  Mac: a coroutine's stack unseen, 7 printed as 11, defect d-lane-dlsym-make-1"* and *an installer or a jump reached
  inside the shared cache* (`rebind.c:109-121`); Linux's *a switch or a jump this file never sees (… hand-written
  assembly, a C++ exception or `_Unwind_*`)* (`sigjump.c:68-81`); a library's own assembly switch (boost.context on this
  disk, `/opt/homebrew/lib/libboost_context.dylib`, its three `fcontext` symbols C++-mangled, `nm -gU`, branching to its
  own assembly, critic § 6). **Windows**: `grep -rln "CreateFiber\|SwitchToFiber\|ConvertThreadToFiber" runtime/
  selfhost/` finds nothing, and no route of the signal guard stands there (`sigguard.c:215-221`, *"none on Windows"*).

### 3.3 The thunk, and the guard at every entry of a function C can be handed

The seed's emission of `malloc.hero` (`heroes build … --emit-c`, `ground/malloc-emitted-seed.c`, 371 lines, at
`a246032bf`, whose `selfhost/` is `f9f42fcc4`'s): **`deep`'s own entry calls `hero_thread_guard("malloc.deep")` at every
call** (`:244`); the thunk C is handed calls `hero_callback_entered("malloc.deep", __builtin_return_address(0),
__builtin_frame_address(0))` as its first line (`callback_thunk.hero:137`), the function, then `hero_callback_left()`
(`:257-259` of the emission). **The thunk's question runs on C's stack**: `hero_callback_entered` →
`hero_callback_asked` → `hero_sig_inside` / `hero_rebind_inside`, and on a refusal `hero_signal_stop`
(`sigguard.c:912-990`). The set of guarded functions is `selfhost/emit/callback_guard.hero` (207 lines): *every
function whose address is taken, in a program that declares an `extern` taking a callback* (`callbacks_of` `:78`,
`addresses_taken` `:95`; its `Callbacks` is *"ONE computation with every consumer"*, `:61`); **a function the callback
calls whose address is not taken has no guard** (read; the compiler-engineer confirms by an emission). **No set of the
functions reachable from a callback exists on `main`** (`grep -rn -i "reachable from\|reaches_callback\|callback_reach"
selfhost/emit/*.hero selfhost/ir/*.hero`: four hits, about types, a declaration and instances), **but the emitter already
walks a call graph from a frontier**: `emit/members.hero:137-216` with `emit/worklist.hero` (critic § 5), which (c2)'s
set and R2's would build on. The stop: `hero_thread_refuse` `thread.c:132-143`, `hero_signal_stop` `sigguard.c:146`.

### 3.4 What the spec says (read at `f9f42fcc4`, 526 lines, 36,316 bytes)

- **§ 9, `spec/heroes-spec.md:340-341`**: *"A source nested up to 2,000 deep compiles on every platform, and none is
  refused for its depth; running out of stack aborts."* **Its ratification** (the critic's item 10): panel 232's R4,
  *"§ 9, K1: running out of stack aborts. It lands with defect 2253 repaired on Darwin and on Linux"*, whose reasoning
  names this class, *"a frame past the guard may write rather than die"* (`docs/panel/232-*.md:244-251`, K1 at
  `:171-174`), landed at `641b43017` (`git log --oneline -S"running out of stack aborts" -- spec/heroes-spec.md`). False
  on a `malloc`'d coroutine stack (§ 2), as panel 262's table already said of `so` (*"(R1 is silent; § 9's "aborts" is
  the false sentence)"*, `docs/panel/262-*.md:223`).
- **§ 13, `:438`**, panel 259's: *"Where C leaves a callback's frames without their returning (by a jump, an exception
  or a coroutine it never resumes, say), a value they held of more than 4,096 bytes, or holding a `partial` record, and
  a buffer they lent to C stay held until the program ends, counted with the live ones against 1 GiB for the whole
  program, past which it aborts …"*; and panel 257's handler sentence beside it, which **panel 262's R1 replaces once
  lane land262 lands it** (ratified 2026-11-27 at about 08:25; `docs/panel/262-reports/synthesis/spec-adopted.txt`,
  ending *"and a callback C calls on a thread of its own aborts, saying it ran on a thread this program did not start"*:
  **the words this defect prints on the program's own thread**). `lane-land262` stands at `f9f42fcc4`, nothing of its own
  (`git log --oneline main..lane-land262`: empty, 11:03).
- **Nothing about a callback on a coroutine's stack running out**: `grep -c "makecontext\|coroutine"
  spec/heroes-spec.md`: 1, line 438.

### 3.5 Panel 104's refusal, and what it measured

`docs/panel/104-deep-recursion-stops-with-a-word.md` (ratified 2026-09-03): **(a)** the guard-page handler landed; **(b)**,
*"a depth counter in every emitted function's prologue … one increment and one compare per call"*, was refused by both
seats: per 10^8 calls on this Mac, `-O0` 0.50 s plain against **0.81 s (+55–60%)**, and at `-O2` 0.000 s plain against
**0.81 s**, *"the counter forbids the recursion-to-loop transformation"*; and on bytes, *"one `MAX` is wrong at Windows'
64 MB and for large frames"*. *"(b) returns only as a frame-address probe on a platform whose VEH cannot name the frame,
never as a depth counter."* **Read, not re-run**: 104 never measured a stack-pointer compare, and its speed ground yields
to robustness since the author's amendment of 2026-11-22.

### 3.6 Lane unwinder, in flight: panel 258's R2

Ratified this morning (the answer *3a*, about 08:25; `.claude/worktrees/rebel-scratch/coord/unwinder-brief.md`): where a
program declares an `extern` with a function-typed parameter, **each counted slot of a function reachable from the
function handed over** is `__attribute__((cleanup))`, the unit `-fexceptions`, and the jump hook runs
`_Unwind_ForcedUnwind` to the save point's frame, collecting owned values onto a per-thread list released when the extern
call holding the catch returns; where unwind information is missing, today's behaviour; no change to a program with no
such extern (`docs/panel/258-*.md:48-56`). `lane-unwinder` at `9f5c59e6b`, **nothing beyond `main` at 11:03**; its
container `unw` up. **Never read, build or run in its worktree or scratch.** Route (c) needs the set R2 builds (§ 3.3),
and route (b) moves the frames R2's unwinder walks (§ 4).

### 3.7 The signal guard of steps 199a to 199o, on `main`

`git log --oneline main | grep -E "step 199[a-z]"`: 199a `65da4688c`, 199b `3e02951e0`, 199c `590ab2503`, 199d
`77fade16c`, 199e `01706218e`, 199f `8d8263f15`, 199g `d783018e7`, 199h `a0088cee2`, 199i `94c2266a4`, 199j `4ca0c34f3`
and `6e43e7977`, 199k `27f2a23c9`, 199l `b68ea0636`, 199m `a9c52cd02`, 199n `b72a726ac`, 199o `c67ce8972`; **lane
sig-family's last steps landed at `d8ab3094e`** and the branch is gone (`git rev-parse --verify -q lane-sig-family`:
none). The thunk's walk is bounded by the coroutine range a frame lies on (199f and defect 2852,
`sigguard.c:955-970`), a jump off a coroutine's stack (199m), the spans of handlers a jump left (199d, 199h), the shadow
cleared on a range's stack under ASan (199e, 199k, 199l, 199o). Thirteen of the sixteen subjects name a coroutine, a
fibre, a stack, a frame, a context or a switch (`grep -ciE` of each; 199g's, 199n's and 199j's notes do not): which
rules route (b) breaks is a question the seats run.

### 3.8 Instruments (each asked by the clerk, re-asked by the critic)

- **This Mac**: `/usr/bin/time -l` (instructions retired), `sample`, `vmmap`, `lldb`, `otool`, `nm`. Panel 259's cost
  instrument, `cost.sh` with `cost.h` and `cost_cb.hero`, in `docs/panel/259-reports/evidence.tar.xz` under
  `docs/panel/259-briefs/shapes/cost/`: `tar -xJf` into your scratch, never the tree.
- **Linux arm64**: `heroes-linux-arm64:latest`, `338a1044ac90`, **no `/usr/bin/time`, `perf`, `valgrind` or `gdb`;
  `lldb`, Debian clang 22.1.8, glibc 2.41, OpenSSL 3.5.7, no `/usr/include/boost/context`**. A cost there is timed by the
  program itself (`clock_gettime`) or by the shell, alternated and repeated, the load named.
- **Split stacks are not on offer**: both clangs accept `-fsplit-stack` and emit **0** `__morestack` calls on arm64 (the
  clerk at `-O0` default, the critic again).

### 3.9 The secondary question: step 199j's stop (issue 2871, `4ca0c34f3`; the critic's item 7, critic § 9)

The coordinator added it at about 10:45; the repair read it at `f9f42fcc4`. **The stop is
`hero_signal_switch_begin`, `runtime/parts/sigguard.c:600-622`**, which calls `hero_signal_stop` directly: **no arm of
`hero_signal_refuse`**, so `tests/harness/spec_refusals.hero`, which reads that function's arms, holds none of its words,
and **no callback in it**. Each delivery marks the contexts of the switch it interrupted (`hero_signal_in_switch`,
`sigguard.c:671`, called from Darwin's trampoline `rebind_entries.c:723` and Linux's `siginstall.c:169`), and the switch
wrappers (`rebind_jump.c:154`, `sigjump_save.c:249`) stop a handler's `swapcontext` or `setcontext` naming one. Its
words open *"panic: a signal handler called `<call>` naming a context that the code the signal interrupted was itself
switching from or to, inside a `swapcontext` or `setcontext` of this thread that had not yet loaded the context it
switches to"* (`sed -n 600,622p runtime/parts/sigguard.c`). **Its cases**: `tests/golden/run/fixedbugs-2871-*`, **7**
(`ls tests/golden/run | grep -E "\.hero$" | grep -c fixedbugs-2871`), four refused and three that run, with the header
`fixedbugs-2871-a-handler-switching-contexts-inside-a-switch.h`. Its commit body: *"C alone dies too"*, and *"The switch
family beside, ten sets: 166 passed 0 failed here; Linux arm64 162 and 4, the four red on the base too (filed nowhere
yet, in the lane's report)"*. **The four are named nowhere the repair could find**: the commit names none, issue 2871 is
open (`gh issue view 2871 --json title,state`), lane sig-family-2's scratch is gone, and `grep -rn "162 and 4\|four red"`
in `.claude/worktrees/rebel-scratch/` finds nothing: the seats name them by running (their briefs). **It is to be read
against** § 13's handler sentence (panel 257's, `:438`) and panel 262's adopted R1, with panel 258's stop sentence there
(*"Where C jumps out of a signal handler whose signal interrupted this runtime's allocator … the program aborts"*) as the
shape a sentence would take. **And the two questions meet**: the stop reads the same thread record the recursion
overwrites (`own`, `hero_signal_stack`, 48,872 bytes below `lo`, § 2.3; the trampoline's `rebind_entries.c:722-723`,
the hang's own lines, § 2.4), so under this defect it may fire in its words falsely, or loop: a shape to run. **PARKED,
the arm step 199m added, is panel 262's R1's first sentence's, not this sitting's** (the coordinator's note of 10:38,
`.claude/worktrees/rebel-scratch/coord/agents-live.md`, *"land262: PARKED (199m) claimed by R1's first sentence"*).
Panel 258's stop sentence inside the adopted text, cut and counted offline: 272 bytes, claude-legacy 61, cl100k_base 60
(`ground/parts/s13-stop-258-in-262.txt`, `heroes measure`).

## 4. The routes (the three the draft named, the clerk's, and the critic's, each a question for the seats)

*Reaches* is a reading until a seat runs it. **Every row is held to § 5's shapes, a stack in the image's data (shape A)
among them.**

| | route | reaches | what it leaves, read | to R2's unwind | to the signal guard | seat |
|---|---|---|---|---|---|---|
| **(a)** | **a guard page the runtime places on that stack**: at `makecontext`'s wrapper, the lowest whole page(s) inside `[lo, hi)` made `PROT_NONE`. (a1) kept from `makecontext` on; **(a2) placed at a callback's first entry onto the range, taken off when the last returns**; (a3) its size, a page or a window an unprobed C frame cannot jump | stacks the runtime saw made, a static array's included (no `free` there) | **a protection inside memory C owns, outliving the runtime's knowledge of its use**: the guard at the block's lowest addresses, where an allocator may write when C frees it (an inference, unread), a pool reusing it, `realloc`, `read` into it (`EFAULT`); under (a2) a jump out or a yield inside the callback leaves it standing; a page not wholly inside the block cannot be taken; a copy-stack library's shared block (§ 5 C) keeps it through every copy | none | 199i's arm asks the window below `lo`: with a guard inside, `lo` moves up by it | ffi-pragmatist; compiler-engineer prices (a2)'s thunk |
| **(a4)** | **(a) with `free`, `realloc` and `munmap` wrapped**, the guard taken off before the allocator writes there (the critic's § 8.1) | (a)'s | answers (a)'s allocator path; leaves a pool allocator, `read` into the block, and `free` called inside the shared cache (`rebind.c:109-121`), unseen by the rewrite | none | none beyond (a)'s | ffi-pragmatist |
| **(b)** | **callbacks C calls run on a stack the runtime owns**, `mmap`ed with its own guard: the thunk switches, runs the function and every C call it makes there, and switches back | **every stack a callback is entered on, but for the thunk's own frames** (corrected by the critic's item 6): `hero_callback_entered`'s question and a refusal's stop run on C's stack before any switch (`sigguard.c:912-990`, `callback_thunk.hero:137`), so the switch must come first, or the question's frames stay on C's stack; and a nested entry must know it is already on its own stack | a runtime stack held for as long as a callback on it is suspended or abandoned (a yield inside the callback, a coroutine never resumed, a jump out the hook does not see): a pool, its bound, and what § 13's held-memory sentence says of it (§ 7); the switch on two architectures × two systems; stack-passed arguments of a callback with more than eight | **the unwinder must cross the switch**: without CFI naming the caller's frame on the other stack `_Unwind_ForcedUnwind` stops there, and R2 degrades for every callback | **every rule that asks which stack a frame is on** (§ 3.7) meets a stack it has never seen | both, each from its side |
| **(c)** | **a stack check at each call of the functions a callback reaches**: the stack pointer compared with a per-thread limit, the range's `lo` plus a reserve, kept by the switch wrappers. (c1) the address-taken set alone (misses a helper's recursion); **(c2) every function reachable from a callback**, R2's set (`emit/members.hero`'s walk, § 3.3); (c3) every function of a program with a callback `extern` | Heroes frames, on stacks the runtime saw made | **C frames the callback calls run below the last check unguarded** (§ 5 D); an unseen switch leaves the limit stale (the slow path re-derives from the ranges); the limit word lives in the thread-local block (§ 2.3); per call cost, and whether it blocks `-O2`'s transformations as 104's counter did | a cold call in each prologue | the limit follows the switches the wrappers decode | compiler-engineer |
| (d) | **`sigaltstack`** | none | already in place per claimed thread (§ 3.1): it decides where a handler runs, never where the first fault comes; **the clerk found no arrangement in which it stops the writes**: a question, name one or rule it out | none | none | both, by reading |
| (e) | **a probe at the callback's entry** against the space left on the range learned from `uc_stack`: refuse, or switch (= (g)), below a reserve | entries near the bottom | a recursion after the entry; alone it does not close the defect; a recursion through C passes every probe (§ 5 D) | none | none | compiler-engineer |
| **(n)** | **a static bound at the thunk**: (e)'s probe compared with the maximum stack the callback's call tree can use, computed at compile time (clang `-fstack-usage`, the call graph of § 3.3), a recursion in the tree checked by (c) or refused, an indirect or C call making the bound unknown (the critic's § 8.2) | callbacks whose tree is bounded | an unbounded tree falls to (c) or another route | none | none | compiler-engineer |
| (f) | **refusing to call back on a stack the runtime cannot bound**: at the thunk, a frame on a range whose byte below is readable (§ 3.2's probes), or on no known stack, is refused, told | every callback on such a stack | **a new abort of correct programs whose callbacks never recurse**: a sentence of the spec | none | a new arm of the thunk's question | ffi-pragmatist counts what it refuses |
| (g) | **(b) only where (f) would refuse** | (f)'s set | (b)'s costs on (f)'s set; *can bound* a judgement made by § 3.2's probe | (b)'s, on that set | (b)'s, on that set | ffi-pragmatist |
| (h) | **the stack swapped at `makecontext`**: the wrapper hands the C library a guarded stack of the runtime's own of the same size and restores `uc_stack` after | stacks made through the wrapper | when to unmap it (C frees its block unseen): a runtime stack held per context, as (b)'s (§ 7); **C that reads its own block** (a collector scanning it, a library measuring use by a pattern, ASan's fiber annotations, a library comparing `sp` with its block, **a copy-stack library copying its own block**, § 5 C) | none | ranges name the runtime's stack, not C's | ffi-pragmatist |
| **(o)** | **the runtime's guarded allocator handed through a library's own door**: `ASYNC_set_mem_functions` (OpenSSL), a boost `stack_allocator`, a library's coroutine attributes (the critic's § 8.3) | libraries with such a door | no protection inside C's memory and no swapped stack; reaches no library without a door | none | none | ffi-pragmatist |
| (i) | **a guard below the block where the page below is free** (`MAP_FIXED_NOREPLACE`; Darwin's `mach_vm_allocate` without `VM_FLAGS_ANYWHERE`) | blocks at a region's or a mapping's bottom | this Mac's common placement has the thread-local block there (§ 2.3), but 2 of the critic's 60 runs placed the stack at a region's first byte, and Linux's 256 KiB block is its own mapping with nothing below it (§ 2.3): (i)'s layout exactly, until a later mapping lands there (§ 2.6) | none | none | ffi-pragmatist, runs on both |
| **(p)** | **a hardware write watchpoint** on the bytes below `lo` (Linux `perf_event_open` breakpoint; no user API on Darwin, as recalled; 8 bytes a register on arm64) (the critic's § 8.4) | as many words as registers | to be ruled out by name, with the run or the page that rules it out | none | none | compiler-engineer, by reading |
| (j) | **split stacks** (`-fsplit-stack`) | none | measured unavailable on both arm64 platforms (§ 3.8) | none | none | none |
| (k) | **a depth counter** (panel 104's (b)) | Heroes frames | a count is not a size (104's own ground) | none | none | compiler-engineer, one line |
| (l) | **the runtime's per-thread state out of the program's reach**: its words in a mapping of its own with a guard page, reached through the thread key, not C11 `_Thread_local` | the false message, the hang, the guard's own bounds | **the writes into C's memory stay**: honesty of the message (design.md §4.17), never §1.12 alone | none | the record `own` moves | compiler-engineer |
| **(q)** | **(l) and the process-wide words too**: shape A writes `hero_stack_prev_segv`, the handler the guard chains to, and `hero_signal_stopping` (§ 2.5) (the critic's § 8.5) | shape A's victims | as (l): no route alone | none | the chained handlers move | compiler-engineer |
| (m) | **a canary below the range, read when the callback returns or switches away**, told | known ranges | tells the corruption after it, never prevents it | none | none | compiler-engineer, by reading |

**Other routes the seats find are owed in their reports**, with what would have to be true for a route nobody listed to
exist (CLAUDE.md § RUN IT). **cgo, as recalled, is (b) and (c) together** (the critic's item 14): the callback's Go code
on a stack the runtime owns, and a check at each function's entry that grows it; *"the route cgo takes"* is a question
about both rows, which the historian verifies.

## 5. The shapes beside the defect, each to be run where a route claims it (CLAUDE.md § RUN IT)

1. **Stack sizes**: 16 KiB, 64 KiB, 256 KiB, 1 MiB, 8 MiB from `malloc`, both platforms: § 2.6's placements say where
   each lands; on Linux the 16 and 64 KiB blocks come from the heap, so they are this Mac's shape there (unrun as
   stacks). OpenSSL's default ASYNC fibre is one small stack (§ 2.7; `ffi-pragmatist.md`).
2. **The canary** at every `CO_N` of `run.sh`, plain and `--sanitize`, both platforms: a route leaves 0 bytes written.
3. **`malloc.hero` sized by `CO_N`** (§ 2.2): the right answer at exit 0 is the defect, not its absence.
4. **Neighbouring stacks, by direction** (§ 2.6): on this Mac the later above the earlier, on Linux the later below (256
   KiB and up, raw `mmap`); a recursion on one into the other's suspended frames, resumed; **and `mmap` with no guard
   with a mapping made after it**, which ends `bare.hero`'s told end.
5. **Shape A, a stack in the image's data** (§ 2.5), both platforms: the process-wide words below it.
6. **C, a stack shared by two coroutines** (a copy-stack library: greenlet, libco's shared stack, as recalled): read,
   unrun (critic § 4 C).
7. **D, a recursion through C**: Heroes → C → Heroes … unbounded (a tree walker in C calling the callback that calls the
   walker): every level passes (e)'s probe, and under (c) the C frames between are unchecked (critic § 4 D).
8. **E, a Heroes heap value below the block**: an array allocated before the stack, read after the callback returns at
   exit 0: a wrong answer at exit 0; and C `free`ing a neighbour after such a run (critic § 4 E). Read, unrun.
9. **A mutual recursion, frames larger than a page, a spawned thread** (defect 2870's four shapes, on `malloc`).
10. **A callback that yields** (`swapcontext` away inside it, resumed later, and never resumed); **a jump out of the
    callback** (`longjmp` to a save on C's stack, then another callback: R2's door).
11. **A stack the runtime never saw**: a `makecontext` held by an image's static initializer, a switch by hand-written
    assembly or boost.context.
12. **A stack carved from the thread's own** (panel 262's `mj_carved`) and one `mmap`ed with a guard page: no route may
    move what they do today (`guarded.hero`: `panic: stack exhausted`, § 2.1; the carved one unrun by the clerk).
13. **199j's stop under this defect** (§ 3.9): a handler switching contexts after a recursion wrote the record `own`;
    and **a correct program the stop might refuse** (a handler switching to a context the interrupted switch has already
    finished saving into, say).
14. **Windows**: what each route would be there (fibres, `CreateFiber`'s stacks, a hand switch onto `malloc`), by
    reading, **or by a run of minutes on the box** (§ 8).

## 6. What the landing owes (for the synthesis; the seats say what they would add)

- `run` goldens under `tests/golden/run/`, a `fixedbugs-<n>-*` per shape the route claims, `<n>` the defect's issue
  number once filed, **annotated in their sources** (CLAUDE.md § 9), their emissions blessed on this Mac; among them
  `malloc.hero` sized to return (§ 2.2) and the canary, each a program that must end told, never at exit 0 with bytes
  below written, **and judged by the count, never by `--sanitize`'s silence**.
- **The emitted C of a program with no callback `extern` byte-identical**, the `hero_program_sources` line aside (the
  compiler's own, every example), measured over the `run` goldens and `examples/`; the cost per callback and per call
  measured before and after, best of N, the first discarded, the load named (CLAUDE.md § Verification, *never slow the
  compiler down*, at rank 5 under robustness's 3).
- `stack.c`'s note of step 199i (`:1061` on) and `thread.c`'s guard rewritten to say what the route now reaches.
- Both arm64 platforms at the gate; Windows as the seats find it (§ 8); the CI's legs after the push.
- Lane unwinder's R2 met where it stands at the landing: whichever lands second re-runs the other's cases.
- Any sentence of the spec the route or 199j's stop needs (§ 7) priced by the spec-warden and read by the blind seat
  where the author authorises its run.

## 7. Does a sentence of the spec move? The clerk's reading, corrected by the critic (critic § 10)

**The route moves none only if it makes § 9's *"running out of stack aborts"* true on every stack a callback runs on,
adds no abort and holds nothing the spec does not already name.** Read, route by route: **(f)** adds an abort of correct
programs (a sentence of § 13); **(a), (a4), (c), (e) and (n)** reach only stacks the runtime saw made, so § 9's sentence
stays false on a stack no wrapped `makecontext` named **and must say what happens on a stack in the image's own data
(shape A) as well**, or the route must reach them; **(b)** and **(h)** hold a runtime stack for each callback suspended
or abandoned (b), or for each context `makecontext` named until C frees a block the runtime does not see (h), a resource
panel 259's held-memory sentence does not name (*"a value they held of more than 4,096 bytes, or holding a `partial`
record, and a buffer they lent to C"*); **(g)** carries (b)'s on (f)'s set. Not needed, as far as reading goes: a
sentence for (l), (q) or (m) alone, which change no promise. **And step 199j's stop (§ 3.9) may move § 13 whatever route
the main question adopts**: a stop no sentence states, beside panel 258's that one does; so it alone may register the
blind reading. **Panel 262's ratified thread clause** (§ 3.4) is the false message's own words, and its landing (lane
land262) meets this defect's program. Each seat says in its report whether the route it supports, and 199j's stop,
need a sentence of `spec/heroes-spec.md`.

## 8. How a seat works

- **Your copy, one seat's**: `git worktree add --detach .claude/worktrees/263-<seat> f9f42fcc4`, run from
  `/Users/joseph/Temp/heroes/heroes-lang`, and your scratch `.claude/worktrees/263-<seat>-scratch/` (`TMPDIR` there).
  Build there: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (the repair's took `real 23.35` under load),
  then stage 2, `./heroes build selfhost/main.hero -o heroes-next` (the critic's took `real 311.81` beside a gate, a
  duration nobody should quote; `sources 50e8403d5a885325`). `./heroes --version` reads `sources 1e1eb23a1927d35b`; say
  if yours does not. A runtime you edit is a copy of `runtime/` in your scratch under `HEROES_RUNTIME`, never your copy's
  edited in place, so your column *before* stays the base's.
- **Never read, build or run** inside another seat's copy or scratch, the critic's, or **any lane's worktree or scratch:
  `lane-sig-family-3`, `lane-win-paths`, `lane-win-paths-h`, `lane-unwinder`, `lane-land262`, `lane-site-rebel`**, nor
  **`gate-push10`** (a gate runs there); `lane-sig-family` is gone (`git worktree list`, `git branch --list 'lane-*'`,
  11:03). Never the trunk's compiler, another seat's, or a lane's.
- **Linux arm64**: `docker run --rm --pull never --name p263-<seat> heroes-linux-arm64:latest`, one container at a time,
  removed when idle, no `docker pull`, your copy mounted (`-v`), every file it writes inside the repository's root.
- **The Windows box is up until about 12:50** for lane unwinder (`.claude/worktrees/rebel-scratch/coord/unwinder-brief.md`:
  *"the Windows box (`ssh win`, up until about 12:50 …)"*), so **Windows by reading is a choice, written as one**: a seat
  may change it for a run of minutes, `ssh win`, in a folder of its own under `C:/w/`, never the lane's, one run at a
  time, never turning the box off or restarting it, and saying in its report what it ran there.
- **Every run bounded** in time and bytes: a program that may not end is built and its binary run under `timeout -k 5
  <s>`, its output through `head -c`; after a timeout, look for the binary (`ps -axo pid,comm`) and stop it **by its
  id**, never `pkill`.
- **Write your report as you go** to your copy's `docs/panel/263-reports/<seat>.md`, your programs under
  `docs/panel/263-reports/programs/<seat>/`, and copy both to the same paths under `.claude/worktrees/panel263/` when you
  finish or stop. **Never commit, never edit a tracked file of any tree but your copy, no GitHub write** (`gh` reads
  only), **nothing outside the repository's root** (no `/tmp`), no download.
- **Contract**: per route, `verdict` (approve, approve with a condition, object, veto), the design.md section it rests on
  (§1.12, §1.11, §4.19, §4.17: reached by `grep -n`), `cost` (lines by file, and per callback and per call **in measured
  numbers, before and after, on both platforms**), `prediction` (one falsifiable sentence and the landing that checks
  it), `condition` (what would change your mind); whether the route, and 199j's stop, need a spec sentence (§ 7); and
  **every place this brief was false or silent**, first. Say what you ran and what you read, apart. A veto is a refusal
  on soundness, never a price.
