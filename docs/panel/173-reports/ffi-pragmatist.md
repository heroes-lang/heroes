# Panel 173 — ffi-pragmatist

**verdict: VETO on `docs/panel/173-briefs/prototype.diff` as written.** Three
conditions lift it, and I ran all three. The mechanism is sound; these fifty
lines are not.

**section: design.md §1.11** (the founding constraint: *anything a real program
needs comes from C libraries through the FFI*) and **§4.19** (*treat its
ergonomics as a priority rather than an afterthought*), with
`.claude/rules/platforms.md` § *Linux is where a leak in a binding is visible*
(CL-055) as the instrument.

Every number below is from a command run on 2026-09-21, on this Mac (Darwin
25.6.0 arm64) or in `heroes-linux-arm64` (Debian 13, glibc 2.41, Debian clang
22.1.8, `uname -m` = `aarch64`). Copies:
`<scratch>/ffi-pragmatist-173` (prototype), `<scratch>/ffi-stock-173` (stock,
`git checkout -- runtime/`), `<scratch>/ffi-chain-173` (my chained variant).

---

## The veto, in one table

`docs/panel/173-briefs/libabrt.h` is the C I wrote: a library that installs a
SIGABRT/SIGTRAP handler which writes one line and `_exit(77)`, so whose handler
ran is read off the exit code alone. `LIB_CTOR=1` makes it install from an
`__attribute__((constructor))`, which is how every real crash reporter,
test harness and `glog`/Breakpad-style library installs: **before `main`**,
therefore before `hero_args_set` and therefore before the prototype's installer.

| program | runtime | exit x10 | library's handler ran? |
|---|---|---|---|
| `q4_main` (installs from a call in `main`) | stock | `77` x10 | yes |
| `q4_main` | prototype | `77` x10 | yes |
| `q4_ctor` `LIB_CTOR=1`, **Darwin** | **stock** | `77` x10, 32B | **yes** |
| `q4_ctor` `LIB_CTOR=1`, **Darwin** | **prototype** | `133`/`134`, 250B | **NO** |
| `q4_ctor` `LIB_CTOR=1`, **Linux arm64** | **stock** | `77` x10, 64B | **yes** |
| `q4_ctor` `LIB_CTOR=1`, **Linux arm64** | **prototype** | `134` x10, 282B | **NO** |

That is an ABI break, measured, ten runs out of ten on two platforms: a C
library's SIGABRT disposition is silently destroyed by linking against the
Heroes runtime. §1.11 says everything a real program needs comes from C through
the FFI; a runtime that eats a library's process-wide signal handling has taken
a boundary the library never agreed to, and the program cannot opt out.

**And the repository already knows the answer.** `runtime/parts/stack.c:363-375`
saves the previous disposition and *calls it*:

```
363:    struct sigaction *prev = signum == SIGBUS ? &hero_stack_prev_bus : &hero_stack_prev_segv;
364:    if ((prev->sa_flags & SA_SIGINFO) != 0 && prev->sa_sigaction != NULL) {
365:        prev->sa_sigaction(signum, si, ctx);
```

with its own comment at `stack.c:35-36`: *"The previous disposition is saved at
install and a fault that is not ours is handed to it; only a saved
SIG_DFL/SIG_IGN is …"*. The prototype saves into `hero_lease_prev_trap` and
`hero_lease_prev_abrt` and **never reads them**, going straight to `SIG_DFL` and
`raise`. It is not a new question. It is the runtime's own settled rule, unapplied.

### The lift, measured

I built `<scratch>/ffi-chain-173`: the prototype with its `SIG_DFL`+`raise` tail
replaced by ten lines copied verbatim from `stack.c:363-375`.

    cd <scratch>/ffi-chain-173 && rm -rf build heroes && clang -I runtime seed/heroes.c runtime/runtime.c -o heroes   # exit 0

| chained variant | exit x10 | stderr |
|---|---|---|
| `q4_ctor` `LIB_CTOR=1`, Darwin | **`77` x10**, 282B | our 3 lines **then** `library: my SIGABRT handler ran` |
| `q4_ctor` `LIB_CTOR=1`, Linux | **`77` x10**, 314B | glibc's line, our 3 lines, **then** the library's |
| `q4_ctor` no ctor, Darwin | `133`/`134`, 250B | our 3 lines |
| `lease070`, `b_later`, `b_out`, `control`, `panic_lease` | unchanged from the prototype on both platforms | unchanged |

