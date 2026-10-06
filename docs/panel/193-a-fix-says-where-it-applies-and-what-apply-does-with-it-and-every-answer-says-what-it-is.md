# Panel 193: a fix says where it applies and what `--apply` does with it, and every answer says what it is

Sat 2026-10-05 from 22:40 to 2026-10-06 02:34, on the seats' archive of
`00217c39`, convened by the author's yes of 2026-10-05 (CLAUDE.md § 4, a tool
surface). The seats:
- `compiler-engineer`, `ffi-pragmatist`, `spec-warden` and `historian`, each in
  its own copy, their reports in `193-reports/`;
- `llm-ergonomist`: **not run**, the coordinator's ruling below;
- the completeness critic, twice: over the briefs before any seat
  (`193-reports/completeness-critic-briefs.md`, which repaired them), and over
  the reports (`193-reports/completeness-critic.md`).

The account's session limit stopped three seats (the compiler-engineer, the
ffi-pragmatist, the historian) before 01:30, and its weekly limit the
compiler-engineer again before 02:01; each resumed from its own report file,
at 01:30 and, on a new account, at 02:01, the two times read by the clock.

## The proposal

Defect 271: a fix that applies away from its message's line says it neither
in the message nor in `heroes check --json`. Its rendered half landed on lane
b12-str192 at `1697cec1` (a fix's line gains ` (at file:line:col)` where it
stands off its message's line). **The sitting rules the JSON half**: what
`check --json` writes for a fix, and, after the critic's first pass, what
`check --apply --json` answers (defect 371: `--apply` ignores `--json` in
silence at exit 0). The proposal as briefed: *each object of a diagnostic's
`"fixes"` gains the place of the text it replaces, so a tool can apply a fix
without re-deriving it.*

## The verdict table

| seat | verdict | section | cost | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object** | §4.17, §1.7, §1.1 | +105 lines, all in `selfhost/cli/` | at most 130 lines at the landing, all in `cli/` | places alone only if §4.17 says one answer promises one fix at a time and `--apply --json` is refused |
| ffi-pragmatist | **object** | §1.11, §4.17 | a C11 consumer of 528 lines | a json-c consumer of schema 2 equals `--apply` on 537 roots | per-file byte offsets, characters for display, `text`, a first-round outcome, `"schema": 2`, the terms in one home |
| spec-warden | **object** | §1.6, §1.2, §4.17 | 0 spec tokens | the spec stays at 6990 / 7117 | places only with what `--apply`'s first round does; `--apply --json` refused; the schema held in two places |
| historian | **approve**, advisory | the precedents | none | byte offsets plus characters match every applier read | changes if a one-shot applier reproduces a multi-round one |

## What the sitting measured

- **A place alone does not make an answer's fixes writable together.** Every
  `certain` fix of one answer written at its place equals `check --apply` on
  754 of 757 roots; on the three `certain137-*` roots it writes
  `print(total(xs.must())ad_operand`, defect 137's corruption, and on `applyx`
  it writes a module `--apply` never writes (the compiler-engineer; the critic
  reproduced). A C consumer reaches 757 only by re-deriving `certain.chosen`
  in 47 lines (the ffi-pragmatist).
- **With each certain fix's first-round outcome** (`written`, `withheld`,
  `twin`, `elsewhere`, `malformed`, computed by `certain.hero` itself), writing
  only the `written` fixes equals a one-round `--apply` on 757 of 757 and the
  full `--apply` on 754; asking again after any answer that wrote a fix reaches
  it on 757 (the compiler-engineer and the critic). *Asking while any fix is
  `withheld`* does not: on `certain137-inside` the second answer holds none and
  a third round still writes one (the critic, falsifying the ffi-pragmatist's
  rule).
