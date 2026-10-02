# Panel 185: a macro is named as a macro, an arm takes a statement, a block that leaves leaves, and a spaced sign has two readings

2026-10-02. Full panel: five seats and the completeness critic, on five
questions, convened on the author's answer of 2026-10-01 (*2a*,
`docs/records/log/2026-10-01-2224-six-answers-panel-184-ratified-panel-185-convened-the-openers-message-reworded-the-blind-seats-command.md`).
The frozen tree was the trunk at `03e70520`. Briefs:
`docs/panel/185-briefs/` (the shared brief repaired on the critic's first pass;
the version it read is `00-shared-before-the-critic.md`; the evidence is
`185-briefs/probes/q1` to `q5`). Reports: `docs/panel/185-reports/`. The seats
stopped at the account's session limit at about 01:20 and resumed from their
own report files at 01:44. The blind seat's one session cost 0.61 USD.

## The proposal

Five questions, each with its routes, as `00-shared.md` states them:

- **Q1** (defect 143): a macro-only C name in an `extern` group. (1a) a second
  clang round and a call-form probe, parameters stated unchecked; (1a′) the
  compiler writes a `static inline` wrapper from the declaration; (1b) the name
  refused, the message saying it is a macro and naming a repair a program can
  use today; (1d) that repair, a `static inline` wrapper in a header of the
  program's own; (1c) a `macro function` mark. Beside it, defect 145's
  no-evidence shape.
- **Q2** (defect 147): spec § 8's `Inline` production lists less than its own
  prose, design.md §4.7 and the parser. (2a) the production says what the
  others say; (2b) the parser refuses what the production does not list; (2c)
  the production gains the mutation and a loop after `=>` is refused. Beside
  it, `_ = e` on an arm's line.
- **Q3**: a value block whose last statement leaves on every path is refused
  at that statement. (3a) the block leaves, so its arm jumps; (3b) the refusal
  stands with a message naming the repair. And whether (3a)'s predicate is
  panel 184's R4's.
- **Q4**: design.md §4.15's certain fix for a spaced `-` at an arm's start,
  whose premise bulleted arms falsify. (4a) lane 135c's three clauses; (4b)
  both fixes always guesses; (4c) the premise stands.
- **Q5**: the forgotten `f`, panel 184's (1b) wording back to a sitting on the
  author's condition. (5a) any hole-shaped `{...}` in a plain literal refused;
  (5b) panel 184's (1b) amended; (5c) no refusal, R2's note alone; (5d) a hole
  judged by its own text, a name or a chain on one.

## The verdict table

| seat | Q1 | Q2 | Q3 | Q4 | Q5 |
|---|---|---|---|---|---|
| compiler-engineer | **(1b) with (1d)**, built; (1a), (1a′) object; (1c) **veto** | **(2a)** with `_ = e` admitted, 4 lines; (2b), (2c) object | **(3a)**, built as (3x); (3b) object; R4 as prototyped a defect, its seal built | **(4a′)**, built; (4a), (4c) object; (4b) conservative | **(5b) amended**, built; (5a), (5d), (5c) object |
| ffi-pragmatist | **(1b) with (1d)**; (1a), (1a′) **veto**; (1c) object | | | (4c) object; leans (4b) for integers | |
| spec-warden | **(1b)**, 0 spec tokens; (1a), (1a′), (1c) **veto** (Principle 0) | **(2c)** split, -20; (2a) object | (3a) merged into R4 (+21 to +22); (3b) object | the two-clause rule (+22 in design.md); (4b) conservative | **(5b)** short (+23 to +38 over R1); (5c) conservative; (5a), (5d) object |
| historian (advisory) | (1a), (1a′) approve; (1b) object | **(2a)**; (2c) conservative | **(3a)**, with R4's predicate | (4a) | **(5c)**; (5a), (5b) object |
| llm-ergonomist (blind) | not read | A1 (2a) and A3 (2c) approve; A2 (2b) object | **B1 (3a)** approve; B2 (3b) object | not read | **C1 (5a)** approve; C2 (5d), C3 (5c) object; (5b) not shown |

## What the sitting measured

