# Panel 193, the compiler-engineer's report

Written 2026-10-05 from 23:15, stopped by the account's limits twice,
resumed 2026-10-06 at 01:30 and at 02:01, finished at 02:10 (`date` each
time). Worked in `<scratchpad>/193-compiler-engineer/`, a copy of `00217c39`
made by `git -C /Users/joseph/Temp/heroes/heroes-lang archive 00217c39 | tar
-x`, its compiler built there from the seed (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, `real 4.25`). From that seed compiler I built
`heroes-base` from the untouched `selfhost/` (`real 103.50`), then one
compiler per route, so before and after differ by the route alone:

| compiler | what it carries |
|---|---|
| `heroes-route` | route A, each fix's place (`real 63.95`) |
| `heroes-ab` | A, and route B: `--apply --json` answers composed edits (`real 63.68`) |
| `heroes-final` | A, B, and A+: each certain fix says what `--apply`'s first round does with it |
| `heroes-eslint` | **the route I would adopt**: A, A+, and `--apply --json` answering `--apply`'s own program, ESLint's shape (built from `out/eslint/selfhost/`) |
| `heroes-r1` | my tree with `ROUNDS` set to 1, the first round's oracle, the constant restored after |

Nothing in the trunk or any worktree was built or run; lane str192's commit
`1697cec1` was read with `git show`. No paid run. Every number below names
the command that produced it; the scripts (`out/consumer.py`,
`out/check_b.py`, `out/check_aplus.py`, `out/code_lines.py`, a replica of
`suite_layout.hero`'s `code_lines`) are measurements, not tools of the tree.

## Verdict

- **verdict**: **object**. Not a veto: no route on the table is a core
  construct and every one fits the ceiling (section 5).
- **section**: design.md §1.7 (`:410-420`) and Part 5 (`:2708-2710`) for the
  classification: every line of every route lands in `selfhost/cli/`, none in
  the type checker, the lowering or the backend, so none of it is core; §1.1
  (`:178-180`) for the ceiling, which all fit. The objection stands on §4.17
  (`:2125-2131`), which makes a `certain` fix applicable mechanically **one
  fix at a time**. design.md does not cover how one answer's fixes combine,
  nor `--json` at all (`grep -c -- --json docs/design.md`: 0); I say so
  rather than invent a rationale.
- **implementation_cost**, in `suite_layout.hero`'s unit (non-blank lines
  outside `test` blocks, comments counted), with file citations:
  - route A, the place: `selfhost/cli/check_json.hero` 50 to 68, **+18**;
  - A+, the first round's outcome: `selfhost/cli/certain.hero` 193 to 235,
    **+42** (`order_of` shared with `in_order`, `outcomes`, `word_of`), and
    `check_json.hero` 68 to 75, **+7**;
  - `--apply --json` in ESLint's shape: a new `selfhost/cli/apply_edits.hero`
    of **30** and `selfhost/cli/check.hero` 274 to 282, **+8**;
  - **my route in total: +105**, `selfhost/` 78,955 to 79,060 lines over 416
    files (+0.13%); zero lines under `lexer`, `parse/`, `resolve/`,
    `check/`, `ir/`, `emit/` or `runtime/`;
  - measured alternatives: route B, composed edits, **+177** (a 158-line
    module, `certain.hero` +6, `check.hero` +13 to **287 of 300**); a refusal
    of `--apply --json` at exit 2, the 3 lines `cli/verbs.hero:28-30` already
    spends on `build`'s two stops (read, not built).
- **needed_for_self_hosting**: no (Principle 0). The compiler never reads its
  own `--json`.
- **argument** (114 words, `wc -w`): No route on the table is core: every
  line I built lands in `selfhost/cli/`, none in the checker, lowering or
  backend, and all fit (§1.7). The objection is to the purpose clause. Places
  are right on every shape I ran, yet one answer's certain fixes written at
  their places give defect 137's `print(total(xs.must())ad_operand` on the
  three `certain137-*` roots, and write a module `--apply` never writes. A
  place makes a fix locatable; only the applier makes an answer applicable.
  So each certain fix also says what `--apply`'s first round does with it,
  computed by `certain.hero` itself (49 lines, 757 of 757 by construction),
  and `--apply --json` answers `--apply`'s own program (38 lines, 757 of
  757).
