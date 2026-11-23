# Panel 257, the llm-ergonomist: REGISTERED, NOT RUN

**Nothing in this file has been run, and no session of it is started by this sitting.** The author's order for
this sitting (2026-11-23) is that no paid run is authorized; the blind reading is therefore *registered*, so
that whoever starts it, on the author's word, runs the experiment the sitting meant. **The sitting's resolution
is provisional on this reading**, and says so. Written 2026-11-23 by the sitting's coordinator, and repaired
after the critic's first reading (`257-reports/critic-briefs.md` B3 to B8, B18, B20).

## What it measures, and why the sitting wants it

A reader of the spec who writes a Heroes binding to a C library that installs a signal handler and leaves it
by a jump must know **which of their programs run and which abort, and whom the abort accuses**. The sentence
under test is the spec's whole answer; the runtime's words are the other half. The seat reads **only a copy of
the specification** (panel 175 item 7: never `design.md`, never the repository) and answers `blind/task-t1.md`
and `blind/task-t2.md`. It measures a sentence's effect on a reader, which the compiler-engineer and the
ffi-pragmatist (who test truth) cannot.

## The arms, hashed at the synthesis

| label | the spec the session reads |
|---|---|
| `now` | `spec/heroes-spec.md` at `828567a30`, sha256 `15804f3fb52a62c208bc59ef...` (`shasum -a 256`) |
| `adopted` | `now` with the **resolution's sentence** applied (`257-reports/spec-sentence.diff`) |
| `rival` | `now` with the **second-ranked text** the synthesis names applied |

The synthesis writes both texts and their sha256 into `blind/registration.md` **before any session**; the run
refuses to start where a hash differs. **Three arms, not four**: a fourth text is a second sitting. **The
spec-warden predicts per candidate text** (`arms/arm-f1`, `f1b`, `f2`, `f3` and its own, critic B20), so that
whichever two the synthesis hands to `adopted` and `rival` has its row.

## The rows

- **t1**: seven programs (`blind/programs/p1` to `p7`), **5 sessions per arm**, 15 sessions. Graded by
  `blind/answers.md`: `right(p_i)` per program (RUNS or ABORTS). **The grading is the landed build's own run,
  on both platforms**: a program whose two verdicts differ (p7 until defect 2702 lands on Linux) is graded by its
  own row and named, never by one, and is out of D1 while it splits. A run is `RUNS` where it prints `answer ran
  for 7` and `result 70`; p6 now ends so that a run that is not refused prints `result 70` (critic B8).
- **t2**: three designs and one abort to read (`blind/task-t2.md`), **5 sessions per arm**, 15 sessions. **Its key
  is UNRUN**: no program of the three designs exists (critic B7); it is graded by the landed build's run of the
  ffi-pragmatist's three timer-driven programs (its item 3c, kept in `257-reports/programs/ffi-pragmatist/`),
  which must exist before t2 is drawn. Question 2 (*which design do you ask for*) has no decision rule and is
  **dropped from the scoring** (it stays in the task as context); questions 1 and 3 are scored.
- **verdict**: one session reading all 30 reports and writing the seat's verdict in the agent file's structure,
  **31 sessions in all**; its folder is built **after** the 30 (panel 256's verdict folder held the two specs, a
  diff and a task, never reports) and is not drawn by `make_sessions.py`.

## The decision rules, registered before any session

- **D1**: under `adopted`, `right(p4)` and `right(p7)` are each at least 4 of 5, and under `now` at most 2 of 5 on
  at least one of the two: the sentence **moves** a reader who knew nothing of the mechanism (p7 only where its two
  platforms agree; else D1 rests on p4 alone and the run says so). A D1 that fails says the sentence does not
  teach what it says, and the sitting is reopened on the text.
- **D2**: under `adopted`, `right(p1)`, `right(p2)`, `right(p3)` and `right(p5)` each fall by **no more than 1**
  against `now`: the sentence must never make a reader think a program that runs aborts.
- **D3**: under `adopted`, at least 4 of 5 t2 readers say of design B *neither* (the language cannot follow it),
  and at most 1 says *the library's C is wrong*.
