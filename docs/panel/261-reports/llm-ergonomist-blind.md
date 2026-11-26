# Panel 261, llm-ergonomist (the blind seat): the paid run, as registered, scored

Written 2026-11-26 by the session the coordinator of M-rebel-library launched to run the registered blind reading
(its brief: `.claude/worktrees/rebel-scratch/coord/blind261-brief.md`), on the author's authorisation (*ok a 20
dollars*, recorded in `261-briefs/llm-ergonomist.md`). The run is `261-briefs/llm-ergonomist.md` and
`261-briefs/blind/registration.md` as the synthesis completed them (both on `main` at `e74a3ed7f`; `registration.md`
sha256 `9e241bee...e285`, copied by the runner as it started, `registration-at-start.md`). Every number below is a
command's, named beside it or in the answers' archive. The readers had no compiler: a session's *prints* and
*compiles* are its reading of its arm's text; the grades are the compilers'.

## In one paragraph

**31 sessions counted (30 graded and the verdict), each `success`, `is_error` false, `completed`; 8.78 USD spent**
(8.771908 by the 31 JSONs, and 0.003128 recorded by one session cut mid-run and rerun; the runner's own count, which
takes that cut session's empty JSON at 0.50, 9.271908), of the author's 20.00. **Every one of the 30 programs is
right**: it compiles under its arm's judge on this Mac and in the Linux arm64 image, prints its task's key byte for
byte three times on each, and is bound by the three conditions. So **D1 holds** (`adopted`: t1 5 of 5, t2 5 of 5),
**D2 holds** (no reader of either arm is not prints-right), **D3 holds** (0 of the 10 `adopted` t2 and t3 programs
refused for a mode; t1 0 under each arm), **D4: 0 lost updates under each arm**, and no self-write anywhere. By the
registration's rules **the resolution's text stays as adopted: not reopened, not withdrawn, not amended**. What the run
measures is the form each reader chose, not a difference in correctness, which none of the 30 got wrong: **all 15
`now` readers passed the state by value and answered the new state** (the reference's route), and **all 15 `adopted`
readers wrote `@` in the table's function type, `@` on every changing function and `lent` on every read-only one**
(t2's `GET`, `KEYS` and `COUNT` in 5 of 5; t3's `locked`/`push` as `_: Turnstile lent` in 5 of 5; t1's `GET` and
`/stats` in 5 of 5). The verdict session approves. **The `adopted` judge is the route compiler, not a landed build**,
which does not exist; no `adopted` program reaches a clause the route does not build (§ What the registration could
not do as written, item 1).

## How it was run

- **The inputs, by hash, before any session** (`shasum -a 256`): `arms/spec-now.md` is `git show
  df92f9281:spec/heroes-spec.md`, `3df14254...d93f`, 35,246 bytes; `arms/spec-adopted.md` is
  `261-reports/synthesis-adopted.txt`, `e9b0c819...758c`, 36,286 bytes, `cmp` equal to the base with
  `synthesis-adopted.diff` (`202d8a03...4582`) applied by `patch`; `task-t1.md` `16953413...abe8`, `task-t2.md`
  `2b952b79...4c45`, `task-t3.md` `cea2d2b6...8cfd`; `brief-session.md` `e8fee1a4...0222`, its diff against panel
  256's bytes `cmp` equal to the one `blind/registration-runner.md` registers. **Every one equals the registration's.**
  The key's six files, unpacked from `261-reports/evidence.tar.xz`, each equal their registered hash. The warden's
  report on `main` (`1a7cf8e8...0f43`, the hash the synthesis read), its section `## Predictions registered before the
  blind run` `98a3455b...cc7f`, pinned in `run.sh` and copied as the runner started (`warden-at-start.md`, and at each
  resume).
- **The runner**: panel 259's, rebuilt as `blind/registration-runner.md` asks. Panel 259's archive holds its whole
  runner; its five diffs against panel 256's committed bytes, applied by `patch` to `docs/panel/256-briefs/blind/`,
  **each applied clean and each rebuilt file is `cmp` equal to the archive's**. Seven tools `cmp` equal to panel 256's
  (`scan.py`, `check_area.py`, `read_runs.py`, `spent.py`, `brief-verdict.md`, `after.py`, `reads.py`). Changed for
  261, each with a `diff -u` against panel 256's bytes and against panel 259's rebuilt file, **written before any
  session**: `run.sh` (comments, the warden's path and section hash, the registration's hash; the caps, margin and
  ceilings unchanged, 0.60, 1.20, 0.10, 20.00; the session command flag for flag panel 259's), `preflight.sh` (the arm
  check by `patch` of the six-hunk diff), `check_folders.py` and `make_sessions.py` (two arms, three tasks, a folder
  holding `brief.md`, `task.md`, `spec.md` alone), `make_verdict.py` and `brief-verdict-reports.md` (three tasks, two
  versions, the reports' names listed), `inputs.sha256`. The diffs and the hashes stand in the area's
  `tools/registration-runner.md`, whose first 7,996 bytes are `blind/registration-runner.md` byte for byte (§ What the
  registration could not do as written, item 3). Two changes after sessions ran, each recorded there with its diff
  (§ The cut and the outage).
- **The area**: `.claude/worktrees/261-llm-ergonomist/`, the top of an empty git repository of its own (`.gitignore`
  `*`, `reader <reader@invalid>`, no commit), `home/` and `tmp/` inside, ignored by this repository (`git
  check-ignore`). The static preflight (`bash preflight.sh`): **58 checks `ok`, 0 `FAIL`**. **The free preflight**
  (13:35:14): a key the API refuses, `run.json` `total_cost_usd` 0, `api_error_status` 401, its transcript
  `prompt-alone=yes git-status=reader, no commit claude-md-or-memory=no`, no file naming its id under
  `/private/tmp/claude-501/`. `claude --version`: **2.1.323 (Claude Code)**, the registration's.
- **Every session**: `claude -p "<the registered prompt>" --bare --restricted --safe-mode --strict-mcp-config --model
  claude-opus-5-5 --tools "Read,Write" --allowedTools "Read,Write" --disallowedTools
  "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" --max-budget-usd <cap> --output-format
  json < /dev/null` under `env -i` with `HOME`, `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the area, one at a time, from
  its own folder, the runner stopping before any session where `spent + cap + 0.10` would pass 20.00.
- **The draw**: `key.txt`'s first line, `now=l adopted=y`. **The scorer was not blind**: the key was read before
  grading, and the labels are told apart by the programs themselves (`@` in a function type is the `adopted` arm's
  alone). The grading is mechanical where a command can count and by hand where the registration says to read, and
  both stand in the archive.
- **The run**: first paid session 13:35:22, the verdict ended 14:36:08 (`run.log`). `tools/read_runs.py`: 31 lines,
  every one `subtype=success is_error=False terminal=completed`, the model `claude-opus-5-5` alone; every graded
  session 4 turns, the verdict 36.
- **After the run**: the 30 folders' inputs read again by sha256 against `key.txt` and `tools/inputs.sha256`: **90
  files, 0 differences**, each folder holding beyond them only `result.md`, `run.err`, `run.json`.
  `tools/scan.py`: **33 transcripts (the preflight, the 30, the cut session, the verdict), prompt-alone=NO 0,
  git-status=LEAK 0, claude-md-or-memory=YES 0**; `tools/reads.py`: **0 sessions with a path outside their folder**, 0
  without a cwd. **Every `context` answer says no rule, contract or memory reached it** (read whole, 31 of 31); four
  name the working directory's path, which the system prompt gives (`t2-l-2`, `t3-y-1`, `t3-y-2`, the verdict:
  *"261-llm-ergonomist"*), a path and not a rule, as in panels 257 and 259. **0 sessions voided.**
- **No session wrote `report.md`** (31 of 31 `report.md=no`): each says it holds `Read` alone and gave its report as
  its final answer, kept as `result.md`, the brief's fallback, as in panels 257 and 259.

## The cut and the outage

- **14:06:27**: the coordinator's session restarted and the runner took the signal 4 s into **t1-l-4** (`run.log`:
  *signal received: runner stopping*). Its `run.json` and `run.err` were empty; its transcript records one request and
  a cost-state of **0.003128 USD**, a second request in flight, its cost unrecorded; no file naming its id outside the
  area. Read before any further session: 23 sessions counted, 5.823859 USD by their JSONs (6.323859 by the runner,
  which counts an unreadable JSON at 0.50); the runner resumed at 14:08:56 by its rule (a folder holding `run.json`
  skipped, its cost counted) and ran the six left.
- **14:17:07**: `tools/make_verdict.py` stopped the runner on t1-l-4's empty JSON (`JSONDecodeError`), where its own
  rule is to leave a session that is not a success out of the verdict. **Repaired**: an unreadable `run.json` is no
  success (`make_verdict.py` `faea28d3...ee09`). No verdict session had run.
- **About 14:25 to 14:30**, a network outage cut every agent; **nothing of this run was in flight** (the runner had
  ended at 14:17:07). Read at 14:31: 29 graded sessions counted, **7.353686 USD** by their JSONs, 7.853686 by the
  runner. **By the coordinator's instruction** (*a session cut mid-run counts only if its JSON says success and
  completed; otherwise it is rerun and its spend still counts*), t1-l-4's cut files moved to
  `cut/t1-l-4-20261126-140627/`, its folder back to its three registered files (`check_folders.py` exit 0), and
  `tools/spent.py` counts `cut/*/run.json` too (`0f46d187...ab28`), so the cut stays in the ceiling's sum at 0.50. **t1-l-4
  was rerun at 14:31:46**, a success at 0.289228 USD, then the verdict. The cut run wrote no program; the graded t1-l-4
  is the rerun's.

## The spend, to the cent

| cell | sessions | USD | each |
|---|---|---|---|
| t1 `now` | 5 | 1.55 (1.546114) | 0.27 to 0.35 |
| t1 `adopted` | 5 | 1.57 (1.572868) | 0.28 to 0.34 |
| t2 `now` | 5 | 1.39 (1.388398) | 0.27 to 0.28 |
| t2 `adopted` | 5 | 1.30 (1.297638) | 0.25 to 0.27 |
| t3 `now` | 5 | 0.89 (0.893148) | 0.17 to 0.19 |
| t3 `adopted` | 5 | 0.94 (0.944748) | 0.19 to 0.19 |
| the verdict | 1 | 1.13 (1.128994) | |
| t1-l-4's cut run | (not counted) | 0.003128 recorded, a lower bound | |
| **all** | **31** | **8.78** (8.775036) | |

The 30 graded 7.642914 (mean 0.254764, the most any spent 0.350360, under its 0.60 cap); the verdict 1.128994, under
its 1.20; the free preflight 0. **The runner's own sum, the one the ceiling was enforced on, is 9.271908 USD**: it
takes the cut run's unreadable JSON at 0.50. **Unspent of the author's 20.00: 10.73 by the runner's count**, 11.22 by
the measured one. The registration's estimate, *about 7 to 12 USD* (an inference, unrun), and the warden's 11 (7 to
17) both hold the measured 8.78.

## The judges, and the key on both platforms

- **`now`**: the base's stage 2, `.claude/worktrees/panel261-scratch/synth/bin/heroes-base`, `sources
  4dcf3a9f20257573`. **`adopted`**: the route compiler the synthesis names, `.../synth/bin/heroes-route` (the brief
  says `.../bin/route`; the file is `heroes-route`), `sources 7cc4a75a5a7c6bf1`. **Beside them**, reported and never
  graded: the seat's `heroes-261-v4` by its absolute path (`sources 1283d3066f568534`), and each arm's program under
  the other arm's judge. None rebuilt on this Mac.
- **Linux arm64**: one container, `docker run --rm --pull never --name p261-blind heroes-linux-arm64:latest` (image
  `338a1044ac90`), `aarch64`, Debian clang 22.1.8, over `git archive df92f9281 selfhost runtime seed vendor spec` and
  the same tree with `261-reports/programs/compiler-engineer/route-v4.diff` applied by `patch -p1` (25 files changed,
  the diff's 25; `git apply` from inside this repository applied nothing, silently, and was not used): clang on each
  tree's seed, then stage 2, **`sources 4dcf3a9f20257573` and `sources 7cc4a75a5a7c6bf1`, the Mac judges' own keys**
  (12:36 to 12:47 UTC). The container stopped and removed after grading (`docker ps -a`: none named `p261-blind`).
- **The key, run**: the three references under base, route and v4 on this Mac, and under base and route in Linux arm64,
  `check` 0, `build` 0, three runs each exit 0 and `cmp` equal to `expected.txt` (`score/mac-key/`,
  `score/linux/out/refs.tsv`). The key holds on both platforms the grades read.
- **Every program**: `check` (its whole output and `--brief` kept), `build -o`, the binary three times under `timeout
  20`, output bounded by `head -c 65536`, each exit and each `cmp` with the task's key kept (`score/grade_mac.sh`,
  `score/linux/grade.sh`). On this Mac: base accepts 15 of 30 (the `now` programs), route 30 of 30, v4 30 of 30, each
  accepted one three times `cmp` equal; stderr 0 bytes in every run. In Linux: 45 program and compiler pairs (the 30
  under their judge, the 15 `now` programs under the route beside), every one `check` 0, `build` 0, three runs exit 0
  `cmp` equal, run 1's stderr 0 bytes.

## The 30, scored (`score/score.py`, `score/reading.tsv`, `score/score.out`)

| | compiles (Mac, Linux) | prints-right | bound | **right** | not prints-right | refused-for-mode | lost-update | self-write |
|---|---|---|---|---|---|---|---|---|
| t1 `now` | 5 (5, 5) | 5 | 5 | **5** | 0 | 0 | 0 | 0 |
| t1 `adopted` | 5 (5, 5) | 5 | 5 | **5** | 0 | 0 | 0 | 0 |
| t2 `now` | 5 (5, 5) | 5 | 5 | **5** | 0 | 0 | 0 | 0 |
| t2 `adopted` | 5 (5, 5) | 5 | 5 | **5** | 0 | 0 | 0 | 0 |
| t3 `now` | 5 (5, 5) | 5 | 5 | **5** | 0 | 0 | 0 | 0 |
| t3 `adopted` | 5 (5, 5) | 5 | 5 | **5** | 0 | 0 | 0 | 0 |

- **Beside the judges**: every `now` program checks, builds and prints right under the route as under the base (15 of
  15, both platforms) and under v4 (this Mac); every `adopted` program is refused by the base at its first `@` in a
  type, `expected_type` (*"expected a type, found `@`"*), one diagnostic each, two in `t2-y-3` (both platforms), which
  the registration's list would count as a refusal for a mode, under the language its reader did not read; every
  `adopted` program checks, builds and prints right under v4.
- **Bound**: the table is a `[Route]` literal or a function answering one (t1), a map literal (t2), a map literal
  keyed by a string, a nested map or a record key (t3), in each a top-level function per entry, the function taken
  from it and called through the value; the only comparisons are a t1 entry's method and pattern segments against
  the request's and the lookup's `.ok` and `.err` (`grep` of every program for a comparison with a literal word and
  for every `match` outside a lookup: one, below). **One reading named**: `t1-l-2` binds each route to a `variant
  Handler` case holding the function (`reads`, `removes`, `stores`, `counts`, one route each) and its `respond`
  matches **on the handler's kind** to call it; that `match` compares no route's name, path, command, state or event
  and names no function, so rule (3) reads it as bound by its letter; the verdict session says it *"comes close to the
  task's ban"*.
- **Mode** (by hand, `reading.tsv`): **`now`, 15 of 15 by value with the new state answered** (t1: the notes or the
  state in a reply record, or a `Change` variant; t2: the store in a `Reply`; t3: the turnstile returned, `locked` and
  `push` as `return t`). **`adopted`, 15 of 15 `@` on the changing functions and `lent` on the read-only ones in an `@`
  table** (the fit): t1 `get` and `stats` `State lent` (4) or `{str: str} lent` (`t1-y-4`, which hands the handler
  `@state.notes` and the count by value); t2 `GET`, `KEYS`, `COUNT` `Store lent`, 5 of 5; t3 `locked`/`push`
  `_: Turnstile lent`, 5 of 5, the spec's own `skip(_: Lex lent)` shape. **t1's count of requests is kept outside the
  handlers in 10 of 10** (in `main`, or in the router's own `respond`/`serve`), so every t1 program of either arm has
  read-only routes.
- **Lost-update**: none (no program prints a line off the key). **Self-write**: none (`perl` over the 30 for a line
  `<place> @ <the same place>`: 0); `t3-y-2`'s choice points consider `t.coins @ t.coins` and refuse it as *"a fake
  write"*.

## The decision rules (`261-briefs/llm-ergonomist.md` § The decision rules)

- **D1, the text teaches the form where it is needed: HOLDS.** `adopted` right: t1 **5 of 5**, t2 **5 of 5** (at least 4
  each asked); t3 5 of 5. `now`, reported: t1 5, t2 5, t3 5.
- **D2, the text makes no reader wrong: HOLDS.** Not prints-right under `adopted` 0, 0, 0 against `now`'s 0, 0, 0 (at
  most `now` + 1 asked).
- **D3, the read-only function is predictable: HOLDS.** Refused-for-mode across the 10 `adopted` t2 and t3 programs:
  **0** (at most 1 asked); every one of their 15 read-only functions (`GET`, `KEYS`, `COUNT` five times, `locked`/`push`
  five times) written `lent`. `now`, reported: 0 of 10. **t1 beside it**: refused-for-mode 0 under each arm; each of
  the 10 keeps its count outside the handlers. **The self-write row**: 0 under each arm, so D3 does not pass on
  self-writes.
- **D4, lost updates: 0 under `adopted`, 0 under `now`** of 15 each: `adopted` at `now`, as the thesis predicts; no
  finding against the text. The `now` arm's silent class (*a change written into a copy that nothing hands back*) was
  written by none of its 15 readers.
- **D5, locality (reported)**: the `choice_points` items that concern a mode (a function's or the table type's mark,
  copy-in and answer-out, the call's `@`), read by `score/d5.py` and corrected by hand (`score/d5-items.out`: items on
  deleting a key, on cells and on record construction set aside): **`now` 15 items in 15 sessions, 6 citing one
  section (§ 9 each), 9 none, 0 two or more; `adopted` 29 items in 15 sessions, 7 citing one section (§ 9 five times,
  § 5 twice), 22 none, 0 two or more.** No item cites two sections. A read-only member rests on two rules in two
  sections, § 5's refusal of an `@` parameter nothing writes and § 9's fit; 11 of the 15 `adopted` readers name both
  in the items `d5.py` reads as concerning a mode (a lexical count, `nothing writes` and `fit`, over the items' whole
  text), each item citing at most one of the two sections.
- **What the run does to the resolution**: D1 holds, so the text is not reopened; D2 holds, so it is not withdrawn; D3
  holds, so neither the text nor question 5's rule is amended.

**What the run cannot say** (a ceiling): every reader of both arms wrote a right program, so D2 and D4 compare 0 with
0, and the run separates the arms by the form the readers chose, never by correctness. The registration's premise that
`now` invites the lost update (*"a change written into a copy that nothing hands back, which compiles and prints a
wrong count"*) is not met by any `now` reader at these tasks; the verdict session names that class too and says a run
where `now` readers never make it weakens its approval.

## Measured beside the grades, never graded

- **Length**: the `adopted` programs are shorter, non-blank non-comment lines mean t1 90.0 against 100.4, t2 83.6
  against 95.4, t3 27.6 against 31.0; bytes t1 3,618 against 4,074, t2 2,612 against 3,143, t3 1,078 against 1,221
  (`grep -cv`, `wc -c`).
- **A field called through `x.f(y)`**: under `now`, 5 of 5 t1 readers bind the route's function field to a name
  before calling it, and each report says the direct call would be read as UFCS, `answer(route, ...)` (`grep` of the
  five); under `adopted`, whose § 9 says the field comes first, 3 of 5 call `route.handler(@state, ...)` or
  `r.answer(@state, ...)` directly. **The base calls the field**: `t1-l-1`
  with its call rewritten `route.handler(state, req, name)` checks, exit 0, and prints the key's lines under the base
  (`probe/fieldcall`, scratch; panel 079's ruling, which the adopted § 9 states).
- **The table's type was written in every `adopted` program**, by a record field, an annotation or a function's
  answer; four readers (`t2-y-4`, `t3-y-1`, `t3-y-2`, `t3-y-3`) say they wrote it so as not to rest on *"in any
  order"*, two of them reading it as ambiguous. So the run measured nothing of R3's order-free join.

## The spec-warden's predictions, scored (`warden-at-start.md`, registered before any session)

Read as `registration.md` § How a prediction is read: `adopted` has no warden text of its own, so **W1's predictions
are read for it**; the lines its diff has and W1's does not (§ 5's *"and `x @ x` not"*, § 9's names' fit sentence,
*"in any order"*, the `extern` value sentence, § 13's callback clause) are **reached by no program** (no self-write, no
named position, every table typed, no `extern`, no `spawn`), so every score below is on lines the two texts share.

| prediction | registered | measured | point | range |
|---|---|---|---|---|
| `now` t1 / t2 / t3 right | 3 (2-5) / 3 (2-5) / 4 (3-5) | 5 / 5 / 5 | wrong | right |
| `now` not prints-right t1 / t2 / t3 | 2 (0-3) / 2 (0-3) / 1 (0-2) | 0 / 0 / 0 | wrong | right |
| `now` refused-for-mode t2+t3; t1 | 1 of 10 (0-3); 1 (0-2) | 0; 0 | wrong | right |
| `now` self-writes; lost-updates | 0 (0-1); 1 of 15 (0-2) | 0; 0 | right; wrong | right |
| `adopted` D1 holds; t1 / t2 / t3 right | holds; 4 / 4 / 4 (3-5) | holds; 5 / 5 / 5 | right; wrong | right |
| `adopted` D2 holds; not prints-right t1 / t2 / t3 | holds; 1 (0-2) / 1 (0-3) / 1 (0-2) | holds; 0 / 0 / 0 | right; wrong | right |
| `adopted` D3 holds; refused t2+t3; t1 | holds; 1 of 10 (0-3); 1 (0-2) | holds; 0; 0 | right; wrong | right |
| at least 3 of 5 t2 readers `lent` (or no mark) on `GET`, `KEYS`, `COUNT` | at least 3 | 5 | right | |
| at least 3 of 5 t3 readers `lent` on `locked`/`push` | at least 3 | 5 | right | |
| the refusal when one comes: a by-value `Store` in an `@` table | | none came | unscored | |
| `adopted` self-writes; D4 lost-updates, at or below `now` | 0 (0-1); 0 (0-1) | 0; 0, at `now` | right | right |
| the spend; no session past its cap | 11 USD (7-17); none past | 8.78 (runner 9.27); none past | wrong | right |

Every direction and every range holds; every point that is not 0 or a rule's verdict is under the measurement, the
warden having expected readers to fail where none did.

## The verdict session

Built by `make_verdict.py` after the 30 (35 files: `spec-A.md` `now`, `spec-B.md` `adopted`, `changes.diff`, the three
tasks, **30 reports** named by label, `brief.md`); 36 turns, 1.128994 USD. **`verdict`: approve.** It wrote the three
tasks under each text, predicting the key's lines for all six; its argument: under A every handler needs a hand-made
return-and-write-back protocol whose omission the build accepts in some shapes, under B *"that step is gone"*, and
every misuse of the marks it tried is refused. Its two weak points of B: that a plain copy does not fit an `@` slot is
*"implied, not stated"*, and `x.f(y)` now depends on the record's fields. Its conditions to object: a program where
`x.f(y)` calls a field where the writer meant the UFCS function or the reverse; a plain-copy function accepted in an
`@` slot; and the § 13 callback text, untested by any task. **It read the arms off the programs** (*"Judging by the
marks they used, the `-l-` readers had spec A and the `-y-` readers had spec B"*), so label blinding cannot hold for a
write task whose arm has syntax of its own; and by its `context` it read only the part of `spec-B.md` that holds
§ 9, beside `changes.diff`. Its claims about A's silent mistakes are its reading, unrun by it, and no `now` reader made
one.

## What the registration could not do as written, and what was done instead

1. **The `adopted` arm's judge is the route compiler** (`heroes-route`, `sources 7cc4a75a5a7c6bf1`), with
   `heroes-261-v4` beside it, as the coordinator's brief names them; the registration says the `adopted` grades wait
   for the landed build, which does not exist (*"a reading graded against a compiler nobody built is not a
   measurement"*). The route lacks R3's order-free join and self-write rule, R4, R5 and R8's two changes; **no
   `adopted` program reaches any of them** (read from the programs above), so the route's verdict on these 15 is what
   the adopted text says of them. **The landed compiler's answer on the 15 is owed when it is built, unrun.**
2. The brief names the route compiler `.claude/worktrees/panel261-scratch/synth/bin/route`; the file there is
   `heroes-route`.
3. **`blind/registration-runner.md` asks the run's session to write the runner's diffs below its list**; this session's
   brief forbids an edit of a registered file, so the file in the tree is untouched and its continuation is the area's
   `tools/registration-runner.md` (its first 7,996 bytes the registered file's, sha256 `54ca3317...be84`), carried by
   the archive.
4. **The run was cut once and resumed twice** (§ The cut and the outage): t1-l-4's cut run is not counted and was rerun
   by the coordinator's instruction, its recorded 0.003128 USD in the measured spend and 0.50 in the runner's; two
   runner files changed after sessions ran (`make_verdict.py`, `spent.py`), each recorded with its diff before the
   session that needed it. `preflight.sh`'s check that `spent.py` is panel 256's bytes no longer holds; `run.sh` does
   not run it.
5. **No session could write `report.md`**: 31 answers are the fallback `result.md`.
6. **A ceiling**: 30 of 30 right, so D2 and D4 read 0 against 0 and the run does not separate the arms on
   correctness (above).
7. **The scorer was not blind**, and the verdict read the arms off the programs (above). **`t1-l-2`'s bound** rests on
   reading rule (3) by its letter (above). In Linux only run 1's stdout and stderr were kept per program; all three
   runs were compared.

## Where the answers are

`261-reports/llm-ergonomist-blind-answers.tar.xz` (sha256 `6e7aa713...b377`, 2,229 entries, text only): the area's
`key.txt`, `order.txt`, `run.log`, `runner.out`, `runner-resume.out`, `resume-1407.md`, `read_runs.out`, `scan.out`,
`reads.out`, `warden-at-start.md` and the two `warden-at-resume-*.md`, `registration-at-start.md`, `run.sh`,
`preflight.sh` and its two outputs, `tools/` (with `registration-runner.md`), `preflight/run.json` and its
`cli-tmp-diff.txt`, `cut/`; every session's `result.md`, `run.json` and `run.err`, and the verdict's `brief.md`,
`changes.diff`, `result.md`, `run.json`, `run.err`; `score/` (`extract.py`, `grade_mac.sh`, `score.py`, `d5.py`,
`reading.tsv`, `score.out`, `d5.out`, `d5-items.out`, `runner-diffs.md`, `progs/` the 30 extracted programs,
`mac/` every program's `check` output, exits and run outputs under the three compilers, `mac-key/` the references',
`linux/` the container's scripts, its build logs and every program's grades). The transcripts, the session folders'
inputs (reproducible by hash) and the scratch stay in `.claude/worktrees/261-llm-ergonomist/` and
`.claude/worktrees/261-llm-ergonomist-scratch/`, uncommitted.
