# Panel 195, ffi-pragmatist (the C boundary and the platforms)

Started 12:22:28 (`date`). Copy: `<scratchpad>/195-ffi-pragmatist/`, from
`git archive 96f3a588 | tar -x`, `round-b12/run-4254/seed-new.c` over
`seed/heroes.c` (sha1 2687685540fee01dcd2bb5042bcf226182fbdd75, the critic's).
Instruction counts only, no durations. Findings are added below as they are run.

## Written 12:23 — the setup

- Seed compiler: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-seed`,
  exit 0. Branch compiler: `./heroes-seed build selfhost/main.hero -o heroes` (running).
- The compile flags the C here is judged under are `selfhost/cli/flags.hero`
  `flags()`, sixteen, read from the copy: `-std=gnu11 -g -D_USE_MATH_DEFINES
  -D_CRT_SECURE_NO_WARNINGS -Wall -Werror=return-type -Werror=uninitialized
  -Werror=format -Werror=conditional-uninitialized -fno-strict-aliasing
  -fno-delete-null-pointer-checks -Werror=shorten-64-to-32 -Werror=sign-conversion
  -Werror=incompatible-pointer-types-discards-qualifiers
  -Werror=incompatible-pointer-types -fsigned-char`; link `-rdynamic` (POSIX),
  `-Wl,/STACK:67108864 -Wl,/INCREMENTAL:NO` (Windows); `--sanitize` adds
  `-fsanitize=address,undefined`. To these every hand-written file below adds
  `-Wextra -pedantic` once, so a warning the route's C provokes is seen.

## Written 12:30:44 — this Mac (Darwin arm64, Apple clang 21.0.0): every route's C, built and run

Branch compiler built 12:22:52-12:24:45 ("wrote heroes", exit 0). Its `--emit-c`
of `k.hero` (the brief's reproducer), of `ks.hero` (mine: the same loop over a
32-element `[str]` constant, summing `K[at % 32].len()`) and of the critic's
`spawnk.hero` is the base; `x/gen.py` rewrites only the constant's function
(and, for the element route, the read) into each route's C. `x/rtg/` is the
runtime with ir12's one line added to `hero_array_incref` and
`hero_array_decref` (`diff` is those two lines). Every file compiled with the
sixteen flags of `flags()` plus `-Wextra -pedantic -fsyntax-only`: **no warning
in any route's C** but two `-Wunused-variable` in the element route, from
temporaries my hand edit left declared (the emitter would not write them).
Built `x/cc.sh` (the sixteen flags, `-rdynamic`, the program and `runtime.c`
in one clang), run, `/usr/bin/time -l` instructions retired; every row printed
the right sum and exited 0 (the leak check passed):

| route | `[i64]` -O0 | `[i64]` -O2 | `[str]` -O0 | `[str]` -O2 |
|---|---|---|---|---|
| landed (`f7576a01`), today's runtime | 4,600,830,901 | 2,578,634,156 | 8,520,431,718 | 3,395,321,656 |
| static C table + one memcpy, today's runtime | 1,717,040,343 | 934,066,906 | 3,640,008,152 | 1,409,006,600 |
| one copy per thread (`_Thread_local`), today's runtime | 194,614,927 | 88,993,237 | 220,612,866 | 93,623,019 |
| **one process-wide heap copy, CAS-published, the cell holding one count**, today's runtime (unlisted: `onceheld`) | 174,738,830 | 78,504,420 | 200,505,844 | 83,679,239 |
| one process-wide copy, count -1 sentinel (critic's route 2), guarded runtime | 154,755,363 | 69,480,821 | 180,096,425 | 74,364,758 |
| static `const` block, count -1 (ir12), guarded runtime | 141,684,878 | 52,449,198 | 167,736,104 | 57,692,004 |
| static writable block, count -1 (critic's route 3), guarded runtime | 141,737,740 | 52,502,746 | 168,120,058 | 57,655,298 |
| the name hoisted (`local.hero`, `ksl.hero`: the floor) | 116,605,066 | 39,062,990 | 142,600,286 | 43,424,953 |
| **`K[i]` on a static C array of elements** (critic's route 1), today's runtime | **79,700,547** | **23,425,678** | **105,485,398** | **28,436,519** |

My `landed` at -O0 is 4,600,830,901 against the brief's 4,597,018,435 and the
critic's 4,599,763,643, so the hand compile is the compiler's. The element
route is **below the floor**: it calls no `hero_array_at` at all, the fixed-array
guard `storageless.hero:109` already emits standing in for it.

`onceheld` is a route nobody listed, built because the critic's route 2 said it
"still needs the guard": it does not, if the cell holds a real count. The read
is an acquire load, a CAS on the first read, then `hero_array_incref`; `main`
releases the cell's count before `hero_runtime_check_leaks()`. No count is
negative, no runtime line changes, no page is read-only.

### A release too many (one extra `hero_array_decref(h3_own3)` before the function's own, `-O0`)

| route | plain | `-fsanitize=address,undefined` |
|---|---|---|
| landed | exit 0, silent | exit 134, `heap-use-after-free` |
| memcpy | exit 133, SIGTRAP (runtime's message) | exit 134, `heap-use-after-free` |
| per thread | exit 0, silent | exit 134, `heap-use-after-free` |
| **onceheld** | exit 0, silent | **exit 134, `heap-use-after-free`** |
| oncesent (count -1) | exit 0, silent | **exit 0, silent** |
| static const (count -1) | exit 0, silent | **exit 0, silent** |
| static writable (count -1) | exit 0, silent | **exit 0, silent** |

Every count -1 route makes the emitter bug invisible to `--sanitize`; every
route whose block is heap memory with a real count keeps ASan's name for it.

### The static block with TODAY's runtime (no guard), and where it lands

    nm -m: static const block   (__DATA_CONST,__const) _h_k_K_block, _h_ks_K_block
           static writable      (__DATA,__data)        _h_k_K_block
           element table [i64]  (__TEXT,__const)       _h_k_K_elems
           element table [str]  (__DATA_CONST,__const) _h_ks_K_elems
    k_sconstNOGUARD -O0, -O2:  exit 138 (SIGBUS), stdout and stderr EMPTY
    k_srwNOGUARD    -O0:       exit 134, "panic: the process is dying of SIGABRT,
                               raised by this process, under k.total ... memory
                               given back twice among them"
    k_srwNOGUARD    -O2:       the same under k.main

**The critic's route 3 is falsified here**: a writable static block with a
missed guard does not "move a count", it is handed to `free()` (the decref reads
-1, `fetch_sub > 1` is false, the block is doomed, `drop.c` releases it), and
Darwin's malloc aborts on a pointer it never gave out. Loud here only because
this allocator checks; freeing a non-heap pointer is undefined behaviour in C.

## Written 12:36 — sanitizers on this Mac, the binding's words, the stamp

**ASan+UBSan** (`-O0 -fsanitize=address,undefined`), every route of `k`, `ks`
and `spawnk` (two `hero_thread_spawn` workers and `main` reading `K`): all exit
0 with the right sum, **except one copy per thread on `spawnk`: exit 134,
"panic: 2 heap blocks still live at exit"** — the two workers' copies, which
nothing releases without the registry ir12 named. **TSan** (`-O1
-fsanitize=thread`, `spawnk`, every route): 0 warnings, all exit 0 but the same
per-thread leak. To exercise the process-wide route's lost race I spun 2*10^7
iterations before the build (`v/spawnk_onceheld_race.c`): **"lost 2"** on 3 of
3 runs (three builds, one published, two released), TSan 0 warnings, ASan
clean, leak check passed.

**A binding's header** (defect 361): `x/hostile.h` defines `e`, `b` and `h`, a
program binds it, the unit includes it between `heroes_guard_open.h` and
`heroes_guard_close.h`. `heroes_guard_open.h` pushes `h` and `b` (and `len`,
`cap`, `elem`, `refcount`, `HeroArrayHeader`, `hero_desc_int`, `UINT64_C`,
`hero_panic`: `grep -c 'push_macro("<w>")'` = 1 each) and **not `e`** (0). Built:
landed, static block with members `h`/`b`, element route, `onceheld`: compile
and print 4843751. **ir12's spelling, members `h`/`e`: clang error, "expected
member name or ';' after declaration specifiers"** — a program binding such a
header could not be built at all. A search of this Mac's SDK and
`/opt/homebrew/include` for `#define e`/`b`/`h` found none (so the case is
built, not met); the words are what defect 361's rule covers either way.

