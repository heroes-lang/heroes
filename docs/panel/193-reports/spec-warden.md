# Panel 193, the spec-warden

Written 2026-10-05 from 23:16:21 to 23:34:05 (`date`), as I went. Every number
below was produced in this session by the command named beside it; a sentence
that is an inference says so.

**My copy**: `<scratchpad>/193-spec-warden/`, made at 23:15:59 with `git -C
/Users/joseph/Temp/heroes/heroes-lang archive 00217c39 | tar -x -C <dir>`; the
compiler built there from the seed (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, `real 4.07`). No paid run; `heroes measure`
without `--refresh` only. Lane str192's commit read with `git show 1697cec1`.
My scratch files are under `<copy>/out/` and `<copy>/out/price/`.

**The ceiling, reached by grep this sitting**: design.md §1.6
(`grep -n '^### 1\.6' docs/design.md` gives `:253`), lines 255-256 of my copy:
*must fit in 10240 tokens, measured by `claude-opus-5` through `POST
/v1/messages/count_tokens`*.

## Verdict

- **verdict**: object (not provisional: the route I would adopt does not move
  the spec, so its delta needs no estimate)
- **section**: design.md §1.6 and §1.2 for the spec, which owes nothing; §4.17
  (`:2125-2131`, `:2142-2144`) for the objection. §4.17 calls a `certain` fix
  one that *may be applied mechanically* and holds that it *writes nothing
  else*, one fix at a time; **design.md does not cover how an answer's fixes
  compose**, and says nothing of `--json` at all (`grep -c -F -- --json` and
  `schema`: 0). The tool rule is `.claude/rules/cli-surface.md` (panel 016).
- **spec_token_delta**: measured **0** for the route I would adopt:
  `spec/heroes-spec.md` stays at legacy 6990, cl100k 7117, real 9392 (recorded
  2026-10-03), headroom 848. The refused alternatives, measured on the vendored
  tables (legacy / cl100k): a parity sentence +36 / +36, with the combination
  clause +65 / +65, under its own heading +72 / +73; the reader's count of
  those is unmeasured (§6).
- **removal**: nothing, and none is owed: the spec does not move. What the
  change does displace is a model's reading budget (§4), which I would pay back
  by pointing models at `check --apply` and the text form, not at the JSON.
- **needed_for_self_hosting**: no.
- **argument**: The spec owes nothing: it names no command, and §1.6 scopes it
  to syntax, semantics and library; a sentence would be the prompt's first
  tool text, with Principle 0's burden unmet. My objection is to the clause *so
  a tool can apply a fix without re-deriving it*. On `certain137-last` one
  answer holds `total(xs.must())` and `xs`, a call and the call inside it;
  written at their places back to front they give `print(total(xs.must())`,
  `unclosed_bracket`, where `--apply` writes a file that checks clean. A place
  makes a fix locatable, not an answer applicable. The most robust resolution
  binds every choice (CLAUDE.md § Precedence), so places land only with the
  combination `certain.applied` computes, reported and never restated.
- **prediction**: (1) tokens: the landing leaves `spec/heroes-spec.md` at 6990 /
  7117 and `SPEC_TOKENS` at 7117; any spec diff falsifies it. (2) rewrite rate,
  registered before any session runs: on 178's comment-stripped case **arm A,
  with no place, passes at least 4 of 5**, against the coordinator's *at most
  2*, provided the strip leaves no trailing whitespace; so B leads A by fewer
  than 2. (3) robustness, free: over the route's own JSON, writing every
  certain fix of one answer at its place, back to front, disagrees with
  `--apply` on at least the three `certain137-*` roots.
