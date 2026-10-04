# Panel 191, the completeness critic's second pass, over the reports

Opened 2026-10-04 16:10 by `date`; written from 16:35. My role names what is
missing and gives no verdict. My first pass is
`completeness-critic-briefs.md` beside this file.

## What I worked from

- **The reports**: `compiler-engineer.md` (closed 16:00) and
  `ffi-pragmatist.md` (last written 16:08), read whole, with the corrections
  each seat wrote underneath taken as its claim.
- **The briefs**: as repaired at 14:38 (`00-shared.md` and the seats' two),
  diffed against the `*-before-the-critic.md` texts I read.
- **The exports**: `<scratchpad>/191-shared/a-object/` and `.../c-dirwide/`.
- **My copies**, each a fresh `git archive 7f4c0cc5` in `<scratchpad>`:
  - `191-critic/`, the base;
  - `191-critic-aobj/`, with `a-object.diff`;
  - `191-critic-cdw/`, with `c-dirwide.diff`;
  - `191-critic-cdw3/`, with the two-step diffs;
  - `191-critic-seedexp/`, for one seed experiment;
  - `191-critic-cdw2/`, a failed `-p1` attempt that holds `Oops.rej` files,
    kept and not used.
- **My box work** is in `<scratchpad>/191-critic-work/box/`.
- **The box**: one folder of my own, `/c/w/191-critic/`. One heavy build: the
  base compiler from its seed, 16:28:36 to 16:28:48 by the box's clock.
  Everything else there was light: log reads, `probe`, `mutate`, one small
  build each. Nothing was removed there or here.
- **Not used**: Docker, any paid run, any seat's copy or folder. I read,
  built and wrote nothing in the lane worktree the session was pointed at
  (`.claude/worktrees/lane-round-b10`).

## 1. Checked, and found as the seats wrote

| claim | command | result |
|---|---|---|
| the exports' sizes | `wc -l` | `a-object.diff` 114, `c-dirwide.diff` 232, `c-dirwide-over-a-object.diff` 135 lines, as written |
| the exports' sha256 | `shasum -a 256` | `4ea03e6bc231ae4b`, `1ce10acae22ed6c7`, `a586cfe81f5bac22`; the box compilers `c61e7c704f35e6db` and `cbbd261db0b26db5`; this Mac's `603387833e66b849`. All as the seats wrote |
| the diffs apply to a pristine extract | `patch -p1` | clean. `c-dirwide` made directly and made over `a-object` give the same `runtime/` (`diff -r`). The over-`a-object` diff's paths carry three parts (`191-ce-work/ra/runtime/...`), so it needs **`-p2`**, which its `README.txt` does not say |
| `codepage.c`'s size | `wc -l`; `grep -c -v '^[[:space:]]*$'` | `a-object` 81 lines (78 non-blank); `c-dirwide` 137 (131 non-blank). So *+53 non-blank (81 to 137)* is right in non-blank lines, with its endpoints in `wc -l` |
| the `runtime` suite over `c-dirwide`'s tree | `heroes run tests/harness/main.hero -- ./heroes runtime`, compiler from the unchanged seed, 16:19 to 16:20 | **8 passed, 0 failed**. No seat reported it for this tree. The suite reads source text, Windows branches included, so this covers the new helpers' allocation (`hero_alloc`) |
| *350 emitted files* spell the generated `main` | `grep -rl -F 'int main(int argc, char **argv)' tests/emission tests/golden \| wc -l` | 350 |
| a package cannot bring a resource | `selfhost/cli/libraries.hero` | true. `:193` passes `-Wl,` words only after `filter_words` has admitted `-Wl,-rpath,` and `-Wl,-framework,` alone (`:230-236`; refused at `:205`, tested at `:395`) |
| the carrier adds no new kind of compiler requirement | the Windows view, `grep -F` | today's runtime already needs a GNU-compatible compiler there: `__attribute__` (`stack.c:178`, `:201`) and `__builtin_*_overflow` (`panic.c:71-72`), and the seed uses those builtins on 4,235 lines |
| an ASCII program's build prints nothing new under the route | the box, `heroes build hello.hero` with the base compiler and with `c-dirwide`'s exported one | both: exit 0, stdout 0 bytes, stderr only `wrote hello-<name>.exe`; the program prints `hello` under both. The route's binary carries one MANIFEST of 368 bytes, the base's none. So under lld-link no new link line reaches `warnings` |
| the landing's cases leave the seed unmoved (the compiler-engineer's Q2 prediction) | a test block appended to `selfhost/cli/process.hero` with the cases' kind of literals (`"clang"` and `"-o"`, which non-test code also spells, plus new ones), then `heroes build selfhost/main.hero --emit-c -o regen-1.c` | **unmoved**: `regen-1.c` is byte-identical to `seed/heroes.c` (`cmp`), as `regen-0.c` was before the block. Commit `34836e73` (274) records a test literal that did move it (*reorders one string constant*), so the exception exists; this shape did not hit it |

