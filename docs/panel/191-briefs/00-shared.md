# Panel 191, the shared brief: how the runtime reaches a name on Windows (defect 238)

Written 2026-10-04 from 14:07 by the coordinator; **repaired from 14:38**
after the completeness critic's first pass
(`docs/panel/191-reports/completeness-critic-briefs.md`; the text it read is
`00-shared-before-the-critic.md`).

**The frozen tree is `7f4c0cc5`**: batch 9's round head with panel 190's
sitting. It is a commit of the repository, not yet on `main`. Every seat
works from `git archive 7f4c0cc5`, never from a working tree. Since the
critic's pass, batch 9's Windows leg has run green on that runtime (below),
and no committed line of batch 10's lanes touches `runtime/`
(`git diff --stat 7f4c0cc5 lane-b10-{cli,harness,ir} -- runtime/`, empty at
14:30, the critic).

**Convened** by the author's answer *6b* of 2026-10-04
(`docs/records/log/2026-10-04-1236-the-author-answers-1a-2a-3a-4a-5a-6b.md`): a
sitting of its own, after panel 190, in the **soundness lane**. The seats are
the compiler-engineer and the ffi-pragmatist, with the completeness critic.
**No paid run.** Every number below names the command that produced it; what
was not run says so.

**The design it rests on**:
- §1.11 (`docs/design/design.md:467`), the founding constraint: everything
  comes from C;
- §1.12 (`:575`), robustness and a complete boundary, whose *"a guarantee
  that ends quietly is not one"* bears on any route with a silent floor;
- §4.19 (`:2273`), the FFI;
- §4.20 (`:2600`), the runtime in C.

## Why this sitting

**Defect 238** (`blocking`, `docs/work/defects/238-*.md`): on Windows the
runtime reaches file names, arguments and the environment through the narrow
API. So a name above ASCII is refused, read as another file's, or written
under another name.

**Its rows**, carried from panel 189's ffi-pragmatist on the trunk's compiler
at `7d9f2e8f` (2026-10-04, `docs/panel/189-reports/ffi-pragmatist.md`):
- `heroes check café.hero` exits 2, *argument 2 is not UTF-8*.
- `heroes check ŝ.hero` judges an `s.hero` beside it, and `heroes fmt ŝ.hero
  --in-place` rewrites `s.hero`.
- A built program's `args_checked()` gives `ŝ` as `s`.
- `read_file("café.txt")` fails `file_not_found` for a file that is there.
- `write_file` names its file `cafÃ©-out.txt`.

**Rows of panel 189 the list above omitted** (`:102-107`, `:289`):
- `check dé/p.hero`, a directory above ASCII;
- `probe` and `mutate` over a folder holding `café.hero`, and `probe` over a
  folder holding `ŝ.hero`;
- `HEROES_RUNTIME` under `rté`.

**Re-running all of them on `7f4c0cc5` is owed.** **Seven** commits changed
`runtime/` since `7d9f2e8f`: 273 `df453108`, 274 `34836e73`, 275 `cf7d6efa`,
227 `5e4f2efb`, 243 `ea5c22ca`, 239 `5ba5296c` and 277 `7f875977`
(`git log --format='%h %s' 7d9f2e8f..7f4c0cc5 -- runtime/`, the critic). Two of
them, 273 and 274, changed `read_file`, which is row 4's door.

## Measured for this brief

**The runtime's narrow name calls, as Windows compiles them**: measured by the
critic with `unifdef -D_WIN32 -U__APPLE__ -U__linux__` and a word-bounded
count (`<scratchpad>/191-critic/doors.sh`). The `...A` names were found by
`grep -o -E '\b[A-Z][A-Za-z0-9]*[a-z0-9]A[[:space:]]*\('`, not from a list.

**40 distinct lines** call a narrow name API:
- `replace.c` 20 (the in-place writer that `fmt --in-place` goes through);
- `fs.c` 10;
- `run.c` 5;
- `os.c` 3;
- `dir.c` 2.

