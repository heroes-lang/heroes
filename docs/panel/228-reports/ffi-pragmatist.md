# Panel 228, the ffi-pragmatist: a hole's report inside `heroes check --json`, read from C

Started 2026-10-25 02:33 CET (`date`). Written as it goes. Every number and exit code
comes from a command run in this session, named beside it; a sentence that is an inference says so.

**My worktree**: `.claude/worktrees/228-ffi-pragmatist`, detached at
`9a38f1b26923bc241d1d5b211c88da0746117375` (`git rev-parse HEAD`). What I wrote there: the compilers
at its root (`heroes`, `heroes-next`, `heroes-routeA`, `heroes-routeD`); route A's prototype
(`selfhost/check/holes_json.hero` new, `selfhost/cli/check.hero` edited, § 1); and under `fp228/`
(untracked, `git status --porcelain` printing `?? fp228/`) the critic's two probes copied from
`228-critic/critic228/jsonc/` (never built there), the coordinator's hole programs copied from
`rechecked-issues-scratch/hole/`, a copy of cJSON (§ 3b), my programs and my consumers. No paid run;
no credential printed.

**The compiler**: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real 8.12`; stage 2
below.

**json-c**: 0.19 (`brew list --versions json-c`), header `/opt/homebrew/include/json-c/json_object.h`;
clang: Apple clang 21.0.0 (`clang --version`). What the header itself says, read: `json_object_object_get`
is *Deprecated/discouraged: used json_object_object_get_ex instead* and returns NULL *if the field is
not found* (`:423-427`); `json_object_array_length` (`:562-566`) says nothing of NULL;
`json_object_array_get_idx` *with anything other than a json_type_array will trigger an assert*
(`:651-652`); `json_object_get_int` and `_int64`: *null is equivalent to 0 (no error values set)*
(`:745-746`, `:792-793`); `json_object_get_string`: NULL in, NULL out (`:991-992`).
**Stage 2**: `./heroes build selfhost/main.hero -o heroes-next`, `real 88.44 user 66.01 sys 16.02`,
`./heroes-next --version` printing `sources 451802aa66692eb3`, the briefs' key. Every *today* answer
below is this compiler's.

**A slip, said once**: one shell helper of mine wrote a scratch file under `/tmp` and removed it in the
same command (`ls /tmp/x.*` then printed no match). That is outside the repository's root, which the
hard stops forbid; every later scratch file is under `fp228/`.

## 1. What I ran the consumers on

**Programs** (`fp228/hole/`, the coordinator's, copied; `fp228/rich/`, mine): `main.hero` (one hole,
`d: i64 = ???`), `clean.hero` (no hole, two lines I wrote), `both.hero` (`a: i64 = "x"` then a hole);
`rich/main.hero` with `rich/lib.hero`, eight holes: a statement hole in `owed() -> i64`, `b: lib.Box`,
`o: Op` whose field `f` is `(function(a: i64, b: i64) -> i64)`, `xs: [i64]`, `s: str?`,
`sh: lib.Shape` (a variant), `x = ???` used later, `y: i64`, with seven names in scope and eight
functions returning `i64`, two of them documented with backticks and quotes; `rich/untyped.hero`
(`x = ???` then `y: i64 = ???`), `generic.hero` (`print(id(???))`), `fails.hero` (`fail(code: ???,
...)`).

**Today's answers**, `heroes-next check --json <file>`: `main` exit 0, `clean` exit 0, `both` exit 1.
**The holed program's document and the clean one's differ only in the file's name and digest**
(`cat` of both): `"diagnostics": []`, no other member; stdout holds the text report for `main` and
0 bytes for `clean`. So today a reader of the document alone reads a holed file as finished: defect
2158, reproduced.

**Route A's answers are a compiler's, not typed by hand.** In my worktree only I wrote
`selfhost/check/holes_json.hero` (a prototype, never landed: route A's member with the shared brief's
keys, each list at the text's cap, the content `holes.report` carries today, its doc line with `# `
taken off and `""` where a function has none, a case written `.circle` as the text writes it), and in
`selfhost/cli/check.hero` the `--json` document gains `"holes"` after `"diagnostics"` and the text
report is not printed under `--json`. `heroes-next check selfhost/main.hero` exit 0; `heroes-next
build selfhost/main.hero -o heroes-routeA`, `real 41.85`. **Route D's** is the same compiler with
`check_json.SCHEMA` 3, built as `heroes-routeD` (`real 37.07`), the constant restored to 2 after
(`grep -A1 'constant SCHEMA'`). Every answer parses (`python3 -I` `json.load` over the 15 of today and A; D's, by every consumer
that read them without a parse refusal).