**Ten lines buy back the whole ABI. Nothing else moves.** There is no trade to
weigh here, which is why this is a veto and not a price.

---

## Question 1 — Linux, on the platform (RUN, not inferred)

    docker run --rm -v <proto>:/src:ro -v <stock>:/stock:ro heroes-linux-arm64 \
      bash /src/docs/panel/173-briefs/linux_run.sh        # DOCKER EXIT: 0
    aarch64 · ldd (Debian GLIBC 2.41-12+deb13u3) 2.41 · Debian clang version 22.1.8

| program | stock exit/stderr x10 | prototype exit/stderr x10 | first line of stderr |
|---|---|---|---|
| `lease070` | `134`/32B x10 | `134`/**282B** x10 | `munmap_chunk(): invalid pointer` |
| `b_later` | `134`/32B x10 | `134`/**282B** x10 | `munmap_chunk(): invalid pointer` |
| `b_callback` | `134`/32B x10 | `134`/**282B** x10 | `munmap_chunk(): invalid pointer` |
| `b_out` | `134`/41B x10 | `134`/41B x10 (**silent, correct**) | `free(): double free detected in tcache 2` |
| `control` | `134`/111B x10 | `134`/111B x10 | `panic: 1 lease(s) never ended …` |
| `panic_lease` | `134`/32B x10 | `134`/**282B** x10 | `panic: array index out of range` |

Four Linux facts, each measured, none inferred:

1. **The signal is SIGABRT (134) on every allocator path**, not SIGTRAP. Linux
   never produces 133 from `free`. The 133/134 split is Darwin's alone.
2. **glibc's message is `munmap_chunk(): invalid pointer`, not
   `free(): invalid pointer`** for the lease shape. The brief predicted the
   latter; the brief is wrong for a 24-byte block freed at +16. Plain
   `variants.c` case 0 (a 64-byte block freed at +16) *does* say
   `free(): invalid pointer`. So the glibc line is not one string.
3. **The prototype prints BESIDE glibc's line, never instead of it**, 10/10:
   32B + 250B = 282B exactly.
4. **`b_out` stays silent on Linux too.** The class boundary holds on both
   platforms.

### And Linux is where the prototype breaks the build

    clang -fsanitize=address,undefined -I runtime -c runtime/runtime.c -o /w/x.o
    runtime/parts/alloc.c:203:10: error: unknown type name 'sig_atomic_t'; did you mean '__sig_atomic_t'?
      203 | volatile sig_atomic_t hero_exit_sweep_running = 0; /* CRITIC-172 PROTOTYPE */
    1 error generated.                                         exit 1

    CPATH=$B ./heroes build --sanitize $B/lease070.hero -o /w/p_san
    internal error: the runtime did not compile
    error: no runtime object                                   exit 2

I isolated the trigger; it is the sanitizer, not the standard:

| flags | exit |
|---|---|
| `` (default) | 0 |
| `-std=c11` | 0 |
| `-std=gnu11` (what `flags.hero:93` actually passes) | 0 |
| **`-fsanitize=address,undefined`** | **1** |
| `-std=c11 -fsanitize=address,undefined` | 1 |

`alloc.c` never includes `<signal.h>`; without ASan something else in the TU
pulls it in on glibc, and with ASan nothing does. **Darwin hides this
completely**: `./heroes build --sanitize` there is exit 0 on both runtimes.
`.claude/rules/platforms.md` (CL-055) says a program that declares an `extern`
runs its Linux leg under `--sanitize`, so this is not a corner: it is every FFI
program in the repository, red on both Linux legs of the CI matrix
(`.github/workflows/ci.yml:290`, `ubuntu-latest` and `ubuntu-24.04-arm`).

**Cost of the repair: one line.** I added `#include <signal.h>` above
`alloc.c:203` in a fourth copy and re-ran:

    one-line fix, runtime under ASan: exit 0 (0B)
    fixed --sanitize build exit: 0

### `--sanitize` does not compose, and the divergence is platform-shaped

With the include fixed, `lease070 --sanitize`:

| platform | exit x10 | bytes | ASan speaks | **our report prints** |
|---|---|---|---|---|
| Linux arm64 | `1` x10 | 1939B | yes, with `giveaway.h:3` and `lease070.hero:7` named | **NO** |
| Darwin arm64 | `134` x10 | 1324B | yes, same frames | **yes** |

