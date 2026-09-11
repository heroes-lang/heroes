# Panel 117 — both sides of an `assert`, when a side is an aggregate

Convened 2026-09-07 by author instruction (*"when in doubt, panelize"*), on open
defect **016** in `docs/work/DEFECTS.md`. Full five seats. The working tree was
frozen at `ca9652af` from briefs-out to this synthesis, and every seat that
builds was given the same `git archive` snapshot with a prebuilt compiler and a
2.88 s seed.

**Status: `provisional — author ratification pending`.**

---

## The proposal, verbatim as the seats received it

> Defect 016: `assert` drops both sides for every aggregate. The spec promises
> both sides at `spec/heroes-spec.md:202` with no exception; the compiler prints
> neither for `[T]`, `{K: V}`, `T?`, a variant or a record, and prints no note
> that they were withheld. Measured today at `ca9652af`:
> `assert [1, 2] == [1, 3]` prints only `assert failed: [1, 2] == [1, 3]` while
> the scalar case prints left and right. Cause, measured:
> `selfhost/emit/abort.hero` emits `hero_panic_assert_sides` only when
> `builtins.to_str_entry` answers for BOTH sides, and that table
> (`selfhost/emit/builtins.hero:243`) answers only for the integers, the floats,
> `bool` and `str`, failing for `.array`, `.fixed`, `.map`, `.fallible`,
> `.named`, `.case_ty`, `.ptr`, `.cstr`, `.unit`, `.function_ty`, `.failure`,
> `.generic`, `.error`. And the runtime cannot help: `HeroDesc`
> (`runtime/heroes_runtime.h:302`) carries size, copy, drop, eq, hash and NO
> renderer and no field structure, with ONE `hero_desc_array` for every `[T]`
> that by its own comment "needs to know nothing about what the array holds". So
> there is no rendering of an aggregate anywhere in this language today. THE
> NARROW QUESTION FOR THIS SITTING, and it is deliberately not the whole
> reflection ruling: does `assert` owe both sides for an aggregate, and if so by
> what mechanism? The wider ruling on reflection at run time, on compile-time
> derivation in general, and on an annotation mechanism stays with
> M-reflection-verdict (`docs/ROADMAP.md` row 41), whose own section already
> measures the pressure: `examples/json/` is 671 lines rendering a variant by
> hand and 20 hand-written render functions across `examples/`. Options must be
> enumerated from the world rather than from what a convener can think of, and
> at least these four exist: (a) leave it, and narrow `spec:202` to name the
> scalar types, paying the token cost out of the spec's headroom; (b) emit no
> rendering but print a NOTE that the sides were withheld, which removes the
> silence and satisfies nothing; (c) derive a rendering in the emitter for the
> assert call site only, from the static types the compiler already has, which
> is the shape panel 116 vetoed for the owned release as a second elaborator in
> the printer against design.md Part 5; (d) give `HeroDesc` a render function
> pointer, an ABI bump on `HERO_RUNTIME_ABI`, which decides a piece of
> M-reflection-verdict's question in the runtime rather than in a sitting
> convened for it. A fifth may exist and the brief does not claim the list is
> complete. Constraints: CLAUDE.md §12 says the spec beats the compiler, so
> today this is the compiler's defect; §14 forbids a milestone id in
> user-visible output, so no message may name a milestone; §9 owes a
> `fixedbugs` case per shape the day it is repaired; and the resolution adopted
> is the most robust and complete one, never the cheapest and never a
> compromise.

**Two errors in that brief, both found by seats and both recorded rather than
quietly fixed.** It priced (d) as costing "an ABI bump", and panel 027 had
already measured that `_Static_assert(HERO_RUNTIME_ABI == N)` is **blind** to
adding a struct field: a new header against a stale object links at exit 0 and
reads past the end of the old struct. The bump is not the price; it is a guard
that cannot see this change. And it handed the compiler seat panel 116's veto as
an argument against (c); that seat measured the veto and found it **does not
reach**, and said so rather than use it.