- **The unit**: the diagnostic's `col` counts characters (14 on a line holding
  `è€😀`, where bytes give 20 and UTF-16 15); a consumer applying by the column
  read as UTF-16, LSP's default, deleted a closing quote instead of a comma, and
  read as bytes cut a byte-order mark (the ffi-pragmatist). Every applier the
  historian read applies by byte offsets into the file as stored and keeps line
  and column beside (rustfix, Go, clang's `Replacement`); every recorded
  failure sits at a conversion, columns in another unit (clangd 2018, lsp-mode
  2020, llvm-cov 2023, vim-lsp 2026) or offsets from a transformed text (rustc
  2019, Motoko 2026).
- **Offsets index one text of every file**, concatenated
  (`source.hero:14-24`); the route converts them to each file's own bytes.
  Over 137 cases made CRLF, the `written` fixes applied by byte offset equal
  `--apply` on 137 (the critic, scoring the historian's prediction).
- **Two documents would say `"schema": 1`**: the route's `--apply --json`
  (ESLint's shape, `{schema, file, rounds, not_applied, output}`) and the
  diagnostics answer. A 19-line json-c consumer that checks `"schema": 1`
  crashes at exit 139 on the first where today it refuses loudly; and a clean
  program's answer is **0 bytes**, no document, which every JSON parser refuses
  at its first character (three seats and the critic).
- **The replaced `text` guards a fix's own bytes, not the program**: an
  imported module edited after the check passes the guard and the applied file
  fails to compile (the critic).
- **Cost**: +105 lines in `cli/`; `check --json` +20.7% to +21.9% bytes over the
  510 check cases, 44% of it the per-fix `file`, which differs from its
  diagnostic's on 0 of 2,488 objects today; instructions +2.5% to +4.5% on the
  fix-heavy cases, and **+15.6% on one 14,039-byte line with 2,000 fixes**
  (148.6 to 171.8 billion), the places computed by two `locate` calls a fix.
  The base itself spends 148.5 billion on that file (defect 372, below).
- **Nothing else moves**: with the added keys removed the answer equals today's
  on 761 of 761 roots; own tests 1,235, `surface` 355, `fixes` 769, `layout` 5,
  `check` 511, `permissive` 6, `full` 7, each 0 failed, on the route's compiler
  (the critic reproduced the compiler-engineer's).
- **The specification moves 0 tokens**; design.md says nothing of `--json` nor
  of how one answer's fixes combine (the spec-warden). The published promises
  are `heroes --help` (*schema 1*), `site/src/llms.txt:60` and the errors page,
  held by no instrument.

## Disagreements, unsmoothed

1. **`"schema"`, 1 or 2.** The historian: additive fields keep 1, as `cargo
   metadata` does. The ffi-pragmatist: 2, so a consumer can refuse an answer
   with no places by its number. The compiler-engineer: 2 is robust, 1
   conservative. The critic: neither holds while two documents share 1 and a
   clean answer carries no number.
2. **`check --apply --json`: answer or refuse.** The spec-warden refused, at
   exit 2, until a consumer needs its edits, arguing against an answer of
   edits; the compiler-engineer built ESLint's shape, which answers the program
   `--apply` writes and not edits. The two never met (the critic).
3. **The per-fix `file`, always or only where it differs**: it differs on 0 of
   2,488 objects and costs 44% of the bytes added.
4. **Guesses**: every guess (881 objects) gets a byte place that looks as
   applicable as a certain fix's; no seat ruled (the critic).

## The resolution: `provisional — author ratification pending`

The most robust and complete, CLAUDE.md § 4; each conservative alternative is
named where it stands.

**R1. Every fix carries its place**: `file`, `line`, `col`, `end_line`,
`end_col` (lines from 1 and split at `\n` only, columns from 1 in characters,
the diagnostic's own unit), `byte_start` and `byte_end` (that file's own bytes
as stored, from 0, the end excluded), and `text`, the bytes it replaces. **The
`file` on every fix**, never inferred from its diagnostic: the bytes are the
price, and a consumer that infers is the failure the historian's records show.
*Conservative*: `file` only where it differs.

**R2. Every certain fix says what `--apply`'s first round does with it**:
`first_round` is `written`, `withheld`, `twin`, `elsewhere` or `malformed`,
computed by `cli/certain.hero`'s own ordering and `chosen`, never restated. A
guess carries no `first_round`, its place is for showing, and the answer says
so: **a tool writes a `certain` fix marked `written` and nothing else**, and
asks again after any answer that wrote one, until an answer writes none.
*Conservative*: places alone, with §4.17 saying one answer promises one fix at
a time.

**R3. Every answer says what it is, and a clean program gets one**: each
document opens `{"schema": 2, "kind": ...}`, `"diagnostics"` for `check
--json`, `"applied"` for `check --apply --json`; a clean program's answer is
`{"schema": 2, "kind": "diagnostics", "diagnostics": []}`, where it was 0
bytes. *Conservative*: schema 1 and the keys added under it, as `cargo
metadata` adds them, which leaves the 0-byte answer and two documents sharing
one number.

**R4. `check --apply --json` answers the applied document** (closing defect
371): `{schema, kind: "applied", file, rounds, not_applied, output}`, `output`
the program `--apply` writes, so it agrees with `--apply` by construction (757
of 757). *Conservative*: refused at exit 2, the spec-warden's.

**R5. Every answer names the files it read and their digests**, so a tool can
refuse a tree that moved since the check; the per-fix `text` stays, since it
names the bytes a fix replaces. *Conservative*: `text` alone.

**R6. The places are computed in one forward walk per file**, defect 256's
technique, never two `locate` calls a fix; the cost on many fixes over one long
line is the measured price this removes.

**R7. The terms have one home, design.md §4.17**: the units, the end excluded,
lines split at `\n`, every place in the text as checked, the outcomes and the
rule of R2. The specification moves 0 tokens.

**R8. The promises are held to the writer**: `heroes --help`, `llms.txt` and
the errors page say schema 2 and that the answer is written to stderr, each
held by an instrument (a compiler test for the help; the site's claims check or
a records check for the pages, the landing's to choose); **no page tells a model
to apply places**, since no model was measured (the spec-warden and the
critic). `suite_surface.hero:286`'s pin is anchored on the diagnostic's own
row.

**R9. The blind seat was not run.** The coordinator ruled between 01:30 and
02:01 that its
experiment on 178's case could not separate the arms (ten line-ending commas
under a message that states the rule; the spec-warden predicted arm A at 4 of
5) and that two seats measured the real consumer. What it loses (the critic):
the only measurement of a model, a case that may separate
(`fixedbugs-130-commas-in-braces-are-told-as-without-them`, an inference),
whether a model respects `withheld` on `certain137-last`, and whether it
applies a guess with a place; about 2.2 USD a case under the brief's cap, the
author's to fund.

**R10. Filed with the sitting, into batch 12 by the author's rule of
2026-10-05**: defect 372, the base's `check --brief` spending 148.5 billion
instructions on one 14,039-byte line with 2,000 diagnostics, 25.1 billion at
500 on a 3,508-byte line; defect 373, a CRLF file moving a diagnostic's column
at a line's end by one, 12 of 137 cases. Both reproduced by the coordinator at
02:35 on the base's compiler.

**R11. The landing**: batch 12, one lane holding `selfhost/cli/check_json.hero`,
`check.hero`, `certain.hero`, a new module for the applied document, the
places' walk, `selfhost/cli/table.hero`'s help, design.md §4.17, `llms.txt` and
the errors page, and its cases; it closes 271, 371, 372 and 373.

## Predictions to score

- **compiler-engineer**: the route adds at most 130 lines in the layout unit,
  all under `cli/`. R3 to R6 widen the route, so the landing scores it on the
  route as briefed and reports the rest apart.
- **spec-warden**: the spec stays at 6990 / 7117 tokens; writing every certain
  fix at its place disagrees with `--apply` on at least the three
  `certain137-*` roots (**true**, measured by the compiler-engineer and the
  critic); arm A at least 4 of 5 (**unrun**, R9).
- **ffi-pragmatist**: a json-c consumer of schema 2 that writes `written` fixes
  by byte offsets and asks again equals `--apply` on 537 roots. Its *ask while
  any is withheld* is falsified by `certain137-inside` (the critic); scored at
  the landing with R2's rule.
- **historian**: per-file byte offsets applied once equal `--apply` on the 139
  answer files and their CRLF forms but the three `certain137-*`: **true** on
  137 of 137 CRLF cases (the critic).

## The critic's passes

The first pass repaired the briefs: the answer is on stderr, 178's case holds
ten fixes, `--apply` runs up to 64 rounds and is not `check.hero:71`, offsets
index one concatenated text, the rendered half had landed, and the blind case
gave its answer away; it found defect 371. The second reproduced every number
the resolution rests on and found what changed it: the crash a shared schema
number causes, the 0-byte clean answer, the re-ask rule, the guard's limit, the
cost on one long line, the CRLF column, and the question no seat asked, *what
licenses writing a fix*, which R2 and R3 answer.

## Author's verdict

**Pending**, queued as the decision issue `panel 193`. A yes settles R1 to R11
as written, the robust route at each disagreement; each conservative
alternative is named where it stands.
