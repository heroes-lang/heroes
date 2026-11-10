# Panel 243, the ffi-pragmatist's report

Written 2026-11-10 as the runs went, from about 18:58. Seat: the ffi-pragmatist (design.md §1.11,
§4.19, §1.12). Worktree `.claude/worktrees/243-ffi-pragmatist`, detached at `89db84347`. Scratch
`.claude/worktrees/p243-scratch/ffi-pragmatist/`. Platform: this Mac (Darwin 27.2.0, arm64, Apple
clang 21). Docker off, as the brief says; Linux arm64 and the box are cited from panel 242's
ffi-pragmatist § 1.4 and § 1.7, never re-run. Every number has its command beside it; a sentence
with no run behind it says *unrun* or *reasoning*.

## 0. The compiler I judged with

- `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`: exit 0, `real 12.04 user 11.41`.
- `./heroes build selfhost/main.hero -o heroes-next`: exit 0, `--version` printed `heroes 0.6.0`,
  `sources 14e9e5ce168a2336`, the brief's key. `real 329.01` against `user 146.67`: three seats'
  stage 2 builds ran at once (`ps`), so the duration is discarded by the ratio.

Every Heroes run below uses that `heroes-next`.


## 1. Q1, defect 2421: a by-value argument past `...`

Scratch `q1c/` (plain C) and `q1h/` (Heroes). Every Heroes build with `heroes-next`.

### 1.1 What C reads past `...`, this Mac (arm64, LP64)

`q1c/widths.c` (53 lines): one variadic per read type, `rd_long`, `rd_int`, `rd_ll`, `rd_size`,
`rd_dbl`, each `noinline`, reading its one extra argument by `va_arg`. `clang -std=c11 -Wall
-Wextra` at `-O0` and `-O2`, exit 0 and no warning; both levels printed the same:

| passed | read as | printed | C11 §7.16.1.1p2 after §6.5.2.2p6's promotions |
|---|---|---|---|
| `int32_t -1` | `long` | 4294967295 | undefined (int is not long) |
| `int32_t -1` | `long long` | 4294967295 | undefined |
| `int32_t -1` | `size_t` | 4294967295 | undefined |
| `int32_t 0644` | `int` | 644 | defined: what a correct `open` reads for its mode |
| `uint16_t 0644` (`mode_t` here, `sizeof` 2) | `int` | 644 | defined: promoted to `int` |
| `int64_t 4294967338` | `int` | **42** | undefined: `twoarity`'s shape |
| `int64_t 4294967338` | `long` | 4294967338 | undefined by the letter (`long long` is not `long`), same width |
| `int64_t 4294967338` | `long long` | 4294967338 | defined here (`int64_t` is `long long` on Darwin) |
| `uint64_t` max | `size_t` | 18446744073709551615 | undefined by the letter, same width |
| `double 1.5` | `long` | 4609434218613702656 (`0x3ff8000000000000`) | undefined |
| `double 1.5` | `double` | 1.5 | defined |
| `float 2.5` | `double` | 2.5 | defined: promoted |
| `int32_t 7` | `double` | 3.45846e-323 | undefined |
| `const char *` | `long` | the address | undefined |

`va_arg(ap, mode_t)` itself: clang `-Wall` says *"second argument to 'va_arg' is of promotable
type 'mode_t' (aka 'unsigned short'); this va_arg has undefined behavior because arguments will be
promoted to 'int'"* (`-DPROMOTABLE`). So a correct C library reads its mode as `int`: an
inference from the warning, since Darwin's and glibc's `open` sources are unread here.