- **prediction**: at the batch gate that lands this sitting's resolution, if
  it is my route, the landing adds **at most 130 lines** in `layout`'s unit
  (105 measured here), **every one under `selfhost/cli/`** and none under
  `parse/`, `resolve/`, `check/`, `ir/`, `emit/` or `runtime/`, with `layout`
  green and `DECIDED` unchanged. A line outside `cli/`, a new `DECIDED` row
  or more than 130 lines falsifies it. If route B is chosen instead, the same
  count reads at least 170 and `cli/check.hero` stands within 20 lines of its
  300.
- **condition**: I would **approve the proposal as framed**, places alone, at
  18 lines plus a 3-line refusal, if the sitting writes in §4.17 (the
  schema's home, `check_json.hero:1`) that one answer's places promise one
  fix at a time and that `check --apply` is the only writer of several, and
  refuses `--apply --json` at exit 2: then nothing false is promised. Or if a
  corpus shows no answer whose certain fixes touch: today 3 of 171 roots hold
  6 touching pairs. I would turn to a **veto** only for a resolution that
  enters the core, and none on the table does. The nearest is one fix of
  several edits made by changing `record Fix`: 113 construction sites in 57
  files across `parse/`, `resolve/`, `check/` and `emit/`. I would object
  to that on cost, not veto it.

## 1. What the frozen compiler answers today

- **The stream**: `heroes-base check --json <178's case> > out 2> err`:
  exit 1, stdout 0 bytes, stderr 1,799. Three `expected_end_of_line`
  diagnostics, at 13:10 with 3 fixes, 19:15 with 3 and 24:10 with 4: ten
  byte-identical `{"title": "delete the ,", "replacement": "", "certainty":
  "certain"}` (a Python count of the parsed answer). The seed compiler's
  answer is the same bytes (`cmp`).
- **A clean file gives no document**: `report` prints nothing when every stage
  is quiet (`selfhost/cli/check.hero:100-103`), stderr empty at exit 0. Three
  seats found it; no route here moves it.
- **The column's unit**: on `out/col.hero`, whose line 2 holds `è€😀` (2, 3
  and 4 bytes) before a `,`, `"col": 14`, where bytes give 20 and UTF-16
  units 15 (Python over the same line). **Characters**, as
  `source.hero:265-269` computes (`chars().len() + 1`).
- **Where a place comes from**: `record Fix` (`selfhost/diag.hero:46-50`)
  holds `span: token.Span`, two byte offsets (`token.hero:19-21`) into one
  text of every file (`source.hero:14-24`, `:132-171`). The root is
  `files[0]` at 0, so a root fix's offsets are its own; any other file's are
  `span.start - files[k].start`.
- **Defect 371**: `heroes-base check <178's case> --apply --json`: exit 0,
  stdout 1,218 bytes equal to the case's `.fixed` (`cmp`), stderr 0.

## 2. The routes I built

**Route A, the place** (`cli/check_json.hero`). Each fix gains, after its
three fields: `file`, `line`, `col`, `end_line`, `end_col`, `byte_start`,
`byte_end`, `text`.
- `file`, `line`, `col`: `source.locate` of the span's start, the function,
  per-file line and unit (characters) of the diagnostic's own `file`, `line`,
  `col` and of lane str192's rendered ` (at file:line:col)`. Written
  **always**: JSON has no excerpt, and 80% of certain fixes stand on their
  message's line (the ffi-pragmatist's count).
- `end_line`, `end_col`: `locate` of the end, half-open; an insertion's end
  equals its start.
- `byte_start`, `byte_end`: the span less `source.file_at(s, start).start`,
  **that file's own bytes as stored**, half-open, never the joined text's.
- `text`: the bytes replaced, so a consumer can refuse a changed file.
- A span that is not one, or whose ends lie in two files, gets **no place**:
  the writer must not abort on a span `certain.applied` itself refuses to
  write (`cli/certain.hero:58-60`). A compiler test pins a second file's
  place past two `é` (`"col": 9`, `"byte_start": 10`) and a malformed span.

**A+, the first round's outcome** (`cli/certain.hero`, `cli/check_json.hero`).
Every certain fix gains `"first_round"`: `written`, `withheld`, `twin`,
`elsewhere` or `malformed`; a guess gains nothing. `certain.outcomes` runs
the applier's own `order_of` (which `in_order` now uses too, one ordering in
one place) and `chosen`, over the same kept diagnostics `--apply`'s first
round gets, so the outcome is **reported, never restated**. A compiler test
pins an enclosing fix `withheld`, the fix inside it `written`, that fix's twin
`twin`, a guess with none, and `over` writing exactly the `written` one.

