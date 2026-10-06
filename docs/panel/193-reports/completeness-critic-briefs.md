# Panel 193, the completeness critic, first pass: the briefs

Written 2026-10-05 from 23:10:45 (`date`). No verdict. Every number below was
produced in this session by the command named beside it. A sentence that is an
inference says so.

**My copy**: `<scratchpad>/193-critic/`, made with `git -C
/Users/joseph/Temp/heroes/heroes-lang archive 00217c39 | tar -x`. The compiler
was built there from the seed with `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` (`real 4.16`). For one measurement (item 1.3) I
also built two compilers from `selfhost/` with that seed compiler:
`heroes-r64`, unchanged, in the copy, and `heroes-r1`, in a second copy
`<scratchpad>/193-critic/r1/` whose `ROUNDS` constant (`check.hero:241-242`)
was set to 1. Nothing in the trunk or in any worktree was read as the frozen
tree. Git objects of the lane `lane-b12-str192` were read with `git show`; its
worktree was not entered. No paid run. One stray file: my first command
redirected a stdout to `/tmp/x` (0 bytes, 22:51). It is left in place, since
I never `rm`.

## 1. Framing facts: false, imprecise, incomplete or unverifiable

### 00-shared.md

1.1 **"Today's output ... `{"schema": 1, "diagnostics": [...]}`"**: holds,
but the brief does not say which stream. `./heroes check --json
<178's case> >out 2>err`: exit 1, **stdout 0 bytes, stderr 1799 bytes**.
`report` writes it with `io.eprint_raw` (`check.hero:150-151`). A consumer,
the ffi-pragmatist's included, that reads stdout gets nothing.

1.2 **"one `expected_end_of_line` at 13:10 carries three fixes"**: true of
that one diagnostic. The case's answer, from the same run, holds **three**
diagnostics: 13:10 with 3 fixes, 19:15 with 3, and 24:10 with 4. That makes
**ten** fix objects, every one `{"title": "delete the `,`", "replacement":
"", "certainty": "certain"}`, byte-identical. `diff <case>.hero <case>.fixed`
shows the ten line-ending commas deleted (lines 13-15, 19-21, 24, 27 and
29-30). The llm-ergonomist's "three identical fixes" understates the case
(see 1.20).

1.3 **"`check --apply` (`selfhost/cli/check.hero:71`, `certain.hero`), from
the span, inside the compiler"**: the span part holds; the rest is
incomplete.
- `:71` is the branch that hands back a file no stage speaks of. The writing
  happens in `report` (`:107-148`), then `settle` (`:194-236`), then
  `certain.applied`.
- `settle` re-reads the program from disk and asks the same stage again,
  **up to `ROUNDS` = 64** (`:241-242`).
- `certain.applied` writes only the root's fixes. A fix past the root is
  `elsewhere`. It drops a **twin** (same span and same text) and **withholds**
  a fix that touches one already written (`certain.hero:41-45`, `:104-214`).
- The header of `certain.hero` (`:7`) says every fix's span is "the ORIGINAL
  text's".

So `--apply` is not one application of one answer's fixes. **Measured**:
`out/rounds.py` ran `check --apply` with `heroes-r64` and with `heroes-r1`
over 541 roots (all 510 `tests/golden/check/*.hero`, plus the 31 surface
fixtures that carry a `.fixed` or `.applied`).
- `heroes-r64` equals the answer file on **170 of 170**.
- One round differs from 64 on **3** roots, all three
  `tests/golden/surface-fixtures/certain137-{inside,last,types}/main.hero`.
- One round differs from 64 on **0 of the 139** `check/` cases that have an
  answer.

1.4 **"its fix row is at `:37` to `:40`"**: imprecise. The row's string is
`check_json.hero:40`, its `push` spans `:39-41`, and the loop spans `:34-41`.
Defect 271's own line carries the same range. **"lines 45-50"** of
`diag.hero`: `record Fix` is `:46-50`, and `:45` is blank.

