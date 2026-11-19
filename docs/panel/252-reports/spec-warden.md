# Panel 252, the spec-warden's report

## Registered predictions

Written 2026-11-19 from 16:08 CET (`date`), before any build, refresh or measurement of this seat, having
read only `00-shared.md`, `spec-warden.md`, `llm-ergonomist.md` (§ What it measures, § Its layout, § The
grading, § The decision rules) and design.md §1.6 at the frozen commit. Each line names the counted rule of
`llm-ergonomist.md` § The decision rules that scores it, as worded there, my forecast, and the arm it would
pay for. A forecast that the rule does NOT fire is a prediction too, and it is scored the same way.

- **W1 (task 1, scores Q1-R's 0 tokens, pays for no arm)**: at least 3 of `now`'s 5 answers have the
  private module use the face (`area.hero` uses `shapes`, the ring), and **"The refusal meets readers"
  FIRES**: `now`'s right answers under the probe exceed its right answers under the frozen stage 2 by at
  least 3.
- **W2 (task 1, would pay for Q1-C, +26 vendored)**: **"Q1-C's sentence carries" does NOT fire**: `q1-c`'s
  right answers under the frozen stage 2 exceed `now`'s under the frozen stage 2 by at most 2.
- **W3 (task 2, scores Q2-R's 0 tokens)**: **"The spec reads as Q2-R" FIRES**: at least 4 of `now`'s 5
  predict both line 6 and line 7 accepted.
- **W4 (task 2, would pay for Q2-C, +14 vendored)**: **"Q2-C's sentence carries" FIRES**: at least 4 of
  `q2-c`'s 5 are right by its row (line 7 refused, line 6 not). Registered as an observation of
  readability: I will argue it does not pay for an order-dependent rule (§ Q2 below).
- **W5 (task 3, would pay for Q3-R, +63 vendored)**: **"Q3-R serves the thesis" does NOT fire**: `q3-r`'s
  right answers by the rubric exceed `now`'s by the rubric by at most 2. Recorded beside it: at least 3 of
  `now`'s 5 answers hold an API that accepts a mismatched pair (*"readers write the hazard `containers`
  wrote"*).
- **W6 (task 4, scores the trap)**: **"The trap meets readers" FIRES**: at least 2 of `now`'s 5 are silent
  wrong.
- **W7 (task 4, would pay for Q4-R, +56 vendored)**: **"Q4-R serves the thesis" does NOT fire**: `q4-r`'s
  right answers exceed `now`'s by at most 2 (the refusal's value is the compiler's, counted *by
  construction* as silent turned loud, not the sentence's).
- **W8 (task 5, scores the refusal)**: **"The refusal meets readers" FIRES**: at least 3 of `now`'s 5 write
  an exponent literal (my point forecast: 4 or 5).
- **W9 (task 5, would pay for Q5-R, +41 vendored)**: **"Q5-R is read as needed" FIRES**: `now`'s right
  answers once regraded exceed its right answers under the frozen stage 2 by at least 3.
- **W10 (task 5, would pay for Q5-C, +35 vendored)**: **"Q5-C's sentence carries" FIRES**: `q5-c`'s right
  answers exceed `now`'s by at least 3.
- **W11 (instrument: `heroes measure --refresh`, scored in this sitting)**: the frozen spec refreshes to
  `real 11384` exactly, the pin of 2026-11-15, the document being byte-identical since.
- **W12 (same instrument)**: the `proposal` arm's real delta lies between +180 and +250; no arm's real total
  exceeds 11,700, so no arm and not the proposal reaches design.md §1.6's 16384 (no budget veto).

## Work log

Every command below ran in my folder,
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/252-spec-warden/` (called `sw/`),
a `git archive` of `181555e9a32d0b18878f9f10e48d2eadd939c285`, with `TMPDIR=sw/tmp`.

- **16:08** the copy: `find sw -type f -not -path 'sw/tmp/*' | wc -l` **20,037**; `spec/heroes-spec.md` 492
  lines, 29,407 bytes, sha256 `1d532d514831aa70...`, the brief's. `pmset -g assertions`:
  `PreventUserIdleSystemSleep 1`, Amphetamine's *Single-Use - System* held 217 h 28 min.
- **16:08** seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o sw-heroes`, exit 0, `sources
  5aae00294f7cd6fa`. Stage 2: `./sw-heroes build selfhost/main.hero -o sw-heroes-next`, exit 0,
  `./sw-heroes-next --version`: `heroes 0.6.0`, **`sources 23c7d2a626b29fd5`**, the writer's: I am on the
  frozen commit. No clock was taken (five agents share the Mac).
- **16:10** the arms, copied from `bw/arms/` into `sw/arms/`: the thirteen sha256 prefixes and line counts
  equal `00-shared.md`'s table, every one; `spec-now.md` is `cmp`-equal to the frozen spec; `proposal` is
  `cmp`-equal to `spec-now.md` with `q3-r.diff`, `q4-r.diff` and `q5-r.diff` applied by `patch`.
- **16:10** the vendored counts, `./sw-heroes-next measure arms/spec-<arm>.md` from `sw/`: all thirteen
  rows equal `bw/measure-arms.txt` (`now` 8551 / 8702, maximum 8702, through `proposal` 8706 / 8862).

### Refresh log (the binding instrument, design.md §1.6)

The key: `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a` in the one shell that refreshed,
`echo ${#ANTHROPIC_API_KEY}` printed **108**, its length alone. Script `sw/sw-tools/refresh_arms.sh`: each
arm copied over `sw/spec/heroes-spec.md`, `./sw-heroes-next measure spec/heroes-spec.md --refresh`, the
frozen spec copied back and `cmp`-checked before the next arm (it stops on a failed restore or a non-zero
exit). **Thirteen refreshes, the brief's limit, one per arm, each exit 0 and each restored**; log
`sw/tmp/refresh-log.txt`. Nothing was pasted into any tree. Every row printed `SPEC_REAL_MODEL
"claude-opus-5"` and `SPEC_REAL_TAKEN "2026-11-19"`.

| time (CET) | arm | sha256, first 16 | real | `SPEC_DIGEST` | **real delta** | vendored delta | real / vendored |
|---|---|---|---|---|---|---|---|
| 16:11:47-48 | `now` | `1d532d514831aa70` | **11384** | `583fa94c01eddbf8` | 0 | 0 | |
| 16:11:48-50 | `q1-c` | `8448edd80708be52` | 11417 | `376cf29042b69da0` | **+33** | +26 | 1.27 |
| 16:11:50-51 | `q1-a` | `f82f959ec30b2479` | 11455 | `7ce9031e4061619e` | **+71** | +54 | 1.31 |
| 16:11:51-52 | `q2-c` | `4313f5f3bd0efd53` | 11405 | `54d6b6052b2a64a6` | **+21** | +14 | 1.50 |
| 16:11:52-54 | `q3-r` | `dfad79aa38933f85` | 11470 | `466fe26e1c6f5505` | **+86** | +63 | 1.37 |
| 16:11:54-55 | `q4-r` | `1fddf579d07fd044` | 11446 | `42d39c6c6f441957` | **+62** | +56 | 1.11 |
| 16:11:55-56 | `q4-r2` | `13337dd1fed04b0b` | 11444 | `731078431d65e41c` | **+60** | +52 | 1.15 |
| 16:11:56-58 | `q4-c` | `aad99aafd0d1a2ca` | 11403 | `722525e234db97fa` | **+19** | +17 | 1.12 |
| 16:11:58-59 | `q5-r` | `7beb56374de59fc2` | 11433 | `7f13a8290bfe5f9c` | **+49** | +41 | 1.20 |
| 16:11:59-12:01 | `q5-c` | `7dab16286fad68ad` | 11425 | `28aafde8034b3670` | **+41** | +35 | 1.17 |
| 16:12:01-02 | `q5-p` | `d2e79efdb7999586` | 11384 | `421b9016708612f3` | **0** | +1 | |
| 16:12:02-04 | `q5-u` | `2027d86f05837b83` | 11400 | `2e7f4283463e8eac` | **+16** | +14 | 1.14 |
| 16:12:04-05 | `proposal` | `c7885b36d17a3bd0` | 11581 | `6dbcd1496d148a3f` | **+197** | +160 | 1.23 |

**The budget, settled**: the largest arm, `proposal`, reads 11,581 real, plus the FFI floor's 60 (panel 030
R3, as `measure` prints it) is 11,641 against design.md §1.6's **16384** (grepped today: `docs/design.md:255-256`
at the frozen commit, *"must fit in 16384 tokens, measured by `claude-opus-5` through `POST
/v1/messages/count_tokens`"*). Every arm of the sitting adopted at once, all eleven deltas summed (+458
real), would read 11,842, and 11,902 with the floor: **no single arm comes within 4,700 of the ceiling, and all of
them together stay 4,482 below it. There is no budget veto in this sitting**; every verdict below stands on Principle 0 and the payment rule.
`proposal`'s +197 is exactly `q3-r` + `q4-r` + `q5-r` (86 + 62 + 49): the three edits do not interact
in the tokeniser.

**W11 and W12 scored**: W11 (`now` refreshes to 11384) **held**; panel 251's spec-warden had read the same
11384 at 10:20 this morning (`docs/panel/251-reports/spec-warden.md:45`), which I read at 16:09, after
registering, so W11 carried less information than it looked. W12 (`proposal` between +180 and +250, no arm
above 11,700) **held**: +197, the largest 11,581. **The vendored delta under-priced every arm but `q5-p`**,
by 11% (`q4-r`) to 50% (`q2-c`); the brief's arms were correctly called lower bounds.

### Probes run (my stage 2, `sources 23c7d2a626b29fd5`; folders under `sw/probes/`)

- **`q1-sibling/`, Q1-C's wording**: `geom.hero` (a `record Point`, uses nothing), `geom/internal/calc.hero`
  (`use geom`, `square_sum(p: geom.Point)`), `geom/shapes.hero` (uses `geom` and `geom/internal/calc`),
  `main.hero` (uses `geom` and `geom/shapes`). `check` exit 0, `build` exit 0, the binary under `timeout -s
  KILL 20` prints `25`, `0`, exit 0. **A module under `internal` uses the module that names it, and the
  compiler takes it**: the refusal fires on a ring, not on that `use`.
- **`q2-plain/`, Q2's shape with no type parameter**, `check` each: `x = [["a"], []]` exit 0; `x = [[],
  ["a"]]` **exit 1**; `m = {"b": ["c"], "a": []}` exit 0; `m = {"a": [], "b": ["c"]}` **exit 1**; `x =
  [{"a": []}, {"b": ["c"]}]` exit 1; `m = {"a": []}` exit 1 (rightly: nothing says the value's type).
  Every refusal: *"error[cannot_infer]: the array literal is empty, so there is nothing to infer its type
  from — write the annotation"*, said of a literal that holds `["a"]`.
- **`q45/`, Q4 and Q5**: `a-bytes.hero` built and run: `f"[{s[0]}]"` `[117]`, `print(s[0])` `117`, `c =
  'a'` then `f"[{c}]"` `[97]`, the slice `[u]`, `print(0.000001)` `1e-06`, `print(0.000_001)` `1e-06`.
  A record in a hole: **`error[hole_not_renderable]`** (*"a hole in an `f"…"` literal writes its value as
  `to_str` does, and `to_str` takes an integer, a float, `bool` or `str`"*); a record in `print`:
  **`error[bad_operand]`** (*"`print` takes any integer, a float, `bool` or `str`"*). `to_f64("1e-06")`:
  `error[bad_operand]`, *"`to_f64` takes an integer or a float, found `str`"*. `print(inf)`:
  `error[unknown_name]`. `x / x` and `-1.0 / x` with `x = 0.0` print `nan` and `-inf`. `print(1e-06)`:
  `error[exponent_literal]`.
- **The `spec` and `grammar` suites over the arms** (harness built in `sw/` as `sw-harness`, run `./sw-harness
  ./sw-heroes-next spec` and `... grammar` with each arm copied over the spec and the frozen spec restored
  and `cmp`-checked after each, `sw/sw-tools/suite_arms.sh`, outputs `sw/suites/`): the frozen spec reads
  **`spec` 35 passed 0 failed, `grammar` 12 passed 0 failed**. Every arm of the brief (`q1-a`, `q1-c`,
  `q2-c`, `q3-r`, `q4-r`, `q4-r2`, `q4-c`, `q5-r`, `q5-c`, `q5-p`, `q5-u`) and my `w1-true`,
  `w4-one-home`, `w5-r-strtod-u`, `w5-c-u` read **`spec` 31 passed, 4 failed** and **`grammar` 12 passed, 0 failed**,
  the four reds `spec/budget`, `spec/spendable`, `spec/real` and `spec/ledger`, whose messages are the
  pinned count and digest (*"the spec measures 8765 and the record says 8702"*), which any change to the
  text moves and its commit re-pins. So **`named`, `rejected`, `offered`, `shape`, `inventory` and
  `anchors` accept `Pair<i64, str>`, `record Pair<A, B>`, `2.5E3`, `0.000_001`, `0.000001` and `alias Point
  = types.Point` in a code span, and `grammar` accepts `q3-r`'s and `q1-a`'s productions** (the brief's
  open question, answered by run). The suite prints failures only; that my copy's 35 is the trunk's count is
  unread. `w4-min`, written after this batch, was not run through the suites.

### My own wordings, vendored, **a lower bound in those words** (`sw/sw-tools/my_arms.py`, each edit
asserted to match once, `./sw-heroes-next measure myarms/spec-<arm>.md`; no refresh left to price them)

| arm | what it says | vendored max | delta |
|---|---|---|---|
| `w1-true` | § 1, after the `internal` sentence: *"Modules may not form a cycle, so a module `geom` uses cannot use `geom`."* | 8723 | +21 |
| `w2-brace` | § 9: *"`ok(...)`, `[]`, `{}` and a case name give none"* | 8704 | +2 |
| `w4-min` | § 11: *"`print` writes its values with no separator and exactly one trailing newline, and it and a hole (section 2) take the types this language renders as text: a number but a `u8`, `str` or `bool`."* | 8716 | **+14** |
| `w4-one-home` | `w4-min` and *", so `to_str(s[i])` writes a byte's number and `s.slice(from: i, to: i + 1)` its character."* | 8753 | +51 |
| `w5-strtod` | § 11: *"... `nan`, and C's `strtod` reads it back as the same value"* | 8709 | **+7** |
| `w5-r-strtod` | `q5-r`'s § 2 sentence and `w5-strtod` | 8750 | +48 |
| `w5-r-strtod-u` | § 2: *"A float is digits, a point and digits, or digits with an exponent, or both, `_` standing as in an integer: `2.5`, `1e-06`, `0.000_001`."* and `w5-strtod` | 8758 | +56 |
| `w5-c-u` | § 2: *"A float has digits on both sides of its point and no exponent, `_` standing as in an integer: `2.5`, `0.000_001`."* and `w5-strtod` | 8747 | +45 |

## Q1, the private module and its face

- `verdict`: **Q1-R approve** (0 spec tokens), on the compiler-engineer's census finding nothing that needs
  modules acyclic; **Q1-C object** (its sentence is false, measured); **Q1-A veto** (Principle 0 unmet);
  **Q1-R2 object** (a rule a reader meets, owing a sentence, for a narrowing no measurement asks for).
- `section`: design.md §1.6 (the payment rule, `docs/design.md:323-340`), §4.1 (`:942-943`); CLAUDE.md § 2
  and § 12 (*"A refusal is held to the same standard as a feature"*); panel 035's *"A false sentence ... is
  the worst possible spend"* (`docs/panel/035-*.md:59-62`).
- `spec_token_delta`: Q1-R **0**; `q1-c` 11,384 to 11,417, **+33 real** (+26 vendored); `q1-a` 11,384 to
  11,455, **+71 real** (+54); my true sentence `w1-true` +21 vendored, a lower bound.
- `removal`: Q1-R removes design.md §4.1's clause *"and modules may not form a cycle"*, corrected beneath
  with its date as panel 135 corrected Part 7, and the eight `module_cycle` comments in `selfhost/` the
  brief lists (a comment resting on a lifted refusal is a false sentence); `module_cycle` leaves the
  diagnostics (`grep -rl` finds it in 1 golden file, 4 files of `tests/`, 10 lines of `selfhost/`), legal
  before `1.0.0` (CLAUDE.md § 2; panel 232's R9 promises codes from `1.0.0`; task 2266, the code list's
  instrument, is open and unbuilt, `gh issue view 2266`). For `q1-c` and `q1-a`: **nothing, and that is a
  problem**.
- `needed_for_self_hosting`: no, for every arm.
- `argument`: Q1-R makes the spec's silence true at no cost, and a reader's natural face-and-private-module
  ring then builds (the writer's probe compiler's `25` and `26`, carried from `00-shared.md`, not rebuilt
  here). `q1-c`'s *"so a module under `internal` cannot use the module
  that names it"* is false: `sw/probes/q1-sibling` has `geom/internal/calc` use `geom` and builds, because
  `geom` does not use `calc`. Should the refusal stay, the true sentence is `w1-true`, +21 vendored. Q1-A is
  a re-export panel 031's R4 refused (*"There is no re-export"*), meets none of panel 135's three return
  conditions, which count `selfhost/` and `examples/`, not `packages/rebel/`, covers types and not Rebel's
  twice-written codes or event lists (by the brief's census, read, not run), and no blind cell reads it.
- `prediction`: W1 (the refusal meets readers) and W2 (Q1-C's sentence does not carry), registered 16:08.
- `condition`: the compiler-engineer measuring a compiler fact that needs acyclicity (the cache's order,
  a ring's `extern` headers, `check/sized.hero`'s cycle) turns Q1-R to the refusal with `w1-true`'s
  sentence, paid only if W2's counted rule fires, else by a named removal; Q1-A's veto lifts on a measured
  return condition of panel 135 or a blind cell where `q1-a` beats `now` by at least 3. Q1-R2 would owe a
  sentence beside § 1's `internal` one (*"a cycle may pass only through a module and the modules under its
  own `internal`"*, unpriced), so it costs what Q1-C costs and closes less than Q1-R.

## Q2, a literal among them

- `verdict`: **Q2-R approve** (0 spec tokens), with its repair widened to every container literal; **Q2-C
  veto** (Principle 0's burden, held to a refusal by CLAUDE.md § 12, unmet).
- `section`: CLAUDE.md § 12 (*"Spec beats compiler: the compiler has the bug"*; a refusal held to a
  feature's standard); design.md §4.12 (panel 203: *"a pair's verdict is the same in either order"*,
  `docs/design.md:1989-1990`); §1.6.
- `spec_token_delta`: Q2-R **0**; `q2-c` 11,384 to 11,405, **+21 real** (+14 vendored; the arm with the
  largest real-over-vendored ratio, 1.50); `{}` alone in § 9's list, `w2-brace`, +2 vendored.
- `removal`: none owed by Q2-R; for `q2-c`, nothing, and that is a problem.
- `needed_for_self_hosting`: no.
- `argument`: § 9 lists what gives no type, `ok(...)`, `[]`, a case name, and `{"a": []}` is not among them;
  § 2 makes `"a"` a `str`. The spec says `K` is `str`, so the compiler has the bug. The bug is wider than
  the brief's: with no type parameter anywhere, `x = [[], ["a"]]` and `m = {"a": [], "b": ["c"]}` are
  refused while their reversals build (`sw/probes/q2-plain`), and the message calls a literal holding
  `["a"]` empty. Q2-C would write an order-dependent refusal no program or compiler fact needs, contrary to
  §4.12's order-free verdict, and in § 9 alone, so it would also be false of § 10's literals.
- `prediction`: W3 (*"The spec reads as Q2-R"* fires) and W4 (*"Q2-C's sentence carries"* fires),
  registered 16:08. W4 firing would measure that readers can follow the sentence, not that the rule should
  exist: it pays for nothing here.
- `condition`: a program or a compiler fact that needs the first element to decide a literal's type (none
  found: the frozen compiler already accepts the reversals); Q2-R's order-free repair failing to build.
  **On `{}`**: § 9's list is examples (*"give none"*), and § 10 already says *"an empty one needs an
  annotation: `xs: [i64] = []` · `m: {str: i64} @= {}`"*, so `{}` is stated; I would not spend `w2-brace`'s
  +2 under Q2-R. § 10's sentence is itself stricter than the compiler (`[["a"], []]` builds with an empty
  literal unannotated, its type from § 2's context), a question for the synthesis, not a price.

## Q3, a record with a type parameter

- `verdict`: **Q3-R veto**, lifted if task 3's counted rule fires or the compiler-engineer measures a
  compiler need; **Q3-C approve** at 0 spec tokens, with `containers`' false promise repaired in the package
  at once.
- `section`: CLAUDE.md § 2 (Principle 0) and § Precedence (panel 215, the author 2026-10-16: *"robustness
  decides THAT a measured class of wrong programs closes, Principle 0 HOW, so a refusal lands at once and a
  new form still needs § 2's ticket"*); design.md §4.12 (`:2020`, the ruling); §1.6.
- `spec_token_delta`: `q3-r` 11,384 to 11,470, **+86 real** (+63 vendored), the sitting's largest single
  arm.
- `removal`: nothing, and that is a problem, unless the counted rule pays.
- `needed_for_self_hosting`: no (the compiler compiles itself under *"functions only"*; the engineer's
  answer governs).
- `argument`: Principle 0 binds a sitting that amends §4.12: rank 4, above design.md's rationale.
  `:1972-1973` says `Stack<T>` is comprehensible, a reason the line was drawn, not a ticket; `:2020` is the
  ruling. The `deque` call printing `taken 10` in silence (the writer's run, carried) is a measured wrong program, so by § Precedence
  it closes now, in the package, while the form waits for its ticket. The instruments hold on the arm
  (`spec` 31 and 4, the pins alone; `grammar` 12 and 0); `Nest<[T]>` refused at its declaration costs no
  sentence, the spec stating no instantiation rule today (`grep -c -i -E
  'instantiat|monomorph|polymorphic'`: 0).
- `prediction`: W5, registered 16:08: *"Q3-R serves the thesis"* does not fire (at most 2), and at least 3
  of `now`'s 5 hold an API that accepts a mismatched pair.
- `condition`: `q3-r`'s right answers by the rubric exceed `now`'s by at least 3: the veto lifts and the
  arm's text is approved as written, +86 real, paid by that rule; or a compiler need measured by the
  engineer. The type table's first column (`site/src/lib/tables.ts`) does not move: `q3-r` adds no row.

## Q4, a byte becoming text

- `verdict`: **the refusal of every `u8` in a hole and in `print` approve** (robustness, rank 3); **`q4-r`'s
  text object** (one rule in two homes, and +62 real where +14 vendored keeps the spec true); **Q4-R2
  object**; **Q4-C object**.
- `section`: design.md §1.12 and CLAUDE.md § Precedence; `.claude/rules/spec-shape.md` § Where a rule lives;
  `.claude/rules/verification.md` § Bounded discovery (*"a wrong one accepted"* is `blocking`); §1.6.
- `spec_token_delta`: `q4-r` **+62 real** (+56); `q4-r2` **+60 real** (+52); `q4-c` **+19 real** (+17);
  the sentence I would adopt, `w4-min`, **+14 vendored, a lower bound** (the real/vendored ratio of the
  Q4 arms ran 1.11 to 1.15, so about +16 real: an inference, unrefreshed).
- `removal`: nothing named; `w4-min` is the price the refusal itself compels, since § 11's *"takes ... a
  number"* and § 2's *"as `to_str` does"* become false the day the compiler refuses a `u8`. Its payment is
  the blind seat's registered count *"each of `now`'s silent wrong answers would be a refusal under Q4-R's
  compiler"*, W6's rule: my reading of §1.6, for the synthesis to accept or refuse.
- `needed_for_self_hosting`: no.
- `argument`: `f"[{s[0]}]"` writes `[117]`: a wrong program accepted, so it closes. Q4-R closes the class;
  Q4-R2 keys on a shape (*"indexes a `str`"*: is `s[i] + 1` one? `(s[i])`?) and misses `c = s[0]`, so the
  class stays open. Q4-C describes the trap the thesis says must be an error, and no cell reads it. § 11
  already defines what renders as text, and both positions already refuse a record by an existing class
  (`hole_not_renderable`, `bad_operand`), so one home in § 11 and the two meanings as the diagnostic's
  `guess` fixes: `w4-min`.
- `prediction`: W6 (*"The trap meets readers"* fires) and W7 (*"Q4-R serves the thesis"* does not fire),
  registered 16:08. W7 not firing is the measurement that the fix text in the spec buys nothing the
  diagnostic does not.
- `condition`: W7 firing turns me to `w4-one-home` (+51 vendored), the fix text earning its tokens, still
  in one home. **The shape beside**: `c = 'a'` then `f"[{c}]"` writes `[97]` (run), untouched by any arm; a
  character literal defaulting to `u8` would bring it under the refusal and is a § 2 sentence, unpriced,
  and a question of how many character literals rest on `i64` (the engineer's census). **No new class is
  owed**: widening two existing ones is a choice the engineer may refuse with a reason.

## Q5, an exponent printed and refused

- `verdict`: **name § 11's reader, approve** in every route (`w5-strtod`, +7 vendored); **the float form
  sentence, approve** (§ 2 defines no float); **Q5-R approve if W9's rule fires**, else Q5-C's form;
  **Q5-U approve merged** into the form sentence, **object standalone**; **Q5-P object**.
- `section`: design.md §1.6; §4.9 (`snprintf("%.*g")` until `strtod` reads back the bits,
  `docs/design.md:1752-1755`); panel 035's R6 (`:175-179`) and its *"worst possible spend"*; CLAUDE.md § 2.
- `spec_token_delta`: `q5-r` **+49 real** (+41); `q5-c` **+41 real** (+35); `q5-p` **0 real** (+1); `q5-u`
  **+16 real** (+14); my `w5-r-strtod-u` +56 vendored, `w5-c-u` +45 vendored, lower bounds.
- `removal`: none named; Q5-R paid by W9's rule, Q5-C by W10's, if they fire.
- `needed_for_self_hosting`: no.
- `argument`: *"reads back as the same value"* names no reader, and the language has none: `1e-06` is
  refused, `inf` is no name, `to_f64` refuses a `str`, while `print` writes `1e-06`, `-inf` and `nan` (run).
  So it is false for a reader of the spec, and **Q5-R does not make it true**: `inf` and `nan` stay
  unreadable. Naming `strtod` does, at +7 vendored. § 2 defines no float, though the productions use the
  `float` terminal; panel 035's R6 refused the exponent and ruled nothing about the definition. The
  compiler takes `_` in a float: merged into the form sentence it costs +8 vendored, against +16 real alone.
- `prediction`: W8, W9 and W10 fire, registered 16:08.
- `condition`: W9 not firing: `w5-c-u` (Q5-C's form, `_`, `strtod`); the engineer finding that `1e400` or
  `1e-400` cannot be refused or rounded soundly: Q5-R waits. **Q5-P**: 0 tokens, but it moves every program
  and golden that prints a small or large float, writes over 300 characters at the extremes, leaves
  `inf` and `nan` unread, and no cell reads it; I would not take it.

## The payment, design.md §1.6

- **The 1,500 of the consolidation block does not bind this sitting.** Its text: *"the rows from
  M-buildable-structs to M-compatibility-promise, the consolidation block, spend at most 1,500 real tokens
  before the thesis harness has run"* (`docs/design.md:290-293`). M-compatibility-promise closed
  2026-10-30 (`ROADMAP.md`, row 74, read in my copy; M-rebel-library has no row of its own, its page
  enclosing M-core-packages, row 76, and M-doc-generator, row 80, both past the block's rows 66 to 74) and the harness ran on 2026-10-24 (row 70,
  M-thesis-harness; measurement 052; §1.6 itself, *"Metric 2 first ran on 2026-10-24"*). What binds is
  the payment rule, unconditional (`:323-340`), and the clause beside it: the next raise rests on what the
  harness measures a token to buy.
- **What this sitting would spend**, real: the texts I approve are Q1-R 0, Q2-R 0, Q3-R 0 unless paid (+86
  if W5 is falsified), `w4-min` about +16, and Q5's form with `strtod` and `_`, +56 vendored (about +65 to
  +70 real by the Q5 arms' ratio of 1.14 to 1.20: an inference). Against `proposal`'s +197 real.
- **Every sentence paid by a counted rule is scored before it lands**, the blind seat's run coming before
  the synthesis: a rule that does not fire leaves its text owing a named removal, and I name none, so it
  waits.

## Premises of the briefs found false, each with the command that shows it

1. **`q1-c`'s sentence is false, and the blind seat's `q1-c` cell reads it.** *"so a module under `internal`
   cannot use the module that names it"* (`00-shared.md` § Question 1, `bw/arms/q1-c.diff`): in
   `sw/probes/q1-sibling/`, `geom/internal/calc.hero` writes `use geom`, `./sw-heroes-next check main.hero`
   exits 0 and the built binary prints `25`. The refusal fires on a ring only. The same premise stands in
   `spec-warden.md` Q1, *"a refusal a reader meets at the first private module"*: a reader meets it at the
   first ring. For task 1's own shape (the face uses its private module) the sentence leads a reader to
   the right program, so the cell still reads what it was built to; the text is not adoptable as written.
2. **Q5-R does not make what `print` writes read back in the language** (`00-shared.md`: *"so what `print`
   writes reads back in the language"*): `print` writes `nan` and `-inf` (`sw/probes/q45/g-nan.hero`, run),
   `print(inf)` is `error[unknown_name]`, and `to_f64("1e-06")` is `error[bad_operand]` (`c-tof64-str`). No
   arm makes `inf` or `nan` readable; only naming `strtod` makes § 11's sentence true.
3. **Q2's order-dependence is not a matter of a type parameter's inference alone** (`00-shared.md` §
   Question 2, which places it in § 9 and repairs it as *"a waiting literal's parts"*): `x = [[], ["a"]]`
   and `m = {"a": [], "b": ["c"]}` are refused with no generic anywhere, their reversals accepted
   (`sw/probes/q2-plain/`, `check` each). Q2-R's repair as written would leave them refused, and Q2-C's
   § 9 sentence would be false of § 10's literals.
4. **Q4-R does not owe a new diagnostic class** (`00-shared.md`: *"a new diagnostic class"*, *"It compels
   ... the class and its goldens"*): a record in a hole is `error[hole_not_renderable]` and in `print`
   `error[bad_operand]` today (`sw/probes/q45/b-record-hole`, `e-print-record`). A new class is a choice,
   posed to the compiler-engineer as a question, not a cost the arm compels.
5. **Not false, but under-priced as the briefs say they would be**: every vendored delta read low against the
   real one, by 11% (`q4-r`) to 50% (`q2-c`) (§ Refresh log).

Everything else I leaned on checked out by command: the copy's 20,037 files, the spec's hash, stage 2's
`sources 23c7d2a626b29fd5`, the thirteen arms' hashes and vendored counts, `proposal` as the union of
three arms, design.md's lines (`:255-256`, `:290-293`, `:323-340`, `:942-943`, `:1989-1990`, `:2020`),
panel 035's `:59-62` and `:175-179`, panel 031's R4 and R6, panel 135's return conditions (`:156-165` of its
file), and `git log -S'exponent' 181555e9a -- spec/heroes-spec.md` printing `d06b22669` alone.

## Verdicts at a glance

| question | the text I would adopt, and its home | price | verdict |
|---|---|---|---|
| Q1 | none (Q1-R); design.md §4.1's clause and eight comments go | 0 | Q1-R approve on the engineer's census; Q1-C object (false); Q1-A **veto**; Q1-R2 object |
| Q2 | none (Q2-R, its repair widened to every container literal) | 0 | Q2-R approve; Q2-C **veto** |
| Q3 | none (Q3-C), the package's promise repaired | 0 | Q3-R **veto** unless task 3's rule fires (then +86 real, approve) |
| Q4 | § 11, `w4-min`: *"... and it and a hole (section 2) take the types this language renders as text: a number but a `u8`, `str` or `bool`."* | +14 vendored | refusal approve; `q4-r` text object; Q4-R2, Q4-C object |
| Q5 | § 2, *"A float is digits, a point and digits, or digits with an exponent, or both, `_` standing as in an integer: `2.5`, `1e-06`, `0.000_001`."*; § 11, *"and C's `strtod` reads it back as the same value"* | +56 vendored | approve if W9 fires, else `w5-c-u` (+45 vendored); Q5-P object |

No budget veto: the largest arm, 11,581 real, and every arm together, 11,842 (11,902 with the FFI floor),
stand under 16,384. Report closed 16:21 CET (`date`); nothing in it was pasted into the tree, nothing
committed, no paid run but the thirteen refreshes.
