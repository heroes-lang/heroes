# Panel 255, the ffi-pragmatist's report (soundness lane, defect 2679)

Written 2026-11-22 as I went, by the ffi-pragmatist seat. Every run below is in my copy,
`.claude/worktrees/p255-scratch/ffi-pragmatist/` (written `F/` below), a detached worktree at
`a75179641` (`git log -1`: `a7517964102a395ce8719ab2a30ed7dbb8d91aa7`, `git status --short | wc -l`: 0
before I wrote `F/p255/`), `TMPDIR=F/p255/tmp`, this Mac (`sw_vers`: macOS 27.2.1, build 26B131),
unless a line says otherwise. Another seat builds and runs on this Mac and in Docker at the same time:
every time here is indicative. A claim without its command says *unrun*.

## Status

Started 17:25 (`uptime`, load 4.50). Sections below are appended as each question is answered.

## Q1. Darwin's saved stack pointer decodes at a jump, by one per-process cookie and `xpacd`

**The instructions** (`dyld_info -arch arm64e -disassemble /usr/lib/system/libsystem_platform.dylib`,
`F/p255/q1/platform-arm64e.dis`, 8,631 lines; the `-arch arm64` view, `platform-arm64.dis`, is a second
image, uuid `38C97926-...` against arm64e's `FDF30FD1-...`, `dyld_info -uuid`):

- `__setjmp` (arm64e): `mov x12, sp`; `mov w9, #0xcbed`; `pacdb x12, x9`; `mrs x16, TPIDRRO_EL0`;
  `ldr x16, [x16, #0x38]`; `eor x12, x12, x16`; `str x12, [x0, #0x60]`. So **the stack pointer is word 12,
  bytes `[96,104)`, signed with the DB key and the constant discriminator `0xcbed`, then XORed with the
  thread's TSD slot 7** (the pointer-munge token). Frame pointer and return address are words 10 and 11
  (`stp x10, x11, [x0, #0x50]`), the FP signed `pacdb x10, sp`, both XORed with the same token.
- `_setjmp` saves the mask (`sigprocmask` into `+0xb0`) and the alternate-stack flag (`+0xbc`), then
  `b __setjmp`; `_sigsetjmp` stores its flag at `+0xb8` and branches to one or the other.
- `__longjmp` reads words 0 to 12 and `d8`..`d15` at `+0x70`..`+0xa8`, XORs, `autdb x12, #0xcbed`,
  **`ldr xzr, [x12]`** (a probe of the target stack), `mov sp, x12`, `autdb x10, sp`; `_longjmp` first
  does `ldr x8, [x21, #0xb0]` (8 bytes: `[176,184)`) and `sigprocmask(SIG_SETMASK, ...)`, then
  `ldur x0, [x21, #0xbc]` and `__sigunaltstack`; `_siglongjmp` reads `+0xb8` and branches.
- `getcontext` stores the SP **with no XOR**, `pacda x11, #0xcbed`, at `mcontext + 0x108` (the public
  `__ss.__sp`), FP `pacda #0x4517` at `+0xf8`, LR at `+0x100`; `__setcontext` reads them back with `autda`
  and the same `ldr xzr, [x11]` probe.

**Which code a program runs, and as which architecture.** `file heroes` (stage 2 in my copy):
`Mach-O 64-bit executable arm64`; `lipo -archs heroes`: `arm64`. A Heroes program is clang's default
target here, arm64. The probe `F/p255/q1/decode.c` reads the first instruction word of each function in
its own process through `dlsym`: `_setjmp` and `setjmp` `0xd503237f` = `pacibsp`, so **an arm64 process
runs the arm64e code of the shared cache** (only `dyld_shared_cache_arm64e*` files exist,
`ls /System/Volumes/Preboot/Cryptexes/OS/System/Library/dyld/`).

**The probe**: each save goes through a naked stub that stores `sp` to a global and branches (`b`) to the
C library's own, so the SP the save stored is the one recorded; one calibration
`C0 = word12 ^ sp` at a single `_setjmp` in `main`; then at every other save the decode
`xpacd(word12 ^ C0) == sp` is asked, for `setjmp`, `_setjmp`, `sigsetjmp(,0)`, `sigsetjmp(,1)`, at:
`main`; 64 frames of 64 KiB deeper (**0x411040 bytes, 4.07 MB, below** the first); a second thread
(16 MB stack) and 60 frames deeper there; a `SIGUSR1` handler on a `malloc`ed alternate stack
(`0x7cdecffa00` against `0x16d911f40`: the high bits differ); and a `fork`ed child, its main and a new
thread. Built `clang -O0` both as arm64 and `-arch arm64e`:

| process | calibrated `C0` vs TSD slot 7 | `word12 ^ C0 == sp` (no strip) | `xpacd(word12 ^ C0) == sp` | fails |
|---|---|---|---|---|
| arm64 (`decode-arm64.log`) | **equal** (`0x3212975d39302b07` both) | yes, 36 of 36 | yes, 36 of 36 | 0, child exit 0 |
| arm64e (`decode-arm64e.log`) | differ in the top bits only (`0xd3903ca6...` vs `0xd3ca3ca6...`) | no, 0 of 36 | **yes, 36 of 36** | 0, child exit 0 |

So: in an arm64 process the `pacdb` leaves the pointer unchanged (the data keys sign nothing there, by
this run) and the decode is one XOR with the process's munge token; in an arm64e process the PAC bits sit
above the address and `xpacd` strips them, the low bits XOR to the SP under the same calibrated `C0`.
**One cookie recovers every SP across 4 MB of one stack, two threads, an alternate stack on the heap and
a forked child.** The wrong-slot worry (critic 1, two frames 0x20 apart) does not arise in this form: the
slot is fixed (word 12, read from the instructions above), never searched, and the verification is
exact equality with a known SP at frames 4 MB apart and on a second stack.

`clang` accepted `__asm__("xpacd %0")` for the arm64 target (the default CPU has PAC; every Apple
Silicon does, M1 and up); `__builtin_ptrauth_strip` exists only under `-arch arm64e`.

**What would have to be true for the decode to be a premise verified before `main`, failing loud:**
1. the save the program reaches is libsystem_platform's (the runtime's slot ends in a tail call to the
   pointer taken by `dlsym` before `main`, `rebind.c:703` to 720, so it is, or an interposer's that
   reaches it: TSan's `setjmp` wrapper on Darwin calls the real `_setjmp`, carried, unread here);
2. word 12 holds `pac?(sp) ^ k` for one `k` per process: verified in the constructor by saves through
   each of the four save functions at two stack pointers at least 1 MB apart (a deeper frame, and a
   second stack: the constructor may switch to a small `mmap`ed stack), each decoding exactly;
3. `k` does not change after `main` (it is set once per process; this run: equal in a thread and after
   `fork`; a later `exec` makes a new process and a new constructor);
4. a context's SP is `__ss.__sp`, signed and not XORed: `xpacd` alone, verified the same way by one
   `getcontext`;
5. **failing loud**: where any check fails the runtime records *no decode* and falls to the route the
   sitting names for that case (A or T below, never silently C), and the `heroes doctor`-style check or a
   test of the runtime suite prints which.

Unrun: the `macos-26` leg's libsystem_platform (the CI after a push); Darwin x86-64 (no Rosetta here,
critic 3; its `_setjmp` is not read here); TSan's interposed `setjmp` (no `--sanitize=thread` run).

