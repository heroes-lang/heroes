# Panel 180, completeness critic

No verdict: this names what is missing, and what I ran to find it.

**Where and with what.** Everything below ran on 2026-09-27 in
`/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-180/critic/`:
- **The trunk copy**: `git archive 29ed5601`, with a compiler built from the seed (4.25 s).
- **`bii/`**: the compiler-engineer's (b)+(ii) prototype, rebuilt by me from its diff (`bii.patch`: `selfhost/grammar_expr.hero` and `selfhost/parse/type.hero` only). It was built with the seed compiler in 119 s, not taken from the seat's binary.
- **`dbg/`**: the trunk plus one line in `selfhost/cli/syntax_cmds.hero`, so that fmt's output guard also prints the text it rejected.

I never ran more than 2 heroes processes at once, and never touched the trunk or another seat's directory except to read. My spec copy is back at its pristine digest (`shasum` 4123f19d).

## 0. What I re-ran, and whether it holds

- **00-shared.md's table, all 17 rows: hold.** Its 25 files (`m_*.hero`, `r*.hero`), copied to `probes/shared/`, give the same exit codes and diagnostic codes on my build.
- **The coordinator's run claims: hold.** `heroes run` prints `2` for all five: `[a` / `- b]`, `[base * qty` / `- discount]`, `[a` / `(b)]`, `[a` / `!b]` and `deltas`.
- **The brief's ten fragments**, exact texts, in `probes/ten/`:
  - The trunk accepts 1, 3, 5, 8 and 10: the ergonomist's Q column, 10 of 10.
  - Under (b)+(ii) it accepts 1, 3, **4, 5, 6**, 8 and 10. The ergonomist's R and S columns match that on **8 of 10**; they are wrong on 5 and 6.
- **The compiler-engineer's map, all 117 shapes rather than a sample.** I ran its `cases.py` and `run.py`, copied into `map/`, on my trunk and my (b)+(ii) builds.
  - Both tables are identical, row for row, to the seat's `trunk.tsv` and `bii.tsv`.
  - Accepted: 68 on the trunk, 81 under (b)+(ii).
  - 14 rows move to accepted and one to refused, row 55: `[1` / `- 2]`, `spaced_minus_element`.
- **The (b)+(ii) gates on my build:**
  - own tests: 756 passed (89.6 s);
  - `layout`: 3/0;
  - `surface`: 167/0;
  - `canonical`: 2/0;
  - `grammar`: 9/0;
  - `corpus`: 55/0 (300 s);
  - `annotations` and `check`: not run by me.
- **Smaller checks that also hold:**
  - The seat's census cross-check grep finds two hits, both constant bodies at depth 0 (`selfhost/print/owners.hero:41,45`).
  - The warden's RbComma prices at 6732 vendored (+39).
  - 00-shared.md's pointers:
    - `layout.hero:40` is `is_line_ender` and `:131` is `maybe_terminator`;
    - there are 14 `skip_terminators` calls in `grammar_expr.hero`, and 7, 3, 2, 2 in `parse/*`;
    - design.md:1942 carries the quotation.

## 1. The four attacks the coordinator named

### 1.1 Is (ii)'s whitespace key robust?

**What fmt prints** (`probes/fmt/c01`):

| input | fmt prints |
|---|---|
| `x = - 1` | `-1` |
| `a -b` | `a - b` |
| `a- b` | `a - b` |
| `a - - b` | `a - -b` |
| `[1, - 2, - b]` | `[1, -2, -b]` |

So fmt's canonical spelling is exactly the key: a unary minus against its operand, a binary one spaced.

Where a comment follows a unary sign, fmt wraps the unary in parentheses, with `(` on a line of its own: `a` / `(` / `-  # c` / `b` / `)` (c02, c05 to c08, in lists, map keys and map values). Every one of those outputs parses under (b)+(ii). I tried to make (ii) produce a new fmt refusal from shapes the seat's map lacked and could not. **The seat's "fmt's output stays legal with no change" survives.**

**Where (ii) fires** (`probes/attack/`; trunk result, then (b)+(ii)):