## 2. A shape red under the adopted route: a directory whose name holds a lone surrogate

Measured on the box at 16:26 to 16:29. The names were made by PowerShell's
`[IO.Directory]` and `[IO.File]` (`mk-sur.ps1`), and read back wide by their
UTF-16 units (`D800` stands alone). The compilers were run from Git Bash:
- the base: the frozen tree's, built in my folder from its seed;
- `c-dirwide`: the exported `heroes-c-dirwide.exe`, sha256 `cbbd261db0b26db5`
  on both machines, with `c-dirwide`'s `runtime/`.

| folder | base `7f4c0cc5` | `c-dirwide` |
|---|---|---|
| `sur/d1`: `ok.hero`, and a directory `x<D800>/` holding a correct `a.hero` | `probe` and `mutate` **exit 2, *cannot read the directory sur/d1*** | **the same, exit 2** |
| `sur/d3`: `ok.hero`, and an **empty** directory `x<D800>/` | **exit 2, the same message** | **the same, exit 2** |
| `sur/d2`: `ok.hero`, and a file `x<D800>.hero` (the compiler-engineer's W14) | exit 2, *cannot read `sur/d2/x?.hero`* | exit 2, defect 239's own *the name of `sur/d2/x<0xED><0xA0><0x80>.hero` is not UTF-8 ... rename it* |

**The message is false**: `sur/d1` and `sur/d3` are readable. What stops the
walk is one subdirectory's name. By my reading, unrun piece by piece:
1. the wide listing carries that name back as its WTF-8 bytes
   (`hero_win_name_bytes`, `codepage.c:107`);
2. the walk then recurses with a path holding them;
3. `hero_win_wide` refuses those bytes, through `MB_ERR_INVALID_CHARS`
   (`codepage.c:93`);
4. the walk returns 0;
5. `probe.hero:164` and `mutate.hero:145` report the root as unreadable.

What this means for the sitting:
- **The route does not make this shape worse.** The base says the same.
- **But it leaves it red with a false message**, an empty such directory
  included. That meets the compiler-engineer's own Q1 condition:
  *a row or shape red through (a⁗)+(d)+the wide door on the box*.
- **A false message is `blocking`** by the class list
  (`.claude/rules/verification.md` § Bounded discovery).
- **On Linux the matching shape**, a directory named by a byte that is not
  UTF-8 (239's own `onlydir/`), is walked by `opendir` on bytes. That is by
  reading; I did not run it for this pass.

## 3. Contradictions between the seats, and which side holds

- **(d)'s message: 257 bytes (compiler-engineer) against 256
  (ffi-pragmatist).** The source settles it.
  - The text at `codepage.c:73-77` is 255 bytes with code page 1252
    (`printf '%s' ... | wc -c`), then a newline: 256.
  - The check runs after `hero_stdout_is_bytes()` (`os.c:497` before
    `:502`), so a real program writes a bare LF. The ffi-pragmatist's
    `names-d.exe` read 256.
  - The 257 fits a probe whose stderr stayed in text mode (CRLF), my
    inference. Its W17, written *as `dcheck.c` does*, would test that mode
    and not the program's.
- **The CI's linker.** The ffi-pragmatist writes *`link.exe` is the CI's
  linker by the tree's own record* as a fact. The record
  (`flags.hero:181-191`) is from 2026-08-31, on a clang whose version it
  does not give. The box's official LLVM 23.1.1 defaults to lld-link, both
  seats measured. The CI's 20.1.8 from `C:\Program Files\LLVM\bin` is unrun.
  The compiler-engineer's *I cannot run* is the side the evidence supports.
- **The box's restarts: two, or four.** The compiler-engineer names 12:29
  and 15:31; § 5 reads four.
- **A citation that does not resolve.** The ffi-pragmatist's
  `GetModuleFileNameA` at *`fs.c:211`* and `GetFinalPathNameByHandleA` at
  *`replace.c:118`* are line numbers of its unifdef'd Windows view. In
  `runtime/parts/` those lines are comments; the calls are `fs.c:261` and
  `replace.c:136`. Its `dir.c` citation names the view's path; these two do
  not.
- **Units mixed in one sum.** The compiler-engineer's *(a⁗)+(d) 89 lines*
  adds `codepage.c`'s 81 (`wc -l`) to `runtime.c` +4 and `os.c` +4
  (non-blank lines). `a-object/README.txt` says `runtime.c` **+5** (diff
  lines). In one unit the sum is 86 non-blank, or 90 by `wc -l` and diff
  lines. Its prediction's unit (`wc -l`) is stated, and `c-dirwide`'s
  `codepage.c` is 137 now.

## 4. Claims asserted, not measured in the report that makes them

- **The 415-library scan** (*"none of 415 libraries on the box carries a
  resource section"*, *"with a positive control"*). It appears only in the
  ffi-pragmatist's Q2 `experiment` and `argument` (`:606-610`). No section,
  command, directory list or output shows it. The string table and the
  second manifest it names beside it are the compiler-engineer's runs
  (`compiler-engineer.md:87-95`). Separately, by my inference: an archive
  member is pulled only when referenced, and a resource object defines
  nothing a program references. So a library's resource object would not
  collide even if it existed.
- ***Below the floor the manifest is ignored*.** Both seats' Q4 arguments
  and predictions rest on this premise; it is unrun, no instrument being
  older than build 26100. The ffi-pragmatist predicts that on 17763 every
  program *prints (d)'s sentence*. That holds only if the loader of an
  older Windows skips the unknown `activeCodePage` element in the 2019
  `windowsSettings` namespace. If it refused the image instead, with a
  side-by-side error before `main`, (d) would never speak. Which of the two
  happens is the question under both predictions.
- ***17763 is Windows 10 1809 and Server 2019*.** That is my first pass's
  recollection, carried into a prediction as fact. Nobody seated can verify
  it; no historian sits here.
- **Two inferences stated as such by their authors, and still unbuilt.** The
  ffi-pragmatist's Q-l column for (b), and W21's third clause (*-1 against
  any runtime that makes the doors wide without the manifest*), rest on the
  mechanism, not on a (b) build. The compiler-engineer's *(b) whole, +180 to
  +250* is an estimate.

## 5. Facts to weigh: the box's restarts, read from its own log

`Get-WinEvent` over the System log (`ev.ps1`, `ev41.ps1`, run 16:23 to
16:24, read only):

| unclean stop | boot | Kernel-Power 41 |
|---|---|---|
| 01:15:16 | 01:58:12 | `BugcheckCode=0` |
| 08:59:25 | 12:28:15 | `BugcheckCode=0` |
| 12:29:21 | 12:30:15 | `BugcheckCode=0` |
| 15:31:23 | 15:45:23 | `BugcheckCode=0` |

- **Four unclean stops today, not two.** Each boot is followed by EventLog
  6008 naming the stop.
- **No crash inside the guest.** Crash dumps are enabled
  (`CrashDumpEnabled=7`, `AutoReboot=1`), yet there is no `MEMORY.DMP` and
  no minidump. There is no WER report since noon, and no Application error
  from 15:00 to 15:32.
- **No warning before 15:31.** No System event at all is logged between
  15:25:00 and the stop. The low-virtual-memory events of the day (12:53,
  13:02) are panel 190's clang.
- **So each stop came from outside the guest**: its host, its hypervisor or
  its provider, whose side only the author can read. That agrees with the
  compiler-engineer's *a user process cannot normally bring Windows down*,
  which it wrote as an inference. What it costs: any long run on the box can
  be cut with no trace inside it, and the batch legs inherit that.
- **Memory**: the box showed 1,313 MB visible and 365 MB free at 16:23.

## 6. Questions the sitting should have asked and did not

1. **A child's words in its own code page.** `run_clang` reads clang's or
   the linker's stderr with `read_file` (`selfhost/cli/toolchain.hero:137`).
   Bytes that are not UTF-8 make that read fail, and the words are then
   dropped: nothing is forwarded.
   - The manifest sets only the Heroes process's code page. MSVC
     `link.exe`, a narrow program, keeps its own.
   - So a link error naming a path above ASCII would arrive without the
     linker's words, under every route, with `-fuse-ld=link` or a clang that
     defaults to it.
   - The seats ran clang succeeding on such a path, never a child failing
     about one. Unrun; my reading.
2. **Who sets the floor.** No Windows support policy exists in the tree.
   Both seats' conditions, *a supported Windows below the floor that a user
   must run*, can therefore be read only by the author. The synthesis should
   put the floor to the author as a decision, with the measured prices:
   - (d) refuses every program below the floor, the compiler included;
   - (b) whole is about 250 lines, unbuilt.
3. **The closed records of 239 and 243.** Commit `db34bb1c` (14:40, the
   closing of batch 9's five at the C boundary) says *239's name that is not
   UTF-8 is made and named there* about the box.
   - Both seats measured that the name made there is a valid Unicode name,
     misread by the narrow door. 239's case was testing 238 as the platform.
   - On Windows, 239's *rename it* and 243's *its value ... is not UTF-8*
     are false for a valid name or value, the ffi-pragmatist's finding.
   - A commit body cannot be corrected underneath, but the two records under
     `docs/records/done/` can, with the date (`.claude/rules/records.md` § A
     record is never rewritten). No seat asked who writes that.
4. **`heroes doctor`'s `cc` row on Windows.** The compiler-engineer saw it
   fail on the box (exit 2, *cc not found*) and set it outside the sitting.
   It is filed nowhere: `git grep -i doctor` over `docs/work/defects/` at
   `7f4c0cc5` and at `main` finds only 243.
   - The CI is green on it only because its image carries MinGW-Builds gcc
     15.2.0 as `cc`, a compiler Heroes does not use on Windows. Run
     37196853219's Windows log reads *ok cc ... MinGW-Builds ... 15.2.0*.
   - Its advice, *build-essential, base-devel*, is a Linux one.
   - By § Bounded discovery it is filed apart with a class.
5. **The OEM code page.** Under the manifest `GetOEMCP` reads 65001 while the
   console stays at 437, both measured by the ffi-pragmatist. A library that
   converts for the console through `CP_OEMCP` would then write UTF-8 to a
   437 console. Unrun; not asked.
6. **A program built before the landing launching one built after.** A narrow
   Heroes launcher passes `café`'s bytes through `CreateProcessA` as
   `cafÃ©`, and a manifest program receives that as valid UTF-8: a wrong
   value with no failure. This is an inference from the two halves the
   ffi-pragmatist measured. It reaches any tool or harness built by an older
   compiler.
7. **W17 against the compiler-engineer's Q4 prediction.** The prediction is
   *0 occurrences of "does not honour the UTF-8 code page" in the box's and
   the CI's logs*. W17 is the case that makes the program say exactly that.
   If its captured stream reaches a log, the prediction scores itself false.
   It should name what it excludes.
8. **What else the manifest could carry.** The runtime now owns a manifest,
   and `longPathAware` is the obvious next entry. By my recollection, the
   wide calls honour it and the narrow ones keep `MAX_PATH`. So adding it
   later would give `c-dirwide`'s wide listing and its narrow opens two
   different length limits. A design note for the record, not a question
   for this sitting's resolution.
9. **The asm tree on another Windows target.** `x86_64-pc-windows-msvc` is
   the only target named (`seed/README.md:38-39`). The tree's `.rva` and its
   relocation are unrun for arm64 or i686.

## 7. Routes nobody listed or built

- **The repair of § 2's shape.** Two ways, neither built:
  - a widener that takes back the WTF-8 bytes the listing itself produced,
    so the walk descends and 239's message names each file inside, as Linux
    does with a byte-named directory;
  - or naming the directory itself by 239's message.
- **(b) whole**: unbuilt by either seat. W21's third clause rests on its
  mechanism.
- **`c-dirwide` with the UCRT's wide `argv`**, `_configure_wide_argv`. Both
  seats measured it refusing a lone surrogate (1113) and splitting as the
  CRT does, so with it spec `:324-325` would hold on Windows. Neither built
  it into a route; both hand the question to panel 192. Its documentation
  status is unread, and its `__argc` rewrite is a measured side effect.

## 8. The predictions, read as written

- **Compiler-engineer.**
  - **Q1** (6 seeds at the landing's box leg, 3 with the door narrow): the
    shape it names is measured both ways in its own § Resumed. Its
    condition is met by § 2's directory (above).
  - **Q2** (no `selfhost/` file but test cases, not the seed, not
    `seed/README.md` or `.github/`; `codepage.c` under 160 by `wc -l`):
    the seed half held on my probe (§ 1), and `codepage.c` is 137.
  - **Q3** (all red but W18 at the base, all green after): sixteen of its
    eighteen were shown red.
  - **Q4**: see § 6, 7.
- **ffi-pragmatist.**
  - **Q1** (`examples/ledger`'s SQLite opens a path above ASCII under every
    route): the binding is there (`examples/ledger/db/sqlite.hero:83`,
    `filename: cstr lent`). Unrun, as it says.
  - **Q2** (no resource error in the box leg's `examples/` builds): open.
  - **Q3** (W21: -1, 6, and -1 without the manifest): two clauses measured;
    the third is the (b) inference.
  - **Q4**: rests on § 4's loader premise and on § 4's recollection of 17763.
- **Panel 189's prediction 3** (the box's `cache`, `run` and `emission`
  under the manifest read as without): still unscored. Under the routes the
  box ran only the compiler's own tests and the net's own tests; I showed one
  program's build output unchanged (§ 1).

## Corrected in place while I wrote

Each correction was made at about 16:37, before anyone read this file:

| where | it said | it says, and why |
|---|---|---|
| § 1, the GNU extensions | `stack.c:174`, `:197` | `:178`, `:201`: the first two were my unifdef'd view's line numbers, the slip § 3 names in the ffi-pragmatist's report |
| § 6, question 1 | `toolchain.hero:136` | `:137`, by `grep -n` |

## Not run

- Linux for § 2's matching shape.
- `-fuse-ld=link` through `heroes build` (the compiler names no linker).
- The CI's own clang and `link.exe` (a dispatch, outward).
- Anything below build 26100.
- The console rows the ffi-pragmatist left.
- Any timing, any paid run.