**`--apply --json` in ESLint's shape** (`cli/apply_edits.hero`, 30 lines;
`cli/check.hero` +8). `{"schema", "file", "rounds", "not_applied", "output"}`
on stdout, `output` being the program `--apply` hands back, through all three
of its exits (a clean file, stdout, `--in-place`). The artifact stays the
program; `--json` says how to print it, which is cli-surface.md's own
sentence. A root that is not UTF-8 answers its diagnostics on stderr at exit
1, as `--apply` refuses it today.

**Route B, composed edits, built and not adopted** (`heroes-ab`): each
round's written fixes carried back through a list of pieces to edits against
the file as read, each with A's place and `text`. Two rounds compose to one
edit on `certain137-last`, `total(xs.must()).must()` to `total(xs)`.

## 3. The proof against `check --apply`

`out/consumer.py` reads only `check --json`'s stderr and applies every
`certain` fix by three policies, compared byte for byte with `check --apply`:
**literal** (every fix by its byte offsets, one pass, into whatever file it
names), **onepass** (`certain.hero`'s rules re-derived from the JSON: root
only, ordered, runs settled shortest first, twins dropped, touching fixes
withheld), **rounds** (onepass, then `check --json` asked again, up to 64).

**The brief's 171 roots** (the 139 `check/` cases with a `.fixed` or
`.applied`, the 31 fixture roots with one, `applyx/main.hero`; 178's case
among them; 671 certain fixes; `--apply` changes 170):

| consumer | agree | differ |
|---|---|---|
| literal | 168 | 3, and `applyx`'s other module written: **167** fully |
| onepass | 168 | 3 |
| rounds | 171 | 0 |
| A+: the `written` fixes, one pass, against the one-round oracle | 171 | 0 |
| ESLint's `output`, B's edits | 171 | 0 |

**Widened to all 761 roots** (510 `check/` cases, 251 fixture `.hero`
files): no root outside the 171 holds a certain fix; the 4
`fixedbugs-227-*` roots are not UTF-8, hold no fix, and `--apply` refuses
them at exit 1. On the 757 `--apply` hands back: literal 754, onepass 754,
rounds 757; **A+'s `written` fixes 757 against the one-round oracle and 754
against the full `--apply`**; **B 757 (661 edits on 170 roots, rounds 0 on
587, 1 on 167, 2 on 2, 3 on 1)**; **ESLint's shape 757**. The text path of
every route compiler equals `heroes-base`'s on 761 of 761.

Why each difference:
- **`certain137-inside`, `-last`, `-types`** (3, 2 and 2 rounds; 3, 1 and 2
  fixes enclosing another): the literal consumer writes defect 137 again,
  outside the compiler: `print(total(xs.must())ad_operand bad_operand`,
  `print(bump(k)))`, `print(copy_text("a", to: "b")none"))`, `function
  adder(n: i64) -> (function(i64) -> function(i64) -> i64)4)`, and on `-last`
  the file's last newline deleted. Onepass and A+ write nothing corrupt and
  stop a round or two short: `print(total(xs).must())`.
- **`applyx/main.hero`**: the root's bytes agree; the literal consumer also
  writes `geom.hero`, which `--apply` never does.
- **Twins**: 0 in the whole corpus; the rule is held by its unit test alone.
  A literal consumer handed a twin deletes twice; no case shows it.
- **Stale places**: 0 of 671.

**Two things no answer of `check --json` can tell a consumer, measured:**
- **`withheld` absent does not mean done.** Walking `certain137-inside` one
  round at a time with `heroes-r1`: answer 1 holds 4 `written`, 3
  `withheld`; answer 2 holds 4 `written` and **none withheld**, yet answer 3
  still writes 1 (the `.must()` chain the checker reports at its first link,
  `check.hero:177-181`); answer 4 is quiet, and the result equals
  `main.fixed` (`cmp`).
- **A loop on `check --json` reaches `--apply` run until quiet, not one
  `--apply`.** `out/shapes/stages.hero` (`fn main()`, then `print(totl)` with
  `total` bound): `--apply` fixes the stage that spoke first and stops,
  `print(totl)`; a consumer looping on `check --json` goes on to the name
  stage's certain rename, `print(total)`, which equals `check --apply` run
  twice (`cmp`). The answer does not say its stage. The corpus holds no such
  root, so the ffi-pragmatist's 537 of 537 and this are both true. B and
  ESLint's shape give one `--apply`'s answer here by construction.

