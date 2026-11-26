# Panel 261, the blind reading: the registration. REGISTERED, NOT RUN.

Written 2026-11-26 by the sitting's clerk, from 01:20, at the first base `07e747bc6`; **repaired** the same morning,
from 09:44, at the base `df92f9281` from the critic's first reading (`docs/panel/261-reports/critic-briefs.md` § 4,
its seven items, each applied below where it is marked *repaired*), **before any session is drawn and before any
session runs; none is drawn and none runs**. **Authorised by the author on 2026-11-26** (their words *ok a 20
dollars*, meant as: *the blind seat of 261, paid: 20 dollars*; relayed to the repair by the coordinator, and carried
by the clerk's draft as relayed at 00:59): **the coordinator starts it after the synthesis**, on that word, with a
ceiling of **20.00 USD** for its **31 sessions**, this file's inputs, by hash, and what the synthesis adds below. The tasks, the grades, the key, the
decision rules, the size and the caps are `docs/panel/261-briefs/llm-ergonomist.md` as it stands at the commit that
carries this file. The spec-warden's predictions are its own (`docs/panel/261-reports/spec-warden.md`, `## Predictions
registered before the blind run`), copied and hashed by the runner as it starts.

## The inputs, by hash (`shasum -a 256`, the clerk at 01:20 at the first base `07e747bc6`; **re-run by the repair at 09:59 at the base `df92f9281`**: every hash and byte count below holds but the `now` arm's, which the move changed)

| file | sha256 |
|---|---|
| `arms/spec-now.md`: `spec/heroes-spec.md` at the base `df92f9281` (516 lines, 35,246 bytes) | `3df14254998f68eb4a95ffe6f4c52b6772e2b0298a88ed82eee4b436fd12d93f` (at the first base, 34,828 bytes: `aaf23b9e7f76472e672a1ce51d6c7efbe00f5e21c2b83d8f5caea44a6af616f3`) |
| `arms/spec-adopted.md`: `now` with the synthesis's adopted text applied, **written by the synthesis**, its text and diff hashed here beside it | `e9b0c819cce1c91ec74e43f2197bdd490239ccacae53af7ca9e943052bb2758c` (527 lines, 36,286 bytes; the synthesis, 2026-11-26, § What the synthesis added below) |
| `arms/spec-rival.md`: `now` with the rival text applied, **registered and not run** (§ What the synthesis added below) | `4fc894caa1bd2e8d99d634de36b857ea18c8859881ed2972e894c7273670331b` (521 lines, 35,724 bytes) |
| `tasks/task-t1.md` (`blind/task-t1.md`, 2,058 bytes, `wc -c`) | `1695341319fbd053b581589cb0c3330e0f002f642edc6a4dc9f57bb3b521abe8` |
| `tasks/task-t2.md` (`blind/task-t2.md`, 1,916 bytes) | `2b952b79348c1b9b8135c9be3bffb6cef33375086588e849a8ef8c48ca424c45` |
| `tasks/task-t3.md` (`blind/task-t3.md`, 1,430 bytes) | `cea2d2b6e5f03a40226d5820aaa56c887fcd5cdc27152bd9b9cf1022904f8cfd` |
| `tools/brief-session.md` (`blind/brief-session.md`, 1,757 bytes; the repair) | `e8fee1a40c583c725ea6339666d5d1b2289549fe2b344f5b5aa5a484abe30222` |

**The key's files, never in a session's folder**:

| file | sha256 |
|---|---|
| `reference-t1/expected.txt` (7 lines) | `2ae4e08e5a8c1cdec0836c5da27564d985dc27dfab8d269913648c2ffdd74806` |
| `reference-t2/expected.txt` (10 lines) | `b5a66e86ce310f7d941fddef87341d7e43f2bad41133dc9be861d0dd257ef96c` |
| `reference-t3/expected.txt` (6 lines) | `9fb8cf31f5691eccd1bd4acc6e31fa4f4ed74494ec1d0fcdafa147407e95ea48` |
| `reference-t1/main.hero` (3,475 bytes) | `9207b1ceec9488a49a6f3b502378c9ac8d2a0e8c9995f864fd3716ea908bfaae` |
| `reference-t2/main.hero` (3,240 bytes) | `891e9d218038c74b5d4d9be30d6bdfdfce3c1edc1d641046f59a101a8c82a74b` |
| `reference-t3/main.hero` (1,082 bytes) | `e147918d54d975ebbaee0b1dedb360a5d9a1250d4d88af9d15f39523c48ae4c6` |

## The reference, run

Each task written by the clerk **under `now`**, with no mode in a function type: every bound function takes the state
**by value and answers the new state** (route (f) of `00-shared.md` § 7). Checked, formatted and run by the first
base's stage 2 (`heroes-next`, `sources 30e08049445a2331`) from `.claude/worktrees/panel261-scratch/tree`, finished at
01:20:17 (`stat` of the results file): `heroes-next check` exit 0 each; `heroes-next fmt` prints each file byte for
byte as written (`cmp`); `heroes-next run` exit 0 each, stderr empty, the lines of each `expected.txt`
(`reference-results.txt`). **Re-run by the repair at the base `df92f9281`** with its stage 2 (`sources
4dcf3a9f20257573`), from `.claude/worktrees/panel261-repair-scratch/tree` over copies in `../refs/`, 09:58: `check` exit
0 each, `fmt` byte for byte (`cmp`), and `run` **three times each**, exit 0, stderr 0 bytes, each output `cmp`-equal to
its `expected.txt`, nine runs in all (`reference-results.txt`, its second half). On Linux arm64 the critic ran the nine
at the first base (`critic-briefs.md` § 4.1, all `cmp`-equal), and **the repair at the base `df92f9281`**: one
container, `docker run --rm --pull never --name p261-repair heroes-linux-arm64:latest` over `git archive df92f9281
selfhost runtime seed vendor spec` and the three references (`.claude/worktrees/panel261-repair-scratch/linux/`,
about 10:05, `docker ps` reading *Up 3 minutes* at 10:08, to 10:15:34, the log's mtime), `aarch64`, Debian clang 22.1.8; the seed, then stage 2, whose `--version` reads **`sources
4dcf3a9f20257573`**, the Mac's key; `check` exit 0 each and `run` three times each, **nine runs, exit 0, stderr
empty, each `cmp`-equal to its `expected.txt`** (`linux/run.log`); the container removed (`docker ps -a`: none named
`p261-repair`). So the key holds on both platforms the grades read, at the base. **So each task can be written
today, bound, and print right**: the experiment measures what a reader writes under each arm, not whether the task is
possible. Two names the clerk's first draft used were refused and are part of what a reader meets:
`error[needs_label]` at a call of a function with two `[str]` parameters (spec § 9 lines 305 to 309), and
`error[builtin_name_taken]` for a parameter named `args` (spec § 5 line 186, § 11 line 380).

## Every session's folder

**`brief.md`, `task.md`, `spec.md`**, nothing else (panel 257's names; a write task holds no `programs/`, as panel
260's t1): `brief.md` the runner's `brief-session.md`, `task.md` one of the three tasks, `spec.md` one arm, never
named. The key's files, this registration and the references are never in a session's folder.

**The runner** is panel 256's (`docs/panel/256-briefs/blind/`, 23 entries by `ls`, 01:20) adapted as panels 257 and
259 adapted it (panel 260 committed no runner: `ls docs/panel/260-briefs/blind/`, the repair). **This sitting's
`brief-session.md` owes, word for word, before any session**: the spec named `spec.md`; no `programs/` line; under
`answer`, the reader's `main.hero` whole in one fenced block, then `prints` (the lines it expects, in order),
`compiles` (*yes* or *no*, and if no, which line and why) and `parameters` (for each function the program binds, each
parameter and why it is written so); and `choice_points` and `context` as panel 257's. **Written by the repair**:
`blind/brief-session.md` (1,757 bytes, sha256 `e8fee1a4...0222`), and its diff against panel 256's committed
`brief-session.md`, whole, in **`blind/registration-runner.md`** beside this file, with what the run's session must
change in the runner's other files and register there before any session.

## The grades, per session, read the same way by any two graders

The reader's `main.hero` is extracted as written, byte for byte from its fenced block.

- **compiles**: `heroes check` and `heroes build` of it exit 0, on this Mac and in the Linux arm64 image, **under the
  compiler of the language its reader read** (*repaired*, the critic's § 4 item 3): an `adopted` session's program
  under **the landed compiler** (the synthesis's adopted route, built); a `now` session's program under **the base's
  stage 2**, the grade D2 reads, with the landed compiler's answer **reported beside it** and never graded (a `now`
  reader who writes a mode in a function type is refused by the language it read and may be accepted by the landed
  one). **A refusal is recorded with every code `heroes check` prints, each with its line and its message's first
  sentence** (*repaired*, item 6: `check` prints several), never the first alone.
- **prints-right**: its binary, run three times on each platform, prints `reference-tN/expected.txt` byte for byte,
  exit 0. A program whose two platforms disagree has no key and is named apart.
- **bound**: (1) one function, or one literal, builds a list, a map or a record whose entries hold function values,
  one per route, command or pair, each a top-level function; (2) the function that answers is taken from that table
  (by index, by key, or by walking it and matching a pattern) and called through the value; (3) no `if`, `else if` or
  `match` anywhere chooses which function answers by comparing a route's name, a path written as a literal, a command's
  name, a state or an event, beyond the table's own lookup, its `.ok` and `.err`, and (t1 only) the matching of a
  path's segments against a pattern. **The table's own lookup includes a walk of its entries that compares each
  entry's key fields for equality with the request's** (*repaired*, the critic's § 4 item 4): in t1 an entry's method
  with the request's method and its pattern's segments with the path's (the t1 reference's `if route.method ==
  r.method` inside `for route in table`, `reference-t1/main.hero:93-94`); in t3 an entry's state and event with the
  machine's, where the table is a list of `(state, event, function)` records walked by `if t.state == m.state &&
  t.event == e`; in t2 an entry's name with the command's, where the table is a list walked so. **What rule (3)
  forbids** is a comparison with a literal written outside the table (`if r.path == "/stats"`, `if name == "GET"`, `if
  m.state == "locked"`) or a chain or `match` that names the answering function itself. Failing any of the three is
  **unbound**.
- **lost-update**: the program compiles, runs and prints a line different from the key where the difference is a
  change of state that did not persist (a note, a count, a coin, a passage), read from the program: a change written
  into a copy that nothing hands back. Named per session.
- **mode** (reported): for each bound function, how its state parameter is written: `@`, `lent`, by value with the new
  state answered, by value with nothing answered, or other; and, in t1, where the count of requests is kept (in the
  handlers, or outside them), which decides whether t1 has read-only handlers at all (D3 below).
- **self-write** (reported, *repaired*, the critic's § 4 item 2): a bound function that writes its state parameter, or
  a part of it, to itself (`app @ app`, `app.n @ app.n`), a write that changes nothing and passes `never_rebound` at
  the base (`00-shared.md` § 3.4, probes `c1`, `c8`). Such a program is graded on every other row as written, and
  named in this row, per arm and task, with the line; **whether such a write is a write by design is the sitting's
  question**, which the critic found no ruling on.
- **refused-for-mode** (*repaired*, the critic's § 4 items 1 and 6): a refusal any of whose printed codes is about a
  mode, **read by its message where one code has several**: `never_rebound` counts only with the message of **an `@`
  parameter nothing writes** (*"... is an `@` parameter and nothing writes it"*, `selfhost/resolve/written_nowhere.hero:92`)
  or **an `@` on `_`** (*"`_` binds nothing, so an `@` on it marks a parameter nothing can write"*, `:75`), and
  **never** with the cell's (*"... is declared a mutable cell and nothing re-binds it"*, `selfhost/resolve/errors.hero:340`)
  or a cell declared on `_` (`:357`), which are mistakes about cells, reported as such; `mutable_parameter_as_value`,
  `lent_parameter_as_value`, `marker_mismatch`, `expected_type` at an `@` in a type, `expected_function_type_params_close`
  at a `lent` in a type, and the landed route's new codes, which the synthesis lists here before any session, count by
  their code. A program refused for several codes is refused-for-mode where any one of them is, every code named.

## The decision rules (`llm-ergonomist.md` § The decision rules, restated by number only)

D1, D2, D3, D4, D5 there. **D3 gates on t2 and t3 alone, and t1 is reported beside it** (*repaired*, the critic's § 4
item 5): t2's `GET`, `KEYS` and `COUNT` and t3's `locked` and `push` change nothing by the task, whatever a reader
writes, while t1's `GET` routes change nothing only where the reader keeps the count of requests outside the handlers,
a choice the task leaves
open (the reference keeps it in `main`, `s.answered @ s.answered + 1`); so t1's refused-for-mode programs are counted
per arm and reported with where each keeps its count, never gated. **The self-write row** is reported beside D3, and
a D3 that passes on self-writes says so. **The synthesis adds here, before any session**: the adopted text's hash,
the landed compiler's `--version` line and the codes of the landed route that count as refused-for-mode (with the
message that counts, where a code has several).

## How a prediction is read

The spec-warden predicts for each of its own texts before any is adopted. The `adopted` arm is read against the
warden's text whose diff is byte for byte the arm's; else against its text of the same route, every line the arm's
diff has and the text does not named in the scoring; else the arm is **unpredicted**, neither right nor wrong.

## What the synthesis added, 2026-11-26, before any session

Written by the sitting's synthesis (`docs/panel/261-a-function-type-carries-its-parameters-marks-lent-fits-where-at-is-asked-and-a-callback-c-calls-is-checked-at-its-thunk.md`),
**before any session is drawn; none is drawn and none runs.** Every hash is `shasum -a 256` of the file named, run by
the synthesis that afternoon; every count is `heroes measure` of the file with the base's stage 2 (`sources
4dcf3a9f20257573`) from a tree of the base's `vendor/`, no `--refresh` (none authorised): **a lower bound, vendored
only**.

**The arms of the run are `now` and `adopted`**, as `llm-ergonomist.md` § The arms registers them (*"A rival text would
be a third column of 15 sessions, which the ceiling does not hold at the caps below (45 × 0.60 + 1.20 = 28.20 USD): a
second reading needs its own authorisation"*). The coordinator's brief to the synthesis asked for two arms, `adopted`
and `rival`; the synthesis keeps the registered design, since D2 compares `adopted` with `now` and a third column does
not fit the ceiling, and **registers the rival's text and hash here, not run**, so that its reading needs only its own
authorisation and no rewrite of this file.

| arm | its text | text sha256 | diff sha256 | cl100k_base | claude-legacy |
|---|---|---|---|---|---|
| `now` | the base's spec | `3df14254998f68eb4a95ffe6f4c52b6772e2b0298a88ed82eee4b436fd12d93f` | — | 10,245 | 10,067 |
| `adopted` | `docs/panel/261-reports/synthesis-adopted.txt`, the base with `synthesis-adopted.diff` applied by `patch -p1` (`cmp` equal), 36,286 bytes | `e9b0c819cce1c91ec74e43f2197bdd490239ccacae53af7ca9e943052bb2758c` | `202d8a03a086281da72f4da493534f55df4d30442e687f47fbbcf37858044582` (13,638 bytes) | **10,555 (+310)** | 10,366 (+299) |
| `rival` (registered, not run) | `docs/panel/261-reports/synthesis-rival.txt`, the spec-warden's `spec-w1-fit.txt` byte for byte (`cmp` of the texts and of the diffs) | `4fc894caa1bd2e8d99d634de36b857ea18c8859881ed2972e894c7273670331b` | `15d0c16c854599ee1d31c48424c4ed3a94f1169d6aa13a1071c6824f2a7cca07` (12,263 bytes) | 10,394 (+149) | 10,211 (+144) |

**`adopted`** is the synthesis's sentence: the warden's `spec-w1-fit` with five changes, each measured alone against the
base (`programs/synthesis/spec/pieces.py`): § 3's `TypeArg` (+13, the warden's); § 5's *"an `@` argument counting and
`x @ x` not"* (+7, the warden's piece, in no text of its); § 9's field first at a dot (+19, the warden's); § 9's values
bullet (**+206**, the warden's +100 and three sentences: the names' fit the right way round, the order-free type of a
literal's elements and of a branch's or an arm's value, and the `extern` function that is no value); § 11's `spawn`
(+12, the warden's); § 13's callback (+53 in place of the warden's +5: an `@` parameter the place C hands, a `lent` one
that place through a pointer to const, and the two aborts). The `spec` suite on it: 32 passed, 4 failed, the four
checks that pin the count and its ledger (`budget`, `spendable`, `real`, `ledger`), as on every warden text; the
`grammar` suite: 12 passed, 0 failed (`programs/synthesis/spec/suite-*-adopted.txt`).

**`rival`**, the second-ranked text, is the spec-warden's `spec-w1-fit`: the route the resolution takes (the fit), the
text the warden approved, false against the resolution on four clauses (§ 13's *"with neither mark"*, no `extern`
exception, no order-free literal, `x @ x` a write).

**How the warden's predictions are read**: `rival`'s diff is `spec-w1-fit.diff` byte for byte, so W1's predictions are
read for it as registered. `adopted` has no warden text whose diff is its own; its route is W1's, so **W1's predictions
are read for `adopted`**, the scoring naming the lines its diff has and W1's does not: § 5's *"and `x @ x` not"*; § 9's
*"and a function whose parameters share no type names none, so it fits a type naming them"*, *"Where the elements of a
list or a map, the branches of an `if` or the arms of a `match` are such values, they take the type each of them fits,
in any order"* and *"A function of an `extern` group is no value where a parameter is `@`, or a record `lent`: those
marks are C's (section 13)"*; and § 13's *"an `@` one the place C hands it and a `lent` one that place through a pointer
to const, a number's too, and a null place, or two that overlap where one is `@`, aborts, naming the callback;"* in
place of W1's *", with neither mark,"*. The warden's report, `## Predictions registered before the blind run`, read by
the synthesis at sha256 `1a7cf8e8d786c7183ec0095bf1a502244adaa8ca976d3a54d64bfa747e130f43` (`docs/panel/261-reports/spec-warden.md`).

**The codes that count as refused-for-mode, added to § The grades' list** (the critic's second reading, item 3: as
registered, the route's answers to a mode mistake were not counted, and the warden's own expected refusal would have
passed D3). Each with the message that counts, as the compiler-engineer's route prints it (`heroes-261-v4`; the cases'
outputs in `docs/panel/261-reports/programs/critic/leg/mac-v4/`):

