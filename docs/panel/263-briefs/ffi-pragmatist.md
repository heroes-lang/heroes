# Panel 263, the ffi-pragmatist's brief

**Repaired 2026-11-27 from about 11:00 by the coordinator's repair, on the critic's first reading, and re-based on
`f9f42fcc4`** (`00-shared.md`'s head). Read `00-shared.md` first, whole, then the critic's first reading
(`docs/panel/263-reports/critic-briefs.md`, beside these briefs under `.claude/worktrees/panel263/`). **Your agent file
is your copy's** (`.claude/agents/ffi-pragmatist.md`: *compile, don't opine*); design.md is reached by `grep -n` (§1.11,
§1.12, §4.19). Your veto on ABI breakage and on soundness stands; a cost is written, never a reason by itself (CLAUDE.md
§ Precedence, the amendment of 2026-11-22).

**Your copy**: `.claude/worktrees/263-ffi-pragmatist`, a detached worktree at **`f9f42fcc4`** (`00-shared.md` § 8, with
its never-touch list: `lane-sig-family-3`, `lane-win-paths`, `lane-win-paths-h`, `lane-unwinder`, `lane-land262`,
`lane-site-rebel`, `gate-push10`), scratch `.claude/worktrees/263-ffi-pragmatist-scratch/`, `TMPDIR` there; the seed's
compiler and stage 2 built there; a runtime you edit is a copy in your scratch under `HEROES_RUNTIME`. Linux arm64: the
container `p263-ffi-pragmatist`, `--rm --pull never`, one at a time. **The Windows box** is up until about 12:50 for
lane unwinder: Windows *by reading* is a choice, and a run of minutes there is yours to make (`00-shared.md` § 8). **No
paid run, no credential printed, no GitHub write, no download, no file outside the repository's root, no `pkill`, no
commit; every run under `timeout -k 5`.** Report as you go: `docs/panel/263-reports/ffi-pragmatist.md` in your copy,
programs under `docs/panel/263-reports/programs/ffi-pragmatist/`, both copied to the same paths under
`.claude/worktrees/panel263/` when you finish or stop.

## The bindings this is about (each asked by the clerk or the repair; say where one is false)

- **OpenSSL's ASYNC jobs** run a function of the program on a fibre libcrypto makes with `makecontext`, on a stack from
  `OPENSSL_malloc` unless the program gives its own through `ASYNC_set_mem_functions`
  (`tests/golden/run/fixedbugs-2853-openssl-async-fibres.h:9-15`, panel 259's ffi-pragmatist's reading: *"The library's
  default stack (32 KiB in the first build run, `stack-overflow ... in h_ajheap_e4` under --sanitize) is too small for
  these frames"*). **That reading was of Homebrew's libcrypto 3.5.9** (`docs/panel/259-reports/ffi-pragmatist.md:21`,
  `:292`); **this Mac's is 4.1.0** (`pkg-config --modversion libcrypto`, `/opt/homebrew/Cellar/openssl@4/4.1.0/lib`),
  whose `nm -u` imports **`_mmap` and `_mprotect`** beside `_makecontext`, `_getcontext` and `_setcontext`: **whether
  4.1.0's default fibre is still a `malloc`'d 32 KiB block, or a guarded mapping, is unrun** (the critic's item 11), and
  the image holds 3.5.7. `openssl@3`, `@3.3` to `@3.6` and `@4` are all in `/opt/homebrew/opt` (`ls`). The golden
  `fixedbugs-openssl-async-jobs-on-heap-fibres-run-under-the-sanitizer.hero` runs jobs on fibres from `malloc` (256 KiB:
  its `main` calls `aj_mem()`, whose `aj_alloc` returns `malloc(AJ_STACK)` while nothing is carved, the header's
  `:33-39`).
- **A library switching by its own assembly**, which no wrapper sees: boost.context on this Mac
  (`/opt/homebrew/lib/libboost_context.dylib`, its `make_fcontext`, `jump_fcontext` and `ontop_fcontext` C++-mangled,
  `nm -gU`), **not in the image** (`ls /usr/include/boost/context`: no such directory); a hand-written switch of your own
  reaches both platforms.
- **`ucontext` on `malloc`** (`reproducer/co.h`), **on a static array** (`reproducer/static-co.h`, shape A,
  `00-shared.md` § 2.5), and on a stack carved from the thread's own (panel 262's `mj_carved`,
  `tests/golden/run/fixedbugs-2852-*`).
