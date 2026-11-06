# Panel 234, the ffi-pragmatist's report (main brief: F, G, H, I, J, B and C)

Written as it goes, 2026-11-05. Every command was run in this session. The
compiler: built in `.claude/worktrees/234-ffi-pragmatist/` (detached at
c935ff008) with `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`;
`./heroes --version` prints `heroes 0.6.0` / `sources 1a1e340778db789a`. The
Linux compiler: built inside `heroes-linux-arm64:latest` from the same seed,
under another name, `heroes-linux`, the worktree mounted at `/w`
(`docker run --rm -v <worktree>:/w -w /w heroes-linux-arm64:latest ...`); it
prints the same two lines. The machines: this Mac (macOS 27.0.1, Darwin arm64,
Apple clang 21.0.0) and the container (Linux 7.0.14-linuxkit aarch64, Debian
clang 22.1.8). Every file this report names under `S/` is in
`.claude/worktrees/234-ffi-pragmatist-scratch/`, the seat's scratch; every
temporary file of clang and zig was kept there too (`TMPDIR`,
`ZIG_GLOBAL_CACHE_DIR`). **Windows x86-64 and Linux x86-64 are unrun in this
sitting**: where a claim is about them it says so. One partial instrument for
Windows exists on this Mac and is used where named: `zig cc -target
x86_64-windows-gnu` (zig 0.15.2), which type-checks a Windows arm against
**mingw-w64's** headers, not the Windows SDK the CI's leg compiles against, and
runs nothing on Windows.

## F. The platform layer: P1, P2 and P3, compiled

### F.0 The carried facts, re-run on both machines

`S/F/facts/facts.c` (59 lines) prints the width and sign of each platform
typedef and the layout of each struct; compiled `clang -std=gnu11 -Wall
-Werror` and run on each machine.

| fact | Darwin arm64 | Linux arm64 (glibc) | carried claim |
|---|---|---|---|
| `clockid_t` | 4 bytes, **unsigned** | 4 bytes, **signed** | holds (`u32` / `i32`) |
| `CLOCK_MONOTONIC` | 6 | 1 | new |
| `pthread_t` | 8 bytes, **pointer** | 8 bytes, **integer** | holds |
| `sockaddr_in` | `sin_len` u8 @0, `sin_family` u8 @1 | no `sin_len`, `sin_family` u16 @0 | holds |
| `sin_port`, `sin_addr`, `sin_zero` | @2 u16, @4, @8 8 bytes | the same offsets and widths | new: the shared part |
| `sa_family_t` | 1 byte | 2 bytes | holds |
| `struct addrinfo` | `ai_canonname` @24, `ai_addr` @32 | `ai_addr` @24, `ai_canonname` @32 | **new: the order differs** |
| `socklen_t` | 4, unsigned | 4, unsigned | the same |
| `timespec` | `tv_sec` 8 @0, `tv_nsec` 8 @8 | the same | the same on both (Windows unrun) |
| `time_t`, `long`, `size_t`, `ssize_t`, `off_t` | 8 each | 8 each | the same on both |
| `TIME_MONOTONIC` | not defined | not defined | F16 holds |
| `MSG_NOSIGNAL` | **defined** (0x80000) | defined (0x4000) | new |
| `SO_NOSIGPIPE` | defined | not defined | new |

The same facts asked through a Heroes binding, the way a package author
would first write it (`S/F/raw/`, six files of 11 to 16 lines, each built
with the seat's compiler on each machine; the whole output of every build
kept beside it as `<name>.mac.out` and `<name>.linux.out`):

| binding | Mac | Linux arm64 |
|---|---|---|
| `sa_darwin.hero`: `sockaddr_in` with `sin_len: u8`, `sin_family: u8`, `sin_zero: i8[8]` | exit 0, prints `2` | exit 1: `ffi_unknown_field` (`sin_len`), `ffi_field_type` (`sin_family` is `u16`), `ffi_field_type` (`sin_zero` is `u8[8]`) |
| `sa_glibc.hero`: `sin_family: u16`, no `sin_len`, `sin_zero: u8[8]` | exit 1: `ffi_incomplete_record` (does not name `sin_len`) | exit 0, prints `2` |
| `sa_partial.hero`: `partial`, `sin_port: u16` and `sin_addr` only | exit 0, `8080` | exit 0, `8080` |
| `clock_u32.hero`: `clock_gettime(clk: u32, @ts: Timespec)` | exit 0 | exit 1: `ffi_parameter_type`, *a different sign from the header's `int`* |
| `clock_i32.hero`: `clk: i32` | exit 1: `ffi_parameter_type`, *a different sign from the header's `clockid_t`* | exit 0 |
| `clock_u16.hero`: `clk: u16`, narrower than both | **exit 0, runs** | **exit 0, runs** |