## Q2. Route T on Darwin: built, `F/p255/T/rebind.c`

**What I built** (a copy of `rebind.c` with its § *route J: the records* replaced): every save point
claims the buffer's record as today (keyed by buffer and thread, 4,096 records, 8 probes), gives it a
fresh 52-bit serial, and writes the token `(serial << 12 | slot) ^ secret` (the secret from
`arc4random_buf` before `main`) into the buffer's span BEFORE its `musttail` call: `[104,112)` of a
`jmp_buf`/`sigjmp_buf`, `[0x40,0x48)` of a context. A jump returns at once where no entry stands (no
read at all: shapes 7 and 8), else reads 8 bytes of its TARGET's span, takes record `slot`, and trusts
it only where its serial and thread match (a lock-free read, buffer word and serial read again after).
No pending buffer, no fingerprint, no settle. `makecontext` writes token 0 (no record).

**(a) Does a jump of Darwin's C library read or check the span?** By the instructions (Q1's listing):
`__longjmp` reads `+0x00`..`+0x68` and `+0x70`..`+0xb0`; nothing at `[104,112)` (`+0x68`). `_longjmp` /
`siglongjmp(,1)` loads **8 bytes at `+0xb0`** (`ldr x8, [x21, #0xb0]`), so `[180,184)` IS loaded, as the
high half of the word handed to `sigprocmask`, whose `sigset_t` is 4 bytes on Darwin; and
`ldur x0, [x21, #0xbc]` reads `[188,196)`. So `[180,184)` is read and (by the size of `sigset_t`)
unused: I use `[104,112)` only. A context: `__setcontext` reads `mcontext+0xa8..+0x120` and
`+0x1a8..+0x220`; `[0x40,0x48)` of the context (`mcontext+0x00`, the exception state) is neither written
by `getcontext` (`stp`/`str` at `+0xa8..+0x11c`, `+0x10`, `+0x1a8..`) nor by `makecontext` (`+0xa8`,
`+0xf8`..`+0x11c`; it `bzero`s the STACK, not the context).
**By a run** (`F/p255/q2/poison.c`, plain clang, `-O0` and `-O2`): save, write `0xEE` over `[104,112)`
and `[180,184)` (and the context's `[0x40,0x48)`), jump: `setjmp`/`longjmp`, `_setjmp`/`_longjmp`,
`sigsetjmp(,0)`/`siglongjmp` and `sigsetjmp(,1)`/`siglongjmp` each landed, the mask after the jump was
the saved one where it should be (`SIGUSR2 blocked after=0` for `(,1)`, `=1` for `(,0)`), and
`getcontext`/`setcontext` resumed; `ALL OK`, exit 0, both levels (`poison-O0.log`, `poison-O2.log`). The
SDK's `offsetof` of `__mcontext_data.__ss.__sp` printed 328 = `0x148`, as `__setcontext` reads it.

## Q5 (first part). How common are buffers A cannot prove live, on this Mac

The search, its words and what it found (`grep -rE '\b(sig)?jmp_buf\b'` over `/opt/homebrew/include`,
`/opt/homebrew/opt/*/include` (228 entries in the first) and the SDK's `usr/include`; then each hit read):

- **Thread-local**: `grep -rE '(__thread|_Thread_local|thread_local)[^;]*jmp_buf|jmp_buf[^;]*(__thread|_Thread_local|thread_local)'`
  over the same three trees: **0 matches**. None found by those words; a thread-local buffer declared in
  a `.c` file (no headers' sources are on this Mac) is not reached by this search.
- **In a library's data, reached by a handler**: `expect.h:382` declares `EXTERN sigjmp_buf exp_readenv;
  /* for interruptable read() */`, and `nm -m libexpect5.45.4.dylib` places it in `(__DATA,__common)` of
  the dylib. Expect is a Tcl extension, which Tcl loads by `load`/`package require`, i.e. `dlopen` after
  `main` (by reading; not run): its interrupted read is the shape *a correct jump out of a handler
  through a buffer in a library `dlopen`ed after `main`*, which A refuses.
- **On the heap, in a struct the library frees**: libpng's `png_struct` (`png.h`/`pngconf.h` name
  `jmp_buf`; the struct is libpng's `malloc`), the issue's own shape; Boost.GIL's `jpeg_io_base` holds
  `jmp_buf _mark` in an object of the user's. Both jump on an error path, not out of a handler.
- **On the saving function's stack**: Boost.Test's `execution_monitor.ipp:787` (`sigjmp_buf
  m_sigjmp_buf` inside `signal_handler local_signal_handler`, `:924`, `sigsetjmp` at `:930` in the
  same frame, `siglongjmp` out of its signal handler at `:884`): the shape A keeps (the stack).
- Lua: absent (`ls /opt/homebrew/opt/lua*`: no match), so `lua_pcall` is unrun, as the brief says.

### Q2 (b) to (f), by runs (`F/p255/q5/res/`, `F/p255/q5/ctx/`, logs `F/p255/logs/res.log`, `ctx.log`)

Each program below is built by stage 2 of my copy with `HEROES_RUNTIME` naming the route's runtime
(`F/p255/<route>/rt`, a copy of `runtime/` with the route's files), plain and `--sanitize`, run under
`timeout 20`.

- **(b) a partial copy** (`res/residue.h`, `how: 1`): a buffer filled at a returned frame, a later save
  point, then a handler that saves into its own buffer, copies every byte of it into the old one but
  `[104,112)`, jumps to the old one (landing back inside itself) and calls the program through twelve
  frames (`ijstale.c`'s depth, past the thunk's walk of eight). **today: exit 134, refused** (*"jumped to
  a buffer this runtime holds no record of as it now stands"*), plain and `--sanitize`. **T: exit 0,
  `answer ran for 30`, `seen 300`, plain and `--sanitize`**: the jump read the stale token, took the
  record of the shallow save, dropped the handler's entry, and **the Heroes function ran inside the
  signal** (the handler returned normally after it: `raised 0` printed after `answer ran`).
- **(f) a fill by a save the runtime never sees** (`how: 2`): the handler refills the old buffer by
  libsystem_platform's own `__setjmp` sequence written out in assembly (the arm64 listing of Q1, which is
  what a save made inside the shared cache writes: rebind.c's own header counts 52 cache images that
  import a save point and whose binds are not rewritable), then jumps to it through the rewritten
  `longjmp`. **today: exit 134, refused**, both modes (the fingerprint taken at the later save point no
  longer matches). **T: exit 0, `answer ran for 30`, both modes: the Heroes function ran inside the
  signal.** Libsystem's `__longjmp` accepted the hand-written fill (arm64: the `autdb` of Q1 leaves an
  unsigned pointer unchanged in this process), so the jump is the real one.
- With the callback called directly from the handler (one frame, `q5/residue.h`, the first version), T
  is saved by the thunk's walk (*"C called it while handling a signal"*, exit 134): the record failed
  there too, and only the walk's eight frames caught it (`logs/T-q5.log`, `t_partial`, `t_unseen`).
- **(c) a stale token**: a token names one save of one buffer by a 52-bit serial; a token copied before
  its buffer is saved again names a record whose serial moved, so a jump to the copy is a miss, a told
  refusal (by construction of `hero_rebind_jump`, not run as its own program). The serial does not wrap
  in a process (2^52 saves).
- **(d) contexts**: Darwin's `ucontext_t` (880 bytes under `_XOPEN_SOURCE`, the SDK's `sizeof`) has
  spans neither written nor read: the machine context's exception state, `[0x40,0x50)`, and its `x1` to
  `x18` (Q1's listing; the poison run of `[0x40,0x48)` resumed). So T reaches contexts by a span of their
  own and needs no record keyed by address there. **Shape 3 on Darwin**, which `repro/shapes.h` compiles
  out (`#if defined(__linux__)`), built with `_XOPEN_SOURCE` (`ctx/ctx.h`: a heap context filled by
  `getcontext`, freed, then `setjmp`): **today `--sanitize` exit 134, heap-use-after-free, `#0
  hero_rebind_fingerprint rebind.c:334`** (plain exit 0); T, A and C exit 0 both modes.
- **(e) Is it sound for the runtime to WRITE into memory the program handed the C library?** On this
  OS's bytes, by the instructions and the poison run: the eight bytes `[104,112)` lie strictly inside the
  range `__setjmp` itself writes (`[0,0xb0)`), so any buffer the C library can save into holds them, and
  no save or jump of libsystem_platform reads them. So the write corrupts nothing today. **It is not a
  premise the runtime can verify before `main` and fail safe on**: a save can be checked for writing
  the span (fill, save, compare), but a jump reading or checking it can only be checked by jumping, and a
  libsystem_platform that starts to use those bytes (a signed field, a shadow-stack word, a canary)
  turns T's write into a corrupted restore or a trap in every program, on the user's next macOS, with
  nothing for a calibration to refuse. A read (C's decode) fails toward the told refusal; a write
  fails toward corruption. And T's record is the token's word against the bytes' word: wherever the
  registers in the buffer came from somewhere the token did not ((b) and (f) above), T follows the token.

## Q6. The binding's C, and routes D, E, F

**No route changes what a binding writes.** The libpng binding (`F/p255/q5/pngdemo.h` + `pngdemo.hero`,
`extern "pngdemo.h" pkgconfig "libpng"`, `pd_decode(path: cstr lent) -> i32`) is one shim of 25 lines
whose `setjmp(png_jmpbuf(png))` is libpng's own contract (an error is a `longjmp` to a buffer inside its
heap `png_struct`): a Heroes program cannot call `setjmp` itself, so every libpng binding has this C under
every route, and clang verified it against Homebrew's `png.h` (1.6.59) in every build below (build exit 0
under today, T, A and C, `logs/<route>-q5.log`). The emitted C does not depend on the route: the route is
which runtime `HEROES_RUNTIME` names, and the same `.hero` built under the four.
**The ten names on Darwin**: `nm -gU` of the libpng program under today, T and A: **0** of `setjmp`,
`_setjmp`, `sigsetjmp`, `__sigsetjmp`, `longjmp`, `_longjmp`, `siglongjmp`, `__longjmp_chk`, the four
context names defined; `nm -u`: it imports `_setjmp` and `_longjmp` from libSystem. Darwin's arm defines
none of them (it rewrites slots), so `ffi_defined_elsewhere` (`selfhost/cli/supplied_words.hero`) tells
a Darwin program nothing different under any route. What C changes on Darwin is the rewrite table:
`_setjmp`, `__setjmp`, `_sigsetjmp`, `_getcontext` and `_makecontext` leave it (no record is kept), so
their slots stay libsystem's and `dlsym` answers libsystem's own for those five names; T and A keep
today's table. Linux's ten definitions, and what C does to them there, are the compiler-engineer's.

**D (releases told) against Darwin's allocators** (`F/p255/q6/de.c`):
`dyld_info -arch arm64e -imports /usr/lib/system/libsystem_malloc.dylib` lists `_mach_vm_deallocate` and
`_madvise` and **no `_munmap`**: libmalloc gives memory back by Mach calls inside the shared cache, whose
binds rebind.c cannot rewrite (its own header). A D on Darwin sees a program's `free` (its slot is
rewritable) but not a free made inside a cache library, not libmalloc's own page returns, not an arena
(libpng's `png_create_read_struct_2` hands libpng a `free_fn` of the program's, which may be an arena that
never calls `free`), and not `mach_vm_deallocate` called by anyone. So D's premise (*every release a
program makes passes through a name the runtime can see*) is false on Darwin by the import table.
Under ASan, Darwin's rebind already takes the interposer's pointer by `dlsym` (today's `--sanitize`
repair), so D would not displace ASan's `free` there; on Linux, where the runtime defines names, it
would, as `sigjump.c:56` says of a jump. **Not built**: it reads given-back memory wherever its premise
fails, which is the defect.

