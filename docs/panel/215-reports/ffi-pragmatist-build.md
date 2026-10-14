# Panel 215, the ffi-pragmatist builds Q2's route C, by type identity

Started 2026-10-14 11:34 (`date`), after reading the brief
(`215-briefs/build-q2-ffi-pragmatist.md`), the critic's pass 2 whole (§ 6 above all) and my own first
report. Copy: `.claude/worktrees/scratch-panel-215/ffi-build`, `git worktree add --detach ... 7e5c02048`
from the repository's root; `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` real 14.21,
user 13.66, sys 0.26, `heroes 0.4.0`, load 4.69. `TMPDIR` set to the copy's `build/` for every
command below.

(written as each command runs)

(The session restarted at about 12:30 and killed the first build of the route's compiler, exit 137;
the copy's state was read after it, `git diff --stat`: four files modified and three new, intact, and
the Windows folder `/c/w/p215-ffib` holding its four probes. Resumed 12:32, load 2.83.)

## 1. The C question before the compiler: three facts the critic's `ident.c` did not ask

Sources in the copy's `build/q2c/` (kept at the end under `215-reports/ffi-pragmatist-work/build/`).

**1a. clang's dump does not expand an array of records.** `clang -fsyntax-only -Xclang
-fdump-record-layouts dump.c` over `struct arr { int32_t k; struct counter many[2]; }` prints `8 |
struct counter[2] many` and nothing below it, where a nested `struct counter inner` is expanded
member by member. So a lock inside an array element has no path in the dump: the route asks
`many[0]`, and dumps `__typeof__(((T *)0)->many[0])` in a round of its own for what that element holds.
An anonymous union holding a mutex is expanded (`m`, promoted, path `m`).

**1b. A lock that is a typedef of a scalar cannot be told apart from an `int`, on two of the three
platforms, under the emitter's own `-std=gnu11`** (`edges.c`):

| platform | lock | its type | `__builtin_types_compatible_p(int, L)` | its POSIX feature macro |
|---|---|---|---|---|
| this Mac, Apple clang 21.0.0 | `sem_t` | `typedef int sem_t` (`sys/semaphore.h:43`) | **1** (and `struct plain`'s `int n`: 1) | `_POSIX_SEMAPHORES -1` |
| this Mac | `pthread_spinlock_t` | absent | (not a type) | `_POSIX_SPIN_LOCKS -1` |
| `heroes-linux-arm64`, Debian clang 22.1.8 | `pthread_spinlock_t` | `typedef volatile int` (`bits/pthreadtypes.h:103`) | **1**, and `volatile int` 1, `int32_t` 1 | `200809` |
| `heroes-linux-arm64` | `sem_t` | a union (`bits/semaphore.h:39`) | 0 | `200809` |

`__builtin_types_compatible_p` ignores top-level qualifiers, and a typedef is not a type in C. So the
brief's list, asked as written, **calls every `int` member of every group record a spinlock on glibc
and a semaphore on Darwin**, and refuses every copy of every such record. Under strict `-std=c11`
glibc hides `pthread_spinlock_t` while `_POSIX_SPIN_LOCKS` stays 200809, so the feature macro is not
the gate either. The route as built gates each lock in C: it is asked only where
`__builtin_classify_type` of a value of it is 12 (record) or 13 (union). That drops glibc's spinlock and
Darwin's `sem_t` from what the route can see; it is a fact of C, not of the list.

**1c. Windows: every lock is a record, and the question unit must include no header of its own**
(`winedges.c`, `rlwin.c`, `rlguard.c`, the box's clang 23.1.1, `-std=gnu11`):

- `classify CS 12 SRW 12 CV 12`, `mtx_t 12`, `cnd_t 12` (UCRT's `<threads.h>` exists there), `int 1`;
  `box.l~SRW 1`, `box.cs~CS 1`, `box.cv~CV 1`, `box.n~SRW 0`, `box.p~SRW 0`, **`SRW~CV 0`** (two
  one-pointer structs, told apart by identity), `int~mtx_t 0`.
- **A unit that includes `<windows.h>` after raylib's names does not compile**: `rl.h` holding the
  four declarations raylib.h makes that windows.h also makes, then `#include <windows.h>`: `wingdi.h:4639
  redefinition of 'Rectangle'`, `winuser.h conflicting types for 'CloseWindow'`, `'DrawTextA'`,
  `'ShowCursor'`, 4 errors, exit 1. The same unit with no include of its own, asking Windows' locks
  only `#if defined(_WINNT_)`: exit 0.

