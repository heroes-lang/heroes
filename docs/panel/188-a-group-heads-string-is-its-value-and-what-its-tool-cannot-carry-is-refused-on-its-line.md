# Panel 188: a group head's string is its value, and what its tool cannot carry as one name is refused on its line

2026-10-03, a full panel: `compiler-engineer`, `ffi-pragmatist`,
`spec-warden`, `historian`, `llm-ergonomist` (a blind A/B of two sessions and
a generation arm of thirteen, each a fresh session outside the repository),
and the completeness critic over the briefs first, over the reports after,
and over the adopted route last. Convened by the author's answer *1a*
(`docs/records/log/2026-10-03-1627-the-author-answers-1a-2a-3a-panel-188-convened-the-x86-containers-and-18-lane-worktrees-removed.md`),
the blind seat's paid sessions capped at 3 USD in all; they spent 2.6151. The
trunk was frozen at `826ddc2f` from the briefs to this synthesis. The briefs
are `docs/panel/188-briefs/`, repaired after the critic's first pass before
any seat was launched (each keeps the text the critic read as
`<name>-before-the-critic.md`), with dated corrections beneath what the
sitting later measured false; the reports are `docs/panel/188-reports/`. The
compiler-engineer and the ffi-pragmatist were sent back after their reports,
by messages that reached them at 17:29:35 and 17:29:43 by their transcripts,
to build and to measure what this synthesis could not adopt unbuilt. **The
coordinator's messages to the seats carried times it had not read from
`date`** (*17:32*, *17:35*, *17:45*, *17:46*, *17:50*, each one to five
minutes after the message actually arrived); three reports copied them, and
each has a dated correction beneath. **And the sitting lost 156 minutes to a
permission prompt**: the critic's third pass ran, at 18:27:23, a command that
began by `rm -rf` of its own scratch folder; the prompt reached the author's
phone by Remote Control while the author was away from it (the author's
account, 2026-10-03), and the command's result is stamped 21:03:33 in the
critic's transcript. The machine did not sleep (the kernel's last sleep is
2026-10-01 at 16:33, `sysctl kern.sleeptime`), and the sitting ran without
`caffeinate -i` (`/panel` § 3c) until 21:10. No number in any report rests on
a duration.

## The proposal

Defect 216 (`blocking`): `extern "a>b.h"` passes `check` at exit 0 and stops
`build` at exit 2 with an internal error and clang's text. The facts before
the briefs (`00-facts.md`, F1 to F8) and the critic's first pass widened it to
a class with three causes: what `#include <...>` cannot carry (`>`, a line
end, a NUL, the empty name); the escapes no reader of a header string decodes
(`extern "a\\b.h"` written `#include <a\\b.h>`, two backslashes, so `build`
says a header present is missing); and a malformed name accepted (`extern
"stdio.h>"` builds). Five questions (`00-shared.md`): **Q1**, the class,
routes (1a) to (1i); **Q2**, the message, its code and its fix's certainty;
**Q3**, where it lives against the parser's budget (8,689 of 8,696 lines and
panel 187's registered prediction); **Q4**, whether the spec owes a sentence,
against panel 055's absolute-path sentence removed at `aab44f9b`; **Q5**, the
`link` and `package` strings and every other string handed to C.

## The verdict table

| seat | verdict | cost or delta | prediction | condition |
|---|---|---|---|---|
| compiler-engineer | **approve stage E as the route** (its §§ 13 to 21): (1f); one judgement of a group head's string on its value, every class nothing carries told before any thesis rule; C's undefined set; control and invisible characters; a package names one package; `\` apart; a leading `-` refused; the guard at `cli/produce.hero`; the linker's line asked about each declared name; `--` before a package. Object to (1a) header-only or beside `machine_locked` in `parse/`, to (1c), to (1d) alone, to (1g), to (1i); no veto | **+350 lines**, 73,970 to 74,320, 0.47%; `selfhost/parse/` **8,689 to 8,660** | `parse/` at most 8,689 at the next `m-*` tag; the landing's census moves no program outside the new `fixedbugs-216-` cases | a real name holding a refused byte or a Default_Ignorable code point; Windows reading `\` otherwise or ignoring `--`; a newer Unicode moving the property; a writer of `#include <...>` taking its list from anywhere but `emit_externs` |
| ffi-pragmatist | **veto (1i)**; **approve (1h)**: (1f) + a refusal on the value of `>`, LF **and** CR, NUL, the empty name, `\`, `"` and every control character + true messages at `build`; no objection on its axis to (1b) or (1c); (1g) for `\` and `"` only; object to (1d) alone. Its backstop, **measured and withdrawn**: drop the two `-Werror` flags, guard the compiler's own line | **0 real names refused** of 103,736 header names, 554 `.pc` names, 2,046 library names (this Mac and Linux arm64); the flags: 0 of 58,288 real files | the tree's group-head strings refused by none of it; the SQLite, libcurl and raylib rungs build on three platforms; the Windows box's include roots hold 0 names outside `[A-Za-z0-9._+/-]` and 0 headers its flags would flag; the guard never fires on a program of `examples/` or `tests/` | the veto lifts only for a real header needing a byte an angled include cannot carry **and** a path no working directory or `@` reaches; (1c) preferred if Windows shows an admitted byte naming another file there |
| spec-warden | approve (1a) and (1f) at **0 spec tokens**; approve the sentence a6, paid by the removal r1, provisional; object to (1b), to (1d) alone, to (1g); **veto (1c) and any (1b) sentence** | a6+r1 re-wrapped **-12 legacy, -13 cl100k** (vendored, lower bounds; reproduced by the critic) | P1: a6+r1 reads -20 to -6 real at its landing; P2: the census changes 0 tracked files' exit; P3: at most 1 of 10 fresh sessions bracket the name on the status quo | **accepts no sentence, without a veto, if the synthesis keeps F4's standard and says so**; a6 owed either way if P3 reads 2 or more of 10; a6 must not land with (1i) |
| historian (advisory) | approve (1h) = (1f) + (1a) + (1b), every refusal on the decoded value, (1a)'s line end widened to every control character; object to (1c), to (1d) alone, to (1i); (1g) not needed | | (1f) without (1b) makes `extern "a\\b.h"` name two files, Windows against this Mac; GCC and clang end `#include <a\>b.h>` in different places; with no `extern` example in view a model brackets the name at least once | real `//`, `'` or `/*` names found; Windows clang reading `\` as an ordinary byte; a positive rule never widened; the census finding a line relying on the undecoded spelling |
| llm-ergonomist, A/B (2 sessions, 0.3477 USD) | **A** (today's *internal error*) and **B** (a draft refusal at `check` with a `certain` fix): **two one-turn repairs of two**, each built and printing `1.4142135623730951`; context clean. One session per arm: **no difference between the arms was measured** (the critic) | | A 90, B 97 of 100 (the sessions' own guesses) | A: *"internal error"* reads as a compiler bug and points at a file the author never wrote |
| llm-ergonomist, generation (13 sessions, 2.2674 USD) | **0 of 13 bracketed the name**: 10 given the header's name, 3 given none; all thirteen wrote `extern "math.h" link "m"`, byte-identical, which builds and prints; the three given no name each weighed `"<math.h>"` and chose against it by § 13's example | | | |

