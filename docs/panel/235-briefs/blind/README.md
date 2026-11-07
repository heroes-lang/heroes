# Panel 235's blind seat: the reader test

The coordinator's experiment for panel 235, the opening sitting of
M-package-manager, designed here and committed with its materials before
its first session, as panel 234's was (`docs/panel/234-briefs/blind/README.md`).
**Built and dry-run on 2026-11-07; no paid session has run.** Its materials
stand complete before the spec-warden registers predictions, and the
predictions are registered before any session runs. The runs are paid within
the author's cap of 25 USD for the whole sitting
(`docs/roadmap/milestones/M-package-manager.md`, the author's answers of
2026-11-06), and this runner stops itself at 22.00 USD.

**What it measures.** The author decided that the dependency system copies
one widely used package system and names a dependency in code by an alias
the program's catalogue resolves, and gave the sitting a criterion, meant as
*the most modern and current solution, the one that works best for language
models* (the same page, 2026-11-07). The sitting must choose **(U)** how a
`use` line marks a fetched dependency and **(F)** the catalogue's file format.
For each competing form, the test measures whether a model that reads the
specification, plus one section stating that form, writes a correct catalogue
and program **at the first turn**, and **after one repair turn** that carries
the form's own messages: design.md Part 11's metrics 2, the first-try rate,
and 4, turns to green, here capped at two turns, on three tasks written for
this sitting and never on the held-out suite.

## The arms: today's specification and one section

Each session reads `heroes-spec.md`: the specification at the lane's commit
(`spec/heroes-spec.md`, sha256 `0d88388a…f923a`, read by the dry run) with **one
section appended as `## 14. Packages`**, the place a new section takes
(`.claude/rules/spec-shape.md`: a new section goes at the end with the next
number). Every section states the same facts in the same order: the
catalogue's file name, beside the file you compile, none for a program that
uses no package; the format, with one example of an index entry (`json`,
`1.4.2`) and one git entry (`yaml`, `https://git.example.org/ana/yaml`,
`v0.3.1`), each with a well-formed `sha256-` sum; that an entry with a version
comes from the Heroes index, which knows the package by the entry's name, the
version exact, three numbers, and an entry with a git address comes from
there at its tag, its name the program's choice; that the sum is the
publisher's fingerprint, `sha256-` then 64 lowercase hex digits, and a package
whose fetched files do not match it is refused; what an entry's name may be;
how a `use` reaches a package's module, binding the last part, `as` renaming
it; and that a program's own module is reached as in section 1, `use
text/lines`. No example names a task's package, module or entry.

| arm | experiment | section | catalogue | how a `use` reaches a package | words (`wc -w`) | without the production | vendored tokens, `claude-legacy` / `cl100k_base` |
|---|---|---|---|---|---|---|---|
| U-angle | U | `sections/angle-toml.md` | `heroes.toml` | `use <json>/parse` | 213 | 193 | 429 / 438 |
| U-bare | U | `sections/bare-toml.md` | `heroes.toml` | `use json/parse`, an entry and a program folder of one name an error | 216 | 216 | 428 / 437 |
| U-pkg | U | `sections/pkg-toml.md` | `heroes.toml` | `use pkg/json/parse`, `pkg` reserved as a first name | 217 | 217 | 433 / 443 |
| F-yaml | F | `sections/angle-yaml.md` | `heroes.yaml` | `use <json>/parse` | 209 | 189 | 413 / 430 |
| F-toml | F | `sections/angle-toml.md`, U-angle's file | `heroes.toml` | `use <json>/parse` | 213 | 193 | 429 / 438 |
| F-lines | F | `sections/angle-lines.md` | `heroes.mod` | `use <json>/parse` | 221 | 201 | 421 / 431 |
| F-hero | F | `sections/angle-hero.md` | `heroes.catalogue` | `use <json>/parse` | 222 | 202 | 439 / 456 |

Measured 2026-11-07 with `./heroes measure <file>` on the lane's compiler,
the vendored rows only, never `--refresh`: each figure is **a lower bound, not
the reader's tokeniser**, as the instrument prints. The two example sums
alone read 77 and 75 on the same two tables, the same in every section. The
angle arms carry the production `Use = "use" [ "<" ident ">" "/" ] ident { "/"
ident } [ "as" ident ] NEWLINE .`, which `wc` counts as 20 words; U-bare and
U-pkg carry their rule's sentence instead.

**The formats as the sections give them.** TOML: each entry a table under
`dependencies` (`[dependencies.json]`), each value a quoted string. YAML: the
same mapping under `dependencies:`, the example's values written as plain
scalars, as YAML catalogues are commonly written. The line format: one line
per entry, `require <name> index <version> <sum>` or `require <name> git
<address> <tag> <sum>`, `#` beginning a comment line. Heroes' own syntax: one
`package <name>` declaration per entry, its fields indented below it as a
record's are (section 4), each value a string literal: `version: "1.4.2"`.

