# Panel 252, the completeness critic's first reading: the briefs against the world

Read 2026-11-19 from 14:54 CET (`date`), before any seat started. Every command ran in the critic's own
copy, `.claude/worktrees/rebel-scratch/252-critic/tree/` (a `git archive` of
`181555e9a32d0b18878f9f10e48d2eadd939c285`, 20,037 files by `find -type f | wc -l` and by the archive's
`tar -t | grep -v '/$' | wc -l`, as `00-shared.md:67-68` says), or against the git objects of the
worktree `.claude/worktrees/rebel-round`, never its working tree. Load at the start 13.26 (`uptime`).
No paid run, no `claude -p`, no API call, no credential printed, no GitHub write, no sub-agent, no
commit. Temporary files under `.claude/worktrees/rebel-scratch/252-critic/`.

Kinds: **wrong** (a fact a brief hands a seat that the command contradicts), **unrun** (a claim with no
command behind it, or a negative or an *only* not searched), **route** (an option nobody listed),
**rule** (a rule of `SKILL.md` § 2, 3, 3b, 3c or CLAUDE.md a brief does not hand its seat).

Written as it goes; the findings are numbered in the order they were found.

## Findings

### 1. Q2-C's sentence is false of line 6 of the very task that measures it, so its cell scores the arm's error, not the sentence's effect (wrong; changes what the blind seat measures and what the spec-warden prices)

`00-shared.md:242-244` offers Q2-C as *"the spec follows the compiler"*, arm `q2-c`: *"`ok(...)`, `[]`, `{}`
and a case name give none, **nor does a literal holding one**,"* (`bw/arms/q2-c.diff`). Task 2's line 6 is
`print(nodes({"a": ["b"], "b": []}))`, a literal holding `[]`. **The compiler takes `K` from it**: task 2's
program, cut from `252-llm-ergonomist/tools/task-t2.md` into `252-critic/work/t2/main.hero`, under my stage 2
(`sources 23c7d2a626b29fd5`), `heroes-next check main.hero` prints one error, `error[cannot_infer]` at
`main.hero:7:26`, and nothing for line 6 (exit 1); `bw/q2/n-two-entries.hero` (`{"a": ["b"], "b": []}`) is
accepted, exit 0. So the arm states a rule stricter than the compiler, and a reader who reads it exactly
predicts **line 6** refused; the task asks for *"the line the compiler refuses"*, singular, so that reader
names line 6, and the rule *"Q2-C's sentence carries when at least 4 of `q2-c`'s 5 predict the refusal of
line 7"* (`llm-ergonomist.md:194-195`) counts them against the sentence; the grading row (`:179`, *"line 6
not refused"*) marks them wrong. **Repair before the run** (the arm is in five session folders and
`key.txt`): word Q2-C as the compiler behaves, measured on `bw/q2/`: a literal gives none when every one of
its elements or values gives none, its keys not read (`a`, `k`, `o`, `g`, `l` refused; `b`, `c`, `d`, `n`,
`j`, `f` accepted); re-measure it; re-make the five `t2-g-*` folders and `key.txt`. Smaller, same place:
`bw/q2/` holds **15** programs (`ls *.hero | wc -l`), not the *"sixteen"* of `compiler-engineer.md:75`, and
`f-array-lit` (accepted) is in no table; all 15 re-run here print the writer's `.out` byte for byte (`cmp`).

### 2. The q3-r arm's example is task 3's answer, and task 3's two arms are graded by two different instruments (wrong framing; changes what the blind seat measures)

