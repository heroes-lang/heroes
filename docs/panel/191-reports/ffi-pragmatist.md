# Panel 191, the ffi-pragmatist's report

Opened 2026-10-04 14:44 by `date`. Written as I go; a section marked
*in progress* is not a finding yet.

## My copy and my compiler

`<scratchpad>/191-ffi-pragmatist/`, made by `git -C <trunk> archive 7f4c0cc5
| tar -x`. Its seed's sha256 begins `26ccaa9d96478a20`, as the brief says.
The compiler built from it between 14:44:06 and 14:44:11 by `clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes` (Apple clang, this Mac) prints
`heroes 0.2.0`.

The box answered at 14:44:17 by its own clock: `MINGW64_NT-10.0-26100`,
clang 23.1.1, 22,753,240 KB free on `C:` (`df -k`). No folder under `/c/w/`
carried 191 in its name. Mine is `/c/w/191-fp-win/`.

**The tree on the box.** `git archive --format=tar.gz 7f4c0cc5`, 20,917,149
bytes, sent in 1 MB parts by `<scratchpad>/191-fp-work/send.sh` (the
`windows-b9.sh` shape, nothing removed), sha256 `76856490db548528` on both
sides. Unpacked to `/c/w/191-fp-win/t/`, seed `26ccaa9d96478a20`, built by
`seed/README.md`'s Windows line between 14:49:03 and 14:49:44 by the box's
clock: exit 0, 0 warnings, `heroes 0.2.0`, `heroes.exe` 14,921,728 bytes.
Every probe goes to the box as a script over stdin (`ssh win 'bash -s'`), as
in panel 189, so no MSYS command-line parser touches a byte.

## The box's facts, re-read (14:49 to 14:52, `b2-facts.sh`, `b3-linker.sh`)

| fact | read | how |
|---|---|---|
| Windows | Server 2025 Standard, 24H2, build 26100.32690 (UBR `0x7fb2`) | `ver`, the registry's `CurrentVersion` |
| ACP | **1252** | the registry's `Nls\CodePage\ACP`, and `GetACP()` from a program |
| OEMCP | **437** | the registry, and `GetOEMCP()` |
| the console, as cmd reads it over ssh | `Active code page: 437` | `cmd //c chcp` |
| the console, from a program | `GetConsoleCP` 437, `GetConsoleOutputCP` 437; `GetConsoleWindow` answers a window; **stdin, stdout and stderr are pipes**, `GetConsoleMode` fails on each | `cp.c`, the same from Git Bash, `cmd` and PowerShell |
| PowerShell | 5.1.26100.32684; `[Text.Encoding]::Default` Windows-1252; `$OutputEncoding` us-ascii; `[Console]::OutputEncoding` IBM437 | `powershell -NoProfile` |
| Git Bash | bash 5.3.15; `LANG`, `LC_ALL`, `LC_CTYPE` unset | `echo` |
| **the linker clang 23.1.1 uses** | **`lld-link` 23.1.1, with and without `-fuse-ld=lld`**; `-defaultlib:libcmt`, the static CRT | `clang -### t.c -o t.exe` |
| MSVC's linker | present, `link.exe` 14.44.35207 (VS 2022 BuildTools), reached by `-fuse-ld=link` | `clang -fuse-ld=link -###` |
| tools on `PATH` | `lld-link`, `llvm-rc`, `llvm-mt`, `llvm-readobj` present; `rc`, `mt`, `cvtres`, `llvm-cvtres` absent; `link` is MSYS's coreutils `/usr/bin/link`, not MSVC's | `command -v` |
| `WIN32_FIND_DATAA.cFileName` | `CHAR cFileName[ MAX_PATH ]`, `MAX_PATH` 260 | `minwinbase.h:111`, `minwindef.h:60`, SDK 10.0.26100.0 |

**So the critic's F4 resolves differently on the box than on this Mac**:
clang 22.1.8 cross from this Mac picks MSVC `link.exe`, the box's clang
23.1.1 picks `lld-link` by default, with no `.cfg` file beside it. A user's
default therefore depends on their clang's version, and both linkers are on
the box to be measured.

## Q1-a: is each row red at `7f4c0cc5`? (in progress)

### The names, made three ways (14:58, `q1-setup.sh`, `wide.ps1`, `mk.hero`)

The probe programs were built on the box by the trunk compiler: `launch`
(a launcher made of Heroes: each line of a plan file run through
`hero_run_go`, so `CreateProcessA`), `mk`, `names`, `cat`, `args`, all
exit 0. Then the names, listed afterwards by the wide API
(`Get-ChildItem`, `ls.ps1`) as UTF-8 bytes:

| maker | `café.hero` on disk | `ŝ.hero` | `漢.hero` | `dé/` |
|---|---|---|---|---|
| **wide**: PowerShell 5.1 `[IO.File]::WriteAllText`, as Explorer and every .NET program name a file | `café.hero` | `ŝ.hero` | `漢.hero` | `dé` |
| **narrow**: a Heroes program's `write_file`, the trunk's runtime | **`cafÃ©.hero`** (U+00C3 U+00A9) | **`Å<U+009D>.hero`** | **`æ¼¢.hero`** | **`dÃ©`** |
| **git**: a commit of the wide names, `git clone`d | `café.hero` | `ŝ.hero` | `漢.hero` | (not committed) |

So under code page 1252 a Heroes program's `write_file` writes every one of
these names wrong, and the byte `0x9D`, which Windows-1252 leaves
undefined, lands on disk as U+009D (whether it reads back is run below). And **git names a checked-out file
as the wide world does**: a tracked case's file would be the wide row, not
the narrow one. The wide maker also made `x<U+D800>.hero`, a lone surrogate,
which NTFS took (Q-c), and a name of 100 × U+6F22 (Q-g).

### The compiler, launched from Git Bash (15:00, `q1-a.sh`)

The trunk's `heroes.exe`, `HEROES_RUNTIME` the tree's own runtime. Exit and
the first line of what it said:

| row | wide names | narrow names (a Heroes program made them) | git names |
|---|---|---|---|
| `check café.hero` | **2**, *argument 2 is not UTF-8* | **2**, the same | **2**, the same |
| `check ŝ.hero` | **2**, *cannot read `s.hero`* (best fit) | **2**, the same | **2**, the same |
| `check 漢.hero` | **2**, *cannot read `?.hero`* (no best fit: `?`) | **2**, the same | **2**, the same |
| `check dé/p.hero` | **2**, *argument 2 is not UTF-8* | **2**, the same | (not committed) |
| `probe only/`, `mutate only/` (`café.hero`) | **2**, *the name of `only/caf<0xE9>.hero` is not UTF-8 ... rename it* | **0**, 1 seed, every family held | (not committed) |
| `probe only2/` (`ŝ.hero`) | **2**, *cannot read `only2/s.hero`* | **0**, 1 seed | **2**, *cannot read `only2/s.hero`* |

And the decoys, all wide names:

