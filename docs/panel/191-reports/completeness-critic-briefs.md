# Panel 191, the completeness critic's first pass, over the briefs

Opened 2026-10-04 14:08 by `date`; written from 14:34. My role names what is
missing and gives no verdict.

## What I worked from

- **The briefs**, read whole as they stood at 14:08 (`ls -la
  docs/panel/191-briefs/`): `00-shared.md`, `compiler-engineer.md`,
  `ffi-pragmatist.md`, `completeness-critic.md`.
- **My copy**, `<scratchpad>/191-critic/`, made by `git -C <trunk> archive
  7f4c0cc5 | tar -x`. Its seed's sha256 begins `26ccaa9d96478a20`
  (`shasum -a 256 seed/heroes.c`), as the brief says. My compiler was built
  from it between 14:17:35 and 14:17:40 and prints `heroes 0.2.0`.
- **My scripts and cross-links** are in `<scratchpad>/191-critic-work/`:
  `count.sh`, `crt.sh`, `winview.sh`, `doors.sh`, `link/`.
- **Cross measurements on this Mac**, with Homebrew `llvm@22` (clang 22.1.8,
  `llvm-rc`, `llvm-readobj`) and `lld@22` (`lld-link`). There is no Windows
  SDK or CRT here. So they settle what this linker and this driver do, never
  what the box's do.
- **Not used**: the Windows box, Docker, any paid run, any lane's worktree.
  Nothing was removed. The only thing I wrote in the trunk is this file.

## 1. Framing facts re-run and found as written

