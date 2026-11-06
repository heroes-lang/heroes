# Panel 234, the spec-warden's report

Written as it goes, 2026-11-05, in `.claude/worktrees/234-spec-warden/`,
detached at `a22d08938` (the briefs and the reader test's materials), whose
compiler, built from the seed, says `heroes 0.6.0` / `sources
1a1e340778db789a`. Scratch: `.claude/worktrees/234-spec-warden-scratch/`.
`spec/heroes-spec.md` is the same bytes at `a22d08938` as at `f0c4c99d2`
(`git diff --quiet`, exit 0).

## 1. The reader test's predictions, registered before any session

### What they rest on

Read whole: `blind/README.md`, the four session templates, both S cards, both
S modules and drivers, `S/check.txt`, the three S tasks and their nine cases
(bytes read with `od -c`), both Q cards, both Q modules and the C module under
thick, the six Q references, the three Q tasks and their nine cases, the
specification (473 lines), and the critic's F1, F18 to F24 and F37.

**The S variants differ in the four names and nothing else**, re-run here:
`diff S/card-G.md S/card-H.md` changes card lines 15, 16, 22 and 24 alone, and
`diff S/core-H/core/strings.hero S/core-G/core/strings.hero` changes the four
declarations, the two internal calls (`contains`, the cut) and test lines;
the drivers are identical.

**Base rates, measured before this sitting** (writing a Heroes program from
the specification alone, first turn, strict arm): the assistant's ten tasks,
Haiku 5.5 16 of 30 and Sonnet 5.5 18 of 30 green at turn 1, 27 and 29 by turn
2 (measurement 050); the held-out run, Haiku 5.5 86 of 120 and Sonnet 5.5 94
of 120 at turn 1, silent at turn 3 10 and 4 of 120 (measurement 052).

**Run here, with this worktree's compiler, on copies of the arms' trees**
(`234-spec-warden-scratch/names/`, `234-spec-warden-scratch/q/`):

| a first try writes | the compiler says | what a second turn gets |
|---|---|---|
| a G name against H, or an H name against G (`cut`, `index`, `lines`, `split_once`, `split_lines`), or a guess (`index_of`, `find`) | `unknown_in_module`: *module `core/strings` declares nothing called `cut`*, the excerpt, **no list of what it declares and no near name** | what is absent, never what exists |
| `strings.HasPrefix(...)` | `unknown_in_module`, no suggestion | the same |
| `strings.has_prefix(line, "ERROR ")` | `needs_label`, both names offered as fixes | the labels |
| `line.has_prefix(prefix: ...)` | `needs_qualifying`, fix `strings.has_prefix` | the qualified name |
| a wrong field of `cut`'s result | `no_such_field`: *`Halves` has `before`, `after`* | the fields |
| `strings.index(...) >= 0`, Go's contract | `bad_operand`, fix `.must()` (an abort) | a repair that aborts if taken literally |
| `keys @= 0`, S2's natural counter | `builtin_name_taken` | *pick another name* |
| `print("bad: " + n)` | `mixed_arithmetic`, fix `n.to_str()` | the rendering |
| thick: `sql.exec(...).must()` alone on a line | `discarded_value`, fix (certain) `_ = ...` | the discard |
| thick: `_ = sql.exec(...)` | `discarded_failure` | `.must()` or `.default` |
| thick: `db = sql.open(...).must()`, then `sql.close(@db)` | `not_mutable`, fix `db @= ...` | the cell |
| thin: `sql.exec(db, ...)` alone on a line | `discarded_value` | the discard |
| thin: `db @= nullptr` | `cannot_infer`, fix `db: <Handle> @= nullptr` | the annotation |
| thin: `SQLITE_OK` unqualified | `needs_qualifying`, fix `sql.SQLITE_OK` | the qualified name |

And at run time, thin only: **a statement never finalized prints every right
line and then exits 134** (`panic: 2 C handle(s) never given back`); **a
`bind_text`/`step` whose codes are discarded with `_ =` and no `reset`
prints `count: 1` and `sum: 3` for `3 5 -2 10` at exit 0**, a silent wrong
answer the thick shape has no route to; one `Stmt` cell reused for a second
statement after its `finalize` builds and runs right.

So every compile-time slip but one carries its own repair, and the one that
does not, `unknown_in_module`, is the bare arms' own. **The shapes part at run
time, the bare arms at the name.**

### The rule that scores them

- **The unit is a share of a cell's answers, pooled over the two models**: ten
  answers at five samples, eight or six if the README cuts to four or three,
  so the bands survive the cut (F19). A void session leaves the denominator;
  a session the budget stopped stays in it, not right, as the README scores it.
- **The grade is the README's**: `right@1`, `right@2` (which includes
  `right@1`) and `silent` exactly as it defines them, standard output byte for
  byte.
- **A cell is a point and a band**: a hit if the measured share lies inside the
  band, its ends included; a miss otherwise.
- **An effect is an interval** for arm A's pooled share minus arm B's: a hit if
  the measured difference lies inside it, its ends included.
- **Per model**, unscored per cell (five answers say too little): Sonnet 5.5
  near the pooled point plus 0.1, Haiku 5.5 near it minus 0.1. Scored once:
  **Sonnet's `right@1` over all eighteen cells is at least Haiku's**.
- **What I expect of myself**: about two in three cell bands hit. Fewer than
  half would say the failure model below is wrong, not unlucky.

### The failure model, in one paragraph

A first turn is right when it makes no slip of the language (the codes of
measurements 049 and 050, plus S2's `keys`), no slip at the package (a label,
a name, a `T?` unhandled), and no slip of logic the build cannot see (a line
trimmed before its `ERROR ` test, a status code discarded). A card arm knows
every name and label; a bare-G arm knows Go's names and parameter names, which
exist in G, but not the `Halves` fields nor that `Trim(s, " \t")` and
`SplitN` are absent; a bare-H arm knows that four names differ and not what
they are, so it is right at turn 1 only by routing around `index`, `cut` and
`lines` with `split`, `join` and a loop, and its second turn learns only what
is absent. Thin Q writes twice the statements and can discard a status; thick
Q must answer every `T?`, and Q1's empty case asks it for NULL by name.

