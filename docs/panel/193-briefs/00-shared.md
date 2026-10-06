# Panel 193, shared brief: where `check --json` says a fix applies

Convened 2026-10-05 by the author's yes of that evening, given in the
conversation (CLAUDE.md § 4, a tool surface), recorded in the sitting's
decision issue. Every fact below was produced by the command named beside it,
run by the coordinator between 22:40 and 22:47 or by the completeness critic's
first pass between 23:10 and 23:12 (`../193-reports/completeness-critic-briefs.md`,
its commands there); a sentence that is not a measurement says so. **This
brief was repaired after 23:13 on the critic's pass**: its first version gave a
wrong stream, a wrong count, a wrong applier and a blind case that gave its
answer away.

## The question

**Defect 271** (`issues/2026-10/04/2026-10-04-0004-defect-271-a-fix-that-applies-away-from-its-message-s-line-says-neither.md`,
class `adjacent`): a fix that applies away from its message's line says it
neither in the message nor in `heroes check --json`. **Its rendered half has
landed** on batch 12's lane str192, commit `1697cec1` at 22:54 (after the first
version of this brief): a fix's line gains ` (at file:line:col)`, its start
only, the line per file, the column in characters, and only where the fix
stands off its message's line. **This sitting rules the JSON half**, a tool
surface (CLAUDE.md § 4; `.claude/rules/cli-surface.md`: *`--json` says how to
print, never what*, panel 016's P2), **and what `check --apply --json`
answers**: defect 371, filed 23:13, `--apply` ignores `--json` in silence and
prints the program at exit 0.

**The proposal, to be judged and not assumed**: each object of a
diagnostic's `"fixes"` gains the place of the text it replaces, so a tool can
apply a fix without re-deriving it. The sitting settles: which fields (offsets,
line and column, an end), in which unit, in which file's coordinates, whether
`"schema"` moves, **and what an answer promises about how its fixes combine**
(the critic's question, below).

**The routes on the table**, the critic's additions marked:
- the place as offsets and line and column, start and end;
- (critic) each fix also carries the text it replaces, so a tool can refuse a
  file that changed since the check;
- (critic) one fix of several edits for one mistake, as rustc, SARIF and LSP
  write it;
- (critic) the answer declares its column unit, as clang's SARIF and LSP do;
- (critic) `check --apply --json` returns the edits `--apply` actually writes,
  against the original text, so it agrees with `--apply` by construction;
- (critic) **nothing**: the stopping rule's third shape
  (`.claude/rules/cli-surface.md`), if two existing invocations compose to it.

## What is measured

- **The stream**: `check --json` writes its answer to **stderr**, not stdout
  (`check --json <178's case> > out 2> err`: stdout 0 bytes, stderr 1,799;
  `selfhost/cli/check.hero:150-151`, the critic).
- **Today's answer** on `tests/golden/check/fixedbugs-178-a-comma-ending-every-line-is-one-message-a-run.hero`:
  `{"schema": 1, "diagnostics": [...]}`, each diagnostic `code`, `message`,
  `file`, `line`, `col`, `notes`, `fixes`, each fix exactly `title`,
  `replacement`, `certainty`. **Three diagnostics carry ten identical fixes**,
  13:10 three, 19:15 three, 24:10 four, each *delete the `,`*, `replacement`
  `""`, none placed (the critic's count; the first brief said three).
- **A diagnostic's `col` counts characters**: 14 on a line holding `è€😀`, where
  bytes give 20 and UTF-16 units 15 (the critic). `lex --json` already writes a
  place as `line`, `col` and `text`, no offset and no end.
- **The compiler's own fix**: `record Fix` at `selfhost/diag.hero:46-50`,
  `title`, `replacement`, `span: token.Span`, `certainty`. **A span carries no
  file**: its offsets index one text of every file read, concatenated
  (`selfhost/source.hero:14-24`), so a raw `span.start` is not a file's own
  offset (read by the critic; the frozen compiler prints no span, so unrun).
- **Who writes the JSON**: `selfhost/cli/check_json.hero`, 65 lines, its fix
  row at `:34` to `:41`; `check --permissive --json`, the control arm, goes
  through the same writer.
- **Who applies fixes, and how** (the critic, reading and measuring): not
  `check.hero:71`, which hands back a clean file, but `report` (`:107-148`),
  `settle` (`:194-236`) and `certain.applied`: up to `ROUNDS` = 64 rounds, the
  program read again each round; a fix outside the root file is not written; an
  identical twin is written once; a fix touching one already written waits for
  the next round. One round against 64 over 541 roots: all 170 answer files
  agree but the three `surface-fixtures/certain137-*` cases. `certain.hero:15-18`
  records two writes by places alone that went wrong: an abort at exit 134, and
  `print(total(xs.must())rint(0)`.
- **Fix construction sites**: 113 in `selfhost/`, 108 in code and 5 in tests
  (`git grep -c` of `diag.Fix(` and a bare `Fix(title`; the critic split them).
- **Readers of the JSON**, the grep and the critic together:
  - `tests/harness/fix_names.hero:99` (every fix's `"title"`);
  - `tests/harness/suite_surface.hero` rows `:231` to `:235` (`lex` and
    `check` JSON escaping), `:286` (pins `{"schema": 1,`), `:372` (an empty
    answer), `:425` (the per-file line of a diagnostic in another module);
  - a compiler test, `selfhost/cli/check.hero:370-379`, pinning the schema
    line;
  - `heroes --help` (`selfhost/cli/table.hero:92`) says *schema 1*;
  - `site/src/llms.txt:60`, the page published for language models: *`heroes
    check --json` emits schema 1 with stable snake_case error codes, source
    locations, notes and suggested fixes*, literal text no check reads;
  - the site's errors page, English and Italian (`errors.html:39`), which
    promises the JSON in prose;
  - panel 187's R2 instrument, outside the tree, read at each recovery round
    (its rebuilt `replay.py` tolerates an added field and a moved schema);
  - `docs/roadmap/verify.md`. No editor extension reads it.
- **The specification and design.md say nothing of `--json`** (`grep -c` reads
  0 in both at `00217c39`); `check_json.hero:1` cites design.md §4.17 as the
  schema's home. The spec: 9,392 tokens on the reader's tokeniser (recorded
  2026-10-03), 7,117 on the vendored maximum, headroom 848 of 10,240.
- **Precedents the critic ran**: rustc 1.90 writes byte offsets beside
  character columns; clang 21's parseable fix-its give byte columns, and its
  SARIF declares `unicodeCodePoints` and carried no fix for the same error;
  LSP 3.17 defaults to UTF-16 (its specification page).

## The critic's question, which every seat answers

**What does an answer promise about how its fixes combine?** Today `certain`
fixes are applied together, two `guess`es can be alternatives (`lone_brace`'s
two), every place is in the original text, and twins, touching fixes, further
rounds and other modules are handled only inside `--apply`. A tool given only
places would write what `--apply` never does. And panel 016's own question:
now that the message says where a fix applies, must the JSON say at least the
same, in the same unit and per-file coordinates, and may it say more?

## What a seat owes

- Read `CLAUDE.md`, `.claude/rules/cli-surface.md` and
  `.claude/rules/diagnostics-and-goldens.md` in your copy.
- **Your own directory**, `<scratchpad>/193-<seat>/`, a copy of the frozen tree
  made by `git -C /Users/joseph/Temp/heroes/heroes-lang archive 00217c39 | tar
  -x -C <dir>`; build your compiler inside it from the seed (`clang -I runtime
  seed/heroes.c runtime/runtime.c -o heroes`, a few seconds), never the trunk's
  or another seat's. `<scratchpad>` is
  `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
  The trunk is moving under another session tonight: never read it as the
  frozen tree. Lane str192's rendered half is on its branch,
  `lane-b12-str192`, commit `1697cec1`, readable with `git show`.
- **Your report**, written as you go, at
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-b12-hook/docs/panel/193-reports/<seat>.md`:
  a verdict (approve, object, veto), the route you would adopt, its cost, a
  falsifiable prediction, and what would change your verdict.
- **No paid run** of any kind (`claude -p`, an API call); one you find worth
  running goes in your report with its size. Never `rm`. English, no em dashes.
- Every number in your report comes from a command you ran, named beside it; a
  negative claim goes out as a question naming what you searched. **zsh**: an
  unquoted `$var` holding several words is ONE word, so a loop over flags runs
  them through `bash -c` (the coordinator's own first measurement of defect 371
  read exit 2 for that reason).
