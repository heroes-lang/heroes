# Panel 212, compiler-engineer's report

Seat: compiler-engineer (design.md §1.1, §1.7, Part 5). Tree `09060ea3`, my worktree
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-panel-212/compiler-engineer`
(compiler built from the seed, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, **7.56 s**
real; then `heroes build selfhost/main.hero` with the verb in it, **30.55 s** warm). Everything below is
measured in that tree unless it says *inference*. Nothing outside it was written but this file.

## Verdict in one paragraph

**Object, no veto.** The proposal adds **no core construct** (no lexer, parser, checker, IR, descriptor,
ownership or emitter line moves; Part 5's table is untouched), and every module I built passes `layout`
at or under 300. What I object to is three things the brief handed the seats as settled and that are not:
(1) the surface `render|plan|push [<path>...]` **does not parse in the argv table** (one operand, one
`Operand` kind per verb), (2) the fourteenth verb **breaches `selfhost/main.hero`'s ceiling** (300 of 300
today, 302 with the verb), (3) the pure half **needs no verb**: I built it as a `heroes run` program
(R11) with **byte-identical output on all 2014 files**. `plan` and `push` (network code, roughly 400
to 500 lines, *inference*) would be a permanent resident of the one binary, its seed, its help, its site
page and its net, for an act that happens once.

## 1. Does `render` build, and over how many files

Built as `heroes issues <file-or-directory>`: reads, asks `git`, writes nothing, no network (the brief's
`render`). `./heroes issues issues > all.ndjson`, one JSON object a line:

- **2014 files read, 2014 rendered, 0 refused, exit 0** (1952 closed, 62 open, as F1).
- It refuses only what a card can break: a file that does not open with its card (`issues/README.md`:
  *the file does not open with its card*, exit 1), a card with no body text to take a title from (*no
  title can be read from the file*, exit 1), an unreadable path (exit 2). **Today's tree gives it zero
  hits**, because for every gap below I had to write a reading, and no reading is "refuse". So a
  `records` check that "refuses a card the rule cannot render" (brief item 11) has **no failing card to
  show** at this HEAD; it would guard future cards only.
- **Exit contract** (`.claude/rules/cli-surface.md`): 0 clean, 1 the input has a diagnostic, 2 could not
  run, held by the three cases above (run, not argued). `GitHub refused` and `an issue differs` have no
  home in that contract for a verb that talks to a server; that is the push half's question.
- **Determinism**: two whole runs, `cmp`: identical. The same file given as `issues/...`,
  `./issues/...` and an absolute path gives **three different answers** (the operand is copied into
  `path` and into the `Record:` link), and the `Record:` link carries `git rev-parse HEAD`, so it moves
  when `main` does. Two inputs the rule must name and the verb must normalise: the repository-relative
  spelling (about 12 lines, `git rev-parse --show-prefix`) and an explicit `--pin <commit>`. Without
  them "two runs on one file give one answer" holds only for one spelling and one HEAD. Nothing else
  varies (no clock, no locale, no environment is read).

## 2. The rule I had to invent for each of D1 to D7, with the cards each touches

Every count is a note the verb itself emits per card (`notes` in the JSON), summed over the 2014 files.

| gap | the reading I wrote | cards |
|---|---|---|
| **D1 title** | heading, else a defect's bold first field cut at ` — `, else the item's second field, else (single-field) the first, else (old log row) the second field of the `YYYY-MM-DD \| ...` row, else the first line of text | heading 266, defect item 555, second field 523, single field 11, log row 642, first line 17 (sum 2014) |
| **D1 prefix** | `Defect NNN: ` unless the title already opens `Defect` (kept); `Defects 018 to 021: ` from a name that says `defects-018-to-021-` | prefix kept **110**; number from the item's `**NNN — ` where the name has none **34**; defect with no number anywhere **47** |
| **D1 numberless** | no prefix, no `Defect` field value, **title = the item's second field** (the first field is `Covered`, `panel 062`, `The record`) | 47 numberless, 44 re-titled by their second field; 617 of 664 defects carry a `Defect` field, ranges expanded (`018 019 020 021`) |
| **D1 length** | title cut at a space to 256 bytes with ` ...`, a code span closed if the cut opens one; **the full text goes in the body under `## Title in full`** | **833 cards (41%)** carry that section; max title 257 bytes. GitHub counts characters, not bytes (unverified here) |
| **D1 shared titles** | none written; a title is not a key | 1979 distinct of 2014, **26 titles shared by 61 cards** (`panel 021` 4, `panel 020` 3...) |
| **D2** ` \| ` in a code span | fields split on ` \| ` outside a code span (backtick parity) | **21 item lines** (the brief says 20 of 1219; mine is 21 of 1258); a naive split shifts every field after the span |
| **D3 closed day** | body line `**Closed D`/`Closed D`/`**CLOSED D` first; else the author date of the card's commit (`git log -1 --format=%as`); else `filed` | body **518**, commit **1433** (decision 1192, defect 176, task 48, feature 17), filed **1** (card says `commit: none`). Of the 512 body-closed cards with a hash, **114 differ from the commit's author day, 113 of them earlier** (repaired, then closed after the legs). The commit day is never before `filed` (0 negative) and equals it on 1201 of 1433 |
| **D4 `self`** | last line of `git log --diff-filter=A`; where git calls the file added by no commit, the **first commit that touches it** (`git log --reverse`) | 74 `self` cards; 72 answered by `--diff-filter=A`; **2 fall through** and both resolve to the merge `3088caa79` (verified by `git log --reverse`, run on both files) |
| **D5 labels** | `area:none` and a defect with no class get **no label** (nothing is invented) | `area: none` **297**, defect with no class **180** |
| **D6 headings** | `**Origin:**`, `**Class: x**`, `**Recommendation`, `**RATIFIED/Ratified/Verdict` become `## Origin/Class/Recommendation/Verdict`; a `## ` line stays; any other prose goes under `## Notes`; a section named *The repair*, *What was done*, *Verdict*, *What closed it* ends with `Commit: <40 hex>`; a hash written in backticks is **taken out** of the span so GitHub links it | emitted: Origin 729, The repair 493, Class 484, Notes 386, What was done 81, Prior art 46; **811 cards** had a hash unwrapped |
| **D7 task lists** | the first item line of a card becomes the sections; every other `- [ ]`/`- [x]` line is left alone and GitHub will render it as a tracked list | source: **20 cards, 59 lines** (matches the brief). Rendered: **147 cards** keep at least one task item (169 items, max 7), **11** keep two or more |