ASan on Linux calls `_exit(1)`; no SIGABRT is raised, so the handler never runs.
ASan on Darwin aborts, so it does. **On the one configuration the project
mandates for FFI programs, the mechanism is inert and ASan already prints
strictly more** (the freeing C function, by file and line, plus the allocating
Heroes line). Stock and fixed-prototype are byte-for-byte equivalent there:
`1/1957B` versus `1/1939B`, the difference being path lengths in the trace.

---

## Question 2 — the two paths on Darwin

    clang -o /w/variants docs/panel/173-briefs/variants.c   # exit 0, 724B of warnings

| `variants.c` case | what it frees | exit x10 (Darwin) | exit x3 (Linux) | glibc's line |
|---|---|---|---|---|
| 0 | interior of a 64B heap block | **`133`** x10 | `134` | `free(): invalid pointer` |
| 1 | double free of a 64B block | **`133`** x10 | `134` | `free(): double free detected in tcache 2` |
| 2 | a stack pointer | **`134`** x10 | `134` | `double free or corruption (out)` |
| 3 | `(void*)0x1234` | **`134`** x10 | **`139`** (SIGSEGV) | none |
| 4 | interior of a **100000B** block | **`134`** x10 | `134` | `free(): invalid pointer` |

Darwin's rule, as far as this measures it: **the nano/small allocator's own
checks raise SIGTRAP (133); everything the allocator classifies as "not one of
mine" raises SIGABRT (134)** — and a large allocation is on the other path, so
case 4 flips. Darwin's allocator prints nothing at all in all five (the 9B of
stderr is `before N`).

**The goldens cannot pin the exit code on Darwin, and not because of the
platform split.** The same binary is nondeterministic run to run:

    s_lease070  x10: 133 133 133 133 133 133 133 133 133 134
    p_b_later   x10: 133 133 133 133 133 133 133 133 133 134
    p_b_callback x10: 133 133 134 133 134 133 134 133 133 133

The coordinator's note that `tests/harness/expectation.hero:52-93` judges
`!panic:` by containment with any non-zero exit code is therefore not a
convenience, it is the only thing that makes such a golden possible at all.

### What `siginfo_t` can and cannot tell the handler

`docs/panel/173-briefs/sigprobe.c`, compiled and run on both (Linux needed
`#include <stdint.h>` added; the version in the briefs does not compile there,
`error: use of undeclared identifier 'uintptr_t'`):

| case | Darwin: sig / si_code / si_pid | Linux: sig / si_code / si_pid |
|---|---|---|
| interior free | 5 / 0 / 0 | 6 / **-6** (SI_TKILL) / = pid |
| `abort()` | 6 / 0 / = pid | 6 / **-6** / = pid |
| `__builtin_trap()` | 5 / 0 / 0 | 5 / 1 (TRAP_BRKPT) / — |
| double free | 5 / 0 / 0 | 6 / **-6** / = pid |
| large interior free | 6 / 0 / = pid | 6 / **-6** / = pid |
| a library's `assert(0)` | 6 / 0 / = pid | 6 / **-6** / = pid |

**`siginfo_t` cannot distinguish the allocator's abort from a library's abort
from the runtime's own abort, on either platform.** Every self-raised SIGABRT
looks identical. So the shared brief's option *"a handler that reports only when
the signal's origin is not the runtime"* is **not implementable from
`siginfo_t`**; it has to be a flag, and the flag has to be set at all 22
`abort()` sites (measured: `alloc.c` 5, `os.c` 7, `stack.c` 6, `failure.c` 3,
`panic.c` 1) or at one funnel they all pass through.

---

## Question 3 — real libraries, not `static inline`

**The direct spelling is already a compile error, on both platforms.** This is
the good news, and it is the thesis working: I could not write the wrong binding.

| program | spelling | build exit | diagnostic |
|---|---|---|---|
| `q3_free_ptr` | `function free(p: ptr)` from `stdlib.h`, called with a `cstr` lease | **1** | `error[type_mismatch]: expected `ptr`, found `cstr`` |
| `q3_free_cstr` | `function free(p: cstr)` | **1** | ``error[ffi_writable_parameter]: `p` of `free` is declared `cstr`, and the header says `void *` — C does not promise to leave it alone`` |
| `q3_sqlite_ptr` | `extern "sqlite3.h" link "sqlite3"`, `function sqlite3_free(p: ptr)` | **1** | `error[type_mismatch]: expected `ptr`, found `cstr`` |
| `q3_sqlite_cstr` | `function sqlite3_free(p: cstr)` | **1** | ``error[ffi_writable_parameter]: … the header says `void *``` |
| `q3_charstar` | `char *` taker (`writable.h`), declared `cstr` | **1** | ``error[ffi_writable_parameter]: … the header says `char *``` |