What the prototype writes, read off its answers (`fp228/ans/A-*.err`):
- `A-main`: one hole, `"expected": "i64"`, `in_scope` `p`, `q` **and `d`** (defect 2203: `d`'s own
  initialiser), `"functions": [{"call": "dist2(a: Point, b: Point)", "doc": ""}]`;
- `A-rmain`: eight holes; **`"expected": "Box"` and `"expected": "Shape"`, bare** where `main.hero`
  must write `lib.Box` (defect 2205), while `"call": "lib.cube(side: i64)"` beside them is qualified;
  two statement holes with **no `"expected"`** (`owed`'s, with `"returns": "i64"`, and `x = ???`'s,
  defect 2204's fourth shape); `more_in_scope` 2 to 8; the `i64` hole's `functions` five long, `add`,
  `five`, `four`, `one`, `owed`, with nothing saying `six`, `three` and `two` were cut;
- `A-runtyped`: `{"name": "x", "type": "?"}` at both holes, and `y` listed at its own hole (2203);
- **`A-both`, exit 1: no `"holes"` member at all**, the document coming from the diagnostics path:
  the brief's route A writes the member only where every stage was quiet;
- `A-clean`: `"holes": []`.

## 2. The consumers

`fp228/consumers/`, C11 against json-c 0.19's real header, each `clang -std=c11 -Wall -Wextra
-Wpedantic -Wshadow -Wconversion -I/opt/homebrew/include <c>.c -L/opt/homebrew/lib -ljson-c`: **clang
exit 0, zero warnings, on every one** below. Each reads the saved stream (a tool spawning `heroes`
reads the same bytes from a pipe) and answers *finished* (exit 0), *unfinished* with each hole and
what goes there (3), *broken* (1) or *refused* (2); a crash shows as its signal's exit, 139 for
SIGSEGV. `wc -l`: `common.h` 43, `fill.h` 22, `today.c` 16, `a_naive.c` 16, `a_schema.c` 18,
`d.c` 18, `a_lax.c` 20, `a_getex.c` 38.

- `today.c`: today's consumer, schema 2 checked, *finished* when `"diagnostics"` is empty;
- `a_naive.c`: diagnostics first, then the idiomatic `json_object_array_length(json_object_object_get(
  doc, "holes"))`, and per hole `strcmp(expected, "i64")` to offer `0`, the read a filler tool makes;
- `a_schema.c`: the same, written to panel 193's advice, `"schema"` checked first;
- `a_getex.c`: the read the header recommends, every member through `json_object_object_get_ex`, an
  absent or mistyped `"holes"` refused, a hole without `"expected"` told as a statement;
- `a_lax.c`: `get_ex`, an absent `"holes"` read as none, so that an older `heroes` "still works";
- `d.c`: `a_naive.c` refusing any schema but 3.

## 3. Route A against route D, the matrix

`./consumers/<c> ans/<answer>.err` for every consumer and every answer, one loop, the exit code of
each (`fp228/ans/`; the first line of each read too). The right answer is 3 for a holed file, 0 for
a clean one, 1 for `both`; **on an older answer only a refusal (2) is right**, since today's holed
and clean documents differ in nothing a reader can use, so a 0 on today's `clean` is right by chance.

| consumer | today `main` | today `clean` | today `both` | A `main` | A `clean` | A `both` | A `rich` | A `untyped` | D `main` | D `clean` | D `both` | D `rich` |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| `today` | 0 **silent** | 0 | 1 | 0 **silent** | 0 | 1 | 0 **silent** | 0 **silent** | 2 | 2 | 2 | 2 |
| `a_naive` | **139** | **139** | 1 | 3 | 0 | 1 | **139** | **139** | 3 | 0 | 1 | **139** |
| `a_schema` | **139** | **139** | 1 | 3 | 0 | 1 | **139** | **139** | 2 | 2 | 2 | 2 |
| `a_getex` | 2 loud | 2 loud | 1 | 3 | 0 | 1 | 3 | 3 | 2 | 2 | 2 | 2 |
| `a_lax` | 0 **silent** | 0 (chance) | 1 | 3 | 0 | 1 | 3 | 3 | 3 | 0 | 1 | 3 |
| `d` | 2 loud | 2 loud | 2 loud | 2 loud | 2 | 2 | 2 | 2 | 3 | 0 | 1 | **139** |

**What it says.**
1. **The critic's probe reproduces**: `probe2.c` copied, built with the critic's flags (clang exit 0,
   one warning, the unused `argv`), exit **139** on today's answer, `array_length -> 1` exit 0 on
   route A's.
2. **Panel 193's advice does not make a consumer safe under A.** `a_schema` checks the number and
   crashes exactly where `a_naive` does, on every older answer **including a clean file's**, because
   under A the number does not move: the schema check is the one line that cannot see the change.
   Under D the same consumer, unchanged, refuses loudly (exit 2).
3. **Under A only `a_getex` is safe on an older answer**, and `a_lax`, which uses the same safe call,
   reads a holed file as finished at exit 0: `get_ex` removes the crash, not the ambiguity. What an
   absent member means is a rule the consumer must know; under A it means *an older `heroes`*, but
   also, on the brief's own route A, *a file with an error* (`A-both`), so even `a_getex` has to read
   `"diagnostics"` before `"holes"` to tell the two apart.
4. **D does not save a consumer from the shape of a hole.** `d` reads D's one-hole answer right and
   crashes on D's eight-hole answer, at the statement hole that carries no `"expected"`: json-c gives
   NULL and `strcmp(NULL, "i64")` faults. `a_naive` does the same on A's. A number refuses an old
   answer; it cannot refuse a member that a new answer leaves out by design. **This is the strongest
   C reason against the member as drafted, A or D: a key that is present on one hole and absent on
   the next is the json-c idiom's crash, on the newest compiler.**
5. **D's price, measured**: `today.c`, which reads today's answers right on a clean file and a broken
   one, refuses every D answer (exit 2), clean ones included; and `heroes-routeD check --apply --json`
   writes `"schema": 3` on the `"kind": "applied"` document (`head -3`), whose content did not change,
   because one constant numbers both kinds (`check_json.hero:38`, `:122`). In the tree, four pins
   read the number (`grep -n schema` over the readers the brief names): `tests/harness/apply_tree.hero:91`
   and `suite_surface.hero:684` on the applied document, `suite_surface.hero:520` on a diagnostics
   document, and `suite_surface.hero:617` on a clean one byte for byte (`err_is`), which route A's
   `"holes": []` moves too. A loud refusal is the
   failure panel 193 preferred, and its cost lands on readers who never asked for holes.

**How many consumers would be written the safe way (an inference, not a measurement).** The header
itself marks `json_object_object_get` *Deprecated/discouraged* and points to `get_ex`
(`json_object.h:423`); `json_object_array_length` documents nothing for NULL (`:562-566`). Panel 193
measured the idiomatic read and found it reads absent as 0; the critic and I find it reads absent as a
crash. My expectation, unmeasured: most consumers written quickly against json-c use
`json_object_object_get` (it is one call, returns the value, and is what a reader copies from an
example), and of those that use `get_ex` a share will default an absent array to empty, as `a_lax`
does, which is wrong in silence. So under A I expect most hand-written C consumers to be either
`a_naive` (crash on an older `heroes`) or `a_lax` (silent), and a minority `a_getex`. **One fact under it is measured**: json-c's own README
(`/opt/homebrew/Cellar/json-c/0.19/README.md:397-398`) names `json_object_object_get()` as *the* way
to get a child, and names `json_object_object_get_ex` zero times (`grep -c`): the documentation
teaches the read that crashes.

### 3b. The same question through cJSON, the library design.md names

design.md §1.11's table (`docs/design.md:551`) names **cJSON**, not json-c, as what a program binds
for JSON in C; panel 193 and the critic measured json-c. cJSON is not installed here (`brew list
--versions cjson` prints nothing); a copy, **cJSON 1.7.12** (`cJSON.h:81-83`), stands in a Dart
package cache, `~/.pub-cache/hosted/pub.dev/ffigen-16.1.0/third_party/cjson_library/` (`mdfind -name
cJSON.h`), read there and copied with its `license.txt` into `fp228/cjson/` (nothing written outside
the repository; `shasum -a 256 cJSON.c` `06885ac9...`). Its source answers what json-c's header did
not: `cJSON_GetArraySize(NULL)` **returns 0** (`cJSON.c:1741-1744`), `cJSON_ArrayForEach` over NULL
runs no iteration (`cJSON.h:282`), `cJSON_GetStringValue` of a non-string is NULL (`cJSON.c:82-87`),
and `cJSON_Parse` is `cJSON_ParseWithOpts(value, 0, 0)`, the end of the document not required to be
the end of the input (`cJSON.c:1096-1098`).

The twins (`cj_naive.c`, `cj_schema.c`, `cj_d.c`, `cj_count.c`, `cj_applied.c`, the same logic in
cJSON's calls, `cJSON_GetObjectItemCaseSensitive`, `cJSON_GetArraySize`, `cJSON_ArrayForEach`,
`cJSON_GetStringValue`; `cJSON_IsNumber` before the schema and the count, a hole's line and column
read by `->valueint` directly): `clang -std=c11 -O1 -c
cJSON.c` 0 warnings, each consumer `clang -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion`
exit 0, **0 warnings**. Exit codes, one loop:

| consumer | today `main` | today `clean` | today `both` | A `main` | A `clean` | A `both` | A `rich` | A `untyped` | D `main` | D `clean` | D `rich` | S `rich` |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| `cj_naive` | 0 **silent** | 0 | 1 | 3 | 0 | 1 | **139** | **139** | 3 | 0 | **139** | 3 |
| `cj_schema` | 0 **silent** | 0 | 1 | 3 | 0 | 1 | **139** | **139** | 2 | 2 | 2 | 3 |
| `cj_d` | 2 loud | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 3 | 0 | **139** | 2 |

(`S rich` is defined in § 7.) **So under route A the idiomatic read of an older answer fails with
both libraries, in opposite directions: json-c crashes (139), cJSON reads the holed file as finished
at exit 0, in silence**, and the schema check of panel 193 changes neither, since the number does not
move. With cJSON the critic's *crash* is the better of the two outcomes the route leaves: the library
this project tells its programmers to bind turns route A's version gap into a silent wrong answer.
Only under D does either consumer refuse (`d`, `cj_d`, exit 2). And both libraries crash at a
statement hole's missing `"expected"` (`strcmp` of NULL), D or not.

## 4. Route B: a consumer that reads stdout as one document

B's streams are composed from the compilers' own bytes (`fp228/ans/B-*.out`, a Python `-I` script):
B's holes document is route A's real document with `"kind": "holes"` and its `"diagnostics"` line
removed; under `--apply --json`, today's real `"kind": "applied"` stdout followed by it; under
`--json --dump-scopes`, today's real scope dump (ending `holes in `main`: unused bindings are not
reported there`) followed by it. Python's `json.loads`: `B-main` one document; `B-apply` **refused**,
*Extra data: line 12 column 1 (char 489)*; `B-dump` **refused**, *Expecting value: char 0*.

json-c says what it does on its own: by default it allows *parsing of multiple json objects in a
single character stream*, which only `JSON_TOKENER_STRICT` disables (`json_tokener.h:150-153`).
Measured, each consumer 0 warnings:

| consumer | today `--apply --json` | B `--apply --json` | B `--json` | B `--json --dump-scopes` | today `--json` (text) |
|---|---|---|---|---|---|
| `b_applied` (json-c default, today's reader of the applied document) | 0 right | **0, the holes document dropped in silence** | 2 | 2 | 2 |
| `b_applied_strict` (`JSON_TOKENER_STRICT`, whole input) | 0 right | **2 loud**, *unexpected character at byte 489* | 2 | 2 | 2 |
| `cj_applied` (cJSON, `cJSON_Parse`) | 0 right | **0, dropped in silence** | | 2 | |
| `b_holes` (B's reader: stderr for diagnostics, stdout for holes) | | 2 (stderr is empty under `--apply --json`) | 3 right | **2 loud**, no document | 2 loud |

So under B, on a holed file: **a reader of `--apply --json` using json-c's or cJSON's default parse
(`b_applied`, `cj_applied`) keeps reading the applied document and drops the holes in silence; a
strict reader (Python's `json.loads`, json-c with `JSON_TOKENER_STRICT`) breaks loudly on a stream it
reads right today; and `--json --dump-scopes` is readable by neither.** B's one merit from C:
today's stdout is text or empty, never JSON, so `b_holes` refuses every older answer loudly (exit 2
on today's `main` and `clean`). And a reader of stderr alone, `today.c`, still reads a holed file as
finished: defect 2158 stands under B as filed.

## 5. Route C: a consumer that reads `"diagnostics"` to decide broken or not

C's documents are composed from today's real text report (`fp228/ans/C-*.err`): each hole one entry,
`"code": "hole"`, its report's lines as `"notes"` (my choice of note form, the brief giving none),
fed to the consumers as if `check` had exited 0, and as if it had exited 1. Each consumer 0 warnings:

| consumer | today `main` | C `main` at exit 0 | C `main` at exit 1 | C `rich` at exit 0 | C `rich` at exit 1 |
|---|---|---|---|---|---|
| `c_exit` (the exit code first, as panel 193's consumer was told) | 0 **silent** | 0 *finished*: **silent** | 1 *broken: hole* | 0 **silent** | 1 *broken: hole* ×8 |
| `today` (diagnostics non-empty is broken) | 0 **silent** | 1 *broken*, against exit 0 | 1 *broken* | 1 | 1 |
| `c_aware` (written for C: `hole` apart, the type read out of the notes) | 0 **silent** | 3 right | 3 right | 3, the statement hole *(not said)* | 3 |

C at exit 0 makes the exit code and the document disagree, and the consumer that trusts the exit code
reads a holed file as finished; C at exit 1 makes every existing consumer call a program the spec
calls valid broken, and the thesis grader (`tests/harness/thesis_grade.hero:151-161`, read, not run)
grade it `compile_error`. What goes at the hole is prose in `"notes"`: `c_aware` finds the type by
the prefix `expected type: ` and has nothing for a statement hole. And on an older answer `c_aware`
reads a holed file as finished, with no number to refuse by.

## 6. Route E: a count, then the text

E's documents (`fp228/ans/E-*.err`) are today's with `"holes": <n>` after `"diagnostics"`, n counted
from today's text; stdout is today's text. `e_text.c`, 51 non-blank non-comment lines (`grep -v`), reads the
count with `json_object_get_int(json_object_object_get(doc, "holes"))` and then parses the report
line by line, by the prefixes the report prints; `a_full.c`, 44 lines, reads route A's member whole
with `get_ex`. Both print one normalized form, and `diff` judges.
- On `main` the two agree (`diff` exit 0).
- On `rich`, **`e_text` invents a field**: `fields of Op: f: (function(a: i64, b: i64) -> i64), name: str`
  is split at its commas, so it prints `field f:(function(a: i64` and `field b:i64) -> i64)`, a field
  `b` the record does not have, at exit 3, in silence. The text's separator `, ` also stands inside a
  type, and its markers are prose: `… and N more` is U+2026 (the parser matches `\xe2\x80\xa6`),
  `position: statement — any type, or none` an em dash, a doc line `      # `; none is versioned, so
  a rewording of the report, which defects 2203 to 2205's repairs may make, breaks the parser
  silently.
- On an **older answer** `e_text` and `cj_count` print *finished* at exit 0 for `main`: an absent
  count is 0, as json-c's header promises (*null is equivalent to 0*). That is panel 193's own
  measurement, `start 0 end 0` for every fix, in a new member.

So a count alone is what a consumer needs to say *unfinished*, and only if it can tell an absent
count from zero; to say *what goes there* it must parse unversioned prose. The cost is not lines
(51 against 44) but a class of silent misreadings no line count shows.

## 7. The types, and the shape of a hole object

**What the documents hold.** `types.c` reads every type string from route A's answers (`A-main`,
`A-rmain`, `A-runtyped`, and three programs of `fp228/types/` made to reach `render_ty`'s other
branches) and asks two things of each. Does it parse as spec § 3's `Type`? A 39-line
recursive-descent recognizer of the production, non-blank lines by `awk`. And what does a classifier
reading its last byte and its `.` take it for? 13 distinct strings:

