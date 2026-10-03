# Panel 188, the ffi-pragmatist's report

Written 2026-10-03 by the ffi-pragmatist, between 16:29 and 17:03 by `date`.
**Complete.** My copy is `<scratchpad>/188-ffi-pragmatist/tree/`, extracted
from `826ddc2f` by the shared brief's recipe: `shasum -a 256 seed/heroes.c`
begins `2c809845ed0f7ba8` and the compiler built from it `afc05be6b2c50184`,
both as the brief states (run 16:29). Every case is under
`<scratchpad>/188-ffi-pragmatist/` and each section names its script. No
paid run, no timing, no `measure --refresh`. Where the compiler's own words
hold an em dash this report writes a comma, so those quotations are exact but
for that.

*Appended 2026-10-03, 17:33 to 17:42, at the coordinator's question: § The
backstop, measured on real headers, at the end; it withdraws the backstop's
two flags, and a dated line under each place that recommended them says so.*

## The verdict, in the charter's form

- **verdict**: **veto (1i)**. **Approve (1h)**: one decode of the string's
  value shared by `check` and every emitter (1f), a refusal at `check` on the
  value of what `#include <...>` cannot carry (1a, with *a line end* meaning
  LF **and** CR) together with `\`, `"` and every control character, true
  messages at `build` for whatever still reaches clang (1d), and
  `-Werror=extra-tokens -Werror=null-character` as the backstop. **No
  objection on this seat's axis to (1b) or (1c)**: each refuses zero real
  headers. **(1g) approved for `\` and `"`, objected to as the whole rule**,
  since it judges the spelling. **Object to (1d) alone.**
  *Corrected underneath, 2026-10-03 17:42, at the coordinator's question:
  the backstop clause is withdrawn, its two flags dropped and the guard
  scoped to the one writer of `#include <...>`; § The backstop, measured on
  real headers, at the end.*
- **section**: design.md §4.19 (*"The emitter emits `<header.h>` and never
  `"header.h"`"*, the decoy paragraph, and *"Clang verifies the declared
  signature against the real header"*), §1.11 point 3, §1.12 (*"any C library
  must be bindable"*). **design.md does not say which characters a header's
  name may hold**; the nearest text is §4.19's absolute-path paragraph (panel
  055), which is about where a header is, not what its name holds.
- **experiment**: the C below, `#include <VALUE>` with the header on disk under
  the value, on Apple clang 21.0.0, Debian clang 22.1.8 and Debian clang
  18.1.8, identical on every row; every ASCII byte swept; F1, F6 and F7
  through the compiler on this Mac and in the Linux arm64 container,
  identical; the survey of every include root on both machines; `-include`;
  `link` and `package` through the compiler, the linker and `pkg-config`.
- **argument**: Every refusal set on the table costs zero libraries: of 103,736
  header-name instances under every include root of this Mac and the Linux
  arm64 image, of 554 `.pc` names and 2,046 library names, none holds a
  character outside the positive set, and a refused name stays bindable
  through a one-line header of the program's own. So the boundary does not
  choose among (1a), (1b), (1c), (1g). It refuses (1i): `-include` reads a
  decoy from the working directory and expands a name beginning `@` into
  compiler flags, so clang would verify signatures against a header nobody
  named. And a NUL must be refused at `check`: `extern "stdio.h<NUL>x"` binds
  `stdio.h` at exit 0 on both platforms.
- **prediction**: under (1h) as stated, none of the tree's 648 `extern` header
  strings nor any of its `link` and `package` strings is refused, and the
  SQLite, libcurl and raylib rungs of §4.19's ladder in `examples/` check and
  build unchanged on all three platforms; and when the Windows box is next
  online, `survey/survey.py` over its clang's resource `include/` and its
  Windows SDK's include roots finds **zero** header names outside
  `[A-Za-z0-9._+/-]`. Scored at the next push's platform legs.
- **condition**: the veto on (1i) lifts only if a real header needs a
  character `#include <...>` cannot carry (today none: 0 names hold `>`, LF,
  CR or NUL) **and** the compiler hands clang a path that neither the working
  directory nor `@` expansion can reach. My preference moves from (1h) to
  (1c) if the Windows leg shows a byte (1h) admits failing there or naming
  another file (`:` as a stream, a trailing `.` or space dropped, all recalled
  and unrun). My no-objection to (1c) becomes an objection if any platform's
  include roots hold a real header outside its set.

## Verdict per route

"Real headers refused" is the survey of § 2 below: the same number for every
refusal route because the population holds no character any of them refuses.

