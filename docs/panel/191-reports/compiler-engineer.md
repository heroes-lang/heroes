# Panel 191, the compiler-engineer's report: how the runtime reaches a name on Windows (defect 238)

Opened 2026-10-04 14:45 by `date`; written as I go. Status line changed at
15:59: **closed**; the last section, *Resumed at 15:52*, holds what ran after
the session limit and the verdicts' amendments.

## What I work from

- **The briefs**, read whole at 14:42: `00-shared.md` and
  `compiler-engineer.md`, both as repaired after the critic's first pass, and
  the critic's report `191-reports/completeness-critic-briefs.md`.
- **My copy**, `<scratchpad>/191-compiler-engineer/`, made at 14:43 by
  `git -C <trunk> archive 7f4c0cc5 | tar -x`. Its seed's sha256 begins
  `26ccaa9d96478a20` (`shasum -a 256 seed/heroes.c`), as the brief says. The
  compiler built from it by `clang -I runtime seed/heroes.c runtime/runtime.c
  -o heroes` between 14:43:21 and 14:43:25 prints `heroes 0.2.0`.
- **My scratch work** is `<scratchpad>/191-ce-work/`.

## The Windows view, re-run before pricing

`unifdef -D_WIN32 -U__APPLE__ -U__linux__` over `runtime/*.c`, `runtime/*.h`
and `runtime/parts/*.c` into `<scratchpad>/191-ce-work/winview/`, then the
`...A` names found from the world with
`grep -o -E '\b[A-Z][A-Za-z0-9]*[a-z0-9]A[[:space:]]*\('` and the CRT's
narrow name calls by word boundary, comment lines filtered. It reproduces the
brief's **40 lines**: `replace.c` 20, `fs.c` 10, `run.c` 5, `os.c` 3,
`dir.c` 2. The second `GetModuleFileNameA` the raw grep returns is inside a
comment, as is `str.c:416`'s `getenv`. The source lines:

- `replace.c` 106, 129, 136, 201, 238, 267, 501, 529, 533, 540, 599, 628,
  663, 684, 691, 705, 724, 770, 803, 804;
- `fs.c` 48, 61, 72, 126, 127, 162, 177, 186, 220, 261;
- `run.c` 570, 574, 604, 623 (the constant `"NUL"`), 687;
- `os.c` 747, 847, 857;
- `dir.c` 213, 248.

Two structures move with them under route (b): `WIN32_FIND_DATAA`
(`dir.c:212`, `replace.c:105`) and `STARTUPINFOA` (`run.c:628`). And one door
the count cannot see because it is not a call: `main`'s `argv`, kept by
`hero_args_set` (`os.c:496-517`) and lent by `hero_args_raw`
(`os.c:554-558`).

## Q-b first: which linker, measured (14:45 to 14:55)

**On the box** (clang 23.1.1, `InstalledDir: C:\Program Files\LLVM\bin`, no
`.cfg` file beside it, `ls "C:\Program Files\LLVM\bin"`):

- `clang -### t.c -o t.exe` runs **`lld-link`**, with `-defaultlib:libcmt`.
  So the box's default linker is lld-link **without** `-fuse-ld=lld`.
- `clang -### -fuse-ld=link t.c` runs MSVC's `link.exe` from BuildTools
  14.44.35207 (`...\bin\Hostx64\x64\link.exe`, with `cvtres.exe` beside it).
- `where link.exe` finds only `C:\Program Files\Git\usr\bin\link.exe`,
  coreutils' `link`. `rc.exe` and `mt.exe` exist under `Windows Kits\10\bin\
  10.0.26100.0\x64\` and are **not on PATH**; `llvm-rc.exe` and
  `llvm-mt.exe` are, in `LLVM\bin`.

**On this Mac**, re-run: Homebrew clang 22.1.8 `--target=x86_64-pc-windows-msvc
-###` runs `link.exe`, as the critic measured. So the two clangs I can reach
disagree on the default. **The CI's clang 20.1.8 I cannot run** (a push or a
dispatch): what stands is the critic's reading of run 37196853219's log, and
`selfhost/cli/flags.hero:181-191`, which records `link.exe`'s incremental
message on that leg on 2026-08-31. A user's default is whichever their clang
was built with: **a route must work under both.**

## The carriers of route (a), each under both linkers on the box

Probe: `<scratchpad>/191-ce-work/probe/` (`acp.c` prints `GetACP()`;
`link.sh`, `obj.sh`), run in `/c/w/191-ce-win/probe/`. The manifest is panel
189's `utf8.manifest`, 8 lines and 368 bytes by `wc`.

| carrier | lld-link 23.1.1 (default) | link.exe 14.44 (`-fuse-ld=link`) |
|---|---|---|
| none | exit 0, `GetACP 1252` | exit 0, `GetACP 1252` |
| **(a′)** `-Wl,/MANIFEST:EMBED -Wl,/MANIFESTINPUT:utf8.manifest` | exit 0, `GetACP 65001` | **refused: `LNK1158: cannot run 'mt.exe'`** |
| (a′) plus `-Wl,/MANIFESTUAC:NO` | exit 0, `65001` | **refused, `LNK1158`** |
| **(a1)** `utf8.res` by the box's `llvm-rc` 23 | exit 0, `65001` | exit 0, `65001` |
| **(a″)** the same 432 bytes made on this Mac by `llvm-rc` 22 | exit 0, `65001` | exit 0, `65001` |
| **(a⁗), new**: the manifest INSIDE an object, see below | exit 0, `65001` | exit 0, `65001` |

- **(a′) is closed by measurement**: link.exe embeds an input manifest only
  through `mt.exe`, which is on no PATH a user gets outside a Developer
  Prompt. The critic's lld-link 22 result stands, and lld-link 23 agrees.
- The `.res` made by `llvm-rc` 23 on the box and by `llvm-rc` 22 on this Mac
  are **byte-identical** (`cmp`), 432 bytes, sha256 `178d9a7f2a47c56c`.
- link.exe through clang leaves no side-by-side `.manifest` file (`ls`).

**What each carrier does beside a second resource** (a string table compiled
by `llvm-rc` to `other.res`, 108 bytes):

| link | lld-link | link.exe |
|---|---|---|
| `utf8.res other.res` | exit 0, 2 resources, `65001` | exit 0, 2 resources, `65001` |
| `utf8.res` twice | refused, `duplicate resource: type MANIFEST ...` | refused, `CVT1100: duplicate resource` |
| (a⁗)'s object and `other.res` | **refused**, `more than one resource obj file not allowed` | **refused**, `LNK1241: resource file rsrc.obj already specified` |
| (a⁗)'s object and `-Wl,/MANIFEST:EMBED` | refused, the same | refused, `LNK1158: cannot run 'rc.exe'` |

Resources read back by `llvm-readobj --coff-resources` on each `.exe`.

### (a⁗), a carrier nobody listed: the manifest inside the runtime's object

Found by asking what would have to be true for the manifest to reach the
links `heroes` does not write (Q-d). Every Heroes program links the runtime,
since its generated `main` calls `hero_args_set` first
(`selfhost/emit/decls.hero:207-208`, `:242-243`), which only the runtime
defines; and a COFF object may carry a resource section:
that is all `cvtres` makes. So the runtime's C can carry it, in
`<scratchpad>/191-ce-work/probe/rsrc.c`'s shape:

- the XML as a `const char[]` in section `.rsrc$02`, `__attribute__((section,
  used))`;
