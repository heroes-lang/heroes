# Panel 179, compiler-engineer seat: the formatter's probe as a capability of the one command

Written 2026-09-27, 00:05 to 00:40, in `<scratchpad>/179-compiler-engineer/`, a copy of
`83ac68c1` (`git -C /Users/joseph/Temp/heroes-lane-gm archive 83ac68c1 | tar -x`), with its own
compiler built from the seed (`clang -O2 -I runtime seed/heroes.c runtime/runtime.c -o heroes`,
exit 0, 4,999,960 bytes). The machine read `uptime` load 7.40 at the start, 16.12 at the end, so
**every timing below is unrun as a clock**: only user seconds are quoted, and marked as taken under
load. Every count names its command.

- `verdict`: **object** (the shape and the cost I approve; two sentences of the proposal I do not)
- `section`: design.md §1.1 (the ceiling), §1.7 and Part 5 (it is not a construct: zero lines in the
  checker, the lowering, the descriptors, the ownership pass or the emitter), §3.5 (one command),
  Part 11 metric 3 (mutation operators as data, the precedent), §4.15 (the formatter is mandatory and
  makes any textual difference semantic, which is what the probe defends). The stopping rule's
  admission clause (`.claude/rules/cli-surface.md`, panel 016) does not cover a test instrument for a
  tool; I say so rather than invent a clause. Existence was the author's decision of 2026-09-26.
- `implementation_cost`: about **360 counted lines, three new modules, all reuse**, plus about 15
  lines of table, dispatch and harness rows. Grounded in the prototype I wrote and ran tonight,
  `selfhost/zz_probe179.hero`: **309 lines by `wc -l`, 241 by `suite_layout`'s rule** (a replica of
  `code_lines`, `tests/harness/suite_layout.hero:559-570`; the suite itself ran green with the file
  in the tree, `heroes run tests/harness/main.hero -- ./heroes layout`: `layout: 2 passed, 0
  failed`). It holds both deformations, the in-process judge and its text helpers in one file. The
  product splits it: `selfhost/probe/deform.hero` about 170 counted lines (the comment insertion, the
  bracket break, the CRLF and no-final-newline copies); `selfhost/probe/judge.hero` about 100 (the
  runner, the tallies, a site report); `selfhost/cli/probe.hero` about 90, modelled on
  `selfhost/cli/mutate.hero` (154 counted, 231 `wc -l`), which already walks a directory with
  `process.files_under`. `selfhost/cli/table.hero` (237 `wc -l`): one `Tag`, one `Command` row, two
  flags. `selfhost/main.hero` (237): one `use`, one match arm, and the exhaustive match refuses a row
  without an arm. `tests/harness/suite_surface.hero`: three rows. Reused unchanged: `parse.parse`
  (`selfhost/parse.hero:37`), `fmt.format_lexed` (`selfhost/print/fmt.hero:124`),
  `refuse_output_the_formatter_broke` (`selfhost/cli/syntax_cmds.hero:125-161`, 38 lines, calling
  `anchors.first_moved` at 155), `owners.shape` (`selfhost/print/owners.hero:74`). The prototype
  `use`s `cli/syntax_cmds` directly and compiles; the cleaner seam moves those 38 lines to
  `print/guard.hero`, moved not new. The compiler at `83ac68c1` is 72,110 lines of Heroes under
  `selfhost/` (`cat $(find selfhost -name '*.hero') | wc -l`, my probe excluded); 360 is 0.5%.
  `syntax_cmds.hero` is 151 counted lines, not 410: the brief's worry was `wc -l`.
- `needed_for_self_hosting`: **no**
- `argument`: Neither core nor sugar: no construct, no pass touched; tool surface (§3.5), Part 11
  metric 3's shape turned on the formatter. A subcommand: its artifact is a report over generated
  programs and its operand a file or directory, which no `fmt` flag can express (`cli-surface.md`;
  `mutate` is the precedent). About 360 lines, all reuse; I wrote and ran it tonight. I object to
  two sentences: owner blocks judged by `anchors`, the printer's own rule, see a broken walk and
  never a wrong rule, which is what the seats' independent reader found (`d02`); and a failing
  variant's text is 108,422 bytes for a compiler seed. And the whole-tree run cannot be in the net:
  about two million variants by tonight's counts.
- `prediction`: at the step that lands the probe, `suite_layout` counts each of its three modules at
  or under 300 lines and the three together at or under 420, and `heroes probe
  selfhost/check/walk.hero` reports **39,731** variants for the comment and bracket deformations
  (10,364 + 29,367), the number the Python generators and the Heroes prototype both produced tonight.
  Scored by `heroes run tests/harness/main.hero -- ./heroes layout` and the probe's own summary line.
  A second number to take then, not a prediction: the fully judged run over `walk.hero` against
  `83ac68c1`'s printer refuses at least 1% of the parsing bracket-break variants (tonight's 1-in-100
  sample: 5 of 225).