So route C's unit includes **only the program's own headers** and asks each lock first whether it is
a type there at all (`typedef L hero_lk_t_j;` at `#line j "hero_lk_ty"`): a lock the program's
headers never declare cannot be the type of a member they declare. No platform list, no guard
spelling, no header clash.

**1d. The unit reads cleanly by position** (`unit1.c`, this Mac): a member that does not exist and a
lock type the headers never declared each err once at their own position (`hero_lk_has:3`,
`hero_lk_ty:2` and `:3`), and clang says nothing at the identity lines that name them; the three yes
answers are `hero_lk_is:1` (`counter.lock`), `:17` (`arr.many[0].lock`) and `:21` (`anon.m`, through
the anonymous union).

## 2. The route as built

In the copy, against `7e5c02048`: three new modules and four touched, 583 lines in the new files by
`wc -l` after § 6's correction (95, 129 and 160 lines of code outside comments and tests; 575 and 158
before it), `git diff --stat` on the touched
four 107 insertions and 5 deletions. Built by today's compiler, `./heroes build selfhost/main.hero -o
build/heroes-c`, exit 0. Today's compiler below is `./heroes` (from the seed at `7e5c02048`); the
route's is `build/heroes-c`.

- `selfhost/emit/lock_screen.hero`: the lock list (the brief's eleven names), the identity unit and
  its reading. Per lock, at `#line j "hero_lk_ty"`, `typedef L hero_lk_t_j; extern hero_lk_t_j v;
  _Static_assert(__builtin_classify_type(v) == 12 || ... == 13)`; per question, at `#line q
  "hero_lk_has"`, `typedef __typeof__(((T *)0)->P) hero_lk_m_q;`; per (question, lock), at `#line
  q*11+j "hero_lk_is"`, `_Static_assert(!__builtin_types_compatible_p(hero_lk_m_q, hero_lk_t_j))`. A
  yes is an error at `is` where neither `has` nor `ty` erred; an error anywhere else means the headers
  do not compile alone and nothing is read. No header is included but the program's (§ 1c).
- `selfhost/cli/locks.hero`: at `build`, per header list (`cli/probing.hero`'s probings, so each
  module's records are asked of its own headers). The layout screen's answer is read from the cache
  `check_records` just wrote (`layout.screened`, added to `cli/layout.hero`): a record the screen vouched
  for is asked of its own type alone (its fields' records answer for its fields, the field assertions
  holding them to the header's); **every `partial` record and every record the screen flagged is
  dumped** and every member path asked; each array member's element is dumped in a further round,
  `__typeof__(((T *)0)->P[0])`, up to four. `layout_text.Member` gained `rank` (how many `[..]` its type
  text ends with), the one more character of a type's text read.
- `selfhost/check/lock_copies.hero`: the compiler-engineer's B prototype's positions
  (`compiler-engineer-work/pinned.hero`) unchanged in what they allow, its table seeded from clang's
  answer instead of `rec.pinned`, its diagnostics returned rather than pushed, the message naming the
  record, the lock's C type, its C path and the copy. Called from `cli/rounds.hero` after the layout
  check and before any unit compiles; `compile.Artifact` now carries the resolver's output, which the
  walk needs and which `build` did not keep.
- **What it is not**: `heroes check` asks clang nothing, so the route moves nothing at `check`
  (measured in § 4); no word, no spec token, no formatter change (`git diff --stat` touches no
  `selfhost/print/` file).

## 3. Each shape, today against the route

### 3a. This Mac (Apple clang 21.0.0)

Sources: `build/shapes/` (the compiler-engineer's `pin/mx.hero` and `mx.h`, issue 1335's copy; the
critic's pass-2 `race/copy.hero`, `counter.h`, `b.hero`, `race.hero`, `race.h`; `ctor.hero`,
`arr.hero`, `holder.hero`, `lend.hero`, `lend.h` written here), `build/kept/r01..r10/b.hero` with the
experiment's `counter.h`. Command per file: `<compiler> build <f> -o <bin>`, then the binary run.

| shape | today | the route |
|---|---|---|
| issue 1335's copy, `m2: Box @= m` (`mx.hero`) | exit 0 | **exit 1, 1 `ffi_lock_copied`** at `15:16` |
| the critic's `copy.hero` (by-value parameter, result, `d @= c`, `return d`, three calls and three assignments) | exit 0 | **exit 1, 10**, one per site, the same ten the B prototype counted |
| a counter returned by a constructor helper (`ctor.hero`) | exit 0 | **exit 1, 2**: the result (`8:10`) and `return c` (`11:12`) |
| a counter in a `[T]` (`arr.hero`: `[Counter(...)]`, `.push(Counter(...))`) | exit 0 | **exit 1, 2**, each element; `@cs[0]` itself is not refused |
| a Heroes record holding one, copied (`holder.hero`, `h2: Holder @= h`) | exit 0 | **exit 1, 1** at `15:19` |
| the critic's `b.hero` | `3` | `3` |
| the critic's `race.hero` (a C thread contending) | `200000 true` | `200000 true` |
| every `@` lend and field lend (`lend.hero`: `@c`, `@c.n`, `@h.c`, `@h.c.n`, a field read, a Heroes record holding one built in place) | `2 7 1` | `2 7 1` |
| the ten readers' `b.hero` | `3` each | `3` each |
| defect 413's six goldens, `heroes run` against each `.expected` (z_stream and libuv among them, held in arrays on purpose) | five `same`, the past-the-end case exit 134 by design | the same five `same`; the 134 case's output byte-identical to today's (`cmp`) |
| `examples/ctime` (`partial` `struct tm`, 30 harmless copies) | `same` | `same` |

The diagnostic, as it prints (`mx.hero`):

```
error[ffi_lock_copied]: this copies a `Box`, whose C struct `struct box` holds a `pthread_mutex_t` at `m`, which C forbids to copy
  at mx.hero:15:16
     |
  15 |     m2: Box @= m
     |                ^
  note: a copy of a lock is a second lock holding the first one's state: the copy of a held mutex reads held, and unlocking one leaves the other locked
  note: lend the place instead: a function taking `@c` receives the caller's own cell, `f(@c)`; a field is read in place, `c.n`
