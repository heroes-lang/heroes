# 095 — More air, and the formatter that is not a fixpoint on its own compiler

Date: 2026-08-26. Convened by author instruction during a repo-wide `heroes fmt`
sweep that was reverted twice in the same session. Full panel, five seats.

**Status: `provisional — author ratification pending`.**

**The sitting's finding outranks its own question, and this is the second time
that has happened** (panel 061 was the first). The proposal is admissible and
measurably cheap. It also **cannot be applied**: `heroes fmt` is already not a
fixpoint on **31 of its own 165 modules**, which means `--in-place` refuses them.
Everything the author asked for is downstream of a defect nobody had measured
before today.

## The proposal, verbatim

Author instruction 2026-08-26, verbatim intent: *"voglio più aria ... voglio
molte più righe vuote per capire meglio come è strutturato un file"*.

1. a blank line before every `for`, `while`, `if`, `match`
2. a blank line after a block closes and code resumes at the outer level
3. a blank line before the `return` that closes a function
4. a blank line BEFORE a comment line inside a body, and NOTHING between the
   comment and the code it documents
5. never two blank lines in a row

**The actual proposition.** The incumbent policy's stated principle is **"blank
lines are content"** (`DESIGN-LOG:46`, repeated at `selfhost/print_fmt.hero:30-32`):
the author's layout survives, runs collapse, the formatter inserts nothing. Rules
1-4 make blank lines **structure** — the formatter inserts them whether the author
wrote them or not. That is a reversal of the principle, not an extension.

**This sitting is the first ruling, not an overturn.** `DESIGN-LOG:46` and `:57`,
the two entries that fixed the blank-line policy, both carry `—` in the panel
column. Verified by grep in this session. The policy was set by the assistant on
2026-08-04 plus one author instruction, and has never been adjudicated.

## Resolution — provisional, and it is an ORDER OF WORK rather than a refusal

Nothing about rules 1-5 was found unsound. Four seats approve with conditions;
the historian objects on precedent and holds no veto. The conservative
resolution is therefore not *no* but *not yet, and not in this order*:

**Stage 1, before any rule and before any sweep.** `heroes fmt` exits 0 on all
165 `selfhost/*.hero`. Today 31 fail. This is the compiler-engineer's blocking
condition and it is the whole sitting's gate.

**Stage 2.** `assert_canonical` — `dump(text) == dump(fmt(text))` — exists in the
selfhost compiler. `selfhost/print_dump.hero:12` claims it does; it does not. Its
first named fixedbug is the record-field remark defect below.

**Stage 3.** Rules **1, 2 (narrowed), 3, 5** land, with rule **4a** (the blank
above a comment) and rule **4b** (nothing between comment and code) **scoped to
statement bodies and excluded from `record`, `variant` and `extern`-group
bodies**. Rule 2 is narrowed to a dedent that closes a `for`/`while`/`match`,
never one that closes an `if`.

**Stage 4.** The sweep, once, over the ruled-on format.

**What a veto would have compelled**: no seat vetoed. The compiler-engineer's
stated flip-to-REJECT is *"if condition 1 cannot be met without a two-pass
formatter, or if it costs > 150 lines in the knot"* — in which case rules 1 and 2
go and 3, 4a and 5 stay. The llm-ergonomist's flip-to-REJECT is *"enforcement is
rejection **and** the spec sentence is refused on token budget"*, which the
spec-warden's zero-token result makes unreachable.

## Verdict table

