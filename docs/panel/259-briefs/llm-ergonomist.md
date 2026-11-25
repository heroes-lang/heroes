# Panel 259, the llm-ergonomist: REGISTERED, NOT RUN

**Nothing in this file has been run, and no session of it is started by this sitting.** No paid run is authorised
(the coordinator's brief to the clerk, 2026-11-25; the author was asked at 06:11 and has not answered, the
coordinator's brief to the repair); the blind reading is **registered**, so that whoever starts it, **on the author's
word**, runs the experiment the sitting meant. **The paid run waits for the author's yes.** The sitting's resolution is
provisional on this reading and says so. Written 2026-11-25 by the sitting's clerk at the first base `2cf85ea64`;
**repaired** the same morning from the critic's first reading (`docs/panel/259-reports/critic-briefs.md`, F14, F15,
F19, F25, its item 2) at the base `3ec775b66` (`00-shared.md` § 2): the key now names its platforms, the decision rules
read what they say on both, a *may* in a text has a registered grading, `task-t2.md` no longer points at a program its
session does not hold, the session folder's names are settled, and design C's key header is registered.

**Authorised 2026-11-25 at 09:25 by the author** (the answer to the coordinator's question of 06:11, meant as:
*the blind seat of 259, paid: 20 dollars*): the run is started by the coordinator after the synthesis, as § The area
says, with **a ceiling of 20.00 USD for the whole run** in place of the 15.00 registered below; the per-session caps
stay as registered (0.60 USD each graded session, 1.20 the verdict, margin 0.10), the runner stopping before any session
where `spent + cap + margin` would pass 20.00. The paragraphs above and below stand as they were written.

## What it measures, and why the sitting wants it

A reader who binds a C coroutine library from the specification alone must know **which of their programs keep a
value across a switch, which abort, and which the document leaves unpromised**, and which stack memory to hand the
library. The sentence under test (if the synthesis adopts one) is the specification's whole answer; the runtime's
words are the other half. The seat reads **only a copy of the specification** and the task's files (panel 175 item
7: never `design.md`, never the repository) and answers `blind/task-t1.md` (seven programs, `blind/programs/q1` to
`q7`, with `co.h`, `sw.h`, `jump.h`) and `blind/task-t2.md` (three designs for a binding). It measures a sentence's
effect on a reader, which the compiler-engineer and the ffi-pragmatist, who test truth, cannot.

**The programs are label-stripped**: `co.h`, `sw.h` and `jump.h` are the lane's `cr.h`, `own.h` and panel 258's
`ab.h` with every comment that named the runtime, a wrapper, a defect or the project taken out and the prefixes
renamed (`cr_` to `co_`, `ow_` to `sw_`, `ab_` to `jump_`); the `.hero` files carry no comment
(`grep -c '#' blind/programs/q*.hero` prints 0 for each). The critic read them for a leak and found none (F20).

## The arms, hashed at the synthesis

| label | the spec the session reads |
|---|---|
| `now` | `spec/heroes-spec.md` at the base `3ec775b66`, sha256 `c915f0f2d47f35c6b3162d198c27bedfbb3fe2b962e05185a878619356372bee` (`shasum -a 256`; the same bytes at the first base `2cf85ea64`) |
| `adopted` | `now` with **the resolution's sentence** applied, if the synthesis adopts one |
| `rival` | `now` with **the second-ranked text** the synthesis names |

The synthesis writes both texts and their sha256 into `blind/registration.md` **before any session**; the run refuses
to start where a hash differs. **Three arms, not four**: a fourth text is a second sitting. **If the synthesis adopts
no sentence**, the run's arms are `now` and `rival` alone (the sentence the sitting refused), and D1 below reads
*rival moves a reader* as the reason a sentence was weighed and left out.