- the resource tree, type 24, name 1, language 1033, as file-scope
  `__asm__` in section `.rsrc$01`: 88 bytes, its one image-relative address
  written `.rva`, the relocation the object shows (`llvm-readobj --sections`:
  one relocation on `.rsrc$01`).

Measured: it links alone under both linkers, `GetACP 65001`, and its tree
reads back as the `.res`'s does, one MANIFEST, ID 1, language 1033, 368 bytes.

**Its price is the refusal in the table above**: a program carrying it can
link no other resource at all, an icon or a version block, under either
linker. Loud, never silent: both linkers stop and name the object. Whether a
Heroes program can bring a resource to its link today is the question that
prices this; I answer it below.

Answered by reading the compiler (written at 15:13): **no, not through
`heroes build`.** A package's link words are filtered to `-L`, `-l`, `-Wl,` and
`-F` (`selfhost/cli/libraries.hero:193`, the refusal at `:205` naming what is
accepted), and a `link` name becomes `-l<name>`, a library. So the refusal
falls only on an `--emit-c` author who links a resource by their own line,
and on any future feature that gives a program an icon.

## Route (a⁗) with (d), built and run on the box (15:00 to 15:13)

**What I built** (`<scratchpad>/191-ce-work/ra/`, a second `git archive
7f4c0cc5` extract), three files, all in `runtime/`:

- `runtime/parts/codepage.c`, **new, 81 lines** by `wc -l` (82 when first
  counted, before one unverified sentence left its comment): the XML, the
  88-byte tree, and (d)'s check `hero_codepage_require_utf8`, empty off
  Windows;
- `runtime/runtime.c`, **+5**: the part included before `os.c`;
- `runtime/parts/os.c`, **+4**: `hero_args_set` calls the check after the two
  stream calls and before the stack guard.

No line of `selfhost/`, no line of `seed/heroes.c`, no link line. On this Mac
the route's runtime compiles under `-Wall -Werror -std=gnu11`, and the seed
builds with it (`heroes 0.2.0`). On the box, the route's `runtime.c` compiles
**with no warning under `flags()`'s sixteen** and under `-std=c11
-pedantic-errors` (the base's also does), and its object holds `.rsrc$01`, 88
bytes, and `.rsrc$02`, 369 (the XML and its NUL), by `llvm-readobj --sections`.

**The compiler, built by the UNCHANGED seed line** on the box (15:04):
`clang -I runtime seed/heroes.c runtime/runtime.c -Wl,/STACK:67108864 -o
heroes.exe` carries one MANIFEST resource, 368 bytes (`llvm-readobj
--coff-resources`). So does the CI's plain line without the stack flag, and so
does the same line under `-fuse-ld=link`. The base binary built the same way
has no resource section. **So Q-d's three lines need no change under this
carrier**: they compile `runtime/runtime.c`, and the manifest is in it.

**Exported** for the ffi-pragmatist at `<scratchpad>/191-shared/a-object/`
(15:13): `a-object.diff` (`diff -ruN` of `runtime/` against a pristine
extract, 114 lines), `codepage.c`, the box's compiler `heroes-a-object.exe`,
sha256 `c61e7c704f35e6db18ea5d695947179b32b1809198d47afbebb498ac4ff49c11`
(the same on both machines), this Mac's build, sha256 `603387833e66b849...`,
and a `README.txt`. The manifest travels in `runtime.c`, not in the compiler:
a program built against a pristine `runtime/` carries none.

### Q1-a and the rows: red at `7f4c0cc5`, then through the route

**Names made from the wide world, never by a shell.** A tool of mine,
`wtool.c` (129 lines, `<scratchpad>/191-ce-work/rows/`), takes ASCII with
`\uXXXX` escapes and makes files and folders by `CreateFileW` and
`CreateDirectoryW`, lists by `FindFirstFileW`, hashes a file opened by a wide
name, and starts a program by `CreateProcessW` with a wide command line, a
wide working folder and one wide environment value, its two streams to files.
So no launcher's code page touches a name (Q-k, the launchers, is the
ffi-pragmatist's). The script is `rows.sh`; both runs are kept as
`out-base.txt` and `out-ra.txt` beside it. Run 15:08 (base) and 15:09
(route), each compiler from its own folder, the runtime found beside it.

| row | base, `7f4c0cc5` | route (a⁗)+(d) |
|---|---|---|
| 1 `check café.hero` | **exit 2**, *argument 2 is not UTF-8* | exit 0 |
| 2a `check ŝ.hero` (an error) beside `s.hero` (clean) | **exit 0: `s.hero` judged** | exit 1, `unknown_name` at `ŝ.hero:2:11` |
| 2b `fmt ŝ.hero --in-place`, both unformatted | **`s.hero` rewritten** (31 to 29 bytes), `ŝ.hero` untouched | `ŝ.hero` rewritten (31 to 29), `s.hero` untouched |
| 3 `args_checked()` over `a`, `ŝ`, `café` | `a`, **`s`**, **refused** (*not UTF-8*) | `a`, `c5 9d`, `63 61 66 c3 a9` |
| 4 `read_file("café.txt")`, the file made wide | **`file_not_found`** | `read ok hello` |
| 5 `write_file("café-out.txt")`, the name on disk | **`cafÃ©-out.txt`** (`Ã©`) | `café-out.txt` |
| 6 `check dé/p.hero` | **exit 2**, *not UTF-8* | exit 0 |
| 7a `probe` and `mutate` over a folder holding `café.hero` | **exit 2**, *`walk1/caf<0xE9>.hero` is not UTF-8: rename it* | exit 0, both |
| 7b `probe` over a folder holding `ŝ.hero` | **exit 2**, *cannot read `walk2/s.hero`* | exit 0 |
| 8 `doctor` with `HEROES_RUNTIME` under `rté` | **runtime FAIL**, *`rt<0xE9>` is not UTF-8* | `ok runtime ...rows-ra/rté` |
| 9 `build café.hero -o café.exe`, then run it | **exit 2**, *not UTF-8* | exit 0, prints `1` |
| 10 a project root above ASCII, the compiler's working folder inside it: `build main.hero` (two modules), run | exit 0, `42` | exit 0, `42` |
| 10 the same, every name absolute (`#line`, `-o`, the publish) | **exit 2**, *not UTF-8* | exit 0, `42` |
| 10 `fmt fmtme.hero --in-place` there | formatted | formatted |
| 11 the compiler copied under `inst-é` with its runtime, no `HEROES_RUNTIME` | **`doctor`: runtime not found; `build`: cannot find the runtime** | `doctor`: `ok runtime ...inst-é/runtime`; `build` and run, `hello` |
| 12 an ASCII program, built and run | `hello`, 6 bytes, FNV `b7cbe5cf7d4d4791` | **the same bytes** |

**Every row is red at `7f4c0cc5` and green through the route**, but one: a
project root above ASCII is **green at the base** while every name the
compiler is given is relative. The working folder is never spelled, so no
narrow door sees it. It is red the moment one name is absolute, which is how
an editor or a build tool calls a compiler.

`doctor` exits 2 in rows 8 and 11 under both compilers, for its `FAIL cc not
found` line on Windows, which is outside this sitting.

**Two rows are not repaired by (a), and the first is Q-c.**

