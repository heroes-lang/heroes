# Panel 014 — may a `match` arm's body be a statement?

Date: 2026-08-04. Trigger: found at M2 step 2 by parsing design.md's own
appendix — the M6 acceptance program — for the first time. Queued as a design
question with the conservative default already implemented, and convened in
the debrief of the same day on the author's instruction (`arm → panel`).

Four judges. The ffi-pragmatist has no standing: the question adds no C, no
ABI surface and no `extern` form, and panel 013 already established that arm
syntax never reaches the emitter.

## Proposal (as put to the judges)

§4.7 and `spec/heroes-spec.md`: "An arm is an expression or an indented block;
a block's value is its last expression." `assert` is a **statement**, so this,
from the acceptance program, does not parse:

```
    match tokenize("2 $ 3")
        .ok _  => assert false
        .err e => assert e.code == "unknown_char"
```

It occurred in **8 places**. The conservative default was implemented
(rejected) and the appendix rewritten with block arms.

- **P** (recommended) — an arm's body **is a block**: one statement inline, or
  an indented block. `=> assert false`, `=> break`, `=> return 1` become
  legal. Measured spec cost as proposed: **+1**.
- **Q** — status quo. 0 tokens, "0 lines", already shipped.
- **R** — make `assert` an expression of type `()`, so the arm rule need not
  change.

One rationale was put to the judges with the proposal and is **struck from the
record** by the compiler-engineer's condition 1: that M4's desugaring of `?`
produces an arm whose body is a `return` and that the current tree cannot
represent it. Both halves are false — `ArmBody::Block([Return])` represents it
today (verified), and design.md:1640 makes `?` its **own** exit edge in the
ownership pass, so `?` need never become an AST arm at all. A change justified
by a constraint that does not exist would be justified again by another one.

## Verdicts

| Judge | Verdict | Cost/delta | Prediction | Condition |
|---|---|---|---|---|
| compiler-engineer | **approve P on cost, object to its rationale · VETO R · object to Q** ("not 0 lines") | P **built and measured in a scratch clone**: +52 −42, **net +10 lines across 5 of 49 files**, suite green with **zero** test-expectation and **zero** golden edits. Landing zones: lexer 0 · checker 0 · descriptors 0 · ownership 0 · emitter 0. R: a new expression form, a new §4.14 precedence row, and a new type judgment at *every* value position (`f(assert x)`, `[assert x]`, `return assert x`), because `x = assert y` is not statement position and panel 003's ⇐-`()` check never fires there — plus Part 5's "`assert` → `if` plus `panic`" has no value to give a three-address temp | at M4 close: **zero** lines attributable to arm-body shape — no third `ArmBody` variant, no bottom type in `types/`, `?` lowers as an exit edge. At M3c close: the arm-body value rule costs **≤25 lines**, the same order as panel 003's own estimate, since both are one ⇐-check. Falsified if either needs a bottom type or a reachability pass | (1) the panel record **strikes** the false `?`/M4 rationale; (2) design.md §4.7 gains one sentence on the diverging arm **before M3b** — "an arm whose body cannot produce a value (`break`, `continue`, `return`) is admissible; the `match`'s type comes from the arms that can" — because otherwise `x = match k` with `.a => return 1` reaches M5a relying on `-Werror=uninitialized` to catch a frontend hole, the backend covering for the checker |
| llm-ergonomist | **approve P (variant 2)**, no veto | over four tasks: arm-lines **14 → 9**; first-try compile-clean **2/4 → 4/4**; on all-expression arms (the common case) the output is **byte-identical**, so P costs nothing where `match` is used most; silent wrong programs in what it actually wrote: **0 both** | first-try on check/control-flow arms **≈55% (Q) vs ≈85% (P)**, +25 ± 10 points; on all-expression arms **delta 0 ± 3**; arm-lines per match **0.6×**. Belief probe: "is `.err e => assert e.code == \"x\"` legal?" — Q ≈50% correct (a coin-flip on undocumented `assert`), P ≥90% | the diff carries the ~30-token cost of **enumerating what a statement is** (the word appears nowhere in the spec) and of the **value rule** — "a `match` used as a value requires every arm to produce a value" — which is the only sentence in this panel that converts a silent wrong program into a loud one |
| spec-warden | **object to P as worded**; proposes **S** (parser fix, 0 spec tokens); rejects R on the number | measured: baseline **2048**; P as proposed **2049 (+1)**; P *honestly worded* **2063 (+15)** or **2070 (+22)** once reconciled with §4.14; **R needs two sentences → 2086 (+38)**, 38× P's advertised price, and still leaves `=> break` illegal. Named removal found and measured: the spec says "`if` is an expression" **twice** (lines 105 and 136) — cutting the duplicate is **−7** | if P lands as worded, M6 will need the §4.14 reconciliation clause and the binding count will read **≥2065**, not 2049 — the +1 revealed as ≈+22 within one milestone. If S lands, the 8 appendix sites revert to inline form and the spec still measures **2048** at M3 close | S ships at 0 spec tokens with a golden pinning `=> assert false`, **or** P lands as the *enumerated* wording paired with the measured −7 dedupe, net ≤ **+8** |
| historian (advisory) | **approve R extended** (also classify `break`/`continue`/`return` as diverging expressions); **no objection to P on historical grounds** — the objection is cost | 30 sourced rows. Rust arm body is an expression, and `=> break` is legal *because* `break` is "diverging and has a type of `!`" (Reference); same in Kotlin (`Nothing`) and Zig (jumps are `PrimaryExpr`). **Kotlin's grammar is P verbatim** (`controlStructureBody : block \| statement`), and Scala's `CaseClause => Block` has been P since 2.x — P is not unprecedented. `assert`-as-a-statement is the anomaly: only Java and Python, neither with an expression-valued `match`; **D** has `assert` in the expression grammar with type `void`, Zig makes it an ordinary function | under R the appendix parses clean with **zero** change to the arm rule. Under P, M3b/M3c will still need the value rule P appeared to avoid: **P ⇒ ≥1 arm-specific checker rule, R+divergence ⇒ 0**, arms reusing ordinary unification with the jumps bottom-typed | withdraws if Heroes turns out to have no `()` in expression position (it does — `print(x)` parses as an expression statement, and panel 003 makes statement position a ⇐-check against `()`), or on a documented regret over block/statement arm bodies in Kotlin or Scala (searched; found only the exhaustiveness regret, KT-47709, to which Heroes is immune) |

