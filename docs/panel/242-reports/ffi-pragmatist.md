# Panel 242, the ffi-pragmatist's report

Written 2026-11-10 as the runs went, from about 12:20. Seat: the ffi-pragmatist (design.md §1.11,
§4.19, §1.12). Worktree `.claude/worktrees/242-ffi-pragmatist`, detached at `1edabb957`. Scratch
`.claude/worktrees/p242-scratch/ffi-pragmatist/`. Platforms: this Mac (Darwin 27.2.0, arm64), the
container `heroes-linux-arm64:latest`, the box `ssh win` in `C:/w/p242-ffi-pragmatist/`. Every
number has its command beside it. A sentence with no run behind it says *unrun*.

## 0. The compiler I judged with

- `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` in the worktree: exit 0,
  `--version` printed `sources 5aae00294f7cd6fa` (the seed's), 95,895,588 instructions retired
  (`/usr/bin/time -l`).
- `./heroes build selfhost/main.hero -o heroes-next`: exit 0, `sources 45824e7205ca021e`, the
  tree's key as the shared brief names it; 813,154,368,325 instructions retired. The
  compiler-engineer's stage 2 was building at the same moment (`ps`), so no seconds are written.
- `pmset -g assertions`: `PreventUserIdleSystemSleep 1` (powerd, display on), so I took no
  assertion of my own.

Every Heroes run below uses `heroes-next`.

## 1. Question 2, route (c): a `net/http` package over libcurl, compiled

Scratch folders `c1/` (C), `h1/` to `h6/` (Heroes), each named below. Every build with
`heroes-next`.

### 1.1 The C an index read needs, compiled against the real header

`c1/index_read.c`, 58 lines (`wc -l`): every flag `fetch/transport.hero`'s `got` passes the curl
program (its word list, lines 68-94) as its libcurl option, a write callback into a buffer bounded by the cap,
`CURLOPT_ERRORBUFFER`, and `curl_easy_getinfo(CURLINFO_RESPONSE_CODE)` for `--write-out
%{http_code}`. `clang -std=c11 -Wall -Wextra -Werror -pedantic`:

| header | clang | `file://` read | cap 5 | missing file | a folder | `http://` |
|---|---|---|---|---|---|---|
| SDK libcurl 8.7.1 (`pkg-config --modversion libcurl`) | exit 0 | code 0, 22 bytes | code 63 | code 37 | code 0, 0 bytes | code 1, *Protocol "http" disabled* |
| Homebrew libcurl 8.23.0, keg-only (`PKG_CONFIG_PATH=/opt/homebrew/opt/curl/lib/pkgconfig`) | exit 0 | the same | 63 | 37 | 0, 0 bytes | 1 |

**A correction to the brief**: *"this Mac has 8.7.1 ... so neither shows it"* reads
`pkg-config`'s default answer. This Mac also holds **libcurl 8.23.0**
(`/opt/homebrew/Cellar/curl/8.23.0`, `brew info curl`: keg-only), and its `typecheck-gcc.h:44-47`
wraps `curl_easy_setopt` in `CURL_IGNORE_DEPRECATION`, which is `_Pragma("GCC diagnostic push")`
(`curl.h:51-55`). That is defect 2229's shape, here. **C does not mind it**: the same 58 lines build
with `-Werror -pedantic` and read the same. Only the compiler's binding probe is affected.

### 1.2 The binding as a package would carry it, `pkgconfig "libcurl"` over `curl/curl.h`

The brief's form, `h1/`: `[dependencies.net] path = "net"`, `use <net>/http`.

- **One variadic, one declaration per module.** `curl_easy_setopt` declared twice in one group, at
  `value: cstr` and `value: i64`: `error[declared_twice]`, exit 1. Split over two modules, a call
  from a third is `error[extern_across_modules]` (exit 1, *"write a Heroes function beside the
  `extern`"*). So a package needs **one module per C value type of the option**, with a wrapper in
  each, and a shared handle module, `handle.Curl` naming the record across them (checked exit 0).
- **A string option**: `value: cstr lent` past `...` is refused `ffi_writable_parameter`
  (design.md:2960, condition (c)); the lease form, `x: cstr @= text.lease()` and `end_lease(@x)`,
  builds, as `examples/curl/main.hero:84-86` does.
- **The plain header against 8.7.1** (`h1/`): build exit 0, runs, `curl_easy_perform` 0.
- **The plain header against 8.23.0**: build exit 1, and **not** defect 2229's `ffi_macro_name`.
  Ten `error[shadowed_header]`s, one for each of `curlver.h`, `system.h`, `easy.h`, `multi.h`,
  `urlapi.h`, `options.h`, `header.h`, `websockets.h`, `mprintf.h` and `typecheck-gcc.h`, each
  *"stands in for the C library's `<curl/curlver.h>`, which `curl/curl.h` includes"*
  (`h1/build823.log`). But `curl.h` includes every one of them **in quotes**
  (`grep -n '#include "' curl.h`: lines 62, 63, 3342-3348, 3355), and design.md:3152-3153 says
  *"a header's own sibling included in quotes is its own"*. The SDK holds files of the same names,
  so the rule fired. **A finding for the coordinator to file, not filed by me (no GitHub
  write)**: a correct program, binding Homebrew's libcurl the way Homebrew's own `brew info curl`
  says to reach it, refused with a message that misreads the include spelling. Searched for it:
  `gh issue list --search` with `keg-only`, `Homebrew curl`, `sibling quoted shadowed`, `curlver`
  and `pkgconfig shadowed SDK`, all states. Found only the rule's own closed history (2343, 2344,
  2356, 2359). By verification.md § Bounded discovery's list this is `blocking`'s *"a correct
  program refused"* and *"a false message"*. It is beside this sitting, not inside it, and it
  means **defect 2229's 8.18 shape cannot be reached by the compiler on this Mac at all**: the
  refusal comes first.

### 1.3 The write callback: three spellings, none both sound and able to read

libcurl's write data is `char *data, size_t size, size_t n` and *"is not null-terminated"*
(`CURLOPT_WRITEFUNCTION.3:22`, 8.23.0's manual). In `h2/` and `h3/`:

| callback parameter | compiler | what it gives |
|---|---|---|
| `data: [u8] counted_by n` | parse error, `expected_function_type_params_close` | refused: a function type spells no `counted_by` |
| `data: ptr` | builds | nothing: a `ptr` is opaque (spec line 90), no form reads its bytes |
| `data: cstr`, read by `validated()` | **build exit 0, `--sanitize` exit 0** | reads to the first zero byte: 7 bytes holding a zero (`h3/nul.txt`), `validated()` read **2**, and nothing said so |

On `file://` libcurl leaves a zero after each chunk (1023 bytes into a 1024-byte buffer,
`CURLOPT_BUFFERSIZE` 1024, 1024- and 5000-byte files, every chunk read exactly, ASan silent), so
the read past the end that the manual permits is **unrun**. It would need `https://`, and this
sitting runs `file://` alone. What *was* measured is the wrong value at exit 0 above.

### 1.4 Two things the index read needs that a binding cannot say past `...`

- **The HTTP status.** `got` tells a 404 or 410 (`absent`) from everything else (`unreachable`) by
  curl's `--write-out %{http_code}` (`transport.hero:108-118`). `curl_easy_getinfo(h, info: u32,
  @out: i64)` is refused `ffi_parameter_type`, exit 1, *"past a header's `...` the header gives
  none"*, and the compiler's own note prescribes the remedy: *"write a header of your own that
  includes `curl/curl.h` and declares a `static inline` function"* (`h4/`, first build).
- **Every `long` and `curl_off_t` option has no checked width.** Past `...` C converts nothing.
  `c2/slot.c`, a variadic reading `va_arg(ap, long)` after the caller passed an `int`: `int 5` read
  as 5, **`int -1` read as 4294967295**, at `-O0` and `-O2` on this Mac. The Heroes binding
  `opt_size` at `value: i32` (`h5/`) builds exit 0 and its cap held in 3 of 3 runs (the slot's
  high half happened to be zero). Nothing checked it. At `value: i64` it is right here, and on
  Windows, where `long` is 32 bits (`c2/lw.c`: `sizeof(long)` printed 4 on the box and 8 on this
  Mac), it is C's undefined behaviour (C11 §7.16.1.1p2); that call is unrun on the box, which has
  no libcurl.
  design.md:3891-3894's shared spelling for a `long` parameter, `i32`, rests on *"a narrower width
  C converts exactly"*, which is true through a prototype and false past `...`.

### 1.5 The one route that ships no C: `CURLOPT_WRITEDATA` with a `FILE *`

`h4/`: 8 modules, 116 lines of Heroes (`wc -l *.hero`): `handle`, `opt_text` (leased), `opt_long`,
`opt_size`, `opt_sink`, `sink` (`fopen` and `fclose` from `stdio.h`), `info`
(`curl_easy_strerror`) and `main`. libcurl's default write function `fwrite`s into the file. Built
exit 0 against 8.7.1; `file://` read 0, cap 63, missing 37, `http` 1. **But it drops the status**
(1.4), its messages become `curl_easy_strerror`'s generic words (*"Couldn't read a file:// file"*,
where the curl program names the path), and libcurl's own manual refuses it on Windows:
*"If you are using libcurl as a Windows DLL, you must also use CURLOPT_WRITEFUNCTION(3) if you set
this option or you might experience crashes"* (`CURLOPT_WRITEDATA.3:21-23`). No test pins curl's
words: `git grep -E "curl. exited|Couldn't open file|curl: \("` under `tests/` printed nothing.