### Experiment S: points and bands, pooled over the two models

| task | arm | `right@1` | `right@2` |
|---|---|---|---|
| S1 | card-G | 0.6 [0.4, 0.8] | 0.85 [0.65, 1.0] |
| S1 | card-H | 0.65 [0.45, 0.85] | 0.85 [0.65, 1.0] |
| S1 | bare-G | 0.55 [0.35, 0.75] | 0.8 [0.6, 1.0] |
| S1 | bare-H | 0.3 [0.1, 0.5] | 0.65 [0.45, 0.85] |
| S2 | card-G | 0.5 [0.3, 0.7] | 0.75 [0.55, 0.95] |
| S2 | card-H | 0.5 [0.3, 0.7] | 0.75 [0.55, 0.95] |
| S2 | bare-G | 0.3 [0.1, 0.5] | 0.65 [0.45, 0.85] |
| S2 | bare-H | 0.2 [0.0, 0.4] | 0.5 [0.3, 0.7] |
| S3 | card-G | 0.8 [0.6, 1.0] | 0.9 [0.7, 1.0] |
| S3 | card-H | 0.8 [0.6, 1.0] | 0.9 [0.7, 1.0] |
| S3 | bare-G | 0.7 [0.5, 0.9] | 0.9 [0.7, 1.0] |
| S3 | bare-H | 0.7 [0.5, 0.9] | 0.9 [0.7, 1.0] |