| shape | trunk | (b)+(ii) |
|---|---|---|
| `[a` / `- b]`, `[a` / `-  b]`, `[a` / `- -b]`, a float | len 2 | refused |
| `[a  # c` / `- b]` | 2 | refused |
| nested, `[[a` / `- b]]` | inner 2 | refused |
| inside a call, `print([a` / `- b].len())` | 2 | refused |
| map key, `{1: 10` / `- 2: 20}` | 2 entries | refused |
| map value, `{"k": a` / `- b}` | `expected_map_entry_colon` | `spaced_minus_element` |
| sign then comment, `[a` / `-  # c` / `b]` | 2 | **refused** |
| sign alone on its line, `[a` / `-` / `b]` | 2 | **refused** |
| `[a` / `-b]`, `[a` / `-(b)]`, `{1: 10` / `-2: 20}` | 2 | 2, the residual |
| first element, `[` / `- 1` / `2` / `]` | `[-1, 2]` | `[-1, 2]`, does not fire |
| after an explicit comma, `[a,` / `- b]` and `[a` / `, - b]` | 2 and refused | 2 and 2 |
| `[a -` / `b]` | 1 | 1 |

(ii) never runs in a call's arguments or a record construction: `separator` is called only from `array_literal` (:492) and `map_literal` (:527). Those shapes are refused anyway on both compilers: `f(n: a` / `- b)` and `P(x: a` / `- b)` give `expected_args_close`.

The key is exact about what it claims. **What the landing still owes:**
1. **The rule's wording.** It fires after a NEWLINE that separates *with no comma*, not on "an element line that begins with `- `". A sentence in the second form is false for the first element and for a line after an explicit comma.
2. **It refuses two shapes the trunk accepts**, where the sign stands apart from its operand by a comment or a line end.
   - No such line exists in the tree: the census counts 0 element lines beginning with `-`.
   - fmt never prints one, because it wraps them in parentheses.
   - The message says *"this line begins with `- `"*, which is not what those lines show.
3. **The "negative element" fix does not converge for the sign-then-comment shape.** It replaces the span from `-` up to the next raw token, which is the comment there, with `-`, and the result is still refused. Read from the prototype's code, unrun; `fixes` is unrun, as the seat says. This is on top of the join fix deleting a trailing comment, which the seat already reported.

**Can a model's natural continuation be unspaced?** Nobody measured it, and no instrument in the tree can: `harness/README.md` says metric 2 is *"frozen until it can run"*, and `M-generated-programs` generates programs by construction. What I could run is precedent:
- **Swift 6.4 uses the same key, the other way round.** `[a` / `- b]` is a subtraction (prints `1 [2]`); `[a` / `-b]` is a prefix minus and is refused (`expected ',' separator`).
- **Python 3.14.7 subtracts in both spellings** (`1 [2]` twice).

So both break-before-operator languages read the spaced form as binary, which supports refusing it, and Swift reads the unspaced one as a sign. That is precedent, not a measurement of models.

### 1.2 Does (b) widen anything that changes meaning?

**One separator, not two.** A NEWLINE before a `,` in a list counts once:

| shape | result |
|---|---|
| `[1` / `, 2]` | 2 |
| `[1` / `,` / `2]` | 2 |
| the same with a blank line, or a comment line, between | 2 |
| `[1` / `2` / `, 3]` | 3 |
| `{"a": 1` / `, "b": 2}` | 2 |

Doubled separators stay loud: `[1,` / `, 2]` and `[1` / `,` / `, 2]` give `expected_expression`, and `[1, 2` / `,` / `]` now says `trailing_comma`. The index and the Place work: `xs[1` / `]` prints 8, and `xs[1` / `] @ 5` then prints 5.

Every widened shape formats, reparses and is a fixpoint under (b)+(ii). I found no program that parses today whose meaning (b) changes. The map agrees: nothing moves from accepted to refused except row 55, which is (ii).

**What (b) does change: two texts become false.** Both state the parser fact (b) removes, *"the parser skips a line's end before a group's `)` and not before an index's `]`"*:
- `selfhost/print/breaks.hero:119-121`;
- the header of `tests/golden/surface-fixtures/comments101/indexparens.hero`, which `tests/harness/suite_surface.hero:347` quotes verbatim in its `out_is`.

Correcting them edits an existing file under `tests/golden/` and one under `tests/harness/`. The compiler-engineer's prediction says that will not happen (*"no byte of any EXISTING file ... changed to accommodate it"*). The synthesis should restate the prediction to exempt comment text, not leave a false fixture header standing. Lane g's F1 parenthesis restoration becomes unnecessary for the index, and stays harmless.

### 1.3 A sentence true of (b)+(ii) in full

**The facts it must carry**, each run:
- **The line enders.** `is_line_ender` (`layout.hero:40`) lists:
  - an identifier;
  - an integer, float, string or character literal, and an interpolation's last piece;
  - `true`, `false`, `nullptr`;
  - `return`, `break`, `continue`;
  - `???`, `?`, `)`, `]`, `}`.

  The spec states no ender list anywhere.
