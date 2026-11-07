# Panel 235's blind seat: the reader test

The coordinator's experiment for panel 235, the opening sitting of
M-package-manager, designed here and committed with its materials before
its first session, as panel 234's was (`docs/panel/234-briefs/blind/README.md`).
**Built and dry-run on 2026-11-07, and repaired the same day on the
completeness critic's first pass (`docs/panel/235-reports/critic-briefs.md`,
findings 2 to 14); no paid session has run.** Its materials stand complete
before the spec-warden registers predictions, and the predictions are
registered before any session runs. The runs are paid within the author's cap
of 25 USD for the whole sitting (`docs/roadmap/milestones/M-package-manager.md`,
the author's answers of 2026-11-06): this runner stops the reader test at 19.00
USD, the blind seat and the cap probe at 3.00 each, and every kind together at
22.00; the 3.00 between 22.00 and 25 is the spec-warden's margin for `heroes
measure --refresh`.

**What it measures.** The author decided that the dependency system copies
one widely used package system and names a dependency in code by an alias
the program's catalogue resolves, and gave the sitting a criterion, meant as
*the most modern and current solution, the one that works best for language
models* (the same page, 2026-11-07). The test asks **(U)** how a `use` line
marks a fetched dependency and **(F)** what the catalogue's file format is.
For each competing form, it measures whether a model that reads the
specification, plus one section stating that form, writes a correct catalogue
and program **at the first turn**, and **after one repair turn** that carries
the form's own messages: design.md Part 11's metrics 2, the first-try rate,
and 4, turns to green, here capped at two turns, on three tasks written for
this sitting and never on the held-out suite. What a result can do is fixed
before any session, in § The decision rule: a U result either leaves the
author's form standing or goes to the author as a measured case against it,
and is never adopted by the sitting.

## The arms: today's specification and one section

Each session reads `heroes-spec.md`: the specification at the lane's commit
(`spec/heroes-spec.md`, sha256 `0d88388a…f923a`, read by the dry run) with **one
section appended as `## 14. Packages`**, the place a new section takes
(`.claude/rules/spec-shape.md`: a new section goes at the end with the next
number). **Every section opens with one sentence of the same shape amending
section 1**: *Section 1's every `use` starts at the directory of the file you
compile now holds for the program's own modules; a `use` reaches a package by*
the arm's mark (`<json>`, `pkg/json` or the entry's name alone, `json`). Then
every section states the same facts in the same order: the catalogue's file
name, beside the file you compile, none for a program that uses no package;
**the one sentence identical in all eight**, *The catalogue holds the program's
dependencies and nothing else: no name, version or description of the program
itself.*; the format, with one example of an index entry (`json`, `1.4.2`) and
one git entry (`yaml`, `https://git.example.org/ana/yaml`, `v0.3.1`), each
with a well-formed `sha256-` sum; that an entry with a version comes from the
Heroes index, which knows the package by the entry's name, the version exact,
three numbers, and an entry with a git address comes from there at its tag,
its name the program's choice; that the sum is the publisher's fingerprint,
`sha256-` then 64 lowercase hex digits, and a package whose fetched files do
not match it is refused; what an entry's name may be; how a `use` reaches a
package's module, binding the last part, `as` renaming it; that a program's own
module is reached as in section 1, `use text/lines`; and last **the `Use`
production**: U-angle `Use = "use" [ "<" ident ">" "/" ] ident { "/" ident } [
"as" ident ] NEWLINE .`, U-pkg `Use = "use" [ "pkg" "/" ident "/" ] ident { "/"
ident } [ "as" ident ] NEWLINE .`, and U-bare saying that `Use` stays as
section 1 writes it and giving that production. The F arms read U-angle's
spelling, and carry its opening sentence and production word for word. No
example names a task's package, module or entry.