| fact in the brief | command | result |
|---|---|---|
| `7f4c0cc5` is batch 9's round head, with panel 190's sitting | `git branch -a --contains 7f4c0cc5`; `git log -1` | the head of `lane-round-b9` (and of the three `lane-b10-*` branches), *Panel 190 sat*. **It is not on `main`** (`main` is `f6a3122e`, `git merge-base --is-ancestor` false) |
| its runtime is batch 9's closing runtime | `git diff --stat 38d6c6b1 7f4c0cc5 -- runtime/` | empty |
| convened by *6b*, soundness lane, after 190 | the log entry's item 6 | as written |
| 238's rows | `docs/work/defects/238-*.md` at the frozen tree; `docs/panel/189-reports/ffi-pragmatist.md:99-117` | the five rows match, carried from `7d9f2e8f` |
| `hero_args_set` at `os.c:496` | `grep -n` | yes; the generated `main` calls it first (`selfhost/emit/decls.hero:207-208`, `:242-243`; the seed's `main` at `seed/heroes.c:1384216`) |
| `link_flags()` at `:168` to `:194` | `grep -n` | yes. Its only non-test caller is `link_line` (`selfhost/cli/link.hero:132`), the one link line the compiler writes |
| the seed's Windows line | `seed/README.md:17` | yes |
| 239 at `dir.c:213`; 243's `hero_env_shown`; 277 at `fs.c:261`; 240 is `59e17a14` | `grep -n`; `git merge-base --is-ancestor 59e17a14 7f4c0cc5` | yes, `hero_env_shown` at `os.c:845`, 240 inside the tree |
| `CreateProcessA` at `run.c:687` | `grep -n` | yes |
| no `CP_UTF8`, `_wfopen`, `FindFirstFileW`, `GetCommandLineW`, `wmain` or `activeCodePage` in `runtime/` | `grep -rl -F` | true. Also none in `selfhost/`, `tests/`, `.github/`, `tools/` or the seed, and none of `MultiByteToWideChar`, `WideCharToMultiByte`, `CommandLineToArgvW`, `SetConsoleOutputCP`, `GetACP` |
| the table's counts | `grep -h -F '<call>' runtime/*.c runtime/*.h runtime/parts/*.c \| grep -v -E '^[[:space:]]*(//\|/\*\|\*)' \| wc -l` | **all 17 rows reproduced exactly** by this comment filter, which the brief does not state. But see F2 |
| panel 189's route files | `ls <scratchpad>/189-ffi-pragmatist-cases/win/route/` | present, 7 files |
| the box is a Hyper-V machine | `docs/panel/190-reports/ffi-pragmatist.md:1133-1134` | yes, event 1801, and memory read at 4,256,112,640 then 8,588,828,672 bytes. The phrase *grows with demand* is § 27's (`:1296`), not § 24's |

No direct C name call sits outside `runtime/`. The `extern` groups in
`selfhost/` and `tests/harness/` bind only `hero_os.h`,
`heroes_runtime.h` and `stdlib.h` (`atof` and three `hero_dir_*`). So the
runtime holds every door, as the brief assumes.

## 2. Facts false, partial, or not verifiable

**F1. *Batch 9 changed three of the runtime's Windows doors* undercounts what moved under row 4.**

`git log --format='%h %s' 7d9f2e8f..7f4c0cc5 -- runtime/` lists **seven**
commits: 273 `df453108`, 274 `34836e73`, 275 `cf7d6efa`, 227 `5e4f2efb`,
243 `ea5c22ca`, 239 `5ba5296c` and 277 `7f875977`.

Two of them, 273 and 274, change `read_file`, which is the door of row 4:
- 273 adds a Windows call, `_fstat64` on the open file (`os.c:612`).
- 274 answers `file_not_found` only for `ENOENT` or `ENOTDIR` (`os.c:749`).

My inference, unrun: row 4 still reads `file_not_found` at `7f4c0cc5`,
because the narrow `fopen` of the name read in code page 1252 fails
`ENOENT`. The re-run should say whether it does.

**F2. The table counts substrings, not calls, and leaves out six narrow calls.**

I took the Windows view with `unifdef -D_WIN32 -U__APPLE__ -U__linux__`. The
runtime branches only on those three platform macros plus toolchain macros,
which I checked with `grep -h -E '^[[:space:]]*#[[:space:]]*(if|elif)'`. I
then counted with a word-boundary pattern,
`(^|[^A-Za-z0-9_])<call>[[:space:]]*\(`.

| call | brief | compiled on Windows | where, or why it differs |
|---|---|---|---|
| `fopen(` | 2 | 2 | `os.c:747`, `:857` |
| `getenv(` | 2 | **1** | `os.c:847`; `str.c:416` is inside a block comment |
| `remove(` | 8 | **2** | `fs.c:162`, `:186`; the other six are `hero_fs_remove` |
| `rename(` | 4 | **0** | two are `hero_fs_rename`; `fs.c:228` and `replace.c:603` are POSIX only |
| `stat(` | 12 | **0** | all POSIX (`stat`, `lstat`, `fstat`) |
| `mkdir(` / `_mkdir(` | 2 / 1 | 0 / 1 | the `mkdir(` count includes the `_mkdir(` line, `fs.c:72` |
| `chmod(` | 2 | **0** | one is `fchmod`; both are POSIX |
| `CreateFileA` | 9 | 9 | one, in `run.c`, opens the constant `"NUL"` |
| `GetFileAttributesA` | 7 | 7 | |
| `SetFileAttributesA` | 3 | 3 | |
| `MoveFileExA` | 2 | 2 | |
| `DeleteFileA` | 2 | 2 | |
| `FindFirstFileA` | 2 | 2 | `dir.c:213` and `replace.c:106` |
| `FindNextFileA` | 1 | 1 | |
| `GetModuleFileNameA` | 1 | 1 | |
| `CreateProcessA` | 1 | 1 | |
| **not in the brief**: `RemoveDirectoryA` | none | 2 | `fs.c:177`, `:186` |
| `GetFileAttributesExA` | none | 2 | `fs.c:126`, `:127` |
| `GetFinalPathNameByHandleA` | none | 1 | `replace.c:136` |
| `CreateHardLinkA` | none | 1 | `replace.c:705` |
| `CreateSymbolicLinkA` | none | 1 | `replace.c:724` |
| `CreateJobObjectA` | none | 1 | `run.c:675`; the name is `NULL`, so not a door |

The `...A` names came from the world rather than from a list:
`grep -o -E '\b[A-Z][A-Za-z0-9]*[a-z0-9]A[[:space:]]*\('`.

The brief's table sums to **60** lines. The Windows view holds **40**
distinct lines that call a narrow name API (`doors.sh`):
- `replace.c` 20, the in-place writer that row 2's `fmt --in-place` goes through;
- `fs.c` 10;
- `run.c` 5;
- `os.c` 3;
- `dir.c` 2.

Route (b)'s door-by-door price is over these 40 lines. The types
`WIN32_FIND_DATAA` (`dir.c:212`, `replace.c:105`) and `STARTUPINFOA` move
with them.

**F3. *A seven-line application manifest*: `wc -l` reads 8 lines and 368 bytes.**

That is `<scratchpad>/189-ffi-pragmatist-cases/win/route/utf8.manifest`. The
brief carries the count from panel 189's report (`:272`). Small, but it is
CL-077's shape exactly.

**F4. *Which lld-link takes*: the linker is unverified, and the tree contradicts itself on it.**

- **The driver.** On this Mac,
  `clang --target=x86_64-pc-windows-msvc -### t.c -o t.exe` (clang 22.1.8)
  gives **`link.exe ... -defaultlib:libcmt`**: MSVC's linker and the static
  CRT. With `-fuse-ld=lld` it gives `lld-link`.
- **The tree says link.exe on the CI.** `selfhost/cli/flags.hero:181-191`
  records MSVC `link.exe`'s incremental-link message on the CI's Windows leg
  (2026-08-31).
- **The tree says lld-link elsewhere.** `selfhost/emit/ffi_build.hero:67` calls
  lld-link "the Windows linker", and
  `docs/ref/environment/windows/WINDOWS-MACHINE.md:395` records lld-link's
  error on the first box.
- **The two clangs differ.** The CI's clang is **20.1.8**, from
  `C:\Program Files\LLVM\bin`, on image `windows-2025-vs2026` 20260925.250.1,
  Windows Server 2025 Datacenter 10.0.26100. I read these from run
  `37196853219`'s Windows log with
  `gh run view --job 111420424790 --log`. The box's clang is 23.1.1.

Which linker each one uses decides how a `.res` is consumed (cvtres under
`link.exe`) and whether R1 below applies.

**F5. The batch 9 changes the brief names are repaired, not closed.**

**239, 240, 243 and 277 are open at `7f4c0cc5`** (`ls docs/work/defects/`).
Each reads *Repaired at ..., the net is owed at the batch's close*. Their
Windows cases are owed to batch 9's leg, the leg now holding the box. That
leg is still running: `0470afd5` (13:55) says the box's annotations *is
re-read ... once its leg's loop ends*.

So *its runtime is batch 9's closing runtime* holds only if that leg is
green on those four. A red there moves `runtime/` after `7f4c0cc5`.

The future half is unverifiable:
- *No lane of batch 10 edits `runtime/`* holds for committed work at 14:30.
  `git diff --stat 7f4c0cc5 lane-b10-{cli,harness,ir} -- runtime/` is empty,
  and `lane-b10-ir` holds one commit, `075b425d`, touching no runtime file.
- The lanes' plans are written nowhere in the tree. I searched with
  `git grep -i -E 'b10-cli|b10-harness|b10-ir|lane b10|batch ten'` over
  `docs/` and `.claude/`, at `7f4c0cc5` and at `main`.

**F6. The box's facts: not re-run (the box is held), and one is missing.**

The brief carries the ACP alone. Panel 189 read **OEMCP 437** and the SSH
session's console as **IBM437** (`189-reports/ffi-pragmatist.md:30-38`).
Q1's console shape needs both.

**F7. The CI half of Q2 cannot be measured in this sitting as briefed.**

*Whether llvm-rc is on the CI's Windows runner* needs a CI run, and a CI run
needs a push or a `workflow_dispatch` (`ci.yml:282`) on the public
repository: an outward-facing act, asked for every time. No CI step asks
for it today (0 mentions of `llvm-rc` in that run's 2,066-line log). It must
come back unrun, or as a dispatch put to the author with its size.

**F8. No design.md section is cited anywhere in the briefs** (CLAUDE.md § 1).

The routes rest on:
- §1.11 (`design.md:467`), the founding constraint;
- §1.12 (`:575`), robustness and a complete boundary;
- §4.19 (`:2273`), FFI;
- §4.20 (`:2600`), the runtime, in C.

One sentence of §1.12, *"a guarantee that ends quietly is not one"*, bears
directly on route (a)'s floor. Below it, (a) does not fail: it silently
reads 238's rows again.

**F9. The transfer script the seats are told to imitate removes things.**

`<scratchpad>/platforms/windows2-u.sh` runs `rm -rf` on both machines, at
`:28`, `:29`, `:39` and `:52`. Its pattern, copied, breaks *never `rm`* and
*nothing removed there*. The brief should say which lines are not to be
copied.

## 3. Routes nobody listed

**R1. (a′) The manifest handed to the linker as XML, with no `.res` and no resource compiler.**

The flags are `-Wl,/MANIFEST:EMBED -Wl,/MANIFESTINPUT:<file>`. I measured
this cross, on a no-CRT object, `lld-link /nodefaultlib /entry:main`,
reading each result with `llvm-readobj --coff-resources`:

| link | RT_MANIFEST, ID 1 | what it holds |
|---|---|---|
| 189's `utf8.res` | 368 bytes | the file verbatim |
| `/manifest:embed /manifestinput:utf8.manifest` | 579 bytes | merged with lld's default `trustInfo asInvoker` |
| the same plus `/manifestuac:no` | 328 bytes | `activeCodePage` alone, re-serialised |

This removes `llvm-rc` from route (a)'s cost. Three things are unrun:
- (a′) on the box's lld-link;
- (a′) with `link.exe`;
- whether the box's lld-link merges without `mt.exe`. Homebrew's build here
  merged by itself. That a build without libxml2 falls back to `mt.exe` is my
  recollection of lld's source, unverified.

**R2. (a″) The compiler writes the `.res` itself.**

`llvm-rc`'s output for 189's files is a fixed **432 bytes**: two runs were
byte-identical by `cmp`, sha256 beginning `178d9a7f2a47c56c`. A `.res` on
the clang line reaches the linker as is (`-###`), and lld-link took it
(`a.exe` above); `link.exe` takes it through cvtres, unrun.

So the compiler could carry the bytes and write them into `build/`. No new
tool would be needed on a user's machine, and `heroes doctor` would have
nothing new to check. Today `doctor` checks no linker and no resource tool on
Windows: it asks `uname -s`, then `cc --version`
(`selfhost/cli/doctor.hero:108`, `:116`).

**Closed by measurement: the runtime's own object carrying the options.**

lld-link 22.1.8 refuses `#pragma comment(linker, "/manifest:embed")` and
`"/manifestinput:..."`, saying *`/manifest: is not allowed in .drectve`*. I
did not try `link.exe`.

**R3. (g) The UCRT's UTF-8 locale.**

The call is `setlocale(LC_CTYPE, ".UTF8")` in `hero_args_set`. That UCRT
10.0.17134 and later reads narrow file names as UTF-8 under it is my
recollection, unverified.

Its reach, if it works:
- it reaches only the program's own CRT's narrow calls: `fopen`, `remove`,
  `_mkdir`;
- it does not reach the **35** Win32 `...A` lines of F2;
- it does not reach `argv`, which is built before `main`;
- it does not reach a bound DLL's own CRT. The program links `libcmt`
  statically: the driver's default, measured above, and the compiler's flags
  name no CRT (`grep` of `flags.hero` and `link.hero`).

Any bound library calling `setlocale(LC_ALL, "")` would also undo it. The
runtime's own `parts/f64.c:47-52` makes exactly that argument against
process-wide locale state. I name it so that the sitting refuses it on a
measurement, not by omission.

**R4. An instrument for the floor.**

Both instruments are build 26100: the box and the CI's image (F4). Neither
can run below 1903. A Hyper-V-isolated `servercore:ltsc2019` container
(build 17763 by my recollection) on the box would need nested virtualisation
in that Hyper-V VM and the Containers feature. All of it is unrun and
unknown. Otherwise the floor stays unrun, and the brief should say so before
the seat looks.

## 4. Questions the sitting should ask and does not

**Q-a. Is each Q3 case red at `7f4c0cc5` on the box?** This is the question I
would put first.

A Heroes program's narrow world looks self-consistent under code page 1252.
- `write_file("café.txt")` makes `cafÃ©.txt`, and `read_file("café.txt")`
  opens it again.
- The harness, itself a narrow Heroes program, passes `ŝ.hero` through
  `CreateProcessA`. The child's CRT hands the same UTF-8 bytes back.

So a case that makes its own names at run time may pass on the unrepaired
tree. That is my inference. It rests on Windows' 1252 tables mapping every
byte both ways, a recollection to be run. Panel 189's rows were found only
with names from the wide world: PowerShell's `[IO.File]`, and arguments
from Git Bash.

Three facts make this sharper:
- No tracked path is above ASCII today:
  `git ls-tree -r --name-only 7f4c0cc5 | LC_ALL=C grep -c '[^ -~]'` gives 0.
  A tracked case would be the first. The harness's walk does ask git with
  `-z` (`tests/harness/shell.hero:445`).
- I found no rule requiring a case to be red before its repair. I searched
  `.claude/rules/`, `/step` and `/panel` for *red before*, *fails before*,
  *shown red*, *without the repair*, *before the repair*, *against the base*.
- The settling run on the box: one name made by a Heroes program, one by
  PowerShell, each through `heroes check` and through a built program, at
  `7f4c0cc5`.

**Q-b. Which linker, on the box, on the CI and on a user's default (F4), and does each route work under both?**

**Q-c. Does `spec/heroes-spec.md:324-325` stay true on Windows?**

The sentence is *`args()` ... one that is not UTF-8 aborts*. My inference,
from recollection of `WideCharToMultiByte` and unrun: under (a) an unpaired
surrogate converts to U+FFFD. That is valid UTF-8, so `args()` gives a wrong
value and never aborts, which is 238's own class. Under (b), the runtime
decides.

With a spec sentence behind it, the panel skill (`SKILL.md:33-34`) makes
this shape a full-panel question. The sitting should say whether it is.

**Q-d. What about links `heroes` does not write?**

There are four such lines:
- `--emit-c` (`selfhost/cli/table.hero:100`), whose C a user links by their
  own line;
- the seed's line;
- the CI's two seed lines, `ci.yml:428-429` and `:471`.

Under (a) they are silent, under (d) they refuse, under (b) they work.
`ci.yml` is also read by `site/src/lib/claims.ts`, so a change there owes
the site's build before the push.

**Q-e. Can route (e) refuse at run time at all?**

Narrow `argv` already holds the ASCII `s` for `ŝ`, so best fit cannot be
seen from inside. A run-time refusal therefore needs `GetCommandLineW`,
which is (b)'s door.

Two of the routes also carry messages:
- (e) at check time is a new diagnostic, so not the soundness lane;
- (d)'s message needs its stream, its exit code, and ASCII-only text, to be
  legible under any console code page.

**Q-f. Best fit to ASCII punctuation, row 2's worst case.**

From recollection of Microsoft's 1252 best-fit table and the WorstFit
disclosure of January 2025, all unverified, best fit turns:
- `／` (U+FF0F) into `/`;
- `＼` (U+FF3C) into `\`;
- `＂` (U+FF02) into `"`.

*Read as another file's* would then become a file in another directory, or
an extra argument through `run.c`'s command line. To run at `7f4c0cc5`:
`heroes check ..／x.hero`, and a program's `args_checked()` over `a＂b c`.
Both (a) and (b) close it. (e) cannot see it, and (f) leaves it.

**Q-g. Long names under (a).**

`WIN32_FIND_DATAA.cFileName` is `MAX_PATH` bytes by my recollection of
`minwinbase.h`; `grep` it on the box. A name of 100 × U+6F22 is 300 UTF-8
bytes in 100 UTF-16 units. What do `dir.c:212-213` and `replace.c:105-106`
return for it under (a)?

**Q-h. Route (b)'s argument door is not free in any of its three forms.**

- **`CommandLineToArgvW`** adds `shell32` to every link. It also allocates
  by `LocalAlloc`, outside rule 1's C family
  (`tests/harness/suite_runtime.hero:25-27`), which that rule cannot see.
- **`wmain`** moves emission (`decls.hero:207-208`, `:242-243`) and the
  seed.
- **A parser of `GetCommandLineW` by hand** must match the CRT's split, or
  an ASCII program's `argv` moves.

In every form, `hero_args_raw` (`os.c:554-558`) lends `main`'s `argv`
borrowed today, and under (b) the runtime must own the copies.

**Q-i. Where this sitting meets sitting 192.**

`read_file` and `write_file` lend `path.cstr()`
(`selfhost/library_source.hero:208`, `:222`). So defect 245's interior NUL
opens a shorter name at the runtime's own doors, on every platform. A door
converting a `const char *` cannot see the NUL. Only a door taking the
`str`'s length can refuse it. Which sitting owns that refusal?

**Q-j. What is today's floor?**

None is stated anywhere: no `_WIN32_WINNT`, `WINVER`, *1903* or *Windows
10/11* outside generated `#line` numbers, by `grep -rn` over the tree. The
runtime's Win32 calls are old: Vista-era, by recollection. So (a) sets the
project's first floor, and silently.

Which supported Windows editions fall below 1903? From recollection,
unverified: Server 2019 and LTSC 2019 at 17763, Server 2016 at 14393. No
seat is convened who verifies this; the historian is not seated.

**Q-k. The launcher and the console.**

Panel 189's probes went through Git Bash (MSYS2). That launcher converts its
UTF-8 to UTF-16 itself, and 189 recorded *a byte the MSYS command-line parser
had made*. A user is in PowerShell 5.1, `cmd` or Explorer. And the console
read over `ssh win` is a pseudo-console. One row per launcher, and the
console named as read over ssh.

**Q-l. Vary the bound library's CRT.**

Run a `/MD` DLL on `ucrtbase` and a `/MT` static library sharing `libcmt`.
Under (a) they should agree. Under R3 they would not.

**Q-m. Rows panel 189 measured that the brief's list omits**
(`189-reports/ffi-pragmatist.md:102-107`, `:289`):
- `check dé/p.hero`, a directory above ASCII;
- `probe` and `mutate` over a folder holding `café.hero`, 239's door on
  Windows;
- `probe` over a folder holding `ŝ.hero`;
- `HEROES_RUNTIME` under `rté`, 243's door.

And one shape beside them: a project whose root folder is above ASCII, as a
user's profile often is. That puts every door on one path at once: the
`MoveFileExA` publish, `replace.c`, `run.c`'s redirects, `#line`, and
clang's `-o`.

## 5. Facts a seat is handed that it should measure itself

- **The linker** (F4). Both seats should run `clang -### t.c` on the box, and
  the route under `-fuse-ld=lld` and without it.
- **The ACP**, with the OEMCP and the console's code page, in the session and
  from a program (`GetConsoleOutputCP`). That is the ffi-pragmatist's job.
- **The floor, *Windows 10 1903***. It is a documentation fact carried from
  189 and never run. Cite its source or write unrun.
- **The table** (F2). Re-run it as the Windows view before pricing (b).
- ***lld-link takes it; no `mt.exe`***. That was 189's run with the box's
  default linker of that day. Re-run it.
- **The CI's runner**. Read it from a log (F4's command), not from `ci.yml`'s
  comment. Note that *"Windows carries clang with Visual Studio"*
  (`ci.yml:364`) no longer matches the log's
  `InstalledDir: C:\Program Files\LLVM\bin`.

## Not run

I did not run anything on the box or in Docker, any paid run, any route's
build, any suite, or any timing. Everything above marked *recollection* or
*inference* is unrun, and stays a question until a seat runs it.