`bw/arms/q3-r.diff`, the arm five `t3-f-*` sessions and the verdict read: *"`record Ring<T>` holding `items:
[T]` is written `Ring<i64>` in a type"* (`spec-q3-r.md:306-307`, `spec-proposal.md:309-310`). Task 3
(`tools/task-t3.md`) asks for *"a ring"* of *"items"*, used first with `i64`. The frozen spec holds no word
`ring` (`grep -n -i 'ring\b' arms/spec-now.md`, filtered of *string*, *during*: four lines, none the
word). So the q3-r readers are handed the type's name, its field, its first instance and the spelling of
both, and the now readers are handed nothing; a difference in the cell is the example's as much as the
rule's, and design.md Part 11's question is the rule's. **And the two arms are graded differently**
(`llm-ergonomist.md:180-181`): `now` by building under the frozen stage 2, 3 of 3, where a missing `@`
or a `[]` with no type is wrong; `q3-r` *"built ... on the compiler-engineer's slice where it exists ...
elsewhere graded by the arm's words"*, where the slice is asked only *"if it can be built in your folder in
the sitting's time"* (`compiler-engineer.md:99-104`). Graded by words, an answer the compiler would refuse
can pass; so *"`q3-r`'s right answers exceed `now`'s by at least 3"* (`:196`) leans toward `q3-r` by
construction wherever the slice is not built. **Repair**: an example that is not the task's (a `Pair<A, B>`
or a `Stack<T>`, design.md §4.12's own word, `docs/design.md:1972-1973`), measured again; and a like-for-like
grade, either both arms built (the rule read only where the slice builds the `q3-r` answers) or both graded
by the same words rubric beside `now`'s build, the rule registered on the pair it reads.

### 3. Q5-R's sentence contradicts its own example (wrong; changes what the spec-warden prices, what the compiler-engineer builds, and the verdict session's spec-B)

`bw/arms/q5-r.diff`: *"A float has digits on both sides of its point and may end with an exponent: `2.5`,
`1e-06`, `2.5E3`."* `1e-06` has no point (`printf '%s' 1e-06 | grep -c '\.'` prints 0), so by the
sentence it is not a float, and the sentence gives it as one. A reader cannot tell whether `1e21` (no point)
is a float, an integer, or an error, which is the very case the compiler-engineer is asked to build
(`compiler-engineer.md:130-134`, *"`1e400`"*); the arm stands in `spec-proposal.md`, the verdict session's
`spec-B.md`. **Repair**: word the form the route will read (digits, a point and digits, or digits with an
exponent, or both), let the compiler-engineer's build say which, and re-measure; the arm's +39 is of the
words as they stand.

### 4. Whether 10 USD holds every graded cell: yes at panel 251's medians, by 2.2% per session; at its maxima the stop leaves tasks 3, 4 and 5 short, not 4 and 5; what I would cut first (wrong, small; the answer the coordinator asked for, with numbers)

**The basis, reproduced.** Panel 251's 51 `run.json` (`251-llm-ergonomist/s/*/run.json` and `verdict/`,
`python3 -I`): per task, median and max, t1 0.1602 / 0.2003, t2 0.1847 / 0.2112, t3 0.1595 / 0.1820, t4
0.1399 / 0.1508, the verdict 0.3772, all 51 8.690507; the brief's numbers hold, and its sessions ran with
`--bare` as these will (`251-llm-ergonomist/run.sh:55-61`), so they are the right basis. A least-squares fit
of the 50 graded costs to their `usage` is exact (max residual 0.000000): USD **4, 5, 0.20 and 20 per
million** input, cache-write, cache-read and output tokens; each session's non-output part is **0.072 to
0.076**, so the spend is set by output tokens (251's medians 3,328 to 5,543 per task, max 6,936). Its fourth
task, t4 (a short prediction, median 0.1399), is not in the brief's account; a short program costs less.

**The stop rule over `order.txt`, simulated** (`python3 -I`, `spent + 0.40 + 0.10 <= 9.50` before each
folder, as `run.sh:101-105`): at 251's medians (four writing tasks at 0.1847, task 2 at 0.1602) all 51 run,
**9.3672**, the brief's 9.37, the verdict starting at 8.99 against 9.00. **Every graded cell starts only up
to a uniform rise of 2.21% per session** over those medians (bisection on a scale factor). At 251's maxima
(0.2112, 0.2003) **44 run and the stop comes before `t3-y-5`**: tasks **3, 4 and 5** each short by one
replicate, six cells, not *"task 5's, then 4's"* (`llm-ergonomist.md:162-163`); Q3's rule then reads four
replicates too. Whether this sitting's tasks cost like 251's is unrun: t4 and t5 are short programs (by the
fit, cheaper), t1 writes two files and t3 a ring under a spec with no generic types (unknown).

**What I would cut first: the `t2-g` cell, 5 sessions, 0.80 USD at 251's prediction median (1.00 at its
max).** Its arm is false as worded (finding 1), and Q2 is settled first by the compiler-engineer's build
under CLAUDE.md § 12, where Q2-C *"buys nothing"* if Q2-R builds and moves nothing else
(`compiler-engineer.md:88-89`); the `now` cell of task 2 keeps the reading that matters, how a reader takes
§ 9 today. Without it, every remaining graded cell starts up to a **12.4%** rise per session (same
bisection), and at 251's maxima 43 of 45 run, task 5's last pair short. **Before any cut, a route nobody
listed**: the rule starts the last session at a spend of at most 9.00, so the realised worst case is 9.40
plus one session's overshoot of its cap, and 0.60 of the 10 is unreachable by construction; `CEILING=9.90`
keeps every start at or under 9.40 and the worst case at 9.80 plus the 0.10 margin, inside 10, and at 251's
maxima runs 46 instead of 44 (same simulation). Whether 9.50 is the author's figure or the coordinator's is
not on disk: `coord/brief-writer-common.md:74-75` writes *"the bound **10 USD for the sitting** (the author's,
2026-11-19): each session capped at 0.40 USD, the runner stopping before 9.50 in all"*.

**A rule the grading does not register**: a session that ends in an API error, at its cap, or with no
`report.md` and no usable `result` leaves a `run.json`, so the runner counts it as run and skips it on resume
(`run.sh:97-100`); no row says whether it is re-run, voided or graded wrong. Panel 251 met none; this
sitting's rules should say before the run.

### 5. The preflight proves the three leaks shut but passes on silence, and does not look where a fourth would land (rule; cheap to close before any spend)

What holds, each by command: `run.sh` (sha256 `0687200295cdabdd`, read whole) ends every `claude -p` with
`< /dev/null` (`:75`, `:111`); `s/` is its own repository's root (`git -C s rev-parse --show-toplevel`),
user `reader` / `reader@invalid`, no commit (`rev-parse --verify -q HEAD` exit 1), `.gitignore` the two
bytes `*` and a newline; from `s/t4-y-1` with the runner's `env -i ... GIT_CEILING_DIRECTORIES`, toplevel
`s`, `git status --short` 0 lines, *"does not have any commits yet"*, branch `main`, and nothing written
into `s/.git` (no `index` after); `HOME` is `$HERE/home`, and `~/.claude/projects` holds no folder named for
the area (`grep -c`, 0); `check_folders.py` exit 0; 156 files under `s/` with `.gitignore`. **The coordinator's
probe settles where the status lands**: `coord/blindtest/s/probe4`'s transcript (the only one of the four
probes with one under `home/`; probes 1 to 3 left `run.json`, `total_cost_usd 0`, `Invalid API key`, and no
transcript there or under the real `~/.claude/projects`) holds it at `attachment.context.gitStatus`, the
path `preflight.py:32-37` reads, rendered *"Git user: reader ... Status:\n(clean) ... Recent commits:\n"*;
the same CLI, 2.1.311, is on `PATH` (`/opt/homebrew/bin/claude --version`) and wrote panel 251's 839
entries (`grep -o '"version"'`). **The free preflight is free** by the same probes (four `run.json`,
`total_cost_usd 0`, an API 401).

**What the preflight does not prove**: (i) it passes when the transcript holds no `gitStatus` at all
(`preflight.py:38-39` prints and appends nothing to `bad`) and when `run.json` has no `session_id` (then
`:19` and `:25` check nothing); (ii) it prints the attachment kinds and fails on none, where panel 251's
51 full sessions held exactly `session_context` and `date`, 51 each (`grep -o '"type":"..."'` over
`251-llm-ergonomist/transcripts/`); (iii) it never reads the request the CLI stored beside them,
`api-request-shape`, whose `system` is where a project file would land: in probe4 it is two blocks, *"You
are a Claude agent, built on Anthropic's Claude Agent SDK."* and `CWD: <the folder's path>\nDate:`; the
probe's `home/.claude/plugins/store/cc-plugin-agents-md_builtin-*.json` records *"claude-md-or-agents-md
ok"*, and the root holds an `AGENTS.md` pointing at `CLAUDE.md`, whose text reached none of 251's
transcripts (`grep -r -l 'AGENTS'`, `'rules an AI agent follows'`: 0 each). **Repair, a few lines and
free**: fail on an absent status or session id, on any attachment kind but those two, and on a `system`
other than those two blocks; and record the request's `output_config` (probe4: `effort: medium`,
`thinking: adaptive`), the reader the sitting measures. **And `TMPDIR`**: `env -i` passes none
(`grep -c TMPDIR run.sh`: 0), where `00-shared.md:423-424` binds every seat to keep `TMPDIR` inside its
folder; whether the CLI writes under the system's temporary folder in a session with only `Read` and
`Write` is **unrun** (the files I found in `/private/tmp/claude-501/` at the probe's minute are other
sessions' `bash-edit-diff`); `TMPDIR="$HERE/tmp"` on both `env -i` lines answers it at no cost.

### 6. The refreshes' *"0 USD"* is carried and sits outside the runner's accounting (unrun; rule)

`00-shared.md:33-36` and `spec-warden.md:45-47` count up to ten `--refresh` calls *"at 0 USD inside the
bound"*, on Anthropic's page *"as panel 252's 2026-11-15 brief read it, carried through panel 251's"*.
Today's bundled reference (Claude Code 2.1.326's `claude-api` skill, read by `grep -n -i -E
'free|bill|cost|charge|pric|rate limit'`): `shared/token-counting.md` (56 lines) says nothing of price; the
only *"free"* is a comment in a command example, *"free first pass on the token-counting endpoint"*
(`shared/preserved-thinking-migration.md:157`), the line panel 251's critic quoted. **And `spent.py` counts
only `s/*/run.json`**, so a refresh's cost, if any, is invisible to the stop rule that holds the 10 USD.
Each refresh is three requests, two probes and the document (`selfhost/cli/refresh.hero:169-176`, `:212`;
251's `spec-warden.md:33-40`), each asked of `claude-opus-5` (`selfhost/cli/count_tokens.hero:41-42`,
`PINNED_MODEL`); at that model's input price (USD 5 per million, the skill's model table) ten documents
of about 11,400 tokens would be about 0.57 USD if billed, an inference.
**Repair**: the brief cites a page read today for the price, or the spec-warden logs each refresh's time so
the account can be read against the console afterwards and the coordinator subtracts a stated 0.50 from
`CEILING` while refreshes are owed.

### 7. Q1: a route nobody listed, the face naming a private module's type as its own, and two sittings its record owes (route; unrun claim in the historian's brief)

Q1's options are R (cycles allowed), R2 (rings through `internal/` only), C (the refusal stated) and the
status quo (`00-shared.md:184-194`). What Rebel's 32 packages wrote instead is mostly **a mirror of the
face's types, field for field** (`00-shared.md:157`); the shape that removes the mirror without a ring is
**a face that names a type declared in its private module as its own** (a re-export, or a transparent
alias in the face: the program writes `geom.Point`, the record lives in `geom/internal/types`, the face
uses nothing that uses it). Its record, read today: **panel 031's R4**, *"Every module you name needs its
own `use`" ... "There is no re-export"* (`docs/panel/031-modules-the-namespace.md:178-183`); and **panel 135**,
which sat on design.md Part 7 item 5's `alias` and **deferred it again**, ratified 2026-09-13, with the
form's settled shape (transparent, an error writing both spellings, no generics, an alias never naming
itself) and three return conditions (`docs/panel/135-*.md:124-165`, `:217-221`); its account of Go reads
*"1.9's warrant was refactoring, not brevity"* (`:136-137`). The historian's brief asks that very question of
Go afresh (`historian.md:41-42`, *"was the alias proposal motivated by moving types between packages?"*)
and its *Read before you search* list names neither sitting (`:24-32`). **Repair**: list the route as
Q1-A with its price (a form, so Principle 0's ticket; whether Rebel's mirrors meet a return condition of
135, condition (b) being *"a commit ... changes one structural type spelling at 8 or more sites"*, is
**unrun**); point the historian and the spec-warden at 031's R4 and 135; and ask the compiler-engineer
what Q1-A costs beside Q1-R. Searched: `grep -n -i -E '\balias|re-?export'` over the spec (one line, the
word *aliasing* at 98) and over `docs/panel/*.md`.

### 8. Q3: the record the brief cites misses panel 029, a type parameter brings polymorphic recursion through a type, and Q3-C's *"repaired in the package"* names no mechanism (unrun; a shape beside; a question)

- **The search**: *"no Part 6 row, no Part 7 item (`sed -n 3723,4284p docs/design.md | grep -i generic`)"*
  (`00-shared.md:260-261`): that range is Part 7 alone; Part 6 opens at 3555 (`grep -n '^## Part'
  docs/design.md`). Run over 3555-3722, Part 6 holds no generic-types row (its traits row and Zig's
  history alone), so the claim holds and the command does not show it. A wider pattern than the brief's
  (`generic types?|generics on types|record [A-Za-z]+<[A-Z]|parametric (type|polymorph)|...` over
  `docs/panel`) finds 12 files, among them **panel 029, *generics by monomorphisation***, whose **R4**
  refuses polymorphic recursion in the pass (`f<T>` calling `f<[T]>`, MLton the precedent) and **R5**
  rules the mangled suffix on the canonical rendered type (`docs/panel/029-*.md:110-136`, `:180-213`);
  no file of the 12 rules on records with a type parameter (read at each hit). **Repair**: the brief
  names 029's R4 and R5 to the compiler-engineer as the record Q3-R extends.
- **The shape beside, not in the case list**: a record or variant that holds itself at another instance,
  `record Nest<T>` holding `inner: [Nest<[T]>]`, instantiates without end as 029's `f<[T]>` did; and a
  recursive generic variant (`variant List<T>` with a case holding a `List<T>`). The slice asked
  (`Ring<T>`, `i64` and `str`, `compiler-engineer.md:99-104`) holds neither; Q3-R's price owes both
  (§1.12: refused at the declaration, or built, never a hang or a C stack overflow).
- **Q3-C's repair** (`00-shared.md:292-293`): a `Deque` record and a `[T]` list share no identity, values
  being copies (spec § 3, line 98), so what check could make `deque.pop_front(@jobs_queue, @urgent)`
  abort, as `deque.hero:17-18` promises, is **a question, unrun**; if none, Q3-C's repair is to strike the
  promise and leave the silent wrong program standing (reproduced here: both witnesses built by my
  stage 2 print `taken 10`, exit 0, stderr empty, 3 of 3 each). The compiler-engineer's Q3 item 4 asks
  which of `cache`, `indexed`, `ordered` keep the promise; it should also ask whether any can.

### 9. Q5: a float literal takes `_` today though the spec says it of an `i64` alone; Q5-R's reading has no count; Q5-P has no cell (unrun claim corrected; rule; route)

- `printf 'function main()\n    tol = 0.000_000_001\n    big = 1_000_000.0\n ...'` built by my stage 2,
  run: `1e-09`, `1000000.0`, exit 0 (`252-critic/work/q5r/sep.hero`). Spec § 2 line 66 grants `_` to *"One
  `i64`"*; for a float it says nothing, and the brief's hazard (*"each one where a dropped zero is a
  different number the compiler accepts"*, `00-shared.md:365-368`, 429 literals and 81 over 300
  characters, reproduced: 429 in 88 files with `url/idna/internal/conformance_cases.hero`'s 72 left out)
  has a mitigation the brief does not weigh: grouping, which the compiler takes and the spec does not
  state. **Route**: a sentence giving a float's `_` (beside Q5-R's or Q5-C's form sentence, measured);
  the spec-warden prices it, the compiler-engineer says whether `heroes fmt` keeps it.
- **The decision rule for Q5-R** (`llm-ergonomist.md:204-206`) reads *"Q5-R reads `now`'s right answers
  once regraded"*, with no count; every other robust arm has one (*"at least 3"*, *"at least 4"*). A
  prediction the spec-warden registers for Q5-R cannot be scored *"on these counts as worded"* (`:207-208`)
  without one. **Repair**: register it, e.g. *"`now`'s regraded right exceed its frozen right by at
  least 3"*, before the run.
- **Q5-P (+1) changes the one sentence that shows a reader an exponent**, § 11's *"(`1.0`, `1e-06`)"*,
  which is what task 5 tempts; no cell reads it. Under the budget of finding 4 it fits only in place of
  another cell (5 sessions, about 0.92 USD at 251's writing median): a route, priced, for the coordinator.

### 10. Q4: a narrower robust arm nobody listed, and panel 061's measurement the record omits (route; record)

Q4-R refuses every `u8` in a hole or a `print` (`00-shared.md:325-327`), so every correct program that
prints a byte's number pays a `to_str` (the census the compiler-engineer is asked for). **A narrower
robust arm**: refuse a hole or a `print` argument whose expression is an index into a `str` (`s[i]`,
`word[0]`), the trap's own form, leaving a `u8` bound by name (`b: u8 = 65`, `[65]` in `bw/q4/bytes.hero`,
reproduced) to print its number; what it misses is `c = s[0]` then `f"{c}"`, a cost to state beside Q4-R's.
**The record**: panel 061 measured the same printing, *"`b: u8 @ 'a'` prints 97, `c: i32 @ 'z'` prints
122"*, and ruled on the spec's false sentence (*"Character literals are `i64`"*), not on the printing
(`docs/panel/061-the-fields-nobody-named.md:279-284`); decision 512's sentence followed. The brief's
*"no sitting names a byte in a hole (grep of `docs/panel/` and its reports: two hits, both array indices)"*
(`00-shared.md:320`) holds of a hole and misses this; the historian and the spec-warden should have it.
Searched: `grep -rn -i -E '(u8|byte)[^.]{0,80}(hole|interpolat|f"|print)|...'` over `docs/panel`.

### 11. Smaller points, each with its command

- **The probe's patch is not on disk.** `bw/q1/modules.hero.orig` is `cmp`-equal to the frozen
  `selfhost/modules.hero`, so `compiler-engineer.md:44-45`'s *"the patch in `bw/q1/`'s `modules.hero.orig`
  against the frozen file"* holds no patch. Rebuilt here from `00-shared.md:79`'s two lines inserted after
  line 280 (`function cycles(...)`), in my copy, restored after (`cmp`): **`sources 7023dd011002b5d5`**,
  the writer's key, so the grading compiler is reproducible from the words; keep the patched file or a
  diff in `bw/q1/` all the same.
- `00-shared.md:135-136` names five of the eight comments `grep -rn module_cycle selfhost` prints; the
  three it omits (`grammar_expr.hero:8`, `mutate/typo.hero:30`, `mutate/sites.hero:14`) are in
  `compiler-engineer.md:48-50`.
- **The number 252 is held twice**: `p252/docs/panel/252-briefs/` and `252-reports/` of the 2026-11-15
  sitting stand uncommitted there (`git -C p252 status --short`), so a commit from that worktree would
  meet this sitting's; panel 251's critic said the same of 251. A line in the synthesis.
- **The machine awake** (`00-shared.md:430-432`): at 14:54 `pmset -g assertions` named Amphetamine's
  *"Single-Use - System"* hold (216 hours) beside powerd's, which by `.claude/rules/verification.md` § A
  long run holds the machine awake (2026-10-12) is the hold that counts; `caffeinate` costs nothing.
- **The folder's path, a route**: probe4's `api-request-shape` puts `CWD: <path>` in the system prompt,
  so each session reads `.../rebel-scratch/252-llm-ergonomist/s/t3-f-1`, its task number, its label and its
  replicate. Folders named at random under an area with a neutral name, inside the root, the mapping in
  `key.txt`, would leave `heroes-lang` and `joseph` alone; cheap, and of small value, since the brief
  already tells the reader it is a model judging a specification.
- **The census's clock** (`compiler-engineer.md:34-39`): 1,852 roots per compiler (`grep -rl '^function
  main('` over `examples` 71, `tests/golden/run` 837, `tests/golden/check` 783, and 160 drivers, plus
  `selfhost/main.hero`); one Rebel driver checked in `real 0.71` at load 9.8: about 11 minutes of wall per
  census at `-P 2`, and the frozen compiler's census is shared by every route, so run it once.
- **The verdict session** reads task 3 alone under a `spec-B` holding Q3-R, Q4-R and Q5-R together; its
  `verdict` can speak for Q3 only, and its `spec-B` carries finding 3's sentence.
- **Panel 251's 51 sessions wrote no `report.md`** (`run.log`: `report.md=no` 51, `result.md=yes` 51; 0
  permission denials over the 50 `run.json`): `result.md` is the record the grading reads, as `after.py`
  keeps it.
- The refreshes' *"free"*: today's reading is panel 251's critic's (`251-reports/critic-briefs.md`, finding
  6) and mine (finding 6 here), not the 2026-11-15 brief the shared brief carries.

## What held, so no seat re-pays for it

Each by its command at the frozen commit, in my copy or `git -C .claude/worktrees/rebel-round`: the
commit line; `origin/main` `0becec5b6` (2026-11-15 23:30:55), the merge base `f6c49d84c`, 3 commits
`181555e9a..origin/main`; `f4e3594d5` touching the two evidence files alone; 42 files, 11,786 and 444 since
`3a5ee2691`, 14 / 27 / 1; spec, seed, `selfhost/`, `runtime/` unmoved (`git diff --quiet`, exit 0); no
`panel/252` path at the frozen commit; the spec 492 lines, 29,407 bytes, `1d532d514831aa70...`; 20,037 files;
**the seed `sources 5aae00294f7cd6fa` (`real 9.65`) and stage 2 `sources 23c7d2a626b29fd5`** (`real 129.67`,
`user 95.26`, `sys 24.07`); `measure` of the spec (8551, 8702, 11384) and of all ten arms, every sha256, line
count and vendored maximum as the table gives; every design.md line cited (942-943, 1972-1973, 1976-1996,
2020, 2022-2023, 810, 1752-1755, 253, 290-293, 324-338), every spec line cited, every `selfhost/` and `runtime/`
line and count (`modules.hero` 280 / 324-328 / 431-432, `walk.hero` 1790 and 1806, `generic_argument.hero:285`,
`interp.hero:26-31`, `builtins.hero:176-190`, `number.hero:146-172`, `units.hero:7-20`, the six `wc -l`);
panel lines 031:193-198, 203:52-56, 128:97-102, 035:59-62 and 175-179, 121:291-293, 234:316-319, 251:143 and
253-258, 119:66, 251-briefs 21-26 and 521-526; `SKILL.md` and the seat definitions at every cited line; the
README's 189-191 and 491-493; issue states and titles of decisions 210, 523, 1166, 2403 (its words and its
reading of 2026-11-10), 259, 618, 858, 595, 512, task 2266 open, defects 415, 488, 552, 578 (issues 1754,
1834, 1906, 1940), 2276, 2391, 2589 open, decision 2590 closed; Q1's census reproduced byte for byte (34
packages, 47 lines), 144 ledgers, 160 drivers, 8 `containers` modules, 1,629 and 2,330 lines; Q3's grep, 20
lines; `bw/q2/`'s 15 programs `cmp`-equal to their `.out`; `bytes.hero`'s eight lines; `prints.hero`'s twelve;
the four exponent literals and `5.` refused as said; the exponent goldens; 8 of 811 `run` expectations, 12
`cannot_infer`; the task references, each built and run 3 times by my stage 2 (t1's ring refused
`module_cycle` and `26` under my probe, without the ring `26` under both; t3 `3 4 5 green blue`; t4 `117 3
...` and `u 3 ...`; t5 `exponent_literal` and `true false` twice); **the blind area**: `order.txt` 51 lines,
five rounds alternating, the verdict last; `check_folders.py` exit 0; 156 files; the leak grep over every
session's `brief.md`, `task.md` and the verdict's `changes.diff` printing 0 files, no task file holding
*cycle* or *exponent*; `spec-A.md` the frozen spec, `spec-B.md` the proposal, `changes.diff` equal to `diff
-U2` of the two (headers aside), the verdict's task task 3's; `brief-session.md`, `brief-verdict.md` and
`after.py` panel 251's bytes; `run.sh` and `order.txt` at their sha256; the three repairs as finding 5
records. Not re-run: the writer's Q1 builds under `bw/q1/cycle`, `third`, `public` (read, not built);
`bw/q2`'s two runs printing `1`; the `coordinator-todo.md` lines the brief marks carried.

## What each brief hands its seat of `SKILL.md` § 3, 3b, 3c and CLAUDE.md

| seat | own folder, built from the seed | report as it goes | no credential | no paid run | machine awake, a run bounded | adopted route built |
|---|---|---|---|---|---|---|
| compiler-engineer | `:8-10`, `00-shared.md:398-411` | `:11-14` | `:14-15` | `:14` | `00-shared.md:425-432` | `:29-30` |
| spec-warden | `:9-10` | `:11-14` | `:15`, `:51-53` (length only) | refreshes named, `:45-54`; their cost, finding 6 | shared | not asked (builds no route) |
| historian | builds nothing (no Bash) | `:5-7` | `:9-10` | `:9-10` | n/a; a fetched file outside the root said, `:19-22` | n/a |
| llm-ergonomist | the coordinator runs it, inside the root (`:32-33`) | the coordinator copies (`:213-219`) | the key by length, `run.sh:83-87` | 51 sessions, caps, stop rule; finding 4 | `caffeinate -i -w`, `run.sh:91` | n/a |

The ffi-pragmatist does not sit, and `00-shared.md:18-23` says why with what was searched (CL-023 permits
choosing seats). Every negative of `00-shared.md:435-437` is bound on every seat. Missing from the tables:
nothing but findings 4 (a failed session's rule), 5 (the preflight's silent passes, `TMPDIR`), 6 (the
refreshes' account) and 9 (Q5-R's count).

## Tally

Ten numbered findings and a list of smaller points. **wrong** 4 (1, 2, 3, and 4's *"task 5's, then 4's"*);
**unrun** 3 (6, 8's Q3-C mechanism, 9's float `_` corrected by running); **route** 5 (4's `CEILING`, 7's Q1-A,
9's float `_` sentence and Q5-P cell, 10's narrower Q4 arm, 11's neutral folder names); **rule** 4 (4's failed
session, 5's preflight, 6's account, 9's Q5-R count); **record** 3 (7's panels 031 R4 and 135, 8's panel
029, 10's panel 061).

**Those that change what a seat measures, to repair before any seat starts**: 1 (Q2-C's arm, in five
session folders), 2 (q3-r's example and the asymmetric grade), 3 (Q5-R's words), 5 and 9's count (before
the runner spends), 7 and 8 (the option set and the record the compiler-engineer, the spec-warden and the
historian start from). Finding 4 is the coordinator's answer on the bound, with the cut I would make first.

Finished 2026-11-19 at 15:14 CET (`date`), begun at 14:54; no seat report stood in `252-reports/`, and the
blind area held no `run.json`, no `run.log` and no `s/preflight/` when I last listed it.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/252-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out. `coord/blindtest/`, the coordinator's preflight of a blind session, is in no archive and
is gone.