## What the sitting measured

- **The class, on the value** (the ffi-pragmatist § 1.2): every byte 0x01 to
  0x7F but `/` written into `#include <a<byte>b.h>` with the file named by the
  same bytes, on Apple clang 21.0.0, Debian clang 22.1.8 and 18.1.8: **exactly
  three fail, LF, CR and `>`**; with the NUL and the empty name, that is what
  an angled include cannot carry **for a byte in the middle of a name**. Every
  other byte, `\` and `"` included, is carried with zero diagnostics even under
  `-Weverything`; UTF-8 names too.
- **A name's last byte** (the critic's second pass § 3, reproduced by the
  compiler-engineer's stage A): a `\` before the closing `>` sends clang down
  its token path. Under (1f) with no refusal of `\`, `extern "a  b\\"` and
  `"a\tb\\"` build and bind **another header**, silently (the program printed
  9, the other file's value, where the named one returns 7); `"d//b\\"` gets a
  false *clang read the header*; `"e/*b\\"` exits 2. On the one C compiler
  Heroes runs: `selfhost/` holds no `CC` override (`grep`).
- **A NUL names its prefix** (the ffi-pragmatist § 1.4, the critic § 5):
  `extern "stdio.h<NUL>x"` binds the system's `stdio.h` and runs, at exit 0,
  on both platforms; no message at `build` can see it.
- **The escape face is the compiler's** (the critic's first pass; `od -c` of
  the emitted units, the compiler-engineer § 1): no reader of a group head's
  string decodes spec § 2's five string escapes, for a header and for a
  `link` alike (`link "a\\b"` says *no library called `a\\b`* while `liba\b.a`
  is there, the ffi-pragmatist § 4.1).
- **The survey** (the ffi-pragmatist § 2): **0 of 103,736** header names under
  every include root of this Mac and the Linux arm64 image hold a byte outside
  `[A-Za-z0-9._+/-]`; 0 of 554 `.pc` names and 0 of 2,046 library names
  outside `[A-Za-z0-9._+-]`; the tree's 648 group heads in `.hero` files none
  (832 with `.md` and `.txt`, the critic).
- **C's text** (the historian, read as page images of the drafts): C11 (N1570)
  6.4.7p1, an `h-char` is *"any member of the source character set except the
  new-line character and `>`"*, with no empty sequence; **6.4.7p3**, *"If the
  characters `'`, `\`, `"`, `//`, or `/*` occur in the sequence between the
  `<` and `>` delimiters, the behavior is undefined"*, footnote 81 giving
  escapes as the reason. Unchanged in N2310 (read for C17, a post-C17 draft by
  its own log), in C23 (N3096) and in the C2y draft N3783 (6.4.8p3).
  6.10.2p5's guaranteed names do not include `/`. C++ made the five
  conditionally supported (CWG 787, 2009).
- **Precedent** (the historian): Go's type checker unquotes an import path,
  then refuses named characters in the value (`validatedImportPath`); gc
  refuses the empty path, a NUL, every control character and a backslash
  (*"use slash"*); Go's `SafeArg` refuses a pkg-config name beginning `-` or
  `@`; Nim's and Cython's own libraries write C's brackets inside the string.