- **Neighbouring stacks** (`reproducer/adj.c`, `00-shared.md` § 2.6): on this Mac a later 64 KiB to 8 MiB `malloc`, or
  a later 256 KiB `mmap`, lies **directly above** the earlier; on Linux a later 256 KiB to 8 MiB `malloc` lies **below**
  the earlier with a page between, a raw `mmap` **directly below**, and 16 and 64 KiB come from the heap ascending.
- **Windows**: `CreateFiber`'s stacks, and a hand switch onto `malloc` there, by reading or by a run of minutes on the
  box; the runtime knows no fibre there (`00-shared.md` § 3.2).

## What you build and run (this Mac and the Linux arm64 image; `-O0` and `-O2`; three runs each unless a rate needs more)

**`--sanitize` cannot see these writes** (`00-shared.md` § 2.2, the critic's item 2): every claim that a binding or a
route leaves **0 bytes** below a stack is made by a count (a canary below the stack, as `canary.hero` does) or a
watchpoint, plain **and** sanitized, never by a sanitized exit 0.

1. **The defect in a real binding**: an OpenSSL ASYNC job whose function is the program's and recurses, **on the
   library's default stack** (no `ASYNC_set_mem_functions`), sized to end inside it, to pass its bottom by a few KiB,
   and to run far past it, **on libcrypto 4.1.0 and on 3.5.x** here (`/opt/homebrew/opt/openssl@3`) and 3.5.7 in the
   image. Read the default stack's allocator and size from the run (an `ASYNC_set_mem_functions` that counts and passes
   through, or `vmmap` / `/proc/self/maps` around the fibre), never from memory; and how many bytes below it a recursion
   that returns wrote, by a canary you place there. Then `reproducer/run.sh <your heroes-next> <your scratch>/rs 3 30`
   (`00-shared.md` § 2) on both platforms, `CO_SAY=1` once per build, `adj.c` once on each.
2. **Route (a), built in a runtime copy, with its variants** (`00-shared.md` § 4): (a1) `makecontext`'s wrapper protects
   the lowest whole page(s) inside `[lo, hi)`; **(a2)** the thunk protects them at the first callback entry onto the
   range and takes them off when the last callback on it returns (and at a jump the hook sees); (a3) the guard's size,
   one page against the deepest frame an unprobed C library can push past it (name one, measured); **(a4)**, (a) with
   `free`, `realloc` and `munmap` wrapped, the guard taken off before the allocator writes (the critic's § 8.1: Darwin's
   rebind and Linux's symbol wrappers are the runtime's mechanism for `makecontext` today). For each: the canary (0 bytes
   at every `CO_N`, plain and sanitized?), `malloc.hero` by `CO_N`, `static.hero`, OpenSSL's default fibre, and **every
   way C's memory outlives the protection**: C `free`s the stack after its coroutine ends (does `free` fault, on which
   allocator, at which level?), reuses the block through a pool, `realloc`s it, `read`s into it, `free`s it from inside
   the shared cache (which the rewrite does not see); the coroutine yields inside the callback and C frees it; a jump out
   the hook does not see; a copy-stack library's shared block (`00-shared.md` § 5 C, read). **A route whose protection
   makes a correct program fault is a soundness veto.**
