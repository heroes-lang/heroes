# Panel 215, ffi-pragmatist

Started 2026-10-14 08:35:00 (`date`). Seat sections: design.md §1.11, §1.12, §4.19, §4.20
(each cited below by the line `grep -n` gives in this copy's `docs/design.md`).

Copy: `.claude/worktrees/scratch-panel-215/ffi-pragmatist`, made by `git worktree add --detach ...
7e5c02048`; `shasum -a 256 seed/heroes.c` opens `0f2a62cf4596661e`; `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` real 8.95 s, `heroes 0.4.0`. This Mac: Apple clang 21.0.0.

Read before measuring: `00-shared.md`, `ffi-pragmatist.md`, the critic's pass 1 and its `pad/`,
`taskA/`, `taskB/` files, the milestone page's paragraph on 1333 to 1335
(`docs/roadmap/milestones/M-buildable-structs.md:75-96`), measurement 044, spec § 13 whole, design.md
§1.11, §1.12, §4.19 and §4.20's record and `eq`/`hash` paragraphs, `.claude/rules/c-boundary.md`.

Sources and transcripts: `215-reports/ffi-pragmatist-work/` (every file named below is there).

## In short, most consequential first

1. **Issue 1334 is not an `-O0` defect.** Heroes builds one unit per module with no LTO; with the record
   passed through another module, libc's `write`, `send`, `memcmp` and OpenSSL's `SHA256_Update` read
   uninitialised gaps at `-O0` to `-O3` on Linux x86-64 (21/5/10/10 MSan reports), and built plain at `-O0`
   the program prints `eq true` beside `sha false` for the same two records. Linux arm64: 0. The loss is
   SysV x86-64's ABI (`{ i8, double }`), and pure C loses the gap the same way after a `memset`.
2. **Q1: G** (the critic's route, gaps zeroed in place at each lend, clang-checked `offsetof` arithmetic)
   reads 0 at every level; so do C and F, at a wider cost. **D is refused** (zeroes a `partial`
   `SHA256_CTX`'s state with a success code, strands `TAILQ`'s kept pointer, leaves the runtime's own
   audit reading the gaps); **E is vetoed** (`ffi_unknown_field`, or the record stops being the header's type).
3. **Q2: B**, the binding's mark, changes no emitted C; **C only as a note**: no header on three
   platforms marks a type non-copyable, the dump spells each lock three ways, and a list is blind to
   self-referential structs. A mutex, `mtx_t`, `CRITICAL_SECTION` or `SRWLOCK` copied while held reads held in
   the copy, and the program runs on to exit 0 with no message on every platform that has it; zlib alone
   catches its own copy.
4. **Q3: R1 stands.** All three census structs build and run with and without `rest: zero` on Darwin
   and glibc, and `sockaddr` and `fd_set` on Windows (no OpenSSL there); it saves 67 to 189 cl100k per program and changes no call C sees.
5. **No route adopted here breaks the C ABI**; `HERO_RUNTIME_ABI` moves for none of C, F, G; callbacks
   already cross through a wrapper of the header's own type.

(written as each command runs)

## Where the bytes are lost: the emitted C, read

`./heroes build small.hero --emit-c -o small.c` on the critic's probe (exit 0):
`struct cd h_small_cd_through(struct cd h0_x) { ... t1 = h0_x; return t1; }` (`small.c:196-205`) and
the lend `t7 = w_cd(&h2_b);` (`small.c:271`). The extern `w_cd` receives the address of the cell;
the record crossed by value only between two Heroes functions.

## Q1, padding across a call by value (issue 1334)

### 1.1 Where the bytes go is the platform's ABI, and pure C loses them the same way

`q1/abi.c` (four padded structs returned and taken by value), `clang --target=<t> -O0 -S -emit-llvm`
(`q1/abi-lowering.txt`): on `x86_64-linux-gnu` `struct {char c; double d;}` crosses as
**`{ i8, double }`**: its 7 padding bytes are not part of what a call carries. On `aarch64-linux-gnu`
and `arm64-apple-macos` it crosses as `[2 x i64]`, on `x86_64-pc-windows-msvc` through memory: the bytes
travel. `{char; int}` and `{float; char}` cross as one `i64` everywhere; a 32-byte record goes `byval`/`sret`
everywhere. So the class is SysV x86-64's, for a record of 16 bytes or less whose eightbyte ends in padding.

**Pure C, no Heroes** (`q1/purec_a.c`, `purec_b.c`, two translation units; the record zeroed whole by
`memset` first, CERT DCL39-C's first remedy, then passed and returned by value through the other unit and
written with `write`; run by `q1/purec.sh` in both containers, Debian clang 22.1.8, `q1/purec-msan.txt`):

| leg | -O0 | -O1 | -O2 | -O3 |
|---|---|---|---|---|
| `heroes-linux` x86-64 | 2 | 2 | 2 | 2 |
| `heroes-linux-arm64` | 0 | 0 | 0 | 0 |

The two reports at `-O2` (`q1/purec-msan-O2-frames.txt`): `main purec_a.c:12` (the `write` after the
call) and `take_cd purec_b.c:4` (C's own by-value parameter, written by C). Lines 10 and 15 (the same
record written before any call, and by address) are clean. **No construction-side zero survives a
by-value call on x86-64, in C itself, at any level**, which is the critic's finding on route B stated
without Heroes in it.

### 1.2 Heroes modules are separate units, so 1334 is not an `-O0` defect

`heroes build` writes one translation unit per module (`build/tu-*/main.c`, `rec.c`, `library.c`) and links
them without `-flto` (`build/words/*.rsp`, read). The critic's probe is one module, so clang inlines
`cd_through` at `-O1` and up; that, not the language, is what read clean there.

`q1/probe/` (`main.hero`, `rec.hero`, `wire.h`): `rec.through(x: Cd) -> Cd` in its own module; the
program's one construction ends `rest: zero`; three shapes, each then read as bytes by **libc's `write`
and `send` (a `socketpair`), libc's `memcmp`, and OpenSSL's `SHA256_Update`**, all bound against the real
headers with today's spelling `@buf: rec.Cd counted_by n lent` (`q1/spellings.txt`: the bare `@buf: Cd`
is `ffi_parameter_type`, and `[Cd]` against `const void *` is refused, its element held to `u8`); a record against zlib's `crc32(uLong, const Bytef *, uInt)` is `ffi_parameter_type` too (`q1/crc-run.txt`), so the byte-readers a record reaches today are the `void *` ones (`q1/zs-run.txt`: `SHA256_Update` prints `1 1`):
S0 a lent local filled by the call by value; S1 an array element pushed from it, copied into a lent
local; S2 a field of a Heroes record built from it, copied into a lent local. Script
`msan_units.sh`: a compiler made from the seed in the container builds the program, then the very
units it wrote are compiled plain and under `-fsanitize=memory -fsanitize-memory-track-origins=2` at
four levels (`q1/routes-<leg>-none.txt`; the full reports `q1/msan-kept/none-m-O*.txt`): the origins below were read from each report's copying frame: `tmp` through line 37, `tmp14` through 40, `tmp24` through 43 at `-O0`; `t10.sroa.4` through 37 and `t14` through 41 at `-O2`.

| x86-64 (`heroes-linux`) | -O0 | -O1 | -O2 | -O3 |
|---|---|---|---|---|
| MSan reports | 21 | 5 | 10 | 10 |
| origins, each mapped to the `.hero` line whose copy carried it (an `awk` over the kept reports) | `tmp`, `tmp14`, `tmp24` (S0, S1, S2) | `t14` (S1) | `t10.sroa.4` (S0), `t14` (S1) | the same |
| the program's own output, built plain | **`eq true true true`, `memcmp 0 -122 -15`, `sha true false false`** | all equal | all equal | all equal |

Linux arm64: 0 reports at every level, every output equal (`q1/routes-arm64-none.txt`). The `-O0` plain
line was read three times and moved each time (`memcmp 0 -224 0`, `0 -121 -15`, `0 -122 -15`): **two
records `==` calls equal give two different SHA-256 digests and `memcmp` nonzero, at exit 0, with no
sanitiser**, and what decides it is the stack's leftovers. Above `-O0` the readers are `write`, `send`,
`memcmp` and the runtime's own `hero_lend_audit` (`runtime/parts/lend.c:302`, which checksums every byte
of a lent region, `hero_lend_sum`, when it is given back and again at exit). The two-module probe of the
critic's shape alone (`q1/probe/q1m-*`, `q1/two-module-routeA-B.txt`) reads 5 reports at `-O0` and 0
above: whether the gap reaching C is the lend region's zero (`heroes_runtime.h:725-728`: a lent local is
zeroed in the prologue) or the stack's bytes depends on what the optimiser does with the uninitialised
slice of the returned temporary, which is an accident, not a promise.

