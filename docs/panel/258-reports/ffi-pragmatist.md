# Panel 258, the ffi-pragmatist's report (2026-11-24)

Seat: ffi-pragmatist, soundness lane. Base `a23ea157a`, worktree `.claude/worktrees/258-ffi-pragmatist`.
Compilers: the seed by clang (`real 22.46`), then stage 2 (`./heroes build selfhost/main.hero -o heroes-next`,
`real 397.85 user 223.06 sys 52.17`, load about 70: no duration below is leaned on). **Written as I go; a
section marked *in progress* was not finished when the file was last copied.**

The counter: a scratch copy of `runtime/` (`HEROES_RUNTIME=<copy>`, never the tree) whose jump wrappers
write one line to fd 2 per jump that reaches them (`HERO258 door=<wrapper> seen route=<C|A> standing=<0|1>
inside=<hero_callbacks_inside>`), `sigjump_save.c` on Linux and `rebind_jump.c` on Darwin, switched on by
`HERO258=1`. The door programs (`doors.h`, `door.hero`, `door_spawn.hero`, `cx258.cpp`, `cxdoor.hero`) are
reproduced in § Reproducers. Each callback holds a ~1 KB string and a 50-element array, as `leak/main_1k.hero`.

## Verdict

**Object to (c) as specified, and to (d) as specified: a release AT the jump. Veto on (g) as the sitting's
resolution.** Support (a') now, in its spec-free form, and as the resolution **"collect at the jump, release
at the return"**: (d)'s forced unwinding as the collector, every owned value moved onto a per-thread list
hung off the thread key instead of being released, and the list released when the extern call holding the
landing returns into Heroes code ((h)'s point); handles never consumed by the runtime. On Windows the
collector is a `__finally`, which UCRT's `longjmp` runs by itself (§ 3, run on the box: 1,000 of 1,000).

**Section**: design.md §1.11 (the FFI is every package's door; *"the glue we refused to write"*) and §1.12 (a
program must not corrupt memory; the boundary is complete); the spec's *"A lend lives for its call and no
longer"* and the lease and handle gates (`spec/heroes-spec.md` lines 465-475).

**The cost it names**: a release before the extern call holding the save point returns frees what C still
reads at the catch site. Measured: a lent message freed at the hook makes a plain build report an empty
message with exit 0, and ASan a heap-use-after-free (§ 4 mode 2); the interim reports it correctly
(`pngmsg_lent`, 3 of 3). That converts a leak into a silent wrong answer, which rank 3 forbids and the
interim cannot do. (g) breaks every binding shape measured in § 2 at its first jump.

**Spec**: yes. The resolution I support decides when a lend and a lease of a frame a C jump left end
(neither lines 465-466 nor line 470 says), so it needs a sentence, **which sends the sitting to the full panel**.
(a') in the form I support needs none: the handle, lease and thread gates keep the abort lines 473-474
specify and only tell the truth; forgiving them would need one.

**What my veto compels**: (g) not adopted; a jump over a callback stays legal for a binding. **The conservative
resolution would have been (a')**: every balance told the truth, every leak kept (§ Conservative).

**Prediction** (one, falsifiable): § Prediction.

## 1. The doors

### Darwin arm64, this Mac, stage 2, run

Three jumps per door with the counter on (`HERO258=1 DOOR=<k> DOOR_N=3`), then `leaks --atExit` at 1,000
(`DOOR_N=1000`, counter off). `inside` is the thread's count of callbacks entered and not left, read at the
jump: it climbs 1, 2, 3, which is the count defect 2704's interim reads at exit.

| door | seen by the hook | route | exit, 3 jumps | `leaks --atExit`, 1,000 jumps |
|---|---|---|---|---|
| 1 `longjmp` | yes, 3 of 3 | C (decoded), standing 0 | 0, `sum 21` | 7,000 leaks, 3,952,000 bytes |
| 2 `_longjmp` | yes, 3 of 3 | C | 0 | 7,000, 3,952,000 |
| 3 `siglongjmp`, mask saved | yes, 3 of 3 | C | 0 | 7,000, 3,952,000 |
| 4 `siglongjmp`, no mask | yes, 3 of 3 | C | 0 | 7,000, 3,952,000 |
| 5 `__longjmp_chk` | n/a on Darwin (glibc's name); the door ran `longjmp` | C | 0 | not run again |
| 6 `__builtin_longjmp` | **cannot be written**: clang 22 refuses `__builtin_setjmp` for `arm64-apple-macos` and `aarch64-linux-gnu` ("not supported for the current target"), accepts it for `x86_64-linux-gnu` and `x86_64-pc-windows-msvc` | | | |
| 7 `setcontext` to a `getcontext` point | yes, 3 of 3 (`door=setcontext`) | Darwin's wrapper prints no route | 0, `sum 21` | 7,000, 3,952,000 |
| 8 `swapcontext` away, coroutine never resumed, its stack freed | the two swaps are seen (6 lines for 3 rounds); **the abandonment is not an event**: the `free` of the stack is (D5) | | 0, `sum 21` | 7,000, 3,952,000 |
| 9 `pthread_exit` in a callback, main thread | **no** | none | **0, nothing printed**: the main thread ends, the process exits 0, `main`'s `print` never runs | 0 leaks (the process ends at the first jump) |
| 10 `pthread_cancel(pthread_self())` + `pthread_testcancel` | **no** | none | 0, nothing printed | 0 |
| 11 `exit(0)` in a callback | no (no jump) | | 0, nothing printed | 0 |
| 12 a `SIGUSR1` handler that returns with its register file set to a `getcontext` point (libsigsegv's shape) | **no** | none | 0, `sum 21` | 7,000, 3,952,000 |
| 13 a C++ exception (`throw 13`, a `-O2` dylib, `catch (...)` in C++ under the callback) | **no** | none | **0, `sum 21`**: the exception unwinds through the Heroes frames (compact unwind) and is caught | 7,000, 3,952,000 |
| spawn + `longjmp` | yes, 1 of 1 | C | 0, `answer 107` | |
| spawn + `pthread_exit` (9) and + cancel (10) | **no** | none | **134**, `a null pointer was read through, at offset 0x38, called from h_0take_1b9a87` at `t.wait()`, 3 of 3 each | |

**Read**: on Darwin the hook sees exactly the setjmp family and the context calls a rewritten slot reaches.
Three doors abandon the same 7 blocks a jump and never reach it (12, the register file edited in a handler;
13, a C++ exception; 8, a coroutine dropped), and two end the thread with no event at all (9 and 10; on the
main thread the process ends, in a `spawn` the answer is a null read). A resolution hung on the hook (c),
(f), (g) covers the first family alone; (d) covers 13 by itself (§ 4's prototype); (e) (detection by stack
pointer at the next thunk entry or extern return) is the only one that would notice 12 and 8 at all, after
the fact.

### Linux arm64, glibc 2.41, clang 22.1.8, the container `heroes-linux-arm64:latest`, stage 2 built inside it, run

Same programs, same counter (`sigjump_save.c`'s copy).

| door | seen | route | exit, 3 jumps |
|---|---|---|---|
| 1 `longjmp`, 2 `_longjmp`, 3 and 4 `siglongjmp` (mask, no mask) | yes, 3 of 3 each | C, standing 0 | 0, `sum 21` |
| 5 `__longjmp_chk`, called by name | yes, 3 of 3 (`door=__longjmp_chk`) | C | 0 |
| 5' **a shared library built `-O2 -D_FORTIFY_SOURCE=2`** (`libjl258.so`, `nm -D`: `U __longjmp_chk@GLIBC_2.17`), linked by `link "jl258"` | **yes, 3 of 3**: the executable exports `longjmp`, `_longjmp`, `siglongjmp`, `__longjmp_chk`, `setjmp`, `_setjmp`, `__sigsetjmp` (`nm -D`, type T), and the library's versioned reference binds to them | C | 0, `sum 21` |
| 6 `__builtin_longjmp` | cannot be written on arm64 (above). **On x86-64 it can never be seen**: `clang --target=x86_64-linux-gnu -O2` (and `x86_64-pc-windows-msvc`) compiles it to four instructions, `rbp`, the target and `rsp` loaded and an indirect `jmp`, and the object's only undefined symbol is the callback (`nm`, `objdump`): no call exists to interpose | | |
| 7 `setcontext` | yes, 3 of 3 | C | 0 |
| 8 coroutine dropped | the swaps seen (6), the abandonment not | C | 0 |
| 9 `pthread_exit` (glibc unwinds by force through the Heroes frames, which hold no cleanup: § 4's (d) table) | **no** | | **0, nothing printed** |
| 10 cancel self | **no** | | 0, nothing printed |
| 11 `exit(0)` | no | | 0, nothing printed |
| 12 the register file edited in a `SIGUSR1` handler | **no** | | 0, `sum 21` |
| 13 C++ exception through the frames | **no** | | 0, `sum 21` |
| spawn + 1 | yes | C | 0, `answer 107` |
| spawn + 9, spawn + 10 | no | | **134**, the same null read at offset 0x38 in `wait`, 3 of 3 (defect 2754's row, and its cancellation twin) |
| **route A forced** (`HERO_JMP_FORCE_UNCALIBRATED 3` in a second copy), doors 1, 3, 5, 7 | yes, 3 of 3 each | **A**, standing 0 | 0, `sum 21` |

**Route A knows no landing address.** Its record is the buffer's address, the innermost handler's serial, a
fingerprint and a size (`sigjump_record.c:131-137`, read; the module doc: *"never an address on a stack"*).
`sigjump.c:47`'s sentence *"the save points record which buffer they filled and the frame of the call"* is not
what `hero_jmp_note` stores: **a stale comment, read, not run**. So on route A (glibc's other four
architectures, a glibc whose layout moved) a hook that releases (c, f) has no target to walk to, and a save
it never saw (`dlvsym`, a buffer refilled by `memcpy`) has no record at all.

## 2. The binding shapes that put a Heroes frame under a library's jump

All on this Mac, libpng 1.6.59 (`pkgconfig "libpng"`), stage 2, the counted runtime. Images: panel 257's
`good.png` (180 by 180 RGBA, one IDAT of 2,149 bytes) and `trunc.png` (200 bytes), and `badcrc.png`, `good.png`
with one bit of the IDAT's zlib stream flipped (byte 334).

### Shape C, the one the first brief should have named: the program drives libpng inside ONE save point

`pngc.h` is the whole C a binding needs: `pg_guard(decode)` is `if (setjmp(png_jmpbuf(p))) return -3; return
decode(0);`, five lines, and the program binds `png_read_info`, `png_get_image_height` and `png_read_row`
**directly in its `extern` group** (handles `record Png tag png_struct_def`, `record PngInfo tag png_info_def`;
clang accepted all three against the real `png.h`, build exit 0). Its decode holds a ~1 KB string and an
array of row numbers. **No program-initiated error anywhere**: libpng's own error on the input jumps.

| image, 3 decodes, counter on | exit | hook | libpng says |
|---|---|---|---|
| `good.png` | 0, `decoded 3, libpng jumped 0` | 0 lines | |
| `badcrc.png` | 0, `decoded 0, libpng jumped 3` | 3 of 3, `longjmp`, route C | `IDAT: incorrect data check` |
| `trunc.png` | 0, `jumped 3` | 3 of 3 | `Read Error` |

| 1,000 decodes, `leaks --atExit` | exit (leaks) | leaks |
|---|---|---|
| `good.png` | 0 | **0 leaks** |
| `badcrc.png` | 1 | **8,000 leaks for 7,856,000 bytes**, `jumped 1000` |
| `trunc.png` | 1 | **7,000 leaks for 5,296,000 bytes**, `jumped 1000` |

**What a service using this shape loses**: about 7.9 KB and 8 blocks per corrupt upload at this callback's size,
forever, and the exit audit says nothing (the interim). It scales with what the decode holds at the error,
which for a real decoder is the rows decoded so far: a 4,000 by 4,000 RGBA image failing at its last row
abandons 64 MB if the program keeps its rows in an array (arithmetic, unrun). **This shape needs no mistake
by the program, and it is the cheapest binding to write** (one shim for the whole library, against one
shim with its own save point per function that can reach `png_error`, a set this seat did not count): so it is the shape a binding written to
§1.11's taste is most likely to take.

### Shape A: a read callback that stops the decode with `png_error` (panel 257's `pngread.h`, unchanged)

libpng's read function returns `void`; **the only way to stop a decode from inside it is `png_error`**. The
program's callback is an upload service's byte budget: it holds the chunk it read (a ~64 KB string) and calls
`pr_fail()` on the third call. 1,000 decodes of `good.png`: exit 0, `refused 1000`, the hook seen 1,000 of
1,000 (route C); `leaks --atExit`: **5,000 leaks for 163,952,000 bytes**, about 164 KB a refused upload.

### Shape B: the progressive reader, its callbacks calling back into libpng

`pngb.h` feeds `png_process_data` 512 bytes at a time, as a network reader does. The program's info callback
calls back into libpng (`png_get_image_width`, `png_get_image_height` through `pb_width`, `pb_height`) and
refuses an image past a pixel budget with `png_error` (the decompression-bomb check; `pb_reject`); its row
callback calls back into libpng's `png_progressive_combine_row`. 1,000 refused and 1,000 accepted decodes:
exit 0, `refused 1000, decoded whole 1000`, hook 1,000 of 1,000; **5,000 leaks for 2,688,000 bytes**, every one
from the refusals (the accepting path abandons nothing).

### Lua, a watchdog, and what is likely to be written

- **Lua: unrun.** No `lua.h` on this Mac (`/opt/homebrew/include`, `pkg-config --list-all`) nor in the
  container; nothing was downloaded. Read, not run: every `luaL_check*` and `luaL_error` raises by `longjmp`
  (Lua built as C), so **a Heroes function registered with Lua that checks its arguments abandons its frame on
  every bad argument a script passes**. It is shape A with the error raised by the library's own checker.
- **The watchdog**: `jumps/alarm.hero`, the shared brief's row (exit 134 on the false *signal handler used the
  allocator* words); not run again by this seat. § 4 shows what an asynchronous jump does to the C library's
  own allocator with no Heroes code at all.
- **Which real bindings will put a Heroes frame under a jump** (read, a list from the libraries' documented
  error models, not a measurement): libpng (shapes A, B, C), libjpeg-turbo (`jpeglib.h:784`, this Mac:
  *"Error exit handler: does not return to caller"*, so a binding that recovers `longjmp`s from it, and a
  source manager's `fill_input_buffer` is a callback), Lua (`luaL_check*`), and any watchdog. **Not**
  giflib or libtiff: `gif_lib.h` and `tiffio.h` here name no `setjmp` or `longjmp`, and libtiff's
  `TIFFErrorHandler` returns (`tiffio.h:301`); their errors come back as values. **The cheapest binding of each is shape C**: one C save point and the program's code inside it.
  No program in the tree is one today (shared brief), and Rebel's PNG is Heroes.

## 3. Windows (the runtime interposes nothing there)

**Microsoft's documentation, read 2026-11-24** (`learn.microsoft.com`, the pages *longjmp* and *try-finally
statement*, msvc-170):

- *longjmp*: *"In Microsoft C++ code on Windows, longjmp uses the same stack-unwinding semantics as
  exception-handling code. It's safe to use in the same places that C++ exceptions can be raised."* and
  *"Don't use longjmp to transfer control from a callback routine invoked directly or indirectly by Windows
  code."*
- *try-finally*: *"Exiting a try-finally statement using the longjmp run-time function is considered abnormal
  termination ... If a __try block is prematurely terminated for any reason, including a jump out of the
  block, the system executes the associated __finally block as a part of the process of unwinding the
  stack."*

So the shared brief's *"The jump runs no code"* is **false on Windows by the documentation**: UCRT's
`longjmp` unwinds, and a `__finally` between runs.

**Run on the box** (`ssh win`, under the lock: taken 02:23:24 and released 02:23:29, again 02:24:17 to
02:24:22; clang 23.1.1, `Target: x86_64-pc-windows-msvc`, which lowers the save to `callq _setjmp` and the jump
to `callq longjmp`, the UCRT's). `win/winjmp.c`: a library file saves and jumps (`winlib.c`, no SEH of its own);
between them a frame holding a 1,000-byte block, either plain or with its release in a `__finally`. 1,000
jumps a pass, `-O0` and `-O2`, three runs each:

| frame between the save and the jump | `-O0`, 3 runs | `-O2`, 3 runs |
|---|---|---|
| plain (a Heroes frame today: no handler) | 1,000 landed, **0 released**, exit 0 | the same |
| its release in a `__finally` ((d)'s Windows arm) | 1,000 landed, **`__finally` ran 1,000 of 1,000, `AbnormalTermination()` true 1,000 times, 1,000 released**, exit 0 | the same |
| a `__builtin_longjmp` over a `__finally` | **the `__finally` ran 0 times**, 6 of 6 runs; my probe's other counts on this line are garbage (its loop bound read -591,803,228, 983,040, 0), which I did not chase: only the zero is reported | |

The first pass's `-O2` binaries exited 127 three times under `timeout` though their build said 0; the second
pass ran them without `timeout` and they exited 0: unexplained, and nothing above rests on the first pass.

**Read for the sitting**: **(d)'s Windows arm exists and is the platform's own**: a `__finally` on the owned
slots of an emitted function runs when UCRT's `longjmp` crosses it, with no hook (Windows has none) and no
`-fexceptions`. It carries the same timing as (d) everywhere: the `__finally` runs during the unwind, before
the landing, so it must **collect** and not release (§ 4 mode 2). Whether a Heroes thunk's frames are crossed
the same way is an inference from this C (the emitted C is C, clang compiles both); **a Heroes program on
the box was not run** (building stage 2 there was not justified for this question).

## 4. Where a release may run, from the C side

### D7 and a lent message: a release AT the jump, against the real libpng (this Mac, run)

`hazard/d7.c` routes libpng's jump through a hook with `png_set_longjmp_fn`, the place the runtime's
interposed `longjmp` stands, and lets the hook release what the abandoned frame held before the real
`longjmp`. Each mode once plain, once under `MallocScribble=1`, once under ASan (`-fsanitize=address`):

| mode | what the hook releases | plain | `MallocScribble=1` | ASan |
|---|---|---|---|---|
| 0 | nothing (the interim) | 0, the message reported | 0 | 0 |
| 1 | **D7**: the `png_struct` itself, by its consumer `png_destroy_read_struct`, whose `jmp_buf` `longjmp` reads next | **139** (SIGSEGV) | **139** | **SEGV**, a READ |
| 2 | **a message string the callback owned and lent to `png_error`**; the binding's error function keeps the pointer (libpng passes it on uncopied) and the catch site reports it after the landing | **0, and the binding reports an EMPTY message**: a silent wrong answer | 0, empty | **heap-use-after-free**, READ of size 2 |
| 3 | mode 2's string, released at the guard's **return** to its caller, after the catch site: (h)'s moment | 0, the message reported | 0 | 0 |

**D7, the critic's literal form, is shown** (mode 1), **and refuted for a correct binding by an ordering
argument (an inference, not a run)**: the jump's target is `png_jmpbuf(P)`, so P existed when the save ran, in
a frame below every abandoned one; a frame above the save that owns P would have to have created it after
the save ran on it. A correct binding cannot write that; a binding that marks an accessor of the shim's P
`acquires png_destroy_read_struct` can, and the shim's own cleanup then double-frees P anyway.

**The general form stands, and mode 2 is it**: what a C library still reads between the jump and the return
of the extern call that holds its save point (the catch site's report, the error function's kept pointer) is
a value the abandoned frames lent to C. **A release at the hook ends a lend while C is still inside the call
that received it**, and the plain build says nothing and prints the wrong text: rank 3 of CLAUDE.md
§ Precedence, which the interim cannot reach. The only moment the FFI's contract says C has handed every lend
back is **the return of the extern call into Heroes code**, (h)'s point, which is also never inside a
signal handler (a Heroes function cannot be one; spec line 427).

### The same, in Heroes, against today's runtime (stage 2, the tree's runtime, run)

`png/pngmsg.h` keeps the message libpng's error function is given and reports it after the landing;
`pngmsg_lent.hero`'s callback hands `png_error` its own `str` lent (`msg: cstr lent`, `word.cstr()`),
`pngmsg_lease.hero`'s binding declares no `lent`, so the callback hands a lease and writes its `end_lease`
after the call, which the jump skips.

| program | plain, 3 runs | `--sanitize` |
|---|---|---|
| `pngmsg_lent` | **exit 0, 3 of 3**, `the binding reports: the program refused stream 0 at byte 4096` | exit 0, the same |
| `pngmsg_lease` | the message reported, then **exit 134, 3 of 3**: `panic: 1 lease(s) never ended — every .lease() owes one end_lease, and this program is missing that many`, `taken at pngmsg_lease.hero:10:16, in pngmsg_lease.refuse` | exit 134, the same |

**The shared brief's last row (the lease gate, "read, not run") is run here and holds**: a correct program is
accused of a missing `end_lease` its source writes. It is the handle gate's class (defect 2753), on a gate
the interim does not touch. **And these two programs are the falsifier of my prediction**: today, and under
(a') or (h), `pngmsg_lent` reports the message; under any release before the extern call holding the save
point returns, it reads freed bytes (§ 4's C mode 2 says what that looks like).

### An asynchronous jump out of the C library's own allocator (no Heroes code at all; run on both)

`hazard/sigfree.c`: a `SIGALRM` every 200 µs whose handler `siglongjmp`s out of a loop that calls `malloc` and
`free` (the allocation kept observable through a `volatile` sink: my first build let clang delete the pair,
and its four billion "allocations" were thrown away). Mode 2 releases nothing (the interim); mode 0 frees what
the abandoned frame held **after the landing**; mode 1 frees it **inside the handler, before the jump** (a
release at the hook). 20,000 jumps wanted, ten runs a mode, `timeout 40`:

| | mode 2, no release | mode 0, release after landing | mode 1, release in the handler | ASan, mode 1 |
|---|---|---|---|---|
| macOS 26 arm64 (this Mac, quiet: load about 2.5) | **10 of 10 die**: 9 exit 133 (SIGTRAP), 1 exit 137 | 9 of 10 exit 133 | 6 of 10 exit 133 | 5 of 5 exit 137 |
| Linux arm64, glibc 2.41 | **10 of 10 exit 134**, `Fatal glibc error: malloc.c:2601 (sysmalloc): assertion failed` | 9 exit 134, 1 exit 139 | 9 exit 134, 1 exit 139 | 3 exit 0, **2 deadlocked** (`timeout`, 124) |

The macOS crash reports (`~/Library/Logs/DiagnosticReports/sigfree-*.ips`, read): *"BUG IN LIBMALLOC:
ulock_wait failure"* in `_xzm_xzone_malloc_freelist_outlined`, the lock the interrupted `malloc` held and
never released; under ASan *"BUG IN CLIENT OF LIBPLATFORM: Trying to recursively lock an os_unfair_lock"* in
ASan's own allocator. An earlier, loaded pass (load 73) read 2 of 5, 2 of 5 and 1 of 5 deaths: the rate moves
with the timer's luck, the outcome does not.

**Read**: an asynchronous jump that abandons the C library's allocator destroys the C heap **whether or not
anything is released**: the pure leak dies 10 of 10 on both systems. POSIX makes every unsafe call after such
a jump undefined, and here it is measured. **No resolution of this sitting makes the `SIGALRM` row safe**; what
the runtime owes it is a true message (today's names a Heroes handler that does not exist, defect 2755) and
no release from inside a handler, which can only add a deadlock (glibc ASan, 2 of 5). **Hazard 2 settles as
out of every resolution's scope**, and (c)'s *"at every C call site"* frame is right to exclude it.

### Resolution (d), forced unwinding, prototyped in C (run on both; no Heroes compiler change)

`unwind/fu_cb.c` is a frame shaped like a callback's under (d): two owned slots under
`__attribute__((cleanup))`, the file built `-fexceptions`. `fu_main.c` is shape C's guard and a hook (libpng's
jump routed by `png_set_longjmp_fn`) that calls `_Unwind_ForcedUnwind` with a stop function landing at the
guard's frame (`_Unwind_GetCFA` above the guard's frame address, or end of stack), then `longjmp`s. The
library frames are built with nothing a Heroes program chooses. 1,000 jumps each:

| | Darwin arm64, real libpng 1.6.59 (`png_read_row` on `badcrc.png`) | Linux arm64 glibc, a stand-in `.so` with libpng's error model (container has no libpng) |
|---|---|---|
| hook calls `_Unwind_ForcedUnwind`, callback built `-fexceptions` | **2,000 of 2,000 slots released**, 1,000 landed, exit 0 (11 stop calls a jump: libpng's own frames walked) | **2,000 of 2,000**, exit 0 (7 stop calls a jump with the library's frames kept by `-fno-optimize-sibling-calls`) |
| the same, callback built WITHOUT `-fexceptions` | 0 of 2,000 (the walk passes, no landing pad) | 0 of 2,000 |
| the same, the library's `.eh_frame` and `.eh_frame_hdr` stripped (`objcopy`, 0 FDEs) | not possible on Darwin (compact unwind is the platform's) | **0 of 2,000, exit 0, every jump landed**: the walk meets the gap, the stop function gets `_UA_END_OF_STACK` and lands. **It falls back to the leak, never to a crash** |
| **a C++ exception** through the callback (door 13), no hook | **2,000 of 2,000**, caught by the C++ caller | **2,000 of 2,000** |
| **`pthread_exit`** in the callback (door 9), no hook | **0 of 2,000**: Darwin's `pthread_exit` does not unwind | **2,000 of 2,000**: glibc's `pthread_exit` is a forced unwind |
| a plain `longjmp`, no forced unwind | 0 of 2,000 | 0 of 2,000 |

What it costs the frame, one function measured (`objdump`, Darwin, `-O2`): **the entry path is the same twelve
instructions with and without `-fexceptions`**; the landing pads add 7 instructions off it (text 108 to 136
bytes), plus a 16-byte `__gcc_except_tab` and an 80-byte `__eh_frame`. The cost over a whole program, and the
compiler's own, is **unmeasured**.

**Read, for the C boundary**: (d) is the only mechanism measured here that **covers three doors with one
rule**: the hooked jumps (through the hook), a C++ exception and glibc's `pthread_exit` and cancellation (by
themselves, since they already unwind by force). It changes no C signature and no layout. It needs unwind
information in every frame between, which every arm64 build here had by default, and where a library lacks
it, it degrades to today's leak. **It does not by itself answer § 4's mode 2**: the cleanups run during the
unwind, before the real `longjmp`, so a cleanup that releases a value lent to `png_error` frees it before the
catch site reads it, exactly as (c) does. Hence the shape the verdict argues for: (d)'s cleanups as the
**collector**, each moving its value onto a per-thread list instead of releasing it, and the release at (h)'s
point. The rule binds the two doors that have no hook as well: a C++ `catch` may read a lent value as a C
catch site does, so its cleanups collect too, and a thread ended by glibc's `pthread_exit` releases its list
at the thread's end, through the thread key and never a `_Thread_local` read (D8, `alloc.c:1668-1672`).

## 5. ABI, and what a binding sees

**`HERO_RUNTIME_ABI` (32, `heroes_runtime.h:108`) moves "whenever the declarations change shape"** (its
comment, read). By mechanism, read and not built:

| resolution | new emitted runtime names (ABI moves; `heroes_guard_open.h` and `heroes_guard_close.h` must add each, the push/pop lists that keep a header's macros off the runtime's names) | `hero_os.h` (decision 2228's promise) | what a binding's C sees |
|---|---|---|---|
| (a') | none: runtime only | untouched | nothing |
| (c) | yes: the record's push and pop in every callback-reachable prologue and exit | untouched | nothing in its signatures; **a lend or a lease ends at the jump** (§ 4 mode 2) |
| (d) | maybe none (the cleanups can call today's releases); **every emitted unit built `-fexceptions`** | untouched | **`__EXCEPTIONS` is defined in the unit that compiles the binding's header** (`clang -dM -E`, both systems): glibc's `pthread.h:565` takes its cleanup-attribute branch of `pthread_cleanup_push` (read); no Darwin SDK or Homebrew header here branches on it (`grep`). Header verification is unchanged, since clang checks the header under the flags it is compiled with |
| (e) | possibly none: `hero_callback_entered` already receives the thunk's frame address (`heroes_runtime.h:131`) | untouched | nothing |
| (g) | none | untouched | **every binding of § 2 stops working** (below) |
| (h) | yes: a test at each extern call's return, or in the thunk | untouched | nothing; C keeps every lend until the call holding its save point returns |

**No mechanism moves a C signature, a layout, `str`'s NUL termination or a refcount C can see** (design.md
§1.11's list). The one binding-visible change is (d)'s macro, and the one binding-visible *hazard* is the
moment a release runs (§ 4).

**Whether a C library holding a Heroes value across the abandoned frames is made unsafe by releasing it:
yes, measured** (§ 4 mode 2, a silent wrong message in a plain build, a heap-use-after-free under ASan) for
any release before the extern call holding the save point returns; and **a lease is worse**: the spec makes
`s.lease()` *"a COPY of the bytes that C may read for as long as the program says"* (`spec/heroes-spec.md`
line 470), the program can no longer say anything once its frame is gone, and a binding that does not
declare `png_error`'s message `lent` must be handed a lease. Releasing it at the jump frees bytes the
catch site reads; releasing it at (h)'s point does not.

**Handles are a separate case and no resolution should run their consumers for the program**: at the hook
it is D7 (§ 4 mode 1); at (h)'s point a child handle (a `png_info` of a `png_struct` the catch site already
destroyed, shape A's `pr_decode` destroying before it returns) would be consumed after its parent (an
inference from the binding's order, not run). The spec makes the handle and lease gates abort *"when `main`
returns, saying how many"* (lines 473-474): **telling the truth there** (the count, and that a C jump left the
frames that took them) needs no spec sentence; **forgiving** them does.

**(g), the told refusal, and what it breaks.** Every shape of § 2 would end its program at the first jump:
shape C at the first corrupt image (`badcrc.png`, `trunc.png`), shape A at the first stream over budget,
shape B at the first oversized image, a Lua binding at the first bad argument a script passes, a watchdog at
its first timeout. The binding author's only way out is a C shim per function that can fail, each with its
own save point, so that no Heroes frame ever stands between a save and a jump: for libpng every read
function, for Lua every argument check moved out of Heroes into C. **That is design.md §1.11's "the glue we
refused to write", made mandatory**, and it is why I hold my veto against (g) as the resolution (below). Prior
art (Rust's `C-unwind` and its rule about frames a `longjmp` may cross, cgo's rule against C jumping over Go
frames, OCaml's callbacks): **unverified, named only as a question**, as the brief asks: do those languages
refuse the jump, or leave it undefined?

## Conservative: what (a') is, and what it would have been

The conservative resolution is (a'), and it is the one to build first whatever the sitting adopts: no release,
every balance told the truth. Measured or read, row by row:

| row | today | (a') |
|---|---|---|
| heap blocks | the audit off (the interim) | as today |
| the large-value room (2752) | a false `compiler bug` abort mid-run | must not abort a correct program: the room given back to the giving frame's take (a runtime matter, no spec) |
| handles (2753), leases (this report, run), threads | an abort accusing the program | the abort lines 473-474 specify, **told truly**: how many, and that a C jump left the frames that took them |
| a `spawn` thread ended by `pthread_exit` or cancellation (2754, and its cancellation twin, run) | a null read caught at `wait` | a told abort at `wait` naming the end C chose |
| the watchdog (2755) | words naming a Heroes handler that does not exist | true words; § 4 shows the C heap itself is gone, so no release can help |
| lend regions (2756) | resident memory with no gate | a leak, said in the exit audit's words |

## Prediction

**`pngmsg_lent.hero` (this report's `png/` folder, libpng 1.6.59, this Mac) prints `the binding reports: the
program refused stream 0 at byte 4096` and exits 0, 3 of 3, under today's runtime, under (a') and under a
release at (h)'s point; under any build of (c) that releases at `hero_jmp_jump`, or of (d) whose cleanups
release during the forced unwind, it prints a different or empty message, or reads freed memory, in at least
1 of 3 plain runs, and `--sanitize` reports a heap-use-after-free 3 of 3.** Checkable the day either is
prototyped in `selfhost/`: build that program with the prototype and run it six times, three of them
`--sanitize`. A clean pass under a release at the jump falsifies my objection.

## Where the briefs were false or silent

1. **"The jump runs no code"** (shared, the question): false for three doors, by run or by documentation:
   glibc's `pthread_exit` and cancellation unwind by force (§ 4's (d) table: a `-fexceptions` frame's
   cleanups ran 2,000 of 2,000), a C++ exception unwinds, and UCRT's `longjmp` runs `__finally` blocks
   (Microsoft's words; the run is § 3's).
2. **(c) "walks it from the jumping frame to the target"**: on route A there is no target. The record holds no
   stack address (`sigjump_record.c:131-137`, read); `sigjump.c:47`'s *"and the frame of the call"* is stale.
3. **The doors list** names `__builtin_longjmp` as a door to run: clang 22 refuses it on both arm64 targets
   here; it exists on x86-64 alone (`clang --target=... -fsyntax-only`, four targets).
4. **The binding shapes**: the brief names a read callback and a progressive reader, and not **shape C**, the
   program-driven decode inside one save point, which leaks on every corrupt input with no mistake by the
   program and is the cheapest binding to write.
5. **The lease row ("read, not run")**: run here (`pngmsg_lease`, exit 134, 3 of 3, false words).
6. **(a') "forgives ... or tells the truth"**: forgiving the handle, lease and thread gates contradicts the
   specification's lines 473-474; telling the truth does not. The brief did not say which needs a sentence.
7. **Hazard 2, the asynchronous jump**: settled by run (§ 4): the C heap dies whatever is released.
8. **Silent**: a release's timing against C's own reads after the landing (§ 4 mode 2, a lent message, and a
   lease). It is the strongest reason (c) and (d) as specified are wrong at the boundary.
9. **Not done by this seat**: a jump whose target is above a coroutine switch; a jump from a thread other than
   the main one beyond `spawn` + `longjmp` (seen, route C, both systems); an Objective-C or Swift exception
   through AppKit's layer (D2); a dyld-shared-cache image's jump (D3); Lua (not installed); door 6 RUN on x86-64 (only its `objdump`, § 1); a Heroes program on the Windows box.

## What I ran, and what I only read

**Ran** (this Mac, and the container `heroes-linux-arm64:latest` named `hero258-ffi-arm64`, removed at the
end): both compilers in my worktree; every door on both systems with the counter; `leaks --atExit` on every
Darwin door and every shape; the forced route A; the fortified shared library; shapes A, B, C;
`pngmsg_lent`, `pngmsg_lease`, plain and `--sanitize`; `hazard/d7.c` in four modes three ways;
`hazard/sigfree.c` ten runs a mode on both systems plus ASan; the (d) prototype on both, with and without
`-fexceptions`, with the library's unwind tables stripped; `clang -dM -E` under `-fexceptions`; `objdump` of
one function both ways, and of `__builtin_longjmp` for two x86-64 targets; `win/winjmp.c` on the Windows box
under the lock, twice, `-O0` and `-O2`, three runs each.

**Read only**: `sigjump_record.c` (route A's record), `rebind.c` and `rebind_jump.c`, `callback.c`,
`sigguard.c:560-660`, `heroes_runtime.h:15-30, 108, 131`, `spec/heroes-spec.md` lines 427, 456-475,
`design.md` §1.11 and §1.12, glibc's `pthread.h:555-600`, `jpeglib.h:784`, `gif_lib.h`, `tiffio.h:301`, and
Microsoft's two pages.

## Reproducers

Everything is in the session's scratchpad, `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/9d1670cb-db5d-4975-9d5b-1db104217e45/scratchpad/ffi258/` (it may be cleared): `doors/` (`doors.h`,
`door.hero`, `door_spawn.hero`, `cx258.h`, `cx258.cpp`, `cxdoor.hero`, `jl258.h`, `jl258.c`, `jldoor.hero`),
`png/` (`pngc.h`, `pngc.hero`, `pnga.hero`, `pngb.h`, `pngb.hero`, `pngmsg.h`, `pngmsg_lent.hero`,
`pngmsg_lease.hero`, panel 257's `pngread.h`, `badcrc.png` = `good.png` with bit 6 of byte 334 flipped),
`hazard/` (`sigfree.c`, `d7.c`), `unwind/` (`fu_cb.c`, `fu_main.c`, `fu_lib.cpp`, `mp.h`, `mp.c`, and the
Linux variants `fu_cb_l.c`, `fu_main_l.c`), `win/` (`winjmp.c`, `winlib.c`), and `patch_count.py`, which makes
the counted runtime copy from `runtime/`.
