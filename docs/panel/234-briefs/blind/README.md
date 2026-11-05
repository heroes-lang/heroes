# Panel 234's blind seat: the reader test

The coordinator's experiment, the paid reader test the author authorised on
2026-11-05 (*both paid runs*), designed here and committed with the briefs
before its first session, repaired on the critic's first pass (F1, F18 to F24,
F37 of `docs/panel/234-reports/critic-briefs.md`). Its materials stand
complete before the spec-warden registers its predictions, and the warden
registers them before any session runs. The seat has no agent of its own: its
sessions are its evidence, run as panel 232's were
(`docs/panel/232-briefs/blind/README.md`). It is not the llm-ergonomist's
reading of locality (`00-shared.md`, frame 6).

**What it measures**: whether a model writing Heroes from the specification
alone writes a correct program against a package, and how the package's
**names** under the author's rule (item C, question (xi)), its **interface
card** (item E, question (x)) and its **thickness over C** (item D, question
(xii)) change that, on first-try correctness and on correctness after the
compiler's messages (design.md Part 11's metrics 2 and 4), on tasks written
for this sitting, never on the held-out suite.

## Experiment S: names and cards, over `core/strings`

One package, written by the coordinator, `S/core-H/core/strings.hero`:
fourteen functions with Go's parameter names (`s`, `substr`, `prefix`,
`suffix`, `sep`, `old`, `new`), the contracts of `conventions-draft.md`, a
`test` block for each, 8 tests passing; and a second copy,
`S/core-G/core/strings.hero`, differing **only in four names**:

| contract | variant G (Go's name, kept) | variant H (the author's rule) |
|---|---|---|
| the position of a substring, `i64?` failing `not_found` (Go answers `-1`) | `index` | `position` |
| the last such position, the same | `last_index` | `last_position` |
| the text before and after the first separator, a `Halves?` failing `not_found` (Go answers two strings and a `found` flag) | `cut` | `split_once` |
| the lines, `[str]` without their ends (Go 1.24's `Lines` yields each line with its end) | `lines` | `split_lines` |

The other ten are the same in both: `contains`, `has_prefix`, `has_suffix`,
`count`, `trim_space`, `trim_prefix`, `trim_suffix`, `split`, `fields`,
`replace_all`, each with Go's contract over ASCII white space. **The four
renamed are the ones the boundary of `conventions-draft.md` §2.2 calls another
contract** (a sentinel, a `found` flag, other content). **The H names are the
coordinator's choice under the rule**, and the experiment measures the rule
with these names.

**The variants' difference, checked** (F22): G was made from H by renaming the
four as whole words (`python3`, `re.sub(r'\b' + h + r'\b', g, s)`), and
`S/check.txt` holds the two commands that show it, with their output: the
words of the two modules counted (`tr -cs 'A-Za-z_' '\n' | sort | uniq -c`)
differ only in those four, and so do the two cards'. The spec-warden reads
both cards and both modules before registering, and says whether either card
leans.

**Four arms**: `card-G` and `card-H`, where the session reads the package's
card (`S/card-G.md`, `S/card-H.md`); `bare-G` and `bare-H`, where it reads no
card. **Every arm is told its variant's rule in one sentence**, as a reader of
the packages' conventions page would know it (F37): G, *every function keeps
Go's name, in snake case, also where its contract differs from Go's*; H, *a
function keeps Go's name, in snake case, where its contract is Go's, and has
another name where its contract differs*; both, *with Go's parameter names*
and *a function that can fail answers a `T?`*. So `bare-H` against `bare-G`
measures the rule **announced**, and a bare reader who knows Go's signatures
labels its calls right (F20).

**Three tasks** (`S/tasks/S1` to `S3`, each a `task.md` closing the readings
F21 found and three cases, the first shown in the task, the second and third
hidden), each reading the path of a text file from `args()[0]`:

- **S1, a log**: for each line that starts with `ERROR `, print the part after
  its first `: `; such a line holding no `: ` prints `?`.
- **S2, a configuration**: lines `key = value`; skip a line that is empty or
  holds only spaces and tabs, and one whose first character that is not a
  space or a tab is `#`; print `key|value`, key and value cut at the first `=`
  and each without the spaces and tabs at its ends; a line with no `=` prints
  `bad: N`, `N` its line number from 1, skipped lines counted; finally `keys:
  K`, the number of `key|value` lines printed.
- **S3, words** (the control: no function it needs differs between G and H):
  print the number of words, runs between spaces, tabs and line ends, then how
  many times `args()[1]` occurs in the file, not overlapping. No case holds a
  `\r`, a `\v`, a `\f` or a non-ASCII space, where Go's `Fields` would differ.

## Experiment Q: SQLite, thin and thick

Two shapes of `core/database/sql`, both reached by `use core/database/sql`,
written by the ffi-pragmatist (`ffi-pragmatist-sql.md`), with their cards and a
reference program for each task in each shape, copied here as `Q/thin/` and
`Q/thick/`. **Two arms**: `thin` and `thick`, each with its card.

**Three tasks** (`Q/tasks/Q1` to `Q3`, a `task.md` and three cases each),
**taking their data from `args()` and reading no file**, so no answer must
split lines or turn text into an integer in Heroes, which the language offers
no built-in for (F1: `to_i64` refuses a `str`, there is no `split`); SQL
turns the text into numbers through an `INTEGER` column:

- **Q1, a sum**: the arguments are integers; insert each into an `INTEGER`
  column; print, computed by one query, `count: N` and `sum: S`, `0` when there
  are no rows. **Its second case has no argument**: SQL's `sum` over no rows is
  `NULL`, and how each shape hands a `NULL` to the program is a real
  difference of thickness, which this case measures by name (F21; the critic
  ran it in C: `sum_type_is_null=1`, `sum_as_int64=0`).
- **Q2, a filter**: the first argument is a threshold, then pairs of a name and
  an age; print, one per line, the names whose age is greater, in the order
  SQL's `ORDER BY name` gives (bytes, F21). **Its second case holds `o'neil`**:
  a program that writes a name into its SQL text instead of binding it breaks
  there, so the bound parameter the task's first version asked for and could
  not grade is graded by the output (F21).
- **Q3, a constraint**: the arguments are names, some repeated; insert each
  into a table whose name column is `UNIQUE`; print `added: A` and
  `duplicates: D`. Its third case holds `d'arc` twice.

## The card format

```
# <package>: `use <path>`

<one line: what the package is for>

## Types
<record or variant, its fields>

## Functions
- `<signature>`: <one line: what it answers; the codes it fails with>

## Constants            (where a program reads one)
## Codes
- `<code>`: <when>
```

## The cells and the sessions

**A cell is one arm on one task on one model, five samples** (F19).
Experiment S has 4 arms × 3 tasks, experiment Q 2 × 3; each on two models,
`claude-haiku-5-5` and `claude-sonnet-5-5`: 36 cells and 180 first turns. **A
first turn whose program does not build gets one second turn**, a fresh
session given its program and the messages of **`heroes build`**, the command
that judged it, never `heroes check`, which passes a wrong C binding that
`build` refuses (F18; issue 974); a program that builds and answers wrong
gets none, the compiler having said nothing. Each session is a folder under
`build/b234/<n>` (git ignores `/build`; inside the repository's root, CLAUDE.md
§ Hard stops) holding `heroes-spec.md` (the specification at the sitting's
commit, byte for byte), `brief.md`, `task.md`, the card where its arm has one,
and, for a second turn, `previous.hero` and `diagnostics.txt`; nothing else.
The order is drawn by `random.Random(234).shuffle`; the key, folder to
experiment, arm, task, model, sample, turn and parent, is written to
`.claude/worktrees/234-coordinator/sealed/key.tsv`. Each session runs from
inside its folder:

```
cd build/b234/<n> && GIT_CEILING_DIRECTORIES=<root>/build/b234 claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Answer in English." \
  --bare --no-session-persistence --system-prompt "You work in the directory <root>/build/b234/<n>. You read its files with the Read tool, by absolute path." \
  --model <model> --restricted --safe-mode --strict-mcp-config \
  --tools "Read" --allowedTools "Read" \
  --disallowedTools "Bash,Write,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 0.20 --output-format stream-json --verbose < /dev/null > <root>/build/b234-out/<n>.jsonl 2> <root>/build/b234-out/<n>.err
```

The CLI here is `claude` 2.1.311, whose `--help` lists every flag above
(checked 2026-11-05). `brief.md` asks for the program as one fenced block
marked `hero` and a section `context`. The key is the trunk's
`ANTHROPIC_API_KEY` from `.env`, never printed. The script that prepares,
runs, grades and scores is `.claude/worktrees/234-coordinator/blind234.py`,
kept in the sitting's evidence.

**The budget**: at most **0.20 USD a session** and **25 USD for the run**,
inside the author's authorisation. Panel 232's 80 sessions of the same command
and models cost 1.5590 USD, about 0.02 each (F19); these write a program
rather than a prediction, so more, and the caps stand an order of magnitude
above the last measured cost. The first session of each model runs alone and
its cost is read before the others; if the projection of the whole run passes
25 USD, every cell is cut from five samples to four, then three, before the
rest runs, which the predictions survive because they are proportions. A
session the bound stops is scored as no answer and never re-run; if the first
of a model is stopped by the bound, the run stops and the author is asked.

## Grading, mechanical

Each answer's program is placed as `main.hero` in a fresh folder beside its
arm's `core/` tree and built with the trunk's compiler, `heroes build main.hero
-O0 -o prog`; the binary runs on each of its task's three cases under
`sandbox-exec`, with the thesis harness's Darwin profile
(`tests/harness/thesis_sandbox.hero`, `profile`: nothing allowed by default, no
read under `/Users` or the temporary folders but its own folder, no network),
bounded at 10 seconds and 1 MiB of output; its standard output is compared
byte for byte with the case's truth, written by hand by the coordinator and
confirmed by the reference programs (S: both variants' references give every
truth, 18 of 18, run on 2026-11-05; Q: the ffi-pragmatist's references in both
shapes). The grader was tried on the S references, on a G answer graded
against the H tree (`unknown_in_module`, its path cut from the message), and
on a session that read outside its folder (void).

**The verdicts per answer**: `built@1`; `right@1` (built and every case right
at the first turn); `right@2` (right after at most one second turn);
**`silent`** (the final program built and answered a case wrong at exit 0);
and the first diagnostic's code where a first turn did not build.

**Read before the key is joined**: a session whose tool calls are anything but
`Read` of a path inside its own folder, or whose `context` names a project
file, rule, memory or commit, is void and counted apart.

## The comparisons the sitting reads

- **C, names**: `card-H` against `card-G`, and `bare-H` against `bare-G`, on S1
  and S2, with S3 their control.
- **E, the card**: `card-*` against `bare-*`, by variant.
- **D, thickness**: `thick` against `thin`, by task, Q1's empty case read apart.

Each on `right@1`, `right@2` and `silent`, as a share of a cell's answers, by
model and together, with the spec-warden's registered predictions scored on
the rule it registers, **a floor as well as a ceiling named** where a cell can
show nothing (F20).
