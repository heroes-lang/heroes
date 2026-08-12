# Panel 043 — a widening that cannot fail, and a price the spec is allowed to name

**Convened** 2026-08-12 on the author's instruction (*"per sciogliere i decide
rimasti convoca il panel"*), over the whole of `docs/debrief/DECIDE.md` § Open.
**Four judges.** **Status: Q2 resolved; Q1 provisional — author ratification
pending.**

**The ffi-pragmatist was not convened, and that is a decision.** Neither question
touches the C boundary: the conversion happens entirely inside Heroes and the
spec clause has no C behind it. Convening it would have been thirty minutes for a
verdict carrying no information — the failure this skill's first section was
written about, after panel 037.

**One of the three open items was not a question.** The §11 sweep is work whose
shape the author already decided (2a, standing sweep). It is reported below
rather than judged, because it went *backwards*.

---

## Q2 — RESOLVED. The paragraph stays, restated.

Three judges reached the same cut by three routes that share no evidence.

| judge | route | what it says to keep |
|---|---|---|
| llm-ergonomist | blind A/B, four tasks, two spec variants | the discovery of `join`, not the opinion |
| spec-warden | the document's own precedent | the **price** is semantics; the imperative is opinion |
| historian | C++, Scheme, Python, C++17's `register` | a **cost property**, never a preference |

**The spec-warden withdrew its own proposal, on the merits.** It proposed this
removal at panel 041 and was refused; this sitting was its rehearing, and it did
not restate the case. design.md Part 2:497 governs *choosing* — *"where a choice
trades speed for simplicity, take simplicity every time"* — and says nothing
about *describing*. §4.10:1327 settles it by example, pairing both in one
sentence: ***"Price, declared:** mutating a shared array copies it, O(n).
Performance is not a goal, and in exchange…"*. **So the spec may name a price.**
The −39 is withdrawn rather than deferred, because deferring keeps a bad argument
alive on a timer.

**The historian sourced the boundary.** Reference documents do carry performance
statements — C on `inline`, the Rust Reference, Java's API spec, Python's library
reference. What separates them is *what kind of statement*:

- a **cost property of a named operation**, which binds the implementation. C++
  formalises it: *"Complexity"* is a named element of a specification, read as an
  upper bound. Scheme's proper-tail-recursion rule is the same kind.
- an **instruction to prefer one construct**, which binds the author and rests on
  a premise about the current implementation.

The second is CLAUDE.md §11's *"asks the world, never the value"*, and the wild
failure is sourced: **C++17 removed `register`** — a spec statement whose only
content was performance — and real builds broke. Java carries the live hazard
today (*"the constant factor is low compared to that for `LinkedList`"*).

**The llm-ergonomist measured the clause working, and argued against it anyway.**
Blind, four tasks, two variants, no idea which contained what:

| | `+` accumulated in a loop |
|---|---|
| with the clause | **0 of 4** |
| without | **2 of 4** |

A large effect for 36 tokens, reported honestly against its own conclusion. But
every move it caused was **correctness-neutral** — `out @ out + piece` is a
*correct* program, only slow — and one move **cost** correctness: the clause drove
`split_lines` off a `chars()` scan (all `str`, structural equality) into a
byte-index scan needing `fit_i64(text[i]) == '\n'`, i.e. straight into the u8/i64
hazard `M-sized-integers` created that afternoon.

It also reported its own susceptibility unprompted: had it seen the difference
first it would have *performed the hypothesis*, and it would distrust any
non-blind repeat. That is why the blind order was built in.

**Two claims were checked rather than relayed, and one is worse than reported.**

1. **The array half advises the impossible.** `a + b` on `[i64]` is
   `error[bad_operand]: `+` takes `i64` with `i64`, `f64` with `f64`, or `str`
   with `str`` — there is no array concatenation, no `concat`, no `extend`.
   *"Build a long array in chunks"* **has no addressee in the language it
   describes**, and has been in the spec for a month. The judge called it inert;
   it is worse — it is unfollowable.
2. `range(...).map(to_str)`, the shape the clause pushed the judge into, is
   `error[builtin_as_value]`. The diagnostic is excellent; the point is that the
   advice leads there.

**And the compiler-engineer priced what the paragraph documents.** `hero_array_push`
(`runtime/parts/array.c:90-106`) allocates `len+1` and copies **every** element;
there is no capacity field anywhere in the runtime. Measured at `-O2`:
**1.07 / 3.34 / 13.08 / 50.93 s** at n = 20k / 40k / 80k / 160k — 4× per
doubling, textbook quadratic. So the two lines are not stray advice under §13:
they document the cost of two **built-ins**, in the section that documents
built-ins.

### The resolution, adopted

Keep the declarative half, delete the imperative, and repair the half that
advises the impossible. `+` copying both sides and `push` copying the array are
**value semantics** — a reader cannot predict that `b = a; a @ push(a, 9)` leaves
`b` unchanged without them. *"Build a long string with `join`, not repeated `+`"*
is a preference, and the preference follows from the price on its own, which is
Python's shape at the same token cost.