| seat | verdict | section | cost / delta | condition (sharpest) |
|---|---|---|---|---|
| compiler-engineer | approve-with-conditions, **4b rejected as written** | design.md §4.1; CLAUDE.md §11 ratchet | **+79 lines**, one module; 0 lines elsewhere; 514 tests, 1 failed (an expected-string) | `heroes fmt` exits 0 on all 165 selfhost modules **first, alone** |
| llm-ergonomist | approve-with-conditions | thesis / locality | 33 → **47** lines on the sample (+42%), **14/47 = 29.8% blank** | resolve first-in-block and comment-first-in-body **before** landing |
| spec-warden | approve-with-conditions | design.md §1.2, §1.6, §4.15, §1.0 | spec **3592 → 3592, delta 0**; source tokens **delta 0** | rule 4 vs rule 1 precedence, because `#~v` resolves to the next **physical** line |
| ffi-pragmatist | approve-with-conditions | design.md §1.11, §4.19 | emitted C **byte-identical**, `#line` stripped, across three layouts | rule 4 carve-out for `extern` groups, stated not inferred |
| historian | **object** (advisory, no veto) | precedent | — | a canonical formatter that inserts before a control statement, live 3+ years, no reversal |

## The two findings that outrank the proposal

### 1. `heroes fmt` is not a fixpoint on 31 of 165 selfhost modules

Measured by the compiler-engineer and **independently re-measured by the
coordinator in the same session: 31 of 165, the same number**, with
`./heroes-next` at `e6bd83b` and no formatter change. Among them
`print_fmt.hero`, `check_walk.hero`, `check_map_keys.hero`, `ir_lower.hero`,
`emit_gate.hero`. `examples/` is clean, which is why nothing in the net sees it.

The failure is visible **only because of the output guard committed earlier the
same day** (`docs/defects/003`). Before that guard, `fmt --in-place` wrote
non-fixpoint output silently, which is what the morning's reverted sweep did to
118 files. The guard did not cause this; it made a four-month-old defect loud.

**Cause**, isolated by the engineer at `selfhost/check_map_keys.hero:102-108`:
`arm_alignment` decides run membership from **source adjacency**
(`print_fmt.hero:655`, `joined = run_index.len() > 0 && line == previous_end + 1`),
and pass 1's own 88-column break **moves those lines**. `spans_lines`'s comment at
`:718-727` records this exact lesson from defect D4 — *"true of source a person
writes and false of source THIS FORMATTER writes"* — and the lesson was written
about a different symptom of the same premise.

**A second defect found in passing**: `fits_on_one_line` omits the `return `
(7 chars) that `plain_valued` prints, so an arm is measured as fitting and is then
broken.

**And the result that surprised the sitting**: apples-to-apples on the same 165
files, HEAD refuses **31** and the engineer's rules-1-4 prototype refuses **7** —
a strict subset. **The proposal introduces no new non-fixpoint and removes 24.**
On the idempotence metric the author's rules *improve* the formatter.

### 2. `heroes fmt` turns a remark into documentation — the fourth instance of the class

Found by the compiler-engineer, **reproduced by the coordinator with
`--dump-ast`** in this session:

```
record Point
    x: i64
    # a remark, not documentation for y

    y: i64
```

`fmt` deletes the blank line, and the tree changes:

```
before:  field y: i64
after:   field y: i64
           doc # a remark, not documentation for y
```

design.md §4.1 makes that blank line the difference between a remark and
documentation, and §4.1's last bullet has the compiler reuse doc comments in
`???` output, error messages and `outline`. The blank is implemented at
`selfhost/cursor.hero:194-220` (`take_docs`, where `above != wanted` ends the run).

This is the panel-061 class — output that parses and means something else — which
`selfhost/cli_syntax_cmds.hero:71-75` **states in its own comment that the new
guard cannot catch**. `assert_canonical` would catch it, since the dumps differ.

**Rule 4b would make this defect law**: *"nothing between the comment and the code
it documents"* makes an ordinary remark inside a `record`, `variant` or `extern`
body **unwritable**. Hence its exclusion from data bodies in the resolution.

## Disagreements, not smoothed over

**Does the spec owe a sentence?** The two seats that answer it disagree flatly.

- The **llm-ergonomist** makes it condition 1: *"State the enforcement in the
  spec, or do not land the rules."* Its argument is that its whole write-side risk
  assessment turns on a fact the spec does not state — does an unformatted file
  build? — and that today's silence is harmless only because blanks are content.
  Its 31-word candidate ends *"The compiler accepts your spacing regardless"*,
  and it says that clause *"costs almost nothing and buys everything"*.
