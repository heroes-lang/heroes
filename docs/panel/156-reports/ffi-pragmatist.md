# Panel 156 — ffi-pragmatist report

Seat: FFI pragmatist. Standing sections: design.md §1.11 and §4.19 (the runtime
is minimal and everything real comes from C), §1.12 (robustness). Veto scope:
the C ABI, and anything that makes a binding categorically harder.

Method: compile, don't opine. Every number below comes from a command run in
this session, on the machine named beside it. Anything unrun says so.

**verdict: approve, with one defect found that nobody asked about.**
**No veto. Nothing in this sitting touches the C ABI, the layout of a value
crossing the boundary, or the `importc`-style verification.**

---

## 1. THE QUESTION THAT OUTRANKS THE REST: measured, and the answer is NO

> Can a null read through a C boundary produce a wrong answer at exit 0 rather
> than an abort, at `-O2`, on any platform?

**Not on macOS arm64 and not on Windows x86-64, measured today.** Linux is
UNRUN and named below.

`heroes build tests/golden/surface-fixtures/nullread/main.hero`, this session:

| machine | level | exit | stdout | stderr |
|---|---|---|---|---|
| macOS arm64, clang 21 | `-O0` | 134 | `7` | `panic: a null pointer was read through … at offset 0x0, called from node_value` |
| macOS arm64, clang 21 | `-O2` | 134 | `7` | `panic: … at offset 0x0, called from main` |
| Windows x86-64, clang 22.1.8, **after my patch** | `-O0` | 127 | *(empty)* | `panic: … at offset 0x0` |
| Windows x86-64, clang 22.1.8, **after my patch** | `-O2` | 127 | *(empty)* | `panic: … at offset 0x0` |

**`-fno-delete-null-pointer-checks` is in the list AND it reaches the program,
and I proved it by taking it away.** `selfhost/cli/flags.hero:65` carries it;
`flags()` is what `cli/units.hero` and `cli/toolchain.hero:runtime_object` both
build their clang line from. The load-bearing test is the same translation unit
recompiled by hand, the one the build actually produced
(`build/tu-3dafcdf7232d721e/main.c`), at `-O2`, on this Mac:

```
clang -std=gnu11 -g -Wall -fno-strict-aliasing <FLAG> -O2 \
      -I runtime -I fix build/tu-3dafcdf7232d721e/main.c runtime/runtime.c -o x
```

| `<FLAG>` | exit | stdout | stderr |
|---|---|---|---|
| `-fno-delete-null-pointer-checks` | **134** | `7` | the panic line, `called from main` |
| `-fdelete-null-pointer-checks` | **133** | `7` | **EMPTY** |

Two things follow and both are worth writing down.

**The flag is doing work.** Take it away and the program still does not run to
a wrong answer — but it dies at 133 (SIGTRAP: clang folds the known-null
dereference to a trap instruction, which no SIGSEGV handler sees) **with an
empty stderr**. That is the SAME §1.12 violation as Windows' 139, in a third
costume. So panel 154's flag is not merely closing a wrong-answer path; it is
what keeps the fault reaching a handler at all.

**Panel 154's `8372224` at exit 0 did not reproduce here today.** I got 133, not
0. I am not calling that record wrong: the critic measured under all fourteen
flags on a witness built by `heroes build`, and I recompiled one TU with four.
What I can say is what I ran. **UNRUN, and it is the command that would settle
whether the wrong-answer path is dead or merely moved:**

```sh
# strip line 65 of selfhost/cli/flags.hero, rebuild the compiler from selfhost
# (59 s, over this sitting's per-command budget), then:
./heroes-next build tests/golden/surface-fixtures/nullread/main.hero -O2 -o /tmp/nr && /tmp/nr; echo $?
```

**UNRUN: Linux.** The container is
`docs/ref/environment/linux/Dockerfile`; the command is the two above run
inside it. glibc/x86-64 is the one of the three where `abort()` does not flush
stdio and where the `-O2` answer is therefore least predictable from here.

**Verdict on the outranking question: it does not decide this sitting.** No
wrong answer at exit 0 was produced on either machine I could reach.

---

## 2. R4, WINDOWS — the reading was right, and I built and measured the repair

### The reading is confirmed

`runtime/parts/stack.c`'s `hero_stack_veh` has exactly two arms:
`EXCEPTION_STACK_OVERFLOW`, and `EXCEPTION_ACCESS_VIOLATION` **with
`ExceptionAddress == 0`** — the PC, so that arm is defect 013's null FUNCTION
POINTER being called. **No arm reads `ExceptionInformation[1]`.** A read through
null has a valid PC, so it falls to `EXCEPTION_CONTINUE_SEARCH`.