| route | the C it implies, and what three clangs did with it | real headers it refuses | verdict on this seat's axis |
|---|---|---|---|
| (1a) `>`, a line end, a NUL, the empty name | none of them can be written in `#include <...>` on any clang: `>` reads `'a' file not found` with `-Wextra-tokens`; LF, CR and CRLF read `expected '>'`; the empty name `empty filename`; a NUL either `'a<U+0000>b.h' file not found` or, where its prefix names a file, **that file, opened at exit 0** (§ 1.4). The ASCII sweep (§ 1.2) finds exactly LF, CR and `>` among 126 bytes | 0 | **approve, as the floor**, on two conditions measured here: *a line end* is LF **and** CR, and the rule reads the VALUE (over the spelling, `\n` is two harmless bytes and the refusal never fires, the critic's point) |
| (1b) also `'`, `\`, `"`, `//`, `/*` | all five carried, file found, **zero diagnostics even under `-Weverything`**, on all three clangs (§ 1.1); through the compiler `'`, `//`, `/*` build and print `7` today on both platforms, `\` and `"` fail only by the escape face | 0 (`//` is a spelling no file path shows, so the survey is vacuous for it; the tree's 648 lines hold none) | **approve** (no binding harder). It adds nothing to what fails: `'`, `//`, `/*` rest on C's text alone (UB, recalled; the historian's), against three implementations that read them identically |
| (1c) only `[A-Za-z0-9._+/-]` | everything in the set carried; refuses besides (1a)'s list every byte the sweep shows clang carries (space, `'`, `<`, `*`, `?`, TAB and the other controls, every byte above ASCII) | **0** of 103,736 names; the cost lands on a program's own header names, which work today (space, `'`, `<`, `é`, `@` measured) | **approve** (no library refused; its failure is loud and a rename or a one-line header repairs it, § 1.5). Not my recommendation: its set is a premise about the world where (1a)'s list is a measurement of C, and panel 185 R1 makes the program's own header part of every macro binding |
| (1d) nothing at `check`, true messages at `build` | two members **build at exit 0** on every clang under the compiler's flags: `stdio.h>` and `stdio.h<NUL>x` (§ 1.3), so no `build` message can see them | 0 | **object as the only answer**; approve as the complement of a refusal, with the backstop (§ 1.6) |
| (1f) emit the value | the value is found on all three clangs (`#include <a\b.h>`, `<a"b.h>`, `<a<TAB>b.h>`, zero diagnostics); the spelling the compiler writes today misses the file and says it is missing, on both platforms, for headers **and for `link`** (§ 4.1) | 0 | **approve, necessary**: one decode in the function both readers share, so `check` judges the bytes `build` writes |
| (1g) no escape in a header's string | covers `\`, `"`, LF, CR at once (only an escape writes them) | 0 (no name needs `\`, `"` or a TAB) | **approve for `\` and `"`**; object to it as the whole rule, because it judges the spelling: `"a\tb.h"` is refused while a raw TAB, the same value, builds on both platforms (§ 1.3) |
| (1h) a combination | as composed above | 0 | **approve** |
| (1i) `-include` | carries `>` but **not `"`** (clang writes `#include "a"b.h"` into its built-in buffer unescaped); reads a **decoy from the working directory**; **expands a name beginning `@` as a response file** in both spellings (§ 3) | 0 | **veto** |
| (1e) a route nobody listed | `-Werror=extra-tokens -Werror=null-character` in the compiler's flags: both exit-0 shapes become clang errors, every carried name still builds, the seed and the runtime compile with zero diagnostics (§ 1.6); for `package`, a `--` before the name (§ 4.2) | 0 | **approve, under any route** |

*Corrected 2026-10-03 17:42: row (1e)'s first half, the two `-Werror`
flags, is withdrawn (§ The backstop, measured on real headers, at the end);
its `--` for `package` stands.*

## What (1h) refuses, and why each

- **`>`, LF, CR, a NUL, the empty name**: what `#include <...>` cannot carry,
  measured on three clangs over every ASCII byte (§ 1.2); a NUL also names its
  prefix at exit 0 (§ 1.4), so it is refused at `check` and not left to
  `build`.
- **`\` and `"`**: clang carries both (§ 1.1), but no real header holds
  either (0 of 103,736, § 2), only an escape writes them, C leaves them
  undefined (recalled, the historian's), and on Windows `\` is a separator
  and `"` cannot stand in a name (recalled, unrun): one string would name two
  files on two platforms.
- **Every other control character** (TAB, U+0001 to U+001F, U+007F): clang
  carries them (§ 1.2), no real header holds one (§ 2), a message cannot show
  one, and judging them on the value refuses `"a\tb.h"` and a raw TAB alike,
  which is the objection to (1g).
- **What it admits**: every other byte C carries, a space, `'`, `<`, `*`,
  `?`, `//`, `/*` and UTF-8 beyond ASCII, because each builds today on three
  clangs and refusing it refuses a program that works, the program's own
  header, for no library gained. If the panel wants no undefined behaviour in
  emitted C, adding (1b)'s `'`, `//`, `/*` costs this seat nothing (0 real
  names); that is the historian's text to decide.

## 1. The C each route implies

### 1.1 `#include <VALUE>`, three clangs (`c/run.sh`)

For each name, `inc/<value>` holds `#include <stdint.h>` and `static inline
int32_t seven(void) { return 7; }`, and `u.c` is `#include <value>` over a
`main` returning `seven() - 7`. Each unit is compiled `-std=gnu11
-fsyntax-only`, to an object under the compiler's own flag list
(`selfhost/cli/flags.hero`, `flags()`), under `-std=c11`, under `-Wall
-Wextra` and under `-Weverything`; `-H` names the file clang opened; the
object is linked and run. On this Mac (`c/mac.txt`) and in the container
(`c/linux-clang22.txt`, `c/linux-clang18.txt`): **every row's exit codes,
run and diagnostics are identical on the three clangs** (`diff`).

| name (the value) | compiles | file opened | what clang says |
|---|---|---|---|
| `ab.h` | yes | `inc/ab.h` | nothing |
| `a>b.h` | **no** | none | `extra tokens at end of #include directive`, then `'a' file not found` |
| `a`, LF, `b.h` | **no** | none | `expected '>'` |
| `a`, CR, `b.h` | **no** | none | `expected '>'` |
| `a`, CR LF, `b.h` | **no** | none | `expected '>'` |
| `a\b.h` (one backslash) | yes | `inc/a\b.h` | nothing, under `-Weverything` too |
| `a"b.h` | yes | `inc/a"b.h` | nothing |
| `a'b.h` | yes | `inc/a'b.h` | nothing |
| `d//b.h` | yes | `inc/d//b.h` | nothing |
| `e/*b.h` | yes | `inc/e/*b.h` | nothing |
| `a b.h` | yes | `inc/a b.h` | nothing |
| `a<b.h` | yes | `inc/a<b.h` | nothing |
| `a`, TAB, `b.h` | yes | that file | nothing |
| `a??)b.h`, raw | yes under `gnu11`; **no** under `c11` | that file / none | `trigraph ignored`; under `c11` `trigraph converted to ']'`, `'a]b.h' file not found` |
| `a??)b.h`, the compiler's line splice (defect 207) | yes, both dialects | that file | nothing |
| `a`, NUL, `b.h`, no file `a` | **no** | none | `null character(s) preserved in string literal`, `'a<U+0000>b.h' file not found` |
| `a`, NUL, `b.h`, a file `a` present | **yes, runs, exit 0** | **`inc/a`** | only the warning above |
| the empty name | **no** | none | `empty filename` |
| `ab.h\` | yes | that file | nothing |
| `<a\>`: a backslash before `>` | yes, file `a\` | that file | nothing: a header name has no escapes |
| `é.h` | yes | that file | nothing |
| `<stdio.h>` (`#include <<stdio.h>>`) | **no** | | `'<stdio.h' file not found, did you mean 'stdio.h'?` |
| `stdio.h>` (`#include <stdio.h>>`) | **yes, runs** | the system's | `extra tokens at end of #include directive` only |
| `<stdio.h` | **no** | | `'<stdio.h' file not found, did you mean 'stdio.h'?` |

### 1.2 Every ASCII byte (`c/sweep.sh`)

The briefs' list is an enumeration, and a deny-list's completeness is the
question, so the list was taken from the world: every byte 0x01 to 0x7F but
`/` (the separator), `#include <a<byte>b.h>` with the file named by the same
bytes. **On all three clangs exactly three fail: 0x0A (LF), 0x0D (CR) and
0x3E (`>`).** Every other byte, every control byte included, builds with zero
warnings, under the backstop flags too. Two-, three- and four-byte UTF-8
names (`é`, `ü`, `€`, `日本`, `𝄞`) build with zero warnings
(`c/sweep-mac.txt`, `c/sweep-linux-clang22.txt`, `c/sweep-linux-clang18.txt`).
With the NUL and the empty name, **what `#include <...>` cannot carry is
`>`, LF, CR, NUL and the empty name, measured, on these three clangs**; a
byte that is not UTF-8 never reaches a header string, since `check` refuses
the whole file (`heroes-cases/bad-utf8`, exit 2, *cannot read `p.hero`*).

### 1.3 Through the compiler, both platforms (`hdr/heroes.sh`)

F1, F6 and F7 re-run on my compiler on this Mac, and on a compiler built from
the same seed inside the container (`heroes-linux`, sha256 `c2fcd60b4c3ecebb`):
**`hdr/mac.txt` and `hdr/linux.txt` are identical row for row and message
for message** once the build hashes are normalised. So F1 on Linux, unrun in
the facts, is now run. Beyond F1, F6 and F7:

| `extern "..."` (source) | file beside | `check` | `build` | run |
|---|---|---|---|---|
| `"a\tb.h"` | `a`, TAB, `b.h` | 0 | 1, `ffi_missing_header` naming `a\tb.h`, **while the file is there** | |
| `"a<TAB>b.h"` (a raw TAB) | the same file | 0 | 0 | `7` |
| `"ab.h<NUL>junk.h"` | `ab.h` | 0 | **0**, two `-Wnull-character` warnings | **`7`** |
| `"stdio.h<NUL>x"`, binding `puts` | none | 0 | **0**, the same warnings | **`hi`** |
| `"@x.h"` | `@x.h` | 0 | 0 | `7` |

### 1.4 A NUL names its prefix

F7 reads *"a raw NUL byte in the name: `check` 0, `build` 2"*. That holds
only where the prefix names no file. Where it does, clang opens the prefix
and the program builds and runs: `extern "stdio.h<NUL>x"` binds the system's
`stdio.h`, `extern "ab.h<NUL>junk.h"` binds `ab.h`, on both platforms
(`heroes-cases/nul-stdio`, `nul-own`; `hdr/*/nul-prefix`, `nul-stdio`). This
is §4.19's central promise failing at its root: clang verifies the
signature against a header the author did not name. No message at `build` can
see it, because clang succeeds. **So a NUL must be refused at `check`, by
every route.**

### 1.5 A refused name stays bindable (`heroes-cases/wrap-*`)

A header any route refuses is reached through a one-line header of the
program's own, the route §4.19 already prescribes for a function-like macro
(panel 185 R1). Measured through the compiler on this Mac, `extern "wrap.h"`
over `function seven() -> i32`, each `check` 0, `build` 0, printing `7`:

| `wrap.h` | the header beside it |
|---|---|
| `#include "a>b.h"` | `a>b.h` (the quoted form, inside the program's own header) |
| `#include <a b.h>` | `a b.h` |
| `#include <a"b.h>` | `a"b.h` |
| `#include <a\b.h>` | `a\b.h` (bytes checked by `od -c`) |

So the cost of any refusal route is bounded at one file of one line, for a
population that today is empty (§ 2). That is why no refusal route is a veto
matter for this seat. Unrun on Linux through the compiler; the direct C of
§ 1.1 carries the same names there.

### 1.6 The backstop, a route nobody listed (`c/backstop.sh`)

Today's flag list does not promote `-Wextra-tokens` or `-Wnull-character`, so
`#include <stdio.h>>` and `#include <stdio.h<NUL>x>` build at exit 0. With
`-Werror=extra-tokens -Werror=null-character` added to the compiler's flags,
on Apple clang 21.0.0, Debian clang 22.1.8 and 18.1.8 (`c/backstop-mac.txt`,
`c/backstop-linux-clang22.txt`, `c/backstop-linux-clang18.txt`):

- `stdio.h>`: **error**, *extra tokens at end of #include directive
  [-Werror,-Wextra-tokens]*;
- `a<NUL>b.h` with `a` present: **error**, *null character(s) preserved in
  string literal [-Werror,-Wnull-character]*;
- `ab.h`, the trigraph splice, `a\b.h`, `a"b.h`, `a'b.h`, TAB, `é.h`: still
  build, zero diagnostics;
- `seed/heroes.c` and `runtime/runtime.c`, `-fsyntax-only` under the full
  list plus the two: exit 0, **zero diagnostics**.

It turns the deny-list's failure direction from silent to loud for any member
a future clang or a future change lets through, at every writer of an
`#include` (the unit and every probe), for no cost measured. It is an emission
rule (`.claude/rules/generated-c.md`), so its landing is the
compiler-engineer's to price.

*Corrected 2026-10-03 17:42: "(the unit and every probe)" is false. Every
probe compiles under `compiling.probe_words`, which drop each word beginning
`-W` (`without_diagnostics`, `selfhost/cli/compiling.hero:141`), so the two
flags would reach the unit's compile and the runtime's and no probe. And this
paragraph's recommendation is withdrawn: § The backstop, measured on real
headers, at the end.*

## 2. What a strict rule would cost: the survey (`survey/survey.py`)

Every file under each root, symlinks followed, named as an author writes it in
`extern "..."` (its path relative to the root; a framework's `Headers/` as
`<Framework>/<path>`). Counted per root, overlaps not removed. The script
also counts each narrower route's characters; every column is zero.

| machine | root | names | outside `[A-Za-z0-9._+/-]` |
|---|---|---|---|
| Mac | `$(xcrun --show-sdk-path)/usr/include` | 3,480 | 0 |
| Mac | the SDK's 274 framework header roots, sub-frameworks included | 7,092 | 0 |
| Mac | `/opt/homebrew/include` | 19,206 | 0 |
| Mac | every keg's `include/`, `/opt/homebrew/Cellar/*/*/include` (132) | 27,297 | 0 |
| Mac | every `-I` directory the 502 `.pc` files answer (156, 153 present; absent: `/usr/include`, `/usr/include/apr-1`, `/usr/local/include`) | 27,796 | 0 |
| Mac | clang's resource `include/` | 299 | 0 |
| Mac | the three non-symlinked CommandLineTools SDKs' `usr/include` | 9,733 | 0 |
| Mac | the Xcode toolchain's `usr/include` | 5 | 0 |
| Mac | `/Library/Frameworks` (three frameworks, none with `Headers/`); `/usr/local/include` absent | 0 | 0 |
| Linux arm64 | `/usr/include` | 8,034 | 0 |
| Linux arm64 | `/usr/local/include` (present, empty) | 0 | 0 |
| Linux arm64 | clang 22's resource `include/` | 300 | 0 |
| Linux arm64 | gcc 14's `include/` | 44 | 0 |
| Linux arm64 | the 4 `-I` directories the 52 `.pc` files answer | 450 | 0 |
| **total** | | **103,736** | **0** |

The same for `link` and `package` against `[A-Za-z0-9._+-]`: **0 of 502 `.pc`
names on this Mac and 0 of 52 on Linux; 0 of 1,866 library names on this Mac
(`lib<name>.{dylib,a,tbd}` in `/opt/homebrew/lib`, the SDK's `usr/lib`, the
kegs' `lib/`) and 0 of 180 on Linux (`.so`, `.a` in `/usr/lib`,
`/usr/lib/aarch64-linux-gnu`, `/usr/local/lib`)**. And the tree: of 648
`extern "..."` lines in `.hero` files, **none** holds `//`, a backslash or a
character outside the set (`grep -rhoE` in `tree/`; the critic's 832 counts
`.md` and `.txt` too).

**What no survey reaches**, and it is (1c)'s real cost: the program's own
headers beside the `.hero` file, where a space, `'`, `<`, `é` and `@` build
today (§ 1.3, and the critic's rows). Unrun: the Windows box's SDK and clang
(offline, § Unrun).

## 3. Route (1i), `-include` (`include-flag/run.sh`)

The header handed to clang as `-include <name>` (and joined, `-include<name>`)
instead of an `#include` line, the unit holding none. **This Mac
(`include-flag/mac.txt`) and the Linux container (`include-flag/linux.txt`)
are identical** (`diff`).

| name | `-include` | what clang says |
|---|---|---|
| `a>b.h` | builds, `7` | nothing: the built-in buffer uses the quoted form |
| `a\b.h`, `a'b.h`, `d//b.h`, `e/*b.h`, `a b.h`, `a<b.h`, TAB | builds, `7` | nothing |
| `a"b.h` | **fails** | `<built-in>:1:13: extra tokens at end of #include directive`, `missing terminating '"'`: clang writes the name unescaped, `#include "a"b.h"` |
| LF, CR | fails | `missing terminating '"' character`, `expected "FILENAME" or <FILENAME>` |
| the empty name | fails | `empty filename` |
| `a??)b.h` | builds under `gnu11` (`trigraph ignored`), fails under `c11` | the line splice cannot be written in an argument |
| `<stdio.h>` | fails | `'<stdio.h>' file not found` |

And three failures `#include <...>` does not have, each measured:

- **A decoy in the working directory wins.** `-include ab.h -I src`, an
  `ab.h` returning 8 in the working directory and one returning 7 in `src/`:
  the program exits 1, **the decoy read**; `#include <ab.h> -I src` reads
  `src/ab.h`. A `stdio.h` in the working directory replaces the system's
  (`puts` undeclared). Panel 036's decoy beside the unit is not read by
  `-include` (and `#include "ab.h"` reads it, reproduced); the decoy has
  moved, not gone. §4.19's `<header.h>`-never-`"header.h"` paragraph is
  exactly this: `-include` is the quoted form by another door.
- **A name beginning `@` is a response file.** `x.rsp` holding `ab.h
  -DSMUGGLED=1` in the working directory: `-include @x.rsp` and
  `-include@x.rsp` each give `#define SMUGGLED 1` (`-E -dM`); `-###` shows the
  driver expanding the separate spelling and cc1 receiving `-include`
  `@x.rsp` for the joined one, which cc1 then expands. `#include <@x.rsp>` is
  inert (file not found). This is panel 055's `@file` door, CVE-2018-6574's
  class, opened by a header string; `machine_locked` does not refuse a
  leading `@` (`extern "@x.h"` builds today, § 1.3).
- **The empty name joined swallows the next word**: `-include` followed by
  `-I inc` reads *`'-I' file not found`* and makes `inc` a linker input.

And every diagnostic points at `<built-in>:1:10`, not at a line of the unit,
so each place the compiler reads clang's text back to a `.hero` line would
change, and every probe writing its own `#include` would need the flag
instead. **(1i) gains one character no real header holds (`>`: 0 of 103,736)
and loses the property §1.11 point 3 rests on, that clang reads the header the
author named. Veto.**

## 4. Q5: `link` and `package`

### 4.1 Through the compiler (`q5/heroes.sh`)

`extern "ab.h" link "<s>"` and `extern "ab.h" package "<s>"` over
`function seven() -> i32`, `ab.h` a `static inline` so the program needs no
library. `q5/mac-heroes.txt` and `q5/linux-heroes.txt`; the two differ only
where marked.

| `<s>` (source) | `link`: `build` | `package`: `build` |
|---|---|---|
| `zz9`, `a>b`, `d//b`, `e/*b`, `a<b`, `a??)b`, `@x.rsp` | 1, `ffi_missing_library`, true | 1, `ffi_package`, *not installed*, true |
| `-x` | 1, true (`-l-x`, a library named `-x`) | 1, *the package `-x` is not installed on this machine*: **false**, `pkg-config` said *unknown option -- x* and was asked for no package |
| `a\\b`, `a\"b`, `a\tb`, `a\nb`, `a\rb` | 1, naming the **spelling** (`a\\b`) | 1, naming the spelling |
| `a'b` | **Mac 2**: ld64 says `library 'a'b' not found` and `missing_library` (`selfhost/emit/ffi_build.hero:64`) cuts the name at the first `'`; *internal error: linking failed*, *clang refused the generated C*. Linux 1 | 1 |
| `a b`, and `m ` (a trailing space) | Mac 1. **Linux 2**: GNU ld says `cannot find -la b` and the parse (`:71`, `words`) cuts at the space; the same false *clang refused the generated C* | 1 |
| a raw NUL (`zz9<NUL>junk`, `m<NUL>junk`, `zlib<NUL>junk`) | **134 on both**: *panic: an argument contains a NUL byte*, the compiler aborting | **134 on both**, the same panic |
| the empty string | **2 on both**: the argument `-l` alone takes the next word, `-o`, so the output path becomes an input, *no such file or directory: 'p.NNNN-29.tmp/p'*, *clang refused the generated C* | 1, *the package `` is not installed on this machine*: `pkg-config` was asked for no package |
| `--version` | 1, true | 1, *the package `--version` answered with `3.0.7`* (Linux `1.8.1`): an option, called a package |
| `--atleast-pkgconfig-version=1` | | **0, runs, `7`, on both**: a `package` clause naming no package, accepted |
| `zlib >= 99` | | 1, *the package `zlib >= 99` is not installed on this machine*: **false**, zlib 1.2.12 (Mac) and 1.3.1 (Linux) are installed and `pkg-config` refused the version |
| `zlib >= 1`, `zlib sqlite3`, `zlib,sqlite3` | | 0, runs: `pkg-config`'s version and list grammar, which spec § 13 does not name |
| `./seven.pc`, `seven.pc`, `../pc/seven.pc` | | 0, runs, **from the `.pc`'s directory**; from another working directory `ffi_package`, *not installed*. pkgconf reads a `.pc` FILE relative to the working directory, not the program's (Mac only; `q5/mac-locked/pc`) |

And the escape face for `link`, with the library really there:
`link "a\\b"` and `lib/liba\b.a` under `--library lib` reads *this machine has
no library called `a\\b`* **while the library the string names is there**, on
both platforms (`q5/mac-heroes-extra/esc`, `q5/linux-heroes-extra.txt`).

### 4.2 The linker and `pkg-config` directly (`q5/direct`, `q5/linux-direct.txt`)

- `clang -### u.o -l -o out.bin`: the link line carries **`-l-o`** and
  `out.bin` as an input, on both (ld64; GNU ld, `/usr/bin/ld`).
- The value `-la\b` links `liba\b.a` and runs; the spelling `-la\\b` reads
  `library 'a\\b' not found` (Mac), `cannot find -la\\b` (Linux).
- GNU ld's message for `-la` + LF + `b` breaks the line after `-la`. So
  decoding `link` strings (1f) would hand a real LF, TAB or space to a
  message the compiler reads one line and one word at a time: **an inference
  from these two measurements**, that (1f) for `link` owes the refusal of
  whitespace and control characters in the same commit, or a
  `missing_library` that does not read the name out of the linker's text.
- **`--` before the package name repairs the option face** on pkgconf 3.0.7
  (Mac) and 1.8.1 (Linux): `pkg-config --cflags --libs -- --version` reads
  *Package --version was not found*, exit 1, and `-- zlib` still answers
  `-lz`. The comment at `selfhost/cli/libraries.hero:303-305`, *"A package
  name reaches `pkg-config` as one word whatever is in it, so no `.hero` file
  can hand this program an argument of its own making"*, is true of the word
  and false of its meaning for a string beginning with `-`: panel 055's shape,
  a premise in a comment going on reading as correct.

### 4.3 Does `machine_locked` cover every absolute form it means to

No. It reads the spelling's first two bytes. **`extern "../../../../../../../../../../../../../../../../<the absolute path without its leading />"`
checks 0, builds 0 and prints `7`** on this Mac (`q5/mac-locked/climb2`): a
header named by where this machine keeps it, which is what panel 055 refused.
And the `.pc` paths of § 4.1 name a file by where the working directory is.
Linux unrun for both.

### 4.4 Any other string handed to C undecoded

Every one of the 19 emit call sites of `emit_externs.unquoted` (8 files: `externs`,
`ffi_build`, `ffi_field`, `ffi_incomplete`, `ffi_layout`, `ffi_lookup`,
`ffi_record`, `ffi_tag`) reads a group's header or its `link` or `package`
name; the parser's `module_text.unquote` serves `machine_locked` and
`parse/use_line.hero:243`, a `use` line, which never reaches C (`grep -rn
'unquoted(\|unquote(' selfhost`). So the three FFI strings are the whole of
it on that search; the program's string literals go through `c_text.literal`,
which escapes every byte (defect 207).

## Corrections and additions to the facts

- **F7's NUL row** holds only where the prefix names no file; where it does,
  `build` is 0 and the program runs on the prefix (§ 1.4).
- **(1a)'s *line end* is two bytes**: CR fails as LF does on all three clangs
  (§ 1.1, § 1.2).
- **F1 on Linux** is now run: identical to this Mac (§ 1.3).
- **The briefs' character list** was an enumeration; the sweep (§ 1.2) is the
  measurement it stood for, and it adds nothing to `>`, LF, CR, NUL and the
  empty name.

## Found beside, for the coordinator to file and class

None is on `docs/work/DEFECTS.md` by `grep -n -i` for `NUL byte`,
`machine_locked`, `missing_library`, `--atleast`, `cannot read`, `utf-8`;
the nearest is 183 (a `.pc` prefix holding a space). The class is the
coordinator's; the one I would read by § Bounded discovery's list is beside
each.

1. A raw NUL in a `link` or `package` string: the compiler panics, exit 134
   (the compiler crashing where the author can be told: `blocking`).
2. `link ""`: `-l` swallows `-o`, exit 2, a false *clang refused the
   generated C* (`blocking`).
3. `link "a'b"` on this Mac and `link "a b"` (or `"m "`) on Linux: exit 2 by
   `missing_library`'s parse, the same false line (`blocking`; defect 216's
   shape for `link`).
4. `package "--atleast-pkgconfig-version=1"` builds; `package "--version"`
   and `package "-x"` are called packages, and the second's *not installed*
   is false (a wrong program accepted and a false message: `blocking`).
5. `package "zlib >= 99"`: *not installed*, false (`blocking`, a false
   message); constraints and lists accepted unspecified.
6. A `package` naming a `.pc` file relative to the working directory, and a
   header reached by a `..` climb to an absolute path: `machine_locked`
   misses both (`adjacent`).
7. The escape face for `link` and `package`: the same cause as F1's, this
   sitting's to repair with (1f) or (1g).
8. Outside the sitting: a `.hero` file holding a byte that is not UTF-8 gets
   *cannot read `p.hero`* at exit 2 from `check`; the exit-code contract
   (`.claude/rules/cli-surface.md`) decides whether that is right.

## Unrun

- **The Windows box**: `ssh -o ConnectTimeout=20 -o BatchMode=yes win true`
  timed out at 16:31 and 16:53 (exit 255), Tailscale *offline, last seen 2h
  ago*. So unrun there: every table above, the survey of its SDK, whether
  clang on Windows reads `\` in `#include <a\b.h>` as a separator, and
  whether NTFS's reserved characters (`<>:"|?*` and the control bytes, recalled
  from Microsoft's naming rules, unverified) or its dropping of a trailing
  `.` or space make a name (1h) admits mean another file there. Those are the
  facts my condition turns on.
- On Linux: the wrapper of § 1.5 through the compiler, the `.pc` path and
  the `..` climb of § 4.3.
- C11 6.4.7's text: the historian's; nothing here confirms or refutes it.
- No timing was taken.

## Cost

No paid run. Four containers of `heroes-linux-arm64`, one at a time, each
started only over an empty `docker ps -q` (the first wait: `7996fa2f7a49`,
another session's, from 16:30 until it left before 16:36:33): leg one
16:37:04 to 16:37:18, leg two 16:47:01 to 16:47:32, leg three 16:50:36 to
16:50:49, leg four 16:53:11 to 16:53:27, by their own `date`. `clang-18` was
fetched by `apt-get` (network) in legs one, three and four, as the brief's
recipe, with `apt-get update` first (the critic's correction). Nothing written
in the trunk but this file.

## Where everything is

Under `<scratchpad>/188-ffi-pragmatist/`: `c/run.sh`, `c/sweep.sh`,
`c/backstop.sh` and their outputs (`c/mac.txt`, `c/linux-clang22.txt`,
`c/linux-clang18.txt`, `c/sweep-*.txt`, `c/backstop-*.txt`);
`survey/survey.py` and `survey/{mac,linux}-*.txt`; `include-flag/run.sh`,
`include-flag/{mac,linux}.txt`; `hdr/heroes.sh`, `hdr/{mac,linux}.txt`;
`q5/heroes.sh`, `q5/{mac,linux}-heroes.txt`, `q5/linux-direct.txt`,
`q5/direct/`, `q5/mac-locked/`, `q5/mac-heroes-extra/`,
`q5/linux-heroes-extra.txt`; `heroes-cases/` (the NUL, the wrappers, the
UTF-8 case); `linux1.sh` to `linux4.sh`, the four container legs.

## The backstop, measured on real headers (appended 2026-10-03, 17:33 to 17:42)

**The coordinator's question**, about 17:32: the two flags of § 1.6 judge a
header's contents wherever clang does not class its directory as a system
one, so a real header holding `#endif FOO`, `#include <x.h> junk` or a NUL
would stop a binding that builds today, which is this seat's own charter
failing. Measured in my copy; nothing written in the trunk but this file.

*The coordinator's correction, 2026-10-03 at 18:19 by `date`: "about 17:32"
is the label the coordinator wrote on its message, a guess; the message
reached this seat at 17:29:43 by its transcript's timestamp.*

### The calibration (`backstop/calib/`)

One header planting nine shapes. Through `-I`, Apple clang 21.0.0 and Debian
clang 22.1.8 each report seven: an `#endif` or `#else` with tokens after it,
even where it closes a skipped block; `#ifndef X Z`; `#undef X W`;
`#include <stdint.h> V`. Neither reports an `#ifdef A B` or an
`#include ... U` inside a skipped block. On both, `-isystem` reports 0,
`-isystem -Wsystem-headers` 7, `CPATH` 7 and `C_INCLUDE_PATH` 0; and on Linux
`-I /usr/include` reads *ignoring duplicate directory "/usr/include"*, a
system directory staying one. So a proxy over-approximates, clang is the
judge, and `-Wsystem-headers` gives the non-system verdict on a header clang
classes as system.

### (1) The real headers clang flags as non-system (`backstop/scan.py`, `backstop/judge.py`)

**The proxy**: every file under § 2's roots, deduplicated by real path; a
candidate holds a NUL byte, or a directive with tokens after it outside a
comment: `endif`, `else`, `include`, `ifdef`, `ifndef`, `undef` (the
coordinator's set) and also `include_next`, `import`, `line`, `elifdef`,
`elifndef` and `pragma once`; line splices joined first, a block comment
allowed to span lines, a literal not. Checked on the calibration headers: it
finds all nine planted shapes, a directive after a `*/` that closes a
comment, and `#include "q.h" ;`, and none of `#endif /* fine */`,
`#endif // fine` or `#include <a//b.h>`. **Then clang on every candidate**:
`#include <name>` with its root on `-I` (a framework's parent on `-F`),
`-fsyntax-only` under `probe_flags()` plus `-Wextra-tokens -Wnull-character`,
judged non-system (`-Wsystem-headers`) and as clang classes the root, each
bare and after a prelude of `stddef.h`, `stdint.h`, `stdio.h` and
`sys/types.h`, three processes at most.

| machine and clang | roots | distinct files | proxy candidates | files clang flags, judged non-system | real C headers a binding would meet |
|---|---|---|---|---|---|
| this Mac, Apple clang 21.0.0 | 566 (§ 2's 94,908 names) | 50,729 | 17 | 12, every one a Mach-O binary | **0** |
| Linux arm64, Debian clang 22.1.8 and 18.1.8 | 8 (§ 2's 8,828 names) | 7,559 | **0** | 0 | **0** |
| the tree's own C headers, `*.h` under `tests/`, `examples/`, `runtime/` | 3 | 194 | **0** | 0 | **0** |

The Mac's 17, by path (`backstop/mac-scan.txt`, `backstop/mac-judge.txt`):

- **`usr/include/libxml/xmlversion.h`**, line 454 `# undef
  LIBXML_ATTR_ALLOC_SIZE(x)` and line 471 `# undef
  LIBXML_ATTR_FORMAT(fmt,args)`, in the Xcode SDK and in the CommandLineTools
  SDKs `MacOSX15.4`, `MacOSX26.5` and `MacOSX27.0`: 4 files. **Clang flags
  none of them in the default configuration**: both lines sit in the `#else`
  of `#ifndef LIBXML_ATTR_ALLOC_SIZE` (and `_FORMAT`), live only where the
  macro is already defined. With `'-DLIBXML_ATTR_ALLOC_SIZE(x)='` clang flags
  `xmlversion.h:454:31` judged non-system, and nothing as the system header
  it is. On this Mac `pkg-config --cflags libxml-2.0` answers no `-I` at all,
  so the compiler reaches the SDK copy as a system header; and the three
  `.pc` answers pointing into a CommandLineTools SDK name its `editline`,
  `ffi` and `uuid`, never libxml. Debian's copy holds neither line, and
  reached through `pkg-config`'s own `-I/usr/include/libxml2` it draws no
  warning with the macro predefined or not (`backstop/xmlv-linux.txt`).
- **`/opt/homebrew/Cellar/boost/1.92.0/include/boost/iostreams/detail/broken_overload_resolution/stream.hpp:184`**,
  `#endif BOOST_IOSTREAMS_DETAIL_BROKEN_OVERLOAD_RESOLUTION_STREAM_HPP_INCLUDED`:
  C++; every compile as C stops at a fatal error before that line, and no
  Heroes binding reads it.
- **12 Mach-O dynamic libraries**,
  `/opt/homebrew/Cellar/qtbase/6.11.2/lib/Qt<X>.framework/Versions/A/Qt<X>`
  for `Concurrent`, `Core`, `DBus`, `Gui`, `Network`, `OpenGL`,
  `OpenGLWidgets`, `PrintSupport`, `Sql`, `Test`, `Widgets` and `Xml`, inside
  the framework directories Qt's `.pc` files answer with `-I`
  (`/opt/homebrew/lib/QtCore.framework`, beside its `Headers`). Clang flags
  them with `-Wnull-character` (34 lines); they are binaries no `extern`
  names.

So **zero real C headers on either machine would stop a binding under the
two flags** in their default configurations, and one, `xmlversion.h` in four
copies, would under a configuration predefining its macro and reached as
non-system, which no route reaches it as on this Mac. **What the surveys
cannot see is the population the flags meet first**: a program's own headers
and the C it vendors beside itself, reached through `-I <source dir>`, where
an `#endif FOO` builds today with a printed warning (`-Wextra-tokens` is on
by default, F6's build output shows it) and would be refused. The tree's own
194 hold none; the world's are unmeasured.

### (2) How the compiler hands a header's directory to clang (read in my copy)

- **the program's own directory**: `-I <source dir>`
  (`selfhost/cli/compiling.hero:91-92`), non-system;
- **`--include <dir>`**: `-I <dir>` (`:83-84`), non-system;
- **a package**: `libraries.compile_flags` (`:86-87`), `-I` and `-F` by the
  allow-list, a `-isystem` answer refused (`selfhost/cli/libraries.hero:403`,
  a test); non-system, except a `-I` naming a default system directory
  itself, which clang ignores as a duplicate (above), and `pkg-config` drops
  such directories from its own answers (`libxml-2.0`: no `-I` on the Mac,
  `-I/usr/include/libxml2` on Linux);
- **the runtime**: `-I <runtime>` (`:89`), non-system, and ours (zero
  diagnostics under the flags, § 1.6);
- **`CPATH`**, the route `selfhost/parse/group_head.hero:93` names to the
  author: non-system, measured on both clangs; **`C_INCLUDE_PATH`**, which the
  compiler names nowhere: system;
- **the default directories** (the SDK; `/usr/include`, `/usr/local/include`,
  clang's and gcc's own): system;
- **`-isystem`, `-iquote`, `-idirafter`, `-iframework`, `-Wsystem-headers`,
  `--system-header-prefix`**: nowhere in `selfhost/` (`grep -rn`).

**And which compiles the flags would reach if added to `flags()`**: the
unit's (`selfhost/cli/units.hero:104`, `compiling.unit_words`), the
runtime's (`selfhost/cli/toolchain.hero:226`) and the link line
(`selfhost/cli/link.hero:129`, which reads no header). Every header probe,
the pointee check (`selfhost/cli/pointee.hero:105`), `header_ask`
(`selfhost/cli/header_ask.hero:40`) and the reads and layout probes
(`compiling.hero:117`, `:220`), compiles under `probe_words`, which drop every
word beginning `-W` (`without_diagnostics`, `:141`). So the flags would judge
header contents in exactly one compile, the unit's, through every non-system
route above; and § 1.6's *"the unit and every probe"* was wrong.

### (3) What the flags leave to catch in a header's NAME once R2 refuses at `check`

**Nothing**, by the sweep (§ 1.2), which compiled every ASCII byte in a name
without and under the two flags: **no byte builds with a warning or fails
only under them**. The three that fail, LF, CR and `>`, fail without them,
and R2 refuses those three, the NUL, the empty name and every other control
character. The five UTF-8 names and the trigraph splice draw nothing under
them (§ 1.2, § 1.6). And the name is the only program text in a directive the
compiler writes: `#include <` + name (`selfhost/emit/c_text.hero:113`, the one
writer, for the unit and every probe); `#ifdef ` + name
(`selfhost/emit/ffi_asked.hero:170`, `:210`) takes an identifier; a `#line`
name goes through `c_text.literal`; the `#pragma clang diagnostic error`
lines (`selfhost/emit/extern_probe.hero:136`,
`selfhost/emit/layout_screen.hero:85-87`) name fixed flags. **So once R2
refuses at `check`, the flags' whole remaining reach is the contents of
headers, never a byte the compiler wrote.** Nor can clang narrow them to the
compiler's own line: `#pragma clang diagnostic push`, `error
"-Wextra-tokens"`, the `#include`, `pop` turns all seven planted shapes inside
the header into errors, exit 1 (`backstop/calib/scoped.c`), a header
inheriting the diagnostic state of the line that includes it.

### Verdict on the backstop: drop the flags; adopt it scoped, as a predicate on the compiler's own `#include` line

- **Drop** `-Werror=extra-tokens -Werror=null-character`. Measured benefit
  once R2 lands: none, in any line the compiler writes. Measured cost: none
  over 58,288 distinct files of two machines in their default
  configurations, and an unmeasurable one by construction, every program's
  own header and vendored C, where today's printed warning would become the
  refusal of a binding that builds. A flag whose only reach is code the
  compiler did not write, and whose only effect there is a refusal, is §1.11
  failing for nothing bought.
- **Adopt it scoped**: R2's predicate asked once more by the one writer of
  `#include <...>` (`c_text.header_lines`) before the line is written,
  failing as the compiler's own error (exit 2, naming the name it would not
  write) if a name ever reaches it unjudged, from a reader nobody routed
  through `check`. That is §1.12's *check rather than assume* on the
  compiler's own bytes and on nobody else's, it costs no header anything, and
  it fails in the loud direction `.claude/rules/module-shape.md` asks of a
  fallback.
- **What this corrects above**, each with a dated line beneath it: the
  verdict block's backstop clause, the first half of row (1e), and § 1.6's
  last paragraph. Row (1e)'s `--` before a `package` name stands: it repairs
  the compiler's own argv and judges nobody's header.

**Prediction**: when the Windows box is next online, `backstop/scan.py` and
`backstop/judge.py` over its clang's resource `include/` and its Windows SDK's
include roots find no real C header that clang flags with `-Wextra-tokens` or
`-Wnull-character` in the default configuration; and once R2 is in `check`,
the scoped guard never fires on any program in `examples/` or `tests/`, since
it asserts a state `check` makes unreachable, so a firing is a compiler
defect and never a program's.

**Condition**: the flags come back only if a failure in the compiler's own
`#include` line turns up that the name's bytes cannot predict, on some
platform (none on three clangs); and even then the first repair is to refuse
that byte at `check`, because a clang flag cannot be scoped to the
compiler's line (measured above).

**Cost**: one more container, 17:37:49 to 17:38:04 by its own `date`, started
over an empty `docker ps -q`, with `clang-18` fetched by `apt-get` once more.
No paid run, no timing.
