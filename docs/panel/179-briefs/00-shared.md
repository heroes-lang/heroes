# Panel 179, shared brief: the formatter's probe becomes a capability of the one command

Written 2026-09-27 at 00:05, by the coordinator, for every seat, on the
author's instruction of the evening before. Every number
below names the command that produced it, run while this brief was written;
the numbers marked CARRIED were not re-run tonight and say where they come from.

## The sitting's shape, and why

**Retro-record.** The author decided in conversation on 2026-09-26 (in Italian,
meant as: *put it in, I am telling you to; convene the panel meanwhile*) that
the probe enters the tool. What the sitting decides is its SHAPE, its cost and
its place in the harness, not whether it exists. Record real objections; do not
stage dissent.

**Three seats, not five**, on CLAUDE.md § 4's clause *choosing only the seats
whose input differs* (CL-023): the llm-ergonomist's input is the spec alone and
the proposal changes no spec sentence, and the ffi-pragmatist's input is the C a
binding needs and the proposal reaches no C. Both exclusions are written here so
the author can overrule them. A completeness critic runs over the three reports.

**Sitting 179**, because 178 sits in the peer session's lane
`/Users/joseph/Temp/heroes-lane-panel-178` and is not on the trunk yet
(`ls docs/panel | grep -E '^1[78][0-9]'` on the trunk lists 170 to 177).

## The proposal, in ten lines

The generators three skeptic seats wrote tonight as throwaway Python, which
found every silent comment move and every refusal defects 096 to 101 record,
become a capability of `heroes`: from one `.hero` file the parser accepts, it
generates the variants the seats generated (a comment inserted at every
position, trailing and own-line, at several indents; every bracket broken after
every token with a comment; CRLF and no-final-newline copies), runs the
formatter on each, and checks exit 0, a fixpoint, the same tree, and every
comment keeping its owner block and its neighbour tokens; it reports the first
variant that fails with its text; exit 0 when every parsing variant holds, 1
when one fails, 2 when the tool cannot run. Flag on `fmt` or subcommand: the
sitting's question, under `.claude/rules/cli-surface.md`'s stopping rule.

## Where things stand, measured

- The trunk is `fa813325`, clean: `git log -1 --format=%h; git status --short | wc -l` read `fa813325` and `0`.
- The formatter's guard the probe would reuse is NOT on the trunk. It is in lane
  g, committed as `83ac68c1` in `/Users/joseph/Temp/heroes-lane-gm`, whose
  working tree also holds 30 uncommitted files of a fifth repair round (`git
  status --short | wc -l` there read 30). **Every seat copies the committed
  state, never the working tree**: `git -C /Users/joseph/Temp/heroes-lane-gm
  archive 83ac68c1 | tar -x -C <your directory>`.
- Line counts at `83ac68c1`, by `git -C /Users/joseph/Temp/heroes-lane-gm show 83ac68c1:<file> | wc -l`:
  `selfhost/cli/syntax_cmds.hero` 410, `selfhost/print/owners.hero` 345,
  `selfhost/print/anchors.hero` 363, `selfhost/print/page.hero` 451,
  `selfhost/print/fmt.hero` 1636. The `layout` suite counts non-test, non-blank
  lines against a ceiling of 300 (`grep -n 'constant CEILING' -A1
  tests/harness/suite_layout.hero` reads 300 at line 44) with a table of files
  decided over it; `wc -l` is not that count.
- On the trunk, `selfhost/cli/syntax_cmds.hero` is 340 lines (`wc -l`);
  `run_fmt` at line 40 and `refuse_output_the_formatter_broke` at line 102
  (`grep -n '^function'`); the trunk's guard checks parse, fixpoint and tree.
  At `83ac68c1` the same file adds the comment-anchor check
  (`grep -n 'anchors\.'` on the shown file hits at lines 101 to 105 in its
  comment and below).
- The one argv table: `selfhost/cli/table.hero`, `fmt_flags` at line 106 (one
  flag today, `--in-place`), the `fmt` command row at line 130
  (`grep -n fmt_flags selfhost/cli/table.hero`).
- Rows of `tests/harness/suite_surface.hero` whose argv starts with `fmt`:
  4 on the trunk, 42 at `83ac68c1` (`grep -c 'argv: "fmt'`).
- `.hero` files under `selfhost`, `tests` and `examples` on the trunk: 886
  (`find selfhost tests examples -name '*.hero' | wc -l`).
- The seats' generators, recovered from their transcripts after tonight's
  reboot wiped the scratchpad, at `/Users/joseph/Temp/heroes-recovery-2026-09-26/`
  (`wc -l`): `skeptic-g4/skeptic-g4__gen.py` 138, `skeptic-g4__tokgen.py` 93,
  `skeptic-g4__rd.py` 241 (the independent owner-block checker),
  `skeptic-g5/$S__parengen.py` 85, `skeptic-g5/$S__rd2.py` 64. Read them: they
  are the specification of what the probe generates and checks.

## CARRIED numbers, not re-run tonight

From the seats' reports in the coordinator's session, which are not on disk:
the bracket-break generator made 7,464 variants from 25 seed files and 123,822
from a wider seed set; the parenthesis generator 15,228 parsing variants over
the gallery, examples and fixtures; the multi-comment generator 8,838 over the
whole tree. The third seat timed `fmt` on `selfhost/check/walk.hero` at 0.29 to
0.31 s user on a quiet machine before the reboot. The machine is loaded now
(`uptime` read 9.34 on 8 cores, another agent's suites), so **no seat times
anything tonight unless it first reads a load under 2 and says so**; a timing
taken under load is written as unrun.

## Rules that bind this sitting

- `.claude/rules/cli-surface.md`, the stopping rule (panel 016): a capability
  enters only if the fixpoint invocation, the golden harness or the Part 11
  harness must type it; a subcommand answers a different question (another
  artifact class), a flag changes how one question is answered about the same
  input; never a second binary, never a script.
- CLAUDE.md § 10 and § Precedence: robustness beats speed; the most robust and
  production-ready resolution, never the cheapest.
- CLAUDE.md § RUN IT: a number is measured in the session that writes it; a
  negative claim goes out as a question naming what was searched.
- Every seat works in its own directory `<scratchpad>/179-<seat>/` where
  `<scratchpad>` is
  `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e7e3d0df-8db3-44f2-b628-af9bb1a6080e/scratchpad`,
  builds its own compiler there from the seed (`clang -O2 -I runtime
  seed/heroes.c runtime/runtime.c -o heroes`, then `rm -rf build`), never uses
  the trunk's `./heroes` or another seat's, and never reads or writes another
  seat's directory, the trunk, or any lane. The trunk is frozen until the
  synthesis.
- **The scratchpad was wiped by a reboot tonight.** Each seat writes its report
  to its own directory AND a copy to
  `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-179/<seat>.md`, which is
  on the real disk; the coordinator moves the reports into
  `docs/panel/179-reports/`.
- English, no em dashes. A claim that was not run is written as unrun.