- **A lone surrogate, `x<D800>y`, as an argument**: the base gives `x?y`,
  the route `x` U+FFFD `y` (`ef bf bd`). Both are valid UTF-8 and both are
  wrong values, so **`spec/heroes-spec.md:324-325`'s *one that is not UTF-8
  aborts* is already false at `7f4c0cc5` for this argument**, and (a) does not
  make it true: it moves the wrong value from `?` to U+FFFD. Under (b) the
  runtime chooses; under (a) Windows does. By the brief's rule, that half
  goes to panel 192.
- **A lone surrogate in a file name**, `walk3/caf<D800>.hero`: the base says
  *cannot read `walk3/caf?.hero`*, the route *cannot read
  `walk3/caf<U+FFFD>.hero`*. Loud in both, and false in both about which name
  is there. Defect 239's *not UTF-8: rename it* is what it should say, and
  only a wide walk can see it.

### What the route moves among the compiler's own tests on Windows (15:13 to 15:15)

`heroes.exe test selfhost/main.hero` with the route's compiler, on the box:
**1,190 tests, 1 failed**, in two minutes. The one is the case I predicted
from its own comment, before running it: `selfhost/cli/process.hero:387`, *a
file whose name is not UTF-8 is named by its bytes, and the walk never panics
(defect 239)*, at `assert walked.files == [dir + "/only/plain.hero"]`.

Its premise, in its comment, is the narrow world's self-consistency: *Windows'
narrow `fopen` makes `café.hero` and its `FindFirstFileA` answers 0xE9 again in
the ANSI code page*. Its maker is a plain C program, so no manifest, code page
1252: it makes `café.hero`, U+00E9, a valid name. The route's walk now reads
that name as the UTF-8 it is, so the walk holds two files. **The case was
testing 238's own defect as if it were the platform.** Batch 9's leg had it
green on the same runtime (`38d6c6b1`). It is owed a Windows branch: on NTFS
the only name that is not UTF-8 is one holding a lone surrogate, which a maker
makes with `CreateFileW` (row 7c above, `wtool mk "caf\ud800.hero"`).

Its twin in the net's own tests, `tests/harness/shell.hero:987`, rests on the
same sentence. Run below.

## Route (b)'s argument door, Q-h, measured, and route (g) on a run (15:15)

Probe `<scratchpad>/191-ce-work/argdoor/argdoor.c`, one program printing
three readings of its own command line: `main`'s narrow `argv`;
`CommandLineToArgvW(GetCommandLineW())`; and the UCRT's own
`_configure_wide_argv(_crt_argv_unexpanded_arguments)`, which fills `__wargv`
with **the same parser `main`'s `argv` came from**, declared in
`<corecrt_startup.h>`. Each raw command line handed over by `CreateProcessW`
as is (`wtool runraw`). Built plain and with the manifest. Output kept as
`argdoor/out.txt`.

| command line after the program | narrow `argv`, no manifest | narrow `argv`, manifest | `CommandLineToArgvW` | UCRT `__wargv` |
|---|---|---|---|---|
| `a ŝ café x<D800>y` | `a` **`s`** **`caf\xe9`** **`x?y`** | `a` `ŝ` `café` **`x<U+FFFD>y`** | exact, `\ud800` kept | exact, `\ud800` kept |
| `"a""b" c` | `[a"b] [c]` | `[a"b] [c]` | **`[a"b c]`**, one word | `[a"b] [c]` |
| `"a\"b" a\\\"b` | `[a"b] [a\"b]` | the same | the same | the same |
| `"" x`, `"ab"c d`, `a\\\\"b c"` | as the CRT | the same | the same | the same |
| `＂a b＂ c` (U+FF02) | **`[a b] [c]`**: best fit made two words one | `[＂a] [b＂] [c]` | `[＂a] [b＂] [c]` | `[＂a] [b＂] [c]` |

What it settles:

- **`CommandLineToArgvW` is refused by measurement**: on `"a""b" c` its split
  is not the CRT's, so an ASCII program's `args()` would move. That is beside
  its `shell32` link and its `LocalAlloc`, the brief's two.
- **A door nobody listed, the UCRT's own wide argv**, agrees with the CRT's
  split on every line by construction, keeps a lone surrogate so the runtime
  can refuse it, needs no `shell32`, no `wmain` and no parser of ours. Its
  wide words are the CRT's memory, as `main`'s `argv` already is. Its price:
  it is the UCRT's startup entry point, declared in `<corecrt_startup.h>`
  (the probe compiled and linked against it on the box); whether Microsoft
  documents it for a program's own use I did not read, and I have not run it
  under a C library other than the UCRT, which this project does not target.
- **Q-f for arguments** (the ffi-pragmatist's question, measured here in
  passing): with no manifest, best fit turned the fullwidth quotes into ASCII
  ones and **two arguments became one**. The manifest closes it, as every wide
  form does.
- **Q-c**: under the manifest, a lone surrogate reaches `argv` as U+FFFD,
  valid UTF-8, so `args()` cannot abort on it; only a wide form keeps it.

**Route (g), the UCRT's `.UTF8` locale, on a run** (the plain build, no
manifest; `café.txt` made by `CreateFileW`; `PROBE_VALUE=café` set wide):

- `fopen` of the UTF-8 name: failed under the C locale; `setlocale(LC_CTYPE,
  ".UTF8")` answers `English_United States.utf8`, and then **`fopen`
  opened it**. So the recollection holds: the UCRT reads a narrow name as
  UTF-8 under that locale.
- **`CreateFileA` of the same name, same locale: failed.** The Win32 `...A`
  calls do not read the C locale, and they are 35 of the 40 doors.
- **`getenv` under it: `caf\xe9`**, code page 1252's byte. The narrow
  environment does not move.
- `argv` was built before `main` (the narrow column above).
- **After a library's `setlocale(LC_ALL, "")`, `fopen` failed again.**