- **What the run does to the resolution**: D1 or D3 failing moves the resolution to the `rival` where it
  satisfies both and `adopted` does not; D2 failing **withdraws the sentence** until repaired. The warden's
  predictions (its report, `## Predictions registered before the blind run`) are copied and hashed as the run
  starts.

## The area, the runner, the spend

- **The folder**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p257-scratch/257-llm-ergonomist/`,
  **drawn at the synthesis and not before**: the top of an empty git repository of its own (`git init`,
  `.gitignore` `*`, local user `reader` <`reader@invalid`>, no commit), inside this repository's root and
  ignored by it (no file stands outside the root), with `home/` and `tmp/` and one folder per session holding
  only its registered files. **The repository's `CLAUDE.md` and `AGENTS.md` are above it** (the critic's `dirname`
  walk, B18), so no `find` can show the contract absent; **what kept the contract out of panel 256's sessions was
  `--bare` (`run.sh` line 110), the area being the top of its own repository, and the preflight's transcript with
  `scan.py` (`claude-md-or-memory=no`, 47 of 47 in `llm-ergonomist-blind.md`)**, and those are the registered
  checks here too.
- **The runner is panel 256's, ADAPTED, not unchanged** (the critic's B3, by `git show rebel-round:...`), and
  **panel 256's run is the proof it works**: by 11:08 `rebel-round` (`22710bc62`) held
  `docs/panel/256-reports/llm-ergonomist-blind.md`, 46 `claude -p` sessions from 2026-11-22 23:16:32 to
  2026-11-23 00:12:56 by that `run.sh` as registered, all `success`, 0 voided, the free preflight clean. **The
  files that change, each registered with a `diff` against panel 256's committed bytes**: `run.sh` (the caps,
  the sitting's paths, `WARDEN`, `REGISTRATION_SHA`, 46 and 45), `check_folders.py` (46 folders, and a session
  folder holding a `programs/` subfolder, which its flat listing would read as a stray), `brief-session.md` (the
  input names `spec.md` and `programs/`, the report structure of t1 and t2, not `prediction`), `make_sessions.py`
  (the arms `d`, `p`, `v` are letters of `adopted` and `rival`, its label pool must not use them), `preflight.sh`
  (it embeds `run.sh`'s constants). `scan.py`, `check_area.py`, `read_runs.py` and `spent.py` stay as 256's, by
  hash. **The free preflight runs first, and costs 0.**
- **Every session** is the command of `.claude/skills/panel/SKILL.md` § Procedure (`claude -p ... --restricted
  --safe-mode --strict-mcp-config --tools "Read,Write" ... --max-budget-usd`, and `--bare` as 256's `run.sh`),
  with `< /dev/null`, `HOME`, `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the area, counted a success only where
  `subtype` is `success`, `is_error` false and `terminal_reason` `completed`.
- **The cap**: **0.60 USD a session, 1.00 for the verdict, margin 0.10, ceiling 12.00 USD**, the runner stopping
  before any session where `spent + cap + margin <= ceiling` fails (256's `run.sh`, the critic's B4). **The
  verdict therefore runs only if the 30 sessions spent at most 12.00 - 1.00 - 0.10 = 10.90 USD, a mean of
  0.363.** Panel 256's run, **measured** (`llm-ergonomist-blind.md` lines 31 to 36 of `rebel-round`): 10.775607
  USD over 46 sessions, the graded ones 0.171964 to 0.377992 USD each, a mean of (10.775607 - 0.294765) / 45 =
  **0.233**, the verdict 0.294765. So 30 sessions are about **7.0 USD** at that mean (5.2 at the cheapest, 11.3 at
  the dearest, and at the dearest the verdict does not run). The author's figure for panel 256 was *"up to 20
  USD"*; **no figure has been given for this run**, so the cap is the coordinator's proposal, and the author's
  to change.
- **Never started by a seat or by this sitting.** Whoever starts it needs the author's word.

## What every session must say (the brief asks it, in `brief-session.md`)

Whether any project rule, contract or memory reached its context (a yes voids the session), and **every place
where the specification left it a choice, the choice made, and what the other would produce**
(`choice_points`), never its hesitations or its tools.