`silent`, pooled per arm over S1 to S3 (thirty answers): every arm 0.07,
band [0.0, 0.2]; the likeliest silent program trims a line before testing
`ERROR ` (S1's hidden ` ERROR indented: no`).

### Experiment Q: points and bands, pooled over the two models

| task | arm | `right@1` | `right@2` |
|---|---|---|---|
| Q1 | thin | 0.45 [0.25, 0.65] | 0.7 [0.5, 0.9] |
| Q1 | thick | 0.4 [0.2, 0.6] | 0.6 [0.4, 0.8] |
| Q2 | thin | 0.35 [0.15, 0.55] | 0.6 [0.4, 0.8] |
| Q2 | thick | 0.55 [0.35, 0.75] | 0.8 [0.6, 1.0] |
| Q3 | thin | 0.4 [0.2, 0.6] | 0.6 [0.4, 0.8] |
| Q3 | thick | 0.6 [0.4, 0.8] | 0.9 [0.7, 1.0] |

`silent`, pooled per arm over Q1 to Q3 (thirty answers): thin 0.1 [0.0,
0.25], thick 0.03 [0.0, 0.1], **and thin's at least thick's**.

**Q1's empty case, read apart**: of Q1's final programs that build, the share
answering case 2 wrong (SQL's `sum` over no rows) is thick 0.3 [0.1, 0.5] and
thin at most 0.1.

### The effects each comparison predicts

| comparison | metric | predicted | hit if the difference lies in |
|---|---|---|---|
| **C, names, cards**: card-H minus card-G, S1 and S2 (20 a side) | `right@1` | +0.03 | [-0.15, +0.15] |
| | `right@2` | 0.0 | [-0.15, +0.15] |
| | `silent` | 0.0 | [-0.1, +0.1] |
| **C, names, bare**: bare-H minus bare-G, S1 and S2 | `right@1` | -0.18 | [-0.40, -0.05] |
| | `right@2` | -0.15 | [-0.35, +0.05] |
| | `silent` | 0.0 | [-0.1, +0.15] |
| **C, the control**: H minus G on S3, card and bare each (10 a side) | `right@1`, `right@2` | 0.0 | [-0.2, +0.2] each |
| **E, the card under G**: card-G minus bare-G, S1 to S3 (30 a side) | `right@1` | +0.12 | [0.0, +0.3] |
| | `right@2` | +0.05 | [-0.1, +0.2] |
| **E, the card under H**: card-H minus bare-H, S1 to S3 | `right@1` | +0.25 | [+0.1, +0.45] |
| | `right@2` | +0.15 | [0.0, +0.3] |
| **E, the interaction** | `right@1` | the card is worth more under H than under G | a hit if (card-H minus bare-H) exceeds (card-G minus bare-G) |
| **D, thickness**: thick minus thin, Q1 (10 a side) | `right@1` | -0.05 | [-0.3, +0.2] |
| | `right@2` | -0.1 | [-0.35, +0.15] |
| thick minus thin, Q2 | `right@1` | +0.2 | [0.0, +0.45] |
| | `right@2` | +0.2 | [0.0, +0.45] |
| thick minus thin, Q3 | `right@1` | +0.2 | [0.0, +0.45] |
| | `right@2` | +0.3 | [+0.05, +0.5] |
| thick minus thin, Q1 to Q3 (30 a side) | `right@1` | +0.12 | [0.0, +0.3] |
| | `right@2` | +0.13 | [0.0, +0.3] |

**Two first codes, registered because they say why**: in bare-H, S1 and S2,
`unknown_in_module` is the first code of at least half the first turns that
do not build; in S2, all four arms, `builtin_name_taken` is the first code of
at least 10% of the first turns (four of forty at five samples).

**What the names prediction says in words**: on a card the author's rule
neither helps nor costs at these tasks, because Go's contract on `index` and
`cut` is a compile error in Heroes (`bad_operand`; a `Halves?` cannot be read
as three values), and `lines`' Go contract (ends kept) changes no S1 or S2
output. Without a card the rule costs a first turn, because the new names
cannot be known and the compiler does not list them; most of that cost is
repaid at the second turn by routing around them.

### Ceilings and floors: cells where both readers will be right, or both wrong

- **Ceilings** (Sonnet 5 of 5 and Haiku at least 4, so the comparison there
  shows nothing): **S3 `right@2` in all four arms**, the control at its
  ceiling as a control should be; **S1 card-G and card-H `right@2` on Sonnet**;
  **thick Q3 `right@2` on Sonnet**.
- **Floors** (at most 1 of 5): **bare-H and bare-G S2 `right@1` on Haiku**, so
  the bare names comparison on Haiku's S2 at turn 1 is read between two floors
  and says nothing; **thin Q2 `right@1` on Haiku**.

### Does a card lean?

**The S cards do not lean.** Their one-liners are the same words in the same
order; `Halves`' fields are Go's own result names, `before` and `after`, in
both; each card leaves out `matches_at` and `is_space`, public in both
modules, so each says less than its module by the same two functions. Two
residual pulls run opposite ways and I expect neither to show at twenty a side:
H's `split_once` and `split_lines` describe themselves, G's names are recalled
from Go. **The design leans in two places, neither of them a card.** The tasks
give the rule's benefit little room (the paragraph above): S1 and S2 need
`cut`/`split_once` and `lines`/`split_lines`, whose Go contracts either fail
the build or change no output, and no task needs `index`, `last_index` or a
line's end. And bare-H is told that four names differ and not what they are,
against a compiler that names nothing in its place, so bare-H against bare-G
measures the price of an unknowable name, which is real for a reader without
the card and cannot show the rule's benefit.

**The Q cards do not misdescribe their modules**, read against the code:
thin's ten functions, two handles, four constants and two codes are the
module's whole face (its `sqlite3_*` members are not callable across a
module, panel 033 R5), and thick's six functions, four types and six codes
likewise. **Two differences favour a shape by design**: Q1's case 2 favours
thin (`column_int64` reads NULL as 0; thick's `integer` fails `null_value`),
which the README names; and the thin card carries SQLite's protocol in prose,
which is what thinness is. **One lean the README does not name: the grade is
standard output alone.** A thin program that forgets one `finalize` prints its
right lines and exits 134 (run above), and is scored right; a thin program
ends two to four handles where a thick one ends one, so the stdout-only grade
flatters thin. **I ask the coordinator to record each final program's exit
status per case**, so the stricter reading (right means right output **and**
exit 0) can be scored beside the registered one; under it I predict thin's
`right@2` lower by 0.0 to 0.2 and thick's by at most 0.05.

### What the run will cost

**About 8 USD, band 4 to 15**, for 180 first turns and the about 70 second
turns these predictions imply (45 in S, 23 in Q). Per session: Haiku 5.5
about 0.016, **measured** for a Haiku session writing a program through
`claude -p` (0.010 to 0.023, measurement 054); Sonnet 5.5 about 0.05, **an
inference** from panel 232's mean of 0.0195 over both models for short
answers plus a program's output. The first session of each model checks it:
a Sonnet session above 0.10 or a Haiku one above 0.04 says this projection is
wrong. Under 25 USD with room, so no cell is cut.

## 2. The instrument and the baseline, run here

- **The ceiling, read by grep today** (`grep -n '16384' docs/design.md`, §1.6
  at line 255): *16384 tokens, measured by `claude-opus-5` through `POST
  /v1/messages/count_tokens`*, the payment rule unconditional at every level
  (a named removal, or a registered prediction naming an instrument that
  exists today, panel 046). The consolidation block's 1,500 was a cap on the
  rows M-buildable-structs to M-compatibility-promise before the thesis
  harness ran; the harness has run (measurement 052, 2026-10-24) and the
  block has closed, so nothing of it binds this milestone, and the payment
  rule does, unchanged.
- **The baseline, at `a22d08938`**: `heroes measure spec/heroes-spec.md`
  offline reads `claude-legacy` **7,876**, `cl100k_base` **8,019**, maximum
  **8,019**, spread 143; `--refresh` with the trunk's `.env` loaded (the key's
  length printed, 108, never the key) reads **10,514 real** on
  `claude-opus-5`, taken **2026-11-05**, digest `72dff50a68d58c19`: the
  brief's figure, recorded 2026-10-30, holds today. Headroom **5,870**, of
  which the FFI floor mortgages **60** (panel 030 R3).
- **How a draft is priced here**: written into this worktree's
  `spec/heroes-spec.md` by a script that asserts its anchor matches exactly
  once, measured offline and with `--refresh`, then the file restored with
  `git checkout -- spec/heroes-spec.md` and its digest checked against
  `72dff50a68d58c19` before the next draft.

## 3. Item A and the frame: Principle 0's reach

- **verdict**: **object** to the page's sentence as worded, *Principle 0 binds
  nothing in this milestone*; **approve** it narrowed: *Principle 0 binds no
  package and no function of one; it binds every form, built-in and sentence of
  the specification the milestone would add (items F, H, I, J, L, M), and
  §1.11 with the admission test binds the packages.* **Approve**
  `conventions-draft.md` §6 as the packages' stopping rule in Principle 0's
  place, amended as below.
- **section**: design.md §1.0, whose object is *every syntactic form* and whose
  burden is *for any proposed form*; §1.11; §1.6's payment rule; §1.2.
- **why a package owes Principle 0 nothing, and where that stops**. A package
  is Heroes code: no production, no built-in, no token of the specification.
  §1.0's text reaches none of it. **But the stop depends on item B's route.**
  Under B1, B2, B3 or B6 a package is a file the program's own tree holds, and
  §1.11's *Heroes ships a minimal runtime and nothing else* holds by its
  letter. **Under B5, the tree compiled into the compiler, the compiler ships
  the packages**: every function becomes a name the compiler offers to every
  program without a file in its tree, which is what a built-in is to a reader,
  and both §1.11's sentence and §1.0's burden then bind each one. So B5 is not
  a placement choice; it is an amendment of §1.11, the author's to ratify, and
  it would bring the whole tree under Principle 0.
- **What the packages do owe, which is §1.2's**: a card is read in the prompt,
  so its tokens are the packages' rent (item E); and a name that invites a wrong
  contract raises the rewrite rate, which is the author's naming rule read as
  §1.2.
- **§6, amended**: (1) *a program already needs it* is a command's count, issue
  805's `grep -rl '^function <name>(' examples` or a named acceptance program,
  written beside the function; (2) *easy to get subtly wrong* is shown by a
  run, a plausible hand-written loop that builds and answers wrong (the
  `silent` class), never asserted; (3) **a card line exists only for an
  admitted function**, so the card budget is the admission test's ledger.
- **cost**: 0 tokens of the specification.
- **prediction**: every one of the twenty packages is written with no form added
  (checked at each package's step); the first that cannot be is the falsifier,
  and the form it needs owes §1.0's ticket at a sitting of its own.
- **condition**: the sitting adopting B5 (above), or a package step that needs a
  form, reverses *binds no package* for that package.

## 4. Item E: the card's budget

**Measured** with `heroes measure` (vendored, a lower bound) and, for the real
count, **each card appended to this worktree's specification and refreshed**,
the file restored after each (`234-spec-warden-scratch/price/prices.tsv`):

| card | vendored (lower bound) | real, beside the specification | share of the specification's 10,514 |
|---|---|---|---|
| `S/card-H.md` (14 functions) | 585 | **+762** | 7.2% |
| `S/card-G.md` | 581 | **+759** | 7.2% |
| `Q/thin/card.md` (10 functions) | 898 | **+1,230** | 11.7% |
| `Q/thick/card.md` (6 functions) | 727 | **+923** | 8.8% |

The real count runs **1.27 to 1.38 times** the vendored one, the code-heavy thin
card the highest: a budget written in vendored tokens would be a third low.

- **verdict**: **approve** a card per package, under three numbers.
  1. **Per package, at most 1,024 real**, measured as above. The thin SQL card
     is **206 over**: thinness costs the prompt 307 real tokens against thick
     for the same three tasks (+33%), a cost of item D the reader test does not
     score and this one does.
  2. **The twenty cards together, at most the specification's real count**
     (10,514 today, a mean of 525 a package). The reason is §1.11 made a
     number: the packages are the language's thin companion, so the whole
     library's interface is never larger than the whole language's. This is a
     **discipline**, not a measurement, and it says so; it is re-set the way
     §1.6 says its own next raise is set, by what item N measures a card token
     to buy.
  3. **A card pays by §1.2**: its T real tokens buy their place if the reader
     test's lift in `right@1` times the measured median correction round-trip,
     **3,619 tokens** (measurement 052), is at least T. For the strings card
     (about 760) that is a lift of at least **0.21**.
- **section**: design.md §1.2, §1.6, §1.11; `.claude/rules/spec-shape.md`
  (*always measure with the real*).
- **cost**: 0 tokens of the specification.
- **prediction**: (a) by my reader-test predictions, the strings card **pays
  under H** (predicted lift +0.25, worth about 905 against its 762) and **does
  not under G** (+0.12, worth about 434 against 759), scored when the sessions
  are; (b) the twenty cards, written under the admission test, total **at most
  8,000 real**, scored at the milestone's close by the same route.
- **condition**: a card-minus-bare lift at or under 0 in both variants says a
  card buys nothing at these tasks, and the budget falls to an index line per
  package.

## 5. The specification's sentences each route would move, priced

Every draft applied to this worktree's `spec/heroes-spec.md` by
`234-spec-warden-scratch/price/price.py` (each anchor asserted to match exactly
once), measured offline and with `--refresh`, then restored and checked byte
for byte. Deltas against **10,514 real** and **8,019 vendored**; every draft's
whole text is under `price/drafts/`.

| id | item, route | the words (new text in *italics*) | real | vendored |
|---|---|---|---|---|
| F0 | F, P3 or P4 (C shipped with a package) | no change | 0 | 0 |
| **F1** | F, P1 or P2' with `net` and `time` in the runtime | *Anything beyond this document, maths, JSON, databases, comes from C libraries; threads, processes, files, **sockets and clocks** from the language's `hero_os.h`.* | **+4** | +2 |
| **F2** | F, P1' (one object, a family of headers) | *...; threads, processes, files, sockets and clocks from the language's own headers, `hero_os.h` and the `hero_*.h` beside it.* | **+21** | +16 |
| **F3** | F, P2 (a platform layer, a second object) | *...; threads, processes and files from the language's `hero_os.h`, and sockets and clocks from its platform layer's `hero_net.h` and `hero_time.h`, linked where a group names one.* | **+37** | +29 |
| H1 | H, `tag` on an `extern function` | `Member = "function" ident [ "tag" ident ] "(" ...`, and *A function whose C name this language reserves is declared under another, with C's after `tag`, any word: `function c_match tag match(x: i32) -> i32`.* | +59 | +46 |
| H2a | H, the family, listed | *A quoted word stands for itself, and `as` `link` `package` `tag` `partial` `owned` `counted_by` `lent` `consumes` `acquires` `borrows` `transfers` `retains` `when` `rest` `zero` only where a production writes them, a name everywhere else;* | +83 | +70 |
| H2b | H, the family, as a rule | *A quoted word stands for itself, and one that follows what it modifies, as `tag` and `owned` do, is a name everywhere else;* | +32 | +24 |
| I0 | I, per group, compiler tightened, seeds not counted | no change (and `process.hero`'s three `hero_dir_*` move to its `hero_os.h` group) | 0 | 0 |
| **I3** | I, per group with the language's own headers | *clang checks every result type, constant and record field against that header **and the language's own**, ...* | **+5** | +5 |
| I1 | I, per module, as the compiler runs today | *... against **its module's headers and the language's own**, ...* | +8 | +7 |
| I2 | I, per module | *... against **its module's headers**, ...* (false on the seeds) | +3 | +2 |
| J1 | J, `alias`, panel 135's settled form, gross | *`alias Env = {str: i64}` names a type: the two are one type everywhere, an error writes both, and an alias never names itself, even through another.* and `\| "alias" ident "=" Type NEWLINE` | +66 | +48 |
| L0 | L, the specification silent | no change | 0 | 0 |
| L2 | L, short | *(`write_failed`; it replaces a file whole through a renamed copy, and writes in place where a copy cannot serve)* | +31 | +20 |
| L1 | L, the routes | *(`write_failed`; it writes a copy beside the file and renames it over it, so a reader sees the old text or the new, and writes in place where the path is not one regular file with one link it may write, or no copy can be created or renamed there)* | +64 | +54 |
| L3 | L, five routes | L1 with *`write_failed`, as when the device is full* and *named, created or renamed there for another reason* | +79 | +66 |
| M1 | M, `sort_by` built in | *... `all` (with `find`, stopping at the first that decides) · `sort_by` (stable, its function saying whether its first argument goes first) · `range`.* | +25 | +19 |

**The routes the sitting is likeliest to adopt, priced for real**: F2 or F3 for
the author's platform layer (+21 or +37), F1 if `net` and `time` join the
runtime as today's layer (+4), and I3 (+5). Every one sits far under the
ceiling (headroom 5,870), so **no route is vetoed on budget**; each still owes
§1.6's payment, a named removal or a registered prediction naming an instrument
that exists. F1 pays most of itself: it removes *sockets* from the list of what
comes from C libraries, which becomes false the day `net` is a runtime part.

**L: every draft I wrote is false on a shape I ran.** `write_file` goes in
place on the four routes the brief names (`runtime/parts/write.c:233`, `:241`,
`:251`, `:259`), **and fails `write_failed` without writing in place on a
fifth**, a copy the device refuses (no space, a quota, an I/O error,
`hero_write_device_refused`), which L1 and L2 read as *in place*. L3 carries the
fifth and is false on a sixth, **measured** (`234-spec-warden-scratch/write/`):
through a link to an ordinary file the file behind it is **replaced** (inode
231473481 to 231473485, the link still a link), where L3 says *in place* for a
path that is not a regular file; behind a second hard link it is written in
place (inode unchanged, the other name reading the new text), as L3 says. A
sentence true on every route costs more than +79, and no reader's program
changes by it.

## 6. Items F, H, I, J, L, M: verdicts, and Principle 0's tickets

**F, where the platform's C lives (spec § 13)**
- **verdict**: **approve** whichever of F1, F2 or F3 matches the route the
  sitting rules, at its price, and **no sentence for a header that does not
  ship yet**: F3 names `hero_net.h` and `hero_time.h`, which are written into
  § 13 in the commit that ships them, not before.
- **section**: design.md §1.6; CLAUDE.md § 12 (*spec beats compiler*: § 13
  must not keep saying sockets come from C libraries once the runtime holds
  them).
- **cost**: F1 +4 real / +2 vendored; F2 +21 / +16; F3 +37 / +29.
- **prediction**: the sentence the landing commit writes costs within 5 real of
  the matching draft above, scored by that commit's `--refresh`.
- **condition**: a route that puts the platform's C in a package (P3, P4) moves
  nothing in § 13, and then any of these sentences is an addition with nothing
  to say.

**H, a C name spelled as a reserved word (issue 1)**
- **verdict**: **veto on Principle 0 for `tag` on an `extern function`**: it
  holds **none of the three tickets**. *Compiler need*: none; the compiler
  compiles itself without one, its C reached through `hero_os.h`'s `hero_*`
  names and the headers of `selfhost/`'s groups. *A measured Part 11 effect*: none.
  *A measured argument*: **measured here, the demand this milestone brings is
  zero**. clang's own declaration dump of the headers the twenty packages bind
  (`stdio.h` to `netdb.h`, `sqlite3.h`, `curl/curl.h`, `openssl/evp.h`, `sha.h`
  and `hmac.h`; `234-spec-warden-scratch/kw/all.c`) holds **no function
  declaration** named with any of the 20 reserved or foreign words a C
  identifier can spell, on this Mac (SDK, Homebrew OpenSSL 4.1.0) and in the
  Linux arm64 image (glibc). It holds **two fields**: `function` in OpenSSL's
  `struct ossl_dispatch_st` (`openssl/core.h`, the API for writing providers,
  not for digesting) and `constant` in glibc's `struct timex`
  (`bits/timex.h`, `adjtimex`), and **no package on the page binds either
  struct**. The function position also has a route today, the shipped
  `static inline` header the compiler prescribes in its own refusal, and a
  `partial` record reaches every field of such a struct but the colliding one.
  **Approve the contextual words' family rule in design.md §4.19** (question
  (vii)), at 0 tokens of the specification; **object** to H2a and H2b in the
  specification: the words already block nothing a reader writes (F11), so the
  sentence buys a reader nothing measurable and has no payment named.
- **section**: design.md §1.0; §1.6; §4.19.
- **cost**: H1 +59 real / +46 vendored; H2a +83 / +70; H2b +32 / +24.
- **prediction**: no package of this milestone needs a C function or field
  spelled as a reserved word, checked at each package's step against its
  header's declarations by the same dump.
- **condition**: a package of the milestone that must call such a function
  where a `static inline` wrapper cannot reach it (a variadic, which no wrapper
  forwards), or must read such a field, gives the form its ticket.

**I, a group's header (spec § 13)**
- **verdict**: **approve I3 with the compiler made per group** (the language's
  own headers counted), the most local route: a reader of one group sees its
  authority, and deleting an unrelated group can no longer break a binding at a
  distance (§1.3). **I1 is the fallback** if the compiler seat prices the
  per-group check past what it buys. **Object to the status quo**: § 13's
  *against that header* is false today on `selfhost/cli/process.hero`'s three
  `hero_dir_*` under `extern "stdlib.h"` (F9), so one of the two must move.
- **section**: design.md §1.3, §4.19; CLAUDE.md § 12.
- **cost**: I3 +5 real / +5 vendored; I1 +8 / +7; I0 0.
- **prediction**: under I3, `process.hero` builds unchanged, the seeds being
  exactly the language's own headers, `heroes_unit.h` and `hero_os.h`
  (`selfhost/emit/externs.hero:86-87`; `math.h` left them with panel 205's R3).
- **condition**: a compiler-engineer's measurement that a per-group check cannot
  be made without a translation unit per group (a cost past §1.12's need) sends
  the sitting to I1.

**J, `alias` (issue 2209, Part 7 item 5)**
- **verdict**: **veto on Principle 0**: none of the three tickets. *Compiler
  need*: the compiler compiles itself with none, and both commits that met
  condition (b) were repaired in one commit by a record and a qualified name.
  *A Part 11 effect*: none measured. *An argument*: **condition (b) does not
  measure what it was meant to.** It counts a refactor's size (8 sites in one
  commit), while Go's warrant, which the condition names, is *gradual* repair: a
  type moved between packages whose users cannot all be changed in the commit
  that moves it. Inside one repository every user can be, so (b) can be met
  with no demand at all, and was, twice. And the one use that would warrant the
  form, an old spelling kept beside the new while users outside the repository
  catch up, is **refused before `1.0.0` by frame 2** (CLAUDE.md § 2: *no old
  spelling kept beside the new, no deprecated alias*). **Recommendation**:
  item 5 stays deferred, (b) struck and replaced by *(b') after `v1.0.0`, a
  type of a core package moved to another package while programs outside this
  repository spell its old name*, (a) and (c) kept; not refused to Part 6,
  since (b') is a falsifier that can come true.
- **section**: design.md §1.0, Part 7 item 5; CLAUDE.md § 2.
- **cost**: J1 +66 real / +48 vendored, gross; panel 135's removals (-43, -6)
  were priced against an older § 4 and are unrun against today's.
- **prediction**: `examples/json`'s value type moving into `encoding/json` at
  step 4 lands in one commit with no alias, its users respelled `json.Value`.
- **condition**: a program outside the repository, after `v1.0.0`, that a type
  moving between core packages would break.

**L, `write_file` and the replace (spec § 11)**
- **verdict**: **object** to any § 11 sentence about replacing. A true one must
  carry four routes in place, a fifth that fails, and the link shapes; three
  drafts cost +31 to +79 real and each is false on a shape I ran (§ 5). No
  reader's program changes by it: the replace is visible only to a reader
  racing the write or surviving a crash. **The route**: the specification stays
  silent (0 tokens; a silence that is a ruling), design.md §4.20 carries the
  routes, and the `os` package's card says what its own functions do. **Goose's
  question (4) is answered by the runtime**: `write_file` already stages,
  flushes and renames where it can (`runtime/parts/replace.c`), so no package
  owes a second whole-file write.
- **section**: design.md §1.6, §4.20; CLAUDE.md § 12.
- **cost**: L0 0; L2 +31, L1 +64, L3 +79 real (+20, +54, +66 vendored), each
  false on one shape.
- **prediction**: a draft true on all six shapes measured costs at least +90
  real, scored by `--refresh` if the sitting writes one.
- **condition**: a program a reader writes that is right with the sentence and
  wrong without it.

**M, `sort_by` (issue 1154)**
- **verdict**: **veto on Principle 0 for `sort_by` as a built-in**: no ticket.
  *Compiler need*: **met without a built-in, measured**: `selfhost/stable.hero`'s
  `order(by: [i64]) -> [i64]`, a stable merge, orders by a key in the 8 modules that
  `use stable`, at 12 call sites outside comments, since defect 105; of issue 1154's four hand-written sorts,
  `ir/verify.hero` and `resolve/cycles.hero` now call the built-in `sort`, a
  third became `stable.order` (defect 363, `resolve/constant_body.hero:35-48`),
  and one insertion sort remains, `selfhost/modules.hero:149-172`, ordering by
  a span's start, an `i64` key `stable.order` takes. *A Part 11 effect*: none. *An argument*:
  panel 137's adoption never met panel 097's condition 5 (F27), and frame 1
  closes the library. **Approve the `sort` package's `by`** (conventions §4.4),
  0 tokens of the specification.
- **section**: design.md §1.0; panel 097 condition 5; the milestone page's
  first paragraph.
- **cost**: M1 +25 real / +19 vendored; the package route 0.
- **prediction**: `sort.by`'s card line costs at most 60 real, scored when the
  `sort` card is written.
- **condition**: a program the closure list needs that `stable.order` and a
  package cannot serve.

## 7. Item K: the repairs design.md owes (documentation, no ticket)

Each verified here at `a22d08938`; 0 tokens of the specification; **approve**
all six.

1. **The `#include` order: done.** design.md:1019-1023 and :2583 (panel 204,
   ratified 2026-10-10). Issue 970's first item closes at the sitting's design
   commit with nothing written.
2. **Part 7 item 4** (:3426), *no `import *`, no aliases, no package
   hierarchy*, should read: *always-qualified references (`list.map`), no
   `import *`; since M-package-layout (2026-09-02, panel 099) a `use` may be a
   path, `use net/http` binding `http`, and `as` renames a binding (`use
   syntax/decl as sd`).*
3. **§4.1** (:896), *there are no aliases and no wildcard*, should read: *every
   module you name needs its own `use`, which `as` may rename (`use syntax/decl
   as sd`), there is no wildcard, and modules may not form a cycle.*
4. **§3.5** (:870-886) should gain panel 056's deliverable B: ***A per-project
   file is examined and deliberately unplaced*** *(panel 056, ratified
   2026-08-15): neither Part 6 nor Part 7. Three conditions, all met, return it
   to a panel, kept on M-package-manager's page: one key, not two, a `prefix
   <dir>` from which `-I` and `-L` are both derived; no string in the file also
   in a `.hero` file, so deleting the file never changes which symbols are
   linked; and one named binding that `package "<name>"` and one command line
   cannot build.*
5. **§1.11's Tier-2 list** (:521-524) should read: *... the program's edges,
   `read_file`, `write_file`, `args`, `args_checked` and `exit`, with
   `validated`, which copies a C string back as a `str?`, written over
   `extern`s against a `hero_os.h` this project ships* (all five and
   `validated` are functions of `selfhost/library_source.hero`, lines 192 to
   271).
6. **§1.11** (:534-535), *the one edge that cannot be Tier 2 is `exit(code)`*,
   should read: *`exit(code)` is Tier 2 as well: the `_Noreturn` C's flow
   analysis needs is `hero_exit`'s, in `hero_os.h`, which the Heroes function
   calls once it has refused a code outside 0 to 255 (task 2250).* And *the spec
   names all four* becomes *names all five, and `validated`*. **A correction to
   the brief, measured**: `exit` became a Heroes function on **2026-08-12**,
   `0503df194` (*M-ffi-ladder step 8: the closure list's last three rows, as
   Tier 2*, which adds `function exit(code: int)` calling `hero_exit` to
   `crates/heroes/src/library/source.hero`), not on 2026-08-18; `7327bd9bf` is
   the day the library's text moved into `selfhost/library_source.hero`. So the
   sentence has been false since 2026-08-12, and F28's date is six days late.

- **verdict**: **approve** the six, repairs 2 to 6 in the sitting's design.md
  commit, repair 1 closed with nothing written.
- **section**: design.md §1.11, §3.5, §4.1, Part 7 item 4; CLAUDE.md § 12 (the
  specification has had `as` since 2026-09-02, so design.md is the one wrong).
- **cost**: 0 tokens of the specification.
- **prediction**: after that commit, `grep -n -E 'no aliases|cannot be Tier 2'
  docs/design.md` prints nothing, and `args_checked` and `validated` each appear
  in §1.11's Tier-2 paragraph.
- **condition**: none; these are documentation, and a repair found wrong is
  corrected underneath with its date.

## 8. Item N: measuring the packages' effect (question (xiii))

**Three facts the design must meet, each read today.**

1. **The held-out suite opens before this milestone can close.** Its README
   (§ When it opens): *within 30 days of the first `v*` release tagged after
   the first held-out run*. The first held-out run was 2026-10-24 (measurement
   052) and `v0.5.0` was tagged 2026-10-25 (`git tag --sort=creatordate`), so
   the keys are committed **by 2026-11-24** and the suite is *a public benchmark
   like any other, burned by construction*; the register records no opening
   yet. Twenty packages will not be done in nineteen days, so **at the close
   the held-out suite is public**, and *a new series needs a new sealed suite*
   (the same section).
2. **No task today can carry a package.** The grader writes the answer alone as
   `answer.hero` in its scratch and builds it there
   (`tests/harness/thesis_grade.hero:149-166`), so an answer writing `use
   core/strings` is `unknown_module`. And a context is any file standing in
   `{{SPEC}}` (`thesis_prompt.hero:21-26`), so the cards need no new prompt
   machinery: a context file holding the specification and then the cards.
3. **A suite the assistant writes never enters a published number**
   (`docs/metrics/tasks.md` § Two suites), and no model reads a held-out task
   (`docs/metrics/writing-a-held-out-task.md`). So the package tasks are the
   author's, and the sessions that write the packages and their cards never
   see them, or the packages are tuned to the test.

**The paired route at the close, weighed against before and after in time.**
A run now and a run at the close would measure everything the milestone moves
at once: the packages, every compiler change, every sentence of the
specification (items F and I alone move § 13), the seeds the batches refresh,
and the held-out suite turning public between the two. **The paired run holds
all of those fixed**: one compiler, one specification, one suite, one day, and
the arms differ in the packages alone. It is the author's *before and after*
read as a counterfactual, and it costs one run where the temporal route costs
two. What it cannot see, an effect of the packages through the compiler (a
near-name for a qualified name added this milestone), it rightly gives to both
arms. **Verdict: approve the paired route as the primary; a temporal run now
is dominated, and if the author wants one for the word *before*, it is
exploratory.**

- **verdict**: **approve** the paired route at the close as the primary, on a
  new sealed suite of package tasks; **object** to a temporal before-run as the
  primary (confounded, above).
- **section**: design.md Part 11 (metrics 2 and 4), §1.2, §1.6 (*the next raise
  rests on what that harness measures a token of specification to buy*; the
  cards are such tokens).
- **cost**: 0 tokens of the specification; the run about 20 USD under a cap of
  40, and the instrument's three changes below.

**The design.**

- **Suites.** **Primary: a new sealed suite of package tasks**, written by the
  author by `docs/metrics/writing-a-held-out-task.md`, 20 tasks and 10 in
  reserve. Each is an ordinary task whose natural program uses at least one
  core package (text, numbers, JSON, SQL, time, paths), naming none, so the
  same `task.md` serves both arms, and each carries two references, one with
  the packages and one without, both graded by `dry` in their arm. **Secondary,
  no harm: the 40 held-out tasks in both arms**, labelled public if opened by
  then, after a `canary-probe` (the current models declare a June 2026 cutoff).
  **The assistant's ten** prove the instrument's new flag and are no number.
- **Arms.** Paired: the context `spec` with no tree beside the answer, against
  a context file holding the specification and the twenty cards, with the tree
  beside the answer. The strict arm alone.
- **Models.** `claude-sonnet-5-5`, the primary as in measurement 052;
  `claude-haiku-5-5`, the weakest, where an invented API is likeliest.
- **n.** Three samples a task, three turns (`green@3`'s horizon), fixed before
  the run.
- **The instrument owes, before any run**: (1) a flag naming a package tree,
  which `dry`, `grade` and `run` copy beside `answer.hero`; (2) `binds_c` kept
  on the answer's text alone, so a package's own `extern` builds while an
  answer's is still refused, the sandbox reaching SQLite in memory and a
  `file://` case for the HTTP client; (3) the spec-and-cards context written
  under `tests/thesis/contexts/` by a command, its digest in the provenance.
- **Budget: a cap of 40 USD** (`--max-dollars 40`, `plan` printing its bound
  first). Arithmetic from what ran: panel 226's variants, Haiku and Sonnet over
  ten held-out tasks, cost 3.44 USD for 180 generations and 262 requests
  (measurement 052), about 0.013 USD a request; this run asks 720 generations
  (240 primary, 480 secondary) and, at about 1.5 requests each with a context
  roughly twice as long, mostly cached, about **20 USD**.

**The prediction, in the shape `preregister` holds a run to** (registered
here as an observation: it buys no sentence, so it pays nothing under §1.6):

- **Primary**: `green@1`(spec and cards, tree beside) - `green@1`(spec) on
  `claude-sonnet-5-5`, strict arm, over the package suite's 20 tasks at three
  samples each; the cluster bootstrap over tasks, 2,000 resamples, the seed in
  the provenance; McNemar's mid-p over the discordant pairs. **The smallest
  effect of interest is 10 points.** `green@1` and not `green@3`, because the
  packages' promise is the first try (a tested function where a parser was
  written by hand, an API shown where one would be invented), and measurement
  052 levelled every pair of arms by turn 3.
- **Outcomes**, as 052's: confirmed if the interval's lower bound is above 0,
  falsified if its upper bound is below 0, inconclusive otherwise, published as
  measured and n never extended after reading.
- **The numbers I expect**: Sonnet `green@1` **+15 points, band +5 to +30**;
  Haiku **+20, band +5 to +35**; `green@3` **+5, band -5 to +15** (the messages
  repay a hand-written parser's compile errors and not its silent ones);
  `silent@3` **with the packages at most without**; turn-1 answers refused
  `unknown_in_module` **at most 5%** with the cards; the held-out secondary's
  `green@3` difference on Sonnet **within ±5 points**, and its `green@1`
  within ±8 (the cards, about as long again as the specification, cost a
  reader nothing it can measure there).
- **The block**: `preregister --sealed <the package suite> --model
  claude-sonnet-5-5 --model claude-haiku-5-5 --context spec --context <the
  spec-and-cards file> --arm strict --samples 3 --turns 3 --compiler
  ./heroes-next` with the new tree flag, printed at the package suite's freeze.
  **Unrun**: the flag and the suite do not exist yet, so no block can be
  printed today.
- **condition**: if the package suite's references show a task the packages do
  not touch, it is replaced from the reserve before the freeze; and a primary
  that reads under 10 points says the cards and the tree buy a reader less than
  a correction round, which item E's budget then answers by shrinking.

## 9. What I could not run, or ran only here

- **The reader test**: its sessions are the coordinator's; my predictions are
  registered above and score when they run.
- **Windows and Linux x86-64**: unrun. The reserved-word scan ran on this Mac
  and in the Linux arm64 image only.
- **A draft's real count off this worktree**: none; every real number above is
  this worktree's `--refresh`, its specification restored and checked after
  each.
- **A `write_file` sentence true on every route**: not written; the six shapes
  are in § 5, and two of them were run.
- **I3's compiler tightening**: unpriced in code; the compiler-engineer's.

## 10. The seat's summary

- `verdict`: **veto** on Principle 0 for three forms: `tag` on an `extern
  function` (H), `alias` (J), `sort_by` as a built-in (M). **object** to *Principle
  0 binds nothing in this milestone* as worded (A), to a contextual-word sentence
  in the specification (H2a, H2b), to any § 11 sentence on replacing (L), to § 13's
  status quo on the group's header (I), and to a temporal before-run as N's
  primary. **approve** F at its route's price, I3 (I1 the fallback), the card
  budget (E), K's six repairs, and N's paired route. Not provisional: every count
  above was measured, vendored and real; the reader-test predictions are
  registered and unscored.
- `section`: design.md §1.0, §1.2, §1.6, §1.11; CLAUDE.md § 2 (frame 2).
- `spec_token_delta`: baseline 10,514 real / 8,019 vendored. Real (vendored):
  F1 +4 (+2), F2 +21 (+16), F3 +37 (+29), H1 +59 (+46), H2a +83 (+70), H2b +32
  (+24), I3 +5 (+5), I1 +8 (+7), I2 +3 (+2), J1 +66 (+48), L2 +31 (+20), L1 +64
  (+54), L3 +79 (+66), M1 +25 (+19). Cards beside the specification: H +762, G
  +759, thin +1,230, thick +923.
- `removal`: F1 removes *sockets* from the list of what comes from C libraries,
  inside its +4. Nothing else is named, and that is a problem for any addition
  the sitting adopts: each still owes a removal or a prediction naming an
  instrument that exists.
- `needed_for_self_hosting`: no. The compiler compiles itself without a `tag` on
  a function, without `alias`, and with `stable.order` and `sort` for its sorts.
- `argument`: The packages cost the specification nothing; what can cost it is
  the forms riding the sitting, and none holds a ticket. The headers the twenty
  packages bind declare no function spelled as a reserved word, on this Mac or
  under glibc. `alias`'s condition (b) counts a refactor's size, met twice with
  no demand, and its real warrant is refused before 1.0.0. `sort_by` is already a
  module, `stable.order`. The live cost is the cards: 759 to 1,230 real each, so
  they get a budget and must pay by §1.2. The paired run at the close isolates
  the packages, and the held-out suite opens by 2026-11-24, so N needs a new
  sealed suite.
- `prediction`: § 1's registered set, scored when the sessions run; the strings
  card pays under H and not under G (§ 4); N's primary +15 points on Sonnet,
  band +5 to +30 (§ 8).
- `condition`: a package step that needs a form; a C function or field a package
  must bind that is spelled as a reserved word; a program outside the repository
  after 1.0.0 that a moved type would break; a reader-test lift that pays a card
  in neither variant.