---

## The verdict table

| seat | verdict | section stood on | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| **compiler-engineer** | **veto** on (c) and (d); objects to (a) and (b) | design.md §1.1, with §1.6 and §1.12 | (c) 190–230 code lines and a new module or a raised `DECIDED` row; (d) that plus a sixth descriptor member, 16 hand-written descriptors, an ABI hop and a regenerated 747,095-line seed | at row 41, an adopted per-type renderer's own module exceeds 150 code lines **and** either `DECIDED.len()` moves off 16 or `structural.hero` is split; and no adopted map renderer prints identical text for two `==`-equal maps without a key order for non-scalar keys | a deterministic `{K: V}` rendering needing no total order over non-scalar keys; or a measured recursion answer with panel 076's exactness; or Part 11 metric 4 showing aggregate sides move first-attempt compile rate |
| **ffi-pragmatist** | **veto** on (d) only; **approves** per-type generation, compiled | design.md §4.20's ruling at :2305-2312, secondarily §1.11 / §4.19 | (d) breaks the descriptor ABI and the seed; per-type generation touches no ABI | at M-typed-inspection, per-type generation gives `assert` both sides for every aggregate in `examples/sqlite/main.hero` with `HERO_RUNTIME_ABI` still 21, `HeroDesc` still five members and the seed's line 8 assertion untouched | lifts the (d) veto only for a construct that makes a five-member positional initialiser a **compile error** under `flags()` as written, or a NULL-`render` guard naming the type plus a proven two-stage bootstrap |
| **spec-warden** | **object** (no budget breach by any option) | design.md §1.6, §1.2, §1.0, and CLAUDE.md §12 / panel 039 | measured: (a) **+16**; its own **(e)** +19, **net +3** with a named **−16** removal; (b), (c), (d) zero spec tokens | at row 41 the reference wording costs **0** further tokens whichever way the sitting rules, while (a)'s enumeration costs a second edit of **≥ +8**; and an aggregate row in `suite_special.hero` goes red the day any mechanism lands | approves on one commit landing (e), spending the measured −16, moving `SPEC_TOKENS` 3871 → 3874 with the payment in the body, and adding the aggregate row |
| **llm-ergonomist** | **approve** the narrowing, with a four-word amendment; **no veto**, neither variant is non-local | the spec's own `test` sentence | +14 spec tokens once, against 2–3 compile-run cycles per failing aggregate assert, each 300–800 tokens | on a seeded corpus: the decomposition rate differs by **≥30 points** between the two wordings, and the unchanged wording yields a wrong causal hypothesis in **≥50%** of failing-assert transcripts against **≤10%** | flips to object if models read `each` element-wise in >20% of cases without the amendment; the argument collapses if transcripts show immediate decomposition with no wrong hypothesis |
| **historian** (advisory) | **approve**; would object to a byte dump and to a renderer reachable only from `assert` | precedent, sourced | Rust's `core::fmt` measured at 14.2 KiB → 1.2 KiB when formatting was removed in one published firmware case | if a renderer reachable only from `assert` lands, the first non-test consumer adds a **second** printer rather than reusing it; and a type-listing narrowing is wrong within one milestone, because nested cases and `record … partial` are members nobody enumerates | a second language that shipped an assert-only aggregate renderer and kept it clean five years or more; or evidence the descriptor already carries per-field types |

---

## The disagreement, stated plainly

**Two veto-holding seats disagree about per-type generation, and it is not a
misunderstanding.**

