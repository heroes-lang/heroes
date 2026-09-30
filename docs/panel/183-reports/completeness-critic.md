# Panel 183, completeness critic, second pass: the reports

Written 2026-09-30 from 09:40 to 11:00 by the completeness critic, after every
seat and before the synthesis. I read the nine briefs (the two clean blind
briefs included) and the eight reports, then ran what they left unrun. Every
number below was run by me in this pass unless the sentence says whose it is,
and each finding names its command and what it printed. `<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`;
my work is under `<scratchpad>/183-critic/_critic/p2/` (`<p2>` below), the
model sessions under `<scratchpad>/183-critic-rep/`. I read the seats'
directories to copy their patches and probes into mine and wrote nothing there.
Nothing here is a duration: a virtual machine, a backup and my own runs held
the machine at load 25 to 77 throughout.

## The findings, in the order they weigh

1. **The tree under the rule moved during the sitting** (§ 1). Batch 3's
   52b2d378 (06:58, on the trunk at 09:38) repaired the `wrong-closer` class the
   seats measured as unrepaired, removed `cursor.recover_past_closer`, the parser
   half the engineer would ratify, and adds 128 `missing_body` on single missing
   closers that 171e8c45 does not print (the example I read: four, each on a
   body that exists). Both predictions that name
   `wrong-closer` at 68 of 159 are false for the trunk; the instrument's own
   denominators also move with the machine's load.
2. **PE, PEL and proto-b4 are one compiler on (b)'s own class and three on
   everything else** (§ 2). Planted 591 times (the full instrument drew the class
   6 times), the three name the same openers and tell the same stray closers.
   They differ on the `extern` member line, `break`, a declaration at column 4,
   labels and the margin they compare. On every measure run here PEL is equal to
   or ahead of PE, and proto-b4 is behind both while costing the most lines.
3. **The ffi-pragmatist's objection condition is met** (§ 3.2). libsodium, a
   §1.11 library, names a parameter `variant` in three functions; the landed rule
   itself prints a false *never closed* on it at column 0, on the trunk today,
   and only PEL's label test removes it.
4. **The engineer's design.md sentence describes proto-b4's margin, not PE's
   code** (§ 3.3), shown by one program.
5. **Both clean blind readings' repair predictions are false by their own
   clauses** (§ 6): 80 fresh sessions compiled in one turn under every output,
   on the sitting's two programs and on five real mutants.
6. **A route nobody listed** (§ 5.1): the spec's own NEWLINE sentence (lines 11
   to 15) ends a reach inside a `(` at a name-led line. Probed on PEL, it moves no
   tracked file and takes (b)'s hidden pairs from 127 to 66, where no word list
   can reach. And the largest group no compiler reaches, 165 of 591, is the head
   class the spec-warden asked about and nobody answered (§ 5.2).
7. **The precedents, run** (§ 7): Swift ends a list at a statement word only
   where a separator was due, and no compiler here bounds by margin; Go's
   semicolon rule is the precedent for § 5.1.

## The compilers

| name | what it is | its provenance, run |
|---|---|---|
| `OLD` `NEW` `RULE` | c85bccb8's seed, 171e8c45's seed, 41807577's `selfhost/` built by `OLD` | my first pass |
| `CTL` | 171e8c45's `selfhost/` unpatched, built by `NEW` | prints `NEW`'s bytes on all 392 probe outputs (`diff -rq`, 0 lines) |
| `PE` | + the engineer's `pe-full/selfhost/{layout,next_line}.hero` | `cmp` silent against the ffi-pragmatist's `PE` |
| `PEL` | + the ffi-pragmatist's `pel/selfhost/` two files | differs from its `PE` in `next_line.hero` only (`diff -rq`) |
| `B4` | + the spec-warden's `proto-b4/selfhost/` two files | `diff -rq` against 171e8c45's `selfhost/`: those two files only |
| `TRUNK` | 77b8ca98's seed (`clang`, `real 4.15`) | the trunk at 09:38; 6d781be1 (09:59) touches nothing under `selfhost/` (`git diff --stat`, empty) |
| `TPE` `TPEL` `TB4` | 77b8ca98 plus the same two files, built by `TRUNK` | `git diff --quiet 171e8c45 77b8ca98 -- selfhost/layout.hero selfhost/next_line.hero` exits 0 |
| `PELN` `PELNi` | `PEL` plus a clause of mine (§ 5.1), built by `NEW` | a probe of a route, not a proposal |

Every build exited 0 (`build.log` in each tree under `<p2>`).

## 1. The ground moved under the sitting

