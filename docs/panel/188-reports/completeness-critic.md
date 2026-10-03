# Panel 188, the completeness critic's second pass: the reports

Written 2026-10-03 from 17:17 by the completeness critic, over the six
reports in `docs/panel/188-reports/` (`compiler-engineer.md`,
`ffi-pragmatist.md`, `spec-warden.md`, `historian.md`,
`llm-ergonomist-a.md`, `llm-ergonomist-b.md`) and the briefs as repaired
after my first pass (`docs/panel/188-briefs/`, the `*-before-the-critic.md`
files being what that pass read). No verdict on the routes. No paid run, no
web search or fetch. Every re-run is in my own copies, never in a seat's:
`<scratchpad>/188-critic/` (826ddc2f, the trunk's compiler `afc05be6`) and
`<scratchpad>/188-critic-1h/` (826ddc2f with the compiler-engineer's patch
applied). Where the compiler's own words hold an em dash, this report writes
a comma in its place. Written as I go; in progress until the closing section
says otherwise.

`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`;
my files for this pass are under `<scratchpad>/188-critic/work2/`.

## 1. The compiler-engineer's (1h), re-run in my copy

**The patch is the whole change.** `route-1h.patch` (copied out of the
seat's folder, sha256 `c20d5918dd607ae3`) holds 13 files, 6 of the compiler
(`cli/libraries`, `diag`, `emit/externs`, `emit/ffi_build`, the new
`head_names`, `parse/group_head`) and 7 of `tests/golden/check/`. `patch -p1
--dry-run`, then `patch -p1`, onto a fresh `git archive 826ddc2f`: clean, no
offset, no fuzz. A read-only `diff -rq` of the seat's `selfhost/`,
`tests/golden/check/`, `runtime/`, `tests/harness/` and spec against my
patched copy prints nothing, so nothing the seat built lies outside the
patch. Built by the trunk's compiler from that copy: `heroes-1h`, sha256
`9695251491d1b056`, not the seat's `f3e0d6a6a5a8d237`; the trees being equal,
the difference is where each was built, not what.

| the seat's claim | my command | my result |
|---|---|---|
| `selfhost/parse/` 8,689 to **8,660**, 36 of room | the mirror `code_lines.py` (checked against the suite in my first pass) over `selfhost/parse/*.hero`, both trees | **8,689 to 8,660, 54 files, room 36**: verified |
| `layout` whole 5 and 0, `budget` asked | `./heroes-1h run tests/harness/main.hero -- ./heroes-1h layout`, whole | **5 passed, 0 failed**: verified |
| `check` form whole 447 and 0 | the same, `check` | **447 passed, 0 failed**: verified |
| the leaf 157; `group_head` 140 to 111; `externs` +6; `diag` +4; `libraries` 297 to 298; `ffi_build` 248 to 229; the compiler 73,970 to 74,090 (+120) over 389 to 390 modules | the mirror per file and over `find selfhost -name '*.hero'` | every figure as stated: verified |
| **66 cases**; on the trunk 8 at exit 2 or 134 on this Mac, on the route **0 of 66** | the seat's generator `make_cases.py` copied and pointed at my folder (it writes **63**), plus the three programs it does not write, copied by hand (`q5-package-constraint`, `-list`, `-option0`, made outside the generator); my runner, the trunk's compiler and `heroes-1h`, each on its own copy of the 66 folders | **66**; the trunk's rows at 2 or 134 are exactly the seat's corrected eight (`f1-gt`, `f6-angle-both`, `f7-nul`, `x-angle-own`, `q5-link-empty`, `q5-link-squote` at 2; `q5-link-nul`, `q5-package-nul` at 134); `heroes-1h`: **none at 2 or 134, no internal error**; 28 rows change exit codes. Verified, with one note: the count holds only with the three hand-made cases, which no script regenerates |
| **the census, 0 moved of 1,073** (846 at exit 0 and 227 at exit 1 on the trunk) | the file list rebuilt by the seat's stated rule (every `.hero` outside `tests/golden/check/`, `selfhost/` and `archive/`, plus `selfhost/main.hero` once): **1,073 files, identical to the seat's list**, `comm` both ways empty; `check --brief` by the trunk's compiler and by `heroes-1h`, text and exit compared, three at a time | **0 moved**; the trunk 846 at 0 and 227 at 1. Verified, and **for the final route**: the seat's own census ran against stage B (`census_one.sh` names `heroes-b`, and its § 6 says so), not the stage C it proposes. Stage C changes only the linker's reader, which `check` never reaches, so the result carried; it is now run rather than carried |

**And the route as built aborts on correct programs, which neither the 66
cases nor the census could see.** The compiler-engineer found it and wrote it
up in its § 13 after I had started; I re-ran it on my build of stage C, and
also re-ran my first pass's shapes, which I should have done first
(`<copy>/work2/firstpass-on-1h/`):

| `extern "..."`, the header beside it | the trunk | stage C (`heroes-1h`) |
|---|---|---|
| `"é.h"` (my first pass's `non-ascii`) | check 0, build 0, prints `7` | **check 134, build 134**: *panic: string slice splits a character* |
| `"éa.h"`, `"aé"`, `"日本.h"` | check 0 | **134** each |
| `"aéb.h"` (above ASCII in the middle) | | check 0, build 0, `7` |
| `"ab.h" link "é"`, `"ab.h" package "é"` | | check 0, build 1, true messages (the slicing is the header's alone) |
| my first pass's other 17 shapes (empty, padded, `\t`, `\r`, raw TAB, raw NUL, `<`, `d/`, `~`, `..`, the three absolute `link` and `package` forms) | | no exit 2 or 134 |

The cause, by the seat's reading and mine of `head_names.hero:152-155`:
`c_spelling` slices the value from byte 1 to `len - 1` before reading that
either end is an ASCII bracket. So **"no exit 2 or 134" holds for the 66
cases and is false of stage C as a route**, as the seat now says; it is a
crash where the trunk builds, which is `blocking`, and the census of 1,073
programs read 0 moved because no program in the tree names such a header.
The lesson is the instruments', not only the seat's: a census can only
report the population it holds, so the shapes beside a new refusal have to
include the bytes the population lacks (a character above ASCII at either
end: the facts' first text listed *"a name holding a byte above ASCII"* as
unrun, and my first pass ran `é.h` on the trunk only). The seat's stage D
repairs it (its § 13); its section is labelled *"Corrected 17:52"* in a file
last written at 17:39 by `ls -l` and read at 17:42 by my `date`: a time later
than the file itself, so the label is wrong, as F6's *15:58* was.

**What the seat did not run on its final route, and the change reaches.**
Stage C was gated by the compiler's own tests, `check` whole and `layout`
whole; `emission`, `surface`, `canonical`, `annotations` and `fixes` ran on
stage B. **Nothing that builds a program with a `package` clause ran on
either**, and `cli/libraries.hero:294` now puts `--` before every package
name, so every such build changes. Fifteen tracked programs carry a
`package` clause (`grep -rlE '...package' --include='*.hero'`, `archive/`
excluded): five `run` goldens (raylib, sdl3), `examples/raylib` and
`examples/sdl`, two `unsupported` cases, three `check` cases and three panel
probes. The suites that build them are `run` and `corpus`, and
`.claude/rules/verification.md` names `check run emission determinism
corpus` for any change to what the checker refuses. I ran `run`, `corpus` and
`unsupported` whole on `heroes-1h` (§ 1a, below). On Windows the `--` is
unrun (the seat says so), and Windows is where a different `pkg-config`
might read it differently.

### 1a. The suites the seat did not run on its final route, run

All on `heroes-1h`, from my patched copy's root, each output to a file read
whole (`<copy>/work2/gate-*-1h.txt`):

| run | result | why it was owed |
|---|---|---|
| `run`, whole | **261 passed, 0 failed** (the sixth gate's count); raylib and sdl3 installed and answering `pkg-config --cflags --libs -- <name>` on this Mac, so the five `package` goldens were built through the new `--`, not skipped | builds every `run` golden under (1f)'s emission and the `--` |
| `corpus`, whole | **55 passed, 0 failed** | builds `examples/`, `raylib` and `sdl` among them |
| `unsupported`, whole | **131 passed, 0 failed** | two of its cases carry a `package` clause |
| the compiler's own tests | **1,105 tests, all passed** | the seat's stage C figure, re-run |

So on this Mac nothing the route reaches broke where the seat did not look.
Not run by me: `emission`, `determinism`, `wholes`, `descriptors` (the
`selfhost/emit/**` row; the seat ran `emission` on stage B, 738 and 0, and
stage C's one further change is the linker's reader, which no emission
reaches), `warnings`, `surface`, `probe`, the seed and its fixpoint, Linux
and Windows. The full net is owed at the round's gate in any case.

## 2. Four seats approve "(1h)", and no two mean the same thing

The repaired brief defines (1h) as *"a combination: a refusal at `check`
together with (1d) or (1f)"*, which leaves the refusal's set open, and each
seat filled it differently. Read from the four reports (the blind arms judge
no route):

| element | compiler-engineer (built, stage C) | ffi-pragmatist | historian | spec-warden |
|---|---|---|---|---|
| (1f), the value | yes | yes | yes | yes |
| `>`, LF, CR, NUL, the empty name, in a header | refused | refused | refused | refused ((1a)) |
| the same in `link` and `package` | refused: empty, NUL, LF, CR | not proposed as a refusal; its Q5 rows filed apart | not proposed (Q5: Go's `SafeArg` as precedent) | not addressed |
| other control bytes (TAB, U+0001 to U+001F, U+007F) | **admitted**: TAB, 0x01 and DEL build and print `7` (its § 5 table; my run, `x-raw-tab`, `x-raw-soh`, `x-raw-del`, `f7-tab`, `x-tab-first`) | **refused** | **refused** (*"(1a)'s 'a line end' should read 'a control character'"*) | not addressed |
| `\` and `"` | refused, `undefined_header_name` | refused | refused ((1b)) | **admitted**: objects to (1b) |
| `'`, `//`, `/*` | refused, `undefined_header_name` | **admitted** (*"adding (1b)'s ... costs this seat nothing"*) | refused ((1b)) | **admitted** |
| `--` before a package's name | built | proposed (§ 4.2) | precedent points elsewhere: Go **refuses** a leading `-` or `@` (`SafeArg`) | |
| `-Werror=extra-tokens -Werror=null-character` | **not built, not mentioned** | proposed *"under any route"* | | |
| the linker's reader | rebuilt (stage C) | filed apart (its item 3) | | |
| the certain fix for `"<name>"` | certain where what is left passes the judgement | | certain for exactly `"<" name ">"` | |
| a spec sentence | none needed (§ 12; `aab44f9b`'s ground) | | if any, exactly the checker's set | a6, paid by r1 |

So **"four seats approve (1h)" is not a consensus on a route**: they agree on
(1f) and on the floor (`>`, LF, CR, NUL, the empty name, in a header) and on
nothing else. The synthesis has to choose element by element: rows 3 to 9
are each held one way by at least one seat and another way, or not at all,
by the rest. The coordinator's stage D, announced at 17:35, refuses the
control bytes in all three strings (rows 3 and 4, the ffi-pragmatist's and
the historian's side), makes a package name one package (§ 4's grammar
row), and, by its note of 17:46, replaces the backstop of row 8 with the
pragmatist's scoped guard; § 9 says whether its sections landed before I
closed.

*The coordinator's correction, 2026-10-03 at 21:40 by `date`: "17:35" and
"17:46" are the labels the coordinator wrote on its notes, guesses rather
than clock readings; the notes reached this seat at 17:30:45 and 17:44:12 by
its transcript's timestamps.*

What each disputed row rests on, and which part is checkable:

- **Control bytes.** Both builders measured the same fact: clang carries
  every byte but LF, CR and `>` (the ffi-pragmatist's sweep of 0x01 to 0x7F
  on three clangs; the compiler-engineer's TAB, 0x01, VT, FF, DEL). The split
  is policy: the ffi-pragmatist's grounds are a survey (0 of 103,736 real
  names hold one), that *"a message cannot show one"*, and the (1g)
  objection; the historian's is gc's precedent. None of the three is a
  failure of the admitted bytes; the compiler-engineer's admission rests on
  them building. My one check of *"a message cannot show one"*: the route's
  own NUL message prints the source line with the raw NUL byte in it
  (`od -c` of `cases-1h/f7-nul/out-check.txt`: one `\0` in `1 | extern
  "a\0b.h"`, seven carets under the seven bytes of the string), so a message
  quoting a control byte quotes it raw, as the pragmatist says; the
  excerpt's renderer is the compiler's general one, not this patch's.
- **`'`, `//`, `/*`** and **`\`, `"`**: § 3.
- **`--` against a refusal of a leading `-`**: `--` depends on every
  platform's `pkg-config` reading `--` as the end of options, measured on
  pkg-config 3.0.7 (this Mac) and pkgconf 1.8.1 (Linux arm64) by both
  builders, **unrun on Windows**; a refusal at `check` depends on no tool.
  Neither is wrong on the evidence; nobody compared them.

## 3. The (1b) dispute: two questions, and what each side rests on

The coordinator's summary, *"the spec-warden objects to the refusal and
vetoes any (1b) sentence, the compiler-engineer approves (1b) on the
historian's confirmation, the ffi-pragmatist approves it at no library
cost"*, joins two questions the reports keep apart.

**(a) A (1b) sentence in the spec. Nobody proposes one.** The spec-warden
vetoes b1, b2 and b3 on §1.0's burden (redundancy where nobody errs, §1.4);
the compiler-engineer wants no sentence (*"the refusal's messages state the
rule each time, which is `aab44f9b`'s ground"*); the historian says a
sentence, if any, states exactly the checker's set; the ffi-pragmatist does
not price sentences. So the veto meets no proposal, and the generation run
(13 of 13 sessions, no writer of any of the five marks, the task giving no
reason to write one) adds nothing against it.

**(b) The (1b) refusal.** For it: the compiler-engineer (approve *"on
condition"*), the historian (approve), the ffi-pragmatist only for `\` and
`"` (its own (1h) refuses those two and admits `'`, `//`, `/*`, with no
objection either way). Against it: the spec-warden, **objecting, not
vetoing, and on no budget ground** (its table says so in those words): the
refusal *"turns away names clang builds today ... for 0 measured
writers"*. Which part of each side is checkable:

| claim | side | status |
|---|---|---|
| C11 6.4.7p3 leaves `'`, `\`, `"`, `//`, `/*` between `<` and `>` undefined, unchanged in C17 (by N2310's unmarked page), C23 and the C2y draft N3783 (6.4.8p3) | for | read by the historian from the drafts' page images, with URLs and date; I cannot fetch them, and nothing in the tree contradicts it (`selfhost/emit/c_text.hero:14` already cites 6.4.7 for *"no escapes"*) |
| the compiler-engineer's approval is *"on the historian's confirmation"* | for | **half its condition**: its report makes (1b) rest on the text **and**, for `\`, *"a Windows behaviour unrun"*. The text is confirmed; the Windows half is not run, and the historian's evidence for it is documentary: MSVC's pages describe `cl.exe`, and the one source about **clang** on Windows is a 2020 commit message (*"since clang treats it as normal repeated path separators"*) |
| the `\` message's sentence *"on Windows a `\` separates directories and elsewhere it is a byte of a name"* | for | in the patch as built (`head_names.hero:143`), and its author says it is *"recalled, unrun"* and must be run before it lands; the coordinator's stage D rewords it (§ 9) |
| `'`, `//`, `/*` build and print `7` today | against | true: F1, and three clangs on two platforms with zero diagnostics under `-Weverything` (the ffi-pragmatist); Windows unrun |
| no writer of the five | against | true on every instrument here: 0 in the tree's header strings (all four counts, § 5), 0 of 103,736 real header names (the ffi-pragmatist's survey, this Mac and Linux), 0 of 13 generation sessions |
| no library cost | neutral | true on this Mac and Linux, by the survey; `//` cannot occur in a file's path at all, so the survey is vacuous for it (the ffi-pragmatist says so); Windows's SDK unrun |

**What none of the four says, run: Heroes hands its C to clang alone.** The
CLI runs `program: "clang"` by name (`selfhost/cli/compiling.hero:167-169`
and `:297-306`, `selfhost/cli/clang_floor.hero:106-108`), and the only
environment variable it reads is `HEROES_RUNTIME` (`grep -rhoE 'env\(name:
"[A-Z_]+"\)' selfhost`). So C's undefined behaviour reaches a Heroes program
only through clang's own reading, and the historian's GCC-against-clang
prediction is not a Heroes fact.

**And clang's own reading has a hole the seats' tables miss, measured on this
Mac (Apple clang 21.0.0).** Both builders measured a trailing `\` carried
(`ab\`, `x-trailing-bs`), and it is, alone. But a `\` just before the
closing `>` stops clang's angled lexing, as the historian read in clang's
source (*"Skip escaped characters"*): `#include <a\>b.h>` opened the file
`a\>b.h`, silently (`<copy>/work2/bs-gt/`). Then clang re-reads the line as
tokens, and the name is rebuilt from them. Controls in
`<copy>/work2/bs-gt2/` and `bs-gt3/`, each with both candidate files on
disk, `clang -std=gnu11 -I. -Wall -Wextra`:

| the `#include` line | clang opened | diagnostics |
|---|---|---|
| `<a  b.h>` (two spaces, no `\`) | `a  b.h` | none |
| `<a  b\c.h>` (a `\` inside) | `a  b\c.h` | none |
| `<ab\>` (a trailing `\` alone) | `ab\` | none |
| `<a  b\>` (two spaces, then a trailing `\`) | **`a b\`, one space: another file** | none |
| `<a`TAB`b\>` | **`a b\`: another file** | none |
| `<d//b\>` | nothing | *expected '>'*: the `//` became a comment |

**Through a compiler, the spec-warden's route measured.** In a third copy of
my own (`<scratchpad>/188-critic-no1b/`), the patch applied and
`head_names.undefined_mark` made to return nothing, so (1f) and (1a) stand
and (1b) is gone; built by the trunk's compiler, `heroes-no1b`; the same five
names (`<copy>/work2/trail-*`), the file the string means returning 7 and the
other one 9:

| `extern "..."` | the route as built | (1a) + (1f), no (1b) | the trunk |
|---|---|---|---|
| `"ab\\"` | check 1, `undefined_header_name` | builds, prints `7` | build 1, false `ffi_missing_header` (the escape face) |
| `"a  b\\"` | check 1 | **builds, prints `9`: another header bound, silently** | build 1, the escape face |
| `"a\tb\\"` | check 1 | **builds, prints `9`, silently** | build 1, the escape face |
| `"d//b\\"` | check 1 | build 1, `ffi_unknown_name`, *"clang read the header and could not find it"*: **false**, clang never read it | build 1, the escape face |
| `"e/*b\\"` | check 1 | **build 2**, *internal error*, clang's *unterminated /\* comment* | build 1, the escape face |

So **(1f) without a refusal of `\` turns today's false message into a wrong
program accepted, a false message and an exit 2**, all three `blocking`, on
the one compiler Heroes runs. This is measured ground for refusing `\` (or at
least a `\` at a name's end), beyond C's text and beyond the unrun Windows
behaviour, and it weighs against the spec-warden's objection for `\` only:
it says nothing for `'`, `//` or `/*`, which every clang here reads as
themselves unless a trailing `\` sends the line down the token path. The
ffi-pragmatist's (1h), which refuses `\` and admits the other three, is not
touched by it. Windows's clang is unrun for all of it.

**The thesis-rule classification, checked and consistent.** `diag.hero:93-95`
defines a thesis rule as one *"the THESIS adds, as opposed to a rule without
which the program has no meaning"*, and admits `machine_locked_path` because
*"C would take the program on exactly one machine"*; `undefined_header_name`
meets the same test (without it clang gives the program a meaning), and
`unwritable_name` rightly does not. By the measurement above, the `\` half of
`undefined_header_name` is closer to robustness than to the thesis: without
it, three of five shapes bind another header or fail. Whether `\` should sit
apart from `'`, `//`, `/*`, so that `check --permissive` keeps it, is a
question no seat was asked.

## 4. Q5's findings: the item of 216, or filed apart

The rule (`.claude/rules/verification.md` § The batch, amended by § Bounded
discovery): *"only a shape with the repair's own cause stays in the item;
every other real defect found beside it is filed apart"*, depth one from the
item's reproducer, and filed with its own class (a crash, an exit 2, a false
message or a wrong program accepted is `blocking` wherever it is found; the
two builders class their findings that way). The rule needs the item's cause
written down first, and **216's item does not have one that reaches past a
header**: its line names `>` in a header, and its class line *"No angled
`#include` can spell a `>`"*. The repaired shared brief widened the class to
three causes for a header (what `#include <...>` cannot carry; escapes not
decoded; a malformed name accepted). So the partition below is by mechanism,
for the coordinator to adopt once 216's cause is written:

| finding (who ran it) | its mechanism | the same cause as 216's? | if filed apart, its class by the list |
|---|---|---|---|
| a NUL in `link` or `package`: the compiler aborts, 134 (both builders; my trunk run, `q5-link-nul`, `q5-package-nul`) | a group head's string carries a byte its destination (a spawned argument) cannot carry, and nothing judges it at `check` | **yes, if 216's cause is written as the string's, not the header's**: the header's NUL (F7) is already in the class, and the route's one judgement covers all three strings. No, if it stays *"`#include` cannot carry"* | `blocking` (a crash) |
| `link ""`: a bare `-l` takes `-o`, exit 2 (both builders; my `q5-link-empty`) | the empty string's destination cannot carry it | the same answer: the empty header (F7) is in the class | `blocking` (an exit 2) |
| `package "--atleast-pkgconfig-version=0"` accepted at exit 0; `"--version"` and `"-x"` called packages, the second's *"not installed"* false (both builders) | `pkg-config` reads a name beginning with `-` as an option | **no**: the string is carried whole; the tool's parse is the cause, and its repair is `--` or a refusal of a leading `-`, not 216's judgement | `blocking` (a wrong program accepted; a false message) |
| `package "zlib >= 1"`, `"zlib zlib"`, `"zlib,sqlite3"` accepted; `"zlib >= 99"` *"not installed"*, false (the ffi-pragmatist; the compiler-engineer's open item; my `q5-package-constraint` and `-list` build on both compilers) | `pkg-config`'s list and version grammar | **no**, and it needs a ruling (a feature or a hole) that no rule reaches: by the list, `systemic`, which goes to a sitting or the author, and this sitting is one | `blocking` for the false message; the grammar itself a ruling |
| `link "a'b"` (this Mac) and `link "a b"` (Linux) at exit 2 (both builders; my `q5-link-squote`) | the compiler's reader of the linker's line | **no**. But (1f) makes `link "a\tb"` exit 2 on Linux (the compiler-engineer's § 8), so (1f) for a `link` string cannot land without stage C's reader or without refusing whitespace and control bytes in a library's name: filed apart, landed with or before (1f) | `blocking` (an exit 2) |
| `machine_locked` misses a `..` climb to an absolute path, and a `.pc` path read from the working directory (the ffi-pragmatist) | what `machine_locked` judges | no | `adjacent` (the pragmatist's class) |

**The same rule, applied to the sitting's own widening.** F8's escapes are
a different mechanism from 216's `>`: a reader that does not decode, repaired
by (1f), where the `>` is repaired by a refusal. By the rule as written they
are their own item (a false message, `blocking`), filed apart and landed in
the same batch, since the refusal must read the decoded value. Treating the
Q5 rows by the rule and F8 not would apply it to the findings and not to the
framing.

## 5. Other claims, checked

| claim (seat) | my command | result |
|---|---|---|
| design.md §1.6's ceiling at line 255; §1.4 at line 235, *"redundancy is spent deliberately, where errors actually occur, and nowhere else"*; §1.0's burden at lines 379-381 (spec-warden) | `sed -n` each | verbatim; verified |
| panel 181's warden: *"the refusal can be inferred, but nothing forces the inference: a rule a reader can miss, which is the case for stating it"* | `grep -rn -F 'a rule a reader can miss' docs/` | `docs/panel/181-reports/spec-warden.md:107-108`, verbatim; verified |
| `345f167b` names panel 181's sentence as the first application of the author's 2026-09-28 instruction | `git log -1 --format=%b 345f167b` | *"panel 181's spec sentence is its first application"*; verified |
| the reader's instrument reads 1.17 to 1.24 times the cl100k delta (ledger rows 6928 and 7117) | the two rows read with a script | 129 / 104 = 1.24 and 228 / 194 = 1.18; verified |
| a6+r1 re-wrapped -12 legacy and -13 cl100k; b3 +26 and +25; r1 -22 and -22; each draft changes only its sentence (spec-warden) | the three drafts copied out, `diff` against the spec, `heroes measure <draft>` offline on my compiler | each `diff` shows only the stated sentences; **-12 / -13, +26 / +25, -22 / -22**: verified, vendored, lower bounds |
| 648 header lines and 250 names in the tree (spec-warden, ffi-pragmatist); 832 (mine, first pass); 848 lines and 314 names (compiler-engineer) | the same `extern "..."` grep under each scope | **648 and 250 are `.hero` files only; 832 and 286 add `.md` and `.txt`**; every file type outside `archive/` reads 847 on the trunk and 867 and 300 in my patched copy, so **848 and 314 do not reproduce from the scope the compiler-engineer wrote**, and its command is not written down. Harmless: the census it backs is run with the compiler (§ 1) |
| 19 call sites of `emit_externs.unquoted` across 8 files (ffi-pragmatist) | `grep -n ' unquoted(s' selfhost/emit/externs.hero` | **the pragmatist is right and my first pass was wrong**: I wrote *"called on 15 lines as `emit_externs.unquoted` and on 5 inside `externs.hero`"*, and one of those 5 is the function's own definition (line 62), so 15 + 4 = 19 calls. Corrected here, 2026-10-03 |
| `extern "stdio.h<NUL>x"` binds the system's `stdio.h` and runs (ffi-pragmatist, § 1.4) | `<copy>/work2/fp-checks-*` | the trunk: check 0, build 0, prints `hi`; the route: check 1. Verified |
| a `..` climb reaches an absolute location past `machine_locked` (ffi-pragmatist, § 4.3) | the same, with as many `../` as reach `/` and then this folder's absolute path | the trunk and the route both: check 0, build 0, prints `7`. Verified; the route does not cover it |
| the historian's *"which five escapes spec § 2 defines"*, left unverified | `spec/heroes-spec.md:47` | exactly `\n` `\t` `\r` `\\` `\"` in a string, the five it named; verified |
| the historian's prediction 2, clang's half: *"clang names `a\>b.h`"* | `<copy>/work2/bs-gt/` | clang opened `a\>b.h`; verified. GCC's half is unrun and, by § 3, not a Heroes fact |
| the thirteen generation sessions wrote byte-identical files (llm-ergonomist-gen) | `md5 -q` over the session folders (read only); `cmp` of each `spec.md` | one md5, `b0ac3760...`, on all thirteen (and the coordinator's `build/` copy); every session's spec equal to the trunk's; verified |
| the blind arms' *"predictions 90 and 97 of 100"* (the coordinator's summary) | the two reports | each is the session's own guess under `prediction`, not a measurement; **the A/B measured one repair in one session per arm, both successful, so it measured no difference between the arms** |

## 6. Routes nobody listed, after the reports

1. **Give `"<name>"` Nim's and Cython's meaning instead of refusing it.** The
   historian found that both define a bracketed header string as `#include
   <name>`, and that Nim's own library writes `header: "<math.h>"`; CLAUDE.md
   § 6 says to copy Nim's surface. Accepting the spelling would make the
   bracketed form a second spelling of one header, which the thesis
   resists, and would need no fix at all; the historian cites the precedent
   only for the fix's certainty, and no seat weighs it as a route.
2. **Refuse a package name beginning with `-` (Go's `SafeArg`, the
   historian's precedent) rather than, or as well as, passing `--`.** The
   compiler-engineer built `--`, which depends on each platform's
   `pkg-config` (Windows unrun); a refusal at `check` depends on no tool,
   and the two together are defence in depth. Nobody compared them.
3. **Refuse `\` apart from `'`, `//`, `/*`**, by § 3's measurement: the `\`
   half has a measured failure on the one compiler Heroes runs, the other
   three rest on C's text. It would let `check --permissive` keep the `\`
   refusal as robustness while dropping the others as thesis rules. The
   coordinator's stage D may already answer part of this (§ 9).

## 7. Questions the sitting should have asked, and did not

1. **Which (1h)**, element by element (§ 2): four approvals of one label are
   four routes.
2. **Is `pkg-config`'s list and version grammar part of what `package`
   means**, or a hole? Both builders raise it; no seat was asked; by the
   list it is a ruling no rule reaches (§ 4). The coordinator's stage D takes
   it as a hole (a package names one package).
3. **Does (1f) for a `link` string need the linker's reader (stage C), a
   refusal of whitespace and control bytes in a library's name (the
   ffi-pragmatist's inference, § 4.2), or both?** Stage D's control-byte
   refusal over all three strings answers the control half; whitespace in a
   `link` name stays on stage C's reader.
4. **What Windows owes before this lands.** Unrun there, every one: clang's
   reading of `\` inside `<...>`; whether `"` can name a file on NTFS; `--`
   before a package; § 3's token path; the linker's missing-library line
   (both readers, the trunk's and stage C's, hold two wordings only, ld64's
   `library '` and `cannot find -l`, read in both; what Windows's linker
   prints for a missing library is unrun here, so whether either reader
   catches it there is a question). `.claude/rules/verification.md` closes a C-boundary defect only
   after the push's platform legs have run its cases; `selfhost/emit/ffi*`
   and `extern*` are C-boundary files.
5. **Whether a6 is owed after the generation run.** The spec-warden's
   condition that would have owed a6 under either standard (2 or more of 10
   bracketed) did not fire: 0 of 10, and 0 of 3 with no header named. Those
   three considered `"<math.h>"` and chose against it by § 13's
   `sqlite3.h` example, so the document as it stands already carried the
   reader to the bare spelling. a6 now rests on panel 181's standard alone,
   which is the synthesis's choice between two precedents, as the
   spec-warden says.

## 8. The repaired briefs, read against my first pass

- **One sentence credits me with a reading I did not give.**
  `docs/panel/188-briefs/compiler-engineer.md` (repaired) says: *"the
  critic's reading is that (1f), the string's value written into the
  `#include`, is owed whatever is refused, and that a refusal at `check`
  then covers what an angled include cannot carry; build that combination
  unless you measure a better one."* My first pass listed *"Emit the value"*
  as a route nobody listed and gave no verdict on any route
  (`completeness-critic-briefs.md` § Missing, *Routes nobody listed*, item
  1). The builder was steered toward one combination under the critic's
  name. Its effect looks small, since all four seats approve (1f) on their
  own grounds (spec § 2, CLAUDE.md § 12) and the builder measured stage A
  alone before combining; but the record should say it was the
  coordinator's steer (`.claude/rules/records.md` § And the record says whose
  idea it was).
- **F7's NUL row** (*"check 0, build 2"*) is incomplete, and the
  ffi-pragmatist's correction (a NUL binds its prefix at exit 0 where the
  prefix names a file) is now re-run (§ 5); the facts file still carries the
  partial row.
- Everything else the repairs say about my first pass matches it: F6's time,
  F8's escapes, F4's precedent, the empty name, the NUL, `\t` and `\r`, `<`,
  the clang-18 recipe, B's message and gutter, the model named on the
  command.

## 9. What arrived after the reports, and what had not by my close

- **`llm-ergonomist-gen.md`** (the spec-warden's P3, 13 sessions): read;
  its file count and spec copies verified (§ 5); its bearing on a6 in § 7
  item 5.
- **The ffi-pragmatist's backstop on header contents**, appended 17:33 to
  17:42: read. Its verdict drops `-Werror=extra-tokens
  -Werror=null-character` (0 real C headers flagged over 58,288 files on two
  machines, an unmeasurable cost on programs' own headers, and no reach into
  a line the compiler writes) and adopts instead a guard in the one writer of
  `#include <...>` that refuses, as the compiler's own exit-2 error, a name
  `check` did not judge. Two notes for the synthesis:
  - its proof that the flags catch nothing in a NAME rests on § 1.2's
    sweep, which puts each byte in the middle of a name (`a<byte>b.h`), and
    so cannot see a position: § 3 above measures a `\` before the closing
    `>` binding another file or breaking the line. Its own route refuses
    `\`, so its verdict stands; but **its § 1.2 sentence "what `#include
    <...>` cannot carry is `>`, LF, CR, NUL and the empty name, measured" is
    complete for one byte in the middle and incomplete for a name's last
    byte**, and the seats' framing of (1a) as "what C cannot carry,
    measured" and (1b) as "what C leaves undefined, on paper" puts the `\`
    on the wrong side;
  - it calls the refusal **"R2"**, a label from the coordinator's request,
    defined in no brief or report I read.
- **The compiler-engineer's stage D**: at 17:44 by `date` its report was
  unchanged since 17:39 (489 lines), its § 13 holding only the crash above
  (stages B and C abort on a header name beginning or ending above ASCII,
  reproduced here in § 1) and the statement that stage D repairs it.
  **Stage D's code, cases and gates had not landed**, so nothing of stage D
  is checked here. The coordinator's note of 17:46 (by its own label; my
  clock read 17:44 when it arrived) replaces stage D' with the pragmatist's
  scoped guard inside stage D. What stage D will owe, by this pass: the
  non-ASCII shapes at both ends of a name and in `link` and `package` (the
  seat lists them); § 3's five trailing-`\` names if any part of the `\`
  refusal is loosened; my first pass's 21 shapes; and the suites of § 1a,
  which build through `--`.

## 10. Summary for the coordinator

**Run and holding** (my copies, this Mac): the patch is the whole change and
applies clean; `parse/` 8,689 to 8,660 (36 of room); the compiler +120;
`layout` 5 and 0, `check` 447 and 0, the compiler's own tests 1,105; the 66
cases (63 by the generator, 3 by hand) with the trunk's eight exits at 2 or
134 and the route's none; **the census of 1,073 programs, 0 moved, now for
the final stage rather than stage B**; and, which the seat did not run,
`run` 261 and 0 (the `package` goldens built through `--`), `corpus` 55 and
0, `unsupported` 131 and 0. The spec-warden's prices, quotations and
citations; the ffi-pragmatist's NUL-prefix and `..` climb; the historian's
five escapes and its clang prediction; the generation run's thirteen
identical files.

**False or broken**: stage C aborts (134) on `extern "é.h"` and any header
name beginning or ending above ASCII, a correct program the trunk builds
(the seat's own correction, re-run here); the seat's *"Corrected 17:52"*
written by 17:39; the compiler-engineer's 848 and 314, which do not
reproduce from the scope it states; my own first-pass count of 15 + 5 call
sites, which is 15 + 4 (the ffi-pragmatist's 19 is right); the repaired
compiler-engineer brief's *"the critic's reading is that (1f) ... is owed"*,
which my first pass never gave.

**Contradictions, and which side is checkable**: four seats approve "(1h)"
with four different refusal sets (§ 2): the control bytes, `\` and `"`,
`'`, `//`, `/*`, the `link` and `package` reach, `--` or a refusal, and the
backstop. On (1b), the sentence is vetoed and nobody proposes it; the
refusal is a split on policy, except for `\`, where **I measured that (1f)
without refusing `\` binds another header silently (two shapes), gives a
false message and an exit 2 on this Mac's clang** (§ 3), the one compiler
Heroes runs.

**Q5 by bounded discovery** (§ 4): the NUL and the empty string in `link`
and `package` share 216's cause if 216's cause is written as the group
head's string's; the `-` option face, the version and list grammar (a ruling
the sitting can give), the linker's reader and `machine_locked`'s reach are
causes of their own, filed apart, the reader landed with or before (1f) for
`link`; and by the same rule F8's escapes are their own item.

**Unrun, and owed before the push**: everything on Windows (`\` inside
`<...>`, `"` on NTFS, `--`, the token path, the linker's wording); the seed
and its fixpoint; the full net; Linux for my § 3 measurement; stage D.

Second pass complete, 2026-10-03, at the time the last line below gives,
with stage D not yet in its report.

Closed 17:45:19 by `date`; the compiler-engineer's report then still read
489 lines, last written 17:39.

## Third pass: stage D

Begun 18:19:36 by `date`, at the coordinator's request, over stage D alone:
`docs/panel/188-reports/compiler-engineer.md` §§ 13 to 17 (669 lines, last
written 18:16) and its patch `<scratchpad>/188-compiler-engineer/work/route-1h-d.patch`,
sha256 `f5e758b0aef153bade21041bced32b284cc1495f8c62f0f4084a4e12651fe0cf`
(`shasum -a 256`: it begins `f5e758b0aef153ba`, as stated). My copy for this
pass is `<scratchpad>/188-critic-d/`, a fresh `git archive 826ddc2f` with the
patch applied; my files are under `<scratchpad>/188-critic/work3/`. No paid
run, no web, nothing run in the seat's folder (its patch, generator and case
programs copied out and read). In progress until this section says
otherwise.

**The clock.** My readings went 18:25:48 (the census) and then 21:05:32 (the
guard run's progress) with no long call of mine between them; at 21:05
`pmset -g assertions` showed another process's `caffeinate` holding the Mac
awake, so whether the machine slept or this session waited I cannot tell.
No duration is written below, so nothing here rests on it; from 21:05 my
runs hold their own `caffeinate -i`.

### T1. The patch, the compiler, the costs: run and holding

| claim | my command | result |
|---|---|---|
| 20 files, applies clean to `826ddc2f` | `patch -p1 --dry-run`, then `patch -p1`, onto a fresh `git archive 826ddc2f` | 20 files (7 of the compiler: `cli/libraries`, `cli/produce`, `diag`, `emit/externs`, `emit/ffi_build`, `head_names`, `parse/group_head`; 13 of `tests/golden/check/`, six `fixedbugs-216-` cases with their `.expected` and one `.fixed`); nothing but *"patching file"* lines: verified |
| the whole change | read-only `diff -rq` of the seat's `selfhost/`, `tests/golden/check/`, `runtime/`, `tests/harness/`, `spec/`, `seed/` against my copy | all six empty: verified |
| the compiler | the trunk's from the seed (`afc05be6b2c50184`), then `./heroes build selfhost/main.hero -o heroes-d` | built; `heroes-d` sha256 `f8376f70ec3ecaf3` (the seat's `e7088725e32b5d47`, built elsewhere; the trees equal) |
| +279 over the trunk; `parse/` 8,660; the leaf 274; `emit/externs` 184; `diag` 147; `cli/produce` **299 of 300**; `cli/libraries` 298; `emit/ffi_build` 229; `parse/group_head` 111 | the mirror `code_lines.py`, per file and over `selfhost/**` | **74,249 against 73,970 (+279), 390 modules; `parse/` 8,660 over 54 files**; every file as stated: verified |
| `layout` whole 5 and 0; `check` whole 450 and 0 | `./heroes-d run tests/harness/main.hero -- ./heroes-d layout`, then `check` | **5 and 0; 450 and 0**: verified |

### T2. The case table and the census, on D itself

**The cases**: the seat's current generator (it writes **87**) and **11** made
by hand (`d2-pkg-pcfile`, `d2-pkg-pcsub`, `q5-package-constraint`,
`q5-package-list`, `q5-package-option0`, `t-bs-2sp`, `t-bs-alone`,
`t-bs-dslash`, `t-bs-slashstar`, `t-bs-tab`, `x-eacute`), copied with their
files; 98 folders, each compiler on its own copy (`work3/cases-trunk.txt`,
`cases-d.txt`):

| | at exit 2 or 134 | internal error |
|---|---|---|
| the trunk | **9**: `f1-gt`, `f6-angle-both`, `f7-nul`, `q5-link-empty`, `q5-link-squote`, `x-angle-own`, `x-tab-gt` at 2; `q5-link-nul`, `q5-package-nul` at 134 | 7 |
| stage D | **0** | 0 |

Exactly the seat's nine and its zero. The rows the trunk builds and runs and
D refuses are the seat's **18**: the control characters 7 (`d1-hdr-c1`,
`-ff`, `-us`, `-vt`, `x-raw-del`, `-soh`, `-tab`), a package's list, version
or `.pc` file 6 (`d1-pkg-vt`, `d2-pkg-comma`, `-pcfile`, `-pcsub`,
`q5-package-constraint`, `-list`), C's undefined set 3 (`f1-squote`,
`-slashslash`, `-slashstar`), and the two wrong programs accepted today
(`f6-angle-gt`, `q5-package-option0`). Verified.

**The census**: the same 1,073 programs as before, `check --brief` by the
trunk's compiler and by `heroes-d`, from my D copy's root, three at a time
(`work3/census-d.txt`): **0 moved**, the trunk 846 at 0 and 227 at 1.
Verified, on D itself.

### T3. The new rules attacked at depth one

37 cases written by `work3/make_attack.py` (each header file named by the
string's value), run on the trunk and on D (`work3/attack-trunk.txt`,
`attack-d.txt`), plus `pkg-config` run directly. What held:

| rule | shapes | D |
|---|---|---|
| D1 `unshowable_name` | U+0080, U+0085, U+009F in a header; U+0085 in a `link` and in a `package`; U+0001 in a `link` | refused in all three strings, the message naming the code point: *"this header's name holds the control character U+0085, which no message can show: write the name without it, as `extern "math.h"`"* |
| | U+00A0, U+00AD, U+2028, U+2029 in a header, the file beside | admitted; check 0, build 0, `7` on both compilers |
| | letters whose UTF-8 holds a byte in 0x80 to 0x9F: `ą` (C4 85), `ő` (C5 91), `…` (E2 80 A6), `€` (E2 82 AC) | admitted, build, `7`: the rule reads code points (`value.chars()`), no continuation byte is read as a C1 control |
| `escape_in_header_name` | a `\` first (`"\\b.h"`), in the middle (`"a\\b.h"`), last (`"ab\\"`); the seat's five trailing shapes | refused at each position, before `machine_locked_path` for the first; kept by `check --permissive` |
| | a `\` in a `link` | admitted; build 1, `ffi_missing_library`, true |
| `option_like_name` | `package "-"` | refused |
| | `link "-x"`, `link "-"` | admitted: `-l-x` names a library called `-x` and can never be an option; build 1, true |
| the non-thesis-first order, under `check --permissive` | `"a'b>c.h"`, `"/a>b.h"`, `"a\tb\\"`, `"a'b\\c.h"`, `package "zlib>=1 x"`, `package "-a>b"` | each exit 1 with its carried-nothing code (`unwritable_name`, `escape_in_header_name`, `option_like_name`); the thesis-only `"a\tb.h"`, `package "seven.pc"` and U+0085 pass at 0, as designed |
| the guard in `cli/produce.hero` | every tracked `.hero` outside `archive/` and `selfhost/` with a group head: **588**; D's `check` refuses 127; the other **461** through `heroes-d build <file> --emit-c`, which enters `produce` (`work3/guard-run.txt`) | **0 firings** of *"a group head's name was never judged"*; 253 at exit 0 and 208 at exit 1 for other reasons, none at 2. The seat's ground for *"never fires"* was its 98 cases and the census, and the census runs `check` alone and never reaches `produce`; this run does |

**What D misses, measured.**

1. **D2b's `.pc` test misses three spellings `pkg-config` reads the same
   way, on both platforms.** `head_names.package_thesis` (line 177) tests a
   lowercase `.pc` and needs `last > 2`. `pkg-config` treats a name ending in
   `.pc` **in any case**, and a bare `.pc`, as a file read from the working
   directory:
   - this Mac, pkg-config 3.0.7 (`work3/pcprobe/`): `seven.pc`, `seven.PC`,
     `seven.Pc`, `.pc` each answer from the folder's file (exit 0) and fail
     from `/tmp`; `plainfile`, `seven.txt`, `seven.pcx`, `x.pc.bak` are not
     read as files (exit 1). This filesystem folds case, so `seven.PC` opened
     the file `seven.pc`;
   - Linux arm64, pkgconf 1.8.1, the files in the container's own
     filesystem: `seven.PC` read the file `seven.PC`, `seven.Pc` the file
     `seven.Pc`, `.pc` the file `.pc`; from `/`, exit 1. (A first try through
     a bind mount from this Mac folded the case and is discarded.)

   Through the compiler (`attack-d/d2b-pc-upper`, `-mixed`, `-bare`):
   `package "seven.PC"`, `"seven.pC"` and `".pc"` **check 0 on D, build 0
   and run from the working directory's file**, which is the shape D2b
   refuses for `"seven.pc"` (same folder: check 1, `machine_locked_path`).
   `package "sub/seven"` with `sub/seven.pc` in the working directory is not
   read from it (build 1, `ffi_package`, true), as the seat's sweep says.
2. **D1's reason reaches characters its set does not.** D1 refuses the C0 and
   C1 controls *"which no message can show"*; Unicode's invisible format
   characters are admitted, and a missing header's message then shows a
   name that is not the one it holds (`attack-d/d1-*-missing`, header
   absent, `build` exit 1, `ffi_missing_header`):
   - U+200B (zero width space) before `stdio.h`: the message reads *"`​stdio.h`
     is not on this machine's include path"*, that is, to a reader, that
     `stdio.h` is missing;
   - U+202E (right-to-left override) in `"x‮h.oidts"`: the name displays
     reversed in the message and in the source, the Trojan Source shape
     (CVE-2021-42574, recalled, not fetched);
   - U+FEFF and U+00AD the same as U+200B.

   Each message is true of its bytes and reads false. The trunk does the
   same; D's new rule is where it would be closed, by its own reason. Its
   class, a false message or an inexact true one, is the coordinator's.
3. **The trunk has one more exit 2 in the class, and D closes it only through
   a thesis rule.** `extern "a'b.h"` with the header **missing**
   (`work3/squote-missing/`): the trunk stops `build` at **2**, clang's
   *'a'b.h' file not found* not matched back to the group (the reader of
   that line is in `selfhost/emit/clang_place.hero`, by grep, not read
   further); `"a'b\\c.h"` does the same. F1 ran `a'b.h` only with its header
   present. D refuses `'` at `check` with `undefined_header_name`, a thesis
   rule: `check --permissive` passes it (exit 0), the guard does not judge
   it (`cannot_carry` is the other half), and my stage C with (1b) switched
   off (`heroes-no1b`, second pass § 3) builds it at **2**. No build meets
   `--permissive` today, since it exists on `check` alone; but **if the
   synthesis drops (1b)'s `'`**, as the ffi-pragmatist's (1h) and the
   spec-warden's position do, this exit 2 returns unless that reader is
   repaired, as stage C repaired the linker's for `link "a'b"`. So the `'`,
   like the `\`, is partly robustness on this Mac's clang.
4. **Smaller, for the coordinator to class**: `"\\b.h"` (a root-relative
   Windows path) is now told as an escape first, and its advice *"write `/`
   between directories"* leads to `/b.h`, which `machine_locked_path` then
   refuses: a mistake told only after the first is fixed (`adjacent` by the
   list). And a `\` in the middle of a name is carried by clang (my second
   pass: `<a  b\c.h>` opened the right file); D refuses it with the
   non-thesis code it gives the trailing `\`, where the seat's own Q2 rule
   (a thesis rule where the tool carries the name) would make the middle
   case a thesis rule. A question, not a defect.

### T4. Unrun, for stage D

- **Windows**, all of it: the cases, the census, `--` before a package, the
  `\` messages, U+2028 and U+2029 in a header, the D2b spellings, the guard.
- **Linux arm64** for my attack cases and the guard run (only the
  `pkg-config` probe ran there); the seat ran its 96 cases there.
- **Not re-run by me on D**: `emission` (the seat's 738 and 0), `run`,
  `corpus`, `unsupported` (its 261, 55, 131), `annotations` and `fixes`
  narrowed; and never run by anyone on D: `surface`, `canonical`, `probe`,
  `determinism`, `wholes`, `descriptors`, `warnings`, the seed and its
  fixpoint, the full net.
- **A real `build`** over the 461 programs (I ran `--emit-c`, which enters
  `produce` and its guard before any tool).

Run since: the compiler's own tests on D, `./heroes-d test
selfhost/main.hero` under `caffeinate -i`: **1,108 tests, all passed**, the
seat's figure.

### T5. Close

**Holding, on this Mac, in a fresh copy of my own**: the patch (20 files,
clean, the whole change); the costs (+279, `parse/` 8,660, `cli/produce`
299 of 300); `layout` 5 and 0, `check` 450 and 0, the compiler's own tests
1,108; the 98 cases (87 generated, 11 by hand), the trunk's nine at exit 2 or
134 and D's none, and the 18 programs D newly refuses; the census, 0 moved
of 1,073 on D itself; the guard silent over 461 programs `check` passes; the
order under `--permissive`; D1 on C1 controls in all three strings and on
multibyte letters; `\` at every position; a leading `-`.

**Missed by D, measured**: (1) D2b passes `package "seven.PC"`, `"seven.pC"`
and `".pc"`, which `pkg-config` reads as working-directory files on both
platforms, the shape D2b refuses for `"seven.pc"`; (2) D1's reason covers
invisible format characters its set does not (U+200B, U+FEFF, U+00AD,
U+202E), whose missing-header messages read as another name; (3) the trunk
exits 2 on a missing header named with `'`, which D closes only through
(1b)'s thesis rule, so dropping `'` from (1b) reopens it unless clang's
*file not found* reader is repaired; (4) two smaller items to class.

**Unrun**: T4.

Third pass closed 21:08:17 by `date`.

