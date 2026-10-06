# Panel 193, the completeness critic, second pass: the seats' reports

Written 2026-10-06 from 02:32:01 (`date`). No verdict. This pass covers the
reports of the compiler-engineer (02:10), the ffi-pragmatist (01:41), the
spec-warden (23:34) and the historian (01:41), and the repaired briefs (23:14).
Every number below comes from a command run in this session. A sentence that
is an inference says so.

**What I ran it on.**
- `<scratchpad>/193-critic/`: the frozen tree at `00217c39`, with `heroes`
  built from the seed.
- `heroes-r64`: built from that copy's untouched `selfhost/` by the seed
  compiler. It is not byte-identical to the compiler-engineer's `heroes-base`
  (`cmp`: they differ at byte 1234), so I took instruction counts on my own
  pair only.
- `r1/heroes-r1`: `ROUNDS` set to 1, the one-round oracle from my first pass.
- **`ce/heroes-ce`**: a fresh archive of `00217c39` with the four files of the
  compiler-engineer's adopted route copied in (`out/eslint/selfhost/cli/`:
  `check_json`, `certain`, `check`, `apply_edits`; `diff -rq` shows nothing
  else differs), built by my seed compiler at 02:14-02:16. It is route A, A+
  and `--apply --json` in ESLint's shape.

My scripts are under `<scratchpad>/193-critic/v2/`. Other seats' folders were
only read. On the Windows box I ran the ffi-pragmatist's frozen `heroes.exe`
twice through a pipe and wrote nothing. No paid run.

## 1. The numbers the synthesis would rest on, re-run

### 1.1 Reproduced exactly

- **Line costs** (`v2/code_lines.py`, a replica of `suite_layout.hero:792-807`):
  - `selfhost/` reads **78,955 over 415 files** before and **79,060 over 416**
    after, so **+105**.
  - Per file: `check_json.hero` 50 to 75 (A +18, A+ +7), `certain.hero` 193 to
    235 (+42), `check.hero` 274 to 282 (+8), `apply_edits.hero` 30.
  - Nothing outside `selfhost/cli/` differs (`diff -rq`).
  - (The compiler-engineer's "over 416 files" is the after count; before it
    is 415.)
- **The answer is unchanged but for its additions**: removing the added keys
  gives the base's answer on **761 of 761 roots in each arm** (`check --json`
  and `--permissive --json`, same exit, same stdout; `v2/census.py`,
  `v2/analyse.py`). The text path is identical too: rich, `--brief` and
  `--permissive` output equal the base's on 761 of 761 roots each.
- **The ffi-pragmatist's census** (normal arm, the 541 roots), all from the
  route's own places:

  | measure | total | roots |
  |---|---|---|
  | fixes | 1,551 | 281 |
  | certain | 670 | 170 |
  | certain off their message's line | 134 | 43 |
  | spanning more than one line | 368 | 89 |
  | empty spans | 178 | 22 |
  | non-ASCII before a fix on its line | 1 | |
  | non-ASCII inside a fix | 2 | |
  | another file than the diagnostic's | 0 | |
  | twins | 0 | |
  | stale `text` | 0 | |

  Certain fixes on their message's line: **536 of 670**.
- **The compiler-engineer's both-arm counts** over 761 roots: **2,488** fix
  objects, **361** off their diagnostic's line, **254** of them certain.
- **Consumers against the applier** (757 roots where `--apply` exits 0):
  - the `first_round: written` fixes, applied once by their bytes, equal the
    one-round oracle on **757 of 757** and the full `--apply` on **754**;
  - every certain fix written literally also equals `--apply` on 754;
  - both miss the same three roots, `certain137-{inside,last,types}`;
  - `--apply --json`'s `output` equals `--apply` on **757 of 757**, and the
    route's own `--apply` text equals the base's on 757.
- **The outcomes over the 761** (normal arm): `written` 664, `withheld` 6,
  `elsewhere` 1, nothing else.
