# Panel 262, llm-ergonomist (the blind seat): the paid run, as registered and amended, scored

Written 2026-11-27 by the session the coordinator of M-rebel-library launched to run the registered blind reading (its
brief: `.claude/worktrees/rebel-scratch/coord/blind262-brief.md`), on the author's authorisation (*ok a 25 dollars*,
2026-11-26 about 21:45, at the head of `262-briefs/blind/registration.md`). The run is that registration **as the
coordinator amended it before any session** (its last section, *Amended before any session, 2026-11-27, by the
coordinator*; `registration.md` sha256 `84cd466e...a302`, copied by the runner as it started) and the runner
`262-briefs/blind/registration-runner.md` registers. Every number below is a command's, named beside it; the scoring's
files are in `.claude/worktrees/blind262-scratch/score/`, the run's notes in `.claude/worktrees/blind262-scratch/notes.md`.
The readers had no compiler: a verdict is their reading of their arm's text; the key is the plain build's, one verdict
on both graded platforms.

## In one paragraph

**31 sessions (30 graded and the verdict), each `success`, `is_error` false, `completed`; 11.66 USD spent**
(11.657696 by the 31 JSONs, `tools/read_runs.py`), of the author's 25.00; the most expensive session the verdict,
1.332397. **The key did not move on either platform** under the pinned commit's build once `h13.hero` lost the word
`layer` (the amendment's rules 2 and 3). **`adopted`'s fifteen readers gave the key's verdict or a text-right
`NOT PROMISED` on every one of their 85 units: 0 wrong.** So **D1 holds** (`h1`, `h3`, `h13`, `n1`, `n2`, `n3`: 5 of 5
build-right each, 0 `NOT PROMISED`), **D2 holds** (`h7`, `h9`, `h10`, `h11`: 0 `ABORTS` each), **D3 holds** (`h2`,
`h5`, `h6`, `h8`: 5 of 5 text-right each, no `ABORTS` to grade for words), **D5 holds** (`h12`: 0 `RUNS`, 5
`NOT PROMISED`, text-right). By the registration's rules **the resolution's text stays as adopted: not moved to
`rival`, not reopened, not amended.** **`rival` holds D1, D2, D3 and D5 too**, and is wrong on `n4` (5 of 5 `RUNS`,
the key aborts) and once on `h4`; **`now` is wrong on 21 units**: `h12` 5 of 5 `RUNS` (the key aborts), `n2` and `n3`
5 of 5 `NOT PROMISED` each and `n1` 1 (the key runs, `now` silent), `n4` 5 of 5 `RUNS`. **The spec-warden's `spec-w1`
column, read for `rival`, predicted D2 failing (60 %); `rival` passes D2 on all four**, which the warden registered as
the result that would falsify its recommendation (below). **The two readers labelled the 27 wrong units apart and
agree on every one** (same set 27 of 27; W, L, H and O kappa 1.00): under `now`, `h12` and `n4` are **L** (10), `n1`,
`n2` and `n3` **W and H** (11); under `rival`, `n4` **L** (5) and `h4` **O** (1); `adopted` has none (§ The labels).

## How it was run

- **What the registration assumed that was false on the day, and what was done** (each found before any session):
  1. **`main` had moved the spec** at `750d2540d` (step 208b, 2026-11-27 03:15:31 +0100, one line, 411, § 13's first
     paragraph; the handler sentence, line 438, untouched): `main`'s spec read `736b534c...efd3`, not the registered
     `9ff7d2a1...2fc3`. The session stopped at 03:18 and said so; **the coordinator pinned `now` to `9b88d3e84`'s
     spec** (`736b534c...efd3`, verified) and made `adopted` and `rival` the synthesis's two diffs over it
     (`d66dfd30...7b17` and `ecfb9931...4d7c`, `patch -p1` and `cmp`): rule 1 of the amendment.
  2. **`h13`'s key had moved**: `h13.hero` opened `extern "hero_os.h" layer`, which step 208a (`b626d4c05`, merged at
     `f1d7da1ba`) refuses, and `main`'s stage 2 at `d42929b67` refused it, plain and `--sanitize`
     (`error[ffi_layer_unreached]`, `fix (certain): take `layer` out`). **`h13.hero` lost the word and nothing else**
     (`06be8628...b42e`, the registered bytes kept as `h13.hero.registered`): rule 2.
  3. **`claude --version` printed `2.1.326 (Claude Code)`**, where the registration carried `2.1.323`: rule 4.
  The fourth assumption the brief named, **no `Write` tool under `--bare --restricted --safe-mode`, held**: the free
  preflight's transcript was offered `Read` alone, and so were all 32 transcripts (the preflight, the 30, the verdict;
  their `api-request-shape` entries).