| call | lines on Windows | call | lines on Windows |
|---|---|---|---|
| `CreateFileA` | 9 (one opens the constant `"NUL"`) | `fopen(` | 2 (`os.c:747`, `:857`) |
| `GetFileAttributesA` | 7 | `remove(` | 2 (`fs.c:162`, `:186`) |
| `SetFileAttributesA` | 3 | `RemoveDirectoryA` | 2 |
| `MoveFileExA` | 2 | `GetFileAttributesExA` | 2 |
| `DeleteFileA` | 2 | `FindFirstFileA` | 2 (`dir.c:213`, `replace.c:106`) |
| `GetFinalPathNameByHandleA` | 1 | `FindNextFileA` | 1 |
| `CreateHardLinkA` | 1 | `GetModuleFileNameA` | 1 (`fs.c:261`) |
| `CreateSymbolicLinkA` | 1 | `CreateProcessA` | 1 (`run.c:687`) |
| `getenv(` | 1 (`os.c:847`) | `_mkdir(` | 1 |

`rename`, `stat` and `chmod` compile on POSIX only. `CreateJobObjectA`
(`run.c:675`) passes no name. `grep` finds no `CP_UTF8`, `_wfopen`,
`FindFirstFileW`, `GetCommandLineW`, `wmain` or `activeCodePage` in the tree.

**Where the arguments enter**: `hero_args_set(int argc, char **argv)` at
`runtime/parts/os.c:496`. `hero_args_raw` (`:554-558`) lends `main`'s `argv`
borrowed.

**What batch 9 changed at the Windows doors**:
- **239**: the directory walk reads each name through `hero_dir_at_shown`
  (still `FindFirstFileA` at `dir.c:213`), and a byte that is not UTF-8 is
  named, never a panic;
- **243**: an environment value goes through `hero_env_shown`;
- **277**: the executable's path through `GetModuleFileNameA`;
- **240**: a `#line` name carries no numeric escape, the clang 23 refusal that
  stopped panel 189's route at build.

**Batch 9's Windows leg** ran all four green on `38d6c6b1`, `7f4c0cc5`'s
code (14:40, `docs/records/done/2026-10-04-1440-defect-*`): the compiler's
tests 1,190 and 21 suites at 0 failed. Those cases make their names through
the narrow API themselves: 239's test has a small C program write the byte
`0xE9` into a name, and reads it back through `FindFirstFileA`. See Q1's
first question.

**The link.** On Windows, `selfhost/cli/flags.hero`'s `link_flags()` (`:168`
to `:194`) returns `-Wl,/STACK:67108864` and `-Wl,/INCREMENTAL:NO`. The
seed's Windows line is `seed/README.md`'s.

**Which linker clang uses is not settled, and the tree contradicts itself on
it** (the critic):
- **the driver's default**: `clang --target=x86_64-pc-windows-msvc -###`
  gives MSVC's `link.exe` with `libcmt`, the static CRT (clang 22.1.8 on this
  Mac), and `lld-link` under `-fuse-ld=lld`;
- **the tree's two sayings**: `flags.hero:181-191` records `link.exe`'s
  message on the CI's Windows leg, while `selfhost/emit/ffi_build.hero:67`
  calls lld-link *"the Windows linker"*;
- **the CI's machine**: its clang is 20.1.8 from `C:\Program Files\LLVM\bin`,
  on image `windows-2025-vs2026`, read from run 37196853219's log;
- **the box**: its clang is 23.1.1.

**The box.** Windows Server 2025 Standard, 10.0.26100, ANSI code page 1252:
`ver` and the registry's `Nls\CodePage\ACP`, read over `ssh win` at about
14:03. Panel 189 read OEMCP 437 and the ssh console as IBM437, carried. It is
a Hyper-V machine whose memory grows with demand (panel 190, § 24).

## Carried, not re-run: panel 189's route

From panel 189's ffi-pragmatist, section *A route for the Windows boundary,
compiled*; files in `<scratchpad>/189-ffi-pragmatist-cases/win/route/` (read
only; copy what you need).
- **The manifest**: `utf8.manifest`, 8 lines and 368 bytes by `wc`,
  declaring `<activeCodePage>UTF-8</activeCodePage>`. It was compiled by
  `llvm-rc` to `utf8.res`, a fixed 432 bytes (the critic, two runs identical
  by `cmp`), and handed to clang as one more input.
- **What it changed**: `GetACP()` 65001, and `argv`, `fopen` and `getenv`
  right.
- **The compiler built with it**: it checked `café.hero`, formatted `ŝ.hero`
  in place without touching `s.hero`, ran `probe`, and found a runtime under
  `rté`.
- **Where it stopped**: `build café.hero` on clang 23's octal `#line` escape,
  defect 240 since.
- **Its argument**: the manifest changes what every narrow C call in the
  process means, a bound library's included.