1.5 **"`tests/harness/suite_surface.hero:226` to `:235` (rows over `lex
--json` and one over `check`'s JSON escaping)"**: inside that range, `:226`
is `doctor --json`, `:227-230` hold no JSON, `:231-234` are `lex` and `:235`
is `check`. The same file reads `check`'s JSON at **three more rows**
(`grep -n -- '--json' tests/harness/suite_surface.hero`): `:286`, `:372` and
`:425`. See 2.1.

1.6 **"Readers ... (`git grep -l` for `--json` outside ...)"**: the grep
returns **16 files** (`git grep -l -e '--json' 00217c39 -- . ':!selfhost/cli/'
':!docs/panel/' ':!issues/' ':!docs/records/'`). I opened the 12 the brief
does not list, and none reads `check`'s JSON: `archive/`, `seed/heroes.c`,
`selfhost/token.hero`, `provision.ps1` (Tailscale's `status --json`), a
case's comment, the json102 README and the rule file. The brief's four are
right, as far as a literal grep reaches. A search by the JSON's own keys finds
more readers (section 2).

1.7 **"The specification and design.md say nothing of `--json`"**: true for
the strings searched. `grep -c -- --json spec/heroes-spec.md` gives 0,
`grep -n -- --json docs/design.md` gives no line (exit 1), and `json` in any
case appears only as a library domain (spec `:342`, `:417`; design.md `:476`,
`:536`, `:3936`). **What the negative leaves out**: `check_json.hero:1`
names **design.md §4.17** as the schema's home. §4.17 (`design.md:2142-2154`)
promises one turn per message and names **Part 11's metric 4**,
turns-to-green (`:4044-4045`), as the counter "once it runs". The design
speaks of the JSON's purpose by citation, if not by name.

1.8 **Spec numbers**: verified. `./heroes measure spec/heroes-spec.md` gives
`real 9392 claude-opus-5, 2026-10-03` and `maximum 7117`. The brief omits
**headroom 848 against the 10240 ceiling** (60 of it mortgaged to the FFI
floor, so the check goes red at 10240 over a measured 9452). The spec-warden
needs that figure if a sentence is owed.

1.9 **"113 sites"**: verified. `git grep -c 'diag.Fix(' 00217c39 --
selfhost/` gives 112 lines. The one bare `Fix(title` not preceded by `diag.`
is `diag.hero:180`, a test, so the total is 113. Of these, **108 are in
production code and 5 in `## Tests` sections**, over 57 files (an awk split
on `## Tests`). At least seven helpers return a `diag.Fix`
(`escape_report.hero:118`, `join_fix.hero:21`, `parse/at_prefix.hero:252`,
`parse/line_end.hero:287`, `parse/list_line.hero:152`,
`rename_fit.hero:196`, `statement_front.hero:33`), so the count is a floor,
as stated.

1.10 **"No editor extension reads it (the grep)"**: true. But an extension
exists. `editors/vscode/` holds a manifest, a TextMate grammar and icons, and
no code (`cat editors/vscode/package.json`). It is the likeliest future
reader, and its host counts positions in UTF-16 (1.16).

1.11 **"Its rendered half ... is being repaired by batch 12's lane str192"**:
the lane exists (`git worktree list`: `lane-b12-str192 fe1b8f2b`). It
**committed after the briefs were written** (the brief files are 22:47-22:48
by `ls -la`): `1697cec1`, 2026-10-05 22:54:04, "Defect 271, its message half".
`git show 1697cec1` shows the form it chose. A fix whose start is on another
line than its message's now ends **` (at <file>:<line>:<col>)`**. That place
is **the start only**, comes from `source.locate`, and so counts **per-file
lines and character columns**; a fix on the message's own line says nothing
more. It touches only `diag_render.hero`, so the JSON's `"title"` is
unchanged. The briefs do not tell the seats that the other half has already
picked a form, a unit and a coordinate system.

1.12 **"Convened 2026-10-05 by the author's yes of that evening"**: not
verifiable from the frozen tree (`grep -rln 'panel 193' issues/ docs/` in my
copy is empty). The yes postdates `00217c39`, so citing it is the
coordinator's.

### compiler-engineer.md

1.13 **"the column's unit ... bytes or characters?"**: **measured
characters**, meaning what `s.chars()` yields, which counts as code points
here (spec § 10 defines nothing finer). The test file `out/col.hero` has
line 2 `    s = "è€😀", x = 1`, with characters of 2, 3 and 4 bytes before
the comma. `check --json` gives **`"col": 14`**; bytes would give 20 and
UTF-16 units 15. The code is `source.hero:265-269`, `chars().len() + 1`.
`lex --dump-tokens --json` on the same file puts the comma at col 14 too, and
a place there is **`line`, `col` and `text`, with no offset and no end**: an
in-tree precedent no brief cites.