| row | what the trunk did |
|---|---|
| `check ŝ.hero`, an `s.hero` holding a type error beside it | **exit 1, `error[type_mismatch]` at `s.hero:2:14`**: a file nobody named |
| `fmt ŝ.hero --in-place`, an unformatted `s.hero` beside it | **exit 0, and `s.hero` rewritten**: sha256 `85762f6c90bf5d0f` to `6c13c84b1ee9105e`; `ŝ.hero` untouched |
| `probe decoy-walk/` (`ŝ.hero` correct, `s.hero` a type error) | **exit 0, *2 seeds***: the walk hands `ŝ.hero` back as `s.hero`, so one file is probed twice and the other never, and the run reads green |
| **Q-f**: `check ..／x.hero` from `fw/sub/` (a file whose name holds U+FF0F), `fw/x.hero` a type error | **exit 1, `type_mismatch` at `../x.hero:2:14`**: best fit turned the fullwidth solidus into `/`, and **the compiler judged a file in another directory** |
| **Q-f**: `check ..＼y.hero` (U+FF3C), `fw/y.hero` a type error | **exit 1, `type_mismatch` at `..\y.hero:2:14`**: the same through `\` |
| `HEROES_RUNTIME` = `...\rté`, a real runtime there | **exit 2**, *its value, `C:\w\191-fp-win\q1\wide\rt<0xE9>`, is not UTF-8, so this compiler cannot look in the directory it names* |
| the control: `HEROES_RUNTIME` ASCII, `build p.hero` | exit 0, the program prints `7` |

Two of these messages are **false on Windows**, which batch 9's repairs made
true elsewhere: *the name of `only/caf<0xE9>.hero` is not UTF-8 ... rename
it* (239) and *its value ... is not UTF-8* (243). The name and the value are
valid Unicode on disk and in the environment; the runtime's narrow door made
them 1252 bytes. On Linux the same words are true. So batch 9 changed 238's
rows from a panic and a silent *not found* into a refusal that tells the
user to rename a correct file.

### The compiler, launched by a Heroes program (15:00, `q1-bc.sh`, section B)

`launch.exe` runs each line of its plan through `hero_run_go`, so
`CreateProcessA`, the door the net's harness uses for every case:

| row | wide names | narrow names | git names |
|---|---|---|---|
| `check café.hero`, `ŝ.hero`, `漢.hero`, `dé/p.hero` | **2**, *cannot read `../café.hero`* and the same for each | **0, 0, 0, 0** | **2**, *cannot read* each |
| `probe only/` | **2**, *the name ... is not UTF-8 ... rename it* | **0** | (not committed) |
| `probe only2/` | **2**, *cannot read `../only2/s.hero`* | **0** | **2**, the same |
| `check ŝ.hero` in `decoy-check/` | **2**, *cannot read `../ŝ.hero`* (no best fit: the launcher's bytes are already narrow) | | |

**The critic's inference is now a measurement**: under code page 1252, names
a Heroes program makes and a Heroes program passes on are self-consistent,
and **every row reads green on the broken tree**. `CreateProcessA` reads the
UTF-8 bytes as 1252 (`Ã©`), the child's CRT turns them back into the same
bytes, and `fopen` opens the file `write_file` made under the same wrong
name.

### A built program, under four launchers (15:00, section C)

`names.exe café plain ŝ 漢` (the trunk built it), run in a folder holding
`café.txt` and `ŝ.txt`; `args_checked()`'s bytes, then `read_file` and
`write_file` by names written in the source:

| launcher | names made | arguments `café` / `ŝ` / `漢` | reads `café.txt`, `ŝ.txt` | names written on disk |
|---|---|---|---|---|
| Git Bash | wide | **`not_text`** / **`73`** (`s`) / **`3f`** (`?`) | **`file_not_found`**, both | **`cafÃ©-out.txt`**, `Å<U+009D>-out.txt`, `æ¼¢-out.txt` |
| PowerShell 5.1 | wide | the same | the same | the same |
| `cmd` (a UTF-8 `.cmd`, `chcp 65001`) | wide | the same | the same | the same |
| **a Heroes launcher** | wide | **`63 61 66 c3 a9` / `c5 9d` / `e6 bc a2`: right** | `file_not_found`, both | the same wrong names |
| Git Bash, PowerShell, `cmd` | narrow | `not_text` / `73` / `3f` | **ok**, both | the same wrong names |
| **a Heroes launcher** | narrow | **right** | **ok**, both | the same wrong names |

And the best fit opening another file in a built program: `cat.exe ŝ.txt`
(it reads the file its first argument names), an `s.txt` holding `decoy`
beside the real one: from Git Bash and from PowerShell alike, **`name [73 2e
74 78 74] read [64 65 63 6f 79 0a]`**, the decoy, exit 0.

**What Q1-a answers.** Every row of 238 and of 189's omitted list is red at
`7f4c0cc5` where the name comes from the wide world (PowerShell, Explorer,
git, a user's shell), under every launcher a user has. **The rows go green
where both the name and the launch stay inside Heroes**: a Heroes-made name
through a Heroes launcher reads right, and so does an argument a Heroes
launcher passes, whoever made the file. The written name is wrong under
every launcher, but only a wide reader sees it: a Heroes program reading its
own output back sees nothing wrong. So **a case is red on this tree only if
its name, or its argument, or its observer comes from outside the Heroes
runtime**: a tracked file (git names it wide), a launcher that is not a
Heroes program, a reader that lists wide. Q3 below builds them.

### Section D: the other launchers, the other shapes (15:02, `q1-d.sh`)

| row | what the trunk did at `7f4c0cc5` |
|---|---|
| **Q-k**: `check café.hero` from PowerShell 5.1 (`&`, so `CreateProcessW`) | **2**, *argument 2 is not UTF-8* |
| **Q-k**: `check ŝ.hero` from PowerShell, the decoy beside it | **1, `type_mismatch` at `s.hero:2:14`** |
| **Q-k**: the same two from `cmd` (a UTF-8 `.cmd` under `chcp 65001`) | **2** and **1, at `s.hero:2:14`** |
| **Q-f**: one argument `a＂b c` (U+FF02) to `names.exe`, from Git Bash, PowerShell and `cmd` | **two arguments, `61 62` (`ab`) and `63` (`c`)**: best fit made the fullwidth quote a real one, and the CRT's split ended the argument there |
| **Q-f**: the same from a Heroes launcher | one argument, `61 ef bc 82 62 20 63`, right |
| **Q-c**: an argument holding a lone surrogate, `x` U+D800 `y`, from PowerShell, through `args_checked()` | **`ok`, `78 3f 79`, `x?y`** |
| **Q-c**: the same through `args()` | **`78 3f 79`, no abort, exit 0** |
| **Q-c**: `probe sur/`, the file `x<U+D800>.hero` | **2**, *cannot read `sur/x?.hero`* |
| **Q-g**: `probe long/`, the file of 100 × U+6F22 | **2**, *cannot read `long/????...?.hero`* (100 `?`) |
| the compiler installed under `José/bin`, its runtime beside it, `HEROES_RUNTIME` unset, an ASCII working directory | **2**, *cannot find the Heroes runtime ... nothing beside the compiler, since the compiler's own path, `C:\w\...\Jos<0xE9>\bin\heroes.exe`, is not UTF-8* |
| a two-module project under `José/proj`, the working directory there, relative names, the ASCII compiler | **0**, `wrote main.exe`, prints `12`; `heroes run main.hero` **0**, `12` |
| the same project by an absolute name | **2**, *argument 2 is not UTF-8* |
| the compiler under `José/bin` and the project under `José/proj` | **2**, *cannot find the Heroes runtime* (as above) |
| `build café.hero -o café.exe` | **2**, *argument 2 is not UTF-8* |

**So Q-c's spec sentence is already false on Windows at `7f4c0cc5`**,
before any route: `spec/heroes-spec.md:324-325` says an argument that is not
UTF-8 aborts `args()`, and an argument that is not even well-formed UTF-16
reaches `args()` as `?`, valid UTF-8, a wrong value with no failure. Route
(a) is measured against that below. And **a project whose folder is above
ASCII is not red by itself**: a working directory above ASCII with
relative names builds and runs on the broken tree. It goes red the moment
an absolute name crosses a door: the compiler installed in such a folder
(277's door), `HEROES_RUNTIME` (243's), an absolute argument. A user's
profile is such a folder, and an installer puts the compiler there.

## Route (a), built on the box (15:04 to 15:09)

### A probe compiler, so that every carrier reaches every link

Route (a) needs the manifest on every link `heroes build` makes. Building
that properly is the compiler-engineer's; to run the rows end to end I made
a **probe**: `<scratchpad>/191-fp-work/probe/`, `git archive 7f4c0cc5
selfhost runtime`, with one change, `selfhost/cli/flags.hero`'s
`link_flags()` appending on Windows the words of `HEROES_PROBE_LINK`, split
on `|` (24 lines, formatted, `heroes check selfhost/main.hero` exit 0). Its C
emitted on this Mac (`--emit-c`, 41,377,197 bytes, between 15:04:40 and
15:06:33), sent gzipped (sha256 `a2ef4df01849a7fa` on both sides), compiled
once on the box (`probe-heroes.o` and `runtime.o`, 0 warnings, 15:08:22 to
15:08:33), then **linked once per carrier and linker** with the seed's
stack flag, with 189's `acp.c` linked the same way beside it (`route-a.sh`).

### The carriers, under both linkers

| carrier | linker | link | `RT_MANIFEST` (`llvm-readobj --coff-resources`) | `GetACP()` |
|---|---|---|---|---|
| none (the control) | lld-link | 0 | none | 1252 |
| **(a1)** `utf8.res`, by `llvm-rc` | **lld-link** (the box's default) | 0 | 368 bytes, the file verbatim | **65001** |
| **(a1)** `utf8.res` | **link.exe** 14.44 (`-fuse-ld=link`) | 0 | 368 bytes | **65001** |
| **(a′)** `-Wl,/MANIFEST:EMBED -Wl,/MANIFESTINPUT:utf8.manifest` | lld-link | 0 | 579 bytes, merged with lld's default `trustInfo` | **65001** |
| (a′) the same plus `-Wl,/MANIFESTUAC:NO` | lld-link | 0 | 328 bytes | **65001** |
| **(a′)** | **link.exe** | **exit 1158**: `LINK : fatal error LNK1158: cannot run 'mt.exe'` | none, no binary | |
| (a′) plus `/MANIFESTUAC:NO` | link.exe | **the same `LNK1158`** | none | |

**The `.res`.** `llvm-rc /FO utf8.res utf8.rc` on the box: exit 0, **432
bytes, sha256 `178d9a7f2a47c56c`**, a second run byte-identical by `cmp`:
the same bytes the critic's cross `llvm-rc` 22.1.8 made on this Mac. So (a″),
the compiler writing those 432 bytes itself, hands the linker exactly the
file (a1) handed it, and its link rows are (a1)'s, under both linkers.

**Why (a′) dies under `link.exe`.** `mt.exe` is on the box, in the SDK's
`bin\10.0.26100.0\x64\`, and not on `PATH` (`PATH` holds no Windows Kits
directory); clang's driver passes `link.exe` its library paths and no `PATH`.
`link.exe` takes a `.res` through `cvtres.exe`, which sits beside it in the
MSVC `bin` directory, so (a1) needs nothing from `PATH`. A Developer Command
Prompt puts the SDK's `bin` on `PATH`, so there (a′) would link; an ordinary
shell is the box's own case, and it fails. **So (a′) works only under
`lld-link`, and `link.exe` is the CI's linker by the tree's own record**
(`flags.hero:181-191`) **and the default of clang 22.1.8** (the critic's
`-###`): (a′) is refused by measurement, and the carrier is the `.res`,
made by `llvm-rc` (a1) or written by the compiler (a″).