- **(1i), run** (the ffi-pragmatist § 3, the compiler-engineer § 2):
  `-include` carries `>` and not `"`, reads a **decoy from the working
  directory**, and **expands a name beginning `@` as a response file**.
- **Q5** (the ffi-pragmatist § 4, the compiler-engineer § 3; each reproduced
  by the coordinator on the trunk's compiler, `<scratchpad>/repro188/`): a NUL
  in a `link` or `package` string aborts the compiler, 134; `link ""` hands
  the linker a bare `-l` that takes the output as an input, 2; the reader of
  the linker's *missing library* line exits 2 on `link "a'b"` under ld64 and
  `link "a b"` under GNU ld; `package "--atleast-pkgconfig-version=0"` builds
  and runs with no package named, `"--version"` is called a package and
  `"-x"` told falsely as not installed; `"zlib >= 99"` is told *not
  installed* with zlib 1.2.12 installed; `"zlib sqlite3"` builds; a `..` climb
  to the root passes `machine_locked`, while the same path written absolute is
  refused. No other string reaches C undecoded.
- **`pkg-config`'s argument** (the compiler-engineer § 15, one directory per
  byte): pkg-config 3.0.7 and pkgconf 1.8.1 answer byte for byte alike; 119 of
  126 bytes are carried inside one name, and **TAB, LF, VT, FF, CR, space and
  `,` split it**; `<`, `>`, `=`, `!` compare only between spaces (`plain>=1` is
  a name); a name ending `.pc` loads a file relative to the working directory.
- **The backstop on header contents** (the ffi-pragmatist's appended
  section): clang flags `-Wextra-tokens` and `-Wnull-character` in a header
  reached through `-I` or `CPATH`, never through a system directory; over
  50,729 distinct files on this Mac and 7,559 in the Linux image, **0 real C
  headers** a binding would meet would be refused, and the tree's own 194 none;
  every probe drops `-W` words, so the flags would judge one compile, the
  unit's; a diagnostic pragma around the `#include` carries into the header
  (measured), so a flag cannot be narrowed to the compiler's own line; and once
  `check` refuses on the value, **no byte of a name fails only under the
  flags**. A program's own headers, where `#endif FOO` prints a warning today,
  no survey reaches.