3. **Route (b) from the C ABI's side** (the compiler-engineer builds the thunk and the guard's half): a switch to a stack
   the runtime owns, guarded, for the callback's frames and every C call it makes. **The thunk's question runs on C's
   stack before any switch today** (`sigguard.c:912-990`; the critic's item 6), so say how deep those frames reach below
   the thunk's. Build the switch in C and assembly for arm64 on both systems, and run, both platforms: **a callback whose
   arguments do not all fit in registers** (more than eight integer ones, a record by value if the language lets a
   callback take one: `grep -n` spec § 13), floats, a record returned; **a callback that calls back into C that calls
   back** (nested entries on one runtime stack), and **the unbounded alternation**, Heroes → C → Heroes … (shape D);
   **a callback that yields** (`swapcontext` away inside it, resumed, and never resumed: how many runtime stacks are then
   held, and what bounds them; `00-shared.md` § 7 asks whether the spec must say); **a jump out** (`longjmp` to a save on
   C's stack); **under `--sanitize`**, ASan's fiber annotations (`__sanitizer_start_switch_fiber`) or the false reports
   without them. Cost per callback: panel 259's `cost_cb.hero` (20,000,000 calls) and libc's `qsort` of 1,000,000
   `i64` through a callback, before and after. *"The route cgo takes"* is recalled as (b) and (c) together
   (`00-shared.md` § 4): a precedent only where the historian's report marks it `verified`.
4. **(f), (g), (h), (o) and (i)**: **(f)** count what a refusal at the thunk would refuse: every `run` golden whose
   callback runs on a coroutine's stack with nothing of no access below it (`ls tests/golden/run | grep -iE
   "fibre|fiber|coroutine|openssl-async" | grep -c "\.hero$"`: 105 cases at `f9f42fcc4`, then read each one's header),
   OpenSSL's default fibre, the heap-fibre golden; and the words it would need in § 13. **(g)**, (b) where (f) would
   refuse: the judgement *nothing of no access below* made at the thunk. **The runtime can already ask whether a byte is
   readable without faulting** (corrected by the critic's item 5): `mach_vm_read_overwrite` (`rebind_jump.c:78`,
   `rebind_record.c:79`) and `process_vm_readv` (`sigjump_record.c:116`, `:159`), which read a context *"wherever it is
   mapped"* today (`grep -n "mach_vm_read_overwrite\|process_vm_readv" runtime/parts/*.c`); no code asks it of the page
   below a stack. Price that probe at a thunk, and say what it gets wrong (a mapping made after the judgement, § 2.6).
   **(h)**, the stack swapped at `makecontext`: build it, run OpenSSL's fibres and the reproducer under it, and **name a
   binding that reads its own stack block** (a collector that scans it, a library measuring its use by a pattern, ASan's
   annotations naming it, a copy-stack library copying it) with a run. **(o)**, the runtime's guarded allocator handed
   through a library's own door (the critic's § 8.3): `ASYNC_set_mem_functions` with a runtime allocator of `mmap`ed,
   guarded stacks, run with OpenSSL's fibres and a recursing job; which libraries offer such a door, and what it leaves.
   **(i)**, one run on each platform: how often the page below a `malloc`'d stack of each size of `00-shared.md` § 5
   item 1 is free (the critic: Linux's 256 KiB block had nothing mapped 8 MiB below, § 2.3), and what one later mapping
   does to it.
5. **(d), `sigaltstack`**: say whether any arrangement of it stops the first write below a stack (`00-shared.md` § 4: the
   clerk found none), by a run if you find one.
6. **The secondary question, step 199j's stop** (`00-shared.md` § 3.9; the critic's item 7): **its seven cases**,
   `heroes-next run tests/harness/main.hero -- heroes-next run fixedbugs-2871`, at both levels and under `--sanitize`, on
   this Mac and in Linux arm64; **the four switch-family cases red in Linux arm64 on the step's base** (its commit body;
   named nowhere the repair could find: name them by running `fixedbugs-27` and `fixedbugs-28` in the image, with the
   compiler-engineer's list beside yours in the synthesis); **one binding shape a correct program could be stopped by**
   (a green-thread scheduler whose `SIGALRM` handler switches to a context the interrupted switch has already finished
   saving into, say), built in C and run; and whether *"C alone dies too"* (the commit) holds for each shape you build,
   C with no Heroes code. Linux x86-64 and Windows by reading, or Windows by a run of minutes on the box.
7. **The shapes nobody named** (CLAUDE.md § RUN IT): at least one binding shape of your own beside `00-shared.md` § 5's,
   compiled and run, and the C it needs under the route you support.

## What you judge

Per route: `verdict`, `section`, `experiment` (the C you wrote and whether clang and the run accepted it, both
platforms), `argument`, `prediction` (a falsifiable claim about a named binding, stated for the libcrypto release you
ran: *OpenSSL 4.1.0's default ASYNC fibre with a recursing job ends told, writing 0 bytes below its stack by a canary's
count, under the route*), `condition`. **A route that breaks a binding the tree runs today** (a `run` golden red, a
correct program refused) **or leaves bytes written below a stack in any binding it claims is a veto**; name the binding,
platform, level, build and count. Say whether the route you support, and 199j's stop, need a sentence of the spec
(`00-shared.md` § 7), and what a veto would compel.
