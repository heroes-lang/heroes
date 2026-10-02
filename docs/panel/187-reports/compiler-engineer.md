# Panel 187, the compiler-engineer's report

Written as it goes, 2026-10-02 from 22:13 to 23:55 (clock from `date`), cut
by the API's session limit after 22:44 and resumed at 23:10. The frozen
tree is the trunk at `07ccb72a`. My copy is `<scratchpad>/187-compiler-engineer/`
(`git archive 07ccb72a | tar -x`), its compiler built inside it from the seed,
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0, sha256
prefix `60cc49e98b56ddc0` (the brief's); the seed's own prefix
`568bce290b6ed3bb`, 38,648,442 bytes (the brief's). `<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
K below is that compiler.

**Where I built and ran.** My copy and folders of this seat beside it, each
a copy of it (`cp -c -R`, or `tar` with `build/` left out):
`187-compiler-engineer-v1/` to `-v6/` (the builds below), `-f/` (route
(1f) and the budget, on V1's tree), `-g0/` and `-g1/` (my copy and V1's tree
with a bare `git init`, for the two suites that ask git), and
`187-compiler-engineer-work/` for scripts, case copies and outputs. Nothing
was built or run in the trunk, another seat's folder or a worktree. The
critic's script and inputs were copied from `187-critic/` and `p187/`; its
`drop_line` experiment in `187-critic-p166/` was read (its one-line diff and
`build.txt`), not built or run. No paid run.

**The unit.** Every line count is `tests/harness/suite_layout.hero`'s
`code_lines` (lines 697 to 715 at `07ccb72a`: non-blank lines, the lines of a
`test "..."` block left out), reproduced line for line in
`187-compiler-engineer-work/code_lines.py`. The ceiling test is `lines >
limit` (`suite_layout.hero:585`): a file AT its ceiling passes, one line more
is red.

## Verdicts, in one table

| question | route | verdict | what it rests on (built or measured below) |
|---|---|---|---|
| Q1 | (1a) every audit row closed | **object** | an open loop: the front end grew 10,794 lines in four days under it (§ 0) |
| Q1 | (1b) the instrument's totals held by a suite | **object** | 3,618 code lines of Python outside the tree, 13.8 to 15.8 min a run, a ratchet that rewards adding recovery code (§ Q1) |
| Q1 | (1c) the classes alone | **approve, with (1f)**; alone it is a clock | zero compiler lines; an `adjacent` item ages to `blocking` |
| Q1 | (1d) a structural change | **object** | unbuilt; its named instance, the parser told where the lexer joined lines, exists since panel 181 (`parse/apart.hero`); the one structural defect found is a deletion (Q3) |
| Q1 | (1f) what stays pinned in goldens | **approve, built** | 10 pins and V1's repair golden: `check` 426 and 0, `annotations` 585 and 0, `fixes` 681 and 0 |
| Q1 | (1g) a count of exchanges | **object** as the definition | a loop applying certain fixes moves on 2 of the 16 open audit rows |
| Q1 | (1h) the rulings | **no verdict of this seat**; each pin states the ruling it rests on | |
| Q1 | (1e) **the parser's line budget**, a route nobody listed | **approve, built** | `layout/budget`: 5 and 0 at 8,684 of 8,696; red one line under; the net's own tests 198, all passed |
| Q2 | the reworded message naming the `[` | **approve**, unbuilt | `parse/list_line.hero:223-228`, 199 lines, its `separator` already takes the opener (`open`) |
| Q3 | the local route (V1: `drop_line` deleted) | **approve, built and gated** | full net 4,752 and 0 over 26 suites; own tests 1,056; census 0 of 1,834 move |
| Q3 | the stop narrowed to a tab margin (V2) | **object** | V1's reading everywhere but two control-arm probes, where it keeps one debris message |
| Q3 | the parser told where the lexer joined, as new architecture | **object** | already built (`apart.hero`), and the stop was the one place that split a joined line |
| Q4 | `g2/r09`'s second message false, `blocking` | **object** | true where it stands, the convention 8 goldens bless; it names no function: `adjacent` |
| Q4 | `g2/r09` repaired by naming the function (V5) | **approve, built** | 11 lines in `parse/heads.hero`, 1 in `parse/tails.hero`; 24 goldens move in words only |

No route adds a core construct (design.md Part 5's seven; the recovery is
the lexer's and the parser's, erased before the checker sees anything), so
**this seat exercises no veto**. What I hold every route to is §1.1, *"It
sets the ceiling. If we exceed it the compiler never gets finished"*, and
§1.7, *"it determines the size of your compiler"*.

## 0. The recovery's files, counted

| file | lines (the unit) | its ceiling |
|---|---|---|
| `selfhost/parse/opening.hero` | **300** | 300 (CEILING): no line of room |
| `selfhost/grammar_expr.hero` | **1085** | 1085 (`DECIDED`): no line of room |
| `selfhost/open_line.hero` | 298 | 300 |
| `selfhost/cursor.hero` | 297 | 300 |
| `selfhost/parse/heads.hero` | 260 | 300 |
| `selfhost/parse/headless.hero` | 269 | 300 |
| `selfhost/line_joins.hero` | 254 | 300 |
| `selfhost/lexer.hero` | 247 | 300 |
| `selfhost/parse/statement_end.hero` | 243 | 300 |
| `selfhost/parse/apart.hero` | 241 | 300 |
| `selfhost/parse/tails.hero` | 203 | 300 |
| `selfhost/parse/list_line.hero` | 199 | 300 |
| `selfhost/parse.hero` | 187 | 300 |
| `selfhost/parse/swallowed.hero` | 144 | 300 |
| `selfhost/line_above.hero` | 137 | 300 |
| `selfhost/sign_above.hero` | 110 | 300 |
| `selfhost/parse/top_level.hero` | 100 | 300 |
| `selfhost/bracket_reach.hero` | 81 | 300 |
| `selfhost/join_fix.hero` | 28 | 300 |

**Any repair that adds a line to `opening.hero` or `grammar_expr.hero` first
moves code out of it.**

**The whole, and what the cluster added.** The compiler is 380 modules,
72,050 lines (`find selfhost -name '*.hero'`). The front end, measured as the
transitive `use` closure from `selfhost/parse.hero`
(`187-compiler-engineer-work/frontend-ce.py`, each module read with `git -C
<trunk> show <commit>:<path>`), was **40 modules and 8,407 lines** at
`0338b598` (2026-09-28 18:58, defect 130 filed), `parse/` 10 modules and 2,403
lines; at `07ccb72a` it is **109 modules and 19,201 lines**, `parse/` 54
modules and 8,696. **It grew 10,794 lines in four days.** By `git log
--no-merges --numstat 0338b598..07ccb72a` over those 109 files (raw lines,
tests and comments included): net +15,559 over 87 commits, and the commits
whose subject starts *Defect 130* to *Defect 133* account for **+11,749** of
it (131 +3,931; 130 +2,759; 132 +2,029; *130 and 131* +1,470; *133 and 130*
+929; 133 +631). The brief's scale for one person is Pascal-P4, about 4,000
lines for a whole compiler: the front end's growth in four days is about 2.7
of it.

## 1. Each open row's code path, measured

**The instrument.** V4 (`187-compiler-engineer-v4/heroes-v4`, prefix
`f944ac807fac1f7f`): every `message: ` argument in `selfhost/` outside
tests prefixed with the `file:line` that writes it
(`187-compiler-engineer-work/tag-sites.py`, 339 sites). Nothing in the
compiler decides on a message's text (`grep '\.message'`: two modules
rewrite one, `indent_runs.hero:255` and `parse/records.hero:184`, the rest
render or test), and the control holds: **538 file-and-arm pairs read the
same codes and places on V4 as on K, 0 differ** (`sites-ce.py`, every case
and probe, both arms). A (b) row's hidden mistake has no message to tag, so
V6 (`-v6/heroes-v6`, prefix `1c3193ea6574a0a2`) reports, at each of the 33
call sites of the drop primitives (`cursor.skip_line`, `drop_rest_of_line`,
`recover_to_next_decl`, `balanced_block`, `brace_habit.pass` and its
`drop_rest_of_line`), the `file:line` that called it and the bytes it
consumed; its codes and places equal K's on every row run
(`drops-ce.py`). `cursor.hero:263` is the generic `cursor.error`,
`cursor.hero:232` the generic `expect`; the site named is the specific one.

| cause | rows | the extra message, or what drops the hidden one |
|---|---|---|
| A1, C's three-clause loop header | 131-33a, 54a, 54b (the instrument's `c-for`, 52 EXTRA) | the lexer tells each `;` (`scan.hero:299`) and the loop habit the `for (` (`parse/loop_habit.hero:58`) |
| A2, a line a recovery hands on, told again by `primary` | 131-55a, 55b, `g1/a11` | `grammar_expr.hero:429` after `parse/broken_arm.hero:55` (the arm's `=>` two dedents out) or after `parse/top_level.hero:82` (a block under `use`) |
| A3, the colon habit told once a line | 131-56a, 56b | `parse/colon_habit.hero:98`, then the next head's `:` named a missing body at `parse/opening.hero:274` |
| A4, a closer the lexer pairs with another opener | 131-41a | `closers.hero:151` twice, then `parse/type.hero`'s two `expect`s |
| A5, `drop_line`'s stop | 166's four | `parse/opening.hero:310` (Q3) |
| B1, a head's line that goes on is told once | 131-16a, 16b, 53a, 53b | `skip_line` called at `parse/members_below.hero:87` drops 16a's and 16b's `)`; at `parse/opening.hero:192` it drops 53a's and 53b's `: i64 )`. The fork, measured on the follow-ups: with a member below the `)` is told at `members_below.hero:202`, without one `empty_variant` and `empty_record` at `:263` and `:252`; 53a's `:` is told at `opening.hero:218` and the `)` only once `->` is written |
| B2, a failed head's line dropped as debris | `g4/ti` | `skip_line` at `parse/opening.hero:147` drops the `)`; with the operand written it is told at `:218` |
| B3, a function among a variant's cases dropped | `g2/r12` | `skip_line` at `parse/member_lines.hero:205` drops `function f()`, `balanced_block` at `:202` its body |
| B4, a line past an inner closing brace | 131-32 | `brace_habit.pass` at `parse/braced_lines.hero:154` passes the function's braces after its lines are read, `} while (1 == 1)` with them; `heroes lex --dump-tokens` shows the lexer hands the parser every token of that line |
| C1, the missing body told at the next line, unnamed | `g2/r09` | `parse/heads.hero:192` (`expected_field`), then `opening.hero:274` (Q4) |

So **eight (a) rows are four causes, five (b) rows are two** (B1 holds four of
them, B4 the fifth), and each of the six shapes belongs to one cause: r12
B3, r09 C1, ti B2, a11 A2; `hc/h09` and `g1/b07` are (f), one message per
mistake. 130-34a is `parse/list_line.hero:227` (Q2). 131-22's open half is
its control arm, which no golden form runs (its normal arm's two messages are
`open_line.hero:256` and `cursor.hero:232`), and
131-52a three messages for three edits (`layout.hero:181`,
`parse/members_below.hero:76`, `parse/member_lines.hero:322`).

## Q3. Defect 166's cause: the local route, confirmed on `07ccb72a`

### What the stop is, and what it was for

`drop_line` is `selfhost/parse/opening.hero:305-314` on `07ccb72a`; the
condition the critic made never true is line 310, as on `62d65e48`:
`if c.pos != from && head_lines.margin_width(text, at: cursor.current_span(c).start) >= 0`,
which stops the drop at any token that begins its line of the TEXT, whether
or not a line end stands before it. Its one caller is `grammar_expr.hero:1156`,
the `.err` branch of `arms_of` (a failed arm), and it was the one caller when
the stop came in (`git show 9811d4cd:selfhost/grammar_expr.hero`, line 1127).
Without line 310, `drop_line` is `cursor.drop_rest_of_line` token for token
(the same guard, `c.pos == from || !at_fresh_line(c)`, and the same loop,
`skip_line`).

- **By its own comment** (`opening.hero:301-304`): *"never into the next line
  of the text: after an `=>` or an `else` over a line whose margin holds a tab
  the lexer puts no line end, and dropping to the next one took that line
  too."*
- **By its commit**, `9811d4cd` (2026-09-29 03:07, defect 131's second round):
  *"After an `else` or a failed arm's `=>`, where the lexer puts no line end
  before such a line, it is no longer dropped with the line above
  (`opening.drop_line`)."*
- **By the golden it came with**,
  `tests/golden/check/fixedbugs-131-a-body-in-a-tab-margin.hero`, the shape
  `under_a_failed_arm` (lines 73 to 78), `+ =>` over a tab-indented `y = 3 )`,
  whose `)` must be told. **The golden's own dated note says the premise has
  gone** (lines 92 to 101, *"Read again 2026-10-01, lane recovery-b5 (ruling
  5, `selfhost/indent_runs.hero`)"*): the lexer lays a tabbed line out at the
  level its fix writes, so *"the tabbed bodies here reach the parser indented
  and `opening.tabbed_line` is not asked for them"*. Measured below: the stop
  fires on no tracked file, that golden included.

### What I built

- **S0**, the control: my copy's own `selfhost/` built by K, `heroes build
  selfhost/main.hero -o heroes-s0`, exit 0, prefix `e3731193b1fd5e64`, so a
  difference between a variant and K comes from the sources.
- **V1, the local route in its clean form**: `drop_line` and its four comment
  lines deleted from `opening.hero`; its one call site becomes
  `.err => cursor.drop_rest_of_line(@c, from: from)`, its two comment lines
  rewritten in two (*"... and the rest of its line goes, a line joined to it
  too (130, 166)"*); `sign_above.hero:23-26`, the one other place that names
  `opening.drop_line`, put in the past tense. Behaviourally it is the critic's
  `c.pos < 0`. `heroes fmt --in-place` changed none of the three files. Built,
  exit 0, prefix `d2e4d2b61cc6570f`. **34 changed lines** in `selfhost/`.
- **V2**, the stop narrowed to what its comment says it is for, line 310
  asking `head_lines.margin_has_tab(...)`. Built, prefix `137bd3bd73aa2829`.
- **V3**, the stop unchanged and a `ZZSTOP <line>:<col> tab= prev= here=` line
  on stderr where it fires. Built, prefix `f48925a709793314`.

### What I ran, and what it read

1. **The critic's 147 files** (`rerun-ce.py`, its script with my five
   compilers, every file copied into `187-compiler-engineer-work/cases/` first;
   22:25:06 to 22:25:27): **143 read the same on all five; the 4 that move are
   166's `n1`, `n2`, `n3`, `n9`**, under V1 and V2 alike, each from
   `expected_pattern@9:9` twice to once, the parse exit and what `--apply`
   writes unchanged. S0 and V3 read as K on all 147. V3's stop fires on five,
   every time at a line the lexer joined (`prev=pipe`, `eq_eq` or `minus`,
   `tab=0`), `n5` among them without moving it.
2. **The census**, `check --brief` and its exit code, normal arm and
   `--permissive`, over every `.hero` file of the copy outside `archive/`
   (1,834 files, the tracked ones at `07ccb72a`), K against V1, V2 and V3,
   three processes at a time (`census-ce.py`, 14,672 runs, 22:26:16 to
   22:28:31): **0 of 1,834 move in either arm under V1, V2 or V3, and V3
   prints no stop on any of them.** K's exits: 895 at 0, 938 at 1, 1 at -6 in
   the normal arm; 1,010, 823 and 1 under `--permissive`. The -6 is
   `docs/panel/184-briefs/blind/task3a.hero`, *panic: stack exhausted in
   checkwalk.synth*, on every compiler: defect 169, filed `blocking` at
   `07ccb72a` (`docs/work/DEFECTS.md:973`), the checker's.
3. **The shapes beside it**, 61 probes I wrote (`probes/`, `probes2/`) and the
   critic's 23 in `c166/`: a failed arm's line running into the next line of
   the text with no line end between, by a join (`|`, `==`, `,`, `-`, a chain,
   a comment after the operator, a comment or blank line between, the last
   arm, the end of the file, CRLF), by an open bracket (a call, group, list or
   brace pattern, a `match` inside a call or a list), and by a tab margin
   (twelve shapes). **No tab-margin shape fires the stop** unless its line is
   joined too (`t09`, `h18`). Every message, both arms, classified
   (`subset-ce.py`; a duplicate is a message K says twice at one place):

| arm | files that move | duplicates removed | other messages no longer said | messages added |
|---|---|---|---|---|
| normal (`heroes check`) | 23 | 18 | 7, every one `expected_arm_arrow` at the `)` of a call or list written as a pattern across lines (`b09`, `h07`, `h08` twice, `h09` twice, `h15`): debris of the one pattern already refused | 0 |
| control (`--permissive`) | 34 | 20 | 16: the same 7; 2 `expected_end_of_line@9:5` below a join into a tab margin (`t09`, `h18`); 2 `expected_pattern@9:9` after a failed `==` (`j09`, `n5`); and **5 true messages**, `expected_expression` in the body of a joined arm whose pattern failed (`h01`, `h05`, `h14`, `h16`, `h17`) | 0 |

4. **The compiler's own tests**: V1's sources **1,056 tests, all passed**; my
   unchanged copy 1,056, all passed.
5. **V1 gated as a lane is**: its seed regenerated (`heroes-v1 build
   selfhost/main.hero --emit-c -o seed/heroes.c`, exit 0, 38,642,537 bytes,
   5,905 fewer, prefix `8e697bc0382243ae`); the compiler built from it by
   clang, prefix `45ca8514744beb95`, which reads `n1`, `n2`, `n3`, `n9` byte
   for byte as `heroes-v1` does; **the fixpoint by `cmp`, exit 0**. My
   unchanged copy's fixpoint by the same commands: `cmp` exit 0.
6. **The full net** in V1's tree with that compiler, alone, held awake by
   `caffeinate -i`. The first pass (22:39:42 to 23:08:19): sixteen suites,
   each 0 failed (check 415, ir 24, emit 8, unsupported 119, run 242,
   annotations 574, determinism 272, emission 700, descriptors 334, wholes
   334, cache 7, units 3, fixes 670, lines 243, corpus 55, warnings 303), then
   *"harness: the record checks could not run"*, exit 2: `records` asks `git
   ls-files` (`tests/harness/shell.hero:375-390`, *"there is no sound
   fallback"*), a `git archive` copy has no `.git`, and the harness stops at
   the first suite that cannot run (`tests/harness/main.hero:336-340`), so the
   nine after it never ran. **Re-run by name, alone, same tree and compiler**
   (23:11:33 to 23:19:23): surface 342, special 10, spec 20, grammar 9, layout
   4, canonical 2, probe 27, order 3, runtime 8, each 0 failed. **`records` in
   `-g1/`** (V1's tree, a bare `git init`, nothing added: `git ls-files
   --others` lists its 6,981 files) and in `-g0/` (my copy the same way): 24
   passed, 0 failed, the two outputs identical. **So V1 reads 4,752 passed
   and 0 failed over 26 suites, each suite's count equal to the head gate's
   last run** (`git show -s 6bec7c8c`: *"At each suite's last run, 4,752
   passed and 0 failed over 26"*). **The net's own tests**: 197, 1 failed in
   V1's git-less tree (*"a dead citation is caught..."*, at
   `!excused["site/dist/nosuch.html"].is_err()`), and 197, all passed, in both
   `-g0/` and `-g1/`: the missing `.git`, not V1. No line any record cites
   moves (`grep` for `opening.hero:NNN` and `sign_above.hero:NNN` finds none).
   **The brief's own instruction cannot run whole in a `git archive` copy.**

### Why the normal arm loses nothing and the control arm does

**The parser is already told where the lexer joined lines.** A file whose
lexer refused a break is parsed twice since panel 181 item 2 and defect 132,
joined and apart (`selfhost/parse/apart.hero:14-19`, `lexer.lex_apart`), and
`apart.weigh` keeps what the lines earn apart and holds what the join alone
causes. So in the normal arm the lower line's own mistakes are told by the
apart reading: `h01` (`x |` over `2 => f(1 +)`) reads
`continuation_outside_brackets@8:11 expected_pattern@8:9 expected_expression@9:19`
on K and on V1 alike. **The stop is the one place where the joined reading
splits a joined line in two**, and its double is the apart reading's message
said a second time.

Under `--permissive` the program IS the join (design.md line 1967 at
`07ccb72a`: *"`check --permissive`, where the code is a thesis rule, reads the
join"*). There V1 reads `x |` over `2 => f(1 +)` as it reads the one-line `x |
2 => f(1 +)` (`h02`), `expected_pattern@8:9` alone; K told the `1 +` only
because the stop read the lower line as a new arm. The one-line twins `h02`
and `h06` hide that mistake under every compiler in both arms. That is a (b)
of its own, *an arm whose pattern failed hides its body's mistakes*, whose
cause is `arms_of`'s `.err` branch and not the join: filed apart, `adjacent`.

### V2 against V1

V2 reads as V1 on all 147 files, on all 1,834 of the census in both arms,
and on every probe's normal arm. Its control arm moves 32 files where V1's
moves 34: the two it keeps are `t09` and `h18`, the joins into a tab margin,
where it keeps K's debris `expected_end_of_line@9:5`. So V2 keeps four lines
for one debris message on two probes.

## Q1. What a finished recovery is

**The ceiling's reading of the evidence.** Seven batches of "every row closed"
took the front end from 8,407 to 19,201 lines and left `opening.hero` and
`grammar_expr.hero` without a line of room (§ 0), and the batches' growth is
three quarters the cluster's own commits. A definition of done that the
parser can only approach by growing is not a definition; one that lets the
parser shrink while it gets better is. Q3 is the proof that the second kind
exists: a deletion that removes 25 messages from the normal arm and adds
none.

- **(1a) object.** It is the loop the author's *D1a D2a D3a* ruling ended,
  and its price is § 0's growth. Nothing in it bounds the next shape.
- **(1b) object.** The instrument is 3,618 code lines of Python (non-blank,
  non-comment, `<scratchpad>/instrument/tool/*.py`, ten modules, 4,228 by `wc
  -l`, `ops.py` alone 1,647), about a whole Pascal-P4; `heroes mutate`, its
  in-tree home, is 1,366 lines in seven modules (`selfhost/cli/mutate.hero`,
  `selfhost/mutate/*.hero`) and counts killed and survived. A port is unbuilt
  and unpriced; in `tests/harness/` it costs the compiler nothing and the net
  13.8 to 15.8 minutes of wall a run at `--jobs 3` (F4), beside a net whose
  first sixteen suites ran in 28 min 37 s of wall in V1's tree and the nine
  after them in 7 min 50 s (from `date`, a busy machine, not a timing). Its corpus is a snapshot of 641 programs or a moving
  tree. And **a ratchet on ONE and EXTRA rewards adding recovery code**,
  which is exactly § 0's direction. As a measurement at a recovery batch's
  gate, written in its closing commit, as the lanes ran it: approve.
- **(1c) approve, with (1f)**: zero compiler lines, the classes exist and
  `records/tagged` executes them. Alone it refiles twenty rows with a clock,
  two tags each, after which they return as `blocking` and are repaired by
  the growth (1a) is.
- **(1d) object.** Unbuilt and unpriced, and its one named instance exists:
  the parser has been told where the lexer joined lines since panel 181
  (`parse/apart.hero`, 241 lines, `lexer.lex_apart`, `parse/joined_lines`,
  `parse/readings`, `parse/which_arm`). What I measured is one exception to
  that architecture, removed in Q3. A wholesale resynchronisation of a
  19,201-line front end is a milestone, not a sitting's resolution.
- **(1f) approve, built** in `187-compiler-engineer-f/` (V1's tree): ten
  pins in `tests/golden/check/`, one per cause of § 1, each function one row
  and each diagnostic annotated from the row's own measured output
  (`pins-ce.py`): `panel-187-a-c-style-for-header-is-told-by-the-lexer-and-by-the-loop`,
  `-a-line-a-recovery-hands-on-is-told-again-by-the-expression`,
  `-the-colon-habit-is-told-once-a-line` (its certain fix checks clean, so it
  carries a `.fixed`), `-a-closer-the-lexer-pairs-with-another-opener`,
  `-a-heads-line-that-goes-on-is-told-once`,
  `-a-failed-heads-rest-is-dropped-as-debris`,
  `-a-function-among-a-variants-cases-is-dropped`,
  `-a-line-past-an-inner-closing-brace-is-dropped`,
  `-a-closer-of-another-kind-inside-a-list-is-one-message` (130-34a, Q2's two
  readings in its header) and `-a-variants-cases-in-braces-cost-three-edits`
  (131-52a, ruling 1's reading in its header); and V1's own repair golden,
  `fixedbugs-166-a-line-joined-under-a-failed-arm-is-dropped-with-it` (six
  shapes, an `.applied`). Every line's codes agree with its annotations; the
  ten pins read byte for byte the same on K and the 166 case does not (it
  pins the repair). In that tree: **`check` 426 passed, 0 failed;
  `annotations` 585 and 0; `fixes` 681 and 0; `canonical` 2 and 0.** Cost:
  no compiler line; 233 lines of cases. **Its measured limit**: no golden
  form runs `--permissive` (`suite_golden.hero` has no such word), so a
  control-arm row (131-22's open half, V1's five control-arm hides) cannot be
  pinned today.
- **(1g) object as the definition**: of the 16 open audit rows, 2 carry a
  `certain` fix (131-56a and 56b, `trailing_colon`), so a loop applying what
  the compiler certifies stops on 14 at the first exchange. The blind seat's
  one-turn rate is the ergonomist's evidence.
- **(1h)**: no standing of this seat. Each pin names the ruling it rests on,
  so ratifying the pins ratifies those readings in the open.
- **(1e) the parser's line budget, approve, built.** The executor `layout`
  already has for one file (`DECIDED`) held one level up:
  `tests/harness/suite_layout.hero` gains `BUDGETS`
  (`"selfhost/parse/ 8696"`, the head's own total), `over_budget` and the
  check `layout/budget`, 69 lines added (48 in the unit) and one test. In the
  `-f/` tree: **`layout` 5 passed, 0 failed** (8,684 of 8,696, V1's 12 lines
  of room); with the row at 8,683, **`layout/budget` fails**, *"selfhost/parse/:
  8684 lines of code, past its budget of 8683"*, exit 1; with V5's
  `heads.hero` copied in, 8,695 of 8,696, green. **The net's own tests 198,
  all passed.** A repair of an `adjacent` row pays for its lines by deleting
  or moving others; a `blocking` repair that cannot raises the row with its
  reason, as `DECIDED` rows are raised. Its premise, written in the constant's
  comment: a module moved out of `parse/` escapes it; the front end's `use`
  closure would not, at the cost of a module walk in the harness, unbuilt.

**The route I adopt, built**: Q3's deletion first; every open row repaired
under the budget or pinned in a golden with its reason; each pin's cause one
`adjacent` item; items 130 and 131 closed into them. Done is checkable today:
no audit row without a golden, no (c), (d) or (e) row standing, `layout/budget`
green.

## Q2. Row 130-34a

`x = [1, 2` over `print(x) )`: `expected_separator@3:14` is written at
`parse/list_line.hero:223-228` (V4: `cursor.hero:263` through
`list_line.hero:227`), C3's closer rule; with the `]` written it is
`statement_end.hero:103`'s `expected_end_of_line`, with the `)` deleted
`closers.hero:169`'s `unclosed_bracket`. **The reworded route is within
reach and cheap**: `separator` already takes the opener's position (`open`,
its caller at `grammar_expr.hero:573` passes `open: at`), so naming the `[`
at 2:9 and both edits is a message change in a file of 199 lines with 101 of
room, and no line in `opening.hero` or `grammar_expr.hero`. Unbuilt: that is a
reading of the signature. Approve it as the one message true under both
readings; a second message for the `[` would add one under reading (B).

## Q4. What becomes of what stays open, and `g2/r09`

**`g2/r09`'s second message, measured.** `heads.next_member` tells the
`function` among the fields (`parse/heads.hero:189-194`), reads its head
(`head_read`) and returns `Owed(owes: .body, owner: "a `function`")`
(`:206-207`); `grammar_expr.declarations` reads that owed body
(`grammar_expr.hero:795`, `block`), `opening.body_follows` finds the record
block's dedent and `opening.absent` names the body missing there
(`opening.hero:260-276`), at the dedent's span: the first column of the next
non-blank line, `5:1`, whose excerpt is `function main()`. **The place is the
convention of every missing body** (`opening.hero`'s table, row one, *"named
missing at the next line"*), the same at the top level
(`r09_function_moved_to_top.hero`: `missing_body@6:1`, *"(found
`function`)"*, under `function main()`), and blessed by golden cases: of 109
`missing_body` lines in `tests/golden`, 47 begin *"a `function` needs"* and 15
of those are told at the next declaration's first word, in 8 cases of
defects 130, 131, 133 and 135 (`fixedbugs-130-past-a-comment-line`,
`fixedbugs-133-a-bodiless-head-above-a-foreign-word` and six more). Every
clause of the message is true where it stands; it names no function. That is
the `adjacent` class's own *"a true message less exact than it could be"*; were
it false, those 15 golden lines would be `blocking` too.

**Naming it is within reach, built** (V5, `-v5/heroes-v5`, prefix
`c849d3df7b728b17`): where a function's head was read whole, its owner names
it (`heads.owed_owner` and `heads.function_named`, `parse/heads.hero` 260 to
271; the one literal in `parse/tails.hero:150` replaced, 203 to 203). It
reads *"the `function` `f` needs an indented body -- one level deeper, exactly
4 spaces (found the end of the block)"* in the record and *"(found
`function`)"* at the top level. Its price: **24 of the 415 `check` goldens
move, in words only** (the junk message reads *"the `function` `a_function`'s
body goes on the lines below its head"*; codes and places stand), and one unit
test asserts the old owner (`heads.hero`'s *"a block of declarations is read
line by line..."*). The language refused my first two drafts, a parameter
`head` shadowing `heads.head` and an unlabelled second `str`, and wrote the
certain fix for the second. Under the budget it fits after V1, 8,695 of
8,696. **None at all** is not a repair: moved out, `f` is still refused for
its body, so holding the message would be a (b).

**The filings**: one `adjacent` item per cause of § 1, A1 to A4, B1 to B4
and C1 (C1 repaired by V5), each carrying its pin; 166 closes with V1 and its
golden; the (b) of Q3's control arm (an arm whose pattern failed hides its
body's mistakes, `h02`) filed apart; items 130 and 131 closed into the pins.
Defects 177 to 182, filed beside the sitting, are of the same kinds and owe
the same treatment, unmeasured by me.

## Cost, per route built

| route | compiler lines (the unit) | other | build time |
|---|---|---|---|
| Q3, V1 | `parse/opening.hero` 300 to 288 (-12), `grammar_expr.hero` 1085 to 1085, `sign_above.hero` 110 to 111; 34 changed lines | the seed -5,905 bytes; one repair golden | removes work per failed arm; the four timed builds (S0, V1, V2, V3) read 77.2 to 77.8 s `user`, two at a time on a busy machine, so no difference is readable |
| Q4, V5 | `parse/heads.hero` 260 to 271, `parse/tails.hero` 203 to 203 | 24 `.expected` re-read, one unit-test assertion | no work added per parse |
| Q1, (1f) | none | 11 cases, 233 lines, 11 `.expected`, a `.fixed`, an `.applied` | the `check` form +11 cases |
| Q1, (1e) | none | `tests/harness/suite_layout.hero` +69 lines (686 to 734 in the unit), one test | one more read of 54 files in `layout` |

## Prediction

**At the next `m-*` tag after this sitting, `selfhost/parse/` measures at
most 8,696 lines in `code_lines`' unit if the budget is adopted; if it is not,
more than 9,000.** Checkable with `187-compiler-engineer-work/code_lines.py
selfhost/parse/*.hero` on the tagged commit. A second, at the coordinator's
instrument run on V1 (below): **every total of both arms within 3 mutants of
this head's run.** Its reason is a question rather than a premise: I did not
read the 96 operators, and the guess is that few of them write a failed arm
whose line the lexer joins or whose pattern opens a bracket.

## Condition

My verdict on Q3 changes if the instrument's run on V1 shows a parse-stage
HIDDEN pair the head's run does not have, or EXTRA above 439; or if a program
is found where a tab margin, and no join or bracket, fires the stop on K
(V3's instrument gives the search, and 84 probes and 1,834 files found none).
On (1e), if the author reads the budget as a cap a `blocking` repair cannot
pass, I withdraw it: robustness outranks compiler size (CLAUDE.md §
Precedence, rank 3), and the row exists to be raised with a reason. On (1b),
the evidence that would move me is a port that runs inside the net's present
wall time and ratchets a rate on a frozen corpus.

## Asked of the coordinator: one instrument run

Lane recovery-b6's gate plan, 13,594 mutants, 13.8 to 15.8 minutes of wall
at `--jobs 3` (F4), on V1's seed-built compiler
`<scratchpad>/187-compiler-engineer-v1/heroes` (sha256 prefix
`45ca8514744beb95`), compared with this head's run (`<scratchpad>/inst-187/round3/`):
every total in both arms, the APPLY flags, and the per-operator EXTRA. V5
moves words only and is not worth a run.

## Not run

The census of `check` against V5, the formatter's probe by hand (no
`selfhost/print/` file moves), Linux arm64, Windows, the site's build (no
file `claims.ts` reads moves); V5's and the pins' trees through the full
net; Q2's reword. Each `-g` tree has a bare `git init` with nothing
committed, so `records` there judges with no tag and no history, the same
for both trees.
