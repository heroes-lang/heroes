# Panel 237, the ffi-pragmatist's report

Written 2026-11-08 in worktree `.claude/worktrees/237-ffi-pragmatist/`, detached at `163912031`, as I
went. Every program lives under this worktree's `build/ffi/`. No paid run, no credential, no write
outside this worktree, no commit. The machine stood at load 23 (`uptime`) beside three lanes, so no
duration below is a measurement of cost.

## The compiler

- `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`: exit 0, `sources 1a1e340778db789a`.
- `./heroes build selfhost/main.hero -o heroes-next`: exit 0; `./heroes-next --version` prints
  `heroes 0.6.0` and `sources 0d7a6741345899d8`, the coordinator's and the critic's key.
- Linux arm64: the tree at `163912031` by `git archive 163912031 | tar -x` (tracked files only, no
  `.claude/worktrees`, checked with `test -d`), built in `heroes-linux-arm64:latest` (Debian clang
  22.1.8) the same two ways: see § Linux.

## 1. Question 1 in C, by hand: which order lets a file stand in for `<stdint.h>`

`build/ffi/q1/`: a folder `evil/` holding `stdint.h` (`#include_next <stdint.h>`, then
`#define __builtin_add_overflow(a, b, r) ((*(r) = (a) + (b)), 0)`), `stdbool.h` (`true` defined 0)
and `math.h` (`HUGE_VAL` defined 1.0); `probe.c` includes the three with angle brackets and prints the
overflow flag of `INT64_MAX + argc`, `true` and `HUGE_VAL`; `probe_quoted.c` is the same with quoted
includes. `orders.sh` compiles each under every order with `-H` and counts the `evil/` files read.
Clean output: `overflow=1 ... true=1 huge=inf`.

| order or variable | angle includes | quoted includes |
|---|---|---|
| `-I evil` (today's order for the program's folder, `--include`, a package) | 3 read, `overflow=0 true=0 huge=1` | 3 read, wrong |
| `-iquote evil` | 0 read, clean | 3 read, wrong |
| `-isystem evil` | 3 read, wrong | 3 read, wrong |
| `-idirafter evil` | 0 read, clean | 0 read, clean |
| `CPATH=evil` | 3 read, wrong | 3 read, wrong |
| `C_INCLUDE_PATH=evil` | 3 read, wrong | 3 read, wrong |
| `OBJC_INCLUDE_PATH`, `CPLUS_INCLUDE_PATH` | 0 read, clean | 0 read, clean |
| `CCC_OVERRIDE_OPTIONS=+-Ievil` | 3 read, wrong | 3 read, wrong |
| `CCC_OVERRIDE_OPTIONS=+-D__builtin_add_overflow(...)` | `overflow=0` | `overflow=0` |