- **The walk the ffi-pragmatist's prediction contradicts**
  (`v2/walk.py`: the route's answer, then the one-round oracle, repeated):
  - `certain137-inside`: answer 1 has 4 `written` and 3 `withheld`; answer 2
    has **4 `written` and no `withheld`**; answer 3 has 1 `written`; answer 4
    is quiet (exit 0) and equals `--apply`.
  - `-last` and `-types` settle in 3 answers.
- **The stage**: `out/shapes/stages.hero` gives `function main()` after one
  `--apply`, and only a second `--apply` writes `print(total)`.
- **Sizes**, 510 `check/` cases:
  - `--json` 1,415,939 bytes before, 1,725,472 with A and A+ (+21.9%);
  - `--permissive --json` 1,148,719 before, 1,327,170 after;
  - A's own additions over both arms are 458,786 bytes, of which the per-fix
    `file` is **203,105 (44%)**.
- **Judges on the adopted route** (`ce/`): the compiler's own tests **1,235
  passed**; `surface` 355/0, `fixes` 769/0, `layout` 5/0, `check` 511/0,
  `permissive` 6/0, `full` 7/0.
- **The `:286` pin**: `"line": 10` matches **twice** in the route's answer of
  `certain-labels.hero` (once in the base's). Anchored on the diagnostic's own
  row, it matches once in both.
- **Shapes**:
  - `applyx`: `main.hero` 56 bytes, `geom.hero` 89 bytes, the fix at
    `geom.hero` bytes **66 to 71**, `total`, outcome `elsewhere`; the joined
    offset is 57 later, 56 plus the joiner.
  - `fixedbugs-242`: the BOM spans bytes 0 to 3 and columns 1 to 2.
  - `eof.hero`: 39 bytes, `byte_end` 39, and `--apply` writes 38.
  - `certain137-last`: 490-513 `withheld` around 496-505 `written`, in 515
    bytes.
  - `certain137-types`: `(function(int) -> int)` around two `i64`.
- **The spec-warden's citations and prices**:
  - Each of the nine words it greps reads 0 in the spec; `fix` reads 5
    (`Prefix`, `Postfix` and one `fixed`).
  - A clean program's answer is 0 bytes on both streams.
  - Only `check_json.hero` and `seed/heroes.c` name `"replacement"`.
  - 178's case holds 10 commas outside its comments, all line-ending.
  - A `sed` strip leaves trailing spaces on lines 13, 19, 24 and 27, which
    `--apply` keeps, with 15 empty lines.
  - `DELTA_GATE` 50 (`suite_spec.hero:183-184`); design.md §1.6 at `:253`,
    with `:255-256` as quoted.
  - The stopping rule at `cli-surface.md:18`; `claims.ts:217` reads verb
    names only; `check.hero` is the one module that uses both `cli/table`
    and `cli/check_json`, and `table.hero` has 0 `use` lines.
  - design.md 87,901 / 90,605; `spec-warden.md:53` as quoted.
  - **Every price reproduces** from its own files (`heroes measure`): the spec
    drafts 7026/7153, 7055/7182 and 7062/7190; `llms.txt` 1109/1108 to
    1128/1130; the §4.17 draft 138/140; the mock answers 536/592, 1006/1079
    and 636/709 without a per-fix `file`, so the `file` is 370 of the 487
    added cl100k tokens.
- **The blind seat's files**:
  - `case.hero` 23 lines, 370 bytes, 0 lines with trailing whitespace, 4
    empty, no `#`.
  - `applied.hero` 360 bytes, reproduced by my `--apply` (`cmp`).
  - `a.json` equals my base answer, and `b.json` equals my route's answer
    without `first_round` (route A alone), file names aside.
  - The same ten fixes at 2:10, 8:15 and 13:10.
- **json-c 0.19** (`/opt/homebrew/include/json-c/json_object.h`): "null is
  equivalent to 0 (no error values set)", and my probe reads an absent
  `byte_start` as **0**, presence "no".
- **Windows**: the frozen `heroes.exe` answers on `c178crlf.hero` and
  `u1crlf.hero` hold **0 `\r`**, where the files hold 34 and 3.
- **The historian's locally runnable claims**: rustc 1.90's diagnostic keys are
  `$message_type`, `children`, `code`, `level`, `message`, `rendered` and
  `spans`, with no version field; clang's SARIF goes to stderr (stdout 0
  bytes, stderr 2,124).
- **The historian's CRLF prediction, scored here first**: of the 139 answered
  `check/` cases, 2 already hold `\r`. On the other **137 converted to CRLF**,
  the `written` places applied by bytes equal `--apply` on the CRLF file
  **137 of 137** (`v2/crlf139.py`). Every place's `text` matched the CRLF
  file's bytes (the script asserts it).

### 1.2 Reproduced with a difference

- **Touching pairs: the compiler-engineer's "3 of 171 roots hold 6 touching
  pairs" against the ffi-pragmatist's 7 in 3.** By `certain.hero`'s own
  `touches` (overlap, or two insertions at one point) over the route's places
  there are **7 pairs in 3 roots**: 3 in `-inside`, 1 in `-last`, 3 in
  `-types`. There are **6 `withheld` fixes**, because one fix in `-types`
  encloses two. The 6 counts fixes, not pairs; the ffi-pragmatist's 7 is the
  pair count.
- **Instructions retired** (`/usr/bin/time -l`, median of three, my pair):
  - `fixedbugs-140`: 1,664.87 M to 1,665.39 M;
  - `fixedbugs-135`: 181.67 M to 189.76 M (+4.5%);
  - 178's case: 64.07 M to 65.93 M (+2.9%);
  - `certain137-inside`: 159.12 M to 160.45 M (+0.8%).

  These agree with the compiler-engineer's within noise. **But no seat
  measured the shape that grows**; see 3.2.
- **Route B's +177**: not reproducible from what is saved. The final tree's
  `apply_edits.hero` reads 167 lines and the tree 79,208 (+253 for A, A+ and
  B together), so B alone is +186 there, not +177. B is not adopted; the
  synthesis rests on nothing here.

### 1.3 Not re-run by me, and why

- **The ffi-pragmatist's C consumer**:
  - its per-unit results (byte 532, utf16 534);
  - its C line counts (196, 47, 47, 6);
  - its Windows text-mode runs.

  The place data under them reproduces (1.1), but I did not compile its 528
  lines.
- **The historian's web pages**, beyond the LSP sentences re-read raw (§ 4 of
  this report) and two local runs.
