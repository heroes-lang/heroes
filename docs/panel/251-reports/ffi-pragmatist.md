# Panel 251 (sitting A of M-rebel-library), ffi-pragmatist

Written as I go, 2026-11-19, from 10:08 CET (`date`). My folder is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/251-ffi-pragmatist/` (called `F/`
below), made by `git archive 3a5ee2691 | tar -x`, **20,022 files** (`find -type f | wc -l`, as the brief's),
its `.git` file pointing nowhere. Every number below carries its command and its log under `F/logs/`.

## 0. The copy, and premises of the briefs found false

**My copy is on the frozen commit**: the seed by `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`,
exit 0, `real 53.90`; stage 2 by `./heroes build selfhost/main.hero -o heroes-next`, exit 0, `real 551.46
user 303.44 sys 68.32` (load 12 to 14, other sessions building), **`sources 23c7d2a626b29fd5`**
(`F/stage2.log`). Apple clang 21.0.0, Darwin 27.2.0.

Premises found false, each run:

1. **"libSystem's own by a handle" is not a repair of the `--sanitize` loop** (Q1.1's first option). Under
   ASan, `dlsym` on libsystem_c's own handle, opened `RTLD_NOLOAD | RTLD_FIRST`, answers ASan's
   `wrap_sigaction`, exactly as `RTLD_DEFAULT` does: dyld applies an interposer to every lookup, a handle
   included (`F/q1/asan/fp-look.c`, `F/logs/q1-1-lookups.txt`). Only the second option works.
2. **`kmiss` does not build on Darwin**: `kmiss.h` clobbers `x18`, which Darwin reserves, and the frozen
   compiler refuses the header, `error[ffi_header_refused]: ... inline asm clobber list contains reserved
   registers: X18 [-Werror,-Winline-asm]`, at `-O0` and `-O2` (`F/logs/q1-3-matrix-base.txt`). Its Darwin
   twin, `kmiss-darwin.h`, is the same file less `"x18"` in the clobber list (`diff`: one line), and is the
   `kmiss` row below.
3. **No crash report exists to join**: the storms left none. `find ~/Library/Logs /Library/Logs -name
   '*.ips' -newer <marker>` prints 0 after 50 storms, `~/Library/Logs/DiagnosticReports` does not exist,
   and `launchctl list` shows `com.apple.ReportCrash` loaded (pid 99453, last exit `-9`). So `ips.py` had
   nothing to read; the storms' own exit statuses are the measure.
4. **D-rebind, as 249 built it, displaces TSan's own signal handler** (found under Q1.5, not a premise of
   the brief but of 249's file): its re-install asked the handler through `sigaction`, which TSan answers
   with the program's, and installed that raw. Repaired, below.

## Question 1: a C handler reaching Heroes, route J on Darwin

### 1.1 The `--sanitize` loop, repaired

Reproduced first: the critic's `q.c` linked with the overlay's `rebind.c` (`117dcde29c5ebea7`) dies before
`main`, 3 of 3, `AddressSanitizer: stack-overflow`, `r_sigaction` calling itself. **Why**, measured: under
ASan every lookup of `sigaction`, `signal`, `longjmp`, `_longjmp` and `siglongjmp` answers ASan's wrapper,
by `RTLD_DEFAULT` and by libSystem's own handle alike (premise 1); the overlay then rewrote ASan's own image's
slot, which ASan's wrapper calls to reach the C library. TSan's runtime interposes the save points as well
(`nm -m`: `wrap_setjmp`, `wrap__setjmp`, `wrap_sigsetjmp`, `wrap_swapcontext`, besides the jumps), and Guard
Malloc carries an `__interpose` section in `__AUTH_CONST`.

**The repair**: an image holding a section named `__interpose` is left unrewritten (every interposer by
construction: ASan, TSan, Guard Malloc, any `DYLD_INSERT_LIBRARIES` tool), and the C library's pointers are
still taken by `dlsym(RTLD_DEFAULT)` before `main`, so a rewritten slot reaches the interposer's wrapper,
which reaches the C library. **Run**: `q.c` with the repaired file, J off and on, plain and
`-fsanitize=address`, `-O0` and `-O2`: **3 of 3 `main reached`, exit 0, in each of the 8 cells**
(`F/logs/q1-1-qprobe.txt`). **`flib` through the compiler** (`heroes-next build flib.hero --sanitize -O2`
and `--sanitize -O0`, `HEROES_RUNTIME` the overlay laid over the frozen runtime): the overlay's own file
**12 of 12 dead before `main`** (ASan's stack overflow, static and dylib); the repaired D-rebind and
D-rebind+J **3 of 3 refused with R1's words in each of the 8 cells, `installed` line printed first,
nothing before `main`** (`F/logs/q1-1-rflib-sanitize.txt`).

### 1.2 Route J, built into D-rebind (`F/q1/rebind-j.c`, 222 lines, `74f666a3998d6012`)

Started from the overlay `bw/routes/drebind/`, its `runtime.c` and `sigguard.c` taken as they are (the
thunk asks `hero_tramp_inside()`), its `rebind.c` replaced by mine. What J adds:

- **The save points' slots** `_setjmp`, `__setjmp`, `_sigsetjmp`, `_getcontext`, the save half of
  `_swapcontext` (the SDK's `libsystem_platform.tbd` lists these, with `_makecontext` and the jumps) each
  record **the buffer, the thread and the innermost trampoline entry live at the save**, then call the C
  library's own **as a tail call** (`__attribute__((musttail))` through the pointer taken before `main`).
  `otool -tV` of the object: each ends in `br x1` or `br x2`, a branch, at `-O0` and at `-O2`
  (`F/logs/q1-2-musttail.txt`); **compiled and now run** (249 carried it compiled only).
- **`_makecontext` forgets** its context's record, the context now naming another stack. `musttail` is
  **refused** there (clang: *"'musttail' attribute may not be used with variadic functions"*), so that one
  wrapper is a naked function in assembly, arm64 and x86-64 both written; `clang -arch x86_64 -c` exit 0,
  not run on x86-64.
- **Each jump's slot** (`_siglongjmp`, `_longjmp`, `__longjmp`, `_setcontext`, the jump half of
  `_swapcontext`) **restores the entries recorded for its target**. On one stack these are exactly the
  entries shallower than the target, the brief's rule; **I record the entry rather than the frame** because
  Darwin's `setjmp` mangles the saved SP (`eor` with a cookie, then `autdb`, read in `dyld_info -disassemble`
  of libsystem_platform), and because a record of the entry stays right across two stacks (an alternate
  signal stack, a context swapped back into a suspended handler), where comparing addresses would not.
- **The entries are a list living in each trampoline's own frame**, the innermost in the pthread key's slot;
  `hero_tramp_inside()` asks whether the list is empty. The records are a fixed table of 4,096 written with no
  lock (a slot claimed by a compare-and-swap; any race or a full table is a miss). **Nothing in a rewritten
  slot locks, allocates or calls `dlsym`.**
- **Beside TSan, the re-install asks the kernel raw** (`__sigaction(sig, NULL, &q)`), not `sigaction`: with
  `sigaction`, the kernel held the program's handler `h` and TSan's own was gone; with the raw query it holds
  TSan's `sighandler` wrapped by the trampoline (`F/q1/beside/fp-tsan-kernel.c`, `F/logs/q1-5-tsan-displaced.txt`).

**What a `dlsym(RTLD_DEFAULT, "setjmp")` reaches**: libsystem_platform's own `setjmp` (plain and under ASan),
TSan's `wrap_setjmp` under TSan, **never the rewritten slot**, so a save point a library reaches by `dlsym`
is never recorded and its jump is a miss. **Whether libSystem makes a save inside itself, asked of the
whole shared cache** (`dyld_info -arch arm64e -imports -all_dyld_cache`, 4,115 images, 5.7 s,
`F/logs/cache-imports.txt`): **none of libSystem's 44 own images (`/usr/lib/system/`) imports a save point
or a jump** (libxpc imports `_signal` alone); inside libsystem_platform, which defines them, `sigsetjmp`
branches to `setjmp` or `_setjmp` and `swapcontext` calls `getcontext` (`dyld_info -disassemble`), both
beneath a slot J rewrites. **The cache's frameworks do save**: `_setjmp` is imported by 42 images, `__setjmp`
by 6, `_sigsetjmp` by 8 (ImageIO's libTIFF, libpcap, CarbonCore, libLLVM ...), and those binds are not
rewritable (`MH_DYLIB_IN_CACHE`), so their jumps are misses (`F/logs/cache-family-counts.txt`).

**The miss, run both ways** (`F/q1/cases/miss.c`, a buffer copied by `memcpy`, built apart `-O2
-fomit-frame-pointer`, bound by `ij.hero`; `F/logs/q1-3-matrix2-*.txt`, 20 runs per cell):

| shape | base | D-rebind | **J, a miss keeps every entry** | J, a miss drops every entry |
|---|---|---|---|---|
| jump WITHIN a handler through the copy, then the callback | inside the signal, exit 0, 40 of 40 | inside, 40 of 40 | **refused, R1, 40 of 40** | **inside the signal, exit 0, 40 of 40** |
| jump OUT of a handler through the copy, then a correct callback | runs, 40 of 40 | runs, 40 of 40 | **refused, 40 of 40: a correct program, told R1's words, which are false of it** | runs, 40 of 40 |

So the proposal's direction is the sound one, run: the other way runs Heroes code inside a signal in
silence. Its price is the second row: **a correct program refused with words that say *"C called it while
handling a signal"* when no signal is running**, a false message (`.claude/rules/verification.md` §
Bounded discovery's `blocking`). J knows a miss happened (`hero_j_count(2)`), so the words can differ: owed
(§ 1.7).

### 1.3 The C a binding writes, compiled against the frozen runtime and bound from Heroes

Three shapes, each compiled and linked by `heroes-next build` with `HEROES_RUNTIME` naming the variant
(`F/q1/matrix.sh`; `F/logs/q1-3-matrix-{base,drebind,drebindj}.txt`), every library built apart `-O2
-fomit-frame-pointer` by `clang -Wall`, exit 0, no warning; D-rebind here is the repaired one, J off.
**20 runs per cell, `-O0` and `-O2`**:

| case | base | D-rebind | **D-rebind+J** |
|---|---|---|---|
| (1) the tree's `fixedbugs-2494-a-heroes-function-a-c-handler-calls-is-refused` (`static inline` handler, frame record) | refused 40/40 | refused 40/40 | **refused 40/40** |
| (2) `flib`, the handler built apart, static archive | **inside the signal, exit 0, 40/40** | refused 40/40 | **refused 40/40** |
| (2) `flib` through a two-level dylib | **inside, 40/40** | refused 40/40 | **refused 40/40** |
| (3) `injump` (`ij.c`, `sigsetjmp` then a helper's `siglongjmp` within the handler) | **inside, 40/40** | **inside, 40/40** | **refused 40/40** |
| (3) `ijv.c` HOW 1 (`setjmp`/`longjmp`), 2 (`_setjmp`/`_longjmp`), 3 (a nested signal jumping back into the outer handler) | **inside, 120/120** | **inside, 120/120** | **refused 120/120** |
| `kmiss` (Darwin twin) | **inside, 40/40** | refused 40/40 | **refused 40/40** |
| the twelve depth programs (`dp-*.hero`, a jump out of a handler then a correct callback, deeper and shallower) | exit 0 with their lines, 24/24 cells | 24/24 | **24/24 cells, 480 runs, no false refusal** |

**Storms** (`fliblocale`, `storm.py` unchanged, `F/q1/storms.sh`, `F/logs/q1-3-storms.txt`, built `-O0`):
base **20 of 20 armed static and 5 of 5 through the dylib end `exit -9`, SIGKILL in silence, no line**;
D-rebind+J **20 of 20 armed static and 5 of 5 through the dylib end `exit -6` with R1's refusal line**, 25
refusal lines. No crash report was written (premise 3).

(1.3's goldens, Guard Malloc and the second rebinder: below, as run.)

### 1.4 The runtime's own installs, and the two forked children, under D-rebind+J

Every install and read the brief names goes through the rewritten `_sigaction` slot of the executable:
`r_sigaction` calls the C library's `sigaction` through the pointer taken before `main`, then, where an
action was given, `retramp`, which asks the kernel raw and, where the handler is neither default nor
ignored, re-installs it by `__sigaction` with the trampoline. **Each step is one system call; none locks,
allocates or calls `dlsym`.** A read (`hero_term.c:247`, `:272`, `hero_proc.c:1628`) gives no action, so it
is the C library's call alone.

**`hero_proc_child`, reached in code**: a copy of each variant with `hero_proc_spawn_enters` answering false
(`F/rt/*-fork/`), and a C driver (`F/q1/fork/fp-fork.c`) compiled against the runtime and
`platform/hero_proc.c` and `hero_stream.c`, which installs three handlers, ignores SIGPIPE, keeps **one
thread installing in a loop and one saving and jumping in a loop** (so a thread is inside each rewritten
slot at the fork: 0.7 to 12.0 million installs and 0.5 to 1.3 million jumps beside each run), and starts a
child in the folder `/` 200 times. **Base, D-rebind and D-rebind+J, `-O0`, `-O2` and ASan `-O0`, 3 runs of
200 each: 1,800 children per variant, every one started, saw SIGPIPE at its default and ended 0, no hang
inside the 60 s bound** (`F/logs/q1-4-forked-child.txt`). **That the fork path was the one taken is
measured**: a copy whose `hero_proc_child` writes one byte with `write(2)` printed 200 bytes for 200 starts.
`run.c:1805`'s child is the same kind of calls, fewer (at most four resets and one SIGXFSZ ignore); it is
taken by every launch with a writes ceiling (the harness's), run below.

### 1.5 The census libraries and other interposers under D-rebind+J

Each read today (`readlink -f /opt/homebrew/opt/<name>`): libuv 1.53.0, ncurses 6.6, GLib 2.90.1, OpenSSL
3.5.9, SQLite 3.54.1, Go 1.27.3. 249's driver and snapshot (`drv.c` `1ea6fad76aa98389`, `snap.h`
`fdae15aafe0b6d2f`), extended to print the trampoline's and J's counters, plus a driver of OpenSSL's
password prompt (`EVP_read_pw_string` under a pty, interrupted by SIGINT after 300 ms). Base against
D-rebind+J, 3 runs each, one output kind per cell (`F/logs/q1-5-census*.txt`):

| library | what moved, base and J | deliveries through the trampoline | entries left | J's records |
|---|---|---|---|---|
| libuv (`uv_signal_start`, a delivery, the loop takes it) | the same lines | 1 | 0 | none |
| GLib (`g_unix_signal_add`, the loop takes it) | the same | 1 | 0 | none |
| ncurses (`newterm`: SIGINT, SIGTERM, SIGTSTP, SIGWINCH; SIGWINCH raised; `endwin`) | the same | 1 | 0 | none |
| OpenSSL (init, RAND, sha256, an SSL object) | nothing moved, both | 0 (8 slots rewritten) | 0 | none |
| OpenSSL's prompt (installs around its read, answers -2 on SIGINT, gives every action back) | the same | 1 | 0 | none |
| SQLite (control) | nothing moved | 0 | 0 | none |
| the Go archive (preemption, a recovered nil panic, `signal.Notify`, two callbacks) | the same | 136 to 145 | 0 | none |

**Beside other interposers** (`F/q1/beside/fp-shapes.c`: J's three shapes asked of the trampoline itself,
want 1, 1, 0): plain, **ASan**, **TSan**, **Guard Malloc** (`DYLD_INSERT_LIBRARIES=/usr/lib/libgmalloc.dylib`)
and **a `DYLD_INSERT_LIBRARIES` tool** (249's tracer, an `__interpose` of libsystem_kernel's entries, 47
lines): **3 of 3 each read 1, 1, 0**, each interposer counted and left unrewritten. **Under TSan the shapes
hang with no rebind at all** (`fp-shapes-tsan-norebind`, killed at 20 s): an in-handler jump is TSan's own
limit; with the raw query J keeps TSan's behaviour, that hang included, where 249's file hid it by
displacing TSan's handler.

**Beside a second rebinder** (`F/q1/beside/fp-rebind2.c`, fishhook's way, `_sigaction` slots only; 3 runs
each, `F/logs/q1-5-second-rebinder.txt`): when it rewrites FIRST (a linked dylib's constructor), D-rebind
rewrites after it and takes the slot: the handler still reaches the trampoline, and **the other tool's hook
counts 0 installs, displaced in silence**; when it rewrites AFTER D-rebind, the two chain and both see the
install (1 and 1). A cost to the other tool, not a hole in Heroes' rule.

**The goldens through the harness's own `run` form** (`heroes-next run tests/harness/main.hero --
heroes-next run <filter>`, harness and cases built on the variant's runtime; the form runs `-O2`,
`--sanitize` and `-O0`, and the `-O0` binary again under Guard Malloc, `suite_run.hero:414-426`;
`F/logs/q1-3-golden-*.txt`): base **5/5, 1/1, 15/15**; D-rebind+J `fixedbugs-2558` **5 passed, 0 failed**,
`recovers-leaves-no-line` **1 passed**, `fixedbugs-2494` **13 passed, 2 failed**. The two
(`...-a-handler-that-allocates-inside-the-allocator-...`, `...-lends-inside-the-lend-tracking-...`) pin the
allocator's and the lend record's guard, which their headers call *"the ground R2 leaves to this guard"*;
under J the thunk's first line refuses first, with R1's words, which are **true** of them (built by hand,
5 of 5 at `-O0` and `-O2` each: base reaches the allocator's guard, J the thunk's,
`F/logs/q1-3-golden-2494-two.txt`). **Their `.expected` moves on Darwin under J: owed by the lane, not a
defect.**

### 1.6 Forward compatibility: what this Mac's SDK settles, and what it hands the historian

Settled by the SDK (Xcode's `MacOSX.sdk`, 27.2): `mach-o/loader.h` defines every structure the rewrite
reads, `MH_DYLIB_IN_CACHE 0x80000000` (243), `S_NON_LAZY_SYMBOL_POINTERS` and `S_LAZY_SYMBOL_POINTERS`
(517, 519), `INDIRECT_SYMBOL_LOCAL` (1085), `LC_DYLD_CHAINED_FIXUPS` (343); `mach-o/dyld.h:74` declares
`_dyld_register_func_for_add_image` `__API_AVAILABLE(macos(10.1))` with **no deprecation mark**; `man dyld`
documents `DYLD_INSERT_LIBRARIES`. **Measured on today's format**: a program built here (`minos 27.2`,
`sdk 27.2`, arm64, not arm64e) carries `LC_DYLD_CHAINED_FIXUPS` and its slots in a `__got` section the
indirect symbol table still names, and the rewrite finds them (the census's counts, 1 to 8 slots per
program). **Not settled here, for the historian, by name**: whether Apple makes third-party binaries
arm64e (an `__auth_got` slot holds a signed pointer, and an unsigned write would fail its check); whether
dyld keeps the slots writable by `vm_protect(VM_PROT_COPY)` after binding (249 passed the hardened runtime,
`codesign -o runtime`); whether `_dyld_register_func_for_add_image` is deprecated; and the CI's `macos-26`
leg, run by nobody before the push.

### 1.7 What route J closes on Darwin, and its residue, in words beside route K

**Closed, run**: a Heroes function a C handler calls after a jump WITHIN that handler (four shapes, 160 of
160 refused), through a handler built without frame records as a static archive or a two-level dylib (80 of
80), through a frame register holding the context (`kmiss`'s twin, 40 of 40), and in the storm that dies by
SIGKILL in silence at base (25 of 25 refused instead); with no false refusal across the twelve depth
programs (480 runs), the 2558 goldens and recovers-leaves-no-line; nothing moves in six census libraries
and the Go archive; both forked children stay async-signal-safe; `--sanitize` reaches `main` again.

**Residue under D-rebind+J, each run or named unrun**:
1. **A jump OUT of a handler to a buffer J holds no record of** (one copied by `memcpy`, run; a save reached
   by `dlsym`; a save inside a shared-cache framework, 52 distinct images import one; a 4,096-record table past its
   eight probes; a context `makecontext` re-aimed): **a correct program refused with R1's false words**
   (40 of 40 for the `memcpy` shape). Never silent.
2. **D-rebind's own, from 249, unchanged by J**: an installer reached by `dlsym`, an installer inside the
   shared cache (51 cache images import `_sigaction`, 99 `_signal`), a raw `__sigaction` naming its own
   trampoline; there the walk of eight frame records is the only guard. **Not re-run today.**
3. **An interposer's own installs** (an image with `__interpose`, now left unrewritten) are not
   trampolined after registration; unrun.
4. A C++ exception or `_Unwind_*` out of a handler leaves its entry: a later correct callback refused;
   inferred from the code, unrun. TSan with an in-handler jump hangs with no rebind at all (TSan's own).
5. **Windows**: no walk, no probe, no route; owed. **x86-64 Darwin**: J compiles (`-arch x86_64`), unrun.

**What it needs**: no spec sentence (§ 13's *"nor called by one"*, line 404, stands); **new refusal words
for residue 1**, since J knows a miss kept the entry (`hero_j_count(2)`): *"... C left a signal handler by a
jump this runtime did not see saved, so this thread still reads as inside it ..."*, where R1's words are
false; no new diagnostic class (J defines no C library name on Darwin, every rewrite is by slot, so panel
239's `ffi_defined_elsewhere` does not widen here; its three new global names are `hero_`-prefixed); **no
change a binding's author must learn** (the three shapes' C compiled and linked unchanged). **2522 does not
close on Darwin by J alone**: residues 1 and 2 stand, measured or carried, and the proposal closes it only
on no residue.

### Q1, the verdict

- `verdict`: **approve** route J on Darwin, built as **D-rebind+J with my two repairs** (an image holding
  `__interpose` left unrewritten; the re-install asking the kernel raw), **object** to closing 2522 on it and
  to R1's words on its miss.
- `section`: design.md §1.12 (*"a Heroes program must not segfault ... the boundary is complete"*), §1.11,
  §4.19.
- `experiment`: `F/q1/rebind-j.c` (222 lines, `74f666a3998d6012`), `clang -Wall -Wextra` clean at `-O0` and
  `-O2`, arm64 and x86-64; the three binding shapes compiled unchanged and run 20 of 20 per cell under base,
  D-rebind and D-rebind+J (§ 1.3); the miss both ways (§ 1.2); 1,800 forked children per variant (§ 1.4).
- `argument`: J closes, on this Mac, every shape 249 left open inside libraries that save and jump through
  the C library's own names: 160 of 160 in-handler jumps refused, no false refusal in 480 depth runs, the
  census unmoved, ASan and TSan intact once repaired. Its miss errs the sound way: dropping on a miss ran
  Heroes inside the signal, 40 of 40. But a miss refuses a correct program with words that blame a signal
  no longer running: a false message, `blocking`. And D-rebind's `dlsym` installer and cache-framework
  installer remain, so "closes only on no residue" is not met on Darwin.
- `prediction`: under D-rebind+J, a library that copies its `sigjmp_buf` before jumping out of a handler
  (`F/q1/cases/miss.c` HOW 2) is refused 20 of 20 at `-O2` on the CI's `macos-26` leg, and the same with the
  miss-specific words prints no *"while handling a signal"*.
- `condition`: I would approve closing 2522 on Darwin when a miss is told in its own words (run on HOW 2),
  and when an installer reached by `dlsym(RTLD_DEFAULT, "sigaction")` is either reached (for example the
  `_dlsym` slot rewritten to answer the runtime's own) or run and shown refused. A Darwin with arm64e
  third-party slots would turn this into a veto of the rewrite itself.

## Question 2: what C does at the end of `main` with a thread running

### 2.1 In C alone (`F/q2/c/fp-q2.c`; `F/logs/q2-1-{darwin,linux}.txt`)

A worker inside `malloc`/`free`, inside `printf`, holding a mutex an `atexit` handler also takes, and (Darwin)
a dispatch queue's block, while `main` returns or calls `exit(0)`; **200 runs each** under `timeout -s KILL
60`, output through `head -c 65536`. **This Mac: 1,600 runs, all exit 0, no torn line, nothing on stderr.
Linux arm64 (glibc 2.41, clang 22.1.8): 1,200 runs (no libdispatch there), the same.** C ends a running
thread with the process, in silence, both ways. (My first run read `141` for every printing worker: the
64 KB bound itself, `head` closing the pipe; the workers were slowed under it and run again.)

### 2.2 Through the runtime, base, R and E (my own builds, `F/rt/{R,E}`)

R and E as I built them: `spawn.c` writes state 2 where a thread's body ends (under the table's lock;
`spawn.c:222` names it and no line wrote it), and a new runtime function `hero_thread_unjoined(&running)`
counts the slots not free under the table's lock; the gate asks it **first**. R aborts naming how many; E
says how many it ended and reads no balance. 14 programs (the writer's ten and four of mine: a worker
printing, one allocating, one inside `hero_net_accept`, one beside an `atexit` callback bound as spec § 13
shows), **200 runs for eight, 20 for six** (`F/q2/rt/one.sh`, `F/logs/q2-2-runtime.txt`):

| program | base | **R** | E |
|---|---|---|---|
| `unjoined`, `t1`, `unjoined_str` (a worker holding a `str`) | 134 *"heap blocks still live ... compiler bug"* (a false message) | **134, "1 thread(s) never joined ... 1 still running"** | 0, its line |
| `fp-alloc`, `fp-atexit` | 134, *"5 or 6 heap blocks ... compiler bug"* | **134, told** | 0, its line; the `atexit` callback's line printed, 3 of 3 read |
| `fp-net` (blocked in `accept`, holding the listener) | 134, *"1 C handle(s) never given back"* (false of the program) | **134, told** | 0 |
| `cr-lease` | 134, *"1 lease(s) never ended"* (false) | **134, told** | 0 |
| `cr-late`, `unjoined_noheap`, `t3` | **0, in silence** | **134, told** | 0, its line |
| `finished_unjoined` | 0 | 134, *"0 of them still running"* | 0, *"1 more had finished"* |
| `t2-trap` (both threads print) | 0 or 134 *"compiler bug"*; 2 torn of 200 | 134 (5 torn of 200, defect 2582's shape) | 0 (5 torn) |
| `exit_running` | 0 | 0 (`exit` skips the gate) | 0 |
| `fp-print` slowed under the bound | 0 (1 line cut: `worker ` without its number) | 134 (1 torn) | 0 |

**No status but 0 and 134 on any route** once my bound stopped biting. `leaks --atExit` here
(developer mode enabled, no wait) reads **0 leaks** for E's running threads, 9 of 9
(`F/logs/q2-3-leaks-darwin.txt`).

### 2.3 Linux arm64 under `--sanitize` (`F/logs/q2-3-linux.txt`)

The frozen tree fed by `git archive`, the **seed** built in the container (not stage 2: a premise lost,
below), each program `--sanitize -O0`, `ASAN_OPTIONS=detect_leaks=1`, 10 runs: base reaches its gate's false
*"compiler bug"* line and LeakSanitizer never runs; R is told; **under E, LeakSanitizer reports 0 leak
records with the thread still running, 40 of 40: it reads a running thread's blocks as reachable.** `fp-net`
did not build with the seed there (`error[ffi_missing_link]: the linker cannot find hero_net_accept`): owed
with a Linux stage 2.

### 2.4 Q2's C, and what moves

R's door is `int64_t hero_thread_unjoined(int64_t *running)`, inside `spawn.c`, never in `hero_os.h`
(`diff` of `hero_os.h`: unchanged); both runtimes pass `clang -fsyntax-only -Wall` here and built every
program in Linux arm64. **No binding moves**: `examples/threads/main.hero`'s three doors unchanged, its
output byte-equal under base, R and E, exit 0, 5 of 5 each (`F/logs/q2-4-example-threads.txt`). **Rebel's
`threads`** (the writer's two programs, 10 runs each, `F/logs/q2-4-rebel-threads.txt`): under R both are told
in the runtime's words before Rebel's own count is read (*"never joined"*, where Rebel says *"waited for"*);
**under E Rebel's own rule, *"a thread never waited for stops the program"*, is silenced, exit 0**, because
E reads no balance while a slot is unjoined.

### 2.5 A worker that never ends, and the one shape that decides between R and E

Blocked in `accept` (`fp-net`) or never ending (`t3`): R 134, E 0, as above; C's `abort` and `exit` both end
a thread blocked in a system call. **A worker blocked in `print` because stdout is a pipe nobody reads**
(`fp-quiet`: only the worker prints; `F/logs/q2-5-blocked-print.txt`, the program's own `time`):

| route | status | time | stderr |
|---|---|---|---|
| base | 141 (SIGPIPE when the reader went) | 15 s | nothing |
| R and E as first built (the gate's `fflush(stdout)` first) | 141 | 15.00 s | **nothing** |
| **R2** (stdout flushed only where `ftrylockfile` takes it) | **134** | **0.22 s** | **its line** |
| E2 (the same flush) | 141 | 15.00 s | its line, then C's own `exit` waits on the lock |

The worker holds stdout's lock inside a blocked `write`; any `fflush(stdout)` waits for it, so does C's
`exit`. **R can keep its promise in C** (`abort` flushes nothing); **E cannot**, short of `_exit`, which skips
the `atexit` handlers spec § 13 itself shows and the runtime's own `hero_term_at_exit` (`hero_term.c:170`).
**And today's runtime has the same hole outside this question**: `hero_panic` flushes stdout first
(`panic.c:59`), so an ordinary panic on `main` (an index past the end) while a worker is blocked writing a
stalled pipe **never says its line, 3 of 3**, hanging until the reader goes; with stdout to `/dev/null` the
same panic speaks at 0.22 s, 3 of 3. A defect beside 2552, for the coordinator to file.

### Q2, the verdict

- `verdict`: **approve R**, with its gate flushing stdout only where no thread holds it (R2); **object to E**.
- `section`: design.md §1.12; Part 7 item 13 (`docs/design.md:4085`).
- `experiment`: `F/q2/c/fp-q2.c` (C alone, 2,800 runs on two platforms), R and E built into the frozen
  runtime (`F/rt/R2`, `F/rt/E2`), 14 programs, 200 or 20 runs each, the blocked-pipe case timed.
- `argument`: C already ends running threads silently with exit 0, 2,800 of 2,800, so E adds a line to what
  C does and inherits its hole: E's program cannot end while a worker is blocked holding stdout's lock (15 s,
  then SIGPIPE), and it silences every balance, Rebel's own documented rule among them. R tells the program's
  mistake in true words where base tells a false *"compiler bug"* or nothing, and keeps that promise in the
  blocked case once its flush is a try-lock. No binding and no door's type moves.
- `prediction`: under R2, Rebel's `bw_unwaited_done` (a finished thread never waited for) is refused 10 of 10
  on the CI's Linux x86-64 leg with *"0 of them still running"*, and `examples/threads` stays exit 0.
- `condition`: I would accept E if its ending were run to keep its word with a worker blocked inside stdout's
  `write` (the program ending within a second, its `atexit` handlers run), and if it kept a handle's and a
  mailbox's own rules where no thread is running.

## Question 3: any value across, at the boundary

### 3.1 A handle moving into a thread (`F/q3/move/fp-move.c`; `F/logs/q3-1-*.txt`)

Opened and used on thread A, then used and closed on thread B, **20 runs each, this Mac and Linux arm64,
plain and under TSan**: **SQLite** (3.54.1 here, 3.46.1 there, default mode serialized, `threadsafe 1`),
**libcurl** (8.23.0, 8.14.1, an easy handle performed on both), **OpenSSL** (3.5.9, 3.5.7: an `SSL` mid-handshake
on memory BIOs, resumed on B, freed there): **every run exit 0, every answer as on one thread, no TSan
report.** Their documentation, read where installed: libcurl's `libcurl-thread(3)`, *"You can pass the
handles around among threads, but you must never use a single handle from more than one thread at any given
time"*; SQLite's `sqlite3.h:1856-1896`, single-thread mode *"can only be used by a single thread"* (a
`SQLITE_THREADSAFE=0` build or a config call), multi-thread safe *"as long as no two threads attempt to use
the same database connection at the same time"*; OpenSSL's `openssl-threads(7)`, *"most objects are not
safe for simultaneous use"*, nothing against a move. **What does not move, run**: OpenSSL's error queue is
the thread's (`ERR_get_error(3)`: *"from the thread's error queue"*; B saw none of A's error, 80 of 80); and
**a held lock**: an error-checking mutex locked on A and unlocked on B answers `EPERM`, 20 of 20 on each
platform (`F/logs/q3-1-lock-*.txt`; a default mutex is undefined behaviour). **So yes, a mark is needed**, and
it is **a change a binding's author must learn**: a handle that may not cross (a lock held, a connection of a
single-thread SQLite, a GUI object) is known only to the binding's author, from the library's page.

### 3.2 A mailbox in C, as the handle the proposal makes it (`F/q3/mailbox/fp-mailbox.c`)

The handle is **a slot's index and its generation, never an address**; slots, locks and conditions are never
freed, only messages; a receive waits in 20 ms slices that read a stop flag (the stop may come from a signal
handler, so it is a flag and never a lock); a close wakes every waiter. **32 senders and one receiver,
1,000 messages each, and four other cases, 20 runs each, plain, ASan with UBSan, and TSan, this Mac and
Linux arm64** (`F/logs/q3-2-*.txt`): 32,000 of 32,000 received, 0 out of order per sender; **a send through
a copy after the close: told and aborted, 134**; **a close while another thread waits in a receive: the
receive answers *closed***; a stop: *stopped*; a mailbox nobody closed: told at `main`'s end, 134; **no
sanitizer report in the 400 sanitized runs (600 in all)**. **Windows**: `zig cc` 0.15.2, `x86_64-windows-gnu` and `aarch64-windows-gnu`,
exit 0, no warning (SRWLOCK, CONDITION_VARIABLE, CreateThread); **run owed**. The first Windows compile
failed on `windows.h`'s `MB_OK` macro taking my enum's name, the shape the runtime's macro guard exists for.
**What the proposal's text leaves unsaid**, found writing it: what a receive already waiting answers at the
close (I chose *closed*; the spec names `timeout` and `stopped` only), and what becomes of messages queued at
the close (freed here; a message carrying a handle whose life moved into it would leave that handle never
released, inferred, unrun).

### 3.3 `ptr` and `cstr` (`F/q3/ptr/`)

The language refuses the direct way: `p.to_i64()` is `error[bad_operand]: to_i64 takes an integer or a
float, found ptr`. **A two-line header passes it anyway**: an address to an `i64` in C, the `i64` into
`hero_thread_spawn`, back to an address in the worker, which reads the C cell, **20 of 20, `42`**
(`F/logs/q3-3-launder.txt`). **In the tree, of the 38 files that name a thread door, none binds such a
conversion, and one binds the platform layer's handle-as-number door** (the golden
`the-platform-layer-hands-a-connection-to-a-thread-as-a-number`, `hero_net_number`/`hero_net_of`, which
Rebel's `net` binds too). So the proposal's refusal costs no program in the tree, and it cannot be enforced
at the boundary: C turns an address into a number, and that stays the binding's word, as § 3's `ptr`
sentence already says.

### 3.4 C's own `send`, `close` and `wait`

**A premise found false**: of the brief's 30 `extern` members named `close`, **26 sit in
`docs/panel/178-reports/`**, a sitting's evidence, not a program anything builds; the programs hold **4** (3
`check` goldens, 1 `run` golden), and **0** members named `send`, `wait`, `receive`, `spawn` or `mailbox`
(`F/logs/q3-4-names.txt`). **But the grammar decides the cost**: `Member = "function" ident ...` (spec line
486) makes one name both the Heroes name and the C symbol, and no rename exists. A built-in's name on a
member is refused today (`function len(s: cstr lent) -> u64` in an `extern` group:
`error[builtin_name_taken]`, run). **So under spelling B, C's `close(2)`, `send(2)` and `wait(2)` have no
spelling a binding can write**: `bind_close.hero`, exit 0 today, would be refused while correct, and the one
route left is a C header of the program's own (`posix_shim.h`, `static inline int fd_close(int fd)`, built and
run, exit 0). **In those words, a change a binding's author must learn: under B, three POSIX calls need a C
file the author writes by hand.** Under M their names stay free and nothing changes for a binding.

### Q3, the verdict

- `verdict`: **veto spelling B**; from the boundary **approve the proposal's shape under spelling M**, on
  two conditions: a mark a binding's author writes for a handle that may cross, and the mailbox's handle
  never an address.
- `section`: design.md §1.12 (*"any C library must be bindable, because a library Heroes cannot reach is C
  code the author has to keep writing by hand"*), §1.11, §4.19; the mark is not covered by design.md.
- `experiment`: `bind_close.hero` (C's `close` bound by its own name, exit 0 today) against the grammar's
  `Member` and today's `builtin_name_taken` on `len`; the shim `posix_shim.h` it would force, exit 0;
  `fp-move.c` and `fp-lock.c` on two platforms; `fp-mailbox.c` on two, compiled for Windows.
- `argument`: Under B a correct binding of C's own `close`, `send` or `wait` is refused and has no spelling
  without a C file: §1.12's definition of the boundary failing, for three POSIX calls. M costs bindings
  nothing. The values crossing whole and the mailbox are sound in C as built, provided its handle is an
  index with a generation. A handle crossing by moving its life is right for SQLite, libcurl and OpenSSL
  (run) and wrong for a held lock (EPERM, run), and only the binding's author knows which a handle is, so the
  permission must be the binding's mark, refused by default.
- `prediction`: under M, `tests/golden/run/fixedbugs-396-posix-fills-its-buffers.hero`'s `close(fd: i32) ->
  i32` member builds unchanged; under B it is `error[builtin_name_taken]` and no edit to the `.hero` alone
  makes it build.
- `condition`: I would lift the veto on B if the grammar gained a member rename (a Heroes name for a C
  symbol), measured to bind C's `close`, `send` and `wait`; or if B's names stayed free for `extern` members
  alone, measured not to collide with the built-ins at a call.


## Owed, unrun, and what the coordinator may file

**Windows, owed on every question**: no route of Q1 has a walk or a probe there; R and E unbuilt there; the
handle move unrun; the mailbox compiled (x86-64 and arm64 by `zig cc`) and **not run**. **Linux**: Q1's J is
the compiler-engineer's (B'); `fp-net` under `--sanitize` there needs a stage 2. **Unrun on Darwin**: J on
x86-64 (compiled only), an interposer that installs its own handler after registration, a C++ exception out
of a handler, D-rebind's `dlsym` installer and cache installers (249's rows, not re-run), the CI's `macos-26`.

**Defects found, for the coordinator to file (I file nothing)**:
1. **A runtime panic never says its line while another thread is blocked writing stdout** (`hero_panic`'s
   `fflush(stdout)`, `panic.c:59`): 3 of 3 on the frozen runtime, the program hanging until the reader goes,
   then SIGPIPE; the repair measured on the gate is a try-lock flush (R2, 0.22 s, 134). By
   `.claude/rules/verification.md` § Bounded discovery a told failure that never tells reads `blocking`.
2. **D-rebind as 249 built it displaces TSan's signal handler** (the kernel held the program's handler):
   repaired in `F/q1/rebind-j.c` by the raw query.
3. **D-rebind as 249 built it kills every `--sanitize` program before `main`** (carried as 249's, reproduced
   12 of 12): repaired by leaving interposing images unrewritten.
4. **Under J, a miss tells a correct program R1's false words** (40 of 40): owed new words.
5. Two 2494 goldens' `.expected` move on Darwin under J (true, earlier refusal), owed by the lane.

**Every number above was run today in `F/`; nothing is carried but what says *carried*.** I wrote no
file outside my folder and this report; Go and zig ran with `HOME`, their caches and `TMPDIR` inside `F/`;
six containers `fp-251-1` to `fp-251-6`, each gone after (`docker ps` read empty); no paid run, no
`heroes test`, no commit.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/251-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