1.14 **"every case under `tests/golden/check/` that has a `.fixed` or an
`.applied` file"**: that is **139 of 510** (61 `.fixed`, 78 `.applied`, none
with both; `ls | sed 's/.*\.//' | sort | uniq -c`). This proof set leaves
out the **31 answer files under `tests/golden/surface-fixtures/`**, which
hold the only multi-round cases (1.3) and the only certain fix outside the
root (1.15). On the 139, one round suffices (1.3), so a consumer of one
answer can in principle match `--apply` byte for byte there. On the
`certain137-*` fixtures it cannot, by construction.

1.15 **"a fix whose span lies in another file than its diagnostic's, if
any"**: the tree already holds the nearest shape,
`tests/golden/surface-fixtures/applyx/`. There the diagnostic and its certain
fix both lie in `geom.hero`, a used module, not the root.
- `check --json` gives `"file": ".../applyx/geom.hero"`, `"line": 3`,
  `"col": 12`.
- `check --apply` writes nothing there and says `note: 1 certain fix(es) are
  in another module and were not applied: .../geom.hero:3`.
- `suite_surface.hero:427` and `suite_fixes.hero:317` pin that behaviour.

**A `Span` carries no file.** Offsets index one text holding every file of
the compilation: root first, each later file after one joining newline, then
the library (`source.hero:14-24`, `:113-132`; panel 031 R7). So for any file
but the root, **a raw `span.start` is not that file's own offset**. The route
owes a subtraction of `files[k].start`, the translation `locate` already makes
for `"line"`. This is an inference from reading: the frozen compiler prints
no span.

1.16 **CRLF "(does the compiler read one?)"**: **yes, measured** on 178's
case converted to `\r\n`. It gives the same three diagnostics at the same
13:10, 19:15 and 24:10. `--apply` writes CRLF back, and the output equals
`.fixed` converted to CRLF (`cmp`; 34 `\r` in 34 lines). Byte offsets would
therefore differ by one per line between the two forms of one program, while
line and col agree (inference for the offsets, since no span is printed).

1.17 **"instructions retired (`/usr/bin/time -l`)"**: verified on this Mac.
`/usr/bin/time -l ./heroes build selfhost/main.hero` printed `821077910687
instructions retired`.

1.18 **A shape not in step 3**: a root holding a byte that is not UTF-8. The
compiler's text puts U+FFFD where the byte was (`source.hero:76-78`), which
would shift every later offset. **Measured closed by construction**: a file
with `0xE9` in a comment and two trailing commas below gets **one `not_text`
and no fix**, and `--apply` exits 1 with nothing on stdout. So no certain fix
reaches a root that is not UTF-8.

### ffi-pragmatist.md

1.19 **Windows**: verified at 22:59:51 with `ssh -o BatchMode=yes win
'clang --version; df -k /c; uname -s'`. It gave `clang version 23.1.1`,
`MINGW64_NT-10.0-26100` and **30,891,356 KB free** on `/c`. **LSP's default
unit**: verified on the 3.17 specification page (read 2026-10-05). If the
server omits it, the encoding "defaults to the string value `utf-16`"; "the
only mandatory encoding is UTF-16"; it is negotiated through
`general.positionEncodings` and `capabilities.positionEncoding`;
`TextEdit` is `{range, newText}`; "the end position is exclusive".

### llm-ergonomist.md

1.20 **"arm A passes at most 2 of 5 on 178's case, where three identical
fixes ... name no place"**: the premise is weaker than it reads.
- There are ten identical fixes, not three (1.2).
- The message itself says no `,` ends a statement's line, and the `.fixed`
  is exactly every line-ending comma deleted (1.2).
- **The case's own header, lines 1-10, explains the repair**: "a `,` that
  ends a statement's line or an arm's opens a run, told once with its deletion
  `certain`, and every later one in the same declaration ... is one more
  certain deletion". An arm-A session that reads the file is told where every
  fix applies.
- This is a convention, not an accident. 363 of the 510 `check/` cases are
  `fixedbugs-` cases (111 of them with an answer file), and 507 of 510 open
  with a comment (`head -c1`). Any case the compiler-engineer picks will
  likely carry such a header.

The expectation that arm A does well on 178 is an inference (unrun, no paid
run). It would make a tie say "this case", not "the place does not help".

1.21 **Allocation**: "two arms of five sessions", "one case's `.hero` file"
per session, and three cases ("178's among them") cannot all hold. The
prediction counts five arm-A sessions on 178's case alone. Either the other
two cases get no session, or "2 of 5 on 178's case" cannot be scored.

