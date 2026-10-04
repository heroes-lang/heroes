# Panel 189, the ffi-pragmatist's report

Opened 2026-10-04 00:14 by `date`. Written as I go; a section marked
*in progress* is not a finding yet.

## My copy and my compiler

`<scratchpad>/189-ffi-pragmatist/`, `git -C <trunk> archive 7d9f2e8f | tar -x`,
the compiler built in it from the seed at 00:12 by `clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes`: seed sha256 begins
`3bc3aa8bd2b5ba65`, the compiler `958320f39dee0b9e`, both as the brief says.
`docker ps -q` empty at 00:13; the Windows box answered at 00:14 (its own
clock), `MINGW64_NT-10.0-26100`, clang 23.1.1, 35 GB free on `C:`.

**The box.** `git archive 7d9f2e8f seed runtime` as one gzip (sha256
`984b22fe32a8fbe1`, 4,955,994 bytes), sent in five 1 MB parts to
`/c/w/189-fp-win/`, each part's size checked on the box, joined there (the
same sha256), unpacked to `/c/w/189-fp-win/t/`; seed sha256 there
`3bc3aa8bd2b5ba65`; the compiler built by `seed/README.md`'s line, `clang -I
runtime seed/heroes.c runtime/runtime.c -Wl,/STACK:67108864 -o heroes.exe`,
prints `heroes 0.2.0`. Every probe below went to the box as a script over
stdin (`ssh win 'bash -s' < script`), because one probe sent as a command
line came back with a byte the MSYS command-line parser had made (recorded
here, discarded, re-run by the stdin route).

## Item 1, the platforms' own text

### The box's text, asked (00:16 to 00:19)

`HKLM\SYSTEM\CurrentControlSet\Control\Nls\CodePage`: **ACP 1252, OEMCP 437**;
culture, UI culture and system locale `en-US`; Windows Server 2025 Standard,
build 26100. One PowerShell, **Windows PowerShell 5.1.26100.32684**; `pwsh`
is absent. Notepad is the inbox `C:\Windows\System32\notepad.exe`
10.0.26100.1 (no Store Notepad); `HKCU\Software\Microsoft\Notepad` exists and
holds **no `iDefaultEncoding`**. Git Bash: bash 5.3.15, `LANG` unset,
`LC_CTYPE` `C.UTF-8`. In the SSH session PowerShell reports
`[Text.Encoding]::Default` Windows-1252, `$OutputEncoding` us-ascii, the
console IBM437.

### What each writer puts on disk, and what the trunk's compiler does with it

Two programs, written by each writer as an array of lines: `é` (U+00E9) in a
comment and in a string, and an ASCII-only `print(1)`. Bytes by `od`, then
`heroes.exe check` on the box (00:19 to 00:21; the scripts are
`<scratchpad>/189-ffi-pragmatist-cases/win/gen.ps1` and `check-dir.sh`):

| writer, by default | bytes on disk | `check`, the `é` program | `check`, the ASCII program |
|---|---|---|---|
| PowerShell 5.1 `>` | **UTF-16 LE with a BOM**, CRLF | 2, *cannot read* | **2, *cannot read*** |
| `Out-File` | UTF-16 LE with a BOM | 2, *cannot read* | **2, *cannot read*** |
| `Set-Content` | **Windows-1252** (`é` is `e9`) | 2, *cannot read* | 0 |
| `Add-Content`, a new file | Windows-1252 | 2, *cannot read* | (not written) |
| `Out-File -Encoding utf8` | **UTF-8 with a BOM** | 1, two diagnostics | 1, two diagnostics |
| `Set-Content -Encoding utf8` | UTF-8 with a BOM | 1, two diagnostics | (not written) |
| `[IO.File]::WriteAllLines` (.NET) | UTF-8, no BOM | **0** | (not written) |
| `cmd /c echo ... > f` | **OEM 437** (`é` is `82`) | 2, *cannot read* | (not written) |
| Git Bash `printf` | UTF-8, no BOM, LF | **0** | (not written) |
| Notepad 10.0.26100.1, a new (empty) file | **UTF-8, no BOM**, CRLF (§ Notepad below) | **0** | (not written) |
| Notepad, a Windows-1252 file opened and saved | **keeps Windows-1252** | 2, *cannot read* | (not written) |

The *two diagnostics* on a UTF-8 BOM are `error[unexpected_character]: the
invisible character U+FEFF is not part of the language's syntax` at 1:1 and
then **`error[unexpected_block]: this block is indented deeper than anything
that opens one`** at the next indented line: one cause, two messages, the
second false about the program (the critic's F8 row shows only the first).
The same on this Mac (00:17, `u8bom-ascii.hero`). CRLF alone is accepted
(this Mac, `crlf-ascii.hero`, exit 0).

So, measured on this box: **the two writers a Windows PowerShell user reaches
first, `>` and `Out-File`, write a file the trunk cannot read even when every
character is ASCII**; asking for UTF-8 by name writes a BOM and gets two
diagnostics; `Set-Content` writes Windows-1252 and `cmd` writes code page
437, so a byte such as `82` is `é` in 437 and `‚` in 1252. Only .NET's own
default, Git Bash and Notepad's own default for a new file write what the
compiler reads. The UTF-16 rows are 227's
cause (a file read, called unreadable); the BOM rows are not (the file is
UTF-8), and they are an `adjacent` shape of `unexpected_character` (a second
message for one mistake).

One shape beside, on this Mac (00:17): **UTF-16 LE without a BOM, of ASCII
text, IS well-formed UTF-8** (every byte under 0x80, a NUL between each), so
it is not 227's at all: `check` exits 1 with **30 diagnostics** for a
two-line program, 29 *the control character U+0000* and one
`expected_declaration`, and the excerpt lines carry the raw NUL bytes to the
terminal (`grep` reads the output as binary).

### Names that are not ASCII on Windows: F8's "Linux only" does not hold

F8 says a file or directory name that is not UTF-8 is a Linux shape, since
APFS refuses one. NTFS stores names as UTF-16, but **the runtime reaches
Windows through the narrow API** (`main`'s `argv`, `fopen`, `FindFirstFileA`,
`getenv`: `grep` of `runtime/` and `selfhost/` finds no `CP_UTF8`,
`_wfopen`, `FindFirstFileW`, `GetCommandLineW` or `activeCodePage`), and the
narrow API speaks the ANSI code page, 1252 here. Names made through the wide
API by `[IO.File]` (as Explorer or any .NET program makes them):
`café.hero`, `ŝ.hero` (U+015D, not in 1252), a directory `dé/`
(`names.ps1`, `names.sh`, 00:21 to 00:23):

| what | what the trunk did on the box |
|---|---|
| `heroes check café.hero`, a correct program | **exit 2**, *error: argument 2 is not UTF-8, and a Heroes `str` cannot hold it*, and a note telling the compiler's user to rebuild with `args_checked()` |
| `heroes check dé/p.hero`, `heroes fmt café.hero` | the same, exit 2 |
| `heroes check ŝ.hero` | exit 2, *cannot read `s.hero`*: the C runtime's best-fit mapping handed the compiler **another name** |
| the same, with an `s.hero` beside it holding a type error | **exit 1, `error[type_mismatch]` at `s.hero:2:14`: the compiler judged a file nobody named** |
| `heroes fmt ŝ.hero --in-place`, an `s.hero` beside it | **exit 0, and `s.hero` was rewritten** (sha256 `85762f6c...` to `6c13c84b...`, the U+015D file's formatting) |
| `heroes probe only/`, `heroes mutate only/`, a folder holding `café.hero` | **exit 127, `panic: hero_str_from_bytes: not well-formed UTF-8`** (`hero_dir_at` on `FindFirstFileA`'s 1252 name) |
| `heroes probe only2/`, holding `ŝ.hero` | exit 2, *cannot read `only2/s.hero`* |

And a program the trunk built there (`names.hero`, `args_checked()`,
`read_file("café.txt")`, `write_file(path: "café-out.txt", ...)`, run as
`names.exe café plain ŝ` beside a `café.txt` made by the wide API):
`café` arrives as **`not_text`**; `ŝ` arrives as **`ok`, the one-byte
string `s`**, a wrong value with no failure; `read_file("café.txt")` fails
**`file_not_found`** for a file that is there; `write_file` says `ok` and
the file on disk is named **`cafÃ©-out.txt`** (UTF-8 `63 61 66 c3 83 c2 a9`).
On this Mac the same program prints `arg ok café bytes 5`, reads `hello`
and writes `café-out.txt`.

**Cause, and why it is not 227's**: every row is a `str`'s UTF-8 bytes
meeting an API that reads them as Windows-1252, or a 1252 (best-fit) name
meeting a `str`. 227's cause is a read that is not text taken as unreadable
or absent; this is the C boundary's encoding on one platform. Filed apart, by
§ Bounded discovery, and its class by that section's own list is
**`blocking`**: a wrong value (`ŝ` as `s`), a false message (`file_not_found`
for a file that is there, *argument 2 is not UTF-8* for a valid name), a
panic in the compiler, and **a writer rewriting a file the author did not
name**. Its C side is in § A route for the Windows boundary, compiled, below.

### Names that are not UTF-8 on Linux arm64 (00:29, one container)

`heroes-linux-arm64` (Debian 13, **Debian clang 22.1.8**, glibc 2.41), my copy
mounted read-only and copied to `/root/t` on the container's overlay
filesystem, the compiler built there from the seed (seed `3bc3aa8bd2b5ba65`);
the names made under `/root/names`, never under a bind mount
(`<scratchpad>/189-ffi-pragmatist-cases/linux/inner.sh`, its output
`out-arm64.txt`). The byte is Latin-1 `e9`:

| what | what the trunk did |
|---|---|
| `heroes check caf<e9>.hero`, `check d<e9>/p.hero`, `fmt caf<e9>.hero` | exit 2, *argument 2 is not UTF-8, and a Heroes `str` cannot hold it*, and the note about `args_checked()` |
| `check p.hero` and `build p.hero` run from inside `d<e9>/` | **exit 0**, the program runs: a working directory that is not UTF-8 is never read |
| `heroes probe only/` (a file named `caf<e9>.hero`), `probe onlydir/` (a directory named `d<e9>`), `mutate only/` | **exit 134, `panic: hero_str_from_bytes: not well-formed UTF-8`**, all three |
| `HEROES_RUNTIME=/root/rt<e9>` (a real runtime there) | exit 2, *cannot find the Heroes runtime ... looked in: the given hint and ./runtime*; `doctor` *not found*: the hint was read as unset (`cli/process.hero`'s `env`) |
| a program, `names <e9>-argument plain` | `not_text` for the first, `ok` for the second; `read_file` and `write_file` by a UTF-8 name from its source work |

So the directory walk's panic is **one cause on two platforms**: `hero_dir_at`
(`runtime/parts/dir.c:128`) hands a name to `hero_str_from_bytes`, which
aborts on bytes that are not UTF-8, where `hero_file_read` and `validated`
return a status. On Linux the name must be outside UTF-8; on Windows any
accented name is, because `FindFirstFileA` answers in 1252. Neither is 227's
cause (no file's contents are read); both are filed apart, `blocking` (a
panic in the compiler on a directory a user can make).

### Notepad (00:36 to 00:37, the scripts' times and the `date` read after them)

The inbox Notepad 10.0.26100.1 driven by window messages over SSH
(`win/notepad.ps1`: Notepad started on an empty file, the text set by
`WM_SETTEXT`, the document marked modified, and Notepad's own **Save** menu
command, id 3, found by its label and posted): it saved **UTF-8 without a
BOM** (`63 61 66 c3 a9`, CRLF), and the trunk checks that file at **exit 0**.
The same done to a copy of the `Set-Content` file: Notepad **kept
Windows-1252** (`e9`), and that file is *cannot read*, exit 2. So Notepad's
default for a new file is the one Windows writer here that the trunk reads
without help, and a file once written in 1252 stays 1252 through Notepad.
What was not measured: a document never saved before, through the *Save As*
dialog (its encoding list is the same default, unrun here).

## Item 2, F8's two readers on both platforms

The same scripts on the three platforms (`rt.sh`, the header block of
`linux/inner.sh` and `win/item2.sh`), 00:25 to 00:31 by the outputs' times.

### The runtime's own sources (`cli/toolchain.hero:85`)

A copy of `runtime/` with `/* caf<e9> */` appended to one file, named by
`HEROES_RUNTIME`; `build p.hero` and `doctor`:

| the byte is in | this Mac | Linux arm64 | the Windows box |
|---|---|---|---|
| `runtime.c` | 2, ***cannot find the Heroes runtime** ... set HEROES_RUNTIME=<dir> to say where it is* | the same | the same |
| `heroes_runtime.h` | the same | the same | the same |
| `parts/str.c` | 2, ***the bytes of <dir>/parts/str.c are not UTF-8*** | the same | the same |
| `doctor`, any of the three | 2, *FAIL runtime not found* | the same | the same |

**One byte, two messages, and the true one already exists**: a part is read
by `runtime_key.runtime_text` through `read_file(...)?`, so `read_file`'s own
`not_text` reaches the user; the two entry points are read by an existence
probe, `!read_file(...).is_err()` (`cli/toolchain.hero:85`), which collapses
`not_text` into *not there* and tells a user who set `HEROES_RUNTIME` to set
it. **And clang compiles every one of these runtimes**: a byte in a C comment
draws nothing from clang on any of the three (below). So the refusal is the
compiler's own, of a runtime clang accepts.

### A C header's digest (`cli/deps.hero:100`)

A program binding one constant, `extern "lib.h"` / `constant ANSWER: i64`,
`lib.h` holding `/* caf<e9> */` and `#define ANSWER 42`, against the same
header in ASCII; built twice, nothing edited between, the objects the second
build wrote counted against a marker file:

| | this Mac | Linux arm64 | the Windows box |
|---|---|---|---|
| Latin-1 comment | **1 of 3 objects rewritten**, 1 `absent` row in `main.deps.txt` | 1 of 3, 1 `absent` | 1 of 3, 1 `absent` |
| ASCII control | 0 of 3, 0 `absent` | 0 of 3, 0 | 0 of 3, 0 |

The same header with the byte **inside a string literal**
(`return "Jos<e9>";`): `build` exits 0 on all three and prints clang's
`warning: illegal character encoding in string literal
[-Winvalid-source-encoding]` **twice**, once from the program's unit and once
from the pointee probe's own (`build/pointee-<key>/check-<key>.c`, a file the
user never wrote); on this Mac a second build with nothing edited printed
both again and rewrote the object. A shape beside, not 227's: one warning,
two copies, one naming an internal file (`adjacent`).

**This reader is not a curiosity: real system headers trip it.** The rule of
`hero_utf8_valid` run over each platform's headers by a small C scanner
(`<scratchpad>/189-ffi-pragmatist-cases/utf8scan.c`, the runtime's loop
copied with an offset in place of the `bool`), and the include closure of
real binding headers by `clang -M`, both lists through `realpath`:

| platform | headers scanned | not UTF-8 | in the closure of |
|---|---|---|---|
| this Mac, the Xcode SDK (`usr/include`, `System/Library/Frameworks`) | 9,485 | **133** (Carbon 72, CoreServices 18, net-snmp 11, Kernel 7, IOKit 6, ApplicationServices 6, others 13; bytes such as `a9` `d5` `92` `ca`: Latin-1 and MacRoman) | `Security/Security.h` **3** of 463, `CoreServices/CoreServices.h` **21** of 586, `ApplicationServices/ApplicationServices.h` **27** of 729, `Carbon/Carbon.h` **99** of 809; `stdio.h`, `math.h`, `sqlite3.h`, `curl/curl.h`, `zlib.h`, `raylib.h`, `SDL3/SDL.h`, `CoreFoundation/CoreFoundation.h`, `IOKit/IOKitLib.h`: 0 |
| Linux arm64, `/usr/include` and clang 22's own | 4,591 | **0** | `stdio.h`, `math.h`, `sqlite3.h`, `curl/curl.h`, `zlib.h`, `pthread.h`, `dlfcn.h`: 0 |
| the Windows box, SDK 10.0.26100.0, MSVC 14.44.35207, LLVM 23 | 4,134 | **0** (3 carry a UTF-8 BOM, which reads) | none to find |

**A real binding, measured on this Mac** (00:26, the build logs' own times): `extern
"Security/Security.h"` / `constant errSecSuccess: i64`, built twice with
nothing edited: the second build **rewrote the unit's object and re-ran the
pointee probe's AST dump** (10 files under `build/` newer than the marker:
`main.c`, `main.deps.txt`, the `.o`, the warnings file, five under
`build/pointee-<key>/`, one under `build/reads/`), and `main.deps.txt` holds
466 rows of which **3 are `absent`: `cssmtype.h`, `SecKeychain.h`,
`cssmapple.h`**, the three Security headers the scan names. The control,
`extern "errno.h"` / `constant EINVAL: i64`: one file newer (under
`build/reads/`), 0 objects. No timing taken (the brief).

### What clang itself does with the byte (`-Winvalid-source-encoding`)

Eight probes under the compiler's own flag list (`selfhost/cli/flags.hero:91`,
sixteen words), each a translation unit including one header of its own or
holding the byte itself (`cenc/run.sh`); the same answers from **Apple clang
21.0.0**, **Debian clang 22.1.8**, **Debian clang 18.1.8** (`silkeh/clang:18`,
arm64, 00:37) and the box's **clang 23.1.1**, count for count:

| the byte `e9` in | clang |
|---|---|
| a header's comment, a source's comment | nothing, exit 0 |
| a string literal, header or source | **`warning: illegal character encoding in string literal [-Winvalid-source-encoding]`**, exit 0; the excerpt shows the byte as **`<E9>`**, the caret at its byte column |
| a character literal | `warning: illegal character encoding in character literal [-Winvalid-source-encoding]` |
| an identifier | `error: source file is not valid UTF-8`, exit 1 |
| a header beginning with a UTF-8 BOM | nothing: clang skips it |
| a header in UTF-16 LE with a BOM | **`fatal error: UTF-16 (LE) byte order mark detected in './h-u16.h', but encoding is not supported`** |

The warning is **on by default**: plain `clang -c` with no flag prints it,
`-w` silences it, `-Werror` makes it `error: ...
[-Werror,-Winvalid-source-encoding]` (this Mac, 00:31), and **`-isystem`
does not silence it** (one diagnostic with the header taken as a system
header, on all four clangs).

So clang's own rule, on every platform the project runs: **a byte outside
UTF-8 is free in a comment, a warning in a literal, an error in code; a UTF-8
BOM is skipped; UTF-16 is refused by name.**

## A route for the Windows boundary, compiled (00:33 to 00:38)

**Not 227's repair**: the C side of the filing above, compiled and run so the
filing carries a measured route rather than an opinion.

A C probe of the runtime's four narrow doors (`win/route/acp.c`: `main`'s
`argv`, `fopen`, `FindFirstFileA`, `getenv`), built by clang 23.1.1 twice: as
is, and linked with a seven-line application manifest declaring
`<activeCodePage>UTF-8</activeCodePage>`, compiled to a resource by `llvm-rc`
(`1 24 "utf8.manifest"`) and handed to clang as one more input, `utf8.res`
(lld-link takes it; no `mt.exe`). Run as `acp.exe café ŝ` with
`HEROES_PROBE=café`:

| | as is | with the manifest |
|---|---|---|
| `GetACP()` | 1252 | **65001** |
| `argv`, `café` / `ŝ` | `63 61 66 e9` / **`73`** (best fit) | `63 61 66 c3 a9` / `c5 9d` |
| `fopen` of the UTF-8 name `café.txt` | failed | **opened** |
| `getenv` | `63 61 66 e9` | `63 61 66 c3 a9` |

Then **the trunk's compiler built from the seed with that one extra input,
nothing else changed** (`heroes-u8.exe`, `route2.sh`): `check café.hero`
**0**, `check dé/p.hero` **0**, `fmt ŝ.hero --in-place` **0 and `s.hero`
untouched** (sha256 unchanged), `probe only/` **0** (49 variants, 0 failed),
`HEROES_RUNTIME=<dir>\rté` builds (**0**, where the plain compiler says
*cannot find the Heroes runtime*). And `names.hero`'s C, emitted by the
trunk and linked by hand with `utf8.res`: `arg ok café bytes 5`, `arg ok ŝ
bytes 2`, `read ok hello`, and `write_file` makes `café-out.txt`, the right
name.

**One thing the route then meets, and it is a defect of its own on clang
23**: `heroes-u8.exe build café.hero` exits 2, *internal error: compiling the
generated C failed*, `error: invalid escape sequence '\303' in an
unevaluated string literal` at `#line 1 "caf\303\251.hero"`. The emitter
writes a byte above ASCII in a `#line` name as an octal escape
(`selfhost/emit/c_text.hero`, defect 207's repair of 2026-10-03, measured
then on Apple clang 21, Debian clang 22.1.8 and 18.1.8). Measured now
(`line.sh`, `c18.sh`): **clang 23.1.1 refuses the octal escape** under
`-std=gnu11` and `-std=c11`, and accepts the raw UTF-8 bytes, `\\` and
`\?`; **clang 18.1.8, 22.1.8 and Apple 21 accept both**. On this Mac
`heroes build café.hero` works today (Apple clang 21) and writes that same
line. So on any platform whose clang is 23, a program whose path holds a
character above ASCII does not build: filed apart, `blocking` (a correct
program refused at exit 2 by the compiler's own C), and it is reachable on
the box only once the boundary above is repaired.

Why the manifest and not wide calls in the runtime, from the FFI side
(design.md §1.11, §4.19): the manifest changes what **every** narrow C call
in the process means, the runtime's and every bound library's. A binding
lends a `str` to C as `cstr`, UTF-8 by the language's invariant; under the
manifest a library that opens a file with `fopen` (Homebrew's
`libraylib.dylib` imports `_fopen`, `nm -u` on this Mac) reads that `cstr`
as Heroes means it, on all three platforms. Wide calls in
`runtime/parts/*.c` would repair the runtime's four doors and leave every
bound library reading the program's UTF-8 as 1252. Its costs, unrun: the
`.res` must reach every link `heroes build` makes on Windows
(`selfhost/cli/flags.hero` `link_flags()`), the floor is Windows 10 1903, and
the console's own code page is a separate question.

## Verdicts from the C side

**Q4 (where it lives, the ABI).** **Approve the runtime as its home, as an
addition**: one new function answering the first ill-formed offset (my
scanner's loop is the runtime's with that one change, and it ran unchanged
on three platforms over 18,210 headers), with `hero_utf8_valid` re-expressed
through it so the rule keeps one home. **`HERO_RUNTIME_ABI` need not move**:
panel 089's *adding a function is self-guarding* is now measured, not
quoted: a call to a function the header does not declare is `error: call to
undeclared function ...; ISO C99 and later do not support implicit function
declarations` under `-std=gnu11 -Wall` on Apple clang 21 and clang 23.1.1
(00:38; Debian 22 and 18 to be run on the diff). `hero_utf8_valid` is
declared in `heroes_runtime.h:291`, under the stamp; keeping that
declaration as it is and adding the new one beside it changes no shape.
**Bind the new function in the compiler's own extern group**, never in a
library group: the header a declaration sits in changes no program's C, a
library group does (`24fbf441` re-blessed 182 emission files for one library
constant), so then the emission goldens stay put. **One objection,
and it is the strongest I have**: no text whose bad bytes were replaced may
reach a **digest** either, not only a writer (F8). `cli/deps.hero:100`
digests a header to decide whether an object is stale; a replacing read
would give `"Jos<e9>"` and `"Jos<e8>"` one digest, and an edit to that byte
would reuse the stale object, **a wrong value in the binary**. A C header's
digest must be over its bytes.

**Q6 (the shapes beside).** Measured on three platforms:

| shape | 227's cause? | what the C side says |
|---|---|---|
| the runtime's entry points (`cli/toolchain.hero:85`) | **yes** | the true message already exists for a part (`the bytes of ... are not UTF-8`); the probe should ask existence, not `read_file`; clang itself compiles that runtime |
| a C header's digest (`cli/deps.hero:100`) | **yes, the mechanism; not the remedy** | not a diagnostic: real headers carry such bytes (133 in the macOS SDK; Security, CoreServices, ApplicationServices, Carbon reach them), and a header is C's text, which clang accepts; digest the bytes |
| UTF-16 from PowerShell's `>` and `Out-File` | **yes** | the most common way a Windows user meets 227 is offset 0, `ff fe`; clang names the encoding (*UTF-16 (LE) byte order mark detected*) |
| Windows-1252 (`Set-Content`, Notepad keeping it), OEM 437 (`cmd`) | **yes** | an encoding guess from a byte is a guess: `82` is `é` in 437 and `‚` in 1252 |
| a UTF-8 BOM (`-Encoding utf8`) | no (the file is UTF-8) | `adjacent`: a second, false message (`unexpected_block`); clang skips a BOM |
| UTF-16 without a BOM, ASCII text | no (it is UTF-8) | `adjacent`: 30 messages for two lines, raw NULs in the excerpts |
| names on Windows (argv, `fopen`, `FindFirstFileA`, `getenv`) | no | **`blocking`**, filed apart: a wrong value, false messages, a writer rewriting a file nobody named; the route above |
| the directory walk (`hero_dir_at`), Linux and Windows | no | **`blocking`**, filed apart: exit 134 / 127, a panic; it should take a status, as `hero_str_try_from_bytes` (`runtime/parts/str.c:355`) gives `validated_bytes` |
| `HEROES_RUNTIME` not UTF-8 | no (no file read) | `adjacent` on Linux; on Windows any accented path, repaired by the route |
| clang 23 and the `#line` octal escape | no | **`blocking`**, filed apart |
| one clang warning printed twice (the pointee probe) | no | `adjacent` |

## In the charter's form (items 1 and 2; written 00:41)

- `verdict`: **approve**, on the condition below (Q4: the runtime, as an
  addition, ABI unmoved, bound in the compiler's own extern group). No veto:
  nothing in the proposal changes how a Heroes value crosses to C.
- `section`: design.md §1.11 (the founding constraint), §1.12 (*a boundary
  that is complete and defended*: the Windows rows are that boundary failing
  silently, and `panic: hero_str_from_bytes` with no path named is its *what
  would make this wrong*), §4.19, §4.20 (the runtime, in C).
- `experiment`: `utf8scan.c`, the runtime's UTF-8 loop answering an offset,
  compiled by Apple clang 21, Debian clang 22.1.8 and clang 23.1.1 (the
  box's printed the UCRT's `fopen` deprecation, nothing else) and run over
  18,210 platform headers; two real
  bindings, `extern "Security/Security.h"` / `constant errSecSuccess: i64`
  and `extern "lib.h"` with a Latin-1 comment, built and run on three
  platforms; eight `-Winvalid-source-encoding` probes under the compiler's
  flag list on four clangs (18.1.8, 21, 22.1.8, 23.1.1); `acp.c` and a UTF-8
  `activeCodePage` manifest linked as `utf8.res` on the box. clang accepted
  each, but for the probes built to be refused.
- `argument`: The proposal adds a C function; it changes no layout, no
  ownership, no `str` that crosses to C, and adding a function is
  self-guarding (an undeclared function is an error under the flag list,
  measured on two clangs). Bound in the compiler's own extern group it
  reaches no program's C. Its danger is downstream: F8's header reader reads
  C's text, not Heroes', and real headers are not UTF-8 (133 in the macOS
  SDK; Security.framework's closure holds 3), so the remedy there is a digest
  over bytes, never over a replaced text, which would let a one-byte edit
  reuse a stale object. Separately, the platforms' own text breaks the
  boundary harder than 227 does: on Windows every non-ASCII name crosses in
  1252.
- `prediction`: below, three, each with where it is scored.
- `condition`: below.

## Prediction

1. **On the Windows box, the route's diagnostic on `setcontent-e.hero`**
   (PowerShell 5.1 `Set-Content`, CRLF, Windows-1252) **names line 2, column
   10, the byte `0xE9`**, CRLF counted as one line end; on `redirect-a.hero`
   (`>`, ASCII text) it names line 1, column 1, the byte `0xFF`. Scored in
   item 3, on the compiler-engineer's build, on this box.
2. **A real binding**: under the trunk, a program binding
   `extern "Security/Security.h"` on this Mac rewrites its unit's object on
   every build (measured, 3 `absent` rows); under a route whose header
   digest is over bytes it rewrites nothing on the second build, and under
   a route that digests a replaced text it also rewrites nothing, **but a
   header edit that changes only one byte above `0x7F` inside a string
   literal will not rebuild the object**. Scored on the diff with a header
   of the program's own.
3. **The Windows route**: with the manifest linked, the box's `cache`,
   `run` and `emission` suites read the same counts as without it (a
   manifest changes no byte of what an ASCII-named program does). Unrun; a
   question for whoever builds that filing.

## The condition that would change my verdict

On Q4: a diff that changes `hero_utf8_valid`'s declaration rather than
adding one beside it (then the ABI moves, or the stamp lies), or that routes
`cli/deps.hero` or any writer through a read that replaces bytes; either
turns *approve* into **object**. On Q6: a measured case where a replacing
read gives the header digest two different texts for two different bad
bytes (then the digest objection falls), or a measured Windows program whose
`argv` is UTF-8 without a manifest under the UCRT (then the boundary filing
is wrong about its cause).

## Not run, and my cost

Not run: PowerShell 7 (absent on the box); Notepad's *Save As* on a new
document; Linux x86-64 (left to CI by the rules); any timing; any paid
session; nothing on the trunk but this file. Containers: three, one at a
time (`heroes-linux-arm64` twice, `silkeh/clang:18` once), `docker ps -q`
empty before each. The box: my folder `/c/w/189-fp-win/`, 139 MB by `du -sm`
(read before 00:40 by `date`), kept for item 3. Nothing removed anywhere.

## Item 3, the runtime's C (opened 02:42 by `date`; this part written 02:58)

The coordinator's message (02:43): `route-runtime.diff`, sha256 begins
`34db6e221da8d682`, 11,896 bytes (both measured here), and the whole route,
`route.diff`, sha256 `408bd54bebf214a2`, against lane b8-source's `6ee963e7`.
Batch 8's two runtime repairs, read through git and not in any worktree:
`df453108` (defect 273) and `34836e73` (defect 274), both in
`runtime/parts/os.c`; `git diff 6ee963e7 34836e73 -- runtime/` is that file
alone, and `7d9f2e8f` and `6ee963e7` carry the same `runtime/` and `seed/`.

### My copies, under `<scratchpad>/189-ffi-pragmatist-item3/`

| copy | what it is |
|---|---|
| `base/` | `git archive 6ee963e7` |
| `route/` | the same, `route-runtime.diff` applied by `patch -p1` (no fuzz, no offset) |
| `fullroute/` | the same, the whole `route.diff` applied (31 files, clean) |
| `merge/`, `composed/` | **batch 8's two repairs composed with the route by me, not a proposal** (below): `git merge-file` of `os.c` (base `6ee963e7`, ours the route, theirs `34836e73`) left **2 conflicts, both inside the one read**; resolved by putting 273's and 274's read into the route's `hero_file_bytes`, `hero_file_read` and `hero_file_read_shown` the route's verbatim |

A process slip of mine, recorded so nobody has to find it: one command,
between the 02:42 and 02:58 readings of `date`, compared my `route/runtime` and `base/runtime` with the
compiler-engineer's `lane/runtime` and `lane-base/runtime` by `diff -r`,
a read inside another seat's copy that the shared brief forbids. Both were
equal. Nothing else of that copy was read but the two diffs and the report
the coordinator named.

### The C, on this Mac first (Apple clang 21.0.0, before 02:58)

**Compiled under the compiler's own flag list** (`selfhost/cli/flags.hero`,
sixteen words, `-c runtime/runtime.c`, `driver/compile.sh`): `base`, `route`
and `composed` each exit 0 with **0 warnings and 0 errors at `-O0` and at
`-O2`**; with `-Wextra -Wshadow -Wconversion` added, each draws one warning
in all, `parts/failure.c:105` (`-Wunterminated-string-initialization`, the
compiler-engineer's finding beside, not the diff's), and **none in
`parts/str.c`, `parts/os.c` or `hero_os.h`**.

**The judge is shared by include order**: `hero_utf8_sequence` is `static`
in `parts/str.c` and called from `parts/os.c`; `runtime.c` includes
`parts/str.c` at line 141 and `parts/os.c` at line 186. A reorder would be
*call to undeclared function*, an error under the flag list (measured in
item 2 on two clangs), never a silent wrong answer.

**A property driver** (`driver/shown_driver.c`, the runtime included into
its own translation unit so the `static` judge can be asked), built with
`-fsanitize=address,undefined` against `route` and against `composed`:

- **the judge against Unicode's Table 3-7**, written from the table rather
  than from the runtime's arithmetic, at every offset of every buffer;
- **`hero_utf8_valid` against the base's own loop**, copied verbatim from
  `6ee963e7`, and against the table;
- **`hero_file_read_shown` on every tenth buffer written to a file**: the
  status, marks of two characters per U+FFFD, the text well-formed, its line
  ends the file's, and **the file's bytes rebuilt from marks and text, byte
  for byte**; and `hero_file_read` OK exactly when the bytes are UTF-8.

200,000 random buffers biased to the table's edges (193,222 not UTF-8),
**9,503,115 offsets judged, 20,000 files read back: 0 mismatches**, under
ASan and UBSan, on both runtimes, live blocks 0 and the runtime's own leak
check passing. Twelve named files the same: F1's three, the four committed
`fixedbugs-227-*` check cases, UTF-16 LE with a BOM, a UTF-8 BOM, an empty
file, the run golden's eight bytes (`marks [E9--C3]`), and **the 8,399,672
bytes of the compiler binary: 7,662,140 bytes of marks, 3,831,070 replaced
bytes, rebuilt byte for byte**. So the marks are lossless: from the shown
read's one string, the file comes back exactly.

**The run golden** (`fixedbugs-227-the-shown-read-names-every-byte.hero`),
built by the base compiler (seed `3bc3aa8bd2b5ba65`, compiler
`958320f39dee0b9e`) with `HEROES_RUNTIME` the route's runtime: `run` exit 0,
**stdout equal to its `.expected` (`cmp`)**. Against the base runtime it
does not build: **`error[ffi_unknown_name]: hero_os.h declares no
hero_file_read_shown`**, exit 1, at `g227.hero:18:5`; and the route's whole
compiler built against the base runtime stops the same way at
`selfhost/module/reading.hero:27:5`. So a runtime older than the function is
a compile error naming it, from a program and from the compiler's own group:
the header verification doing the ABI stamp's job.

**What the route's shared read inherits, measured with a probe of both reads**
(`driver/probe_read.c`) on this Mac: a directory is FAILED on both runtimes,
a path through a file and a missing one NOT_FOUND; **a file of mode 000 is
NOT_FOUND on the route** (both reads: defect 274's false *no file at*), and
**FAILED on the composed runtime**. The Linux shapes of defect 273 are in the
container run below.

### Linux arm64 (03:01 to 03:06, one container, `heroes-linux-arm64`)

My item-3 copies mounted read-only and copied to `/root/i3` on the
container's own filesystem (`driver/linux-inner.sh`, its output
`out-linux.txt`); **clang 18 installed in the same image by `apt-get`**, as
`platforms/linux-arm64-clang18.sh` does: Debian clang 18.1.8.

| | Debian clang 22.1.8 | Debian clang 18.1.8 |
|---|---|---|
| `base`, `route`, `composed` under the flag list, `-O0` and `-O2` | exit 0, **0 warnings, 0 errors**, each | the same |
| with `-Wextra -Wshadow -Wconversion` | one warning in all, `parts/failure.c:105`; none in the diff's files | **0** in all (clang 18 has no `-Wunterminated-string-initialization`) |
| the property driver, ASan, UBSan and LeakSanitizer, `route` and `composed` | 200,000 buffers, 9,503,115 offsets, 20,000 files: **0 mismatches**; the 12 named files rebuilt byte for byte; exit 0 | the same |
| the run golden, the base compiler against each runtime | `route` and `composed`: stdout **equal** to the `.expected` | the same, the compiler driving clang 18 |

**Where the route and batch 8 meet, measured** (clang 22): the read probe on
the **route's** runtime died at its first path, a directory, **`panic: out
of memory`**, in the read both functions share; on the **composed** runtime,
both reads answer: the directory FAILED, **`/proc/self/status` OK, 1,108
bytes**, a path through a file and a missing one NOT_FOUND, an empty file OK
and 0 bytes (a mode of 000 read OK: the container runs as root).

And the compiler itself. The route's compiler built twice, against the route's
runtime and against the composed one (`heroes0`, then `heroes build
selfhost/main.hero`, both exit 0), on **defect 236's own shape, a `use`d
module that is a directory**:

| compiler and runtime | `check main.hero` |
|---|---|
| the base compiler, the base runtime | exit **134**, *panic: out of memory* (batch 8's finding, reproduced) |
| the route's compiler, the route's runtime | exit **134**, *panic: out of memory* |
| the route's compiler, the composed runtime | exit 2, *error: cannot read `geom.hero`* (defect 236's answer) |

**The route compiler's own tests**: on the route's runtime **1,133 tests, 1
failed**, `"a module that is there and cannot be read fails loud, as the root
does"`, *panic: out of memory*, the very test batch 8's leg found red; on the
composed runtime **1,133 tests, all passed**.

So the two meet in one function and compose in one way only: the route reads
every source through `hero_file_read_shown`, whose read is the shared
`hero_file_bytes`, so **defects 273's and 274's repairs must land in
`hero_file_bytes`**. Landed where `df453108` and `34836e73` put them, in
`hero_file_read`'s own body, they repair `read_file` for programs and leave
the compiler's every source read, the header keys and the runtime key with the
old read: `git merge-file` leaves exactly that choice as its two conflicts.

### The Windows box (03:08 to 03:09, clang 23.1.1, beside batch 8's leg)

The runtimes, the driver and the run golden sent as one archive (sha256
`5fee3620c2255658` on both sides) to `/c/w/189-fp-win/i3/`; the box's own
compiler from item 1 (`/c/w/189-fp-win/t/heroes.exe`, the same seed);
`driver/win-item3.sh`, output `out-win.txt`. Nothing timed.

| | clang 23.1.1, `x86_64-pc-windows-msvc` |
|---|---|
| `base`, `route`, `composed` under the flag list, `-O0` and `-O2` | exit 0, **0 warnings, 0 errors**, each |
| with `-Wextra -Wshadow -Wconversion` | one warning in all, `parts/failure.c:105`; none in the diff's files |
| the property driver, no sanitizer, `route` and `composed` | 200,000 buffers, 9,503,115 offsets, 20,000 files: **0 mismatches**; 12 named files rebuilt, among them **the box's own `heroes.exe`, 14,395,392 bytes, 9,131,602 bytes of marks** |
| the same with `-fsanitize=address` | 20,000 buffers, 0 mismatches, 11 named files rebuilt, exit 0 |
| the run golden, the box's compiler against each runtime | `route` and `composed`: stdout **equal** to the `.expected` |
| the run golden against the base runtime | **`error[ffi_unknown_name]: hero_os.h declares no hero_file_read_shown`**, exit 1 |
| the read probe, a directory | **route: NOT_FOUND** for both reads (defect 274's Windows shape, its commit saying Windows' `fopen` of a directory fails EACCES; now in the compiler's source read); **composed: FAILED** |
| a path through a file, a missing one; a read-only file; an empty one | NOT_FOUND; OK; OK, 0 bytes; the same on both runtimes |

### The condition from item 2, checked by running it (this Mac, the route's whole compiler)

The route's compiler built from `fullroute/` (the lane's seed, then `heroes
build selfhost/main.hero` against the route's runtime: `heroes-route`).

**No replaced text reaches a writer: held.** A root holding `0xE9` in a
comment and a `certain`-fixable mistake (`Point(x: 1, z: 2)`, the rename to
`y`): `check --apply`, `check --apply --in-place`, `fmt`, `fmt --in-place`
each **exit 1, `error[not_text]`, stdout empty, the file unchanged** (sha256
the same after all four); the same program in ASCII, `check --apply
--in-place`: *rewrote*, `z:` to `y:`.

**A replaced text does reach a digest, with its marks, and the key is exact
for every byte edit: held in substance.** `driver/keycase.sh`, a header whose
`static inline` function returns the byte of a one-byte string literal:

| step | the route's compiler | the base compiler |
|---|---|---|
| 1. the literal holds `0xE8` | builds, prints 232 | builds, prints 232 |
| 2. nothing edited, built again | **0 unit objects rewritten** | 1 rewritten (the `absent` key) |
| 3. only that byte edited to `0xE9` | **1 rewritten, prints 233** | 1 rewritten, prints 233 |

So my item-2 prediction 2 is answered for the route as built: its key is not
a lossy text, and a one-byte edit inside a literal rebuilds.

**But the key is not one-to-one, and a stale object follows from it.**
`reading.key_of` keys a file that is not UTF-8 as `"not UTF-8, <n> bytes of
marks:\n"` + marks + text, and a file that is UTF-8 as its own text, so a
UTF-8 file whose bytes ARE another file's key text gets that file's key.
Step 4 (a header with `0xE8` in a comment, `answer` returning 4) then step 5,
the header replaced by valid UTF-8 that is not C, the line `not UTF-8, 2
bytes of marks:` followed by `E8` and the step-4 text with its U+FFFD:

| step 5 | the route's compiler | the base compiler |
|---|---|---|
| the header no longer compiles | **`build` exit 0, 0 objects rewritten, the binary prints 4** | `build` exit 1, `error[ffi_unknown_name]: lib.h declares no answer` |

A built file nobody wrote, on purpose, and so a hardening rather than a
danger in practice; but it falsifies `key_of`'s own comment (*"the same for
the same bytes and different for different ones"*), and the same key keys
the runtime (`cli/runtime_key.hero`). Making the two branches unable to meet
costs a few lines (a row the text branch can never produce, for instance a
key whose 16 characters are not all hexadecimal); case 5 is its test.

### Where it belongs, from the C side (F3)

**In the runtime, declared in `hero_os.h`, bound only in the compiler's own
groups; `HERO_RUNTIME_ABI` stays 26**, by design.md §4.20's own rule
(*adding a function to this ABI is self-guarding, an undefined symbol at
link, while adding a struct field is not*). Measured, not quoted, and
earlier than the link §4.20 names: a runtime
older than the function is a compile error naming it, from a program
(`ffi_unknown_name`, this Mac and the box) and from the compiler's own group
(`selfhost/module/reading.hero:27:5`, this Mac); no declaration that existed
changed shape (`hero_utf8_valid` and `hero_file_read` keep theirs); the
`static` judge is reached by include order, and a reorder is an error, never
a silent answer. Nothing about it is the compiler's alone: it is a file read,
the runtime's job, and the net and one run golden already bind it through
the same header. Not re-run by me: the compiler-engineer's `emission` whole,
744 and 0, the evidence that no program's emitted C moved.

What the shown read costs in memory, by arithmetic from the code and so an
inference: the marks and the text are at most five bytes per byte of the
file (two of marks and three of U+FFFD), held twice at the copy into a `str`;
measured, the driver's whole process on the 8.4 MB binary peaked at 68 MB of
resident memory (`/usr/bin/time -l`), its own copies included.

## Item 3 in the charter's form (written 03:10)

- `verdict`: **approve**, on two conditions of landing. The C is sound on
  every clang and platform this project runs, and it changes no value that
  crosses to C. **No veto.**
- `section`: design.md §1.12 (robustness; the read must not die on a
  directory, and a key must not keep a stale object), §4.19 (the header
  check as the version guard), §4.20 (*adding a function to this ABI is
  self-guarding ... while adding a struct field is not*, and *a primitive
  that computes a length guards it before allocating*).
- `experiment`: `route-runtime.diff` applied by `patch` to `6ee963e7`, and a
  composition with batch 8's `df453108` and `34836e73`; compiled under the
  sixteen-word flag list at `-O0` and `-O2` on Apple clang 21.0.0, Debian
  clang 22.1.8 and 18.1.8 (arm64) and clang 23.1.1 (the box): **0 warnings, 0
  errors, every one**; a C driver including the runtime, checking the
  judge against Table 3-7 at 9,503,115 offsets and rebuilding every file from
  its marks, under ASan, UBSan and LeakSanitizer where they exist: **0
  mismatches on three platforms**; the run golden equal to its `.expected` on
  all four clangs; the route's whole compiler built on this Mac and on Linux.
- `argument`: The diff adds a function and splits a read; no declaration
  changes shape, and a runtime without the function is a compile error
  naming it, so the ABI number need not move and the compiler's own groups
  are the right place. Its marks are lossless: every file, a 14 MB
  executable included, comes back byte for byte. Two things break at
  landing, both measured. Batch 8's two repairs sit in `hero_file_read`'s
  body, and the route reads every source through the shared
  `hero_file_bytes`: merged where they stand, Linux arm64 fails defect 236's
  test, *out of memory*. And the header key has two branches one file can
  cross: a stale object, found by a crafted header.
- `prediction`: at batch 8's gate, the round that lands 227, **if
  `hero_file_bytes` keeps the route's own read, the Linux arm64 leg reads the
  compiler's own tests with exactly one failure, `"a module that is there and
  cannot be read fails loud, as the root does"`, *panic: out of memory*, on
  clang 22.1.8 and on 18.1.8; with 273's and 274's read in `hero_file_bytes`,
  it reads all passed** (measured on my composition: 1,133 and 1 against
  1,133 and 0, clang 22.1.8).
- `condition`: **object** if the landing leaves 273 and 274 outside
  `hero_file_bytes`, or leaves `key_of`'s two branches able to meet (case 5
  above, then its test); **veto** only on a measured platform where the
  driver's rebuild fails or the judge disagrees with Table 3-7, which none of
  the three did.

## Not run in item 3

The route's whole compiler on the box (its own tests there are batch 8's
leg's to run, and it was running); `records`; any timing; any paid run. The
`emission` count is the compiler-engineer's. Nothing removed anywhere; my
folders are `<scratchpad>/189-ffi-pragmatist-item3/` and
`/c/w/189-fp-win/i3/`.