So, run, not inferred: **the clock has one raw spelling on the two POSIX
machines** (`clk: u16`, Part 7 item 10's corrected reading, a narrower
parameter C converts exactly; it cannot carry glibc's negative dynamic clock
ids, which a monotonic clock never needs), and `Timespec` with two `i64`
fields builds on both; **the socket address has none**: the one record both
machines accept (`partial`, port and address) cannot name `sin_family`, so
with `rest: zero` it hands `bind` a family of 0. A socket needs C of the
project's own on Darwin against Linux already, before Windows; the clock needs
it only for Windows, where `clock_gettime` is not a UCRT function and
`tv_nsec` is a 4-byte `long` (both unrun; the second is checked against
mingw-w64's headers in F.4).

### F.1 The two capabilities, built under P2, P3 and P1

**The C**, the same in the three routes but for where it stands (every file
compiled under the nineteen words of `selfhost/cli/flags.hero`'s `flags()`,
which `level_words` gives the runtime and every unit; P2's by hand with
`-Werror` added):

| route | files | lines | conditional lines (guards included) | platform-macro lines |
|---|---|---|---|---|
| P2, a layer beside the runtime | `S/F/p2/platform/hero_time.h` 13, `hero_time.c` 47, `hero_net.h` 24, `hero_net.c` 251 | 335 | 2, 6, 2, 13 = **23**; **the two headers hold their guard and nothing else** | 7 |
| P3, `static inline` headers in the package | `S/F/p3/core/time/hero_time.h` 51, `core/net/hero_net.h` 266 | 317 | 8, 15 = **23** | 7 |
| P1, runtime parts behind `hero_os.h` | `S/F/p1/runtime/parts/clock.c` 47, `parts/net.c` 251; `hero_os.h` +13 lines, no conditional; `runtime.c` +2 includes | 311 | 6, 13 = **19** | 7 |

The Windows arms are 15 lines of the clock and 119 of the sockets, against 24
and 123 for the POSIX arms beside them; the Darwin-against-glibc arms inside
the POSIX one are three: the clock id (`CLOCK_UPTIME_RAW` against
`CLOCK_MONOTONIC`, F.0's meaning, not a width), `sin_len`, and SIGPIPE
(`SO_NOSIGPIPE` once per socket against `MSG_NOSIGNAL` per `send`; this
Mac's SDK defines `MSG_NOSIGNAL`, F.0, and its `man 2 send` does not name
it, so the Darwin arm uses the option its documentation gives).

**The Heroes**, 127 lines in six files, **byte-identical in the three routes
but for one group line** (`diff -r` of the trees prints only it): a C module
named for its header (conventions §1.4) holding the group and one function
per member, `core/time/hero_time.hero` 8 and `core/net/hero_net.hero` 28; a
face, `core/time.hero` 17 (a `test` block) and `core/net.hero` 53; two
programs, `clock_main.hero` 7 and `echo_main.hero` 14. No platform typedef
reaches a `.hero` file: the groups see `int64_t`, `uint64_t`, `void *`,
`const char *` and `struct hero_net_socket *`, a socket being the layer's own
struct behind a pointer so it is a handle with `acquires hero_net_close` and
`consumes`. The group line per route: P2 `extern "hero_net.h" link
"hero_platform"`; P3 `extern "core/net/hero_net.h"`; P1 `extern "hero_os.h"`.

**What I did for P2's link, exactly** (the compiler cannot link a second
object of its own): `S/F/p2/platform/build-layer.sh mac|linux` compiles the
two C files under the nineteen words plus `-Werror -O0 -g` and archives them
as `libhero_platform.a`; each group names it with `link "hero_platform"`, and
each build is `heroes build <file> --include platform --library
platform/<mac|linux>`. The `link` word and both flags are the by-hand part:
under the route the compiler would find the header on its own include path
beside the runtime's and link the object when a group names the header, so
no package would spell either. **P1** is built in a copy of the runtime,
`S/F/p1/runtime/`, named by `HEROES_RUNTIME`, so no tracked file moved; the
programs' binaries hold the parts (`nm`: `hero_net_accept` ... `hero_time_monotonic_ns`).

**The runs** (`S/F/common/echo_client.c`, 46 lines, is a test fixture and no
binding: it connects to 127.0.0.1, retrying for five seconds, sends `hello
from panel 234`, and exits 0 only on the same line back; each server binary
ran under `timeout 20`, its output bounded by `head -c 4096`, and `pgrep -x`
found none left after):

| | Mac | Linux arm64 |
|---|---|---|
| P2 clock / echo | builds, `true true` / client exit 0, `echoed: hello from panel 234` | the same |
| P3 clock / echo | the same, **no flag and no compiler change** | the same |
| P1 clock / echo | the same | the same |
| P2's layer, the Windows arms | type-checked by `zig cc -target x86_64-windows-gnu` against mingw-w64's headers, exit 0 after one repair (below); **unrun on Windows** | |
| P3's whole echo unit (`--emit-c`, `winsock2.h` and `windows.h` inside the Heroes unit) | the same check, exit 0; **unrun on Windows** | |
| P1's runtime copy | the same check, exit 0, and mingw-w64's own `winsock2.h:15` warns *"Please include winsock2.h before windows.h"*, since `parts/spawn.c` includes `<windows.h>` earlier in the one translation unit; what the Windows SDK does with that order is **unrun** | |

The one repair the Windows check forced: `#pragma comment(lib,
"ws2_32.lib")` is MSVC's, an unknown pragma under mingw-w64 (an error under
`-Werror`), so even the Windows arm splits by toolchain (`#if
defined(_MSC_VER)`, two more conditional lines, counted above).

**The handle shape** (conventions §4.3), run on the Mac under P2: a listen
that fails (port 70000) hands back a null through a result marked `acquires`
and begins no life (`listen_failed ... platform error 22`, exit 0, no
report); a connection the program never closes is `panic: 1 C handle(s)
never given back ... 1 taken at core/net/hero_net.hero:16:12, in
hero_net.accept`, **exit 134**. The report names the floor's wrapper line,
not the program's line that took the connection (`leak_main.hero:8`): with
panel 033 R5's two layers, the line it gives is always the package's.

### F.2 The veto of panel 114 R3, run on each two-arm shape

A deliberate wrong return type (`int32_t` for the clock) and a deliberate
wrong parameter type (`uint32_t cap` for `hero_net_read`), both in the arm
this Mac does not compile (the glibc arm), in four variants
(`S/F/r3/`, each marked `DELIBERATELY WRONG` in its source):

| variant | Mac | Linux arm64, where the arm is live |
|---|---|---|
| **P2**, the C file includes its arm-free header first | layer and programs exit 0, run right | **the layer is refused**: `hero_time.c:45: conflicting types`, `hero_net.c:210: conflicting types`, *previous declaration is here* at `hero_net.h:16`; no program links |
| **P2x**, the C file does not include its header (declares its own types) | exit 0, right | everything builds; the clock prints **`monotonic seconds: 4`** against `/proc/uptime` 803 s: **silent, wrong** |
| **P3a**, the prototypes outside every arm | exit 0, right | **refused**: `ffi_header_refused: ... line 47: conflicting types for 'hero_time_monotonic_ns'`, with *previous declaration* at line 10, and the same for `hero_net_read` |
| **P3b**, each signature inside its arm (the natural single-header shape) | exit 0, right | the echo is refused (`ffi_parameter_type`, *declared wider than the header's `unsigned int`*), but **the clock builds and prints `monotonic seconds: 0`** against 803 s: **silent, wrong** |

Three things this settles, run:

1. **In every route the Mac's green says nothing about the Linux arm.** Panel
   114 R3's premise holds for P2 as much as for P3.
2. **Where one prototype outside every arm pins the signature (P2, P3a), a
   wrong type in any arm is a compile error on the machine that compiles
   it**: the arm is held to C's exact `conflicting types`, stricter than the
   boundary's own rule.
3. **Where nothing pins it (P3b, P2x), a narrower result is silent even on
   the machine that compiles the arm**: spec § 13's *a result may be wider
   than C's* admits `-> i64` over `int32_t`, so panel 114's sentence *"a wrong
   return type in the live arm is a `_Static_assert` failure"* is false for
   that direction, and R3's instrument (2), a report of what this machine did
   not verify, would not have caught it: Linux verified it and passed it.

And one difference no wrong type is needed for: **a P3 header's own C is held
to weaker checks than P1's and P2's.** The emitted unit compiles every group
header between `#pragma clang diagnostic ignored "-Wall"` and `ignored
"-Wsign-conversion"` (`S/F/p3/echo_main.emitted.c`, lines 5 to 7), right for
a system header. Run (`S/F/strict/`): the same `size_t n = want;` with `want`
an `int64_t` is refused in P2's object (`-Werror=sign-conversion`) and builds
in a P3 header, printing **`9223372036854775807`** for `conv_sign(-2)`; a
64-to-32 shortening is refused in both (`ffi_header_refused`). **Panel 114
R3's third instrument stands today**: a syntax error in a shipped header
(`S/F/syntax/broken.h`, `return x +;`) is `error[ffi_header_refused]: ...
clang refuses it at line 3: expected expression`, exit 1, where R3 measured
`exit 2, internal error`.

### F.3 What each placement costs (P3 against B2's `core/`)

Run in `S/F/p3/`, `S/F/p3bare/` and `S/F/p3shadow/`:

| spelling | from the root | the C module alone (`heroes test core/time/hero_time.hero`) | the root driver (`heroes test driver.hero`) |
|---|---|---|---|
| `extern "core/time/hero_time.h"` (used) | builds, runs | `ffi_missing_header`, whose note says *install the library's development files* | `1 test, all passed` |
| `extern "hero_time.h"` | `ffi_missing_header` without a flag; builds with `--include core/time`, which **every program using the package, and every program using a package that uses it, must pass** | `1 test, all passed` | needs `--include core/time` |
| a shipped header named `time.h`, with `--include core/time` | `ffi_header_refused: ... incomplete type 'struct timespec'`: its own `#include <time.h>` includes itself; and **a second module of the same program binding the system's `<time.h>` is handed the package's file** and refused the same way; the note says *`time.h` is not this program's to repair*, false of a file the program ships | | |

So under P3 the spelling names the package's place (`core/...` under B2,
another prefix under B1 or a B3 copy), a header may never be named like a
system one, and the refusal misdirects in both failures. Under P1 and P2 the
header stands on the compiler's own include path beside `hero_os.h`, so the
spelling is `extern "hero_net.h"` from any file and no placement enters it
(by hand here, through `--include platform`).