**U-angle and F-toml read the same section and run as two arms**, five
samples each, so that the pair is an A/A control: what two cells of the same
form differ by is the noise of a five-sample cell, measured, which every
other comparison is read against.

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
  `v0.9.4` (module `quote`, `function quote_field(field: str) -> str`); split
  `red,green,blue`, quote each field, print `"red";"green";"blue"`. The
  catalogue must give the two packages two names. Their modules are named
  apart, so that no answer needs `as`, which would measure section 1 and not
  the catalogue.
- **T3, the program's own module beside a package of the same name**: the
  session's folder holds `geom/area.hero` (`function square(side: i64) ->
  i64`), and a package its authors call `geom` comes from
  `https://git.example.org/cara/geom` at `v3.0.2` (module `polygon`,
  `function hexagon_perimeter(side: i64) -> i64`); print `area: 25` and
  `perimeter: 30`. Under U-bare the entry must be named otherwise than `geom`;
  under the other arms it need not.

Each package is a module of Heroes in `packages/`, compiled and its `test`
block passed by the lane's compiler (`heroes test`, 1 test each, 2026-11-07),
formatted by `heroes fmt`. **The world** (`world.json`) is everything the
grader can fetch: the index holds `roman` at 2.1.0 and nothing else; git
answers at the three addresses, each at its one tag; each fingerprint is the
sha256 over the package's folder as `world.json` defines it, recomputed and
checked against the task and the world by every dry run.

## The sessions

**A cell is one arm on one task on one model, five samples**: 7 arms, 3
tasks, `claude-haiku-5-5` and `claude-sonnet-5-5`, so 42 cells and **210 first
turns**, the order drawn by `random.Random(235).shuffle`, the key (folder to
arm, task, model, sample, turn and parent) written to
`.claude/worktrees/235-coordinator/sealed/key.tsv`, where `prepare` refuses to
draw it twice. **A first turn refused with messages gets one second turn**, a
fresh session given its catalogue and program in `previous/` and the
messages that refused them in `diagnostics.txt`; a first turn that builds
and answers wrong, or crashes, or gives no program, gets none: nothing told
it anything.

Each session is a folder under the lane's `build/b235/<n>` (git ignores
`/build`) holding `heroes-spec.md`, `brief.md` (`templates/brief.md`, or
`templates/repair.md` for a second turn, the same text in every arm),
`task.md`, T3's `geom/area.hero`, and for a second turn `previous/` and
`diagnostics.txt`; nothing else. The brief asks for three sections,
`catalogue` (one fenced block), `program` (one fenced block marked `hero`) and
`context`. Each session runs from inside its folder, as panel 234's did:

```
cd build/b235/<n> && GIT_CEILING_DIRECTORIES=<lane>/build/b235 claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Answer in English." \
  --bare --no-session-persistence --system-prompt "You work in the directory <lane>/build/b235/<n>. You read its files with the Read tool, by absolute path." \
  --model <model> --restricted --safe-mode --strict-mcp-config \
  --tools "Read" --allowedTools "Read" \
  --disallowedTools "Bash,Write,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 0.20 --output-format stream-json --verbose < /dev/null > <lane>/build/b235-out/<n>.jsonl
```