- **The spec-warden's prediction 2** (arm A at least 4 of 5): it needs a paid
  run.

## 2. Contradictions between seats, and which is checkable

2.1 **The schema: 2 (ffi-pragmatist) against 1 (spec-warden, historian), the
compiler-engineer between them.** The seats argue opposite directions:
- an old reader meeting a new answer: every in-tree reader holds under 1,
  measured above;
- a new reader meeting an old answer: json-c reads the absent place as 0,
  measured by the ffi-pragmatist and again by me.

Checkable facts that neither side used:
- **a clean program's answer is 0 bytes** (1.1), so no number guards it;
- **the adopted route writes `"schema": 1` on a second, different document**
  (3.1).

So the number cannot do the job the ffi-pragmatist gives it unless a clean
answer is a document and each document says its kind. Whichever number is
chosen, those two are owed.

2.2 **`--apply --json`: answer (compiler-engineer, historian) or refuse at exit
2 (spec-warden; the ffi-pragmatist accepts the refusal).** The two positions
never met. The spec-warden's refusal rests on *an answer of edits changes
what* (its § 5) and was written at 23:34. The compiler-engineer's ESLint shape
was written at 02:03:42 (its two files' times, `stat`), and it is not edits: run here, it answers
`{"schema", "file", "rounds", "not_applied", "output"}`.
- `output` is `--apply`'s program, and `not_applied` repeats the stderr note,
  which `--apply` still prints.
- **`rounds` is new information**: 3 on `certain137-inside`, 1 on 178's case,
  0 on `applyx`, printed nowhere today but in the 64-round note.

The sitting should ask the spec-warden about this shape, or rule on it,
rather than read its refusal as aimed at it. The measurement in 3.1 bears on
it.

2.3 **The ffi-pragmatist's prediction against the compiler-engineer's walk.**
The prediction says a consumer that "asks again while any fix is `withheld`"
equals `--apply` on all 537 roots. **It is falsified by a measurement already
in hand**, the compiler-engineer's and mine (1.1): on `certain137-inside`,
answer 2 holds no `withheld`, and answer 3 still writes. The rule that holds
is the compiler-engineer's: ask again after any answer that had a `written`
fix. The ffi-pragmatist's own measured loop ("until `check` exits 0") is a
third rule. It never ends on an `.applied` case, and it crosses stages
(`stages.hero`). The terms must state one rule; on the 757 roots only the
compiler-engineer's is measured to reach `--apply` and stop.

2.4 **The per-fix `file`: always (compiler-engineer, ffi-pragmatist), or only
where it differs (historian).** Checkable, and checked:
- it differs on **0 of 2,488** fix objects today;
- it is **44%** of A's added bytes (203,105) and 370 of 487 added tokens on
  178's answer (the spec-warden's mock-ups, re-measured).

Against that, json-c reads an absent int as 0 (measured), and every C consumer
of an optional `file` must code an inheritance rule. It is a ruling between
bytes and a rule, with both sides measured.

2.5 **"One fix at a time"**: the compiler-engineer and the spec-warden read
§4.17 as promising mechanical application one fix at a time. The section does
not say it in those words (`grep -c 'at a time'` over `design.md:2090-2183`:
0). It is their reading, and the synthesis should cite it as one.