### Every row under route (a1), lld-link (15:10 to 15:29)

`heroes-a1-lld.exe` (the probe compiler, the `.res` linked in), its runtime
the tree's, `HEROES_PROBE_LINK` naming the `.res` so that every program it
builds carries it (`names.exe`: 1 `RT_MANIFEST`). The names made again, by
the wide maker and by `mk.exe` built by this compiler, in
`/c/w/191-fp-win/qa-a1-lld/` (`q1-route.sh`, then `q1r2-a1-lld.sh`).

**Under (a1) a Heroes program's `write_file` names its files right**: `mk.exe`
made `café.hero`, `ŝ.hero`, `漢.hero` and `dé/p.hero`, byte for byte the wide
maker's names. So the narrow and the wide worlds are one world.

| row | trunk (Q1-a) | route (a1) |
|---|---|---|
| `check café.hero`, `ŝ.hero`, `漢.hero`, `dé/p.hero`, from Git Bash, names made either way | 2, 2, 2, 2 | **0, 0, 0, 0** |
| the same through a Heroes launcher | 2 wide, 0 narrow | **0** both |
| `probe only/`, `mutate only/`, `probe only2/` | 2 wide, 0 narrow | **0** both |
| `check ŝ.hero`, the decoy `s.hero` beside it | 1, `s.hero:2:14` | **0**: `ŝ.hero` itself was judged |
| `fmt ŝ.hero --in-place`, an unformatted `s.hero` beside it | `s.hero` rewritten | **`s.hero` untouched**, `85762f6c90bf5d0f` before and after |
| `probe decoy-walk/` | 0, **124 variants: `s.hero`'s 62, twice** | 0, **111 variants: `s.hero`'s 62 and `ŝ.hero`'s 49** |
| Q-f: `check ..／x.hero`, `check ..＼y.hero` | 1, `../x.hero`, `..\y.hero` | **0, 0** |
| `HEROES_RUNTIME` under `rté` | 2, *not UTF-8* | **0**, the program prints `7` |
| `names.exe café plain ŝ 漢`, Git Bash, PowerShell, `cmd`, a Heroes launcher, names made either way | `not_text` / `s` / `?` (a Heroes launcher: right) | **`63 61 66 c3 a9` / `c5 9d` / `e6 bc a2` under all four** |
| its `read_file` of `café.txt`, `ŝ.txt`, `漢.txt` | `file_not_found` (wide names) | **ok**, the right bytes |
| the names its `write_file` gave | `cafÃ©-out.txt`, `Å<U+009D>-out.txt`, `æ¼¢-out.txt` | **`café-out.txt`, `ŝ-out.txt`, `漢-out.txt`**, under all four launchers |
| `cat.exe ŝ.txt`, the decoy beside it | read `decoy` | **read `the real one`** |
| Q-k: `check café.hero` from PowerShell and from `cmd`; the decoy from PowerShell | 2, 1 | **0, 0, 0** |
| Q-f: one argument `a＂b c`, from Git Bash, PowerShell, `cmd` | two arguments, `ab` and `c` | **one, `61 ef bc 82 62 20 63`**, under all three |
| the compiler installed under `José/bin`, its runtime beside it | 2, *cannot find the Heroes runtime* | **0**, prints `7` |
| the project under `José/proj` by absolute names | 2 | **0**, prints `12` |
| everything at once: the compiler under `José/bin`, run from `José/proj`, source and output absolute, the output named `mainé.exe` | 2 | **0, `wrote ...\José\proj\mainé.exe`**, which prints `12` |
| `build café.hero -o café.exe`, then run it; `heroes run café.hero` | 2 | **0, prints `1`; `1`** (defect 240's repair holds: no octal escape on clang 23.1.1) |

**What (a) does not repair, measured:**

| row | trunk | route (a1) |
|---|---|---|
| **Q-c**: an argument `x` U+D800 `y` from PowerShell, through `args_checked()` and through `args()` | `78 3f 79`, `x?y`, no abort | **`78 ef bf bd 79`, `x` U+FFFD `y`, `ok`, and `args()` does not abort** |
| **Q-c**: the file `x<U+D800>.hero`, `probe sur/` and `check` from PowerShell | 2, *cannot read `sur/x?.hero`* | **2**, *cannot read `sur/x\uFFFD.hero`* (U+FFFD in the message) |
| **Q-g**: `probe long/`, one file named 100 × U+6F22 | 2, *cannot read `long/????...?.hero`* | **2, *no `.hero` file under long***: false, the file is there |
| **Q-g**: `probe long2/` and `mutate long2/`, three correct programs `a.hero`, `b` + 100 × U+6F22 + `.hero`, `c.hero` | 2, *cannot read `long2/b????...?.hero`* | **0, *1 seeds*; *1 programs under long2***: two of three files silently gone |

**Q-g, door by door** (`ffa2.c`, the long name's path 312 bytes of UTF-8,
105 UTF-16 units, linked with and without the `.res`, 15:29):

| door | no manifest (1252) | the manifest (65001) |
|---|---|---|
| `FindFirstFileA` / `FindNextFileA` over `long2/` | three names; the long one `b???...` (106 bytes) | `a.hero`, then **`FindNextFileA` fails, error 234 (`ERROR_MORE_DATA`)**; called again it hands back the long entry **cut to 259 bytes**, then `c.hero` |
| `fopen`, `CreateFileA`, `GetFileAttributesA`, `GetFileAttributesExA` of the long path | fail (`ENOENT`, error 3) | **open, found** |
| `FindFirstFileA` of the exact long path | fails, 3 | **fails, 234** |
| `heroes check long2/b漢...漢.hero` through a Heroes launcher, route (a1) | | **0** |

So under (a) the narrow doors that **take** a name work at any length this
test reached, and the doors that **return** a name into
`WIN32_FIND_DATAA.cFileName[MAX_PATH]` fail for a name whose UTF-8 is longer
than 259 bytes, while its UTF-16 is far under NTFS's 255 units. And the
runtime's walk turns that failure into silence: `dir.c`'s loop ends on any
`FindNextFileA` failure without asking `GetLastError()` for
`ERROR_NO_MORE_FILES`, and still answers `ok`
(`<scratchpad>/191-fp-work/winview/dir.c:206-227`). **This is a hole route
(a) opens rather than one it leaves**: on the trunk the long name is a loud
`cannot read`; under (a) it, and every name after it, are not there. The
other `FindFirstFileA`, `replace.c:106`'s `hero_fs_is_surrogate`, answers
*not a link* on the same failure, so a symbolic link with such a name would
be written over rather than through: an inference from that code and the
234 measured above, not run on a link. `GetModuleFileNameA` and
`GetFinalPathNameByHandleA` are given 4,096 bytes and refuse a cut name, so
they are not this hole.

## The box went offline at about 15:30

My last call that reached it wrote a 1 KB file at 15:29:46. From 15:31 every
`ssh win` timed out, and at 15:38 Tailscale read `apponfly-vps ... offline,
last seen 8m ago` (`tailscale status`), at 15:42 *last seen 12m ago*. Nothing
of mine was running there: my one stuck run had been stopped at 15:26 by
`taskkill` of its own process tree (below). What is still owed on the box is
marked *unrun* below, and is run if it comes back.

**A process slip, recorded.** My first route run went to the box as a script
over stdin, and stopped answering after `probe decoy-walk` at 15:10:56 with
no child process left: a child had most likely read the script from the
same stdin, which panel 189's route (`bash -s`) never met because its
children did not read. I stopped my own process tree there (`taskkill //PID
6072 //T //F`, 15:26; no file removed), and from then on every script went
as a file run with `< /dev/null`. The rows it had reached are kept (the
output file and the box's own files agree); the rest were re-run.

## The compiler-engineer's form of route (a), read (15:30)

Exported to `<scratchpad>/191-shared/a-object/` at 15:13: the manifest
**inside the runtime's own object**, `runtime/parts/codepage.c` (82 lines):
the XML in section `.rsrc$02`, the resource tree in `.rsrc$01` by
file-scope `asm` with one `.rva`, and route (d): `hero_args_set` refusing
to start, exit 2, an ASCII line on stderr, where `GetACP()` is not 65001.
Three files (`a-object.diff`, sha256 `4ea03e6bc231ae4b` by my `shasum`,
applied by `patch -p1` to a fresh `git archive 7f4c0cc5`, clean). No
`selfhost/` line, no seed change, no link-line change.

**From the C side this is the form that reaches the links `heroes` does not
write** (Q-d): an `--emit-c` user's own line, the seed's line, the CI's two
seed lines, all link `runtime.c` and so carry it; and (d)'s refusal lives in
the same object as the manifest, so a link cannot take one without the
other. Its stated price is mine to test: *a program can link no other
resource*, refused by name by both linkers. That is run on the box below, if
it comes back.

### Unchanged elsewhere, measured

| platform | the a-object runtime | how |
|---|---|---|
| this Mac (Apple clang) | the compiler from the unchanged seed: 0 warnings; **its own tests 1,190, all passed** (15:36 to 15:37); an ASCII program (`sort`, `join`, `print`): **output equal to the trunk's, emitted C byte-identical, 12,322 bytes** | `heroes test selfhost/main.hero`; `cmp` |
| Linux arm64, `heroes-linux-arm64`, Debian clang 22.1.8 | 0 warnings; **1,190, all passed** (13:39 to 13:40 by the container's clock); the ASCII program: **output equal, emitted C byte-identical, 12,818 bytes**; a program reading `args_checked()` over `café ŝ 漢` and `read_file("café.txt")`: the same lines under both runtimes | one container at a time, `docker ps -q` empty before each, trees copied onto the container's own filesystem |

My probe compiler (route (a1) through `link_flags()`) changes nothing off
Windows by construction: its hook sits after the POSIX branch's `return
["-rdynamic"]`. Not run as a separate suite.

## Resumed 15:52 (the account's session limit stopped me about 15:46)

The coordinator's message at 15:52 named my last written words. The box was
back on the tailnet at 15:52 (`tailscale status`: *idle*). My folder there
held no unread run: my last script, `qg2.sh`, had come back whole at
15:29:46, and `floor.sh` had never landed. Everything below is from 15:52
on.

## The compiler-engineer's c-dirwide, built and run on the box (15:54)

`<scratchpad>/191-shared/c-dirwide/` (exported 15:42): the a-object form,
(d), and **the directory door made wide**: `FindFirstFileW` and
`FindNextFileW` at `dir.c` and `replace.c:106`, a name carried back as UTF-8
(a lone surrogate as its three WTF-8 bytes), and a check of `GetLastError()`
for `ERROR_NO_MORE_FILES` after the loop. Its compiler did not arrive (the
box left mid-copy), so I built it: `c-dirwide.diff` (sha256
`1ce10acae22ed6c7`) applied to a pristine `git archive 7f4c0cc5`, and
`c-dirwide-over-a-object.diff` (`a586cfe81f5bac22`) applied over my a-object
copy, agree file for file (`cmp` of all five). Built on the box from the
unchanged seed, 15:54:20 to 15:54:30: **0 warnings**, `heroes 0.2.0`, and
the compiler itself carries one `RT_MANIFEST` (`cdw-box.sh`).

| row | trunk | route (a1) | **c-dirwide** |
|---|---|---|---|
| `cp.c` linked with the runtime's object, lld-link | | | **`GetACP` 65001, `GetOEMCP` 65001**, `RT_MANIFEST` 368 bytes |
| the same under link.exe 14.44 | | | **65001, 65001**, 368 bytes |
| `names.exe café plain ŝ 漢`, built by it, in a wide folder | wrong | right | **right: arguments, reads, and `café-out.txt`, `ŝ-out.txt`, `漢-out.txt` on disk** |
| **Q-g**: `probe long2/` (`a.hero`, `b` + 100 × U+6F22 + `.hero`, `c.hero`) | 2, *cannot read `b????...`* | **0, *1 seeds*** | **0, *3 seeds***; `mutate` *3 programs* |
| Q-g: `probe long/` | 2 | **2, *no `.hero` file*** | **0, *1 seeds*** |
| **Q-c**: `probe sur/`, the file `x<U+D800>.hero` | 2, *cannot read `sur/x?.hero`* | 2, *cannot read* `x` U+FFFD | **2, *the name of `sur/x<0xED><0xA0><0x80>.hero` is not UTF-8 ... rename it***: the bytes named, the file never read as another |
| `probe only/`, `only2/`; `decoy-walk/` | | | 0, 0; **111 variants** (both files) |
| **Q-c**: the argument `x` U+D800 `y` from PowerShell, `args_checked()` | `78 3f 79` | `78 ef bf bd 79` | **`78 ef bf bd 79`: U+FFFD, `ok`** |

So c-dirwide closes Q-g (the hole route (a) opened) and names a
lone-surrogate file truthfully. **The argument door still turns a lone
surrogate into U+FFFD under every form of (a)**: that half of Q-c is below.

## Q-l, a bound library's own `fopen` (15:55, `ql.sh`)

`lib191.c`, eleven lines of C opening the `cstr` it is lent with **its own**
`fopen` and returning the size, bound from Heroes (`extern "lib191.h" link
"lib191md"` / `"lib191mt"`, `lib191_size(path: cstr lent) -> i64`), lent
`"café.txt".cstr()`, `"ŝ.txt".cstr()`, `"漢.txt".cstr()`, files made wide
of 6, 13 and 4 bytes. Built two ways: **a `/MD` DLL** (`clang -shared
-fms-runtime-lib=dll`; it imports `VCRUNTIME140.dll`,
`api-ms-win-crt-runtime-l1-1-0.dll`, `api-ms-win-crt-stdio-l1-1-0.dll`:
the UCRT's DLL) and **a `/MT` static library** (`llvm-lib`, sharing the
program's `libcmt`). Each binding built by three compilers, `--include .
--library .`, all exit 0:

| the library's `fopen` of the program's `cstr` | trunk | route (a1) | c-dirwide |
|---|---|---|---|
| the `/MD` DLL: `GetACP` it sees; `café`, `ŝ`, `漢` | **1252; -1, -1, -1** | **65001; 6, 13, 4** | **65001; 6, 13, 4** |
| the `/MT` static library | **1252; -1, -1, -1** | **65001; 6, 13, 4** | **65001; 6, 13, 4** |

**This is the FFI half of 238, and only a process code page reaches it.** A
library's narrow call follows the process's ANSI code page, which the EXE's
manifest sets for every module in the process, the DLL's own CRT included.
**Under route (b) alone the code page stays 1252**, so a bound library reads
the program's UTF-8 `cstr` exactly as the trunk's column does: that column
*is* (b)'s for a library, since (b) changes no line the library runs and
no code page. That is an inference from the mechanism; no (b) build was
exported to run it.

## Route (g), refused on a measurement (15:55, `g.c`)

`setlocale(LC_CTYPE, ".UTF8")` answers `English_United States.utf8`. Then,
`g.c` run as `g.exe set café` beside a wide `café.txt`:

| door | without (g) | **with (g)** | with the manifest instead |
|---|---|---|---|
| `GetACP()` | 1252 | **1252** | 65001 |
| `argv`, `café` | `63 61 66 e9` | **`63 61 66 e9`**: built before `main` | `63 61 66 c3 a9` |
| the program's own `fopen` | failed | **opened** | opened |
| the `/MD` DLL's `fopen` | -1 | **-1**: its own CRT, its own locale | 6 |
| the `/MT` library's `fopen` | -1 | **6**: the program's CRT | 6 |
| `CreateFileA` | failed | **failed** | opened |
| `FindFirstFileA` | `63 61 66 e9` | **`63 61 66 e9`** | `63 61 66 c3 a9` |
| `getenv` | `63 61 66 e9` | **`63 61 66 e9`** | `63 61 66 c3 a9` |

(g) reaches the program's own CRT and a library that shares it, and nothing
else: not `argv`, not one Win32 `...A` call, not a DLL on the UCRT. Refused.

## Q-h: a fourth form of route (b)'s argument door, run (15:56, `wargv.c`)

The UCRT builds **its own wide `argv` on request**:
`_configure_wide_argv(_crt_argv_unexpanded_arguments)` from
`<corecrt_startup.h>`, then `__wargv`. That is the C runtime's own split of
`GetCommandLineW`, its own allocation (as `argv`'s), with no `shell32`, no
`LocalAlloc`, no hand parser and no `wmain`. Run on the box, under the
static CRT (`libcmt`), under the manifest, and under `/MD`:

| | narrow `argv` | **the UCRT's wide `argv`**, each converted with `WC_ERR_INVALID_CHARS` |
|---|---|---|
| `_configure_wide_argv` | | **returns 0, `__wargv` set**, under all three builds |
| `café ŝ 漢 a＂b c` from Git Bash, code page 1252 | `63 61 66 e9`, `73`, `3f`, then **`ab` and `c`: five arguments** | **`63 61 66 c3 a9`, `c5 9d`, `e6 bc a2`, `61 ef bc 82 62 20 63`: four, all right** |
| the same under the manifest | right, four | right, four, byte-equal to the narrow ones |
| `x` U+D800 `y` from PowerShell, 1252 | `78 3f 79` | **refused: error 1113, `ERROR_NO_UNICODE_TRANSLATION`** |
| the same under the manifest, static and `/MD` | `78 ef bf bd 79` | **refused: 1113** |
| `q"r\s` from PowerShell 5.1 (its own quoting) | `71 72 5c 73` | `71 72 5c 73`: the same split |

So the door that keeps spec `:324-325` true on Windows exists, costs one
call in `hero_args_set`, and has none of Q-h's three costs. One side effect,
measured: the call rewrote the CRT's global `__argc` (5 where `main`'s
`argc` was 6 under 1252). The runtime keeps its own copy of `argc`
(`hero_argc`, `os.c:514`), so it does not read the global; a bound library
that did would. (My first run read past `__wargv`'s end in that 1252 case,
error 87; the loop was fixed and the case re-run, `wargv2.sh`.)

## The console's output, named (15:58, `console-prep.sh`, `ssh -tt`)

`say.hero` prints `café ŝ 漢`. Through a pipe all three builds (trunk, (a1),
c-dirwide) write the exact UTF-8 bytes, `63 61 66 c3 a9 20 c5 9d 20 e6 bc
a2 0a`. **Under a pseudo-console** (`ssh -tt`, Git Bash; `cp-u8.exe` there
reads stdout as a console, `GetConsoleMode` yes) all three arrive as **`63
61 66 e2 94 9c e2 8c 90 20 e2 94 bc c2 a5 ...`: `caf├⌐ ┼¥`**, code page 437's
reading of the bytes. Under the manifest `GetACP` and `GetOEMCP` are 65001
and `GetConsoleCP` and `GetConsoleOutputCP` stay **437**: the manifest
does not reach the console. Not repaired by any route here, and named as the
brief asks. The same rows from `cmd` and PowerShell under `ssh -tt` are
**unrun**: both started an interactive shell rather than the program (my
launch through the pseudo-terminal, not a result).

## The floor (Q4, Q-j), 15:57

- **What route (a) does where the manifest is not honoured**: nothing; the
  process keeps code page 1252 and every row of Q1-a's trunk column comes
  back, in silence. That is the trunk column, which a manifest the loader
  ignores reproduces by construction (an inference: no Windows below the
  floor was run).
- **What route (d) prints**, run on code page 1252 by cutting the manifest
  from c-dirwide's `codepage.c` and keeping the check (`runtime-d`, a probe):
  `names-d.exe` **exit 2, stdout 0 bytes, stderr 256 bytes**: *error: this
  program reads every file name, argument and environment value as UTF-8,
  and this Windows reads them in code page 1252: it does not honour the
  UTF-8 code page the program carries, so a name above ASCII would be read
  as another. Nothing was run.* ASCII, so every console code page shows it.
  **Its sentence is true only where the manifest is in the image**: in the
  a-object and c-dirwide forms the check and the manifest are one object, so
  it is; under (a1) or (a″) with (d), a link that lacks the `.res` (an
  `--emit-c` user's own line, the seed's line) would print *does not honour
  the UTF-8 code page the program carries* about a program that carries
  none, a false message (inference from the two layouts; this probe *is*
  that case, and printed it).
- **The instrument below the floor: unrun, and why.** Docker is absent on
  the box; `Get-WindowsFeature` cannot list features (*the service cannot
  be started ... 0x80070422*, servicing disabled); and the guest is given no
  virtualisation extensions for a nested hypervisor
  (`SecondLevelAddressTranslationExtensions` False,
  `VMMonitorModeExtensions` False; *A hypervisor has been detected*). A
  Hyper-V-isolated container cannot run here.
- **Where *Windows 10 1903* comes from: unrun.** It is a recollection of
  Microsoft's page *Use UTF-8 code pages in Windows apps*, not fetched in
  this sitting (no web tool in this seat; Spotlight and five toolchain trees
  on this Mac hold no copy). What the box's own files say: the SDK defines
  `ActiveCodePage` in the AppX schema's **`uap8`** namespace
  (`Include/10.0.26100.0/winrt/UapManifestSchema_v8.xsd:41-47`, one value,
  `UTF-8`, referenced from `UapManifestSchema_v7.xsd:61`), and the fusion
  manifest's element lives in `http://schemas.microsoft.com/SMI/2019/
  WindowsSettings`. Neither names a build.

## Two of the compiler-engineer's open conditions, narrowed (16:02 to 16:05)

**The CI's clang on the `.rsrc` object.** The CI's clang is 20.1.8 (the
critic, from run 37196853219's log). This Mac's `silkeh/clang:20` image holds
**Debian clang 20.1.8**, a different build of that version. The manifest's
section variable and the `asm` tree, cut verbatim from c-dirwide's
`codepage.c` into a file needing no `windows.h` (`rsrc20.c`), assemble there
for `x86_64-pc-windows-msvc`: exit 0, `.rsrc$01` 88 bytes with one
`IMAGE_REL_AMD64_ADDR32NB` to `hero_codepage_manifest` at `0x48`, `.rsrc$02`
369 bytes. That object, linked on the box with `cp.c`: **lld-link 23.1.1,
exit 0, `GetACP` 65001; link.exe 14.44, exit 0, `GetACP` 65001**, one
`RT_MANIFEST` of 368 bytes each (`rsrc20.sh`). What stays unrun is the CI's
own `link.exe` build (image `windows-2025-vs2026`) and the CI's own clang
binary: a dispatch, an outward act.

**A narrow door other than the listing returning a name into a byte-sized
buffer.** From the critic's measured list of 40 lines (the Windows view,
`unifdef`, re-read by me as `<scratchpad>/191-fp-work/winview/`), the narrow
calls that **return** a name are four: `FindFirstFileA` and `FindNextFileA`
(made wide by c-dirwide), `GetModuleFileNameA` (`fs.c:211`) and
`GetFinalPathNameByHandleA` (`replace.c:118`). The last two are given
`HERO_FS_PATH_MAX`, 4,096 bytes, and each refuses an answer that fills the
buffer, so a name too long for them is a loud *no answer*, never a cut one.
By reading; not run with a path past 4,095 bytes of UTF-8.

**Both forms off Windows.** `unifdef -U_WIN32` with `-D__APPLE__`, and again
with `-D__linux__`, over c-dirwide's five files and a-object's: byte-equal
views, all five, both platforms. So my a-object runs on this Mac and on Linux
arm64 stand for c-dirwide.

## Q3 from the box: which cases can be red at `7f4c0cc5`

Measured above, and they agree with the compiler-engineer's W1 to W18 (a C
helper that makes, passes and reads names wide, at test time):

- **A case whose name, launch and observer all stay inside Heroes cannot be
  red at the base**: names `mk.exe` made, launched by `launch.exe`, read
  `check` 0, `probe` 0, `mutate` 0, the arguments' bytes right, the reads
  ok. And the name `write_file` gave is wrong only to a wide reader.
- **A tracked file is named wide** (git clone, measured), so a tracked case
  passed by the harness's own `CreateProcessA` is red at the base: *cannot
  read `../café.hero`* for git's names. Tracked paths above ASCII are not
  needed, and the helper's way keeps `git ls-tree` at 0 such paths.
- **Q-g's long name cannot be tracked**: Linux refuses it, *File name too
  long*, `NAME_MAX` 255 bytes, a 306-byte name (the arm64 image; APFS took
  it). It has to be made at test time on Windows, as W13 does.

Three shapes I would add to the eighteen, each measured red at the base:

| case | at `7f4c0cc5` | under the manifest | why it earns a case |
|---|---|---|---|
| **W19**, the fullwidth decoy: `check ..／x.hero` from a folder whose parent holds an `x.hero` with an error | **exit 1, `type_mismatch` at `../x.hero:2:14`**: a file in another directory | exit 0 (route (a1), the same narrow doors) | the sharpest row: best fit crosses a directory |
| **W20**, a built program reading the file its argument names, `ŝ.txt`, an `s.txt` beside | **read `decoy`** | read `the real one` (a1) | 238's *read as another file's* inside a user's program, not the compiler |
| **W21**, a bound C library opening the program's `cstr` with its own `fopen`, `/MD` DLL and `/MT` library (`lib191`) | **-1**, both | **6**, both, under (a1) and under c-dirwide | **the only case that tells (a) from (b)**: (b) alone leaves it -1 |

## Verdicts, in the charter's form (written 16:06)

Each argument counted by `wc -w`: 119, 116, 115, 115
(`<scratchpad>/191-fp-work/verdict/`).

### Q1. Soundness, on the box

- `verdict`: **approve** c-dirwide, the manifest inside the runtime's object
  with (d) and the directory door wide, as sound on every row and shape I
  ran; **object** to (a) alone in any carrier (Q-g, silent); **object** to
  (b) alone (Q-l); **refuse** (g) and (a′) on their measurements; **object**
  to (e) and (f). No veto: no route changes how a value crosses to C.
- `section`: design.md §1.11 (`:542`, *FFI ergonomics rank alongside
  comprehension*), §1.12 (`:579`, *a guarantee that ends quietly is not
  one*), §4.19, §4.20.
- `experiment`: every row of 238 and of 189's list, three makers by four
  launchers, at the base, under (a1) and under c-dirwide; `lib191` as a `/MD`
  DLL and a `/MT` library, bound from Heroes, under three compilers; `g.c`
  for (g); `wargv.c`; `ffa2.c` for Q-g. clang 23.1.1 accepted every one.
- `argument`: Every row of 238 is red at 7f4c0cc5 for a name from the wide world under Git Bash, PowerShell and cmd, every row but the arguments' under a Heroes launcher, and green where name and launch stay inside Heroes. Only a process code page reaches a bound library: lib191's own fopen, as a /MD DLL and a /MT library, reads the program's cstr as 1252 at the base (-1 three times) and right under the manifest (6, 13, 4). So (b) alone leaves §1.11's boundary broken for every binding taking a path. The manifest alone opens a silent hole, one seed of three; the wide listing closes it, three of three. A lone-surrogate argument stays U+FFFD under every (a).
- `prediction`: **`examples/ledger`'s SQLite binding opens a database at a
  path above ASCII on Windows under every route, the base included, with no
  shim**, because `sqlite3_open` takes UTF-8 and widens it itself; **a
  binding whose library opens a path with `fopen`, as zlib's `gzopen` does,
  opens it only under a route carrying the manifest**, and under (b) alone
  needs a shim per call. Unrun: no SQLite or zlib for Windows on the box.
- `condition`: a row or shape red under c-dirwide on the box; a bound
  library measured to read a narrow path in a way the process code page does
  not reach (then the manifest's §1.11 argument weakens); or a supported
  Windows below the floor that users must run (then (b) whole, the manifest
  kept for libraries where it is honoured).

### Q2. Cost, from the C side

- `verdict`: **approve** the manifest carried inside the runtime's object;
  **refuse** (a′) (`LNK1158` under link.exe); **object** to a `.res` on the
  link line, (a1) or (a″), as the default.
- `section`: design.md §1.11 (`:542`), §4.20 (`:2600`, the runtime every
  program links).
- `experiment`: the carriers under lld-link 23.1.1 and link.exe 14.44; a
  program's own string table and a second manifest against each carrier; the
  box's 415 libraries scanned for `.rsrc`, with a positive control; clang
  20.1.8's object linked by both; (d) with the manifest cut. All accepted by
  clang, the refusals by the linkers, by name.
- `argument`: The carrier decides who can forget it. Inside the runtime's object the manifest reaches heroes build, the seed's line, the CI's two and an --emit-c author's own line with none of them changed, and (d) shares that object, so its sentence cannot lie; with a .res on the link line, a link that forgot it makes (d) say the program carries a code page it does not, measured. The XML route dies under link.exe, LNK1158. The price, a program's own resource, is refused loudly by both linkers and touches no binding: none of 415 libraries on the box carries a resource section, and a DLL keeps its own. A second manifest is refused under every carrier.
- `prediction`: at the batch that lands 238 in this form, **the box's leg
  builds every `examples/` program that builds there today with no linker
  error naming a resource**, and the first message naming one comes the
  first time a program asks for an icon or a manifest entry of its own.
- `condition`: a binding or a roadmap program that needs a resource or a
  manifest entry of its own (comctl32's visual styles, an icon); or the CI's
  own link.exe refusing the `.rsrc` object on a dispatch. Then a `.res`
  written by the compiler (a″), merged by `cvtres` with the program's, and
  (d) moved beside it.

### Q3. The instruments

- `verdict`: **approve** cases whose names, launches or observers come from
  the wide world, the compiler-engineer's eighteen with W19 to W21 above;
  **object** to any case made, launched and read inside Heroes.
- `section`: design.md §1.12 (`:579`); §4.19 for W21, a binding's own case.
  The document has no rule that a case be red before its repair: the brief
  asks it, and the critic found none.
- `experiment`: the narrow maker and the Heroes launcher at the base (green),
  git's checkout (wide), the 306-byte name on Linux (refused).
- `argument`: At 7f4c0cc5 a name a Heroes program makes, handed on by a Heroes launcher, reads green on every row: check, probe, mutate, the arguments' bytes, the reads. CreateProcessA turns UTF-8 into mojibake and the child's C runtime turns it back. So a case is red at the base only if its name, its launch or its observer comes from the wide world: git checks a tracked file out wide, and only a wide reader sees cafÃ©-out.txt. The bound-library case is the one that tells (a) from (b). A long name cannot be tracked, since Linux refuses a 306-byte name, and the fullwidth decoy is the sharpest row: the base judged a file in another directory.
- `prediction`: **W21 reads -1 against `7f4c0cc5`'s runtime and 6 against
  c-dirwide's, and -1 against any runtime that makes the doors wide without
  the manifest**; checkable on the box with `lib191` as it stands.
- `condition`: a case made, launched and read inside Heroes shown red at
  `7f4c0cc5`: then the self-consistency I measured is wrong.

### Q4. Platforms and the floor

- `verdict`: **approve** (d) as c-dirwide carries it, in the manifest's own
  object; **approve** Linux and macOS as unchanged, measured; the floor
  stays **unrun**, and a number for it should be written only once run or
  cited.
- `section`: design.md §1.12 (`:579`).
- `experiment`: (d) on code page 1252 (exit 2, stdout 0 bytes, stderr 256);
  the a-object runtime's own tests on this Mac and Linux arm64; the
  instrument below the floor, unavailable on this guest; the SDK's `uap8`
  schema.
- `argument`: Off Windows nothing moves: with the a-object runtime the compiler's own tests read 1,190 passed on this Mac and on Linux arm64, an ASCII program's emitted C is byte-identical, and c-dirwide's POSIX view equals a-object's in all five files. On Windows the floor is a documentation fact nobody here ran: the box and the CI are build 26100, and this guest cannot host an older one. Below it the manifest is ignored, and every row of the base comes back in silence. (d) makes that one ASCII sentence and exit 2, run here on code page 1252, and true by construction because it shares the manifest's object. Its price is every Heroes program refused there.
- `prediction`: on a Windows 10 1809 or Server 2019 machine (build 17763),
  every Heroes program built in this form prints (d)'s sentence naming its
  code page and exits 2, the compiler included. Unrun: no such machine here.
- `condition`: the manifest measured honoured below 1903 (then the floor
  moves), or a supported edition below it that a user must run (then (b)
  whole, as in Q1's condition).

### Handed on

- **Q-c, to panel 192**: spec `:324-325` is already false at `7f4c0cc5`
  (`x?y`), stays false under every (a) (`x` U+FFFD `y`), and can be made true
  by the UCRT's wide `argv`, which two seats ran: `_configure_wide_argv`
  returns 0 under the static CRT and `/MD`, and `WC_ERR_INVALID_CHARS`
  refuses the lone surrogate with 1113. Whether it should is what a `str` may
  hold. One side effect for a bound library: the call rewrites the CRT's
  global `__argc`.
- **Q-i, to panel 192**: with the compiler-engineer. Every door here,
  `hero_win_wide` included (length -1), stops at an interior NUL; only a door
  that takes the `str`'s length can refuse it.
- **The console, named**: `caf├⌐ ┼¥` under a real console for every route,
  the console's output code page 437 untouched by the manifest. Not repaired
  here.

## What I built and ran, and my cost

- **The box**: one folder, `/c/w/191-fp-win/`, 643 MB by `du -sm` at 16:05
  (`du` itself could not open the long and lone-surrogate names, which MSYS
  does not name); 22,557,872 KB free on `C:` then. Heavy builds there, one at
  a time: the trunk compiler from its seed (14:49), the probe compiler's
  object (15:08), c-dirwide's compiler (15:54). One stuck run of mine
  stopped by `taskkill` of its own tree (15:26). **Nothing removed**,
  there or here.
- **This Mac**: my copy `<scratchpad>/191-ffi-pragmatist/`; the probe tree,
  the a-object and c-dirwide constructions, every script and output under
  `<scratchpad>/191-fp-work/`. Two own-test runs (the a-object compiler here,
  and in the container).
- **Containers**: four, one at a time, `docker ps -q` empty before each:
  `heroes-linux-arm64` three times, `silkeh/clang:20` twice.
- **Not run**: Windows below the floor (no instrument); the CI's own clang
  binary and link.exe on the `.rsrc` object (a dispatch, outward); the
  console rows under `cmd` and PowerShell (both started interactively under
  `ssh -tt`); a route (b) build (none exported, so Q-l's (b) column is
  inferred from the mechanism); a symbolic link past 259 bytes (the
  compiler-engineer ran it, 16:00); any timing; any paid run. In the trunk I
  wrote nothing but this file.

## Sentences corrected in place while I wrote, listed so none is lost quietly

The file is untracked (`git status`), written as I go; each correction was
made before anyone read it, and is listed here so the record keeps what it
said.

| where | it said | it says, and why |
|---|---|---|
| the makers' paragraph (14:59) | the byte `0x9D` *still round-trips (as U+009D)* | *lands on disk as U+009D (whether it reads back is run below)*: only the write had been run. It was then run: `probe only2/` over the narrow names read 1 seed, exit 0 (15:00) |
| Q1's `argument` (16:06) | every row red *under Git Bash, PowerShell, cmd and a Heroes launcher alike* | *every row but the arguments' under a Heroes launcher*: through that launcher the arguments' bytes were right even with wide names (section C) |
| the W19 to W21 table (16:06) | a column headed *under c-dirwide* | *under the manifest*: W19 and W20 were run under (a1), whose narrow doors and code page are c-dirwide's; W21 under both |
| the heading *Q1-a: is each row red at `7f4c0cc5`? (in progress)* | *in progress* | finished at 15:02; the heading is left as written, and this row is its correction |
| the a-object paragraph (15:30) | `codepage.c` *(82 lines)*, from its `README.txt` | **81 by `wc -l`** (16:08), the exported file and my applied copy alike; the compiler-engineer's own report corrects the same number |
| my cost (16:06) | containers: *four* | **five**: `heroes-linux-arm64` three times (the two runtime comparisons and the 306-byte name), `silkeh/clang:20` twice |
| the (a1) column's `probe long/` row (15:31) | **2**, written before its exit was captured | **2**, *no `.hero` file under long*, now run (`exitlong.sh`, 16:08); `probe long2/` under (a1) exit 0, *1 seeds*; c-dirwide's `probe long/` exit 0, *1 seeds* |