**E (a read that cannot fault)**: Darwin has no `process_vm_readv`; `mach_vm_read_overwrite` on the own
task is the equivalent. Measured (`de.log`, plain and `-fsanitize=address`): on a page `munmap`ed it
answers `kr=1` (`KERN_INVALID_ADDRESS`), no fault; on a freed 192-byte block it answers `kr=0` and the
bytes read are the allocator's own (`06 00 06 00` plain, `48 00 77 77` under ASan), with **no ASan
report**: the read of given-back memory still happens, hidden from the sanitizer, one Mach trap per
pending pair. It removes the fault, not the read, and a heap tagged by Apple's memory integrity
enforcement is unrun here. **Not built**: it cannot be more robust than C, which reads no given-back
memory at all.

**F in assembly**: not built by me (the compiler-engineer's question). On Darwin arm64 a save point
that is not a tail call would have to leave `sp`, `x29`, `x30` and `x19`..`x28` as the caller's at the
moment libsystem's `__setjmp` runs, and libsystem signs `x30` with `pacibsp`/`retab` only in its own
frame: a stub that `bl`s to `_setjmp` hands it the stub's `x30`, so the jump would return into the stub,
whose frame is gone by then. By reading Q1's listing; unrun.

## Q3. Routes A and C on Darwin: both built

### A, `F/p255/A/rebind.c` (today's file, four changes)

1. `hero_rebind_lives(buf, 192)`: true only where the bytes lie on this thread's stack
   (`pthread_get_stackaddr_np`/`_stacksize_np`), in a writable segment (`initprot & VM_PROT_WRITE`) of
   an image loaded before `main` (collected in the constructor from `_dyld_image_count()`, up to 1,024
   segments, before the add-image callback is registered), or on the alternate stack **only while the
   thread runs on it** (`SS_ONSTACK`: a registered alternate stack the thread is not on may have been
   freed; my first build accepted any registered one, corrected before any run).
2. `hero_rebind_settle` fingerprints the pending buffer only where it lives; elsewhere its record is
   marked `printed = 2`, which no jump trusts (the told refusal).
3. `hero_rebind_fingerprint` is `no_sanitize("address")`: a machine read of a mapped stack, where a
   returned frame's bytes may lie under a later frame's redzone.
4. **The read at a jump outside every handler is gone**: `hero_rebind_jump` asks `now == NULL` first and
   returns before `hero_rebind_settle` (shapes 7 and 8). A pending buffer not settled there stays
   pending until the next save point, and a jump outside every handler refills nothing.

### C, `F/p255/C/rebind.c` and `F/p255/C/sigguard.c`: the target's stack pointer decoded

- **No save point is rewritten and no record is kept**: `_setjmp`, `__setjmp`, `_sigsetjmp`,
  `_getcontext` and `_makecontext` leave the table; `hero_rebind_records`, the pending key and the
  `makecontext` assembly are gone.
- **The entries are addresses in the claimed thread's record**, as B' keeps them on Linux: `sigguard.c`'s
  `HeroSignalStack` gains `entries[8]`, `depth`, `unsure` under `HERO_SIG_REBIND`; the trampoline pushes
  its own frame and pops it. **Nothing ever reads through an entry** (today's linked list lives in the
  trampolines' frames; walking it after a jump the runtime did not see would read a dead frame, which may
  lie on a coroutine stack the program freed: the defect's class again).
- **The decode** (Q1): word 12 of a `jmp_buf`, `xpacd(word ^ k)`; a context's `+0x148`, `xpacd(word)`.
  `k` from one `_setjmp` at a known SP (a naked stub records `sp` and branches to the C library's own,
  `br x2`), then **verified before `main`** for `setjmp`, `_setjmp`, `sigsetjmp(,0)`, `sigsetjmp(,1)` and
  `getcontext` at that SP, at 16 frames of 64 KiB deeper (1 MiB) and on a 256 KiB `mmap`ed stack (a naked
  stack switch); any mismatch leaves `hero_rebind_decodes` 0 and **every jump inside a handler then keeps
  every entry, marked unsure: the told refusal**. Darwin x86-64 gets no decode (judged by reading: the
  fallback).
- **A jump outside every handler reads nothing** (`depth == 0` returns first).
- **Two builds, the first refused by my own run.** C1 carried `parts/siginstall.c`'s rules whole: the
  prune drops an entry on the alternate stack whenever the target is on the thread's own, and the thunk
  counts only an entry on its own frame's kind of stack and above it. `F/p255/q5/ctx2/` (a handler swaps
  to a coroutine on a heap stack; `how 2`: the coroutine calls the program while the handler is
  suspended in `swapcontext`, the signal still being handled) **ran the Heroes function under C1, exit 0,
  `answer ran for 2`, plain and `--sanitize`** (`logs/ctx2-C1.log`), where today and T and A refuse it.
  So C (C2, the build every C row below is) classifies an address as the thread's own stack, the
  alternate stack registered now, or OTHER (a context's stack, extent unknown); drops an entry only when
  it is deeper than the target **on the same known stack**, or at a **`longjmp`** (never a context
  swap, which suspends rather than kills) when the entry is on the alternate stack and the target on the
  thread's own; and the thunk refuses while **any** entry still stands after the thunk's own frame has
  pruned the dead ones. An entry on an OTHER stack is never proved left by an address: a handler running
  on a coroutine's stack that jumps out within that stack is refused, a known cost (by construction; no
  program of mine runs it).
- **What C1's hole says about Linux, a question and not a premise**: C1's thunk rule is
  `siginstall.c:211` to 220 as it stands today (`hero_sig_on_own(own, w) == here && w > frame`), so
  today's Linux arm may run `ctx2`'s `how 2` the same way. Run by me once in `heroes-linux-arm64:latest`
  under today's runtime (`F/p255/linux-ctx2.sh`, my copy mounted read-only): the result is in the
  table of Q4 below; the compiler-engineer holds that platform.

## A finding outside the question: today's Linux runs a Heroes function inside a handler, measured

`F/p255/q5/ctx2/ctx2.h` (a correct C program's shape: a handler `swapcontext`s to a coroutine whose
context came from `getcontext` + `makecontext` on a 64 KiB heap stack; `how 1`: the coroutine swaps
back and the handler, **still running**, calls the program through twelve frames; `how 2`: the
coroutine calls the program while the handler is suspended in `swapcontext`). Run once by me on Linux
arm64 (`heroes-linux-arm64:latest`: `ldd (Debian GLIBC 2.41-12+deb13u3) 2.41`, `Debian clang version
22.1.8`, `aarch64`), **today's runtime** (`HEROES_RUNTIME` = a copy of HEAD's `runtime/`, stage 1 from
the seed in the container, `sources 05186f2b38e77a97`), `F/p255/linux-ctx2.sh`,
`F/p255/logs/linux-ctx2-clean.log`:

