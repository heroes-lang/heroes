# Panel 189, the completeness critic: second pass, over the seats' reports

Opened 2026-10-04 03:14 by `date`; closed 04:03, each section written after
the `date` reading it names. No verdict. My folder is
`<scratchpad>/189-critic-pass2/`:

- `trunk/`, `git archive 7d9f2e8f`, its compiler from the seed at 03:13,
  `958320f39dee0b9e`;
- `routegit/`, a private clone of the trunk's object store (`git clone
  --shared --no-checkout`, no network, its own index), detached at
  `6ee963e7`, the coordinator's `route.diff` (sha256 `408bd54bebf214a2`, as
  the report says; `route-runtime.diff` `34db6e221da8d682`, the same) applied
  by `patch -p1` (31 files, no fuzz) and its 31 paths staged by name. Its
  seed compiler (linking the route's runtime) is `ddfcacfbeca6c399`; **the
  route's compiler, `heroes-route`, built from it at 03:22, is
  `1cd0651051b9160e`**. No seat's report gives its route binary a hash.

I read the repaired briefs (00:08 to 00:11) the seats worked from, then every
report: `compiler-engineer.md`, `ffi-pragmatist.md` (items 1 to 3),
`spec-warden.md`, `historian.md`, `llm-ergonomist-a.md` to `-d.md`. I read
no seat's folder but the two diffs the coordinator named.

## What is missing, each with the command that settles it (summary, 03:40)

1. **`surface` goes red on the route**, 354 and 1 against the lane's own 355
   and 0 (its own compiler, no route), run here; the rest of the net green: `heroes mutate tests/golden/check` stops at exit 2 on the
   route's own committed case. Settle the rule (`mutate` counting a file
   that is not text as one the compiler refuses, or the proposal's exit 1
   for every verb) and run `heroes-route run tests/harness/main.hero --
   ./heroes-route surface` in a copy holding the cases.
2. **Nothing measured separates one message per file from one per line**:
   the sixteen blind sessions are sixteen repairs, arm A's included. The run
   that would: two arms of four over `mixed-several` padded to a few hundred
   lines, the brief not naming the characters, P (the route's first
   diagnostic) against L (its three), the llm-ergonomist brief's command at
   0.25 USD; about 1.4 to 2.0 USD of the **2.262** left of the 5.
3. **No blind arm read the route's real message**, its two notes and its
   64-line encoding reading; the arms read the coordinator's draft. And the
   reading **guesses wrong for code page 437** (run here: `caf<82>` told
   *0x82 is `‚` in Windows-1252*, where `cmd` wrote `é`). Settle: `heroes-route
   check` over each writer's file of the ffi-pragmatist's item 1, and an arm
   whose brief does not name the target.
4. **The route's diagnostic writes NUL bytes** on a UTF-16 file with its mark
   (15 and 16, run here), PowerShell 5.1's default: defect 244's cause,
   reached by 227's commonest Windows case. Settle by landing 244 with 227,
   then `heroes-route check <file> | tr -cd '\000' | wc -c` reads 0.
5. **`key_of`'s two branches meet** (a stale binary, the ffi-pragmatist's
   step 5): neither repaired nor filed. Settle: a key the UTF-8 branch can
   never produce, then its case 5 refusing the build.
6. **The landing tree is not the measured one**: on `lane-round1004a` the
   route's `os.c` hunk fails (`patch --dry-run`, run here), and `source.hero`,
   `lexer.hero`, `suite_fixes.hero`, `cli/input.hero` and `suite_records.hero`
   (931 lines) have moved. Settle: the route on the round tree, 273 and 274
   inside `hero_file_bytes`, built; its cases, the compiler's own tests,
   `check`, `annotations`, `fixes`, `surface`, `records` and the census over
   its 1,957 files plus the cases.
7. **Unfiled shapes the sitting met**: the uncapped `unexpected_character`
   flood (31,600 for 32 KB, the `UNSEEN` table rebuilt per call); the
   compiler's own argv not UTF-8 on Linux and this Mac (a note for a
   program's author; a file named `caf<e9>.hero` cannot be checked);
   bidirectional controls accepted (no sitting queued); clang's text with a
   path that is not UTF-8 (Linux only, unmeasured: a quoted include of a
   header under `d<e9>/` with a `#warning`, `heroes build` on trunk and route
   in the arm64 container).
