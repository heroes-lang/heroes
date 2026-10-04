# Panel 189: a source byte that is not UTF-8 is told `not_text`, on each line it stands on, and nothing writes the file back

2026-10-04, written from 04:06 by the clock (`date`). A full panel:
`compiler-engineer`, `ffi-pragmatist`, `spec-warden`, `historian`,
`llm-ergonomist` (a blind experiment of four arms of four, each a fresh
session outside the repository), and the completeness critic over the briefs
first and over the reports after. Convened by the author's answer *3a* of
2026-10-03 (`docs/records/log/2026-10-03-2321-the-author-answers-1a-2a-3a-4a.md`),
the blind seat's paid sessions capped at 5 USD in all (the author's *"5
dollari"*, `docs/records/log/2026-10-03-2343-panel-189-convened-for-not-text-the-blind-seat-capped-at-5-usd.md`);
they spent **2.738**. The trunk was frozen at `7d9f2e8f` from the briefs to
this synthesis. The briefs are `docs/panel/189-briefs/`, repaired after the
critic's first pass before any seat was launched (each keeps the text the
critic read as `<name>-before-the-critic.md`); the reports are
`docs/panel/189-reports/`. **The compiler-engineer stopped at a session
limit** when the account changed, before 01:41, and was resumed at 01:45 with
its state read back (its own record); the ffi-pragmatist, done with its items
1 and 2, was sent back at 02:43 for its item 3, the runtime's C, once the
compiler-engineer's diff existed. **Three faults of
process**, each the critic's: the compiler-engineer wrote `err.txt` (246
bytes, its `mutate` message) and `out.txt` (empty) into the trunk's root at
01:56, where the shared brief allows a seat one file, its report, and its
report does not mention them; the ffi-pragmatist ran `diff -r` inside the
compiler-engineer's copy, recorded by it as a slip; and the shared brief asked
every seat for `date`, which the historian's charter gives no shell to read.
The machine did not sleep during the sitting (the kernel's last sleep is
2026-10-01 at 16:33, `sysctl kern.sleeptime`, read at 04:07). No number below
rests on a duration.

## The proposal

Defect 227 (`blocking`): a `.hero` file holding a byte that is not UTF-8 is
answered *error: cannot read `p.hero`* at exit 2 by every verb, where the file
was read and the author can be told which line holds the byte; a `use`d module
holding one is told falsely that it is not there (`00-facts.md`, F1 and F2).
The proposal (`00-shared.md`): every verb that reads a `.hero` file tells one
that is not UTF-8 with `error[not_text]` at exit 1, at the first byte that is
not UTF-8, naming the file, the line and column and the byte's value; a
`use`d module the same, at its own file. Six questions: **Q1** the code and
its class, with two routes to be listed even to be refused (a bad byte
accepted in a comment; an encoding declaration or transcoding); **Q2** the
message and its fix; **Q3** how many messages, what is read after, and what is
written, with the critic's requirement that no route hand a text with
replaced bytes to a writer (F8); **Q4** where it lives and lands (on lane
b8-source's `6ee963e7`); **Q5** whether the spec owes a sentence; **Q6** the
shapes beside, only those with 227's cause (*a read that is not text taken as
unreadable or absent*) belonging to it.

## The verdict table