- `condition`: I withdraw the objection if the synthesis (a) keeps an independent owner model as a
  second judge (`skeptic-g4__rd.py`'s `model`, 241 Python lines, about 250 in Heroes, a fourth
  module) or writes in the module doc that the probe sees walks and not rules; and (b) reports a site,
  the first differing lines as `mutate/score.hero:217 locate` does, never the text. I would object
  again, not veto, to a `fmt` flag, and to the whole-tree run in the net at its measured size.

## 1. The shape under the stopping rule

`.claude/rules/cli-surface.md`: *a subcommand if it answers a different question, meaning a
different artifact class; a flag if it changes how one question is answered about the same input;
nothing if two existing invocations already compose to it.*

- **Not nothing.** No invocation generates the variants; the harness is a program and not a
  subcommand precisely because `heroes run` and the compiler compose to it
  (`tests/harness/main.hero:9-11`), and a harness suite cannot judge in process: its `use` paths
  resolve from `tests/harness/` and it spawns the compiler under test, which is the 100,000 spawns
  the brief rules out.
- **Not a flag.** `fmt`'s artifact is the canonical text on stdout (`help.hero:42`, *the artifact on
  stdout*); the probe's is a verdict and a site. `fmt`'s operand is `.file_operand`
  (`table.hero:130`) and the probe wants `.optional_operand`, a file or a directory, as `mutate` has
  (`table.hero:132`). The table has no way to let a flag change the operand kind or the artifact.
- **A subcommand**, and the precedent is in the table: `mutate`, *make one plausible mistake per
  site and count what the compiler catches*, the Part 11 metric 3 shape. The probe is *make every
  plausible comment placement per site and count what the formatter keeps*, with the opposite
  polarity (a refusal is bad, not good), so it is not an operator of `mutate`. Which sitting admitted
  `mutate` as a verb I did not find: `grep -ln mutate docs/panel/*.md` hits 011, 022, 026, 027, 029,
  034, 038, 040, 041 and 044, none titled for it, and `table.hero:1` cites panel 016 for the table.
  A question for the coordinator, not a premise.

What `heroes --help` would print, in `help.hero:21-48`'s format. As a subcommand:

```
  probe [file]
      deform a file at every place a comment can go and check that `fmt` keeps each one (default: tests/golden/surface-fixtures)
      --stride <n>  judge every n-th variant, deterministic, for a bounded run
      --site  print the first failing variant's site, the first lines the two texts differ on
```

As a flag:

```
  fmt <file.hero>
      print the canonical form
      --in-place  rewrite the file instead of printing it; prints nothing, and refuses a file it cannot parse
      --probe  print no canonical form; deform the file at every place a comment can go and report whether `fmt` keeps each
```

The third line of the flag form contradicts the summary above it and the contract below it. Exit
codes are the tool's own (`help.hero:43-44`): 0 every parsing variant holds, 1 one failed and its
site is on stderr, 2 the tool could not run (the seed does not parse, the path cannot be read).

## 2. Where it lives and what it reuses

| module | counted lines | what it is | reuses |
|---|---|---|---|
| `selfhost/probe/deform.hero` | ~170 | the deformations as data: trailing comment per token line, own-line comment at six indents per line, the two end comments (`gen.py` single), the three bracket breaks after every in-bracket token (`tokgen.py`), CRLF and no-final-newline copies | `lexer.lex_one`, `source.new_source`, `source.floor_start` |
| `selfhost/probe/judge.hero` | ~100 | parse the variant, format it, run the four checks plus the printer's own word, tally, keep the first site | `parse.parse`, `fmt.format_lexed`, `refuse_output_the_formatter_broke`, `anchors.first_moved`, `owners.shape` |
| `selfhost/cli/probe.hero` | ~90 | argv, the walk over a file or a directory, the stride, the report, the exit code | `cli/mutate.hero`'s shape, `process.files_under`, `cli/argv`, `cli/io` |

The prototype's own split: comment insertion 60 lines, bracket break 40, judge 45, helpers 80, of
its 241 counted. Not ported tonight: `gen.py`'s `multi` mode (the all-lines and dedent-run forms,
about 40 lines more) and the parenthesis generator (`parengen.py`, 85 Python lines, about 50 more).

## 3. The prototype, run on one seed

`heroes run selfhost/zz_probe179.hero -- selfhost/check/walk.hero [judge [stride-word]]`; the
Python count is `py/count.py`, the recovered `gen.py` and `tokgen.py` imported unchanged and lexing
through this seat's `heroes lex --dump-tokens --json` (20,223 tokens, the same stream the Heroes
side reads in process).