So **(g) is refused on a measurement**: of the 40 doors it can reach only the
CRT's five name calls, `fopen` (run, reached) and `remove` and `_mkdir`
(the same C library's conversion by my inference, unrun); not the 35 Win32
lines, not `getenv`, not `argv`. And the first C library that sets its own
locale undoes it in silence, as `runtime/parts/f64.c:47-52` already argues
against process-wide locale state.

## Q-g through my route: (a) alone introduces a silent truncation (15:19 to 15:20)

`WIN32_FIND_DATAA.cFileName` is `CHAR cFileName[MAX_PATH]` and `MAX_PATH` is
260, read from the box's SDK (`minwinbase.h:111`, `minwindef.h:60`). A name
of 100 × U+6F22 is 100 UTF-16 units on disk and **300 bytes of UTF-8**.

- `probe walk4`, a folder holding that name and nothing else: the base says
  *cannot read `walk4/???...?.hero`* (best fit, loud and wrong); **the route
  says *no `.hero` file under walk4*, which is false.**
- `check <that name>`, given directly: base, the `???` refusal; route, exit
  0. A name going IN is converted to UTF-16, whose length is what Windows
  limits, so input doors are not where it breaks.
- **What `FindNextFileA` does with it, under the UTF-8 code page**
  (`<scratchpad>/191-ce-work/rows/nlist.c`, linked with the manifest; a folder
  of `a b c y z .hero` and the long name between `c` and `y`, listed wide
  first): it answers `.`, `..`, `a`, `b`, `c` and then **ends the listing,
  last error 234 (`ERROR_MORE_DATA`)**. `y.hero` and `z.hero` are never
  seen. The route's `probe` over that folder reports **3 seeds of 6**, exit 0.

**So route (a) makes a wrong answer the base did not give**: a directory walk
that stops at the first name whose UTF-8 passes 259 bytes and says it is
complete. `runtime/parts/dir.c:248`, `} while (FindNextFileA(search,
&found));`, does not tell the end of a listing (`ERROR_NO_MORE_FILES`) from a
failure. It reaches every walk: `probe`, `mutate`, the harness's, a build's
`files_under`, and `hero_dir_remove_tree`. And `replace.c:104-111`'s
`hero_fs_is_surrogate`, which reads a file's own reparse tag by
`FindFirstFileA` on its own name and answers *not a link* when that fails,
would let `fmt --in-place` replace a symbolic link with such a name by a
plain file, defect 136's class (by reading, unrun).

**It is closed in two sizes**, both priced below: the end of a listing told
from a failure (about 5 lines, and the walk then refuses loudly where it
cannot list), or **the directory door wide**, `FindFirstFileW` and
`FindNextFileW` at `dir.c:213`, `:248` and `replace.c:106`, which lists the
name, and also names a lone-surrogate name *not UTF-8* as defect 239 meant
instead of *cannot read `caf<U+FFFD>.hero`*.

## The directory door made wide, built and run (15:25 to 15:36)

Built far enough to compare, as the brief allows for route (b), and because
Q-g makes it the door that decides: **(a⁗)+(d) and three narrow lines of
(b)**, `dir.c:213`, `:248` and `replace.c:106`, in a third tree
(`<scratchpad>/191-ce-work/rc/`).

- `codepage.c` gains two helpers, **+53 non-blank lines (81 to 137)**:
  `hero_win_wide`, a UTF-8 name to UTF-16 in scratch memory
  (`MultiByteToWideChar` with `MB_ERR_INVALID_CHARS`, so a C string that is
  not UTF-8 fails the door instead of guessing), and `hero_win_name_bytes`,
  UTF-16 back to UTF-8 with a lone surrogate kept as its three WTF-8 bytes.
- `dir.c` **+13 −7**: `FindFirstFileW`/`FindNextFileW`, the name carried
  back, and **the end of a listing told from a failure**: `GetLastError() !=
  ERROR_NO_MORE_FILES` after the loop fails the walk.
- `replace.c` **+7 −2**: `hero_fs_is_surrogate` asks `FindFirstFileW`.

On the box (15:29 to 15:30): its runtime compiles with no line under
`flags()` and under `-std=c11 -pedantic-errors`, and the compiler builds from
the unchanged seed. The rows (`out-rc.txt`) read **line for line as route
(a⁗)'s but one**, and the three walk shapes:

| shape | base | (a⁗)+(d) | with the wide door |
|---|---|---|---|
| `probe walk3`, a lone-surrogate name | *cannot read `caf?.hero`* | *cannot read `caf<U+FFFD>.hero`* | **defect 239's own message**: *the name of `walk3/caf<0xED><0xA0><0x80>.hero` is not UTF-8, ... rename it* |
| `probe walk4`, 100 × U+6F22 | *cannot read `???...`* | ***no `.hero` file under walk4*** | **1 seed** |
| `probe walk5`, six names, the long one fourth | (not run) | ***3 seeds***, exit 0 | **6 seeds** |

Exported at `<scratchpad>/191-shared/c-dirwide/`: `c-dirwide.diff` against a
pristine extract, 232 lines, and `c-dirwide-over-a-object.diff`, 135. **Its
compiler did not arrive**: the box went down during the copy (below), and
`README.txt` there says how to build it from the diff.

## Elsewhere: Linux and macOS, run, not assumed (15:17 to 15:32)

Route (a⁗)+(d) off Windows is one call to an empty function, and the run says
so:

- **This Mac**, the route's tree: the compiler's own tests **1,190, all
  passed**; the seed's fixpoint through the route's compiler **byte-identical**
  (`cmp`); `emission` **754 passed, 0 failed**; `wholes` **361, 0**;
  `descriptors` **361, 0**; `runtime` (the allocation and shared-state rules
  over every file of `runtime/`) **8, 0**.
- **Linux arm64**, the project's `heroes-linux-arm64` image, Debian clang
  22.1.8, aarch64: the seed built inside, the compiler's own tests **1,190,
  all passed**, the fixpoint **byte-identical**, `runtime` **8, 0**. Run
  without `--rm` under this sitting's *never `rm`*: the stopped container
  `p191-ce-linux-route` is left for the author to remove.
- **The seed does not move under either built route**: neither touches the
  emitter, so `seed/heroes.c` is today's and the fixpoint holds.

## Q2. The cost, file by file

Measured by `diff -u` against the pristine extract, non-blank lines; the
`selfhost/` rooms in `layout`'s unit (`code_lines`: no blank line, no `test`
block), by an `awk` re-implementation of `tests/harness/suite_layout.hero:785`.