- **The inputs, by hash, before any session** (`shasum -a 256`): `262-briefs/blind/inputs.sha256` read 41 `OK` and one
  `FAILED`, the old `programs/h13.hero` line kept as history by the amendment; `arms.sha256`'s three new lines `OK`
  from the panel262 worktree's root. The area's `tools/inputs.sha256` (41 files: the three pinned arms, the two tasks,
  the session brief, the 35 programs) equals the registered hashes, the last line per path (`check_folders.py` and a
  comparison by script, 0 of 41 differ). The warden's report on `main` read `d490152b...5c88`, the hash the synthesis
  read; its section `## Predictions registered before the blind run` `4c47b7ad...2bd0` (28 lines), pinned in `run.sh`
  and copied as the runner started (`warden-at-start.md`).
- **The runner**: panel 261's, as its run ran it (its area's `run.sh`, `preflight.sh` and `tools/`), adapted, every
  change a `diff -u` against 261's file in `262-briefs/blind/registration-runner.md`, written before any session (its
  copy in the area `4e125c5b...362e`): three arms and two tasks, each folder `brief.md`, `task.md`, `spec.md` and its
  task's `programs/` (t1's 28 files, t2's 8); the caps 1.00 (t1), 0.60 (t2), 1.50 (the verdict), margin 0.10,
  **ceiling 25.00**; the order **`random.Random(136237622)`** over the 30 cells sorted (the seed drawn at 03:34:39 by
  `secrets.randbits(31)` and written down before the first session), the verdict last; the per-session limit 1,800 s;
  one check added, the runner stopping unless the preflight was offered `Read` alone; the key, the classes and the
  rules fixed before any session in `tools/key-grades.tsv` (`a3613cf8...78a8`). Unchanged from 261's: `after.py`,
  `brief-verdict.md`, `check_area.py`, `read_runs.py`, `reads.py`, `scan.py`, `spent.py`.
- **The area**: `.claude/worktrees/262-llm-ergonomist/`, drawn at 03:34, the top of an empty git repository of its own
  (`.gitignore` `*`, `reader <reader@invalid>`, no commit; `git check-ignore` names it ignored by this repository),
  `home/` and `tmp/` inside. **Static preflight** (`bash preflight.sh`): **98 checks `ok`, 0 `FAIL`**. **The free
  preflight** (03:40:16): a key the API refuses, `run.json` `total_cost_usd` 0, `api_error_status` 401, its transcript
  `prompt-alone=yes git-status=reader, no commit claude-md-or-memory=no`, the tools offered `Read`, no file naming its
  id under `/private/tmp/claude-501/`.
