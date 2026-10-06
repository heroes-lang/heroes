# Panel 194, ffi-pragmatist

Started 2026-10-06 15:36:41 (`date`). Seat sections: design.md §1.11, §1.12,
§4.19 (to be quoted by grep in this copy before the verdict).

Copy: `<scratchpad>/194-ffi-pragmatist/`, made 15:36:53 by
`git -C /Users/joseph/Temp/heroes/heroes-lang archive 7a26a0a6 | tar -x`;
`shasum -a 256 seed/heroes.c` opens `fc9751a29a1ecb8a` (equal to the brief);
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` exit 0,
8,364,712 bytes, `heroes 0.2.0` (15:36:59).

Read before measuring: `00-shared.md`, `ffi-pragmatist.md`, the critic's first
pass, panel 178's sitting and its ffi-pragmatist report, defects 092 and 094's
issue files, `docs/panel/194-evidence/092-routes.md` (all in this copy).

(written as each command runs; each block carries the clock it was written at)

## Platforms (15:38:04 to 15:40:20)

- Darwin arm64: Apple clang 21.0.0, this Mac.
- Linux arm64: `heroes-linux-arm64` (arm64), Debian clang 22.1.8; compiler
  built from the seed in the container, `heroes-la64`, exit 0, `heroes 0.2.0`.
- Linux x86-64: `heroes-linux` (amd64, run WITHOUT `--platform`, `uname -m`
  `x86_64`), Debian clang 22.1.8; `heroes-lx64` built the same way, exit 0.
- Windows: `ssh win` answers, MINGW64 on Windows 10.0.26100, clang 23.1.1,
  target `x86_64-pc-windows-msvc` (15:38:09). Headers only, task 7 below.
- The emitter's flags (`selfhost/cli/flags.hero:93-108`): `-std=gnu11`,
  `-fsigned-char`, no `_GNU_SOURCE`, so glibc's sixth `utsname` field is
  `__domainname`.

## 1a. `utsname` (`w/uts/`), 15:38 to 15:40:53

Every program run with this copy's compiler; Linux with the compilers above.

| program | form | Darwin arm64 | Linux arm64 | Linux x86-64 | `.hero` tokens, vendored cl100k_base (lower bound) |
|---|---|---|---|---|---|
| `today_full` | all five fields, 1,280 zeros | check 0, run 0, `Darwin arm64` | refused at build: `ffi_incomplete_record` (`__domainname`) | same | **4,006** |
| `linux_full` | all six fields, 390 zeros | refused at build: `ffi_field_type` (`sysname` is not `i8[65]`) | run 0, `Linux aarch64` | run 0, `Linux x86_64` | (Linux twin of the above) |
| `today_partial` | `partial`, the two fields read, 512 zeros | run 0 | n/a | n/a | **1,668** |
| `today_z3const` | **the program's own header, `#define UTSNAME_ZERO {0}`, bound as `constant UTSNAME_ZERO: Utsname`** | check 0, run 0 | (as `linux_partial_z3`) run 0 | run 0 | **167**, plus the 2-line header **22** = 189 |
| `today_shimfn` | a `static inline struct utsname utsname_zero(void)` in the program's own header, returned by value | check 0, run 0 | unrun | unrun | 168 plus header |
| `r1` | `Utsname(rest: zero)` | `unknown_name` for `zero` today (R1 is not built in this copy) | | | **158** |

**A route nobody listed is open today and it is 094's repair** (41c5d1b4, an
ancestor of 7a26a0a6 by `git merge-base --is-ancestor`): a brace list `{0}` in
a header of the program's own, bound as a group `constant` of the record's
type, builds the whole record at zero on three legs. The emitted accessor is
`struct utsname hero_constant_value = UTSNAME_ZERO; return hero_constant_value;`
under `-Werror=excess-initializers` and the conversion errors
(`today_z3const.c:126-139`). It costs a header of the program's own: design.md
§1.11 point 2 admits *a thin C shim ... for awkward struct-passing* but says
*shims are for the hard cases only*. R1 saves 31 tokens and the second file
over it, and nothing in clang's verification.

Emitted C, this Mac (`--emit-c`, `wc -l`): `today_full` 5,516 lines and 1,304
temporary assignments, its construction one compound literal
`(struct utsname){.sysname = {t1, ...}}` (`today_full.c:4005`); batch 12's
`f7576a01` is in the tree and the count is still per element. `today_partial`
1,913; `today_z3const` 1,690; `today_shimfn` 1,672.

## 1b. `sockaddr_un`, `connect` to a path (`w/sun/`), 15:41 to 15:42:15

A shim header IS used, and every route needs it: `connect` and `bind` take
`const struct sockaddr *`, and `connect(fd, @addr: SockaddrUn, len: u32)` is
refused at build, `ffi_parameter_type` (`direct_darwin.hero`, exit 1), as the
critic found. `un_shim.h` (170 tokens) holds two `static inline` casts that
pass `(socklen_t)sizeof *a`, so the program never states a length (092's
shape cannot arise through it). The path is written by `strlcpy` through
`a.sun_path.ptr()` declared `ptr counted_by size lent`, the field lend § 13
already has.