So: **how I spelled it, and what refused me.** A `cstr` lease cannot be passed
to a `ptr` parameter (`type_mismatch`), and a `void *` or `char *` parameter
cannot be declared `cstr` (`ffi_writable_parameter`). There is no third
spelling. The filed shape only reaches a real library by the indirect route,
which is what `beside_sqlite.h` is:

```c
static const char *sql_stashed;
static inline void sql_stash(const char *s) { sqlite3_initialize(); sql_stashed = s; }
static inline void sql_later_free(void) { sqlite3_free((void *)(uintptr_t)sql_stashed); }
```

    q3_sqlite_later build 0
    x10: 134/250B 133/250B 133/250B 134/250B 133/250B 133/250B 133/250B 133/250B 134/250B 134/250B
    panic: a C function freed bytes this program still leases — 1 lease(s) were live …

**Yes: the report prints through a real dynamic library's free** (`-lsqlite3`,
SQLite 3.51.0 on Darwin, 3.46.1 in the container). And SQLite's own allocator is
**not** a different signal or a different message. `docs/panel/173-briefs/sqlfree.c`,
plain C, `clang -o /w/sqlfree sqlfree.c -lsqlite3` exit 0, 0B of warnings:

| `sqlfree.c` case | exit x10 (Darwin) |
|---|---|
| 0 — `sqlite3_free(block+16)` | `133`/`134` mixed, 22B (the version banner only) |
| 1 — `free(block+16)` | `133`/`134` mixed, 22B |
| 2 — `sqlite3_free` twice on SQLite's own pointer | `133` x10, 22B |

SQLite's default `mem1` forwards to the system allocator, so it inherits the
allocator's signal and adds no message of its own. `sqlite3_initialize()` first
matters: without it `sqlite3_free` reaches a null `xSize` and dies 139, which is
SQLite's uninitialised state and not the allocator.

---

## Question 5 — is the sentence true?

The message asserts *a C function freed bytes this program still leases*. I ran
every neighbouring shape. **It is false on six of the nine paths that reach the
handler.**

| program | what actually happened | Darwin exit/bytes | Linux exit/bytes | message printed? | **true?** |
|---|---|---|---|---|---|
| `lease070` | C freed the lease | 133/134, 250B | 134, 282B | yes | **TRUE** |
| `b_later` | C freed the lease later | 133/134, 250B | 134, 282B | yes | **TRUE** |
| `b_callback` | C freed the lease via a callback | 133/134, 250B | 134, 282B | yes | **TRUE** |
| `q3_sqlite_later` | SQLite freed the lease | 133/134, 250B | UNRUN | yes | **TRUE** |
| `b_out` | C double-freed C's own `@out` pointer, **no lease live** | 133, 0B | 134, 41B | **no** | correct silence |
| `q5_own` | a C library double-freed **its own** pointer | 133/134, 250B | 134, 291B | yes | **FALSE** |
| `q5_own_interior` | a C library freed an interior of **its own** block | 133, 250B | 134, 274B | yes | **FALSE** |
| `q5_cabort` | a C library called `abort()` (an assert). **Nothing was freed.** | 134, 250B | 134, 250B | yes | **FALSE** |
| `q5_ctrap` | a C library hit `__builtin_trap()`. **Nothing was freed.** | 133, 250B | 133, 250B | yes | **FALSE** |
| `q5_overflow` | Heroes stack exhausted | 134, 292B | 134, 292B | yes, as a **second** `panic:` | **FALSE** |
| `panic_lease` | Heroes array index out of range | 134, 282B | 134, 282B | yes, as a **second** `panic:` | **FALSE** |

`q5_raise` did not build: ``error[reserved_word]: there are no exceptions in
this language`` on `function raise(sig: i32)`. An externally delivered SIGABRT
is therefore **UNRUN** and would need a different spelling.

Note that `q5_cabort` and `q5_ctrap` are the ones that matter for §1.11. A
library's own `assert` is the most ordinary thing a C library does on a bad
argument, and with a lease live anywhere in the program the runtime will print
*a C function freed bytes this program still leases* about a program in which
nothing was freed. That is not a weak claim, it is a wrong one, and it points
the reader at the FFI when the bug is elsewhere.