| arm | experiment | section | catalogue | how a `use` reaches a package | words (`wc -w`) | without the production | vendored tokens, `claude-legacy` / `cl100k_base` |
|---|---|---|---|---|---|---|---|
| U-angle | U | `sections/angle-toml.md` | `heroes.toml` | `use <json>/parse` | 264 | 244 | 504 / 514 |
| U-bare | U | `sections/bare-toml.md` | `heroes.toml` | `use json/parse`, an entry and a program folder of one name an error | 286 | 272 | 530 / 542 |
| U-pkg | U | `sections/pkg-toml.md` | `heroes.toml` | `use pkg/json/parse`, `pkg` reserved as a first name | 289 | 269 | 544 / 556 |
| F-yaml | F | `sections/angle-yaml.md` | `heroes.yaml` | `use <json>/parse` | 260 | 240 | 488 / 506 |
| F-toml | F | `sections/angle-toml.md`, U-angle's file | `heroes.toml` | `use <json>/parse` | 264 | 244 | 504 / 514 |
| F-json | F | `sections/angle-json.md` | `heroes.json` | `use <json>/parse` | 268 | 248 | 519 / 540 |
| F-lines | F | `sections/angle-lines.md` | `heroes.list` | `use <json>/parse` | 272 | 252 | 496 / 507 |
| F-hero | F | `sections/angle-hero.md` | `heroes.catalogue` | `use <json>/parse` | 273 | 253 | 514 / 532 |

Measured 2026-11-07 with `wc -w` and `./heroes measure <file>` on the lane
`pm-opening`'s compiler built from its seed (`heroes 0.6.0`, `sources
1a1e340778db789a`), the vendored rows only, never `--refresh`: each token
figure is **a lower bound, not the reader's tokeniser**, as the instrument
prints. The two example sums alone read 77 and 75 on the same two tables, the
same in every section. `wc` counts each production as 20 words, U-bare's as
14. The F sections run from 260 to 273 words; the U sections from 264 to 289,
U-bare and U-pkg each stating one rule U-angle has no need of (an entry named
as the program's own folder is an error; `pkg` is reserved), which is the
difference between the forms.

**The formats as the sections give them.** TOML: each entry a table under
`dependencies` (`[dependencies.json]`), each value a quoted string. YAML: the
same mapping under `dependencies:`, the example's values written as plain
scalars, as YAML catalogues are commonly written. JSON: an object under
`"dependencies"`, each entry an object of string values. The line format,
`heroes.list`: one line per entry, `dependency <name> index <version> <sum>`
or `dependency <name> git <address> <tag> <sum>`, `#` beginning a comment
line. Heroes' own syntax, `heroes.catalogue`: one `dependency <name>`
declaration per entry, its fields indented below it as a record's are (section
4), each value a string literal: `version: "1.4.2"`. **The F arms share one
word**, `dependencies` or `dependency`, and differ in format alone: the line
format no longer borrows Go's file and verb (`heroes.mod`, `require`), and
Heroes' syntax no longer borrows section 13's `package "raylib"`.

**U-angle and F-toml read the same section and run as two arms**, so that the
pair is an A/A control: what two arms of the same form differ by is the noise,
measured, and § The decision rule reads it first.

## The tasks

Three tasks, the same for every arm, each a `task.md` giving the packages'
names, sources, versions or tags, fingerprints, each module's function and
signature, and the exact expected output (`tasks/T1` to `T3`):

- **T1, one package from the index**: `roman` 2.1.0, module `numeral`,
  `function to_roman(n: i64) -> str?`; print `4 = IV`, `1987 = MCMLXXXVII`,
  `2026 = MMXXVI`, one per line.
- **T2, two packages both called `csv`**, from
  `https://git.example.org/ada/csv` at `v1.2.0` (module `fields`, `function
  split_line(line: str) -> [str]`) and `https://git.example.org/ben/csv` at
  **`1.10`** (module `quote`, `function quote_field(field: str) -> str`); split
  `red,green,blue`, quote each field, print `"red";"green";"blue"`. The
  catalogue must give the two packages two names. Their modules are named
  apart, so that no answer needs `as`, which would measure section 1 and not
  the catalogue. **Ben's tag is `1.10` so that YAML's number trap can fire**,
  the same in every arm: written plain, `tag: 1.10` is the float 1.1 by YAML
  1.2.2's core schema, and refused; unquoted, `tag = 1.10` is TOML's float,
  `"tag": 1.10` JSON's number and `tag: 1.10` a Heroes number literal, each
  refused by its format's own rule; the line format reads `1.10` as text. The
  sections' git example keeps `v0.3.1` in every arm.
- **T3, the program's own module beside a package of the same name**: the
  session's folder holds `geom/area.hero` (`function square(side: i64) ->
  i64`), and a package its authors call `geom` comes from
  `https://git.example.org/cara/geom` at `v3.0.2` (module `polygon`,
  `function hexagon_perimeter(side: i64) -> i64`); print `area: 25` and
  `perimeter: 30`. Under U-bare the entry must be named otherwise than `geom`;
  under the other arms it need not. The given module's comment names no
  panel and no test.