| Linux arm64, today | plain | `--sanitize` |
|---|---|---|
| `how 1`: the handler, still running, calls the program | **exit 0, `answer ran for 1`** | **exit 0, `answer ran for 1`** |
| `how 2`: the coroutine calls while the handler is suspended | **exit 0, `answer ran for 2`** | **exit 0, `answer ran for 2`** |

On this Mac, today: both refused, exit 134 (*"jumped to a buffer this runtime holds no record of"*),
plain and `--sanitize` (`logs/ctx2.log`). **By reading, the cause on Linux** (unrun beyond the four runs
above): `makecontext` is none of `sigjump.c`'s ten names, so the coroutine's context keeps the record
`getcontext` wrote at `main`'s frame; the swap into it prunes by that frame, which lies above the
handler's wrapper on the thread's own stack, and drops the live entry; and `how 2` is the thunk rule
C1 carried (an entry on another kind of stack is not counted). **This is Heroes code running inside a
signal handler under today's runtime on Linux: a soundness defect of its own, blocking by its class**,
for the coordinator to file (no GitHub write here). Under Darwin's C as built (C2) both are refused
(Q4); Linux's C as the compiler-engineer builds it would fix `how 1` by the decode (the coroutine's SP
is on a heap stack) and keep `how 2` unless it takes C2's thunk rule.
And the binding's C was judged at the boundary as it should be: my first `ctx2.h` allocated 1,024 bytes
for a context, enough on Darwin (`sizeof(ucontext_t)` 880) and not on glibc (4,560); the Linux build
refused the header, `error[ffi_header_refused]: ... allocation of insufficient size '1024' for type
'ucontext_t' ... with size '4560' [-Werror,-Walloc-size]`, before anything ran
(`logs/linux-ctx2-first.log`). The header verification caught a real overflow in the binding's C.