- **Stage D, built and gated in the compiler-engineer's copy**
  (`<scratchpad>/188-compiler-engineer/work/route-1h-d.patch`, 20 files,
  sha256 `f5e758b0aef153ba`): on 98 cases on this Mac and 96 in the Linux arm64
  image, the trunk's compiler exits 2 or 134 on **9 per platform** and stage D
  on **none**; the compiler's own tests 1,108; `check` 450; `annotations` and
  `fixes` on the six `fixedbugs-216-` cases 6 and 6; `layout` whole 5;
  `emission` 738; `run` 261 (the five `package` goldens built through `--`);
  `corpus` 55; `unsupported` 131; the census, `check --brief` over 1,073
  programs, trunk against stage D, **0 moved**; each 0 failed. Of programs the
  trunk builds and runs it refuses **18** on this Mac: 2 wrong programs
  accepted today (`stdio.h>`, `package "--atleast-pkgconfig-version=0"`) and
  16 the resolution prices (7 control characters, 6 a package's list, version
  or `.pc` file, 3 of C's undefined set), none in the tree or the surveys.
  **Building it found two faults of the stages before it**, both repaired in
  D: stages B and C aborted at `check` with 134 on `extern "é.h"`, a correct
  program, since the name was sliced before its ends were read as ASCII
  (neither the 66 cases nor the census held such a name; the critic
  reproduced it); and stage C told a thesis rule before what nothing carries,
  so `extern "/a>b.h"` said only the path and `check --permissive` would have
  let the `>` through. Not run on D: `surface`, `canonical`, `probe`,
  `determinism`, `wholes`, `descriptors`, `warnings`, the seed and its
  fixpoint, the full net, Windows.
- **Stage C re-run by the critic from its patch alone**, in its own copy:
  every claim held, the census re-run on stage C itself rather than stage B,
  and `run` 261, `corpus` 55 and `unsupported` 131, which the seat had not run.
- **Stage D re-run by the critic from its patch alone** (its third pass): every
  claim above held in a fresh copy of its own, and the guard of R8, run where
  it lives over the 461 tracked programs with a group head that `check`
  passes (`build --emit-c`), fired **0 times**. Its 37 attack cases at depth
  one found three misses inside D's rules (`.pc` in any case, invisible format
  characters, the leading `\`'s advice) and one exit 2 beside them, below.
- **Stage E, D's misses repaired by each rule's own reason** (the
  compiler-engineer §§ 18 to 21, `route-1h-e.patch`, 21 files, sha256
  `6e75f7ae94e62a02`): a package ending `.pc` in any case, or a bare `.pc`,
  refused as `"seven.pc"` is (`"seven.pcx"` and `"sub/seven"` pass, being
  names to `pkg-config`); the invisible characters refused by Unicode's
  **Default_Ignorable_Code_Point**, the property for what a renderer shows as
  nothing, which holds all twelve Bidi_Control code points and U+200B, U+FEFF
  and U+00AD besides (read from Unicode 15.0.0 as the Linux image's perl
  carries it: 17 ranges, 4,174 code points; this Mac's perl carries 13.0.0);
  U+2028 and U+2029, whose missing header **exited 2 on the trunk** because
  clang's message prints them as `<U+2028>`, found while building E; and a `\`
  in a rooted name told both facts in one message. On 57 attack cases this
  Mac's trunk exits 2 or 134 on 5 and E on **1**; on the 98, the trunk 9 and E
  **0**; on 154 in the Linux arm64 image the trunk 14 and E **1**, every exit
  equal to this Mac's. The one left: a missing header named with a
  private-use, unassigned or noncharacter code point (U+E000, U+0378,
  U+FFFE), which clang also escapes, on the trunk, D and E alike: the reader
  filed apart (R12), not a refusal, since refusing unassigned code points is
  the whole Unicode table and its version. The compiler's own tests 1,109,
  `check` 450, `annotations` and `fixes` 6 and 6, `layout` 5, `run` 261,
  `corpus` 55, `unsupported` 131, the census 0 of 1,073 moved, each 0 failed.
  **+350 lines over the trunk** (0.47%); `selfhost/head_names.hero` at 293 and
  `cli/produce.hero` at 299 of their 300, so the next line in either moves
  code. Not run on E: `emission` (E touches `head_names` and `shown_char`
  alone), the guard over the tracked programs (run on D; E changes only the
  rooted `\` message's text in its predicate), `surface`, `canonical`,
  `probe`, `determinism`, `wholes`, `descriptors`, `warnings`, the seed and
  its fixpoint, the full net, Windows.
- **The blind arms**: above, and `llm-ergonomist-gen.md` for the thirteen.

## Disagreements, unsmoothed

- **Four approvals of "(1h)" were four routes** (the critic's § 2): the seats
  agreed on the value and on the floor (`>`, LF, CR, NUL and the empty name in
  a header) and on nothing else: the control bytes, `\` and `"`, `'`, `//` and
  `/*`, the reach into `link` and `package`, `--` against a refused leading
  `-`, and the backstop. The resolution below chooses element by element.
- **(1b), C's undefined five in a header.** The compiler-engineer approves on
  the historian's text (verified) and, for `\`, a Windows behaviour (unrun);
  the ffi-pragmatist approves `\` and `"` and has no objection either way on
  `'`, `//`, `/*`; the historian approves; **the spec-warden objects**, not on
  budget and without a veto: the refusal turns away `a'b.h`, `d//b.h` and
  `e/*b.h`, which build and run today, for 0 measured writers. **What a
  command settled**: the text (verbatim, unchanged to C2y), the cost (0 of
  103,736 names; a refused name stays bindable through a one-line header of
  the program's own, the ffi-pragmatist § 1.5), and **for `\` alone a measured
  failure**, a header bound silently (above). For `'`, `"`, `//` and `/*` none
  settles it: three clangs read them as themselves, and clang is the only C
  compiler Heroes runs.
- **The control bytes.** The ffi-pragmatist and the historian refuse them
  (clang carries each, no real name holds one, no message can show one; gc's
  rule); the compiler-engineer's stage C admitted them and counted `"\tb.h"`
  building as a benefit. Stage D built the refusal at the coordinator's
  request, as a thesis rule by the builder's own Q2 rule (the tools carry the
  bytes), priced at 7 programs that run today.
- **The spec sentence (Q4).** The spec-warden drafts a6 paid by r1 and
  reconciles F4's precedent with panel 181's by design.md §1.4, a test that,
  adopted, would also restore panel 055's absolute-path clause; the
  compiler-engineer holds that the refusal's message states the rule (F4's
  ground); the historian, that panel 055's removal is the project's precedent
  and a sentence, if any, states exactly the set the checker refuses, which
  the spec-warden's veto on a (1b) sentence makes unwritable. **P3 then
  measured the premise a6 rested on: 0 of 13.**
- **(1c).** The ffi-pragmatist has no objection on its axis; the spec-warden
  vetoes its sentence; the compiler-engineer and the historian object.
- **The backstop.** Proposed by the ffi-pragmatist *under any route*, then
  measured by it and withdrawn in favour of a guard on the compiler's own
  line; the critic notes that its proof that the flags catch nothing in a
  name rests on a sweep of one byte in the middle, which misses the trailing
  `\`, which the route refuses at `check` anyway.
- **`--` or a refused leading `-`** in a package: compared by nobody until
  the critic's § 6; stage D does both.

## The resolution: `provisional, author ratification pending`

The seats agreed on two things, so the resolution is chosen element by
element, every element built in stage E and run (above), and it says where it
parts from a seat. **The codes**, by the compiler-engineer's rule *one code
per reason, a thesis rule where the tool carries the name*: `unwritable_name`
and `escape_in_header_name` and `option_like_name` are not thesis rules and
`check --permissive` keeps them; `undefined_header_name`, `unshowable_name`,
`package_comparison` and `machine_locked_path` are thesis rules, and Part 11's
control arm drops them. **Every class nothing carries is told before any
thesis rule**, so the control arm stays honest.

**R1. A group head's string is its value (Q1's (1f)).** The five escapes
spec § 2 gives a string are decoded where the emitters read a header, a
`link` and a `package` (`emit/externs.unquoted`, through `escape.unescape`,
the decoder the lexer's literals already use) and where the front end judges
them, so `check` judges the bytes `build` writes. A repair and not a language
change: § 13's production makes each of the three a `string`, § 2 gives a
string its escapes, and the compiler had the bug (CLAUDE.md § 12). Approved by
every seat that judged it; it moved no blessed emission and no program of the
census. The formatter and `--dump-ast` keep reading the spelling, as a
re-printer must. **It cannot land alone**: under it a line end in a `link`
goes from exit 1 to 2 and in a `package` leaks the compiler's internal
marker, and a trailing `\` binds another header; R2, R3 and R7 land with it.

**R2. What its tool cannot carry as one name, refused at `check` on the
value: `unwritable_name`, not a thesis rule.** In all three strings, the empty
string, a NUL and a line end (LF or CR); in a header, `>`; in a package,
whitespace and `,`, where `pkg-config` splits its argument. C's whole angled
spelling carried in, `"<stdio.h>"`, is told as that, with `fix (certain):
drop C's brackets`, certain only where the bare name passes the same
judgement and a `guess` otherwise; applied in place it checks, builds and
runs, and a second `--apply` writes the same bytes. The NUL is refused here
because it binds its prefix at exit 0 and aborts the compiler in a `link` or
a `package`; the empty `link` because a bare `-l` takes the output as an
input. Approved by every seat that judged the floor; its reach into `link`
and `package` is the compiler-engineer's, built.