`claude` 2.1.311's `--help` lists every flag above (checked 2026-11-07, no
session run), and says that `--bare` skips CLAUDE.md's auto-discovery and
`--safe-mode` disables CLAUDE.md, so the lane's and the trunk's, which stand
above the folders, do not reach a session by those flags; the void rule below
is what checks it. The key is the trunk's `ANTHROPIC_API_KEY` from the
trunk's `.env`, never printed.

**The budget.** At most **0.20 USD a session** (`--max-budget-usd`), and a
**hard stop at 22.00 USD**: a session starts only if the ledger's total, plus
0.20 for every session running, plus 0.20 for itself, stays within 22.00, so
the run cannot pass it while each session keeps its cap; a session whose
output reports no cost is charged its 0.20. The ledger is
`.claude/worktrees/235-coordinator/sealed/ledger.jsonl`, read again before
every start, across separate invocations. The first session of each model
runs alone (`run --first`) and `project` prints what the rest would cost from
its ledger before the rest runs. **A projection, not a measurement**: panel
234's ledger, read 2026-11-07, holds 139 Haiku sessions at a mean of 0.0044
USD and 162 Sonnet sessions at 0.0468 (the most, 0.0617), so 105 first turns
of each would be about 5.4 USD and a second turn for every one about 10.8;
these sessions read about 450 more tokens of specification and write two
blocks instead of one, so they may cost more. If the projection passes the
hard stop, every cell is cut from five samples to four, then three, before
the rest runs. A session the bound stops is scored as no answer and never
re-run.

## Grading, mechanical (`blind235.py`, `judge`)

1. **The answer's parts**: the first fenced block under the heading
   `catalogue`, under `program`, and the text under `context`; headings are
   read outside fences only, so a program's comment never reads as one.
   Without the headings, the first block marked `hero` is the program and the
   first other block the catalogue.
2. **No catalogue**: refused, `catalogue_missing`.
3. **The catalogue read in the arm's format**: TOML by Python's `tomllib`,
   after a check that names a value written without its quotes; YAML by a
   reader written for this test (block mappings and sequences, flow mappings
   and sequences on one or several lines, plain, single- and double-quoted
   scalars, comments, `---` and `...`), a plain scalar resolved by **YAML
   1.2.2's core schema**, so `version: 2.1` is a number and refused where a
   string is owed, and an anchor, an alias, a tag, a block scalar or a quoted
   string over two lines refused as what a catalogue does not use; the line
   format and Heroes' own syntax by readers written from their sections. **A
   catalogue that does not parse is refused with its parse errors alone.**
4. **Each entry checked**: its name lowercase letters, digits and `_`; its
   fields `version` and `sum`, or `git`, `tag` and `sum`, no other; every
   value a string; the version three numbers; the sum `sha256-` and 64
   lowercase hex digits. Then **fetched from the world**: an index entry by its
   name and version, a git entry by its address (a trailing `.git` or `/`
   read as the same repository) and tag; then its fingerprint compared.
5. **Each `use` line checked against the arm's spelling**: a fetched module
   names an entry that the catalogue holds and a module its package has; a
   `use` written in another arm's spelling, or an address written in a
   `use`, is refused with the arm's spelling as its fix; under U-bare an entry
   named as the program's own folder or module is refused; any other `use` is
   the program's own and is the compiler's to judge.