- **Every session**: `claude -p "<the registered prompt>" --bare --restricted --safe-mode --strict-mcp-config --model
  claude-opus-5-5 --tools "Read,Write" --allowedTools "Read,Write" --disallowedTools
  "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" --max-budget-usd <cap> --output-format
  json < /dev/null` under `env -i` with `HOME`, `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the area, the key from the
  trunk's `.env` (`ANTHROPIC_API_KEY length 108`, its value never printed), one at a time, from its own folder.
- **The draw**: `key.txt`'s first line, **`now=s adopted=q rival=k`**. **The scorer was not blind** (the key read
  before grading, as at panel 261). The verdict session matched the reports to the versions by their quotations
  (below), so the labels' blinding did not hold for it.
- **The run**: first paid session 03:40:30, the verdict ended 04:30:21 (`run.log`), the runner's exit 0.
  **The session that started it was paused from about 03:45 to 06:51 by the account's five-hour limit; the runner, a
  background process of that session, kept running and finished in that time.** The coordinator, resuming it, asked
  whether any session had run (its own log of 06:51 read *no claude -p session had run: spend 0*); the area said they
  all had (31 `run.json`, `run.log`'s last line *spent 11.657696 USD in all*), and nothing was started again. `tools/read_runs.py`: 31 lines, every one
  `subtype=success is_error=False terminal=completed`, the model `claude-opus-5-5` alone; every t1 session 21 turns,
  every t2 session 12, the verdict 62.
- **After the run**: every folder's inputs read again by sha256 against `key.txt`: **702 files (the 30 folders' 630 and
  the verdict's 72), 0 differences**, each folder holding beyond them only `result.md`, `run.err` (0 bytes in all 31)
  and `run.json`. `tools/scan.py`: **32 transcripts, prompt-alone=NO 0, git-status=LEAK 0, claude-md-or-memory=YES 1,
  the verdict's**, and **every hit there is a tool result of a report it read**: six readers' own `context` answers
  saying *"No CLAUDE.md ... reached me"* (`t1-k-2`, `t1-q-1`, `t1-s-1`, `t1-s-3`, `t2-q-1`, `t2-q-4`), read back by
  the verdict, none a text of `CLAUDE.md` or a memory (the scan's own rule reads what reached a session, and a
  report's words are that). `tools/reads.py`: **0 sessions with a path outside their folder**, 0 without a cwd. No
  permission denial and no safety stop in any JSON. **No session wrote `report.md`** (31 of 31, `Read` alone): each
  gave its report as its final answer, kept as `result.md`, the brief's fallback.

## The spend, to the cent

| cell | sessions | USD | each |
|---|---|---|---|
| t1 `now` | 5 | 1.94 (1.942985) | 0.36 to 0.41 |
| t1 `adopted` | 5 | 2.00 (1.999403) | 0.37 to 0.43 |
| t1 `rival` | 5 | 2.00 (1.995591) | 0.38 to 0.41 |
| t2 `now` | 5 | 1.63 (1.626794) | 0.29 to 0.39 |
| t2 `adopted` | 5 | 1.39 (1.390504) | 0.25 to 0.30 |
| t2 `rival` | 5 | 1.37 (1.370023) | 0.26 to 0.28 |
| the verdict | 1 | 1.33 (1.332397) | |
| **all** | **31** | **11.66** (11.657696) | |

The 30 graded 10.325299 (t1 mean 0.395865, the most any t1 spent 0.425442, `t1-q-3`, under its 1.00 cap; t2 mean
0.292488, the most 0.385352, `t2-s-3`, under its 0.60); **the most expensive session the verdict, 1.332397**, under its
1.50; the free preflight 0. **Unspent of the author's 25.00: 13.34.** The registration's inference, *about 13 to 18
USD for the 31* (unrun), was above the measurement; t1 cost 0.36 to 0.43 against its 0.57 to 0.80 scaled from 259.

## The key on both platforms

Run before any session under **`9b88d3e84`'s stage 2, `sources 7c7c978a4dfeddf3`** (built from that commit's seed,
`sources 1e1eb23a1927d35b`), `sh blind/key.sh` over `h1` to `h13` and `n1` to `n4`, plain and `--sanitize`, three runs
each, `h13.hero` amended:

- **this Mac** (Darwin arm64, 03:30:49 to 03:31:42): 34 cells, each one distinct output of three; **102 of 102 outputs
  equal to the registered `key-mac-repair/`** after `key.sh`'s normalisation, ASan's warning lines set aside;
- **Linux arm64** (one container `blind262`, `heroes-linux-arm64:latest` `338a1044ac90`, `aarch64`, Debian clang
  22.1.8, glibc 2.41-12+deb13u3, 03:32:09 to 03:37:23, removed with `--rm`, `docker ps -a` naming none after): the
  same stage 2 key, 34 cells each one distinct output of three, **every first run equal to this Mac's** (0 of 34 differ).

**No cell moved from the registered key; no program splits between the platforms or between the plain and `--sanitize`
builds**, so every rule has its key. `n3` runs on both (it stays in D1); `n1 --sanitize` runs on Darwin under the day's
runtime. Before the amendment, `main`'s stage 2 at `d42929b67` (the same `sources 7c7c978a4dfeddf3`) refused the
registered `h13.hero` and gave the 16 others' registered verdicts (96 of 96 outputs equal).

## Every unit against the key

255 units (15 t1 sessions × 13 programs, 15 t2 × 4), extracted by `score/extract.py` and graded by `score/grade.py`
against `tools/key-grades.tsv`, the verdict words cross-checked by a second reading by heading (`score/crosscheck.txt`:
0 of 255 differ). **No reader gave the Mac and Linux different verdicts.** Every t2 `RUNS` printed the key's lines in
order (graded line by line); every `ABORTS` was read by hand for its words. In the tables `R` is `RUNS`, `A`
`ABORTS`, `NP` `NOT PROMISED`, and `*` marks a wrong unit.

| session | arm | h1 | h2 | h3 | h4 | h5 | h6 | h7 | h8 | h9 | h10 | h11 | h12 | h13 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| key | | R | A | R | A | A | A | R | A | R | R | R | A | R |
| `t1-s-1` | `now` | R | NP | R | A | NP | NP | NP | NP | NP | NP | NP | R* | R |
| `t1-s-2` | `now` | R | NP | R | A | NP | NP | NP | NP | NP | NP | NP | R* | R |
| `t1-s-3` | `now` | R | NP | R | A | NP | NP | NP | NP | NP | NP | NP | R* | R |
| `t1-s-4` | `now` | R | NP | R | A | NP | NP | NP | NP | NP | NP | NP | R* | R |
| `t1-s-5` | `now` | R | NP | R | A | NP | A | NP | NP | NP | NP | NP | R* | R |
| `t1-q-1` | `adopted` | R | NP | R | NP | NP | NP | R | NP | R | R | NP | NP | R |
| `t1-q-2` | `adopted` | R | NP | R | NP | NP | NP | R | NP | R | R | NP | NP | R |
| `t1-q-3` | `adopted` | R | NP | R | NP | NP | NP | R | NP | R | R | NP | NP | R |
| `t1-q-4` | `adopted` | R | NP | R | NP | NP | NP | R | NP | R | R | NP | NP | R |
| `t1-q-5` | `adopted` | R | NP | R | NP | NP | NP | R | NP | R | R | NP | NP | R |
| `t1-k-1` | `rival` | R | NP | R | A | NP | NP | NP | NP | R | NP | NP | NP | R |
| `t1-k-2` | `rival` | R | NP | R | NP* | NP | NP | NP | NP | R | NP | NP | NP | R |
| `t1-k-3` | `rival` | R | NP | R | A | NP | A | NP | NP | R | NP | NP | NP | R |
| `t1-k-4` | `rival` | R | NP | R | A | NP | A | NP | NP | R | NP | NP | NP | R |
| `t1-k-5` | `rival` | R | NP | R | A | NP | A | NP | NP | R | NP | NP | NP | R |

| session | arm | n1 | n2 | n3 | n4 |
|---|---|---|---|---|---|
| key | | R | R | R | A |
| `t2-s-1` | `now` | R | NP* | NP* | R* |
| `t2-s-2` | `now` | NP* | NP* | NP* | R* |
| `t2-s-3` | `now` | R | NP* | NP* | R* |
| `t2-s-4` | `now` | R | NP* | NP* | R* |
| `t2-s-5` | `now` | R | NP* | NP* | R* |
| `t2-q-1` | `adopted` | R | R | R | A |
| `t2-q-2` | `adopted` | R | R | R | A |
| `t2-q-3` | `adopted` | R | R | R | A |
| `t2-q-4` | `adopted` | R | R | R | A |
| `t2-q-5` | `adopted` | R | R | R | A |
| `t2-k-1` | `rival` | R | R | R | R* |
| `t2-k-2` | `rival` | R | R | R | R* |
| `t2-k-3` | `rival` | R | R | R | R* |
| `t2-k-4` | `rival` | R | R | R | R* |
| `t2-k-5` | `rival` | R | R | R | R* |

**The grades** (`score/grades.tsv`, counted by `awk`): of 255 units, **117 build-right, 111 text-right, 27 wrong**; by
arm, `now` 85 units: 25 build-right, 39 text-right, **21 wrong**; `adopted` 85: 50 build-right, 35 text-right, **0
wrong**; `rival` 85: 42 build-right, 37 text-right, **6 wrong**. Every `NOT PROMISED` stands on a program its arm's
class puts in a *may*, and is text-right, but twelve: `now`'s eleven on `n1` to `n3` (silent) and `rival`'s one on
`h4` (no text-right), all wrong; no `ABORTS` falls on a program the key runs (`awk`: 0).

**The words** (by hand, `score/reading.tsv`): **all 18 `ABORTS` name the key's cause.** The 13 on `h4` (`now` 5,
`rival` 4) and `h6` (`now` 1, `rival` 3) say C called the callback from inside a signal handler (*"C called it from
inside a signal handler"*, `t1-s-5`), the sentence's second claim; the 5 on `n4` under `adopted` say it ran on a thread
the program did not start (*"The callback ran on a thread this program did not start"*, `t2-q-5`), and each says
nothing is printed before, the key's lines.

## The decision rules (`registration.md` § The decision rules, sets unmoved by the amendment)

- **D1, no widening: HOLDS.** `adopted`: `h1`, `h3`, `h13`, `n1`, `n2`, `n3` **5 of 5 build-right each, 0
  `NOT PROMISED`** (at least 4 and at most 1 asked). `now`, reported: build-right `h1` 5, `h3` 5, `h13` 5, `n1` 4,
  `n2` **0**, `n3` **0**; `NOT PROMISED` `n1` 1, `n2` **5**, `n3` **5** (259's run predicted 2 to 3 each). `rival`: 5
  of 5 and 0 on all six. **What it says**: under `now` every reader read the handler sentence as covering a coroutine
  program with no signal handler (`n2`, `n3`, 10 of 10), the widening 259 found, at its most; neither candidate text
  leaves it.
- **D2, no narrowing: HOLDS.** `adopted`: `h7`, `h9`, `h10`, `h11` **0 `ABORTS` each** (at most 1 asked); `h7`, `h9`,
  `h10` 5 of 5 `RUNS` (promised), `h11` 5 of 5 `NOT PROMISED` (its *may*, text-right). `rival`: **0 `ABORTS` each**
  (`h9` 5 `RUNS`, the rest 5 `NOT PROMISED`). `now`: 0 `ABORTS` each, 20 of 20 `NOT PROMISED`. **What it says**: no
  reader of any arm read a *may* as *aborts* on a program the key runs; the narrowing 257 found did not come back.
- **D3, the aborts and their words: HOLDS.** `adopted`: `h2`, `h5`, `h6`, `h8` **5 of 5 text-right each** (every one
  `NOT PROMISED` on its *may*); no `ABORTS` among them, so no words to grade. `rival`: 5 each (`h6` 3 build-right with
  the words, 2 text-right); `now`: 5 each (`h6` 1 build-right with the words). **What it says**: the texts are read as
  leaving these four open, never as promising a run.
- **D4, reported.** **`h4`**: `now` **5 `ABORTS`**, the words right (no text-right under `now`); `adopted` **5
  `NOT PROMISED`**, text-right (*"from inside it"*, the synthesis's class); `rival` **4 `ABORTS`** with the words and
  **1 `NOT PROMISED`, wrong** (`t1-k-2`: *"the text never says this case must abort. It only says 'may abort'"*).
  **`n4`**: `now` **5 `RUNS`, wrong**; `adopted` **5 `ABORTS`, the thread's words, build-right**; `rival` **5
  `RUNS`, wrong**. `n3` against the day's build: it runs on both platforms, no split. **The labels per arm and task,
  each reader's** (§ The labels): `now` t1 L 5 and 5; `now` t2 W 11, H 11, L 5, each reader alike; `rival` t1 O 1 and
  1; `rival` t2 L 5 and 5; `adopted` none. Agreement: same set on 27 of 27 units, kappa 1.00 on W, L, H and O.
- **D5, the list not read as exhaustive: HOLDS.** `adopted`: `h12` **0 `RUNS`** (at most 1 asked), 5 `NOT PROMISED`,
  text-right. `now`, reported: **5 of 5 `RUNS`**, every one wrong (the key aborts in the guard's words), **each
  labelled L by both readers**. `rival`: 0 `RUNS`. **What it says**: `now`'s list is read as closed by every reader, so an exit it does not
  name (`h12`'s `siglongjmp` onto a stack never given to `makecontext`) is read as promised; both candidates name it.
- **What the run does to the resolution**: D1 and D2 hold for `adopted`, so the resolution does not move to `rival`
  and the sitting is not reopened on the text; D3 and D5 hold, so the text is not amended. **Beside the rules,
  reported**: `rival` holds D1, D2, D3 and D5 as well, and differs from `adopted` on `n4` (5 `RUNS` against the key's
  abort, which only `adopted`'s thread sentence names) and on `h4` (one reader wrong where `rival` has no text-right).

## The spec-warden's predictions, scored (`warden-at-start.md`, registered before any session)

Read as `registration.md` § How the warden's predictions are read and the amendment's rule 1: **`rival`'s diff over the
pinned spec is `spec-w1.diff` byte for byte** (`cmp`), so the `spec-w1` column is read for `rival`; the `now` column
for `now`; **`adopted` is unpredicted** (no warden diff is its own: `cmp` of `synthesis/spec-adopted.diff` with each
`spec-wN.diff`, none equal), and the `spec-w2` column is read against no arm. A point is right where it equals the
measurement; a range where it holds it.

| prediction | registered | measured | point | range |
|---|---|---|---|---|
| `now` D1 build-right `h3` / `h13` / `h1` / `n1` / `n2` / `n3` | 5 / 4 / 3 / 3 / 2 / 2 (1 to 5) | 5 / 5 / 5 / 4 / 0 / 0 | right on `h3`; wrong on the rest | holds on four; **out on `n2`, `n3`** |
| `now` `NOT PROMISED` on `n1` to `n3` | 2 to 3 each | 1 / 5 / 5 | | **out on all three** |
| `now` D2 `ABORTS` `h7` / `h9` / `h10` / `h11` | 2 / 1 / 2 / 1 (0 to 3) | 0 / 0 / 0 / 0 | wrong | holds |
| `now` D3 right `h2`, `h5`, `h6`, `h8`; words right | 5 each (4 to 5); right | 5 each; right | right | holds |
| `now` D5 `RUNS` on `h12` | 2 (1 to 4) | 5 | wrong | **out** |
| `now` `h4` `ABORTS` | 3 to 5 | 5 | | holds |
| `now` `n4` `ABORTS` | 1 to 3 | 0 | | **out** |
| `rival` D1 `h1` / `h3` / `h13` / `n1` / `n2` / `n3` | 3 / 5 / 5 / 5 / 4 / 4; `h1` at risk | 5 / 5 / 5 / 5 / 5 / 5 | right on `h3`, `h13`, `n1`; wrong on `h1`, `n2`, `n3` | (none given) |
| **`rival` D2**: `h9` 0; `h7`, `h10`, `h11` 1 to 2 each; **fails on one at least, 60 %** | | `h9` 0; 0 / 0 / 0; **passes** | right on `h9`; **wrong on the failure** | **out on `h7`, `h10`, `h11`** |
| `rival` D3 | 5 each | 5 each | right | |
| `rival` D5 `RUNS` on `h12` | 0 to 1; passes | 0; passes | | holds |
| `rival` `h4` `ABORTS`; `n4` `ABORTS` | 3 to 5; 1 to 3 | 4; 0 | | holds; **out** |
| D4 labels, `now` (both tasks) | W 8 to 14, H 4 to 10 (with W), L 2 to 4, T 1 to 3, N 2 to 6, I 1 to 3, P 0 to 2, R 1 to 4, S 1 to 4 | W 11, H 11 (every one with W), L 10, T 0, N 0, I 0, P 0, R 0, S 0 (each reader alike) | | holds on W, P and *H with W*; **out on H, L, T, N, I, R, S** |
| D4 labels, `rival` (both tasks) | W 0 to 2, N 3 to 6, L 0 to 1, T 0, I 0, P 0 to 2, R 1 to 3 | W 0, N 0, L 5, T 0, I 0, P 0, R 0; O 1 (unpredicted) | | holds on W, T, I, P; **out on N, L, R** |
| the spend, 31 sessions; no session cut by its cap | 14.5 USD (12 to 18); none | 11.66; none | wrong; right | **out (below)**; |

**The warden's own falsifier, registered beside its table**: *"w1 passing D2 on all four (then the list's may does not
invite N, and w1, 119 vendored tokens cheaper, is the better text)"*. **`rival` (w1) passed D2 on all four**: 0
`ABORTS` of 20 on `h7`, `h9`, `h10`, `h11`, the readers reading its *may* as open (`NOT PROMISED`) rather than as
*aborts*. By the warden's registered words that falsifies its recommendation of w2 over w1. The registration's own
rule reads only `adopted`'s D1 and D2 to move the resolution, and `adopted` (not w2) holds both; whether the
measurement turns the choice between the two texts is the sitting's and the author's, with `rival`'s two wrong
answers' kinds (`n4` 5 of 5, which `rival` leaves silent, and `h4` 1) beside it.

## The `context` question

**31 of 31 answers say no instruction, rule, contract, memory or document outside their folder reached them** (each
`result.md`'s `context` read whole, `score/context.txt`). Seven name the working directory's path, which the system
prompt gives (`t1-q-3`, `t1-s-3`, `t2-k-1`, `t2-k-5`, `t2-q-1`, `t2-s-3`, `t2-s-5`: *"262-llm-ergonomist"*), a path
and not a rule, as in panels 257, 259 and 261. The scan's one flag, the verdict's, is the readers' own words read back
(above). **0 sessions voided.**

## The verdict session

Built by `make_verdict.py` after the 30 (72 files: `spec-A.md` `now`, `spec-B.md` `adopted`, `spec-C.md` `rival`,
`changes.diff` and `changes-C.diff`, the two tasks, the 35 programs, **30 reports** named by label, `brief.md`); 62
turns, 1.332397 USD. **`verdict`: approve** the change to `spec-B.md`, and *"A model predicts these programs better
from spec-B than from spec-C"*: the five `spec-B` readers agreed with each other on all 17 programs; `spec-C`'s clause
*"a jump or a `setcontext` onto a stack once given to `makecontext` or into a coroutine the signal did not interrupt"*
reads two ways, its readers flagging two correct programs (`h7`, `h10`) as *may*; `spec-C` keeps `spec-A`'s split on
a callback called inside a handler (`h4`, `h6`) and says nothing of a callback on a thread C started (`n4`); both fix
`spec-A`'s *"a handler of C's"*, read as a running callback (`n2`, `n3`, 5 of 5). **It matched the reports to the
versions by their quotations** (*"`q` reports quote B's ... `k` reports quote C's ... `s` reports quote A's"*), so the
labels' blinding did not hold for it; by its `context` it read `spec-A.md` whole and `spec-B.md` and `spec-C.md` as
the diffs over it.

## What is unrun, and what the run could not do as registered

1. **The labels** are the two readers', run by the coordinator after this report's first writing (§ The labels); this
   session labelled none. **N, T, I, P, R and S have no instance** in this run's wrong answers, so their agreement is
   unmeasured still, as after panel 259's trial.
2. **The amendment's three changes** (the pinned spec, `h13` without `layer`, the version) are the coordinator's
   process choice, recorded for the author to turn; under the registration's letter the run would not have started.
3. **The x86-64 legs and route A** are not graded (§ The key); nothing here says anything of Darwin x86-64 or of glibc's
   other architectures. **S** (a callback recursing past a `malloc` stack) stays unrun by any seat.
4. **The scorer was not blind**, and the verdict read the arms off the quotations (above).
5. **No session could write `report.md`**: 31 answers are the fallback `result.md`.
6. **The run went on while its session was paused** (about 03:45 to 06:51, the five-hour limit): no session was cut,
   none was started twice; the runner's own stops (the ceiling, a session not a success, a file outside the area) never
   fired.

## The readers' packets

Prepared, not run (§ The labels' rule, *The two readers*), in **`.claude/worktrees/blind262-scratch/readers/`**: one
folder two fresh readers can each be handed: `README.md` (their brief), `grades.md` (§ The grades as registered and
the three rules), `labels-template.tsv`, and **27 units** under random ids (`units/u<6 hex>/`, 159 files): each the
session's `result.md`, its task, the program's files, and `unit.md` naming the program, the reader's verdict and that
it is wrong. Never the arm's name or its spec, the key's outputs, the decision rules' results or another reader's
labels; the map from id to session is `score/readers-map.tsv`, outside the folder. The units: `now` `h12` 5, `n1` 1,
`n2` 5, `n3` 5, `n4` 5; `rival` `h4` 1, `n4` 5. **One registered text names an arm**: § The grades' label T reads
*"(`now` has no "on its thread")"*, handed verbatim as the registration asks.

## The labels, by the two readers (added 2026-11-27 about 07:05)

**Two fresh readers**, started by the coordinator after the run, each handed a copy of the packet
(`.claude/worktrees/blind262-readers/A/` and `B/`, their `units/` the packet's by `diff -r`), labelled apart. Their files are in
`262-reports/blind-readers/`: `labels-A.tsv` (sha256 `34f3721d...4683`), `quotes-A.md`, `labels-B.tsv`
(`47119670...f5b7`), `quotes-B.md`, and **`agree.txt`**, the registered `262-briefs/blind/labels-259/agree.py` over the
two, run by this session (`322421...a35d`): **units 27; same set 27; W 11 and 11, L 15 and 15, H 11 and 11, O 1 and 1,
each kappa 1.00; N, T, I, P, R and S given by neither.** (The two files differ only in the order of `W,H` and `H,W`.)
No unit is labelled S; both readers' quotes say why: no answer block offers § 9's stack abort as its program's ground.

**Each unit mapped back to its arm** by `score/readers-map.tsv` (written when the units were drawn, never in the
packet) and the draw:

| unit | session | arm | program | reader A | reader B |
|---|---|---|---|---|---|
| `u09c3f3` | `t1-s-1` | `now` | `h12` | L | L |
| `ub9ca38` | `t1-s-2` | `now` | `h12` | L | L |
| `u0473ad` | `t1-s-3` | `now` | `h12` | L | L |
| `u61937c` | `t1-s-4` | `now` | `h12` | L | L |
| `u269cc5` | `t1-s-5` | `now` | `h12` | L | L |
| `u080bd2` | `t2-s-1` | `now` | `n2` | W, H | W, H |
| `uf2dfc3` | `t2-s-1` | `now` | `n3` | W, H | W, H |
| `u7da432` | `t2-s-1` | `now` | `n4` | L | L |
| `u35bc76` | `t2-s-2` | `now` | `n1` | W, H | W, H |
| `uc1367f` | `t2-s-2` | `now` | `n2` | W, H | W, H |
| `u60e4d9` | `t2-s-2` | `now` | `n3` | W, H | W, H |
| `uae4e9c` | `t2-s-2` | `now` | `n4` | L | L |
| `u7e974c` | `t2-s-3` | `now` | `n2` | W, H | W, H |
| `u957c01` | `t2-s-3` | `now` | `n3` | W, H | W, H |
| `u9d8d7e` | `t2-s-3` | `now` | `n4` | L | L |
| `ue7adf3` | `t2-s-4` | `now` | `n2` | W, H | W, H |
| `u5867b3` | `t2-s-4` | `now` | `n3` | W, H | W, H |
| `u096416` | `t2-s-4` | `now` | `n4` | L | L |
| `u0df6bf` | `t2-s-5` | `now` | `n2` | W, H | W, H |
| `ud61665` | `t2-s-5` | `now` | `n3` | W, H | W, H |
| `ua0e913` | `t2-s-5` | `now` | `n4` | L | L |
| `u847741` | `t1-k-2` | `rival` | `h4` | O | O |
| `ub3a4ed` | `t2-k-1` | `rival` | `n4` | L | L |
| `u728a97` | `t2-k-2` | `rival` | `n4` | L | L |
| `u7a9a15` | `t2-k-3` | `rival` | `n4` | L | L |
| `u24836e` | `t2-k-4` | `rival` | `n4` | L | L |
| `u562d58` | `t2-k-5` | `rival` | `n4` | L | L |

**Per arm and task, each reader's alike** (D4 keeps them apart; here they are equal): **`now` t1: L 5** (`h12`, the
list read as closed); **`now` t2: W 11 and H 11** (`n1` 1, `n2` 5, `n3` 5: the sentence's *may* read as covering a
coroutine switch, *handler* read as a callback's frame of C's), **L 5** (`n4`, the silence read as a promise, the key
aborting in the thread's words, L's second clause by the third rule); **`rival` t1: O 1** (`h4`: both readers quote
*"the text never says this case must abort. It only says 'may abort', and only about the cases it lists"*, a
prohibition read as having no promised run-time result, which no label names); **`rival` t2: L 5** (`n4`, as under
`now`); **`adopted`: no wrong unit, no label.**

## Where the answers are

`262-reports/llm-ergonomist-blind-answers.tar.xz` (sha256 `0cc57786...0539`, 62 entries): each of the 31 sessions'
`result.md` and `run.json`, never a key or a spec. The area (`.claude/worktrees/262-llm-ergonomist/`: `key.txt`,
`order.txt`, `run.log`, `runner.out`, `read_runs.out`, `scan.out`, `reads.out`, the preflight's outputs, the
transcripts under `home/`) and the scratch (`.claude/worktrees/blind262-scratch/`: `notes.md`, `score/`, the keys
`key-mac-9b88/` and `linux/L-key/`, `readers/`) stay uncommitted. The readers' labels, quotes and agreement are
`262-reports/blind-readers/` (five files); their working copies `.claude/worktrees/blind262-readers/`.