**R3. A `\` in a header, refused apart, as robustness: `escape_in_header_name`,
not a thesis rule.** C11 6.4.7p3 leaves it undefined, and on clang a `\`
before the closing `>` binds another header or none (measured twice, above).
Its message is true on this Mac and makes no claim about Windows until the
Windows box has run one (the builder flagged its own first sentence as
recalled; the historian's evidence for clang on Windows is a 2020 commit
message); a `\` in a rooted name (`"\\b.h"`, `"C:\\x.h"`) is told both facts
in one message with the header's own route, never advised toward a `/` that
`machine_locked_path` refuses next. **One code at every position**, though
clang carries a `\` in the middle of a name on this Mac: the rule does not
split a byte's meaning by where it stands, and the trailing case and Windows
are where it breaks (the critic's question, answered so). No fix is
`certain`: on POSIX `\` and `/` name different files (gc's *use slash* is
prose; clang's own fix-it is off by default and tested only on Windows).

**R4. C's undefined `'`, `"`, `//` and `/*` in a header, refused:
`undefined_header_name`, a thesis rule** (route (1b) less R3), dropped by
`--permissive` as `machine_locked_path` is, `diag.hero:93-95` admitting both
for one reason: without them C gives the program a meaning the thesis does
not. C's quoted spelling carried in, `"\"stdio.h\""`, gets `fix (certain):
drop C's quotes`, certain only with this rule (without it a file named with
quotes is writable on POSIX, and the fix would change the meaning). **Taken
over the spec-warden's objection, on robustness** (CLAUDE.md § Precedence, 3
over 4): the emitted C stays out of C's undefined behaviour, as
`.claude/rules/generated-c.md` keeps it out of overflow's, at a cost of 0 of
103,736 real header names, none of the tree's, none of 13 generated programs,
and 3 programs that run today, each bindable through a one-line header of its
own. **And the `'` is partly robustness too** (the critic's third pass,
reproduced): with its header missing, `extern "a'b.h"` stops the trunk's
`build` at exit 2, the reader of clang's *file not found* line cutting the
name at the quote; R4 makes that unreachable for any program `check` passes,
and the reader is filed to be repaired on its own (R12). **Conservative,
recorded for the author**: drop R4 (about 22 lines and `diag.hero`'s entry);
`a'b.h`, `d//b.h` and `e/*b.h` keep building, `a"b.h` builds on this Mac
under R1, a name NTFS is recalled not to hold (unrun), and **the exit 2 above
returns** unless that reader is repaired first.

**R5. What no message can show, in all three strings: `unshowable_name`, a
thesis rule.** Every other control character, U+0001 to U+001F and U+007F to
U+009F past what R2 already holds, by the compiler's own line between what a
message can show and what it cannot (`selfhost/shown_char.hero:21`); every
code point Unicode marks **Default_Ignorable_Code_Point**, the characters a
renderer shows as nothing (the twelve bidirectional controls among them, the
Trojan Source shape, and U+200B, U+FEFF, U+00AD, whose missing-header message
read as another name: the critic's third pass), from a table read out of
Unicode 15.0.0 and kept in `shown_char.hero`; and U+2028 and U+2029, Unicode's
own line and paragraph ends. The message names the code point (*"the control
character U+0009"*, *"the invisible character U+200B"*); in a package TAB, VT
and FF take R2's code, since `pkg-config` splits there. The control bytes are
the ffi-pragmatist's and the historian's (gc's rule), over the
compiler-engineer's stage C, which admitted them; the rest is the critic's
measurement taken by the rule's own reason. Priced at the programs that run
today with such a name, 7 control characters and 7 invisible or separator
characters measured, none in the tree or the surveys.

**R6. Panel 055's refusal judges the value, and reaches a package's file.**
`machine_locked` moves into the leaf word for word, its golden unchanged, and
reads the decoded string, so `"\tb.h"` and `"\"stdio.h\""` stop being called
paths (two false messages on the trunk) while `"\\b.h"` still is one; and a
`package` whose value ends in `.pc` in any case, or is `.pc` alone, which
`pkg-config` reads as a file from the directory `heroes` runs in (both
versions measured), is `machine_locked_path` too, routed to
`PKG_CONFIG_PATH` (the compiler-engineer's own addition, separable, widened in
stage E on the critic's measurement). A `..` climb to the root is not reached
by either: filed apart (R12).

**R7. What `link` and `package` hand their tools (Q5, (1d) as the
complement).** (a) The reader of the linker's *missing library* line asks
about each `link` name the program wrote, ld64's and GNU ld's two wordings,
anchored, instead of parsing a name out of the line: the trunk's exit 2 on a
quote under ld64 and on whitespace under GNU ld, which R1 widened to a TAB,
gone on both. (b) `--` before a package's name, and a package beginning with
`-` refused at `check`, `option_like_name`, not a thesis rule: `--` depends on
each platform's `pkg-config` (Windows unrun), the refusal on no tool (Go's
`SafeArg`); `@` is a name to pkgconf, measured, so only `-`. (c) **A `package`
names one package**: R2 refuses where `pkg-config` splits, and `<`, `>`, `=`
and `!` are refused as `package_comparison`, a thesis rule, since they compare
only between spaces and the refusal rests on the plausible mistake (its
version syntax), not on the tool. § 13 says *a package*; the list and version
grammar was reachable by accident and told a false *not installed* when a
version was refused. (d) `cli/libraries.hero`'s comment, which said no `.hero`
file could hand `pkg-config` an argument, says what is true.

**R8. The compiler asks its own question again before any tool reads a
name.** `cli/produce.hero`, which every verb that writes C passes through
before `pkg-config`, the probes, the unit and the linker run (`build`, with
`--emit-c` and `--dump-ir`, `run`, `test`), hands every header, `link` and
`package` value to the same non-thesis predicate, and a refusal stops it as
the compiler's own *internal error*, exit 2: a state `check` makes
unreachable, so a firing is a compiler defect and never a program's
(design.md §1.12, *check rather than assume*, on the compiler's own bytes).
**Not in the writer of `#include <...>` itself**: `c_text.header_lines` and
its eight callers have no error path, and a panic there is exit 134, outside
the contract; giving them one is about fifteen call sites in ten files,
priced and not built, and owed the day a writer takes its list from anywhere
but `emit_externs` (the builder's condition). **The ffi-pragmatist's
`-Werror=extra-tokens -Werror=null-character` are not adopted**, on its own
measurement: nothing left to catch in a byte the compiler writes, and a reach
only into header contents, where a program's own `#endif FOO` would become a
refused build.

**R9. Where it lives (Q3)**: a leaf, `selfhost/head_names.hero` (293 lines in
the suite's unit), that reads a `str` and nothing of the parser, with the
lines that report it in `parse/group_head.hero` and the Unicode table in
`selfhost/shown_char.hero`, the module for what a message can show. The budget row's own comment names this seam (*a
move pays for lines only along a seam, code that reads no parse*), panel
055's judgement of the same string moves with it, and `selfhost/parse/` goes
from 8,689 to 8,660, so panel 187's registered prediction holds. What stays in
`parse/` is all that reads the parse.

**R10. No spec sentence (Q4).** F4's standard governs a header's spelling as
it governs its absolute path: the message states the rule each time it fires,
and the specification stays at 9,392 real tokens. In order: **P3 measured the
premise a6 rested on**, 0 of 13 bracketed, and the three sessions given no
header's name each weighed `"<math.h>"` and refused it **by § 13's example**,
so the error design.md §1.4 would spend redundancy on does not occur on
today's document; §1.0's burden binds an addition whatever pays for it
(design.md §1.6, lines 379 to 381), and a6 has no measured thesis effect; the
historian's precedent asks a sentence, if any, to state exactly the refused
set, which the spec-warden's veto on a (1b) sentence forbids, and a6 states a
subset; and the spec-warden accepts no sentence on exactly this condition,
said here. **What it rests on**: § 13's `extern "sqlite3.h" link "sqlite3"`
decided the spelling in 13 of 13 sessions' `reading`; a spec change that
removes that example or brackets its name owes a6, or a sentence of its kind,
in the same commit. **The sitting's other reading, recorded for the author**:
a6+r1 re-wrapped (*"A group names its header without the `<` `>` C puts around
it"*, paid by deleting *"A package answering with anything this compiler does
not pass on is refused, naming what it said."*), -12 legacy and -13 cl100k,
the spelling stated by rule rather than by example; by the spec-warden's
§1.4 test it would also restore panel 055's absolute-path clause (a6abs+r1, -5
and -6). This is the one element where the synthesis does not take what its
seat drafted, and the reason is a measurement (CLAUDE.md § 12).

**R11. The routes refused, and what the vetoes compel.** **(1i) is vetoed**
(the ffi-pragmatist): `-include` reads a decoy in the working directory and
expands a name beginning `@` as a response file, so clang would verify
signatures against a header nobody named; the veto compels that no route hand
a header to clang by a flag. **(1c)** is not adopted: its sentence is vetoed
(the spec-warden), it refuses a space, `<`, `é` and `~`, each carried by C
with defined behaviour, and its set is a premise about the world where R2 to
R5 are measurements of C and of the tools. **(1g)** is not adopted: it judges
the spelling where the defect is the value. **(1d) alone** is not adopted
(`stdio.h>` and `stdio.h<NUL>x` build at exit 0 on every clang, so no `build`
message sees them); its pieces are R7. **`"<name>"` given Nim's and Cython's
meaning** (the critic's § 6) is refused: two spellings of one header, against
design.md §4.15's *exactly one correct way* and Part 6's row against two
spellings of one thing; R2's certain fix is the road to the one spelling.

**R12. Landing, closing and filings.** The landing joins the next batch,
which holds at least about fifteen defects by the author's instruction of
2026-10-03 (`.claude/rules/verification.md` § The batch), as its FFI lane:
`route-1h-e.patch` applied to the trunk, R1 to R9, the six `fixedbugs-216-`
goldens with each diagnostic annotated in its source, the unit tests, and the
cases the builder's generators do not write made by a script or kept as
goldens (the critic's note on the hand-made ones). `head_names.hero` and
`cli/produce.hero` stand at 293 and 299 of their 300, so a line added to
either in the lane moves code first. At the round's gate, beside the full net,
the seed and its fixpoint and the census (the spec-warden's P2): the suites
stage E did not run, `emission`, `surface`, `canonical`, `probe`,
`determinism`, `wholes`, `descriptors` and `warnings`, and the guard over
every tracked program with a group head. Then the platform legs: Linux arm64
under both clangs, and **the Windows box**, which owes clang's reading of `\`
and `"` there, `--` before a package on its `pkg-config`, its linker's
*missing library* wording, F1, and its include roots surveyed; the CI's
x86-64 after the push. **Defect 216 closes only after those legs** (a
C-boundary defect, `.claude/rules/verification.md` § The batch), its cause
written in its body as the string's, not the header's (the critic's § 4), so
the NUL and the empty string in a `link` or a `package` are its shapes.
**Filed as their own items, by § Bounded discovery's rule, and landed in the
same lane**: the undecoded escapes, the package read as an option, the
package's list and version grammar, and the reader of the linker's line, each
`blocking`; and the reader of clang's *file not found* line
(`emit/ffi_build.missing_header`), which cuts a header's name at a quote and
cannot match a code point clang prints as `<U+XXXX>` (a private-use,
unassigned or noncharacter one), so a missing header named with either stops
`build` at exit 2 on the trunk; R4 and R5 make the quote and the separators
unreachable from a program `check` passes, the private-use residue is not,
and the reader is repaired on its own (`blocking`), as stage C repaired the
linker's. **Filed apart, beside the lane**: the `..` climb to the root past
`machine_locked` (`adjacent`, the ffi-pragmatist) and `ffi_package` pointing
at the group's first member rather than the `package` string (`adjacent`,
the spec-warden), which share the lane's files; a source byte that is not
UTF-8, which `check` answers *cannot read* at exit 2 (`blocking`, the
ffi-pragmatist); and the `certain` rename of `sqrt` to `sort` (`blocking`,
the critic's first pass). Each was reproduced by the coordinator on the
trunk's compiler before filing.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | `selfhost/parse/` at most 8,689 lines at the next `m-*` tag | that tag's commit, `code_lines.py selfhost/parse/*.hero` |
| compiler-engineer | the landing's census moves no tracked program outside the new `fixedbugs-216-` cases | the landing round's gate |
| ffi-pragmatist | none of the tree's group-head strings is refused, and the SQLite, libcurl and raylib rungs check and build on three platforms | the landing round's gate and its platform legs |
| ffi-pragmatist | the Windows box's clang resource `include/` and SDK roots hold 0 header names outside `[A-Za-z0-9._+/-]` and 0 headers its flags would flag | the next Windows leg |
| ffi-pragmatist | the guard of R8 never fires on any program in `examples/` or `tests/` | the landing round's full net |
| spec-warden | P1: a6+r1 re-wrapped reads -20 to -6 real | only if a6 lands |
| spec-warden | P2: the census changes the exit of 0 tracked files besides the sitting's cases | the landing round's gate |
| spec-warden | P3: at most 1 of 10 fresh sessions bracket the name on the status quo | **scored 2026-10-03, held: 0 of 10**, and 0 of 3 with no header named |
| historian | (1f) without (1b) makes `extern "a\\b.h"` name two files on Windows and this Mac | the Windows box, a tree holding only `a/b.h` |
| historian | GCC and clang end `#include <a\>b.h>` in different places | **clang's half scored by the critic: it opened `a\>b.h`**; GCC's unrun and, Heroes running clang alone, not a Heroes fact |
| historian | with no `extern` example in view, a model brackets the name at least once | unrun, unfunded |
| llm-ergonomist | one-turn repair rates A 90, B 97 of 100 | a run of 100 per arm, unfunded |

## The critic's passes

**First, over the briefs** (`188-reports/completeness-critic-briefs.md`): the
finding that changed the framing, that the escape face of F1 was the compiler
writing the string's spelling and not clang failing; eight false statements
(route (1a)'s premise that clang fails on `\` and `"`; F1's second face read
as clang's; F6's time; *"`what` is `"link"`"*; the B arm labelled (1b) while
refusing `<`, and its claim that a name cannot hold `<`; B's gutter; the
`clang-18` recipe; one paid session against two); 21 shapes beside F1, among
them the NUL and the empty name; four routes nobody listed ((1f), (1g), the
refusal with (1d), (1i)); and eight questions the sitting did not ask, the
first being whether a header string is its value or its spelling. Each was
repaired before the seats launched. **One repair credited the critic with a
reading it did not give** (*"the critic's reading is that (1f) ... is owed"*,
in the compiler-engineer's brief): it was the coordinator's steer, corrected
beneath that brief.

**Second, over the reports** (`188-reports/completeness-critic.md` §§ 1 to
10): stage C re-run from its patch alone, every claim holding, and the suites
the seat had not run; four approvals of "(1h)" read as four routes; the
trailing `\` measured, which moved `\` from C's text to a measured failure and
made R3; Q5 partitioned by § Bounded discovery, with 216's cause to be written
first and F8's escapes their own item; three routes nobody listed (Nim's
meaning, a refused leading `-`, `\` apart), two of them built into stage D;
the compiler-engineer's guessed time and an unreproducible count, its own
first-pass count corrected, and the coordinator's guessed times.

**Third, over stage D** (the same file, § Third pass, closed 21:08:17), at the
coordinator's request because 159 of D's lines had been run by their author
alone: in a fresh copy of its own, the patch clean and whole, every cost,
`layout`, `check` and the compiler's own tests, the 98 cases (the trunk's nine
at exit 2 or 134, D's none, and the 18 programs D newly refuses), the census
on D itself, the order under `--permissive`, and **the guard run for the
first time where it lives**: 461 tracked programs with a group head that
`check` passes, through `build --emit-c`, 0 firings (the seat's evidence, its
cases and the census, never reached `produce`). It attacked D's new rules at
depth one, 37 cases, and measured three misses inside them: `.pc` in any
case and a bare `.pc` passing D2b; invisible format characters passing D1
while its own reason covers them; and a `\` first advised toward a path
`machine_locked_path` refuses next; repaired in stage E (above). And one
outside them: the trunk's exit 2 on a missing header named with `'`, which
R4 closes only as a thesis rule (filed, R12).

## Author's verdict

**RATIFIED 2026-10-03**, on the author's answer to the recommendation put to
them that evening, meant as: *OK, ratify as you said, and push.* **Recorded
as a reading**, CLAUDE.md § 4's default; not `by delegation`.

**What the yes settles**: R1 to R12 as the resolution above states them,
the two elements where the synthesis parts from a seat included (R4 over the
spec-warden's objection, R10 without a spec sentence). The landing is batch
8's FFI lane, `lane-b8-ffi`, opened at `dcaca1a3`. **What it does not
settle**: the platform legs R12 owes before defect 216 closes, the Windows
box's above all.

## After the ratification: Windows, measured (2026-10-03, appended at 22:47)

The Windows box came on after the author's ratification, and two seats ran
R12's Windows facts there (Git Bash on Windows, clang 23.1.1, target
`x86_64-pc-windows-msvc`, linker lld-link, no `pkg-config`): the
compiler-engineer's § 22 and the ffi-pragmatist's *Windows, measured*.

- **Scored**: the historian's first prediction **held** (`#include <a\b.h>`
  opens `a/b.h` in a tree holding only that; clang warns
  `-Wnonportable-include-path-separator`); the ffi-pragmatist's survey of the
  box's seven default include roots **held** (0 of 5,455 names outside
  `[A-Za-z0-9._+/-]`), and so did its backstop scan (0 candidates). R3 and
  R4 now rest on Windows too: a `\` is a separator there and a trailing one
  is *file not found*, and NTFS refuses `"` and `*` in a name.
- **Stage E on Windows** (155 cases): the trunk's 33 exits at 2 or 134
  become 12, none introduced by E. The 12 are the batch's to close: lld-link's
  *could not open 'X.lib'* is a third wording the linker's reader does not
  read (defect 224, widened), and a `link` string beginning with `-` is an
  lld-link option (`link "-out:pwn188"` builds at exit 0 and writes a file of
  the author's naming).
- **Taken in batch 8's FFI lane, provisional on the author's reading**: the
  `option_like_name` refusal of R7 (b) extended to a `link` string by its own
  reason, the premise that `-l-x` always names a library being the Mac's and
  Linux's and false on Windows; R3's message made true on both platforms;
  `machine_locked_path`'s library route, which names `LIBRARY_PATH` that
  lld-link ignores, filed as defect 233.
- **What the ratified rules still admit on Windows**, measured by the
  ffi-pragmatist: names NTFS stores as another file, so that the include opens
  something else at exit 0 where the Mac and Linux say *file not found* (`:`
  past a name's second byte, a component ending in `.` or a space, a `..`
  through a directory that does not exist, a `~` 8.3 alias, the device name
  `NUL`), filed as defect 234; and names no NTFS file can hold (`*`, `<`, `?`,
  `|`), told truly on Windows, filed as defect 235. The rule that would close
  both refuses 0 of the real names surveyed on the three platforms and none of
  the tree's 700 group-head strings. **Whether to extend R2 and R6 by their
  own reasons to them, or to convene a sitting, is put to the author**
  (`docs/work/DECIDE.md`).
- **Corrected beneath § 22**: `f1-trigraph` and `f7-lt` build 1 on Windows
  because NTFS cannot store `?` or `<` in a name (§ 22.1's own table), so their
  *missing header* is true there, not clang reading them differently.