**1.1 The class the sitting called unrepaired was repaired while it sat.** The
coordinator's lane queue, `<scratchpad>/lane-recovery-b3-items.md` (04:52,
before the briefs), holds B1, *"`wrong-closer` 68 of 159"*, and says of B2,
question (b)'s class: *"Not this batch ... before panel 183"*. Lane recovery-b3
repaired B1 at 52b2d378, 06:58:20, *"past a closer of another kind the parser
resumes where the lexer closed the bracket, and an extern group whose header
failed has its signatures read"*, and the trunk took it in 77b8ca98 at 09:38:08
(`git log --format='%h %ad %s' --date=iso 171e8c45..HEAD`). The seats measured
171e8c45 as briefed; the spec-warden's report closed at 06:26, the historian's at
06:14, the engineer's at 07:06, the ffi-pragmatist's at 09:27 (file times). The
spec-warden's *"no open item names it"* was true of `DEFECTS.md`, `DECIDE.md`
and the milestone files it grepped; the item was in a queue no brief named.
52b2d378's body records the lane's run, *"as the first mistake `wrong-closer`
hides 0 of 159 (68), `bracket-open` 6 of 164 (8)"*, and 46846f97's *"hidden
parse-stage seconds 17 of 14733"* (read, not re-run; mine are in 1.3). So the
engineer's *"`wrong-closer` as the first unchanged at 68 of 159"* and the
spec-warden's *"`wrong-closer` 68/159 (unchanged)"* are false for the tree the
landing goes on. It also answers the spec-warden's neighbour: the pairing its
proposed first clause states (*"A closer of any kind closes the innermost opener
still open"*) is what the trunk's parser now follows.

**1.2 The parser half of question (a) is gone from the trunk.** `git grep -n
"recover_past_closer\|recover_past_pair\|paired_closer"`: at 171e8c45,
`cursor.recover_past_closer` (`cursor.hero:412`) and seven callers; at 77b8ca98
the function is gone (a comment at `parse/unclosed.hero:86` still names it), and
the callers call `unclosed.recover_past_pair` and
`unclosed.paired_closer` (`parse/unclosed.hero:48`, `:90`), which follow the
lexer's pairing whatever the closer's kind. The engineer's separation (*"`RULE`
still recovers past a closer, and `recover_past_closer` ... ignores `closes`"*)
describes code the trunk no longer has, so "ratify the parser's use of `closes`"
would ratify a version no seat read.

**1.3 The trunk, on the seats' own plan.** `recovery.py --compiler <TRUNK or
TPEL> --jobs 4`, with `census.json`, `baseline.json`, `plan-singles.jsonl` and
`plan-pairs.jsonl` copied from `instrument/run-e5cc73eb/` into each `--out`
(`<p2>/inst/full-TRUNK`, `full-TPEL`): 641 programs, 13,594 singles, 15,800
pairs, no instrument error, no exit but 0 or 1 (24.4 minutes of wall each,
under load, so not a duration). The first column is the seats' baseline read
from its own files by the same script (`<p2>/full.py`).

| | `e5cc73eb` | `TRUNK` | `TPEL` |
|---|---|---|---|
| `bracket-open` singles, ONE / EXTRA of 156 | 89 / 67 | 89 / 67 | 110 / 46 |
| normal-arm diagnostics over the 13,594 singles | 16,339 | 16,337 | 16,216 |
| parse-stage seconds hidden, of 14,685 | 91 | 17 | 16 |
| as the first, hidden: `bracket-open` / `wrong-closer` / `string-open` | 8/164, 68/159, 1/170 | 6/164, 0/159, 0/170 | 5/164, 0/159, 0/170 |

`TPEL` against `TRUNK`: 38 singles moved, all `bracket-open`, 37 to fewer
diagnostics, none to more, and no single gains an `unclosed_bracket`. So on the
trunk the engineer's first two clauses hold (ONE 110; 16,216 against its
*"16,227 or fewer"*) and its third does not (`wrong-closer` 0, not 68); the
spec-warden's EXTRA 46 and *"no single gains an `unclosed_bracket`"* hold, and
its 7/164 and 68/159 do not (5 and 0). The prediction to write for the landing's
gate is the last column, on this pinned plan. Note what the whole instrument
cannot see: its 156 `bracket-open` singles read 89 and 67 on the trunk as before,
while 1.4's plan of 3,000 shows the trunk's added messages.

**1.4 The trunk added false messages on the class, and the narrowing masks
them.** Over 3,000 single missing closers (§ 2.3's plan), `TRUNK` prints 7,088
diagnostics to `NEW`'s 6,936 with the same ONE and EXTRA counts, so a gate that
reads classes cannot see it: 42 singles gain messages and none loses any, 159
added, 128 of them `missing_body` (`<p2>/singles.py`). One, rebuilt from the plan
(`examples/assembler/assemble.hero`, the `]` at the end of line 85 left out): the
`)` on line 86 closes the `[`, `ok(` stays open, and every `if` below is read
inside it. `NEW` and `PEL` print `84:18 unclosed_bracket`, `86:9
expected_expression`; `TRUNK` adds `missing_body` at 89:9, 93:9, 99:9 and 103:9,
four bodies that exist (the shape of defect 131, `docs/work/DEFECTS.md:190`:
the message *"names a body that is there, one level down, as missing"*).
Attributed by building each commit with `NEW` and running the 42 from their own
directories: `NEW` 101 diagnostics (14 `missing_body`),
042a14a4 108 (14), **52b2d378 253 (142)**, `TRUNK` 253 (142), `TPEL` 96 (5). The
one example is read; the other 41 are counted by code, not read. A narrowing on
the trunk would hide the regression rather than repair it, and 52b2d378's gate
(class counts) did not see it.

**1.5 The instrument's denominators move with the machine.** Its corpus is the
files that check clean in 0.8 s (`recovery.py`, `baseline`, `--dear 0.8`).
46846f97's run held one program more than the seats' (*"`selfhost/grammar_expr.hero`
measured under its 0.8 s"*): 13,627 mutants against 13,594. A prediction written
*"110 or more of 156"* or *"16,227 or fewer"* holds on a pinned plan only: copy
`census.json`, `baseline.json`, `plan-singles.jsonl` and `plan-pairs.jsonl` from
`instrument/run-e5cc73eb/` into the gate's `--out` folder first, as I did for 1.3.

## 2. The three narrowings of (b): where they differ, and what the measurements favour

**2.1 What each computes**, read from the code in `<p2>/{pe,pel,b4}/selfhost/`:

| | `PE` (engineer) | `PEL` (ffi-pragmatist) | `B4` (spec-warden) |
|---|---|---|---|
| words that end a reach | `starts_afresh`'s fourteen, `if`, `match`; `function` only before a name | `PE`'s, except a keyword before one `:` (a label) | at column 0 the landed seven; elsewhere `if while for match return assert` |
| `else` | strictly shallower | the same, decided before the label test | the same |
| the margin compared | `l.level * 4`, the level the lexer last laid out: the statement's margin | the same | `margin_of(l.open_brackets[0].start)`: the physical line of the outermost bracket still open |
| `code_lines`, `layout.hero` / `next_line.hero` | 202 / 214 | 202 / 225 | 203 / 228 |

The last row is the harness's own measure: a copy of `tests/harness` with line
45's `CEILING` 300 set to 0, `./heroes-X run hl/main.hero -- ./heroes-X layout`
in each tree; the unpatched tree reads 201 / 207, so `PE` costs 8 lines, `PEL`
19, `B4` 23. The spec-warden priced `B4` in raw diff lines (+33, -1) and left the
unit to the engineer; in the unit it is the dearest.

**2.2 On one union of every seat's probes.** 196 probe files (the engineer's
92, the spec-warden's 44, the ffi-pragmatist's 47, the sitting's 13) and 8 of
mine, 164 distinct programs, `check --brief` in both arms from each probe's
directory, eleven compilers (`<p2>/run_probes.sh`, outputs in `<p2>/out/`). The
permissive arm printed the normal arm's bytes on every probe but
`ffi/kw/L3_PE_naive`, under every compiler. Counted against `NEW` (the trunk's
builds against `TRUNK`), each `unclosed_bracket` added, judged true or false by
reading its source (true: the author never wrote that closer):

| | true added | false added | false removed |
|---|---|---|---|
| `PE`, `TPE` | 17 | 8 | 0 |
| `PEL`, `TPEL` | 17 | 4 | 2 |
| `B4`, `TB4` | 8 | 7 | 0 |

- `PEL` has every true reach end of `PE` and drops four of its false ones, the
  label shapes `A_body_label`, `L3_col4`, `L3_NEW_turn1` and my `sodium_L3_col4`;
  it also removes two the landed rule prints, `E_label_col0` and my
  `sodium_E_col0`.
- `B4` misses nine true shapes the other two reach: the `extern` stray (x1, x2,
  x2b, t3a, t3b, D, G), `break` (x4), a declaration at column 4 (x5); and on F it
  keeps `NEW`'s false `empty_record` (`NEW` 4 diagnostics, `PE` 2, `B4` 4).
- `B4`'s own false one is `margin_ref` (mine: `x = g(1,` over `2) + g(3,` over
  `return 4)`): `6:17 unclosed_bracket` for a `(` closed on line 7, and four more,
  five messages for one mistake; `PE` and `PEL` print `NEW`'s one. The margin row
  above is the whole difference.
- The four false ones `PEL` keeps (`b_if_col0`, `g5`, `g6`, `g7`) all three
  share: a statement word at or left of the statement's margin inside a bracket
  the author closed. How often a model writes that shape is still unmeasured.
- The same tallies hold on the trunk's builds, and `TPE`, `TPEL`, `TB4` differ
  from `TRUNK` on the same 43, 40 and 22 probes as their frozen twins from `NEW`.

**2.3 On the instrument, planted by class.** `recovery.py --only
bracket-open,extra-closer --per-op 3000 --pair-samples 600 --jobs 3`
(`extra-closer`, *"one `)` too many at a line's end"*, is Task 2's line 10),
planned once by `NEW` (6,000 singles, 2,121 pairs) and seeded identically into
every other compiler's folder (`<p2>/inst/big-*`); no instrument error and no
exit but 0 or 1 in any run.

| 3,000 single missing closers | ONE | EXTRA | diagnostics |
|---|---|---|---|
| `NEW` | 1,730 | 1,270 | 6,936 |
| `PE`, `PEL` | 2,059 | 941 | 4,145 |
| `B4` | 2,059 | 941 | 4,237 |
| `TRUNK` | 1,730 | 1,270 | 7,088 |
| `TPEL` | 2,059 | 941 | 4,152 |

By line, column and code: `PE` and `PEL` move 718 singles, 711 to fewer
diagnostics, 7 to the same count, none to more, and differ from each other on
none; `B4` moves 678, and 40 that `PE` reaches it does not. Over 3,000 single
stray closers no compiler moves any single or prints an `unclosed_bracket`: no
false report where no bracket is open. The full instrument's 156 `bracket-open`
singles (engineer: ONE 89 to 110) sampled this class too thinly to show its
size: the narrowings take 11 % of single missing closers from several messages
to one and remove 40 % of their diagnostics.

**(b)'s own class**, a missing closer and then one `)` too many below it in the
same file, 591 pairs:

| | stray told as itself | as another code | hidden | opener named |
|---|---|---|---|---|
| `NEW` | 189 | 212 | 190 | 183 |
| `PE`, `PEL`, `B4` | 268 | 196 | 127 | 277 |
| `TRUNK` | 193 | 212 | 186 | 183 |
| `TPEL` | 273 | 196 | 122 | 277 |
| `PELNi` (§ 5.1) | 379 | 146 | 66 | 388 |

On the class the question was raised on, the three narrowings are one compiler
(`PE` against `PEL`: 0 pairs differ; against `B4`: 8, in diagnostic counts
only). Every hidden pair of the 2,121 is in this class. By the first line at or
left of the statement's margin between the two mistakes (`<p2>/bclass.py`): 92
pairs have a column-0 declaration (told under every compiler); 86 a statement
word (the narrowings name 86 where `NEW` names 25, and tell exactly the two
mistakes in 61 where `NEW` does in 11); 203 a name (the narrowings name 55, `NEW`
25); and 165 no such line, where nothing moves under any compiler (§ 5.2).

**2.4 What the measurements favour, as a finding.** `PEL` over `PE`: equal on
(b)'s class and on 3,000 singles, four fewer false reports on the probes, two of
the landed rule's removed, a real library's parameter handled (§ 3.2), for 11
more lines. `PEL` or `PE` over `B4`: on no measure here is `B4` ahead; it costs
the most lines, reaches 40 fewer single missing closers and nine fewer true probe
shapes, and adds a false report of its own through its margin. The margin bound
itself is supported by what it does not do: no narrowing moves a stray-closer
single or a tracked file. What (b) as posed would do on this plan I did not run;
the seats' runs of it (8 goldens red, 25 false `unclosed_bracket` over the
13,594 singles) stand unrepeated.

## 3. Contradictions between seats, and which side the runs take

**3.1 (a) at column 0 alone.** The spec-warden approves (a) *"as landed"*, and its
sentence keeps *"with no indentation"* because *"an `extern` member at column 4
ends nothing"*; the ffi-pragmatist objects *"if the landing keeps column 0 alone
and drops the member line"*. On x1, x2, x2b, t3a, t3b, D, G and F, `B4` prints
`NEW`'s bytes (§ 2.2), so the spec-warden's resolution is the one that objection
is written against. The runs are on the ffi-pragmatist's side for what the
author reads; its turn counts (one against two) are its own runs, not repeated.

**3.2 The label test is a condition, by its seat's own words.** The
ffi-pragmatist: *"Object (the label test a condition rather than a
recommendation) if a header of a library in §1.11's table names a parameter
`match`, `use`, `test`, `record`, `constant`, `variant` or `assert` (libsodium
and cJSON are not on this machine, so I could not grep them)"*. `git clone
--depth 1` of libsodium (75c6d55, 2026-09-28) and cJSON (6d9f244, 2026-09-16),
comments and strings stripped, preprocessor lines skipped (`<p2>/hdr/words.py`):
libsodium's `include/sodium/utils.h` names a parameter `variant` at lines 91, 95
and 100 (`sodium_base64_encoded_len(const size_t bin_len, const int variant)` and
the two base64 converters); cJSON's two headers name none. §1.11's table:
*"Hashing, crypto | OpenSSL, libsodium"*. That binding, in the seat's layouts
(`<p2>/probes/critic/sodium_*.hero`):

| the parameter `variant: i32` | `NEW`, `TRUNK` | `PE` | `PEL`, `TPEL` | `B4` |
|---|---|---|---|---|
| on the member's line, or at column 8 | `expected_parameter` | the same | the same | the same |
| at column 4 | `3:5 expected_parameter` | `2:39 unclosed_bracket`, false, and 2 more | `3:5` alone | `3:5` alone |
| at column 0 | `2:39 unclosed_bracket`, false, `3:1`, `3:8 expected_name` | the same | `3:1 expected_parameter` alone | as `NEW` |

The column-0 row is the landed rule's own false report on the trunk today:
rule (a) ends a reach at `variant` at column 0 whatever follows. So the label
test repairs (a) as landed, not only `PE`. (Every layout is refused, `variant`
being reserved; what moves is what the binding's author reads.) The grammar
agrees with the test's premise: `grep -n -E
'"(constant|function|record|variant|test|extern|use|return|break|continue|assert|for|while|if|else|match)" *":"'
spec/heroes-spec.md` finds no production with a keyword before `:` (exit 1).

**3.3 The engineer's sentence and PE's code.** The sentence: *"at a margin no
deeper than the line the brackets opened on"*; its verdict: *"the line the
outermost bracket opened on"*; the ffi-pragmatist's brief quotes the same words
as the rule under judgement. `PE` compares `l.level * 4`, and `line_start`
returns before the level moves on every line inside brackets
(`layout.hero:93`) and on a depth-zero continuation (`:111`), so `l.level` is the
margin of the statement holding the brackets. The two differ where the
outermost open bracket opened on a continuation line: on `margin_ref` the
sentence says the reach ends (`return` at column 10, no deeper than line 6 at
10) and `PE` does not end it. The words describe `B4`'s reference, and `B4`'s
reference is the one that misfires there. The sentence the resolution writes
should name the margin of the statement the brackets opened in.

**3.4 PE's false reports.** The engineer counts one on its own probes, and my
run agrees (`b_if_col0` alone); over every seat's probes it is eight (§ 2.2).

**3.5 The blind verdicts and the sessions.** § 6.

## 4. Claims asserted and not measured, with the command that settles each

- **Speed.** The engineer timed `NEW` against `PE` and the ffi-pragmatist `PE`
  against `PEL`, both under load and saying so; the ffi-pragmatist names the
  first's confound (`NEW` is a bare-clang seed build, `PE` a `heroes build`).
  `CTL` is the right control. Unrun by me. On a still machine: `for i in 1 2 3 4
  5; do for c in ctl/heroes-ctl pe/heroes-pe pel/heroes-pel; do /usr/bin/time -p
  $c check selfhost/main.hero; done; done` in `<p2>`, `real` read against `user`
  plus `sys`.
- **"The census and the check form say so"** (engineer, Task 4: a close at
  column 4 costs the parser what one at column 0 does). They cannot: under `PE`
  the tokens of all 1,358 tracked files equal `NEW`'s (the engineer's run; the
  ffi-pragmatist's for `PEL`), so the tree holds no column-4 close. What does
  reach it is the moved singles (711 fewer, none more, § 2.3) and the probes.
- **The formatter's probe by hand.** `.claude/rules/verification.md` puts
  `selfhost/lexer.hero` and `selfhost/parse/**` under *"the run by hand below
  before a push"*; its `bracket` family breaks brackets after every token. No
  seat ran it (the engineer ran the net's `probe`, 24 and 0). By the grammar no
  legal program puts a reach word at the head of a line inside brackets, so I
  expect nothing to move: an inference. Command: `heroes probe <root> --family
  bracket` and `--family single` over `selfhost`, `tests`, `examples`, with the
  landing's compiler built at `-O2`.
- **How often a model writes a statement word at the statement's margin inside a
  bracket it closed** (`g5` to `g7`, `b_if_col0`): unmeasured, and the one false
  report every narrowing keeps.
- **PEL on the instrument**: unmeasured in the sitting; now § 2.3 and 1.3.
- **The one-turn rates both clean readings predicted**: now § 6.
- **The ffi-pragmatist's C-boundary census** (363 `extern` files, 272 emitted C
  files, 0 moved) I read and did not re-run.

## 5. Routes nobody listed

**5.1 The spec's own NEWLINE sentence as a reach's end.** Spec lines 11 to 15:
inside brackets a line that ends with a literal, a name or a closer keeps its
NEWLINE, and *"that NEWLINE may stand only before a closing bracket or a `,`, or
where a production writes it"*. `grep -n -E 'NEWLINE|Sep '
spec/heroes-spec.md`: the only productions that write one inside brackets are
the `[` and `{` literals (`Sep = "," | NEWLINE`, lines 210 to 214); `Args`,
`Params`, `TypeArgs` and a group take `,` alone. So inside a `(`, a line after a
kept NEWLINE that opens with anything but a closer or a `,` is refused by the
spec whatever its first word, and the parser already says so
(`line_end_before_continuation`, *"inside brackets a line end may stand only
before a closing bracket or a `,`"*). The lexer holds both facts at
`line_start`: `maybe_terminator` pushes a `.terminator` for a kept NEWLINE
inside brackets (`layout.hero:230` to `233`), and `l.open_brackets` holds the
innermost bracket. This is the clause that reaches u4_k (`x = f(1, 2` over `y =
3 )`), one of the four reproducers (b) was raised on and one no word list can
reach, and with it most name-led statements at the margin after an unclosed
call (165 of the 203 name-led pairs, below). I built it
on `PEL`, bounded by the statement's margin (`<p2>/peln/`, `<p2>/pelni/`; for
`PELNi`, `git diff --no-index --numstat` against `PEL` reads +13 -1 in
`layout.hero` and +11 in `next_line.hero`, 5 of them `PELN`'s predicate left
unused: raw lines, not the harness's unit):

- `PELN`, any line but a closer, a `,`, a blank or a comment: over the tracked
  tree 1 output moved (the refused `fixedbugs-130-a-line-going-on-below-a-bracket-never-closed`,
  one message renamed at 36:5), 0 exits; on the seats' probes 3 true and 6 false
  added over `PEL`, two of the false because it overrode `PE`'s own `else`
  exemption.
- `PELNi`, only a line that opens with a name (a word no keyword): over the
  1,358 tracked files 0 outputs moved in either arm, 0 exits, 0 token streams
  (`<p2>/census_lex.sh`, against `NEW`); on the probes 20 true and 5 false added
  against `PEL`'s 17 and 4: u4_k, n6 (three mistakes told once each, where every
  seat's narrowing prints two, one of them at the `return` line), and my
  `t2_inner_mistake` (Task 2 with `y = total +` between: `5:19`, `7:15`, `8:25`,
  `9:18`, all four, where they print two), and one false, `m1_at_arg_line`, whose
  `f(` closes at the end of line 4. On the instrument (§ 2.3's plan): (b)'s
  class hidden 66 against `PEL`'s 127 and `NEW`'s 190, the stray told as itself
  379 against 268, the opener named in 165 of the 203 name-led pairs against 55
  and both mistakes told exactly in 135 of them against 29; 4 more single
  missing closers moved, all to fewer; 0 of 3,000 stray-closer singles moved.
- Precedent: Go, whose semicolon rule ends the call at the kept line end and
  tells all three of Task 2's mistakes (§ 7). Swift does not (a name-led line
  after `f(1` is *"expected ',' separator"* and the list goes on).

A probe, not a proposal: its goldens, its code review and whether its margin
bound is the right one are the engineer's to price.

**5.2 The head class.** 165 of (b)'s 591 pairs (28 %) are a head's bracket left
open with the stray closer in its body: in 160 the missing `)` is a function
head's own (`function main(`), in 4 an `if` head's, in 1 a `for` head's; the
instrument calls all 165 `inside`. No compiler here names an opener in any of
them. The spec-warden asked (*"a body is always deeper than its head, so no
margin rule reaches them ... a question for the compiler-engineer"*); no report
answers. The historian's F# reading is the shape of an answer: a context closes
at `else` *"only when an enclosing `if` can take it"*. On `if_head_else` (mine:
`if f(x > 2` over its body, `else`, a body, then `print(x +)`), every compiler
from `NEW` on prints `unclosed_bracket`, a debris `expected_expression` at the
`else` (5:5) and the true 7:14, and no narrowing removes the debris. A bound one
level deeper for a bracket opened on a head line, with `else` at the head's own
margin, is unprototyped.

**5.3 A note instead of silence.** The clean Task 1 reading's condition 2: P's
13:1 kept as a note on the `unclosed_bracket` (*"gave up here"*) rather than an
error with advice, which it says makes P and Q equivalent; clang's `note: to
match this '('` and rustc's label are that shape the other way round. Nobody
weighed where the reach ended as information for the author. Unrun.

## 6. The blind readings' predictions, run

Task 1's clean reading: *"Under Q: at least 18 of 20 resubmissions compile on the
first try, and at most 1 of 20 touches line 13. Under P: 12 to 15 of 20 ... 5 to
8 of 20 contain an edit to line 13"*, false *"if the two rates come out within a
factor of two of each other"*. Task 2's: *"One-turn repairs ... S about 8.5 in
10, R about 1.5 in 10"*, false *"if measured one-turn repair rates under R and S
come within 2 in 10 of each other"*. I ran them: 40 fresh sessions, `claude -p
--tools "" --setting-sources "" --no-session-persistence --output-format json`,
each in its own folder under `<scratchpad>/183-critic-rep/runs/`, outside any git
tree and under no `CLAUDE.md` (`git rev-parse` fails there, and a walk of every
ancestor finds none), answered by `claude-opus-5`, the CLI's default (each
session's `modelUsage` also lists one `claude-haiku-4-5` call). The prompt: the
clean seats' own `spec.md`, program and one output (their files), and one
sentence asking for the program the model would submit next. Every answer went
through `NEW`'s `check`
(`<scratchpad>/183-critic-rep/judge.py`).

| output | compile in one turn | line 13 changed | `report` deleted | stray `)` removed |
|---|---|---|---|---|
| P | 10 of 10 | 0 | 0 | |
| Q | 10 of 10 | 0 | 0 | |
| R | 10 of 10 | | | 10 |
| S | 10 of 10 | | | 10 |

Both predictions are false by their own clauses. The Task 2 seat's two turns
under R came from its stated choice (*"I recorded the fix that trusts R's
silence"*); no session made it, since the stray `)` is in the source it reads.
Then on real programs: five (b)-class mutants from § 2.3's plan in which `NEW`
hides the stray closer and `PEL` tells it (31 to 105 lines, no `use`, each
original checking clean; `<scratchpad>/183-critic-rep/real/`), four sessions per
output. `NEW`: 20 of 20 compile, 20 restore both damaged lines, 19 give back the
original byte for byte. `PEL`: 20, 20, 20. The one inexact answer, under `NEW`,
deleted a correct match arm (`-0 => 3`) and compiled: a silent wrong edit, 1 of
20 against 0 of 20, too few to weigh. 80 sessions, 12.62 USD.

What this does and does not say. A model that reads the whole program repairs
these in one turn under every output, so at these sizes and this N the rule's
gain is not visible in one-turn repair; it is visible in what the messages say
(§ 2.3's counts, § 1.4's false `missing_body`, R's silence). Whether it becomes
visible in a longer file, where a model leans on the compiler more than on its
own reading, is the experiment still owed: the same script over the (b)-class
mutants of files of 300 lines and more.

## 7. Precedents, run

The historian had no shell and marked its readings of CPython's stray closer
and clang's skip *unrun*. Task 2's program in the six languages installed here
(`<p2>/prec/`; GHC, Elm and F# are not):

| compiler | the opener named | the stray `)` told | the later mistake told |
|---|---|---|---|
| CPython 3.13, 3.14 | no: one `invalid syntax` at line 7 | no | no |
| clang 21, `-fsyntax-only` | as a note, `expected ')'` at `if` | no: its skip pairs it with the opener | yes |
| rustc 1.90 | no (the file balances) | no | yes, among 7 errors, three `cannot find value total` |
| Go 1.27, `go build` | no: `unexpected newline in argument list` at the opener's line end | yes | yes |
| Swift 6.4, `swiftc -parse` and `-typecheck` | as a note, `expected ')'` at `if` | yes, twice | no |
| Nim, `nim check` | no: `expected: ')'` at `if` | yes | no |

- CPython: the historian's reading holds. The stray closer pops the bracket and
  the opener is never named.
- clang: the run agrees with the source the historian quoted (*"If there is a
  LHS token at a higher level, we will assume that this matches the unbalanced
  token and return it"*) rather than with its reading of `b.c` (a skip to the
  next `;`): after `if` the skip stops at the stray `)`, pairs it with the
  opener and never tells it. R's behaviour, with a note.
- Swift: narrower than *"a start-of-line declaration or statement word"*. It
  breaks a list at such a word only where a separator was due: `f(1` over `if`,
  `return` or `let` ends the list (*"expected ')' in expression list"* at the
  word), and below `return` and `let` the later `print(y +)` is told; `f(1,` over
  `return` is *"expected expression in list of expressions"* and the later
  mistake is lost; `f(1,` over an `if` expression is read as an element and
  refused at type-check (*"'if' may only be used as expression in return, throw,
  or as the source of an assignment"*: SE-0380's scope, which the historian's
  premise needs, holds on this machine); a name-led `y = 2` after `f(1` is
  *"expected ',' separator"*. So Swift is the precedent for a word rule at a
  separator's place, no compiler here bounds by margin, and Go is the precedent
  for § 5.1.

## 8. The questions the sitting did not ask

- **Does batch 3's parser rule owe what batch 2's does?** 00-shared.md folds
  batch 2's parser commits into (a) as *"the parser's use of the offsets the
  lexer hands it"*. 52b2d378 replaced that use (*"the parser passes the closer the
  lexer's stack paired with the opener, whatever its kind"*) during the sitting,
  from the coordinator's own queue, and on the trunk it adds `missing_body` on
  single missing closers (1.4; the example read, on bodies that exist). By the
  sitting's own framing it is (a)'s second half, and nothing judged it.
- **The second reading's last two questions never reached a clean seat.** The
  repository's `llm-ergonomist-second-reading.md` asks for debris (task 3) and
  *"across both readings: is the rule the two better outputs share one you could
  predict from the specification, and if not, what sentence would let you, and
  does the specification need it?"* (task 4). The clean Task 2 brief
  (`blind/brief-task2.md`, `cmp` equal to the one the session read) has neither,
  and a fresh session cannot answer "across both readings". The spec-warden's
  condition for a spec sentence (*"only if a blind reading ... showed a model
  misreading the reach"*) rests on exactly that question. § 6's sessions repaired
  every program without any such sentence, which bears on the answer and is not
  it.
- **Whether the ffi-pragmatist's absence lost a C-facing half.** The seat did
  not stay absent: the sitting widened at 07:07. What it brought that no other
  seat had: nothing moves at the C boundary (its census, not re-run by me), the
  label test, case F. What it could not finish, libsodium and cJSON, is § 3.2, and
  it turns the recommendation into the seat's own condition.
- **What stops the next landing without a sitting.** The rule landed on the
  measurement that nothing that compiles changed; 52b2d378 then changed the same
  mechanism while this sitting ran. No seat was asked.

Outside the sitting: the engineer's 1,000 nested `(` ending in *"panic: stack
exhausted in grammarexpr.postfix"*, exit 134, is the runtime's designed guard
(`runtime/parts/stack.c:23`, journal 031's robustness guards), a controlled
abort and not a segfault; whether `check` should refuse such a nesting with a
diagnostic is a question. The ffi-pragmatist's *"internal error: a diagnostic
landed inside the Heroes library"* is panel 171's message class
(`docs/panel/171-lent-and-the-pointer-c-hands-back.md`), on an archived file.

## 9. The resolution the measurements support, as a finding and not a verdict

- **(a)**: sound by the grammar and by every census (0 exits moved over 1,358
  files: the seats' for `PE`, `PEL`, `B4`, mine for `PELNi`); the column-0 bound
  adds nothing to soundness. Amended into one predicate, it should carry the
  label test, since the landed rule prints a false *never closed* on a real
  §1.11 library's parameter name at column 0 today (§ 3.2).
- **(b)**: as posed, refused by the engineer and objected to by the
  spec-warden, on measurements I did not repeat (8 goldens red, the brace
  habits); the historian's approval is conditional on the grammar answer, which
  the engineer gave. Narrowed: `PEL`'s predicate, `PE`'s words and
  margin with the label test, and a design.md sentence that names the margin of
  the statement the brackets opened in (§ 3.3). `B4` is behind on every measure
  here (§ 2.4).
- **What any narrowing leaves**: 127 of (b)'s 591 pairs still hidden (122 on the
  trunk), 148 name-led and 165 head-class pairs with the opener unnamed. The
  kept-NEWLINE clause reaches most of the first at no census cost (§ 5.1); the
  head class is unprototyped (§ 5.2). Both belong to the batch after the sitting
  rather than to its resolution, unless the sitting chooses to widen.
- **The trunk**: the landing goes on a compiler carrying 52b2d378's added
  `missing_body` (1.4), which the narrowing hides; it is a defect in its own
  right, for the list rather than for this sitting, and the predictions have to
  be restated on the trunk and on a pinned plan (1.3, 1.5).
- **The ergonomic case** rests on what the messages say, not on one-turn repair:
  80 sessions compiled in one turn under every output (§ 6).