The **compiler-engineer** vetoes it on the ceiling and on robustness.
`selfhost/emit/structural.hero`, which already generates `eq` and `hash`, is
**293 code lines against §11's ~300** with no `DECIDED` row: seven lines of
room. Its own module doc says eq and hash live together "because they are ONE
invariant: two values that compare equal must hash equal". A renderer adds a
third walk owing a third clause, *equal values render equal*, **and that clause
is false today**, measured: `{"a":1,"b":2,"c":3}` iterates `b a c` while the
`==`-equal `{"a":9,"a":1,"b":2,"c":3}` iterates `b c a`. Sorting instead is
refused by `runtime/parts/sort.c:120-134`, which picks a comparison by
descriptor pointer identity over twelve scalar descriptors, so a map with record
keys has no order at all. And panel 076 gave eq a worklist with no bound because
"`true` is the identity of `&&`" while hash got a cap because it may lose
information; **concatenation has neither property**, so it reopens the ~52,200
frame cliff at exit 139 with no output.

The **ffi-pragmatist** approves per-type generation and compiled it, on the
ground that a type the worklist misses becomes an **undefined symbol at link**
where (d) gives `pc 0x0`. It also compiled the driver and got the sides printed.

**Both are right about what they measured.** The engineer measured the cost and
the unkeepable clause; the FFI seat measured the failure mode. Neither addresses
the other's evidence, and the resolution below is chosen so that neither veto
has to be overridden.

**A second, smaller disagreement.** The ergonomist's variant two pushes a model
toward decomposing a whole-aggregate `assert` into per-field asserts, which it
measured as the better test independently. The resolution adopted below makes
the whole-aggregate assert *useful*, which points the other way. That tension is
real, it is not resolved here, and it is the first thing to look at if the
adopted route is ever measured against the corpus.

---

## The sixth option, which no brief carried and which the sitting produced

**(g) Recurse to the first differing leaf, and print the field path with two
scalar sides.** The historian found it in Zig, from source rather than from
documentation: `std.testing.expectEqual` **never renders the aggregate**. It
switches on `@typeInfo` and recurses — a struct with `inline for` over its
fields, a union by tag then payload, an optional by payload, an array through
`expectEqualSlices` — and the line `expected {any}, found {any}` fires only on
the **scalar** arm. Its doc comment says the point out loud: it prints
diagnostics "to show exactly how they are not equal". Zig has a general renderer
too, `Writer.printValue`, and its test path still chose leaf recursion.

Rendered for this language, the failing case in the defect report becomes:

```
assert failed: summarize([1, 2, 3]) == Stats(count: 3, total: 7, label: "sum")
  first difference at .total
  left:  6
  right: 7
```

**It answers every objection this sitting raised, and by construction rather
than by compromise.**

- **The map problem dissolves.** No iteration order is promised, because nothing
  is rendered in order. The walk reports the first differing key, and a key that
  is present on one side only. The compiler-engineer's measurement stands and
  stops being an obstacle.
- **The recursion cliff dissolves.** The walk is `eq`'s walk, which panel 076
  already gave a worklist and no bound, for the reason that makes it sound.
  Nothing new is bounded, capped or deferred.
- **No third invariant clause.** There is no rendering, so *equal values render
  equal* is never owed. `structural.hero`'s one invariant stays one, and its
  seven remaining lines are not spent on a renderer.
- **No ABI event.** `HeroDesc` is untouched, `HERO_RUNTIME_ABI` stays 21, the
  seed is not regenerated, and the FFI seat's veto is satisfied without being
  argued with.
- **Nothing can go quiet.** The historian's caution, and D's bug 22700, are
  about a *second copy of what the language is* reachable from one consumer. A
  leaf walk that reuses `eq`'s is not a second copy: if it falls behind, `eq`
  falls behind with it, and `eq` is exercised by every program.
- **It does not annex M-reflection-verdict.** No `to_str` over a record's
  fields is derived, so row 41 keeps its question whole, and `print(p)` stays
  `error[bad_operand]`.
- **And it is better for the author than a full rendering.** Two rendered
  records leave the reader to diff them by eye. A field path says where to look.
  That is `spec:202`'s promise served rather than met on a technicality, which
  is what CLAUDE.md §4 means by the most complete resolution.

**It is unrun.** No seat compiled it, because no seat was asked about it: it
arrived in the last verdict. The cost is therefore an estimate and this
paragraph says so.

---

## The resolution adopted, provisionally