- **condition**: approve when (a) no spec text; (b) each fix's place lands with
  a datum `selfhost/cli/certain.hero` computes, what `--apply`'s first round
  does with it (written, dropped as a twin, withheld for the next round, in
  another module, malformed: its five outcomes), so a consumer that writes
  exactly the written ones writes what that round writes, by construction; (c)
  `check --apply --json` is refused at exit 2 until a consumer must type its
  edits; (d) the schema number is held in `llms.txt` by a placeholder and in
  `--help` by a compiler test; (e) `llms.txt` names stderr and `--apply`.
  **Veto** on any spec sentence about the JSON without a measured Part 11
  effect. The objection falls if the sitting shows that no consumer ever
  writes two fixes of one answer together.

## 1. The spec: what it says, and why it owes nothing

- `./heroes measure spec/heroes-spec.md` (23:17:43): `claude-legacy 6990`,
  `cl100k_base 7117`, `maximum 7117` (*a lower bound, not the reader's
  tokeniser*), `real 9392 claude-opus-5, 2026-10-03`, headroom 848, 60 of it
  mortgaged to the FFI floor, so 9452 meets the ceiling.
- **The spec names no command at all.** `grep -c -F` on `spec/heroes-spec.md`
  gives 0 for each of `--json`, `schema`, `heroes check`, `--apply`,
  `certain`, `guess`, `diagnostic`, `stderr` and `heroes ` (a command word);
  `fix`'s five hits are `Prefix`, `Postfix` and `fixed`. Its sections
  (`grep -n '^#'`) run from *1. Files and layout* to *13. FFI*, none about the
  tool.