### 1.6 The route that is header-verified: a header of the package's own

`h6/net/net_http.h`, **43 lines of C** (41 non-blank): `curl/curl.h` with the `#undef` of
`examples/curl/curl_bound.h`, a write callback into a bounded buffer, a typed door per value type
(`net_set_text`, `net_set_long(CURL *, CURLoption, long)`, `net_set_size(..., int64_t)`,
`net_set_body`), `net_status`, and a body handle the Heroes side copies out through
`@out: [u8] counted_by n lent`. `h6/net/http.hero`, 59 lines of Heroes. The checker caught my own
mistakes against the header, as it should: `curl_easy_perform` declared `-> i32` is
`ffi_return_type` (*"returns `CURLcode` ... not `i32`"*), and `lent` on a handle is `lent_shape`.
Corrected, against 8.7.1: build exit 0; `file://` code 0, 22 bytes; cap 5, code 63, 5 bytes kept;
missing 37; `http` 1. `--sanitize`, exit 0. `leaks --atExit` on the `-O0` binary: *"0 leaks for 0
total leaked bytes"*. The `long` door takes `v: i32` through a prototype, design.md:3893's spelling
for all four legs. **This header is C of the package's own**: function bodies, an allocator and a
callback. For a *program* it is the shape design.md:598-604 sanctions: *"a function-like macro ...
is reached through a `static inline` function in a header of the program's own"*, and shims are
*"for the hard cases"*, which a variadic, a callback and defect 2229's macro are. Whether a
*package* may ship it is the ruling design.md:572 leaves: *"none ships platform C of its own"*,
and this reaches no OS, so it is not platform C. It is not panel 241's R5 route (b) either, which
was 122 lines of a hashing algorithm in C, refused as *"no shim and no hard case"*. (Corrected
while writing: a first draft of this line called it that route, from the critic's summary before I
had read R5.)