1.22 **"at most 2.5 USD by the CLI's report"**: this is arithmetic (10 x
0.25) resting on `--max-budget-usd` being a ceiling no session passes. `claude
--help` (CLI 2.1.285) says only "Maximum dollar amount to spend on API calls
(only works with --print)". The cap has not been tested at its level here.
Panel 189's eight 0.25-capped sessions cost 0.1729 to 0.1794 USD (its
reports' tables), none near it. One-shot sessions in the scratchpad
`run.json` files cost up to 0.6054 (`185-llm-ergonomist`). **No model is
named.** Panel 188's command carried `--model claude-opus-5-5`, and
`SKILL.md:185-186` warns that the CLI must support the sitting's model or the
seat runs on another. The command's flags are verified (`SKILL.md:161-165`;
all present in `claude --help`).

1.23 **"A session passes where its file equals ... what `check --apply`
writes"**: sound for one-round cases. If a chosen case needs a second round
(the `certain137-*` shape), no one-turn session can pass from one answer.

### historian.md (run locally where a tool was installed)

1.24 **rustc** 1.90.0, `--error-format=json` on `out/fixit.rs`: a
`MachineApplicable` "remove this `mut`" carries **`byte_start` 37,
`byte_end` 41** (0-based, half-open) and **`column_start` 32**, a character
column (bytes 38, UTF-16 33), with `suggested_replacement` `""`.

**clang** (Apple clang 21.0.0): `-fdiagnostics-parseable-fixits` writes
`fix-it:"fixit2.c":{1:53-1:53}:";"`, a **byte** column (characters 47,
UTF-16 48). `-fdiagnostics-format=sarif` exists. It declares `"columnKind":
"unicodeCodePoints"`, places the error at `startColumn` 47, and carried **no
`fixes`** for that same diagnostic. It also prints `1 error generated.` after
the JSON on the same stream. Upstream LLVM is unverified.

**gcc** is not installed here, so unverified. SARIF's `replacement` and
`deletedRegion` were not read by me.

## 2. Readers of the JSON the briefs missed

2.1 **`suite_surface.hero:286`** pins the first bytes `{\n  "schema": 1,\n`,
plus `"certainty": "certain"` and `"line": 10`. **`:372`** pins an empty
answer at exit 0. **`:425`** pins `"line": 5,` for a diagnostic in
`cross/geom.hero`, the per-file line that a fix's place must also follow.

2.2 **`selfhost/cli/check.hero:370-379`** is a compiler test asserting
`{\n  "schema": 1,\n  "diagnostics": [\n`. The brief's grep excluded
`selfhost/cli/` as the writer's folder.

2.3 **Panel 187's R2 instrument**, which found defect 271 (its item). It reads
`check --json` and `check --permissive --json` from **stderr** at every
recovery round's gate (`verification.md` § Bounded discovery, *A recovery item
closes*). It lives outside the tree (defect 210, open), so no grep of the tree
can find it.
- Its source, `<scratchpad>/instrument/tool/recovery.py`, **is gone**:
  `inst-187/round11.log`, 2026-10-05 15:56:17, says `can't open file
  .../instrument/tool/recovery.py ... No such file or directory`, exit 2, and
  `tool/` holds an empty `__pycache__`.
- It was rebuilt as `<scratchpad>/inst-r2/replay.py` (220 lines, docstring
  "rebuilt 2026-10-05").
- It reads `file`, `line`, `col`, `code` and `message` with `.get`, and never
  `schema` or `fixes` (`replay.py:35-45`). So it survives an added field and a
  moved schema, by reading.

2.4 **The site's errors page, both editions**: `site/src/html/docs/errors.html:39-40`
and `site/src/html/it/docs/errors.html:39-41` say the messages "also come out
as JSON ... for when the reader is not a person but another program". This
published promise is prose, so a literal grep cannot see it, and the Italian
edition is a deliverable translation (CLAUDE.md § 11).

2.5 **`heroes --help`**: `selfhost/cli/table.hero:92` prints "print the
diagnostics as JSON (schema 1) instead of text".

2.6 **The control arm**: `check --permissive --json` goes through the same
`report`, so an added field lands in Part 11's control arm
(`design.md:4019-4024`) as well.