8. **Unscored or unrun predictions**: the ffi-pragmatist's prediction 1 (the
   route's compiler never ran on the box: build it there, `check`
   `setcontent-e.hero` and `redirect-a.hero`); the spec-warden's P4, Q5's
   only thesis instrument (paid, not authorised); P2, run here on
   `1cd0651051b9160e` and holding, owed on the landing compiler.
9. **Two rulings the synthesis owes**: whether `HEROES_RUNTIME` (243) and a
   `pkg-config` answer (241) share 227's cause by 00-shared's own words; the
   compiler-engineer says 241 does, and § Bounded discovery keeps such a
   shape in the item.
10. **Process**: two stray files of the compiler-engineer's in the trunk's
    root (`err.txt`, `out.txt`, 01:56:17), unrecorded; the ffi-pragmatist's
    read of another seat's copy, recorded; a `date` rule the historian's
    tools cannot meet; the route's binary given no hash.

## The spec-warden's P2 and the compiler-engineer's census

**P2 was run by no seat on any build of the route.** The spec-warden
measured its eighteen files on the trunk (`read_file` and today's `check`)
and registered P2 for the landing compiler; the compiler-engineer's controls
cover four of its six text rows (`valid-fffd`, `nul-comment`, `utf8-bom`,
`utf16le-nobom-ascii`) and not U+FFFF (row l) or a NUL inside a string (row
m).

**Run here, 03:22, on `heroes-route` (`1cd0651051b9160e`)**, over the
eighteen rebuilt from the spec-warden's table (F1's six copied from
`189-facts`; g to r written by me, so a reconstruction, not its script;
Python's strict decoder: twelve not UTF-8, six UTF-8):

| rows | trunk `check` | route `check` | `error[not_text]` |
|---|---|---|---|
| a to j, p, q (twelve) | 2 each | **1 each** | **one each** |
| k (U+FFFD), l (U+FFFF), m (NUL in a string), o (`café`) | 0 | 0 | none |
| n (a UTF-8 byte-order mark) | 1 | 1 | none (`unexpected_character`, `unexpected_block`) |
| r (UTF-16 LE, no mark, ASCII) | 1 | 1 | none |

**P2 holds on this build.** It and the census agree in kind and test
different sets: the census (the compiler-engineer's own binary, unhashed;
1,918 tracked files of `6ee963e7` and its 4 cases) sees the three cases that
are not UTF-8 move and nothing else; P2's six text rows are shapes the census
does not hold: of the 1,918 tracked `.hero` files of `6ee963e7`, **0** open
with a byte-order mark, **0** hold a NUL, a U+FFFD, a U+FFFF or a UTF-16 shape
(scanned at 04:03), so the census tests P2's *none of the six* only through
the route's own U+FFFD control case. Both are registered for the gate that
lands 227, on the landing compiler, which is neither of these.

**Found running it: the route's new diagnostic writes NUL bytes to the
terminal on the commonest Windows file.** For p and q (UTF-16 with its mark,
what PowerShell 5.1's `>` and `Out-File` write, the ffi-pragmatist's item 1)
the gutter prints line 1 raw: **15 and 16 NUL bytes** (`tr -cd '\000' | wc
-c`), between two U+FFFD and the letters. The reading note itself is right
(*the file opens with 0xFF 0xFE, the byte-order mark of UTF-16, so it was
saved as UTF-16: save it as UTF-8*). The compiler-engineer names the raw
gutter as a shape with another cause, *filed apart* (its Q6 table); this is
that shape reached by the route's own diagnostic, on the file 227's commonest
reporter will hand it.

## One message per file or one per line: no measurement separates them

No report holds a measurement of repair turns that tells the two apart:

- **The blind arms are at a ceiling, all four of them.** Sixteen sessions,
  sixteen one-turn repairs, every `c.hero` printing `63 61 66 c3 a9 0a`, arm A
  (*cannot read*) included. Arm A's own `reading` sections say why: the
  session's Read tool showed `caf�` and the brief named the target word, so
  *"nothing the compiler printed contributed to that edit"* (a4). The arms
  separate neither A from B to D nor B, C and D from each other; the
  historian's prediction (B, C, D within 1 of 4) holds at 4, 4, 4 and so
  says nothing either way.
- **The compiler-engineer's `mixed-several`** (Windows-1252 quotes on lines 4,
  7 and 9 of a UTF-8 file) shows what each grouping PRINTS: per file names one
  of three places. That is an argument, not a repair measured.
- **The historian's precedents** are practice: rustc 1.86 and CPython one per
  file; gc one per line, ten and stop; `go/scanner`, Swift, javac each one.
