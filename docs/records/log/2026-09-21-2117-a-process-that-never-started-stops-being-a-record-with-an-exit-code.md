# A process that never started stops being a record with an exit code

2026-09-21 | the harness refuses a non-start instead of reporting one, and the
runtime hands over the operating system's own reason | the Windows CI leg lost
106 cases to a silence and neither instrument named the machine | design.md
§4.17 (a diagnostic carries what is needed to fix without opening another file)
| no panel, and § Why not a panel below says on what ground

## What provoked it

GitHub run 35620846459, the push carrying M-declared-extents step 32. The
Windows leg read **1803 passed, 106 failed** where Darwin, Linux x86-64 and
Linux arm64 each read 0 failed. The re-run of that same commit, Windows alone,
read **1903 passed, 0 failed**: intermittent, and defect 074 holds what is known
about the window itself.

This entry is about the other half — **that the two suites which went red both
said something false, and it took two hours to find out what had actually
happened.**

- `suite_run.hero` reported `did not build (exit -1):` with an empty stderr, 101
  times. `-1` is not an exit code any process produced: it is
  `shell.hero`'s sentinel beside `started: false`, which means *there is no exit
  code*. The suite read `.code` and never `.started`.
- `suite_annotations.hero` reported `the annotations and the diagnostics
  disagree` with an empty `reported:` line, 5 times. Its `provoked` answered
  `""` both for a compiler that ran and said nothing and for a compiler that
  never ran.

`shell.hero`'s own doc comment has said since M-argv-execution that `started`
**must be read first**. Measured the same day: **thirteen of the net's
twenty-four suites read `.code` and none of them reads `.started`**, 68 reads of
`.code` in all, while the compiler's own `Ran` is asked in ten places
(`selfhost/cli/`). A rule written in one document and performed by nothing is
CLAUDE.md § 3's own story, arriving in the harness.

## The decision

**A rule that can be forgotten in thirteen places is not a rule.** So the repair
is not thirteen `if`s:

- `shell.run` and `shell.run_fed` now **refuse** a child that never started,
  through one function, `insisted`, that stands between the runtime and every
  caller. The 57 call sites that only ever wanted a program's verdict get the
  refusal without being edited.
- The five call sites whose SUBJECT is the machine — `git` a record check needs,
  `/bin/sh` Windows does not have, a program named on purpose that nobody has —
  say so at the call, with `shell.attempted`. The longer name is on the rarer
  case deliberately.
- `runtime/parts/run.c` stops throwing away the number it was holding.
  `hero_run_why` answers `GetLastError()` on Windows and `errno` on POSIX,
  cleared on the way into every `hero_run_go`. On the POSIX arm the value was
  **already being read** — the child writes its failed `execv`'s `errno` down
  the report pipe and the arm looked at whether the read succeeded while
  discarding what it said.

What this costs is stated rather than hidden: a machine problem now **stops a
case** instead of being reported as the program's own verdict. That is the trade
the Windows leg priced at 106 red lines, not one of which named the machine.

## Why not a panel

CLAUDE.md § 4 convenes before changing the language: `spec/`, design.md Parts 1
to 11, surface syntax or semantics, a diagnostic **class**, or architecture.
None is touched. `hero_run_why` is a C function the **test harness** declares;
no program in `examples/` or `tests/golden/` reaches it, and the diagnostics
that changed are a suite's failure text rather than anything the compiler prints
about a program. The nearest trigger is *architecture*, and what argues against
it is that nothing a Heroes program can observe moved.

**This is written down so the author can overturn it**, which is the point of
recording the ground rather than the conclusion. If the reading is wrong the
lane is the soundness one (`compiler-engineer`, `ffi-pragmatist`), because there
is no reader-facing half for the other three seats to be differentiated about.

## What it does not close

Defect 074 stays open, and deliberately. The instrument now hands over the
operating system's own number for the refusal; **nobody has yet seen that
number**, because the failure has not recurred since. The repair makes the next
occurrence name its own cause instead of costing another two hours. It is not
the cause.
