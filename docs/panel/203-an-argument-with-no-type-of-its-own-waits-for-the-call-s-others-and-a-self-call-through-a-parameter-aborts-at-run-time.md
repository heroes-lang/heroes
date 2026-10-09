# Panel 203: an argument with no type of its own waits for the call's others, and a self-call through a parameter aborts at run time

Convened 2026-10-09 by the coordinator on the author's yes of about 22:47
(the question widget: *both sittings now, 6 USD between them*), under the
author's instruction of about 19:35, *every defect closed*, for defects 541
and 547, the two panel 201 filed apart. **A full panel** (both questions
change what `check` refuses or accepts): the compiler-engineer, the
spec-warden, the historian, the blind seat in six fresh `claude -p` sessions
outside the repository (1.5210 USD; the two sittings' blind seats 3.9496 of
the 6 approved), and the completeness critic before the seats and after them.
The tree frozen at **`46b80b82`**, worktree `lane-panel-203`. Briefs written
from 22:54; the critic's first pass read by 23:06, its repairs applied before
any seat started (the fourteen probes named by file and two added, N1f and
design.md §4.12 falling with the general route, the literal's width asked,
Q2's neighbours through a built-in and a record field measured); seats from
23:06; the historian's reply copied at 23:22, the spec-warden's at 23:27; the
session limit from about 23:31 to 23:46 stopped the compiler-engineer,
resumed on the new account; the blind seat's Q1 sessions from 23:49:30 to
23:50:55, its Q2 sessions from 00:00 to 00:01:13 on 2026-10-10; the
compiler-engineer's reply copied at 00:00, the critic's second pass from
00:02:15 to 00:11:26, copied at 00:12; this synthesis from 00:20, every time
read from `date`. Briefs in `203-briefs/`, reports in `203-reports/`.

## The verdict table

| seat | verdict | on what |
|---|---|---|
| compiler-engineer | Q1 **approve** the general route, its third version (v3), on condition `walk.hero`'s 59 lines over its ceiling are paid; Q2 **object** to the summary route it built; no veto | three versions of the general route and the summary route built in its copy, the census of 3,163 files, the compiler's own tests, instructions retired of `check selfhost/main.hero` in three interleaved runs |
| spec-warden | Q1 **approve, provisional**, sentence G3a replacing N1f, **object** to G0; Q2 **approve** no sentence, **object** to Q2a and Q2b | `heroes measure`, seven drafts, 45 probes through two of the compiler-engineer's prototypes |
| historian (advisory) | Q1 **approve** the general route on two conditions, a verdict independent of argument order and each argument typed once, no verdict on the literal; Q2 **object** to following a parameter through its callers, **approve** the run-time abort, the call-site summary *not objected, a departure* | Pierce and Turner 1998; Odersky, Zenger and Zenger 2001; JEP 101 and 215, JDK-8078093 and 8077247; Go 1.21 and #50285; Rust RFC 212; Swift; C# 12; TypeScript; rustc's lint; Holzmann; MISRA 17.2; GCC's analyzer; Lean 4.18; Nim's `effectsOf`; Dialyzer; Zig #1006 |
| blind seat | Q1: N1f, 2 of 2 accept lines 21 and 25, which the checker refuses; G3a, 2 of 2 accept all six lines, line 25 among them, which v3 refuses; Q2: under the refusal its reader adds a base case, under the run-time panic its reader removes the call, one reading each | `llm-ergonomist-scoring.md` |
| critic, second pass | v3's verdict depends on argument order where two arguments both wait; v3 changes the IR of none of 1,610 programs; line 25 is spec § 6's unwritten rule; a sentence true of v3 is +38/+40; a seam of 67 lines exists; cost linear in nesting; the census gain is 5 distinct programs; Q2's locality has precedent in `may_end`; the abort's note is lost at `-O2`; `y: u8 = 2 + 3` is a defect against a ratified ruling | the engineer's binaries copied, run against the frozen runtime; `build --dump-ir` over 1,610 files; nesting chains to depth 80 |

## What the sitting measured

- **Q1, 541: the general route as built.** v3
  (`tree/selfhost/check/waiting.hero`, 244 code lines, and `settle_waiting`
  and `synth_argument` in `walk.hero`, +81 / -10): an argument with no type
  of its own (a number literal, a generic function's name, `ok`/`fail`, a
  generic call, `[]`/`{}`, `.case`, an array or map holding one) waits while
  the call's other arguments settle the callee's letters, then is checked
  against its parameter with them filled in; with nothing settled, the
  context first, then a generic call, then a literal at `i64`/`f64`. It
  accepts the fourteen probes, `literal2`, `m1`, `m3`, f1 (§ 9's own `fold`
  over a `[u8]`) and runs each to its value; `nestedcall` stays refused.
  The census of 3,163 files moves 10, every one toward acceptance or a
  truer message, none refused that was accepted, no exit 2; `build
  --dump-ir` of the 1,610 programs both compilers accept differs in 0 (the
  critic). The compiler's own tests: 1,542, 1 moved as expected. `check
  selfhost/main.hero` +0.08%, inside the base's own spread of 0.2%; nesting
  to depth 80, linear (the critic).
- **v3 is not order-free.** `first(a: [1], b: [])` is accepted and `first(a:
  [], b: [1])` refused `cannot_infer`, and so `{"k": 1}` with `{}`, and
  `[[1]]` with `[[]]`, on all three versions (the critic, `w/e1a` to
  `e3b`). The engineer's eight order pairs each had one argument with a type
  of its own. The cause read from the fallback, not instrumented: the forms
  that refuse themselves are tried by position within their class.
- **What it still refuses**: `apply_all(fs: [ident, half], x: 200)` with
  `half(x: u8) -> u8`, the literal settling `i64` before the array can say
  `u8` (G3a is false on it); `[].count()` (r1) while `count([])` is
  accepted, so § 9's *`x.f(y)` is sugar for `f(x, y)`* is false today (the
  spec-warden; defect 551's repair deferred the receiver to a sitting);
  `m2`, `y: u8 = first(a: 200, b: 2) + 100`, because the operands of `+`
  get no context (the spec-warden). `genT2`'s one mistake is told twice
  under v3, once under the base.
- **What a literal does.** v3 consults the context before a literal's `i64`,
  so `y: u8 = first(a: 1, b: 2)` is a `u8`; where it then overflows it
  aborts 134 at run time (`ovf`, `retctx`), and a literal that cannot fit is
  refused `int_out_of_range`. Rust and Swift wait for the context, as spec
  § 2 already does outside a generic call; Go settles an untyped constant
  only among the call's arguments and refuses inference from the result, on
  readability (Taylor, #50285).
- **Line 25 is § 6's gap.** `x = ok(1)` is refused on the base and on v3,
  panel 002 having `ok(...)` take its type from the context only (design.md
  `:1229-1236`); spec § 6 (`:159`) never says so, so 4 of 4 readers, in both
  arms, accepted line 25. Line 21's disagreement is N1f's own: 2 of 2 read
  *a generic function's parameter* as a parameter of generic type.
- **The sentence.** G0 (N1f removed) is byte-identical to the text before
  panel 201 and reads 9,831 real, -16; G3a +4/+5 vendored, false on
  `apply_all` and on line 25; the critic's **V3T** with **§ 6's clause**,
  true of v3 once the order is repaired, +38 legacy and +40 cl100k, the
  real count unmeasured (about +50 by the document's ratio, an inference),
  against 333 spendable tokens (panel 202 spends none).
- **`walk.hero`**: 1,868 to 1,929 code lines, 59 over its decided 1,870
  (`tests/harness/suite_layout.hero:479`). The engineer: the new code calls
  `check` and `synth` and cannot move. The critic: seven existing functions
  of `walk.hero` call none of its functions, about 115 code lines, among
  them `field_of_function_type` (46, already reached from `may_end.hero:150`
  and `self_call.hero:215`) and `synth_name` (21), 67 together; unbuilt, and
  whether they reach anything private unchecked. `walk.hero` has been raised
  four times with room, each for a ratified form or a repair
  (`suite_layout.hero:100-135`).
- **Q2, 547: the summary route as built.** `check/param_calls.hero` (102
  code lines) and changes to `self_call`, `self_walk`, `endless`,
  `endless_cycle`, about 205 code lines (`self_call.hero` past 300, a split
  owed), +1.3% on every `check`: it refuses `param_value`, `chain`, `pair`,
  `ufcs`, `xmod`, keeps the three correct shapes, moves 0 of 3,163 files,
  and misses a record field (`viafield`), `map` (`viamap`) and a local alias,
  which still abort 134. `xmod`'s refusal depends on another module's body;
  `may_end` already judges a call through a function's own parameter by what
  is handed in, over the whole program, ratified by panel 199 R4 (the
  critic). The run-time answer: all three shapes abort 134 at `-O0` and
  `-O2`; at `-O0` the panic names *the recursion of paramvalue.go and
  paramvalue.step*, at `-O2` only *stack exhausted in paramvalue.step*, and
  `viamap` loses its `map` frame.
- **`y: u8 = 2 + 3`, `show(v: 2 + 3)`, `e: u8 = 2 * (3 + 4)`, `d: f32 = 0.5
  * 2.0`** are refused on the base and on v3; `1 + b` and `(2)` are
  accepted. Panel 042 (ratified 2026-08-12), its lines 251 and 252, priced this
  residual inside its ruling 3 (*full adoption*, line 286), and it never
  landed.

## Disagreements, stated plainly

- **Is v3 order-free?** The compiler-engineer measured 8 pairs and said yes;
  the historian made it a condition; the critic falsified it on pairs of two
  waiting arguments. The resolution makes the repair a condition of landing,
  the critic's pairs its witnesses.
- **G3a or V3T.** The spec-warden recommended G3a for v2; it is false on
  `apply_all` and on line 25 (the critic; the blind readers of G3a accepted
  25, 2 of 2). V3T names the forms that give no type and the literal's
  place; § 6's clause carries the context rule G3a cannot.
- **Context or literal first.** Rust, Swift and spec § 2 say context first;
  Go says the literal, for the reader. v3 takes the context, the only order
  under which spec § 2's sentence stays true inside a generic call; Go's
  line is the conservative alternative below.
- **Q2.** The compiler-engineer objects to the summary route on its cost and
  its partial reach; the historian objects to following a parameter through
  its callers and not to a call-site summary, which it calls a departure; the
  blind seat cannot rank the two at one reading a side, so the engineer's
  condition (*measurably more often*) is unmet.

## The resolution, ratified by the author (below)

The most robust and complete route at every question (CLAUDE.md § 4,
CL-040); what conservative would have been is below the list.

1. **R1, defect 541: the general route lands, v3, in batch 18, only with:**
   - **(a) the order repaired**: the forms that refuse themselves tried after
     every form that can settle a letter, so the verdict of every pair is
     the same in both orders, the critic's `e1a` to `e3b` and the engineer's
     eight pairs its witnesses;
   - **(b) the receiver typed as the argument it is**: in `x.f(y)` the value
     before the dot is checked against `f`'s first parameter as every
     argument is, so `[].count()` is accepted as `count([])` is and § 9's
     *sugar* is true; unbuilt, held to the census like the rest, and filed
     `blocking` on its own if it does not build, U1 refused either way (it
     would write a defect down as a rule);
   - **(c) the sentence**: § 9's lines 277 to 280 become V3T, *A type
     parameter takes its type from the arguments that have one of their
     own, else from the type the context asks for, else from a generic call
     or a literal among them (section 2); `ok(...)`, `[]` and a case name
     give none, and a call that says none of these is an error.
     `xs.map(double)` takes both from `double`'s signature.*, and § 6's line
     159 reads *`ok(v)` or `fail(code:, msg:)`, each taking its type from the
     context*; +38/+40 on the vendored tables, a lower bound, the real count
     by one `--refresh` at the landing on the author's yes, re-argued above
     +60; the spec suite and the spec-warden's 45 probes and the order pairs
     run against the sentence before it lands. N1f leaves;
   - **(d) design.md §4.12** amended with the spec-warden's draft
     (`203-spec-warden/drafts/d412_new.txt`), recording that panel 105's
     *Flat only* is reversed for every argument with no type of its own;
   - **(e) the messages that quote N1f** (`generic_argument.hero:172`,
     `function_value.hero:224`) and the goldens they and the route move (402,
     415, 544, 546, *a-message-never-shows-a-table-index*) edited by hand,
     each with its annotations, and the compiler's own test near
     `walk.hero:2305`;
   - **(f) `walk.hero`'s 59 lines** paid by moving `field_of_function_type`
     and `synth_name` to a module of their own where they reach nothing
     private, else by a raise written into `suite_layout.hero`'s table with
     its reason, as the four before it;
   - **(g) `genT2`'s mistake told once**, as the base tells it;
   - **(h) the census** moving the engineer's 10 files and no other, and the
     IR of the programs both compilers accept unchanged.

   It closes 541; f1's message printed twice today and f4's three times are
   rows of 541's cause and close with it. Defect 552's repair in lane
   b18-close lands first, and v3's landing re-reads it.
2. **R2, defect 547: the run-time abort is the answer**, panel 199 R2's
   ruling at every level; no compiler change. 547 closes on its measurement,
   *The repair* naming this ruling and the summary route measured (205
   lines, +1.3% on every `check`, 5 shapes refused, 0 tracked files, 3
   shapes still missed), with a `run` witness for each of `param_value`,
   `viafield` and `viamap` pinning the abort, as panel 201 R3's witnesses
   pin 520's. The critic found no defect closed with no change to the
   compiler, so **closing it so is the author's question**, put with this
   ratification and answered *close it on the measurement*. Filed beside it:
   565, the panic at `-O2` losing *inside the recursion of* (`adjacent`, a
   true message less exact than at `-O0`).
3. **R3, `y: u8 = 2 + 3`: a defect, `blocking`, area `check`, against panel
   042's ratified ruling 3**, filed now as 564 with its neighbours to attack in one
   pass (unary `-`, `%`, the shifts, a comparison where neither side has a
   type, an `if` or `match` value in an annotated position). No spec change,
   so no sitting. It lands in batch 18 beside R1, and with both landed `m2`
   becomes the overflow abort the critic's first pass predicted, a `run`
   witness pinning it.
4. **R4, the spec**: V3T and § 6's clause alone. G0 (it leaves the
   literal's place and the forms that give no type to the reader's guess),
   G3a, G3b, G3c, J1, U1, Q2a and Q2b refused.

**The conservative alternative, the author's to choose instead**: the
checker unchanged and 541 closed as a known refusal; N1f reworded so line 21
reads as the checker does (*every parameter of a generic function, whatever
its type*) and § 6's clause added. **A narrower alternative on the literal**:
q1sc, v3 with a literal settling its letter at once, Go's line, `m1` and `m3`
then refused. **On 547**: the summary route as built, five shapes refused at
compile time for 205 lines and 1.3% on every `check`, the abort still owed
for the shapes it misses.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | Q1: the landing census moves exactly the 10 files, `check selfhost/main.hero` within +0.5% of the base in instructions retired | R1's landing |
| critic | V3T with § 6's clause near +50 real at the `--refresh`, an inference from the document's ratio | R1's landing |
| spec-warden | P3: no file goes from accepted to refused, no `run` golden changes its output | R1's landing |
| historian | Q1: a language that shipped this inference and withdrew it would turn its reading; none found | open |
| compiler-engineer | Q2: a blind measurement where readers fix `go` or write the parameter shape more often under the refusal than under the abort would move the seat to approve | unrun |

## Author's verdict

**RATIFIED, 2026-10-10**, R1 to R4 as written above, the author answering
through the question widget between 00:21 and 00:24 by the clocks read
before the question and after the answer, choosing *Ratifica R1-R4* over
*the compiler does not change* (N1f reworded, 541 a known refusal),
*a literal at once* (Go's line) and *I want to read it first*, on the coordinator's summary of each route; in the same widget the author answered *close 547 on the measurement*
over *build the summary* and *leave it open*, and *yes, one* to the
`--refresh` R1's sentence needs at the landing.
Recorded as a reading (CLAUDE.md § 4). The author may overturn it.
