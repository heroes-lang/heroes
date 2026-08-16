# Panel 084 — the question design.md had already answered

**Date**: 2026-08-16 · **Session type**: full panel, five judges, five briefs ·
**Status**: `provisional — author ratification pending`

## The brief was convened on a settled ruling, and that is the sitting's first finding

Panel 082 closed with the llm-ergonomist's veto standing: `sort(xs)` inside
`function smallest<T>(xs: [T])` is legal or not depending on call sites in other
files, so *"fixing the reported line means deleting the function."* This sitting
was convened to ask whether Heroes should admit constraints.

**It should not, and design.md says so in ratified text.** §4.12:1593:

> **No constraints.** No `where`, no bounds. **If an operation on `T` is needed,
> pass it as a parameter.** This is the line that separates cheap generics from
> expensive ones… **Cut exactly on that line.**

The coordinator's brief presented that as an open question. It is CLAUDE.md §1's
named shape — *a silence read as an open question* — except it was not even a
silence: the ruling is written out with its argument. **Second time in one day**,
and the rule that catches it is the one this project already has: before
convening on something the record does not say, grep for the thing that is
**not** there.

**The historian reached the same answer from outside**, without seeing design.md:
Ada's formal subprogram and CLU's where clause, **1977**, and it named the shape
the brief was missing — **D, pass the operation**.

## D is not a proposal. It runs today, and it was measured before anything changed

```
smallest_by<T>(xs, less)                        → 1, then 4       exit 0
sort_by<T>(xs, less)  — a full insertion sort   → 4, 7, 9         exit 0
```

A generic insertion sort over a **user record**, with element mutation, a function
value parameter and a type parameter. Zero new syntax, zero spec tokens. That also
falsifies the ergonomist's hesitation #5 (*"hand-rolling a sort is possibly
impossible"*) and the brief's claim that refusing `sort` on a type parameter
*"deletes a working program"* — the program does not die, **it grows a
parameter**, which is exactly what §4.12 asks for.

## Verdict table

| Judge | Verdict | Rests on | Cost / delta | Prediction | Condition |
|---|---|---|---|---|---|
| **spec-warden** | **veto C** · approve **B re-worded at net zero** · object A | §4.12:1593, §1.6, §1.0 | B as briefed +29 was **83% overpriced**; at `sort`'s own line **+5**, with a **−5** removal → **0**. Built it: 568 pass / 5 fail, all tracing to **one file** | porting that one case takes it to 0 failures with **no second file edited** | the C veto lifts only on compiler-need or an explicit author decision overturning §4.12 — *"my veto is on presenting a ratified ruling as an open question"* |
| **compiler-engineer** | **object** to all four (A, B, C, and the inferred-constraint (i) it was asked to price) | §4.12, §1.3, §1.5 | C's syntax half **built**: +54/−16, 9 files, **IR unchanged**, 573/573 green. (i) **built**: net +30, and its diagnostic **fails §1.3 at depth** | at the landing commit the mutants canary fails until moved, and `values.rs` exceeds 300 | a program §4.12's rule cannot express. Could not construct one |
| **ffi-pragmatist** | **object** to all three as framed | §1.11, §4.19, CLAUDE.md §1 | `join` already ships B **in `sort`'s exact shape**; 7 more built-ins too — **`sort` is the single outlier** | a `check/` case for `partial` through a generic reports zero diagnostics under A or B-as-worded | approve if B is worded over **restricted domains**, not "built-ins" |
| **llm-ergonomist** (three variants, blind) | **veto** the status quo · **object** refuse-early · approve constraints conditionally | locality | wrote the program right first try only under the constraints variant | ≥30% mis-attribute the error line under the status quo | flips to **veto** on constraints if `ordered` admits a user record by structural ordering — *the only guess across three variants that could **compile*** |
| **historian** | **object** — three shapes is one too few | `spec:108-112`, `spec:176` | documentary, sourced | a `sort_by<T>` golden compiles and runs at exit 0 today — **run, and it does** | shape D shown unreachable. It is reachable |

## Disagreements, and what settled them