**The stamp**: `x/rtm/` is the guarded runtime plus a macro
`HERO_ARRAY_STATIC(name, desc, T, n, ...)` (members `h` and `b`) and
`HERO_RUNTIME_ABI 28`; `v/k_smacro.c` is the static route written through it.

    new C (macro, 28)   vs runtime 28   compiles, prints 4843750, exit 0
    new C (macro, 28)   vs runtime 27   clang exit 1: '27 == 28': heroes_runtime.h is from another compiler
    new C (macro, 27)   vs runtime 27   clang exit 1: "type specifier missing" (the macro is absent)
    old C (landed, 27)  vs runtime 28   clang exit 1: '28 == 27': heroes_runtime.h is from another compiler
    ir12's raw struct   vs runtime 27   compiles, then SIGBUS exit 138, nothing printed (above)

So the skew is silent **only** when the emitter writes the struct itself; through
a runtime macro it is a compile error even with the stamp unmoved, and with the
stamp moved it is the stamp's own sentence. The compiler finds its runtime from
`HEROES_RUNTIME`, the working directory or beside itself (`help.hero:45`), so
a compiler and a runtime from different commits meeting is an input channel,
not a hypothesis.

**The guard's price on every program**, landed route, today's runtime against
`rtg/`: `k` -O0 4,597,626,304 → 4,616,345,551 (+18.7M, +0.41%), -O2
2,577,642,655 → 2,583,411,583 (+0.22%); `ks` -O0 +0.18%, -O2 +0.17%. Two runs
of the same `k_landed_O0` binary read 4,600,830,901 and 4,597,626,304, so the
noise is about 3M.