- **Q1.** Through every macro-only name measured on this Mac (`WEXITSTATUS`,
  `htonl`, `FD_ISSET`, `WIFEXITED`), a wrongly declared parameter compiles
  under (1a) and under (1a′), a `double` and a pointer included: a system
  header's macro hides the conversion clang refuses outside it. Declared
  `status: u8`, the wrapper reads 4 bytes from a 1-byte argument
  (`_W_INT(w) (*(int *)&(w))`): AddressSanitizer exit 134, and without the
  sanitizer the program prints `240` where `WEXITSTATUS(1)` is `0`, at exit 0
  (the ffi-pragmatist, reproduced by the critic). Linux gives other verdicts
  for the same file. (1d), the program's own header, works on this Mac and in
  `heroes-linux`, and refuses a wrong width (`ffi_parameter_type`). (1b) was
  built: one clang question asked only after a refused round names the macros;
  `build --emit-c` over the 412 tracked files with an `extern` line reads 206
  and 206 before and after, stderr equal on every line; the five tracked
  `ffi_unknown_name`, none a macro, stay as they are; 91 code lines.
- **Q2.** The parser reads a whole `Statement` on an arm's line already
  (`selfhost/grammar_expr.hero:1165-1209`); (2a) moves no program and no
  compiler line; (2b) stops 23 files checking, the compiler and the harness
  among them, 707 sites; (2c) moves one golden line and adds a diagnostic for
  a loop after `=>`, a shape no probe and no reading found as a mistake.
  `_ = e` admitted costs 4 lines and gives every neighbour its block form's
  message. Today's *`_` would be bound* is false by spec § 5 (defect 153).
- **Q3.** (3a) built as (3x), a check of the construct with no field added to
  the checker's state: 0 of 1,530 files change exit; no level of nesting lost
  on any shape legal today (195, 163, 123, 96, the trunk's); a55 to b8 run
  right (b4 and b7 are other refusals). **Panel 184's R4, as prototyped,
  widens its three path-enders into value positions, and c1, c2, c3 and c6
  then check clean and fail at `build`, exit 2** (the compiler-engineer;
  reproduced by the critic); a 16-line seal (the checker marks those
  statements, the lowering ends the value block after them with
  `.unreachable`) is built and runs them right; its `emission` run is unrun.
  R4 does not reach c5, by its text and by panel 184's choice of route (2d):
  the critic rebuilt (2a) on `03e70520` and the compiler stops checking itself
  on 133 statements, 131 of them after an all-leaving `match`.
- **Q4.** (4a)'s integer clause is falsified by t7, one bulleted arm beside
  `_`: certain, applied, `many` printed where `one` is meant (the
  compiler-engineer, reproduced by the critic). The two-clause rule, deletion
  certain before a string and two guesses before an integer or a character,
  built: no probe gets a wrong program from `--apply`, where today 5 do. No C
  or shell habit writes a spaced sign: 0 against 12,716 attached in 18,998 C
  files (the ffi-pragmatist).
- **Q5.** Of 35 plain literals in the tree holding a hole that parses, (5a)
  refuses 34, 32 of them falsely, and stops 22 files, the compiler among them;
  (5d) 27 false and the same 22; (5b) amended 0 false, the 2 true sites, 0
  files stopped (the compiler-engineer, re-measured, not carried from panel
  184). No bare-brace check has shipped on by default anywhere (the
  historian: RUF027 in preview, Clippy's in `nursery`, Pylint's never merged).

## Disagreements, unsmoothed

- **Q1, the historian against three seats.** It approves (1a′) on GHC's
  `capi` and bindgen's `--wrap-static-fns`; its premise, that the C compiler
  checks the call, is measured false through system-header macros, and a
  soundness veto is a refusal, not a price (CLAUDE.md § 4; § Precedence puts
  §1.12 at rank 3). The precedent stands as what (1b) costs a user: a shim by
  hand, as the Linux kernel's `rust/helpers/` and bindgen's issue #753 show.
- **Q2, the spec-warden against the compiler-engineer and the historian.** It
  approves (2c) on Principle 0, a loop on the arm's line having no decision
  naming it. The critic answers from the record: design.md §4.7 says *one
  statement*, spec § 5's `Statement` holds `While | For`, and Principle 0
  governs what enters, while (2c) is a new refusal owing a measured mistake.
  The blind reading approved A1 and A3 alike and is cited for neither.
- **Q3, does R4 reach c5.** The spec-warden priced (3a) merged into R4 so that
  R4 refuses after an all-leaving `match`; the historian reads it the same
  way; the compiler-engineer and the critic measure that this is panel 184's
  refused route (2a). The record wins.
- **Q4, the historian's (4a)** against the three seats who built the
  two-clause rule; t7 is checkable and was run twice.
- **Q5, three readings of three routes.** The blind reading approves (5a),
  which the compiler-engineer measures at 32 false alarms; the historian
  approves (5c) on the record of every bare-brace check backing away; the
  compiler-engineer and the spec-warden approve (5b). The blind reading never
  saw (5b).

## The resolution: `provisional, author ratification pending`

The most robust and complete one, not the cheapest and not a compromise
(CLAUDE.md § 4); what conservative would have been is written beside each.

**R1. A macro-only C name is refused, and told as a macro, with a repair a
program can use today** (Q1, routes (1b) with (1d)). After a round clang
refused, and only then, one clang question asks which of the undeclared names
are macros; such a name gets its own message, *`<header>` defines `<name>` as
a macro, not a function: a macro has no parameter types for clang to hold this
declaration to*, and a note naming the repair, a `static inline` function in a
header of the program's own that calls the macro, bound by `extern` like any
function. **The note never writes the declared C types into the suggested
shim**: it gives the macro's documented types where the header states them, or
a placeholder the author fills, since a shim drafted from a wrong declaration
is (1a′)'s hole in the author's file. Round one stays first, so a fortified
function and a name that is both a function and a macro keep panel 092's
check. A new code, `ffi_macro_name`, since cause and repair differ from a
misspelling's. Spec § 13 stays true as written (0 tokens). design.md's four
promises of macro reachability (`:565`, `:645`, `:2265-2266`, `:2401-2402`)
are corrected to say how a macro is reached. (1a) and (1a′) are refused under
the ffi-pragmatist's and the spec-warden's vetoes; (1c) under the
compiler-engineer's. *Conservative*: the same refusal with
`ffi_unknown_name`'s message reworded for a macro, no new code.

**R2. Defect 145's two true readings become one** where clang showed no
spelling of the type: the same post-failure question unit asks `typedef NAME
hero_q;`, which tells a type name, a real tag and neither apart (the
ffi-pragmatist's; the critic's first-pass `sizeof` calls a function a
typedef).

**R3. An arm's line holds one statement that names no binding** (Q2, route
(2a)): spec § 8's `Inline` production says what its prose, design.md §4.7 and
the parser say, in the split form that removes the separate production, and
`_ = e` stands on an arm's line, since `_` binds nothing (spec § 5), the
compiler admitting it (4 lines) and its message false today repaired with it
(defect 153). The production's text is the landing's to write and price (the
spec-warden's (2a) split is -11 to -28 vendored as priced; the clause for
`_ = e` is unpriced). *Conservative*: (2a) with `_ = 0` kept a block's line and
defect 153's message made true. (2b) and (2c) are not adopted: (2b) stops the
compiler checking itself, and (2c) is a new refusal no measured mistake
calls for.