### 1.3 Each route, its C written by hand into the units the container's compiler wrote

Patches `q1/routes/route{C,D,F,G}.pl` (shared helper `common.pl`, whose gap is clang's arithmetic on the
header's own struct: `offsetof(struct cd, d) - (offsetof(struct cd, c) + sizeof((p)->c))`), applied by
`PATCH=` in `msan_units.sh` to `main.c` and `rec.c` before compiling. Same probe, same levels:

| route | the C it adds | x86-64 MSan -O0/-O1/-O2/-O3 | x86-64 plain output | arm64 |
|---|---|---|---|---|
| today (B: the construction ends `rest: zero`) | 3 `memset`s at construction | 21 / 5 / 10 / 10 | `-O0` digests differ | 0, equal |
| **C**, by address between Heroes functions | `void h_rec_through(struct cd *hero_ret, const struct cd *hero_px)`, `h_rec_through(&t10, &t9)` | **0 / 0 / 0 / 0** | equal | 0, equal |
| **D**, C handed a temporary zeroed whole, fields copied in | `hero_d_cd(&(struct cd){0}, &h2_b)` at each C call | 6 / 1 / 2 / 2 | equal | 0, equal |
| **F**, gaps re-zeroed where a by-value crossing arrives | `HERO_ZERO_GAPS_CD(&h0_x)` on entry, `(&t10)` after each call | **0 / 0 / 0 / 0** | equal | 0, equal |
| **G** (the critic's), gaps zeroed in place at each lend to C | `HERO_ZERO_GAPS_CD(&h2_b);` before `write(…, &h2_b, …)` | **0 / 0 / 0 / 0** | equal | 0, equal |

**D's survivors** (`q1/msan-kept/routeD.pl-m-O2.txt`): `hero_lend_audit` at exit, origins `t10.sroa.4`
and `t14`: C saw clean bytes, the program's own cells kept the dirty ones and the runtime summed them.
And D is wrong twice more, each run on this Mac (`q1/routeD/`):

- **over a `partial` record**, `d_partial.c`: OpenSSL's `SHA256_CTX` bound as defect 396's golden binds
  it (`partial`, `num` alone), D copying the named field into a zeroed temporary around each call:
  `route D Final returned 1; route D 00000000` where today's binding gives `ba7816bf`. A success code
  and a digest of zeros: the undeclared state was zeroed by the copy;
- **over a record whose address C keeps**, `d_keeper.c`: `<sys/queue.h>`'s `TAILQ_HEAD`, reached through
  `static inline` as §4.19 reaches a macro: after `tq_init` and two `tq_push` through D's temporaries,
  `route D first -1, tqh_last into the temporary`, where today's reads `first 7`. The list's tail
  pointer points into a dead temporary; nothing in the header says so (§ 2.3 below).

**E, the padding named as a field** (`q1/routeE/`): declared by the binding (`pad: u8[7]`, `e_field.hero`)
it is `error[ffi_unknown_field]: cd.h declares no pad in cd`, exit 1: admitting it means exempting a
field from header verification. As a C member of a type of the compiler's own (`e_type.c`) it is
`error: incompatible pointer types passing 'struct hero_cd *' to parameter of type 'const struct cd *'`
under the emitter's own `-Werror=incompatible-pointer-types`: the record stops being the header's struct,
which §4.19 (*a group's `record` is the header's struct*, panel 060) and every `extern` taking
`struct cd *` rest on.

### 1.4 The ABI questions the brief asks

- **An `extern` that takes a record by value** (`q1/cb/cb.h`'s `bump_cd(struct cd)`): every route keeps
  the header's signature, since the call is checked against it (`hero_ffi_probe_h_cb_bump_cd(struct cd
  a0)`, `cb.c:152`). No route can protect the gap there on x86-64: C's own ABI drops it before C runs,
  as `take_cd` shows in pure C (1.1). That residual is C's and stays C's.
- **A callback taking a record by value** (`cb.hero`, `typedef int (*cd_cb)(struct cd)`, prints `12`,
  exit 0, `q1/cb/cb-run.txt`): today's emitter already hands C a wrapper with the header's own type,
  `int32_t h_0cb_cb_seen(struct cd h0_x) { … h_cb_seen(h0_x) … }` (`cb.c:249-252`). Under C or F the
  wrapper's body changes (`h_cb_seen(&h0_x)`), C's view does not. The one emitter cost: today one
  typedef, `h_0fn_5cf1ff1f`, is both the Heroes function value's type and the C callback's
  (`cb.c:109`, `:151`, `:267`); under C they become two.
- **`HERO_RUNTIME_ABI`** (30, `heroes_runtime.h:108`) stamps the runtime's declarations, and none takes a
  group record by value: the descriptor copies by address (`void (*copy)(void *dst, const void *src)`,
  `heroes_runtime.h:489`; generated as `__builtin_memmove(dst, src, sizeof(struct cd))`). C, F and G move
  no runtime declaration. G could move one only if the gap-zeroing went into `hero_lend_take`; written by
  the emitter beside the call, as above, it does not.

### 1.5 What I adopt for Q1, and what I refuse

**Adopt G, with B kept where R1 already has it.** G is the one route that zeroes the bytes at the moment
and place C (and the runtime's audit) reads them: in place, so a C function that keeps the address keeps
the right one; only at a lend, so a record that never reaches C costs nothing; every byte count written
as `offsetof`/`sizeof` against the header's own struct, so clang verifies the arithmetic as it verifies
the binding; nothing for a record with no gap. It reaches a `partial` record (the compiler already reads
every member's offset from clang's dump, `cli/layout.hero`) and must leave a union's other members' bytes
alone (they are data, C11 6.2.6.1p7), which is what `rest: zero` (B) is for at construction.
C and F also read 0, and both change how every by-value type holding a group record crosses a Heroes call
(a Heroes record holding one, as S2, a `T?`, a variant payload), and neither reaches a record C hands back
by value or a callback's parameter; G reaches both at the next lend.
**Refuse D**: it zeroes a `partial` record's state (measured), breaks a kept address (measured), and leaves
the runtime's audit reading the gaps (measured). **Veto E** in both its C forms (1.3). **Object to A**:
a program-visible wrong answer on the CI's own x86-64 leg is not *nothing*.

## Q2, a record C forbids to copy (issue 1335)

### 2.1 Each type bound and copied, on the three platforms

Sources `q2/` (`nc.h`, `nc2.h`, `nc3.h`, `nc4.h`, `win.h`, `unfair.h`, one `.hero` per type); a bare
`pthread_mutex_t` cannot be bound by value at all (`record Mutex tag pthread_mutex_t partial` with no
field is `error[empty_record]`), so each lock is held in a struct of the program's own, the shape a
program meets (defect 094's golden `hero_mutex`, the critic's `struct counter`). Darwin: this Mac (`q2/darwin-q2-locks.txt`, `q2/darwin-q2.txt`);
Linux: `q2/q2run.sh` in `heroes-linux-arm64` (`q2/linux-arm64-q2.txt`); Windows: `/c/w/p215-ffi/q2`,
clang 23.1.1 `x86_64-pc-windows-msvc`, `heroes.exe` built there from the seed with `-Wl,/STACK:67108864`
(`q2/windows-q2.txt`). Every copy below that builds runs to exit 0 with no message; the refusals exit 1.

| type | Darwin | Linux arm64 | Windows | what says *do not copy* |
|---|---|---|---|---|
| `pthread_mutex_t`, copied locked | copy `trylock 16`, unlock 0, lock 0; original unlock 0; both destroy 0 | the same | no `pthread.h` | POSIX's text; no header mark (below) |
| `pthread_cond_t`, copied, original destroyed, copy signalled | 0, 0, 0 | 0, 0, 0 | no `pthread.h` | POSIX's text |
| C11 `mtx_t`, copied locked | no `threads.h` in the SDK (`ffi_missing_header`) | copy `trylock` = `thrd_busy` (1), unlock 0; original unlock 0 | copy `trylock` = `thrd_busy` (3), unlock 0; original unlock 0 | (C11's text, unread here) |
| `CRITICAL_SECTION`, copied entered | n/a | n/a | copy `LockCount -2`, `TryEnter` 1 (same thread), both deleted | Microsoft's documentation (unread here) |
| `SRWLOCK`, copied held | n/a | n/a | copy `try 0`; after one unlock each, both acquirable | (unread) |
| `os_unfair_lock` | unlocked copy `trylock 1`; locked copy `trylock 0`, unlocks from the owner, no abort | n/a | n/a | `os/lock.h:59-63`: *bytewise copying* an unlocked one *is equivalent to initializing a new lock* |
| `_Atomic int` field (`atomic_int n` bound `n: i32`) | copy independent, `add 2` each | the same | the same | nothing: C permits it |
| zlib `z_stream`, copied after `deflateInit` | copy: `deflate` and `deflateEnd` **-2** (`Z_STREAM_ERROR`); original 1, 0 | the same | no `zlib.h` | the library itself: its state points back at the stream |
| `FILE` by value | refused: `ffi_unknown_tag`, `fopen` `ffi_return_type` (`struct __sFILE *`) | the same (`struct _IO_FILE *`) | the same (`struct _iobuf *`) | unreachable: only a handle, whose copy is the pointer |
| `va_list`, `jmp_buf` | refused: `ffi_unknown_tag`; `setjmp` `ffi_parameter_type` (`int *`) | the same (`struct __jmp_buf_tag *`) | not run | unreachable by value |

**What C marks so: nothing a compiler can read.** glibc's `pthread.h`, `bits/pthreadtypes.h`,
`bits/struct_mutex.h`, `stdio.h`, `threads.h` (grepped for *copied*, *copy*, *moved*, *attribute*: licence
lines and unrelated attributes only); Darwin's `pthread.h`, `os/lock.h`, `stdio.h`, `stdatomic.h`: the one
sentence is `os/lock.h`'s, and it says a copy is *fine* when unlocked. C has no attribute for it (clang's
`trivial_abi` is C++'s).

### 2.2 Can route C decide from the header alone?

**It can see the lock, even through `partial`**: clang's record-layout dump, which the compiler already
reads (`cli/layout.hero`), shows the undeclared member (`q2/dump.c`, `dump-darwin.txt`,
`dump-linux-arm64.txt`, `dump-windows.txt`): `struct counter` holds `struct _opaque_pthread_mutex_t
lock` on Darwin, `pthread_mutex_t lock` on glibc (an untagged union keeps its typedef name), and on
Windows `struct csbox` and `struct srwbox` hold `struct _RTL_CRITICAL_SECTION cs` and `struct _RTL_SRWLOCK l`. So C's input is a
**list of names, three spellings per lock**, kept by this project, since no header says it.
**How often it is wrong**, over the ten types bound above: three are unreachable by value (`FILE`,
`va_list`, `jmp_buf`), so moot; of the seven left, a lock list decides five right (the two POSIX, `mtx_t`,
`CRITICAL_SECTION`, `SRWLOCK`); it over-refuses `os_unfair_lock`'s documented unlocked copy if listed and
misses its locked copy if not; and it is **blind to `z_stream` and to every self-referential struct**:
`TAILQ_HEAD`'s dump is `struct item * tqh_first; struct item ** tqh_last`, two plain pointers
(`dump-*.txt`), and a copy of it is silently wrong (1.3, `d_keeper.c`). The population today: the 10 group
records of `examples/` (`grep -rl '^extern' examples --include='*.hero'` reads 21 files; the `record`
lines indented under an `extern`, read by `awk`: 5 handles and `tm`, `Color`, `Vector2`, `Rectangle`,
`AutomationEvent`) hold none of these types, so C would fire on 0 of them.

### 2.3 What each route costs a binding's C

- **B, a mark the binding writes**: no line of emitted C changes; it is a checker rule. It reaches the
  critic's A2 shape (a `partial` record with the lock undeclared) because the mark is on the record, not on
  a field. Two premises it rests on, measured: an `@` parameter reaches C at the caller's own address
  (`critic-work/taskB/b.hero` emitted: `void h_b_bump(struct counter *ph0_c)`, `counter_lock(&(*ph0_c))`,
  prints `3`); and a constant's or a construction's value is fresh, so defect 094's
  `m: hero_mutex @= HERO_MUTEX_INIT` must stay legal. One emitter fact beside it: reading `c.n` through
  that parameter copies the whole 72-byte record first (`t2 = (*ph0_c); t3 = t2.n;`), a C-level read of the
  lock's bytes that another thread may be writing; a marked record's field should be read in place.
- **C as a rule**: no C change either, but a per-platform list (2.2) wrong in both directions. **C as a
  note** is free and useful: where the dump shows a listed lock inside a group record that carries no
  mark, the diagnostic that refuses nothing can still say *this record holds `pthread_mutex_t`, which
  POSIX says is not copied*, pointing at B's mark.
- **D, a check at run time**: the record's bytes are C's, so the runtime has nowhere to keep *where this
  one was born* but a field the header lacks, which 1.3 measured refused (`ffi_unknown_field`) or
  type-breaking (`incompatible pointer types`); short of that, the only run-time check is a copy that
  aborts, which is B moved from the compiler to the user.
- **A**: a copy of a held lock that reads held and is used, on every platform that has the lock, exit 0, with
  two locks where the program wrote one for the mutex and `SRWLOCK` (2.1). **A2**: the copy still
  copies (the critic's pass 1).

**Adopt B, with C as the note that proposes it. Object to A, A2 and D.** No route here breaks the C ABI.

## Q3, R1's payment: the census structs with `rest: zero` and without

Sources `q3/`; every program binds the real header, sets the struct, hands it to C and prints what C did.
Darwin: this Mac (`q3/q3-darwin.txt`); Linux: `q3run.sh` in `heroes-linux-arm64` (`q3/q3-linux-arm64.txt`); Windows:
`/c/w/p215-ffi/q3` (`q3/windows-q3.txt`). cl100k by `./heroes measure <file>` from the worktree's root
(`q3/q3-measure.txt`), each whole program:

| struct, binding | without `rest: zero` (what the program writes) | with it | built and run | cl100k without / with |
|---|---|---|---|---|
| `sockaddr`, `bind` + `getsockname`, 127.0.0.1:47215 | Darwin `SockAddr(sa_len: 16, sa_family: 2, sa_data: [-72, 111, 127, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0])`; glibc the same less `sa_len`, `sa_family: u16` | `SockAddr(sa_family: 2, rest: zero)` and four element writes, `a.sa_data[0] @ -72` … | `bind 0`, `getsockname 0 port 47215`, both forms, Darwin and Linux arm64; Windows (Winsock, `ws2_32`) `bind 0` | Darwin 479 / 398; glibc 458 / 391 |
| `SHA256_CTX`, low-level route, bound whole | `Sha256Ctx(h: [0 ×8], Nl: 0, Nh: 0, data: [0 ×16], num: 0, md_len: 0)` | `Sha256Ctx(rest: zero)` | `1 1 1 186 173` (SHA-256 of `abc`), both forms and `partial` with `num: 0`, Darwin and Linux arm64 | 379 / 284; `partial` 247 |
| `fd_set`, `FD_SET` and `select` on a readable pipe | Darwin `FdSet(fds_bits: [0 ×32])` (`tag fd_set`); glibc `fd_set(__fds_bits: [0 ×16])` (a typedef of an untagged struct: `record fd_set`, no `tag`, the compiler's own note) | `…(rest: zero)` | `select 1 ready 1`, both forms, Darwin and Linux arm64 | Darwin 388 / 292; glibc 338 / 290 |
| Windows `fd_set` (`u_int fd_count; SOCKET fd_array[64]`) and `sockaddr` | `fd_set(fd_count: 0, fd_array: [0 ×64])` | `fd_set(rest: zero)` | `wsa 0`, `bind 0`, `count 1`, both forms | 565 / 376 |

What it says for the FFI seat: **no census struct needs `rest: zero` to be bindable** (every literal form
builds and runs on Darwin, glibc and Windows), **each construction costs 67 to 189 cl100k tokens more
without it**, and it changes no call C sees: its C is the `(T){0}` and `memset` before the stores
(`zeroed.hero`), all of it clang-checked. Neither form makes the bindings portable: `sockaddr`'s
`sa_len` and `sa_family` width, and `fd_set`'s field name and tag, differ by platform with or without it.
The high byte of the port is `-72` in both forms, K's cost (`char` is `i8`), which `rest: zero` does not
touch. Its padding effect is construction-local (Q1: it does not survive a call by value), and its sentence
promises fields, *every field sharing no byte with one it names is zero*, not padding, so it is accurate
as written. Panel 194's conditions (the reader test, MSan on the union shapes) are not my instruments.

**Adopt A** (R1 stands, judged on panel 194's conditions) **with D** (panel 178's condition recorded void).
**Object to B**: withdrawing it puts 64 literal zeros back into every Windows `select` and 1,280 into
Darwin's `utsname` (panel 194's ffi-pragmatist, `today_full`). No view on C.

## What design.md covers, and where it is silent

§1.11 (line 542): *FFI ergonomics rank alongside comprehension, not below it.* §1.11 point 3 (line 571):
*FFI signatures are verified by clang against the real header.* §1.12 (line 581): *A Heroes program must
not segfault and must not corrupt memory*, and (line 599) the compiler and runtime *check rather than
assume*. §4.19 (line 2644): *A group's `record` is the header's struct*. §4.20 (line 2908): `eq` and
`hash` walk fields, never bytes, because *two `==`-equal records hash differently under `memcmp`*.
**The document does not cover padding that reaches C**: §4.20's sentence is Heroes' own hash, and C's
readers (`write`, `send`, `memcmp`, a digest) have no such choice; Q1's measurement is that sentence at
the boundary. **Nor a C type that must not be copied**: `grep -n -i -E "padding|not be copied|non-copyable|
noncopyable|pthread_mutex" docs/design.md` finds only §4.20's line and by-value sentences. The nearest
rule is the spec's § 13, *C receives the place itself, which it may keep only while the place does not
move: a copy, a return and an array's growth move it*, which Q2's mark would make a compile error for
the records that need it.

## Verdicts

### Q1 (issue 1334)

- `verdict`: **approve G** (gaps zeroed in place at each lend, by clang-checked `offsetof` arithmetic),
  with B kept as R1 has it; **object** to A; **veto** D and E.
- `section`: §1.11 (line 542, point 3 line 571), §1.12 (line 581), §4.19 (line 2644), §4.20 (line 2908);
  the document is silent on padding at the boundary (above).
- `experiment`: `q1/probe/` against real `unistd.h`, `sys/socket.h`, `string.h`, `openssl/sha.h`, two
  modules; each route's C hand-written into the units (`q1/routes/*.pl`), clang 22.1.8 accepted all four;
  MSan x86-64 -O0..-O3: today 21/5/10/10, C 0/0/0/0, D 6/1/2/2, F 0/0/0/0, G 0/0/0/0; arm64 0 throughout.
  D over `SHA256_CTX partial`: digest zeros, success code. E: `ffi_unknown_field`, or clang's
  `incompatible pointer types`.
- `argument`: The loss is SysV x86-64's ABI (`{ i8, double }`), so pure C loses the gap through any
  call by value, and no construction-side zero survives (measured, two units, -O0..-O3). Heroes modules
  are separate units: 1334 is an every-level defect, program-visible at -O0 (`sha true false false`).
  G zeroes the bytes where and when C reads them, in place, so kept addresses stay right and records
  never lent cost nothing; every count is clang's arithmetic on the header's struct. C and F also read
  0 but re-plumb every by-value type holding a group record. D breaks `partial` records and kept
  addresses; E breaks verification or the header's type identity.
- `prediction`: with G as `routeG.pl` writes it, `q1/probe/` reads **0** MSan reports at -O0..-O3 on
  `heroes-linux` and prints `memcmp 0 0 0`, `sha true true true` built plain at -O0, where today's
  compiler reads **10** at -O2; and G moves the emission of only the `tests/golden/run` cases that lend a
  group record with a gap. Scored by `msan_units.sh` and the `emission` suite at G's landing.
- `condition`: G turns to object if a gap is found that `offsetof`/`sizeof` cannot name (a bit-field run,
  an anonymous member with no named inner field) so its bytes would have to be written from the dump
  unverified; then C (by address) is the robust route and I would take it. D's veto lifts only if it
  copies whole and in place, which is G.

### Q2 (issue 1335)

- `verdict`: **approve B** (the binding's mark: `=`, a by-value parameter, a return and an array's
  growth refused for the marked record and anything holding it), **with C only as a note** proposing the
  mark; **object** to A, A2, D and to C as a rule. No route breaks the C ABI; nothing here to veto.
- `section`: §1.12 (line 581, 599), §4.19 (line 2644); spec § 13's *the place does not move*;
  design.md is silent on non-copyable C types.
- `experiment`: ten types bound and copied on Darwin, Linux arm64 and the Windows box (2.1): the
  mutex, `mtx_t`, `CRITICAL_SECTION`, `SRWLOCK` copied while held read held in the copy, exit 0;
  `z_stream`'s copy fails `Z_STREAM_ERROR`; `FILE`, `va_list`, `jmp_buf` unbindable by value. clang's
  dump shows the lock inside a `partial` record on all three, spelled three ways.
- `argument`: No header on the three platforms marks a type non-copyable (grepped), so route C is a
  list this project keeps, three spellings per lock, wrong in both directions: it over-refuses
  `os_unfair_lock`'s documented unlocked copy and is blind to self-referential structs
  (`TAILQ_HEAD`: two plain pointers). B changes no emitted C, reaches the critic's `partial` record
  because the mark is on the record, and rests on two measured premises: `@` reaches C at the caller's
  own address, and a constant's value is fresh (defect 094). D has nowhere to keep identity but a
  field the header lacks.
- `prediction`: B lands with **no** change to the emitted C of any `tests/golden` program that does not
  copy a marked record (`emission` reads the same), and
  `fixedbugs-094-the-systems-mutex-initialiser-builds-a-record.hero` keeps building and printing what it
  prints; a lock list as C would fire on **0** of the 10 group records in `examples/`. Scored at B's
  landing.
- `condition`: C as a rule becomes acceptable if a mark readable from the header is found on all three
  platforms (none today), or if its list is measured to fire on no record a binding copies on purpose
  and to miss none a program copies by mistake.

### Q3 (R1's payment)

- `verdict`: **approve A** (R1 stands, judged on panel 194's conditions) **with D** (panel 178's
  condition recorded void); **object** to B; no view on C.
- `section`: §1.11 (line 542), §4.19 (line 2644).
- `experiment`: `sockaddr` (`bind`, `getsockname`), `SHA256_CTX` (bound whole and `partial`) and
  `fd_set` (`FD_SET`, `select`), each built with and without `rest: zero`, run on Darwin and Linux arm64,
  Windows' `fd_set` and `sockaddr` through Winsock: every form builds and runs; cl100k without/with
  479/398, 458/391, 379/284, 388/292, 338/290, 565/376.
- `argument`: None of 044's census structs needs `rest: zero` to be bindable; it removes 67 to 189 tokens
  per construction, changes no call C sees, and its C is clang-checked like the rest. It does not make a
  binding portable (fields and tags differ by platform either way) and its zero does not survive a call
  by value (Q1), and its sentence promises fields, not padding, so it is accurate as written. Withdrawing
  it brings back 64 literal zeros for every Windows `select` and 1,280 for Darwin's `utsname`.
- `prediction`: M-core-packages' `net` and `crypto/sha256` bind `sockaddr` and `fd_set` on Darwin, glibc and
  Windows, and `SHA256_CTX` on Darwin and glibc, **with or without** `rest: zero`, and each construction written without it is
  **67 to 189** cl100k longer (`q3/q3-measure.txt`). Scored at M-core-packages' close, both forms built.
- `condition`: I would object to R1 if its `(T){0}` and `memset` were found to change a call C sees or to
  break a binding that builds without it; it would become need rather than convenience if a census
  struct were found that only a `rest: zero` construction can build.

## What I could not run

- **Linux x86-64 for Q2 and Q3** (the emulated leg was spent on Q1's 40 MSan builds); Q2 and Q3 ran on
  Darwin, Linux arm64 and the Windows box.
- **MemorySanitizer on Darwin and Windows**: it exists only on the Linux legs; Windows' lowering of the
  padded struct is read from clang's IR (`q1/abi-lowering.txt`, *through memory*), not run.
- **The realistic Q1 probe on Windows** (no OpenSSL, no `sys/socket.h` there), and **macOS x86-64**, which
  shares SysV's lowering and would lose the gap as Linux x86-64 does: an inference from the ABI, unrun.
- **G for a `partial` record with a gap, a union, a bit-field, an array of records lent with `counted_by`
  and a field lent with `.ptr()`**: G was written and run for the full record `struct cd` alone.
- **How many group records in `tests/golden/run` have a gap**, which prices G's emission change; unrun.
- **POSIX's, C11's and Microsoft's own words** on copying a mutex, `mtx_t`, `CRITICAL_SECTION` and
  `SRWLOCK`: not read in this session (no web access); 2.1's last column says which source is the
  headers', read, and which is a document, unread.
- **Panel 194's reader test** for Q3: not my instrument.

**A slip of mine**: one command counting the examples' records ran `tee /tmp/dummy_never_used`, which
created an empty file outside the repository's root; the same command removed it (`rm -f`). Nothing else
was written outside the root.