## Written 12:36 — Linux arm64 (`heroes-linux-arm64`, Debian clang 22.1.8), the same C

`docker run --rm -e SAN=1 -e TSAN=1 -v <my copy>:/w heroes-linux-arm64 bash
/w/x/leg.sh bl`, 12:34:47-12:35:26, output `x/linux-leg.txt`:

- Every route of `k` and `ks` at -O0: right sum, exit 0. ASan+UBSan on `k`,
  `ks`, `spawnk`: all clean but per-thread `spawnk` (exit 134, "2 heap blocks
  still live"). TSan on `spawnk`: 0 warnings on every route (the same leak on
  per-thread); the forced race: "lost 2", clean.
- **Sections** (`objdump -t`): static const block `.data.rel.ro` (inside
  `GNU_RELRO`, `readelf -l`: 0x2fa08+0x5f8 covers it; no `BIND_NOW`), static
  writable block `.data`, `[i64]` element table `.rodata`, `[str]` element
  table and `[str]` block `.data.rel.ro`.
- **No guard**: static const → **SIGSEGV, exit 139, stdout and stderr empty**;
  static writable → **glibc `free(): invalid pointer`, exit 134**.
- **A release too many**: landed, memcpy, per-thread, `onceheld`: silent plain,
  ASan `heap-use-after-free` (exit 1). Static const, static writable,
  `oncesent`: **silent under ASan too**, exit 0. The same split as Darwin.
- Hostile header: the same (`e` breaks the build, `h`/`b` survive). Stamp: the
  same three lines.
- **Instructions retired: UNRUN on Linux.** `perf` and `valgrind` are absent
  from the image; my own `perf_event_open` counter (`x/perfcount.c`) gets
  "Operation not permitted", and with `--cap-add PERFMON` "No such file or
  directory": the Docker VM exposes no hardware instruction counter.

## Written 12:42 — Windows (`ssh win`, MINGW64, clang 23.1.1 x86_64-pc-windows-msvc), and the shapes