2.6 **The ffi-pragmatist's condition (1), "`malformed` where no place is
given", against what was built.** By reading `certain.outcomes` and
`check_json.place`: every `written` fix has a place, since `written` needs a
span inside the root. But a certain span that crosses out of the root gets no
place and the outcome **`elsewhere`**, not `malformed`. The corpus holds none.
The condition is met in substance, not in letter.

2.7 **Two numbers that are not contradictions** but must not be mixed: "80%"
(536 of 670) is the ffi-pragmatist's normal arm over 541 roots, and 254 of 361
is the compiler-engineer's both arms over 761. Both reproduce. And the
historian's "the four places that write schema 1 (critic 2.7)": my 2.7 named
six, three byte pins (`check_json.hero:16`, `check.hero:376`,
`suite_surface.hero:286`) and three in prose.

## 3. Findings no seat made

3.1 **Two documents under one number crash an idiomatic consumer.**
`v2/jc/kind2.c`, 19 lines of C11 against json-c 0.19, is a consumer of
`check --json` that checks `"schema" == 1` (the writer's own invitation,
`check_json.hero:12-14`), then reads `diagnostics`.
- Fed `ce/heroes-ce check --apply --json` on 178's case, it prints `schema 1
  accepted; diagnostics key absent (NULL)` and **dies with SIGSEGV, exit 139**
  in `json_object_array_length(NULL)`.
- Fed a real answer, it reads 3 diagnostics, exit 0.
- Fed today's `check --apply --json`, the program as text, it says `refused:
  schema 0`, exit 2.

So today the mistake is refused loudly. Under the adopted route it meets valid
JSON with the right number. A wrapper that
passes a user's `--apply` through with `--json` is the realistic path; under
a route that answers instead of refusing, a non-UTF-8 root gets a diagnostics
document on stderr at exit 1 from the same invocation. The historian's own
record has the precedent: rustc added `$message_type` because "the
json-formatted outputs have no way to unambiguously determine which kind of
message is being output" (its § 7).

3.2 **The route's cost grows with fixes times line length.** `place` calls
`source.locate` twice per fix, and `locate` walks its line from the start
(defect 256's note). On `v2/longline.hero`, one line of 14,039 bytes holding
2,000 `unknown_name` diagnostics each with one fix, `check --json` costs:
- **148,557,231,193 instructions** before and **171,755,276,755** after
  (**+15.6%**);
- with 500 such fixes, 25.10 billion to 26.58 billion (+5.9%).

Four times the fixes cost about sixteen times the increment. The seats'
"at most about 4.5%" holds for the corpus, not for the shape.
`51406d01`'s one forward walk (defect 256's repair) is the known cure.

**Beside the question**: the base alone costs **148.4 billion instructions
for `check --brief`** on that file, so the stages already grow
super-linearly. I found no issue filing it (searched `issues/` for
`unknown_name` or "did you mean" with quadratic, square, billion,
instructions retired, edit distance). It is for the coordinator to file and
class.

3.3 **CRLF moves a diagnostic's column at a line's end.** On **12 of the 137**
CRLF conversions, a diagnostic placed at the end of a line reports its
`"col"` **one larger** than in the LF file (for example
`continuation-outside-brackets-in-other-words` at 13:36 against 13:37): the
`\r` counts as a character. Code, line and fix count are unchanged. **No fix
place splits a `\r\n`**: 0 of 794 places scanned in the 137 files. So "line
and column agree between LF and CRLF" (compiler-engineer, ffi-pragmatist) holds
for every fix place measured, and not for every diagnostic.

3.4 **`text` guards a place, not the program.** In `v2/stale2/`, `main.hero`
calls `geom.aera`, and the route answers a certain `write geom.area` at
`main.hero` bytes 41 to 45. I edited only `geom.hero` afterwards (`area`
renamed `surface`). The `text` check passes, since the root's bytes are
unchanged, and the consumer writes `geom.area`. `check` then says
`unknown_in_module` (exit 1). The language turned the stale write into a loud
error here, but "a consumer can refuse a changed file" (compiler-engineer
§ 2, the shared brief's route) is true of the fix's own bytes only.

3.5 **Every guess gets a byte place.** Under the route, 881 of the 1,551 fix
objects in the normal arm (541 roots) are `guess` and all are placed, as
machine-applicable in form as a `certain` one. Only `first_round` (certain
fixes alone) separates them. §4.17 says "a model will apply whatever the
compiler blesses" (`design.md:2127-2128`). No seat asked whether a guess should
carry a byte place, or say what its place is for.

3.6 **Corrections to my first pass.**
- The stopping rule starts at `cli-surface.md:18`, not `:16-18` (`grep -n`).
- My first pass named SARIF's `fix.artifactChanges[].replacements[]` without
  a page. The historian's reading of the standard (§3.55 to §3.57,
  pp. 182-187) confirms it.
- My first pass's three LSP quotations came through the same kind of fetch
  the historian warns about. Re-read raw (`curl` of the published 3.17 page,
  918,678 bytes, `grep -c -F`), each appears verbatim once:
  - "If the server omits the position encoding in its initialize result the
    encoding defaults to the string value";
  - "the only mandatory encoding is UTF-16";
  - "the end position is exclusive".

## 4. Routes nobody listed

- **A kind per document**: `check --json` and `check --apply --json` each say
  what they are (a key, or a schema of their own), so the number never
  identifies two shapes. This is rustc's `$message_type` precedent (3.1).
- **A document for every answer**, `{"schema": N, "diagnostics": []}` for a
  clean program. Three seats found the empty answer and filed it as beside
  the question. It is the half of the schema guard that no route provides
  (2.1). It moves `suite_surface.hero:372`, defect 109's pin.
- **The stage in the answer**: `report` already holds `stage` (`check.hero:88`).
  Saying it lets a consumer stop where one `--apply` stops, or know that names
  and types were not yet read (`stages.hero`).
- **A hash of every file read** (SARIF's artifact `hashes`; LSP's document
  version, the historian's § 4), beside or instead of per-fix `text`. It
  refuses the 3.4 shape that `text` lets through.
- **One forward walk for places** instead of two `locate` calls per fix: the
  cure for 3.2, already used in `51406d01`.
- **Places on certain fixes only**, or a guess's place marked as for display.
  This is the route 3.5 implies. The precedents read by the historian (rustc,
  ESLint, Go) place every suggestion and separate applicability, so it is a
  ruling, not a default.

## 5. The blind seat's ruling: what it loses

**On 178's case the ruling stands on counts.** The stripped `case.hero` holds
10 commas, all line-ending, and its answer 10 identical deletions; the message
states the rule; the strip left no trailing whitespace. Arm A can pass by
deleting every comma, so the arms could not separate there. That is an
inference, but the counts leave little room.

**What it loses:**
1. **The only measurement of a model.** "Two seats measured the real consumer"
   is true of tools (Python, C), not of the reader the thesis is about. The
   field does not need a model effect: panel 016's item 2 and the tool
   measurements carry it, as the spec-warden argues. But anything the sitting
   writes **for models** (`llms.txt`, the site) is now unmeasured. The
   spec-warden's default, *do not advertise places to models*, then holds by
   default. The synthesis should say so, or a later edit will advertise them
   unmeasured.
2. **A case that separates exists.** In
   `tests/golden/check/fixedbugs-130-commas-in-braces-are-told-as-without-them.hero`
   (answer `.applied`) the answer carries **12 identical certain `delete the ,`**
   beside **13 line-ending commas**: line 36's `},` is the one that stays, as
   does the comma inside line 51's `a: i64, b: i64`. A reader without places
   must choose 12 of 13; one with places need not. That is an inference,
   unrun, and the stripped file must be re-checked to draw the same 12. Across
   the corpus, 81 roots have certain fixes whose replaced text recurs in code
   that must stay (`v2/ambiguous.py`, a rough proxy).
3. **The harm case**, the spec-warden's point 5: stripped `certain137-last`,
   where answer 1 holds `total(xs.must()).must()` `withheld` around
   `xs.must()` `written`. Does a model given places and `first_round` write
   the nested pair, `print(total(xs.must())`, or respect `withheld`? Tools
   were measured; no model was.
4. **Whether a model applies a placed guess** (3.5).

**What recovering it costs**, under the brief's own rule (0.20 USD cap a
session, stop at 2.2 USD by `total_cost_usd`): about 2.2 USD for one case,
about 4.4 USD for 130's case and the harm case together. It is the author's
yes, not mine.

## 6. The question still unasked

**Can a consumer tell, from the bytes it holds, which document it has and
which of its places it may write?** Today it cannot, on four measured counts:
- two documents both say `"schema": 1`, and the idiomatic json-c reader dies
  on the wrong one (3.1);
- a clean answer is no document at all (2.1);
- 881 guesses carry byte places as machine-applicable in form as a certain
  fix (3.5);
- an answer that has no `withheld` can still owe a round (2.3), and an answer
  does not say its stage.

Each seat answered *where* a fix applies; no seat answered *what licenses
writing it*. The terms the seats would write in §4.17 should hold both.