| program | construction | Darwin arm64 | Linux arm64 | Linux x86-64 | tokens (vendored cl100k_base) |
|---|---|---|---|---|---|
| `today_darwin` / `today_linux` | every field, 104 / 108 zeros | run 0: bind 0, listen 0, connect 0, path read back | run 0, same | run 0, same | **836** / **833** |
| `z3_darwin` / `z3_linux` | `#define SUN_ZERO {0}` in the shim, `constant SUN_ZERO`, then `a.sun_family @ ...` | run 0, same | run 0 | run 0 | **522** / **514** (+1 shim line) |
| `r1_darwin` / `r1_linux` | `SockaddrUn(sun_family: ..., rest: zero)` | `unknown_name` today | | | **501** / **493** |

The Darwin binding on Linux is refused at build, `ffi_unknown_field`
(`sun_len`), on both Linux legs: the per-platform binding fails loudly.

## 1c. `pthread_mutex_t` set up correctly (`w/mtx/`), 15:43:23; task 4 inside it

| program | form | Darwin arm64 | Linux arm64 | Linux x86-64 | tokens |
|---|---|---|---|---|---|
| `today_init_*` | Darwin: `tag _opaque_pthread_mutex_t`, `__sig` and `__opaque: i8[56]`, 56 zeros, then `pthread_mutex_init`; Linux: untagged `record pthread_mutex_t partial`, `__align: i64`, then init | init 0, lock 0, unlock 0, destroy 0 | 0 0 0 0 | 0 0 0 0 | 373 / 209 |
| `zero_noinit_*` | the same zero, NO init (wrong) | **22 22 22 at exit 0** | 0 0 0 | 0 0 0 | 359 |
| **`z3_init_*`** | **`constant PTHREAD_MUTEX_INITIALIZER: Mutex`, 094's lowering** | **lock 0, unlock 0, destroy 0** | **0 0 0** | **0 0 0** | **203** / 213 |
| `z3_wrong_*` | `constant PTHREAD_COND_INITIALIZER` bound as a MUTEX | **builds, 22 22 22 at exit 0** | refused at build, `ffi_constant_type`, *excess elements in scalar initializer* | same | |
| `z3_recursive_linux` | `PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP` | | refused, `ffi_constant_type`, *undeclared* (needs `_GNU_SOURCE`, the flags do not define it) | same | |
| `r1_init_darwin` | `Mutex(rest: zero)` then init | `unknown_name` today | | | 199 |

**Task 4, 094's lowering against `PTHREAD_MUTEX_INITIALIZER`: holds on Darwin,
Linux arm64 and Linux x86-64.** The emitted accessor on Linux is
`pthread_mutex_t hero_constant_value = PTHREAD_MUTEX_INITIALIZER;` over glibc's
`{ { __PTHREAD_MUTEX_INITIALIZER (PTHREAD_MUTEX_TIMED_NP) } }`. **Its limit,
measured per platform**: Darwin's `PTHREAD_MUTEX_INITIALIZER` and
`PTHREAD_COND_INITIALIZER` are both `{_PTHREAD_*_SIG_init, {0}}`
(`pthread.h:185,214` in the SDK), so the wrong one compiles and every call
fails with 22 at exit 0; on glibc the shapes differ and clang refuses it. The
lane's sentence (*a brace list has no type*) holds, and which leg catches it is
the header's accident.

**For the mutex, R1 buys 4 tokens over a form already landed, and C's form is
the one C says is valid.** On Linux the mutex needs no long array at all.

## 1d. OpenSSL SHA-256 (`w/sha/`), 15:44 to 15:47:28

`extern "sha_shim.h" package "libcrypto"` (pkg-config answers
`-I/opt/homebrew/Cellar/openssl@4/4.0.3/include ... -lcrypto` here; the Linux
images answer too). Expected digest of `abc` opens `0xba` (186), closes `0xad`
(173).

| program | form | Darwin arm64 | Linux arm64 | Linux x86-64 | tokens |
|---|---|---|---|---|---|
| `evp_today` | **what a binding writes today**: `EVP_MD_CTX_new` (handle, `acquires EVP_MD_CTX_free`), `EVP_MD_fetch` (handle, `acquires EVP_MD_free`), `EVP_DigestUpdate`; the digest into a `struct digest32 { unsigned char b[32]; }` of the shim, 32 zeros | 1 1 1, 186, 173 | same | same | **521** |
| `evp_z3` | the same, the digest buffer from `#define DIGEST32_ZERO {0}` | same | same | same | **441** |
| `evp_r1` | the same, `Digest32(rest: zero)` | unbuilt | | | **428** |
| `dep_today` | deprecated `SHA256_CTX` by value, `h: u32[8]`, `data: u32[16]`, 24 zeros, `SHA256_Init` | 1 1 1, 186 (with a `-Wdeprecated-declarations` warning, not an error) | same | same | **470** |
| `dep_r1` | the same under R1 | unbuilt | | | **268** |
| `dep_noinit` | `SHA256_CTX` zeroed, NO `SHA256_Init` | **Update 1, Final 1, first byte 0: a wrong digest with success returns** | same | same | |

