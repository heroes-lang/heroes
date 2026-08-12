# Panel 035 — Thirteen silences, costed together, and the spec got smaller

**Convened** 2026-08-12, after sweep 001, by author instruction ("vorrei chiudere
tutto prima del prossimo step").
**Trigger** `spec/**`, design.md Parts 1–11, and two diagnostic classes
(CLAUDE.md §4).
**Status** `provisional — author ratification pending`.

## Why they were taken together

Thirteen `Decide:` entries had accumulated in `docs/debrief/QUEUE.md` across
eight milestones — each one a place the spec is **silent** and the compiler had
to choose. They were costed as one package rather than one at a time, because
§1.6's budget is a single pool and **thirteen separate +10s is how a document
reaches its ceiling without anyone deciding it should**.

The result is the opposite of what the count suggests. The package lands at
**2434 → 2422, a net −12**: one deleted line pays for four additions and leaves
change.

## The verdict table

| judge | verdict | its own finding |
|---|---|---|
| spec-warden | **+3 package · veto 2 · refuse 9 at zero** | ran every draft: **nine of the thirteen are already loud**, so they buy no silence. And it found **two silences the thirteen did not contain**, both outranking all of them |
| llm-ergonomist | **object to the allocation, not to any item** | wrote five programs from the spec alone: **confidence tracks risk backwards**. Its least confident guess is its safest; its most confident is the one that runs |
| compiler-engineer | **veto 4b · adopt 4a, 3, 8, 9 · strike 7** | prototyped the M4 repair at **5 lines, 486/486 green**, and found a **live exit-2 defect** next to the proposal: `%` on two `f64` reaches clang as C's `%` |
| ffi-pragmatist | **veto the f64 abort** | compiled the counter-example **in Heroes**: `v @ v * v` eleven times gives `inf`, `v - v` gives `nan`. **No division anywhere.** The proposal's benefit does not exist |
| historian (advisory) | **object 1 · approve 2, 3 · object 4, 5 · approve the budget** | Zig **tried** aborting on float division and moved off it; Python traps and **NumPy overrode it** — the numerical community routed around the language rather than accept it |

## The f64 abort is dead, and three judges killed it from three directions

It was the tidiest idea on the table: abort on `f64` division by zero, and
`inf`/`nan` become unreachable, and every silence about them closes for free.

**The premise is false, and the ffi-pragmatist compiled it in Heroes itself** —
squaring a large value eleven times reaches `inf`, and subtracting it from itself
reaches `nan`, with no division in the program. The compiler-engineer found the
same from the other end: `ir/exprs.rs:47-51` says in its own comment that *"an
out-of-range `f64` literal is an infinity rather than an error"*, and
`emit/ops.rs:53-58` was **deliberately built** to emit `HUGE_VAL` and `(0.0/0.0)`.
Four routes exist; division is one.

**And the spec's own FFI example refutes it.** Line 177 is
`extern function sqrt(x: f64) -> f64`, and `sqrt(-1.0)` is NaN in C and in IEEE
754. Aborting on division does not make NaN unreachable; it makes NaN reachable
**only through the door the compiler cannot check** — a value the language can
neither produce nor name, arriving from libm, which §1.11 makes the source of all
mathematics.

The historian closed it with precedent: **Zig shipped the abort and moved off
it** (issue 395, 2017; the cleanup PR states *"Compile errors for float division
by zero make little sense given that they are perfectly well-defined IEEE
operations"*), and **Python traps while NumPy overrides it** — the numerical
community did not argue, it routed around the language. The real hazard, NaN in
ordered contexts, is already solved here: `sort` aborts on it and, since this
morning, so does a map key.

**What the silence actually needed was one word and one clause.** Spec lines
158–159 were **false**: `print(0.0/0.0)` writes `nan`, which has neither a point
nor an exponent. A false sentence in a 2422-token prompt is the worst possible
spend (§1.4).

## The line two judges found independently, by two different methods

> `- No null. Absence is a different type (\`T?\`).`

The **ergonomist** fell into it: asked for a record holding an optional link to
its own kind, it wrote `next: Node?` on the strength of that sentence, then found
the language has no `none` — so it terminated a linked list with
`fail("no_next", "end of list")`, *a fabricated error code some caller will one
day print to a user*. The line promises an optional; the language ships a result.

The **warden** reached the same line by measurement: it costs **−15** to delete,
and `null` is **already** caught by the lexer with the prescribed text
(`error[reserved_word]: there is no null in this language — absence is a fallible
type: 'int?'`). So the sentence is redundant *and* it is the misleading
invitation.

Deleted. It is the removal that funds the package, and panel 012's rule is
satisfied by a name rather than by a promise.

## The finding the brief did not contain, and it outranked everything in it

The warden went looking outside the thirteen and found that **`main` can fail in
total silence**:

```
$ cat m.hero
function main() -> int?
    return fail("nope", "it did not work")
$ heroes run m.hero
$ echo $?
0
```

No output. No diagnostic. Exit **0** — the one thing a shell can read says the
program worked. Panel 030 had reported the weaker half of this (a `main` that
prints its error still exits 0); this is a `main` that prints *nothing*.

The historian found five languages and one direction: C99 fixed falling off
`main`'s end; Haskell treats any other failure as `exitFailure`; Rust shipped
`?`-in-`main` in 1.26 (2018); Python exits 1 on an uncaught exception; and
**Node.js is this exact situation** — unhandled rejections warned and exited 0
from 2016 until Node 15 (2020) made them fatal, *"four years of 'the error is on
the screen, so it's fine' ended when CI could not see it."* No language moved the
other way.

**Refused, at zero spec tokens**: `main` may not declare a result. Until M7
decides `exit(code)`, a fallible `main` has nothing to mean.

## Four live defects, found beside the questions rather than in them

**D1 — `%` on two `f64` is exit 2.** Spec line 139 grants it (`f64 with f64`) and
the emitter's fallthrough spells every operator the same way, so `7.5 % 2.0`
reached clang as `t3 = t1 % t2` on two `double`s: *`error: invalid operands to
binary expression`* — the compiler blaming itself for a program the spec allows.
Now `fmod`, which truncates toward zero exactly as spec line 135 already
requires, so the C function and the sentence agree without either changing.

**D2 — the nested `match`, carried open since M4 and closed in five lines.** A
`match` as the inline body of another `match`'s arm was rejected with the error
landing on the *following* arm. The cause is one accidental production: after the
inner `match` consumed its arms and its `Dedent`, the postfix loop read the next
arm's leading `.` as a field access. A suffix now never reaches across a block
that just closed. The compiler-engineer prototyped it and counted the payoff —
**45 sites** in the port write `=> match`, against 5 that would need the
bracketed form.

**D3 — the compiler was not in its own build cache key.** `heroes::VERSION` does
not move when the emitter does, so rebuilding the compiler reused the previous
one's artifacts. Two debugging detours in one milestone were stale output read as
real: *a correct fix that appears not to work* is the most expensive shape a
build system has.

**D4 — my own, from this morning.** `hero_str_from_bytes` began validating UTF-8
in sweep 001, which is right — `slice`, `chars` and `len` all rest on it. The
ffi-pragmatist compiled what that does to a **binding**: `getenv` of a Latin-1
value aborts, a PNG aborts, and a SQLite TEXT column another program wrote
aborts, because SQLite does not validate what it is given. That is an abort on
bytes *the environment* chose, not bytes the program chose. Its lifting condition
was two lines: `hero_utf8_valid` is exported, so a binding can branch instead of
dying, and the validator stays single-sourced.

## Resolution — `provisional — author ratification pending`

**R1 — the f64 abort is struck.** Three judges, three grounds, one compiled
counter-example in Heroes. The silence is closed instead by making the spec
**true**: the print row admits `inf`, `-inf`, `nan`, and the abort list says
*integer* division by zero.

**R2 — `- No null. Absence is a different type (T?)` is deleted, −15.** It is the
package's named removal (panel 012) and the defect panel 023 recorded and did not
fix.

**R3 — `main` may not declare a result.** New diagnostic `main_returns`, zero
spec tokens, refused in the checker where a reader looks. Revisit when M7 decides
`exit(code)`.

**R4 — three sentences land and nine items close at zero.** `ok(v)` is named
(closure list, and its absence left `return x` in a `-> int?` function with a
diagnostic carrying no note and no fix); `str` is *indexed **and measured** in
bytes* — the only genuinely silent one of the thirteen; the abort list gains one
word. Everything else — variant construction, `print(match …)`, `INT64_MIN % -1`,
`null`, `print` staying reserved — is **already loud**, measured by running each
draft, and buys no silence for its tokens.

**R5 — `spec/reserved-words.md` is outside §1.6's budget, and gated anyway.**
§1.6 says the spec *"is the prompt"*, singular, and `harness/prompts/first-try.md`
makes it operational; the registry is §4.17's compiler *output* and can never be
in a prompt. But it grew **+85 across eight milestones with no commit naming a
delta** — 831 at M0, **916** today — because `heroes measure` defaults elsewhere.
It gets its own constant beside `SPEC_TOKENS`. Gate it, do not merge it.

**R6 — exponent literals are refused (+23), and the refusal is made safe.**
Principle 0's burden is unmet: a new syntactic form for a type not on the closure
list, with no Part 11 evidence. `1e300`'s message must teach rather than say
`expected ')' … found a name (e300)` — that is what makes the refusal honest
rather than a trap. Queued.

**R7 — the bracketed `match` is vetoed and the nested arm body is adopted.**
Inside brackets the lexer emits no `Indent` (panel 007), so `print(match a … )`
with a nested `match` is a dangling-else with no disambiguator; re-enabling
layout there would break every multi-line array, map and call literal. Two
grammars for one construct is §1.7's exact prohibition. The arm-body half costs
five lines and has nine times the sites.

**R8 — three queue entries are struck as stale rather than answered.** The
`Abort::Assert` gate row was retired at M6 step 7 and `emit/gate.rs:311-321` says
so in its own words; panel 023's C2 was fixed at `dcb22e4`; panel 026's delta gate
**landed** as `measure::gate`. An open item that describes a closed thing is the
same defect class this sweep has been clearing all day.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| spec-warden | at the fixpoint, `grep -c 'ok('` over the Heroes-source compiler is **≥40** and `grep -c '[0-9]e[+-]\?[0-9]'` is **0** — every token spent on f64 edges would have been spent on a type the compiler never types | M8c |
| spec-warden | `reserved-words.md` exceeds **950** by M9 if it gets no constant of its own | M9 |
| llm-ergonomist | task-1 (build a variant) first-try compile is **≤50%** today; ~20 tokens would take it to **≥95%** — a +45pp swing, and the panel spent none of it, so the number stands as the cost of that refusal | harness run |
| llm-ergonomist | of models that print a computed `f64` and assert its text, **≥40%** write the wrong expected string. This is the only prediction whose delta is in *silent* errors | harness run |
| llm-ergonomist | closing all of "Rank 3" in the spec moves the silent-error rate by **0.00** | harness run |
| compiler-engineer | the five-line postfix guard stays under 8 lines and `syntax/expr.rs` under 200 at the fixpoint. Falsified if closing it ever touches `lexer/layout.rs` or `syntax/control.rs` | M8c |
| ffi-pragmatist | at M7, ladder step 2 produces `inf` or `nan` on the first program calling `pow`, `log` or `strtod` with ordinary data. Falsifiable by exhibiting a Heroes expression that yields `inf` without an `extern` — under the struck rule there is none | M7 |
| historian | all thirteen resolutions will be **purely additive; not one will delete a clause**. Oberon is the only budget in the sample that held, and it held by removing on every revision | this commit |

**The historian's prediction is falsified by this commit**, and deliberately: one
clause is deleted and the package is net −12. That was the point of costing them
together.

## What a veto would compel

The ffi-pragmatist's veto on the f64 abort lifts only on a compiled demonstration
that a libm binding stays usable under it — *what `x = pow(0.0, -1.0)` does when
the result is unrepresentable* — plus aborts on **every** non-finite result, not
just division. The compiler-engineer's veto on the bracketed `match` lifts on a
disambiguator that costs no lexer change, or on a Part 11 result showing
`print(match …)` costs more than layout-inside-brackets risks; the harness has not
run since M5b, which is itself the argument for deferring.

The historian's standing condition on variant construction is worth recording
because it is structural rather than local: **restore construct/match symmetry,
or give the spec an EBNF appendix in Wirth's manner** — either removes the
*possibility* of the omission instead of patching this instance.

## The sentence this session is really about

Wirth's Oberon report says what this spec does not:

> "What remains unsaid is mostly left so intentionally, either because it is
> derivable from stated rules of the language, or because it would unnecessarily
> restrict the freedom of implementors."

Heroes' spec opens with something stronger — *"This document is the whole
language"* — and thirteen silences falsify **that claim**, not its length. Sorted
into Wirth's two classes, most of the thirteen are class one: derivable, and the
compiler says so loudly at the moment of the mistake. The residue that fits
neither class is the real defect count, and it was **four**.