| piece | `runtime/` | `selfhost/` | seed | link lines | built? |
|---|---|---|---|---|---|
| **(a⁗)** manifest in the runtime's object | `parts/codepage.c` new; `runtime.c` +4 | **0** | **0** | **0** | yes, run |
| **(d)** the floor refused at start | 13 of `codepage.c`'s 81 lines, the function and its comment; `os.c` +4, its call | 0 | 0 | 0 | yes; the refusal reached by a probe |
| (a⁗)+(d) together | **81 + 4 + 4**, three files | 0 | 0 | 0 | yes |
| **the directory door wide** (3 of the 40 lines) | `codepage.c` +53, `dir.c` +13 −7, `replace.c` +7 −2 | 0 | 0 | 0 | yes, run |
| **(b) whole**, the other 37 lines and two doors | about **+180 to +250**, `os.c`, `fs.c`, `run.c`, `replace.c` | 0 | 0 | 0 | **no: an estimate** |
| (a1) a `.res` by `llvm-rc` at each build | 0 | `link.hero` about +12, a tool found and run; `doctor` +1 check | 0 | seed and CI need `llvm-rc` too | carrier run, wiring not |
| (a″) the compiler writes the 432 bytes | a byte writer in C (a `str` cannot hold `0xFF`): `hero_os.h` grows (batch 9 added three entries there, `7f875977`, `ea5c22ca`, `5e4f2efb`, and moved `HERO_RUNTIME_ABI` 0 times) | `link.hero` about +12 and a binding, so **the seed is regenerated** | regenerated | **the seed line still has no compiler to write it** | carrier run, wiring not |
| (a‴) a `.res` tracked in `runtime/` | one binary file, the first outside `site/` (21 today, all `site/`) | `link.hero` about +12 | 0 | `seed/README.md:17` +1 word; `ci.yml:429`, `:471` +1 word each (outward, owes the site's build) | carrier run, wiring not |
| (a′) the XML to the linker | 0 | `flags.hero` +2 words | 0 | +2 words each | **refused: `LNK1158`** |

The estimate for (b) whole is an inference from the measured price of a door:
the three built doors cost +20 −9 outside the 53 lines of shared helpers, so
about 5 a door, over 37 more lines and the argument and environment doors
(about 25 and 12, below), with wrappers shared between `fs.c` and
`replace.c`. Not built, so not a measurement.

**The rooms the `selfhost/` routes would spend**, in `layout`'s unit:
`cli/link.hero` **213 of 300**, `cli/flags.hero` 211, `cli/toolchain.hero`
262, `cli/doctor.hero` 182, `emit/decls.hero` 245. No route here passes a
ceiling. `layout` does not count `runtime/`: `os.c` is 945 lines, `run.c`
897 and `replace.c` 841 (`wc -l`), and the module-shape rule binds them by
judgement only.

**Is it core or sugar?** Neither. No route adds a construct the checker, the
lowering or the backend must handle; (a⁗) is not even seen by the emitter.
What every route but (a⁗) adds is a **platform axis**: a second link input
on Windows, which `link_flags()` keeps today to one question asked of the
machine (`flags.hero:162-167`).

### Q-d, the links `heroes` does not write

| link | (a⁗) | a `.res` route, (a1) (a″) (a‴) | (b) alone |
|---|---|---|---|
| `heroes build` | carries it | `link_line` names it | runtime doors exact; a bound library's narrow calls in code page 1252 |
| the seed's, `seed/README.md:17` | **carries it, measured** (`llvm-readobj`) | +1 word, or the compiler is 238's | the same as above |
| the CI's, `ci.yml:428-429` | **carries it, measured** (the same line) | +1 word, outward, the site's build | the same |
| the CI's plain line, `ci.yml:471` | **carries it, measured** | +1 word | the same |
| `--emit-c` and the author's own line | **carries it, measured**: `ŝ`, `café` read right; the same C against the base runtime reads `s` and refuses `café` | the author must name it; forgotten, (d) refuses at start | the same |

Under (a⁗) none of the four changes, so `ci.yml` and the site's build are not
touched. **The CI's half of the question stays unrun**: whether the CI's clang
20.1.8, with whichever linker it defaults to, takes the `.rsrc` object. No
`llvm-rc` is needed by (a⁗), so that half of Q2 dissolves; the half that
stays needs a push or a `workflow_dispatch`, an outward act this seat did not
take. Its size: one Windows leg, `only: windows`.

### Q-h, route (b)'s argument door, in each form

| form | what it costs | measured |
|---|---|---|
| `CommandLineToArgvW` | `#pragma comment(lib, "shell32.lib")`, `LocalFree`, about 20 lines | **its split is not the CRT's** on `"a""b" c` (one word, not two): an ASCII program's `args()` moves. Refused. |
| `wmain` | `emit/decls.hero:207-208` and `:242-243`, and **350 emitted files** under `tests/emission` and `tests/golden` that spell `int main(int argc, char **argv)` (`grep -rl`), and the seed's two; `hero_args_set` takes `wchar_t **` | unrun |
| a parser of `GetCommandLineW` of ours | about 60 lines and its own cases, to match the UCRT's rules exactly | unrun |
| **the UCRT's own `__wargv`**, `_configure_wide_argv` | about 25 lines in `hero_args_set`: the wide words to UTF-8 by `hero_win_name_bytes`, a lone surrogate as its WTF-8 bytes, copies for the process's life through `hero_malloc_raw`, which no counter weighs; **no golden moves**, no emitter line, no `shell32` | **the CRT's split on all seven lines**, `ŝ`, `café` and `\ud800` kept |

`hero_args_raw` (`os.c:554-558`) lends `main`'s `argv` today; under any wide
form it lends the runtime's copies.

### Routes (c), (d), (e), (g), priced

- **(c), (a) and (b) both**: (a⁗)+(d) with (b) whole is about 89 + 250 lines
  of `runtime/`. What (b) buys over (a⁗) beyond the directory door: the
  runtime's doors exact below the floor (where (d) refuses anyway), and an
  argument holding a lone surrogate refused as the spec says rather than
  given as U+FFFD. What (a⁗) buys over (b): a bound library's own narrow
  calls (Q-l, the ffi-pragmatist's), and the console's narrow writes. **The
  built middle, (a⁗)+(d)+the directory door, is 153 lines net** (89, then
  +73 −9).
- **(d)** lives in `hero_args_set`, not the generated `main`: there it cost
  **no line of `selfhost/`, no golden and no seed**, and the seed-built
  compiler has it (built from the unchanged seed). In the generated `main` it
  would move the 350 files above. Its message, reached by a probe that
  answers `GetACP()` with 1252 (`<scratchpad>/191-ce-work/dcheck/`): **on
  stderr, nothing on stdout, 257 bytes, none above 0x7F, exit 2** under Git
  Bash, `cmd` (`cmd /c` and a batch file) and PowerShell 5.1. The text names
  the cause and not the remedy, because the remedy's version, *Windows 10
  1903*, is a documentation fact this seat did not run. Below the floor
  itself: **unrun**, no instrument here is older than build 26100.
- **(e), refusing names above ASCII**: at run time it cannot see best fit in
  `argv` (`ŝ` was already `s`) without `GetCommandLineW`, (b)'s door; at each
  file door it would cost a line a door, (b)'s price, to forbid what (a) and
  (b) deliver. At check time it is a new diagnostic, outside this lane.
  Refused: (b)'s cost for less than nothing.
- **(g)**: refused on the run above.
- **(f), leaving it**: every row above is red at `7f4c0cc5`, four of them a
  wrong answer given in silence, exit 0: another file judged (2a), another
  file rewritten (2b), `s` for `ŝ` (3), a file written under another name
  (5). Refused by §1.12.

## The box went away at about 15:30

Tailscale reads `apponfly-vps` **offline, last seen 11 minutes ago** at 15:41,
and still offline at 15:43; every `ssh win` since 15:38 times out. The last
answers I had were at 15:30: the wide-door rows and walks, and the start of
that route's own tests and the net's own tests, which I had started there in
the background. Whether my run, the ffi-pragmatist's or neither took it down I
cannot tell from here, and only the author can start it again (`.claude/rules/
platforms.md`, *the Windows box*). **What this leaves unrun**: the wide-door
route's own tests and net's own tests on Windows; the copy of its compiler
(2,326,528 bytes of about 14.9 MB arrived; I stopped the copy, kept the bytes
renamed `.PARTIAL-DO-NOT-USE` and said so in `191-shared/c-dirwide/README.txt`,
rather than let the command hash a partial file into a note); and three
base-side shapes listed under Q3 below.

## Q3. The instruments: the cases that pin each row

**Why every case needs a name from the wide world.** A Heroes program's names
are narrow at `7f4c0cc5`, and narrow names agree with themselves: defect 239's
two cases make `café.hero` through a narrow C program and read it back
through `FindFirstFileA`, and both were **green at the base** (batch 9's leg,
and my net's own tests at the base, 246 passed) while testing 238's defect as
the platform's behaviour. So a case is red at `7f4c0cc5` only if its names are
made, passed and read back through the wide API. That is what `wtool.c` did
for the rows; the cases carry the same shape inside the test, as 239's cases
already carry a C maker (`selfhost/cli/process.hero:387`,
`tests/harness/shell.hero:987`, `maker_source`).

**No tracked path above ASCII is needed**, and none should be: every name is
spelled in ASCII with `\u` escapes in the C helper's source. `git ls-tree`
reads 0 such paths today and a case written this way keeps it 0, out of
APFS's and git's normalisation questions.

| case | the row | red at `7f4c0cc5` | form | platforms |
|---|---|---|---|---|
| W1 `check café.hero`, launched wide | 1 | **yes**, exit 2 (run 15:08) | the compiler's own tests, a C helper made at test time | Windows; twin on POSIX with the bytes passed as is, green at base there |
| W2 `check ŝ.hero` (an error) beside `s.hero` | 2a | **yes**, exit 0, `s.hero` judged | the same | Windows; twin on POSIX |
| W3 `fmt ŝ.hero --in-place` | 2b | **yes**, `s.hero` rewritten | the same | Windows; twin on POSIX |
| W4 a program's `args_checked()` over `ŝ`, `café` | 3 | **yes**, `s`, refused | the same, the program built by the case | Windows; twin |
| W5 `read_file` of a name made wide | 4 | **yes**, `file_not_found` | the same | Windows; twin with `fopen` |
| W6 `write_file`, the name read back wide | 5 | **yes**, `cafÃ©-out.txt` | the same | Windows; twin |
| W7 `check dé/p.hero` | 6 | **yes**, exit 2 | the same | Windows; twin |
| W8 `probe` and `mutate` over `café.hero`, `probe` over `ŝ.hero` | 7a, 7b | **yes**, exit 2 both | the same | Windows; twin |
| W9 `HEROES_RUNTIME` set wide under `rté` | 8 | **yes**, *not UTF-8* | the same | Windows; twin through `/bin/sh` as `shell.hero:1051` does |
| W10 `build café.hero -o café.exe`, run it | 9 | **yes**, exit 2 | the same | Windows; twin |
| W11 a root above ASCII, every name absolute | 10 | **yes**, exit 2 | the same | Windows; twin |
| W12 the compiler copied under `inst-é` | 11 | **yes**, runtime not found | the same | Windows only (the twin is defect 277's case) |
| W13 a folder of six, a 300-byte UTF-8 name fourth: the walk lists six | Q-g | **unrun at the base** (the box went away); red under (a⁗), 3 of 6; green under the wide door, 6 of 6 | the same | Windows only |
| W14 a lone-surrogate name walked: *not UTF-8: rename it* | Q-c, names | **yes**, *cannot read `caf?.hero`*; still red under (a⁗) | **defect 239's two cases, their Windows branch rewritten** to make this name by `CreateFileW` | Windows; the POSIX branch stands |
| W15 `args_checked()` over `＂a b＂ c` gives three words | Q-f | **yes in C** (`argdoor`, two words); unrun through a Heroes program | the compiler's own tests | Windows only |
| W16 every binary `heroes build` links reads `GetACP()` 65001 | the carrier | **unrun at the base** (it would read 1252) | a `run` golden binding `GetACP` by `extern "windows.h"`, judged where the header is, skipped elsewhere by name (`.claude/rules/platforms.md`) | Windows only |
| W17 (d)'s refusal: stream, bytes, exit 2 | the floor | not applicable at the base (no check); reached by the probe | a compiler's own test compiling `runtime/parts/codepage.c` with `GetACP` replaced, as `dcheck.c` does | Windows only |
| W18 an ASCII program, nothing moves | 12 | n/a: the same bytes at base and route | the existing net and the compiler's own tests | all three |

**The net's own tests a route moves, measured**: under (a⁗)+(d),
`tests/harness/shell.hero:987` (net's own, 246 tests, 1 failed) and
`selfhost/cli/process.hero:387` (compiler's own, 1,190, 1 failed), both
defect 239's, both because their premise was the defect. Nothing else, on the
box. Under the wide door the same two would stay red until rewritten (their
`café.hero` is a valid name), by reading; that run was lost with the box.

**Where they run**: batch 9's box leg ran the compiler's own tests and 21
suites, and **not the net's own tests** (`<scratchpad>/platforms/
windows-b9.sh`, its `SUITES`). The CI's Windows leg runs both on every push:
*the compiler's own tests* **on every event** since 2026-09-03
(`ci.yml:622-633`) and *the net's own tests* on every push (`:643-664`).
(Written first, at 15:43, as *tags and dispatches only* from the comment at
`ci.yml:485-494`, which is the decision before that one; corrected at 15:44
on reading the step itself.) So W1 to W15 belong in the compiler's own tests,
which both Windows legs run; a case in the net's own tests is judged on the
CI's leg alone until the box's leg adds that suite.

## Q4. Platforms and the floor

- **Linux and macOS unchanged, shown**: above, both legs, (a⁗)+(d). The wide
  door's additions are inside `#if defined(_WIN32)`, so off Windows its object
  is (a⁗)'s, by reading; its runtime compiled clean on this Mac under
  `-Wall -Werror -std=gnu11`.
- **Q-j, the floor**: no instrument here is older than build 26100, so
  **below 1903 is unrun** by me. *Windows 10 1903* is carried from panel 189
  as a documentation fact, not fetched in this session. What (d) does is make
  the floor, whatever its true number, a sentence on stderr and exit 2 instead
  of 238's rows read wrong in silence. **It also refuses every Heroes program,
  the compiler included, on any Windows below it**, ASCII programs too: that
  is the price (d) charges, and the only route that charges less is (b) whole.
- **The instrument below the floor** (a Hyper-V-isolated older container): not
  tried; the box was gone before I reached it, and it is the ffi-pragmatist's.

## Q-i, the interior NUL: which sitting owns it

`read_file` and `write_file` cross as `path.cstr()`
(`selfhost/library_source.hero:208`, `:222`), a `const char *`. **No door of
any route here can refuse it**: the narrow call stops at the NUL, and so does
`MultiByteToWideChar` with a length of -1, which is how `hero_win_wide`
measures. Only the library, which holds the `str` and its length, can refuse
a path holding a NUL before it lends the pointer, and that is a ruling on
what a `str` may hold and what `cstr()` promises. **Panel 192 owns it**, on
every platform; this sitting's routes neither help nor harm it.

## Verdicts, in the charter's form

Written 15:45:25 (`date`). Each argument counted under 120 words with
`wc -w` (`<scratchpad>/191-ce-work/verdict/`: 116, 109, 117, 110).

### Q1. Soundness, on the box

- `verdict`: **object** to route (a) alone, in any carrier; **approve**
  (a⁗)+(d) **with the directory door wide** as sound on every row and shape I
  ran.
- `section`: design.md §1.12 (`docs/design/design.md:575-579`, *a guarantee
  that ends quietly is not one*), and §1.11 (`:467`) for why the manifest's
  reach over a bound library's own narrow calls is worth keeping.
- `implementation_cost`: (a⁗)+(d) 89 lines of `runtime/` (`parts/codepage.c`
  81 new, `runtime.c` +4, `os.c` +4); the directory door +73 −9
  (`codepage.c` +53, `dir.c` +13 −7, `replace.c` +7 −2). Nothing elsewhere.
- `needed_for_self_hosting`: no. The compiler builds itself on Windows over
  ASCII paths today (batch 9's leg); this is §1.12's, which CLAUDE.md §
  Precedence ranks above Principle 0, and it adds no form to the language.
- `argument`: Every row is red at `7f4c0cc5` with names made wide, and green
  through the manifest. But the manifest alone is not sound: under code page
  65001 `FindNextFileA` ends a listing at the first name whose UTF-8 passes
  259 bytes, error 234, and `dir.c:248` reads that as the end. `probe` then
  says 3 seeds of 6, exit 0: a wrong answer the base never gave, reaching
  every walk, build and `remove_tree`. Three narrow lines made wide close it,
  measured 6 of 6, and they give a lone-surrogate name defect 239's own
  message. Below the floor (a) is silent; (d) says so on stderr, exit 2. A
  lone-surrogate argument stays U+FFFD, as it was `?`, at the base.
- `prediction`: at the batch that lands 238's repair in this form, the box's
  leg reads `probe` over a folder of six names, one of 100 × U+6F22, as **6
  seeds**, and the compiler's own tests at 0 failed once the two defect-239
  cases' Windows branches make their name by `CreateFileW`; with the
  directory door left narrow, the same folder reads 3.
- `condition`: a row or shape red through (a⁗)+(d)+the wide door on the box
  (its own tests and the net's were lost with the box at 15:30 and are owed);
  or a narrow door other than the listing found to return a name into a
  buffer sized in bytes, which would need its own wide line.

### Q2. Cost, and the links `heroes` does not write

- `verdict`: **approve** the manifest carried inside the runtime's object,
  (a⁗); **object** to the three `.res`-on-the-line carriers as the default
  and **refuse** (a′) on its measurement; (b) whole priced and held back.
- `section`: design.md §1.1 (`:178`, *simplicity is a constraint ... it sets
  the ceiling*) and §1.7 (`:410`, *it determines the size of your compiler*):
  none of these routes is core or sugar, and (a⁗) is the only one the
  compiler never sees.
- `implementation_cost`: (a⁗)+(d)+door **153 lines net of `runtime/`; 0 of
  `selfhost/`; 0 goldens; the seed unchanged (its fixpoint byte-identical,
  run); 0 lines of `seed/README.md`, `ci.yml:428-429`, `:471` or the site**.
  A `.res` route: `link.hero` about +12 (213 of 300), +1 word on each of the
  four lines, `ci.yml` owing the site's build; (a″) also a byte writer in
  `hero_os.h` and a regenerated seed; (a1) also `llvm-rc` on every machine
  and a `doctor` line. (b) whole: about 250 more lines, unbuilt.
- `needed_for_self_hosting`: no.
- `argument`: The carrier decides the cost. The XML route fails under
  link.exe (`LNK1158`, no `mt.exe`). A `.res` works under both linkers but
  must be named on four lines `heroes` does not write: the seed's, the CI's
  two, an `--emit-c` author's; one forgotten is 238 again, silent without
  (d). Carried inside `runtime.c` instead, as `cvtres`'s sections, it reaches
  all four unchanged: measured on each. So the adopted route is 153 lines of
  `runtime/`, none of `selfhost/`, no golden, no seed, no `ci.yml`, no site
  build, no new axis in `link_flags()`. Its price is one refusal, loud under
  both linkers: no second resource in a program, which `heroes build` cannot
  bring today.
- `prediction`: the landing's `git diff --stat` against its base, for 238,
  touches **no file under `selfhost/` but test cases, not `seed/heroes.c`,
  not `seed/README.md` and not `.github/`**, and `runtime/parts/codepage.c`
  stays under 160 lines by `wc -l`; checkable at the batch gate that carries
  it.
- `condition`: the CI's clang 20.1.8, or its default linker, refusing the
  `.rsrc` object on a dispatch (unrun: an outward act); or a program on the
  roadmap needing an icon or a version resource. Either moves me to (a‴), a
  `.res` tracked in `runtime/` and named on the four lines, with (d).

### Q3. The instruments

- `verdict`: **approve** the eighteen cases above, built on a wide-world
  helper; **object** to any case that makes its names through the narrow API,
  because it cannot be red at `7f4c0cc5`.
- `section`: design.md §1.12; the verification rule that a repair is gated
  by its own cases (`.claude/rules/verification.md` § The batch). **The
  document does not say a case must be red before its repair**; the critic
  searched for it and found no rule, and this sitting's brief is what asks it.
- `implementation_cost`: one C helper of `wtool.c`'s shape, about 130 lines,
  as a string constant in the compiler's own tests, as 239's maker is; the
  rows themselves about 15 lines each in Heroes; the two 239 cases' Windows
  branches rewritten.
- `needed_for_self_hosting`: no.
- `argument`: A case red at `7f4c0cc5` needs its names from the wide world:
  defect 239's two cases make `café.hero` through a narrow C program and
  passed at the base while testing this defect as the platform's behaviour;
  both go red under any (a) and must make a lone-surrogate name by
  `CreateFileW` instead. Every row then has a case, built on the helper's
  shape 239 already uses, red at the base by the same mechanism I ran: twelve
  shown red, three unrun there because the box went away. They belong in the
  compiler's own tests, which both Windows legs run on every push; nothing
  else moved on the box, 1,190 and 246 tests, nor on Linux or this Mac.
- `prediction`: at the landing, the box's leg run on `7f4c0cc5`'s runtime
  with the new cases reads them **all red but W18**, and on the landing's
  runtime **all green**; checkable by running the landing's tests against
  both runtimes on the box.
- `condition`: one of W1 to W15 green on `7f4c0cc5`'s runtime: it would be
  testing nothing, as 239's cases were.

### Q4. Platforms and the floor

- `verdict`: **approve** (d) as the floor's only loud form short of (b)
  whole; **object** to shipping any (a) without it.
- `section`: design.md §1.12 (`:579`).
- `implementation_cost`: 13 lines inside `codepage.c` and 4 in `os.c`,
  counted above; no emitter line, against 350 emitted files if it lived in
  the generated `main`.
- `needed_for_self_hosting`: no.
- `argument`: Off Windows the route is one call to an empty function, and the
  run agrees: 1,190 own tests, the fixpoint byte-identical, `runtime` 8 and
  `emission` 754 green on this Mac; 1,190, the fixpoint and `runtime` on
  Linux arm64. On Windows the floor is a documentation fact nobody here has
  run, and below it (a) reads every name wrong in silence. (d) turns that
  into one ASCII sentence and exit 2 before the first line, at 13 lines in
  `hero_args_set`, reached and read under three launchers. Its cost is that
  every Heroes program, ASCII ones and the compiler included, refuses to
  start there. Only (b) whole charges less, about 250 lines.
- `prediction`: on any Windows whose `GetACP()` answers 65001 for a process
  carrying the manifest, no Heroes program prints (d)'s sentence: across the
  box's next batch leg and the CI's next Windows run, **0 occurrences** of
  *does not honour the UTF-8 code page* in their logs.
- `condition`: a supported Windows, below the floor, that a user must run
  Heroes on: then (b) whole, and (d) kept for a bound library's narrow calls.

### Handed on, not ruled here

- **Q-c, to panel 192**: `spec/heroes-spec.md:324-325`, *`args()` ... one
  that is not UTF-8 aborts*, is **already false at `7f4c0cc5`** for an
  argument holding a lone surrogate (it arrives as `x?y`) and stays false
  under (a) (`x<U+FFFD>y`). It can be made true by the UCRT's wide `argv`,
  measured here to keep `\ud800`, at about 25 lines and no golden. Whether it
  should is what a `str` may hold.
- **Q-i, to panel 192**: the interior NUL, above.
- **The console**: under every route the runtime writes bytes to a stream
  (`os.c:24-31`), and no route here changes the console's output code page,
  by reading; named, not repaired, and the ffi-pragmatist's to run.

### In one line

Adopt the manifest **inside the runtime's object**, the start-up refusal in
`hero_args_set`, and the directory door wide: 153 lines of `runtime/`, built
and run, reaching every link unchanged, and closing the silent truncation
that the manifest alone would have introduced.

## Resumed at 15:52: what was owed, now run (15:52 to 15:59)

Stopped at the account's session limit at about 15:46 by the coordinator's
count; my last write was 15:48. Appended here rather than written over the
sections above, which keep what they said when they were written.

**The box came back after a reboot, not a pause.** Its System log reads event
6008, *the previous system shutdown at 3:31:23 PM ... was unexpected*, and
event 41 at 15:45:24, *rebooted without cleanly shutting down*; the same pair
stands at 12:29 earlier today. `LastBootUpTime` 15:45:25. My wide-door
own-tests file there was **0 bytes**: that run had not begun to write. The
machine showed 1,361 MB at 15:47 and 3,471 MB at 15:52 (Hyper-V's memory,
growing again). A user process cannot normally bring Windows down, so the
cause is not mine to name.

**Run since, on the box, each alone:**

- The wide door's compiler, its own tests (15:47:52 to 15:49:45): **1,190
  tests, 1 failed**, `selfhost/cli/process.hero:387`, defect 239's case, at
  `assert walked.files == [dir + "/only/plain.hero"]`, as I had predicted by
  reading.
- Its net's own tests (15:52:30 to 15:53:00): **246 tests, 1 failed**,
  `tests/harness/shell.hero:987`, the twin. So **under both built routes the
  only cases that move on Windows are defect 239's two.**
- The three base-side shapes (`rows/left.sh`, output `out-left.txt`), base,
  then (a⁗)+(d), then with the wide door:

| case | base `7f4c0cc5` | (a⁗)+(d) | with the wide door |
|---|---|---|---|
| W13 `probe` over the six-name folder, the 300-byte name fourth | **exit 2**, *cannot read `.../m???...?.hero`* | **3 seeds**, exit 0 | 6 seeds |
| W15 a Heroes program's `args_checked()` over `＂a`, `b＂`, `c`, launched wide | **two words**, `a b` and `c` | three, `＂a` `b＂` `c` | three |
| W16 a program `heroes build` makes, reading `GetACP()` by `extern "windows.h"` | **1252** | 65001 | 65001 |

  So all three are red at `7f4c0cc5`, and **sixteen of the eighteen cases of
  Q3 are now shown red at the base**; W17 (the refusal) and W18 (an ASCII
  program) are not cases that can be. W13 also shows the shape of the new
  wrong: loud at the base, silent under the manifest alone.
- `--sanitize` with the manifest's object (15:58): `acpprobe.hero` built by
  (a⁗)'s compiler with `--sanitize` carries one MANIFEST resource of 368
  bytes, and with clang's `clang_rt.asan_dynamic-x86_64.dll` on PATH runs
  `65001`, exit 0. The sentence I took out of `codepage.c`'s comment at 14:59
  as a recollection is now a run; the comment stays without it.
- **The wide door's compiler reached the shared folder whole** (15:54 to
  15:58): `191-shared/c-dirwide/heroes-c-dirwide.exe`, 14,923,776 bytes,
  sha256 `cbbd261db0b26db59f76450a8cdcc382037c02fbc236644c03fe792703f2b9a4`
  on both machines; its `README.txt` rewritten at 15:58. The cut first copy
  stays beside it, named `.PARTIAL-DO-NOT-USE`.

**Still unrun, and why**: Windows below the floor (no instrument older than
build 26100 here); the CI's clang 20.1.8 on the `.rsrc` object (a push or a
dispatch, outward); a Hyper-V-isolated older container (not tried; the
ffi-pragmatist's).

### Sentences corrected in place while I wrote, listed so none is lost quietly

Each was wrong when written and corrected within minutes, before this report
was read by anyone; the table says what it said and what it says.

| where | it said | it says, and why |
|---|---|---|
| route (g) | `.UTF8` reaches `fopen`, `_mkdir` and `remove` | only `fopen` was run; the other two are marked inference |
| the new part's size | 82 lines | 81: counted before one unverified sentence left its comment |
| where the cases run | the CI's Windows leg runs the compiler's own tests at tags only, from the comment at `ci.yml:485-494` | every event since 2026-09-03 (`:622-633`), and the net's own tests every push (`:643-664`); the correction is also written inline |
| the cost of (a″) | it moves `HERO_RUNTIME_ABI` | batch 9 added three entries to `hero_os.h` and moved it 0 times; the seed is regenerated because `selfhost/` changes |
| route (f) | three rows wrong in silence, 2a, 2b, 7b | four, 2a, 2b, 3 and 5; 7b is loud, exit 2 |
| why every program links the runtime | the ABI stamp makes it so | its generated `main` calls `hero_args_set`, which only the runtime defines |
| the export of the wide door | its compiler and sha256 are in `README.txt` | the compiler did not arrive then (it has now, above) |
| the UCRT's wide `argv` | its memory freed at exit; documented for the startup | its wide words are the CRT's memory; documented or not, I did not read |
| a time | *Answered (15:00)* | *written at 15:13*: the first was not read from the clock |

### The verdicts, amended underneath (15:59)

The verdicts above stand, with these changes, each because a run above
settled what they had left open:

- **Q1**, `condition`: its first half, *the wide door's own tests and the net's
  were lost with the box and are owed*, is discharged: 1,190 with 1 failed and
  246 with 1 failed, both defect 239's. What remains of the condition is its
  second half: a narrow door other than the listing found to return a name
  into a buffer sized in bytes.
- **Q3**, `argument`, re-counted (118 words by `wc -w`): A case red at
  `7f4c0cc5` needs its names from the wide world: defect 239's two cases make
  `café.hero` through a narrow C program and passed at the base while testing
  this defect as the platform's behaviour; both go red under any (a), the wide
  door included, and must make a lone-surrogate name by `CreateFileW` instead.
  Every row then has a case, built on the helper's shape 239 already uses, and
  sixteen are shown red at the base by the same mechanism I ran; the other two
  cannot be. They belong in the compiler's own tests, which both Windows legs
  run on every push. Nothing else moved on the box under either route, nor on
  Linux or this Mac.
- **Q1** and **Q2** otherwise unchanged; **Q4** unchanged.

### One more shape run, 16:00: the in-place writer on a long-named link

The Q-g section said, by reading and unrun, that `replace.c:104-111`'s
`hero_fs_is_surrogate` would let `fmt --in-place` replace a symbolic link
whose name passes 259 UTF-8 bytes. Run on the box (`rows/link.sh`, output
`out-link.txt`; `wtool.c` gained `symlink` and `attrs`): a file `target.hero`
(unformatted, 31 bytes) and a symbolic link to it named `L` and 100 × U+6F22,
made by `CreateSymbolicLinkW`; then `fmt <that name> --in-place`, launched
wide.

| compiler | exit | the link afterwards | `target.hero` afterwards |
|---|---|---|---|
| base `7f4c0cc5` | **2**, *cannot read `L???...?.hero`* | a link | 31 bytes, unchanged |
| (a⁗)+(d) | **0** | **a plain file** | **31 bytes, unchanged** |
| with the wide door | 0 | a link | 29 bytes, formatted |

**So the manifest alone replaces the author's link with a plain file and
leaves the file they meant unformatted, at exit 0**: defect 136's class, a
second wrong answer the base never gave. The wide door writes through the
link, as `fopen` would have.

**Q1's `argument`, amended underneath (113 words by `wc -w`)**: Every row is
red at `7f4c0cc5` with names made wide, and green through the manifest. But
the manifest alone is not sound. Under code page 65001 a narrow listing stops
at the first name whose UTF-8 passes 259 bytes, error 234, read as the end:
`probe` says 3 seeds of 6, exit 0. And `fmt --in-place` on a symbolic link
with such a name replaced the link by a plain file, exit 0, its target
untouched. Two wrong answers the base never gave. Three narrow lines made wide
close both, measured, and name a lone-surrogate file as defect 239 meant.
Below the floor (a) is silent; (d) says so on stderr, exit 2.