```

A record holding one names the chain (`holder.hero`): *this copies a `Holder`, whose field `c` holds
a `Counter`, whose C struct `struct counter` holds a `pthread_mutex_t` at `lock`, which C forbids to
copy*. A result adds *build it where it lives, `c: T @= T(...)`, then hand it to a function that
takes `@c`*; a by-value parameter carries a `guess` fix, `@c`, a guess because every caller must then
write `@`.

### 3b. Linux arm64 (`heroes-linux-arm64:latest`, Debian clang 22.1.8, glibc)

`build/linux.sh`, the copy mounted read-only and worked in the container's own `/w`: today's compiler
from the seed, the route's from its own emitted C (`build/heroes-c build selfhost/main.hero --emit-c -o
build/heroes-c.c`, 59,016,793 bytes, exit 0), both `heroes 0.4.0`. Output `build/linux-arm64.txt`.

| shape | today | the route |
|---|---|---|
| issue 1335's copy | `init 0`, `lock m 0`, **`trylock m2 16`**, `unlock m2 0`, `lock m2 0`, `unlock m 0`, `unlock m2 0`, both destroy 0: the copy of a held mutex reads held, exit 0 | **exit 1, 1 `ffi_lock_copied`** |
| `copy.hero` | `3` | **exit 1, 10** |
| constructor helper | `0` | **exit 1, 2** |
| a counter in a `[T]` | `02` | **exit 1, 2** |
| a Heroes record holding one, copied | `01` | **exit 1, 1** |
| `b.hero`, `race.hero`, `lend.hero`, the ten readers' `b.hero` | `3`, `200000 true`, `2 7 1`, `3` ×10 | the same |
| defect 413's goldens, `heroes run` against `.expected` | four `same` (z_stream among them), past-the-end 134 by design; **libuv `ffi_package`, exit 1: libuv is not in the image** | the same, each output byte-identical to today's (`cmp`) |
| `examples/ctime` | `same` | `same`, identical |

glibc's `pthread_mutex_t` is a typedef of an untagged union, and the route found it at `m` by
identity where the dump spells it `pthread_mutex_t` (my first report's dump) and Darwin's
`struct _opaque_pthread_mutex_t`: the critic's measurement, now through the compiler. **The libuv
golden under the route is run on this Mac only** (§ 3a).

### 3c. The Windows box (clang 23.1.1, `x86_64-pc-windows-msvc`, UCRT)

`/c/w/p215-ffib/tree`, both compilers built there with the seed's Windows line (`-Wl,/STACK:67108864`),
today's from the seed and the route's from the same emitted C; `build/win/ws.h` holds
`CRITICAL_SECTION`, `SRWLOCK` and C11 `mtx_t` each in a struct of the program's own, bound `partial`.
Output `build/windows.txt`.

| shape | today | the route |
|---|---|---|
| issue 1335's shape, a `CRITICAL_SECTION` copied while entered (`wcs.hero`) | **`count m2 -2`**: the copy reads entered, exit 0 | **exit 1, 1**: *this copies a `CsBox`, whose C struct `struct csbox` holds a `CRITICAL_SECTION` at `cs`* |
| the copying way with an `SRWLOCK`: by-value parameter, result, `d @= b`, `return d`, a call and its assignment (`wsrw.hero`) | `try 1`, then **`copy try 0`**: the copy of a held lock reads held | **exit 1, 6** |
| C11 `mtx_t`: a constructor helper and a `[T]` (`wmtx.hero`) | `0 1` | **exit 1, 3** (the result, `return b`, the element) |
| a Heroes record holding a `CRITICAL_SECTION` and an `SRWLOCK`, copied (`wholder.hero`) | `0` | **exit 1, 1**, the chain named |
| every lend kept (`wkept.hero`: `@x.c`, `@x.s`, a field write through `@b`, `@m`) | `2 1 5 0 3` | `2 1 5 0 3` |

Not run on Windows: the POSIX shapes (no `pthread.h` there), the readers' programs (their header is
POSIX), defect 413's goldens and `ctime`.

## 4. The census, the 905 files with an `extern`

The list: `git grep -l -E '^[[:space:]]*extern ' 7e5c02048 -- '*.hero' ':!docs'`, its prefix
stripped, **905** (`build/census-files.txt`). `build/census.sh <compiler> check|emit <file>` prints the
file, the exit and the diagnostic codes; `xargs -P 8`, each compiler in a working directory of its own
(`build/` is relative to it), so each run is cold and neither reads the other's cache.

### 4a. `heroes check`

| compiler | exit 0 | exit 1 | other |
|---|---|---|---|
| today | 732 | 173 | 0 |
| the route | 732 | 173 | 0 |

`diff` of the two sorted outputs: empty. **No file moves at `check`**, as built: the route asks clang,
and `check` asks clang nothing. (Timed, `real 24.30 user 35.13 sys 18.11` and `real 20.31 user 32.64
sys 17.67`, the load at 9.44 before and 14.39 after from other sessions' work: not a cost reading, and
not used as one.)

### 4b. `heroes build --emit-c`, cold, this Mac

| compiler | exit 0 | exit 1 | other |
|---|---|---|---|
| today | 366 | 538 | 1 (`selfhost/main.hero`, 124: the census's 300 s timeout) |
| the route | 366 | 538 | 1 (the same) |

`diff` of the two sorted outputs (file, exit, every diagnostic code with its count): **empty. No file
moves.** The compiler itself, emitted apart below, gives byte-identical C under both (`cmp`).

**The zero has a witness.** Across the corpus the route ran its identity unit in **135** layout
directories (5,769 questions) and clang said yes **once**: `hero_mutex`'s member `m`, a
`pthread_mutex_t`, in `tests/golden/run/fixedbugs-094-the-systems-mutex-initialiser-builds-a-record.hero`
(`fixedbugs-094-mutex.h`: `typedef struct { int32_t id; pthread_mutex_t m; } hero_mutex;`). That
program writes `m: hero_mutex @= HERO_MUTEX_INIT`, reads `m.id` and lends `@m`, so it keeps building,
and `heroes run` prints its `.expected` under both (`same`). Decoded from the kept answers by a script
reading `locks-said-*.txt` against its `locks-*.c` (no stray error in any of the 135). So the census
says what it can: **no program in the tree copies a record holding a lock**, and the one that holds one
is the shape route C must keep, and keeps.

By directory (the files, and how many exit 0 at `check` and at `--emit-c`, identical under both):

| directory | files | `check` 0 | `--emit-c` 0 |
|---|---|---|---|
| `tests/golden/run` | 320 | 318 | 301 |
| `tests/golden/unsupported` | 301 | 301 | 7 |
| `tests/golden/check` | 142 | 0 | 0 |
| `tests/golden/surface-fixtures` | 49 | 40 | 20 |
| `tests/golden/fixedbugs` | 40 | 40 | 8 |
| `examples/` (21 programs) | 21 | 21 | 21 |
| `selfhost/` (main, `cli` 6, `emit` 1, `module` 1, `mutate` 3) | 12 | 3 | 1 |
| `tests/golden/full` 5, `permissive` 4, `ir` 3, `emit` 1, `lldb` 1 | 14 | 5 | 3 |
| `tests/harness` | 5 | 5 | 5 |
| `archive/bootstrap-rs/heroes` | 1 | 0 | 0 |

## 5. The cost

**What the route dumps that today's screen does not**, counted in the route's cold corpus run, in the
135 directories where it asked its question (a script reading every kept dump's `| struct hero_ly_<n>`
heads): today's layout check dumped **47** records there; the route dumped **59**, the same 47 and
**12 more, every one a `partial` record no program builds from two or more fields** (12 directories);
plus **26 array-element wrappers**, in **68** dump runs over 50 directories (50 first rounds, 18 element
rounds). Kept dump text: 345,896 bytes today's, 442,898 the route's. And **135 identity units**, one per
header list with a record that has fields. The critic's count of 37 `partial` lines in 35 files is the
population; 12 is how many reach a build's layout check without already being dumped (the rest are in
files refused before it, or built from two or more fields).

**The compiler itself asks nothing**: `heroes build selfhost/main.hero --emit-c` cold under each made
**0** layout directories (its groups bind no record with fields), and the two C files are identical
(`cmp`). Its two timings, `real 182.13 user 133.94 sys 25.36` today and `real 216.63 user 145.91 sys
25.94` the route's, ran at load 9 to 44 from other sessions with `real` above `user + sys`: **discarded**
as a cost (CL-025); the route runs no clang and no walk for that program, which is the reading.

**The corpus, cold, `xargs -P 8`**: today `real 304.38 user 466.76 sys 221.25`, the route `real
304.28 user 524.01 sys 235.14`, the load 8.35 before, 15.49 between, 15.62 after (other sessions'
work). The difference, +71 s of user plus sys (+10.3 %), is a contended reading and **not** the cost.
**The route's own clang work, replayed alone** (`build/replay.sh`: every identity unit and every
route dump of the 135 directories, sequential, each with the response file the build wrote), twice:
**`user 11.03 sys 6.39`** and **`user 11.10 sys 6.46`**, about **17.5 s**; the layout check's own runs
in the same directories (screens, dumps, check units) `user 4.90 sys 5.09` and `user 5.13 sys 5.33`,
about 10.2 s; load 19 to 23. The identity units alone: `user 9.47 sys 5.02`. Against today's cold corpus
CPU (688 s, contended) the route's clang work is **about 2.5 %**, and **about 1.7 times** the layout
check it sits beside.

**Its worst case is defect 140's thousand-deep nest** (`tests/golden/run/fixedbugs-140-extern-records-a-thousand-deep-build.hero`):
no record dumped (the screen vouches for all), but its identity unit is **5,685,763 bytes, 2,032
questions, 22,352 assertion lines**, `user 2.11 sys 0.05` against its screen's `user 0.04 sys 0.01`.
Linear, not cubic; and eleven lines per question is this build's choice, not the route's: one
assertion per question OR-ing the eleven locks, the lock named only for the records that answer yes in
a second unit, would cut it about eleven times (**unbuilt**).

### 4c. `--emit-c` on glibc, and the brief's list asked without the gate

`build/linux-census.sh` in `heroes-linux-arm64` (the copy read-only, each compiler in its own working
directory, `selfhost/main.hero` left out as above: 904 files), three compilers built there: today's,
the route's, and **the route with its classify assertion removed** (`build/heroes-u`, the brief's
eleven names asked by identity alone; the source restored after, `cmp`). Outputs
`build/linux-out/emit-linux-*.s`.

| compiler | exit 0 | exit 1 |
|---|---|---|
| today | 360 | 544 |
| the route | 360 | 544 (`diff` against today: empty) |
| the route without the gate | 359 | 545 |

The one file the ungated list moves is **defect 413's zlib golden**, refused twice at line 36:

```
error[ffi_lock_copied]: this copies a `ZStream`, whose C struct `struct z_stream_s` holds a `pthread_spinlock_t` at `data_type`, which C forbids to copy
  36 |     zs: [ZStream] @= [ZStream(rest: zero), ZStream(rest: zero)]
