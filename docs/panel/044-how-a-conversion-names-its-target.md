# Panel 044 — how a conversion names its target

**Convened** 2026-08-13 on the author's instruction, after the author first
declined the sitting and then asked for it. **Four judges** (no ffi-pragmatist:
the emitted C is identical under all four forms, so it had nothing to be
differentiated about). **Status: resolved. The shipped form stays, with a
removal that delivers the benefit the alternatives were proposed for.**

**Two of the four candidates were the author's own.** They are judged here on
their merits and they lose, which is the sitting working rather than failing.

---

## The verdict

| judge | ranking | veto |
|---|---|---|
| llm-ergonomist | **A > B >> C** | **veto C** |
| compiler-engineer | **A**, D the only trade | **veto C** |
| spec-warden | **A > D > B > C** | **veto B, C, D** |
| historian (advisory) | B > C > D > A | — |

**A stays.** Three of four rank it first; the fourth has no veto and its central
premise was refuted on the tree during the sitting.

---

## The four

| | written | what decides it |
|---|---|---|
| **A** shipped | `fit_u8(n)`, eight names | the target is *in the token* |
| **B** author | `fit(u8, n)` | the target is *on the line* |
| **C** author | `b: u8 = fit(n)` | the target is *somewhere else* |
| **D** panel 043 | `fit_u8` / `try_fit_u8`, sixteen | A with a prefix |

---

## Why C fell, and it is not the reason anyone expected

### It makes a swapped pair of arguments compile

The llm-ergonomist found it by writing the programme, not by reasoning:

```
write_at(offset: u32, value: u8)

A:  write_at(fit_u8(value)?, fit_u32(offset)?)   →  type error, argument 1
C:  write_at(fit(value)?,    fit(offset)?)       →  COMPILES
```

Under C, `fit` **adopts the type of the slot it lands in, so it can never
disagree with the slot**. The conversion that should catch a transposition is the
thing that absorbs it.

The compiler-engineer reproduced it on the tree with a harder case, where the
fallibility rule — C's only remaining guard — cannot fire because every
conversion widens. Measured under A:

```
draw(x: i32, y: i64);  a: u8;  b: i16
draw(fit_i32(a), fit_i64(b))   # correct
draw(fit_i64(b), fit_i32(a))   # error[type_mismatch] ×2 — caught
```

Under C both spellings are `draw(fit(a), fit(b))` and both compile.

**And Heroes' own anti-inversion guard is off precisely here.** §4.9's
same-typed-argument labelling rule fires only when two parameters *share* a type.
C moves argument pairs from *"different types, caught by the type checker"* into
*"different types, caught by nothing"*.

### It cannot write the closure list's own line

The compiler-engineer probed all 34 positions rather than reasoning about them,
using `.case` as a stand-in because it goes through the same arm table. **`fit`
would get a target at 19 positions and none at 13.** The 13 are the deliverable
the author asked for, and two of them decide the sitting:

- **every argument of a generic call** — `calls.rs:276-282` synthesises *all*
  arguments whenever `generics > 0`, even where `T` is already bound;
- **every binary operand** — `expect.rs` has no `Binary` arm, and ⇒'s
  `contextual()` is a pure-syntax predicate that cannot know a `Call` is `fit`.

design.md's own calculator writes, at **:1144 and :2822**:

```
v @ v * 10 + fit_i64(l.here() - '0')
```

A binary operand. **C cannot write the line the closure list is made of** without
threading `&Resolved` into a syntactic predicate and then teaching `adopts()`
whether `fit` takes an offered `f64` — and `adopts`'s own doc comment records
that getting that wrong reintroduced a two-diagnostics-for-one-mistake defect
*within the hour*, three weeks ago.

Also with no target: map-literal values, `print`, a UFCS receiver, `xs[fit(n)]`,
`while`/`for`/`assert`, and `_ = fit(n)`.

### And `ok`/`fail` are not the precedent I claimed

The assistant argued C is *"the shape `ok`/`fail` already have"*. The spec-warden
refused it: **`ok` never alters its argument's value**, so context picks a
wrapper. `fit`'s context decides whether `300` fails, truncates, or widens. Same
mechanism, different stakes.

### §1.3 names it, and the proposal never cited §1.3

*"A construct is good for an LLM if the meaning of a line can be determined from
that line plus the signature of **the enclosing function**"* — and §1.3's
rejected list names **ambient context** explicitly. Under C, `f(fit(n))` puts the
target in the **callee's** signature. Part 6's `Implicit conversions` row reads
*"`f(x)` works for an invisible reason"*. C is that, one token late.

---

## Why B fell, and both of its costs were mis-stated