Two readings I would flag to the panel as *wrong by the rule's own words*, found only because the verb ran:
the 47 numberless defects are the old log rows filed `kind: defect` (their item's first field is a word
like `Covered`), so the rule "a defect's bold first field" gives them a junk title; and a `## Title in
full` section on 41% of the issues says the rule "item's second field" yields a paragraph, not a title.

## 3. A bare `#NNN`, an `@word`, `owner/repo#N`, a URL to another project's issue

Outside code spans and fences, titles included (a first version left titles unguarded and my own check
found it: 9 cards with a bare ref left, 10 with a foreign URL left; both 0 after the repair).

- `#NNN` and `owner/repo#N`: **put in a code span** (`` `#84611` ``). 20 cards (the brief counted 21).
  The reader loses the proportional font on the number and nothing else; GitHub does not link inside a
  span (the one fact I take from GitHub's behaviour, not run here).
- `@word`: put in a code span. 8 cards, as the brief.
- a URL `https://github.com/<o>/<r>/(issues|pull)/N` for any repository but this one: the host is
  written `redirect.github.com`. **10 cards**; 15 such URLs in 11 source cards, one inside a code span
  and so left. **Run, read-only**: `curl -sI https://redirect.github.com/ziglang/zig/issues/9194` answers
  `301 location: https://github.com/ziglang/zig/issues/9194`, so a reader loses one hop and the link
  still lands. **Unrun and unrunnable here**: that GitHub writes no cross-reference event on the other
  project's issue for the redirect host; only a write settles it. If it does not, the Prior art lines'
  URLs must go in code spans and stop being links, which changes what the credit page reads.
- own-repository URLs and `defect NNN` words are left as written (the second pass is not built).

## 4. The modules' size, in the unit `suite_layout.hero` counts

`heroes run tests/harness/main.hero -- ./heroes layout <file>` on each (4 passed, 0 failed each). One
module at 431 lines of code failed first (`layout`: *431 lines of code, past §11's 300*), and was split
at its two seams; my `awk` copy of `code_lines` agrees with the suite (it said 418 before 13 lines were
added, the suite 431 after).

| module (`selfhost/cli/`) | code lines | what it is |
|---|---|---|
| `issue_render.hero` | **286** | card, structure, title/closed/labels/comment assembly (`render`) |
| `issue_safe.hero` | 118 | refs, mentions, foreign URLs, backticked hashes |
| `issue_body.hero` | 83 | the sections below the item, the hash at a section's end |
| `issue_text.hero` | 86 | lines, find, trim, code-span-aware field split |
| `issue_title.hero` | 81 | number from a name, range, title cut, class |
| `issues_cmd.hero` | 73 | the verb: argv operand, `git`, one JSON line a file |
| `issue_git.hero` | 48 | `git rev-parse`, `git log` through `process.run` |
| `issue_json.hero` | 19 | **redundant, see 6** |
| **total** | **794** (775 without the redundant one) | 8 files, `wc -l` 1,038 |

Plus **10 lines changed in three existing modules** (`git diff --stat`): `table.hero` +3/-1 (the Tag
case, the row, the pin), `argv.hero` 3 lines (the `--` arm and the two test strings), `main.hero` 3
(the `use`, the dispatch arm, two arms widened). The seam is where the file split: the readings of one
card's words (`issue_title`), the body's sections (`issue_body`), and the guard (`issue_safe`) are each
used by `render` alone, so each seam names a concern. 1,038 lines by `wc -l` is 0.6% of `selfhost/` (**161,362 lines
in 618 modules**, `git ls-files selfhost | grep '\.hero$' | xargs cat | wc -l`); design.md §1.6 (line
395 of design.md at this HEAD) says the compiler has **no global ceiling, only the per-file one**, so the
"Pascal-P4, 4,000 lines" calibration of my agent definition describes a compiler this repository left
long ago; the live test is the per-file one, and it passes after the split.

**The seed**: `heroes build selfhost/main.hero --emit-c` is **1,346,133 lines** against `seed/heroes.c`'s
**1,333,626**, so the verb adds **+12,507 lines of generated C (+0.94%)** that the batch's regeneration
carries (no `selfhost/` or `runtime/` change since the last seed, `git log 51e6bbb9b..HEAD -- selfhost
runtime` is empty, so the difference is the verb's alone).

## 5. Time for the 2014 files (`/usr/bin/time -p`, machine still)

- **one process over the directory**: `real 11.86`, `user 6.21`, `sys 4.38`. user+sys = 10.59, so it is
  not waiting; the sys time is about 1,500 `git` spawns (the commit day of each closed card, the
  creation of 74 `self` cards), about 5 ms each.
- **2014 processes, `heroes issues <file>` each** (the brief's loop): `real 25.63`, `user 12.71`,
  `sys 9.78`, 0 failures. Each also asks `git rev-parse HEAD`.

## 6. JSON: the brief's F7 is wrong on this point

The brief says *what is missing is an object and array builder*. **It is not missing.** `selfhost/mutate/json_read.hero`
(407 lines) is a whole RFC 8259 reader (variant `Json`: `.null_value .truth .number .text .items .fields`,
surrogate pairs, a depth bound) and `selfhost/mutate/json_fields.hero` (234 lines) has `written(j)`,
`write_into`, `string`, `strings`, `whole`, `wholes` and the field readers, every string through
`cli/json_text.json_string`. They live under `mutate/` because the recovery arm wrote them (defect 210),
and any module can `use mutate/json_read`. **Cost of the builder: 0 new lines**; an object is
`.fields(names: [...], values: [...])`. My 19-line `issue_json.hero` was written before I read them
and duplicates them; a real build deletes it. The *responses* of a push (issue number, search hits) are
read by `json_read` for free, where the brief assumed a hand scan.

The body holding a quote, a backslash, a newline, a tab and a **four-byte character**: `json_text.hero`'s
own tests do **not** carry the four-byte case (they stop at an em dash and a section sign, and C1's
pair); `json_fields.hero`'s round trip carries `\"` and `\n` only. I wrote the one test that does, 8
lines, **run in a scratch root over the real `json_fields`/`json_read`, passes**: the object writes
`q\" b\\ n\n t\t 😀 end` and `read_json` returns the same value. So the answer is: no new builder, and
**one test of 8 lines, which `json_text.hero`'s own tests do not make**.

## 7. Share `tests/harness/cards.hero` or copy it

**Sharing is impossible with the module system as it is, measured**: every `use` resolves from the
directory of the file being compiled and a path is `ident/ident`, never `..`.
`use tests/harness/cards` from a `selfhost/` module is `error[unknown_module]: ... reads
selfhost/tests/harness/cards.hero, from the directory of selfhost/main.hero`. Compiled from a root at
the repository's top, `cards.hero` is found and its own `use strings` then fails (*there is no module
`strings` ... from the directory of zz_share_main.hero*). The reverse direction fails the same way.
So the choice is **copy or a different reader**, not share or copy.

What a verb needs is not the reader `records/cards` is. `cards.hero` is **763 lines, 514 in `code_lines`'
unit** (it carries `AREAS`, every vocabulary check and 11 tests), because it *judges*; the verb *reads*:
my `card_of` + `wants` are **13 and 13 lines** and read the six fields by position, and `records/cards`
stays the only judge. **The drift `records/cards` would not see**: it holds every card to FIELDS (six
lines, fixed order); my reader trusts the same positions. If a seventh field enters, `records/cards`
passes the new cards and my reader refuses all 2014 (*does not open with its card*), loudly, which is
good; but a changed *meaning* (a new `kind`, a changed `self` rule, a changed `AREAS`) passes both
checks, and `kind:<new>` becomes a GitHub label no list names. **The guard I would write**: one `records`
check, `records/render`, that runs `heroes issues issues` once (11 s) and, per card, compares what the
verb read with what `cards.read_card` read (kind, area, milestone, closed against `holds_open_item`), so
the two readers of one card in two modules disagree out loud. About 40 lines in `tests/harness`,
unrun.

## 8. R11: the same `render` as a Heroes program under `tools/`, run with `heroes run`

**Built and run.** `tools/issues/main.hero` (92 code lines) + `tools/issues/cli/issue_{text,safe,title,body,render,json}.hero`
(byte-identical copies, paths unchanged because the tool's root is `tools/issues/`) + copies of
`bytes.hero` (62) and `json_text.hero` (84). `heroes run tools/issues/main.hero -- issues > tool.ndjson`
is **`cmp`-identical to the verb's output on all 2014 files**, `real 12.71 / user 6.46 / sys 4.59` cold
(about 1 s of it the compile; warm `11.73`). A program reaches the machine with the same externs
(`extern "hero_os.h"`: `hero_run_*`, `hero_fs_*`, `hero_dir_*`) that `cli/process.hero` declares, so
spawning `git` and walking a directory cost about 36 lines in `main.hero` (22 and 14 non-blank). `heroes test tools/issues/main.hero`
runs 11 tests in **1.25 s**, all passing.

| | R1, the verb | R11, a program under `tools/` |
|---|---|---|
| new code | 775 lines in 7 modules + 10 changed | the same 6 render modules + `main.hero` 92, plus **146 lines of copied helpers** (`bytes`, `json_text`) |
| `table.hero` row, Tag, argv arm, 3 `main.hero` arms, the two argv test strings, the `14` pin | yes (10 lines) | none |
| `main.hero` 300 -> 302 (ceiling breach) | yes, needs a seam or a `DECIDED` row | none |
| site: two start pages (`thirteen` -> `fourteen`, `tredici` -> `quattordici`, `<code>issues</code>`) and a push that publishes the site | yes, 4 edits in 2 files | none |
| `heroes --help` listing | yes | **no**: nobody learns of it from the tool |
| seed | +12,507 lines of C | none (a program is not the compiler) |
| judged by the net | `layout`, `canonical` (under `selfhost`), the compiler's own tests: yes, today | **no**: `SOURCE_DIRS` in `suite_canonical.hero` lacks `tools`, `layout` reads `selfhost`, and no suite runs `heroes test tools/issues/main.hero`: **+1 line and about 15 lines in the harness** to put it under the net |
| drift | none (one copy) | **`bytes.hero` and `json_text.hero` are copied**; a fix to `json_string` (defect 290) must reach both; guard: a `records` check that `cmp`s the copies, about 10 lines |
| removable when the transcription is done | by editing 5 files and the site | `git rm -r tools/issues` |

`tools/` today holds only `tools/spike/*.c` (the four C spikes), no `.hero` program: the brief's *`tools/`
exists* is true of C spikes only. Whether a Heroes program run by `heroes run` is a "script" under
CLAUDE.md §10 and design.md §3.5 (*a second binary never exists; neither does a Makefile, a script ...*)
is the panel's, not mine; `.claude/rules/cli-surface.md` line 25 at this HEAD, not 18 as the brief says (*nothing if two existing
invocations already compose to it*) is the sentence R11 stands on.

## 9. A test seam (a transport that can be a recording)

The seam is **free in language terms and costs a design choice**. `selfhost/cli/argv.hero:73` already
takes `dir_probe: (function(str) -> bool)` and its tests pass `yes_dir`/`no_dir`; a transport is the same
(`function send(transport: (function(Request) -> Reply), request: Request) -> Reply`, 12 lines with the
two records). **Run**: that compiles and tests green. **But a function value cannot capture** (design.md
Part 6, *Closures that capture: refused on cost alone*, line 3123 of design.md at this HEAD), so a
*recording* fake cannot push the request onto a list it closes over. Two ways that work: the fake
appends to a file under `build/` (I ran it: a fake `recording` writes `build/wire-log.txt` and the test
reads it back, green, 4 lines per fake, the test touches the disk), or `push` is written as a pure state
machine, `next_request(state) -> Request` and `step(state, reply) -> state`, whose test feeds replies as
data and reads the requests as data, which is the Heroes-shaped answer and the one I recommend (about
60 to 80 lines for the machine, *inference*). The real transport is `curl` spawned as
`selfhost/cli/refresh.hero:80-82` does (`program: "curl"` at line 81), the token in a file read by `--variable name@file` (curl's file form, **unrun** here; `refresh.hero` uses the environment form `%NAME`, which the compiler cannot set for a child), never argv.

`process.run` takes **no standard input** (`selfhost/cli/process.hero:118`, `in_path: "".cstr()`; the brief cites 96-103, which is the function's top), so `gh api --input -`
is out and `--input <file>` is in. **libcurl across the C boundary is out for a stronger reason than the
brief gave**: the compiler is built by `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`
(CLAUDE.md § Commands, *the compiler, from C alone*); an `extern "curl/curl.h" link "curl"` would, *inference, not built*, put
`-lcurl` on that line on every platform (`examples/curl/main.hero` is a Heroes program that binds libcurl
and makes no request).

## 10. What a fourteenth verb costs the rest of the tree, counted

- `heroes doctor`: **0**. `./heroes doctor` prints six toolchain rows (clang, floor, CLT, arch, runtime,
  build cache) and names no verb; `site/src/lib/claims.ts` reads `doctor.hero` only for the Zen's twenty
  lines (`DOCTOR_FILE`, line 49). The brief's "doctor listing" does not exist.
- the table: **1 row, 1 `Tag` case, the pin** (`assert table.len() == 13` -> 14; the file notes each
  step of that count by date) and the help is generated from it (`./heroes` now prints `issues [file]`
  with its summary; the "help is the table spoken" test passes).
- argv: the `--` arm (1 widened line) and **2 test strings** that spell the verb list
  (`selfhost/cli/argv.hero:333, 393`; **the brief says `tests/harness/argv.hero:333, 393`, a file that
  does not exist**). Run: `heroes test selfhost/main.hero` reads **2 FAIL** until both are edited
  (*the commands are lex, parse, ... measure, issues, grammar, this*), all other tests ok.
- `selfhost/main.hero`: the `use`, one arm in the dispatch and two arms widened (a variant dispatch is an
  exhaustive match, so a verb with no arm does not compile: that is the property working). **300 -> 302
  code lines with no `DECIDED` row: `layout` reads 3 passed, 1 failed** (*selfhost/main.hero: 302 lines of
  code, past §11's 300*). A seam or a ceiling row is owed in the same change.
- the site: `verbs()` in `claims.ts:231` counts the `Command(` rows (**14 now, 13 at HEAD**) and the two
  start pages read *thirteen* and *tredici* (`site/src/html/start.html:100`,
  `site/src/html/it/start.html:100`) and list the verbs as `<code>name</code>` at line 105; **4 edits in 2
  files**, then `npm run build` in `site/` before a push (`node_modules` is not installed in my worktree,
  so I did not run it), and a push touching `site/` publishes the site (CLAUDE.md § Hard stops).
- the frozen recovery corpus carries a copy of the table (`tests/golden/recovery/corpus/selfhost/cli/table.hero.frozen`);
  `verbs()` reads `selfhost/cli/table.hero` only, so it is untouched.
- **the surface itself**: `argv.parse` takes **one operand per verb** (*`issues` takes one file, and two
  were given*, run) and each verb has one `Operand` kind; `render <file>` (a word, then a file) and `plan
  [<path>...]` (several paths) do not parse. `Invocation.file` is read at **158 sites** and
  `Invocation(` is built at **25** (`git grep -n 'Invocation(' -- selfhost tests | wc -l`), so subverbs
  or several operands are a table-shape change, not a row: an *inference* of 30 to 40 lines in `argv.hero`
  and a field on `Command`, so all 14 rows are touched. What fits today at zero argv cost is
  **`heroes issues [path]` with mode flags** (`--plan`, `--push`), the shape of `measure --refresh` (the
  one flag that reaches the network, `table.hero:148`) and `mutate --recovery`, which cli-surface.md
  allows as *a flag if it changes how one question is answered about the same input*; plan and push
  answer different questions, which is the stopping rule's own sentence against it.

## 11. What `plan` and `push` would add (inference, not built)

Request building, the response reader (free, §6), the plan's diff against what GitHub holds, the
`github:` rewrite of one card line (a `write_file`), pacing against 5,918 requests, resumption by
`field.defect:NNN` / the permalink: about **400 to 500 code lines** in two or three modules (each under
300), plus tests; `cli/refresh.hero` is **258 lines for one POST and one key**. This is the number I
would hold the proposal to: render is **775**, the whole verb **about 1,200 to 1,300**, against a
transcription that runs once.

## What the sitting should take from this

1. The pure half exists, runs on every card, and is deterministic given the spelling of the path and the
   pin. Its cost is measured, not argued.
2. R11 is real: same bytes, no verb, no table, no site, no seed, deletable; its price is a harness line
   and a `cmp` guard for two copied modules, and the panel's reading of §10's *script*.
3. If R1 is chosen: the surface is `issues [path] [--plan|--push]`, not three subverbs; `main.hero` needs
   a seam in the same commit; `issue_json.hero` is deleted for `mutate/json_fields.hero`; the pin and the
   path spelling become flags/inputs; the site's four lines ride the same change.
4. Two of the brief's facts are wrong and should be corrected in the record: F7's missing JSON builder
   (it exists), and the net's own tests that name the verbs (`selfhost/cli/argv.hero:333, 393`, not
   `tests/harness/argv.hero`).

## Files

- Prototype, in my worktree: `selfhost/cli/issues_cmd.hero`, `issue_render.hero`, `issue_title.hero`,
  `issue_body.hero`, `issue_safe.hero`, `issue_text.hero`, `issue_git.hero`, `issue_json.hero`;
  `selfhost/cli/{table,argv}.hero` and `selfhost/main.hero` edited; `tools/issues/` (R11).
- Outputs (scratchpad, not in the tree): `.../scratchpad/ce/all5.ndjson` (the 2014 renderings),
  `tool.ndjson` (R11, identical), `files.txt`, `loop.sh`.