2.7 **`check --apply --json`**: accepted at exit 0, with the applied program
(1218 bytes, equal to `.fixed`) on stdout and stderr empty. **`--json` is
ignored in silence**, and so are `--brief` and `--dump-scopes` under
`--apply`. `build` refuses two stops ("ask for one artifact at a time",
`verbs.hero:29`). Panel 016 named this shape the worst silent failure:
"`check --json` accepted and ignored, prose fed to a JSON reader"
(`016-command-surface.md:94-96`). I find no issue filing it: the six lines of
`issues/` that name `--apply` and json are lists of flags.

**Where "schema 1" is written, all of it**: bytes in `check_json.hero:16`,
`check.hero:376` and `suite_surface.hero:286`; prose in `llms.txt:60`,
`table.hero:92` and `check_json.hero:56`. `llms.txt` has three placeholders,
all at `:10-11` (`grep -nF '{{'`), so "schema 1" at `:60` is literal. Nothing
in `site/src/lib/` or `tests/harness/` reads it (`grep -rn llms`,
`grep -rnF 'schema 1'`). **A moved schema turns two of these red and leaves
two stale in silence** (`llms.txt:60`, `table.hero:92`). That is the story
`site/src/pages/llms.txt.ts:4-12` records about this very page.

## 3. Routes nobody listed

3.1 **The fix says what it replaces**, as `lex --json`'s `text` does. A
consumer could then refuse a file that changed after the check, instead of
deleting at a stale offset. Robustness outranks every other criterion here.
The proposal lists offsets, line and column, an end, a unit, the schema and
the file, but not the replaced text.

3.2 **One mistake, one fix with several edits.** §4.17 (`design.md:2145`)
counts "a habit carried down a run of lines" as one mistake, yet 178's run is
ten fix objects. rustc's multipart suggestion, SARIF's
`fix.artifactChanges[].replacements[]` and LSP's `WorkspaceEdit` hold one
repair as many edits. This is a change of shape, not an added field.

3.3 **The answer declares its unit**, as clang's SARIF run does
(`"columnKind": "unicodeCodePoints"`, measured in 1.24) and as LSP 3.17
negotiates (`positionEncoding`), instead of a unit fixed only by
documentation.

3.4 **`check --apply --json`**: the edits `--apply` writes, composed over its
rounds against the original text, each with its place. It is the one answer
that agrees with `--apply` by construction (rounds, twins, withheld fixes,
other modules), where a per-fix place cannot on `certain137-*`. It also gives
the flag that is ignored today (2.7) a meaning, or a refusal.

3.5 **Nothing**, the stopping rule's third shape (`cli-surface.md:21-22`):
`check --apply` plus the rendered ` (at file:line:col)` of 1.11 may already
compose to what a reader needs. Its weakness: `--apply` cannot apply one
chosen fix. It is listed so the sitting refutes it on a measurement, not by
omission. That stopping rule, `cli-surface.md:16-18` ("enters the surface
only if the fixpoint invocation, the golden harness or the design.md Part 11
harness must type it, or it has a measured Part 11 effect"), is panel 016's
too, and **no brief cites it**. The Part 11 harness is "frozen until it can
run" (`harness/README.md:1`), and metric 4 has not run.

## 4. The question the sitting should ask and does not

**What does an answer promise about how its fixes combine?** The JSON says
nothing. The compiler's own applier does five things a place alone does not
tell a consumer:
- it applies every certain fix of every diagnostic together (178's ten),
  while two guesses may be alternatives (`lone_brace`'s two,
  `design.md:2167-2168`);
- every place is in the original text;
- a twin is written once;
- a fix touching one already written is withheld and remade the next round,
  up to 64;
- a fix outside the root is not written.

`certain.hero:15-18` records what applying without these rules once did: exit
134, and a file written as `print(total(xs.must())rint(0)`. So the sitting
should rule:
- (i) whether an answer's certain fixes are applied together, with every place
  in the original text;
- (ii) what a consumer does with two places that touch;
- (iii) whether an answer whose application needs a second round says so;
- (iv) whether a consumer's result must equal `--apply`'s, with the proof set
  widened to the 31 surface-fixture answers (`certain137-*`, `applyx`).

**And panel 016's, beside it.** Since `1697cec1` the text says where a fix
applies (its start, as `file:line:col`, in characters, only off the
message's line). Does "`--json` says how, never what" (resolution item 2:
"the format modifier of whatever the command already prints") require the
JSON to say at least that, in the same unit and per-file coordinates? And
does it allow the JSON to say more: an end, a byte offset, a place on the
message's own line?
