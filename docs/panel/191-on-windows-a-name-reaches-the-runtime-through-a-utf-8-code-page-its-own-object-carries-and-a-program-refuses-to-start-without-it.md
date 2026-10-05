# Panel 191: on Windows a name reaches the runtime through a UTF-8 code page its own object carries, and a program refuses to start without it

2026-10-04, written from 16:47 by the clock (`date`). The **soundness lane**:
`compiler-engineer` and `ffi-pragmatist`, with the completeness critic over
the briefs first and over the reports after. Convened by the author's answer
*6b* of 2026-10-04
(`docs/records/log/2026-10-04-1236-the-author-answers-1a-2a-3a-4a-5a-6b.md`)
for defect 238. **No paid run.**

**The tree.** The seats worked from `git archive 7f4c0cc5`: batch 9's runtime,
with panel 190's sitting. The briefs are `docs/panel/191-briefs/`, repaired
from 14:38 after the critic's first pass, before any seat was launched; each
keeps the text the critic read as `<name>-before-the-critic.md`. The reports
are `docs/panel/191-reports/`.

**Interruptions.**
- Both seats stopped at the account's session limit at about 15:46 and
  resumed at 15:52 from their own records.
- The Windows box stopped uncleanly four times that day: 01:15, 08:59, 12:29
  and 15:31. Each reads `BugcheckCode=0`, with no dump and no WER report,
  which places the cause outside the guest (the critic, § 5, from the box's
  System log). One run lost at 15:30 was run again.

**Faults of process**, each recorded by the seat or the critic:
- **Citations from the wrong view**: the ffi-pragmatist's `fs.c:211` and
  `replace.c:118` are line numbers of its unifdef'd Windows view, not of
  `runtime/parts/`. The critic made the same slip in its own report and
  corrected it there.
- **A scan with no command**: the ffi-pragmatist's scan of 415 libraries
  appears in its verdict without a command or an output.
- **Mixed units**: the compiler-engineer summed lines counted two ways in one
  sum.
- **A stuck run**: one of the ffi-pragmatist's runs stalled on a child
  reading its script's input; it stopped that process tree, and removed no
  file.
- **A stopped container**: the compiler-engineer left
  `p191-ce-linux-route`, removed on the author's *5a* at 16:36.

## The proposal

Defect 238 (`blocking`): on Windows the runtime reaches file names,
arguments and the environment through the narrow API, read in the machine's
code page (1252 on the box). A name above ASCII is then refused, read as
another file's, or written under another name.

**The routes the brief listed, each to be priced**:
- **(a)** the UTF-8 code-page manifest, in three carriers: (a1) a `.res` made
  by `llvm-rc`; (a′) the XML handed to the linker; (a″) the compiler writing
  the `.res` itself;
- **(b)** wide calls in the runtime;
- **(c)** both;
- **(d)** (a) and a refusal to start where Windows ignores it;
- **(e)** refusing names above ASCII;
- **(f)** leaving it;
- **(g)** the UCRT's UTF-8 locale.

**Four questions**:
- **Q1**: soundness, each case shown red at `7f4c0cc5`;
- **Q2**: cost;
- **Q3**: instruments;
- **Q4**: platforms and the floor.

## The verdict table

| seat | verdict | cost | prediction | condition |
|---|---|---|---|---|
| compiler-engineer | **object** to (a) alone in any carrier; **approve** (a⁗)+(d) with the directory door wide, a carrier nobody listed: the manifest compiled into the runtime's own object as a resource section; **refuse** (a′) (`LNK1158` under `link.exe`) and (g) | 153 lines net of `runtime/` (`parts/codepage.c` 137 new, `runtime.c`, `os.c`, `dir.c`, `replace.c`); 0 of `selfhost/`, 0 goldens, the seed unchanged (its fixpoint byte-identical), no link line, no site build | the box's `probe` over a folder of six names reads 6 seeds (3 with the door narrow); the landing touches no `selfhost/` file but test cases, nor the seed, `seed/README.md` or `.github/` | a row red through the route; the CI's clang 20.1.8 refusing the resource object (a dispatch, outward, unrun); a supported Windows below the floor |
| ffi-pragmatist | **approve** `c-dirwide`; **object** to (a) alone and to (b) alone; **refuse** (g) and (a′); object to (e) and (f) | the same object; the carrier decides who can forget it | W21, a bound library's own `fopen`: -1 at the base, 6 under `c-dirwide`, -1 under wide doors without the manifest | a row red under `c-dirwide`; a library reading a path the process code page does not reach |