Box at 12:37: `df -k /c` 23,663,992 KB free, four `heroes.exe` of the batch 12
leg running, `/c/w/b12-*` present and untouched. My folder
`/c/w/p195-ffi-82595` (made 12:36, did not exist before), filled by streaming a
572 KB tarball of `runtime/` and my hand-written C; nothing removed. One clang
at a time (`x/leg.sh`, sequential), 12:36-12:39, output `x/win-leg.txt`:

- Every route of `k` and `ks` at -O0 compiles (`__atomic_*` builtins and
  `_Thread_local` included) and prints the right sum, exit 0.
- **Sections** (`llvm-readobj --symbols` on the object): static const block
  `.rdata`, static writable `.data`, element tables `.rdata`.
- **No guard**: static const → **access violation, exit 139, nothing
  printed**; static writable → "panic: the process is dying of exception
  0xC0000374, the heap manager's report", exit 127.
- **A release too many, PLAIN build**: landed, memcpy, per-thread, `onceheld`
  → exit 127, the heap manager's 0xC0000374 with the runtime's message. Static
  const, static writable, `oncesent` → **exit 0, silent**. So on Windows the
  count -1 routes lose a loudness today's heap blocks have **without any
  sanitizer**. ASan (`-fsanitize=address`; UBSan not tried there): landed and
  `onceheld` `heap-use-after-free`, static const and `oncesent` exit 0 silent;
  `spawnk` static const, `onceheld`, element route clean.
- Hostile header and the stamp: the same as Darwin and Linux.
- The race: with a 2·10^7 spin the box never overlapped (lost 0, 3 of 3, on
  two loaded cores), so I replaced the spin with a barrier (all three first
  reads wait in the build until three have arrived, `v/spawnk_onceheld_barrier.c`):
  **lost 2 on 3 of 3 on Windows, Darwin and Linux**, TSan 0 warnings on Darwin
  and Linux, leak check passed everywhere.
- **Instructions retired: UNRUN on Windows** (no counter in Git Bash).

**Shapes beyond a literal array, through `onceheld` alone** (`x/onceall.py`
rewrites every constant function of a unit, arrays and maps):
`fnk2.hero` (`constant STEPS: [(function(i64) -> i64)]`, spawn bound): 25,
exit 0 on Darwin (ASan clean), Linux (ASan clean) and Windows; the two
`hero_thread_guard("fnk2.twice")`/`("fnk2.thrice")` lines are still there,
because the builder is the IR's own function and keeps its `funcref`s.
`mapk.hero` (a map with `BASE + 2`, an `if` body, an f-string element, and
`FROM` indexing out of range): 42, 80, a40b, then `panic: array index out of
range` at `FROM`'s read, exit 134 on Darwin and Linux (ASan otherwise clean),
127 on Windows, which is what the landed route prints and exits there.

