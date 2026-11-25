# Panel 260, the llm-ergonomist: REGISTERED, NOT RUN

**Nothing in this file has been run, and no session of it is started by a seat, by the critic or by the clerk.** The
blind reading is **registered**, so that the run is the experiment the sitting meant. Written 2026-11-25 by the sitting's
clerk at `80e2e39ad` and repaired the same day, before any session, after the critic's first reading (its item 3), at
the re-frozen base **`c3885b81b`** (`00-shared.md`), with its inputs and their hashes in `blind/registration.md`.

**Authorised 2026-11-25 by the author**: the answer *ok a* to the coordinator's question of about 10:31, relayed to the
clerk by the coordinator at 10:37 (its clock read 10:37:01), meant as: *run it, with a ceiling of 20 USD*. **The
coordinator starts the run after the synthesis**, on that authorisation, as § The area says, with **a ceiling of 20.00
USD for the whole run** and the per-session caps of § The size. The sitting's resolution is provisional on this reading
and says so.

## What it measures, and why the sitting wants it

The author's principle (`00-shared.md` § 1) puts a new form in the language: a package that binds the platform layer
declares it where it is defined, and the compiler refuses one that does not. A reader who writes such a package from the
specification alone must **write the declaration, in its home, unprompted**; must **not** write it for a package that
binds only the runtime's own header; and must be able to **predict which packages and programs the compiler refuses,
and which file the refusal names** (the package's writer, never its importer). That is a sentence's effect on a reader,
which the compiler-engineer and the ffi-pragmatist, who test truth, cannot measure. The seat reads **only a copy of the
specification** and the task's files (panel 175 item 7: never `design.md`, never the repository).

## The arms, hashed at the synthesis

| label | the spec the session reads |
|---|---|
| `now` | `spec/heroes-spec.md` at the base, sha256 `c915f0f2d47f35c6b3162d198c27bedfbb3fe2b962e05185a878619356372bee` (`shasum -a 256`, the clerk at `80e2e39ad`, 10:33, and the repair at `c3885b81b`, the same bytes) |
| `adopted` | `now` with **the resolution's text** applied |
| `rival` | `now` with **the second-ranked text** the synthesis names (another home of `00-shared.md` § 7.1, or another wording of the same home) |

The synthesis writes both texts and their sha256 into `blind/registration.md` **before any session**; the run refuses to
start where a hash differs. Under `now` no declaration exists: a reader cannot write one, so `now` is the baseline that
shows the text teaches the form, not a rival.

## The tasks

- **t1, write** (`blind/task-t1.md`, written by the clerk and repaired, its sha256 in `blind/registration.md`): four
  small jobs, each written from the specification alone, **the task naming the four headers alike, as headers the
  language provides, and never saying which needs a declaration** (that is what is measured): **W1** a package over
  `hero_time.h`'s clock (`mark()` and `elapsed_ns(since:)`, two plain calls: the first wording asked for a value kept
  since the first call, which needs state the language does not have, spec line 154, *"There are no mutable
  globals"*) and a program using it from its folder; **W2** a package over `hero_os.h`'s process id, the runtime's
  header, which needs none; **W3** a program over `hero_net.h` with no package; **W4** a program over
  `hero_activation.h`, a member's header **the spec names nowhere** (it names `hero_os.h`, `hero_net.h` and
  `hero_time.h` alone, at lines 401 and 428, and line 428 names W3's `hero_net_stop_asked` itself: `grep -noE
  '[a-z_]*hero_[a-z_]*\.h' spec/heroes-spec.md`), with no package. W1 and W3 alone use headers the spec already
  names, so they cannot tell a text that lists the layer's headers from one that gives a reader a rule for the 39 it
  does not name; W4 can. Two grades, each per job:
  - **declared-right**: the declaration present, in the home the session's arm names and nowhere else, for W1, W3 and
    W4, and absent for W2; read from the reader's files against the arm's text by the grader, **file by file**, by the
    table the synthesis writes into `blind/registration.md` before any session (for each arm and job, each file the job
    can hold, relative to the job's folder: the package's module, the program, the program's catalogue, the package's
    own catalogue, any home the arm's text adds; each *declares* or *nothing*; a declaration in any other file is
    declared-wrong);
  - **built**: the reader's files, extracted as written, build at exit 0 under the landed compiler and print what the
    task asks, **on this Mac and in the Linux arm64 image**, three runs each; a job refused for a reason other than the
    declaration is counted apart, *other*, and named. A reference of the four jobs as written under `now` builds at the
    base (`blind/reference-t1/`, `results.txt`): each job can be written in Heroes alone.
- **t2, predict** (`blind/task-t2.md` and `blind/programs/`, **written by the synthesis's clerk once the spelling is
  adopted**, label-stripped, every comment naming the project, a defect or a seat taken out, hashed into
  `blind/registration.md` before any session): six packages and programs, each in the adopted spelling: (1) a floor that
  binds a layer header and declares; (2) the same floor undeclared; (3) a face whose floor declares and which binds
  nothing itself; (4) a program importing a declared package and declaring nothing; (5) a package that reaches the layer
  only through a header of its own that includes a layer header (`00-shared.md` § 6.2's shape); (6) a package that
  declares and binds nothing. The reader says, for each, *builds* or *refused*, and for a refusal which file the message
  names. **The key** is the landed build's run of each, on this Mac and in the Linux arm64 image; a program whose two
  platforms disagree has no key and is named in the scoring.

## The decision rules, registered before any session

- **D1, the text teaches the form**: under `adopted`, at least 4 of 5 readers are declared-right on W1 **and** at least
  4 of 5 on W3; under `now`, the counts are reported (a reader there has no form to write, and the landed compiler
  refuses every W1, W3 and W4 written under it, which is the baseline). A D1 that fails reopens the sitting on the text.
- **D1c, the text gives a rule for a header it does not name**: under `adopted`, at least 4 of 5 readers are
  declared-right on W4. Failing it amends the text before it lands (a rule a reader applies to a header's name, or the
  list it must hold, the spec-warden's question 3), and the synthesis says which.
- **D2, the text never makes a reader declare what needs no declaration**: under `adopted`, at most 1 of 5 readers
  declare in W2. Failing it narrows the text before it lands.
- **D1b, built** (reported beside D1, not a gate): the built counts of W1 to W4 per arm and platform, and every *other*
  named, so a text that teaches the form and breaks the rest of a reader's program is seen.
- **D3, a reader predicts the refusals and where they land**: under `adopted`, at least 4 of 5 readers are build-right
  on each of t2's six (for a refusal, the file named too), and at most 1 names the importer's file for (2) or (5).
- **D4, locality** (reported, not a gate): the readers' `choice_points` that concern the declaration, and how many
  sections of the specification each cites for it; the thesis prefers one.
- **What the run does to the resolution**: D1 or D3 failing moves the resolution to `rival` where it satisfies both and
  `adopted` does not; D2 failing narrows the text; D1c failing amends it. The spec-warden's predictions (its report,
  `## Predictions registered before the blind run`) are copied and hashed as the run starts, and read by
  `blind/registration.md` § How a prediction is read: the spec-warden can predict only for its own texts, written before
  any is adopted, so an arm is read against its text whose diff is the arm's, else its text of the same home with the
  difference named, else it is *unpredicted*.

## The size, the spend and the cap

- **Size**: **31 `claude -p` sessions**: t1 and t2, three arms, five each, and one verdict session reading the 30
  reports.
- **The cap per session**: **0.60 USD** for each of the 30 graded sessions, **1.20 USD** for the verdict, margin 0.10; **a
  ceiling of 20.00 USD for the run**, the author's (above), the runner stopping before any session where `spent + cap +
  margin <= ceiling` fails; the verdict runs only if the 30 spent at most 20.00 − 1.20 − 0.10 = 18.70 USD.
- **The price it rests on**: panel 257's blind run, **31 sessions for 6.297764 USD** (`docs/panel/257-reports/
  llm-ergonomist-blind.md`, its *How it was run*: *"spent 6.297764 USD in all"*, read by the clerk with `sed -n
  10,14p` and `45,48p`, re-read by the repair at the base `c3885b81b`). This run's t1 gives no program
  (`blind/task-t1.md`, 2,849 bytes as repaired: `wc -c`) and asks for four small ones written; t2's six are not
  written yet: **about 6 to 8 USD for the 31, an inference, unrun**.
- **The runner** is panel 256's (`docs/panel/256-briefs/blind/`, `ls` at the base: `RUN.md`, `run.sh`, `preflight.sh`,
  `scan.py`, `check_area.py`, `check_folders.py`, `make_sessions.py`, `make_t1.py`, `make_t2.py`, `read_runs.py`,
  `spent.py`, `after.py`, `reads.py`, `brief-session.md`, `brief-verdict.md`, `registration.md`, `arms-final.sha256`,
  `reference`, `t2-programs`, `task-t1-m.md`, `task-t1-w.md`, `task-t2.md`, `task-t3.md`; 23 entries, `ls | wc -l`
  at `c3885b81b`) **adapted as panel 257 adapted it** (its `tools/registration-runner.md` inside
  `docs/panel/257-reports/llm-ergonomist-blind-answers.tar.xz`, **re-read by the repair** with `tar -xJOf`, 845 lines:
  257's `brief-session.md` names the spec `spec.md`, adds `programs/` and asks `answer`, `choice_points` and `context`,
  no `prediction`). The files that change are registered with a `diff` against panel 256's committed bytes **before
  any session**, in a `registration-runner.md` beside `registration.md`; **the three changes this sitting's
  `brief-session.md` owes** (`spec.md`; a `programs/` line that holds for t1, which has none, and for t2's Heroes
  files; each job's `prediction` and `confidence` under `answer`) are written word for word in `blind/registration.md`
  § Every session's folder. Panel 256's `task-t1-w.md` is a writing task's precedent: read it before adapting.

## The area, and every session

- **The folder**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-llm-ergonomist/`, **drawn at the
  synthesis and not before**: the top of an empty git repository of its own (`git init`, `.gitignore` `*`, local user
  `reader` <`reader@invalid`>, no commit), inside this repository's root and ignored by it, with `home/` and `tmp/` and
  one folder per session holding only its registered files (`brief.md`, `task.md`, `spec.md`, and `programs/` for t2).
- **Every session** is the command of `.claude/skills/panel/SKILL.md` § Procedure (`claude -p ... --restricted
  --safe-mode --strict-mcp-config --tools "Read,Write" --allowedTools "Read,Write" --disallowedTools ...
  --max-budget-usd <cap>`), with `< /dev/null`, `HOME`, `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the area, **after the
  free preflight** (a key the API refuses, cost 0, its scan clean), and counted a success only where `subtype` is
  `success`, `is_error` false and `terminal_reason` `completed`. `claude --version` printed `2.1.323 (Claude Code)` on
  this Mac (the clerk, 10:35; the repair again, 11:20).
- **Never started by a seat, by the critic or by the clerk.** The coordinator starts it after the synthesis, on the
  author's authorisation of 10:37 (above), and not before the synthesis has written into `blind/registration.md` what
  that file says it adds.

## What every session must say (the brief asks it, in `brief-session.md`)

Whether any project rule, contract or memory reached its context (a yes voids the session), and **every place where the
specification left it a choice, the choice made, and what the other would produce** (`choice_points`), never its
hesitations or its tools.