**Robust, and it is neither the cheapest option nor a compromise between the
vetoes.** Four parts, and the first two land now.

1. **The instrument first, because it is the finding that shames the rest.**
   `tests/harness/suite_special.hero:236-243` pins `assert`'s scalar output,
   `left:  4` and `right: 5`, and has **no aggregate row at all**. The watcher
   was blind exactly where the defect is: CLAUDE.md §1's *repair without its
   adjacent shapes*, inside the check that exists to catch it. An aggregate row
   lands now, asserting today's behaviour, so that the day any mechanism lands
   the row goes red and the sentence's expiry is noticed by an instrument rather
   than by a reader.
2. **The spec sentence, written the way a narrowing is read as honesty rather
   than as retreat.** Built on the spec-warden's **(e)** — state the renderable
   set once, on `print`'s own built-ins row where it belongs, and have the
   `test` sentence defer to it — and **not** the enumerated form the brief
   proposed, because the historian's second prediction is that a type list is
   wrong within one milestone (nested cases and `record … partial` are members
   nobody enumerates) and the warden measured that an enumeration needs a
   second edit within three rows. What landed, at `spec:187` and `spec:202`:

   > `print` … takes the types this language renders as text: a number, `str`
   > or `bool`.
   >
   > An `assert` failure shows the source expression, and both sides where
   > `print` takes them; where a side is an aggregate it shows the expression
   > alone, until this language renders one.

   **Measured at the landing commit: 3871 → 3903, net +32**, gross **+48**
   against the named **−16** removal at `spec:66-68`, re-measured in the landed
   file (3919 with the clause restored) rather than carried from the sitting.
   Headroom 225 → 193. The removal's fact survives at `spec:245-246` and the
   compiler covers it loudly in both directions, which is panel 089's shape.

   **THE WARDEN'S CONDITION WAS THE CHEAPER WORDING AND THE AUTHOR OVERRODE IT,
   ON THE RECORD.** That seat's approval was conditional on its reference-only
   form at **net +3**, and the assistant had exactly that in the file, measured
   at 3876. The author's instruction of this date — *"better robust than
   cheap"* — took the fuller version, which states the aggregate case in the
   document instead of leaving a reader to infer it from `print`'s row two
   paragraphs away, and which carries the restoration condition *until this
   language renders one*. That clause is JEP 306's shape and it is the reason
   the historian read N1426 and P2186R2 as credibility rather than retreat:
   Java narrowed strict floating point in 1.2 with the condition written down
   and restored it in 17 when the condition was met. **The warden's objection
   therefore stands unwithdrawn at +29 against its condition, and its veto was
   never engaged**: it holds one on a budget breach, and 3903 of 4096 is not
   one. `SPEC_TOKENS` moves 3871 → 3903 and the ledger gains its row with all
   three figures re-measured.
3. **The leaf walk, option (g), is the route to the repair**, and it is
   scheduled rather than landed in this sitting, because it is unrun and because
   two vetoes rest on measurements it must be tested against. It goes to
   `docs/work/SCHEDULED.md` at M-reflection-verdict with three things owed
   before it lands: a compiled prototype, the map case run against the two
   `==`-equal maps above, and the depth case run to the frame count panel 076
   measured.
4. **Refused now: (c) and (d).** (d) is double-vetoed with compiled evidence and
   the historian's D precedent; the FFI seat's condition for lifting it is on
   the record. (c) is vetoed by one seat and approved by another, and it is
   refused for now because the map clause it would owe is unanswered — not
   because the veto outranks the approval.

**What the conservative resolution would have been, so the author can choose
it**: land nothing, keep `spec:202` as written, and leave defect 016 open with
its gate at row 41. That is what every seat's narrowest reading permits. It was
not adopted because the spec would go on stating a promise the compiler cannot
keep for maps by **any** mechanism, and because the blind instrument would stay
blind.