## 4. The shapes beside

All with the route compilers on files in `out/shapes/` or in the corpus.
- **An insertion** (`certain137-inside`, *mark the argument*):
  `"byte_start": 825, "byte_end": 825, "text": ""`, the end equal to the start.
- **A fix at the end of the file** (`out/shapes/eof.hero`, 39 bytes, no final
  newline): `"byte_end": 39`, the file's length; `--apply` writes 38 bytes and
  the consumer the same.
- **Characters of two to four bytes** (`    s = "è€😀",`): `"col": 14`,
  `"byte_start": 35`, both right in their units.
- **`\r\n`**: read by the compiler (178's case converted, 34 `\r`); the same
  three diagnostics, every fix's line and columns unchanged, each
  `byte_start` larger by its line number less one; both consumers equal
  `--apply`, which writes CRLF back equal to `.fixed` in CRLF.
- **Two fixes of one diagnostic on two lines**: over the corpus, both arms,
  **361 of 2,488 fix objects stand on another line than their diagnostic, 254
  of them certain**.
- **A fix in another file than its diagnostic's**: measured **0 of 2,488**.
  Read: the 112 `diag.Fix(` lines plus `diag.hero:180`'s bare `Fix(`, the
  twelve lines before each searched for a span taken from a declaration
  (`decls[`, `decl.`, `.declared`, `param.*span`, `field.*span`,
  `elsewhere`): three sites, `emit/ffi_pointee.hero:128`, `:237`, `:310`,
  each a `guess` whose diagnostic stands on the same parameter. A question
  rather than a premise, so the writer asks each fix's own span for its file.
- **Another module's offsets**: `applyx`'s fix reads `geom.hero` bytes 66 to
  71, `total`, equal to its `text`; the joined text's offset would be 57
  later.

## 5. Cost

**Lines**, `out/code_lines.py` over the saved copies (base, after A and B,
final) and the variant tree; the instrument itself, `layout`, read **5
passed, 0 failed** on `heroes-final`'s tree, the largest (A, A+ and B
together), so every touched module is under its ceiling:

| route | where | lines |
|---|---|---|
| A | `cli/check_json.hero` | +18 |
| A+ | `cli/certain.hero` +42, `cli/check_json.hero` +7 | +49 |
| ESLint's `--apply --json` | `cli/apply_edits.hero` 30, `cli/check.hero` +8 | +38 |
| **mine** | all in `selfhost/cli/` | **+105** (78,955 to 79,060) |
| B instead | `cli/apply_edits.hero` 158, `cli/certain.hero` +6, `cli/check.hero` +13 (to 287 of 300) | +177 |
| a refusal instead | `build`'s shape, `cli/verbs.hero:28-30` | 3 (read) |

**The JSON's size** over the 510 `check/` cases (each compiler's stderr,
summed): `check --json` 1,415,939 bytes, **1,709,170 with A (+20.7%)**,
1,725,472 with A and A+ (+21.9%); `--permissive --json` 1,148,719, 1,314,274
and 1,327,170. Of the 458,786 bytes A adds over both arms, **203,105 (44%)
are the per-fix `"file"`**, the golden paths being long. Every answer equals
the base's once the added fields are removed: 1,020 of 1,020 for A, 761 of
761 roots for A+ against A.

**Instructions retired**, `/usr/bin/time -l`, median of three, `heroes-base`
against `heroes-final` (all routes in); no duration, since other work ran on
this Mac:

| case | `check --json` | `check --apply` |
|---|---|---|
| largest, `fixedbugs-140` (46,868 bytes) | 1,666.0 M to 1,665.4 M | 1,676.1 M to 1,676.2 M |
| most fixes, `fixedbugs-135` (59) | 181.9 M to 189.9 M, +4.35% | 329.6 M to 329.7 M |
| 178's case (10) | 64.5 M to 66.1 M, +2.5% | 108.39 M to 108.57 M |
| `certain137-inside` | 159.4 M to 160.6 M, +0.74% | 542.0 M to 542.0 M |

A alone read 189.67 M and 65.85 M on the middle two, so A+ adds about 0.1%.