### F.4 What P1 costs that P2 does not, run

- **The seed widens** (item I's F9): under P1's runtime a group naming only
  `stdlib.h` binds `hero_time_monotonic_ns` and runs (`S/F/p1seed/`,
  `true`); under the trunk's runtime the same group is `ffi_unknown_name`.
  Every platform function added to `hero_os.h` becomes reachable from every
  group whatever header it names.
- **Every program carries the layer**: `hello` built against P1's runtime is
  239,592 bytes against 238,056, and holds 8 `hero_net_` symbols; on Windows
  the runtime object's `#pragma comment(lib, "ws2_32.lib")` would make every
  program link winsock (unrun). The author's fourth instruction asks for the
  opposite, *linked only where a group names it*.
- **One translation unit couples the parts' include orders**: mingw-w64's
  warning above; an SDK error is unrun.

### F.5 The carried facts, and §1.12

F.0 holds every carried fact on the two machines run, and adds three
(`addrinfo`'s order, `MSG_NOSIGNAL` on Darwin, the two meanings of
`CLOCK_MONOTONIC`). What a Windows arm adds, written and type-checked against
mingw-w64 only, **unrun on Windows**: `WSAStartup` once per process
(`InitOnceExecuteOnce`; in P3 the flag is one per unit, a `static` in a
header), `SOCKET` an unsigned 64-bit handle (`sizeof(SOCKET) == 8` asserted
against mingw-w64) with `INVALID_SOCKET` and `SOCKET_ERROR`, `closesocket`,
`recv`/`send` taking and returning `int` (so a chunk at `INT_MAX`), `accept`
taking `int *`, `SO_EXCLUSIVEADDRUSE` for `SO_REUSEADDR`, no SIGPIPE,
`WSAGetLastError` for `errno`, `QueryPerformanceCounter` for the clock
(whether it counts sleep is a question, unread and unrun), and the link of
`ws2_32` by pragma under MSVC's ABI or `-lws2_32` under mingw-w64.

**Which route can let a wrong width reach C in silence on a platform the CI
does not build first** (§1.12): **P3**, on three counts run above: an arm's
signature nothing pins (P3b's 0 s), a sign change in the header's own C
(`9223372036854775807`), and arms that no build compiles until a program
naming the package is built on that platform, which for a package outside
this repository is no CI of ours at all. **P2** only where its own rule
breaks (P2x's 4 s) or where its object is linked lazily and no leg builds a
program naming it; both are rules of the project's own C that the net can
hold. **P1** compiles every arm of every part on every leg at the runtime's
first build, with the include structural (`runtime.c:102`), so it has P2's
safety by construction and pays for it in F.4.

### F.6 Verdict on F

- **verdict**: **approve P2**, on five conditions; **the veto binds P3** as
  drawn; P1 is the conservative route, robust at the boundary, and refused
  here on F.4's measured costs against the author's instruction, not on
  soundness.
- **section**: design.md §1.11 (*FFI ergonomics rank alongside
  comprehension*; *FFI signatures are verified by clang against the real
  header*, point 3 of the resolved tension), §1.12 (*a boundary that is
  complete and defended*), §4.19; panel 114 R3, the seat's ratified veto.
- **cost**: C 335 lines (P2), 317 (P3), 311 (P1); Heroes 127 lines, the same
  in each; the R3 variants, four trees under `S/F/r3/`.
- **the conditions on P2**: (1) the layer's headers hold no conditional but
  their guard, held by a test in the net; (2) every C file of the layer
  includes its own header before anything else, or the layer is one
  translation unit that includes its headers before its parts as
  `runtime.c` does, held by a test (P2x is what breaking it costs); (3) the
  layer is compiled on every CI leg whether or not a program names it; (4)
  the compiler finds its headers on its own include path and links its
  object when a group names one of them, so no package spells `link` or a
  flag for it; (5) its headers are **not seeds**: a group binds a layer
  function only by naming the layer's header (F.4's first bullet is what a
  seed costs).
- **the veto on P3**: panel 114 R3's instrument (2) exists nowhere today
  (the critic's grep, F7, which I did not repeat), so P3 does not land. My
  runs add that (2) is not enough: P3b's wrong result was verified and
  passed on the machine that compiles it. P3 lands only with (2), **and**
  a check that every function a group binds from a header the program ships
  is declared outside every conditional (P3a's shape, refused cleanly), **and**
  a program's own headers compiled without `-Wsign-conversion` ignored, which
  needs R3's instrument (1), the shipped header told apart from the machine's.
- **prediction**: under P2 with its five conditions, **zero** `.hero` files of
  the core packages name a platform typedef or a platform macro (`grep -E
  'clockid_t|socklen_t|SOCKET|_WIN32|__APPLE__|__linux__'` over `core/` and
  the groups, 0 lines), and the first Windows leg to build the layer refuses
  any arm that disagrees with its header at the layer's compile, never at a
  program's run; checked at M-core-packages' close.
- **condition**: a measured case where P2's layer, held by (1) to (5), builds
  green on every leg and answers wrong on one would move me to P1; a compiler
  that holds a shipped header's bound signatures outside its arms and
  reports what a machine did not verify would lift the veto on P3.

## G. The byte buffer

Run in `S/G/` on both machines, every program built with the seat's
compiler; the data: `data/text.txt` 5,570 bytes of UTF-8 with multi-byte
characters, `data/binary.bin` 3,072 bytes (every byte value, then
pseudo-random ones), `data/nul.txt` 13 bytes of UTF-8 holding a NUL. Byte
counts and sums checked against Python's (`641100`, `390852`, `1167`).

| program | what it binds | Mac | Linux arm64 |
|---|---|---|---|
| `posix_read.hero` (47 lines) | `open` (`fcntl.h`), `read(fd: i32, @buf: [u8] counted_by n lent, n: u64) -> i64`, `close` | text: 5,570 bytes, `same as read_file: true`; binary: 3,072 bytes, the sum right, `not text: not_text`; the NUL file: text, `true` | the same, byte for byte |
| `stdio_read.hero` (24) | `fopen`, `fread(@buf: [u8] counted_by n lent, size: u64, n: u64, f: CFile) -> u64`, `fclose` | `5570 bytes; text: true` | the same |
| `blob.hero` (16) + `blob.h` | bytes **C owns** (`const void *` and a length, as `sqlite3_column_blob` and curl's write callback hand them out), copied by `memcpy(@dst: [u8] counted_by n lent, src: ptr, n: u64)` | `7 bytes: 104 0 255 33`, `hero`, the whole `not_text` | the same |
| `char_from_byte.hero` | a `[u8]` built in Heroes, `[104, 105, 0xc3, 0xa8].validated_bytes()` | `hiè`; `[0xff]` fails | the same |
| `result_u8.hero` | `function blob_data() -> [u8]` | `error[ffi_type]`: *`[u8]` cannot cross the FFI boundary, and it is an `extern`'s result* | the same |
| `from_bytes.hero` | `extern "heroes_runtime.h"`, `function hero_str_from_bytes(p: ptr, n: i64) -> str` | **`error[ffi_runtime_header]`**: *no program binds it* | the same |
| `send_plain.hero` | a `[u8]` handed to C **to read**, by value: `fwrite(data: [u8] counted_by n lent, ...)` | `error[lent_shape]` and `error[ffi_type]` | the same |
| `send_at.hero` | the same with `@data` | builds, writes `0 255 104 105` | the same |
| `send_part.hero` | sending the first 4 bytes of a 6-byte buffer with `@data` | writes `hero`, and the caller's buffer is **`before: 6` ... `after: 4`** | the same |
| `send_copy.hero` | the same through a copy lent in its place | `written: 4, the caller's still 6` | the same |

**What § 13 answers.** For bytes C **writes** (`read`, `recv`, `fread`) and
bytes C **owns** (copied by `memcpy` into a counted `[u8]`), spec § 13's
`@buf: [u8] counted_by N lent` and `validated_bytes()` answer question
(iii) whole: no per-byte runtime entry (route 1) and no built-in (route 3)
is needed, and a `[u8]` **result** (route 2) stays refused (`ffi_type`) and
is not needed, the out-parameter doing its work. **Windows** has no
`unistd.h` or `fcntl.h` (the golden `fixedbugs-396-posix-fills-its-buffers`
says so in its header), so the portable `io` is `fread` over `stdio.h`, run
here on two machines and unrun on Windows.

**The fourth route is no route, run.** `hero_str_from_bytes` is refused to
every program (`ffi_runtime_header`, both machines), and no `.hero` file
under `examples/`, `tests/` or `selfhost/` binds it (`grep -rn
'hero_str_from_bytes' examples --include='*.hero'` prints one line, the
comment at `examples/checksum/base64.hero:31`; a grep for `function
hero_str_from_bytes` in the three trees prints none). **F36 and this brief
say `base64.hero` calls it; it mentions it in a comment.** That comment
(`:26-36`) also says *Heroes has no way to build a character from a byte
value* and *`[u8]` in an `extern` position is `error[ffi_type]` today*: both
are false today (`char_from_byte.hero` prints `hiè`; `@buf: [u8] counted_by`
builds), and the milestone page's `io`/`bufio` row (*bulk text through
`hero_str_from_bytes(p: ptr, n: i64)`, exported by panel 035*) names a door a
program cannot open. Both are corrections the milestone owes.

**What is still owed: the send direction.** A `[u8]` handed to C to read
(`send`, `fwrite`, `sqlite3_bind_blob`, a binary POST body) has no form but
the fill form, which needs the bytes in a mutable cell and **leaves the
caller's array exactly `n` long** (`after: 4`, as § 13 says of the fill), so
sending part of a buffer cuts the caller's bytes. Today a package must lend
a copy (`send_copy.hero`, one copy per send). My platform layer's
`hero_net_write` takes `cstr lent` and a count, the golden's shape for
`write`, which aborts on a zero byte (`.cstr()`), so binary data cannot
cross it that way.

- **verdict**: **approve** § 13's fill form as the core packages' byte
  buffer, for `read`, `recv`, `fread` and bytes C owns; **object** to
  calling question (iii) closed: the read-only lend of a `[u8]` is owed, or
  the conventions page names the copy as the rule for every send.
- **section**: spec § 13 (*`@md: [u8] counted_by N lent` hands C `N`
  elements ... and leaves the array exactly those `N`*); design.md §1.11
  (*FFI ergonomics rank alongside comprehension*), §1.12.
- **cost**: ten programs, 7 to 47 lines, one C header of 7 lines; no C of the
  project's own for the fill direction.
- **prediction**: `io`, `bufio`, `net` and `database/sql` are written with
  zero runtime entries for bytes and zero calls of `hero_str_from_bytes`,
  and every send in them goes through a copy until a read-only `[u8]` lend
  exists; checked at M-core-packages' close by `grep -rn
  'hero_str_from_bytes\|@[a-z_]*: \[u8\] counted_by' core/`.
- **condition**: a read-only `[u8] counted_by n lent` (C's `const void *`,
  the array left as it was, `n` past its end refused) would make the send
  direction as direct as the fill; frame 1 applies to it, so it owes
  Principle 0's ticket, and my measured argument for it is the truncation
  above.

## H. A C name spelled as a word Heroes refuses (issue 1)

**The words, read from the compiler, not the brief.** `keyword()` holds 21
(`S/kw.txt`). `declared_word` holds **17 arms and 23 words**: three arms hold
several, `"null" | "nil" | "None"`, `"try" | "catch" | "throw" | "raise"`,
`"import" | "include"` (`selfhost/foreign_words.hero:160-166` of the
function at `:116`). The brief's and F11's *17 foreign words* are the arms.
Each of the six unlisted words is refused in function and in field position
(`S/H/words/`, `reserved_word`, twelve checks), and one is **C89's `raise`**
of `<signal.h>`: `extern "signal.h"` / `function raise(sig: i32)` is
`error[reserved_word]`. A word that is itself a C keyword (`if`, `for`,
`struct`, `int`, `extern` and the rest, fifteen) can never name a C
declaration, so the search is for the other **29**: `assert constant fail
false function in match nullptr record test true use variant` and `class def
elif fn func let var null nil None try catch throw raise import include`.

**The method** (`S/H/scan.py`, 111 lines, its docstring the method; re-run
with `python3 -I scan.py <out> <clang> [--tu <unit>] [--prelude <file>]
<root>... [-- <flags>]`): every `.h` under the roots, each real file once; a
file is a candidate for a word when the word is a token of it once its
comments, literals and each directive's own name are removed (no
declaration can be missed, since a declaration of `w` holds the token `w`);
each candidate is classified by clang, `-ast-dump-filter=<word>` on a unit
that includes that file, a declaration kept only if its own name is the word
and clang locates it in that file; kinds are counted apart. Checked on a
known positive first: `raise` at `signal.h:77`, `FunctionDecl`. Macros
named by a word are counted lexically, apart.

**The counts.** A parameter's name costs a binding nothing (Heroes names an
`extern`'s parameters itself), so the positions that matter are the function
and the field:

| header set | headers | function position | field position | a tag (`record ... tag <w>`) | macros | parameters (no cost) | units clang refused |
|---|---|---|---|---|---|---|---|
| the milestone's own: the closure of `stdio.h`, `time.h`, `sys/socket.h`, `netinet/in.h`, `sqlite3.h`, `curl/curl.h`, `openssl/evp.h`, `hmac.h`, `sha.h`, one unit (`S/H/ms/milestone.c`), **Mac** | 212 | **0** | **1**: `ossl_dispatch_st::function` (`openssl/core.h:42`, a provider's dispatch table, which neither `sha256` nor `hmac` names) | 0 | 0 | 83 | 0 |
| the same, **Linux arm64** | 154 | **0** | **1**, the same field | 0 | 0 | 83 | 0 |
| this Mac's SDK, `usr/include` (`S/H/sdk2/`) | 3,117 | **1**: `raise` | **25** in 22 structs of 15 headers: libxml2 and libxslt (`func` ×4, `match` ×2, `use` ×3, `test`, `def`, `function`), apache2 and apr (`func` ×3, `in`), editline (`function`), net-snmp (`var`, `test`), openpam (`func`), `unwind.h` (`func`), `netinet6/in6_var.h` (`use`), `sys/pipe.h` (`in`), `sys/timex.h` (`constant`) | 0 | 4: `nil` ×2, `assert` (function-like), `null` | 129 | 2,049 of 2,224: 1,991 C++ (libc++'s `__config` and the like), **58 C** |
| Homebrew, `/opt/homebrew/include` (`S/H/brew/`) | 3,913 | **0** | **44** in 42 structs of 31 headers: X11 and xcb (`class` ×5, `match` ×3, `use`, `function` ×2, `fn`), glib (`func` ×4), **SDL3 and sdl2-compat** (`SDL_AssertData::function`, `SDL_HapticEffect::constant`, a union member, `SDLTest_Md5Context::in`), **libuv** (`_uv_barrier::in`), harfbuzz (`var` ×2), nss (`null` ×8), gpgme (`class` ×2), ffmpeg and freetype (`def` ×2), ngtcp2 (`in`, a union member), openssl@4, pkgconf, sox, uvwasi, vde, leptonica | 0 | 6: `true`/`false` (capstone, SDL3), X11's `None` ×2 (`#define None 0L`) | 429 | 2,072 of 2,405: 1,566 C++, **506 other** |

The tag column's six raw hits (`EnumDecl class` ×8, `RecordDecl None`) are
C++ `enum class` and a Metal kernel header read as C, so it is zero. **The
SDK and Homebrew rows are exact for what clang accepted and a lower bound by
the refused C units** (58 and 506; the second still holds C++, 133 of its
units a `class` and 26 a `nullptr`): what those hold, by candidate word, is
in `failed-c.tsv` beside each count. Issue 1's counts (`function` 571, `func`
29, ...) carry no method and count something else; these replace them.

**Both positions already have a route, prescribed by the compiler's own
refusal, built and run on both machines:**

- **function**: `error[reserved_word]: raise is a word Heroes refuses ...
  call it in a header of the program's own that includes "signal.h",
  static inline int c_raise(int x) { return raise(x); }`. Built as written
  (`S/H/raise/`, a 5-line header): `c_raise(x: 0)` prints `0`, Mac and Linux.
- **field**: F11 measured a plain `record R`, which names no C field. **For a
  group's record the refusal prescribes a route**: `error[expected_field]:
  constant is a keyword of Heroes, so no field of a group's record is bound
  by that name — name it again in a header of the program's own that
  includes "sys/timex.h", #define kw_constant constant, and declare the field
  kw_constant`. Built as written (`S/H/field/timex_own.h`, 3 lines;
  `timex_route.hero`): the emitted assertion holds `kw_constant` to C's
  `constant` (`sizeof(((struct timex *)0)->kw_constant) == sizeof(int64_t)`,
  the preprocessor making it `constant`), and it prints `41`, Mac and Linux.
  `partial` is the other way, for a field the package does not need
  (`timex_partial.hero`, `3`, both machines): built zero, never read.

**The two forms F33 lists, priced against that.** An identifier escape
(`r#match`, `@"match"`) is a lexical form every re-printer of a program
learns (CLAUDE.md § 9's walk) and a reader meets in no other place; a `tag`
on a record field puts a contextual word in a new position. Each owes
Principle 0's ticket (frame 1) and the ergonomist's reading (frame 6), and
the need this milestone shows for either is **zero**: no function and no
field its packages need is spelled so, on either machine.

- **verdict**: **object** to either new form in this sitting; **approve**
  the two routes the compiler already prescribes as the conventions' rule
  (a header of the package's own, the C module's, holding the `static
  inline` rename or the `#define`).
- **section**: design.md §1.11 (the binding must be writable), Principle 0
  (CLAUDE.md § 2); §4.19 states no rule for the contextual words (the
  critic's check) and none is needed here.
- **cost**: the scanner, 111 lines; 24 probe programs of 4 to 9 lines; two
  headers of 5 and 3 lines.
- **prediction**: the core packages need **0** renames in either position
  (`grep -rE 'static inline .* c_[a-z_]+\(|#define kw_' core/` prints
  nothing at M-core-packages' close); the first one will be a field of SDL3,
  libuv or libxml2, none of them this milestone's.
- **condition**: a package this milestone writes needing a field spelled so
  **and** a header where the `#define` route fails (a field whose name its
  own header also defines as a macro) would earn the escape a sitting.
- **corrections to the record**: the foreign words are 23, not 17; F11's
  *in field position there is no route* is false of a group's record, run
  on both machines; issue 1's counts carry no method.

## I. A group's header: per group, per module, or per module and the seeds

**What the compiler does, run** (`S/I/`, Mac): a module whose group `extern
"a.h"` binds `fb`, which only `b.h` declares, with a second group `extern
"b.h"` and no seed involved, **builds and runs** (`two.hero`, `2 4 9`), in
either order of the two groups (`two_reversed.hero`, the same); the emitted
unit includes both headers before every `_Static_assert` (`#include <a.h>`,
`#include <b.h>`, then `HERO_RET_I32(fb(...))`). The same module without the
`b.h` group is `error[ffi_unknown_name]: a.h declares no fb` (`one.hero`). So
the check is **per module plus the seeds** (F9's third position), and the
group line `extern "a.h"` above `fb` is a false sentence the compiler
accepts, while its refusal speaks per group (*`a.h` declares no `fb`*).

**Per-group probe units, the brief's shape** (`S/I/probes.sh`, one unit per
group, its header alone, every bound function referenced):

| unit | without the seeds | with the seeds |
|---|---|---|
| `a.h` binding `fa`, `fb` | refused: `fb` undeclared | refused: `fb` undeclared |
| `stdlib.h` binding `hero_dir_scan`, `hero_dir_at`, `hero_dir_release` (the compiler's own `selfhost/cli/process.hero:66-69`) | refused: all three undeclared | compiles |
| `needs_file.h`, which declares `use_file(FILE *)` and includes no `stdio.h` (§4.19's `jpeglib.h` shape), below an `extern "stdio.h"` group | **refused, `unknown type name 'FILE'`, though the module is correct**: `heroes check` and `heroes build --emit-c` of `needs.hero` both exit 0 | refused the same way |

**How often "the header alone" would refuse a correct module, counted**
(`S/I/alone.py`, `alone_cases.py`): every group header of `selfhost/` and
`examples/` compiles alone and with the seeds (41 of 41 on the Mac; 39 of 39
on Linux, 2 unresolved where the container has no raylib or SDL3). Over the
433 group headers of `tests/golden/run/` on the Mac, 28 are refused alone and
20 with the seeds; of the 20, **19 are headers this Mac lacks** (the module's
own build fails on them here, so the case is skipped here, not a probe's
fault) and **1 is a correct case refused**, `fixedbugs-563-a-record-only-group-
is-included-where-it-is-written` (`user.h` needs `Need` from an earlier
group); without the seeds 8 more correct cases are refused, each a header
that uses the runtime's types (`HeroStr`, `HeroMapHeader`,
`HeroArrayHeader`) through the seeds.

**A per-group check clang can answer exactly, without that false refusal**
(`S/I/census.py`, 98 lines): compile the module's own unit as the compiler
does (it compiles, the order kept), ask clang where each bound function is
declared (`-ast-dump-filter`, the exact name, the `FunctionDecl`'s file),
and attribute it to the closure `clang -M` lists for the group's header
alone (preprocessing only, so a header needing another's types still has
its closure). Over `selfhost/` and `examples/`:

| | Mac | Linux arm64 |
|---|---|---|
| bound functions | 139 | 139 (8 unresolved: raylib, SDL3 absent) |
| declared in their own group header's closure | **136** | **128** |
| declared only in the seeds | **3**: `process.hero:66-69` | **3**, the same |
| declared only in another group's header | **0** | **0** |

So what each check would refuse on today's tree and on the two shapes:

| check | today's `selfhost/` and `examples/` | `fb` under `a.h` | a correct order-dependent module (`needs.hero`, `fixedbugs-563`) |
|---|---|---|---|
| per module and the seeds (today) | nothing | admitted | admitted |
| per group, the header alone, without the seeds | `process.hero:66-69` | refused | **refused (wrongly)**; 9 of the run goldens |
| per group, the header alone, with the seeds | nothing | refused | **refused (wrongly)**; 1 of the run goldens |
| **per group by declaration location**, the seeds not counted | `process.hero:66-69` (3) | refused | admitted |
| per group by declaration location, the seeds counted | nothing | refused | admitted |

- **verdict**: **approve a per-group check by declaration location, the
  seeds not counted** for a group whose header is not the seed it binds;
  **object** to the header-alone probe unit as its mechanism, which refuses
  correct programs §4.19 promises to build.
- **section**: design.md §4.19 (*a module's headers are included in the
  order its groups are written, and the order is load-bearing*), §4.17 (a
  diagnostic is true: `a.h declares no fb` is told per group while the
  acceptance is per module), conventions-draft §1.4.
- **cost**: one AST question per bound name on a unit the compiler already
  compiles, and one `-M` per group header; on today's tree it moves three
  lines of `selfhost/cli/process.hero` (`:66-69`) into the module's own
  `extern "hero_os.h"` group at `:40`. Constants (macros, read through `-E
  -dD`'s line markers) and records (a `RecordDecl`'s location) can be
  attributed the same way, **unrun here**: I measured functions only.
- **prediction**: run over the whole tree (`selfhost/`, `examples/`,
  `tests/golden/`), the location check with the seeds not counted refuses
  `process.hero:66-69` and only cases written to bind across groups; checked
  when the compiler-engineer's prototype exists, before M-core-packages'
  first package over C.
- **condition**: a real binding whose function only a group's predecessor's
  header declares, where moving it to that group is impossible, would make
  me take the seeds-and-predecessors position. And item F's route decides
  the seeds' reach: under P1 every platform function is a seed, which is
  why I ask P2's headers not to be one.

## J. The C-width vocabulary (design.md Part 7 item 10), asked of this milestone

**The bindings**, written as the packages would (`S/J/`, twelve files: five
packages' bindings, six variants of their spellings and one check, plus
`curl_j.h`, 6 lines, for `curl_easy_setopt`'s variadic
`long`), every one built on both machines; an uncalled `extern` is verified
like a called one (`uncalled.hero`: `labs(x: f64)`, never called, is
`ffi_parameter_type`), so a binding need not be called to be judged. **The
C types**, as each machine's clang reads its headers (`S/J/types.py`,
`types.mac.txt`, `types.linux.txt`), every parameter, result and field whose
written or desugared type is `long` (not `long long`), `size_t`, `ssize_t`,
`socklen_t`, `time_t`, `clockid_t`, `off_t`, `sa_family_t`, `in_port_t` or
`curl_off_t`:

| package | binding | kind | Darwin arm64 | Linux arm64 | the spelling that builds on both (run) | Windows x86-64 and Linux x86-64 (**unrun**) |
|---|---|---|---|---|---|---|
| io | `fread`/`fwrite` `size`, `nitems` | param | `size_t` = `unsigned long` | the same | `u64` | `size_t` is 64-bit unsigned on both 64-bit targets, so `u64`; unrun |
| io | `fread`/`fwrite` result | result | `unsigned long` | `size_t` | `u64` | the same; unrun |
| io | `fseek` `offset` | param | `long` | `long` | **`i64` and `i32` both build** | `long` is 4 bytes on the Windows box (measured there, `examples/ctime/main.hero:17-19`), so `i64` would be refused and only the narrower `i32` is shared, capping an offset at 2 GiB; unrun |
| io | `ftell` | result | `long` | `long` | `i64` (a result may be wider) | `i64` everywhere; unrun |
| time | `time(time_t *)`, `gmtime_r(const time_t *, ...)`, `difftime(time_t, time_t)` | param | `time_t` = `long` | `time_t` = `long` | `@t: i64`, `i64` | `time_t` is 64-bit on the CI's Windows unless `_USE_32BIT_TIME_T`; unrun; `gmtime_r` is POSIX, Windows has `gmtime_s` with its arguments reversed |
| time | `strftime` `maxsize`, result | param, result | `size_t` | `size_t` | `u64` | unrun |
| time | `struct timespec.tv_sec` | **field** | `__darwin_time_t` = `long` | `__time_t` = `long` | `i64` | unrun |
| time | **`struct timespec.tv_nsec`** | **field** | `long` | `__syscall_slong_t` = `long` | `i64` | **4 bytes on Windows (its `long`), so no spelling of the field is shared**; unrun |
| time | `struct tm.tm_gmtoff` | field | `long` | `long` | `i64`, or left to `partial` (used) | absent from Windows' `tm`; `partial` with the nine `int`s is shared; unrun |
| time | `clock_gettime` `clockid_t` | param | enum, unsigned | `int` | `u16` (F.0); **moot under P2**, which keeps it in C | not a UCRT function |
| net | `recv`/`send` `n`, result | param, result | `size_t`, `ssize_t` | the same | `u64`, `i64`; **moot under P2** | winsock's are `int`; unrun |
| net | `bind`/`setsockopt` `len`, `accept`'s `socklen_t *` | param | `socklen_t` = `unsigned int` | the same | `u32` or `u16`, never `i32` (refused on both, *a different sign*); **moot under P2** | `socklen_t` is `int` in winsock, so only `u16` would be shared; unrun |
| net/http | `curl_global_init` `flags`, `j_setopt_long`'s `long` | param | `long` | `long` | **`i64` and `i32` both build** | as `fseek`: only `i32` shared; unrun |
| crypto | `EVP_DigestUpdate` `cnt`, `HMAC` `data_len` | param | `size_t` | `size_t` | `u64` | unrun |
| crypto | `EVP_sha256` result | result | `const EVP_MD *` | the same | **only `ptr`**: `-> Md` is `ffi_return_type`, `-> Md lent` is no syntax | unrun |
| database/sql | the prepare, step, column and bind functions | all | `int`, `sqlite3_int64`, `sqlite3_uint64`, `double`, pointers | the same | **no platform width at all** | unrun |

**The falsifier, asked of this milestone.** Its words: *a program the
closure list or §4.19's ladder needs, binding a C `long`, `size_t` or
`unsigned long` **parameter**, whose correct spelling is the same on all
three CI legs* (four since 2026-09-17, F10). On the two legs run, **every
platform-width parameter of these bindings has one spelling**: `u64` for
`size_t` at its full range, and for `long` both `i64` and the narrower
`i32`. Whether the other two legs agree is unrun; on the record, `size_t`
is 64-bit on both and `long` is 4 bytes on Windows, so `u64` would be shared
by all four and for `long` only `i32` would, at half the range on the other
three. **If the two unrun legs agree, the row's falsifier is produced for
`size_t` by `io`'s `fread`** (and by libcurl, rung 4 of the ladder, whose
write callback takes `size_t` twice), and the row then stands on what its
falsifier does not test: **a field**. `struct timespec.tv_nsec` is a C
`long` and a field's layout must match, so it has no spelling shared with
Windows (unrun), and a parameter's narrower spelling does not exist for it.

**On the route F adopts (P2)** no package of this milestone needs that
field in a `.hero` file: the monotonic clock and a sleep cross the layer as
`int64_t` nanoseconds, the sockets as `int64_t` and `uint64_t`, so
`clockid_t`, `socklen_t`, `sockaddr_in`, `ssize_t` and `timespec` stay in C;
what remains in the packages' `.hero` files is `size_t` (`u64`), `time_t`
(`i64`), `long` parameters (`i32` where Windows must build, else `i64`) and
`tm`'s nine `int`s (`partial`).

- **verdict**: **object** to a C-width vocabulary (or `alias` for one) in
  this milestone: no binding the packages need lacks a spelling on the legs
  run, and the one case with none, a `long` field, P2 keeps out of every
  `.hero` file. **Approve** correcting item 10's falsifier to name what is
  left: a field.
- **section**: design.md Part 7 item 10 and its correction of 2026-10-25;
  spec § 13 (*a parameter and a field are declared at the header's own
  width and sign ... except a parameter C converts exactly*).
- **cost**: twelve binding probes and one 6-line header; `types.py`, 40
  lines.
- **prediction**: when the Windows leg first builds the core packages, every
  `size_t` binding written `u64` builds there and every `long` parameter
  written `i64` is `ffi_parameter_type`, so the packages that must build on
  Windows write `i32` for a `long` parameter; checked at the first CI run of
  M-core-packages' push.
- **condition**: a `long` (or `unsigned long`) **field** a core package needs
  in a `.hero` file on all four legs: that is the program a C-width type
  would be for, and I would then support the vocabulary's sitting.

## B and C, from the C side

### B. Where clang looks for a package's header under B2's `core/`

Run in `S/BC/` on the Mac: `core/database/sql/sqlite3.hero` (the C module,
`extern "sqlite3.h" link "sqlite3"`, a `test` block), a face
`core/database/sql.hero`, and a root `main.hero` that `use`s
`core/database/sql`.

| placement | from the root (`heroes run main.hero`) | the C module alone (`heroes test core/database/sql/sqlite3.hero`) |
|---|---|---|
| the system's `sqlite3.h` only | exit 0, `true` | `1 test, all passed` |
| a `sqlite3.h` of the program's own **at the root** (fresh tree) | **read for the package's group**: `ffi_header_refused`, its `#error` | (not run) |
| a `sqlite3.h` **beside the module** (fresh tree) | not read: exit 0, `true` | **read**: `ffi_header_refused` |

So clang looks in the runtime's directory, then **the directory of the file
being compiled**, then the system's (design.md §4.19's *`-I<directory of
the .hero source>` after the runtime's*): a package's group resolves its
header from the program's root when a program is built and from its own
directory when the package is tested alone, and **a header at a program's
root shadows the system header every package's group names**; a package
cannot protect its binding from a file the program puts there. For a header
shipped beside the module (`core/net/net.h`, P3), F.3 is the run: from the
root only the spelling that names the place (`core/net/net.h`) or a flag;
alone only the bare one.

**A defect found on the way, run, for the coordinator to file** (I open
nothing): **the per-module cache serves an object built before a shadowing
header existed.** In `S/BC/b2/`, built once, a `sqlite3.h` then created at the
root changes nothing (`heroes run main.hero`: exit 0, `true`, `run.decoy.out`),
and the same tree copied fresh with the same file is refused
(`S/BC/fresh/`, `ffi_header_refused`); the same with the file beside the
module and the module tested alone (warm: exit 0; fresh,
`S/BC/fresh-beside-alone/`: refused). **The same on Linux arm64**
(`S/BC/linux-warm/`: first build `true`; a `sqlite3.h` then at the root,
warm, exit 0 and `true`; the same files in a fresh tree, `S/BC/linux-cold/`,
`ffi_header_refused`). A unit's record is the files clang
opened (`selfhost/cli/deps.hero`), and a file that appears earlier on the
search path is not one of them, so one tree builds two programs by its
cache's state: a program a cold build refuses runs warm. Searched first:
`gh issue list --search` for `cache shadow header`, `negative dependency`,
`header added cache`, `new header search path cache`, `stale object
header`: issue 719 and defect 2147 are about a header whose **content**
changed; none names a new file on the path, so whether it is filed is a
question these words answer no. By `.claude/rules/verification.md`'s
classes it reads as `blocking` (*a wrong one accepted*); the class is the
coordinator's. The packages make it likelier: every program's units over
`core/` are cached, and a program's own `time.h` or `net.h` at its root is
an ordinary file to write.

### C. Conventions §1.4 and §4.3 against the C written here

**§1.4, the C module named for its header.** Kept in every package here
(`core/time/hero_time.hero`, `core/net/hero_net.hero`,
`core/database/sql/sqlite3.hero`), and it reads well where a module binds one
header. Three cases it does not decide, each met: (a) under **P1** every
platform package's C module binds `hero_os.h`, so the rule names them all
`hero_os` (a family of headers, P2 or P1', keeps it meaningful); (b) a module
binding a library through several headers (libcrypto's `evp.h` and
`hmac.h`) or through a header of its own (curl's variadic `curl_j.h`,
`raise_own.h`, `timex_own.h` of H) needs the rule to say which name: I
propose **the header its group names**, the program's own one where there
is one; (c) panel 033 R5's wrapper per member is 6 two-line wrappers for 6
externs in `hero_net.hero` (28 lines), and the price a reader pays is F.1's:
a leak report names the floor's line, never the program's.

**§4.3, a handle as a group's record with its marks, ended by one function
whose parameter is `@`.** Works for every pointer handle here (the socket
struct, `sqlite3 *`, `FILE *`, `CURL *`, `EVP_MD_CTX *`): a null result
begins no life, a handle never ended is exit 134 naming it, and the face's
`close(@s)` ends it. Two holes, run: **(a) an integer descriptor is not a
handle**: `socket(...) -> i32 acquires close` and `close(fd: i32 consumes)`
are both `error[unread_mark]` (`S/BC/fd_marks.hero`), so a POSIX descriptor
or a winsock `SOCKET` gets the marks only when C of the project's own wraps
it in a struct behind a pointer, which my layer does and which §4.3 should
say; **(b) a handle to const has no typed spelling**: `EVP_sha256() -> Md` is
`ffi_return_type` (*returns `const EVP_MD *`*), so it is a `ptr`, which
cannot leave its group (issue 1197's title: *no `ptr` escapes a group*): the
digest stays inside the C module's own functions, as §1.4 already asks.

- **verdict**: **approve** B2's `core/` for packages over system headers
  (one spelling from the root and alone) and **object** to P3's shipped
  headers under it (F.3's placement-bound spelling); **approve** §1.4 and
  §4.3 with the three amendments above (which name a several-header module
  takes; a descriptor is wrapped in C as a struct; a const handle stays in
  the floor). **The cache defect is owed whatever B and F decide.**
- **section**: design.md §4.19 (the header search), §1.12 (one tree, one
  program), §3.1; spec § 13 (*one with a `tag` and no fields is a handle,
  C's pointer to that type*).
- **cost**: `S/BC/`, six trees of 3 to 4 files; one probe of 10 lines.
- **prediction**: with the cache defect unrepaired, a program over `core/`
  that adds a header of its own named like a system one at its root keeps
  building warm and is refused cold; checked by the reproducer above at the
  next batch's gate.
- **condition**: a module of this milestone that needs two headers neither
  of which names it well would move me from *the header its group names* to
  *the library's name*.

## What I could not run, and why

- **Windows x86-64, nothing executed.** The box is off in this sitting. What
  stands instead is a partial instrument, named where used: `zig cc -target
  x86_64-windows-gnu` (zig 0.15.2) type-checked P2's two C files, P3's whole
  emitted echo unit and P1's runtime copy against **mingw-w64's** headers,
  all exit 0 (after the `_MSC_VER` guard); not the Windows SDK the CI's leg
  compiles against, not `lld-link`, not a run. Unrun with it: winsock's
  behaviour, `#pragma comment(lib, ...)` under MSVC's ABI, `winsock2.h`
  after `windows.h` under the SDK (P1), whether `QueryPerformanceCounter`
  counts sleep, `long` and `size_t` on that leg (J's column reads the record,
  `examples/ctime/main.hero:17-19`, for `long`).
- **Linux x86-64, nothing run** (the CI's leg after a push; no push here).
- **The SQLite shapes of item D**: the other instance of this seat's.
- **Item I's attribution of constants and records**: the census attributes
  functions only; macros (through `-E -dD`) and records (a `RecordDecl`'s
  location) are a method, unrun. The header-alone count over
  `tests/golden/run/` ran on the Mac only.
- **Item H's broad sets are a lower bound** by the C units clang refused (58
  of the SDK's, 506 of Homebrew's, the latter still holding C++); the
  milestone's own set refused none, on both machines.
- **raylib and SDL3 in the container**: `pkg-config` has neither, so 8 bound
  functions of item I's census are unclassified on Linux.
- **The compiler's own tests and the net**: not run; no tracked file of my
  worktree changed (P1 ran in a copy of the runtime named by
  `HEROES_RUNTIME`), so nothing a suite judges moved.
- **No clock**: no duration is written anywhere in this report.

## The verdicts in one place

| item | verdict | route |
|---|---|---|
| F | **approve P2** on five conditions; **the veto of panel 114 R3 binds P3**, not P2 | P2; P1 robust and refused on F.4's costs |
| G | **approve** § 13's fill form; **object** to question (iii) closed | the send direction owed; `hero_str_from_bytes` is no route |
| H | **object** to an identifier escape or a field `tag`; **approve** the two renames the compiler prescribes | both built on both machines |
| I | **approve** a per-group check by declaration location, the seeds not counted; **object** to the header-alone probe | 3 bindings move (`process.hero:66-69`) |
| J | **object** to a C-width vocabulary now; **approve** correcting item 10's falsifier to a field | P2 keeps the one `long` field out of `.hero` |
| B, C | **approve** B2 for system headers, §1.4 and §4.3 amended; **object** to P3 headers under B2 | the cache defect owed regardless |