Each package is a module of Heroes in `packages/`, compiled and its `test`
block passed by the lane's compiler (`heroes test`, 1 test each, run
2026-11-07 after ben's comment took its new tag), formatted by `heroes fmt`.
**The world** (`world.json`) is everything the grader can fetch: the index
holds `roman` at 2.1.0 and nothing else; git answers at the three addresses,
each at its one tag; each fingerprint is the sha256 over the package's folder
as `world.json` defines it, recomputed and checked against the task and the
world by every dry run (ben's is now
`sha256-8f8ec08f37590a2ff129f489fdd22e48c2221472e13b22ee04f3892fb40bdb75`,
its comment naming its tag).

## The sessions

**A cell is one arm on one task on one model, five samples**: 8 arms, 3
tasks, `claude-haiku-5-5` and `claude-sonnet-5-5`, so 48 cells and **240 first
turns**, the order drawn by `random.Random(235).shuffle`, the key (folder to
arm, task, model, sample, turn and parent) written to
`.claude/worktrees/235-coordinator/sealed/key.tsv`, where `prepare` refuses to
draw it twice; every draw and every cut is logged in `sealed/key.log`. **A
first turn refused with messages gets one second turn**, a fresh session given
its catalogue and program in `previous/` and the messages that refused them in
`diagnostics.txt`; a first turn that builds and answers wrong, or crashes, or
gives no program, gets none: nothing told it anything.

Each session is a folder under the lane's `build/b235/<n>` (git ignores
`/build`) holding `heroes-spec.md`, `brief.md` (`templates/brief.md`, or
`templates/repair.md` for a second turn, the same text in every arm),
`task.md`, T3's `geom/area.hero`, and for a second turn `previous/` and
`diagnostics.txt`; nothing else. The brief asks for three sections,
`catalogue` (one fenced block), `program` (one fenced block marked `hero`) and
`context`. Each session runs from inside its folder, as panel 234's did, the
lane given to the runner by `--lane` (by default `.claude/worktrees/235-base`):

```
cd build/b235/<n> && GIT_CEILING_DIRECTORIES=<lane>/build/b235 CLAUDE_CODE_MAX_OUTPUT_TOKENS=16384 claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Answer in English." \
  --bare --no-session-persistence --system-prompt "You work in the directory <lane>/build/b235/<n>. You read its files with the Read tool, by absolute path." \
  --model <model> --restricted --safe-mode --strict-mcp-config \
  --tools "Read" --allowedTools "Read" \
  --disallowedTools "Bash,Write,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 0.20 --output-format stream-json --verbose < /dev/null > <lane>/build/b235-out/<n>.jsonl
```

`claude` 2.1.311's `--help` lists every flag above (the coordinator's check of
2026-11-07, no session run), and says that `--bare` skips CLAUDE.md's
auto-discovery and `--safe-mode` disables CLAUDE.md, so the lane's and the
trunk's, which stand above the folders, do not reach a session by those flags;
the void rule below is what checks it. The key is the trunk's
`ANTHROPIC_API_KEY` from the trunk's `.env`, never printed.