**The judges**, run on `heroes-final`: the compiler's own tests **1,236
passed**; `surface` 355/0, `fixes` 769/0, `layout` 5/0, `check` 511/0,
`permissive` 6/0, `full` 7/0. On `heroes-eslint`, my route: its own tests
**1,235 passed** (B's test is not in it), `surface` 355/0, `fixes` 769/0.

**Schema**: my builds keep 1, and every reader in the tree holds. Moving to
2 costs five lines, not a ceiling question: `check_json.hero:18`, the test at
`check.hero:376` (`:391` in my copy), `suite_surface.hero:286`,
`table.hero:92` and `site/src/llms.txt:60`, the last a site push the author
must ask for. With A+ the answer promises how its fixes combine, which is
more than an added field; with the ffi-pragmatist's json-c measurement (an
absent place read as 0), moving is the robust choice. Keeping 1, the
historian's `cargo metadata` precedent, is the conservative one.

## 6. The readers

- `tests/harness/fix_names.hero:99` scans for `"title": "`; no new key is
  called that. `fixes` 769/0 on both route compilers.
- `tests/harness/suite_surface.hero` `:231-235`, `:286`, `:372`, `:425`:
  `surface` 355/0 on both. **But `:286`'s `"line": 10` now matches twice**,
  the diagnostic's and its fix's (the ffi-pragmatist's finding; measured 2
  against the base's 1), so the pin would stay green if the diagnostic's line
  moved. Anchored on the diagnostic's own row, `"\n      \"line\": 10,\n"`,
  it matches once in both: a one-line repair owed with the route.
- `selfhost/cli/check.hero:370-379`: passes.
- `heroes --help`, `selfhost/cli/table.hero:92`: true for `check --json`; with
  ESLint's shape it owes *or, with `--apply`, the program it writes*, one
  line; with a schema move, the number.
- `site/src/llms.txt:60`: true under schema 1, stale in silence under 2; it
  names no stream and no `--apply` (the spec-warden's measurement). Its
  placeholder is the spec-warden's to price; touching it is a push asked
  for.
- The errors page, both editions: prose, true under every route.
- Panel 187's R2 instrument, `<scratchpad>/inst-r2/replay.py:43`: parses JSON
  and reads only a diagnostic's `file`, `line`, `col`, `code`, `message` with
  `.get`. Unaffected (read, not run).
- `docs/roadmap/verify.md:27`: names the command only.
- The control arm: `--permissive --json` goes through the same writer, so the
  fields land there too (section 5's sizes).

## 7. The blind seat's files

In `<scratchpad>/193-compiler-engineer/blind-json/`, made before the blind
seat was stood down: `case.hero` (178's case with every comment removed:
the ten header lines, the blank after them, the three `#~` marks and line
27's note; 23 lines, 370 bytes, **0 lines with trailing whitespace**, 4
empty), `a.json` (1,605 bytes, the base's stderr), `b.json` (2,939, route
A's), `applied.hero` (360, `--apply`'s stdout). The stripped case draws the
same ten fixes: three `expected_end_of_line` at 2:10, 8:15 and 13:10 with
3, 3 and 4 fixes, all `delete the ,`, `certain`. `applied.hero` is the ten
line-ending commas deleted and checks clean at exit 0. A+ marks all ten
`written`; B answers one round of 10 edits (`out/blind-final.json`,
`out/blind-b.json`).

## 8. The critic's question, answered by measurement

**What an answer can promise about how its fixes combine**, with my route,
each clause measured above:
1. every place is in the text as checked, in its own file's bytes, lines and
   characters (0 stale of 671; `applyx`; CRLF);
2. the fixes marked `written` are written together in one pass, and only
   they: that is the first round, by construction (757 of 757);
3. another round may be owed **whether or not** a fix says `withheld` (the
   chain); a consumer asks again after any round that wrote;
4. asking again until no fix is `written` reaches `check --apply` run until
   quiet; one `check --apply` stops at the stage that spoke first
   (`stages.hero`). A consumer that wants exactly one `--apply` asks
   `--apply --json`.

**Panel 016's**: since `1697cec1` the text names a fix's start off its
message's line; route A says at least that, in the same unit and per-file
lines, for every fix. It says more (the end, bytes, the replaced text), as
`"replacement"` already said more than the text before this sitting (the
spec-warden's reading).

## 9. Beside the question

- A clean program gets no JSON document (`check.hero:100-103`,
  `suite_surface.hero:372`): found by three seats; no route here moves it.
- `.claude/agents/compiler-engineer.md` in the frozen tree names
  `selfhost/` as the live compiler in its description (`:3`) and at `:19`
  (grep), which is the tree I measured; I did not read the rest of it.