**Measured on the box BEFORE any change**, `ssh win`, `/c/w/p156`:

```
./heroes.exe build tests/golden/surface-fixtures/nullread/main.hero -O0 -o /c/w/nr0.exe
BUILD_EXIT=0
RUN_EXIT=139   STDOUT=[]   STDERR=[]   "Segmentation fault"
```

Exit 139, both streams empty. That is word for word the state the fixture's own
comment calls *"what design.md §1.12 forbids by name"*, live on one of three
platforms.

### The witness table, measured on the box rather than assumed

The POSIX half of `stack.c` earns its arms with a four-shape probe table. **The
Windows half had none. Here is one**, `clang -O0 acc.c -o acc.exe` on the box,
a standalone VEH reading all four fields:

| shape | `NumberParameters` | `ExceptionInformation[0]` | `ExceptionInformation[1]` | `ExceptionAddress` (PC) |
|---|---|---|---|---|
| read `*p` through NULL | **2** | **0** (read) | **0x0** | NONZERO |
| write `*p = 5` through NULL | **2** | **1** (write) | **0x0** | NONZERO |

So: `ExceptionInformation[1]` IS `si_addr`, `NumberParameters` IS 2 so the guard
is satisfiable, and the PC is NONZERO for both — which is exactly why the
existing `ExceptionAddress == 0` arm misses them and the process falls through.

### The third arm, built and measured

I patched `runtime/parts/stack.c` and sent it to the box (base64 over ssh; md5
`374e2177776f973fec48783611311baf` verified identical on both ends). Two
changes.

**(a) One definition of the window, hoisted above the platform split.** The
`HERO_NULL_WINDOW` block sat inside the `#elif !defined(_WIN32)` branch, so the
Windows arm could not see it. Moving those five lines above
`#if defined(HERO_STACK_GUARD_YIELDS_TO_ASAN)` makes both arms read the same
number — CLAUDE.md's "each rule is written in exactly one place". The POSIX side
still builds: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes2`,
**exit 0 on this Mac** after the hoist.

**(b) The arm, after the null-function-pointer arm and before
`EXCEPTION_CONTINUE_SEARCH`:**

```c
    if (ep->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION
        && ep->ExceptionRecord->NumberParameters >= 2
        && (unsigned long long)ep->ExceptionRecord->ExceptionInformation[1] < HERO_NULL_WINDOW) {
        hero_stack_win_say("panic: a null pointer was read through \xe2\x80\x94 a handle or `ptr` holding `nullptr` reached C where C dereferences it, at offset ");
        hero_stack_win_say_hex((unsigned long long)ep->ExceptionRecord->ExceptionInformation[1]);
        hero_stack_win_say("\n");
        abort();
    }