**The rule this establishes, and it outlives the paragraph:** the specification
may state **what an operation costs**; it may not instruct the reader to prefer
one construct over another for speed. CLAUDE.md §13's home gains the second
sentence; design.md §4.10:1327 is the model for the first.

---

## Q1 — PROVISIONAL. The rule survives its own stress test; the record around it does not.

`fit_<w>` returns `T` where `IntKind::contains` says the conversion cannot fail
and `T?` where it can. Shipped at `M-sized-integers` step 7, bending the author's
ruling that conversions return `T?`.

**The implementation survived a real attack.** The compiler-engineer generated all
**64 width pairs** from an *independent reimplementation* of the rule and
type-checked the result: zero errors, so `contains` is correct at every pair. It
then showed **structurally** — not by sampling — that C's usual-arithmetic
conversions cannot bite: the low test fires only when `from_low < low`, which for
an unsigned source needs `low > 0`, impossible; and an `LL` bound above
`INT64_MAX` would need `from_high > u64::MAX`, impossible. Eighteen boundary
values, clean under `--sanitize`, no warnings under §7's flags.

**And it corrected the proposal's central claim.** I wrote that the rule "does not
bend panel 042's Q2". The compiler-engineer, who cast that veto, says **042 never
ruled on a widening at all** — its Q2 chose among *names*, and its recorded ground
was that no language changed *the incumbent's* return type. So this is not the
thing it refused; the ruling being bent is the **author's**, and only the
author's.

**The historian found the option nobody proposed, and it is the only one with a
shipped precedent.** Heroes' *rule* is not an invention — D's `to!T` derives
fallibility from the pair, and `IntKind::contains` **is** Rust's `impl_from!`
membership test written without a trait. But the *encoding* — one name, `T` here
and `T?` there — was search-negative across Zig, Rust, Swift, D and Ada. Zig
could have shipped it (comptime return types) and shipped uniform `?T`, with
nobody proposing otherwise. Rust refused type-dependent fallibility explicitly —
**for platform reasons that do not transfer to fixed widths**, which is a
correction to any "everyone refused this" reading.

Rust's actual answer to Heroes' actual complaint is **two names, both always
available**: `from` where the pair is lossless, `try_from` everywhere *including*
the lossless pairs, via a blanket impl with `Error = Infallible`. In Heroes:

```
fit_i64(aU8)     -> i64     # a compile error on a lossy pair, with a certain
                            # fix naming the other form
try_fit_u8(anI64) -> u8?    # always available, every pair
```

That kills the lexer's `.must()`, keeps the return type a function of the
**name** rather than the argument, and makes the refused case a diagnostic —
which is the thesis rather than an exception to it. It costs eight more names.

**The locality cost, measured rather than asserted.** Worse by exactly **one
write-side hop**: under the uniform rule `fit_i64(x)` is `i64?` from the name
alone; under the shipped rule the reader resolves `x`'s declaration to know
whether to write `.must()`. That is a second hop to a different place than the
callee, for a *syntactic* decision. Against it: a read-side **gain** — an absent
`.must()` now carries information — and **all four wrong guesses are compile
errors**, verified. §1.4 is better off, because a `.must()` that cannot fail is
redundancy spent where no error can occur, which §1.4 forbids by name.

**Reverting costs 9 lines** and touches 8 `fit_` sites. Measured by applying it.

### Provisional resolution

**Keep the shipped rule**, on the compiler-engineer's three conditions, of which
**(c) is load-bearing**: a `certain` fix *"remove the `.must()`"* on
`bad_operand`, and a `guess` fix appending `.must()` on `type_mismatch` where
found is `Fallible(expected)`. That is what turns the extra §1.3 hop from a §1.2
round-trip into a machine-applied repair — the only thing that makes the shipped
rule better than the uniform one on §1.2 rather than worse.

**The author's alternative is the historian's third option**, and it is not on the
menu the author answered from. It is recorded here because it is the only shape
with a precedent, and because eight more names is a price the author may prefer
to one varying return type.

---

## What this sitting found in the tree, all of it mine, all from today

**1. design.md contradicts itself, and it is the source of truth.** §4.3:831 still
reads *"**One integer type only.** No `i8`, `u32`, `usize`. No choice, no width
conversions, no conversion bugs… Keep them out of v1 without debate"* — while
design.md:1134 and :2812, which `M-sized-integers` edited that same day, call
`fit_i64`. Part 7 item 10 still reads *"never in v1"*. The panel was being asked
to ratify a return-type rule for a family the source of truth forbids. Panel 041
retired item 10's *reason* and panel 042's record retired the *item*; §4.3's
normative paragraph — the one that actually states the rule — was never touched.
CLAUDE.md §12: spec beats compiler. **This is `M-sized-integers`'s §14 debt and it
is repaired in this sitting's commit.**