**Each session's folder holds `brief.md`, `task.md`, `spec.md` and, for t1 and t2, `programs/`**, and nothing else:
`brief.md` is the session brief (`brief-session.md` of the runner, which names its inputs `spec.md`, `task.md`,
`programs/` and gives the report's structure), `task.md` the task (`blind/task-t1.md` or `task-t2.md`), `spec.md` one
arm, never named. **These are panel 257's names**: panel 256's `make_sessions.py:35` writes `brief.md`, `task.md` and
`heroes-spec.md` (`grep -n brief docs/panel/256-briefs/blind/make_sessions.py`), and panel 257's adaptation renamed
the spec `spec.md` and added `programs/` (its `tools/registration-runner.md`, the `make_sessions.py` and
`brief-session.md` bullets, inside `docs/panel/257-reports/llm-ergonomist-blind-answers.tar.xz`, `tar -xJOf`). The two
tasks say `spec.md` and *"the structure `brief.md` gives"*, which those names make true. The labels live in a key
outside every session's folder.

## The key, its platforms and what a right answer is

**The key is the landed build's own run, three runs of three, on this Mac (Darwin arm64) and in the Linux arm64
image**, the two platforms `task-t1.md` names; the x86-64 image (emulated) is recorded in `answers.md` (the critic's
runs at `2cf85ea64`, F25), never graded. A program whose two graded platforms give different verdicts under the
build of an arm **has no key under that arm**: it is named in the scoring and is out of every rule below that reads
it. Today (`now`, at `3ec775b66`) q4 is such a program (`blind/answers.md`): this Mac prints `body holds 11` twice
then aborts with *compiler bug*, the Linux arm64 image ends it by SIGSEGV with nothing printed (and so did the x86-64
image at `2cf85ea64`).

**The verdict of a run**, read from its output by one rule: `WRONG` where any line it prints is not what a correct
reading of its own code prints, **whatever follows** (so this Mac's q4 today is `WRONG`, never two words); else
`ABORTS` where it ends before its end saying why; else `PRINTS` with its lines. A run that ends by a signal with
nothing said (q4 on Linux today) is none of the four words; no reader is graded against it.

**Right, two ways**, each answer graded on both:

- **build-right**: the reader's verdict is the key's (for `PRINTS`, with the lines; an `ABORTS` names the cause in the
  reader's words and is graded by whether it names the switch or the stack, never by wording). A reader who gives the
  Mac and Linux each a verdict is build-right where both are the key's.
- **text-right**: the reader's `NOT PROMISED` for a program the synthesis has classified, **before any session**, as
  inside a sentence of the session's arm that says it *may* abort (its class read from the text alone, never from the
  build), the classification written into `blind/registration.md` arm by arm and program by program.

`NOT PROMISED` is never build-right; it is counted beside, as the reader's honest *the document does not say*.

## The rows

- **t1**: seven programs, **5 sessions per arm**, 15 sessions, graded by `blind/answers.md` (each program's verdict,
  build-right and text-right).
- **t2**: three designs and a question about a told switch, **5 sessions per arm**, 15 sessions. Its key for design
  **A** is q4's under the landed build (**UNRUN** until that build exists); **B** is q3 (runs today, both graded
  platforms); **C** is q3 built with the registered `blind/design-c/sw.h` (`mmap`ed stacks, a `PROT_NONE` page below
  each), which runs today on both (`answers.md`). Question 2 (*which design do you write*) is scored only where the
  landed build separates the designs; question 3 is scored against the adopted text (a function named, or none).
- **verdict**: one session reading all 30 reports and writing the seat's verdict in `.claude/agents/llm-ergonomist.md`'s
  structure, **31 sessions in all**; its folder is built **after** the 30 and holds the two specs, a diff and the
  reports, never a key.

## The decision rules, registered before any session

- **D1, the sentence teaches the defect's outcome**: under `adopted`, at least 4 of 5 readers are build-right on q4;
  under `now`, at most 2 of 5 give **that same verdict**, the adopted build's (under `now` q4 has no key of its own:
  it splits, above). `NOT PROMISED` is no prediction and counts in neither half, so **a text that says q4 *may* abort
  cannot pass D1**: that is the measurement of its hedge, registered now, and an arm that would pass owes *aborts*
  (or a promise) for q4's class. **If the landed build's q4 splits between the graded platforms**, D1 has no key and
  the run waits: a resolution under which the defect's own program is two programs has not been built. A D1 that
  fails says the sentence does not teach what it says, and the sitting is reopened on the text.
- **D2, the sentence never makes a reader think a program that keeps its value does not**: under `adopted`, for each
  of q1, q2, q3, q6 and q7 (each prints on both graded platforms today), the readers who answer `ABORTS` or `WRONG`, or
  `PRINTS` with other lines, rise by **no more than 1** against `now`. Failing withdraws the sentence until repaired.