## Findings that changed the question

**Q is not free, and "0 lines" was the wrong number.** The compiler-engineer
built P and, in doing so, measured three defects the status quo ships:

1. **`heroes fmt` silently deletes code.** An arm whose body is a control form
   — `0 => if x > 0` with its branches below — formats to `0 => if x > 0` with
   the branches **gone**; with a nested `match` the output no longer parses.
   Reproduced before writing this file.
2. **`heroes parse` never terminates.** `1 => for x in xs` plus its body
   loops forever (exit 124). Reproduced, and **fixed in its own commit** ahead
   of this resolution, since it is true under P, Q and R alike.
3. **A cascade.** `=> assert false` cost 2 diagnostics, 16 for the appendix's
   8 sites; fixed in the same commit.

All three have one cause, and it is the special case P deletes: `ArmBody::Expr`
is the **only** body position that renders through `render()` instead of
`valued()` — and `valued()` is the only function that knows how to print a
block under a control head.

**The guard Q appears to provide does not exist** (llm-ergonomist, reading only
the spec). Forbidding `=> assert false` inline forbids nothing, because the
*block* form re-admits the same valueless arm one line lower:

```
tag = match t
    .eof =>
        break        # identical hazard, legal under Q today
```

Both readings are equally silent on `tag = match t` with an arm that produces
no value — and the dangerous body is not `break` (divergent, catchable) but
`.a => v @ v + 1` or `.a => print(x)`, which do not diverge. **The missing rule
is orthogonal to the spelling**, belongs to the checker, and is queued for M3c.

**Two judges found the same hole from opposite directions and priced it
oppositely.** The warden's S — `assert` is sugar for `if` + `panic` (Part 5),
`if` is an expression, so the spec's existing sentence already admits
`=> assert false` at **0 tokens** — is R's parser half. Its own report notes
that nothing rejects `x = assert y`; the compiler-engineer's veto of R names
that same hole as disqualifying, plus a §4.14 precedence row and a
three-address temp with no value. Cheap in spec tokens, expensive in the
language: S dies with R.

**Nobody has to widen the grammar to get `=> break`** (historian). Rust,
Kotlin and Zig classify the jumps as expressions with a bottom type; C# added
*throw expressions* in 2017 and then shipped expression-only switch arms in
2019 — reclassify the form, keep the grammar. And RFC 1216 records the exact
wrong turn to avoid: typing diverging expressions as `()` because the compiler
"doesn't have a sensible type to assign to them". Heroes has not decided the
classification of `break`/`continue`/`return` at all; §4.7's new sentence
(condition 2 above) is where it gets decided, and it must not be `()`.