Apple clang 21.0.0 on this Mac, and **the same 22 rows, byte for byte in their verdicts, on Debian
clang 22.1.8 in `heroes-linux-arm64:latest`** (`docker run --rm -v $PWD:/w ... bash ./orders.sh`).
So: `-idirafter` is the one order under which a file of a named folder never stands in for a library
header, for either include form; `-iquote` keeps it out of an angle include only; `-isystem` is no
route (the critic's finding 9, reproduced). `OBJC_INCLUDE_PATH` and `CPLUS_INCLUDE_PATH` do not reach
a C unit on either platform. Windows: unrun.

### The same files through the compiler, against a real binding (rung 2 of §4.19's ladder)

`build/ffi/c1/matrix.sh`: `main.hero` binds `extern "math.h"` `function sqrt(x: f64) -> f64` and prints
`9223372036854775807 + args().len()`, `sqrt(2.0)` and `true`. Clean, run with one argument: `panic:
integer overflow at main.hero:6:11`, exit 134; with none: `9223372036854775807`, `1.4142135623730951`,
`true`, exit 0. Each shadow file (with an include guard) placed four ways:

| shadow | beside `main.hero` | `--include` folder | `CPATH` folder | `C_INCLUDE_PATH` folder |
|---|---|---|---|---|
| `math.h`: `#define sqrt hero_fake_sqrt`, which returns 42.0 | `error[shadowed_header]`, exit 1 | `shadowed_header`, exit 1 | **build 0, prints `42.0`, exit 0** | **build 0, prints `42.0`, exit 0** |
| `stdbool.h`: `true` is 0 | `shadowed_header`, exit 1 | `shadowed_header`, exit 1 | build 0; no argument: prints **`false`**, exit 0; one argument `one`: **`panic: args()[0] is not UTF-8 text`**, exit 134 | the same |
| `stdint.h`: `__builtin_add_overflow` reports nothing | `shadowed_header`, exit 1 | `shadowed_header`, exit 1 | build 0, `panic: integer overflow`, 134 (the reserved pair gives the builtin back) | the same |

And `CCC_OVERRIDE_OPTIONS='+-D__builtin_add_overflow(a,b,r)=((*(r)=(a)+(b)),0)'`: build 0, with one
argument prints `-9223372036854775808`, exit 0 (the critic's finding 8, reproduced).

**What this adds to the critic's findings 7 and 8**: through `CPATH` the class reaches the FFI's own
promise. A `libm` binding that clang "verified against the header" was verified against a header that
is not libm's, and `sqrt(2.0)` printed `42.0` at exit 0; and a `true` of 0 does not only print
`false`, it makes the runtime's UTF-8 check refuse a plain ASCII argument with a false message
(`args()[0] is not UTF-8 text` for `one`). The first try of the `math.h` without an include guard was
`internal error: the runtime did not compile` (exit 2): the runtime's own compile reads the
environment's folders too.

**design.md §4.19 names `CPATH` as the route** (`docs/design.md:2983`: *the search path is the
machine's own `CPATH`, `LIBRARY_PATH` or `PKG_CONFIG_PATH`*, then *all three measured live*; and `:2991`:
*`CPATH` works there*, of Windows, where `LIBRARY_PATH` does not). So clearing `CPATH` for clang
removes a route the document gives a binding author, on the one platform where its library twin
already fails. Counting the folders clang prints from the environment as named keeps the route and
closes the door. `CCC_OVERRIDE_OPTIONS` is a driver debugging knob: no route in design.md names it
(`grep -c CCC_OVERRIDE docs/design.md` prints 0), so clearing it costs a binding nothing.

### What each route costs a real binding, beyond the opening's 36 headers

**Clearing `CPATH` switches a binding to another library's header, at exit 0** (`build/ffi/rlh/`).
`extern "/opt/homebrew/opt/readline/include/readline/readline.h" link "readline"` is
`error[machine_locked_path]`, whose note says *set `CPATH` to the directory holding it, or name a
`package`* (`selfhost/head_names.hero:220-222`; `emit/header_reach.hero:121` and
`emit/ffi_build.hero:252` say *`CPATH` works too*). Following it, `extern "readline/readline.h" link
"readline"` binding `constant RL_READLINE_VERSION: i64`:

```
CPATH=/opt/homebrew/opt/readline/include heroes-next build main.hero --library /opt/homebrew/opt/readline/lib
  -> build 0, prints 2051 (0x0803), otool -L: /opt/homebrew/opt/readline/lib/libreadline.8.dylib
env -u CPATH heroes-next build main.hero --library /opt/homebrew/opt/readline/lib
  -> build 0, prints 1026 (0x0402), otool -L: the same GNU libreadline.8.dylib
```

The SDK's `readline/readline.h` is a link to `../editline/readline.h` (`ls -la`), so with `CPATH`
cleared the binding is verified by clang against libedit's header and linked against GNU readline.
Refusing a build where `CPATH` is set refuses the repair this compiler's own note prescribes. Both
are categorically harder bindings; the first is a wrong value at exit 0 of exactly the class §1.12
closes.

**`-idirafter` on a package or `--include` folder does the same** (`build/ffi/rl/rl.c`, by hand,
GNU readline 8.3.6's folder that `fluidsynth`'s `.pc` names, linked `-lreadline` from Homebrew):

```
-I         readline.h read from the Cellar   RL_READLINE_VERSION=0x0803  linked 0x0803  sizeof(HIST_ENTRY)=24 offsetof(data)=16  ->data=0x0
-idirafter readline.h read from the SDK      RL_READLINE_VERSION=0x0402  linked 0x0803  sizeof(HIST_ENTRY)=16 offsetof(data)=8   ->data=0x1012dd7c0
```

Both build at exit 0. Under `-idirafter` a record the binding declares is laid out by libedit's
struct (`line`, `data`) while GNU's `history_get` hands back GNU's (`line`, `timestamp`, `data`), so
`->data` reads the timestamp pointer. The layout check would pass, because it asks the header clang
found. **This is the ABI break my seat holds a veto over**: the property that a wrong FFI declaration
is a compile error (§1.11's third point, §4.19) is kept only if the header clang reads is the
library's.

**`-idirafter` on the program's own folder** takes every platform header name from the program's own
headers, not the 36 the opening reads: 3,408 relative names on this Mac (`find . -name '*.h'` under
the SDK's `usr/include`, 3,127, and clang's resource `include`, 289, deduplicated), frameworks not
counted. A program's own `util.h` declaring `static inline int64_t my_util(void)` builds and prints 7
under `-I prog`; under `-idirafter prog` the SDK's `util.h` is read and clang says *call to undeclared
function 'my_util'* (`build/ffi/da/`), which the compiler would tell as a header that declares no such
name, naming the wrong file. In this tree it costs nothing today: 0 of the 663 distinct basenames of
the 849 tracked `.h` files outside `runtime/` and `seed/` is a top-level header of the SDK, clang's
resource directory, or (in the container) `/usr/include` and `/usr/include/aarch64-linux-gnu`. The
refusal takes 36 names and says so; `-idirafter` takes 3,408 and says nothing, or says something false.

**`-iquote` on the program's own folder** needs three things changed, each measured
(`build/ffi/iq/`):

1. the group's include emitted quoted, which reopens §4.19's measured sentence *the emitter emits
   `<header.h>` and never `"header.h"`*: re-run here, a `sqlite3.h` beside the unit in a build folder
   is read by `#include "sqlite3.h"` (`prints=decoy`, the two warnings only `#include_next`'s) and
   not by `#include <sqlite3.h>` (prints `3.54.0`, 0 warnings);
2. **the compiler's own drafted repairs**: `cli/macro_twice.hero:111` drafts a header of the
   program's own reading `#include <a.h>`, `#undef M`, `#include <b.h>`, and `cli/header_order.hero:192`
   drafts `#include <header>` after a line. The golden case's own drafted header,
   `tests/golden/run/fixedbugs-563-cap_fixedbugs-563-limit.h`, compiles under `-I` and prints
   `LIMIT=100`; under `-iquote` it fails, *'fixedbugs-563-cap.h' file not found with <angled> include*,
   **whether the unit includes it quoted or angled**;
3. every program header that includes a sibling with angle brackets: 3 such lines in tracked `.h`
   files and 9 in tracked `.c` files outside `runtime/` and `seed/` (`grep` of each `#include <x>`
   against the including file's directory).

And it leaves `--include` and package folders where they are, so it closes one door of three.

### Linux arm64, through the compiler

The stage 2 compiler built in `heroes-linux-arm64:latest` from the archived tree prints `sources
0d7a6741345899d8`. The `libm` program with `link "m"` added (Linux needs it; without it the three
environment rows were `ffi_missing_link`, my binding's mistake, not the class's), run with no
argument and with `one` (`build/ffi/lx/work/l3.sh`):

| shadow | beside `main.hero` | `CPATH` folder | `C_INCLUDE_PATH` folder |
|---|---|---|---|
| none | build 0; `... 1.4142135623730951 true`, exit 0; `one`: overflow panic, 134 | | |
| `math.h` | `shadowed_header`, exit 1 | **build 0; `sqrt(2.0)` prints `42.0`, exit 0** | the same |
| `stdbool.h` | `shadowed_header`, exit 1 | build 0; prints **`false`**, exit 0; `one`: the false UTF-8 panic, 134 | the same |
| `stdint.h` | `shadowed_header`, exit 1 | build 0; `one`: overflow panic, 134 | the same |

The two platforms agree row for row.

**An empty element of `CPATH` is the working directory** (`build/ffi/q1/cwdcase/`, `build/ffi/c1/cwd/`):
`CPATH=':'`, `'/nonexistent:'`, `':/nonexistent'` and `'/nonexistent::/other'` each make clang read
`./stdint.h` (`-H`), and `clang -E -v` lists `.` first. `export CPATH=$CPATH:/dir` with `CPATH`
unset writes exactly such a value. Through the compiler: `heroes-next build sub/main.hero` run from a
folder holding the shadow `stdbool.h` and `stdint.h`, the program in `sub/`, builds at exit 0 with
`CPATH=':/opt/homebrew/opt/readline/include'` and its run with `one` panics with the false UTF-8
message (134) instead of the overflow; with `CPATH` unset the same build is clean (overflow, 134). So
a rule that counts the environment's folders as named must count an empty element as the directory
clang runs in.

**`SDKROOT`, a route nobody listed, is the platform itself, not a folder before it.** An SDK copy whose
`usr/include/stdint.h` adds the add-overflow macro (`build/ffi/fakesdk/`, the rest symlinked): by hand
`SDKROOT=fakesdk clang probe.c` builds and prints `overflow=0` (clang's own `stdint.h` reads the SDK's
through `#include_next`); through the compiler, the reserved pair gives the builtin back and the run
panics, 134. Whoever sets `SDKROOT` names the platform; I put it to the sitting as the class's
boundary, not as a door to close, and did not try a `stdbool.h` there.

## 2. Question 2 over the packages here: what the refusal and item 2 stop

**The opening's library headers** (`build/ffi/op/names.sh`: the four lines `order_trial.opening`
writes for a unit with no group, under `-std=gnu11 -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS
-O0`, `clang -v -M`, each path made relative to the longest search directory clang printed): **36 on
this Mac**, **38 on Debian 13 arm64** (glibc's `bits/`, `features.h`, `sys/cdefs.h`,
`gnu/stubs.h` among them). The script is mine, not the compiler's; it reproduces the critic's 36.

| environment | packages | distinct `-I`/`-isystem` folders | folders holding an opening name | packages refused by `shadowed_header` |
|---|---|---|---|---|
| this Mac, `pkg-config --list-all` | 507 | 159 | **0** | 0 (by the folder count) |
| `heroes-linux-arm64:latest` | 52 | 5 | 1: `/usr/include/aarch64-linux-gnu`, 24 names, named by `libcurl` | **0 of 52, each built** |
| `heroes-linux-arm64:sdl3-b14` | 155 | 22 | the same one folder, 24 names | **0 of 61** with a `-D`, `-I` or `-isystem`, each built |

`libcurl` is the real test of the refusal's tie rule: its `.pc` names the multiarch system folder, which
holds 24 of the opening's 38 headers by name, and the refusal leaves it alone (build 0, prints `hi`),
because clang drops that `-I` as a duplicate of a system directory and the compiler gives the tie to
the platform. (My folder count is 159 where the critic's was 85 `-I` folders: I counted `-isystem`
folders and a separated value too; neither count holds an opening name.)

**Item 2, settled with the compiler, not a word match** (`build/ffi/pk/run.sh`, one build per distinct
`-D` name, naming the first package that carries it): this Mac's 244 packages with a `-D` carry 24
distinct names (`-DNOMINMAX` 343 times, the Qt switches, `_THREAD_SAFE`, `_DARWIN_C_SOURCE`, ...); 23
build and print `hi`; the 24th, `absl_absl_check`, is `ffi_package` on `-Wno-float-conversion`, the
allow-list's refusal, not item 2's. Debian's 52 carry `-D_XOPEN_SOURCE=600` and `-D_DEFAULT_SOURCE`
(12 ncurses-family packages each): all build. The SDL3 image adds `-D_REENTRANT` (6), `-D_BSD_SOURCE`,
`-DHAS_FCHOWN`, `-DHAS_STICKY_DIR_BIT`, `-DPCRE2POSIX_SHARED`: all build. **Item 2 refuses no package
on any of the three.** The one real `.pc` I know to carry a kept name is MinGW's SDL2, `-Dmain=SDL_main`
(SDL2's own configure for `mingw` and `cygwin`, from memory, **unrun**: no MinGW here, and this
project's Windows target is `x86_64-pc-windows-msvc`, `docs/platforms/windows/WINDOWS-MACHINE.md:357`);
its `Libs` carry `-mwindows`, which no table of `package_words.hero` admits, so it would be refused
before item 2 is asked (an inference from the tables).

**What the refusal would stop, argued for Linux and Windows, unrun**: a package whose folder overlays
the C library on purpose. Debian's `libbsd-overlay` names its own folder (by `-isystem`, which this
compiler hands on as `-I`) holding, from memory of its file list, a `sys/cdefs.h` and `stdlib.h`,
`string.h` and others that `#include_next` glibc's; `sys/cdefs.h` is one of the 38. Neither image holds
it (`ls /usr/include/bsd`: absent), and I did not fetch it. If it is as I remember, the refusal stops
it, and that is right by §1.12: an overlay may redefine what the unit takes from the library, and the
binding has a route that does not overlay (`package "libbsd"` and `#include <bsd/string.h>`, also from
memory). On Windows a vcpkg tree puts every port's headers in one `include/`, so one port shipping a
`stdint.h` would stop every package of that tree: a question for the box.

## 3. The two refusals against real bindings (brief item 3)

- **SDL2 2.32.74 on this Mac**, `.pc` carrying `-D_THREAD_SAFE`: `extern "SDL.h" package "sdl2"` binding
  `SDL_GetPlatform() -> cstr`, built and run (`build/ffi/sdl/mac/`): build 0, prints `Mac OS X`, exit 0.
- **SDL3 3.2.10 on Linux arm64** (`sdl3-b14`), `.pc` carrying `-D_REENTRANT` and four `-I`:
  `extern "SDL3/SDL.h" package "sdl3"` binding `SDL_GetVersion() -> i32` and `SDL_GetPlatform() ->
  cstr`: build 0, prints `3002010` and `Linux`, exit 0.
- **The same SDL2 `.pc` with `-Dmain=SDL_main` appended** (`build/ffi/sdl/pc/`, `PKG_CONFIG_PATH`):
  `error[ffi_package]: ... answered with -Dmain=SDL_main ...: main is a name the C this compiler
  writes, and no package's answer defines or undefines one: it would stand before every header and
  every guard`, exit 1, span on the group's `package` string. Why, in C: the guard pushes and pops
  `main` around a group's headers (`emit/guarded_names.hero`, `main` in `NAMES`), so SDL's own
  `#define main SDL_main` inside `SDL_main.h` is undone at the guard's close, while a `-D` stands
  before the first line of the unit and the guard saves it at its open and gives it back at its
  close: nothing undoes it (`cli/package_macros.hero:6-16`).

## 4. Question 7's C: SQLite 3.54.0 on this Mac

`build/ffi/sq/close.c`, compiled `-fsanitize=address` against `-lsqlite3` (header and library both
`3.54.0`), a statement prepared on `:memory:`:

```
sqlite3_close with a statement unfinalized: 5 (database is locked)
  the handle after it: exec 'create table t(x)' -> 0
  step the statement -> 100, column 0 = 7
sqlite3_close_v2 with the statement still unfinalized: 0 (not an error)
  the statement after it: step -> 100, column 0 = 7
  finalize (which deallocates the zombie connection) -> 0
```

So a refused `sqlite3_close` leaves the connection open and usable; `sqlite3_close_v2` never refuses.
The reader task R1's program under each fence, built with my compiler and run under the critic's
`shim.dylib` (`timeout 20 env DYLD_INSERT_LIBRARIES=...`), which writes the connections open at exit
(`build/ffi/sq/grade.sh`):

| program | build | prints | exit | open at exit | class |
|---|---|---|---|---|---|
| A, close once | 0 | `0 0 5 0` | 0 | **1** | silent (defect 2322's report) |
| A, retry when the close says 5 | **1, `handle_used_after_end`** | | | | the bare form cannot say the retry |
| B, close once | 0 | `0 0 5 0`, then `panic: 1 C handle(s) never given back` | 134 | (abort) | loud |
| B, retry | 0 | `0 0 5 0 0` | 0 | 0 | correct |
| C, `sqlite3_close_v2` | 0 | `0 0 0 0` | 0 | 0 | correct |
| D, `sqlite3_close` once | 0 | `0 0 5 0`, the same panic | 134 | (abort) | loud |
| D, retry | 0 | `0 0 5 0 0` | 0 | 0 | correct |
| D, `sqlite3_close_v2` | 0 | `0 0 0 0` | 0 | 0 | correct |

A refused close is told under B and D, silent under A, and does not happen under C. `5` is defect
2291's measured value, reproduced.

**The library with no `_v2`** (`store.h` of the reader test's R2, `build/ffi/sq/st_*`): the bare close
(what A teaches, and all C can teach where no twin exists) builds, prints `0 0 5 0`, exits 0 with
`stores.txt` 1: silent. `when 0` without the retry: 134, told. `when 0` with it: `0 0 5 0 0`, exit 0,
0 stores open. B's and D's lesson carries; C's has nothing to carry.

**And the release whose failure still releases** (`build/ffi/fc/`): C11 7.21.5.1 says `fclose`
disassociates the stream whether or not it succeeds. A program that makes `fclose` fail (its
descriptor closed under it with `close(fileno(f))`) and retries on a non-zero result:

- bound `function fclose(f: File consumes) -> i32 when 0`, `--sanitize`: on this Mac it builds, prints
  `-1` twice (the retry reached a stream already released) and then panics *1 C handle(s) never given
  back*, a false message, exit 134; **on Linux arm64 glibc's ASan reports `attempting double-free`**,
  exit 1;
- bound bare, `consumes`: the retry is `error[handle_used_after_end]` at compile time, which is right.

So `when 0` is the right word exactly where the documentation says a refused release leaves the
resource alive (`sqlite3_close`, `store_close`), and a memory error where it says the release happens
anyway. The fence teaches it on the right library; the prose that teaches it should tie it to that
sentence of the documentation. That is a spec-warden's line to write, not a reason against B.

**design.md §4.19's copy** (`docs/design.md:2578-2581`) with `special`'s own `main`: `error[lend_kept]`,
exit 1 (the critic's finding 2, reproduced); with `path: cstr lent` and the close `when 0`, build 0,
prints `0`.

## 5. Questions 3 to 6 and 8, from the boundary

- **Question 3**: `shadowed_header` as its own code is right: it is a fact about the build's folders,
  not about any one binding, and its repair is a file moved, never a declaration changed. The
  environment's folders belong to the same code, the folder's origin named. Item 2 stays
  `ffi_package`, since the word refused is a package's answer, but it owes its own note (below).
- **Question 4**: the message names the file, the folder and the library header. The note does not say
  **which of three origins** the folder is, nor which package named it: built with a stand-in package
  `overlay` whose folder holds `stdbool.h` (`build/ffi/pkgshadow/`), the note says *rename the file, or
  move it out of `<folder>`: the folder is this program's own, one `--include` names, or one a
  package's `pkg-config` answers*, at `main.hero:1:1`, and never names `overlay`. A package's file is
  not the author's to rename; the author must ask `pkg-config` of every package by hand to find which
  one named it, which design.md §4.17's *without opening another file* does not allow. The note should
  name the origin (the program's folder, `--include <dir>`, `package "overlay"`, `CPATH`) and, for a
  package, point at its group's `package` string as `ffi_package` does (`main.hero:1:24` there). And
  item 2's refusal carries the allow-list's note, which says `-D` is accepted and gives Go's
  CVE-2018-6574 as the reason a word is refused (`build/ffi/sdl/main_d/b.txt`): neither is this
  refusal's reason. **No `Fix`**: the repair is not text, the same ruling `head_names.hero:205-206` gives
  `machine_locked_path`.
- **Question 5**: on the shadowed folder `check` exits 0 (`--json`: `"diagnostics": []`) and `build`,
  `--emit-c`, `run` and `test` exit 1 (`build/ffi/c1/verbs/`). The FFI's central promise sits on the
  same line: `extern "math.h"` `function sqrt(x: f64) -> i64` is `check` exit 0 and `build`
  `ffi_return_type`, exit 1 (`build/ffi/wrongsig/`). So this refusal is not a new gap; it is feature
  974's, the whole C boundary being judged where clang runs. I would answer it once, for the boundary,
  and not for this refusal alone. The environment door is what makes the present line wrong, not
  `check`.
- **Question 6**: a folder case in `unsupported` (exit 1 expected, annotated in its `main.hero`), one
  per origin: the program's folder, `--include`, a stand-in package, and `CPATH` once the environment is
  ruled on.
- **Question 8**: design.md §4.19 should say, beside its *`-I<directory of the .hero source>`*
  paragraph: the folders a build names, the program's own, `--include`'s, a package's and those
  `CPATH` and `C_INCLUDE_PATH` name, an empty element being the directory clang runs in, are searched
  before the platform's, and a file of one that the runtime's opening reads is refused; and that a
  package's `-D` or `-U` of a name the guard keeps is refused. And it should correct the copy of the
  fence to `path: cstr lent` and the close `when 0`.

## Verdict

- **verdict**: **object** to ratifying item 1 as landed without the environment's folders; **approve**
  item 2, and item 3 as B (D teaches the same word and costs more: `heroes-next measure` over the
  specification with only the fence's lines changed, `build/ffi/tok/`, reads A 8181 and 8327, B 8183
  and 8330, D 8210 and 8357 on claude-legacy and cl100k_base, the vendored rows; the real row is
  unrun, being paid), with design.md §4.19's copy
  corrected to `path: cstr lent` and the close `when 0`; **veto** two routes the questions list:
  `-idirafter` for a package's or an `--include` folder, and clearing `CPATH` and `C_INCLUDE_PATH` for
  clang or refusing a build where they are set. `-iquote` I object to, without a veto. For
  `CCC_OVERRIDE_OPTIONS`, clearing it and refusing a build where it is set cost a binding the same,
  nothing; I lean to the refusal, which tells the person who set it.
- **section**: design.md §1.11, its third point (*FFI signatures are verified by clang against the real
  header*); §4.19, the `importc` mechanism and `docs/design.md:2983`, which names `CPATH` as the route
  to a header folder; §1.12 for the class.
- **experiment**: the C of each route, compiled by Apple clang 21.0.0 and Debian clang 22.1.8
  (`build/ffi/q1/orders.sh`, 22 rows, the same verdicts on both); the readline binding's C under `-I`
  and `-idirafter` (`build/ffi/rl/rl.c`, both accepted, the second wrong); the compiler's drafted
  header under `-iquote` (refused, *file not found with <angled> include*); and with the stage 2
  compiler, the `libm`, SQLite, store, `fclose`, SDL2, SDL3 and readline bindings above, on this Mac
  and, where marked, Linux arm64.
- **cost or delta measured**: the landed refusal refuses 0 of 507 packages here, 0 of 52 and 0 of 61 in
  the two Linux images, and 0 tracked headers; item 2 refuses 0 real packages on the three; counting
  `/opt/homebrew/include` as named would refuse 0 (0 of its 233 entries is an opening name). The vetoed
  routes: GNU readline's `RL_READLINE_VERSION` read as 1026 where the library is 2051, and
  `HIST_ENTRY` laid out in 16 bytes where the library writes 24, at exit 0.
- **argument**: The landed refusal costs no binding I could find: no package of 507 here, 52 or 61 in
  Linux, libcurl's multiarch folder included. But it leaves rung 2 of §4.19's ladder open: a `math.h`
  in a `CPATH` folder makes a clang-verified `sqrt(2.0)` print 42.0 at exit 0 on both platforms, and
  §4.19 and `machine_locked_path`'s own note send binding authors to `CPATH`. Clearing `CPATH` is
  worse than leaving it: the readline binding that note produces then compiles against libedit's
  header while linking GNU readline, 2051 becoming 1026 at exit 0. `-idirafter` on a package folder
  does the same. Count the environment's folders as named, an empty element as the working directory;
  reorder nothing.
- **prediction**: with the refusal extended to the folders `CPATH` and `C_INCLUDE_PATH` name (an empty
  element counted as the directory clang runs in) and no folder reordered, (a) the `libm` program of
  `build/ffi/c1/` with its `math.h` in a `CPATH` folder is refused `shadowed_header`, exit 1, on this
  Mac and Linux arm64; and (b) every one of the 20 `examples/` folders holding an `extern` (curl,
  raylib, sdl, sqlite among them) and the GNU readline binding of `build/ffi/rlh/rel/`, built with
  `CPATH=/opt/homebrew/opt/readline/include --library /opt/homebrew/opt/readline/lib`, build and print
  as they do at `163912031` (the readline one `2051`), with no shim and no new word. Scored by the lane
  that lands the environment's half: its `unsupported` folder case, `special`, `corpus` and `run`, and
  that one readline build on this Mac.
- **condition**: I approve item 1 as landed if the sitting rules the environment the user's own trust
  boundary **and** amends §4.19's `CPATH` sentence and `machine_locked_path`'s note to say a header
  folder named there is not checked, or if a measurement shows clang does not read those folders for
  the compiler's units (refuted here on two platforms). I lift the `-idirafter` veto if no package
  folder on the three platforms holds a header the platform also holds, or if a reordered build refuses
  such a package instead of reading the platform's header (GNU readline under `fluidsynth` refutes the
  first on this Mac). I lift the `CPATH`-clearing veto if `machine_locked_path` and §4.19 name another
  route that keeps the readline binding at 2051, measured, and a cleared variable that changes which
  header a group reads is refused rather than followed. Windows is unrun throughout: a measurement there
  that clang targeting `x86_64-pc-windows-msvc` does not read `CPATH` for a C unit changes the Windows
  half of my recommendation and nothing else.