**A binding written today needs no `SHA256_CTX` value**: the context is a
handle. What it does need is an output buffer, and **`unsigned char *md`
with no count parameter cannot be bound without C**: a record lent whole to
it is `ffi_parameter_type` (`md_record.hero`), and a field lend needs a
`counted_by` sibling the C function does not have. So `hero_evp_final` is a
shim (`sha_shim.h`, 147 tokens with its four lines), and R1 or the `{0}`
constant only zero the buffer it fills.

`EVP_sha256()` returns `const EVP_MD *` and a handle record is refused for it
(`ffi_return_type`, `evp_today.hero` before the change); the note does not
name a route. `-> ptr` builds and runs (`evp_ptr.hero`), and `EVP_MD_fetch`
returns a non-`const` handle. Not this sitting's question; noted.

### Found beside: a SCALAR lent through `@` to a pointer C writes an array through

`function SHA256_Final(@md: u8, @c: Sha256Ctx) -> i32` with `md: @m` over
`m: u8 @ 0`: **`check` 0, `run` 0 on three legs, printing the right first
byte**; the emitted call is `SHA256_Final((void *)&h2_m, &h1_c)` over
`uint8_t h2_m` (`w/sha/mc.c:130,189`). `heroes build --sanitize`: run 0, **no
report**: AddressSanitizer does not see stores made inside an uninstrumented
library (it saw 092's because it intercepts `read`). The C proof,
`w/sha/md_guard.c`, the byte followed by a 0xAA guard: **31 of 63 guard bytes
changed past the one lent**, on Darwin, Linux arm64 and Linux x86-64. In
`md_scalar_canary.hero` the two `i64` canaries survived, so the 31 bytes
landed in frame slots the program did not print. This is 092's *what neither
route closes (2)* (a typed pointer, no count) at a scalar instead of a record,
and it needs no record: none of 092's routes A to E reaches it. Searched
`issues/` for *one element*, *typed pointer*, *scalar lent*, *out-parameter
... array*: not found; a question, not proof it is unfiled.

## 2. Zero as bytes, 178's table re-run (`w/zero/`), 15:50:23

178's own probe (`docs/panel/178-reports/ffi-pragmatist-work/zinit.c`, copied
unchanged), each type in a child with a 3-second alarm. Darwin: `clang
-std=gnu11 -O0 -I/opt/homebrew/include ... -lcrypto`, 0 warnings, run 0.
Outputs: `w/zero/zinit-darwin.txt`, `zinit-heroes-linux-arm64.txt`,
`zinit-heroes-linux.txt`.

| type, all bytes zero | Darwin arm64 | Linux arm64 | Linux x86-64 |
|---|---|---|---|
| mutex: lock | **22** | 0 (initialiser all zero) | 0 (all zero) |
| cond: signal / broadcast | **22 / 22** | 0 / 0 | 0 / 0 |
| rwlock: wrlock | **22** | 0 | 0 |
| attr / mutexattr / rwlockattr passed on | **22 / 22 / 22** (condattr 0) | 0 | 0 |
| `pthread_once_t` | runs once | runs once | runs once |
| `pthread_barrier_t`, then wait | (none on Darwin) | **blocked forever** (alarm) | **SIGFPE** |
| `pthread_spinlock_t`, then lock | (none) | free, 0 | **blocked forever** (alarm): locked at zero |
| `SHA256_CTX`, Update / Final | **1 / 1, digest WRONG**, Final wrote 0 of 32 bytes | same | same |
| `struct flock`, `F_SETLK` | -1 EINVAL (`F_RDLCK` = 1) | **0, a lock TAKEN** (`F_RDLCK` = 0) | **0, a lock TAKEN** |
| `struct sigevent` | zero is `SIGEV_NONE` | zero is **`SIGEV_SIGNAL`** | zero is **`SIGEV_SIGNAL`** |
| `sigset_t`, `fd_set`, `in6_addr`, `mbstate_t`, `SIG_DFL` | valid | valid | valid |

**178's table holds today**, every row. Zero is a defined value of every field
type and a valid value for the library on some types, some platforms only;
two Linux legs on one glibc disagree (spinlock, barrier). Through Heroes today
(1c above): the zeroed Darwin mutex reaches `22 22 22` at exit 0 with no new
form, so R1 makes no new wrong state reachable; it makes one cheaper to write.

## 3. Padding and unions (`w/pad/`), 15:48 to 15:50:03