| seat | verdict | cost or delta | prediction | condition |
|---|---|---|---|---|
| compiler-engineer | **object** to one message per file, **no veto**; approve `not_text`, exit 1, not a thesis rule, **one diagnostic per line holding such a byte** (at most eight lines, the rest counted), the file **never lexed**, **no writer** handing it back, **no fix**; the route built on `6ee963e7` | **+401 lines** of `selfhost/` (267 of code) in 14 files; one runtime function, `HERO_RUNTIME_ABI` stays 26; `modules.hero` 319 of 320; the reading of the likely encoding 64 code lines | the census moves only the new cases (3 of 1,922 files, exit 2 to 1, on its build) | a blind run showing one message per file repairs in no more turns on a file with bad bytes on several lines; a platform disagreeing; `records` refusing the committed cases; a real source that must stay in another encoding |
| ffi-pragmatist | **approve** (Q4: the runtime, as an addition, bound in the compiler's own extern group, the ABI unmoved), on the condition that **no text with replaced bytes reaches a writer or a digest**; item 3: **approve the runtime diff with two landing conditions** | the diff 0 warnings, 0 errors under the compiler's flags at `-O0` and `-O2` on Apple clang 21, Debian clang 22.1.8 and 18.1.8, the box's clang 23.1.1; its property driver 0 mismatches against Unicode's Table 3-7 at 9,503,115 positions, under ASan, UBSan and LeakSanitizer where each exists | at batch 8's gate, the route's read left outside defects 273 and 274 fails defect 236's test on Linux arm64, *out of memory*; inside, all pass | object if the landing leaves 273 and 274 outside `hero_file_bytes`, or leaves `key_of`'s two branches able to meet; veto only on a platform failing the rebuild or Table 3-7 |
| spec-warden | **approve** the refusal at **0 spec tokens**, no sentence (u0); approve `not_text` as one word for one state, on P2; **object** to c1 (a bad byte accepted in a comment: it falsifies design.md §1.10 and lands in every writer); **veto** e1 (an encoding declaration) and e2 (a Latin-1 fallback: Windows-1252's `€` becomes U+0080, a different valid program) | u0: 0; the other reading u5+r1, -9 and -8 vendored, lower bounds | P1: 227's landing moves the spec by 0 tokens; P2: `not_text` fires on exactly the twelve files `read_file` refuses and on none of the six it reads | a message not stating the rule, or its P4 at 2 or more of 10 (a sentence owed); P2 failing (the shared word withdrawn); `measure` counting a replaced text (an objection) |
| historian (advisory) | **approve** `not_text`, if the words say UTF-8 and the byte; one diagnostic per file, as proposed; UTF-16 named only where the bytes prove it, **no single-byte encoding guessed**; no `certain` fix; not a thesis rule; no writer ever runs on a file that is not text | | (its per-question predictions, § The evidence of its report) | real files needing another encoding; a front end guessing single-byte encodings well |
| llm-ergonomist, blind (16 sessions, four arms: A the trunk's *cannot read*, B `unexpected_character`, C `not_text`, D `invalid_utf8`) | **16 one-turn repairs of 16**, every `c.hero` printing `63 61 66 c3 a9 0a`; every session's context clean (its own folder and the harness's system context) | 2.738 USD, 0.1623 to 0.1794 a session | | the arms sit at a ceiling: the Read tool showed `caf�` and the brief named the word, so **no difference between the arms was measured**, A included (the critic) |

## What the sitting measured

- **The class, on the bytes.** The compiler-engineer's 34 cases (Latin-1, a
  lone continuation byte, `0xFF`, a sequence cut at the end, an overlong form,
  a surrogate, past U+10FFFF, the first byte, several lines, a string, a
  character literal, an `f"..."` piece, a module two uses down): the trunk
  answers *cannot read* at exit 2 through every verb; the route, the 29 that
  are not UTF-8 at exit 1 with `not_text`, the 5 controls as the trunk.
- **Not a thesis rule** (the compiler-engineer): `check --permissive` keeps
  the refusal, and of 28 such files 14 check **clean** read as their shown
  text, a program holding U+FFFD where its author's byte was. A refusal
  without which the compiler builds another program is not a thesis rule.
- **Lexing past the bytes** (the compiler-engineer's Q3 table): one more real
  mistake in 3 of 28 files, a false, duplicate or flooding one in 7. The
  first bad byte of each line stands in the same column under every
  replacement convention, 28 of 28.
- **Writers** (the compiler-engineer and the ffi-pragmatist, each on its own
  build): without the route's guard `check --apply --in-place` wrote `EF BF
  BD` over the author's `E9`; with it `check --apply`, `--apply --in-place`,
  `fmt` and `fmt --in-place` exit 1 and the file is unchanged.
- **Digests** (the ffi-pragmatist's item 3): a header's key follows a one-byte
  edit inside a literal (`0xE8` to `0xE9` rebuilds and prints 233; an
  unedited build rewrites 0 objects); **but `key_of`'s two branches can
  meet**: a UTF-8 file whose bytes are exactly the key a non-UTF-8 file gets
  shares it, and the route reused a stale object, exit 0, printing 4, where
  the base compiler refuses the header.
- **The platforms' writers** (the ffi-pragmatist's item 1, on the Windows
  box): PowerShell 5.1's `>` and `Out-File` write UTF-16 LE with a mark,
  *cannot read* even for ASCII; `Set-Content` writes Windows-1252, `cmd` the
  OEM code page 437, so `82` is `é` in one and `‚` in the other; only .NET's
  default, Git Bash and Notepad's default for a new file write UTF-8 the trunk
  reads.
- **P2, run** (the critic, on its build of the route): `not_text` on exactly
  the twelve files that are not UTF-8 and on none of the six that are.
- **The route's net** (the critic, on `6ee963e7` with the route, against lane
  b8-source's own compiler): every suite green but **one red of the route's
  own, `surface`, 354 and 1**: `mutate tests/golden/check` now stops at exit
  2 on the route's own committed case where the row expects a corpus that
  does not compile reported; the net's own tests fail one test with and
  without the route, the lane's own at `6ee963e7`, repaired at `8a989fc2`.
- **The landing tree** (the critic, by `patch --dry-run` on batch 8's round
  tree): every file of `route.diff` applies but one hunk of `runtime/parts/os.c`,
  the read the route splits, where defects 273 and 274 now sit; five of the
  route's 31 paths have moved there, and `suite_records.hero` differs by 931
  lines.

## Disagreements, unsmoothed

1. **Q3, one message per file or one per line.** The proposal, the
   historian (rustc 1.86 and CPython one per file; gc one per line, ten and
   stop; `go/scanner`, Swift, javac each one) and the compiler-engineer's
   objection. **Nothing run separates them**: the blind arms sit at a ceiling,
   and `mixed-several` (Windows-1252 quotes on lines 4, 7 and 9) shows what
   each prints, not a repair. The critic priced the run that would: two arms
   of four over that file padded to a few hundred lines, the brief not naming
   the characters, about 1.4 to 2.0 USD of the 2.262 the cap leaves.
2. **The encoding's reading.** The compiler-engineer built 64 lines naming
   Latin-1 and Windows-1252 for a one-byte encoding; the historian (*for
   single-byte encodings no front end I read guesses*, P2295's windows-1251)
   and the ffi-pragmatist (`82` is `é` in 437 and `‚` in 1252) caution
   against it, and the critic measured the route's note false on `cmd`'s file
   (*0x82 is `‚` (U+201A) in Windows-1252*, where `cmd` wrote `é`).
3. **`mutate` and `measure`.** The proposal says every verb exits 1; the
   route makes them exit 2, *tools, not compilations*, and so turns
   `surface` red.
4. **Whether `HEROES_RUNTIME` (243) and a `pkg-config` answer (241) share
   227's cause.** The historian and the compiler-engineer read 00-shared's
   definition (*a read that is not text taken as unreadable or absent*) as
   covering both; the ffi-pragmatist and the critic's first pass did not;
   both were filed apart. The critic: *the definition's words favour the
   historian*, and § Bounded discovery keeps a shape with the repair's own
   cause inside the item.
5. **Q5, a sentence or none.** The spec-warden: none (u0), its other
   reading u5+r1 priced; the historian: a sentence has the stronger precedent
   (Rust, Python, C++23 and Zig against Go). The spec-warden's P4, the one
   instrument that would owe one, is a paid generation run nobody authorised.

## The resolution: `provisional — author ratification pending`

The most robust and complete resolution (CLAUDE.md § 4); where it is not the
conservative one, the conservative is named so the author can choose it.

**R1. The code is `not_text`, one word for one state.** The state a program
meets when `read_file` reads bytes that are not text is the state the
compiler meets reading a source: one word (the spec-warden's P2, run and
holding; the compiler-engineer decides on the runtime's own
`HERO_OS_NOT_TEXT`). Exit 1. **Not a thesis rule**: `check --permissive`
keeps it, since the compiler would otherwise build another program.

**R2. Every verb that reads a `.hero` file tells it at exit 1, `mutate` and
`measure` included**, as the proposal says (disagreement 3): the file is the
verb's input and its diagnostic is the input's, which is what exit 1 means
(`.claude/rules/cli-surface.md`). `mutate` over a directory counts such a file
among the programs the compiler already refuses, as it counts every refused
program, so `surface`'s row holds; `measure` refuses it with no count (the
spec-warden's condition, held on the route). The route's exit 2 for these two
is not adopted.

*Corrected 2026-10-04 at 04:19 by `date`, before any landing, on reading the
row R2 names: `tests/harness/suite_surface.hero`'s row *mutate refuses a
corpus that does not compile* runs `mutate tests/golden/check` at **exit 2**
and asks its stderr for *this corpus does not compile* and *already refused
by*: `mutate`'s contract for a corpus holding a program the compiler refuses
is exit 2, the tool unable to run on it. So R2 reads: **every verb that
compiles or prints a program** (`check`, `build`, `run`, `test`, `fmt`,
`lex`, `parse`, `probe`) tells a file that is not UTF-8 at exit 1; `mutate`
meets it as it meets every program the compiler refuses, exit 2 and its
corpus message, the file named with its `not_text`, which is the half the
route got wrong and `surface` caught (its message named the byte alone);
`measure`, a tool over a document, refuses it with no count at exit 2, as the
route has it. The sentence above was written without the row read, and is
wrong on both counts.*

**R3. One diagnostic per line that holds such a byte**, its caret on the
line's first one, at most eight lines and the rest counted in a note (the
compiler-engineer's); **the file is never lexed past**, the other files of
the compilation lexed as before. An author, or a model, told every line in
one turn repairs every line in one turn; told the first, meets the second
only after the fix, which is § Bounded discovery's *adjacent* shape written
into a rule. **Conservative alternative**: one per file, at the first bad
byte (the proposal, the historian, rustc 1.86, CPython). **Unmeasured** which
repairs faster on a file with several such lines; the critic's two-arm run,
within the cap, is the instrument (disagreement 1), for the author to fund
or not.

**R4. The message names what the bytes prove, and nothing they do not.**
Its headline names the byte's value, its line and column and UTF-8 (*the
byte 0xE9 at 2:15 is not UTF-8, and a `.hero` file is UTF-8 text*; the
historian's condition). Its note names an encoding **only where the bytes
prove it**: a UTF-16 byte-order mark (`FF FE`, `FE FF`), NUL bytes at every
other position (UTF-16 without a mark), a UTF-8 sequence cut short or one
UTF-8 forbids (overlong, a surrogate, past U+10FFFF). A byte that begins no
UTF-8 sequence is told as a byte from an encoding other than UTF-8, with the
route to save the file as UTF-8, **naming no single-byte encoding**, since
Windows-1252, Latin-1, the OEM code page 437 and Windows-1251 each read it
differently (disagreement 2; the critic's measurement on `cmd`'s file).
**Conservative alternative**: the compiler-engineer's reading as built,
naming Latin-1 and Windows-1252, true for PowerShell's `Set-Content` and
false for `cmd`.

**R5. No fix, `certain` or `guess`**, for a byte in a string or in a comment:
applied, any fix hands back the shown text with U+FFFD over every other such
byte, and which character a byte stood for is an encoding guess
(`.claude/rules/diagnostics-and-goldens.md`; the historian found no front end
offering a transcoding fix).

**R6. No writer receives a text with replaced bytes, and no digest is taken
of one.** `check --apply` (and `--in-place`) and `fmt` (and `--in-place`)
refuse a file that is not text at exit 1 and leave it unchanged (held, two
builds); a header's or the runtime's key is taken over what was read in a
form the UTF-8 branch can never produce, so the two branches cannot meet
(the ffi-pragmatist's case 5 refusing the build at the landing is the test;
the route's `key_of` as built does not hold it).

**R7. Where it lives.** One runtime function that reads a file's bytes and
reports where they are not UTF-8, the rule's one home (`hero_utf8_valid`
re-expressed through it), bound in the compiler's own extern group, never a
library group; `HERO_RUNTIME_ABI` stays 26, a runtime lacking the function
being a compile error naming it (the ffi-pragmatist, measured on four
clangs). **Defects 273 and 274 move inside the one read every source goes
through** (`hero_file_bytes`): a directory fails before a byte is read, every
file is read to its end, `file_not_found` only for `ENOENT` and `ENOTDIR`
(the ffi-pragmatist's composition, 1,133 of 1,133 on Linux arm64).

**R8. The shapes with 227's cause land with 227**: the runtime's own
sources, the runtime's cache key and a C header's digest (in the route), and,
by the sitting's own definition (disagreement 4, ruled the historian's way),
**a `pkg-config` answer that is not UTF-8 (241) and an environment value that
is not UTF-8 (243)**. Their files stand as filed, each with its reproducer,
and close with 227's landing, in its lane.

**R9. Defect 244 lands with 227**: the excerpt under a diagnostic writes a
source line's control characters by their code. Without it the route's own
message on PowerShell's default file, UTF-16 with a mark, prints 15 and 16
NUL bytes to the terminal (the critic), so 227's commonest Windows case would
be told with raw bytes.

**R10. No sentence in the spec** (u0, 0 tokens): § 1 already says comments
and strings are UTF-8 and the message states the rule where it is broken
(panel 055's precedent; panel 188's R10). The historian's precedents for a
sentence and the spec-warden's u5+r1 are recorded; its P4, unrun and unpaid,
is the instrument that would owe one.

**R11. Routes refused**, each with what would make it wrong:
- *a bad byte accepted in a comment, refused in a string* (c1): it falsifies
  design.md §1.10's *"Strings and comments are full UTF-8"*, needs the
  scanner to know a comment in a file it cannot lex, and a writer that keeps
  raw bytes, which this compiler does not have; wrong if a byte-preserving
  writer and a measured need for such comments exist;
- *an encoding declaration* (e1) and *a Latin-1 fallback* (e2), vetoed by the
  spec-warden: a new form with §1.0's burden unmet, one program in several
  byte spellings against §4.15, and e2 turning Windows-1252's `€` into U+0080
  silently; wrong only on a measured population of writers that cannot
  produce UTF-8 at all.

**R12. The shapes with other causes are filed apart** (§ Bounded discovery):
238 to 251 and 275 to 277 in batch 8's round tree, and from this sitting's
second pass:
- 281, the compiler's own argument that is not UTF-8 on Linux and this Mac:
  exit 2 with a note telling the compiler's user to rebuild with
  `args_checked()`, so a correct program in a file so named cannot be checked
  there (238 is Windows' narrow API alone);
- 282, the `unexpected_character` flood: 32 KB of control bytes drew 31,600
  diagnostics and 8.7 billion instructions, `shown_char` rebuilding its table
  at every call (the compiler-engineer's `q6/unseen`; 247 is a two-line
  file's thirty);
- 283, bidirectional controls (U+202E, Trojan Source's character) and U+2028
  accepted in comments and strings at exit 0 (the spec-warden), which no
  sitting has ruled on;
- 284, clang's text read back holding a byte that is not UTF-8: clang 21
  escapes it as `<E9>` in `#warning`, `#pragma message` and `#error` (the
  critic), which leaves a header's path, Linux only, unmeasured.

**R13. The landing.** One lane on batch 8's closed tree, the route rebuilt
there with R2 to R9 (the critic's dry run: one hunk of `os.c` to compose, five
paths moved, `suite_records.hero` 931 lines on): 227, 241, 243 and 244
together. Its gate: the 227 cases, the compiler's own tests, `check`,
`annotations`, `fixes`, `surface`, `records`, the census over the tracked
files and the cases; the platforms before its push, the Windows box running
the route's compiler on the item-1 writers' files (the ffi-pragmatist's
prediction 1, unscored by this sitting); P1 and P2 scored there.

## Predictions to score

- **The spec-warden's P1**: 227's landing moves the spec by 0 tokens; scored
  at the landing's gate, lapsing at the next `m-*` tag.
- **The spec-warden's P2**: `not_text` fires on exactly the twelve files
  `read_file` refuses and on none of the six it reads; held on the critic's
  build of the route, scored again on the landing compiler.
- **The ffi-pragmatist's prediction 1**: on the Windows box, the landing's
  compiler names line 2, column 10 on `setcontent-e.hero` and line 1,
  column 1, byte `0xFF` on `redirect-a.hero`; unscored here (the route's
  compiler never ran on the box).
- **The ffi-pragmatist's item-3 prediction**: with 273 and 274 outside the
  route's read, defect 236's test dies *out of memory* on Linux arm64 under
  both clangs; inside, all pass. Its first half is the failure batch 8's own
  leg met on the trunk's read (defect 273), the same mechanism.
- **The compiler-engineer's**: the census moves only the new cases.

## The critic's passes

**First, over the briefs** (`completeness-critic-briefs.md`): the briefs'
framing facts re-run in its own copy; the blind briefs repaired, because the
first design explained in its own prose that the page could not hold the
file's byte, giving the arm without a message what the messages say; the
platform runs found assigned to no seat, so the ffi-pragmatist was convened.

**Second, over the reports** (`completeness-critic.md`, 395 lines): the
route applied and built (`heroes-route`, sha256 `1cd0651051b9160e`) and the
whole net run on it; ten things missing, each with the command that settles
it. This synthesis answers them so: `surface`'s red by R2; per file against
per line by R3 and its unfunded run; no arm reading the real message, and the
reading's 437 error, by R4; the NULs by R9; `key_of` by R6; the landing tree
by R13; four shapes neither in the route nor filed by R12; P2 run (holding),
P4 and the Windows prediction unscored (§ Predictions); 241 and 243 by R8;
the three faults of process in this file's first paragraph.

## Author's verdict

**Pending**, queued as the DECIDE item `panel 189`, which puts the
ratification, R3's and R4's conservative alternatives, and the critic's
two-arm run (about 1.4 to 2.0 USD within the 2.262 the cap leaves) as the
author's to fund or not. A yes settles R1 to R13 as written, the robust sides
of R2, R3, R4 and R8 included; it does not settle what only the landing can
measure (the spec-warden's P1 and P2 on the landing compiler, the
ffi-pragmatist's Windows prediction), which the lane that lands 227 scores,
and the two-arm run is a yes of its own, with its cost.
