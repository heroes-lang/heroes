# Panel 191, the shared brief: how the runtime reaches a name on Windows (defect 238)

Written 2026-10-04 from 14:07 by the coordinator. **The frozen tree is
`7f4c0cc5`**: batch 9's round head, with panel 190's sitting. Its runtime is
batch 9's closing runtime, and no lane of batch 10 edits `runtime/`. Every
seat works from `git archive 7f4c0cc5`, never from a working tree.

Convened by the author's answer *6b* of 2026-10-04
(`docs/records/log/2026-10-04-1236-the-author-answers-1a-2a-3a-4a-5a-6b.md`): a
sitting of its own, after panel 190, in the **soundness lane**. The seats are
the compiler-engineer and the ffi-pragmatist, with the completeness critic.
**No paid run.** Every number below names the command that produced it; what
was not run says so.

## Why this sitting

**Defect 238** (`blocking`, `docs/work/defects/238-*.md`): on Windows the
runtime reaches file names, arguments and the environment through the narrow
API, so a name above ASCII is refused, read as another file's, or written
under another name. Its rows, **carried** from panel 189's ffi-pragmatist on
the trunk's compiler at `7d9f2e8f` (2026-10-04, `docs/panel/189-reports/ffi-pragmatist.md`):

- `heroes check café.hero` exits 2, *argument 2 is not UTF-8*.
- `heroes check ŝ.hero` judges an `s.hero` beside it, the C runtime's best
  fit, and `heroes fmt ŝ.hero --in-place` rewrites `s.hero`.
- A built program's `args_checked()` gives `ŝ` as `s`.
- `read_file("café.txt")` fails `file_not_found` for a file that is there.
- `write_file` names its file `cafÃ©-out.txt`.

Re-running them on `7f4c0cc5` is owed: batch 9 changed three of the runtime's
Windows doors since.

## Measured for this brief, on `7f4c0cc5` (14:00 to 14:07)