**The assistant's objection was wrong.** I argued `u8.fit(n)` comes free from
UFCS and looks like a method. The compiler-engineer measured otherwise: **UFCS is
not erased before the checker** — `ExprKind::Method` survives into
`types/calls.rs:98` with its own 70-line path, and Part 5's table puts the
erasure in *lowering*. The resolver reports `unknown_name` on `u8` before the
checker sees anything. So the spec's *"`x.f(y)` is sugar for `f(x, y)`"* is a
semantic claim, not a structural one, and forbidding the form costs one
diagnostic rather than a UFCS exception. **My premise was wrong in the safe
direction.**

**The historian cleared it from the other side**: `u8.fit(n)` is `int.Parse` in
C#, `u8::try_from` in Rust, `UInt8(exactly:)` in Swift, and **`Node.new` in Nim**
— the language CLAUDE.md §6 says to copy the surface of, whose manual prints the
type-receiver form. No recorded confusion anywhere.

**But the real hazard is one neither of us named.** Writing task 3 blind, the
llm-ergonomist reached for `bytes[i].fit(u32)` — value first, the Rust and Nim
habit — which desugars to `fit(bytes[i], u32)`: **arguments inverted**. So the
danger of B is not the type-as-receiver, it is that **UFCS invites exactly the
wrong order**. And the rebound is that **A is the only UFCS-safe form**:
`n.fit_u8()` desugars correctly and reads well.

**B's structural cost is also different from the proposal's.** Every language
with a type in argument position has compile-time types; Heroes has none, so B
would be a **builtin-only form no user can write**. Zig's users filed exactly
that asymmetry and Zig **closed it as not planned**.

---

## The measurement that reversed the sitting, and it is an error of mine

**Both variant specs described two forms at once.** `v-b.md` and `v-c.md` left
line 58 reading `` `fit_i8(x)` … `fit_u64(x)` convert between `` while line 59
introduced the new form. The −29 and −4 the proposal published — and which the
assistant relayed to the author — were measured against a **self-contradiction**,
and against an A that nobody had tightened.