---

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | an adopted per-type renderer's module exceeds 150 code lines, and either `DECIDED.len()` leaves 16 or `structural.hero` splits | M-reflection-verdict close |
| compiler-engineer | no adopted map renderer prints identical text for `{"a":1,"b":2,"c":3}` and `{"a":9,"a":1,"b":2,"c":3}` without a key order for non-scalar keys | M-reflection-verdict close |
| ffi-pragmatist | per-type generation gives every aggregate in `examples/sqlite/main.hero` its sides with the ABI still 21, `HeroDesc` still five members, and the seed still compiling | M-typed-inspection close |
| spec-warden | the reference wording costs 0 further spec tokens whichever way row 41 rules; an enumeration would cost ≥ +8 more | M-reflection-verdict close |
| spec-warden | the new aggregate row in `suite_special.hero` goes red the day any mechanism lands | the commit that lands one |
| llm-ergonomist | ≥30 point difference in decomposition rate between the two wordings; ≥50% versus ≤10% wrong causal hypotheses | M-thesis-harness, when metric 4 first runs |
| historian | a renderer reachable only from `assert` gets a **second** printer added by the first non-test consumer rather than being reused | two milestones after any such renderer lands |
| historian | a type-listing narrowing is wrong within one milestone | M-reflection-verdict close |

---

## Two findings this sitting produced that outlive its question

**The thesis argument does not reach here, and the seat that guards the
indicator is the one that said so.** Part 11's metric 2 is single-turn and
spec-only, so no repair happens inside it; metric 4 measures turns *to make it
compile*, and a failing `assert` is not a compile failure. So the debugging
round trip the ergonomist measured is real and is **outside** the project's
measured territory, and metric 4 has never run. Under panel 046 as it amends
§1.6, a prediction naming metric 4 pays nothing. **What is inside metric 2's
denominator is `print`**: `print(Point(x: 1, y: 2))` is `error[bad_operand]` at
exit 1, a first-attempt compile failure. If the thesis is the warrant anybody
reaches for at row 41, the sentence to argue about is `print`'s, not `assert`'s.

**The record contradicted itself and the later entry wins.**
`DESIGN-LOG:174`, panel 020, 2026-08-11, *ruled* the text-only fallback: "A side
an `assert` cannot render is discarded explicitly … so a record or an array has
no rendering and the text-only form is used." `DESIGN-LOG:662`, 2026-09-06,
files the same behaviour as defect 016 against `spec:202`. Both are in an
append-only record, and CLAUDE.md §1's obligation to read forward is what
settles it: the defect is the live state. Worth noticing that the sitting could
only find this because a seat obeyed that rule instead of stopping at the first
entry it matched.

## Author's verdict

**RATIFIED 2026-09-07** (author instruction, *"I ratify"*), all four parts of the
adopted resolution, given the same day the sitting was convened and after the
author had already overridden one seat's wording condition in the direction of
the more robust text.

What that settles, spelled out so nobody has to infer it from one word:

1. **The aggregate row in `tests/harness/suite_special.hero` stands**, pinning
   today's behaviour on purpose. It is now a ratified tripwire: the day a
   mechanism lands it goes red, and unpinning it is part of that landing.
2. **The spec sentence stands as landed**, the fuller wording at 3903 tokens
   rather than the spec-warden's 3876. That seat's objection remains on the
   record unwithdrawn, which is what a ratified override looks like here: the
   author chose, the seat's reasoning is preserved, and neither is deleted.
3. **The leaf walk is the ratified route** to the repair, not merely the
   sitting's preference. It is an item at M-reflection-verdict with a compiled
   prototype, the map case and the depth case owed before it lands. A future
   sitting that wants a different mechanism is overturning a ratified decision
   and owes the argument for it.
4. **The two refusals are ratified refusals.** A sixth `HeroDesc` member is
   refused with the FFI seat's compiled evidence behind it, and the per-type
   renderer is refused *for now* with the compiler seat's map measurement
   behind it — the second is the one that can return, and the condition is that
   seat's: a deterministic `{K: V}` rendering that needs no total order over
   non-scalar keys.