| string | where | a `Type` | the file can write it at the hole |
|---|---|---|---|
| `i64`, `Point`, `Op`, `[i64]`, `str?`, `str`, `(function(a: i64, b: i64) -> i64)` | expected, in scope, field | yes | yes |
| `Box`, `Shape` | expected, from `lib` | yes | **no**: `lib.Box`, `lib.Shape` (defect 2205) |
| `Shape.circle` | in scope, a case's payload `c` | **yes, as module `Shape`'s type `circle`** | no: a case, no type |
| `?` | in scope, `x = ???` | **no**, and its last byte says *fallible* | no |
| `a type parameter` | **expected** and in scope, `return ???` in `id<A>` | no, prose | no: `A` |
| `` the error (`code`, `msg`) `` | in scope, an `.err e` payload | no, prose | no |

Beside them, `lib.Box`, `[lib.Box]?`, `{str: i64}`, `(function(i64) -> str?)`, `()` and `_` all
parse (`_` too, so it would be a false sentinel), and `""` does not (`X-types.err`, a document I
wrote).

**What a consumer does with one.** `fill_record.c` takes the first hole of `A-rmain` whose fields are
all `i64` and writes `<expected>(<field>: 0, ...)` in place of its `???`: it wrote `Box(width: 0,
height: 0, depth: 0)` at 39:18, and `heroes-next check --brief` refused it, `error[needs_qualifying]:
`Box` is declared in module `lib`...`, exit 1; the same file with `lib.Box` checks at exit 0. And a
filler offering the in-scope name of the expected type at `A-main`'s hole writes `d: i64 = d`,
refused `unknown_name`, exit 1 (defect 2203); `dist2(a: p, b: q)`, from `"functions"`, exit 0.

