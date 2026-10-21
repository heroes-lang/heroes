# Panel 227, completeness critic, second pass: the reports

Read 2026-10-21 in the lane `thesis` at `63c78c54b` (`git log -1 --oneline`), `git status
--porcelain` printing `?? docs/panel/227-briefs/` and `?? docs/panel/227-reports/` alone. Every
lane command ran from the lane's root with its `./heroes-next` (`--version`: `heroes 0.4.0`,
`sources 79fe404274caacd0`). Programs were piped to `./heroes-next check /dev/stdin` (or a variant
compiler's), so no probe file was written. In the compiler-engineer's worktree I read files and
ran its built compilers on stdin; I rebuilt and modified nothing there. The one file written is
this one. No credential printed, no `.env` read, no paid run, no API call, no commit.

This pass gives no verdict. It names what is missing, which claims hold when their command is run,
and which side of each disagreement the commands support.

(Written as it goes; sections are appended in the order the checks ran.)

## 1. The spec-warden's three corrections against my first pass: who is right

Every turn-1 answer of `build/thesis-runs/00fb571c-v3/generations/` (90 files), first fenced block,
put through `./heroes-next check --brief /dev/stdin` (or `check` for the full text) by an inline
`python3 -I` script; no file written.

| claim | the spec-warden | my first pass and the shared brief | what the command says | right |
|---|---|---|---|---|
| Haiku answers writing `slice` with nothing to slice | 10 | 9 | 10 answers hold `slice(from:` or `slice(to:` with no receiver (regex over the first block; Sonnet 0 of 28 calling `slice`, Opus 0 of 30). `check --brief` refuses **9** of them for it at turn 1 (`builtin_shape` + `wrong_label`); the tenth, `29364e29` Haiku s1, is refused `expected_arm_arrow` alone, a parse error, and its turn 2 draws `builtin_shape` (+ `needs_label` x2 + `wrong_label` in strict and brief, `builtin_shape` alone in control), green at turn 3 in all three arms (`verdicts.jsonl`) | **both**, counting different things: 10 written, 9 refused for it at turn 1. The tenth is the one sample where the `slice` message was met at turn 2, and it needed a third turn in every arm, control included |
| the 3 `to_i64` refusals (`c7a30e5b` Haiku s2 and s3, `d37cfed1` Haiku s3) | *`to_i64` on a one-character `str` to get its code … the refusal is `arg_error`, `bad_operand` on `to_i64`, an integer or a float. Not an `i64?` used as an `i64`* | `to_i64(...)` used as an `i64` | the argument is a **`u8`** in all three: `to_i64(lower(b))` with `function lower(c: u8) -> u8`, `to_i64(text[i])`, `to_i64(c)` with `c = s[i]`. `check` refuses each with `error[bad_operand]: \`-\` (or \`+\`, \`>=\`, \`<=\`) takes … found \`i64?\``, never with `` `to_i64` takes an integer or a float ``. That message exists: `to_i64("a")` piped to `check --brief` draws `` `to_i64` takes an integer or a float, found `str` ``, and **no** turn-1 answer of the 90 draws it (0 of the 66 answers calling `to_i64`: Haiku 19, Sonnet 23, Opus 24) | **my first pass**. The spec-warden's correction is false, so its priced remedy (*…, and none takes a `str`*, +7 vendored) prices a fault no answer committed. The fault the data holds is an `i64?` used as an `i64`; its remedy (e.g. writing that `to_i64` gives an `i64?` where the sentence writes `to_i64` bare) is **unpriced**. The spec-warden's counts (3 of 19 Haiku, 0 of 47 Sonnet and Opus) hold |
| `all` and `ok` | bindings, not functions; `ok` is not in the sentence | the shared brief: *2 answers define functions named `all` and `ok`, names the sentence lists*; my first pass (G10): *2 more define `all` and `ok`, names the sentence lists* | `18e95dc7` Opus s2 line 100, `all = split_on(text, '\n')`; `18e95dc7` Sonnet s1 line 30, `ok = (c >= 'a' && …)`; both `builtin_name_taken`. The sentence (`sed -n 316,322p spec/heroes-spec.md`) lists `all` and does not list `ok` | **the spec-warden**. The shared brief was wrong twice (bindings, not functions; `ok` is § 6's constructor, not the sentence's), and my first pass once (`ok` is not a name the sentence lists) |

**The same error was carried into the blind result.** `llm-ergonomist.md` says session 3's two
refusals are *a function named `all`*. Session 3 writes `all = args_checked()` (line 2 of
`thesis-drafts/227/blind-out/w3-01.hero`): a binding. With `all` renamed (`perl -pe
's/\ball\b/argv_all/g'` piped to `check --brief /dev/stdin`) both session-3 programs check at exit
0. So under route A **4 of 6 programs check, the other 2 only for that name**, and 6 of 6 are free
of the `repeat` fault. The blind key holds: `shasum -a 256 build/w/N/heroes-spec.md` reads
`4d8e5251e78d` for 2, 3, 5 and `212264059c98` for 1, 4, 6, and each of the twelve `check --brief`
verdicts in the table re-runs as written.

## 2. The compiler-engineer's two numbers from 050: hold

`verdicts.jsonl` (270 rows: `brief`, `strict`, `control`, 90 each) and `turns/*.json` (96 files),
inline `python3 -I`, read-only.

- **8 of 9 green at turn 2 in every arm: holds.** In each arm, 9 rows draw `builtin_shape` at turn
  1, all Haiku; 8 have `green_turn` 2 in `strict`, `brief` and `control` alike; the ninth is
  `6f236494` Haiku s2 in all three (`compile_error`, then `compiled_wrong`, `green_turn` 0). **Each
  arm's turn 2 is its own generation**: `131c4232` Haiku s1's turn 2 reads `input`/`output` 13714 /
  1615 in strict, 12941 / 1684 in brief, 12897 / 2833 in control. So the control arm's 8 of 9 were
  reached on a message holding no `wrong_label`, which is what makes the compiler-engineer's
  reading (*the misleading `wrong_label` cost nothing measurable*) a measurement and not an
  inference. Unsaid by any seat: the tenth receiverless sample (§ 1) met the `slice` message at
  turn 2 and needed turn 3 in every arm. At n = 9 (10) per arm and a ceiling of 8 of 9, no
  difference between messages could have been seen.
- **12 of 13 wrote the conversion at turn 2: holds** (strict; brief 12 of 13, control 13 of 13),
  each turn-2 text found by key prefix `<key>.t2.` plus equal `input` and `output` (one match
  each). The thirteenth, `c7a30e5b` Sonnet s1, holds no `repeat(` at turn 2: it replaced the call
  with a loop (`pad @= ""`). **This corrects the spec-warden's sentence** *at turn 2 all five
  padding repairs wrote `to_u64(N - s.len()).must()`*: four of five did (Opus s2 and s3, Sonnet s2
  and s3, e.g. `pad = to_u64(18 - s.len()).must()`); the fifth dropped `repeat`.

## 3. E2 and E4: built, gated, and the tree they stand on

All read or run in `.claude/worktrees/227-compiler-engineer` without building or editing:

- `git status --porcelain` there matches its § 10 (four modules modified, `builtin_args.hero` new,
  the four `fixedbugs-439`/`458` cases modified, three new cases; `spec/heroes-spec.md` not listed,
  so the trunk's).
- `./heroes-<v> --version` reads `heroes-next 79fe404274caacd0`, `heroes-a 501cc868166f815e`,
  `heroes-e 5666d2623231e108`, `heroes-e2 6758e04670a04ec3`, `heroes-e3 2bb9081d20f47e88`,
  `heroes-e4 2cd2fb7a6cf485a6`, as reported. **The tree is E4**: the provenance mirror
  (`python3 -I -B`, `provenance.tree_sources(<worktree>)` from the lane's `.claude/hooks`) reads
  `2cd2fb7a6cf485a6` for the worktree and `79fe404274caacd0` for the lane.
- **The fixpoint holds**: `build/227fx/s2` and `s3` both say `sources 2cd2fb7a6cf485a6`, and `cmp
  build/227fx/s2.c build/227fx/s3.c` prints nothing (26,759,615 bytes each). `s2` and `heroes-e4`
  differ as binaries (`cmp`: byte 18241390), built by different compilers from one key; the
  report claims nothing about that, and I did not look further.
- **Every stored count matches the report** (`grep 'passed\|failed'` on `build/227out/*.txt`):
  E2 `check` 704/0, `full` 41/0, `permissive` 22/0, `fixes` 1004/0, `annotations` 1196/0, 1740
  own tests; E4 the same with `full` 42/0 and `annotations` 1197/0; `layout` on E2's three files
  4/0 each; route A's sentence `spec` 27/4 (the four pins), `special` 10/0, `grammar` 9/0, `fixes`
  1004/0, `unseen` 3/0; E3's `full` 40/1 as reported.
- **Every shape in its § 3 table re-runs as written** on `heroes-next`, `heroes-a`, `heroes-e2`
  and `heroes-e4`, eight shapes piped to `check --brief /dev/stdin`, and the full form shows the
  two notes it quotes.

**What was not run, and the change owes** (the map in `.claude/rules/verification.md`):

- **`layout` on E4's two extra files**: no `layout-e4-*` output exists (`ls build/227out/`), and
  the report's table cell for E4 reads *(E2's files)*. `check/mismatch.hero` 44 → 56 and
  `check/width_class.hero` are unjudged against their `DECIDED` rows.
- **`spec` on the E or E4 tree without route A's sentence**: the `selfhost/inventory.hero` row owes
  `spec` (*its built-ins' and escapes' tables*), and E adds a `match` of 27 names to that file. The
  one `spec` run is `specA-spec.txt`, route A's sentence judged by `heroes-e`, 27/4, the four
  failures the pins. The combination of route A's sentence with E4 never ran together.
- **`canonical`, `order`, `probe`, `records`, `seed`** (the `selfhost/**` row): no output. And the
  rule *a change to what the checker refuses is judged by every golden tree*: E's report says it
  changes which message, never whether, and left the census as an inference.
- **The census, run here**: every tracked `.hero` file of the lane (`git ls-files '*.hero'`, 4,098)
  through `./heroes-next check --brief` and `heroes-e4 check --brief`, six at a time (38.9 s wall,
  202 s user): **exit codes identical, 1,904 at 0 and 2,194 at 1 under both**; the only four files
  whose codes differ are the four goldens E moves (`fixedbugs-439-…`, three `fixedbugs-458-…`),
  `builtin_shape` → `wrong_arity`, the same count of each. So E4 refuses nothing today's compiler
  accepts on the tracked tree, which turns the compiler-engineer's inference into a measurement for
  this tree. (`check` was first shown to write nothing: `find` for files newer than this report in
  both trees printed nothing after a `check` run. `--brief` drops notes, so the census does not
  compare E4's width note.)

## 4. The three filings the compiler-engineer proposes: reproduced with the lane's compiler

All with `./heroes-next` in the lane (`79fe404274caacd0`), piped from stdin.

**(a) `needs_label`'s `certain` fix writes a label the call already holds: real.**
`cut(from: 0, to: 5, line)` on `function cut(s: str, from: i64, to: i64) -> str`, and
`slice(from: 0, to: 5, line)`: `check --json` gives `needs_label` a fix `"certainty": "certain"`,
`"replacement": "to: "`, an insertion at the third argument. Applying only the `certain` fixes in
memory, as `--apply` would (byte spans from the JSON, highest first), gives `cut(from: 0, to: 5,
to: line)` and `slice(from: 0, to: 5, to: line)`; piped back, `check --brief` refuses each three
or four ways and **no message says `to` is written twice**. I did not run `--apply` itself (it would
write a file); the in-memory application is my route to its result.
*The class*: the compiler-engineer files it `blocking` through
`.claude/rules/diagnostics-and-goldens.md`'s *a fix that leaves the defect standing is a `guess`*.
**That reading is weak by its own words**: after the fix, `needs_label` is gone, so the defect the
diagnostic names does not stand. The stronger ground, which the report cites in passing, is
**defect 135** (`gh issue view 1415`: *every certain fix the audit found with two readings now writes
the one meant, or is a guess*, CLOSED): the third argument here has two readings (`to`, or the
missing `s` written last), so this is a `certain` fix with two readings that 135's audit did not
reach. Whether a closed audit's missed case is `blocking` (*a false message*: the `certain` tag
claims one reading) or `adjacent` is the coordinator's; the facts above are what it turns on.

**(b) A label on a built-in's unlabelled position is accepted: real, and wider than reported.**
`len(foo: line)`, `repeat(s: "ab", n: 3)`, `join(xs: ["a"], sep: ",")`, `slice(xs: line, from: 0,
to: 5)`, `to_str(x: 3)`, `chars(of: line)` all check at exit 0 (`f(b: 1)` on `function f(a: i64)`
draws `wrong_label`). **Shapes the report did not probe, each exit 0**:

- a label **written twice**: `slice(to: line, from: 0, to: 5)` and `slice(from: line, from: 0, to:
  5)`;
- labels naming **the built-in's other parameter, swapped**: `repeat(n: "ab", s: 3)`, `join(sep:
  ["a"], xs: ",")`, `push(x: [1], xs: 2)`.

A declared function refuses each: `cut(to: line, from: 0, to: 5)` and `cut(from: line, …)` draw
`wrong_label` (*it is `s`*); `jn(sep: ["a"], xs: ",")` on `function jn(xs: [str], sep: str)` draws
two. *The class*: the compiler-engineer files it `adjacent` because *no accepted program computes
another value*, having searched for one and found none; I found none either (the types force the
positional reading: an inference, since running a program would write a build and I wrote nothing).
But `verification.md`'s `blocking` list holds *a wrong one accepted*, and `slice(to: line, from: 0,
to: 5)` is a program no reading makes right, accepted, while the same shape on a declared function is
refused. The swapped shapes are programs whose labels state the opposite of what they compute. So
the class turns on whether *a wrong one accepted* covers a wrong program that computes what its
positions say; the report does not raise that reading, and the sitting should.

**(c) A mistake inside an argument is hidden behind a wrong count: real, `adjacent` by the list's
own words** (*a mistake told only after the first is fixed*). `cut(from: "a" + 1, to: 5)` draws
`wrong_arity` alone, and so does the library built-in `range(to: "a" + 1)`; `cut(line, from: "a"
+ 1, to: 5)` (right count) draws `mixed_arithmetic`. `cut(from: undefined_name, to: 5)` draws
`unknown_name`, which comes from an earlier stage. **Its consequence for route E**: `heroes-e4` tells
`slice(from: "a" + 1, to: 5)` and `line.slice(from: "a" + 1)` both ways (`wrong_arity` +
`mixed_arithmetic`) and `cut(from: "a" + 1, to: 5)` still one way. **So E, landed alone, makes a
Tier-1 built-in tell more than a declared function or a library built-in on the same mistake**, the
reverse of the inconsistency §1.7 is cited to remove. The compiler-engineer says three lines in
`user_call` would repair it; nobody proposes landing them with E.

## 5. The combination: A's sentence + E's rule + the width note, and what bears on it

**Disclosed first**: route E was named in my first pass (G5), so my reading of it is not
independent; every fact below carries its command so it can be checked without me.

**No seat's verdict contradicts the combination.** By the reports' own words: the compiler-engineer
approves E (built as E2), objects to A's *message half* and has *no objection from this seat to the
sentence*, and calls the width note *separable*, gated in E4; the spec-warden approves A's sentence
and has *no price objection* to E (0 spec tokens); the historian advises *A's content, best carried
by E's general path*, with Rust's `help:` naming the conversion as precedent for the note; the blind
result approves the `repeat` half and has no reading on `slice`. The census (§ 3) adds the
measurement E lacked: on the tracked tree E4 refuses nothing today's compiler accepts.

**What the combination still owes, or what would change it, from the commands above:**

1. **It has never run as one tree.** Route A's sentence was judged by `heroes-e` (E1's tree), not
   by E2 or E4; `spec` never ran on E's `inventory.hero` change without the sentence; `layout` never
   judged E4's `check/mismatch.hero` (44 → 56) or `check/width_class.hero`; `canonical`, `order`,
   `probe`, `records`, `seed` have no output for any variant (§ 3). The spec-warden's approval is
   conditional on *the compiler-engineer runs `spec`, `special`, `grammar`, `fixes` and `unseen`
   green on that wording*; that ran once, on another compiler, with 4 pins owed.
2. **E alone turns one inconsistency into its mirror** (§ 4c): after E4 a Tier-1 built-in tells a
   mistake inside its arguments behind a wrong count, a declared function and a library built-in
   (`range`) do not. E's §1.7 argument (*a wrong count is told one way whatever the callee*) holds
   for the count's message and fails for what comes with it. **The most complete combination lands
   filing (c)'s three lines in `user_call` with E**, or says why not.
3. **Filing (b) and route A's sentence must be decided together, and nobody joined them.** The
   sentence keeps `join(xs, sep)` and `repeat(s, n)`, names that read as parameters; blind sessions 2
   and 6 each list naming them (`repeat(s: …, n: …)`) as the *other choice* with *the same* result.
   The repair the compiler-engineer sketches for (b) refuses a label at a position the built-in does
   not name, so `repeat(s: "ab", n: 3)` and `join(xs: ["a"], sep: ",")`, accepted today, would be
   refused, and the sentence would then show a reader names it may not write. The other repair names
   those positions in the inventory, so the labels are checked and the sentence states true
   parameter names. Measured: **0 of the 90 turn-1 answers, 0 of the 96 later turns and 0 of the
   12 blind programs** label a built-in's unlabelled position (regex over each first fenced block,
   comments stripped), so in this data neither repair refuses anything a model wrote. The choice is
   still a ruling, and route A's wording depends on it.
4. **E's harness prediction is near-vacuous once A's sentence lands.** E's prediction (2) scores
   *samples whose turn 1 holds a `slice` short of its value*; the spec-warden's P2 predicts those fall
   to at most 10% of Haiku's `slice` answers. If P2 holds, E's denominator is a handful and *differ
   by at most one sample* is near-certain whatever E does; if P2 fails, A's sentence is the thing
   that failed. **The instrument that can see a note is strict minus brief**, not strict minus
   control: `thesis_grade.hero:114-115` gives the `brief` arm `check --brief`, which drops notes
   (E's call shape and E4's conversion both live in notes), and `thesis_report.hero:375` already
   reports *The message's value: green@3, strict minus brief*, exploratory. Nobody named it.
5. **E's note in the `brief` arm**: `--brief` prints `` `slice` takes 3 argument(s), found 2 `` and
   nothing else (§ 3's runs). Today's brief form says `does not take 2 argument(s) of those types`
   plus `has nothing called \`to\` at this position — it is \`from\``. Whether *takes 3, found 2*
   with no shape is better or worse for a reader who believes `slice` takes `from:` and `to:` is
   unmeasured; 050's brief arm, with today's text, went 8 of 9 green at turn 2 (§ 2).
6. **The message half of the `to_i64` fault is open, at 0 spec tokens, and unlisted.** Today an
   `i64?` in an annotated binding draws `type_mismatch` with `fix (guess): \`.must()\``; the same
   `i64?` as an operand (`to_i64(c) - 48`, the trial's three Haiku refusals) draws `bad_operand …
   found \`i64?\`` with **no fix and no note**, on `heroes-next` and on `heroes-e4` alike (piped
   probe). It is the shape beside E4's width note (that note covers converting; this one covers using
   the conversion's `T?`). All three samples went green at turn 2 in every arm (`verdicts.jsonl`),
   so its measured cost is one tier, three samples; what would make it right is §4.17's promise read
   per message, as the compiler-engineer reads it for the width note.
7. **The historian's third condition is answered and nobody connected it.** It asks whether a
   built-in accepting several types (Go's `len`) makes E need one shape per type. E counts
   arguments only and writes a note for an unlabelled first position (`slice` alone today); each
   arm of `check/builtins.hero` asks exactly one count (`grep -n 'given.len()'`: 15 arms, one count
   each), which also answers the compiler-engineer's own condition (*a built-in whose type rule
   accepts two counts*) for those arms.
8. **What the combination cannot move in this data**: green@3 strict against control is
   **concordant on all 90 pairs** of 050 (inline tally of `verdicts.jsonl`; 0 discordant among the
   13 `repeat` samples, the 9 `slice` samples and the whole set). Neither A nor E moves Part 11's
   primary on 050's evidence; the sentence moves turn-1 refusals in both arms alike
   (`type_mismatch` and `builtin_shape` are not thesis rules: `sed -n 190,230p selfhost/diag.hero`
   lists 27 codes and neither). The case for the combination is correctness of the specification's
   text and §4.17 per message, not the thesis metric, and the synthesis should say so.

## 6. The spec-warden's prediction, and *route A lands before the freeze*

- **The freeze binds the compiler as well as the spec, and the report says it of the spec alone.**
  The run's provenance holds `sources` (`79fe404274caacd0` in 050's `provenance.json`) beside
  `contexts` (the spec's path and sha), `thesis_run.hero:777` writes both, and the held-out README
  (lines 31-36) has `report` set `prediction.md`'s preregistered block against each run's
  provenance *field by field, every difference a row of its deviations*. So E, the width note and
  any repair of the three filings change the digest the freeze pins, exactly as A's sentence does.
  **The condition is the same for every part of the combination**: before the freeze, or a recorded
  deviation. (*Breaks the pre-registration*, the spec-warden's word, is stronger than the README's
  *a row of its deviations*; the README is the text.)
- **The freeze is a step, not a date**: the milestone page's run order (`sed -n 158,164p
  docs/roadmap/milestones/M-thesis-harness.md`) puts it after the canary probe and the `none`
  context and before the first held-out run, and `ls tests/thesis/held-out/` holds `README.md`
  alone. The condition is reachable; nothing in the reports says how far away it is.
- **P1's *neither biases the rate* contradicts P1's own point estimate.** The report predicts the
  padding class (`N - s.len()`) refused above the parsed-count class and makes that difference P1's
  trigger; then the pooled rate depends on the held-out mix of the two classes, which the author
  chooses after reading this sitting. And the held-out protocol writes **traps on purpose**:
  `traps.txt` lists the tasks *that tempt a plausible mistake on purpose, since the results are
  reported stratified* (README line 27-28; `docs/metrics/tasks.md` lines 76-98). A `repeat`-count
  trap written by an author who has read this sitting is the likeliest trap there is. **P1 and P2
  say nothing of the trap stratum**: scored pooled, they score the author's task design as much as
  the sentence. The repair in the prediction's own terms: score on the non-trap stratum, or per
  count class, written into row 115 before the freeze.
- **The bar is applied to one half and not the other.** The spec-warden declines the `to_i64` and
  `len` entries for missing the milestone page's bar (*repeats across two model families or tiers
  and on held-out tasks*, line 70), and approves A's `slice` half, which has the same profile: 10 of
  26 Haiku answers, 0 of 58 Sonnet and Opus (§ 1). The report's other argument distinguishes them
  (*`slice(from:, to:)` alone is a call the compiler refuses when copied*: a false shape in the
  specification, not a reader's slip), and that is checkable and holds. But the report does not
  say that this is why the bar does not apply to `slice`, and the synthesis should, or apply it.
- **Its `to_i64` row prices a fault no answer committed** (§ 1): *…, and none takes a `str`*, +7.
  The decision *do not buy it now* may stand on the counts (3 of 19, one tier, all green at turn 2),
  but the remedy priced is the wrong one, and the right one is unpriced.
- **Its `len` row's ground is incomplete**: *Principle 0's burden is unmet with no measured
  failure* holds for refusals, but **4 of the 6 blind readers name `len`'s result type as unstated**
  (sessions 1, 2, 3, 5; all three under route A), each guessing `i64` right (`awk` over each
  `## choice_points`). Under route A the type of `len` becomes load-bearing for every padding count
  (`to_u64(N - s.len())`); session 2 writes so in its choice 3. Not a failure; a measured
  uncertainty P1's trigger may need.
- **P3, P4**: P3 is unpaid by the report's own reading; P4 is a paid run, the author's to ask for.
  Neither is contradicted.
- **Its vendored and stored figures**: I re-ran neither the vendored counts (my first pass ran
  them: 7708 → 7718, 7846 → 7856) nor the API (no call). The arithmetic 10,292 − 6 = 10,286 and
  10,305 − 6 = 10,299 is the spec-warden's on stored answers and holds as arithmetic.

## 7. Conduct

- **The spec-warden's three scripts**: its report says it wrote `calls.py`, `calls2.py`, `verd.py`
  outside the root at 12:41 and removed them at 12:42. `find /private/tmp/claude-501 -name
  'calls.py' -o -name 'calls2.py' -o -name 'verd.py'` prints nothing, and this session's
  scratchpad (`…/7e9f826e-…/scratchpad`) is empty, last modified 12:42. A breach of the hard stop,
  self-reported and undone.
- **The historian's two PDFs are not where it says.** `find
  /Users/joseph/.claude/projects/-Users-joseph-Temp-heroes-heroes-lang -name 'webfetch-1792579*'`
  prints nothing. What the sitting left outside the root under this session's `tool-results/`
  (`find … -newer docs/panel/227-briefs/00-shared.md`) is **seven page images** in three folders:
  `pdf-e303742e-…/page-13.jpg` to `page-17.jpg` (12:40), `pdf-f670f79a-…/page-01.jpg` (12:41),
  `pdf-fd648373-…/page-1.jpg` (12:50), the times of the historian's two PDF reads (Oberon-07 pages 1
  and 13, P1428R0 page 1). The report's account of its residue is wrong in its paths; the residue
  is real.
- **This pass also caused a write outside the root**: reading `compiler-engineer.md` with `cat`, the
  output (38.3 KB) was persisted by the harness to `tool-results/bwufrotk8.txt` (13:33, 39,269
  bytes). The harness did it, not a command of mine, as with the historian's images; I have no tool
  I may use to remove it and report it here. `tool-results/bkvun34oy.txt` (12:40) is another such
  file from the sitting, not mine.
- **The blind folders sit inside the root, against the panel skill's letter**:
  `.claude/skills/panel/SKILL.md` lines 133-153 put the copy *outside the repository*, in the
  scratchpad, *outside any git tree, with no `CLAUDE.md` in or above it*; the hard stop of
  2026-10-08 forbids writing outside the root. The coordinator chose `build/w/N` with
  `GIT_CEILING_DIRECTORIES`, `--bare` and a one-sentence system prompt, and every session's
  `context` names nothing of the project. That is hard stops over process, the right way by CLAUDE.md
  § Precedence, but **the README does not say that a rule gave way, or which**, and § Precedence
  asks for it to be written down. The skill's paragraph is stale beside the hard stop.
- **The llm-ergonomist's verdict was written by the coordinator**, who proposed route A, as the
  README declares. The numbers are mechanical and re-ran here (§ 1); the description of session
  3's refusals (*a function named `all`*) came from the brief's framing, not from the program, which
  is the one place the authorship showed.
- **The compiler-engineer** read the lane's run folder (`verdicts.jsonl`, `turns/`), outside its
  worktree; its brief forbids using the lane's compiler and writing in the lane, not reading it.
  Nothing written in the lane: `git status --porcelain` there prints the sitting's two folders
  alone.

## 8. Questions the sitting should have asked and did not

1. **Are `s`, `n`, `xs`, `sep` the built-ins' parameter names?** Route A's sentence and the repair of
   filing (b) answer it in opposite directions unless decided together (§ 5.3).
2. **Does the freeze condition bind the compiler half?** It does, by the provenance (§ 6); no seat
   says so.
3. **Which instrument sees a note?** Strict minus brief, an exploratory contrast the report already
   prints (§ 5.4).
4. **Are P1 and P2 scored on the trap stratum?** The author writes the held-out traps after reading
   this sitting (§ 6).
5. **Does E land without the `user_call` repair?** If so, it leaves a new asymmetry (§ 4c).
6. **What class is a `certain` fix with two readings that defect 135's audit missed, and is a wrong
   program a built-in accepts (`slice(to: line, from: 0, to: 5)`) *a wrong one accepted*?** (§ 4a,
   § 4b). Both classes turn on readings the reports leave to the coordinator.
7. **Does E change the public vocabulary?** `builtin_shape` → `wrong_arity` on every count mistake
   moves a code `check --json` publishes (design.md §4.17's schema 2); the compiler-engineer's own
   answer treats a new code as a new class. A code's territory moving is the same kind of change,
   and the reports do not say whether it is.

## 9. The gaps, in one list

| # | gap | kind | settled here by |
|---|---|---|---|
| 1 | the spec-warden's `to_i64` correction is false; its +7 prices a fault nobody committed | claim falsified | `check` on the three answers; `to_i64("a")` probe |
| 2 | 10 vs 9 receiverless `slice`: both right, the tenth met the message at turn 2 and needed turn 3 in every arm | reconciliation | regex + `check --brief` + `verdicts.jsonl` |
| 3 | `all`/`ok` are bindings; the blind result repeats *a function named `all`* | claim corrected | `check` source lines; renamed probe |
| 4 | *all five padding repairs wrote `to_u64`*: four did | claim corrected | `turns/*.json` |
| 5 | E4's two extra files never judged by `layout`; `spec` never on E's tree; A's sentence + E4 never run together; `canonical` `order` `probe` `records` `seed` unrun | unrun | `ls build/227out/` |
| 6 | the census E left as an inference: run, E4 refuses nothing today's accepts on 4,098 files | inference measured | census in § 3 |
| 7 | filing (b) is wider: a label written twice and swapped labels accepted; class may be `blocking` (*a wrong one accepted*) | shapes beside | piped probes in § 4b |
| 8 | filing (a)'s `blocking` ground is weak as cited; defect 135's rule is the stronger one | reading | `--json` + in-memory apply; `gh issue view 1415` |
| 9 | E alone reverses the nested-error asymmetry; the `user_call` lines are not proposed with it | route not listed | piped probes in § 4c |
| 10 | route A's sentence and filing (b)'s repair must be decided together | contradiction not joined | § 5.3, 0 of 198 programs |
| 11 | E's prediction (2) is near-vacuous under A; strict minus brief is the instrument for notes | instrument not named | `thesis_grade.hero:114`, `thesis_report.hero:375` |
| 12 | the `bad_operand … found T?` message has no `.must()` hint, unlike `type_mismatch` | route not listed | piped probe on both compilers |
| 13 | the freeze pins the compiler's digest too | condition widened | `provenance.json`, `thesis_run.hero:777`, held-out README |
| 14 | P1/P2 silent on the trap stratum; P1's *neither biases the rate* contradicts its own point estimate | prediction design | held-out README 27-28, P1's text |
| 15 | the bar applied to `to_i64`/`len` and not to `slice`, without the stated reason | asymmetry | § 1 counts, milestone page line 70 |
| 16 | 4 of 6 blind readers name `len`'s type as unstated | data unused | `awk` over the choice points |
| 17 | no route moves 050's primary: 0 discordant pairs of 90 | framing | `verdicts.jsonl` tally |
| 18 | historian's residue misdescribed; this pass left one harness file outside the root; blind folders' rule conflict not written down | conduct | `find` in § 7 |
