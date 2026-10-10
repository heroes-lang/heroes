# Panel 208, ffi-pragmatist's report

**Summary.** verdict: (P) approve, (R) (S) (M) (F) (N) object, no veto.
section: design.md §1.11 and §4.19. Finished 15:57 by `date`.

Seat: ffi-pragmatist. Tree `391628b6`, my copy under
`.claude/worktrees/scratch-b15/208-ffi-pragmatist/tree/`. Started 15:09 by
`date`. Written as I go; a section marked *pending* is not yet measured. Every
script and output named below is under that directory's `c/`.

## 1. What a real program meets (counts)

**The instrument** (`c/sdk_deprecated.py`, run with `python3 -I`): for each
top-level header of an include directory, clang's AST dump gives every
top-level `FunctionDecl`; a unit then includes the header and takes the
address of each, and the names clang calls `is deprecated` are counted. It
counts what clang actually says on a reference, not the word `deprecated` in
a line. Its blind spots: headers that do not compile alone (listed in each
output) and a function hidden behind a function-like macro (section 3 shows
that is exactly `sprintf`'s case on the Mac, so the instrument names
`sprintf` while a program's call of it is silent).

| where | headers walked | deprecated functions clang names | command (output file) |
|---|---|---|---|
| this Mac's SDK `usr/include/*.h` (macOS 26.6.2, Apple clang 21.0.0) | 230 (17 do not compile alone, `ucontext.h` among them) | **748** distinct | `sdk_deprecated.py $(xcrun --show-sdk-path)/usr/include` (`mac_deprecated.txt`) |
| the same, reached from libc's core headers (`stdio`, `stdlib`, `unistd`, `string`, `semaphore`, `dirent`, `netdb`, `time`, `signal`, `pthread`, `math`, `wchar`) | | **24**: `arc4random_addrandom brk daemon gets iruserok iruserok_sa mktemp pthread_setugid_np rcmd rcmd_af readdir_r rresvport rresvport_af ruserok sbrk sem_destroy sem_getvalue sem_init sprintf syscall tempnam tmpnam vfork vsprintf` | grep of `mac_deprecated.txt` |
| the SDK's `sqlite3.h` (Apple marks it) | | **13** (`sqlite3_trace`, `sqlite3_profile`, `sqlite3_expired`, ...) | same file |
| Linux arm64, Docker `heroes-linux-arm64:latest` (Debian clang 22.1.8, glibc 2.41) `/usr/include/*.h` | 169 (22 do not compile alone) | **58** distinct, libc's among them `getwd readdir_r sigblock siggetmask siginterrupt sigsetmask sigstack mallinfo pthread_attr_getstackaddr pthread_attr_setstackaddr`; Debian's `sqlite3.h` **0** | `linux_deprecated.txt` |
| OpenSSL 3.5.7 headers in that image (`/usr/include/openssl`) | 140 | **1,037** (`SHA256_Init`, `SHA256_Update`, `SHA256_Final`, `SHA1_Init`, `MD5_Init`, `RSA_new`, `RSA_free`, `RSA_generate_key_ex`, `AES_encrypt`, `DH_new`, `EC_KEY_new`, `HMAC_Init_ex` all among them) | `linux_libs.txt` |
| libcurl 8.14.1 in that image | 12 (6 alone fail) | **5** (`curl_formadd`, `curl_formfree`, `curl_multi_socket`, ...) | `linux_libs.txt` |
| OpenSSL 4.0.3 on this Mac (Homebrew, `pkg-config openssl`) | 144 | **963** | `mac_openssl.txt` |
| raylib 6.0 on this Mac | 4 | **0** | `mac_raylib.txt` |
| SDL3 3.4.18 on this Mac | 86 (9 alone fail) | **0 of its own** (the 5 named are `stdio.h`'s, reached through it) | `mac_sdl3.txt` |

Names the brief asked about that the headers do NOT mark on macOS 26.6.2:
`gethostbyname` and `system` (no warning on a call, `c/shapes/ux.c`).
`getcontext` is behind `#error ... require _XOPEN_SOURCE` and, with it
defined, warns *first deprecated in macOS 10.6 - No longer supported*: the
one measured name with **no replacement on the platform that deprecates it**.

**The examples.** 21 files under `examples/` declare an `extern` (the brief's
command, rerun: 21). Their groups bind **69** functions, 41 distinct names
(an awk over each group's `function` lines). Crossed with the Mac's 1,703
names (SDK plus OpenSSL 4) and Linux's 1,098 (glibc plus OpenSSL 3 plus
curl): **zero of the 21 binds a deprecated name on either platform.** So
(R), (S), (M) and (F) leave all 21 exactly as they are today; none of them
moves a shipped example. That is a measurement of today's examples, not of
programs people will write: the next one to bind OpenSSL's digest the way
every tutorial before 2021 did (`SHA256_Init`) meets it.

## 2. The binding C per shape, compiled

`c/shapes/shapes.h` holds one declaration per shape and its replacement;
`use_<shape>.c` is the emitted unit's shape (the header, then the use under
`#line 5 "prog.hero"`); `run_shapes.sh` compiles each with the compiler's own
words (`-std=gnu11 -Wall -fsigned-char`, `selfhost/cli/flags.hero:124-141`),
once `-fsyntax-only` and once `-c`. Mac: Apple clang 21.0.0. Linux: the Docker
image's clang 22.1.8 (`c/linux_shapes.txt`).

| shape | Mac | Linux arm64 | flag group | today's verdict | replacement binds? |
|---|---|---|---|---|---|
| deprecated function called | warns at `prog.hero:5` | warns | `-Wdeprecated-declarations` | exit 0, clang's text | yes (`twice2`) |
| deprecated record used by the program | warns | warns | `-Wdeprecated-declarations` | exit 0 | yes (`pair2`) |
| deprecated enumerator read | warns | warns | `-Wdeprecated-declarations` | exit 0 | yes (`LIMIT`) |
| `__attribute__((warning("careful")))` | **only with `-c`**, silent under `-fsyntax-only` | same | **`-Wattribute-warning`**, a codegen diagnostic | exit 0 | yes |
| `unavailable` | **error** | **error** | none, an error | **exit 1 already** | yes |
| `availability(macos, deprecated=10.15)` | warns, *first deprecated in macOS 10.15* | **silent** | `-Wdeprecated-declarations` | exit 0 | yes |
| `availability(macos, deprecated=99.0)` | silent (deployment target below it) | silent | | exit 0 | |
| `#pragma clang deprecated(OLD_MACRO)` | warns at the use | warns | **`-Wdeprecated-pragma`** | exit 0 | yes (`NEW_MACRO`) |
| C23 `[[deprecated]]` under `-std=gnu11` | accepted, warns | accepted, warns | `-Wdeprecated-declarations` | exit 0 | yes |
| glibc's link warning (`mktemp`, `tmpnam`) | none | **the linker**, `ld: warning: the use of 'mktemp' is dangerous` at link time | not clang at all | exit 0 | yes (`mkstemp`) |
| `sprintf` called (Mac, `_FORTIFY_SOURCE=2` default) | **silent** (section 3) | silent (glibc does not mark it) | | exit 0 | yes (`snprintf`) |
| `mktemp` called (Mac) | warns at the `.hero` line | silent in clang, warned by `ld` | `-Wdeprecated-declarations` | exit 0 | yes (`mkstemp`) |
| `getcontext` (Mac, `_XOPEN_SOURCE`) | warns, *No longer supported* | silent | `-Wdeprecated-declarations` | exit 0 | **no replacement on macOS** |

What the table says for the routes, from the C side:

- Six shapes share one group, `-Wdeprecated-declarations`; two shapes the
  brief lists are **other groups** (`-Wattribute-warning`,
  `-Wdeprecated-pragma`) and one is **not a compiler diagnostic at all**
  (glibc's `.gnu.warning` section, printed by `ld`). A route written as
  *the deprecated-declarations warning* covers six of nine.
- `warning(...)` is invisible to every `-fsyntax-only` probe: a rule
  enforced at the probe misses it, one enforced at the unit's real compile
  sees it.
- `unavailable` is already a refusal by clang: not this sitting's question.
- **The platforms disagree on 4 of the 13 rows** (`availability`, `sprintf`'s
  macro, `getcontext`, the link warning), and on whole libraries: the SDK's
  `sqlite3.h` marks 13 functions and Debian's marks 0. So under (R) one
  `.hero` program refused on the Mac builds on Linux, and the reverse for a
  link warning if (R) reached `ld`.
- Every row but `getcontext` has a non-deprecated replacement that binds in
  the same group. **(R) leaves no measured job without a route except
  user-context switching on macOS**, which the platform itself withdrew.

## 3. Why `sprintf`'s call is silent, from the C side

**Not clang's system-header rule, and not the guard's region: a macro.**
Measured with my build of `391628b6` (`tree/heroes`, built 15:23) on the
copies of the reproducers in `repro/`:

1. `heroes build dep_call.hero` prints *'twice' is deprecated: use twice2* at
   `dep_call.hero:5:10`, exit 0, the binary prints 42; `heroes build
   call.hero` (sprintf) exit 0, silent, prints 2. Both reproduced.
2. The emitted unit, `repro/build/tu-439495cd5f32fca4/call.c`: line 27
   `#pragma clang diagnostic ignored "-Wdeprecated-declarations"` (571's
   groups' close), line 85 the probe `(void)(sprintf)(a0, a1, a2);`, line 103
   `#pragma clang diagnostic warning "-Wdeprecated-declarations"` (spoken again
   over the program's definitions), line 189 the program's own call
   `t16 = sprintf(t12, hero_cstr_nonnull(t14), t15);` under
   `#line 9 "call.hero"`. So the call IS in the region where the warning is
   on.
3. `clang -E` of that unit turns line 189 into
   `t16 = __builtin___sprintf_chk (t12, 0, __builtin_object_size (t12, 2 > 1 ? 1 : 0), hero_cstr_nonnull(t14), t15);`.
   This Mac defines `_FORTIFY_SOURCE 2` by default (`clang -dM -E`), and the
   SDK's `secure/_stdio.h:107` makes `sprintf` a function-like macro,
   `#define sprintf(str, ...) __sprintf_chk_func (str, 0, __VA_ARGS__)`. The
   call never names the deprecated declaration, so clang has nothing to say.
4. The same unit compiled with `-D_FORTIFY_SOURCE=0`: *call.hero:9:11:
   warning: 'sprintf' is deprecated*. And a system header's deprecated
   function that is NOT a macro warns at the program's line with the same
   compiler: `mktemp` from `stdlib.h` (`repro/shapes/mktemp.hero`), *mktemp.hero:8:19:
   warning: 'mktemp' is deprecated*, exit 0. So system-header-ness is not the
   cause.
5. Why the trunk's compiler at `9743597b` warned with no call: its probe
   writes `(sprintf)` in parentheses, which suppresses a function-like macro,
   so the probe names the declaration; 571 put the probe under the ignore.
   (Read from the emitted line 85; I did not build `9743597b`.)

So the asymmetry is `twice` being a function and `sprintf` being a macro on
this SDK. `vsprintf` has the same macro, `secure/_stdio.h:114`
`#define vsprintf(str, ...) __vsprintf_chk_func (str, 0, __VA_ARGS__)` (grep;
a program calling it through Heroes is unrun). On Linux glibc does not
mark `sprintf` at all (the image's 58).

## 3b. Found beside, at depth one (each measured on both platforms)

- **`#pragma clang deprecated` on a macro the program binds as a `constant`
  prints clang's raw text at the COMPILER's own lines**, exit 0:
  `build/tu-.../macro.c:49:29`, `:50:37` (the static asserts) and
  `macro.c:73:12` (the accessor), group `-Wdeprecated-pragma`, which 571's
  guard does not name (`repro/shapes/macro.hero`; Mac and Linux alike,
  `c/linux_heroes.txt`). 571's class exactly, one flag group over. Unfiled by
  `grep -rl -E 'deprecated-pragma|pragma clang deprecated' issues/`.
- **A binding of a function the header marks `unavailable` exits 2** with
  *internal error: compiling the generated C failed* and clang's text at the
  unit's lines, even when never called (`repro/shapes/unav_bound.hero`; both
  platforms). The siblings 152, 156, 158 and 164 were this shape and are
  closed; this one is unfiled by `grep -rn 'is unavailable' issues/`.
- **glibc's link warning reaches the author raw**, at the emitted C's path:
  *`/tmp/.../mktemp.c:89:(.text+0x1c): warning: the use of 'mktemp' is
  dangerous, better use 'mkstemp' or 'mkdtemp'`*, exit 0, Linux only
  (`c/linux_heroes.txt`). Not a clang diagnostic; `ld`'s.
- **A deprecated enumerator, and any deprecated constant, is already silent**
  on both platforms: the program reads the accessor `h_enum_OLD_LIMIT()`,
  whose body is under the ignore (`enum.c:69-75`), so the program's line
  never names `OLD_LIMIT`.
- **A program's own header cannot silence its own call**: a first group
  `quiet.h` holding `#pragma clang diagnostic ignored "-Wdeprecated-declarations"`
  leaves *quiet_call.hero:7:10: warning: 'twice' is deprecated*, because the
  unit speaks the warning again after the groups (`repro/shapes/quiet_call.hero`).
- **But a library's own switch does**, through spec § 13's existing
  *one defining `_GNU_SOURCE` first*: a first-group header defining
  `OPENSSL_SUPPRESS_DEPRECATED 1` makes `SHA256_Init` (OpenSSL 4.0.3) build
  silent and run (`sha_quiet.hero`, prints 1; without it, `sha.hero:9:11:
  warning: 'SHA256_Init' is deprecated`); one defining `_POSIX_C_SOURCE
  200809L` makes `mktemp` silent on the Mac, because the SDK guards its
  `__deprecated_msg` with `#if !defined(_POSIX_C_SOURCE)` (`_stdlib.h:235`,
  `_stdio.h:277`; `mktemp_posix.hero`, exit 0, no text). The SDK's `sqlite3.h`
  uses Apple's `API_DEPRECATED` (`os/availability.h`), which has no such
  switch that I found (a question: searched `sqlite3.h:94-106` only).

## 4. The routes' C, compiled, and the verdicts

**Each route's emitted C**, made from the lane compiler's own units by one
edit and compiled with the build's words (`c/routes/routes.sh`,
`routes2.sh`; Mac, Apple clang 21). **The same edits on Linux-emitted units
are unrun**: the container started 15:33 had not finished building the
compiler by 15:56 (the machine shared with two other seats' builds) and I
stopped it (`docker stop p208-ffi`, removed); the Linux shape programs
themselves were built with the lane compiler there at 15:26 to 15:30
(`c/linux_heroes.txt`), which is what section 3b's Linux claims rest on.

| unit | (R) at the use: 571's `warning` made `error` | (S) the `warning` line removed | (P) at the binding: the probe lines under `error` for `-Wdeprecated-declarations` and `-Wdeprecated-pragma` |
|---|---|---|---|
| `twice` called | exit 1, `dep_call.hero:5:10` | exit 0, silent | exit 1, at the probe |
| `sprintf` called (Mac) | **exit 0, silent** (the macro) | exit 0 | **exit 1**, `call.hero:8:116`, the parenthesised probe |
| `OLD_LIMIT` read | **exit 0, silent** (the accessor) | exit 0 | **exit 1** |
| `mktemp` called (Mac) | exit 1, `mktemp.hero:8:19` | exit 0 | exit 1 |
| `OLD_MACRO` read | **exit 0, raw `-Wdeprecated-pragma` at the compiler's lines** | same raw text unless the group is ignored too (S2: silent) | **exit 1** (the accessor's `macro.c:73` still needs S2's ignore) |
| `warned(...)` (`warning` attribute) | exit 0, raw at `warn.hero:5:10` | exit 0, raw | **exit 0: no probe can see it**, the probe is an unused `static` never code-generated and the static assert unevaluated (`warn.c:49`, `:69`, compiled `-c`) |

What the table says:

- **(R) as the brief words it, *a call, constant read or record use is exit 1*,
  is not what flipping clang's verdict at the use delivers**: 3 of the 6 units
  stay at exit 0 (a constant, a macro constant, `sprintf` on the Mac), and the
  macro still prints raw text. Delivering (R)'s own wording needs a check at
  the binding, which is (P).
- **(P), refusing at the `extern` member, is uniform**: one place per name,
  the place every other FFI verdict of §4.19 already lands, and blind to how
  the C spells the call (macro, accessor, parenthesis). Its C is a push, two
  `error` lines and a pop around the probe lines, plus (S2)'s ignore of both
  groups over the program's and the compiler's lines; clang accepted it.
- **(S)'s C is one deleted line plus one added ignore** (S2; 571 missed
  `-Wdeprecated-pragma`). Nothing breaks at the boundary.
- **(F)'s C is the same toggle**, chosen by a flag rather than the program.
- **None touches the ABI**: no layout, ownership, `str`, NUL or refcount
  changes in any route; every edit is a diagnostic pragma.

### Verdicts

- **(R) refuse at the use: object.** design.md §4.19 (the declaration checked
  against the header). Its stated reach is unmeasurable by the mechanism that
  implements it: measured silent on a constant, a macro constant and `sprintf`;
  a refusal that fires on `twice` and not on `sprintf` is a verdict decided by
  the SDK's macro layer, not by the program. Not a veto: no library is left
  unbindable.
- **(P) refuse at the binding, a route nobody listed: approve.** What had to
  be true for it to exist: that the compiler already names every bound C name
  once, in a form a macro cannot hide, before the program's code. It does (the
  probe, `(sprintf)(...)`, and the static asserts). Every measured shape but
  `warning(...)` has a non-deprecated replacement that binds, and the two
  libraries with the most deprecations carry their own switch, which spec § 13's
  existing first-group header reaches (measured: OpenSSL 4's
  `OPENSSL_SUPPRESS_DEPRECATED`, Apple's `_POSIX_C_SOURCE`). The one name with
  no route is `getcontext` on macOS, withdrawn by the platform.
- **(S) silence: object.** §1.11 asks that C's libraries be usable, and their
  headers' advice is part of the library: `SHA256_Init`, `mktemp`, `sprintf`
  are the plausible mistakes a model writes from old tutorials, and the header
  carries the fix in its own words. Silence throws away a `certain` diagnosis
  clang already made. The conservative choice if (P) is refused; it needs
  `-Wdeprecated-pragma` too, or it repeats 571's gap.
- **(M) a word on `extern`: object.** The way out exists in C for both
  heavy deprecators measured, through a form the spec already teaches; a new
  word buys only the switchless cases (Apple's `API_DEPRECATED`, `getcontext`),
  and none of the 21 examples or the closure list was found needing one.
- **(F) a flag: object.** A verdict that is a property of the invocation and
  not of the program text breaks what makes an FFI signature error a compile
  error, and `heroes run` and the CI would disagree on one file.
- **(N) a note at exit 0: object.** design.md Part 8, line 3800 of `391628b6`
  (the brief's `:3744` is another tree's numbering): *this language has no
  warning level*.

**Veto: none.** No route breaks the C ABI, and none leaves a measured library
unbindable.

### Recommendation

**(P)**: a name an `extern` group binds which its header marks deprecated,
function, record, constant or macro (`-Wdeprecated-declarations` and
`-Wdeprecated-pragma` at the probe), is exit 1 on the member's line, a new
class carrying the header's own message as its note and, where it is a
library switch I could name (`OPENSSL_SUPPRESS_DEPRECATED`, `_POSIX_C_SOURCE`),
nothing more than the message (a switch is a `guess`, never `certain`). The
program's own lines and the compiler's then stay silent on both groups (S2),
so no raw clang text survives. **What would make it wrong**: a needed
program (closure list or `examples/`) that binds a deprecated name whose
header has no switch and whose replacement does not bind; or a CI leg where
the probe misses a deprecation the use-site sees.

**Left open by every route, a separate question:** GCC's `warning(...)`
attribute (`-Wattribute-warning`) is visible only at the program's call
compiled with codegen, never at a probe. glibc's fortify uses it for
provable buffer overruns (a question rather than a measurement here:
`_FORTIFY_SOURCE` overruns were not built), so it is not advice, and silencing
it with deprecation would hide a real bug. It is the use-site's, and it wants
its own ruling.

### Prediction

**At the landing, on the CI's macOS leg: a program binding `sprintf` from
`stdio.h` and calling it is refused exit 1 at its `extern` member under (P),
and built at exit 0 with no text under (R) implemented as clang's verdict at
the use**; on the Linux legs both routes build it at exit 0 with no text,
glibc not marking `sprintf` (the image's 58). Scored by building
`r584/call.hero` on each leg.