The llm-ergonomist reported it (*"B's four 'no's are one defect… B's Types
section still prints `fit_i8(x) … fit_u64(x)`"*) and the assistant relayed its
ranking without registering that the broken document had contaminated B's score.

Rebuilt by one hand at equal tightness:

| form | measured | vs shipped |
|---|---|---|
| **A, list written as `fit_i8` … `fit_u64`** | **2927** | **−36** |
| B clean, **with its UFCS carve-out written** | 2941 | −22 |
| C clean, **with its `cannot_infer` limit written** | 2951 | −12 |
| D enumerated | 3046 | +83 |

**And the use-site side, which nobody had produced.** 18 real `fit_` sites in
`examples/` and `tests/golden/run/`. Per site, `fit(i64, x)` and `fit_i64(x)` are
**both 6 tokens**. On the corpus: **A 290 · B 298 · D 310 · C 366** — C breaks 13
of 18 sites and each needs a 7-token binding.

**Totals, spec + use sites: A 3217 · D 3230 · B 3239 · C 3317.**

---

## The closed-list argument was mine and it is false as measured

The proposal's question 2 held that A's eight names cost growth: a ninth width
needs a ninth name. Measured: **a ninth name costs six tokens.** Written as a
rule instead of enumerated, **zero**. And enumeration is what costs, not the
naming scheme — D enumerated is 3046 and D schematic is 2920, **126 tokens apart
for the same sixteen names**.

**So the entire benefit claimed for B and C looked available under A by changing
one line of prose.** It is not: see the resolution below, where the change is
refused by a gate the spec-warden did not run. The claim survives in a weaker
and more useful form — **the growth cost is six tokens, not a naming scheme** —
and six tokens is 0.3% of one §1.2 correction round-trip.

---

## §4.12, where the proposal argued around a rule instead of amending it

The proposal claimed §4.12's *"always inferred at the use site"* was written
against the turbofish — where the type is redundant because the arguments
determine it — and so does not reach a conversion whose target no argument
determines. The spec-warden judged that **honest on its point and silent on the
clause that bites**: §4.12 also states as fact *"there is no syntax to specify
type arguments manually"*, and **B makes that sentence false**. Its words:
*"Not a rule talked around — an amendment left unnamed."*

Recorded because the assistant wrote the proposal.

---

## Zig, which the sitting was half-built on, and what it actually says

**The move is real**: `ziglang/zig#5909`, opened 2020-07-22, closed 2023-06-24,
shipped in 0.11.0, **twelve cast builtins at once**, community reaction positive.
`@as` deliberately unchanged.

**But the stated reason does not transfer, and the assistant verified why.** Zig's
motivation is a refactoring bug: a field changes `u8` → `u16`, the old
`@intCast(u8, y)` stays and *still compiles*. That requires **implicit widening**.
Measured on this tree: `wide: u16 @ small` where `small: u8` is
`error[type_mismatch]`. **The bug Zig's whole operation exists to kill is already
impossible here**, so its reason is void.

**And at the right grain Zig is a B precedent, not a C one.** Zig has two casts.
The *asserting* one moved to C. The *fallible* one — `math.cast(comptime T: type,
x) ?T`, which is exactly `fit`'s shape — **is still form B today**, and the
proposal to make it C-shaped was **closed as not planned** (`#16313`).

**Zig's post-0.11 issue stream is the ⇐ gap list, item for item**: unnamed
collection literals, call arguments, arithmetic operands, unary negation,
`anytype` parameters. One was closed as not planned. The compiler-engineer's
13-position map, derived independently from this tree, matches it.

**And Haskell convicts C on the charge nobody made.** `fromIntegral` has been
form C for decades; the complaint is not that the target is invisible but that it
truncates silently — which `fit` already fixes by returning `T?`. The remedy both
sourced references recommend is **monomorphic named conversions**. Which is A.

---

## Resolution

1. **A stays.** Two vetoes on C, one on B, and A wins the cost formula on both
   halves once the wordings are honest.
2. **The removal is REFUSED, by an instrument the spec-warden did not run.**
   Applied, the ellipsis measured **2932 (−31)** and turned two tests red:

   ```
   reserved but never written in the spec: ["fit_i16", "fit_i32", "fit_i64",
   "fit_u16", "fit_u32", "fit_u8"] — a reader cannot use the name and cannot
   declare it either
   ```

   The saving *is* the deletion of six names the compiler reserves, which is
   exactly what that gate exists to prevent. **So the "growth is free under a
   rule" claim is false**, and it is false because of a rule this project
   already has: a ninth width costs its name in the spec, six tokens, and those
   six tokens buy a reader being able to find it. Reverted; the spec stands at
   **2963**.

   This matters beyond the paragraph. The measured ranking that decided the
   sitting had A at 2927 on the strength of this removal; without it A is 2963.
   **The ordering does not change** — B honest is 2941 + 8 use-site tokens and C
   is 2951 + 76 — but the margin does, and the record should not carry a number
   that cannot be reached.
3. **Panel 043's Q1 is untouched.** The shipped rule — `T` where the conversion
   cannot fail, `T?` where it can — was not this sitting's question and remains
   pending the author's ratification.
4. **Nothing is owed to B or C.** Both were the author's, both were judged on
   their merits, and the record says why rather than that they were dropped.

## Predictions to score

| # | judge | prediction | checkable at |
|---|---|---|---|
| 1 | llm-ergonomist | Swap two arguments at every call site with two integer arguments of different widths: A and B reject **100%**; C rejects **0%** where both conversions widen | first metric-2 run |
| 2 | compiler-engineer | `heroes mutate --operator swap-args` scores **92/128 (72%)** at `check` on today's 19-programme corpus. Under C the rate falls below 72% and `--survivors` names ≥1 mutant whose only edit is an inverted pair of `fit(…)` arguments in widening positions. Under A, B or D it is unchanged | M-program-corpus |
| 3 | compiler-engineer | The corpus cannot express design.md:1144's `v * 10 + fit(l.here() - '0')` under C without a diff to `types/exprs.rs:204`'s `contextual` | M-program-corpus |
| 4 | spec-warden | Implement C behind a flag, rewrite `fit_<w>(` to `fit(` in `run/sized-integers.hero` and change nothing else: **≥12 of 18** sites emit `cannot_infer`; predicted exactly **13** | any time |
| 5 | historian | Run C's gap list. Failures cluster in **binary operands, unnamed collection literals, and generic arguments**, while `f(fit(n))` and `return fit(n)` work — Zig's issue history item for item. **Scored already**: the compiler-engineer's independent map matches, including the generic-argument case | scored 2026-08-13 |

## Method note

Four judges, differentiated inputs. The llm-ergonomist wrote twelve programmes
blind across three spec variants and found the swap by writing it. The
compiler-engineer probed **34 positions** on the real tree rather than reading
the checker, and applied the revert to price it. The spec-warden **found the
convener's variant files broken**, rebuilt all four wordings by one hand, and
produced the use-site number nobody had. The historian dated Zig's move, then
showed it does not transfer and points at B rather than C.

**Three of this sitting's findings are errors by the assistant who convened it**:
two broken spec variants, a wrong premise about UFCS, and a closed-list argument
that measurement values at six tokens.