```

`data_type` is zlib's `int`. That is § 1b through the compiler: the list as the brief words it, on
glibc, calls an `int` a spinlock and refuses the tree's own repair of defect 413, as B with `z_stream`
marked does (the critic's `z413`). It moves one file because the route asks member paths of
dumped records alone; a route that asked every field of every record would ask every `int` field on
Linux against the spinlock (an inference from § 1b, unrun as a census).

## 6. A binding the route refused, found in this build, and corrected

The B prototype's signature check, which this build kept, walked **extern declarations** too. So
`extern "byval.h"` with `function counter_make() -> Counter`, over `static inline struct counter
counter_make(void)` (`build/cannot/byval.h`), a binding that mirrors its header, was refused by the
route at the declaration (`byval.hero:5:14`), where today's compiler builds it and prints `0 41`. That
is a route unable to bind what today's compiler binds, my seat's veto line, and it was this build's,
not the route's: a C call's result is a fresh value, landing in a cell (`c: Counter @= counter_make()`),
which the position rule already allows, and what a call hands C by value is judged at its argument.
**Corrected** in `check/lock_copies.hero` (an extern's signature is skipped), rebuilt as
`build/heroes-c2`:

| program | today | the route as first built | corrected |
|---|---|---|---|
| `byval.hero`, `counter_make() -> Counter` bound and called | `0 41` | exit 1, the declaration refused | **`0 41`** |
| `byval2.hero`, `counter_peek(c: Counter)` bound, called with a cell | `3` | exit 1, two: the declaration's parameter and the argument | **exit 1, one: the argument** (`9:27`, *this copies a `Counter`* ...) |

Every Mac shape of § 3a and the census-witness files re-run with the corrected compiler: the same
counts and the same outputs, row for row. The Linux and Windows rows were run with the first build;
the correction reaches only an extern declaration whose parameter or result holds a lock by value,
and every extern member of those programs takes `@` and none returns a lock record (`grep` of the
indented `function` lines). The censuses had no `ffi_lock_copied` at all under the first build, and
the corrected one refuses a subset of what it refused; that census is **not re-run**, and the
sentence is an inference from the change.

## 7. What C cannot see, which is what B would add

Each run with today's compiler and the route's (corrected), on this Mac and in `heroes-linux-arm64`
(`build/cannot/`, `build/linux-arm64-cannot.txt`); every one builds at exit 0 under both:

| type | what goes wrong when copied | Mac | Linux arm64 | why identity cannot see it |
|---|---|---|---|---|
| zlib `z_stream` (`zstream.hero`, my first report's) | the copy's `deflate` and `deflateEnd` return **-2** (`Z_STREAM_ERROR`); the original 1, 0 | same under both | same under both | no lock: its `state` points back at the stream's own address, plain pointers to C |
| `<sys/queue.h>`'s `TAILQ_HEAD`, copied while empty, then pushed into (`tq.hero`) | **`copy first -1, original first 7`**: the push into the copy lands in the original, a wrong value at exit 0 | same | same | two plain pointers, `tqh_last` pointing into the head itself |
| glibc's `pthread_spinlock_t`, copied while held (`spin.hero`) | **`copy trylock 16`**: the copy reads held | (absent) | same under both | `typedef volatile int`: identity cannot tell it from `int` (§ 1b, § 4c) |
| Darwin's `sem_t` | (unnamed semaphores are not supported on Darwin; **unrun**) | | | `typedef int` |
| Darwin's `os_unfair_lock` (`unfair.hero`) | locked copy `trylock 0` (reads held); unlocked copy `trylock 1`, which `os/lock.h:59-63` documents as a fresh lock | same under both | (absent) | a struct, but on no standard's list; listed, it would refuse the documented unlocked copy |

And three things this build does not see by its own construction:

- **`heroes check` says nothing**: `build/heroes-c2 check` on `copy.hero` and on `mx.hero`, exit 0
  each; the refusal arrives at `build` and `--emit-c`, as `ffi_unknown_field` does. A reader working
  with `check` alone writes the copy and meets it later.
- **A lock spelled by a name the list does not hold, where the list's name is not visible**: the unit
  asks only names the program's headers declare; a header that reaches the mutex through a name such
  as Darwin's `__darwin_pthread_mutex_t` alone is **a question, unrun**.
- **Arrays of records more than four element rounds deep** are not followed (`ROUNDS`); **unrun** on a
  real header.

So B's mark would add the five rows above, each a type whose binding's author must know; and the
critic's `z413` stands beside it: `z_stream` marked under B refuses defect 413's own repair. **C sees
the standards' locks that are records, on all three platforms, and nothing else.**

## 8. The verdict, on what was built

- `verdict`: **approve route C by type identity, as built and corrected here**, as Q2's rule for the
  standards' locks; it is not the brief's wording, which I **object** to on three measured counts
  (below). No veto: the route as corrected binds every header today's compiler binds (both censuses,
  `byval.hero`), and changes no byte of emitted C for a program it accepts (21 of 21 examples, the
  compiler itself, `cmp`).
- `section`: design.md §1.11 (FFI ergonomics alongside comprehension; signatures verified by clang
  against the real header), §1.12 (robustness), §4.19 (*a group's `record` is the header's struct*),
  §4.17 for the message. design.md says nothing of a C type that must not be copied (my first report's
  `grep`); the nearest rule is spec § 13's *the place does not move*.
- `experiment`: § 1 (`edges.c`, `winedges.c`, `rlwin.c`, `rlguard.c`, `unit1.c`, clang accepting each
  as reported), § 3 (five refused and every kept shape on this Mac, Linux arm64 and the Windows box),
  § 4 (905 files, `check` and cold `--emit-c`, this Mac; 904 on glibc with the ungated variant), § 5
  (dumps counted, the route's clang work replayed), § 6 (a binding refused and corrected), § 7.
- `argument`: Identity answers the "three spellings" objection on all three platforms, but the brief's
  route has three defects, each measured: a lock that is a scalar typedef makes every `int` a lock
  (glibc's spinlock refused defect 413's zlib golden), so only record and union locks can be asked; a
  unit including lock headers breaks raylib on Windows, so it may include only the program's own; and
  refusing extern signatures refused `counter_make() -> Counter`. Corrected, it refuses every copy
  shape on three platforms, keeps every lend, moves zero of 905 files, costs about 2.5 % of a cold
  corpus build, and adds no word, so nothing a formatter can drop. It cannot see `z_stream`,
  `TAILQ_HEAD`, the spinlock, `os_unfair_lock`.
- `prediction`: a landing of route C with § 1's two gates and § 6's exemption moves **0** of the 905
  files at `--emit-c` on Darwin and glibc (`diff` of the census as § 4), refuses `wcs.hero`,
  `wsrw.hero`, `wmtx.hero` and `wholder.hero` on the Windows box while `wkept.hero` prints `2 1 5 0 3`,
  and leaves `examples/raylib` and `examples/sqlite`'s emitted C byte-identical; scored by `emission`
  and this census at the landing. Asked as the brief words it (no gate), it moves
  `fixedbugs-413-zlib-knows-a-stream-by-its-address.hero` on glibc, the one-file move § 4c measured.
- `condition`: I turn to **object** to C if a landing includes any header in its question unit that
  the program did not (the raylib clash), asks a scalar-typedef lock by identity, or refuses an extern's
  declaration; to **veto** if any of those reaches the trunk, since each makes a correct binding
  unbuildable. I would add B (a mark) beside C, never instead of it, only for § 7's rows, and only once
  its rule keeps defect 413's arrays legal, which the critic measured B as prototyped does not.

## 9. What I could not run

- The libuv golden on Linux (`ffi_package`: no libuv in the image; run on this Mac only).
- The census on the Windows box; the POSIX shapes there (no `pthread.h`).
- macOS x86-64 and Linux x86-64 (no route-specific reason to expect a difference; **unrun**).
- A clean cost reading of either cold corpus build: every timing here ran at load 8 to 44 from other
  sessions; the replay of the route's own clang runs is the cost figure, and it too ran at load 19 to 23.
- The OR-ed identity unit that would cut defect 140's 5.7 MB unit (**unbuilt**); a header reaching a
  lock only through an internal name; arrays nested more than four element rounds.
- The full net, the compiler's own tests and the formatter's probe over the route's compiler (it is a
  build for measurement, not a landing).

Files: the copy's sources, scripts and outputs are kept under
`215-reports/ffi-pragmatist-work/build/` (no binary); the copy and the Windows folder are removed at
the end, below.

**Cleaned up, 13:40**: `ssh win 'rm -rf /c/w/p215-ffib; ls /c/w'` printed `b17-ffi` alone (another
session's folder, untouched); `git worktree remove --force .claude/worktrees/scratch-panel-215/ffi-build`
exit 0 (two other seats' copies, `ce-build` and `price`, stay where they are). Nothing kept is a binary
(`file` over the 64 kept files: no Mach-O, ELF or PE). **A slip of mine**: one shell line aimed a
`heroes fmt` redirect at `/tmp/..`, the filesystem root; it was refused by permissions and wrote
nothing (`ls /` holds no such file). No other file was written outside the repository's root; the
containers' `/w` was each container's own.