### 1.7 Linux arm64, and what defect 2229's shape actually is

`linux.sh` in `heroes-linux-arm64:latest` (Debian 13 trixie, Debian clang 22.1.8, libcurl 8.14.1,
by `pkg-config --modversion libcurl`; my worktree copied in read-only, the compiler built in two
stages there, `sources 45824e7205ca021e`), log `linux-out/run.log`: `c1` clang exit 0, reads
identical to the Mac's (code 0, 63, 37, folder 0, `http` 1); `c2/slot.c` the same, `int -1` read
as **4294967295**; `h1` (plain header) build exit 0; `h4` (`FILE *`) build exit 0, read 0, cap 63;
`h6` (package header) build exit 0, read 0, cap 63 with 5 bytes, missing 37, `--sanitize -O0`
exit 0.

**The brief's premise, corrected by command.** 8.14.1's `typecheck-gcc.h:47` already wraps
`curl_easy_setopt` in `CURL_IGNORE_DEPRECATION`, and all three versions define that with a
`_Pragma` under clang (`grep -B8 'define CURL_IGNORE_DEPRECATION' curl.h`). The difference is
whether `typecheck-gcc.h` is included. clang presents `__GNUC__ 4`, `__GNUC_MINOR__ 2`
(`clang -E` on this Mac, clang 21, and in the container, clang 22), and 8.7.1's `curl.h:3222-3225`
and 8.14.1's `curl.h:3332-3335` ask for GNU C 4.3 or later, so **under clang neither includes the
type checker**. 8.23.0's `curl.h:3351-3355` adds `(defined(__clang__) && __clang_major__ >= 14)`.
So defect 2229's shape is **a libcurl whose guard names `__clang__`, under clang 14 or later**,
8.18.0 on the CI's Ubuntu 26.04 among them (defect 2229's body), not 8.18 as such.

**Reproduced on this Mac without the `shadowed_header` refusal of 1.2**: 8.23.0's headers copied to
`inc823/curl823/` (a folder name the SDK does not hold), a scratch `pc823/curl823.pc` naming them
and Homebrew's `-L`, group `extern "curl823/curl.h" pkgconfig "curl823"` (`h7/`): build exit 1,
`error[ffi_macro_name]` at `net/opt_long.hero:11:14`, its note prescribing *"a header of your own
that includes `curl823/curl.h` and then writes `#undef curl_easy_setopt`"*. The package header
(`h8/`, `h6` with `#include <curl823/curl.h>`): build exit 0, linked
`/opt/homebrew/opt/curl/lib/libcurl.4.dylib` (`otool -L`), read code 0, cap 63. **So a `net/http`
package's binding has to carry its own header to build on the CI's x86-64 and arm64 legs**, which
both run Ubuntu 26.04 since defect 2229's repair.

### 1.8 The seed's one clang line under route (c), and where it builds

What the line must gain, measured on this Mac over `h6`'s one-file C (`heroes build main.hero
--emit-c -o one.c`, whose line 22 is `#include <net_http.h>`), built the way the seed is:

| line | result |
|---|---|
| `clang -I runtime one.c runtime/runtime.c -o x` (the seed's today) | exit 1, `fatal error: 'net_http.h' file not found` |
| `... -I net ...` | exit 1, the link: `"_curl_easy_strerror", referenced from` |
| `... -I net ... -lcurl` | **exit 0**, and the binary reads `file://` |

So the compiler's line would become `clang -I runtime -I <the placed folder of net> seed/heroes.c
runtime/runtime.c -lcurl -o heroes` on this Mac, and `$(pkg-config --cflags --libs libcurl)` in
place of `-lcurl` wherever the headers are not on clang's own path. The placed folder's name
carries a key (`examples/packages/heroes.packages/core@0.2.0-8c07af1fca61d171/`), so the line
would move with each version the compiler takes.

| where | libcurl development files | the line with libcurl |
|---|---|---|
| this Mac | SDK 8.7.1, `-lcurl` with no search path; Homebrew 8.23.0 keg-only | builds (above) |
| `heroes-linux-arm64` | 8.14.1, `/usr/include/aarch64-linux-gnu/curl/` | builds (`h6` in `linux.sh`) |
| the box (Windows 11, `MINGW64_NT-10.0-26100`, clang 23.1.1, `x86_64-pc-windows-msvc`) | none: `#include <curl/curl.h>` is *"fatal error: 'curl/curl.h' file not found"*; no `pkg-config` on the PATH (`which`); Git's `mingw64/bin` holds `libcurl-4.dll` and `libcurl-openssl-4.dll` with no header and no import library; `C:/w/vcpkg/installed/` holds no `x64-windows/include` or `lib` (listed read-only, nothing touched) | the seed's line today: exit 0, `sources 5aae00294f7cd6fa`; **with `-lcurl`: exit 1, `lld-link: error: could not open 'curl.lib'`**, and the include would fail before it |
| the CI's Windows leg | none, by `ci.yml:517` (*"sqlite, curl and raylib are not there"*), carried | unrun here, and the box agrees |
| `ubuntu:24.04` arm64, no libcurl (`ls /usr/lib/aarch64-linux-gnu \| grep -c libcurl` printed 0) | none | `heroes` from today's seed (built in the Debian container) runs, `--version` exit 0; **the seed's own line with `-lcurl` added** (`linux4.sh`: exit 0 in the Debian container, `readelf -d` showing `NEEDED libcurl.so.4`) gives a `heroes` whose `--version` and `check /dev/null` both **exit 127, *"error while loading shared libraries: libcurl.so.4: cannot open shared object file"***; a small libcurl program, the same (`linux2.sh`) |

The last row is the *"loads for every verb"* cost, run on `heroes` itself. A `heroes` that links
libcurl does not start on such a machine: `check` measured, and `build` and `run` stop at the same
loader. None of the three reaches an index (panel 235's R8). Today the curl program's absence is
told by name, at exit 2, by `fetch` alone (`transport.hero:97-101`).

## 2. Question 2, routes (b) and (d): an `os` package over `hero_os.h`, compiled

`os1/`: `[dependencies.os] path = "os"`, `os/process.hero` 52 lines of Heroes (after `heroes fmt
--in-place`), `main.hero` 9. Its group is `extern "hero_os.h"`, from the package, not the
compiler: `HERO_OS_OK`, `hero_run_reset`, `hero_run_arg`, `hero_run_limit`, `hero_run_go`,
`hero_run_why`, `hero_run_discard_path`, `hero_run_exe_suffix`. A program's output is written to a
file by `hero_run_go` and read back with the built-in `read_file`, the shape `cli/process.hero` and
`transport.got` use. On this Mac: build exit 0; `osrun uname -s` read `Darwin`; `osrun sh -c '...;
exit 3'` told *"`sh` exited 3"*; `osrun no-such-program-p242` told *"could not be started, the
system's number 2"*; `--sanitize` exit 0. **Linux arm64** (`linux3.sh`, the container's own
compiler of the tree): read `Linux`, *"`sh` exited 3"*, number 2. **The box** (`win3.sh`, the box's
compiler of the tree): read `MINGW64_NT-10.0-26100`, suffix `.exe`, *"`sh` exited 3"*, number 2.

**So route (b)'s binding has no FFI obstacle**, and route (d) is the same binding with `git` and
`curl` still run as programs. Neither moves a byte across the C boundary differently from today:
`HeroStr` in and out as now, the same header, the same `lent` words. What route (b) costs is the
compiler's 96 `cli/process` users (the shared brief's count), which is the compiler-engineer's
ledger, not mine.

**Defect 2413's shape, which a package lifted from `cli/process.hero` would inherit.**
`os2/os/dir.hero` binds `hero_dir_scan`, `hero_dir_at` and `hero_dir_release` under `extern
"stdlib.h"`, as `cli/process.hero:66-69` does. The right signature builds exit 0 and lists. The
wrong one (`hero_dir_at(...) -> i64`) is refused `ffi_return_type` exit 1, *"returns `HeroStr` in
`stdlib.h`"*, and the same under `extern "hero_os.h"`. **So the header verification still holds**:
clang reads `hero_os.h` in every unit and the binding is checked against the real declaration.
What is wrong is the file the message names, as 2413 says (`adjacent`). The lift repairs it by
moving three lines into the `hero_os.h` group, and nothing at the boundary is at risk either way.

**What task 2263 owes each `HeroStr` result such a package binds**, read from the bodies at
`1edabb957`. No comment in `hero_os.h` states it for any of these, and no check reads it:

| function | `hero_os.h` | body | what it hands the caller |
|---|---|---|---|
| `hero_run_discard_path` | :314 | `run.c:176-178`, `hero_str_from_bytes` | a new string, its one reference the caller's |
| `hero_run_exe_suffix` | :319 | `run.c:138-144`, `hero_str_from_bytes`, `""` on POSIX | a new string; `""` is `hero_str_empty()` (`str.c:418-422`), the static empty block |
| `hero_dir_at` | :343 | `dir.c:172-177`, `hero_str_from_bytes` of the listing's name | a new string, a copy, so it outlives `hero_dir_release` |
| `hero_dir_at_shown` | :348 | `dir.c:187-192`, `hero_bytes_shown` (`os.c:653-657`) | a new string, the same |
| `hero_env_shown` | :84 | `os.c:860-899`, `hero_bytes_shown` or `""` | a new string, or the static empty block where absent |
| `hero_file_read_shown` | :77 | `os.c:835-840`, the same | the same |

Every one is *"a new string the caller owns, or the static empty block"*, and the empty block
must be safe to release. That is the one sentence 2263 needs per function. A check that reads it
is unbuilt (task 2263's *"where a check reads it"*).

## 3. Question 3, the environment's write, run on the box

### 3.1 The empty value, by route, three readers each

`env/empty.c`, one file for the three platforms (`clang -std=c11 -Wall -Wextra -Werror`, exit 0 on
each). It sets a variable to `set`, then to `""`, and asks this process's block or `environ`, the C
runtime's copy (`_wgetenv` / `getenv`), and a child started with no environment of its own, as
`run.c:1359` and `run.c:1083` start one:

| route | platform | block / `environ` | C runtime's copy | child |
|---|---|---|---|---|
| `setenv(name, "", 1)`, today's POSIX arm | this Mac, Linux arm64 | empty | empty | empty |
| `_wputenv_s(name, L"")`, **panel 241's recorded route** | the box | **absent** | absent | **absent** |
| `SetEnvironmentVariableW(name, L"")`, **today's Windows arm** (`os.c:906-913`) | the box | empty | absent | empty |
| `SetEnvironmentVariableW(name, NULL)` | the box | absent | absent | absent |
| `_wputenv_s("set")` then `SetEnvironmentVariableW(L"")` | the box | empty | **a value**, stale | empty |

**So `_wputenv_s(name, L"")` removes the variable where `setenv(name, "", 1)` keeps it, empty.**
Panel 241's route would change `hero_env_put("")`'s meaning on Windows. Today's arm keeps the POSIX
meaning for every reader but the C runtime's copy, which it never writes (its own comment,
`os.c:904-905`, says so).

### 3.2 `hero_env_put` through the runtime, both arms, on the box

`p1/puttest.hero` binds `hero_env_put` (`hero_compiler.h`), `getenv` (`stdlib.h`, the reader panel
241's R7 named: *"on Windows a value its program's `getenv` cannot see"*), `hero_env_shown` and the
run doors, and re-runs itself as the child. Built with the box's compiler of the tree
(`C:/w/p242-ffi-pragmatist/tree/heroes-next.exe`, two stages from the seed, `sources
45824e7205ca021e`):

| arm | `put "set"`: `getenv`, block, child | `put ""`: `getenv`, block, child |
|---|---|---|
| today's, the trunk's runtime | **absent**, `[set]`, `[set]` | absent, empty, empty |
| over `_wputenv_s`, POSIX meaning kept (`rt-put/`, **7 lines added, 1 removed** in `os.c`, `put-arm.diff`) | `[set]`, `[set]`, `[set]` | absent, empty, empty |
| this Mac, today's (POSIX) | `[set]`, `[set]`, `[set]` | empty, empty, empty |

The arm that keeps the POSIX meaning calls `_wputenv_s(name, value)` and, for `""`, writes the
block after it with `SetEnvironmentVariableW(name, L"")`. **No arm can make `getenv` answer `""`
on Windows**: the C runtime reads an empty value as a removal (row 2 of 3.1), so it cannot hold
an empty variable at all. That is a fact about the UCRT, measured, not a choice an arm makes.

### 3.3 Route (e): the child's environment given at the spawn

Built in `rt-e/`, a copy of `runtime/` (`route-e.diff`, **128 lines added**, 111 of code and 17
of comment, **3 removed**, in `parts/run.c` and `hero_os.h`):

- `void hero_run_env(HeroStr name, HeroStr value)`, staged as `hero_run_arg` is: thread-local,
  given back by `hero_run_reset`, counted in `hero_run_words_held` so the exit check sees one left
  standing. A name that is empty or holds `=` or a NUL, and a value holding a NUL, are panics, as
  a word's NUL is (`run.c:88-95`).
- **POSIX**: the child's `envp` is built in the parent before `fork` (this process's `environ`, each
  staged name replacing its namesake, then the staged pairs), and the child sets `environ` to it
  just before `execvp`. Nothing is allocated between `fork` and `exec`. The array is released in
  the parent after the fork. `NULL` where nothing is staged, so the child inherits exactly as today.
- **Windows**: a wide block from `GetEnvironmentStringsW`, each staged name replacing its
  namesake compared as Windows compares, case-blind (`CompareStringOrdinal(..., TRUE)`), drive
  entries (`=C:=...`) kept, sorted by name as `CreateProcess` asks, passed to the existing
  `CreateProcessA` with `CREATE_UNICODE_ENVIRONMENT`. The error is read before the block is
  released, keeping `run.c`'s *"read on the very next line"* rule.