**(a) What clang leaves in a union's bytes beyond its first member**
(`unionz.c`, the frame dirtied with 0xAA, the emitter's flags plus `-Wall
-Wextra`, 0 warnings; MemorySanitizer on Linux, the record handed to
`write()`):

| shape | Darwin -O0 / -O2 | Linux arm64 -O0 / -O2, MSan | Linux x86-64 -O0 / -O2, MSan |
|---|---|---|---|
| `(struct su){.k = 1}`, the union named by nobody (R1's literal) | 0 / 0 non-zero tail bytes | 0 / 0, no report | 0 / 0, no report |
| `struct uf s = {0}`, the union first (the `{0}` constant) | 0 / 0 | 0 / 0, no report | 0 / 0, no report |
| `(struct uf){.u.c = 0}`, the union's 1-byte first member named | 0 / 0 | 0 / 0, no report | 0 / 0, no report |
| `{}` | 0 / 0 | 0 / 0 | 0 / 0 |
| `memset` then a store | 0 / 0 | 0 / 0 | 0 / 0 |
| CONTROL: member stores, no initialiser | **54** / **54** | **63** / 0, **reported at -O0 and -O2** | **63** / 0, **reported at -O0 and -O2** |

Clang zero-fills the whole union under every initialiser shape, on three legs.
This is **clang's behaviour, not C's promise**: C11 §6.7.9p10 initialises a
union's first named member (the historian's to quote), and GCC 15's release
notes say `{0}` no longer clears a whole union (recalled, **unrun here**: no
GCC on these legs was tried). The emitter is clang-only (`flags.hero`), so the
measurement binds Heroes today; a second C compiler would reopen it.

**(b) 178's point 9, through today's pipeline** (178's critic's `small.hero`
and `copies.hero`, copied; emitted by each leg's compiler, compiled at `-O0`
to `-O3` with the emitter's flags and `-fsanitize=memory`; `msan_hero.sh`):

| program | Linux arm64 | Linux x86-64 |
|---|---|---|
| `copies.hero` (construction, a function return, a record field, an array element, each handed to `write`) | 1 report at every level: `copies.hero:36` / `ctl.h:9`, **the control alone** | the same |
| `small.hero` (`{char; double}`, `{char; int}`, `{float; char}` through a by-value Heroes function) | **0 reports** at every level | **1 report at every level, `small.hero:29`**: `w_cd(@b)` after `b: Cd @ cd_through(x: a)` |

**Holds today**: the `{char; double}` record loses defined padding through a
by-value call on x86-64 (the SysV ABI passes it in one integer and one SSE
register) and not on arm64. So **§ 13 may promise** that a field `rest: zero`
does not name is zero, and that C builds the record with every byte zero
(measured above, clang); **it may not promise** that padding stays zero once
the record is copied or passed, nor that zero is a valid value for the
library (table 2). 178's point 9 *padding is not promised* stands.

## 1e. The C R1 implies, compiled and run (`w/r1c/r1_implied.c`, `r1c.sh`), 15:52:42 to 15:52:53

Today a group record's construction is a compound literal with designated
fields (`w/uts/today_full.c:4005`), so R1's emission is that literal naming
only what the program wrote: `(struct sockaddr_un){.sun_family = ...}`, and
for no field named `(struct utsname){0}` or `{}`. Each built after the frame
was dirtied with 0xAA, under the emitter's flags (`flags.hero:93-108`) plus
`-Wextra -Werror`, at `-O0` and `-O2`, then handed to the real calls:

| | Darwin arm64 | Linux arm64 | Linux x86-64 |
|---|---|---|---|
| compile, both levels | exit 0, **0 diagnostics** | exit 0, 0 | exit 0, 0 |
| `utsname` `(T){0}` and `(T){}`: non-zero bytes; `uname` | 0 of 1,280; 0, `Darwin arm64` | 0 of 390; 0 | 0 of 390; 0 |
| `sockaddr_un` `{.sun_family}`: non-zero bytes (the family's own); bind / listen / connect | 1 of 106; 0 0 0 | 1 of 110; 0 0 0 | 1 of 110; 0 0 0 |
| mutex `(T){}`: lock with no init; init / lock / unlock / destroy | **22**; 0 0 0 0 | 0; 0 0 0 0 | 0; 0 0 0 0 |
| `digest32`, `SHA256_CTX` `(T){}`; SHA256 Init/Update/Final; EVP | 0, 0; 1 1 1, 186; 3, n 32, 186 / 173 | same | same |

Under `-std=c11 -pedantic-errors` the `{}` spelling is **6 errors**, *a C23
extension*; `{0}` and a designated field are clean. So the emitter's R1 for
no field named should be `{0}` or a designated first field, which also keeps
the C11 that CLAUDE.md § 7 names. No ABI change, no marshalling, no new C
construct: R1 is the literal the emitter writes today with fewer elements.

## 7. Windows (`ssh win`, `/c/w/p194-ffi-37357`, headers only), 15:51:32 to 15:52:04

No tree copied, no build, nothing removed; one `clang -fsyntax-only` per
header, sequential (`headers.txt`, `census-win.tsv` in that folder).

- **Of 178's 35 census headers, 9 exist**: `stdio.h stdlib.h time.h math.h
  sys/stat.h signal.h fcntl.h setjmp.h locale.h`. Missing: every socket and
  POSIX header (`sys/socket.h netinet/in.h arpa/inet.h netdb.h sys/un.h
  sys/utsname.h sys/statvfs.h sys/time.h sys/resource.h dirent.h pthread.h
  termios.h poll.h sys/select.h unistd.h pwd.h grp.h glob.h regex.h
  ifaddrs.h net/if.h sys/mount.h`), and `sqlite3.h curl/curl.h openssl/sha.h
  openssl/hmac.h` (no package installed on the default path).
- 178's census rule over the 9: **6 array fields, 2 longer than 8**, both in
  records the script names with a leading `_`: **0 public**.
- **`utsname` means nothing on Windows**: `sys/utsname.h` is missing.
- **`sockaddr_un` does**: `afunix.h` (after `winsock2.h`; alone it fails)
  defines `struct sockaddr_un { ADDRESS_FAMILY sun_family /* unsigned short */;
  char sun_path[108]; }`, and `connect` is `int (SOCKET, const struct
  sockaddr *, int)` with `__stdcall`. So the Linux construction's shape
  (`u16`, `i8[108]`) and the same `const struct sockaddr *` wall: a binding
  needs the same shim, and `SOCKET` is not `int`.

## 5. Defect 092 (`w/c092/`, the lane's programs copied from `docs/panel/194-evidence/092/`), 15:51 to 15:58

**The prototype, rebuilt here**: `docs/panel/194-evidence/092/prototype.diff`
applies to 7a26a0a6 with 16 of 16 hunks (`patch -p1 --dry-run`, then applied
in `194-ffi-pragmatist/proto/`, a second archive of 7a26a0a6); `../heroes build
selfhost/main.hero -o heroes-routes` exit 0 (15:51:21 to about 15:53), its seed
`--emit-c` exit 0 (37,463,659 bytes), built for Linux x86-64 in `heroes-linux`
as `heroes-routes-lx64`, exit 0 (15:55:34). **The x86-64 leg the lane did not
run**, `heroes run`, 4,096 bytes of `A`, `timeout 30` (`run092r.sh`):

| program | today's compiler, x86-64 | routes A and B compiler, x86-64 |
|---|---|---|
| `today-read-into-addrinfo` | 139, after `1032 1094795585` | A: 1, `ffi_parameter_type` |
| `today-void-count-past` | 139 | A: 1, `ffi_parameter_type` |
| `today-void-padded` | **0, silent**, `64 4702111234474983745` | A: 1 |
| `today-void-a-field-lent-whole` | **0, silent**, `4096 1094795585` | A: 1 |
| `today-count-through-a-cell` | **0, silent**, `4096 1094795585` | A: 1 |
| `today-typed-pointer-byte-count` | **0, silent**, `4096 1094795585` | **0, silent, unchanged** |
| `today-typed-pointer-record-count` | **124, did not end in 30 s** | **124, unchanged** |
| `today-const-void-read` | **124, did not end in 30 s** | A: 1 |
| `today-void-count-fits` (correct) | 0, `4 1094795585` | **A: 1, refused: the price** |
| `a-read-into-addrinfo` / `-fits` | | 134 before C runs / 0 |
| `a-void-count-past`, `a-void-a-field-lent-whole`, `a-typed-pointer-byte-count`, `a-const-void-read` | | 134 before C runs, each |
| `a-typed-pointer-record-count` (`nfds: 2`) | | **0, silent**, `2 1` |
| `a-count-through-a-cell` | | 1, `counted_by_shape` |
| `b-read-into-addrinfo`, `b-void-a-field-lent-whole`, `b-typed-pointer-byte-count`, `b-const-void-read` | | 1 at build, `field_lend_extent` |
| `b-void-count-past` | | 134, *past the 0 bytes of the field* (the lane's wording note) |
| `b-typed-pointer-record-count` (`nfds: 2`) | | **0, silent** |
| `b-count-through-a-cell` | | 1, `counted_by_shape` |

**The lane's Darwin and Linux arm64 tables hold on x86-64, row for row.**
On Darwin the exits depend on how the program was built (`heroes run` vs
`heroes build`, rechecked here 15:53): `typed-pointer-record-count` is 0 and
silent under `run` and 139 under `build`; `void-count-past` and `void-padded`
are 133 under `run` and 138 under `build`. Whether an overwrite is seen at all
is the build path's and the platform's accident.

### Found beside, in what is LANDED: § 13's field lend counts bytes, and C may count wider elements

`w/c092/wide-units.hero`: a `u8[16]` field lent through `w.ptr()` to the real
`mbstowcs(wchar_t *dst, const char *src, size_t n)`, declared
`dst: ptr counted_by n lent`, called with `n: 16`. The check compares 16 with
the field's 16 bytes and passes; C writes 16 `wchar_t`, 64 bytes:

- `check` **0**; Darwin run **134** after printing `16` and `438086664293`
  (the next field, `after: i64 @ 7`, overwritten with wide `e`, `f`);
  Linux arm64 **135**; Linux x86-64 **139**, each after the same two lines;
- `heroes build --sanitize`: *AddressSanitizer: stack-buffer-overflow ...
  WRITE of size 64*, exit 134.

`ptr()` takes only `i8[N]` or `u8[N]` (`bad_operand` on `i32[4]`, measured:
`units-*.hero`), so bytes ARE the field's elements; but `ptr` is exempt from
the pointee check (§ 13, *except ... what a `ptr` points at*), so C's unit can
be wider than the field's. The same unit gap 092's routes leave open for
`poll`, already in what ships, at `check` 0. Searched `issues/` and panel 166
for *mbstowcs, wchar, wide char, element size, in elements, unit of the
count, counts in records, pointee wider*: nothing; a question, not proof it is
unfiled. **For 092 this decides between routes**: route B extends exactly this
mechanism to whole records and inherits the hole; a repair of the unit (the
lane's route C) closes both at once.

### 092: each route's C against the real headers (`w/c092/routes_c.c`, `routes_c.sh`), 15:59:35

Each route's guard written as its emitter would write it before the call; the
lent object followed by a 0xAA guard, *past* the guard bytes C changed. The
emitter's flags, `-O0`: **exit 0, 0 diagnostics on all three legs**, and the
output is **identical on Darwin arm64, Linux arm64 and Linux x86-64**
(`sizeof(struct addrinfo)` 48, `struct pollfd` 8, `wchar_t` 4 on each):

| shape, wrong count | today | A / B (bytes: `n <= sizeof obj`) | C (C's unit: `n <= sizeof obj / sizeof *pointee`) | D (the compiler passes the size) |
|---|---|---|---|---|
| `read` into `struct addrinfo`, `n: 4096` | 4,048 bytes past | **refused before C** | **refused before C** | `n` = 48, 0 past |
| `poll` on one `struct pollfd`, `nfds: 2` | 2 bytes past | **2 bytes past, admitted** | **refused before C** | `nfds` = 1, 0 past |
| `mbstowcs` into a `u8[16]` field, `n: 16` | 48 bytes past | **48 bytes past, admitted** | **refused before C** | `n` = 4, 0 past |

**Windows** (`units.c`, `-fsyntax-only`, 15:59:51): `sizeof(wchar_t) == 2` and
`sizeof(WSAPOLLFD) == 16` both hold; `WSAPoll` is `int (LPWSAPOLLFD, ULONG,
INT)`. So C's unit differs per platform, and only a unit C computes on the leg
(route C's `sizeof *pointee`, route D's `sizeof`) is right on four legs; a unit
written in the binding as a number would be wrong somewhere.

### 092: two measurements that decide between A and B for this seat, 16:00

Both compilers on this Mac: today's (`heroes`) and the prototype with A and B
(`proto/heroes-routes`):

| program | today | routes A and B prototype |
|---|---|---|
| `gso-today.hero`: the real `getsockopt(SOL_SOCKET, SO_LINGER)` into `record Linger tag linger`, `@len: u32` | **run 0: `0 0 8`, correct** | **1, `ffi_parameter_type`**: refused as an undeclared lend to `void *` |
| `gso-a.hero`: the same with `@value: Linger counted_by len` | 1, `counted_by_shape` | **1, `counted_by_shape`**: no checked spelling exists |
| `a-wrong-type.hero`: a 48-byte `addrinfo` lent through `@` to `poll_like`'s `struct pfd *`, `nfds: 6` | 1, `counted_by_shape` | **1, `ffi_parameter_type`: clang's pointee check holds** |
| `b-wrong-type.hero`: the same record through `h.ptr()` to `fds: ptr counted_by nfds lent` | 1, `bad_operand` | **run 0, printing `6` and `65536`**: 6 <= 48 bytes, C wrote `revents` into the `addrinfo` (`ai_family` now 65536), silent |

So **route A as built makes a working, correct binding impossible**
(`getsockopt` into a record, and `SO_RCVTIMEO` into `struct timeval` and
`SO_PEERCRED` into `struct ucred` are the same shape; those two unrun), and
**route B opens a second, unverified path for a whole record**: `ptr` is
exempt from the pointee check, so the wrong-type mistake `@` refuses at build
compiles and corrupts at exit 0.

Binding cost of `read` into a record (`w/c092/cost/`, vendored cl100k_base):
today 130, A 134, B 134, D 127 (`n: u64 sizes buf`, the call names no `n`); C
adds nothing over A.

## 178's carried facts, re-run (`w/carry/`), 16:01

- **R2**: 178's `forgot.c`: `ai_socktype` set / forgotten (zero): Darwin **1 /
  2** results; Linux arm64 **2 / 6**; Linux x86-64 **2 / 6**. Holds.
- **L (091)**: 178's `elem_min.hero`, `s.name[1] @ 72`: prints **72, run 0 on
  three legs** (closed by defect 097, as the brief says; now on three legs).
- **The `i8` note**: `u8[256]` against `char sysname[256]`: `ffi_field_type`,
  note *every field is at the header's own width and sign: correct `sysname`*.
  **It does not name `i8`** (holds, as the critic found). The emitter passes
  `-fsigned-char` on every leg (`flags.hero:108`), so `i8` is the one right
  answer on all four legs, including Linux arm64 where C's default `char` is
  unsigned: a `certain` fix is available to the note.
- **The Darwin mutex needs no long array today**: `record Mutex tag
  _opaque_pthread_mutex_t partial` naming `__sig: i64` alone, `Mutex(__sig: 0)`,
  then `pthread_mutex_init`: **0 0 0**, 168 tokens (`mtx_partial_darwin.hero`).

Task 4, 094's own reproducer (`w/c094/cinit.hero`, `PT_INIT {1, 2}`), 16:03:
prints `2`, **run 0 on Darwin, Linux arm64 and Linux x86-64**. Windows: `SOCKET`
is 8 bytes and unsigned (`sock.c`, `-fsyntax-only`, 16:04:06), so `u64`.

## Verdicts, 16:04

Sections quoted from this copy's `docs/design.md`: §1.11 (line 467) *"FFI
ergonomics rank alongside comprehension, not below it. When a choice makes
bindings harder, that is a serious cost"*, and its point 2, *"A thin C shim
remains standard practice ... shims are for the hard cases only"*, and point
3, *"a wrong type in an `extern` is a compile error, which is this project's
thesis applied to the boundary"*; §1.12 (line 575) *"must not segfault and
must not corrupt memory ... any C library must be bindable"*; §4.19 (line
2322) and its panel-186 paragraph on unions (lines 2480-2499). **design.md
does not cover**: a lend's unit (no `counted_by` text at all; it is spec § 13
and panel 166), padding beyond `eq`/`hash` walking fields (line 2730), and
whether all-zero is a valid value of a library's type. On those three this
seat stands on measurement, not on a section.

**No veto on R1 or anything in M-buildable-structs. Two vetoes on 092's
routes: B, and E as the sole route.**

### M-buildable-structs

| route | verdict | section | cost, measured | falsifiable prediction | condition |
|---|---|---|---|---|---|
| **R1** `rest: zero` | **approve, amended** | §1.11 (point 2), §4.19, §1.12 | `.hero`, vendored cl100k_base: `utsname` 4,006 → **158**; `sockaddr_un` 836 / 833 → **501 / 493**; deprecated `SHA256_CTX` 470 → **268**; EVP digest buffer 521 → **428**; mutex **no gain** (168 today via `partial`). Emitted C: the literal the emitter writes today, fewer elements; 0 diagnostics, 3 legs, `-O0`/`-O2` | Under an R1 compiler, `w/uts/r1.hero`, `w/sun/r1_{darwin,linux}.hero`, `w/sha/{dep,evp}_r1.hero` exit 0 with their `today_*` twins' output on Darwin, Linux arm64 and Linux x86-64, and the Darwin `utsname` R1 binding stays refused at build on both Linux legs (`ffi_incomplete_record` or `ffi_field_type`, as `today_full` is today). Falsified by one non-zero exit, a changed output, or a Linux build that succeeds | (a) The emission stays the designated compound literal: `{0}` or a designated first field when nothing is named, **never `{}`** (6 errors under `-std=c11 -pedantic-errors`), never member stores into a bare cell (178's veto 2, met today). (b) **The union clause**: a union none of whose members is named is zero in **all** its bytes, which is clang's behaviour, measured on 3 legs; C11 initialises only its first member (§6.7.9p10, the historian's), so the sentence must not cite C for it; two members of one union stay refused (186). (c) No padding clause. **Veto** if R1 silences `ffi_incomplete_record` or `ffi_field_type`, or emits member stores into an uninitialised cell |
| R0, a second spelling | object | §1.11 | 0 at the boundary: `T(rest: zero)` naming nothing is the same `{0}` | an R0 prototype emits byte-identical C to R1-with-no-field for `utsname` | none: R1 covers it |
| Z1, a validity mark | object | §1.12, §4.19 (clang cannot check it) | +1 word per record | a Z1 prototype leaves `w/mtx/zero_noinit_darwin.hero` (`22 22 22`, exit 0) at `check` 0, because `partial` reaches it | a Z1 that also gates `partial` and is checked per platform against something clang reads |
| **Z2**, zero is bytes | **approve** | §1.12; design.md silent on validity | 0 | the zero-validity table (`w/zero/`) reproduces row for row on any later run of 178's `zinit.c` on these three images | a leg where all-zero bytes are not a defined value of a group-record field type |
| **Z3**, the header's initialiser (094 repaired) | **approve, landed** | §4.19, §1.11 | 0 spec tokens; `PTHREAD_MUTEX_INITIALIZER` 203 tokens on Darwin | `w/mtx/z3_wrong_darwin.hero` (a condition variable's initialiser bound as a mutex) keeps building and returning `22 22 22` at exit 0 under any 094 lowering that keeps the brace list; glibc keeps refusing it | its limit written down beside the repair; 094 may close on Darwin, Linux arm64 and Linux x86-64 (all three run here, mutex and `PT_INIT`) plus Windows' named skip |
| L (091) | approve, landed | §1.12 | 0 | `elem_min.hero` prints 72 at exit 0 on three legs (run here) | none |
| T, text into a field | object for v1 (wait) | §1.11 | the job is done: `strlcpy` through `a.sun_path.ptr()` binds, listens, connects on 3 legs | no task binding here needs T | 178's veto stands if it returns: never without its terminating zero |
| A, `[x; N]` | object | §4.19, §1.11 | writes the platform's N: 256, 65, absent on Windows | — | a measured need for a non-zero repetition |
| R2, omitted means zero | object | §1.12 | removes 186's `build` guard | 178's `forgot.c` (re-run): 1 vs 2 results Darwin, 2 vs 6 on both Linux legs, exit 0 | none |
| D, a zero for every type | object | §1.11 | nothing at the boundary beyond R1 and Z2 | — | — |
| K, C's `char` is `i8` | **approve the note's `certain` fix**; the § 13 sentence is the spec-warden's to price | §4.19, CLAUDE.md § 8 | 0 spec tokens | with the note changed, `w/carry/u8note.hero`'s `ffi_field_type` names `i8[256]` on all three legs, and `i8` is right on all four (`-fsigned-char`, `flags.hero:108`) | a leg where the emitter does not pass `-fsigned-char` |
| **Nothing** (today's forms) | object as the resolution; **it is what conservative would be** | §1.11 point 2 | **Measured**: every task binding builds today. `utsname` needs a 2-line header of the program's own (`#define UTSNAME_ZERO {0}`, 094's lowering, 189 tokens with it); `sockaddr_un` and EVP put the line in the shim they need anyway; the mutex needs nothing (`partial`, or the header's initialiser) | — | — |
| **Unlisted: the program's own `{0}` constant** | record it, do not specify it | §1.11 point 2 | 1 C line per record; a wrong brace list compiles (Darwin mutex, measured) | — | — |

**The strongest reason R1 is wrong, from this seat**: 094's landed lowering
already gives every group record a zero through one line of the program's own
C, on three legs, so **R1 adds no FFI capability**. Of the four task bindings,
one (`utsname`) needs C it would not otherwise have. R1's case is
comprehension, plus a zero the compiler guarantees where a brace list is the
author's unverified word; I approve it because §1.11 point 2 keeps shims
for hard cases, and zeroing a struct is not one. **Principle 0's entry ticket
is not this seat's to issue.**

### Defect 092

| route | verdict | section | cost | falsifiable prediction | condition |
|---|---|---|---|---|---|
| **A**: `counted_by` on a `@` record, an undeclared `@` record to `void *` refused | **object as built; it would be a veto if it landed as built** | §1.12 (*any C library must be bindable*), §1.11 | +4 binding tokens; +0.16% to +0.63% instructions per build (the lane's) | `w/c092/gso-today.hero` (real `getsockopt(SO_LINGER)` into `struct linger`, run 0 today) is refused under any A keeping `counted_by_shape` on a `@` sibling | approve once (i) `counted_by` admits a count read through a `@` cell (`*len` compared before the call), and (ii) the unit is C's (route C). It keeps clang's pointee check: `a-wrong-type.hero` refused |
| **B**: `x.ptr()` lends a whole record | **veto** | §4.19, §1.11 point 3 | +4 binding tokens | `w/c092/b-wrong-type.hero` (a 48-byte `addrinfo` to `struct pfd *`) runs 0 and prints `6 65536` under any B whose `ptr` stays exempt from the pointee check (measured on this prototype) | lifted if a record lent through `ptr()` is admitted only where the header's pointee is `void`, asked by the pointee check |
| **C**: the unit from the header's pointee, `n <= sizeof obj / sizeof *pointee` | **approve; the rule for every counted lend, record AND field** | §1.12, §4.19; design.md silent on units | 0 binding tokens over A; unbuilt in Heroes; its C: 0 diagnostics, 3 legs | under a route-C compiler, `w/c092/wide-units.hero` (`mbstowcs`, landed field lend) and `a-typed-pointer-record-count.hero` (`nfds: 2`) stop before C runs on Darwin, Linux arm64 and Linux x86-64, and the 9 + 15 tracked programs the lane enumerated keep their exit and stderr | the unit is C's `sizeof` on the leg (Windows `wchar_t` is 2, `WSAPOLLFD` 16); an incomplete pointee refuses `counted_by` |
| C': refuse `counted_by` on a typed pointee | object | §1.12 | — | `poll` on more than one descriptor has no binding under C' | — |
| **D**: `n: u64 sizes buf`, the compiler passes the size | **approve as a complement**, after or with C | §1.12, §1.11 | **−3** binding tokens (127 against 130); a `CParam` production and a spec word | under D, `read` into `addrinfo` passes 48 and `poll` passes 1 on three legs (its C, measured) | it needs C's unit for records; a deliberately shorter count stays A's or C's |
| **E**: refusal alone | **veto as the sole route** | §1.12 | — | `today-void-count-fits` and `gso-today.hero`, both correct, have no spelling under E | — |

**Open under every route, found beside** (to file; the synthesis's to class):

1. **A scalar lent through `@` to a pointer C writes an array through**:
   `SHA256_Final(@md: u8, ...)`, 31 of 63 guard bytes past on three legs,
   `check` 0, `--sanitize` silent because the store is inside libcrypto
   (`w/sha/`). No count exists to compare; no route of 092 reaches it, and nor
   does the undeclared typed-pointer record lend (`today-typed-pointer-record-count`,
   124 on x86-64). I know of no header-derivable signal for *how many*; a
   question, not a finding.
2. **§ 13's landed field lend compares bytes while C counts its own unit**:
   `mbstowcs` over a `u8[16]` field, `n: 16`, 64 bytes written, `check` 0,
   134 / 135 / 139, ASan *WRITE of size 64* (`w/c092/wide-units.hero`).
   Route C repairs it with 092.