**The runtime's narrow calls.** `grep -h -F '<call>' runtime/*.c runtime/*.h
runtime/parts/*.c`, lines that are not comments, counted with `wc -l`:

| call | lines | call | lines |
|---|---|---|---|
| `fopen(` | 2 | `CreateFileA` | 9 |
| `FindFirstFileA` | 2 | `GetFileAttributesA` | 7 |
| `FindNextFileA` | 1 | `SetFileAttributesA` | 3 |
| `getenv(` | 2 | `MoveFileExA` | 2 |
| `GetModuleFileNameA` | 1 | `DeleteFileA` | 2 |
| `CreateProcessA` | 1 | `rename(` | 4 |
| `remove(` | 8 | `stat(` | 12 |
| `mkdir(` and `_mkdir(` | 2 and 1 | `chmod(` | 2 |

These are raw counts, code compiled only on POSIX included; a seat refines
them. `grep` finds no `CP_UTF8`, `_wfopen`, `FindFirstFileW`,
`GetCommandLineW`, `wmain` or `activeCodePage` anywhere in `runtime/`.

**Where the arguments enter.** `argv` enters through `hero_args_set(int argc,
char **argv)` at `runtime/parts/os.c:496`.

**What batch 9 changed at the Windows doors**:
- **239**: the directory walk reads each name through `hero_dir_at_shown`
  (`runtime/parts/dir.c`, still `FindFirstFileA` at `:213`), a byte that is
  not UTF-8 named, never a panic;
- **243**: an environment value is read through `hero_env_shown`;
- **277**: the executable's path through `GetModuleFileNameA`
  (`runtime/parts/fs.c:261`);
- **240**: a `#line` name carries no numeric escape, the clang 23 refusal
  that stopped panel 189's route at build.

**The link.** On Windows, `selfhost/cli/flags.hero`'s `link_flags()` (`:168`
to `:194`) returns `-Wl,/STACK:67108864` and `-Wl,/INCREMENTAL:NO`. The
seed's own Windows build line is `seed/README.md`'s.

**The box.** Windows Server 2025 Standard, version 10.0.26100, ANSI code page
1252 (`ver`, the registry's `Nls\CodePage\ACP`, read over `ssh win` at about
14:03). It is a Hyper-V machine whose memory grows with demand (panel 190's
ffi-pragmatist, § 24).

## Carried, not re-run: panel 189's route

Panel 189's ffi-pragmatist route, from its report's section *A route for the
Windows boundary, compiled*, files in `<scratchpad>/189-ffi-pragmatist-cases/win/route/`
(read only; copy what you need):
- **the manifest**: a seven-line application manifest declaring
  `<activeCodePage>UTF-8</activeCodePage>`, compiled to a resource by
  `llvm-rc` and handed to clang as one more input, `utf8.res`, which lld-link
  takes;
- **what it changed**: `GetACP()` 65001, and `argv`, `fopen` and `getenv`
  right;
- **the compiler built with it**: it checked `café.hero`, formatted `ŝ.hero`
  in place without touching `s.hero`, ran `probe`, and found a runtime under
  `rté`;
- **where it stopped**: `build café.hero` on clang 23's octal `#line` escape,
  repaired since as defect 240 (`59e17a14`).

**Its argument**: the manifest changes what every narrow C call in the
process means, a bound library's `fopen` of the program's UTF-8 `cstr`
included, where wide calls in the runtime repair only the runtime's doors.
**Its costs, unrun there**: the `.res` reaching every Windows link, a floor
of Windows 10 1903, and the console's own code page as a question apart.

## The routes, each to be priced, none presumed

- **(a)** The manifest, linked into every Windows program `heroes build`
  makes (`link_flags()`) and into the compiler's own Windows build.
- **(b)** Wide calls in the runtime, converting between UTF-16 and UTF-8 at
  each door:
  - the arguments: `wmain`, or `GetCommandLineW` with `CommandLineToArgvW`;
  - `_wfopen`, `FindFirstFileW`, `GetEnvironmentVariableW`, `CreateProcessW`,
    `GetModuleFileNameW`, `CreateFileW` and the rest.
- **(c)** Both.
- **(d)** The manifest, and a check at start that `GetACP()` answers 65001,
  refusing to run with a named message where the manifest is not honoured.
- **(e)** Refusing names above ASCII, at check or at run time.
- **(f)** Leaving it.

## The questions

**Q1. Soundness, run on the box.**
- For each route you build or price: every row of 238 above, re-run on
  `7f4c0cc5`, `build` and `run` of a path above ASCII included.
- The shapes beside:
  - a file name holding an unpaired UTF-16 surrogate, which NTFS allows: what
    each route reads, and what batch 9's shown read says of it;
  - a bound C library opening the program's `cstr` with its own `fopen`;
  - a child process: the compiler running clang on a path above ASCII
    (`CreateProcessA`, `runtime/parts/run.c:687`);
  - an ASCII-named program, where nothing may move;
  - the console's output of a `str` above ASCII. Name it, do not repair it.

**Q2. Cost.**
- Lines in `runtime/` and `selfhost/`.
- How the `.res` is produced on the box and on the CI's Windows runner, and
  whether `llvm-rc` is there.
- The seed's Windows build line.
- What moves in every program's link, and in its emission (nothing, if only
  the link moves: show it).

**Q3. Instruments.** What the Windows leg and the CI's Windows job run, and a
case per row that runs on Windows, with a twin elsewhere where the shape can
be written so (`.claude/rules/platforms.md`).

**Q4. Platforms.** Linux and macOS unchanged, shown. The floor a route sets
on Windows versions, and what happens below it: run it if a way exists,
otherwise say unrun.

## Seats convened

- **`compiler-engineer`**:
  - builds routes (a) and (b) far enough to compare, or prices (b) where
    building it is out of reach, and says which;
  - the cost in `selfhost/` and `runtime/`;
  - the tests.
- **`ffi-pragmatist`**: the box.
  - every row and shape of Q1 under each route built;
  - a bound library's `fopen` under each;
  - the floor;
  - the console.
- **The completeness critic**: first over these briefs, then over the reports.

## The box, the frozen tree and your copy

**The Windows box** (`ssh win`) is held by batch 9's Windows leg until the
coordinator messages you that it is free, about 14:40. Prepare your bundle
first. Then:
- one folder of your own under `/c/w/`;
- files sent in 1 MB parts, as `<scratchpad>/platforms/windows2-u.sh` does
  (read it, never run it);
- **nothing removed there**.

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
- **Never `rm` anything.** Batch 10's three lanes share this Mac: at most two
  processes of yours at once, and no timing.
- Every time you write is read from `date`, in its own command.

**Your report** is `docs/panel/191-reports/<seat>.md` in the TRUNK, written as
you go:
- a verdict per question, in the charter's form;
- what you built and ran;
- your cost;
- a falsifiable prediction;
- the condition that would change your verdict.

English, no em dashes.