- The **spec-warden** says no sentence is owed, and would **flip to object** if one
  reached the ballot (+30 or +54 tokens; the second trips `design.md:330`'s delta
  gate). Its argument is stronger than price: `grep 'heroes '` over the spec
  returns **zero hits** — the document names **no tool at all**, so its silence on
  `fmt` is consistent rather than inaccurate. And the debt runs the other way:
  today two texts differing only in body blank lines are **both** canonical, so a
  textual difference is *not* semantic, which is what §4.15:1808 claims fmt
  prevents. **The proposal removes a degree of freedom and moves toward §4.15.**

**Unresolved by this sitting.** Both seats are right about different things: the
ergonomist wants the *enforcement* stated somewhere a reader will find it; the
warden refuses to make the spec the first place that documents a tool. The
resolution takes neither and records the question, because the answer decides
whether stage 3 owes a spec edit.

**Does precedent bind here at all?** The historian's own condition 2 is the
sitting's most honest paragraph: *"every precedent above is about a human author's
intent... Heroes' §1.1 thesis is about model error rates, and a model is not a Go
user with a grouping intent."* The llm-ergonomist **did** measure the model side,
and split: **4 backtracks → 1** (a grouping win) but a **confident error** at the
one dedent whose wrong answer compiles. So neither the precedent nor the
experiment closes the question, and they fail in opposite directions.

## The precedent the historian sourced, because it is the strongest argument against

All quotes verbatim, all URLs in the seat's own report; the coordinator did not
independently re-fetch them.