```

with two `_write`-based writers beside it (`hero_stack_win_say`,
`hero_stack_win_say_hex`), the POSIX pair's vocabulary in this branch.

Three points of the arm, each deliberate:

- **`NumberParameters >= 2` is read first.** Those two parameters are documented
  for `EXCEPTION_ACCESS_VIOLATION` and nothing else; a handler that reads a
  union the OS did not fill is itself the corruption §1.12 forbids. Measured as
  2 above, so the guard costs nothing and buys the whole class of other
  exception codes that may reach this handler.
- **Order is part of the rule.** The `ExceptionAddress == 0` arm runs first, so
  an execute-type fault at address zero keeps defect 013's own message. An
  access violation of type 8 (execute) at null sets `ExceptionInformation[1]`
  to 0 as well, so without the ordering the two messages would race.
- **No frame walk, so no `called from`.** `SymFromAddr` needs dbghelp
  initialised before the fault and a PDB beside the binary; neither is measured.
  The line names the failure, as the branch's own comment already promises.

**MEASURED ON THE BOX, after the patch, same fixture:**

```
RUN_EXIT=127
STDOUT=[]
STDERR=[panic: a null pointer was read through — a handle or `ptr` holding `nullptr` reached C where C dereferences it, at offset 0x0]
```

and at `-O2`, identical: exit 127, same line.

**139 with silence → 127 with the sentence.** 127 is what this platform's other
two arms already produce (`abort()` through this CRT), so the exit code is
consistent with `panic: stack exhausted` and with defect 013's message, both of
which the record already puts at 127 on Windows.

**No seed rebuild was needed** and that is a measurement too: `heroes.exe build`
recompiles `runtime/runtime.c` per program (`cli/toolchain.hero:runtime_object`,
keyed on the runtime's own bytes), so changing `runtime/parts/stack.c` and
re-running `heroes.exe build` picked it up. The ~1 minute rebuild in the brief
is not on the critical path for this repair.

**R4 verdict: the missing arm is a defect, the repair is exactly the third arm
reading `ExceptionInformation[1]` against `HERO_NULL_WINDOW`, and it is built
and measured rather than proposed.** The patched file is at
`<scratchpad>/stack.c` and on the box at `/c/w/p156/runtime/parts/stack.c`.

### The repair attacked at the shapes next door, on the box

CLAUDE.md § RUN IT: *a repair is attacked at the shapes next to the one that
provoked it.* Four neighbours, all built and run on the box after the patch:

| neighbour | exit | stderr |
|---|---|---|
| `tests/golden/surface-fixtures/deep/` — stack exhaustion | 127 | `panic: stack exhausted` — **arm 1 not clobbered** |
| `tests/golden/fixedbugs/ffi-a-null-function-pointer-says-so.hero` | 0, prints `before` | the CRT checks `atexit(NULL)` on this platform, as `stack.c`'s own comment already says — **no regression, and no coverage of arm 2 here** |
| a null **WRITE** through a handle (`w_set(p: empty, v: 5)`) | **127** | the panic line, offset `0x0` — **the new arm catches writes too** |
| a **1 MiB** field offset (`struct bignode { char pad[1048576]; int64_t value; }`) | **139** | **EMPTY — the documented gap, inherited exactly** |

The last row is the honest one and it matters: the new arm closes exactly what
POSIX closes and nothing more.

### THE MEASUREMENT `stack.c` ASKED FOR AND NOBODY RAN — run

`runtime/parts/stack.c:160-165` marks its own Windows number as an inference:

> Linux's `mmap_min_addr` defaults to 65536 and Windows reserves the low 64 KiB.
> **Neither was run from this Mac**, and `.claude/rules/platforms.md` says a
> platform fact is run on a platform or it is an inference, so this one is
> marked as the inference it is.

**Asked of the platform, on the box** (`GetSystemInfo`, `clang floor.c`):

```
lpMinimumApplicationAddress = 0x0000000000010000     <- 65536
lpMaximumApplicationAddress = 0x00007FFFFFFEFFFF
dwAllocationGranularity     = 65536
dwPageSize                  = 4096
```

**`HERO_NULL_WINDOW` is 65536 and Windows' own floor is 65536. The inference was
exactly right, and it is now a measurement.** Two consequences the panel should
record:

1. **The window cannot be widened on Windows.** 0x10000 is where the
   application's address space begins; one byte above it is mappable. So the
   1 MiB `bignode` row above is not closeable by a bigger constant on this
   platform, and any proposal to raise the number must be refused here.
2. **There is a route that would close it, and it is Windows-only.**
   `VirtualQuery(addr)` in the handler answers whether the touched page is
   `MEM_FREE`; a fault on free memory is a wild or null-derived pointer
   whatever its offset. **UNRUN** — the command is one more `acc.c`-shaped
   probe on the box calling `VirtualQuery` from inside the VEH — and I do not
   recommend it in this sitting, because it is a second mechanism on one of
   three platforms and the divergence costs more than the row it closes.

### The asymmetry, answered

The brief asks whether Windows is simply missing the second witness or whether
the platform makes it different. **It is simply missing it, and the file's own
comment says why it was easy to miss.** That comment
(`runtime/parts/stack.c`, Windows branch) reads:

> `ExceptionAddress` is where the program was EXECUTING, which is the PC — the
> address it TOUCHED is `ExceptionInformation[1]`, and **that one is zero for
> three different faults**, which is exactly why the POSIX handler above reads
> the PC and not `si_addr`.

That sentence is correct about defect 013 and it is the trap. It says *the
touched address is a bad witness for the null-CALL case*, which is true; it was
then read as *the touched address is a bad witness*, full stop. Defect 045's
repair — the null-READ case — needs precisely that field, and the POSIX handler
does read `si_addr` for it (`stack.c:459`). So POSIX carries two witnesses for
two faults and Windows carried one for one. **There is no platform reason.**

**And the asymmetry runs the OTHER way as well, which nobody has named.**
`ExceptionInformation[0]` is the access type, measured 0 for a read and 1 for a
write above. **Windows knows something POSIX does not**: `si_addr` cannot tell a
read from a write. So the Windows message is entitled to say *written through*
where it says *read through* today, and my patched run of the write fixture
shows the current wording calling a store a read. I did not change the wording,
because the same text ships on all three platforms and a per-platform sentence
is a diagnostic-class decision that belongs to this panel and not to me.

---

## 3. R3, THE FLUSH — what is SAFE in a handler, compiled rather than described

The `7` is lost on Linux and, **measured, on Windows too**: my patched run has
`STDOUT=[]`. So the lost-`7` finding is not a Linux quirk, it is **two of three
platforms**. macOS is the outlier that flushes.

Four alternatives, and only two of them are safe:

| route | safe in a handler? | what it costs a binding |
|---|---|---|
| `fflush(stdout)` (or `fflush(NULL)`) in the handler | **NO.** Not in POSIX's async-signal-safe list. The faulting thread may already hold stdout's lock — the fault can be *inside* a C library's own `fwrite` — and then the handler deadlocks or re-enters. It **usually** works, which is worse than failing. | nothing |
| `setvbuf(stdout, NULL, _IONBF, 0)` at startup | **YES**, and it needs nothing in the handler at all: there is no buffer to lose. | a `write(2)` per `print`. Real, and it is the only cost. |
| a `write`-based `print` in the runtime | **YES**, same reason, and it is what `hero_stack_say` already does for the panic line. | one runtime function changes; **zero** at the C boundary |
| nothing | safe, and the program lies about what it printed | nothing |

**The one I recommend is the third, and the reason is my seat's.** `print` is
the Heroes runtime's, not C's. Unbuffering `stdout` globally (route 2) changes a
`FILE *` that a C library the program links also writes to — SQLite's shell
echo, raylib's log callback, anything that takes `stdout` — so it is a decision
taken on a C library's behalf, at its cost, without asking it. A `write`-based
`print` touches only Heroes' own output path and leaves every binding's
`stdout` exactly where the library expects it.

**UNRUN, and it is one patch plus one rebuild on the box:** whether
`fflush(NULL)` before `abort()` in the third arm recovers the `7` on Windows. I
did not run it because the answer does not change the recommendation — an
unsafe repair that works is still unsafe (CLAUDE.md § Precedence rank 3,
robustness beats convenience) — and because I would rather not leave an unsafe
call in the file I measured on the box.

---

## 4. A DEFECT NOBODY ASKED ABOUT, and it is at my boundary

**`heroes build --emit-c` writes C that does not compile, for any binding that
uses `record … tag <name>`.** Measured:

```sh
./heroes build fix/main.hero -O2 --emit-c -o nr.c     # exit 0
clang -std=gnu11 -O2 -I runtime -I fix nr.c runtime/runtime.c -o nr
# 15 errors: "must use 'struct' tag to refer to type 'node'"
```

The emitted file spells the record's C type `node *`, and the fixture's header
declares `struct node`. **The compiler itself is fine** and this is the good
half of the story: `cli/pointee.hero` PROBES the header, learns the tag needs
`struct`, and re-emits. I measured both rounds in the build cache — the TUs with
no object never say `struct node`; the TUs that produced an object say it
**12 times each**:

```
tu-1f0073dc7040a7b3 obj=0  "struct node" count=0
tu-3dafcdf7232d721e obj=1  "struct node" count=12
tu-6987a5635e92ce1f obj=0  "struct node" count=0
tu-6c4616f85bada1b6 obj=1  "struct node" count=12
```

So the `importc`-style verification is intact and self-correcting — **that is
the thesis working, and it is why I do not veto anything here.** What is broken
is only `--emit-c`, which hands out the PRE-probe spelling. A reader who does
what `--emit-c` invites — take the C and compile it — gets fifteen errors on a
program that builds. That belongs in `docs/work/DEFECTS.md`, not in this
sitting's resolutions, and I name it because §4.19's ladder is a reader taking
the emitted C seriously.

---

## 5. VERDICTS, R1–R5, insofar as they touch C

**R1 — which blame line. OBJECT to pinning either name, and the measurement is
new: the name is not a platform fact, it is an OPTIMISATION LEVEL fact.** On
this same Mac, same compiler, same program:

- `-O0` → `called from node_value`
- `-O2` → `called from main`

`.expected` pins `node_value`. So the golden is already wrong at `-O2` **on the
green leg**, and nobody has noticed because the fixture is run at `-O0`. Any
resolution that pins one symbol name pins it to a level and an inliner. What
`hero_stack_blame` returns is `dladdr`'s answer about a PC, and at `-O2`
`node_value` is a `static inline` that no longer exists as a symbol — which is
the normal case for a real header, because `sqlite3.h`, `raylib.h` and every
`stdint`-style header ship `static inline` bodies. **The C-boundary-correct
answer is: the `panic:` sentence and the offset are the contract; the
`called from` clause is best-effort and must be matched as such.**

**R2 — the arm64 walk. No C objection either way, one caution.** If the walk is
changed so the Heroes caller wins over the faulting C function, note that at
`-O2` there is often no faulting C function left to name — my `-O2` run named
`main`, the C function the inliner merged everything into. A repair tested only
at `-O0` will not hold.

**R3 — the flush. OBJECT to any `fflush` in the handler.** Recommend a
`write`-based `print` in the runtime, per §3. Do not unbuffer `stdout`
globally: that is a decision taken on a linked C library's behalf.

**R4 — Windows. APPROVE, and it is built and measured.** 139-with-silence →
127-with-the-sentence, on the box, this session. Neighbours checked: stack
exhaustion still works, null writes are caught, the 1 MiB offset still falls
through exactly as on POSIX.

**R5 — CI green before the full repair. Recommend: land R4 first, because it is
done.** The Windows leg's red is a §1.12 violation and the repair for it exists
and is measured; nothing about R1's contested blame line blocks it. That leaves
one red leg (Linux) instead of two, and the remaining red is about a message
rather than about a program dying in silence. **No, there is no defensible way
to make the Windows red green without the arm** — the only alternative is
loosening `.expected`, and loosening it to accept an empty stderr would pin the
exact state design.md §1.12 forbids.

---

## 6. WHAT I LEFT UNRUN, and the command for each

1. **Linux, everything.** `docs/ref/environment/linux/Dockerfile`, then the two
   build-and-run lines of §1. Nothing in my report claims a Linux number.
2. **The compiler rebuilt without `-fno-delete-null-pointer-checks`** (§1). 59 s,
   over the per-command budget of this sitting.
3. **`fflush(NULL)` before `abort()` on the box** (§3). Deliberate: an unsafe
   repair that works is still unsafe.
4. **`VirtualQuery` in the VEH** (§2), the Windows-only route that would close
   the 1 MiB row. One `acc.c`-shaped probe on the box.
5. **The suites.** `annotations` and `fixes` judge
   `tests/golden/surface-fixtures/**` per `.claude/rules/verification.md`. I ran
   neither; the tree is frozen and my change lives in a scratchpad and on the
   box.

## 7. Prediction, falsifiable

**With the third arm as written, `tests/golden/surface-fixtures/nullread/`'s
Windows leg will match `.expected`'s `panic: a null pointer was read through`
line and will still FAIL on two things: the missing `7` on stdout, and the
absent `called from` clause.** Falsified if the Windows leg goes green on the
arm alone, or if it still produces exit 139.

**And on my own ladder:** SQLite step 3 of §4.19 needs no shim under this rule.
Nothing here changes a signature, a layout, a NUL-termination or a refcount;
`sqlite3_close(NULL)` and `freeaddrinfo(NULL)` stay legal C called from Heroes,
because the repair guards the FAULT and not the ARGUMENT — which is the whole
reason panel 154's answer was a build flag and not a type rule.

## 8. Condition

**My approval flips to a veto if any resolution here proposes refusing `nullptr`
at the call site** — a non-null type, a `nonnull` attribute, or a runtime check
emitted before every extern call. `sqlite3_close(NULL)`, `free(NULL)`,
`freeaddrinfo(NULL)` and the repository's own shipped `getaddrinfo(hints:
nullptr)` golden are all correct C, five seats settled that at panel 154, and
refusing them makes every real binding need a shim.

**Second condition, narrower:** I object if the panel widens `HERO_NULL_WINDOW`
beyond 65536 for any platform. Windows' own floor is 65536, measured above; one
byte higher and the guard starts claiming faults on memory a C library
legitimately mapped, which is a false panic in a correct binding.

The measurement that would change my mind on the first condition is a header —
any header — that states non-nullness in a form `cli/header_types.hero` can
read. I know of none and I did not search exhaustively, so that is a question
and not a premise.

**Veto: NOT cast.**