- **`type_mismatch`**, only where the two types it prints are function types and one writes `@` or `lent` at a position
  where the other writes another mark or none, whatever else differs: *"expected `(function(App) -> ())`, found
  `(function(@App) -> ())`"* and *"expected `(function(App lent) -> ())`, found `(function(App) -> ())`"* (`m03`),
  *"expected `(function(a type parameter) -> ())`, found `(function(@App) -> ())`"* (`m08`); the warden's expected
  refusal, a by-value `s: Store` in an `@` table, is this message. A `type_mismatch` whose two types carry the same
  marks does not count;
- **`not_mutable`**, only where its message says an `@` argument *"would be refused"*: *"`k` was bound with `=`, which
  binds once, so `@k` would be refused"*, *"`p` is a parameter without `@`, so `@p` would be refused"*, *"`x` is the
  loop variable, bound afresh for each element, so `@x` would be refused"* (`w1`, `w5`), at a call through a value or by
  name; its other messages do not count;
- **`aliased_mutable_arguments`**, by its code: each of its messages names two `@` arguments reaching one value, or a
  `lent` one beside an `@` of its place (`m06`, `m07`);
- **`thread_body`**, by its code, new: *"a thread runs `f(x)` on a copy of `x`, and the `f` given has type ..., whose
  parameter is the caller's place — threads share nothing but a mailbox"* (`q8_spawn`);