6. **Built**: in a fresh folder, every fetched package placed where its `use`
   reaches it (U-bare `json/`, U-pkg `pkg/json/`, the angle arms a folder
   whose name has the bracketed name's length, so that the compiler's columns
   stay the program's), the program as `main.hero`, T3's own module beside
   it; `heroes build main.hero -O0 -o prog` by the lane's compiler; its
   messages handed on with every path in the arm's spelling and, where it
   says `unknown_module`, a note listing the catalogue's entries as the arm
   reaches them.
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
block. **pass@1**: the first turn passes. **pass@2**: the first turn passes,
or it failed and its one second turn passes. **silent** is counted at the
first turn and at the final one. A share is over a cell's first turns that are
not void.

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

## The dry run, before any session (`dry-run.txt`)

`python3 -I .claude/worktrees/235-coordinator/blind235.py dry`, its whole
output in `dry-run.txt`: the world's fingerprints recomputed; each arm's
`heroes-spec.md` assembled; one session folder per arm and task; then **72
hand-made answers** (`dry/<arm>/<task>-<label>.md`, their expected verdicts
in `dry/expect.tsv`), one right and two wrong for every arm and task, each
wrong one a slip typical of its arm (a missing or unclosed quote, another
arm's spelling, an entry not named, a version left unquoted or read as a
number, YAML's indentation, a tab or a colon without its space, Cargo's
shorthand, Go's `require ( … )` block, a duplicate name), and nine more for
what the grader must also tell apart (silent, crash, the compiler's own
message, void, no answer, no catalogue, comments that read like headings,
the task's demand unmet, an address in a `use`); graded as a session's
answer is, from a stream-json file, each verdict and first code against its
expectation. Then the run's own steps on dry paths: `prepare` drawing 210
first turns in 42 cells of five and refusing a second draw; `grade`,
`repairs` and `score` over the 72, every second turn answered with its arm's
right answer, the totals checked; and the hard stop on a probe ledger.

## The cells the spec-warden registers

**Per arm and per model, over the arm's three tasks: pass@1 and pass@2**, a
count out of 15 first turns (fewer where a session is void): 7 arms × 2
models × 2 = **28 cells**. Each with its floor and ceiling where the warden
names one. The score also prints every task's cell, silent and crash, and the
first codes, which are read and not predicted.

**The comparisons the sitting reads**: U-angle, U-bare and U-pkg against each
other (question U); F-yaml, F-toml, F-lines and F-hero against each other
(question F); U-angle against F-toml, the A/A pair, as the noise every
other difference is read against; each by model and pooled, on pass@1,
pass@2 and silent, and T3 under U-bare and T2 everywhere read by name, since
they are where the forms differ most.

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
  are not: TOML's syntax messages are `tomllib`'s, reworded in two cases, a
  value without its quotes named before `tomllib` reads the file, and the
  other three formats' are hand-written; YAML's reader is a subset. A
  difference at pass@2 may be the messages' as much as the form's.
- **The format apart from its words**: in F, `dependencies` (TOML, YAML),
  `require` (the line format) and `package` (Heroes' syntax) come with their
  format, as the sitting's question gives them.
- **Small differences.** Five samples a task cell, fifteen an arm and model:
  one session is 1/15 of a pooled cell, and the A/A pair says how far two
  identical cells fall apart. Two models, both Anthropic's; three tasks,
  written by the coordinator, none from the held-out suite.
- **Habit apart from clarity**: a form a model has seen often (Cargo's TOML,
  Go's `require`) and a form that is clear cannot be told apart here.
- **The landed specification**: section 14 is appended, and section 1's
  `use` sentence and production stand unamended in every arm; section 13
  already uses the word *package* for a system library. Both are the same in
  every arm.
- **YAML's number trap at its full size**: a version of three numbers is a
  string in YAML whether quoted or not, so the trap fires only where a
  version is also wrong (the dry run shows it on `version: 2.1`).
- **The reader's real token count**: the figures above are the vendored
  tables'; `--refresh` was not run.
- **Anything a person reading the catalogue would feel**: only what a model
  writes is measured.

## Running it

```
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes     # in the lane, first
python3 -I .claude/worktrees/235-coordinator/blind235.py dry
python3 -I .claude/worktrees/235-coordinator/blind235.py prepare
python3 -I .claude/worktrees/235-coordinator/blind235.py run --first
python3 -I .claude/worktrees/235-coordinator/blind235.py project
python3 -I .claude/worktrees/235-coordinator/blind235.py run --jobs 6
python3 -I .claude/worktrees/235-coordinator/blind235.py grade
python3 -I .claude/worktrees/235-coordinator/blind235.py repairs
python3 -I .claude/worktrees/235-coordinator/blind235.py run --jobs 6
python3 -I .claude/worktrees/235-coordinator/blind235.py grade
python3 -I .claude/worktrees/235-coordinator/blind235.py score > score.md
```

Every path the script writes is checked to stand inside the repository's
root. The script is kept in the sitting's evidence beside its sealed key,
ledger and grades.
