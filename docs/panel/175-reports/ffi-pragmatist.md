# Panel 175 — ffi-pragmatist

Seat: design.md §1.11 (no standard library; FFI ergonomics rank alongside
comprehension) and §4.19 (a wrong binding is a compile error), veto on C-ABI
breakage or categorically harder bindings. Everything below was run on
2026-09-23 in `<scratchpad>/175-ffi-pragmatist/`, compiler built from the seed
there (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, 3.38 s),
`HEROES_RUNTIME` pointing at that copy. Linux runs are `heroes-linux-arm64`
(glibc 2.41, clang 22.1.8) and `heroes-linux` (x86-64), my directory mounted
read-only and copied inside. **Windows is UNRUN throughout**: the box was not
reached, as instructed. "Project flags" means the sixteen of
`selfhost/cli/flags.hero:91-109`, which is `exp/cc.sh`.

## Verdicts

| route | verdict | ground |
|---|---|---|
| **A** as written (one name, and "no other ends the life") | **object** | refuses correct programs in a class that is common in real headers (below); §1.11, §1.12 |
| **A′** — A whose mark may name MORE THAN ONE releaser, and whose set counts lives per address | **approve** | C ABI untouched, bindings unchanged, every crossing caught before C runs |
| **B** alone | **object** (partial by construction) | zero C cost; approve only as a complement |
| **C** | **VETO** | makes bindings categorically harder: §1.11, and Part 6's function-overloading row is what forces it |
| **D** = A′ + B | **approve** | |
| **E** | **approve, with three conditions** | the word is "under", SIGILL joins, the witness is `si_pid`/`si_code` |
| **F** | **approve** | its spelling already works today |
| **G** | **object** | Darwin and Windows print 0 bytes; §1.12 |
| **E + F** | **approve** | |

## Item 1 — the census, from real headers

**How it was searched.** `clang -fsyntax-only -Xclang -ast-dump=json` over six
translation units, and `census/census.py` over each AST. A releaser is a function
whose NAME matches
`close|free|release|destroy|delete|finali[sz]e|cleanup|dispose|unref|finish|fini$|deinit|detach|join|shutdown|dealloc|unload|_end$|End$|drop|discard|_done$|terminate|dec_ref`
and whose FIRST parameter is `T *` or `T **` after typedef resolution. Handle
types with two or more such functions:

| unit (headers) | functions read | ≥ 2 releasers |
|---|---|---|
| Darwin libc (35 POSIX/Darwin headers, `census/d_libc.c`) | 2030 | 10 |
| Darwin libraries (sqlite3, zlib, bzlib, curl, libxml2, libxslt, expat, ncurses, pcap, ldap, …) | 3122 | 28 |
| Darwin frameworks (CoreFoundation, CoreGraphics, CoreText, ImageIO, CoreVideo, Security, IOKit, CoreAudio, AudioToolbox) | 6961 | 33 |
| Homebrew (raylib, pcre2, OpenSSL, SDL3, libuv, FFmpeg, glib/gobject, cairo, zstd, …) | 13779 | 135 |
| Linux arm64 glibc (32 headers incl. `threads.h`, `mqueue.h`) | 1064 | 5 |
| Linux arm64 libraries (sqlite3, zlib, zstd, curl, libxml2, OpenSSL, ncurses, libssh2, ldap, krb5, gnutls, idn2, z3) | 9140 | 65 |

**What the search cannot see, and it matters:** (1) a generic releaser whose
parameter is `void *` — `free`, `CFRelease`, `os_release`, `xpc_release` — lands
in a `void` bucket and never beside the type it ends, so every CoreFoundation
type's second releaser is invisible to it; (2) names outside the pattern —
`delwin`, `ldap_unbind_ext`, `Z3_del_context` were missed; (3) a handle that is
not the first parameter (krb5 and Z3 group by their CONTEXT, a false grouping);
(4) most name matches are not releasers at all (`sqlite3_str_append`,
`CGContextClosePath`). So the counts are candidates, and the classification
below is by the header's own text or by running it. I did NOT classify all 276.