**The output cap, against panel 234's sessions.** Every reader runs with
`CLAUDE_CODE_MAX_OUTPUT_TOKENS=16384`. Panel 234's 301 result events
(`build/b234-out/`, read again 2026-11-07): its 162 Sonnet sessions wrote at
most 2,612 output tokens in a session and 2,292 in one call; 12 of its 139
Haiku sessions wrote more than 8,192 in one call, the most 15,073, of which
14,256 thinking (session 088), and none more than 16,384. The cap was first set
at 8,192, which would have cut about one Haiku session in twelve, a truncated
answer read as no answer; the coordinator raised it to 16,384 on that count.
What a call cut by the cap leaves of a session is unrun. So every grade records the session's output tokens and whether its last
call reached the cap, and the score counts those first turns in a column of
their own, `capped`. The usage an assistant event carries while it streams is
a partial count (panel 234's largest, 17, against results of up to 15,393), so
the runner reads output tokens from the result event alone: `usage.output_tokens`
for the session, `usage.iterations` for its last call.

**The budget.** At most **0.20 USD a session** (`--max-budget-usd`), whose
reach inside one call is unrun, so each running session **reserves 0.45**: the
0.20 it may report, plus one call past it at the output cap, priced at the
list prices the `claude-api` skill gives (its table of 2026-10-06, per million
tokens: Haiku 5.5 0.10 in and 0.50 out, Sonnet 5.5 2 and 10, Opus 5.5 4 and
20; thinking is billed as output): 16,384 Sonnet tokens out are 0.164, and at
most 40,000 in are 0.08. A reader starts only if the ledger's reader total,
plus 0.45 for every reader running, plus 0.45 for itself, stays within **19.00
USD**; a session whose output reports no cost is charged its 0.45. The ledger,
`.claude/worktrees/235-coordinator/sealed/ledger.jsonl`, read again before
every start, also holds two other kinds of line, each with its own hard stop
of 3.00: `seat` (§ The blind seat) and `capprobe` (below); every kind together
is held within **22.00** at every start. The seat runs with
`--max-budget-usd 2.00`, and its reserve is its stop, 3.00, which admits it
once: its cap plus one Opus 5.5 call past it at 32,000 output tokens, 0.64 out
and at most 0.30 in, is 2.94. One invocation spends at a time (`sealed/run.lock`), so
that every stop sees every session running.

**The cap probe** (`capprobe`, the coordinator's to run): one
`claude-haiku-5-5` session in the lane's `build/b235-capprobe/`, the readers'
flags, `CLAUDE_CODE_MAX_OUTPUT_TOKENS=64`, `--max-budget-usd 0.05`, asked to
write a story of 2,000 words; it prints the calls made, the output tokens in
all and in the last call, and whether the cap held: at most 64 in the last call
and at most 64 a call over the session. Charged as `capprobe`.

**The projection and the samples cut.** The first session of each model runs
alone (`run --first`) and `project` prints what the rest would cost from its
ledger before the rest runs: **the worst case**, what is spent, plus every open
first turn at its model's mean and a second turn for each, plus a second turn
for every first turn already run, **at five, four and three samples a cell**,
and the most samples whose worst case stays within 19.00. `run --samples N`
runs samples 1 to N of every cell and writes the cut in `sealed/key.log`. **A
projection, not a measurement**: at panel 234's means, re-read from its ledger
on 2026-11-07 (139 Haiku sessions at 0.0044 USD, 162 Sonnet at 0.0468, the
most 0.0617), the 120 first turns of each model would cost 6.13 USD and a
second turn for every one 12.27; at four samples 9.82, at three 7.36. These
sessions read a section of 488 to 556 vendored tokens beside the specification
and write two blocks, so they may cost more. A session the bound stops is
scored as no answer and never re-run; a second turn the hard stop leaves unrun
is absent, never a fail.

## Grading, mechanical (`blind235.py`, `judge`)

1. **The answer's parts**: headings are read outside fences only, so a
   program's comment never reads as one. A part begins at a heading naming it
   (`catalogue`, `program`, `context`) and **ends only at a heading of its own
   level or a higher one that names another part**, or at the end: a deeper
   heading inside it, as `### heroes.toml`, stays in the part. The part's
   first fenced block is its content. **Without the headings, the block
   holding `function main` is the program** (else the first marked `hero`,
   else the first), whatever its fence says, and the first other block the
   catalogue, in every arm.
2. **No catalogue**: refused, `catalogue_missing`.
3. **The catalogue read in the arm's format**. TOML by Python's `tomllib`,
   after a check that names every `version`, `git`, `tag` or `sum` written
   without its quotes, on a table's line, in a dotted key or in an inline
   table, with what TOML reads it as (`tag = 1.10`, the number 1.1) and a fix
   writing the same key; dotted keys and inline tables are read as the
   tables they are. YAML by a reader written for this test (block mappings
   and sequences, flow mappings and sequences on one or several lines, plain,
   single- and double-quoted scalars, comments, `---` and `...`), a plain
   scalar resolved by **YAML 1.2.2's core schema** and the number it reads
   named (`tag: 1.10`, the number 1.1); an anchor, an alias, a tag, a block
   scalar or a quoted string over two lines refused as what a catalogue does
   not use. JSON by Python's `json`, after a check that names every value of
   those four fields written as a number, a bare word or in single quotes, and
   with its refusals of a comment, a single quote and a trailing comma said in
   the catalogue's words. The line format and Heroes' own syntax by readers
   written from their sections, Heroes' reading a comment that ends a line, as
   section 1 allows. **A catalogue that does not parse is refused with its
   parse errors alone.** Anything beside the dependencies (`[package]`,
   `name: app`, `"name": "app"`, `module app`, `name: "app"`) is refused,
   citing the sentence every section states.
4. **Each entry checked**: its name lowercase letters, digits and `_`; its
   fields `version` and `sum`, or `git`, `tag` and `sum`, no other; every
   value a string; the version three numbers; the sum `sha256-` and 64
   lowercase hex digits. Then **fetched from the world**: an index entry by its
   name and version, a git entry by its address (a trailing `.git` or `/`
   read as the same repository) and tag; then its fingerprint compared.
5. **Each `use` line checked against the arm's spelling**: a fetched module
   names an entry that the catalogue holds and a module its package has; a
   `use` written in another arm's spelling, or an address written in a `use`,
   or the whole path inside the brackets (`use <roman/numeral>`, C's `#include
   <a/b>`), is refused with the arm's spelling as its fix; **a `use` naming no
   entry and no module of the program's own, whose module a fetched package
   holds, is refused in every arm with the same lookup**, the holder named and
   a `fix (guess)` in the arm's spelling (`unknown_entry` where the arm's
   spelling was used, `use_spelling` where it was not); under U-bare an entry
   named as the program's own folder or module is refused; any other `use` is
   the program's own and is the compiler's to judge.
6. **Built**: in a fresh folder, every fetched package placed where its `use`
   reaches it (U-bare `json/`, U-pkg `pkg/json/`, the angle arms a folder
   whose name has the bracketed name's length, so that the compiler's columns
   stay the program's), the program as `main.hero`, T3's own module beside
   it; `heroes build main.hero -O0 -o prog` by the lane's compiler, its
   temporary files under the lane's `build/b235-tmp`; its messages handed on
   with every path in the arm's spelling and, where it says `unknown_module`, a
   note listing the catalogue's entries as the arm reaches them.
7. **The task's own demands**, read once the build succeeds: every package
   the task names is reached by a `use`, and T3's own module is used
   (`task_unmet`).
8. **Run** under `sandbox-exec` with panel 234's profile (nothing allowed by
   default, no read under `/Users` or the temporary folders but its own
   folder, no network), bounded at 10 seconds and 1 MiB of output; its
   standard output compared byte for byte with the task's `expected.txt`.

**The verdicts**: **pass**, built, exit 0, output equal; **fail**, refused
with messages (steps 2 to 7), the codes recorded, the messages written to a
second turn's `diagnostics.txt`; **silent**, built, exit 0, output different;
**crash**, built, exit other than 0 or out of time; **no answer**, no program
block; **grader_error**, the grader's own failure on one session, caught:
recorded, printed loudly by `grade` and by `score`, counted in no share and
never as the model's fail, and graded again by the next `grade` once the
grader is repaired. **pass@1**: the first turn passes. **pass@2**: the first
turn passes, or it failed and its one second turn passes; a second turn owed
and never run, or void, is **absent**, counted apart and left out of pass@2's
share. **silent** is counted at the first turn and at the final one. A share
is over a cell's first turns that are not void.

**Void, read before the key is joined**: a session whose tool calls are
anything but `Read` of a path inside its own folder (a relative path read
from the folder) is void and counted apart; its `context` is kept in the
grades and read by hand, and one that names a project file, rule, memory or
commit is void too.

**The messages are the coordinator's**, written for this test in the
compiler's form (`error[code]`, `at file:line:col`, the line, a caret, a `fix
(certain)` or `fix (guess)`): `catalogue_missing`, `catalogue_syntax`,
`catalogue_field`, `catalogue_value`, `catalogue_name`, `catalogue_version`,
`catalogue_sum`, `fetch_failed`, `sum_mismatch` (both fingerprints shown, no
fix offered), `unknown_entry`, `use_spelling`, `use_no_module`,
`unknown_package_module`, `entry_collision`, `task_unmet`; then the
compiler's own. `dry-run.txt` shows every one of them on the hand-made
answers.

## The decision rule, registered before any session (`score`)

`score` computes and prints it, and nothing else decides:

- **Per arm, pooled** over both models and the three tasks: pass@1 and
  pass@2, out of 30 first turns (fewer where void, cut by `--samples`, or with
  a second turn absent).
- **The A/A pair first**: U-angle against F-toml, the same section, by a
  one-sided Fisher exact test in each direction on pass@1 and on pass@2,
  uncorrected. If any of the four is below 0.05, `score` prints that the test
  is too noisy to read any difference, and every comparison below is printed
  and marked unread.
- **Question U**: U-bare and U-pkg each against U-angle; **question F**: every
  pair of F-yaml, F-toml, F-json, F-lines and F-hero. Each comparison is a
  **one-sided Fisher exact test in each direction on pass@1 and on pass@2**,
  and within each question the whole family's p-values (U: 2 pairs × 2
  metrics × 2 directions, 8; F: 10 pairs, 40) are corrected by
  **Holm-Bonferroni at α = 0.05**.
- **What question U means**: a control (U-bare, U-pkg) significantly above
  U-angle is **a measured case against the author's position**, which goes to
  the author and is never adopted by the sitting; otherwise **the author's
  form stands as measured**.
- **What question F means**: a format significantly below another is out; the
  formats left tied go to the sitting's other measures (parser size, hazards,
  tokens).

The Fisher test and Holm's correction are the runner's own (Python's standard
library, `math.comb`), checked by every dry run against values computed by
hand: 30/30 against 25/30, one-sided, p = 0.026093; 3/3 against 0/3, 0.05;
2/4 against 0/4, 15/70; 1/2 against 1/2, 5/6; Holm over 0.01, 0.04, 0.03,
0.005 gives 0.03, 0.06, 0.06, 0.02. The dry run also reads the rule on three
built score tables: all arms equal; a control far above U-angle and one format
far below the rest; and the A/A pair apart.

**The habit count.** Every first turn refused where its first message falls on
a line matching one of these patterns (`HABITS` in the runner, read in this
order, the first match winning) is tagged with that ecosystem, and `score`
prints the counts by pattern, ecosystem and model:

| ecosystem | habit | read in | pattern |
|---|---|---|---|
| Go | `require`, alone or opening `require ( ... )` | catalogue | `^\s*require\b` |
| Go | a `module` line naming the program | catalogue | `^\s*module\s+\S` |
| Go | an address with a domain in a `use` | program | `^\s*use\s+[^#\n]*\b[a-z0-9-]+(\.[a-z0-9-]+)+/` |
| Go | a `v` before an index version | catalogue | `(\bversion\b\W*\|\bindex\s+)v\d+\.\d+` |
| Python | a `==` pin | catalogue | `==\s*v?\d` |
| Cargo | `[package]` | catalogue | `^\s*\[\s*package\s*\]` |
| Cargo | the shorthand `name = "1.2.3"` | catalogue | a key other than `version`, `git`, `tag`, `sum` given a quoted version |
| npm | a `^1.2.3` or `~1.2.3` range | catalogue | `["'\s:=][\^~]\d+(\.\d+)*` |

## The cells the spec-warden registers

**Per arm and per model, over the arm's three tasks: pass@1 and pass@2**, a
count out of 15 first turns (fewer where a session is void, cut or absent):
8 arms × 2 models × 2 = **32 cells**. Each with its floor and ceiling where the
warden names one. The score also prints every task's cell, silent, crash,
capped, the first codes and the habit count, which are read and not
predicted.

## The blind seat (`seat`)

One session judges the three U forms without knowing which is whose
(`blind/seat/`, written by the coordinator). `seat` assembles the lane's
`build/b235-seat/`: `brief.md`; `heroes-spec.md`, today's specification
**unamended**; and every row of `seat/variants.tsv` (tab-separated, `#` lines
comments), the file at its right-hand path in `blind/` copied to its left-hand
name (`variant-1.md` to `variant-3.md`, `task-1.md`, `task-2.md`,
`geom/area.hero`); `variants.tsv` itself is never copied. Before it runs, the
folder must hold exactly those eight files, its specification byte for byte
the lane's, and no file the words `U-angle`, `U-bare`, `U-pkg`, `F-yaml`,
`F-toml`, `F-json`, `F-lines`, `F-hero`, `pkg-toml`, `angle-toml`, `bare-toml`,
` arm ` or `panel`, the specification's own four ` arm `, its `match` arms,
excepted. The session takes the readers' flags but `--tools "Read,Write"
--allowedTools "Read,Write"`, `--model claude-opus-5-5` (`claude-sonnet-5-5`,
with the reason printed, where `claude --help` shows that `--model` cannot
take it), `--max-budget-usd 2.00`, `CLAUDE_CODE_MAX_OUTPUT_TOKENS=32000`, a
system prompt naming its folder, and the prompt *Read brief.md in this
directory and follow it exactly. Your inputs are the files in this directory
only. Write report.md here. Answer in English.* It is charged as `seat`, runs
once, and is void where any tool call is other than `Read` or `Write` of a path
inside its folder.

## Pinning (`manifest`)

`manifest` prints the sha256 of the runner (by its path in the trunk,
`.claude/worktrees/235-coordinator/blind235.py`) and of every file under
`blind/` but `dry-run.txt` and the manifest itself (by its path in the lane),
sorted by path; the coordinator saves it as `blind/manifest.sha256` and commits
it. Where that file exists, `prepare`, `run`, `seat` and `capprobe` refuse to
start when any file differs from it, is missing, or is not in it, naming
each; `dry` prints the same comparison as a problem.

## What the critic found and what changed

2. A valid TOML catalogue crashed the grader: dotted keys and inline tables
   are read and located, and any failure of the grader on a session is
   recorded as `grader_error` while grading goes on.
3. Right answers failed: a deeper heading stays in its part; Heroes' syntax
   reads a comment ending a line; without headings the program is the block
   holding `function main`.
4. Rules no section stated: every section states the catalogue holds the
   dependencies and nothing else, and the refusals of `[package]`, `name: app`,
   `module app` and their like cite it.
5. The repair messages leaned: the holder lookup and its `fix (guess)` reach
   every arm alike, and `use <roman/numeral>` is the grader's, in each arm's
   spelling.
6. Second turns never run counted as failed: they are absent, counted apart.
7. Lengths: re-measured, above.
8. U facts contradicted section 1: every section amends it in one sentence of
   the same shape and gives its `Use` production.
9. The words were Go's and section 13's: `heroes.list` and `dependency` in
   the line format, `dependency` in Heroes' syntax.
10. No JSON arm: F-json added, 8 arms, 48 cells, 240 first turns.
11. YAML's hazard could not fire: ben's tag is `1.10` in every arm.
12. An index entry's name and the sum's provenance, stated in every arm and
    ruled by nobody: unchanged here, the same text in every arm; who rules
    them is outside this test.
13. The hard stop bounded reported cost: a 0.45 reserve, an output cap of
    16,384, stops of 19.00, 3.00, 3.00 and 22.00, the seat's cap at 2.00, the
    cap probe, the samples cut.
14. Nothing pinned the grader: `manifest`, and the decision rule above.

## The dry run, before any session (`dry-run.txt`)

`python3 -I .claude/worktrees/235-coordinator/blind235.py dry --lane <lane>`,
its whole output in `dry-run.txt`: the compiler, the specification's and the
runner's sha256, the manifest's state; the world's fingerprints recomputed;
each section's words, its shared sentence, its opening amendment and its
production, and each arm's `heroes-spec.md` assembled; one session folder per
arm and task; then **118 hand-made answers** (`dry/<arm>/<task>-<label>.md`,
their expected verdict, first code and habit in `dry/expect.tsv`): U-angle 17,
U-bare 12, U-pkg 11, F-yaml 13, F-toml 18, F-json 10, F-lines 13, F-hero 15,
and 9 under `extra/` for what the grader must also tell apart (silent, crash,
the compiler's own message, void, no answer, no catalogue, comments that read
like headings, the task's demand unmet, an address in a `use`); 21 of them are
the critic's crafted answers (`critic-` in their names), the F-hero and line
format ones in their arm's new word. They hold one right answer per arm and
task and the slips typical of each arm: a missing or unclosed quote, another
arm's spelling, an entry not named, a value left unquoted or read as a number
(`tag: 1.10`), YAML's indentation, a tab or a colon without its space, JSON's
trailing comma, comment and single quotes, Cargo's shorthand and `[package]`,
Go's `require ( … )` block, `require` and `module`, a `v` before a version,
Python's `==`, npm's `^` range, a duplicate name. Each is graded as a
session's answer is, from a stream-json file, against its expectation. Then
the decision rule's statistics against hand values; the run's own steps on dry
paths with a `claude` that runs nothing: `prepare` drawing 240 first turns in
48 cells of five and refusing a second draw, `run --first` and `project`, `run
--samples 3`, a grader made to fail on one session, `grade`, `repairs` and
`score` over the 118 with one second turn left unrun, the totals and the habit
count checked; the hard stops of each kind and the overall one; a reader's
command and environment; the cap probe on three fake results; the seat's
folder assembled from `blind/seat/` and checked, its model chosen from three
help texts, a run inside its folder and a void one; and the manifest refusing
a changed and an added file on a copy of the materials.

## What the test cannot measure

- **A tool writing the catalogue.** Where a verb (design.md §3.5's `heroes
  add`) writes the entry, a model may never write one by hand; this test
  measures the hand.
- **What lies past one exact pin**: version ranges and their resolution,
  lock files, a dependency's own dependencies and its own `use` lines,
  updates, a local folder as a source, `internal/`.
- **The fingerprint's work**: every sum is given in the task; nothing
  measures computing, checking or trusting one. Fetching is simulated:
  `git.example.org` answers nothing in the world.
- **A real implementation's messages.** The second turn measures the
  coordinator's messages, written to be equally exact across arms, and they
  are not: TOML's syntax messages are `tomllib`'s, reworded in two cases;
  JSON's are Python's `json`, reworded for a comment, a single quote and a
  trailing comma; a value without its quotes is named before either reads the
  file; the other three formats' are hand-written, and YAML's reader is a
  subset. A difference at pass@2 may be the messages' as much as the form's.
- **The format apart from its file**: the F arms share their words,
  `dependencies` and `dependency`, and differ in syntax and in the file's name
  (`heroes.toml`, `heroes.yaml`, `heroes.json`, `heroes.list`,
  `heroes.catalogue`), which comes with the format.
- **Small differences.** Five samples a task cell, fifteen an arm and model,
  thirty an arm pooled: Fisher's test at 30 against 30 calls 30/30 against
  25/30 a difference (one-sided p 0.026) before Holm's correction, and the
  A/A pair says how far two identical arms fall apart. Two models, both
  Anthropic's; three tasks, written by the coordinator, none from the
  held-out suite.
- **Habit apart from clarity**: a form a model has seen often (Cargo's TOML,
  npm's JSON) and a form that is clear cannot be told apart here; the habit
  count names the slips, not their cause.
- **The landed specification**: section 14 is appended and amends section 1
  in one sentence and its production; section 1's own text stands unedited in
  every arm, and section 13 still uses the word *package* for a system
  library. Both are the same in every arm.
- **A session cut by the output cap**: what 16,384 tokens a call does to a
  model that thinks longer is unrun; `capped` counts the sessions it reached.
- **The reader's real token count**: the figures above are the vendored
  tables'; `--refresh` was not run.
- **Anything a person reading the catalogue would feel**: only what a model
  writes is measured.

## Running it

```
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes     # in the lane, first
python3 -I .claude/worktrees/235-coordinator/blind235.py manifest > <lane>/docs/panel/235-briefs/blind/manifest.sha256
python3 -I .claude/worktrees/235-coordinator/blind235.py dry
python3 -I .claude/worktrees/235-coordinator/blind235.py capprobe
python3 -I .claude/worktrees/235-coordinator/blind235.py prepare
python3 -I .claude/worktrees/235-coordinator/blind235.py run --first
python3 -I .claude/worktrees/235-coordinator/blind235.py project
python3 -I .claude/worktrees/235-coordinator/blind235.py run --jobs 6 --samples <N>
python3 -I .claude/worktrees/235-coordinator/blind235.py grade
python3 -I .claude/worktrees/235-coordinator/blind235.py repairs
python3 -I .claude/worktrees/235-coordinator/blind235.py run --jobs 6 --samples <N>
python3 -I .claude/worktrees/235-coordinator/blind235.py grade
python3 -I .claude/worktrees/235-coordinator/blind235.py score > score.md
python3 -I .claude/worktrees/235-coordinator/blind235.py seat
```

Every command takes `--lane <path>`, by default `.claude/worktrees/235-base`.
Every path the script writes is checked to stand inside the repository's
root. The script is kept in the sitting's evidence beside its sealed key,
ledger and grades.
