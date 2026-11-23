# Panel 257, llm-ergonomist (the blind seat): the paid run, scored

Written 2026-11-23 by the session that scored the blind seat, started by the coordinator of M-rebel-library
after the paid run of 2026-11-23, 17:23 to 17:53. Every number below is a command's, named beside it; the seats
had no compiler, so a session's *runs* or *aborts* is its own reading of its arm's text, never a build. **The
scoring session ran no session, no `claude -p` and no API call: its own spend is 0.** It did run, free and local,
the seven C programs the sitting's coordinator had already built against the frozen runtime (§ The key).

## How it was run

- **The run**: 31 fresh `claude -p` sessions (30 graded and the verdict): `run.log`'s first session line is
  2026-11-23 17:23:45 and its last line (*"spent 6.297764 USD in all"*) 17:53:41, by the area's `run.sh`, from the
  session that held the author's word (issue 2703, closed 2026-11-23 16:07:33Z, `gh issue view 2703`; the author's
  *"up to 20 dollars"* is the sitting's `## Author's verdict`). Each ran in a folder of its own under
  `.claude/worktrees/p257-scratch/257-llm-ergonomist/s/` (the verdict in `verdict/`), the top of an empty git
  repository of its own, ignored by this repository, holding no `CLAUDE.md`: `env -i`, `HOME`, `TMPDIR` and
  `CLAUDE_CODE_TMPDIR` inside the area, `< /dev/null`, `--bare --restricted --safe-mode --strict-mcp-config --model
  claude-opus-5-5`, `--tools "Read,Write" --allowedTools "Read,Write"`, `--max-budget-usd` 0.60 (the verdict 1.20)
  (`run.sh:125-132`, `run.sh:50-54`); Claude Code 2.1.323 (`run.log`). **The caps differ from the registration in two
  places, both written before any session** (`tools/registration-runner.md`, in the archive, § *Amended by the
  coordinator before any session ran*): the ceiling is 19.50 USD for the author's 20 (the registered 12.00 is logged
  where the spend passes it, `run.sh:53-54`, and was never passed), and the verdict's cap is 1.20, not 1.00. **All 31
  ran**; the runner's ceiling was never near. The free preflight ran first, at 17:23:37 (stopped before the key was read)
  and again at 17:23:45 as the run's own first step: cost 0 (a key the API refuses on purpose, `preflight/run.json`:
  `total_cost_usd` 0, *"Invalid API key"*), its scan `prompt-alone=yes git-status=reader, no commit
  claude-md-or-memory=no`.
- **What was registered before, and read again**: the spec-warden's predictions, `warden-at-start.md`, sha256
  `67f3524d69fa66410680b20f2fa0b881e2fd94baf82156d86aa9a996f1c6575d` (`shasum -a 256`; `cmp` against
  `docs/panel/257-reports/spec-warden.md` in this lane: equal; `run.log` line 1 pins its predictions section); the
  coordinator's `registration.md`, sha256 `3b888290e86a8569016cc8b17a51adaf5cc6d595909dce21e5cb2229495d71ed` (`cmp`
  against `docs/panel/257-briefs/blind/registration.md`: equal; `run.log` line 2 checked it and copied it). **Every file
  of every session folder read again by sha256 after the run** (`score-tools/hashcheck.py`, in the archive): **31
  folders, 268 files, 0 differences** from `key.txt`'s row and from the registration's hash for the arm's spec, the task
  or the program (`tools/inputs.sha256`). The three arms reproduce from the sitting: `now` is `cmp`-equal to `git show
  828567a30:spec/heroes-spec.md`; that file with `257-reports/spec-sentence.diff` applied by `patch -p1` reads sha256
  `387cd9da261948700d1d1793718c6e082da5ee4b35ca625245984c8f3dc27421` (`adopted`), with `spec-x1.diff`
  `e809ed7491dccdaa3117ab04180208beeb4084ed5fec9d488d52363a3082c33b` (`rival`); the area's two diffs are `cmp`-equal to
  the sitting's, and its two tasks and eight programs (`prelude.h`, `p1.h` to `p7.h`) to `docs/panel/257-briefs/blind/`.
  Seven of the runner's files equal panel 256's committed bytes by `cmp` (`scan.py`, `check_area.py`, `read_runs.py`,
  `spent.py`, `brief-verdict.md`, `after.py`, `reads.py`); the five registered to change (`run.sh`, `preflight.sh`,
  `check_folders.py`, `make_sessions.py`, `brief-session.md`) differ, and their diffs stand in the archive's
  `tools/registration-runner.md` (not in the sitting's `257-briefs/blind/`; read here only in its last section).
- **The model**: `claude-opus-5-5` alone in every `run.json`'s `modelUsage`, 31 of 31 (the scoring session's python
  over the 31 files).
- **The sessions**: 31; every one `subtype` `success`, `is_error` false, `terminal_reason` `completed`
  (`tools/read_runs.py .`, 31 lines).
- **The cost**: **6.297764 USD** (the registered ceiling 12.00 and the author's 20 never near): `run.log`'s last line,
  `tools/spent.py` and the `sum=` line of `tools/read_runs.py`, three readings of the same 31 `run.json`. By cell,
  summed from the `run.json` by `score-tools/cost.py`: t1 `now` 1.111614, t1 `adopted` 1.045032, t1 `rival` 1.095766, t2
  `now` 0.950488, t2 `adopted` 0.907822, t2 `rival` 0.894691, the verdict 0.292351; the 30 graded 6.005413 (mean
  0.200180, 0.157914 to 0.235378 each). Of the author's 20 USD, 13.702236 stand unspent.
- **The leak check**: `tools/scan.py`, 32 transcripts (the 31 and the preflight): **prompt-alone=NO 0, git-status=LEAK
  0, claude-md-or-memory=YES 0**; `tools/reads.py`: 32 transcripts, **0 sessions with a path outside their folder**,
  0 without a cwd. **0 sessions voided.** Every session's `context` answer says nothing but its folder's files and the
  agent's setup reached it; `t1-j-2` and `t2-j-4` name `CLAUDE.md` as absent; four name the words of their folder's path
  (`t1-b-1`, `t1-j-4`, `t2-b-4`, `t2-x-5`: *"257-llm-ergonomist"*, *"p257-scratch"*), which the scan does not count, the
  area standing under the repository's root by CLAUDE.md § Hard stops. **13 of 15 t2 sessions name `answers.md`** (the t2
  task's own text says *"the key says why in `answers.md`"*); none read it (no folder holds one, `hashcheck.out`; 0
  paths outside the folder, `reads.out`). `t2-x-1`, `t2-b-1` and `t2-b-5` tried `programs/a.c` or `watchdog.h` inside
  their own folder, which do not exist.
- **No session wrote `report.md`**: 31 of 31 (`run.log`'s `report.md=no`, 31 lines); the 30 graded open their answer
  saying they cannot write a file (a pattern over the first 400 bytes of each `result.md`: 30 of 30) and give the
  report as the final answer, which `tools/after.py` kept as `result.md`, the brief's fallback, as in panels 254 and 256.
- **The blinding, as it was for the scorer**: **not blind in fact.** The scorer's orientation command printed
  `key.txt`'s first rows (folder, arm) before any answer was read, so the three letters were known (`now=x
  adopted=j rival=b`). The outcomes were written by folder's letter to `outcomes-by-letter.txt` (sha256
  `f95ed68e30c6b3e32ae41b4473da2bfbfcd7a0644e36faeab86fc57a11d09da8`, 2026-11-23 18:26:37) after reading the answers;
  what keeps the grading honest is the key and the predictions, both written before the sessions ran, and
  `score-tools/` holding the extractors, so any reader can count again.

## The draw

`key.txt`'s first line: `now=x adopted=j rival=b`; its rows give each folder's arm and its files' hashes, and each
folder's `spec.md` reads the arm's registered hash (`15804f3f...` for `now`, `387cd9da...` for `adopted`, `e809ed74...`
for `rival`). 5 sessions in each of the six cells (t1 and t2, three arms), the verdict last; the order is `order.txt`.
The key is published whole in `257-reports/blind-key.md`.

## The key: what is "right"

The registered key is `docs/panel/257-briefs/blind/answers.md` (the t1 table and its t2 paragraph), graded by *the
landed build's own run, on both platforms*; a program whose two verdicts differ is graded by its own row and named,
and is out of D1 while it splits (`257-briefs/llm-ergonomist.md` § The rows). The landed builds' rows are the
sitting's seats' measurements, **not re-run here**: `docs/panel/257-reports/critic-reports.md:66-84` (the critic's six
builds, `3 of 3` each), which equal the compiler-engineer's Linux table (`compiler-engineer.md:55-70`, `:380-400`) and
the ffi-pragmatist's Darwin one (`ffi-pragmatist.md:140-155`):

| program | frozen runtime (`now`'s build), both platforms | landed, Darwin | landed, Linux |
|---|---|---|---|
| p1, p2, p5 | runs | runs | runs |
| p3 | **aborts** (U) | runs | runs |
| p4 | aborts (S) | aborts (S) | aborts (S) |
| p6 | aborts (S) | aborts (S) | aborts (S) |
| **p7** | runs | **aborts (SW)** | **runs** |

**p7 splits on the landed builds** (defect 2702 is open: `gh issue view 2702`, 2026-11-23), so `answers.md`'s
*adopted* column (p7 ABORTS) is Darwin's row, and Linux's is RUNS: **p7 is graded both ways and is out of D1**, as the
registration says. **The frozen runtime's row, which I ran**: the seven programs the sitting's coordinator built against
it (`.claude/worktrees/p257-scratch/coord/blindrun/bin-p1` to `bin-p7`, built 11:07, **not rebuilt here**), each run
twice by me under `timeout 20` (`score-tools/today-run-2.txt`): p1, p2, p5 and p7 exit 0 with `answer ran for 7` and
`result 70`; p3, p4 and p6 exit 134 with the panic lines (`p3`: *"jumped to a buffer this runtime holds no record of"*; `p4`
and `p6`: the S words), 2 of 2 each. That row equals the critic's Darwin-today column. **t2's key** is the
ffi-pragmatist's three timer-driven libraries (`ffi-pragmatist.md:222-244`, `:594`): design A runs and C runs on
every build; **B runs on the frozen runtime, aborts (SW) on the landed Darwin build, and runs on the landed Linux build**
(`critic-reports.md:261`, `watchdog_b` on Linux: *"run (3 builds)"*): B splits the same way p7 does. t2 question 3's key
is *neither*: `answers.md` says *"the library's C is correct, the language cannot follow it"*, which § D3 weighs.

## t1, seven programs: arms `now` (`x`), `adopted` (`j`), `rival` (`b`)

Each session answers per program RUNS or ABORTS with a confidence (1 to 5), the abort's sentence and the fix. Graded by
the key above, the answers read one by one (`score-tools/t1_extract.py`, `t1_score.py`; every row below is its output).
The number after the verdict is the session's confidence.

| folder | arm | cost USD | p1 | p2 | p3 | p4 | p5 | p6 | p7 |
|---|---|---|---|---|---|---|---|---|---|
| `t1-x-1` | `now` | 0.235378 | RUNS 4 | RUNS 4 | RUNS 4 | RUNS 4 | RUNS 5 | ABORTS 4 | RUNS 3 |
| `t1-x-2` | `now` | 0.211920 | RUNS 3 | RUNS 3 | RUNS 3 | RUNS 3 | RUNS 4 | ABORTS 3 | RUNS 2 |
| `t1-x-3` | `now` | 0.226270 | RUNS 3 | RUNS 3 | RUNS 3 | RUNS 2 | RUNS 5 | ABORTS 4 | RUNS 2 |
| `t1-x-4` | `now` | 0.234762 | RUNS 4 | RUNS 4 | RUNS 4 | RUNS 3 | RUNS 5 | ABORTS 3 | RUNS 3 |
| `t1-x-5` | `now` | 0.203285 | RUNS 4 | RUNS 4 | RUNS 4 | RUNS 3 | RUNS 5 | ABORTS 4 | RUNS 3 |
| `t1-j-1` | `adopted` | 0.206838 | RUNS 4 | RUNS 4 | RUNS 4 | ABORTS 4 | RUNS 5 | ABORTS 4 | ABORTS 4 |
| `t1-j-2` | `adopted` | 0.218757 | RUNS 4 | RUNS 4 | RUNS 3 | ABORTS 3 | RUNS 5 | ABORTS 4 | ABORTS 3 |
| `t1-j-3` | `adopted` | 0.209967 | RUNS 4 | RUNS 4 | RUNS 3 | ABORTS 4 | RUNS 5 | ABORTS 4 | ABORTS 4 |
| `t1-j-4` | `adopted` | 0.198407 | RUNS 4 | RUNS 3 | RUNS 3 | ABORTS 4 | RUNS 5 | ABORTS 4 | ABORTS 4 |
| `t1-j-5` | `adopted` | 0.211063 | RUNS 4 | RUNS 4 | RUNS 3 | ABORTS 4 | RUNS 5 | ABORTS 4 | ABORTS 4 |
| `t1-b-1` | `rival` | 0.222854 | RUNS 5 | RUNS 4 | RUNS 4 | ABORTS 3 | RUNS 5 | ABORTS 4 | ABORTS 3 |
| `t1-b-2` | `rival` | 0.219807 | RUNS 5 | RUNS 5 | RUNS 4 | ABORTS 4 | RUNS 5 | ABORTS 4 | ABORTS 4 |
| `t1-b-3` | `rival` | 0.213294 | RUNS 5 | RUNS 5 | RUNS 4 | ABORTS 3 | RUNS 5 | ABORTS 4 | ABORTS 3 |
| `t1-b-4` | `rival` | 0.222162 | RUNS 5 | RUNS 4 | RUNS 4 | ABORTS 3 | RUNS 5 | ABORTS 4 | ABORTS 4 |
| `t1-b-5` | `rival` | 0.217648 | RUNS 5 | RUNS 4 | RUNS 4 | ABORTS 3 | RUNS 5 | ABORTS 4 | ABORTS 4 |

**Right of 5, by the landed key** (`t1-score.out`; p7 shown for Darwin and for Linux):

| arm | p1 | p2 | p3 | p4 | p5 | p6 | p7, Darwin landed | p7, Linux landed |
|---|---|---|---|---|---|---|---|---|
| `now` | 5 | 5 | 5 | **0** | 5 | 5 | 0 | 5 |
| `adopted` | 5 | 5 | 5 | **5** | 5 | 5 | 5 | 0 |
| `rival` | 5 | 5 | 5 | **5** | 5 | 5 | 5 | 0 |

**By the frozen runtime's row** (the build a `now` reader's text describes): `now` 5, 5, 0 on p3 (the frozen build aborts
it with U; 5 of 5 read RUNS, as the landed build does), 0, 5, 5, and 5 on p7; `adopted` and `rival` 5, 5, 0, 5, 5, 5, and 0
on p7. **What each arm's readers did**:

- **`now`, p4**: 5 of 5 RUNS, each from the reading *"§ 13's rule is about being called by a handler, and this handler
  has ended"* (`t1-x-4`: *"the spec doesn't say how the runtime notices handlers"*); confidence 2 or 3 in 4 of 5 (sum 15
  of 25). **p7**: 5 of 5 RUNS, confidence 2 or 3 in 5 of 5 (sum 13), the suspended handler named as unsettled by `t1-x-1`,
  `t1-x-2` and `t1-x-3` (*"the spec doesn't say whether a handler that is paused but never returned ... still counts as
  running"*). **p6**: 5 of 5 ABORTS from line 427's *"nor called by one"*; the abort's sentence is written by each reader
  (*"the specification does not give a fixed text"*, `t1-x-1`): 0 of 5 carry the words the sentence under test adds
  (`t1_words.py`: *"not seen to end"*, *"from inside one"*).
- **`adopted`**: p4 5 of 5 ABORTS, quoting *"a jump no function of the C library makes (written in assembly, say)"*
  (`t1-j-1`: *"the first listed case"*); p7 5 of 5 ABORTS, quoting *"by a `swapcontext` away from it"*; confidence 3 or 4
  (sums 19 and 19 of 25). **15 of 15 abort sentences carry the sentence's words** (`t1-words.out`: *"the handler was
  not seen to end"* for p4 and p7, *"C called it from inside one"* for p6), against 0 of 5 under `now`.
- **`rival`**: the same counts. p7 5 of 5 ABORTS, and 5 of 5 name **`setcontext(&before)` as the fix**, *"a `setcontext`
  to a context `getcontext` took ... which the specification counts as the handler ending"* (`t1-b-1` to `t1-b-5`), the
  clause the rival adds; that replacement is the compiler-engineer's `p7s`, which runs on all six builds
  (`critic-reports.md:82`, `ce-clauses.md:21`). The `adopted` readers' fixes for p7 are `siglongjmp` or a returning
  handler (the `p1` and `p5` shapes), never `setcontext`: their text does not mention it.
- **The choice every reader of `adopted` and `rival` reported**: **the word *"may"*.** 10 of 10 name it (the first
  choice point in 10 of 10) and **10 of 10 read it as *does***: *"I read 'may' as 'the runtime detects this and
  aborts'"* (`t1-j-4`), *"as 'will, whenever the runtime cannot rule the callback out'"* (`t1-b-5`); the other reading,
  *the program runs*, is named as the alternative by each. **0 of 10 wrote RUNS on p4 or on p7.**
- **The platform, for `adopted` and `rival`**: 0 of 10 chose a difference between the Mac and Linux for p7; `t1-j-1`
  chose *"the same on both"* and named the other reading (*"p4 or p7 differ"*), and so did `t1-j-2`. **So the p7
  verdict every reader of the two texts wrote is Darwin's, and wrong on Linux until 2702 lands** (5 of 5, 5 of 5).
- **The depth the programs carry**: 4 readers (`t1-b-3`, `t1-b-4`, `t1-b-5`, `t1-j-5`) gave the callback's 64 or 256 KiB
  deeper frame as why the abort happens (*"256 KiB deeper than the abandoned handler frame, so the callback appears to
  run inside the handler"*), which no sentence says and which is the mechanism on Linux and not on Darwin (the sitting's
  § What the seats measured); 2 `now` readers (`t1-x-3`, `t1-x-4`) mention the depth and say it changes nothing.

## t2, three designs for one library and one abort to read

Question 1 (does the function run, A, B, C) and question 3 (whose mistake B's abort is; the key *neither*) are scored;
question 2 (which design to ask for) is dropped from the scoring by the registration. Graded by `t2_q1.py` (the first
line of each design's answer: whichever of *runs* or *abort* comes first) and by `t2-q3-classified.txt`, each answer's
words read by the scorer (`t2_q3.py`).

| folder | arm | cost USD | A | B | C | whose mistake, B |
|---|---|---|---|---|---|---|
| `t2-x-1` | `now` | 0.195075 | RUNS | ABORTS | RUNS | the library's C |
| `t2-x-2` | `now` | 0.188315 | RUNS | ABORTS | RUNS | the library's C |
| `t2-x-3` | `now` | 0.196412 | RUNS | ABORTS | RUNS | the library's C |
| `t2-x-4` | `now` | 0.194370 | RUNS | ABORTS | RUNS | the library's C |
| `t2-x-5` | `now` | 0.176316 | RUNS | ABORTS | RUNS | the library's C |
| `t2-j-1` | `adopted` | 0.178728 | RUNS | ABORTS | RUNS | the library's C |
| `t2-j-2` | `adopted` | 0.175690 | RUNS | ABORTS | RUNS | neither (the language cannot follow it) |
| `t2-j-3` | `adopted` | 0.198212 | RUNS | ABORTS | RUNS | the library's C |
| `t2-j-4` | `adopted` | 0.179898 | RUNS | ABORTS | RUNS | the library's C |
| `t2-j-5` | `adopted` | 0.175295 | RUNS | ABORTS | RUNS | the library's C |
| `t2-b-1` | `rival` | 0.191001 | RUNS | ABORTS | RUNS | neither (the language cannot follow it) |
| `t2-b-2` | `rival` | 0.178267 | RUNS | ABORTS | RUNS | neither (the language cannot follow it) |
| `t2-b-3` | `rival` | 0.157914 | RUNS | ABORTS | RUNS | the library's C |
| `t2-b-4` | `rival` | 0.183648 | RUNS | ABORTS | RUNS | the library's C |
| `t2-b-5` | `rival` | 0.183861 | RUNS | ABORTS | RUNS | the library's C |

**Question 1, right of 5 by the key (A runs, B aborts, C runs)** (`t2-q1.out`): `now` **5, 5, 5**; `adopted` **5, 5, 5**;
`rival` **5, 5, 5**. B is graded by `answers.md` (ABORTS); on the landed Linux build B runs, so the Linux row reads 0 of 5
in `adopted`, in `rival` **and in `now`**: all fifteen readers say B aborts. **The `now` readers had the words**: the t2
task prints the abort's message and asks *"for the design you think aborts"*, so `now` could read B off the task
(`t2-x-1`: *"The abort message says the runtime records when a C signal handler begins and when it ends"*), the first
leak the spec-warden named; **question 1 cannot show the sentence at work on B.**

**Question 3, whose mistake B's abort is** (`t2-q3.out`):

| arm | the library's C | neither (the key) | the Heroes program |
|---|---|---|---|
| `now` | **5** | 0 | 0 |
| `adopted` | **4** (`j-1`, `j-3`, `j-4`, `j-5`) | **1** (`j-2`) | 0 |
| `rival` | **3** (`b-3`, `b-4`, `b-5`) | **2** (`b-1`, `b-2`) | 0 |

12 of 15 blame the library's C, 3 of 15 say *neither*, 0 of 15 blame the Heroes program (*"The Heroes program did nothing
wrong"* in every answer that names it). **12 of 12 who blame the library's C say the handler really never ends or is
still running** (a pattern over their question 3 answers; `t2-x-5`: *"the language is also not 'unable to follow' B,
because there is nothing to follow"*), and **9 of the 12 say that `swapcontext` is not async-signal-safe** (`t2_q3.py`:
`now` 3 of 5, `adopted` 4 of 4, `rival` 2 of 3). The three who say *neither* give the cause the sentence names
(`t2-b-2`: *"it is a limit on what the runtime can see, not a broken rule of C"*). The readers' fact is the
ffi-pragmatist's: *"POSIX.1-2001 had them, POSIX.1-2008 removed them (R); not on the list (READ) ... unspecified by the
standards, documented by the platform"* (`ffi-pragmatist.md:357`), and *"'though C is correct' is not what the standards
say of one program in this list"* (`:366`), so **the key's *neither*, *"the library's C is correct"*, is the sitting's
registered choice, and the run cannot say a reader who blamed the library was wrong about the standards**.

## The cells, and the registered decision rules

| cell | right of 5 |
|---|---|
| t1 `now`, p1 to p7 by the landed key (p7 Darwin / Linux) | 5, 5, 5, 0, 5, 5, 0 / 5 |
| t1 `adopted`, p1 to p7 (p7 Darwin / Linux) | 5, 5, 5, 5, 5, 5, 5 / 0 |
| t1 `rival`, p1 to p7 (p7 Darwin / Linux) | 5, 5, 5, 5, 5, 5, 5 / 0 |
| t2 `now`, `adopted`, `rival`: Q1 for A, B, C | 5, 5, 5 in each (B on Darwin's row) |
| t2 Q3, *neither* | `now` 0, `adopted` 1, `rival` 2 (the library's C: 5, 4, 3) |

- **D1** (*under `adopted`, `right(p4)` and `right(p7)` are each at least 4 of 5, and under `now` at most 2 of 5 on at
  least one of the two; p7 only where its two platforms agree, else D1 rests on p4 alone and the run says so*):
  **p7 splits, so D1 rests on p4 alone**: `adopted` p4 **5** of 5 (at least 4), `now` p4 **0** of 5 (at most 2):
  **D1 holds**, for `rival` too (5 of 5). The p7 half, graded on Darwin's row, would hold (5, 0); on Linux's row it
  would fail (0, 5): both reported, neither decides.
- **D2** (*under `adopted`, `right(p1)`, `right(p2)`, `right(p3)` and `right(p5)` each fall by no more than 1 against
  `now`*): `adopted` 5, 5, 5, 5 against `now` 5, 5, 5, 5 (and by the frozen row p3 0 against 0): **the fall is 0 on each:
  D2 holds**, for `rival` too. No reader of `adopted` or `rival` wrote ABORTS on p1, p2, p3 or p5.
- **D3** (*under `adopted`, at least 4 of 5 say of design B* neither *and at most 1 says* the library's C is wrong):
  `adopted` *neither* **1** of 5 (needs 4), the library's C **4** of 5 (allows 1): **D3 fails**, for `rival` too (2 and
  3) and for `now` (0 and 5): **it fails for all three arms alike**, as the coordinator registered it would.
- **What the registration does with them**: *"D1 or D3 failing moves the resolution to the `rival` where it satisfies
  both and `adopted` does not; D2 failing withdraws the sentence"*, and for D3 *"adds a clause on blame to a later
  sitting, never to this one"*. D1 and D2 hold for `adopted`, so nothing moves and nothing is withdrawn; D3 fails for
  `adopted` and the `rival` alike, so **no arm satisfies it and the `rival` is not preferred on it**; the clause on blame
  is the later sitting's. **The three rules favour `adopted`, the resolution as ratified, by not separating it from the
  `rival`: every count of t1 is equal (5 each on p1 to p6; p7 5 on Darwin's row and 0 on Linux's in both), Q1 is equal,
  and Q3's *neither* is 1 against 2 of five, a difference of one reader, both far below 4.** The positive clause
  (*"a callback then runs"*) moved no reader on p1, p2 or p3, where `adopted` already read RUNS 5 of 5 without it.
- **Chance**: five readers per cell, one model, one run. A cell at 5 of 5 cannot tell an arm from another on that
  program. The one cell that separates `now` from the other two is p4: 0 of 5 against 5 of 5, Fisher's exact test
  one-sided 1/252 = 0.0040, two-sided 0.0079 (`math.comb(10, 5)`, the scorer's own arithmetic).

## The registered predictions, scored

*"The point is what scores"*: **hit** where the point is the count, **miss** otherwise, the range said beside it. **The
spec-warden registered nothing for `adopted` or `rival`** (`registration.md`: *"none of them is for the `adopted` text
below"*); its rows for `now` score directly, and its two nearest texts' rows (`w26`, `w27`) score as **neighbours**,
never as predictions for the text that ran, as panel 256 scored its S26 and S27.

**The spec-warden** (`warden-at-start.md`, *Predictions registered before the blind run*, 11:47):

| prediction | registered | counted | score |
|---|---|---|---|
| W1, t1 `now` p1 to p7 | 5 (4-5), 5 (4-5), 4 (3-5), **1 (0-2)**, 5 (5-5), 5 (4-5), **1 (0-2)** | 5, 5, 5, **0**, 5, 5, 0 (Darwin) / 5 (Linux) | hit on p1, p2, p5, p6; p3 miss inside (4 to 5); **p4 miss on the point, inside (1 to 0)**; p7 miss on the point, inside, on Darwin's row, **outside** (0-2 against 5) on Linux's |
| W2, t1 `w26` row, the nearest text to `adopted`: p1 to p7 | 5, 5, 5, 4 (3-5), 5, 5, 4 (3-5) | `adopted` 5, 5, 5, 5, 5, 5, 5 (Darwin) / 0 (Linux) | neighbour: hit on p1, p2, p3, p5, p6; p4 and p7 miss inside (4 to 5); Linux's p7 outside |
| W3, t1 `w27` row (silent on `swapcontext`), p1 to p6 | 5, 5, 5, 4 (3-5), 5, 5 | `adopted` 5 on each | neighbour: hit on five, p4 miss inside; its p7 (1, 0-3) is for a text that does not name `swapcontext` and is not scored |
| W4, t2 `now`: Q1 A, B, C | 5 (4-5), **3 (1-5)**, 5 (4-5) | 5, 5, 5 | hit on A and C; **B miss on the point, inside** |
| W5, t2 `now`: Q3 *neither*; *the library's C is wrong* | 4 (3-5); 1 (0-2) | **0; 5** | **both miss, outside their ranges** |
| W6, t2 `w26` row, the neighbour of `adopted`: Q1 A, B, C; Q3 *neither*; *the library's C* | 5, 5 (4-5), 5; 4 (3-5); 1 (0-1) | `adopted` 5, 5, 5; 1; 4 | neighbour: Q1 hit on all three; both Q3 numbers **miss, outside** |
| W7, D1 for `w26` on p4 alone (about 65%), and `now`'s side (p4 = 1) | holds | holds (p4 5 and 0) | **hit** (neighbour) |
| W8, D2 for every text (about 85%) | holds | holds | **hit** |
| W9, D3 *holds for every text including `now`* (about 65%); *if D3 fails it fails for all arms alike and is no argument for or against a text* | holds | **fails for all three** | **miss on the call, hit on the clause**: it failed for all three arms alike |
| W10, the first leak: a `now` reader infers B aborts from the task's words | named | `now` B 5 of 5 | **borne out** |
| W11, spend: the 30 graded about 7.2 USD (mean 0.24, 0.18 to 0.32 each; 5.5 to 9.5 in all); the verdict 0.3 to 0.5, and it runs | 7.2 (5.5-9.5) | 6.005413 (mean 0.200180, 0.157914 to 0.235378); verdict 0.292351, ran | the total **inside** its range, the point missed; 7 of 30 sessions below 0.18; **the verdict 0.008 under its range** |
| W12, the synthesis adopts `w26` (or a repaired `f1b`) | `w26` | the sitting adopted neither (R1) | not this run's (the sitting's, scored there) |

**The coordinator** (`registration.md`, *The coordinator's predictions*):

| prediction | registered | counted | score |
|---|---|---|---|
| C1, t1 `now` p1 to p7 | 4 (3-5), 4 (3-5), 3 (1-5), **1 (0-3)**, 5 (4-5), 5 (4-5), **1 (0-3)** | 5, 5, 5, **0**, 5, 5, 0 (Darwin) / 5 (Linux) | hit on p5, p6; p1, p2, p3, p4 miss inside; p7 miss inside on Darwin's row, **outside (0-3 against 5) on Linux's** |
| C2, t1 `adopted` p1 to p7 | 4 (3-5), 4 (3-5), 4 (2-5), **4 (3-5)**, 5, 5, **4 (2-5)** | 5, 5, 5, **5**, 5, 5, 5 (Darwin) / 0 (Linux) | hit on p5, p6; p1 to p4 miss inside; p7 miss inside on Darwin's row, **outside (2-5 against 0) on Linux's** |
| C3, t1 `rival`: as `adopted`, p3 5 (3-5) | as C2, p3 5 | 5 on every program (p7 Darwin 5 / Linux 0) | hit on p3, p5, p6; p1, p2, p4 miss inside; p7 as C2 |
| C4, t2 Q1 A and C, every arm | 4 (3-5) | 5 in all six cells | **six misses on the point, each inside** |
| C5, t2 Q1 B: `now` 1 (0-3), `adopted` 4 (2-5) | 1; 4 | **5; 5** | **`now` miss, outside its range**; `adopted` miss on the point, inside (`rival` B was not registered: 5) |
| C6, t2 Q3 *neither* under `adopted` | 2 (1-4) | 1 | miss on the point, inside |
| C7, D1 holds for `adopted` | holds | holds (on p4 alone) | **hit** |
| C8, D2 holds | holds | holds | **hit** |
| C9, D3 fails for `adopted` and for `rival` alike | fails | fails for both (and for `now`) | **hit** |
| C10, *the sitting expects the blind reading to say the sentence is silent on blame*, the one thing it does not teach | silent | D3 fails for both texts: 12 of 15 blame the library's C, 3 say neither, 0 the Heroes program; no reader or verdict said *silent* in those words | **borne out as D3's failure** (no arm taught *neither*: 1 and 2 of 5); the words are the registration's, not a session's |
| C11, the run: 31 sessions, 0 voided, 7.0 USD (5.2-11.3) | 31, 0, 7.0 | 31, 0, 6.297764 | hit on 31 and 0; the spend **miss on the point, inside** |

**Counted.** Not the pattern of panels 254 and 256 in every cell, where *every count of right sessions the warden missed,
it missed low*: here the predictions missed **low where the readers were all right** (`now` p1 and p2 at 4 and p3 at 3 or 4
against 5; `adopted` and `rival` p1 to p4 at 4 against 5; Q1 A and C at 4 against 5; Q1 B in `adopted` 4 against 5, in `now` 1 or 3
against 5) and **high where they were wrong** (`now` p4, 1 against 0, and p7 on Darwin's row, 1 against 0; Q3 *neither*, 4
against 0 in `now`, 2 against 1 in `adopted`). **Outside their ranges**: `now`'s Q1 B (C5: 0-3 against 5), `now`'s Q3 (W5:
3-5 against 0 and 0-2 against 5), `adopted`'s Q3 in the neighbour's W6, and p7 on Linux's row only (W1, W2, C1, C2, C3):
the first two for the task's own words and a reader's blame, the last for the platform split the registration set
aside.

## The resolution's rows, one by one

*"A row of the resolution that two or more of five readers read wrong reopens that row's text"* (the same measure as
panel 256's). A row is read wrong when the answer is wrong on a task in the part the row decides.

| row | read by | wrong | reopened |
|---|---|---|---|
| R1, *"may abort"* after the four exits | t1 `adopted` p1 to p7; t2 `adopted` A, B, C | p1, p2, p3, p5: 0 of 5 wrong on each (RUNS); **p4: 0 of 5**; p6: 0 of 5; **p7: 0 of 5 wrong on Darwin's row, 5 of 5 on Linux's**; t2 Q1: 0 of 5 on A, B, C | **no**: the 5 of 5 on p7's Linux row is not a misreading of the sentence (10 of 10 read *may* as *does*; the sentence says *may*), it is the landed Linux build running what the Darwin one refuses until 2702 |
| R1, *what it is told*: *"saying the handler was not seen to end, or that C called it from inside one"* | t1 `adopted` abort sentences | 0 of 15 without the words | no: **15 of 15 carry them**, against 0 of 5 under `now` |
| R1, the platforms: *may* true while Linux runs and Darwin refuses | t1 `adopted` p7 | readers read *may* as *does*; 0 of 10 chose a platform difference | **no row reopens; an observation**: the sentence cannot teach a platform difference it does not name, and it is true on both (§ What the run could not show) |
| R2, the positive clause withheld; its text is `rival` | t1, t2 `rival` | the clause moved 0 readers where `adopted` was already 5 of 5 | no; the programs that falsify the clause on Darwin (`fp3`, `n6`, route A) are **in no task**: unread |
| R3, what the landing owes (the remedy, 22 goldens, the four pins, a check of the nouns) | not a reader's row | | unread |
| R4, findings filed apart (2704 to 2707) | not a reader's row | | unread |
| R5, the blind reading registered | this run | | **run** (corrected in the sitting) |

**No row of the resolution is reopened**: none was read wrong by two readers of five on a task whose key is the
registered one, nor by one, outside p7's Linux half, which is the platform split the sentence's *may* names.

## The A/B verdict, summarised (reproduced whole at the end)

`verdict/` (0.292351 USD) was handed `spec-A.md` (`now`, sha256 `15804f3f...`), `spec-B.md` (**`adopted`, sha256
`387cd9da...`: R1's sentence, the resolution's real text**), `changes.diff` (equal to `diff -u spec-A.md spec-B.md` below
its two header lines, `diff`), `task.md` (t1 then t2), `programs/` and `reports/`: the 30 reports, each named by its
folder (`t1-b-1.md` ...), no arm named. **It did not read one of them**: its first `Read` of `reports/` answered *EISDIR*,
it guessed `README.md`, `1.md`, `01.md`, `report.md`, `t1-1.md` and `r01.md` (none exists), and it says so (*"I also
couldn't read the thirty reports ... the verdict rests on my own experiment only"*; its transcript's 20 `Read` calls, `reads.out`'s
`calls=20`, none of them a read of a report). **The brief did not list the 30 names and the session had no tool that lists a folder** (`--tools
"Read,Write"`); `tools/brief-verdict-reports.md` told it to read all thirty and could not tell it how. Its verdict:
**approve**, on its own experiment: *"Without it, a careful reader of spec-A answers 'RUNS' for p4, p7 and design B.
Those answers are plausible and wrong"*. Its table: under A p4 RUNS (2), p7 RUNS (2), B runs (2); under B p4 ABORTS (4),
p7 ABORTS (4), B aborts (4); p1, p2, p3, p5 RUNS and p6 ABORTS under both. **Its answer to question 3 is *neither***
(*"The language cannot follow a `swapcontext` out of a handler. (`swapcontext` is also not async-signal-safe, so the
library is on weak ground anyway.)"*). Its conditions, scored by this run's counts:

- *"Readers of spec-B are no more accurate than readers of spec-A on p4, p7 and t2-B in the thirty reports"*: **p4: 5
  against 0 of 5, more accurate; p7: 5 against 0 on Darwin's row, 0 against 5 on Linux's; t2-B: 5 against 5, no
  difference** (the task's words). Met on t2-B, not on p4.
- *"Readers of spec-B wrongly answer ABORTS for p1-p3 or t2-A"*: **0 of 5 on each**: not met.
- *"The runtime actually runs p4 or p7 on some platform. Then 'may' ... makes the answer non-deterministic, and I would
  object until the spec says on which platforms"*: **met for p7**: the landed Linux build runs it (`critic-reports.md:75`),
  and the shallow `p4s` runs on Linux (`:83`). The sitting wrote *may* for this reason (§ Where the seats disagree: *"The
  sentence says *may*, which is true while Linux runs them and Darwin refuses, and stays true when 2702 lands"*); the
  verdict's objection is to a sentence that does not name the platforms, which R1 does not.
- *"The sentence says 'aborts' instead of 'may abort', and that matches the runtime"*: not met, and the readers already
  read *may* as *does* 10 of 10.

## What the run could not show

- **A build.** Every *runs* and *aborts* is a reader's reading; the key is the seats' measured landed builds, not re-run
  here (only the frozen row, on the seven prebuilt binaries, this Mac, twice). The landed Linux build's p7 and B are read
  in the critic's and the compiler-engineer's tables, unrun here.
- **A message.** The t2 task prints a *shortened* message on purpose (`answers.md`: 337 of 809 characters); no session read
  a real refusal, so whether the real words mislead a reader is for the compiler-engineer's and critic's measured ones.
- **The sentence's effect on design B.** The task's own words carry it (`now` B 5 of 5), so Q1 cannot separate the arms
  and D3 measures the message, not the sentence. **One run worth its price, unrun**: t2 without the message's words (the
  design asked about, not the abort quoted), 15 sessions, the cost of this run's three t2 cells, **0.950488 + 0.907822 +
  0.894691 = 2.753001 USD** (a measured price for sessions of this size; an inference for a changed task), at most 3 USD;
  not run here.
- **The verdict over the reports.** It read none (above). **One session worth its price, unrun**: the verdict again, the
  brief naming the 30 files, cap 1.20; this run's verdict cost 0.292351 without them, and the 30 reports are 199,028
  bytes (`wc -c`), so 0.5 to 1.0 USD is an inference; not run here.
- **The platform.** p7 and B split on the landed builds (defect 2702, open); every reader chose *the same on both*. D1 rests
  on p4 alone until 2702 lands, when the Linux build refuses p7 and B too and the readers' 5 of 5 is right on both.
  **x86-64** (the CI's leg) and **Windows** are unrun by the sitting and by this run.
- **The Darwin programs that falsify the positive clause** (`fp3`, `n6`, route A) are in no task, so the run says nothing
  of whether the `rival` would mislead a reader on them; R2's reason (the clause is false on Darwin) stands from the
  sitting's measurements, not from readers.
- **Blame.** D3's key is *neither*; 12 of 15 readers said the library's C, 9 of them for the reason that `swapcontext`
  is not async-signal-safe, which the ffi-pragmatist's table does not contradict (`:357`). A later sitting on a clause on
  blame starts from that, not from a *neither* the run has not shown a reader can be taught.
- **Which arm, by readers.** `adopted` and `rival` read alike on every cell; the choice between them rests on the
  sitting's measurements of the clause's truth (R2), not on these readers.
- **Other readers.** One model, `claude-opus-5-5`, five per cell, one run; another model, a longer program or a reader
  with a compiler may read otherwise.

## The appendix: every answer whole

`257-reports/llm-ergonomist-blind-answers.tar.xz` (sha256 `5f0a08b451adcd6b2537933239733244eacdd24116d090a7852201cf98040fe9`) holds, byte for byte, every session's
`result.md` and `run.json` (30 and the verdict's), the preflight's `run.json` and `cli-tmp-diff.txt`, `run.log`,
`runner.out`, `key.txt`, `order.txt`, `registration-at-start.md`, `warden-at-start.md`, the scan's, `reads.py`'s and
`read_runs.py`'s outputs, `outcomes-by-letter.txt`, `tools/registration-runner.md` and `tools/inputs.sha256`, and the
scoring session's extractors with their outputs and the frozen-runtime runs (`score-tools/`). The transcripts stay in the
area under `home/.claude/projects/`, uncommitted.

## The A/B verdict, reproduced whole

The verdict session's answer, as `tools/after.py` kept it in `verdict/result.md`, byte for byte:

````markdown
I couldn't write `report.md` because I have no write tool here, so the whole report is below. I also couldn't read the thirty reports. The `reports/` directory exists, but I can't list it, and the five filenames I guessed don't exist. So the verdict rests on my own experiment only.

# report.md

## verdict

**Approve.** This rests on my own experiment only. The thirty reports in `reports/` were not read (see `context`), so this verdict has not been checked against them as the brief requires.

The change adds one sentence to §13. It tells a reader that a callback may abort if C left a signal handler in a way the runtime cannot follow: by a jump in assembly, by `swapcontext`, by a `makecontext` stack, or by a jump that lands inside the handler. Without it, a careful reader of spec-A answers "RUNS" for p4, p7 and design B. Those answers are plausible and wrong, and nothing refuses them, because every program builds. With it, the reader answers "ABORTS" and can quote the line. The meaning can be read from the sentence, so a veto is not warranted. The word "may" is the remaining weakness (see `condition`).

## experiment

The task has no Heroes code to write: `main` is given, and the C is given. The "program" is the verdict for each case. In every task-1 row, "RUNS" means it prints `answer ran for 7` then `result 70`.

**Task t1**

| case | what the C does | spec-A answer (confidence) | spec-B answer (confidence) |
|---|---|---|---|
| p1 | `siglongjmp`, static buffer | RUNS (4) | RUNS (4) |
| p2 | `siglongjmp`, heap buffer freed after the jump | RUNS (4) | RUNS (4) |
| p3 | `siglongjmp` through a `memcpy` copy of the buffer | RUNS (3) | RUNS (4): it is still a C-library jump, which the new sentence does not list |
| p4 | `__builtin_longjmp` on x86-64, a jump written in assembly on arm64 | RUNS (2): the handler was left and the callback is not "called by one" | **ABORTS** (4) |
| p5 | handler sets a flag and returns; C calls afterwards | RUNS (5) | RUNS (5) |
| p6 | the handler itself calls `answer` | **ABORTS** (5): "never … called by one" | **ABORTS** (5) |
| p7 | `swapcontext` from the handler to the earlier context, never resumed | RUNS (2) | **ABORTS** (4) |

Abort sentences and fixes under spec-B:

- **p4:** "`answer` was called while a signal handler of C's had not been seen to end." Fix: leave the handler with `siglongjmp`, or set a flag and return as p5 does.
- **p6:** "C called `answer` from inside a signal handler." Fix: the handler sets a flag, and `run_case` calls `cb` after `raise` returns.
- **p7:** the same sentence as p4. Fix: use `siglongjmp` instead of `swapcontext`, or the flag design.

Under spec-A, the abort sentence for p6 has to be invented: the spec forbids the call but gives no wording.

**Task t2**

| design | spec-A | spec-B |
|---|---|---|
| A (`siglongjmp`) | runs | runs |
| B (`swapcontext` to the scheduler context) | runs (guess, confidence 2) | **aborts** (4) |
| C (flag, then poll) | runs | runs |

- **Design to ask for:** C. The handler returns normally, so every runtime sees it end and nothing jumps across frames.
- **Whose mistake design B is:** neither the Heroes program's nor, by the spec's wording, strictly the library's. The language cannot follow a `swapcontext` out of a handler. (`swapcontext` is also not async-signal-safe, so the library is on weak ground anyway.)
- **What to tell the author:** use design C, or A if you must abandon the computation.

## choice_points

**spec-A**

1. **p4, p7 and t2-B: does leaving a handler by a non-library jump or by `swapcontext` count as "called by one"?** I chose no, so RUNS. The other reading gives ABORTS, which is very likely what the runtime does: the t2 abort message describes exactly this. My choice is therefore a plausible wrong answer that nothing refuses.
2. **p3: does a copied `sigjmp_buf` matter?** I chose no, so RUNS. The other choice would be a wrong ABORTS.
3. **p6: what does the abort say?** The spec doesn't say, so I made up a sentence. A reader can't quote one.

**spec-B**

1. **p4, p7 and t2-B: "may abort" — answer ABORTS or RUNS?** I chose ABORTS. RUNS is still allowed by the word "may", so a reader who picks it gives a wrong answer that nothing refuses.
2. **p3: does a jump through a copied buffer count as one "no function of the C library makes"?** I chose no, since `siglongjmp` is a library function, so RUNS. The other reading gives a wrong ABORTS.
3. **"a jump that lands within it": does "it" mean the handler or the `makecontext` stack?** I read it as the handler. No case here depends on it, but a different program could get a wrong verdict either way.
4. **p2: does freeing the heap buffer after the jump matter?** I chose no, so RUNS.

## argument

Every program builds, so no mistake is ever refused at build time. The only defence is the spec's text. Spec-A forbids a callback being called *by* a handler (p6) but is silent on handlers *left* in ways the runtime cannot see. A literal reader therefore calls p4, p7 and t2-B "RUNS", which is plausible and likely wrong. Spec-B names those exits and says the abort's words, which match t2's message. Two of seven programs and one of three designs go from a guess to a quotable answer. What's left is "may", which still lets a reader answer RUNS, and one sentence that packs four cases together. Those are reasons to tighten the wording, not to block the change.

## condition

These results would change the verdict:

- **Readers of spec-B are no more accurate than readers of spec-A** on p4, p7 and t2-B in the thirty reports, which I didn't read. Then the change adds words without effect, and I would object.
- **Readers of spec-B wrongly answer ABORTS** for p1–p3 or t2-A, applying the new sentence to `siglongjmp`. Then the change creates a new mistake, and I would object.
- **The runtime actually runs p4 or p7** on some platform. Then "may" is not just vague but makes the answer non-deterministic, and I would object until the spec says on which platforms.
- **The sentence says "aborts" instead of "may abort"**, and that matches the runtime. This would strengthen the approval.

## context

- **The thirty reports:** not read. I can't list `reports/`, and every filename I tried (`1.md`, `01.md`, `report.md`, `t1-1.md`, `r01.md`, `README.md`) does not exist. Nothing about which reader disagrees with which is decided here.
- **Other material:** nothing outside this directory reached my context beyond the system prompt, which only names the working directory and the date. I read `brief.md`, `changes.diff`, `task.md`, `programs/prelude.h` and `programs/p1.h` to `p7.h`, all of `spec-B.md`, and `spec-A.md` around the changed line (lines 400–439). Everywhere else spec-A is identical to spec-B, per `changes.diff`.
````