- **No arm read the route's actual message.** On the blind program the
  route prints `error[not_text]: the byte 0xE9 is not UTF-8, ...` (no
  position in the headline) and two notes, the encoding reading and *nothing
  else in this file is read until it is UTF-8* (run 03:28, `heroes-route`).
  The arms read the coordinator's draft, with neither note. The reading is
  64 of the route's 267 code lines, which its builder left *"the blind arms'
  to measure"*; none did.

**What would separate them, and its price.** The ceiling has two causes, the
file short enough to read whole and the brief naming the target, and an arm
that keeps either cannot separate anything. The settling run: the same
`claude -p` command (the llm-ergonomist brief's, `--max-budget-usd 0.25`)
over `mixed-several` padded to a few hundred lines, its brief saying what
the program does without spelling the three characters, two arms of four:
**P**, the route's output cut to its first diagnostic, and **L**, the route's
three; scored on whether `c.hero` builds and prints U+201C and U+201D on all
three lines. Spent so far: **2.738 USD** of the 5 (the sixteen sessions'
`run.json` costs as the four arm files give them, summed here); left
**2.262**, about twelve sessions at the dearest arm's 0.1794, less for a
longer file. Eight sessions fit; a third arm (no notes) to price the reading
fits only if the file stays short.

## The ffi-pragmatist's condition: no replaced text to a writer or a digest

- **Writers: held, measured twice.** The compiler-engineer (item 3) and the
  ffi-pragmatist (item 3, on its own build of `route.diff`): `check --apply`,
  `--apply --in-place`, `fmt`, `fmt --in-place` on a root holding `0xE9` and a
  `certain`-fixable mistake, each exit 1, the file unchanged. The route never
  lexes such a file, and `source_extent.user_text` asserts no fault in the
  root for every writer.
- **Digests: held for an edit, not for a collision.** `reading.key_of` keys a
  file that is not UTF-8 as a header, the marks and the shown text, so a
  one-byte edit inside a literal rebuilds (the ffi-pragmatist's steps 1 to 3).
  But the key is not one to one: a UTF-8 file whose text IS that header and
  marks gets the same key, and the ffi-pragmatist's step 5 built a stale
  binary (`build` exit 0, *prints 4*, where the base compiler refuses the
  header). **Not repaired in the route and not filed**: a landing condition
  that lives only in that report. The same key keys the runtime
  (`cli/runtime_key.hero`). Settled by its case 5 re-run on the landing's
  compiler, after a key whose two branches cannot meet.
- **The spec-warden's condition on `measure`** (no count of a replaced
  text): **held** on `heroes-route`, run here, 03:29: a copy of the spec whose
  first `·` is the byte 0xB7, `measure` exit 2, *`spec-latin1.md` is not
  UTF-8: the byte 0xB7 at line 287, column 30 is the first of 1 that are
  not*, no count. (The wording *"the first of 1 that are not"* for a single
  byte is the route's tool message, `measure` and `mutate` alike.)

## Where the route meets 273, 274 and 275, and the rest of the round tree

Read through git, never in a worktree; `lane-round1004a` at `6344a5bb`, of
which `6ee963e7` is an ancestor.

- **273 (`df453108`) and 274 (`34836e73`)** rewrite `hero_file_read`'s body
  in `runtime/parts/os.c` (273: `fstat`, a directory FAILED before a read, a
  read to the end; 274: NOT_FOUND only for ENOENT and ENOTDIR). The route
  moved that body's old read into `hero_file_bytes`, which its new
  `hero_file_read_shown` and every source read of the compiler use. **`patch
  -p1 --dry-run` of `route.diff` on a clone of the round tree, run here at
  03:24: every file applies but one hunk of `os.c`** (`1 out of 2 hunks
  failed`), the read split; the ffi-pragmatist's `git merge-file` left the
  same two conflicts. Its composition (273's and 274's read inside
  `hero_file_bytes`) passes the route compiler's 1,133 tests on Linux arm64;
  the route's runtime as built fails one there (*out of memory*, defect 236's
  test) and answers NOT_FOUND for a Windows directory and for mode 000 here.
- **274 also edits `selfhost/cli/input.hero`**: a test appended at its end,
  which the route also edits; the dry run applies it as text. The
  ffi-pragmatist read only `runtime/` of that commit.
- **275 (`cf7d6efa`)** touches `parts/f64.c` and `parts/failure.c`, neither
  in the route; the route's one static text, `static const char hex[] =
  "0123456789ABCDEF"`, is sized by its literal, so 275's rule already holds
  for it. They do not meet.
- **The round tree has also moved three more of the route's 31 paths**, which
  no seat examined: `selfhost/source.hero` (defect 228, `17322f0e`, 42
  lines), `selfhost/lexer.hero` (defect 182, `883a3fb9`, its tests) and
  `tests/harness/suite_fixes.hero` (defect 220, `8a989fc2`). The dry run
  applies all three as text; whether the merged tree builds and passes is
  **unrun**. And `tests/harness/suite_records.hero` differs by 931 lines
  there: the `records` that will judge the committed cases is not the one any
  seat or I ran.

## Q6: every shape the sitting met, in the route, filed, or neither

The filings read from the round tree's commits (`git show
lane-round1004a:docs/work/defects/<n>-...md`, never the worktree), 238 to 251
and 275 to 277, then `git grep` over all of `docs/work/defects/` for each
shape's words.

| shape, and who met it | where it is |
|---|---|
| the runtime's sources, a part, a C header's digest (critic F8; compiler-engineer; ffi-pragmatist) | **the route** |
| `mutate`'s presence check (compiler-engineer) | the route (unreachable as its own shape) |
| Windows' narrow API (ffi-pragmatist) | 238 |
| the directory walk's panic (ffi-pragmatist) | 239 |
| clang 23 and the `#line` octal escape (ffi-pragmatist) | 240 |
| a `pkg-config` answer not UTF-8 (compiler-engineer, batch 8's FFI lane) | 241 |
| a UTF-8 byte-order mark told twice (ffi-pragmatist, spec-warden) | 242 |
| `HEROES_RUNTIME` not UTF-8 read as unset (critic, compiler-engineer, ffi-pragmatist) | 243, and 276 for its silent `./runtime` |
| the gutter writing control bytes raw (compiler-engineer, ffi-pragmatist) | 244 |
| a `str` holding NUL lent to C; raw control characters in a string | 245, 251 |
| UTF-16 without a mark over ASCII (critic, compiler-engineer, ffi-pragmatist, spec-warden) | 247 |
| a clang warning printed twice (ffi-pragmatist) | 248 |
| `validated_bytes`' message cut short (compiler-engineer) | 275, repaired at `cf7d6efa` |
| **the uncapped `unexpected_character` flood**: 32 KB of control bytes drew 31,600 diagnostics and 8.7 billion instructions, `shown_char.named` rebuilding its `UNSEEN` table at every call (compiler-engineer, `q6/unseen`, which its report calls *filed apart*) | **neither**: `git grep` of `UNSEEN`, `shown_char`, `31,600`, `billion` over `docs/work/defects/` finds no filing of it; 247 is the UTF-16 file's thirty, not this |
| **the compiler's own argv not UTF-8 on Linux and this Mac**: exit 2 and a note telling the compiler's user to rebuild with `args_checked()`, so a correct program in a file named `caf<e9>.hero` cannot be checked on Linux (critic's first pass; compiler-engineer; ffi-pragmatist, its Linux table) | **neither**: 238 is the Windows narrow API alone (`git grep args_checked` finds 238 and 245) |
| **`key_of`'s two branches meeting**, a stale binary (ffi-pragmatist, item 3) | **neither**: a landing condition in one report |
| **bidirectional controls** (U+202E, Trojan Source's character) and U+2028 accepted in comments and strings (spec-warden) | **neither**, by its own words *not this sitting's question*; no sitting queued |
| **the route's own diagnostic writing NUL bytes** for a UTF-16 file with its mark, 15 and 16 bytes (run here) | 244's cause, filed; **whether 244 lands with 227** decides whether 227's commonest Windows case prints NULs |
| **the route's reading guessing wrong for code page 437**: a `cmd`-written `caf<82>` is told *0x82 is `‚` (U+201A) in Windows-1252* (run here, 03:29), where `cmd` wrote `é` (the ffi-pragmatist's item 1) | **neither**: a property of the route's note, unmeasured on a reader |
| **clang's text read back carrying a byte that is not UTF-8** (00-shared's Q6 lists it) | **unmeasured by every seat**. Run here: clang 21 escapes the byte as `<E9>` in `#warning`, `#pragma message` and `#error`, its stderr UTF-8 each time; what remains is a path holding such a byte, Linux only |

## Contradictions between seats, and which side is checkable

1. **Q3, one per file against one per line** (the proposal and the
   historian's precedents; the compiler-engineer, no veto). Checkable only by
   the blind run above; nothing run separates them.
2. **Whether `HEROES_RUNTIME` shares 227's cause.** The historian: by
   00-shared's words (*a read that is not text taken as unreadable or
   absent*) it seems to; the compiler-engineer: the same cause in an
   environment value, filed apart unless the sitting widens 227; the
   ffi-pragmatist and my first pass: no. Filed apart as 243. A reading of a
   definition, for the synthesis; the definition's words favour the
   historian.
3. **Whether a `pkg-config` answer belongs to 227.** The compiler-engineer:
   **yes**, 227's cause (`cli/libraries.hero:298` takes it as empty); filed
   apart as 241 because lane b8-ffi holds that file (297 of 300 lines).
   `.claude/rules/verification.md` § Bounded discovery keeps a shape with the
   repair's own cause IN the item. The rule against the filing, for the
   synthesis to rule.
4. **The encoding reading.** The compiler-engineer built 64 lines that name
   Latin-1 and Windows-1252 for a one-byte encoding; the historian (*for
   single-byte encodings no front end I read guesses*; P2295's windows-1251)
   and the ffi-pragmatist (*`82` is `é` in 437 and `‚` in 1252*) caution
   against the guess. Checkable: the route's note on each writer's file of
   the ffi-pragmatist's item 1 (437 measured wrong here), and a blind arm
   whose brief does not give the target.
5. **Q5, a sentence or none.** The spec-warden: none (u0), its other reading
   u5+r1; the historian: *a sentence has the stronger precedent* (Rust,
   Python, C++23, Zig against Go). Checkable by the spec-warden's own P4, a
   paid generation run it named and nobody ran.
6. **The ffi-pragmatist's item-2 objection** (no replaced text to a digest)
   **against the route's key**: answered for edits, open for the collision
   (above). Checkable by its case 5.

## Claims asserted and not measured with the command that settles them

- **The route's binary is unhashed** in the compiler-engineer's report, so
  every count it gives (the census, 451, 744, 1,133) names a path, not a
  build. Mine is `1cd0651051b9160e`; the ffi-pragmatist built its own.
- **The ffi-pragmatist's prediction 1** (on the Windows box, the route's
  diagnostic names line 2, column 10 on `setcontent-e.hero` and line 1,
  column 1, byte 0xFF on `redirect-a.hero`) says *"scored in item 3, on the
  compiler-engineer's build, on this box"*; item 3 says *"Not run: the route's
  whole compiler on the box"*. **Unscored.** Command: on the box, the route's
  compiler (the seed with the stack flag, then `heroes build
  selfhost/main.hero` against the composed runtime), `check` on both files.
- **"`records` ... would skip these cases, by reading the code"**
  (compiler-engineer, its condition 3): now run, below, on `6ee963e7`'s
  `records`; not on the round tree's.
- **The census on the landing tree**: run on `6ee963e7` only; the round tree
  holds 1,957 tracked `.hero` files to `6ee963e7`'s 1,918.

## Process, for the audit of the sitting

- **The compiler-engineer wrote into the trunk**: `err.txt` (246 bytes, the
  route's `mutate` message naming its own `q6/mutroot/prog/main.hero`) and
  `out.txt` (empty), both modified 2026-10-04 01:56:17 (`ls -laT`), the minute
  of its Q6 `mutate` row; untracked and not ignored (`git status
  --ignored`). The shared brief allows a seat one file in the trunk, its
  report; its report does not mention these. `records`' walk (`git ls-files
  --cached --others --exclude-standard`) reads untracked files too.
- **The ffi-pragmatist read inside the compiler-engineer's copy** (`diff -r`
  of two runtime folders), recorded by it as a slip.
- **The historian could not read `date`**: its charter gives it no shell, and
  the shared brief asks every seat for it. A brief that assigns a rule its
  reader's tools cannot meet; the seat said so.
- **The spec-warden** wrote seven times without a `date` beside them first,
  corrected before it finished (its own record).
- **The compiler-engineer resumed after a session limit** (01:45) and read
  its copy's state before building on it (its own record).
- No seat reports a paid run, `measure --refresh`, or an `rm`.

## The suites the route owes, run here on `6ee963e7` with the route

The compiler-engineer ran `check` whole, `annotations`, `fixes` and `run`
narrowed to `227` (and `run` whole on an earlier build), `emission` and
`layout` whole, the compiler's own tests and the net's own. It did not run
`records` (no git in its copy), and the map in
`.claude/rules/verification.md` sends a `selfhost/lexer.hero` change, which
the route makes, to `probe` and `surface` as well. Run here in `routegit/`
(the route's cases staged, so `records`' `git ls-files --cached --others
--exclude-standard` walks them), with `heroes-route`, the harness's own
command, each output read whole, from 03:28 to 03:58, up to three at a time
and `cache` alone. Baselines: `basegit/` (a second clone at `6ee963e7`, the
seed's compiler) for `records`, and **`lanegit/`, a third, with lane
b8-source's OWN compiler built from its `selfhost/`** (`a30553a70d269b39`),
the fair baseline, since the seed's compiler is the trunk's and lacks
defects 220 and 236:

| suite | route | lane alone |
|---|---|---|
| `records` | **24 passed, 0 failed** | 24, 0 (seed's compiler) |
| `canonical` | 2, 0 | |
| `order` | 3, 0 | |
| `annotations` whole | **625, 0** (the lane's 621 and the four cases) | |
| `fixes` whole | **709, 0** | |
| **`surface`** | **354 passed, 1 failed** | **355, 0** |
| `check` | 451, 0 | |
| `run` | 264, 0 | |
| `emission` | 744, 0 | |
| `corpus` 55, `descriptors` 356, `determinism` 294, `grammar` 9, `lines` 265, `probe` 27, `runtime` 8, `spec` 21, `special` 10, `units` 3, `warnings` 325, `wholes` 356, `ir` 24, `emit` 8, `unsupported` 131, `cache` 7 | **0 failed each** | |
| the compiler's own tests | **1,133, all passed** | |
| the net's own tests | 200, **1 failed**, *the fixtures half ... catches a lie* | **200, the same 1 failed**; with the seed's compiler on the same tree, 200, all passed |

**So the whole net on `6ee963e7` with the route has one red of its own,
`surface`.** The net's own tests fail one test with and without the route,
the lane's own at `6ee963e7` (`8a989fc2` on its tip touches that suite); not
the route's. The compiler-engineer gave its one failure there as the
citation check that needs git; in a clone with git that check passes and the
failure is this other test, with or without the route. These are
`6ee963e7`'s suites, not the round tree's.

**The red, read whole**: `surface/mutate refuses a corpus that does not
compile`, the row `mutate tests/golden/check` at exit 2 whose stderr must
carry *this corpus does not compile* and *already refused by*
(`tests/harness/suite_surface.hero:516`). On the route it carries *error:
`tests/golden/check/fixedbugs-227-a-character-cut-short.hero` is not UTF-8:
the byte 0xE2 at line 6, column 13 is the first of 2 that are not*. The
route's `mutate` stops at exit 2 on its own committed case instead of
counting it a program the compiler already refuses. Two things meet here that
no seat put together: the proposal's *every verb that reads a `.hero` file
tells one ... at exit 1*, against the route's choice that `mutate` and
`measure`, *tools, not compilations*, exit 2 (its item 2, point 6); and the
cases committed in `tests/golden/check/`, which `surface` hands to `mutate`.
`records` holding green settles the compiler-engineer's condition (3) for
`6ee963e7`'s `records`; the round tree's differs by 931 lines and is unrun.

## What I ran, what I did not, and my cost

Ran, in `<scratchpad>/189-critic-pass2/` only: `git archive 7d9f2e8f` and
its compiler; **four** private clones of the trunk's object store (`--shared
--no-checkout`, no network, each its own index), `routegit` with
`route.diff` applied by `patch -p1` and its 31 paths staged by name,
`roundgit` at `6344a5bb` for the dry run only, `basegit` and `lanegit` at
`6ee963e7`; the compilers built from seeds and `selfhost/`; P2's eighteen
files; the route on the blind program, a code page 437 file, a Latin-1 spec
copy, three clang directives, a module beside a text root; the whole net on
the route, and baselines for `records`, `surface` and the net's own tests. Read through git only: `df453108`,
`34836e73`, `cf7d6efa`, the filings 238 to 251 and 275 to 277, and the round
tree's diffs. Read in another seat's folder: only `route.diff` and
`route-runtime.diff`, as the coordinator named them.

Not run: anything on Linux or the Windows box; any paid session; `heroes
measure --refresh`; any web source; the route merged onto the round tree
(the dry run only); the spec-warden's own P2 script (my eighteen are rebuilt
from its table); any timing (the times above are `date` readings, not
durations). Nothing removed: every folder is new, and the one binary I gave
a second name, `routegit/heroes`, is a copy. Up to three suites at once, plus
one watcher loop.