- **Its costs, unrun there**: the resource reaching every Windows link, a
  floor of *Windows 10 1903* (a documentation fact, never run: cite its source
  or write unrun), and the console's code page as a question apart.

## The routes, each to be priced, none presumed

- **(a)** The manifest, in every Windows program `heroes build` links and in
  the compiler's own Windows build, carried as one of these:
  - **(a1)** a `.res` made by `llvm-rc`;
  - **(a′)** the XML handed to the linker, `-Wl,/MANIFEST:EMBED
    -Wl,/MANIFESTINPUT:<file>`, no resource compiler at all. The critic
    measured this with lld-link 22.1.8 cross from this Mac (RT_MANIFEST
    embedded, merged with lld's default `trustInfo` unless `/manifestuac:no`).
    Unrun: on the box's linker, and with `link.exe`;
  - **(a″)** the compiler writing the fixed 432-byte `.res` itself into
    `build/`, no tool needed on a user's machine. Unrun under `link.exe`,
    which takes a `.res` through cvtres.

  A `#pragma comment(linker, "/manifest...")` in the runtime's own object is
  closed by measurement: lld-link 22.1.8 refuses it in `.drectve`.
- **(b)** Wide calls in the runtime at the 40 lines above, converting between
  UTF-16 and UTF-8 at each door. Its argument door costs something in every
  form (Q-h below).
- **(c)** Both.
- **(d)** (a), and a check at start that `GetACP()` answers 65001, refusing
  to run with a named message where the manifest is not honoured.
- **(e)** Refusing names above ASCII. At run time it cannot see best fit
  without `GetCommandLineW`, which is (b)'s door; at check time it is a new
  diagnostic, outside this lane.
- **(f)** Leaving it.
- **(g)** The UCRT's UTF-8 locale, `setlocale(LC_CTYPE, ".UTF8")` in
  `hero_args_set`, named so that the sitting refuses it on a measurement and
  not by omission. If it works at all (a recollection, unverified), it
  reaches only the program's own CRT calls: not the 35 Win32 `...A` lines,
  not `argv`, not a bound library's CRT. A library calling `setlocale` undoes
  it, the argument `runtime/parts/f64.c:47-52` makes against process-wide
  locale state.

## The questions

**Q1. Soundness, on the box.** First, before any route:

- **Q1-a. Is each case red at `7f4c0cc5`?** Under code page 1252 a Heroes
  program's narrow world can look self-consistent: `write_file("café.txt")`
  makes `cafÃ©.txt`, and `read_file("café.txt")` opens it again, an inference
  the critic names. So a case that makes its own names inside Heroes may pass
  on the broken tree. Settle it with names made from the wide world
  (PowerShell's `[IO.File]`, Explorer's) as well as by a program, each through
  `heroes check` and through a built program. **A case counts only if it is
  red on `7f4c0cc5`.**

Then, for each route built or priced:
- every row above and every shape below, `build` and `run` at a path above
  ASCII included, and **a project whose root folder is above ASCII**, as a
  user's profile often is: every door on one path at once (`MoveFileExA`'s
  publish, `replace.c`, `run.c`'s redirects, `#line`, clang's `-o`);
- **Q-b**: which linker each route meets, on the box, on the CI, and by a
  user's default, and whether it works under each;
- **Q-c**: an unpaired surrogate in a name and in an argument. Under (a),
  `WideCharToMultiByte` likely gives U+FFFD, which is valid UTF-8, so
  `spec/heroes-spec.md:324-325`'s *one that is not UTF-8 aborts* would never
  fire for `args()`: a wrong value, 238's own class. An inference to run. **If
  a route makes a spec sentence false, the sitting names it and that half goes
  to panel 192**, the full panel on what a `str` may hold;
- **Q-f**: best fit of fullwidth `／`, `＼` and `＂` to ASCII on the unrepaired
  tree, a recollection: `heroes check ..／x.hero`, and `args_checked()` over
  `a＂b c`;
- **Q-g**: a long name under (a). `WIN32_FIND_DATAA.cFileName` is `MAX_PATH`
  bytes by recollection (grep it on the box): 100 × U+6F22 is 300 bytes of
  UTF-8 in 100 units of UTF-16;
- **Q-k**: one row per launcher, PowerShell 5.1, `cmd` and Git Bash, and the
  console named as read over `ssh`, a pseudo-console;
- **Q-l**: a bound C library opening the program's `cstr` with its own
  `fopen`, as a `/MD` DLL on `ucrtbase` and as a `/MT` static library;