**A real binding** (`sq.hero`, `extern "sqlite3.h" link "sqlite3"`, this Mac):
landed, static const with `h`/`b`, with `h`/`e`, element route and `onceheld`
all compile against the real header and print `4843750 true`. No header I
searched defines `e`, `b` or `h` (this Mac's SDK, `/opt/homebrew/include`, the
Linux image's `/usr/include`): `e` is a breach of defect 361's discipline, not
a failure met in the wild.

## Written 12:43 — verdicts

Ratios to the hoisted floor, this Mac, `[i64]` -O0 / -O2 (`[str]` -O0 / -O2):
element route 0.68 / 0.60 (0.74 / 0.65); static const 1.22 / 1.34 (1.18 / 1.33);
`oncesent` 1.33 / 1.78; `onceheld` 1.50 / 2.01 (1.41 / 1.93); per thread 1.67 /
2.28; memcpy 14.7 / 23.9; landed 39.5 / 66.0 (59.8 / 78.2).

No route puts an array header across the C boundary: spec § 13 admits no `[T]`
as an `extern` parameter or field, and `hero_os.h`'s spawn comment says the
checker refuses one in a callback. So no route breaks a binding's C ABI; what
the routes touch at the boundary is defect 361's word list, the stamp, and
§1.12's last paragraph (a defensive check that hides a defect).

| route | verdict | section | cost (measured) | falsifiable prediction | condition |
|---|---|---|---|---|---|
| `K[i]`, `K.len()`, `for x in K` on a static C array of elements (literal body, scalar or `str` elements) | **approve** | §1.11, §1.12, §4.19 | below the floor; no runtime line, no header word, no stamp; the guard text `storageless.hero:109` already emits | a compiler with it builds `k.hero` to ≤ 0.75x `local.hero`'s instructions at -O0 on this Mac, and `heroes_runtime.h` is byte-identical | the index abort keeps the text `array index out of range` (exit 134 today); a whole-value read keeps a builder |
| one process-wide heap copy, CAS-published, **the cell holding one real count** (`onceheld`, unlisted) | **approve** | §1.12, §4.19, §4.20 | 1.50x / 2.01x the floor; no runtime line, no header word, stamp stays 27; atomic RMW on one shared line per read (cycles unmeasured) | `spawnk` and the forced race run clean under `--sanitize` on Darwin and Linux, and the one-extra-release reproducer is `heap-use-after-free` under `--sanitize` on all three and 0xC0000374 plain on Windows | `main` releases every cell before `hero_runtime_check_leaks()`; the builder is the IR's own constant function, so `funcref` and the callback guard stay |
| static `const` block, count -1, guard (ir12), **as written** (members `h`/`e`, emitter-written struct, stamp 27) | **veto** | §1.12 (both "must not segfault" and its last paragraph), defect 361 | skew with a runtime without the guard is SIGBUS 138 / SIGSEGV 139 / AV 139 with nothing printed on all three; member `e` is not guarded: a header defining it makes the unit unbuildable; a release too many is silent under ASan on all three and plain on Windows | a compiler emitting it, pointed by `HEROES_RUNTIME` at a runtime-27 tree, builds `k.hero` and the binary dies with an empty stderr | lifted to object if the block is written through a runtime macro whose members are `h`/`b` (`x/rtm/`: `HERO_ARRAY_STATIC`), its name added to `NAMES`, and the stamp moved 27 → 28 because a declaration was added (CL-007 holds: the stamp covers a declaration); still object, not approve, while the release-too-many stays invisible |
| static writable block, count -1 (critic's route 3) | **veto** | §1.12 | a missed guard is `free()` of a non-heap pointer: Darwin malloc abort 134, glibc `free(): invalid pointer` 134, Windows 0xC0000374 127 (C undefined behaviour, loud only because three allocators check) | — (measured) | none: the premise "a count moved rather than a SIGBUS" is false on three platforms |
| process-wide copy, count -1 sentinel (critic's route 2) | object | §1.12 last paragraph | 1.33x / 1.78x; needs the guard (the skew above); release too many silent under ASan on all three | the extra-release reproducer exits 0 under `--sanitize` | the count held, not a sentinel (= `onceheld`) |
| one copy per thread (`_Thread_local`) | object | §1.12, §4.20 | 1.67x / 2.28x; `spawnk`: "2 heap blocks still live at exit", exit 134, Darwin and Linux | the registry it needs adds a thread-exit hook to `spawn.c`'s two arms and a declaration | a registry built and run on three platforms; `onceheld` does the same without one |
| static table + one memcpy | approve at the boundary, not preferred | §4.20 | 14.7x / 23.9x; emitted C writes `a->len`; for `[str]` skipping the per-element incref is sound only because every element is a count -1 literal | an f-string element under it would be a missing incref | scope it to literal elements, or do not take it |
| the read hoisted by the lowering | no boundary objection | — | the floor where it applies; a read in a callee of a loop is not hoisted | — | — |
| nothing more (`f7576a01`) | no boundary objection, not the repair | — | 39.5x / 66.0x the floor | — | the two approved routes are equally clean at the boundary |

## Written 12:44 — one check after the table

The unguarded static `const` block, re-run for its stderr alone: Linux exit 139,
**0 bytes**; Windows exit 139, **0 bytes** (Darwin 138, empty, at 12:30). The
"nothing printed" in the veto row is measured on all three. Report closed.