- **`lent_shape`**, by its code: `@` and `lent` written on one position, of a signature or of a type (*"`lent` after a
  position marked `@` — an `@` position already hands the caller's place"*, `programs/synthesis/probe/lent_shape_type.hero.txt`).

**The landed compiler's `--version` line: none exists at the synthesis.** The compiler-engineer's route
(`heroes-261-v4`, `sources 1283d3066f568534`) is not the adopted resolution: the order-free literal, the self-write
counted as none, the `extern` function's true refusal and every duty of the C boundary are adopted unbuilt (the
synthesis's R3 to R5). By § The grades, an `adopted` program's *compiles* is the landed compiler's, so **the `adopted`
arm's `compiles`, `prints-right` and `bound` grades wait for the landed build**, its line written here before any
grade is read; the `now` arm is graded by the base's stage 2, which exists. Whether the coordinator draws and runs the
sessions before the landed build and grades them after it is the coordinator's, on the author's authorisation; a
reading graded against a compiler nobody built is not a measurement.

**The ceiling** stands at **20.00 USD for the run**, the author's (*ok a 20 dollars*, 2026-11-26, `llm-ergonomist.md`),
with § The size's caps (0.60 USD a graded session, 1.20 the verdict, margin 0.10); the spend of about 7 to 12 USD is
`llm-ergonomist.md`'s inference, unrun, and the warden's prediction is 11 USD (7 to 17).