- **Black shipped rules 1-3's family** in 18.3a0-18.4a0 (2018) — *"always emit an
  extra empty line after `return`, `raise`, `break`, `continue`, and `yield`"* —
  and **removed it in 18.4a3 on 2018-04-24**, 24 days after the first complaint:
  *"Black no longer enforces putting empty lines behind control flow statements
  (#90)"*. In 2020 its author refused the general form: *"we sadly cannot enforce
  any blanket usage of blank lines... this argument proved to be extremely
  unpopular so I backed out of it"*.
- **gofmt refused the equivalent and closed it the same day** (griesemer,
  go#22337): *"gofmt respects the decision of the source author... there's just no
  good way to capture intent well, automatically. Better to let the author
  decide."*
- **Prettier**: *"empty lines are very hard to automatically generate. The approach
  that Prettier takes is to preserve empty lines the way they were in the original
  source code."*
- **Black reversed a second blank-line rule** for the 2024 stable style, maintainer's
  reason: *"at most marginal benefit, especially given its churn"*.
- **The sentence that changed Black's mind**, njsmith 2018-04-21: *"Blank lines
  inside a function are effectively a kind of extra-terse comment... Black shouldn't
  be in the business of adding or removing comments."* That is `DESIGN-LOG:46`'s
  *"blank lines are content"*, reached independently by three projects.
- **Every insertion in any canonical formatter's default output sits at a
  DECLARATION boundary.** The option names encode it: clang-format's
  `SeparateDefinitionBlocks`, rustfmt's `blank_lines_lower_bound` ("between
  **items**"), ktlint's `blank-line-before-declaration`, Black's "before and after
  **function definitions**". None keys on a statement kind.
- **Rule 1 has zero precedent** in any canonical formatter's default output. The one
  live in-body inserter, **elm-format** (0.8.0, 2018-08-22, mandatory, no config,
  not reversed in eight years), inserts **between the clauses of an `if`**, not
  before the construct — so even it does not implement rule 1. Its comment rule
  carries the table's one open, unresolved user complaint, from 2016.
- **The empirical literature is empty.** No controlled experiment isolates
  blank-line insertion. Buse & Weimer (TSE 2010, 120 annotators) rank blank lines
  above comments for *perceived* readability of human-written code. Norcio (CHI
  '82), by secondary attribution only, suggests blank lines help at **semantic
  chunk** boundaries — the opposite of a syntactic rule, and exactly griesemer's
  objection.
- **Go paid for its one comparable change with a corpus measurement first**: *"only
  3% of doc comments in public Go modules were reformatted at all by the draft Go
  1.19 gofmt"*. **Rust built style editions and Black a stability policy** to avoid
  this churn. Heroes has neither, and §10 admits no config file, so **the sweep is
  the only instrument in both directions.**

## Measurements this sitting produced

| what | number | by |
|---|---|---|
| spec tokens, before and after | **3592 → 3592**, delta **0** | spec-warden, re-measured by coordinator |
| source tokens, 191→225-line airy pair | **2587 → 2587**, delta **0** | spec-warden, re-measured by coordinator |
| all 165 files, blank before every indented line | +65.7% lines, **cl100k 608796 → 608796** | spec-warden |
| the mechanism | `\n\n␣␣␣␣` and `\n␣␣␣␣` are **one pretokeniser piece**; a blank before an indented line costs **0**, before a column-0 line costs **1** | spec-warden |
| emitted C, three layouts of `examples/sqlite/main.hero`, `#line` stripped | **0 differing lines, byte-identical** | ffi-pragmatist |
| `otool -L`, split vs tight `package "libcurl"` group | identical | ffi-pragmatist |
| a blank line cannot split an `extern` group | proved by **deleting `|| continues`** at `print_fmt.hero:182`, rebuilding, and formatting `examples/sqlite`: air between all nine members, **one head line**, idempotent | ffi-pragmatist |
| implementation cost | `print_fmt.hero` **948 → 1027, net +79**; 0 lines in lexer, checker, descriptors, ownership, emitter | compiler-engineer |
| non-fixpoint modules, HEAD | **31 of 165** | compiler-engineer, **independently re-measured by coordinator: 31** |
| non-fixpoint modules, rules-1-4 prototype | **7 of 165**, a strict subset | compiler-engineer |
| ratchet, code lines | **30589 → 40275, +31.7%**; `check_walk` 1385 → **2399** | compiler-engineer |
| files newly red in `suite_layout.hero` | **44 of 165** | compiler-engineer |
| reader backtracks, sample function | **4 → 1** | llm-ergonomist |
| the sample under the rules | 33 → **47** lines, **14 blank (29.8%)** — the brief said 44 and "one in five" and was **wrong by a third** | llm-ergonomist, re-measured by coordinator |

**Corrections the seats made to the brief**, recorded because CLAUDE.md §1 exists
for exactly this: 165 files not 160; `match` at body indent **541** not 643; upper
bound **8239** not 8341; the sample **47** lines not 44 and **29.8%** blank not
20%; and **the blast-radius argument was a LINE count against a TOKEN formula** —
the unit mismatch that misdirected panel 067, made again by the coordinator who
wrote that rule into the brief.

**One brief error of the coordinator's own**: the historian was pointed at
`docs/panel/007-*.md`, has no glob tool, and the file is
`docs/panel/007-terminator-enders.md`. It reported the citation as unread rather
than guessing, and the continuation-line ruling is therefore absent from its
analysis.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | `for f in selfhost/*.hero; do ./heroes fmt $f >/dev/null \|\| echo $f; done` still prints at least one file **unless `print_fmt.hero:655` stops asking the source for adjacency**. Today it prints 31 | M-package-layout close |
| compiler-engineer | if rules 1-4 land, `suite_layout.hero`'s `DECIDED` entry for `check_walk.hero` reads **≥ 2399**; under 2000 means fmt refused it or the rules were silently narrowed | the landing commit |
| spec-warden | landing rules 1-5 changes the binding spec count by **exactly 0** (3592 → 3592), and re-rendering the spec's ten code fences in the new form also costs **0**. Over **+10** means an untold cost | the landing commit |
| spec-warden (observation, pays nothing) | with airy fences, Part 11 metric 2's first-try arm moves **< 2 points**, Wilson overlapping zero | M-selfhost-fixpoint |
| ffi-pragmatist | `heroes fmt` over all four of `examples/{sqlite,curl,sdl,raylib}/main.hero` emits **exactly one `^extern ` head line per group the author wrote**, and stripped `--emit-c` is byte-identical before and after, for all four | M-ffi-ladder |
| llm-ergonomist | error rate at a dedent **scales with how anomalous that dedent's depth is** among the blanks in the same function: the proposal loses by ≥15 points where the loop-exit dedent is the minority depth, wins on grouping questions by ≥20, and is a wash (±5) in aggregate on the once-or-per-iteration question | the 20-function experiment, before ratification |
| historian | the formatter carrying rules 1-3 emits a blank no author would write, in one of three shapes: **(a)** a control form as the **first** statement of a block, **(b)** a block whose **only** statement is a control statement, **(c)** a function whose body is one `match`/`if` then `return` | first `fmt --in-place` run, **before the sweep is committed** |

**Note on the historian's shape (a)**: partially answered already.
`fmt_block:394-404` carries a `first` flag whose comment reads *"never before the
FIRST statement: a block does not open on an empty line"* — so (a) does not fire
if rule 1 is implemented inside `fmt_block` and respects that guard. Coordinator's
reading of the code, not a seat's measurement. Shapes (b) and (c) stand open.

## Consolidated conditions

1. **`heroes fmt` exits 0 on all 165 `selfhost/*.hero`, in its own commit, before
   anything else here.** Today 31 fail. (compiler-engineer, blocking)
2. **`assert_canonical` exists**, with the record-field remark defect as its first
   named `tests/golden/fixedbugs/` case. (compiler-engineer, spec-warden)
3. **Rule 4 scoped to statement bodies**, excluded from `record`, `variant` and
   `extern`-group bodies, through the one shared `comments_before` parameter and
   never by letting callers diverge. (compiler-engineer, ffi-pragmatist)
4. **Rule 2 narrowed** to a dedent closing a `for`/`while`/`match`, never an `if`
   (8 → 3 blanks on the sample function). (llm-ergonomist, compiler-engineer)
5. **Rule 1 vs rule 4 precedence written down**, with a `#~v`-above-a-control-line
   golden case that is red before the fix. `suite_annotations.hero:258` resolves
   `#~v` to `line + delta`, the next **physical** line, with no blank-skipping.
   Exposure today: **3** `#~v` in the golden tree, **0** above a control-flow line
   — latent, which is when it is cheap. (spec-warden)
6. **`suite_layout.hero`'s 16 `DECIDED` numbers re-measured with the reason
   written, or `code_lines` stops counting blank lines.** Silence means 44 red
   files. (compiler-engineer)
7. **Attack the adjacent shapes before the commit**: first-in-block,
   comment-first-in-body, `else`, a match arm, an empty body, a one-statement
   function, a `record` field block, an `extern` group member. The provoking case
   is a witness, not the class. (spec-warden, llm-ergonomist, ffi-pragmatist)
8. **`opens_block` carries a comment naming `valued()` as the dispatch it must
   match, and a test that fires when they diverge.** It is a second dispatch over
   statement and expression kinds that must agree with `valued()` arm for arm —
   `docs/defects/003`'s exact shape. (compiler-engineer)
9. **Head-line count golden**: format a group, assert `grep -c '^extern '`
   unchanged. Three lines, and it closes what the new output guard admits it
   cannot see. (ffi-pragmatist)
10. **Name the sweep on the ballot rather than assuming it.** The rules alone
    change **0 of the 165 files the author reads**; `selfhost/` is not canonical
    today (`print_fmt.hero` differs by 419 lines). The delivery is a second commit
    with its own review cost. (spec-warden)
11. **Consider the cheaper alternative first** (llm-ergonomist's E7): canonicalise
    only the blanks that mean nothing — forbid a blank as the first or last line of
    a block, forbid runs, insert none — and leave the author the ones that mean
    something. Same diff hygiene, no signal deleted, no spec sentence owed.

## Author's verdict

**RATIFIED 2026-08-28** (`/decide`, answer `1a`), after the four stages had
landed and the author had read the result on real files.

The ratification covers the resolution as adopted, not the proposal as tabled,
and the difference is what the sitting bought:

- **Blank lines are structure as well as content.** The reversal of
  `DESIGN-LOG:46`'s stated principle stands for the named cases; the content half
  is untouched, so a blank the author wrote inside a body still survives.
- **Rule 3 does not exist.** Dropped by the author on the sitting's evidence: no
  seat defended it, the llm-ergonomist showed it teaches a reading rule 2 already
  owes, and the compiler-engineer showed it was the only one of the four needing
  a new parameter threaded to eight call sites.
- **Rule 2 is narrowed** to a dedent closing a `for`/`while`/`match`, never an
  `if` — the llm-ergonomist's condition, and the one that kept the tight blocks
  tight.
- **Rule 4 is scoped to statement bodies**, excluded from `record`, `variant` and
  `extern` bodies, on the remark-becomes-documentation evidence.
- **The order of work stands as stages 1-4**, and all four landed in that order:
  `aa8a372`, `a0ab809`, `8584d18`, `9944209`.

**What the author decided that this file could not**: the canonical line width
moved 88 → 120 in the same session (`95a0214`), which no seat had been asked
about. It very nearly cancels the air: the canonical form was 47,964 lines
without air at 88 and is 48,096 with air at 120 — **132 lines apart on 48,000**.

**What the ratification does NOT settle, and both were answered outside it**: the
spec owes no enforcement sentence (the coordinator's call on the spec-warden's
argument — the spec names no tool at all, so its silence on `fmt` is consistent
rather than inaccurate, and a sentence would be its first tool sentence), and the
llm-ergonomist's cheaper alternative was **refused after measurement**: all four
of its clauses were already the behaviour, so it was not an alternative but the
status quo the author had said was hard to read.

**What a yes settles.** That blank lines become **structure** rather than content
for the named cases — the reversal of `DESIGN-LOG:46`'s stated principle, which is
this sitting's actual proposition. That the order of work is stages 1-4 above, so
the fixpoint repair and `assert_canonical` land **before** any rule and before the
sweep. That rule 4b does not reach `record`, `variant` or `extern` bodies, on the
evidence of the remark-becomes-documentation defect. That rule 2 is narrowed to a
dedent closing a `for`/`while`/`match`.

**What a yes does NOT settle**, and each of these comes back:

- **Whether rule 3 survives at all.** Two seats condemned it independently and on
  different grounds — the llm-ergonomist because it teaches a reading it does not
  support (the blank before a closing `return` is already owed by rule 2 whenever
  a block just closed, so *"the function ends here"* is right by accident), the
  compiler-engineer because it is the only one of the four needing a new parameter
  threaded to 8 call sites. Nobody defended it. It is on the ballot separately.
- **Whether the spec owes an enforcement sentence.** The llm-ergonomist makes it a
  condition; the spec-warden would flip to object if one reached the ballot. The
  sitting takes neither and the question is live.
- **Whether the cheaper alternative should be taken instead** (llm-ergonomist E7):
  canonicalise only the blanks that mean nothing — no blank at a block's first or
  last line, no runs, insert none. Same diff hygiene, no author signal deleted, no
  spec sentence owed, and it does not touch the incumbent principle at all.
- **Whether the historian's precedent applies.** Its own condition 2 says the whole
  lineage is about human authors' intent while §1.1's thesis is about model error
  rates. A yes should say explicitly whether the panel is overriding that
  precedent or declaring it off-topic, rather than leaving the record ambiguous
  about which.

**What a no settles**: stages 1 and 2 are owed regardless. The 31 non-fixpoint
modules and the remark-becomes-documentation defect are live defects in the shipped
compiler and do not depend on any rule in this proposal.

## Process notes

- The working tree was frozen from the briefs going out to this synthesis, and the
  freeze held.
- **Two seats died mid-flight and were resumed from their transcripts**: the
  ffi-pragmatist to a machine sleep, the compiler-engineer to a session limit.
  Both finished. Resuming with a summary of what the other seats had already
  returned saved the second one from re-deriving the zero-token result.
- Four of five seats compiled something. The historian ran 80 tool calls and
  listed nine claims it could not source, including one it had got wrong on a
  first fetch and corrected — which is the seat working as designed.