**R4. A value block whose last statement is an `if` or a `match` whose every
branch leaves, leaves, and its arm is a jumping arm** (Q3, route (3a), built
as (3x)). Its sentence in spec § 8 says that the arm or block leaves **without
making R4 refuse a statement after it**: R4 refuses only after `return`,
`break` or `continue`, as ratified, and c5 stays legal. The sentence is
unpriced (the seats' merged texts made R4 reach c5); the landing writes and
prices it. *Conservative*: (3b), the refusal with a message naming the
`return` on the arm.

**R5. Panel 184's R4 lands with the seal, its path-enders reaching value
arms**: one predicate for a function's end, a value arm and a value block, so
that the three can never disagree; the lowering ends a value block after an
`exit(code:)`, an `assert false` or an unbroken `while true` with
`.unreachable` (16 lines, built by the compiler-engineer), and the `emission`
suite is run on it before it lands; the defect-139 golden whose four `assert
false` value arms stop being refused is rewritten by hand; `check/walk.hero`'s
decided ceiling (14 lines over, by arithmetic, unbuilt as one tree) is met by
a seam or a ceiling decided with its reason. *Conservative*: R4's widening
kept at a function's end, c1, c2, c3 and c6 refused as today.

**R6. A spaced `-` before a string is deleted with certainty; before an
integer or a character, both fixes are guesses** (Q4, the seats' two-clause
rule, labelled here R6 and not (4a)): design.md §4.15's sentence is amended
to it (+22 in design.md, by the spec-warden), §4.10's rule (*two readings
make two guesses*) applied where it binds. Spec § 8's `Pattern` is not
tightened: the parser accepts `-"a"` and the checker refuses it by type, so
production and parser agree. The compiler's own test at
`selfhost/parse.hero:383` and the defect-123 golden's `.fixed` (to an
`.applied`, by hand) move with it. *Conservative*: (4b), two guesses before a
string too.