**The high half is not zero by rule, only by luck.** `clang -O2 -S` of an `int` handed past `...`
(`q1c/slotasm.c`): `str x0, [sp]`, the whole 64-bit register stored, its upper half whatever it
held (AAPCS64 leaves a 32-bit argument's upper half unspecified). Two measurements:

- `q1c/hi_a.c` + `hi_b.c`: an `int32_t` parameter forwarded past `...`, its register's upper half
  dirty on arrival. Read as `long`: **5 at `-O0`, 244834610708485 (`0xdead00000005`) at `-O2`**.
  (The dirty upper half is made by a cast of a function pointer, itself undefined in C; it models
  what the ABI lets a caller leave, not a program Heroes emits.)
- `q1c/narrow.c`: a checked narrowing, the shape of `.to_i32().must()`, `int64_t -1` range-checked
  and cast, then passed past `...` and read as `long`: **4294967295 at `-O0`, -1 at `-O2`**. clang
  at `-O2` checks `cmp x0, w0, sxtw` and stores `x0` unchanged.

So the value defect 2421 reads **is not one value**: it moves with the optimisation level and
with what the register held. In Heroes, the two shapes I built (`q1h/repro/`, the coordinator's
reproducer, and `q1h/narrowed/`, the `i32` from `wide.to_i32().must()`) printed 4294967295 at both
`-O0` and `-O2`, 4 of 4. The lowering loads the `i32` from a temporary (`--emit-c`:
`t11 = t10.as.ok; ... slot_read(t1, t11)`), and that load zeroes the upper half. A Heroes program
whose value moves with `-O` is **not found** in those two shapes; the C shows the ABI allows it.

Linux arm64 and the box: unrun this sitting (Docker off). Panel 242's ffi-pragmatist § 1.4 and
§ 1.7 measured `int -1` read as `long` as **4294967295 on Linux arm64** too, and `sizeof(long)`
**4 on the box**. Reasoning, unrun: on Windows x64 a `long` is 32 bits, so `i64` past `...` into a
`long` read is the reverse of 2421 (a value above 32 bits loses its high half). On AAPCS64 Linux
and SysV x86-64 a `double` past `...` travels in a floating-point register and a `va_arg(long)`
reads the general-purpose save area, so `f64` read as `long` reads **another argument's register**,
not 1.5's bits as here.

### 1.2 The class in Heroes, today: every by-value type past `...` builds

`q1h/past/<type>/`: `slot_read(count: i32, value: <type>) -> i64` against the reproducer's
`slot.h` (`va_arg(ap, long)`), built and run:

| `value:` | build | printed |
|---|---|---|
| `i32` -1 | exit 0 | 4294967295 |
| `i64` 4294967338 | exit 0 | 4294967338 |
| `u64` 7 | exit 0 | 7 |
| `u8` 255 | exit 0 | 255 |
| `bool` true | exit 0 | 1 |
| `f64` 1.5 | exit 0 | **4609434218613702656** |
| `ptr` (`malloc(8)`) | exit 0 | **4374435968**, the address |

No diagnostic on any. The critic's item 14 holds: the class is every by-value argument past `...`.

### 1.3 Route (R)'s door, as 2107's note asks, built

`q1h/routeR/slot_own.h`, 6 lines, a `static inline` function of a new name with a fixed prototype
calling the variadic through its real prototype (never a re-declaration, panel 052's veto):

```c
#include "slot.h"
static inline long slot_read_long(int count, long value) { return slot_read(count, value); }
```

Bound `function slot_read_long(count: i32, value: i32) -> i64`: **exit 0, prints -1 at `-O0` and
`-O2`**. **The door is header-verified**, which is what this seat guards: bound with the wrong
type it is refused, exit 1 (`q1h/routeR/wrong_<type>/`):

- `value: f64`: `error[ffi_parameter_type]` *"declared a different kind of number from the header's
  `long` ... C would drop the fraction in silence"*;
- `value: u64`: `error[ffi_parameter_type]` *"a different sign from the header's `long`"*;
- `value: cstr`: `error[lend_kept]` (refused first on the lend);
- `value: i64`: exit 0, prints -1: exact here, `long` being 64 bits. On the box, `long` is 32 bits,
  and whether `i64` is refused against it there is unrun.

**What the door checks and what it does not.** It checks the program's value against the C type
the door names, exactly as any prototype does, and a literal format against the door's own
arguments (1.7). It does **not** check that the callee's `va_arg` reads the type the door names.
A door written `long` for a callee reading `int` compiles, and no route can see that, since it
lives in the callee's body. That knowledge is the door's author's, from the library's
documentation, as it is for a C caller. The direct binding has the same gap and lacks both
checks.

### 1.4 libcurl shows 2421 itself, and the `#undef` takes libcurl's own check off

`q1c/curl/timeout.c`: `curl_easy_setopt(h, CURLOPT_TIMEOUT_MS, v)`, once with `long -1` and once
with `int32_t -1`:

- against the SDK's libcurl 8.7.1 (`-lcurl`): 0 and 0;
- against the keg's 8.23.0 (`PKG_CONFIG_PATH=/opt/homebrew/opt/curl/lib/pkgconfig pkg-config
  --cflags --libs libcurl`): **`(long)-1` returns 43, `CURLE_BAD_FUNCTION_ARGUMENT`; `(int32_t)-1`
  returns 0**. libcurl read 4294967295 ms, about 49.7 days, and accepted it. The keg's header
  warned at that line: *"call to 'Wcurl_easy_setopt_err_long' declared with 'warning' attribute:
  curl_easy_setopt expects a long argument [-Wattribute-warning]"* (`typecheck-gcc.h:50`).

So libcurl ships a width check for this exact mistake, and the `#undef curl_easy_setopt` that
`examples/curl/curl_bound.h` and every wrapper in this sitting writes (defect 2229) is what
takes it off. In Heroes, `q1h/curl/direct_sdk/` (a long option bound `value: i32`, called with
-1) builds at exit 0 over the SDK and prints 0. `q1h/curl/direct_keg/`, the same over the keg, is
refused `ffi_macro_name` before any width is asked.

### 1.5 The libcurl package under each Q1 route (the critic's item 17), over the keg and the SDK

`q1h/curl/doors_<sdk|keg>/net_own.h`, 16 lines: `curl/curl.h`, `#undef curl_easy_setopt` and
`curl_easy_getinfo`, then three doors: `net_set_long(CURL *, CURLoption, long)`,
`net_set_text(CURL *, CURLoption, const char *)`, and `net_get_long(CURL *, CURLINFO, int64_t
*out)`, which reads into a `long` of its own and stores an `int64_t`, since `long *` has no
spelling every leg shares. `main.hero` sets `TIMEOUT_MS -1`, the URL `file:///private/etc/hosts`,
`NOBODY 1` and `FILETIME 1`, performs, and reads `CURLINFO_FILETIME` into `@t: i64`.

- **Over the SDK** (`link "curl"`): exit 0; printed `timeout -1: 0`, `perform: 0`, `getinfo: 0
  filetime true`.
- **Over the keg** (`pkgconfig "libcurl"`, the keg's `PKG_CONFIG_PATH`): **exit 1,
  `error[shadowed_header]`**: *"`/opt/homebrew/Cellar/curl/8.23.0/include/curl/curl.h` stands in for
  the C library's `<curl/curl.h>`, which `./net_own.h` includes"*, its note *"`pkg-config`'s answer
  for the library `libcurl` names that folder"*. The message names the folder as the library's own
  and then refuses it as a stand-in.

What each route makes the package write:

| route | `long` option | `getinfo(@out)` | over the SDK | over the keg |
|---|---|---|---|---|
| (C) pin | direct, `value: i64` (right here, undefined on the box) or `i32` (wrong value, 1.4) | a door anyway (2107 refuses `@` past `...`, panel 242 § 1.4) | builds | refused: `ffi_macro_name` direct, `shadowed_header` through any header |
| (R) / (R0) | a door | a door | builds (1.5) | refused `shadowed_header` (2428) today; **builds under Q2's step 3, `timeout -1: 43`** (4.2) |
| (E) | not judged: `curl_easy_setopt` has no format | a door | as (C) | as (C) |

**So under every Q1 route, libcurl 8.23.0 binds over the keg only after Q2 is answered.** Every
route needs a header of the program's own for curl (the macro, 2229; `getinfo`, 2107), and that
header is what 2428 refuses. Route (C) buys the curl package nothing: its `getinfo` needs the
door anyway.

### 1.6 `open`'s mode (the critic's item 6: the tree binds none)

`q1h/open/direct/`: `function open(path: cstr lent, flags: i32, mode: i32) -> i32` past `...`,
called with `O_CREAT | O_WRONLY` and 416: exit 0, `true`, file mode `-rw-r-----` (0640, umask 022).
**A correct program today**, since a correct C library reads an `int` there (1.1, an inference). (R) and (R0) refuse it.
`q1h/open/door/open_own.h`: `static inline int open_mode(const char *path, int flags, int mode) {
return open(path, flags, mode); }`, bound `mode: i32`: exit 0, `true`, `-rw-r-----`. The cost of
(R) on this correct program is one line of C. `mode: i32` is the spelling every leg shares, since
`mode_t` itself has none: 2 bytes here (`sizeof`); reasoning, unrun: 4 bytes on glibc.

### 1.7 (R0): the format goes into the door, and clang judges it

`q1h/printf_door/ok/say_own.h`: `static inline int say_count(long long v) { return
printf("%lld\n", v); }`, bound `v: i64`, called with 4294967338: **exit 0, prints 4294967338**.
`wrongfmt/`, the same door with `"%d\n"`: **exit 1, `error[ffi_header_refused]`**: *"clang refuses
it at line 5: format specifies type 'int' but the argument has type 'long long'
[-Werror,-Wformat]"*. That is the `twoarity` mistake (`n = 42` at exit 0 today, the critic's
item 14), turned into a compile error by the build's own existing `-Wformat` in header code. No
conversion reader is written.

`long long` is the door's right spelling on every leg. Reasoning: `long long` is 64 bits on all
four, and `%lld` names it by type, so the door is right by C11 §7.21.6.1p9's letter wherever it is
compiled.

### 1.8 Route (E): the literal format in the C call

`q1c/e/e.c`, `clang -fsyntax-only -Wformat`, over the SDK and over the keg's headers. Of eight
`printf` shapes, `-Wformat` flags five: `%d` with `int64_t`, `%ld` with `int32_t`, `%ld` with
`int64_t` (same width), `%zu` with `uint64_t` (same width), `%d` with `double`. It is **silent on
all three non-format variadics**: `slot_read(1, i32)`, `open(..., O_CREAT, i64)` and
`curl_easy_setopt(h, CURLOPT_TIMEOUT_MS, i32)`. (The keg's curl width check fires only at code
generation, as an attribute warning, not under `-fsyntax-only`.) So (E) cannot close 2421, whose
reproducer has no format.

`q1c/e/i64fmt.c`: on this Mac, `%lld` is the one literal `-Wformat` accepts for an `int64_t`.
`%ld` and `%jd` are both flagged (`intmax_t` is `long` here), and `PRId64` expands to `"lld"`
(`clang -E`). The lane measured `%lld` with `int64_t` refused on Linux arm64, where `int64_t` is
`long`. So **under (E), or under (R)'s reader judging by C type, no literal format prints an `i64`
on both this Mac and Linux arm64**: the tree's own `run/ffi-variadic-at-a-fixed-arity` and
`fixedbugs-584` (`%lld`) would be refused on Linux arm64. `PRId64` is a macro that no Heroes
string literal can spell. Only a door, whose C names `long long`, is right on both.

### 1.9 The tree's variadic bindings, enumerated, and which each route refuses

Command: `git grep -n -E '^[[:space:]]+function (printf|fprintf|sprintf|snprintf|sscanf|scanf|
sqlite3_mprintf|sqlite3_snprintf|sqlite3_config|sqlite3_db_config|curl_easy_setopt|
curl_easy_getinfo|curl_multi_setopt|SDL_Log|SDL_SetError|TraceLog|TextFormat|open|openat|fcntl|
ioctl|execl|syslog|va_[a-z_]*|tally_bump)\(' -- '*.hero'`, `docs/` left out. The enumeration is
this name list's, so a variadic under another name is not seen. The `extern` groups of
`examples/` and `packages/` name nine headers (`git grep -E '^\s*extern '`): `hero_os.h`, `math.h`,
`stdio.h`, `sqlite3.h`, `time.h`, `raylib.h`, `curl_bound.h`, `SDL3/SDL.h`, `13-lease.h`.

| binding | past `...` | (R0) numbers | (R) + format exception | widened to pointers |
|---|---|---|---|---|
| `examples/curl/main.hero:58` `curl_easy_setopt` | `value: cstr` | keeps | keeps | **refuses a ladder rung** |
| `run/ffi-variadic-at-a-fixed-arity` `printf("%lld")` | `i64` | refuses (door 1.7) | keeps on this Mac; on Linux by type: refuses | refuses |
| `run/fixedbugs-584-sprintf...` `sprintf("%lld")` | `i64` | refuses | as above | refuses |
| `surface-fixtures/twoarity/as_int` `printf` | `i64`, format a parameter | refuses | refuses (no literal) | refuses |
| `twoarity/as_text`, `twoarity-lent/as_text` | `cstr` | keeps | keeps | refuses |
| `run/fixedbugs-2102` `va_scribble`, `va_length` | `cstr` | keeps | keeps | refuses |
| `unsupported/` 2102, 2103, 2106, `tally_bump` | lends | already refused | already refused | already refused |

The table is read from each route's wording; nothing was built under a compiler that implements
any route. A pointer past `...` keeps one representation on every leg: C11 §6.2.5p28 and its
footnote 48 make pointers to qualified and unqualified compatible types interchangeable as
arguments, and §7.16.1.1p2 excepts `void *` against a character pointer. What a pointer **can**
meet is a callee reading it as an integer, the wrong-option mistake (1.1's last row). A
per-type door does not close that either: `net_set_long(h, CURLOPT_URL, 5)` hands curl a `long`
where it reads a `char *`. That class closes only by a door per option constant, or by libcurl's
own `typecheck-gcc.h`, which the `#undef` takes off. Neither route names it, and this report
does not claim it.

### 1.10 R10, Windows, and the notes that draft a binding past `...`

**R10** (design.md:3900-3910, Part 7 row 10) says *"a parameter has a spelling every leg shares,
a narrower width C converts exactly: `i32` for a C `long`"*. That holds through a prototype (1.3:
the door with `value: i32` prints -1) and is false past `...`, where C converts nothing (1.1).
Under (R0) no number is bound past `...`, so R10 needs only the word *prototyped*: *"a
**prototyped** parameter has a spelling every leg shares"*. A `long *` the callee writes
(`curl_easy_getinfo`'s out) is the field's case, not the parameter's: it has no shared spelling,
and the door (1.5) reads into a `long` of its own and hands out `int64_t`.

**Windows, reasoning, unrun on the box**: with `long` at 4 bytes (panel 242 § 1.4), the door
`slot_read_long(int, long)` bound `value: i32` is exact. Bound `value: i64`, it builds here and
whether the box refuses it is unrun (1.3). `say_count(long long)` with `%lld` is right there by
C's type, `long long` being 64 bits. A format reader under (R) would have to know the box's
`long`, and any reader is per platform.

**The notes** (the critic's item 22). `ffi_call.hero:148` offers *"`function NAME(format: cstr,
value: i64) -> i32`"*, which (R0) refuses. Under (R0) it should draft the door of 1.7: *"a
header of your own with `static inline int NAME_<what>(long long v) { return NAME("<format>",
v); }`: the format is then C, which clang checks against `v`'s type"*. The two notes at
`ffi_past_prototype.hero:205,233` already draft a door (*"declares a `static inline` function
... bind that function instead"*). The by-value refusal (R0) adds would take the same sentence
with the C type the callee reads.

**A route nobody listed, unbuilt**: a binding past `...` naming the C type the callee reads,
say `value: i32 as long`, the compiler emitting the cast. It would give the door's first check
(the value's conversion to the named type) without C. It would not give `-Wformat` on a format,
and like the door it could not check the callee's read. It is a new surface form, so it owes
Principle 0's ticket, a § 13 sentence and the blind seat's run. This seat does not recommend it
while the door costs one line and changes no language.

**Precedent the record already holds** (read, not re-verified this sitting): panel 052's
historian row, *"cgo, GHC's `ccall` and Swift all **refuse** `...` rather than approximate it"*
(`docs/panel/052-what-an-extern-accepts-part-two.md:33`), and panel 176's historian, who marked
cgo's *"Calling variadic C functions is not supported"* verified (`176-reports/historian.md:140`).
(R0) refuses only the numbers past `...`, and its door is this project's own.

## 2. Q2, defect 2428: a library header in a named folder, included by a header of the program's own

Scratch `q2/`. The keg: `PKG_CONFIG_PATH=/opt/homebrew/opt/curl/lib/pkgconfig`, libcurl 8.23.0
(`pkg-config --modversion`); the system's `pkg-config --modversion libcurl` 8.7.1, its `--cflags`
empty.

### 2.1 The reproducer, run

`q2/r2428/`, the coordinator's `coord-curlwrap/undef/` copied (`main.hero`, `heroes.toml`,
`net/`): without the variable **exit 0**, prints 0. With it, **exit 1**,
`error[shadowed_header]`: *"`/opt/homebrew/Cellar/curl/8.23.0/include/curl/curl.h` stands in for
the C library's `<curl/curl.h>`, which `net/curl_own.h` includes"*, note *"`pkg-config`'s answer
for the library `libcurl` names that folder"*. Reproduced. In this reproducer, `net/handle.hero`
**already names `extern "curl/curl.h" pkgconfig "libcurl"` directly**, and that group is not
what is refused. The refusal falls on the same file one include down, under `curl_own.h`.

### 2.2 Which `curl/curl.h` each build reads (`-H`, `-fshow-skipped-includes`, `-v`)

The shadow check runs clang with `-v -H -MG` on an opening file (`build/shadowed/words-*.rsp`),
the keg's nine `-I` folders among its words. By hand, `q2/r2428/probe.c` (`#include
"curl_own.h"`), `clang -fsyntax-only -H -fshow-skipped-includes -I net <cflags>`, filtered to
`curl`:

- **without the variable**: every one of the 13 `curl/` entries, beneath `. net/curl_own.h`, is
  `<SDK>/usr/include/curl/...`, 8.7.1's.
  The search list is `net`, clang's own, `<SDK>/usr/include`.
- **with the keg's cflags**: every one of the 14 `curl/` entries is
  `/opt/homebrew/Cellar/curl/8.23.0/include/curl/...`, `typecheck-gcc.h` among them. **0 SDK
  `curl/` files are opened**, skipped ones counted (`grep -c` printed 0). The search list puts
  the nine keg folders (curl, brotli, zstd, libpsl, libssh2, nghttp2, ngtcp2, openssl@4, nghttp3)
  before `<SDK>/usr/include`.

(A first run of mine printed the SDK's files for the keg too. That was my error, not a finding:
zsh does not split an unquoted `$F`, so the nine `-I` words reached clang as one folder that does
not exist. Rerun with `${=KEG}`, as above.)

**What it links** (`otool -L`): the critic's `curldirect` over the keg links
`/opt/homebrew/opt/curl/lib/libcurl.4.dylib` (compatibility 13.0.0); over the SDK, and 2428 over
the SDK, `/usr/lib/libcurl.4.dylib` (7.0.0). **So `pkg-config` pairs the keg's header with the
keg's library, and the system's with the system's.**

**Is the SDK's `curl/curl.h` ever read in the keg's place, and could it be?** In these builds,
never: 0 entries. It could be only where a unit compiles without the keg's `-I` while the link
takes the keg's `-L`, or the reverse. That reverse is measured in 2.4, and the compiler accepts
it today.

### 2.3 The critic's three probes, built

`q2/critic/<probe>/`, copied:

| probe | over the SDK | over the keg |
|---|---|---|
| `curlwrap` (2428's shape) | exit 0, prints 0 | **exit 1** `shadowed_header` on `curl/curl.h` under `net/curl_own.h` |
| `curldirect` (a group naming `<curl/curl.h>`) | exit 0, `libcurl/8.7.1` | **exit 0, `libcurl/8.23.0`** |
| `sqlitewrap` (`own.h` including `<sqlite3.h>`, flat) | exit 0, `3.54.0` | **exit 1** `shadowed_header` on `/opt/homebrew/opt/sqlite/include/sqlite3.h` under `./own.h` |

The keg's sqlite is 3.54.1 (`PKG_CONFIG_PATH=/opt/homebrew/opt/sqlite/lib/pkgconfig pkg-config
--modversion`), its `--cflags` `-I/opt/homebrew/opt/sqlite/include`.

### 2.4 Route (d): an `--include` folder pairs the keg's header with the SDK's library, accepted today

`q2/pairing/`: `extern "curl/curl.h" link "curl"`, printing the header's `LIBCURL_VERSION_NUM`
and the library's `curl_version()`:

- over the SDK: exit 0, `header 526081` (0x080701), `library libcurl/8.7.1`;
- `--include /opt/homebrew/opt/curl/include`, no `pkg-config`: **exit 0, `header 530176`
  (0x081700, 8.23.0), `library libcurl/8.7.1`**, linking `/usr/lib/libcurl.4.dylib`.

So a build that reads a keg's header through `--include` links the SDK's older library at exit 0,
and the shadow check accepts it, since the header a group names is the program's choice (panel
237's `stdiogroup` ruling). `pkg-config` ties the folder to its own `-L` (2.2). **The arms are
not alike.** I showed no wrong value from this pairing; the class is the coordinator's to read
(§ 6).

### 2.5 Who in the platform includes the names the seven keg-only formulae collide on

Command: `grep -rlE '^[[:space:]]*#[[:space:]]*include[[:space:]]*[<"]<name>' <SDK>/usr/include`,
and the same over `runtime/`:

| name | SDK files including it | which | runtime |
|---|---|---|---|
| `curl/` | 0 | | 0 |
| `sqlite3.h` | 1 | `sqlite3ext.h`, SQLite's own | 0 |
| `curses.h`, `ncurses.h` | 5 | `form.h`, `unctrl.h`, `panel.h`, `menu.h`, `tic.h`: ncurses's own | 0 |
| `term.h` | 1 | `term_entry.h`, ncurses's own | 0 |
| `unicode/` | 21 | all under `<SDK>/usr/include/unicode/`, ICU's own | 0 |
| `readline/`, `llvm-c/`, `clang-c/` | 0 | | 0 |

**No header of the C library and none of the runtime includes any of the names grepped**
(the table's; a formula's header under another name is not seen). The only platform
includers are the same library's own older headers. For those seven, then, the refusal's first
arm (*"a platform or runtime header includes it"*, design.md:3159-3160) fires only where the
SDK's copy of the same library mixes with the keg's. That is the one real hazard there (two
versions of one library in one unit), and the first arm keeps it refused. Every refusal of 2428's
and `sqlitewrap`'s shape comes from the second arm, *"or where a platform folder holds a file of
its name"*.

### 2.6 What a build wanting the keg's libcurl must and must not read

It **must** read every `curl/*.h` from the keg's folder and link the keg's `libcurl.4.dylib`,
which `pkg-config` gives (2.2). It **must not** read any SDK `curl/*.h` in the same unit, and
must not link `/usr/lib/libcurl.4.dylib` under the keg's header (2.4). What it can write today:
a group naming `<curl/curl.h>` directly builds for the functions libcurl declares plainly
(`curldirect`, `curl_version`). **`curl_easy_setopt`, the function every libcurl program calls,
cannot be bound at all**: directly it is `ffi_macro_name` (1.4), and through a header of its own
it is `shadowed_header` (1.5). The way round is panel 242's § 1.7 trick, which copies the keg's
headers into a folder of another name (`curl823/`), and a copied header can drift from the
library it describes.

**Under 4.1's step 3** the build writes nothing new. The wrapper's group says `pkgconfig
"libcurl"`, the user sets `PKG_CONFIG_PATH=/opt/homebrew/opt/curl/lib/pkgconfig`, and that is
all: measured in 4.2 for 2428's reproducer and for the doors of 1.5.

## 3. Q3, defect 2420's two rows left, and the danger side

Scratch `q3/`: the lane's shapes `d`, `e` and `g` copied as `lane-<s>/`, and the critic's
`q3danger/{a,d,live,live2}` copied as `danger-<s>/`. Base compiler `heroes-next`.

### 3.1 Built today

| shape | files | exit | printed or refused |
|---|---|---|---|
| `lane-d` | `lib/clock.h` includes `"../sys/timeb.h"` | 1 | `shadowed_header`, *"which `./lib/clock.h` includes"* |
| `lane-e` | `sys/clock.h` includes `"sys/timeb.h"`, found through `-I .` | 0 | 42 |
| `lane-g` | `clock.h` at the root includes `"sys/timeb.h"` | 1 | `shadowed_header`, *"which `./clock.h` includes"* |
| `danger-a` | `sys/clock.h` includes sibling `"types.h"`, a `sys/types.h` with `#include_next` | 0 | 42 |
| `danger-d` | `lib/clock.h` includes `"../sys/types.h"` | 1 | `shadowed_header` |
| `danger-live` `main` | guarded `sys/types.h`; groups `sys/clock.h` then `sys/socket.h` | **0** | **42 2** |
| `danger-live` `main2` | the same, `sys/socket.h` first | 1 | `shadowed_header`, *"which the C library's `<sys/socket.h>` includes"* |
| `danger-live2` | unguarded `sys/types.h`, clock first | **0** | **42 2** |

### 3.2 Which file C reads, from which folder (`-H`, in the unit's own form)

`q3/lane-<s>/unit.c`, written as the build's unit writes it (`#include "heroes_unit.h"`,
`<hero_os.h>`, then the group in angle brackets), `clang -H -fshow-skipped-includes -I runtime
-I .`:

- `lane-d`: `. ./lib/clock.h`, `.. ./lib/../sys/timeb.h`: read from the includer's own folder.
- `lane-e`: `. ./sys/clock.h`, `.. ./sys/timeb.h`: `"sys/timeb.h"` from `./sys/` would be
  `./sys/sys/timeb.h`, absent, so it is found through `-I .`.
- `lane-g`: `. ./clock.h`, `.. ./sys/timeb.h`: from the includer's own folder.

`e` and `g` print **the same line**, though one came from the search and the other from the
includer's folder, and today `e` builds while `g` is refused.

**Would any platform or runtime `#include <sys/timeb.h>` read the program's file?** None exists:
`grep -rlE '#include [<"]sys/timeb\.h'` over `<SDK>/usr/include` printed 0 files, and over
`runtime/` 0. The SDK holds `sys/timeb.h`. So in the lane's three shapes only the program's own
header reads its file, and those shapes cannot test the danger, as the critic's item 16 says.

### 3.3 The danger shape, both orders: the check judges a file at its first open

`danger-live`, its guarded `sys/types.h`. The Heroes build's own unit
(`build/tu-5ec6785f5f68b316/main.c`, groups at lines 22-23), `-H -fshow-skipped-includes`, every
`sys/types.h` line:

```
. ./sys/clock.h
.. ./sys/types.h
... <SDK>/usr/include/sys/types.h
. <SDK>/usr/include/sys/socket.h
.. ./sys/types.h            <- the SDK's include, resolved to the program's file, skipped by its guard
```

Socket first (`socketfirst.c`, a hand probe; `danger-dlive`'s unit form, socket first, lists the
same shape): `. <SDK>/.../sys/socket.h`, `.. ./sys/types.h`,
`... <SDK>/.../sys/types.h`, and later `.. ./sys/types.h` under `sys/clock.h`, skipped.
**In both orders the SDK's `<sys/socket.h>` includes the program's `./sys/types.h`.** For a guarded file, plain `-H`
shows that line only when it is the first open. Clock first, plain `-H` (`q3/danger-dlive/`)
lists no `sys/types.h` under `sys/socket.h` at all.

The landed check runs `-H -MG` without `-fshow-skipped-includes` (`shadowed_tree.hero:53`), and
it judges a path once, setting `seen[path]` before it asks `refused` (`shadowed_tree.hero`,
`shadows`). So the platform's include is never seen in one order (critic item 16). `danger-live2`,
which has no guard, shows the platform's open even in plain `-H`: its unit
(`build/tu-06c49ea15f0617bd/main.c`) lists `.. ./sys/types.h` beneath
`. <SDK>/usr/include/sys/socket.h` (line 102), and it still builds at exit 0, because the `seen`
map passes the second open unjudged. So the hole is the first-open rule as much as the missing
flag.

(My first hand probe, with the group included in quotes from the probe file, showed the
program's `#include_next` reopening itself under `-Winclude-next-absolute-path`, so that the SDK's
`sys/types.h` was never read. The unit as the build writes it, included in angle brackets, does
reach the SDK's file. The unit's listing is the measurement.)

`danger-dlive` (`lib/clock.h` including `"../sys/types.h"`, guarded, with a `sys/socket.h`
group): today **refused in both orders**, clock first by the second arm on `./lib/../sys/types.h`,
socket first by the first arm on `./sys/types.h`. **One file, two spellings**: `ls -i` printed
inode 260525441 for both. A rule that keys on clang's spelling sees two files. The landed one
reads `package_reads.lexical` first.

`danger-a`, the unit as built (`#include "heroes_standard.h"` after the groups, line 47), `-H
-fshow-skipped-includes`: no `sys/types.h` line beneath `heroes_standard.h`. The runtime's later
headers do not reach the program's file, so `danger-a`'s exit 0 is a correct program.

## 4. The routes I recommend for Q2 and Q3, built as a prototype and run

In my worktree only (`selfhost/cli/shadowed_tree.hero`, `selfhost/cli/shadowed.hero`, edited by
`p243-scratch/ffi-pragmatist/proto/edit.py`, `edit2.py` and `edit3.py`, each anchor asserted to
match once; diffs `proto/prototype.diff` and `proto/prototype-step3.diff`). Built by
`heroes-next build selfhost/main.hero -o heroes-proto`, after `heroes check selfhost/main.hero`
exit 0 and `fmt --in-place` on both files. **A prototype for measurement, not a repair**: nothing
is committed, and the compiler's own tests and the net are unrun on it except where § 4.4 says.

### 4.1 The changes

1. **Every open judged, skipped ones listed** (Q3's danger side, the critic's item 16).
   `WORDS` gains `-fshow-skipped-includes`, and `shadows` sets `seen[path]` only once a file is
   refused, so a later open of a file passed at its first is judged too. A separate `listed` map
   keeps the `-M`-only (`asked`) pass to the files `-H` never showed.
2. **The lane's widening** (Q3): `refused` drops `beside &&`, so *"a file a header includes in
   quotes, found from that header's own folder, is its own"*. `quoted` already reads a quoted
   spelling from the includer's folder, `../` included. One assertion of the compiler's own test
   *"a nested header's sibling named in quotes ..."* moves with it: its row `lib/clock.h`
   including `"../sys/timeb.h"` changes from `apart` (refused) to `own`.
3. **A `pkg-config` folder is its library's, outside the platform's own names** (Q2, the critic's
   route (c), narrowed twice by measurement). `told` passes `refused` the folders whose origin is
   `PKGCONFIG`, and the second arm (*"a platform folder holds a file of its name"*) is skipped for
   such a folder's file, **except** for a name C11 lists (N1570 §7.1.2p2, 29 names: step 2) or
   POSIX.1 lists (56 more, from memory and unsourced: step 3). The first arm (*"a platform or
   runtime header includes it"*) is untouched and comes first.

**Step 1, as first built (excuse with no exception), was unsound, and step 2 still was.**
`q2/pcstdio/`: a `fake.pc` whose folder holds `stdio.h` (`#define BUFSIZ 7`), and the program's
own `own.h` including `<stdio.h>`, bound `BUFSIZ`. The base compiler: exit 1, `shadowed_header`
(the second arm). **Step 1: exit 0, prints 7**, where the SDK's `BUFSIZ` is 1024. The unit's `-H`
showed one `stdio.h`, the fake, beneath `own.h`. No platform header in that unit reads
`stdio.h`, so only the second arm stood between the program and the wrong value. Step 2 refuses
`pcstdio`. `q2/pcsocket/` (a `sys/socket.h` with `#define AF_INET 99`, read by `own.h`):
**step 2: exit 0, prints 99**, where `AF_INET` is 2; base exit 1. Hence step 3.

### 4.2 Step 3, every case, base `heroes-next` against `heroes-proto` (`sources 3f1db37e310b5a55`)

The run's whole output is `proto/step3-run.txt` (50 lines). Each case was built from a clean
`build/` with each compiler:

| case | base | prototype step 3 |
|---|---|---|
| `q2/r2428` over the keg (defect 2428) | exit 1 `shadowed_header` | **exit 0**, prints 0 |
| `critic/curlwrap` over the keg | exit 1 | **exit 0**, 0 |
| `critic/sqlitewrap` over the keg | exit 1 | **exit 0**, `3.54.1` (the keg's) |
| `critic/curldirect` over the keg | exit 0, `libcurl/8.23.0` | exit 0, the same |
| `q1h/curl/doors_keg` over the keg (route (R)'s libcurl doors) | exit 1 | **exit 0**: `libcurl/8.23.0`, **`timeout -1: 43`**, `perform: 0`, `getinfo: 0`, `filetime true` |
| `q2/pcstdio` (`stdio.h` in a `pkg-config` folder) | exit 1 | exit 1 (step 1 printed **7**) |
| `q2/pcdanger` (`_stdio.h`, read by the SDK's `stdio.h`) | exit 1 | exit 1 |
| `q2/pcsocket` (`sys/socket.h`, `AF_INET 99`) | exit 1 | exit 1 (step 2 printed **99**) |
| `q2/pccdefs` (`sys/cdefs.h`, *"which the C library's `<sys/_types.h>` includes"*) | exit 1 | exit 1 |
| over the SDK: `r2428`, `sqlitewrap`, `doors_sdk`, `pairing` | exit 0: 0; `3.54.0`; `timeout -1: 0`...; `header 526081` | the same four |
| `q3/lane-d` (`"../sys/timeb.h"`) | exit 1 | **exit 0**, 42 |
| `q3/lane-e` | exit 0, 42 | exit 0, 42 |
| `q3/lane-g` (`clock.h` at the root, `"sys/timeb.h"`) | exit 1 | **exit 0**, 42 |
| `q3/danger-a` | exit 0, 42 | exit 0, 42 |
| `q3/danger-d` (`"../sys/types.h"`, no platform reader) | exit 1 | **exit 0**, 42 |
| `q3/danger-live` clock first | **exit 0, 42 2** | **exit 1**, *"which the C library's `<sys/socket.h>` includes"* |
| `q3/danger-live` socket first | exit 1 | exit 1 |
| `q3/danger-live2` (unguarded) clock first | **exit 0, 42 2** | **exit 1** |
| `q3/danger-dlive`, both orders | exit 1, exit 1 | exit 1, exit 1, both by the first arm now |

So, on these cases: **2428's shape builds over the keg for curl and sqlite. The four danger
probes stay refused, two of them only because of the standard-name exception. The order hole
closes. The lane's two rows build.** `danger-d` alone builds, a correct program: its unit
(`build/tu-081aa9aa041eb62a/main.c`, `-H -fshow-skipped-includes`) lists two `sys/types.h`
lines, its own open beneath `lib/clock.h` and the `#include_next` to the SDK's, and no other
reader.

### 4.3 What the prototype does not settle

- The POSIX list is from memory. It is the historian's to source (POSIX.1-2017 XBD chapter 13),
  and the platforms' own non-POSIX interface headers (`<SDK>/usr/include/sys/*`, Linux's
  `linux/*`, Windows' `windows.h`) are a question: a `pkg-config` folder holding one, read only
  by the program's own header, is excused by step 3. **Unrun.**
- Group scoping (the critic's (a)) is not built: step 3 excuses a `pkg-config` folder's file
  under any group's header, not only under the group whose answer named the folder.
- `-fshow-skipped-includes`: Apple clang 21 and Homebrew's upstream clang 22.1.8 (the version
  Debian's image carries) both list it in `--help` (*"Show skipped includes in -H output"*), and
  clang 22.1.8 run on this Mac over the SDK prints `danger-dlive`'s skipped `.. ./sys/types.h`
  beneath `<SDK>/usr/include/sys/socket.h`. On Linux and on the box's clang 23 it is **unrun**.
- Cost: unmeasured. The prototype adds no clang process; the listing is longer by the skipped
  lines.
- Shape: `fmt` lays the 85 names out one per line, so `shadowed_tree.hero` grows from 373 to 504
  lines (`wc -l`, base and prototype). A landing puts the two lists in a module of their own, under
  the `layout` suite's ceiling. The `layout` suite is unrun on the prototype.

### 4.4 The compiler's own tests of the four shadow modules, under the prototype

`heroes-proto test selfhost/main.hero --only <title>`, once per title, sequentially, for every
`test` block of `cli/shadowed.hero` (4), `cli/shadowed_tree.hero` (2), `cli/shadowed_words.hero`
(3) and `cli/stand_in.hero` (1), titles from `grep '^test "'`: **9 runs, exit 0 each, 10 tests,
all passed** (`proto/test-*.txt`). One of them carries the assertion 4.1 moved (`lib/clock.h`,
`apart` to `own`). The compiler's other tests and the net are unrun on the prototype.

**The tracked `shadowed_header` cases**: `git grep -l shadowed_header -- tests/golden` names 36
files under `unsupported/` and 2 under `run/`. Through the harness, the compiler `heroes-proto`,
narrowed by its third word (`proto/golden-*.txt`, each read whole):

- `unsupported fixedbugs-23`: **63 passed, 0 failed**, the count lane b29-boundary read on the
  base;
- `unsupported fixedbugs-24`: **7 passed, 0 failed**;
- `run fixedbugs-2420`: **2 passed, 0 failed**.

Every `unsupported` file naming `shadowed_header` lies under those two selectors: the same
`grep` with `fixedbugs-2[34]` taken out printed nothing. So no tracked refusal or build of this
class moved under the prototype.

## 5. Verdicts

### Q1, defect 2421

- **verdict**: **approve (R0)**, scoped to numbers: every by-value integer, `bool`, float or
  by-value record past `...` is refused, its note drafting the door of 1.3 and 1.7, with no
  format exception. Pointers (`cstr`, `ptr`, a handle) stay. **Object** to (R)'s format
  exception and to (E). **Object** to widening the refusal to pointers. **Veto (C)**, as a
  resolution and as a pinned known cost.
- **section**: design.md §1.11 and §4.19 (the acceptance ladder, design.md:3083-3093: rung 1
  *"including variadics on Apple ARM64"*, rung 4 libcurl's variadic `curl_easy_setopt`); §1.12;
  Part 7 row 10, design.md:3900-3910 (R10). The document does not cover a by-value number past
  `...`: spec § 13 is silent (the critic's item 21), and panel 052 left *"the variadic slot's
  unchecked width"* queued (the lane's report).
- **experiment**: 1.1 to 1.9, every program compiled by clang against the real header.
  - Accepted, exit 0: `slot_own.h`'s door printed -1 at `-O0` and `-O2`; libcurl's doors over the
    SDK; the `open_mode` door wrote 0640; `say_own.h` with `%lld` printed 4294967338.
  - Refused, exit 1: the door with `%d` (`-Wformat`); the door bound `f64` or `u64`
    (`ffi_parameter_type`).
  - Plain C, accepted: `narrow.c` read 4294967295 at `-O0` and -1 at `-O2`; `timeout.c` over the
    keg returned 43 for `(long)-1` and 0 for `(int32_t)-1`.
- **argument**: Past `...` the C type is unknowable, so no reading of the binding can be right.
  The value is not even fixed: `narrow.c` reads two values at two `-O` levels, and libcurl takes a
  49-day timeout it should refuse. (C) pins a value that does not exist. (R) can be had without
  its format reader: a one-line door moves the call into the program's own header, where existing
  checks judge the value's conversion and any format (the prototype, `-Wformat` as an error).
  What the callee reads stays the door's word, as in C. (E) is silent on non-format variadics
  and, judged by C type, admits no `i64` literal on both Mac and Linux. Pointers keep one
  representation on every leg.
- **prediction**: Under (R0), `examples/curl` (rung 4) builds unchanged, since it passes only a
  `cstr` past `...`. `run/ffi-variadic-at-a-fixed-arity` (rung 1's regression) builds as a door of
  1.7, prints `42` on this Mac and, unrun, on Linux arm64 with `%lld` and `long long`. And
  `twoarity`'s `as_int` as written is refused at its binding; rewritten as 1.7's door with `%d`
  it is exit 1 in clang's `-Wformat` words. Either way, never `n = 42` for `4294967338`.
- **condition**: a program on the closure list or a ladder rung that must pass a number past
  `...` and cannot use a door. For instance, a variadic reached only through a function pointer:
  none is bound in the tree, unsearched beyond the grep of 1.9. Or a measured case where the
  door's C is refused on one leg while the direct binding is right there.

### Q2, defect 2428

- **verdict**: **approve** the critic's route (c) in step 3's form. A folder `pkg-config` names
  for a library is that library's, so the second arm (*"a platform folder holds a file of its
  name"*) does not reach its files. It still reaches a name C11 or POSIX.1 defines, and the first
  arm stays whole. **Veto** route (c) without that exception: `pcstdio` printed 7 and `pcsocket`
  99 at exit 0. **Object** to (b): `sqlitewrap` has no direct group, and (b) would carry
  `--include`'s mispairing (2.4) into wrappers. (a) is unbuilt, stricter, and acceptable. (d)'s
  premise is measured true.
- **section**: design.md §4.19, design.md:3150-3170 (panel 237's R2), whose stated ground is
  N1570 §7.1.2p3, the *standard* headers. That ground does not reach `curl/curl.h`. §1.11: a
  library the user installed is a library Heroes must bind.
- **experiment**: 2.1 to 2.6 and 4.2. `-H` with the keg's flags: 14 keg `curl/` files, 0 SDK
  files. `otool -L`: the keg's dylib. Prototype step 3: 2428, `curlwrap`, `sqlitewrap` and the
  libcurl doors build over the keg; all four danger probes refused; the SDK builds unchanged.
- **argument**: The refused file is the one the build asked `pkg-config` for, and nothing else in
  the platform reads it (2.5: 0 C-library includers of the names grepped for the seven formulae). Its library is
  the one the link takes. Today every wrapper the compiler's own notes draft is refused for a
  keg-only library, curl and sqlite measured, so libcurl 8.23.0 cannot be bound at all. The second
  arm's real job is the platform's own names: steps 1 and 2 each let one through to a wrong value.
  Narrowing it to those names keeps both refusals and frees the library.
- **prediction**: With step 3's rule landed, the libcurl package of 1.5 builds over Homebrew's
  8.23.0 on this Mac with one 16-line header of its own, and its `long` door returns 43 for -1.
  `pcstdio` and `pcsocket` stay exit 1.
- **condition**: a `pkg-config` folder whose file, named outside C11 and POSIX, is read by the
  program's own header where the platform's file was meant, with a wrong value at exit 0. A
  platform `sys/*` header that POSIX does not list is the first place to look.

### Q3, defect 2420's two rows

- **verdict**: **approve** the lane's widening (*"a file a header includes in quotes, found from
  that header's own folder, is its own"*) **only together with** change 1 of 4.1: every open
  judged, `-fshow-skipped-includes` in the listing. **Object** to the widening alone: it would
  join a refusal that judges a file at its first open, and the critic's `danger-live` and
  `danger-live2` already pass that refusal at exit 0 today.
- **section**: design.md:3156-3170 (*"a header's own sibling included in quotes is its own"*),
  §1.12. The document does not say how the listing sees an include skipped under a guard.
- **experiment**: 3.1 to 3.3 and 4.2. `lane-d` and `lane-g` build (42) under the prototype. No
  SDK or runtime file includes `sys/timeb.h`. `danger-live` and `danger-live2` clock first went
  from exit 0 to exit 1. `danger-dlive`'s one file, inode 260525441, is spelled two ways.
- **argument**: The rows refused are correct programs: C reads both files from the includer's
  own folder, and nothing the platform owns reads them (0 includers). The danger the rule exists
  for is a platform header reading the program's file, and that is a fact about every open, not
  the first. Plain `-H` hides it in one order; `-fshow-skipped-includes` shows it. With every
  open judged, the widening loses nothing: every danger shape stays refused in both orders.
- **prediction**: With change 1 landed, `danger-live` and `danger-live2` are exit 1 in both group
  orders on this Mac. Upstream clang 22.1.8 lists the line on this Mac; on Debian's clang 22 and the box,
  unrun.
- **condition**: a clang on a CI leg that refuses or ignores `-fshow-skipped-includes`. Or a
  correct tracked program that judging every open refuses: unrun beyond the 23 cases of 4.2 and
  the shadow modules' own tests in 4.4.

## 6. Found beside the work, for the coordinator to file (no GitHub write by this seat)

1. **The order hole is wider than the critic's guarded shape** (item 16). `danger-live2`, with no
   guard, is read by the SDK's `<sys/socket.h>` in plain `-H` (line 102 of its unit's listing),
   and it builds at exit 0 because `shadowed_tree.hero`'s `shadows` judges a path at its first
   open only. That is *"a wrong one accepted"*, the rule's own class, though no wrong value is
   shown. 4.1's change 1 refuses it.
2. **`--include` pairs a header with another version's library at exit 0** (2.4):
   `LIBCURL_VERSION_NUM` 0x081700 against `libcurl/8.7.1`. No wrong value beyond the version
   mismatch is shown; the class is the coordinator's to read.
3. **The `#undef curl_easy_setopt` that defect 2229's remedy writes takes off libcurl's own width
   check** (1.4): 8.23.0's `typecheck-gcc.h` warns *"curl_easy_setopt expects a long argument"* on
   2421's exact mistake. Under (R0) the door's `long` makes the check moot for widths. Without
   (R0), every libcurl binding through the `#undef` has lost it. An `improvement` to record
   beside 2421.
4. **The `twoarity` fixture** (the critic's finding, re-read):
   `tests/golden/surface-fixtures/twoarity/as_int.hero:12` binds `value: i64` for `%d`, and
   `suite_surface.hero:687` pins `n = 42`. In C, `q1c/widths.c` reads `4294967338` as 42 the same
   way. Under (R0) its `as_int` becomes the door of 1.7.

## 7. What this seat leaves behind

- `.claude/worktrees/243-ffi-pragmatist`: detached at `89db84347`, with the prototype's edits to
  `selfhost/cli/shadowed.hero` and `selfhost/cli/shadowed_tree.hero` **uncommitted** (diff at
  `p243-scratch/ffi-pragmatist/proto/prototype-step3.diff`), and `heroes-next` and `heroes-proto`
  built. Nothing committed, nothing pushed, no GitHub write, no paid run, Docker untouched.
- Probes: `.claude/worktrees/p243-scratch/ffi-pragmatist/{q1c,q1h,q2,q3,proto}`. No built probe
  binary is still running: none of them loops or waits.