## What the sitting measured

**Q1-a, red at the base** (both seats on the box):
- Every row of 238, and panel 189's four omitted rows, is red at `7f4c0cc5`
  when the name comes from the wide world: PowerShell, Explorer, git, a
  user's shell.
- The same rows read green when a Heroes program both makes the name and
  launches the next program. `CreateProcessA` turns UTF-8 into mojibake, and
  the child's C runtime turns it back.
- So a case counts only if its name, its launch or its observer comes from
  outside Heroes. **Defect 239's two existing cases made their names through
  narrow C and passed at the base while testing this defect.**
- Sixteen of the compiler-engineer's eighteen cases are shown red at the base
  (W17, the refusal, and W18, an ASCII program, cannot be). The
  ffi-pragmatist added three, all red at the base:
  - **W19**: `check ..／x.hero` judged `../x.hero`, a file in another
    directory, best fit turning the fullwidth `／` into `/`;
  - **W20**: a built program asked for `ŝ.txt` read the decoy in `s.txt`;
  - **W21**: the bound library's `fopen` returns -1.

**The manifest alone is not sound** (the compiler-engineer):
- **A truncated walk.** Under code page 65001 a narrow listing ends at the
  first name whose UTF-8 passes 259 bytes (error 234, read as the end).
  `probe` then says 3 seeds of 6, at exit 0.
- **A link replaced.** `fmt --in-place` on a symbolic link with such a name
  replaced the link by a plain file, exit 0, its target untouched.

These are two wrong answers the base never gave. The directory door made wide
closes both: 6 of 6.

**Only a process code page reaches a bound library** (the ffi-pragmatist,
W21). `lib191`'s own `fopen` reads the program's `cstr`:
- as 1252 at the base: -1 three times;
- right under the manifest: 6, 13 and 4, as a `/MD` DLL and as a `/MT` static
  library alike.

So (b) alone leaves §1.11's boundary broken for every binding that takes a
path.

**The carriers.**
- **The manifest inside the runtime's object** reaches `heroes build`, the
  seed's line, the CI's two lines, and an `--emit-c` author's own line, with
  none of them changed (the compiler-engineer, by `llvm-readobj` on each).
  The resource object was assembled by clang 20.1.8, the CI's version, and
  linked by both linkers, `GetACP` 65001 (the ffi-pragmatist).
- **(a′) dies under `link.exe`**: `LNK1158: cannot run 'mt.exe'`.
- **A `.res` on the link line** must be named on four lines `heroes` does not
  write. One forgotten is 238 again, silent without (d).

**What moves.** On the box, under both built routes, the compiler's own tests
read 1,190 with 1 failed and the net's own 246 with 1 failed: defect 239's two
cases, and only those. On this Mac and Linux arm64 nothing moves: 1,190
passed, the fixpoint byte-identical, `emission` 754 and `runtime` 8 green.

**(d)'s refusal.**
- **What it writes**: one ASCII sentence of 255 bytes and a newline on
  stderr, exit 2, reached and read under Git Bash, `cmd` and PowerShell 5.1.
  The critic settles the seats' 256 against 257 as 256, written after binary
  mode is set.
- **Below the floor it is unrun.** Nothing here is older than build 26100,
  the box and the CI alike. Whether an older loader skips the unknown
  `activeCodePage` element, so that (d) speaks, or refuses the image before
  `main`, is unrun too (the critic, § 4).
- **What the documentation says**, which both seats carried from panel 189
  as a recollection. The coordinator read Microsoft's *Use UTF-8 code pages
  in Windows apps* (learn.microsoft.com, dated 2025-07-17) at 16:58. It
  names the floor, *As of Windows Version 1903 (May 2019 Update)*. And it
  says a program declaring the property may *target/run on earlier Windows
  builds*, there handling *legacy code page detection and conversion as
  usual*. So by the documentation an older Windows starts the program on
  its legacy code page, and (d) speaks. That is a documentation fact, not
  a run.

## Disagreements, unsmoothed