**What ratification does NOT do.** It does not close the work. Defect 016 closed
because the contradiction between two artifacts closed; the language still
cannot render an aggregate, and an author debugging a failing aggregate test
still reads one line. That is scheduled, and the schedule is now the author's
decision rather than the sitting's proposal.

---

## Corrected 2026-09-11, at panel 131, on this sitting's own three owed measurements

**Part 3 of the resolution above does not survive, and this sitting is the reason
it was testable.** It adopted the sixth option and scheduled it *"with three
things owed before it lands: a compiled prototype, the map case run against the
two `==`-equal maps above, and the depth case run to the frame count panel 076
measured"*, and it said of itself, in its own words, *"It is unrun. No seat
compiled it, because no seat was asked about it."* All three were run at
`docs/panel/131-the-refusals-were-sound-and-their-reasons-were-not.md`. **Two of
the four claims in § Why it answers every objection are false.**

**The sentence *"The map problem dissolves"* is false of the walk as well.** For a
yes-or-no answer the walk is order-independent, and that much holds. But two
`==`-equal left operands measured against one right operand report **two different
leaves** — `m["a"]: 1 != 10` and `m["c"]: 3 != 30` — because the walk looks keys
up in iteration order and stops at the first mismatch. The leaf named is a
function of insertion history rather than of the value, which is this sitting's
own refusal ground for map *rendering*, one level down inside the thing it adopted
instead.

**The sentence *"The recursion cliff dissolves. The walk is `eq`'s walk"* is false
at the source.** `HeroEqWork` (`runtime/parts/array.c:185-190`) is
`{a, b, elem, len}`: no path, no parent, no index base. A nested call returns
`true` provisionally and the outermost drains LIFO, so the first `false` the drain
finds is not the first differing leaf in any order a reader can predict. The walk
cannot be `eq`'s walk without a fifth field and an order change in the one
function panel 076 landed, and the prototype that does it with a C frame per level
reports a leaf to depth 3600 with a path 28,813 characters long and then
`panic: stack exhausted` at 3700, exit 134 — where the behaviour this sitting
called *"strictly worse than the defect"* nowhere, today's thin
`assert failed: a == b`, is correct at exit 0 at depth 100,000.

**Two claims do hold and panel 131 keeps them**: no `HeroDesc` member is needed,
because records and variants are reached statically; and the walk is not a second
copy of what the language is. The cost estimate this sitting could not give is now
measured: `selfhost/emit/differ.hero` is **160 code lines** as a floor and
**350-450 across six modules and the runtime** as the real figure.

**And the Zig precedent that produced the sixth option is overstated here.**
`std.testing.expectEqual` does recurse through **struct fields** to a scalar leaf,
which is what this sitting took from it; but on optionals it prints the whole
payload, on slices it compares pointer and length rather than contents, and Zig
ships a separate `expectEqualDeep` precisely because of that. *"It never renders
the aggregate"* is false of Zig; *"it recurses through struct fields to a scalar
leaf"* is true.

**What is unchanged**: parts 1, 2 and 4 of the resolution, the spec sentence at
§ 12 and its restoration clause, and `tests/harness/suite_special.hero`'s
aggregate pin — which stays, and stays correct, because it was written to go red
the day a mechanism lands and none does. The withdrawal of part 3 is
`provisional — author ratification pending` in its own right, because this
sitting's parts were ratified on 2026-09-07 and a reversal is put to the author
separately rather than inside a longer yes.

**The withdrawal of part 3 was RATIFIED 2026-09-11**, the author's word the same
evening panel 131 sat, on an item put separately from that sitting's other five
parts precisely because it asks for a reversal. So this sitting's parts 1, 2 and 4
stand as ratified on 2026-09-07, and its part 3 is ratified as **withdrawn** on
2026-09-11. `tests/harness/suite_special.hero`'s aggregate pin stays and stays
correct, and the mechanism the defect is owed remains open, to be reached another
way.