- **Mark words are names.** `counted_by`, `lent`, `acquires`, `owned`, `consumes`, `transfers`, `retains`, `borrows`, and also `as`, `tag`, `partial`, `link`, `package` and `when`, are all identifiers to the lexer.
  - The spec quotes them exactly as it quotes keywords.
  - It has no keyword list (`grep -in 'keyword\|reserved' spec/heroes-spec.md`: 0 lines).
  - `(p: ptr counted_by` / `n, n: u64)` is refused, `expected_extent`, on both compilers (w04).
- **Two words that are not enders can end a line inside brackets** in a program that parses: `function` (in a function type) and `fail`. After both the line goes on (w01, w02, exit 0). No other keyword can stand at a line end inside brackets in a program that parses.
- **Where the NEWLINE stands.** Under (b) it stands before every closer and before every `,` (map rows 22 to 24, 34, 37, 52, 64, 78, 99, 111), and where `Sep` writes it. Nowhere else.
- **(ii) fires only after a NEWLINE that separates with no comma.**
- **Beside the sentence, not in it:**
  - an `f"..."` hole holds no line end (w06, `unterminated_string`);
  - continuation lines may start at any column: design.md:1939-1941 says *"indent freely"*, while spec line 26 says *"exactly 4 spaces per level"*.

**Proposed wording, T1** (vendored +69):

> Inside `(` `[` `{` a NEWLINE never ends a statement. A line there that ends
> with a word other than `function` or `fail`, a literal, `?`, `???` or a
> closing bracket carries one, which stands only before a closing bracket or a
> `,`, or where a production writes it, and a line ending otherwise goes on
> below. Where a NEWLINE separates without a `,`, the next line may not begin
> with a `-` set apart from its operand.

**Why "word".** It is the spec's own term (line 7: *"A quoted word stands for itself"*). It covers names, mark words, `true`, `false` and `nullptr` without needing a keyword list the spec does not have.

**Its one imprecision.** For `if`, `match`, `else` and the other non-ender keywords it says "carries one" where the lexer plants nothing. No program that parses has one of them at a line end inside brackets, and a reader applying T1 predicts a refusal exactly where the compiler refuses. If that is not acceptable, T4 names the mark words instead: RbComma plus *"(the words after a C parameter's type included)"* plus the (ii) sentence.