1. **A red the route leaves** (the critic, § 2, measured on the box at 16:26
   to 16:29).
   - **The shape**: a folder holding a *directory* named with a lone
     surrogate makes `probe` and `mutate` exit 2 with *cannot read the
     directory sur/d1*, an empty such directory included.
   - **It is false**: the folder is readable.
   - **The base says the same**, so `c-dirwide` makes it no worse, and leaves
     it red.
   - **The mechanism**, the critic's reading: the wide listing hands the name
     back as WTF-8 bytes, and `hero_win_wide` refuses those bytes when the walk
     recurses (`codepage.c:93`).
   - It meets the compiler-engineer's own Q1 condition. R2 answers it.
2. **Which linker the CI uses.** The ffi-pragmatist writes it as `link.exe`,
   by the tree's record of 2026-08-31. The box's LLVM 23.1.1 defaults to
   `lld-link`. The CI's 20.1.8 is unrun, so the compiler-engineer's *I cannot
   run* is the side the evidence supports. The carrier adopted works under
   both, measured.
3. **The floor.** Both seats approve (d) as the floor's only loud form, short
   of (b) whole. Neither can run below it. Who decides a floor the tree has
   never stated is the author (the critic, § 6).

## The resolution: `provisional — author ratification pending`

**R1. Route `c-dirwide`.**
- The UTF-8 code-page manifest, compiled into the runtime's own object as a
  resource section (a⁗).
- Route (d): a check at start in `hero_args_set` that refuses to run, with
  one ASCII sentence on stderr and exit 2, where `GetACP()` does not answer
  65001.
- The directory door made wide: the listing in `dir.c`, and `replace.c`'s
  link check.

About 150 lines of `runtime/`. None of `selfhost/`, no golden moved, the seed
and every link line unchanged, no site build.

**R2. The directory shape is repaired at the landing**, red at `7f4c0cc5`
first:
- A walk descends into a directory whose name holds a lone surrogate.
- The widener takes back the WTF-8 bytes its own listing produced, so 239's
  message names each file inside, as Linux does with a byte-named directory.
- Where descending cannot be made total, the directory itself is named by
  239's message.
- Never *cannot read* of a readable folder. **238 does not close while this
  shape is red.**

**R3. The cases.**
- The compiler-engineer's eighteen and the ffi-pragmatist's W19 to W21, built
  on a wide-world helper, **each shown red on `7f4c0cc5`'s runtime before it
  counts**. W17 and W18 are the two that cannot be.
- **Defect 239's two cases** keep their POSIX branch, and their Windows branch
  makes a lone-surrogate name by `CreateFileW`.
- **A case made, launched and read entirely inside Heroes is refused as a
  witness**: it is green on the base.

**R4. The floor is the author's** (see the DECIDE item):
- (d) refuses every Heroes program, the compiler included, where the manifest
  is not honoured;
- Microsoft's documentation names Windows 10 version 1903, read at 16:58;
  no seat could run below it.

**The conservative alternative, recorded**: route (b) whole, about 250 lines,
unbuilt. It sets no floor, and leaves every bound library reading the
program's UTF-8 as 1252 (W21: -1).

**R5. Refused**:
- **(a) alone, in any carrier**: the measured silent truncation and the
  replaced link;
- **(a′)**: `LNK1158`;
- **(g)**: measured to reach `fopen` alone, and undone by a library's
  `setlocale`;
- **(e)**: it cannot see best fit without the wide door it refuses;
- **(f)**: 238's rows red.

**R6. Handed to panel 192**, convened by the author's *3a*:
- **Q-c.** `spec/heroes-spec.md:324-325`, *`args()` ... one that is not UTF-8
  aborts*, is false at `7f4c0cc5` for an argument holding a lone surrogate:
  it arrives as `x?y`, and as `x<U+FFFD>y` under (a). The UCRT's wide `argv`
  (`_configure_wide_argv`) with a strict conversion would make it true. Both
  seats ran the parts: the call returns 0, a lone surrogate is refused with
  error 1113, the split is the CRT's, and the CRT's global `__argc` is
  rewritten. About 25 lines, unbuilt as a route.
- **Q-i.** `read_file` and `write_file` lend `path.cstr()`, so an interior NUL
  opens a shorter name at the runtime's own doors on every platform, and no
  door taking a plain C string can see it.

**R7. Corrections owed underneath the closed records of 239 and 243**, dated:
- on Windows, 239's *rename it* and 243's *its value ... is not UTF-8* are
  false for a valid name or value that the narrow door misread;