**2. A comment asserts the opposite of the code it sits on.** `resolve/builtins.rs:63-74`
says *"Widening returns `T?` too… the uniform rule wins"*, written at step 6 and
never revisited when step 7 changed the rule. It also attributes the ground to
panel 042 Q2, which never ruled on widening. **CLAUDE.md §11's named failure mode,
committed by the author of five findings of it elsewhere in this same session.**

**3. `print`'s diagnostic enumerates a set it does not have.** *"`print` takes
`i64`, `f64`, `bool` or `str`"* — while accepting `i16`, `i32`, `u8`, `u16`,
`u64`, all measured, all printing correctly. In the milestone that added eight
widths.

**4. Dead code where §11 wants a loud fallback.** `emit/ops.rs:431-432`'s
`if tests.is_empty() { "1" }` is unreachable: the `to.contains(from)` early return
took every such case. Zero `if (1)` across all 64 emissions. It owes
`hero_unreachable` and a `contains_agrees_with_range` test whose failure names its
dependants.

**5. The §11 sweep went backwards, and `M-sized-integers` did it.** 22 files over
~300 lines at the decision, 19 after three cuts, **23 today** — `emit/ops.rs`
433 → **539**, `emit/inst.rs` 445 → **475**. Not a question for judges; work, and
work this milestone created.

**6. And a number in this sitting's own brief was false.** I wrote *"the twelfth
consecutive sitting with no removal"*. §1.6's *"zero removals in seven
amendments"* is a **closed historical window**, not a live counter: since it,
**11 amendments and 3 subtracted** (panels 035, 036, 042). The removal branch
works. Relaying a frozen number as current is the same expired-premise class the
rest of this list is made of.

---

## The finding that outlives both questions

**Panel 012's two branches are not equal, and one of them is free.**

- The **named-removal** branch has paid three times in eleven amendments.
- The **registered-prediction** branch has **26 rows across panels 031–042 parked
  on a harness that has never run**. `docs/measurements/` holds five files and
  **not one is metric-2**.

§1.1 makes comprehension *the objective* and operationalises it as first-try
compile rate. **There is not one measurement of it in this repository.** Panel 041
had to write *"NOT SCORABLE, and the reason is the instrument"* twice in one
table. Panel 037's two ergonomist predictions — the ones that justify Q2's
paragraph — are among the 26, and this sitting would have made it 27.

Registering a prediction costs nothing and nothing ever comes back. The
spec-warden's condition is the right shape: **an unscored prediction protects its
clause only until a named date.** Without an expiry it is a permanent tenancy, and
that is what happened to these two lines for a month.

---

## Predictions to score

| # | judge | prediction | checkable at |
|---|---|---|---|
| 1 | llm-ergonomist | Over ≥20 string-producing tasks, removing the clause raises `+`-in-a-loop by **≥40pp** (its sample: 0% → 50%), while **\|Δ first-try correctness\| ≤ 3pp** — because every `+`-accumulation programme is correct. If the harness scores correctness only, the clause is invisible to it | first metric-2 run |
| 2 | llm-ergonomist | **≤5%** of programmes under either variant build an array in chunks. It expects exactly **0%**, the array half having no addressee | first metric-2 run |
| 3 | spec-warden | `docs/measurements/` gains **no metric-2 file** before `SPEC_TOKENS` passes **3100** (2956 today, 26 predictions parked, zero metric-2 files). Falsified the day a metric-2 measurement lands first | ongoing |
| 4 | compiler-engineer | At M-selfhost-probe close, `grep -c 'fit_' selfhost/*.hero` ≥ **10**, and a revert to the uniform rule touches **≥25** sites (8 today). **<15** means the rule stays cheaply reversible and ratifying now is free either way; **≥25** means the harness must run before that milestone closes, because after it the measurement can no longer change the answer | M-selfhost-probe |
| 5 | historian | Run the blind instrument on **three** variants — absent, imperative, declarative-of-equal-cost. The declarative is **within noise** of the imperative and both beat absent. Falsified if the imperative's `join` adoption exceeds the declarative's by more than the run-to-run spread, in which case the effect is the instruction rather than the fact | first metric-2 run |

---

## Method note

Four judges, differentiated inputs. The llm-ergonomist received two spec variants
and never learned that the disputed clause was **its own**, added on its ask at
panel 037 — it found the difference only after writing eight programmes, and its
susceptibility report is the reason the blind order was built in. The
spec-warden was given its own refused proposal as a rehearing and withdrew it on
the merits. The compiler-engineer was told a defect had once existed in the code
it was auditing, and it attacked accordingly — generating 64 pairs from an
independent reimplementation, and applying the revert to measure it before
restoring the tree. The historian returned one search-negative that is the
sitting's sharpest Q1 finding and three corrections to the proposal's text.

**Three of this sitting's six tree findings are defects in work committed the
same day, by the assistant convening the panel.** That is the panel doing what it
is for.