**Priced on the vendored instrument** (maximum is cl100k in every row; delta from 6693; variants in `variants/`, made by `make.py`, whose P reproduces today's text byte for byte):

| variant | max | delta | true of (b)+(ii)? |
|---|---|---|---|
| RbComma (the warden's) | 6732 | +39 | no (ii); mark words unresolved |
| T2 = T1 without (ii) | 6735 | +42 | no (ii) |
| T3 = T1 without "goes on below" | 6752 | +59 | yes, by implication |
| RbComma + (ii) | 6759 | +66 | mark words unresolved |
| **T1** | **6762** | **+69** | yes (the imprecision above) |
| T1 + "at any column" | 6766 | +73 | yes, and closes the column gap |
| T4 | 6769 | +76 | yes |

**What the (ii) sentence costs on its own: +27.** Both pairs agree: RbComma+(ii) minus RbComma, and T1 minus T2. T1 minus the warden's § 10 clause is about +53 by arithmetic; I did not run it.

**`real` is unrun for every variant here.** The coordinator's refreshes give Q +87 real against +65 vendored (1.34) and R +65 against +47 (1.38). On that ratio T1 is about +92 to +95 real, an estimate, against 1319 free after the floor.

**What a reader must hold under T1:**
1. which line endings carry a NEWLINE;
2. the three places it may stand;
3. (ii).

That is the ergonomist's R count plus one. **The reader seat never read any wording with ", or a `,`" or with (ii) in it.** It judged P, Q, R and its own S. Its "least to hold" verdict does not cover the converged route until it reads the adopted text, on fragments 5, 6 and at least one (ii) fragment.

**Sentences the seats offered that are false under (b)+(ii), measured:**
- **R and S**: fragments 5 and 6 are accepted.
- **S also**, on the trunk and under (b):
  - it forbids breaks after `::`, `@`, `->` and `function`, which the compiler accepts (map rows 17, 27, 81, 95, 103);
  - it forbids breaks before `,` (rows 22 to 24, 34, 37, 78, 111).

  The error runs in the harmless direction, but S is still a false sentence, which is what defect 104 is about.
- **RbComma** is true only if a reader counts `"counted_by"` as a name. The warden's two extern probes (t13, t18) end the line in `lent` before `)` and `,`, which parse under either reading. The shape that separates the two readings, `counted_by` / `n`, was not probed.
- **The compiler-engineer's rule under (b)** leaves out (ii).

### 1.4 fmt's side finding: two classes, and one is not fmt's

Run through `dbg/`, which prints what the guard rejected.

**Class A, the printer.**
- **Trigger:** a comment after `.` or `::`, in a value whose only bracket is a pair of the author's parentheses.
- **Mechanism:** fmt drops the parentheses, since precedence does not need them and the tree keeps none. It keeps the break after the `.`, which now lands at depth 0, where the next line is an indented block.
- **The seat's four, as rejected:** `y = xs.  # c` / `len()`, `y = p.  # c` / `x`, `y = Point::  # c` / `x`, `y = a + b.  # c` / `c`.
- **Five more members I found:**
  - a variant case's leading `.`, `y = (.  # c` / `plus)` (`expected_case_name`);
  - a binary's operand, `(xs.  # c` / `len() + 1)`;
  - a unary's operand, `-(xs.  # c` / `len())`, printed `-xs.  # c`;
  - a `return` value;
  - an `if` condition, where the error is `indentation_jump`.
- **Where it is legal:** wherever another bracket encloses the break. `print((xs.  # c` / `len()))` prints as `print(` / `xs.  # c` / `len()` / `)`.
- **Its relation to lane g's F1:** it is the shape F1 repaired for the index (`ys[(a  # c` / `)]`, `breaks.restored`), at a position that repair does not reach. The likely path is `spread`'s one-line branch (`brackets.hero:129-134`, *"after the `.`"*) and `part`'s `.field` / `.field_name` fall-through. Which of them skips `restored` I read and did not run.

**Class B, not the printer: the releaser-set reader.**
- **The defect:** `selfhost/handles.hero:191` `releasers` splits the set's *source text* on `|` and drops blanks only. The panel 176 landing review added newlines to `blank` (:214). A comment inside the set becomes part of the next name.
- **In fmt:** it printed `@out: Db acquires a | #cb  # c`, and the guard caught it.
- **In the checker, the same defect** (`probes/releaser/`):
  - `@out: H acquires h_close |  # either one` / `h_close_v2)` fails `heroes check`. It gives `unread_releaser` for `#eitheroneh_close_v2`, a name the program does not contain.
  - The same program without the comment runs and prints 7.
- **The same text reader feeds:**
  - `check/releasers.hero:154`, `check/contracts.hero:171`;
  - `emit/handle_before.hero:174,178`, `emit/handle_text.hero:56`;
  - `print/marks.hero:35`;
  - `releaser_spans` (:219) has the same blind spot for diagnostic spans.
- **Loud, not corrupting, as far as I ran.** The checker refuses first, and `#` never belongs to a name, so the garbled name cannot resolve. That the emitter can never be reached with it is an inference, unrun. It is the panel 176 repair's neighbouring shape, and nobody looked there (CLAUDE.md § RUN IT, CL-061, CL-078).

**The seat's other two reproducers are not fmt defects.** `rel_pipe_after_mark` and `res_pipe` are refused by the parser at exit 1: a mark word is a name, and a result's releaser set sits at depth 0.

**Two defects, neither this sitting's**, for the coordinator to file, the trunk being frozen.

## 2. What is missing

### 2.1 Routes nobody listed

- **(f) Repair what the refusals say**, whichever route lands.
  - **Today:** a break before an operator in a group gives *"expected `)` to close the group, found `+`"*, with no note and no fix (`check --json`: `"fixes": []`).
  - **In a call:** *"expected `)`, or `,` and another argument, found `-`"*, which points the author at a comma.
  - **Go 1.27.1 on the same shapes, run here:** *"unexpected newline, expected )"* and *"unexpected newline in argument list; possibly missing comma or )"*.
  - **Go's history** (the historian's #1006, #3008 and the 2022 change) is a decade of repairing exactly this message.
  - **The fix's certainty.** In a group and an index a line end before an operator has one reading, so moving the operator up could be a `certain` fix. In a call it is a guess, since a missing comma is the other reading.
  - **Unpriced**, although the ergonomist's prediction 3 expects most loud refusals to come from exactly these shapes.
- **(g) Swift's and Scala 3's use of the same key: read a spaced leading `- ` as a continuation** instead of refusing it (Swift does, measured above). It turns a refusal into a meaning, which (ii) exists to avoid; on robustness (ii) ranks above it. It should still be recorded as the alternative.
- **Odin's split, run here.**
  - `x := (1` / `+ 2)` prints 3, and so does `xs[1` / `+ 1]`.
  - `f(1` / `+ 2)` and `[]int{a` / `- b}` are refused (*"Expected a comma, got a newline"*).
  - That is Python's rule in a group and an index, and Go's in calls and literals: the compiler-engineer's (e) without its list half. (e)'s price covers it only in part.

### 2.2 Claims asserted and not measured

- **"Models break before an operator inside brackets", and "they write the continuation unspaced": no instrument exists** (§ 1.1).
  - So the ergonomist's prediction 3 has no instrument that exists today, which its brief required.
  - The compiler-engineer's conditions 2 and 3 cannot trigger.
  - The synthesis should say so, not wait on them.
- **The historian's P2: I ran it, and both halves hold** (§ 2.1): Go refuses `(1` / `+ 2)`; Odin accepts it and refuses `f(1` / `+ 2)`.
- **The historian's condition 2**, how CoffeeScript and V read a line-initial `- b` in a newline-separated list: unrun. Neither is installed (`command -v coffee v`: absent); `npx coffeescript` would fetch from the network.
- **The warden's band for `real` (1.2 to 1.7 times vendored):** Q and R fall at 1.34 and 1.38, on the coordinator's numbers. The adopted wording's own reading is unrun.
- **Unrun, as the compiler-engineer already says:**
  - (ii) leaving the knot for 3 lines;
  - (ii)'s golden, source annotation and `fixes` row;
  - the compiler's time before and after, which CLAUDE.md § Verification owes at landing.

### 2.3 Contradictions between seats

- **The seats converged on a route, not on a sentence.**
  - The ergonomist approves R and S (no NEWLINE before `,`).
  - The warden objects to R as worded and backs RbComma (with `,`).
  - The compiler-engineer's rule includes `,` and leaves out (ii).

  This is checkable, and I checked: fragments 5 and 6 are accepted under (b)+(ii), so R and S are false for the route all three approve.
- **The historian contradicts its own entries.** Its argument says NEWLINE-separated elements with depth-blind terminators are *"a combination none of the surveyed specifications states"*, while its V and CoffeeScript entries both make commas optional across lines. Whether those two plant terminators without regard to depth is unverified in its report, and checkable with their toolchains.

### 2.4 Framing facts a seat took on trust

- **The spec-warden's brief says Q "states the compiler as it is, exceptions included". It does not.**
  - It gets 7 of the warden's 35 probes wrong.
  - The coordinator's "Q column matches, 10 of 10" scores the reader on ten fragments that hold no type, no parameter list and no extern. It does not show that Q is true.
  - The warden caught this; no other seat re-checked.
- **00-shared.md's table named two exceptions**, the index's `]` and the call's `,`. The compiler-engineer found five more classes:
  - types;
  - the `f"..."` hole;
  - `if` and `match` inside brackets;
  - mark words;
  - the silent class.

  The adopted text has to be checked against the whole map, not the brief's table.
- **design.md as the source of truth.** §4.15 rules that terminators go in everywhere and that continuation lines "indent freely". It says nothing about a break *before* a token inside brackets (lines 1936 to 1948, read).
  - (b)'s widening and (ii) are therefore this sitting's rulings, not design.md's.
  - They owe design.md text, which no seat drafted.

### 2.5 Questions the sitting did not ask

1. **What do the refusals say?** (§ 2.1 f.) This is the round trip the thesis pays for.
2. **(ii) is a new diagnostic class**, a language change under CLAUDE.md § 4. It owes its golden and its source annotation under § 9, and neither is priced.
3. **Three facts beside the sentence** that the spec does not state:
   - a string, and so a hole, is one line;
   - continuation lines start at any column (T1 + "at any column" prices this at +4);
   - a C parameter's words are names.
4. **Which instrument keeps the adopted sentence true.** The warden proposes `surface` rows over its 35 probes. The compiler-engineer's 117-shape map already exists and is larger. No seat proposed making the map the net's rows, with each row's expected outcome read off the adopted sentence.

## Files

- **This report:** `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-180/critic/report.md`
- **Probes:** `probes/` holds six directories:
  - `shared` (00-shared.md's table and the coordinator's run files);
  - `ten` (the brief's fragments);
  - `attack` ((ii) and (b));
  - `fmt` (minus spellings and sign comments);
  - `fmtdefect` (the seat's reproducers and eight of mine);
  - `words` (`fail`, `function`, marks, the hole).
- **Releaser defect:** `probes/releaser/` (`control.hero`, `commented.hero`, `rel.h`)
- **The map on my builds:** `map/trunk.tsv`, `map/bii.tsv`
- **The prototype:** `bii.patch`, `bii/heroes-bii`; the debug fmt is `dbg/heroes-dbg`
- **Spec variants and their generator:** `variants/` (`make.py`, `pristine.md`)
- **Precedent runs:** `precedent/go`, `precedent/odin`, `precedent/swift`