**String or structure.** For *what goes there*, a string is what a C consumer uses: it compares it
(`strcmp` against what it knows), prints it, and pastes it, and pasting is exactly what a structure
would make it rebuild. A consumer that needs the type's structure (the element of `[T]`, whether
it is fallible) pays 39 lines of C to recover it from a string **that is a Type**; no string makes
that safe when it is prose or `?`. So, from C: **a string, as the file writes the type at that hole
(defect 2205's spelling), and every string in the document either such a type or one fixed value
that cannot be one.** That value should be `""`, not `?` (a fallible suffix to a byte classifier,
and to a reader of spec § 6), not `null` and not an absent key: json-c and cJSON both hand back NULL
for either, and NULL into `strcmp` is the crash measured above. `""` parses as no Type, ends in no
`?`, and is safe in every libc call. An inference past the measurement: the prose renderings
(`a type parameter`, the error's, a case's) are that value too, or the type the file writes (`A`).

**The shape of a hole object, measured.** In `S-rmain` and `S-runtyped`, route A's own answers with
`"expected": ""` added to each statement hole, and in `S-both`, `A-both` with `"holes": []` added
(each an edit of mine, `python3 -I`, every result parsed), the consumers that crashed read right:
`a_naive` 3, 3 and 1; `a_schema` 3; `d` on `SD-rmain` 3; `cj_naive` and `cj_schema` 3. One price
shows: `a_getex` then says `write a ` for a statement hole, so `"position"` must stand on every hole
too, `"value"` or `"statement"`, for a consumer to tell them apart without asking whether a key
exists. **A member that comes and goes is the json-c and cJSON idiom's crash; every hole carrying
every key, a string never absent, is what makes the idiomatic read safe on the newest answer.**

## 8. Route A'

Under A' the stderr document is A's byte for byte, so every consumer above reads it as it reads A
(an inference from the consumers' code, which reads only the file given for stderr: not rerun).
Keeping the text on stdout costs a C consumer nothing and avoids B's stream composition; it leaves
`--json --dump-scopes`'s stdout as today. From C, A' carries A's objections and none of its own.

## 9. The questions inside every route, from C

1. **Holes beside errors.** As the brief drafts route A, `A-both`'s document (exit 1) has **no
   `"holes"` member**; my first consumers, which read `"holes"` before `"diagnostics"`, exited 139
   on it (the first run's matrix). From C the member stands in **every** `"diagnostics"` document at
   every exit, and a reader is told in one place what it means when another stage spoke.
2. **The cap.** `A-rmain`'s `i64` hole lists 5 functions of the 8 that return `i64` (`grep -c -- '->
   i64$' rich/main.hero`: 8; the five are `add`, `five`, `four`, `one`, `owed`) and **no member
   says three were cut**, while `in_scope` says `more_in_scope`. A C consumer cannot tell *five* from
   *five of eight*. From C: a count beside every capped list, or every item; never a silent cut.
3. **The types**: § 7.
4. **`--apply --json`.** Today it writes 0 bytes to stderr (`wc -c`) and the `applied` document on
   stdout with no hole; `b_applied` and `cj_applied` read it as *0 rounds* and nothing more. If
   `"applied"` carries `"holes"` (with § 7's shape), both kinds change, and route D's one number
   (`check_json.hero:38`, `:122`, measured moving both) moves for a reason on both.
5. **The order of landing.** Measured: a machine filling from today's content writes `Box(...)`
   (refused, 2205) and `d` (refused, 2203). A document is what a tool trusts *without reading*; the
   text is what a person or a model reads and can doubt. From C the member waits for 2203, 2204 and
   2205, or it hands a filler programs the compiler refuses.
6. **The cap's sentence**: not a C question; item 2 above is what a consumer needs either way.

## 10. The twelve golden programs that hold `???`

`grep -rl -F '???' tests/golden --include='*.hero' | grep -v recovery` prints twelve, as the brief
says. Each was checked from its own directory by today's compiler, route A's and route D's
(`fp228/golden/`), and each answer read by the consumers. **7 exit 1** (six `check` cases and
`surface-fixtures/certain137-types`), so the drafted member is absent and their holes are told
nowhere; every diagnostics-first consumer says *broken*, exit 1. **2 hold `???` only in a string or
a comment** (`run/fixedbugs-207...`'s `print("what???)")`, `unsupported/fixedbugs-a-unit-parameter...`'s
comment): 0 holes, exit 0. **3 hold a hole at exit 0**: `ir/fixedbugs-231-a-hole-leaves-its-function-unmerged`
(`_ = ???`, line 19, a **statement** hole with `"returns": "i64"`), `surface-fixtures/apply141/holes`
and `surface-fixtures/holes/geom`, one value hole each.
- On **route A's and route D's own answers**, `fixedbugs-231` makes **every idiomatic consumer exit
  139**: `a_naive`, `a_schema`, `cj_naive`, `cj_schema`, `d`, `cj_d`; only `a_getex` reads it (3).
  The other two read right everywhere (3).
- On **today's answers** of the five that exit 0, `a_naive` (json-c) exits **139 on all five**, clean
  ones included, and `cj_naive` (cJSON) exits **0 on all five**, three of them holed.

So one in three of the tree's own holed programs that check at exit 0 crashes every idiomatic reader
of the member as drafted, under either number.

## 11. Verdict

Written 2026-10-25, `date` reading 02:53 CET as this section began. Each route in the seat's fields; no route
reaches a veto: none changes how a Heroes value crosses the C boundary, none touches a binding or
the header check that makes a wrong signature a compile error. This is a tool's answer that C
programs read, as panel 193's was, and the cost is measured in those programs.

- **verdict**, per route:
  - **A**: **object**. An absent member under an unchanged number fails every idiomatic reader of
    an older `heroes`, json-c by crashing (139, clean files too), cJSON by reading a holed file as
    finished (0); checking `"schema"` first changes neither (§ 3, § 3b).
  - **A'**: **object**, on A's grounds; keeping the text on stdout costs C nothing and avoids B's
    composition (§ 8).
  - **B**: **object**. Two documents, or text then a document, on one stdout: json-c's and cJSON's
    default parsers read the first and drop the holes in silence, strict readers break on a stream
    they read right today, and `--dump-scopes` is nobody's; a stderr reader still sees no hole (§ 4).
  - **C**: **object**. At exit 0 the code and the document disagree, and the exit-first reader says
    *finished*; at exit 1 every existing reader calls a valid program broken and the grader says
    `compile_error`; the content is prose in notes (§ 5).
  - **D**: **object as drafted, and the route I approve under the condition below.** Its number is
    the only thing that lets an idiomatic reader of either library refuse an older answer (`d`,
    `cj_d`, exit 2), but A's member under it still crashes both on D's own answers, at a statement
    hole that carries no `"expected"` (§ 3, § 10); and one constant moves the `applied` document
    too, which is right only if that document gains `"holes"` (§ 9.4).
  - **E**: **object** as the answer to feature 2091: the count reads absent as 0 in both libraries
    (silent *finished*), and *what goes there* goes through unversioned prose that invents a field
    (§ 6). As the answer to defect 2158 alone, a count is enough under a moved number.
- **section**: design.md **§1.11** (`docs/design.md:557`, *FFI ergonomics rank alongside
  comprehension ... When a choice makes bindings harder, that is a serious cost*; its table, `:551`,
  naming cJSON for JSON in C) and **§4.17**'s paragraph on the document (`:2313-2339`), which fixes
  `"schema": 2` and its members. **design.md does not cover my central objection**: it states no rule
  for when the number moves, nor what an absent member means (`grep -n -i schema docs/design.md`:
  one line, `:2316`). §1.12's *must not segfault* binds Heroes programs, not the C programs that read
  `heroes`' answers; I cite it for its sentence *a guarantee that ends quietly is not one* (`:594`),
  not as a rule that reaches them.
- **experiment**: 20 C11 consumers (`fp228/consumers/*.c`, counted by `ls | wc -l`), 15 against **json-c
  0.19**'s real header and 5 against **cJSON 1.7.12**'s, plus the critic's `probe2.c`. **clang accepted every one**, `-Wall
  -Wextra -Wpedantic -Wshadow -Wconversion`, 0 warnings (the probe, with the critic's flags, its one
  unused `argv`). They ran on documents two compilers wrote (route A's prototype and route D's, in my
  worktree only), on today's, on B, C and E composed from those bytes, on four shape edits and one
  document of type strings I wrote, and on
  the twelve golden holed programs; every matrix re-run with the final binaries after the last edit,
  every cell as written.
- **argument** (below, `wc -w`).
- **prediction** (below).
- **condition** (below).

**argument**:

> A member added under an unchanged number fails every idiomatic C reader of an older `heroes`:
> json-c 0.19 crashes (139, clean files included), cJSON 1.7.12, the library §1.11 names, reads
> holed files as finished at exit 0, and checking `"schema"` first, panel 193's advice, changes
> neither. Under D both refuse. But D as drafted still crashes both on its own answer: a statement
> hole omits `"expected"`, and NULL reaches `strcmp`, on golden `fixedbugs-231`. B's second document
> is dropped in silence by both default parsers; C makes exit 0 lie or calls valid programs broken;
> E reads an absent count as 0 and routes content through prose that invents a field. Today's types
> make a filler write refused programs.

**prediction**: on route D landed with the condition below, a json-c 0.19 consumer and a cJSON 1.7.12
consumer, each in its library's idiom (`json_object_object_get` and `cJSON_GetObjectItemCaseSensitive`,
no presence test on any member), refusing every schema but 3 and reading `"diagnostics"` before
`"holes"`, **exit with no signal on every answer of the twelve golden programs holding `???`**: 1 on
the seven with diagnostics, 3 on the three holed (`fixedbugs-231`'s statement hole included), 0 on the
two whose `???` is text; and **refuse at exit 2, with no signal, every answer of a `heroes` built at
`9a38f1b26`** on the same twelve. One exit of 139, or one 0 on a holed program, falsifies it.

**condition** (what turns D, and only D, to **approve**):
1. `"holes"` stands in **every** `"diagnostics"` document at every exit, and in the `"applied"`
   document, so one number moves both kinds for a reason;
2. **every hole carries every key, and no string is ever absent or `null`**: `"position"` always
   (`"value"` or `"statement"`), `"expected"` and `"returns"` always strings, `""` where there is
   none;
3. a **count beside every capped list** (`functions` as `in_scope` already has), never a silent cut;
4. **every type string is a Type the file writes at that hole** (defect 2205's spelling), or `""`:
   never `?`, never prose, never a case's name;
5. **the landing waits for defects 2203, 2204 and 2205**, or a machine fills `d: i64 = d` and
   `Box(...)`, both refused (§ 7);
6. the rule written in **design.md §4.17's paragraph**: the number moves when a member a reader may
   need is added, and what an absent member means.

**Conservative, for the author to choose** (CLAUDE.md § 4): A with conditions 1 to 5 and a §4.17
sentence telling every reader to test presence and refuse an absent `"holes"`. Measured cost: only a
reader written as `a_getex` is safe on an older `heroes`; the idiomatic reader of json-c crashes, and
of cJSON says *finished*.

**What would change my verdict on A**: a measurement that no C reader of `check --json` exists or
will run against a `heroes` older than the landing, which the project's own record of a stale binary
costing 31 minutes (`.claude/rules/verification.md` § The compiler that judges is a build artifact)
contradicts.