- the console's output of a `str` above ASCII: name it, do not repair it;
- an ASCII-named program: nothing may move.

**Q2. Cost.**
- Lines in `runtime/` and `selfhost/`.
- How the manifest reaches each link under (a1), (a′) and (a″).
- **Q-d, the links `heroes` does not write**: `--emit-c`
  (`selfhost/cli/table.hero:100`), the seed's line, and the CI's two seed
  lines (`ci.yml:428-429`, `:471`). `ci.yml` is read by
  `site/src/lib/claims.ts`, so a change there owes the site's build.
- **Q-h, route (b)'s argument door**: `CommandLineToArgvW` adds `shell32` to
  every link and allocates by `LocalAlloc`, outside the runtime's allocation
  rule (`tests/harness/suite_runtime.hero:25-27`); `wmain` moves the emission
  and the seed; a hand parser must match the CRT's split.
- **The CI's half** (whether `llvm-rc` is on its runner) needs a push or a
  `workflow_dispatch`, an outward act. It comes back unrun, or as a dispatch
  put to the author with its size.

**Q3. Instruments.** The cases that pin each row, each shown red at
`7f4c0cc5` (Q1-a), on Windows, with a twin elsewhere where the shape can be
written so (`.claude/rules/platforms.md`). No tracked path is above ASCII
today: `git ls-tree -r --name-only 7f4c0cc5 | LC_ALL=C grep -c '[^ -~]'`
gives 0, so a tracked case would be the first.

**Q4. Platforms and floor.** Linux and macOS unchanged, shown.
- **Q-j**: no Windows floor is stated anywhere today; route (a) would set the
  project's first, silently, which is §1.12's quiet end. Which supported
  editions fall below 1903 is unverified (Server 2019 and LTSC 2019 at 17763,
  Server 2016 at 14393, from recollection).
- **The instrument below the floor** would be a Hyper-V-isolated older
  container on the box, needing nested virtualisation and the Containers
  feature, all unknown. Try it, or write unrun.

**Q-i, where this sitting meets sitting 192.** `read_file` and `write_file`
lend `path.cstr()` (`selfhost/library_source.hero:208`, `:222`). So defect
245's interior NUL opens a shorter name at the runtime's own doors, on every
platform. Name which sitting owns that refusal; do not repair it here.

## Seats convened

- **`compiler-engineer`**:
  - builds route (a) in the form it judges most robust, (a′) or (a″) priced
    against (a1);
  - builds route (b) far enough to compare, or prices it over the 40 lines;
  - the cost, Q-d and Q-h;
  - the tests.
- **`ffi-pragmatist`**: the box.
  - Q1-a first;
  - every row and shape under each route built;
  - Q-b, Q-c, Q-f, Q-g, Q-k and Q-l;
  - the floor;
  - the console.
- **The completeness critic**: its second pass over the reports.

## The box, the frozen tree and your copy

**The Windows box** (`ssh win`) is free from 14:40, batch 9's leg done; the
seats share it.
- **One folder of your own** under `/c/w/`.
- **Files go in 1 MB parts**, as `<scratchpad>/platforms/windows-b9.sh` does
  (read it, never run it). Do NOT copy `windows2-u.sh`: it runs `rm -rf` on
  both machines at `:28`, `:29`, `:39` and `:52`.
- **Nothing is removed there.**
- **At most one heavy build** of yours on the box at a time: it has 2
  processors.

**Docker**: one container at a time, `docker ps -q` empty first.

**Your copy.** `<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
Your copy is `<scratchpad>/191-<seat>/`, made with `git -C
/Users/joseph/Temp/heroes/heroes-lang archive 7f4c0cc5 | tar -x -C <your copy>`.
- Build your compiler inside it from the seed, `clang -I runtime
  seed/heroes.c runtime/runtime.c -o heroes`. The seed's sha256 begins
  `26ccaa9d96478a20`: check yours.
- Never build, run or read inside another seat's copy or a lane's worktree.
- In the trunk, write nothing but your report.
- **Never `rm` anything.**
- Batch 10's three lanes share this Mac: at most two processes of yours at
  once, and no timing.
- Every time you write is read from `date`, in its own command.

**Your report** is `docs/panel/191-reports/<seat>.md` in the TRUNK, written as
you go:
- a verdict per question, in the charter's form;
- what you built and ran;
- your cost;
- a falsifiable prediction;
- the condition that would change your verdict.

English, no em dashes.