**Interchangeable — two releasers both right for one acquisition. Each row was
run twice, once per releaser, clean under ASan on both OSes and under `leaks
--atExit` (0 bytes) on Darwin; LSan is ASan's default on Linux** (`exp/interchangeable.c`):

| acquisition | releasers | the header's own words | run |
|---|---|---|---|
| `sqlite3_open` | `sqlite3_close`, `sqlite3_close_v2` | *"are destructors for the [sqlite3] object"* (`sqlite3.h:331`) | Darwin, Linux arm64 |
| `gzopen(…, "rb")` | `gzclose`, `gzclose_r` (`gzclose_w` for writing) | *"Same as gzclose(), but gzclose_r() is only for use when reading"* (`zlib.h:1665`) | both |
| `ZSTD_createCCtx` | `ZSTD_freeCCtx`, `ZSTD_freeCStream` | `typedef ZSTD_CCtx ZSTD_CStream; /**< … effectively same object */` | both |
| `pthread_create` | `pthread_join`, `pthread_detach` | (either ends a joinable thread; a handle on Darwin only, `pthread_t` is `unsigned long` on glibc) | both |
| `CGBitmapContextCreate` | `CGContextRelease`, `CFRelease` | *"Equivalent to `CFRelease(c)'"* (`CGContext.h:897`) | Darwin |
| `opendir` | `closedir`, `fdclosedir` | *"equivalent to the closedir() function except that this function returns directory file descriptor"* (man fdclosedir; macOS 26.4+) | Darwin |
| `BIO_new` | `BIO_free`, `BIO_vfree` | (OpenSSL) | Linux |
| `BN_new` | `BN_free`, `BN_clear_free` | (OpenSSL) | Linux |
| glibc `popen`/`fopen` | `fclose`, `pclose` — **on glibc only** | measured below | Linux both |

Read from headers and not run: `CVPixelBufferRelease` = *"Equivalent to
CFRelease, but NULL safe"* (and `CVBufferRelease`); `sqlite3_str_free(X)` =
*"the equivalent of calling sqlite3_free(sqlite3_str_finish(X))"*;
`avio_close`/`avio_closep`; glib's `*_free`/`*_unref`/`glib_autoptr_cleanup_*`
triples; libssh's legacy aliases (`channel_free`/`ssh_channel_free`).