## Q4 and Q5. The same programs under every route, this Mac

Every cell is *plain / `--sanitize`*: the exit, and for a refusal its words' kind: **S** *"a signal
handler ... this runtime saw begin had not been seen to end"* (standing), **U** *"jumped to a buffer this
runtime holds no record of as it now stands (copied ..., refilled ..., or saved before more save
points ...)"* (unfollowed), **K** *"C called it while handling a signal"* (the walk), **R** the program's
function ran, **A** an AddressSanitizer report. Programs built by stage 2 of my copy with
`HEROES_RUNTIME=F/p255/<route>/rt`; runners `F/p255/runall.sh`, `q5/run.sh`, `q5/{res,ctx,ctx2,fake,nest}/run.sh`;
logs `F/p255/logs/<route>-shapes.log`, `<route>-q5.log`, `res.log`, `ctx.log`, `ctx2.log`, `fake.log`
(their `C` rows are C1's; C2's are `*-C2.log` and `C-shapes.log`, `C-q5.log`). C1's column is the first C
build, kept for the hole it showed.

| program | today | T | A | C1 | **C (C2)** |
|---|---|---|---|---|---|
| shape 1 `heap` | 0 / 134 A `hero_rebind_fingerprint rebind.c:334` | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 |
| shape 2 `mmap` | **139** / 134 A (SEGV) | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 |
| shape 3 `context` (compiled out on Darwin) | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 |
| shape 3 on Darwin, `ctx/ctx.h` (`_XOPEN_SOURCE`) | 0 / **134 A** `rebind.c:334` | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 |
| shape 4 `stack` | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 |
| shape 5 `handler_heap` | 0 / 134 A | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 |
| shape 6 `handler_mmap` | **139** / 134 A | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 |
| shape 7 `plain_heap` | 0 / 134 A, `#2 hero_rebind_jump rebind.c:425` | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 |
| shape 8 `plain_mmap` | **139** / 134 A | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 |
| `ijn.c` (control) | 134 S / 134 S | 134 S / 134 S | 134 S / 134 S | 134 S / 134 S | 134 S / 134 S |
| `ijstale.c` (the refill) | 134 U / 134 U | 134 **S** / 134 S | 134 U / 134 U | 134 S / 134 S | 134 **S** / 134 S |
| `ijo.c` (`SA_ONSTACK`) | 134 S / 134 S | same | same | same | same |
| `ijv.c` HOW 1, 2, 3 (3 = the nested SIGUSR2) | 134 S / 134 S each | same | same | same | same |
| **libpng 1.6.59**, `q5/pngdemo` (decode, truncated file's error path, decode again, error again) | 0 / **134 A**: heap-use-after-free, `#0 hero_rebind_fingerprint rebind.c:334` < `hero_rebind_setjmp` < **libpng's own `png_create_png_struct`**; freed by `png_destroy_png_struct` | 0 / 0, all four answers | 0 / 0 | 0 / 0 | 0 / 0 |
| correct jump out of a handler, **heap** buffer (`jo_heap`) | 0 R / 0 R | 0 R / 0 R | **134 U / 134 U** | 0 R / 0 R | 0 R / 0 R |
| the same, **`_Thread_local`** (`jo_tls`) | 0 R / 0 R | 0 R / 0 R | **134 U / 134 U** | 0 R / 0 R | 0 R / 0 R |
| the same, a **library `dlopen`ed after `main`** (`jo_lib`, `libjo.dylib`) | 0 R / 0 R | 0 R / 0 R | **134 U / 134 U** | 0 R / 0 R | 0 R / 0 R |
| coroutine, context and stack freed while suspended, then a save (`jo_co`) | 0 R / 0 R | 0 R / 0 R | 0 R / 0 R | 0 R / 0 R | 0 R / 0 R |
| partial copy into a buffer, then a jump INSIDE the handler, 12 frames (`res`, `how 1`) | 134 U / 134 U | **0 R / 0 R** | 134 U / 134 U | 134 S / 134 S | 134 S / 134 S |
| refill by a save never seen, then a jump INSIDE the handler, 12 frames (`res`, `how 2`) | 134 U / 134 U | **0 R / 0 R** | 134 U / 134 U | 134 S / 134 S | 134 S / 134 S |
| handler swaps to a coroutine and back, then calls, 12 frames (`ctx2`, `how 1`) | 134 U / 134 U | 134 S / 134 S | 134 U / 134 U | 134 S / 134 S | 134 S / 134 S |
| the coroutine calls while the handler is suspended (`ctx2`, `how 2`) | 134 U / 134 U | 134 U / 134 U | 134 U / 134 U | **0 R / 0 R** | 134 S / 134 S |
| correct jump out through a buffer a non-saving frame holds, `--sanitize` only: default / `ASAN_OPTIONS=detect_stack_use_after_return=1` (`fake`) | 0 / 0 | 0 / 0 | 0 / **134 U** | 0 / 0 | 0 / 0 |

**What moves against today, and which way.** T, A and C each close every shape of defect 2679 on this
Mac, plain and sanitized, libpng's included. **T** runs the Heroes function inside the signal on the two
residue programs today refuses (the dangerous direction). **A** refuses three correct programs today
runs (heap, thread-local, late library) and the fake-stack one under the user's ASan option, each in
words false for it (*"copied ..., refilled ..., or saved before more save points"*: none of the three
happened). **C** moves no program toward running inside a signal; it changes the refusal's words from U
to S where the handler is in fact still running (`ijstale`, the residues, `ctx2`), which is the
truer sentence. `jo_co` does not reproduce the defect under today: `swapcontext`'s wrapper settles
`from` before the C library fills it (critic 10's reading, confirmed by this outcome), consuming the
pending context, so nothing is pending when it is freed; shape 3 on Darwin is the context reproducer.

**E's other channel, `write` into a pipe** (`F/p255/q6/ewrite.c`, `ewrite.log`, `ewrite-asan.log`):
plain, an unmapped page answers `write=-1 errno=14 (Bad address)` and a freed 192-byte block is
written whole (`write=192`, the given-back bytes read); under `-fsanitize=address` the freed block is
**reported**: `heap-use-after-free ... #0 write.cold.2 ... #1 write (libclang_rt.asan_osx_dynamic.dylib)`,
`freed by ... free`. So E through `write` turns a `--sanitize` program's correct C into a sanitizer
report against the runtime, as today's read does; through `mach_vm_read_overwrite` it hides the read
from ASan instead. Neither removes it.

## `00-shared.md` § 6's two questions, per route, with the program that shows each

**May one program's verdict depend on the platform?** design.md has no rule either way
(`grep -n -i -E 'every platform|all (three )?platforms|portab' docs/design.md`: the coordinator's
search; mine adds §1.12's *"the boundary is **complete**: any C library must be bindable"*, which names
no platform). My answer as the FFI seat: **no**, because a binding is written once and its author
cannot test it on a platform they do not have; a correct C library whose error path works on Linux and
is refused on Darwin is §1.12's completeness failing on one leg, found by a user. Per route, on Darwin:
- **A** would split from a Linux C on `jo_heap`, `jo_tls`, `jo_lib` (Darwin A: 134 U, measured; Linux C:
  the compiler-engineer's) and on the copied-buffer case.
- **T** would split from a Linux C on `res` how 1 and 2 (Darwin T: the function runs inside the signal;
  Linux C reads the true SP, so by construction it refuses them; Linux unrun by me).
- **C (C2)** gives on Darwin the verdicts a Linux C gives by construction on every program above but
  one: `ctx2` how 2, which C2 refuses and **today's Linux runs** (measured, the finding above); a Linux C
  that keeps `siginstall.c`'s thunk rule would run it too, so the split closes only if Linux takes C2's
  rule (any standing entry refuses; an entry on another stack is never proved left by an address).

**Does the route owe the spec a sentence?** Spec lines 427 and 428 lead a reader to predict that a
program which has LEFT its handler by a jump runs its callback.
- **A**: yes. It refuses `jo_heap`, `jo_tls`, `jo_lib` (and `fake` under the user's ASan option), each a
  program that left its handler by a `longjmp`/`siglongjmp` the runtime saw; the reader predicts they
  run, and the refusal's own words name three causes none of which happened. A owes at least new words,
  and a sentence if the sitting keeps it anywhere.
- **T**: the sentence it would owe is the wrong kind: its failure is a program that does NOT leave the
  handler being treated as one that did (`res`), which no sentence makes safe.
- **C (C2)**: no new sentence for any program in this report but one cost it adds against today's
  Darwin, measured below (`cojump`: a handler running on a coroutine's own heap stack that jumps out
  within that stack). It removes a refusal the spec's reader would not expect (the copied buffer, ruling
  B's cost, by construction; the harness run below says how it lands), and the refusals it keeps are
  ruling B's (a jump the runtime cannot see).

### C, third build (C3): the cost C2 added, measured, then removed

`F/p255/q5/cojump/` (a coroutine on a 256 KiB heap stack saves a buffer, raises a signal whose handler
runs on the coroutine's stack, the handler `siglongjmp`s out to that buffer, and the coroutine, the
handler left, calls the program through twelve frames): **today, T, A and C1 run it, exit 0, both
modes; C2 refused it, exit 134 S, both modes** (`logs/cojump.log`, `cojump-C2.log`): an entry on a stack
of unknown extent is never proved left, so a correct jump out within a coroutine's stack stood refused.
**C3** (`F/p255/C/rebind.c` as it stands; C2 kept in `F/p255/C2/`) gives `makecontext` its slot back
(today's naked wrapper, now calling `hero_rebind_known_stack`): the context's `uc_stack` (it lives; the C
library reads the same two fields next) becomes a **known stack**, kept in a table of 256 ranges until a
later stack overlaps it (two live stacks never share an address, so a range a later one overlaps is
dropped); written under a spin flag (`makecontext` is no signal-safe call), read slot by slot under a
sequence word (a slot read mid-write is a stack of unknown extent, the conservative answer). An entry and
a target on the same known stack compare by address like the thread's own. **C3: `cojump` exit 0,
`answer ran for 9`, both modes; `ctx2` how 1 and how 2 still 134 S, both modes** (`logs/cojump-C3.log`,
`ctx2-C3.log`). A stack switched by hand-written assembly (Boost.Context, libco) is never seen and
stays of unknown extent: its handler's entries stand, a told refusal (by construction; unrun).
Every C row of the harness table below is C3.

Two more programs, and C3's column for every program above (`logs/*-C3.log`, `C-shapes.log`,
`C-q5.log` as they now stand; C2's kept as `C2-shapes.log`, `C2-q5.log`, `*-C2.log`):

| program | today | T | A | C1 | C2 | **C3** |
|---|---|---|---|---|---|---|
| `nest` (ijv HOW 3 made correct: SIGUSR2's handler jumps back into SIGUSR1's, which jumps OUT to `main`, then `main` calls through 12 frames), own stack / alternate stack | 0 R / 0 R, both | same | same | not run | not run apart (its `C` rows in `nest.log` straddle the C2 to C3 rebuild) | **0 R / 0 R, both** |
| `cojump` (a handler on a coroutine's heap stack jumps out within it, then the coroutine calls) | 0 R / 0 R | 0 R / 0 R | 0 R / 0 R | 0 R / 0 R | **134 S / 134 S** | **0 R / 0 R** |
| every row of the table above | | | | | as C1 but `ctx2` how 2: 134 S | **as C2**: shapes 1 to 8 clean both modes, `ij*` 134 S, libpng 0/0, `jo_heap`/`jo_tls`/`jo_lib`/`jo_co` 0 R, `res` 134 S, `ctx2` 134 S both, `fake` 0/0 |

## Q4. The harness, per route, on this Mac

Each route's files copied into my copy's `runtime/parts/`, stage 2 rebuilt and installed
(`F/p255/gate.sh`; `--version`: T `sources 7d9b8fb979a3b272`, A `sources 1b1cffd10c08426f`), then
`./heroes run tests/harness/main.hero -- ./heroes run <set>` and `-- ./heroes runtime` (`F/p255/sets.sh`),
each count line read whole from `F/p255/logs/<route>-<set>.log`. The baseline of the three sets is the
critic's (`00-shared.md` § 4: 2571 4/0, 2558 5/0, 2522 25/0); the rest of the *today* column is mine.
Every run beside other sessions' work (load 8 to 74 by `uptime`): times are not claimed.

| set | today | T |
|---|---|---|
| `fixedbugs-2571` | 4 passed, 0 failed (critic) | 4 passed, 0 failed |
| `fixedbugs-2558` | 5 passed, 0 failed (critic) | 5 passed, 0 failed |
| `fixedbugs-2522` | 25 passed, 0 failed (critic) | **23 passed, 2 failed**, exit 1 |
| `fixedbugs-a-c-handler-that-recovers-leaves-no-line` | 1 passed, 0 failed | 1 passed, 0 failed |
| `fixedbugs-2494` | 15 passed, 0 failed | 15 passed, 0 failed |
| `fixedbugs-2257` | 3 passed, 0 failed | 3 passed, 0 failed |
| `fixedbugs-2583` | 6 passed, 0 failed | 6 passed, 0 failed |
| `fixedbugs-2496` | 1 passed, 0 failed | 1 passed, 0 failed |
| `fixedbugs-2503` | 1 passed, 0 failed | 1 passed, 0 failed |
| `an-ignored-signal-stays-ignored` | 1 passed, 0 failed | 1 passed, 0 failed |
| `the-platform-layer-stop` | 3 passed, 0 failed | 3 passed, 0 failed |
| `runtime` | 10 passed, 0 failed, 2 floors outgrown | **9 passed, 1 failed**, 2 floors outgrown, exit 1 |

(The `fixedbugs-2494` filter counts 15, not the brief's 14 `.hero` files: the harness's own count line,
read whole.) **T's two moved 2522 verdicts**, `logs/T-fixedbugs-2522.log`: the refill case
(`...-a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused`) **still refused**, its words now S
(*"had not been seen to end"*) where its `.expected` pins U: the direction holds, the words move; and
the copied-buffer case (`...-through-a-copied-buffer-is-refused-in-words-that-say-so`) **now runs**,
`called 8`, exit 0: ruling B's known cost flips to the correct verdict. **T's `runtime` failure** is the
census of statics (`runtime/threads`): `runtime/parts/rebind.c:325: static uint64_t hero_rebind_secret
= 0;` is neither per-thread, atomic, nor in `SHARED_BY_DECISION`: a reason owed, not a defect.

**A, the same twelve runs** (`logs/A-<set>.log`): `fixedbugs-2571` 4 passed, 0 failed; `fixedbugs-2558`
5 and 0; **`fixedbugs-2522` 25 passed, 0 failed** (no golden case moves: every one keeps its buffer
static or on the stack, as the question read; the refill case keeps its U words, the copied-buffer case
stays refused); the recovers case 1 and 0; `fixedbugs-2494` 15 and 0; `2257` 3 and 0; `2583` 6 and 0;
`2496` 1 and 0; `2503` 1 and 0; `an-ignored-signal-stays-ignored` 1 and 0; `the-platform-layer-stop` 3 and
0; **`runtime` 9 passed, 1 failed**, 2 floors outgrown: the census names `rebind.c:370` to 372,
`hero_rebind_region_lo`, `_hi`, `hero_rebind_regions` (written once before `main`, a reason owed). So
the harness sees none of A's costs: they live only in programs no golden case holds (Q4/Q5's table).

**C (C3), the same twelve runs** (`--version` `sources 7f2474ff76a1a378`; `cmp` of `runtime/parts/rebind.c`
and `sigguard.c` against `F/p255/C/` before the sets: equal; `logs/C-<set>.log`):

| set | today | T | A | **C (C3)** |
|---|---|---|---|---|
| `fixedbugs-2571` | 4 / 0 | 4 / 0 | 4 / 0 | **4 passed, 0 failed** |
| `fixedbugs-2558` | 5 / 0 | 5 / 0 | 5 / 0 | **5 passed, 0 failed** |
| `fixedbugs-2522` | 25 / 0 | 23 / 2 | 25 / 0 | **23 passed, 2 failed**: the same two as T, the same way |
| recovers case | 1 / 0 | 1 / 0 | 1 / 0 | **1 passed, 0 failed** |
| `fixedbugs-2494` | 15 / 0 | 15 / 0 | 15 / 0 | **15 passed, 0 failed** |
| `fixedbugs-2257` | 3 / 0 | 3 / 0 | 3 / 0 | **3 passed, 0 failed** |
| `fixedbugs-2583` | 6 / 0 | 6 / 0 | 6 / 0 | **6 passed, 0 failed** |
| `fixedbugs-2496` | 1 / 0 | 1 / 0 | 1 / 0 | **1 passed, 0 failed** |
| `fixedbugs-2503` | 1 / 0 | 1 / 0 | 1 / 0 | **1 passed, 0 failed** |
| `an-ignored-signal-stays-ignored` | 1 / 0 | 1 / 0 | 1 / 0 | **1 passed, 0 failed** |
| `the-platform-layer-stop` | 3 / 0 | 3 / 0 | 3 / 0 | **3 passed, 0 failed** |
| `runtime` | 10 / 0 | 9 / 1 | 9 / 1 | **8 passed, 2 failed** |

C's two 2522 moves (`logs/C-fixedbugs-2522.log`): the refill case still refused, its words now S
(*"had not been seen to end"*), true there since the handler IS still running, where the `.expected`
pins U; the copied-buffer case runs, `called 8`, exit 0 (ruling B's known cost gone, which is the
author's to read). **The twelve `a-callback-after-*` cases pass under C**, alternate stack and own
stack, `longjmp`, `_longjmp`, `siglongjmp`, deep-first and shallow-first: the decode and the prune follow
every correct jump out the golden tree holds. C's `runtime` failures are the census: two statics owe a
reason (`rebind.c:142` `hero_rebind_stack_writing`, an `atomic_flag` the census does not read as
atomic; `rebind.c:372` `hero_rebind_key_k`, written once before `main`) and one `SHARED_BY_DECISION`
entry, `hero_rebind_records`, now names nothing and is to be deleted. (The census reads `static`
objects only: C's non-static `hero_rebind_cal_sp`, written by the stub's `str`, is not named by it,
and today's non-static `hero_rebind_real_makecontext` is not either: a gap of the instrument, said here
and not filed.)

## Verdicts (Darwin; each built and run in my copy, `wc -l`: today's `rebind.c` 727, T 697, A 783, C 737 plus 6 in `sigguard.c`)

- **T: veto, on soundness** (design.md §1.12; `00-shared.md` § 6's own list: *Heroes code running
  inside a signal*). Under T the program's function ran inside the handler on `res` how 1 (a partial
  copy) and how 2 (a fill by a save the runtime never sees, a save inside the shared cache's shape):
  exit 0, `answer ran for 30`, plain and `--sanitize`, both refused by today, A and C. And its write
  into libsystem's bytes rests on a premise (no jump reads `[104,112)`) that no check before `main` can
  make fail toward refusal. Everything T gives (the copied buffer running, no read of given-back memory,
  no read at a jump outside a handler) C gives without either.
- **A: object.** Sound in every run (no read of given-back memory anywhere; the read at a jump outside
  every handler removed; shapes 1 to 8, libpng and the context shape clean in both modes). But it
  refuses correct programs today runs: a jump out of a handler through a heap, thread-local or
  late-loaded library's buffer (`jo_heap`, `jo_tls`, `jo_lib`: 134 U both modes) and the ASan fake-stack
  shape under the user's option, each in words false for it; the harness sees none of this (2522 25/0);
  it owes the spec words; and it splits a program's verdict from Linux's. §1.12's *"any C library must be
  bindable"* is what it spends (Expect's `exp_readenv` in a `dlopen`ed Tcl extension, by reading). Its
  right place is the floor where C's decode does not verify (Darwin x86-64, a future libsystem), never
  Darwin's route; and as that floor it is better than C's told refusal of every jump (critic 5).
- **C, as C3: approve.** The one route that reads no memory the program may have given back and writes
  none it does not own: one word of the TARGET (alive: the C library reads it next), decoded by a key
  verified before `main` on three stacks, failing toward the told refusal; entries kept as addresses and
  never read through. Measured: shapes 1 to 8 and Darwin's context shape clean, plain and `--sanitize`;
  libpng 1.6.59 clean (today dies under `--sanitize` on the second decode, inside libpng's own
  `png_create_png_struct`); every correct jump out followed (`jo_*`, `nest`, `cojump`, `fake`, the
  twelve `a-callback-after-*`); every jump that stays inside refused, in true words (`ij*`, `res`, `ctx2`);
  harness as today but the two 2522 moves (both toward the truth) and the `runtime` census (reasons owed).
  Its own known costs: a handler on a stack switched by hand-written assembly is never proved left
  (refused, told); Darwin x86-64 gets no decode (A's floor, by reading).
- **C1 (C as first built, `siginstall.c`'s rules carried whole): veto, superseded by C3**: `ctx2` how 2
  ran the program's function while the handler was suspended. The same rule stands in today's Linux and
  ran it there (the finding above).
- **D and E: object, not built**: D's premise is false on Darwin by `libsystem_malloc`'s import table
  (`mach_vm_deallocate`, `madvise`, no `munmap`); E reads the given-back bytes anyway (`mach_vm_read`
  hides it from ASan; `write` is reported by ASan's interceptor). **F: unrun**, the compiler-engineer's.

**The prediction, one, falsifiable**: on the CI's `macos-26` leg, after the push that lands route C on
Darwin as `F/p255/C/rebind.c` builds it, the constructor's calibration verifies (word 12 is
`pacdb(sp, 0xcbed) ^ munge` and a context's `+0x148` is `pacda(sp, 0xcbed)` there too), so
`fixedbugs-2522` gives exactly this Mac's C verdicts: the twelve `a-callback-after-*` cases pass with
their `.expected` unchanged, the copied-buffer case runs, the refill case is refused in S words, and no
case is refused in the unsure words a failed calibration would produce. Checkable at the lane that lands
C (this Mac) and then at the `macos-26` leg after its push.

## Unrun, said so

- The `macos-26` leg's libsystem_platform (my decode, T's span and A's segments are this Mac's, 27.2.1);
  Darwin x86-64 (no Rosetta; C has no decode there by construction, A by reading); arm64e processes
  beyond the Q1 probe (no Heroes program is one: `file`: arm64).
- `--sanitize=thread` (TSan's interposed `setjmp`) under any route.
- Lua (`lua_pcall`): absent here. Expect/Tcl: by reading only. Boost.Test's monitor: by reading only.
- Linux under T, A and C: the compiler-engineer's; my one Linux run is today's runtime on `ctx2`.
- C3's known-stack table under many threads calling `makecontext` at once (written under a spin flag;
  no stress run); a `makecontext` stack later reused by two smaller stacks (by construction only).
- Timing of any route: not owed, not taken.
- The census reasons and the `.expected` rewrites each route owes: not written (no tracked file of the
  copy was kept changed; `git status --short` after the gate restored the tree: `?? p255/` only,
  `sources 61416c0c33ecc5a9`, the tree's key).

## Status

Finished 19:26 (`date`). Every route's files stay in `F/p255/<route>/`, C1 and C2 kept beside C3.

(That C's calibration verified in these runs is read from its outcomes, the runtime printing nothing of
it: with `hero_rebind_decodes` 0 every jump inside a handler keeps every entry, so `jo_heap`, `jo_tls`,
`jo_lib`, `nest` and `cojump` would be refused; they ran, plain and `--sanitize`.)

## The seat's form

- **verdict**: T veto; A object; C (C3) approve; C1 veto (superseded); D, E object (not built).
- **section**: design.md §1.12 (*"must not segfault and must not corrupt memory"*, and the boundary
  *"complete: any C library must be bindable"*), §1.11, §4.19; the sitting's veto list in `00-shared.md`
  § 6. design.md has no rule on a verdict that differs by platform; said above.
- **experiment**: libpng 1.6.59 through a 25-line shim, clang-verified against Homebrew's `png.h` under
  every route (build exit 0 each); decode, truncated-file error path, decode again: today `--sanitize`
  134 heap-use-after-free inside libpng's own `png_create_png_struct`, T/A/C 0 and 0. Plus the decode
  probe (Q1), the poison run (Q2), and the programs and harness tables above.
- **argument**: C is the only route whose every read is of memory the C library is about to read (the
  jump's target) and whose premise is checked before `main` and fails toward refusal; it keeps today's
  set of followed jumps, adds the copied buffer, refuses every jump that stays inside in true words, and
  closes 2679's eight shapes and libpng's sanitized crash. T writes C-library-owned bytes on an
  unverifiable premise and ran a Heroes function inside a handler twice. A is sound but refuses correct
  heap, thread-local and late-library buffers in false words, invisible to the harness, and splits
  verdicts by platform.
- **prediction**: above: on `macos-26`, after C lands, the calibration verifies and `fixedbugs-2522`
  gives this Mac's C verdicts, no case refused in the unsure words.
- **condition**: C's approve turns to object if the `macos-26` (or any supported macOS) calibration
  fails, or if a program shows C's prune dropping a live entry; T's veto lifts only with a check before
  `main` that fails safe on a jump that reads the span, and a token that the registers cannot outrun;
  A's objection lifts if the sitting rules a verdict may differ by platform and the spec carries A's
  refusals in words true for them.