**Two seats said B's wording is wrong, and the measurement agrees with them.** The
brief said *"a built-in whose domain is restricted"*. There is a **second**
restricted domain that is a **type**, not a call — and it escapes: a map built
*inside* a generic and keyed at `f64` by the call is `check` exit 0 and **runs at
exit 0**, while the same map written directly is `float_map_key` at exit 1. That
rule exists because a `nan` key is stored and never found again (panel 069 R4), so
this is a live hazard, and it is **filed rather than fixed here**.

**The seats disagreed about what B costs and the smaller number won**: +29 as the
coordinator worded it, **+5** at `sort`'s own line. The warden's is the true price
and the coordinator's was padding.

**On the ergonomist's condition, the compiler is already right.** It said its
approval of constraints flips to veto if `ordered` would admit a user record by
structural ordering, since `==` is structural and a reader would generalise.
Nothing lands `ordered`, so the condition is moot — but it is the sharpest reason
not to want a one-name vocabulary, and it is recorded rather than dropped.

## Resolution — provisional, author ratification pending

**R1. B lands as a bug fix, not a feature.** `Ty::Generic` comes off
`is_refusable`, so `sort` on an unconstrained type parameter is
`unordered_element` at exit 1 **on the body's own line** — which is what `join`,
`to_str`, `to_i64`, `chars`, `keys`, `len`, `slice` and `repeat` have always done.
It repairs panel 068 R2's ratified invariant on the one row that was breaking it.

**R2. The diagnostic names §4.12's route** (the warden's condition, and §4.17's
rule). A type parameter gets its own note: *"take the comparison as a parameter
instead, `less: (function(T, T) -> bool)`"*. Zero cost — compiler output is not
the prompt (panel 035 D).

**R3. Constraints are refused, and the refusal is §4.12's, not this sitting's.**
Nothing here overturns a ratified ruling; if the author wants to, that is a
different act and the warden said so explicitly.

**R4. The gate's `builtin` row is deleted, not left dead.** It was the last row
keyed on a *value* rather than a capability, `emit/builtins.rs` drops from 300 to
**228** lines, and `emit/tests/mutants.rs`'s canary **had to move** — the
compiler-engineer's finding: that canary *was* the generic route, so closing it
would have left the invariant measuring the checker while claiming to measure the
emitter.

**R5. Spec: −2, and it gets shorter.** +5 for *"never a type parameter"* against a
**−7** removal the warden found — *"Every base writes a value, so
`0xffffffffffffffff` does not fit and is refused"* is **false**: it prints
`18446744073709551615` at exit 0. True when written, killed by `u64` **the same
day, hours later**. The argument survived; the example died.

## Predictions to score

| Judge | Prediction | Outcome |
|---|---|---|
| spec-warden | one file to port, no second file edited, `heroes measure` unmoved | **partly held** — one golden as predicted, but the diagnostic registry and three gate tests moved with the row; the spec moved deliberately, to **3512** |
| compiler-engineer | the mutants canary fails until moved; `values.rs` exceeds 300 | **first held** (moved to `[()]`); **second falsified** — 282, because R2 is one note rather than three diagnostics |
| historian | a `sort_by<T>` golden compiles at exit 0 today | **held, run twice** |
| ffi-pragmatist | `partial` through a generic reports zero diagnostics | **held, and filed** |
| llm-ergonomist | ≥30% mis-attribute the line under the status quo | metric 2 (0 tasks — an observation) |

## Filed, measured, not fixed here

1. **A generic calling a generic does not compile**: `check` 0, `build` **exit 2**,
   `too many arguments to function call`, on `t2 = hero_unreachable(t1);`. Cause
   located: `Checked::instantiations` is keyed by `span.start`, one answer per
   span, so it cannot hold two instances of one enclosing generic, and
   `ops.rs`'s lookup falls back to the literal name `hero_unreachable`. **No golden
   covers it** — of four goldens with ≥2 generics, every call is from `main`. This
   blocks composing generics, and §1.11's Tier 2 (`map`/`filter`/`fold`) *is* a
   library that composes.
2. **A map keyed at `f64` inside a generic runs at exit 0** while the direct form
   is `float_map_key` at exit 1.
3. **`partial` through a generic**: `spec:221` promises a compile error *"for it
   and for any value holding it"*; through a generic it is a run-time abort.