- **D2w, the width of a *may***: under `adopted`, for each of those five, the readers who answer `NOT PROMISED` rise
  by no more than 1 against `now`. It is where a *may* whose class holds a program that keeps its value shows (q7,
  512-byte records on q4's switch, lies inside `arm-r`'s and `arm-t`'s class as the clerk drafted them, critic F18 and
  F19); failing it, the synthesis narrows the class before the sentence lands. D2w withdraws nothing by itself; it is
  scored apart from D2 so that a hedge is never read as a reader's mistake, nor a reader's mistake as a hedge.
- **D3**: under `adopted`, at least 4 of 5 t2 readers are build-right on design A (the same key rule as D1: a split
  build has no key and the run waits; `not promised` counts as no prediction), and at most 1 answers `a wrong value`
  or `the program aborts` for design B or C.
- **What the run does to the resolution**: D1 or D3 failing moves the resolution to `rival` where it satisfies both
  and `adopted` does not; D2 failing **withdraws the sentence** until repaired; D2w failing narrows its class. The
  warden's predictions (its report, `## Predictions registered before the blind run`) are copied and hashed as the
  run starts.

## The size, the spend and the cap

- **Size**: **31 `claude -p` sessions** (t1 and t2, three arms, five each, and the verdict).
- **The cap per session**: **0.60 USD** for each of the 30 graded sessions, **1.20 USD** for the verdict, margin 0.10;
  **a ceiling of 15.00 USD for the run**, the runner stopping before any session where `spent + cap + margin <=
  ceiling` fails. The ceiling is the clerk's proposal and **the author's to change**.
- **The price it rests on**: panel 257's blind run, **31 sessions for 6.297764 USD** (carried from
  `docs/panel/257-reports/llm-ergonomist-blind.md`, its *How it was run*, lines 12 and 47, read by `sed -n 1,47p`), a
  mean of 0.203 a session. This run's t1 inputs are larger: the seven programs and three headers are **17,929 bytes**
  (`cat blind/programs/* | wc -c`, at this base) against panel 257's registered `prelude.h` and `p1.h` to `p7.h`,
  **6,485** (`cat ... | wc -c`), beside a spec of 34,437 bytes; t2 reads `co.h` and `sw.h` alone, **6,550** (`cat ... |
  wc -c`). Scaled by input bytes, about **6.3 to 8.5 USD** for the 31: **an inference, unrun**.
- **The runner** is panel 256's (`docs/panel/256-briefs/blind/`: `run.sh`, `preflight.sh`, `scan.py`,
  `check_area.py`, `check_folders.py`, `make_sessions.py`, `read_runs.py`, `spent.py`, `after.py`, `reads.py`,
  `brief-session.md`, `brief-verdict.md`, listed by `ls`) **adapted as panel 257 adapted it**: its diffs are
  `tools/registration-runner.md` inside `docs/panel/257-reports/llm-ergonomist-blind-answers.tar.xz` (`tar -tJf`).
  The files that change are registered with a `diff` against panel 256's committed bytes **before any session**, in a
  `registration-runner.md` beside `registration.md`, which keeps panel 257's folder names (above) and adds the
  grading of § The key: `answers.md`'s per-platform rows, the verdict rule, build-right and text-right, and D2w.

## The area, and every session

- **The folder**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/259-llm-ergonomist/`, **drawn at the
  synthesis and not before**: the top of an empty git repository of its own (`git init`, `.gitignore` `*`, local user
  `reader` <`reader@invalid`>, no commit), inside this repository's root and ignored by it, with `home/` and `tmp/`
  and one folder per session holding only its registered files.
- **Every session** is the command of `.claude/skills/panel/SKILL.md` § Procedure (`claude -p ... --restricted
  --safe-mode --strict-mcp-config --tools "Read,Write" --allowedTools "Read,Write" --disallowedTools ...
  --max-budget-usd <cap>`, and `--bare` with `--model claude-opus-5-5` as panel 257's run took them), with `<
  /dev/null`, `HOME`, `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the area, **after the free preflight** (a key the API
  refuses, cost 0, its scan clean), and counted a success only where `subtype` is `success`, `is_error` false and
  `terminal_reason` `completed`. `claude --version` prints `2.1.323 (Claude Code)` on this Mac (07:45), the version
  panel 257's run used.
- **Never started by a seat, by the critic or by this sitting.** Whoever starts it needs the author's word.

## What every session must say (the brief asks it, in `brief-session.md`)

Whether any project rule, contract or memory reached its context (a yes voids the session), and **every place where
the specification left it a choice, the choice made, and what the other would produce** (`choice_points`), never its
hesitations or its tools.