**The weaker sentence the brief proposes is the true one on all nine**: *a lease
was live when the process was killed by the allocator* — except that even
"by the allocator" is false for `q5_cabort`, `q5_ctrap`, `q5_overflow` and
`panic_lease`. The sentence that survives every measured path is: **N lease(s)
were live when the process died.** Anything stronger needs the flag that
question 1 of the engineer's brief is about, set at all 22 sites, and even then
`q5_cabort`/`q5_ctrap`/`q5_own` remain false because C's abort is
indistinguishable from the allocator's (see the `si_code` table).

---

## cost

- **ABI**: a C library's SIGABRT and SIGTRAP dispositions, installed before
  `main`, are destroyed. Measured, both platforms, 10/10. **Repair: 10 lines,
  copied from `stack.c:363-375`, measured working on both platforms.**
- **Linux build**: `heroes build --sanitize` exits 2 for every program.
  **Repair: one `#include <signal.h>`, measured.**
- **Truth**: six of nine measured paths print a false sentence. Repair: not
  measured here; it is question 1 of the engineer's brief.
- **Goldens**: see the prediction. Not repairable in `tests/golden/run/` today.
- **Seed**: none. `git diff --stat` touches `runtime/parts/` only, and the seed
  built clean on both platforms from all three copies (`stock seed: 0`,
  `proto seed: 0`, `chain seed: 0`).

## prediction (falsifiable, naming an instrument that exists)

**`tests/harness/suite_run.hero:146-148` will refuse all three goldens the
shared brief says the landing owes.** That code reads:

```
if strings.contains(text: sanitised.must().err, needle: "AddressSanitizer") || …
    report.keep_failure(@r, label: "run/" + item.name, detail: "  tripped a sanitiser:\n" + …)
```

and `suite_run.hero:139` runs **every** `tests/golden/run/` case a third time
under `--sanitize`. I measured that `lease070`, `b_later`, `b_callback` and
`b_out` all print `AddressSanitizer` there (Darwin: 2 occurrences each, 1248B to
1377B; Linux: exit 1, 1939B). **So adding `tests/golden/run/lease070.hero` will
turn `run` red with `tripped a sanitiser`, on Darwin and on both Linux legs, no
matter what the `.expected` says.** Falsify it by adding the case and running
`./heroes run tests/harness/main.hero -- ./heroes run`. If I am right, the
landing owes a harness change (an opt-out for a case that is *supposed* to
provoke the sanitiser) that does not exist today, and the shared brief's
"a `tests/golden/run/` case per shape" is not currently buildable.

## veto condition — exactly what lifts it

1. **The handler chains to the disposition it saved**, as `stack.c:363-375`
   does, instead of `SIG_DFL` + `raise`. Non-negotiable: this is the ABI clause.
   I have run the patch; it costs ten lines and changes nothing else.
2. **`runtime/parts/alloc.c` includes `<signal.h>`**, so the Linux
   `--sanitize` leg CL-055 mandates still builds. One line, run.
3. **The message says only what the runtime knows.** *N lease(s) were live when
   the process died* is true on all nine measured paths; the current sentence is
   false on six. I do not insist on the wording, I insist that no path prints a
   sentence I measured false.

With 1 and 2 applied the C boundary is exactly where it was, and I withdraw the
veto. 3 is an objection, not a veto: a false message is a diagnostics defect,
not an ABI break, and design.md §4.17's standard is the ergonomist's to enforce.

## UNRUN, in those words

- **Windows is UNRUN.** The box is off and I did not start it. The prototype's
  `#else` stub means no report there; whether `stack.c`'s vectored-exception
  handler admits an analogue is **UNRUN**.
- **Linux x86-64 (`heroes-linux`) is UNRUN.** Everything above is arm64.
  The `variants.c` case-3 `139` and the glibc message strings are clang-22 and
  glibc-2.41 facts; per `LINUX-MACHINE.md` § How they differ from the judge, the
  exit codes are platform facts and carry to the CI's Ubuntu leg, the clang
  verdicts do not.
- **An externally delivered SIGABRT (`kill -ABRT`) is UNRUN**: `raise` is a
  reserved word and `q5_raise` would not build.
- **`q3_sqlite_later` on Linux is UNRUN**; it was not in the container script's
  list. Darwin only.
- **No timing was taken**, per the shared brief.