- 239's case on the box was testing 238.

The records are corrected at this sitting's commit; the messages are
repaired by R1.

**R8. Found beside the sitting.** Two are broken today and are filed:
- **336, `heroes doctor`'s `cc` row on Windows** (`blocking`, a false
  message). The toolchain works; `doctor` says FAIL (*cc not found*, advice
  for Linux). The CI's leg is green only because its image carries MinGW's
  gcc as `cc`.
- **337, a child's words that are not UTF-8** (`adjacent`). `run_clang`
  reads clang's or the linker's stderr with `read_file`
  (`selfhost/cli/toolchain.hero:137`). The critic asked it unrun; the
  coordinator ran it on this Mac at 16:55 with a stand-in `clang` failing the
  link: one byte that is not UTF-8 and the author reads `linking failed:`
  over an empty line, the same words in UTF-8 forwarded whole.

Three concern code that exists only after 238's landing. They are carried
in 238's item as checks that landing owes, and are filed with a class if it
measures them red:
- **A program built before the landing launching one built after** (an
  inference from two measured halves): the mojibake arrives as valid UTF-8,
  a wrong value with no failure.
- **The OEM code page under the manifest** (unrun): `GetOEMCP` reads 65001
  while the console stays at 437, so a library converting through
  `CP_OEMCP` writes UTF-8 to that console.
- **The resource object on another Windows target** (unrun): arm64 and
  i686.

`longPathAware`, the manifest's obvious next entry, is a design note, not an
item.

**R9. The landing**: defect 238 is `blocking`, so it lands in the next batch,
on this provisional resolution, as 227 and 231 landed on theirs. It owes:
- R2 and R3;
- R7's corrections;
- the platform legs, the box above all;
- the CI's clang 20.1.8 on the resource object, which the landing's push
  measures.

## Predictions to score

- **The compiler-engineer's, at the landing**:
  - `probe` over the six-name folder reads 6 seeds on the box;
  - the landing's diff touches no `selfhost/` file but test cases, nor the
    seed, `seed/README.md` or `.github/`, and `codepage.c` stays under 160
    lines by `wc -l`;
  - with the new cases, the box reads all red but W18 on `7f4c0cc5`'s runtime
    and all green on the landing's;
  - no log of the box's or the CI's next Windows run holds (d)'s sentence.
- **The ffi-pragmatist's**:
  - W21 reads -1, 6 and -1 (the last unbuilt);
  - `examples/ledger`'s SQLite opens a path above ASCII under every route
    (unrun: no SQLite for Windows on the box);
  - the box builds every `examples/` program it builds today with no linker
    error naming a resource.

## The critic's passes

**First, over the briefs** (`completeness-critic-briefs.md`):
- **The framing repaired**: seven runtime commits, not three; the narrow
  doors counted as Windows compiles them (40 lines, not 60); the manifest's
  8 lines; the linker unverified; the box's code pages; the CI's half unrun;
  design.md's sections cited.
- **Three routes named**: (a′), (a″), (g).
- **Q1-a asked first.**

**Second, over the reports** (`completeness-critic.md`). This synthesis
answers it so:
- the red directory shape: R2;
- the contradictions: disagreement 2, and the 256 bytes;
- the unshown scan and the loader premise: § What the sitting measured;
- the box's restarts: the head of this file;
- its questions: R4, R7 and R8 (the child's words, the floor, 239 and 243's
  records, `doctor`, the OEM page, mixed builds);
- the routes unbuilt: R2 and R6 ((b) whole recorded as R4's alternative).

## Author's verdict

**RATIFIED 2026-10-05**, on the author's answer between 16:12 and 16:14 by
the clock read before and after it, meant as: *I ratify both panels*, this
sitting and panel 192. **Recorded as a reading**, CLAUDE.md § 4's default;
not `by delegation`.

**What the yes settles**:
- R1 to R9 as the resolution above states them, route `c-dirwide` over the
  conservative (b) whole;
- the floor (R4) at Windows 10 version 1903, the recommendation's.

The landing is batch 11's lane b11-windows, defect 238 (`d5133e26`,
`dc7eed87`).

**What it does not settle**: what only the landing can measure, R2's repair,
R3's cases red then green on the box, the CI's clang on the resource object,
and below the floor.