`e1/envtest.hero` stages `P242_U` = `café ∂ 日本 😀` (21 bytes, four code points above ASCII, one
above the BMP), `P242_E` = `""`, `p242_over` = `child` against the parent's `P242_OVER=parent`,
and `P242_KEEP` = `replaced` against the parent's `P242_KEEP=kept`, then re-runs itself as the
child, which reads each with `hero_env_shown`. Then the parent reads its own `P242_U` and
`P242_OVER`. Built with `HEROES_RUNTIME=<rt-e>`:

| platform | `P242_U` in the child | `P242_E` | `P242_OVER` | `P242_KEEP` | the parent's own `P242_U` | sanitized |
|---|---|---|---|---|---|---|
| this Mac | `[café ∂ 日本 😀]` 21 bytes | empty, 0 bytes | `[parent]` (POSIX: `p242_over` is another name) | `[replaced]` | **absent** | `--sanitize` exit 0; `leaks --atExit` 0 leaks |
| Linux arm64 (`linux3.sh`) | the same, 21 bytes | empty | `[parent]` | `[replaced]` | **absent** | `--sanitize -O0` exit 0 (LeakSanitizer's leg) |
| the box | the same, 21 bytes | empty | **`[child]`** (Windows names are case-blind) | `[replaced]` | **absent** | unrun (the box has no ASan leak leg) |

Against the trunk's runtime the same program is `error[ffi_unknown_name]`, *"`hero_os.h` declares
no `hero_run_env`"*, exit 1: the binding is held to the header, as it should be.

**Route (e) removes the process-wide write, the C runtime's copy question and the empty-value
question together.** Nothing is written into the process, and the block the child receives is
built from the system's own block, never from the C runtime's copy. The value above ASCII crosses
whole on Windows only because the block is wide. An ANSI block would pass through the code page
(unrun in that form: I built the wide one, which is the robust choice).

### 3.4 Does any caller still need a process-wide write?

Every caller, read at `1edabb957` (the shared brief's grep, re-run): `cli/symbolized.hero:57`
(`ASAN_OPTIONS` for the program it runs), `fetch/made_repo.hero:68-69` (`GIT_AUTHOR_DATE`,
`GIT_COMMITTER_DATE` for git), `fetch/transport.hero:160-161` (`GIT_TERMINAL_PROMPT`,
`GCM_INTERACTIVE` for git), `tests/harness/suite_fetch.hero:86` (for git), and
`tests/harness/case_setting.hero:214,223`. Each writes the variable for the next child it starts and
for nothing else in its own process. **One premise to state**: `execvp` and `CreateProcessA` find
the *program itself* through the parent's `PATH`, not the child's. So route (e) gives a staged
`PATH` to the child's own lookups and not to the lookup of the child. `case_setting.hero` starts
the case's compiler by its path or `/usr/bin/env` (`ENV`, line 62), and the `PATH` it sets is read
by that compiler's lookups of clang and git. So it is a child's variable. That is read, not run:
the harness was not moved onto route (e) in this sitting, and `case_setting`'s cases are unrun
under it. **No caller I found needs the write to reach its own process.** Searched: `git grep -n
'hero_env_put('` over `*.hero` in the worktree. Also unrun: whether a process-wide `setenv` races a
thread that reads the environment in the compiler (POSIX calls `setenv` MT-unsafe). Route (e)
would remove that question too, since nothing is written.

### 3.5 What putting a door into `hero_os.h` costs, either door

Panel 241's route moves `hero_env_put` there, and route (e) adds `hero_run_env` there. Each is one
name in the promised header (decision 2228, design.md:3641-3645). Each is a `push_macro` line in
`heroes_guard_open.h` (609 today, `grep -c push_macro`), its `pop_macro` in
`heroes_guard_close.h`, and a word in `selfhost/emit/guarded_names.hero`'s list, which
`emit/macro_guard.hero`'s tests hold to the header. § 3.6 runs those tests. Neither returns a
`HeroStr`, so task 2263's ownership sentence does not arise for either. Route (e) then retires
`hero_env_put` from `hero_compiler.h` once its five callers move, so the count of doors stays where
it is. Panel 241's route moves a door whose Windows meaning of `""` must first be chosen (3.1, 3.2).

### 3.6 The guard list holds a new `hero_os.h` door, run

In `tree-e/`, `git archive` of the worktree with `rt-e`'s `run.c` and `hero_os.h` and nothing
else, `heroes-next test selfhost/main.hero --only "<title>"` from that folder:

- *"the runtime's guard files hold the list, the open saving and the close giving back"*: exit 0
  (the files and the list agree with each other, and neither names the new door);
- *"the list holds every word the runtime's headers write"*: **exit 1**, `assert failed:
  missing.join(" ") == ""`, `left: "hero_run_env"`, at `emit/macro_guard.hero:263:12`.

So the move is held by the compiler's own tests, and its cost is three lines in `tree-e2/`:
`#pragma push_macro("hero_run_env")` and its `pop_macro`, each after `hero_run_discard_path` in byte
order, and the word written into `guarded_names.hero:98`'s line (`heroes fmt` exit 0 on it); `diff
-u` counts 4, that line being rewritten. The green run is in
§ 3.7. `hero_env_put` moved by panel 241's route costs the same three lines, plus its declaration
leaving `hero_compiler.h:22-28`.

### 3.7 The same two tests, with the three lines

In `tree-e2/` (route (e) plus the three lines), the same `heroes-next test selfhost/main.hero
--only "<title>"`: *"the runtime's guard files hold the list, ..."* **exit 0**, `1 test, all
passed`; *"the list holds every word the runtime's headers write"* **exit 0**, `1 test, all
passed`. Red without the lines, green with them: the door's guard cost is exactly those three
lines, and the compiler's own tests tell a session that forgets one.

## 4. Verdicts

### Question 2, the transport (my question)

- **(a) the transport stays over `cli/process`: approve.** It moves nothing across the boundary.
  The curl program ships with macOS and with Windows, where libcurl's development files do not
  ship with Windows: `/usr/bin/curl` 8.7.1 on this Mac, `C:\Windows\System32\curl.exe` 8.16.0 and
  Git's 8.21.0 on the box. It is absent from `heroes-linux-arm64` (`/usr/bin/curl`: no such file; `dpkg -l` holds
  `libcurl4-openssl-dev` and no `curl`), and that absence costs `fetch` alone, told by name at exit
  2 (`transport.hero:97-101`). It carries TLS, defect 2229's macro and the Windows link in a
  program that is not ours.
- **(b) an `os` package over `hero_os.h` that `cli/process` moves onto: approve at the boundary**,
  on § 2: bound from a package, built and run on this Mac, Linux arm64 and the box (`Darwin`,
  `Linux`, `MINGW64_NT-10.0-26100`, suffix `.exe`), `--sanitize` exit 0 on this Mac. Conditions:
  the lift moves `hero_dir_*` into the `hero_os.h` group (2413's shape, § 2), and task 2263's
  sentence is written for each of the six `HeroStr` results the package binds (§ 2's table). The
  96 modules are the compiler-engineer's cost, not a boundary one.
- **(c) a `net/http` package over libcurl: object.** And **veto on (c) in any form without a header
  of the package's own**, which is the form left if design.md:572 (*"none ships platform C of its
  own"*) is read as refusing every C file in a package. The program that breaks: a `net/http` package reading a body. Its
  `cstr` write callback builds at exit 0 and read 2 of 7 bytes, a wrong value with nothing said
  (§ 1.3). Its `long` and `curl_off_t` options past `...` carry a width nothing checks: `int -1`
  read as 4294967295 on this Mac and Linux arm64 (§ 1.4). Its `FILE *` form is one libcurl's own
  manual says *"might experience crashes"* as a Windows DLL (§ 1.5). That is unsoundness at the
  boundary, §1.12, and the veto is on that alone. With its own 43-line header (§ 1.6) the binding
  is sound and header-verified on this Mac, Linux arm64 and libcurl 8.23.0. My objection then is
  cost, measured:
  - the header is C of the package's own, which design.md:598-604 sanctions for a program and
    design.md:572 does not say a package may ship (a ruling, the spec-warden's to read);
  - the seed's line gains `-I <placed folder>` and `-lcurl` (§ 1.8);
  - the box cannot build it (`curl.lib` not found; no header, no `pkg-config`), and neither can
    the CI's Windows leg by `ci.yml:517`;
  - a `heroes` that links libcurl does not start where libcurl is absent, for every verb: exit
    127 in `ubuntu:24.04` (§ 1.8);
  - all to move the index's HTTP read alone, since `git` stays a program (panel 235's R10, R19).
- **(d) `git` and `curl` stay programs, only the layer that runs them moves to a package:
  approve**, on § 2's measurements. It is (b)'s binding with (a)'s transport.

### Question 3, the environment's write (my question)

- **(e) the child's environment given at the spawn: approve, and recommended as the robust
  route.** Built (128 lines, § 3.3), run on all three platforms: the value above ASCII whole, the
  empty value empty, an override replacing its namesake by each system's own rule, the parent
  never written, sanitizers clean on this Mac and Linux arm64. It removes panel 241's `getenv`
  question, the UCRT's empty-value question and the process-wide write together. The door count
  stays the same once `hero_env_put`'s five callers move. Its one premise, that a staged `PATH`
  does not find the child itself, is stated in § 3.4 and meets no caller in the tree.
- **Panel 241's recorded route, `hero_env_put` into `hero_os.h` over `_wputenv_s`: object.** Not
  a veto, since nothing breaks the ABI. But plain `_wputenv_s` turns `hero_env_put("")` into a
  removal on Windows (§ 3.1), a meaning a `1.0.0` promise would then fix differently on two
  systems. The 7-line arm that keeps POSIX's meaning (§ 3.2) repairs the child and the block, and
  still cannot make `getenv` read `""`. If the sitting takes this route, it should take that arm,
  not plain `_wputenv_s`.
- **The write kept in `hero_compiler.h` as today: object, as the conservative route.** It is no
  defect (panel 241's R7), and it is what a package cannot reach. On Windows a program's own
  `getenv` does not see what it wrote: *"put [set]: getenv absent"*, measured on the box.

### Question 1 and question 4, from this seat only

- **Question 1 (ii)**: the compiler verifies a placed tree with the runtime's C, `parts/sha256.c`
  (`hero_file_sha256`, `hero_text_sha256`), held to NIST's three vectors at
  `module/fingerprint.hero:70-72` and to `cli/sha256.hero` at `:74-76`. **The verifier-among-the-
  verified question does not arise while those doors stay**: a `crypto/sha256` package could
  replace `cli/sha256.hero`'s 292 lines, and the bytes that check every placed tree, that
  package's own included, would still be the runtime's, which is no package. **My condition on
  route (A) or (C) for `crypto/sha256`: the two C doors and the agreement test stay.** Deleting
  them would make the package check itself through the compiler that carries it, anchored only in
  the seed. Unrun: no route (A) was built by this seat.
- **Question 4**: nothing at the boundary orders it. `os` (b) is the one package measured here on
  three platforms. `path/filepath` and `crypto/sha256` bind no C (panel 241's R5). The order is the
  compiler-engineer's.

## 5. Predictions, each with where it is checked

1. **A `net/http` package that binds `curl/curl.h` with no header of its own is refused
   `ffi_macro_name` on the CI's two Ubuntu 26.04 legs and builds on the Darwin leg (SDK 8.7.1).**
   Checkable at the first CI run of any such package, M-core-packages or later. Reproduced here
   against 8.23.0's headers (§ 1.7). The falsifier: that package green on `ubuntu-26.04`.
2. **libcurl's rung of §4.19's ladder (*"fetch a URL"*, design.md:3080-3083) cannot be climbed by
   a Heroes program without C of its own: no spelling of `CURLOPT_WRITEFUNCTION`'s data both builds
   and reads the bytes.** Checkable now (§ 1.3) and at any milestone that gives a function type
   `counted_by`, which would falsify it.
3. **Route (e), landed, moves all five `hero_env_put` callers onto `hero_run_env`, retires
   `hero_env_put` from `hero_compiler.h`, and turns no case red in the net's `fetch` suite or
   `case_setting`'s cases on the four CI legs.** Checkable at the step that lands it (M-core-packages,
   feature 2315's transport).
4. **Every `heroes` the CI builds under route (c) exits 127 for every verb on a Linux machine
   without `libcurl.so.4`, as the seed's line with `-lcurl` did in `ubuntu:24.04` (§ 1.8, measured
   today).** Checkable at the first release built that way, M-standalone-compiler's row 83 being
   where *"needing nothing else installed"* is judged. The falsifier: a route (c) that links
   libcurl so that `check` starts without it.

## 6. What would change my verdicts

- **(c) from object to approve**: all four of these. A ruling that a package may carry a binding
  header with `static inline` bodies under design.md:572. A libcurl that builds on the Windows leg.
  `heroes` reaching libcurl only for `fetch`, so the other verbs start without it (no form for that
  exists today; *unrun* whether one could). And a measured defect of the curl program route that
  libcurl repairs. **The veto on (c) without its own header** lifts only if the language gains a
  sound way to read `(ptr, n)` bytes in a callback and a checked width past `...`.
- **(e) from approve to object**: a caller that needs the variable in its own process, its own
  `getenv` after the write. None was found (§ 3.4). Or a red `fetch` or `case_setting` case on a CI
  leg under it.
- **Panel 241's route from object to approve**: the sitting choosing the 7-line arm and writing
  `""`'s Windows meaning into `hero_os.h`'s promise in so many words.

## 7. Found beside the work, for the coordinator to file (no GitHub write by this seat)

- **`shadowed_header` refuses Homebrew's keg-only libcurl** (§ 1.2): a correct program bound
  through `pkgconfig "libcurl"` with `PKG_CONFIG_PATH=/opt/homebrew/opt/curl/lib/pkgconfig`. That is
  ten refusals of sibling headers `curl.h` includes in quotes, each told as included in angle
  brackets, against design.md:3152-3153. Reproducer: `h1/` with that variable set, `h1/build823.log`.
  By § Bounded discovery's list, `blocking` (*a correct program refused*, *a false message*). The
  class is the coordinator's to set.
- **The brief's *"libcurl 8.18 makes `curl_easy_setopt` a macro holding a `_Pragma`"*** is,
  measured, *"libcurl adds `__clang__` to the type checker's include guard"* (§ 1.7). The comment
  at `examples/curl/curl_bound.h:13-14` (*"libcurl 8.7.1 on macOS defines the macro with no pragma"*)
  holds only because 8.7.1 never includes the type checker under clang. Wording only.

---

Closed at about 12:56, 2026-11-10. Nothing was committed, pushed or written to GitHub. Nothing was written outside the repository on this Mac, and nothing outside `C:/w/p242-ffi-pragmatist/` on the box. My worktree reads clean (`git status --short` empty).