**Every language that widened arm bodies while keeping the construct an
expression paid a documented tax** (historian): Java invented `yield` after a
mid-flight redesign from `break-with`, and forbade jumps crossing the
expression boundary; Swift, whose cases are statement lists, had to restrict
SE-0380's arms to "a single expression". Heroes escapes the tax for the same
reason Scala and Kotlin do — "a block's value is its last expression" is
already in the spec.

## Resolution — **P**, provisional, author ratification pending

Adopted: **an arm's body is one statement, inline, or an indented block.** It
enters on §1.7 alone — *"does it remove a special case from the compiler?"* —
and it does: the `ArmBody::Expr` bypass, and with it a formatter that deletes
code, a parser that hangs, and a doubled diagnostic.

- **R is vetoed** by the compiler-engineer. **S dies with it**: cheap in spec
  tokens, but it is R's parser half and inherits R's hole.
- **The `?`/M4 rationale is struck**, per condition 1.
- **design.md §4.7 gains the diverging-arm sentence now** (condition 2, and
  the historian's RFC-1216 warning): a jump is admissible as an arm body and
  the `match`'s type comes from the arms that can produce one. design.md is
  the living document and carries no token budget; the *spec* sentence for the
  same rule is measured below and deferred to M3c, when the checker exists.
- **The value rule is queued for M3c** (llm-ergonomist), where it is one
  ⇐-check and, per the engineer's prediction, ≤25 lines.

### The v1 amendment text (measured, not applied — spec v0 stays frozen)

```
- An arm is one statement (`assert`, `break`, `continue`, `return`, a mutation,
  a call) or an indented block; a block's value is its last expression.
```

The enumeration is required by two independent conditions: the word
"statement" appears nowhere in the spec today. The rewrite also absorbs the
warden's named removal — the second "`if` is an expression too" (the sentence
at line 136 already says it).

| variant | claude-legacy | cl100k | binding max | Δ |
|---|---|---|---|---|
| v0 as frozen | 1989 | 2048 | **2048** | — |
| P as first proposed (bare wording) | 1990 | 2049 | 2049 | +1 |
| **P enumerated, dedupe absorbed** | 1999 | 2058 | **2058** | **+10** |
| + the value rule (deferred to M3c) | 2017 | 2076 | 2076 | +28 |

**The warden's threshold is missed by 2 tokens** (it asked for ≤ +8; the
measurement is +10) and that is recorded rather than smoothed: its *governance*
rule — above soft 2000, an addition needs a named removal **or** a
pre-registered falsifiable prediction — is satisfied twice, by the −7 removal
folded into the sentence and by three judges' pre-registered predictions. The
final v1 wording is re-measured when the v1 package lands.

**What a veto would have compelled.** The engineer's veto of R stands: had R
been adopted, `assert` would have entered the expression grammar with no
rejection site for `x = assert y`, a §4.14 precedence row to invent, and a
Part 5 desugaring with no value for the IR — for zero expressiveness over P.

## Predictions to score

| # | Judge | Prediction | Checkable at |
|---|---|---|---|
| 1 | compiler-engineer | zero lines attributable to arm-body shape in desugar/lowering; no bottom type in `types/`; `?` lowers as an exit edge | M4 close |
| 2 | compiler-engineer | the arm-body value rule costs ≤25 lines in `types/` | M3c close |
| 3 | llm-ergonomist | first-try on check/control-flow arms +25 ± 10 points for P; **0 ± 3** on all-expression arms; arm-lines 0.6× | first Part 11 run |
| 4 | spec-warden | if P lands as *bare* wording, the binding count reads ≥2065 by M6, not 2049 | M6 |
| 5 | historian | P ⇒ ≥1 arm-specific checker rule; R+divergence ⇒ 0 | M3c close |
| 6 | historian | under P, if inline bodies ever admit *declarations* (Kotlin's grammar does), a binding scoped to one arm becomes legal and useless — Rust keeps declarations out of expression position deliberately | M3a/M3c |

## Watch list added

- **A `()` value in binding position is unrejected** (`x = print("a")` today,
  `x = assert y` under R). Pre-existing, shared by every option, and it wants
  its own panel priced on its own.
- **Declarations must stay out of inline arm bodies** (historian, prediction 6).
  Heroes' `x = 5` inside an arm would bind a name nothing can read.
- **`heroes fmt` had no test with a control form in an arm body.** The defect
  that deleted code was reachable in three lines; the acceptance program is
  what would have caught it, and it did not contain the shape either.