**R7. The forgotten `f`: route (5b) amended is the measured-robust route, and
it does not land until the author's condition is met** (Q5). It refuses a
plain literal where a brace hole names bindings all bound where the literal
stands (0 false alarms and 2 true sites over the tree, 0 files stopped), its
fix doubling a text `}` as well as a `{` once R1 of panel 184 has landed.
**The condition the author set on 2026-10-01 is unmet**: no blind reading of
any (5b) wording exists, and this sitting showed (5b) to none. So the
sitting asks the author one of two things, below. Until then R1 and R2 of
panel 184 land as ratified, and a forgotten `f` is told by R2's note. (5a)
and (5d) are not adopted: they stop 22 files checking, the compiler among
them (Principle 0), for no site (5b) misses. (5b)'s falsifier, to carry: a
template whose placeholder names a binding in scope (the tracked template,
`examples/template/main.hero`, has none, one program).

**R8. Where it is written**: spec § 8 (R3's production, R4's sentence), spec
§ 2 (R7's sentence, if it lands, beside R1's); design.md §1.11, §3.1, §4.19
(R1's four sentences), §4.15 (R6), §4.7 (R4); `docs/work/DEFECTS.md`: 143 and
145 close with R1 and R2, 147 and 153 with R3. Each lands in a batch of its
own lane with its cases, and R1 at the C boundary is run on Linux x86-64,
Linux arm64 and the Windows box before it is called done (no route was run on
Windows or arm64 by this sitting).

**What a veto would compel**: (1a) and (1a′) are refused outright; the
compiler-engineer's veto on (1c) compels a written answer, given above.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | with R1 landed, `build --emit-c` over the tracked `extern` files moves 0 exits and 0 of the five `ffi_unknown_name`; a golden binding `WEXITSTATUS` reads `ffi_macro_name` on macOS and Linux x86-64 | R1's landing and its Linux leg |
| compiler-engineer | under R3 with `_` admitted, the census moves 0 files, and `check/walk.hero`'s `fall_through()` can be replaced by `_ = 0` on an arm's line with the compiler's own tests green | R3's landing |
| compiler-engineer | with R4 and R5 landed, the census changes exit for exactly the files R4 alone changes (5 today, all under `docs/panel/184-briefs/`), and c1, c2, c3, c6 and a55 to b8 (b4, b7 excepted) build and run | R4 and R5's landing |
| compiler-engineer | under R6, `check --apply` on every probe of `probes/q4/` and on t7 writes no program that prints other than its meant output | R6's landing |
| compiler-engineer | with R7 landed after R1, the census moves exactly 1 file (`docs/panel/184-briefs/blind/task1.hero`) and 0 change exit | R7's landing |
| ffi-pragmatist | on an (1a) build, `WEXITSTATUS(status: u8)` builds on macOS and its `--sanitize` run aborts (scored now: reproduced, exit 134) | scored |
| ffi-pragmatist | on R1, the 412 files still split 206 and 206, and the 206 emit byte-identical C | R1's landing |
| spec-warden | R1, R4 and R6 of panel 184 together add +107 vendored; the stack this sitting approves adds about +132 | the landings, by `heroes measure` |
| llm-ergonomist | under A2, at least 30 per cent of task A's first attempts fail on the rule; under B2, at least 40 per cent of task B's; under C3, at least 10 per cent of task C's are silently wrong | a generation run, unrun |

## Author's verdict

**PENDING.** Queued as `panel 185` in `docs/work/DECIDE.md`: ratify, amend or
overturn R1 to R8, and for R7 choose (a) one blind reading of (5b)'s wording
(one session, 3 USD cap, this sitting's cost 0.61 USD), landing (5b) if it
approves, or (b) a ruling that the reading's earlier objection, a locality of
compiling, falls outside the condition, since the author's ruling of
2026-10-01 put legality outside design.md §1.3, landing (5b) after R1.

## The critic's two passes

`185-reports/completeness-critic-briefs.md` (the briefs, before any seat:
ten repairs, every one taken, among them that `heroes cc` is not built, that
the parser already implements (2a), that (2b) refuses 600 lines of the
compiler, and that lane 135c's rule has three clauses) and
`185-reports/completeness-critic.md` (the reports, before this synthesis: R4
reproduced breaking `build`; R4 kept off c5; (1a)'s hole named as an
out-of-bounds read; the shim's types; the unmet condition on (5b); the blind
reading cited for neither (2a) nor (2c); (4a) falsified; the `typedef` query).
It changed this resolution in R1, R2, R4, R5, R6 and R7. Of the four
findings it asked filed, three are filed in `63f53ca5` (a C object declared
as an `extern` function, defect 152; the false *`_` would be bound*, 153;
glibc's `FD_ZERO`, under 143), beside 151, 154 and 155 from the lanes and the
CI; the fourth, R4 reaching value positions, is no defect of the trunk, R4
being unlanded, and is R5 above.