| seed `selfhost/check/walk.hero`, 2,292 lines, 20,223 tokens | Heroes | Python | difference |
|---|---|---|---|
| comment insertion (`gen.py` single) | 10,364 | 10,364 (T 1,588, O 8,774, END 2) | 0 |
| bracket break (`tokgen.py`) | 29,367 | 29,367 (T, O, B 9,789 each) | 0 |
| total | 39,731 | 39,731 | 0% |

Judged in process, every 100th variant (398 judged):

| deformation | judged | do not parse | hold | refused |
|---|---|---|---|---|
| comment insertion | 104 | 0 | 104 | 0 |
| bracket break | 294 | 69 (23%) | 220 | **5** (2.2% of the parsing ones), all *not a fixpoint* |

The first refusal, `O5310`, is a comment on its own line between `token.` and `Span` in a
parameter's type at `walk.hero:656`, which parses; the shipped command agrees:
`./heroes fmt build/c101_first_arms.hero` exits 2, *not a fixpoint on its own output*. (My fixture
loop swept the same `build/` file and prefixed it `arms.hero`; the guard's message names
`walk.hero`, and the tags 5,310 to 15,913 are `walk.hero`'s token indices.) The other four,
`O5792 B12115 B15239 T15913`, are not saved: the prototype keeps the first text only.

The 30 `comments101` fixtures (`ls tests/golden/surface-fixtures/comments101/*.hero | wc -l`, the
brief's 38 read 31 entries with the README), 851 lines, every variant judged: **7,154 variants,
1,275 do not parse (17.8%), 5,867 hold, 12 refused** in two files, all bracket breaks, none comment
insertions. `innermost.hero` T/O/B31, a break after `ys[` before `(`: not a fixpoint; T/O/B39: the
output does not parse, `expected_index_close`. `parens.hero` T/O/B59, a break after `(` before a
unary `-`: *would move the comment on line 25*; T/O/B116 the same on line 36. Both first texts
confirmed through `./heroes fmt` at exit 2. Whether the lane's uncommitted fifth round already
repairs any of the 17 is unrun: its working tree is out of bounds for a seat.

## 4. The cost of running it

Per variant, in process: three parses (the probe's, the guard's of the output, the guard's of the
input), two formats, two dumps, two `owners.shape` and two `keepable`: about two `heroes fmt` of the
seed less process start. A probe entry point taking the `ParseOutput` saves one parse, about a
sixth.

Measured under load, user seconds only, unrun as a clock: `closeparen.hero` (36 lines, 205 tokens,
479 variants) fully judged through `heroes run`: 24.90 s user; the same in count mode: 7.80 s; the
front end alone, `heroes check selfhost/zz_probe179.hero`: 2.06 s; `heroes fmt` of the fixture:
0.00 s. So about **36 ms user per judged variant on a 36-line file**, which is not the linear
scaling of the carried 0.3 s per 2,292 lines: a fixed per-variant cost dominates on small files and
I could not profile it under load.

From counts, the tree at `83ac68c1`: 933 `.hero` files, 125,884 lines, 663,924 non-layout tokens
(`find selfhost tests examples -name '*.hero'`, `wc -l`, `heroes lex --json` per file, summed).
Variants per line 4.52 and per token 1.45 on `walk.hero`, 8.4 per line on the small fixtures: **1.5
to 2.2 million variants** for the tree. Cost scales with the sum of squared file lengths, 71.7
million (2,292-line files cost 0.5 s a variant, 36-line files 36 ms): tens of CPU hours by either
scaling, against a net of 13 to 20 minutes (`.claude/rules/verification.md`). `walk.hero` alone is
39,731 variants at about half a second: five to six CPU hours, and tonight's 398-variant sample took
five wall minutes under load.

So: **the whole-tree run is neither net nor nightly as proposed.** It is an explicit `heroes probe
<file>` a repair session runs on the file it touched, the way the seats ran theirs, and a nightly
over the tree only if the author wants a day of CPU spent. The net can hold the fixtures slice,
7,154 variants, but at 36 ms each that is 4.3 minutes user, beside `canonical`'s 12 s; it enters the
net only strided (`--stride`, deterministic so a red reproduces) or after the fixed per-variant
cost is found and cut.

## What was not run, and what was searched

Unrun: every wall clock (load 7.40 to 16.12 across the session); the four unsaved `walk.hero`
refusals' shapes; the lane's fifth round against these 17. Searched and not found: a sitting that
admitted `mutate` as a verb (`docs/panel/*.md` for `mutate`). Not measured: `gen.py` multi mode and
`parengen.py` counts on `walk.hero`, since the brief asked for two deformations.

Files: `<scratchpad>/179-compiler-engineer/selfhost/zz_probe179.hero` (the prototype),
`py/count.py` (the Python count), `judge100.out`, `c101.out`, `c101.failures`,
`build/c101_first_{arms,innermost,parens}.hero` (the three first-failure texts), `tree_totals.out`,
`layout.out`.