- §1.6's scope (`:255`): *syntax, semantics, built-in library*. The JSON is a
  format of the tool (panel 016's resolution item 2, `016-command-surface.md:118`),
  and the errors page says its reader is *not a person but another program*
  (`site/src/html/docs/errors.html:39-40`).
- So **no sentence is owed where the budget counts**, and one written anyway
  would be the first tool sentence in the prompt, carrying Principle 0's burden
  as spec text with no measurement behind it: this seat would veto it.

**Where the schema's terms are owed a home, and it is not the spec.**
`check_json.hero:1` cites design.md §4.17 as the schema's home, and §4.17
(`:2090-2182`) holds no word of JSON. A schema with no written terms is a
promise nobody wrote down (the writer's own comment, `:12-14`: *a schema is a
promise*). CLAUDE.md's opening rule, each rule in exactly one place, puts the
terms in §4.17, which makes the writer's citation true and costs no budgeted
token; the combination rule's home stays `cli/certain.hero`, and the answer
reports what that file decides rather than restating it (§6 prices a draft).

## 2. What I measured that bears on the verdict

- **The stream**: `./heroes check --json <178's case> > out/178.stdout 2>
  out/178.stderr`: exit 1, stdout **0** bytes, stderr **1799**; `grep -o
  '{"title": [^}]*}' | sort | uniq -c` gives **10** byte-identical fix objects.
- **A clean program gets no JSON document at all**: `./heroes check --json
  examples/gallery/00-first.hero`: exit 0, **0 bytes on both streams**, and
  `python3 -c "json.load(open('out/clean.stderr'))"` fails *Expecting value:
  line 1 column 1 (char 0)*. `tests/harness/suite_surface.hero:372` pins it
  (`out_empty().err_empty()`), written at defect 109, a panic repair, while
  `lex --dump-tokens --json` on the same empty file prints a document (`:374`).
  So the writer's *a consumer that reads `schema` can refuse a version it does
  not know* holds only where there are diagnostics. A finding beside the
  question, for the coordinator to file and class; it bears on what an answer
  promises.
- **The JSON already says more than the text**: the rendered form prints a
  fix's `title` alone (`selfhost/diag_render.hero:55`), so schema 1's
  `"replacement"` is a datum the text never printed. Nothing reads it:
  `grep -rln -F '"replacement"'` (less `docs/panel/`, `issues/`,
  `docs/records/`, `archive/`) finds only `selfhost/cli/check_json.hero` and
  `seed/heroes.c`. So panel 016's *format modifier of whatever the command
  already prints* has been read since schema 1 as *the same diagnostics in
  machine form*, and `replacement` with no place is half an edit.
- **An answer's places are not independently applicable** (`./heroes check
  --json` on the three `tests/golden/surface-fixtures/certain137-*` roots):
  `certain137-last` answers two certain fixes, `"total(xs.must())"` and `"xs"`,
  the first made from the original text and still holding the `.must()` the
  second removes; `certain137-types` answers `"(function(int) -> int)"` beside
  two `"i64"`s that replace the `int`s inside it.
- **What a consumer doing the obvious thing writes** (`out/certain137-last.naive.hero`):
  the two spans located by reading the replacement strings, since the frozen
  compiler prints none (outer 490-513, inner 496-505, of 515 bytes), and
  written back to front, each on the text the later one left, which is the
  order the compiler itself used before defect 137. The last line becomes
  `    print(total(xs.must())`, and `./heroes check --brief` on it says
  `12:10: error[unclosed_bracket]` (exit 1); the fixture's `main.fixed`, what
  `--apply` writes, checks at exit 0. `selfhost/cli/certain.hero:15-18` records
  the same order in the compiler on 2026-09-30: exit 134, and
  `print(total(xs.must())rint(0)` with a line below.
- **Defect 371 on my compiler**: `./heroes check --apply --json <178's case>`:
  exit 0, stdout 1218 bytes equal to the case's `.fixed` (`cmp`), stderr 0;
  `./heroes build --dump-ir --emit-c examples/gallery/00-first.hero` exits 2,
  *`--dump-ir` and `--emit-c` are both stops*, then, after a dash, *ask for one
  artifact at a time*,
  which is the refusal's existing shape.

## 3. The published promises, and the instrument each lacks

Only two carriers of the number are checked, both bytes:
`grep -rn -e llms -e 'schema 1' site/src/lib/ tests/harness/` finds
`suite_surface.hero:286` alone, and the compiler test is `check.hero:376`.

| carrier | an added field | a moved `"schema"` | held today by | the instrument I would add |
|---|---|---|---|---|
| `site/src/llms.txt:60-61`, *emits schema 1 with stable snake_case error codes, source locations, notes and suggested fixes* | true; it lists kinds of content, not a closed set of keys | **false, in silence** | nothing: `llms.txt.ts` fills three placeholders, none the schema | `{{jsonSchema}}`, filled by `site/src/pages/llms.txt.ts` from a `claims.ts` reader of the writer's own literal, the route `{{ceilingK}}` took after this page said 4096 under a 6144 ceiling (`llms.txt.ts:4-12`); a placeholder the build does not know is already a build error |
| `heroes --help`, `selfhost/cli/table.hero:92`, *(schema 1)* | true | **false, in silence** | nothing: `claims.ts:217` reads `table.hero`'s verb names only | a compiler test in `selfhost/cli/check.hero`, the one module that `use`s both `cli/table` and `cli/check_json` (`grep '^use '`; `table.hero` uses nothing), reading the number the writer prints and asserting the `--json` row says it |
| the errors page, `errors.html:39-40` and `it/docs/errors.html:39-41`, *the same messages also come out as JSON* | true | true, it names no number | nothing can read prose of equivalence | **the parity test**: for a fix off its message's line, the JSON's place equals the rendered ` (at file:line:col)`, a compiler test beside `1697cec1`'s own; it holds this sentence and panel 016's item 2 together, and since `1697cec1` the sentence is less true than it was |

Two things the published page gets wrong for a reader today, measured above:
it names no stream (`grep -n -i -e apply -e stderr site/src/llms.txt`: exit 1,
no line), and a reader of stdout gets 0 bytes; and a clean program gets no
document. It also never names `--apply`, so a model that reads it learns of
*suggested fixes* and not of the one applier that handles rounds, twins and
withheld fixes.

**Whether `"schema"` moves**: an added key that changes no existing key's
meaning keeps every schema-1 reader in the tree right (`fix_names.hero:99`
reads titles; `suite_surface.hero:286`, `:372`, `:425` read the head, an empty
answer, a per-file line), so it does not move. Changing an existing key's
meaning does, and then both silent carriers move in the same commit: the
critic's *one fix of several edits* moves `replacement` into the edits, so it
moves the schema; places added beside today's keys do not. **What the site owes
if it moves**: the number in `llms.txt`, through the placeholder, and a push
touching `site/` publishes it, so it is asked for (CLAUDE.md § Hard stops); the
errors page owes nothing for a move.

## 4. §1.2: what a place costs the model that reads it, and what it saves

Mock-ups of 178's answer, one per route, built by `out/price/mock.py` from the
case's own bytes (key names mine, not any seat's output; each parses with
`json.loads`; the first place it computes is 13:10, the diagnostic's own),
each measured with `./heroes measure` (legacy / cl100k):

| route | the case's own path | as `case.hero` |
|---|---|---|
| today | 536 / 592 | 449 / 505 |
| + a unit declared once | 543 / 600 | 456 / 513 |
| + `file`, `line`, `col` (the text's own place) | 1006 / 1079 | 629 / 702 |
| + an end in lines and columns | 1146 / 1239 | 769 / 862 |
| + byte offsets | 1265 / 1367 | 888 / 990 |
| + the replaced text | 1325 / 1427 | 948 / 1050 |
| one fix per message, ten edits | 1129 / 1198 | 752 / 821 |
| the text's place, without a per-fix `file` | 636 / 709 | 549 / 622 |

So a fix's per-fix `file` is 370 of the 487 tokens the text's place adds on
the case's own path (cl100k), and the full place adds 775 to 835, which is
§1.2's price of one correction round (500 to 2000) on every read of a ten-fix
answer. For a tool those tokens are free; for a model they are paid each read.

**And on the blind run's case a place saves nothing a model needs** (an
inference from these counts, registered as prediction 2): outside its comments
178's case holds **exactly ten commas, every one ending its line**
(`sed 's/#.*$//' | grep -o ',' | wc -l`: 10; ending a line: 10), against ten
*delete the `,`* fixes and a message that states the rule, *no `,` ends a
statement's line*. Arm A can pass by deleting every comma.

So under §1.2 the cheapest correct route for a model is the text form, whose
` (at file:line:col)` is paid only for a fix off its message's line, and
`check --apply`, which writes every certain fix with no edit by the model.
The places serve tools. That is why I would not advertise them to models.

## 5. Principle 0's reading, and what the blind run must show before it runs

**Principle 0 does not reach a JSON field.** CLAUDE.md § 2 and design.md §1.0
(*the burden of proof for any proposed form*) govern forms of the language. A
key in a tool's answer is none, so the principle's text reaches this sitting
only as spec text, which I refuse above.

**Its analogue for the tool is panel 016's stopping rule**
(`.claude/rules/cli-surface.md:18-20`): a capability enters if the fixpoint
invocation, the golden harness or the Part 11 harness must type it, or it has
a measured Part 11 effect. Measured: nothing types a fix's place.
Of what `grep -rn -e '--json' tests/harness/*.hero` finds, the lines that run
`check ... --json` are `fix_names.hero:99`, which reads titles, and the
`suite_surface.hero` rows `:235` (escaping), `:286` (the head), `:372` (an
empty answer) and `:425` (a per-file line). The Part 11 harness
holds metric 2's prompt alone (`ls harness/prompts`: `first-try.md`), and
`grep -rn -i json harness/` finds nothing (exit 1). Metric 4 is unbuilt
(design.md `:4044-4045`; `harness/README.md:1`, *frozen until it can run*).
As a new capability, it would wait.

**But the place is not a new capability**: it is the `span` of the `Fix` the
command already prints. Panel 016's item 2 owes the JSON at least what the text
says since `1697cec1`, and the `replacement` precedent (§2) allows the rest of
that same datum, start and end. That part passes with no Part 11 measurement.
**What is a new capability**: `check --apply --json` answering with edits,
since `--apply` prints a program and an answer of edits changes *what*. It
waits. Meanwhile the pair is refused at exit 2 in `build`'s words, which
closes at no cost to the surface the failure panel 016 called the worst
(`016-command-surface.md:94-96`: *`check --json` accepted and ignored, prose
fed to a JSON reader*).

**What the blind measurement must show, registered before any session runs**
(`<scratchpad>/193-compiler-engineer/blind-json/` did not exist at 23:22:20):

1. It **cannot pass or fail Principle 0 for the place**, which panel 016's
   item 2 carries. It can only decide whether **models** are told about places
   (`llms.txt`, the site).
2. Telling models is licensed only if **B leads A by at least 2 of 5 on a case
   where the place cannot be derived**. 178's stripped case is not one (§4);
   a lead there would be a surprise worth reading, not a license.
3. **Every session's file is classed three ways**: equal to `--apply`'s; does
   not check; checks and differs. **One B session in the third class is a
   finding that outranks any pass**: a program different but valid is §1.4's
   failure, and a place that leads a model there must not be advertised to
   models at all.
4. **The scorer's byte equality must not hinge on whitespace the compiler
   leaves.** `sed 's/#.*$//'` on 178's case leaves two trailing spaces on
   lines 13, 19, 24 and 27, and `./heroes check --apply` keeps them
   (`grep -n '[[:space:]]$' out/178-naive-strip.applied`: those four lines),
   with 15 empty lines in the result. A strip that leaves them fails a model
   that tidies whitespace, in both arms and for a reason unrelated to the
   place; the case must be written without them, or the scorer must say before
   it runs how it treats them.
5. **Harm needs its own case.** 178's commas cannot exercise the composition
   hazard. A stripped `certain137-last` with arm B's places is the case where
   a place can do harm. No paid run is needed for the naive tool's half:
   prediction 3, a script over the route's JSON.

## 6. What I would write, priced

- **The spec**: nothing. 0 tokens.
- **The refused alternatives**, appended to a copy of the spec in
  `out/price/` and measured (legacy / cl100k, against 6990 / 7117):
  - *`heroes check --json` writes, on stderr, each fix with the place of the
    text it replaces: file, line and column in characters, start and end.*:
    7026 / 7153, **+36 / +36**;
  - the same, plus *Every place is in the text as checked; `heroes check
    --apply` writes the certain fixes, and two that touch are never written
    together.*: 7055 / 7182, **+65 / +65**;
  - the second under a heading `## 14. The tool`: 7062 / 7190, **+72 / +73**.

  The last two exceed `DELTA_GATE` (50 vendored, `suite_spec.hero:183-184`).
  The reader's count is unmeasured (no `--refresh`). By the document's own
  ratio, 9392 / 7117 = 1.32, they would be about +48, +86 and +96 real. **This
  is an inference, not a count.**
- **`site/src/llms.txt:60-61`**, as the reader gets it after the fill:
  *`heroes check --json` writes schema 1 on stderr, with stable snake_case error
  codes, source locations, notes and suggested fixes; `heroes check --apply`
  writes the `certain` ones into the program.* 1109 / 1108 before, 1128 / 1130
  after, **+19 / +22**. The source holds `{{jsonSchema}}` where the reader gets
  the number. No ceiling judges this file (`heroes measure` says so).
- **The help row**: unchanged, 0; the compiler test holds its number.
- **design.md §4.17**, the schema's one home, a draft whose key names follow
  the sitting's ruling: **138 / 140** (`out/price/design-417-draft.md`;
  design.md is unbudgeted, 87,901 / 90,605 today).

## 7. A stale sentence in this seat's own file

`.claude/agents/spec-warden.md:53` (in the frozen tree) still says *take the
**maximum** as binding*. design.md §1.6 retired that on 2026-09-09: *One
pinned model id binds and moves only by author decision* (`:299-300`, panel
123 R3), and `heroes measure` prints the maximum as *a lower bound, not the
reader's tokeniser*. I followed §1.6. The file's other paragraphs already
carry 10240 and the real instrument, so this is the one sentence left behind.