**Exclusive — one right releaser per acquisition, several per type:**
`popen`→`pclose` against `fopen`→`fclose` **on Darwin**; FFmpeg
`avformat_open_input` → *"The stream must be closed with avformat_close_input()"*
against `avformat_alloc_context` → `avformat_free_context`; and
`AVIOContext`, which has BOTH shapes on one type: `avio_open` → {`avio_close`,
`avio_closep`} (*"can only be used if s was opened by avio_open()"*),
`avio_alloc_context` → `avio_context_free` (*"must be later freed with
avio_context_free()"*), `avio_open_dyn_buf` → `avio_close_dyn_buf`. **That type
is the one that decides the shape of the mark: the right answer is a relation
from each acquirer to a SET of releasers**, and neither "one name" nor "one type"
is it.

**What each C library does with `fclose` on a `popen` stream — the brief's
unrun line, now run** (`exp/popen_fclose.c`, `exp/popen_next.c`, 5 runs each):

- **Darwin:** `fclose` returns 0 and does not wait; the child is left a zombie
  (my `waitpid` reaped it 200 ms later, status 7). **The next `popen` in the
  process then runs nothing**: `popen("echo ran")` read nothing and `pclose`
  answered 32512, exit 127, **5 of 5**, against "ran", exit 0, 5 of 5 in the
  control that used `pclose`. A silent wrong answer one call later. `pclose` on
  an `fopen` stream answers -1. The man page: *"must be closed with pclose()
  rather than fclose()"*.
- **glibc, arm64 and x86-64:** `fclose` returns 1792 — it WAITED and handed back
  the child's status 7 — no child remains, the next `popen` is fine 5 of 5, and
  `pclose` on an `fopen` stream returns 0. The two are interchangeable there.
- **UCRT:** UNRUN.

So route A will abort `popen_linux.hero` on a platform where that program was
correct. That is the portable answer, and I accept it; it should be said.

## Item 2 — route A's C, compiled against the real headers

**What the runtime is handed: C names, as strings.** `const char *acquirer`
and a NULL-terminated `const char *const names[]` at the acquiring call, and the
releaser's name at the consuming call. Not addresses: measured
(`exp/tu_a.c`, `exp/tu_b.c`, the brief's own `i2.h`), `&h_close` is
`0x100a784a4` in one translation unit and `0x100a7855c` in the other, because a
header's `static` function exists once per unit and Heroes emits one unit per
module — `xacquires/` would abort falsely. Names are stable because an
`extern`'s name is the one name the mangler leaves alone (§4.19).

The emitted shape, applied by hand (`work/routeA.py`) to what the compiler
emitted; the whole diff for `popen_darwin.hero` is:

```c
_Static_assert(HERO_RUNTIME_ABI == 23, "heroes_runtime.h is from another compiler");
static const char *const hero_names_popen[] = {"pclose", NULL};
…
    t5 = popen(hero_cstr_nonnull(t2), hero_cstr_nonnull(t4));   /* unchanged */
    hero_handle_acquired_by(t5, "popen", hero_names_popen);
…
    hero_handle_consumed_by(t6, "fclose");
    t7 = fclose(t6);                                            /* unchanged */
```

The runtime side is `runtimeA/`: 56 added or removed lines in `parts/alloc.c` and 7 in `heroes_runtime.h`, counted with `diff`: two arrays
parallel to the set, moved with it on growth and on backward-shift deletion,
checked inside the lock the set already takes. Compiled with the emitted unit at
project flags, **0 diagnostics in every build**, identical on all three:

| program | Darwin arm64 | Linux arm64 | Linux x86-64 |
|---|---|---|---|
| `popen` → `fclose`, mark `acquires pclose` | 134, aborts before `fclose` | 134 | 134 |
| `sqlite3_open` → `sqlite3_close_v2`, mark `acquires sqlite3_close` | **134 — a correct program refused** | 134 | 134 |
| the same, mark names `sqlite3_close | sqlite3_close_v2` | 0 | 0 | 0 |

The message: *`panic: a C handle was about to be given back to `fclose`, and the
call that made it, `popen`, is marked `acquires pclose` — it was not handed to C,
because a function the mark does not name may free it wrongly or leave what it
holds behind. The handle is at 0x…`*. Today's compiler on `sqlite_v2.hero`:
check 0, run 0, 3 of 3 — correct, and nothing is wrong with it.

**ABI.** It moves, 22 → 23: two declarations are added to `heroes_runtime.h`.
Both stale directions are refused, measured in the containers: an old unit
against the new header is *static assertion failed due to requirement '23 ==
22'*, a new unit against the old runtime is *'22 == 23'* plus *call to
undeclared function 'hero_handle_acquired_by'* — §4.20's point that adding a
function guards itself, where adding a field would not. **What a C library
sees does not change.** The calls to `popen`, `fclose`, `sqlite3_open` and
`sqlite3_close_v2` are the same lines; the probes and `_Static_assert`s over
the header are the same bytes; no value changes layout; nothing is boxed.

**Cost.** 128 M acquire+consume pairs at `-O2` (`exp/bench.c`): 1.39–1.72 s
today and 2.69–3.05 s under A, `real` ≈ `user` in all four runs, so roughly
+10 ns per pair, against C calls that take microseconds. `selfhost/` has no
handle marks (the brief's count), so the compiler's own speed cannot move.

**The shape next to it, and it changes what A must store** (CL-061, CL-078).
A refcounted handle — CFRetain/CFRelease, `g_object_ref`/`unref`,
`X509_up_ref`/`X509_free`, `cairo_reference`/`destroy` — written as `work/rc.h`
and `work/refcount.hero` (`obj_ref(o: Obj) -> Obj acquires obj_unref`, two
unrefs): **today it aborts at 134 with the "given back twice" message, 396
bytes, 3 of 3**, and the same with `-> Obj borrows`, 3 of 3; the same C under
ASan exits 0. The set holds one life per address, so a second reference is
"lost here" (`alloc.c`'s own comment) and its correct release is a stray. There
is no spelling that runs. With a count per slot (`runtimeA2/`), the refcount
program exits 0, and the brief's `handle.hero` double release still aborts
134, 396 bytes, 3 of 3. I searched `docs/panel`, `docs/records/log`, `docs/work`
and `tests/golden` for `refcounted handle|reference-counted handle|_ref(|retain.*handle|same address.*twice|acquired twice`
and found nothing describing this, so it is a question for the coordinator
whether it is a new defect; I think it is. If A lands as written, it builds the
releaser column on top of this one-life-per-address model.

## Item 3 — route C against the real `stdio.h`

Measured with today's compiler (`work/routeC/`): two handles over `__sFILE` in
one module are `duplicate_tag`, and **in two modules too** (the rule is
program-wide); a second `fgets` over `Pipe` beside the one over `File` is
`declared_twice`, and **that refusal rests on Part 6's row against function
overloading**, so it will not be narrowed. An `extern` of module `pipes` called
from `main` is `extern_across_modules`. So under route C a pipe's `fread`,
`fgets`, `fwrite` and `ferror` must be declared AGAIN in a module of their own,
each wrapped in a Heroes function so another module can call it at all.

I built that compiler to see it work (`selfhostC/`, `duplicate_tag` switched
off for `__sFILE`/`_IO_FILE` only, `./heroes build selfhostC/main.hero -o
heroesC`, 76.69 s): `routeC/two_modules/main.hero` checks 0 and runs 0, and the
crossing `fclose(stream: p)` on a `Pipe` is `error[type_mismatch]: expected
`File`, found `Pipe`` — strictly earlier than A. **The price is the binding:**
`pipes.hero` is 26 lines, redeclaring five functions and wrapping six, so a
program can use four stdio calls on a pipe. The C that comes out is the same C:
`hero_ffi_probe_h_main_fgets(void *, int32_t, struct __sFILE *)` and
`hero_ffi_probe_h_pipes_fgets(void *, int32_t, struct __sFILE *)`. What route C
multiplies is **56 functions that take `FILE *` in the Darwin `stdio.h`+`wchar.h`,
62 in glibc's, and 50 that take `AVIOContext *` in FFmpeg's `avio.h`**
(`census/takers.py`), where AVIO would need three types. A Heroes helper that
reads "a stream" is then written once per type. And C buys nothing against the
interchangeable class, or against `gzopen`, whose right releaser depends on a
mode STRING.

**Veto on C**, on §1.11: the binding stops being "one line per C function"
and becomes one per C function per acquirer class, in separate modules, behind
wrappers. That is glue code, the thing §1.11 exists to refuse. It does not
touch the C ABI. It gives up nothing on §1.12 either, because A′ stops the same
crossing before C runs.

## Item 4 — question 2's C side, and route E's line measured

**What the C library says, with no Heroes line** (`work/linux_q2.sh`, 5 runs):

| | Darwin arm64 (libmalloc) | Linux arm64 / x86-64 (glibc) | Windows (UCRT) |
|---|---|---|---|
| `p10`, `b_out` | 133 (SIGTRAP), **0 bytes**, 5 of 5 | 134, 41 bytes, `free(): double free detected in tcache 2`, before anything else | UNRUN (brief: `0xC0000374`, 0 bytes) |
| `handle` | 134, 396 bytes (the runtime) | 134, 399 bytes | UNRUN |

**libmalloc does state its reason, just not on stderr.** Each run left a report
in `~/Library/Logs/DiagnosticReports/` (17 new in the window). For `p10.bin` it
reads `EXC_BREAKPOINT SIGTRAP`, application-specific information
`BUG IN CLIENT OF LIBMALLOC: not an allocated block`, frames
`mfm_free.cold.4 ← mfm_free ← release ← h_p10_main`. `b_out.bin` is the same
through `free_out ← h_bout_main`. So the reason exists, after the process has
died, in a file the user never opens. I propose nothing that reads it: the only
route in is private API.

**What `siginfo_t` can witness** (`exp/siginfo.c`, no Heroes):

| death | Darwin | Linux arm64 | Linux x86-64 |
|---|---|---|---|
| `abort`, `assert`, `raise`, glibc double free | SIGABRT, `si_code` 0, `si_pid == getpid()` | SIGABRT, `si_code` -6, `si_pid == getpid()` | same |
| libmalloc double free | SIGTRAP, `si_pid` 0, pc in `libsystem_malloc.dylib` | (glibc aborts) | (glibc aborts) |
| `__builtin_trap()` | SIGTRAP, `si_pid` 0 | SIGTRAP, `si_code` 1 | **SIGILL**, `si_code` 1 |
| `kill -ABRT` from another process | `si_pid` = the sender | `si_code` 0, `si_pid` = the sender | same |

Two consequences for the wording. The pc says nothing for a self-raised signal:
it is `__pthread_kill` in `libsystem_kernel`, or glibc, never the library that
called `abort`, so the line names an image only for a trap instruction. And
`si_pid` means nothing when `si_code > 0`: on Linux it overlays `si_addr`, and
it read `-1118630236`.

**The line**, prototyped in `runtimeE/parts/os.c` as the `else` of panel 173's
branch, so it speaks only when `!hero_runtime_spoke && held == 0`:

```
panic: the process is dying of SIGABRT, raised by this process, under e.main, and not by this runtime.
  A C library ends a program this way when it catches misuse, memory given back
  twice being the commonest; a reason C printed above, if any, is its own.
```

The second clause is one of `raised by this process` (`si_code <= 0 && si_pid ==
getpid()`), `sent by process N`, or `at a trap instruction in <image>`. It was
run against a C LIBRARY (`work/routeE/libe.c`, shared, `-O2 -fomit-frame-pointer`)
that calls `abort()` and fails an `assert` on purpose, with no lease live, on
three platforms (`work/routeE/run_e.sh`, `work/linux_e.sh`):

| case, no lease | today | route E, Darwin | route E, Linux arm64 | route E, Linux x86-64 |
|---|---|---|---|---|
| library `abort()` | 134, **0 B** everywhere | 258 B, *raised by this process, under e.main* | same | same |
| library `assert` | C's line only | + *raised by this process, under e.main* | same | + *raised by this process* (no frame found) |
| library `__builtin_trap` | 133/133/**132, 0 B** | *SIGTRAP, at a trap instruction in libe.dylib, under e.run* | *… in libe.so, under e.run* | *SIGILL, at a trap instruction in libe.so, under e.main* |
| library double free | 133 (Darwin), glibc's line | *SIGTRAP, at a trap instruction in libsystem_malloc.dylib, under e.run* | glibc's line + *raised by this process, under e.main* | glibc's line + *raised by this process* |
| `kill -ABRT` from outside | 134, 0 B | *sent by process 43074, under e.run* | *sent by process 64, under e.main* | *sent by process 64* |
| Heroes index panic | 32 B | 32 B, silent | silent | silent |

In every case the named function was ON the stack, so every line is true. **The
word has to be "under", because "called from" and "in" were measured false.**
In the draft I printed *called from e.main* where C was called from `e.run`: a
noreturn call inside an `-O2` library does not save the return address, so the
walk skips the direct caller. **The SHIPPED lease line has the same defect**:
with a lease live and a library `assert`, today's runtime prints *`the process
died with 1 lease(s) still live, in e.main`* on Darwin and on Linux arm64, and
the lease and the call were in `e.run`.

**A second hole in the shipped handler:** on Linux x86-64, a library trap with a
lease live is **exit 132 with 0 bytes** today, because the handler installs
SIGTRAP and SIGABRT and x86's `ud2` is SIGILL. With SIGILL added the lease line
prints, 265 bytes. I searched panels 172 and 173 (resolutions, briefs,
reports) for `SIGILL|ud2|x86.*__builtin_trap` and found nothing. Darwin x86-64,
where libmalloc's trap would also be `ud2`, is UNRUN because there is no Intel
Mac here.

**Windows, written and UNRUN:** in `hero_lease_veh`, when `ExceptionCode ==
0xC0000374L && !hero_runtime_spoke && hero_live_held == 0`: *`panic: the process
is dying of exception 0xC0000374, the heap manager's report of a corrupted heap,
and not by this runtime.`* plus the same second line. It has no frame walk,
for `stack.c`'s reason. Whether UCRT's `abort()` reaches a vectored handler at
all is UNRUN.

**F's spelling works today:** `work/p10_handle.hero` declares
`record Block tag void`, `make() -> Block acquires release`, and the brief's
double release is 134, 396 bytes, the runtime's report, 3 of 3. Two `tag void`
records in one program check at 0, after panel 170. So F's sentence points at
something a reader can actually write.

## The seat's answer

- **verdict:** object to A as written, approve A′ (set-valued mark with a life
  count per address), approve D = A′ + B, **veto C**, approve E with its three
  conditions, approve F, object to G.
- **section:** design.md §1.11 (ergonomics alongside comprehension; no glue),
  §4.19 (the mark and the header check), §1.12 (A′ and E), §4.20 (the ABI
  moves by function, self-guarding), Part 6 "Function overloading" (why C
  needs modules).
- **experiment:** route A's emitted shape compiled against the real
  `stdio.h`/`sqlite3.h` at project flags, 0 diagnostics, on Darwin arm64 and
  both Linux arches, with a runtime patch of 56 + 7 lines. Route C built as a narrowed
  compiler that ran. Route E prototyped and run against a `-O2` C library on
  three platforms. The census run over six header ASTs. Windows UNRUN.
- **argument:** Real headers routinely give one acquisition two correct
  releasers. Nine distinct pairs ran clean under ASan (26 runs, two OSes), and SQLite's own header calls
  both `close` functions "destructors". A rule that the one named releaser and no
  other ends a life aborts correct programs there, so the mark must name a set,
  as FFmpeg's `AVIOContext` requires. Route C catches the crossing earlier but
  multiplies every `FILE *` binding into modules and wrappers, which is glue
  §1.11 refuses; A′ stops the same crossing before C runs, leaves the ABI alone,
  and costs about 10 ns a call. E is true only if it says "under", which the
  shipped lease line does not.
- **prediction:** when defect 075's repair lands in M-agreed-retention, with a
  mark that takes one name only, `sqlite_v2.hero` (rung 3 of §4.19's ladder,
  released with `sqlite3_close_v2` as `sqlite3.h:331` allows) exits 134 on
  Darwin arm64, Linux arm64 and Linux x86-64. With a mark that names both it
  exits 0 on all three, and on the Windows box at the landing (unrun today).
  Checkable at the step that lands the repair.
- **condition:** I would drop the demand for a set if a census found no
  interchangeable pair a program needs both sides of. Nine ran clean, so I
  do not expect that. I would lift the veto on C if one `extern` declaration
  could accept both types without breaking Part 6's overloading row, shown with
  no wrapper module and at most one extra line per acquirer class. I would
  withdraw approval of E if any measured case printed a function that was not on
  the stack, or "raised by this process" for a signal another process sent.

## Files

All in `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e7e3d0df-8db3-44f2-b628-af9bb1a6080e/scratchpad/175-ffi-pragmatist/`:
`census/census.py`, `census/takers.py`, `census/*.c`, `census/*.out`,
`census/linux_out/`; `exp/cc.sh`, `exp/popen_fclose.c`, `exp/popen_next.c`,
`exp/interchangeable.c`, `exp/siginfo.c`, `exp/tu_a.c`, `exp/tu_b.c`,
`exp/bench.c`; `runtimeA/`, `runtimeA2/`, `runtimeE/`, `selfhostC/`;
`work/routeA.py`, `work/sqlite_v2.hero`, `work/*_A1.c`, `work/*_A2.c`,
`work/refcount.hero`, `work/rc.h`, `work/p10_handle.hero`, `work/routeC/`,
`work/routeE/`, `work/linux_routeA.sh`, `work/linux_q2.sh`, `work/linux_e.sh`.
