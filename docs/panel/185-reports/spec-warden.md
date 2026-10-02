# Panel 185, the spec-warden's report

Written as I go; sections 0.x are measurements in the order taken. `<sw>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/185-spec-warden`,
a copy of `03e70520` (not a git checkout: `git rev-parse` there says *not a
git repository*).

## 0.1 The ceiling, by grep, at the start of the sitting

`grep -n '^### 1\.6' docs/design/design.md` gives `:253`; `:255-256` read:
*must fit in 10240 tokens, measured by `claude-opus-5` through `POST
/v1/messages/count_tokens`*. The brief's 10,240 holds today.

## 0.2 The spec before, measured in `<sw>`

`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (`real 4.37`,
`user 4.23`), then `./heroes measure spec/heroes-spec.md`, exit 0:
`claude-legacy 6716`, `cl100k_base 6838`, `maximum 6838`, `spread 122`,
`real 9060` (claude-opus-5, 2026-09-28, recorded by the instrument and not
re-taken: `--refresh` is a paid call this sitting does not make). *Headroom:
1180 against the 10240 ceiling, but the FFI floor mortgages 60 of it, so what
is measured against the ceiling is 9120.* The shared brief's figures
reproduce exactly.

**What this seat can measure**: the two vendored tables, before and after, on
a draft written into `<sw>`'s spec. **What it cannot**: any `real` AFTER
count. Every real figure below is an inference from a vendored delta and is
written as a band, never a point (0.3).

## 0.3 How a vendored delta maps to a real one: the ledger's band, re-measured

A Python pass over `<sw>/docs/measurements/010-spec-budget-ledger.md`, regex
`**+v** vendored and **+r** on the reader`: 32 rows carry both; of the 29 with
v >= 9 and r > 0, r/v runs **1.03 to 1.60, median 1.31, pooled 1.40**; of the
21 with 9 <= v <= 70 (the size of every sentence in this sitting), the same
1.03 to 1.60, median 1.30, pooled 1.27. The ledger refuses a factor, so every
real figure below is the band `v x 1.03` to `v x 1.60`, an inference. The
whole document's own ratio is 9060 / 6838 = 1.325, inside the band.

## 0.4 The mortgage this sitting inherits: panel 184's ratified, unlanded sentences

`grep -n 'after a jump\|lone' spec/heroes-spec.md` in `<sw>`: empty. Panel 184
ratified R1 (§ 2), R4 (§ 8) and R6 (§ 9), and none has landed, so their
tokens come out of the same 1,120 before any route of this sitting. Written
into `<sw>/drafts/` by `<sw>/drafts/price.py` (every anchor asserted to match
exactly once) and measured with `./heroes measure`:

| draft | legacy | cl100k | vendored delta | real, inferred band |
|---|---|---|---|---|
| R1, the blind diff's wording (*`{{` and `}}` each write one brace, and a lone `}` is an error*) | 6730 | 6852 | +14 | 14 to 22 |
| R1, panel 184's wording (*write* for *each write*) | 6729 | 6851 | +13 | 13 to 21 |
| R4, the ratified text, after § 8's jump sentence | 6781 | 6905 | +67 | 69 to 107 |
| R6, its floor with N written as 2000 (N is unmeasured) | 6739 | 6864 | +26 | 27 to 42 |
| **R1 + R4 + R6 together** | 6818 | 6945 | **+107** | **110 to 171** |

**So the headroom this sitting actually spends from is not 1,120 but about
949 to 1,010 real** (1,120 less the band above). Panel 184's spec-warden
priced R4 at +64 by the critic's text; the ratified text as written into the
document reads +67 here.

## 0.5 Every route's sentence, written into the document and measured

All rows by `<sw>/drafts/price.py` (the drafts are `<sw>/drafts/<name>.md`),
`./heroes measure` on each, against `base` = legacy 6716, cl100k 6838. The
delta is the vendored maximum's; real is the band of 0.3, unrun.

| question | route | the text, where | legacy | cl100k | delta | real band |
|---|---|---|---|---|---|---|
| Q1 | (1a), (1a') sentence | § 13 after *what a `ptr` points at.*: *A name the header defines only as a macro is called as written: its result is checked, its parameters are not.* | 6741 | 6863 | +25 | 26 to 40 |
| Q1 | (1a), (1a') merged, exact | the exception clause gains *a parameter of a name the header defines only as a macro,* | 6730 | 6852 | +14 | 14 to 22 |
| Q1 | (1a) merged, short | *a macro's,* in the same clause: **false** (a fortified `memset` is a macro AND a declared function, and keeps its check), priced to show what the true version costs | 6720 | 6842 | +4 | |
| Q1 | (1b) stated in the spec | *A name the header defines only as a macro is refused: bind a `static inline` function of a header of the program's own that calls it.* | 6748 | 6870 | +32 | 33 to 51 |
| Q1 | (1b), (1d), the message alone | nothing: § 13's *one that disagrees is refused* stays true | 6716 | 6838 | 0 | 0 |
| Q1 | (1c) mark | `Member = [ "macro" ] "function" ...` and *`macro function` binds a name the header defines only as a macro: its result is checked, its parameters are not.* | 6747 | 6869 | +31 | 32 to 50 |
| Q2 | (2a), blind A1 verbatim | `Inline` lists every `Statement` but the two bindings | 6734 | 6856 | +18 | 19 to 29 |
| Q2 | (2a), A1 factored | `Inline = ( Place "@" Expression \| ... \| Expression ) NEWLINE \| While \| For .` | 6725 | 6846 | +8 | 8 to 13 |
| Q2 | (2a), split | `Statement = <the two bindings> \| Simple .`, `Simple` the rest, `Arm` takes `Simple`, `Inline` deleted | 6690 | 6810 | **-28** | |
| Q2 | (2a), split, prose exact | the same, and § 8's *one statement, inline* becomes *an indented block, or inline one statement that is not a declaration or an `=`, `_ = 0` included* | 6705 | 6827 | **-11** | |
| Q2 | (2c), blind A3 production only | `Inline` gains `Place "@" Expression`; § 8's prose untouched, so it still says *one statement*, which a loop is | 6721 | 6842 | +4 | (prose false) |
| Q2 | (2c), A3 with prose *inline but not a loop* | | 6725 | 6846 | +8 | 8 to 13 |
| Q2 | (2c), A3 with the blind brief's prose | *and a `while` or a `for` after `=>` is a compile error: a loop is written as the arm's block* | 6751 | 6873 | +35 | 36 to 56 |
| Q2 | (2c), split | `Statement = <the two bindings> \| While \| For \| Simple .`, `Simple = ( Place "@" Expression \| "return" [ Expression ] \| "break" \| "continue" \| "assert" Expression \| Expression ) NEWLINE .`, `Arm` takes `Simple`, `Inline` deleted | 6680 | 6798 | **-40** | |
| Q2 | **(2c), split, prose exact** | the same, and § 8's prose *an arm's body is an indented block, or inline one statement that is not a loop, a declaration or an `=`, `_ = 0` included* | 6698 | 6818 | **-20** | |
| Q2 | (2b) | nothing in the spec | 6716 | 6838 | 0 | 0 |
| Q3 | (3a), blind B1 alone | after § 8's jump sentence | 6756 | 6880 | +42 | 43 to 67 |
| Q3 | (3b), blind B2 alone | the same place | 6761 | 6885 | +47 | 48 to 75 |
| Q3 | (3a) B1 appended after R4 | | 6821 | 6947 | +109 (R4 +67, B1 +42) | |
| Q3 | (3b) B2 appended after R4 | | 6826 | 6952 | +114 (R4 +67, B2 +47) | |
| Q3 | **(3a) merged into R4, narrow** | § 8's jump sentence becomes *A jump (`return`, `break`, `continue`, or a block, `if` or `match` whose every path ends in one) is a valid arm body: ...*, then R4's two sentences verbatim | 6801 | 6927 | +89 (**R4 +67, (3a) +22**) | |
| Q3 | (3a) merged, R4's path-enders reaching arms | the jump defined as above for the statement-after rule, and *An arm whose every path ends is valid ...* after R4's `->` sentence | 6800 | 6926 | +88 (R4 +67, (3a) +21) | |
| Q3 | (3a) merged, one predicate for all three rules | *A path ends at `return`, ..., `exit(code:)`, `assert false`, a `while true` ...; a statement after one ... is a compile error* | 6797 | 6922 | +84 | (widens R4) |
| Q3 | (3b), no sentence | nothing | 6716 | 6838 | 0 | 0 |
| Q4 | all three routes | the spec states the refusal (opening paragraph, *the next line may not begin with a `-` that does not touch its operand*) and no fix's certainty | 6716 | 6838 | 0 | 0 |
| Q4 | `Pattern` tightened | `"_" \| string \| [ "-" ] ( integer \| character )` | 6716 | 6838 | 0 | 0 |
| Q5 | (5c), R1 alone | R1's sentence; *A literal without the `f` is unchanged* stays | 6730 | 6852 | +14 (**0 over R1**) | |
| Q5 | (5a), blind C1 | replaces *A literal without the `f` is unchanged.* | 6775 | 6899 | +61 (+47 over R1) | 48 to 75 over R1 |
| Q5 | (5b), panel 184's (1b) amended | *A literal without the `f` holding `{` and `}` around names all bound where it stands is a compile error; such braces, meant as text, are written `{{` and `}}` in an `f` literal.* | 6770 | 6894 | +56 (+42 over R1) | |
| Q5 | (5b), short | *A literal without the `f` may not hold a bound name in braces: braces meant as text are `{{` in an `f` literal.* | 6752 | 6875 | +37 (**+23 over R1**) | 24 to 37 over R1 |
| Q5 | (5d), blind C2 | | 6781 | 6904 | +66 (+52 over R1) | |
| Q5 | (5d), short | *A literal without the `f` may not hold a name, a field or a call in braces, as in `"{n}"`: braces meant as text are `{{` in an `f` literal.* | 6764 | 6887 | +49 (**+35 over R1**) | 36 to 56 over R1 |

**design.md**, which carries no budget, priced on the same tables for the
record (`<sw>/drafts/design_price.py`, base legacy 85522, cl100k 88138):
(1b)'s four corrections, *`#define` constants* kept and *a function-like
macro* named at `:565`, `:645`, `:2265-2266` and `:2401-2402`, **+114**;
(1a)'s two corrections, at `:567-569` and `:645-647`, where the thesis is
stated for the boundary (*a wrong type in an `extern` is a compile error*),
**+29**; (4a)'s three clauses at `:1996-2000` **+61**; (4b)'s *both fixes are
`guess`* **+11**.

**The stacks, measured as one document each** (`<sw>/drafts/stack.py`):

| stack | legacy | cl100k | delta | real, inferred, with the 60 of the FFI floor |
|---|---|---|---|---|
| panel 184's R1, R4, R6 alone | 6818 | 6945 | +107 | 9230 to 9291 of 10240 |
| **the routes this report approves**: R1, R6, (3a) merged into R4, (2c) split with its prose, (5b) short | 6842 | 6970 | **+132** | **9256 to 9331** |
| the same with (2a) for (2c) and (5d) for (5b) | 6861 | 6991 | +153 | 9278 to 9365 |
| the costliest route of every question: R1, R6, R4 + B2, (2c) with the blind prose, (1b) stated, (5d) as C2 | 6981 | 7111 | +273 | 9401 to 9557 |

**No route of this sitting, nor the costliest of all of them together, comes
within 680 tokens of the ceiling on the band's upper end. This seat has no
budget veto to cast.** Every verdict below therefore stands on §1.2, on
Principle 0, or on whether the sentence is true.

## 0.6 The facts the verdicts rest on, each run in `<sw>`

- **What the parser takes after `=>`** (`./heroes check` on
  `probes/q2/s01` to `s14`, `a54`, `a76`): exit 0 for a mutation of a name, a
  field and an element, `while`, `for`, an `if` and a `match` head, `assert`,
  a call; exit 1 `declaration_in_arm` for `k = 5`, `k: i64 @ 5`, `k: i64 = 5`
  **and `_ = 0`**, the last with *`_` would be bound where nothing can read
  it*, which spec § 5 contradicts (*It binds nothing*); `missing_body` for a
  `while` with its body on the same line. That is blind candidate A1's set
  exactly.
- **What entered by decision**: panel 014's ratified amendment text enumerates
  the inline statement as *`assert`, `break`, `continue`, `return`, a
  mutation, a call* (`docs/panel/014-match-arm-body.md:133-134`). A mutation is
  in; **a loop never was**. The production landed by panel 133 dropped the
  mutation, which is defect 147's first half and a transcription, not a
  decision.
- **Who needs a loop after `=>`**: `git ls-files '*.hero' | xargs grep -n '=>
  while '` (lines with no `"`): 1, `tests/golden/run/fixedbugs-139-...:100`;
  `'=> for '`: 0; in `selfhost/`, 0 of each.
- **Who needs a macro-only binding**: the compiler's five `extern` modules
  (`selfhost/cli/files.hero`, `io.hero`, `link.hero`, `process.hero`,
  `selfhost/emit/literal.hero`) bind its own `hero_os.h` and two libc
  functions; the wait macros are reached in C of its own,
  `runtime/parts/run.c:873-874`, behind `hero_run_go`. **The compiler already
  does what (1d) asks a program to do.**
- **What a macro keeps under (1a)**: the result is asked by a `_Generic`
  (`selfhost/emit/extern_probe.hero:5-9`), which a macro answers; the
  parameters are asked by a never-called function whose body makes the call
  (`:146-171`), which a macro has no declaration for. So *its result is
  checked, its parameters are not* is the true sentence.
- **`-"a"` as a pattern**: `./heroes parse` exits 0 with no output on
  `probes/q4/s5c_bulleted_strings_applied.hero`; `check` refuses it with
  `bad_operand`, *`-` takes any integer or a float, found `str`*, the same
  message as `x = -"a"` in an expression (`drafts/neg-str-expr.hero`). `-'a'`
  as a pattern checks clean (`s5d`, `t4`).
- **(5d)'s reach, a regex bound and not the lexer**: plain literals holding
  `{name}`, `{a.b}` or a call in braces, over the 1,530 tracked files: 39
  literals in 10 files, **8 of them in 4 compiler modules**
  (`selfhost/escape_readings.hero:306-307`, `lex_interp.hero:149`,
  `lexer.hero:452`, `:459`, `next_line.hero:314`, each a test string holding
  Heroes source) and 24 in `examples/template/main.hero`, whose templates are
  data. The engineer's census by the lexer is the number that judges.

**Correction to 0.6's second bullet, 02:05, added rather than rewritten.**
*A loop never was* overclaims. Panel 014 also fixed a parser that hung on `1
=> for x in xs` plus its body (`docs/panel/014-match-arm-body.md:60-62`), *true
under P, Q and R alike*. So that sitting had a loop after `=>` in front of it,
and neither listed it nor refused it. Its resolution says *one statement*,
and its enumeration names six kinds with no loop. Whether the list was meant
to be exhaustive is not written. What stands is this: **no decision names a
loop on the arm's line, and none refuses it.**

## 0.7 The instruments on the stack this report approves

`<sw>/drafts/stack-recommended.md` was copied over `<sw>/spec/heroes-spec.md`,
and `./heroes run tests/harness/main.hero -- ./heroes grammar`, then `-- ./heroes
spec`, were run. Then the original was restored (`cmp` against the trunk's
file: identical).
- `grammar`: **9 passed, 0 failed.** `Simple` is defined and reachable,
  `Inline` is gone, and nothing is used undefined. That leaves 43 productions
  against a floor of 40.
- `spec`: **16 passed, 4 failed.** The four are `budget`, `spendable`,
  `real` and `ledger`, and each says the pinned count is 6838 or 9060 while
  the document measures 6970. That is what every spec change reads until its
  landing runs `--refresh` and writes the ledger row. `shape`, `anchors`,
  `offered`, `named`, `rejected` and `inventory` pass.

Three more prices: (5b) in an exact short form (*may not hold, in braces, a
hole whose names are all bound where it stands: braces meant as text are `{{`
and `}}` in an `f` literal*) is legacy 6766, cl100k 6890, **+38 over R1**;
(5d) in an exact short form, with *`{{` and `}}`*, is 6769 / 6893, **+41 over
R1**; and the clause *`_ = 0` included* in (2c)'s prose costs **+7** on its
own (-27 without it, -20 with it).

**And one shape beside Q4's** (`<sw>/drafts/q4-one-bulleted-arm.hero`):
`return match k` with `- 1 => "one"` over `_ => "other"`. `check` gives
`spaced_minus_element`, *fix (certain): write the `-` against its value*.
Applied, the program builds and prints `other` for `name(k: 1)` at exit 0,
where a writer of a bulleted list meant `one`. No other arm opens with a
spaced `-`, so **(4a)'s integer clause would keep this fix `certain`**. That
is the *one* shape beside lane 135c's *many* (CLAUDE.md § RUN IT: one, none,
padded, ...).

# The verdicts

**Every verdict is provisional on the real count.** The vendored figures were
measured in this seat; the real ones are inferred from them (0.3), and the
landing takes them with `--refresh`. **No veto is cast on the budget**: by
0.5's stacks, the costliest route of every question together lands at most at
9,557 of 10,240. The vetoes below are on Principle 0 (design.md §1.6's
payment rule, CLAUDE.md § 2), as this seat's charter allows.

## Q1. A macro-only C name in an `extern` group

- **verdict**: **(1b) approve, with (1d)'s header as the repair its message
  names; (1a) veto; (1a') veto; (1c) veto.** Defect 145's note: approve, 0
  spec tokens, whichever round carries it.
- **section**: design.md §1.6 (payment), §1.2 (real cost), §1.11 point 3 and
  §3.1 `:645-647` (*a wrong type in an `extern` is a compile error, which is
  this project's thesis applied to the boundary*), §1.12 (*complete and
  defended*); CLAUDE.md § 2.
- **cost**: (1b) and (1d) cost **0 spec tokens** (6838 before and after). § 13's
  *one that disagrees is refused* stays true. design.md owes its four macro
  promises corrected, **+114 vendored** and no budget: `:565`, `:645`,
  `:2265-2266`, `:2401-2402`, each saying *function-like* macro and keeping
  `#define` constants, which do work. `:556` and `:735` (QBE has no headers)
  stay true. (1a) and (1a') cost **+14** at the cheapest true text (*a
  parameter of a name the header defines only as a macro*, in § 13's exception
  clause), or +25 as a sentence of its own. They also cost +29 on design.md,
  where the thesis is stated for the boundary and stops being true. The +4
  form, *a macro's*, is false: a fortified `memset` is a macro and keeps its
  check. (1c) costs +31 and a contextual word.
- **spec_token_delta**: (1b) 6838 to 6838; (1a) 6838 to 6852 (real 9074 to
  9082, inferred); (1c) 6838 to 6869.
- **removal**: none owed by (1b). (1a) offers none and names no prediction an
  existing instrument scores.
- **needed_for_self_hosting**: **no**. The compiler binds no macro. It reaches
  `WEXITSTATUS` in C of its own (`runtime/parts/run.c:873-874`, behind
  `hero_run_go`), which is (1d)'s shape.
- **argument**: (1a) buys convenience by giving up the check the thesis
  stands on. Under it, a macro parameter declared at the wrong width compiles
  at exit 0, and the spec must say so, a second gap beside the pointee's. (1d)
  reaches the same macros with every parameter checked. That is measured:
  `probes/q1/shim/` prints 1 for 256, and declared `i64` it is refused,
  `ffi_parameter_type`. So the boundary is complete without the check being
  given up. Neither branch of Principle 0 is met for (1a): no compiler need,
  and the thesis runs the wrong way. Under §1.2, (1b)'s refusal costs one
  round trip when a model binds a macro, and 0 tokens on every other prompt.
  (1c) only makes the gap visible.
- **prediction**: at the landing of (1b), `build --emit-c` over the 412
  tracked `extern` files (`probes/q1/emit-one.sh`) reads 206 and 206 again,
  the five `ffi_unknown_name` keep their text, and only a macro-only name's
  message changes. `heroes measure` reads the spec unchanged by Q1. Both are
  checkable at the landing batch.
- **condition**: two things would lift the veto on (1a). One is a program on
  §1.0's closure list or in `examples/` needing a function-like macro that a
  `static inline` wrapper cannot reach. The other is a measured rate of
  macro-only bindings above S/R, the sentence's real tokens over a round
  trip's (14 to 22 against 500 to 2000: 0.7% to 4.4% of all prompts); 0 of
  412 tracked `extern` files has one. (1b) stated in the spec (+32) pays only
  above 1.7% to 10.2%.
- **robust and conservative**: robust is (1b) with (1d)'s repair in the
  message and design.md's four sentences made true. Conservative is (1b) with
  today's message, and the four sentences are owed either way, being false.

## Q2. A one-line `match` arm (defect 147)

- **verdict**: **(2c) approve, written in the split form with its prose
  exact; (2a) object; (2b) object.** On the side question: `_ = 0` stays a
  line of a block, as spec § 8 already says, and `declaration_in_arm`'s
  *`_` would be bound* is false against § 5 (*It binds nothing*). That is a
  message defect with 0 spec tokens, to be filed.
- **section**: design.md §1.6 and `.claude/rules/spec-shape.md` (*merging beats
  appending*, one home per rule); CLAUDE.md § 2 and § 12 (spec beats compiler);
  design.md §4.7 `:1230` (panel 014).
- **cost**: (2c) split with exact prose is **6838 to 6818, -20 vendored**. That
  is a **named removal**: `Inline`'s two lines go, and every statement form keeps
  one home (`Statement = <the two bindings> | While | For | Simple`, and
  `Arm` takes `Simple`). Without *`_ = 0` included* it is -27. The parser
  refuses a loop after `=>` (the engineer's lines), and **1 tracked line
  moves** (`tests/golden/run/fixedbugs-139-...:100`), 0 in `selfhost/`. (2a)
  split with exact prose is -11 and moves nothing. (2a) as blind A1 is +18.
  (2b) is 0 tokens, but it refuses about 600 lines of the compiler.
- **spec_token_delta**: (2c) 6838 to 6818; (2a) 6838 to 6827; real negative,
  size unrun.
- **removal**: the `Inline` production, measured in the draft.
- **needed_for_self_hosting**: **yes for the mutation** (676 lines in
  `selfhost/` by the critic's grep). **No for a loop** (0 in `selfhost/`, 1 in
  the tree).
- **argument**: the mutation is owed. Panel 014 enumerated it, the compiler
  writes it about 600 times, and the production lost it as a transcription
  slip. A loop on the arm's line has no decision naming it and no compiler
  line using it, and its thesis effect is unmeasured, so under Principle 0 it
  waits. (2a) writes it into the one document a reader trusts. (2c) states
  exactly what is needed, refuses the rest, and is 20 tokens cheaper than
  today, because the split removes a production rather than adding
  alternatives. (2b) refuses the compiler's own source.
- **prediction**: at the landing, `heroes measure` reads Q2's delta at -20 ±
  3 on cl100k. The census of `check` over the tracked files moves exactly 1
  file (`fixedbugs-139-...hero`) and 0 under `selfhost/`. `grammar` reads 9
  and 0, as on the installed stack (0.7).
- **condition**: two things would change it. One is the blind A reading
  showing that a model writes a loop after `=>` more often than as the arm's
  block, and gets it right. The other is a compiler module that needs one.
  Either moves this seat to (2a) in the split form (-11).
- **robust and conservative**: robust is (2c) split; conservative is (2a)
  split, which moves no program.

## Q3. A value block whose last statement leaves on every path

- **verdict**: **(3a) approve, provisional on the blind B reading, written
  INTO R4's sentence and not after it.** Two texts are approved at one price:
  R4's path-enders reaching arms (robust), or narrow (conservative). Three
  objections: to (3a) as B1 appended, to (3b) as B2, and to (3b) with no
  sentence. The one-predicate text, which refuses a statement after
  `exit(code:)`, is also objected to.
- **section**: design.md §1.6 (merging beats appending), §4.7 `:1236-1255`;
  panel 184 R4 (ratified) and its (2a)-wide refusal.
- **cost**: R4 alone is +67. Merged with (3a), narrow, it is **+89, so (3a)
  costs +22**. Merged with R4's path-enders reaching arms, it is +88, so +21.
  Appended, B1 costs +42 over R4 and B2 costs +47. (3b) with no sentence
  costs 0. Programs moved: the engineer's census.
- **spec_token_delta**: 6838 to 6927 (narrow) or 6926 (arms wide), R4
  included; real 9152 to 9202 for both, inferred.
- **removal**: none. The payment is the prediction below, scored by a
  compile, an instrument that exists.
- **needed_for_self_hosting**: **no** (the shape cannot be in the compiler
  today, since it is refused).
- **argument**: § 8 has one word, *jump*, for the arm rule, and R4 uses it
  for the statement-after rule. If R4 is to reach `c5` (R4 states its refusal
  reads defect 139's every-branch predicate), then *jump* must cover an `if`
  or `match` whose every branch jumps. The arm sentence then admits (3a) by
  the same word. Defined once, it costs 22 tokens. (3b) instead needs a
  second term, or else `c5` stays legal while the compiler refuses it, which
  is defect 147's shape again. B2 states a refusal at more tokens (+47) than
  (3a)'s acceptance costs.
- **prediction**: after the landing, `check` exits 0 on `probes/q3/a55`,
  `a69`, `a73` and `b1` to `b8` (adjusted to give a value where the probe
  asks for one), and exits 1 on `c5`. With arms wide, `c1`, `c2`, `c3` and
  `c6` also exit 0. Scored at the landing batch.
- **condition**: two things would move this seat to (3b) with no sentence
  and a message naming the repair. One is the blind B reading showing that a
  model under the current text does not write the a55 shape (so the 22
  tokens buy nothing). The other is the engineer's census showing that (3a)
  moves a tracked program.

## Q4. The certain `-` in a pattern

- **verdict**: **spec: 0 tokens under every route, and `Pattern` stays as it
  is (tightening objected to).** On design.md §4.15's premise: **approve a
  rule of two clauses**. Before a string the fix deletes the `-` and is
  `certain`; before an integer or a character both fixes are `guess`. (4b)
  is approved as the conservative route; **(4a) is objected to**, as is
  **(4c)**.
- **section**: design.md §4.15 `:1996-2000`, whose own preceding sentence
  reads *Both readings are `guess` fixes, where both parse*; CLAUDE.md § 11
  (*a narrowing asks the value, never the world*); §1.2.
- **cost**: spec 6838 to 6838. Tightening `Pattern` is also 0 tokens, so
  tokens do not decide it. It is refused because it would claim a parse
  refusal the parser does not make: `parse` exits 0 on `-"a"`, and `check`
  refuses it with § 7's own message. design.md: the two-clause rule is +22,
  (4a) +61, (4b) +11.
- **spec_token_delta**: 0.
- **removal**: n/a.
- **needed_for_self_hosting**: no.
- **argument**: a `certain` fix is applied without a reading, so a wrong one
  is a silent wrong program delivered by a tool, the worst outcome §1.2 can
  price. A guess costs no extra round trip, because the diagnostic has
  already fired. (4a)'s integer clause is still a premise about the writer:
  one bulleted arm defeats it, measured (0.7, prints `other` where `one` was
  meant). A string's deletion is the only fix that checks, so its certainty
  asks the value and holds.
- **prediction**: under the two-clause rule, applying every `certain` fix
  over `probes/q4/` and `drafts/q4-one-bulleted-arm.hero` yields no program
  that checks clean and prints other than its `_meant` twin, against 3 such
  programs today (`s5a`, `s5d`, the one-arm probe). Checkable at the landing.
- **condition**: a measured rate showing that bulleted arms are never written
  by a model, while a signed integer arm with a spaced `-` is written often
  and the guess costs a measured extra round trip. That would move this seat
  to (4c).

## Q5. The forgotten `f`

- **verdict**: **(5b) approve in its exact short form; (5c) approve as the
  conservative route; (5d) object; (5a) object.**
- **section**: design.md §1.6, §1.2, §1.3 (the author's ruling, 2026-10-01);
  CLAUDE.md § 2; panel 184 R1, R2 and its census (recorded, not re-run here).
- **cost**, all over R1 (+14), and all replacing *A literal without the `f`
  is unchanged*: (5c) +0; (5b) exact short **+38**, long +42; (5d) exact
  short +41, as blind C2 +52; (5a) as blind C1 +47. Programs moved: (5b) 0 of
  1,389 files (recorded, panel 184's engineer); (5a) 22 files stop checking,
  the compiler among them (recorded); (5d) by this seat's regex bound, 39
  literals in 10 files, 8 of them in 4 compiler modules and 24 in
  `examples/template/main.hero` (0.6).
- **spec_token_delta**: (5b) 6838 to 6890 with R1 (real 9114 to 9143,
  inferred); (5c) 6838 to 6852.
- **removal**: none. (5b)'s payment is panel 184's registered prediction:
  the 17 silent `forget-f` sites of 25 caught, 0 files moved, scored by a
  compile.
- **needed_for_self_hosting**: no for all; and (5a) and (5d) **refuse** the
  compiler's own test strings.
- **argument**: (5b) is the only route with a measured thesis effect in the
  record, and it moves no file. Its words cost 38. (5a) and (5d) are local to
  the line, which is their merit for the reader, but they refuse the
  compiler's tests and an example whose templates are data. Under Principle
  0 a rule that stops the compiler compiling itself does not enter on an
  unmeasured benefit. (5c) is true at 0 tokens and leaves the 17 sites
  silent.
- **prediction**: at (5b)'s landing, `check` over the tracked files moves 0
  files, and on panel 184's 25 `forget-f` mutants, 17 that exit 0 today exit
  1.
- **condition**: two things would move this seat. A lexer census of (5d)
  showing 0 compiler files refused moves it to approve (5d), as cheaper for
  the reader at +3 more. The blind C reading vetoing (5b)'s wording moves it
  to (5c).

## Corrections, 02:20, added rather than rewritten

- **Q4's prediction counted 3 programs today; the count is 4.** It was run
  with the compiler's own `./heroes check --apply` on each probe, then `heroes
  run` on what it printed (`<sw>/drafts/q4run/`). `s5a` prints `many` where
  its `_meant` twin prints `two`. `s5d` prints `3` for `2`. `t6` prints `many`
  where `n = 2` under the bulleted reading gives `small`, because only the
  head is fixed and `| - 2` stays a sign. The one-arm probe prints `other`
  for `one`. `s5c`'s applied program is refused (`bad_operand`), so it is not
  silent, and `s5b` prints `minus one` either way. So the prediction reads:
  **4 today, 0 under the two-clause rule, and under (4a) 1 or 2** (the one-arm
  probe for certain, and `t6` unless a spaced `-` after `|` counts as a
  neighbour).
- **Q3's real band** is 9152 to 9202 for the narrow text (+89) and 9151 to
  9201 for arms wide (+88), not *9152 to 9202 for both*.
