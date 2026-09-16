# Panel 155 — llm-ergonomist report

Input discipline held: I read `docs/panel/155-briefs/llm-ergonomist.md` and
`spec/heroes-spec.md`, and nothing else. No design.md, no compiler, no other
brief, no test. I did not compile anything — by instruction.

**verdict: object** (direction right, spelling wrong and undocumented). I do
**not** cast the non-locality veto today; I state below the exact shape that
would draw it.

---

## The finding that reorganises the question

**The specification does not contain the rule this panel is about.** I went
looking for the sentence that refuses `m: {f64: i64}` and it is not there.

Everything the document says about what may be a map key:

- § 3's table row, entire: `` | `{K: V}` | map | ``.
- § 10, the whole map paragraph: `` `m[k]` `` is a `V?` with code
  `missing_key`; `m[k] @ v` inserts or replaces; `keys(m) -> [K]`; sort the keys
  to walk them in order. Not one word on which `K` is admissible.
- § 13, twice and only there: "`record Font partial` names only some, and then
  comparing it and using it as a map key are compile errors — for it and for any
  value holding it", and of a handle, "`nullptr` is its null and `==` compares
  the address; a map key is an error."

So the document refuses **exactly two** key types, both in the FFI section, both
named. A reader applies *expressio unius* and concludes that everything else is
a key. § 7 reinforces it: "`==` is structural equality on any two values of one
type", with § 13's partial record as the single stated exception. § 11 even
tells me a float sorts ("`sort` (a number, `str` or `bool`, …)") and round-trips
through text ("reads back as the same value").

I re-read those four places three times, and each pass made me **more**
confident that `{f64: i64}` is legal. That is the dangerous direction. The
brief's premise — "written out directly, `m: {f64: i64}` is a compile error" —
is not derivable from the prompt a model gets.

Consequence for this sitting: the proposal propagates to call sites a rule that
the reader has never been given. Whatever it does to the *compiler's*
consistency, for the *model* it adds a second undiagnosable refusal rather than
removing one.

---

## experiment

### Task 1 — the blind pair

**Program A**, as given. Under the specification as written: **compiles**, and
prints `1`.

Derivation, line by line:

- `function tally<K>(k: K) -> i64` — § 9: generics on functions, no constraints,
  inferred. `Generics = "<" ident { "," ident } ">"`.
- `m: {K: i64} @ {}` — § 3's `Prefix` admits `"{" Type ":" Type "}"` and `Type`
  admits a bare `ident`, so `{K: i64}` parses with `K` the type parameter. § 10:
  "an empty one needs an annotation: `m: {str: i64} @ {}`" — supplied.
- `m[k] @ 1` — § 5's `Place = ident { "." ident | "[" Expression "]" }`; § 10
  "`m[k] @ v` inserts or replaces".
- `return len(m)` — `len` is in § 11's `Built-ins:` sentence.
- `print(tally(1.5))` — § 9: "A type parameter takes its type from the
  arguments", so `K` = the type of `1.5`; § 11: `print` takes a number, and
  `tally` returns `i64`. One parameter, so the § 9 named-argument rule does not
  bite.

Nothing in the document refuses any step.

**Program B**, as given. Under the specification as written: **compiles**, and
prints `1`. Same derivation with `K` spelled `f64`, which § 3's table lists as a
type. I searched for a refusal and found only the two § 13 sentences quoted
above; neither covers `f64`.

**Do my two answers differ? No — and that is the report.** I cannot derive the
asymmetry the panel is convened over, because the document does not state its
near half. If the compiler refuses B, then by the reader's lights the compiler
and the spec disagree today, before any change is made. The sentence I used is
§ 13's: "comparing it and using it as a map key are compile errors" — it names
`partial` records and, one paragraph down, handles, and by naming them it tells
me the list is closed.

### Task 2 — the repair

Given a diagnostic at `print(tally(1.5))`, the minimal repair that keeps the
call is to change the argument's type:

```
function main()
    print(tally(1))
```

That compiles and is the repair a model under time pressure will produce. It is
also the wrong one: it silently abandons the program's purpose, which was to
tally a float. The honest repair keys on text, which § 10's own example shows is
a legal key and § 11 says round-trips:

```
function tally(k: f64) -> i64
    m: {str: i64} @ {}
    m[to_str(k)] @ 1
    return len(m)

function main()
    print(tally(1.5))
```

I dropped the generic on purpose: once the key must be `str`, the parameter's
type is no longer the key's type and `<K>` buys nothing.

**What the message would have to tell me**, none of which is in the document:

1. that the offending type is `f64` and that a float is not a map key;
2. that the constraint comes from `tally`'s **body**, and which line —
   `m: {K: i64} @ {}`;
3. what *is* a key, or at least that `str` is, since step 2 alone leaves me
   guessing between `str`, an integer, and giving up on the map.

**Would `tally`'s own text have told me?** Its signature, no: `function
tally<K>(k: K) -> i64` carries nothing, and § 9 says it never can ("no
constraints"). Its body, yes — but only to a reader who already knows floats are
not keys, which is the knowledge the document withholds. So under the proposal
the entire explanation of the refusal lives in the diagnostic string and nowhere
else.

### Task 3 — as a reader, not as a judge

Asked for "count how many times each value appears in a list, generic over the
value's type, then call it with floats", I wrote this from the specification
alone, in one pass:

```
function counts<T>(xs: [T]) -> {T: i64}
    m: {T: i64} @ {}
    for x in xs
        m[x] @ m[x].default(0) + 1
    return m

function main()
    m = counts([1.5, 2.5, 1.5])
    for k in sort(keys(m))
        print(k, " ", m[k].must())
```

**Did I expect it to compile? Yes.** How long to become sure: about three
passes over § 3's table, § 10, § 7's equality paragraph and § 13's two map-key
sentences — and the passes increased my confidence rather than shaking it,
because each one showed me a *named* refusal that did not apply to `f64`.

Two things I did while writing it are evidence for the panel:

- I put `sort(keys(m))` in `main` and not in `counts`, deliberately, because
  § 11 says `sort` takes "a number, `str` or `bool`, **never a type
  parameter**". The language has already taught me its rule for "an operation
  that does not work on every type, reached inside a generic body": **the
  operation refuses the type parameter, in the body, locally.** I obeyed that
  rule without being told, and I obeyed it in the same function where I was
  building `{T: i64}` without a qualm — because the document gives the rule for
  `sort` and withholds it for maps.
- Under the proposal, that program is refused at `counts([1.5, 2.5, 1.5])`, and
  the reader who wrote it has no sentence in the prompt to appeal to.

---

## hesitation_points

| # | where I guessed | what a wrong guess produces |
|---|---|---|
| 1 | `1.5` with no contextual type. § 2 says "A literal takes the type its context asks for … otherwise `i64`", and a type parameter asks for nothing. I assumed the float default is `f64`. | Compile error (loud) if `f32` were the default and it mattered; a silently different program only where width matters. Low risk, but the sentence as written literally assigns `i64` to `1.5`. |
| 2 | `len(m)` on a map. § 11 lists `len` with no types; § 10 never uses it on a map. | Compile error, loud. Acceptable. |
| 3 | A type parameter may appear in a body annotation, `m: {K: i64} @ {}`. § 3's grammar permits it; no prose confirms or denies. **This is the whole panel and the document is silent.** | Today: a program that compiles and runs. Under the proposal: a compile error at a *different* file's line. Either way the reader guessed. |
| 4 | Whether `f64` may be a map key. | **Today, per the brief: a compile error on the direct form the reader will write first.** This is a live first-try-rate loss that exists before this panel and that the proposal does not address. |
| 5 | Whether `to_str(k)` accepts a type parameter (the repair I did not take). § 11 says `to_str` cannot fail; `print` restricts to "a number, `str` or `bool`". | Compile error, loud. |
| 6 | Whether a partial record may be *bound to* a type parameter at all. § 13 forbids comparing it and keying on it "for it and for any value holding it" — a type parameter is not a value holding it, so I read it as permitted. | Today: compiles, aborts at run time. **This is the only silent-until-run class in the pair, and it is the one worth paying for.** |

Note on vocabulary: the brief quotes the run-time text as `panic: a partial
record has no structural equality`. The word *panic* does not occur in the
specification; § 6 defines **abort** and uses it throughout. A model that reads
its own crash cannot map that word onto anything in its prompt.

---

## argument

Today's asymmetry already costs first-try rate, and the proposal does not touch
the cause. The spec never says a float is not a map key: § 13 refuses exactly
two key types, partial records and handles, so a reader concludes floats are
fine and writes the direct form first. Refusing the call adds a second
undocumented refusal, at a line whose signature says nothing. The direction is
right — a run-time abort from `==` is the worst class in the language — but
§ 11's `sort` ("never a type parameter") is the document's own precedent for
this exact problem, and it is local. State the key rule in § 10, extend § 13's
sentence to cover type parameters, and call-site checking is left needed for
maps alone.

(119 words.)

## The two worlds, answered directly

**Which produces fewer wrong programs a model writes and believes?** The
proposal's, on the `==` case, decisively: a compile error beats a run-time abort
that a thin test suite never reaches, and "believes" is exactly what a passing
build buys. On the map case it is closer to a wash, and may be negative: today's
world lets a *correct* program through (a float-keyed tally that works, modulo
`nan`, which § 7 already warns about); the proposal refuses it and sends the
model back to a prompt that contains no rule explaining why. A refusal a reader
cannot derive is not a win, it is a retry.

**Is the generic body still readable on its own?** Yes. The body is unchanged
and self-explanatory: `m: {K: i64} @ {}` tells me `K` is keyed on. What stops
being readable on its own is the **call**: `tally(1.5)` plus the enclosing
signature `function main()` does not determine legality. That is a real
locality loss, and it is the one my seat guards.

**Is there a third spelling?** Two, and both are more local than the proposal.

1. **For the partial record — make it a property of the type, in § 13.** That
   sentence already reads "comparing it and using it as a map key are compile
   errors — for it and for any value holding it." Merge one clause: *and it may
   not be the argument of a type parameter.* Then `same(x: a, y: b)` is judged
   from the call's own types plus a sentence in the document — the callee's body
   is never opened. Fully local, one clause, on a type that is already crippled
   in two named ways, so the reader is memorising an extension and not a new
   concept. This spelling also closes shapes the proposal leaves open: a partial
   record reaching `map`, `filter`, `fold` or `find`.
2. **For the map — make it a property of the body, like `sort`.** § 11 already
   spells this: "`sort` (a number, `str` or `bool`, never a type parameter)".
   The matching sentence for § 10 is *a type parameter is never a map key*. The
   body is then judged alone, every call is legal, and the reader has already
   learned the pattern from `sort`. The cost is real and must be stated: no
   generic map-keyed container. Whether the self-hosting compiler needs one is
   not my question, but if it does not, this spelling is strictly better for a
   reader than call-site checking.

**Does the specification already answer the question?** For `sort`, yes,
explicitly, and it answers it the *opposite* way from the proposal. For maps and
for `==` through a type parameter, no — it does not even state the ground rule
the proposal enforces. The compiler, per the brief, disagrees with the document
on `{f64: i64}` today.

---

## prediction

Falsifiable, and checkable by running a model on prompts rather than by reading
code. Let task T be "count how many times each float appears in a list" and task
U be "compare two values of an `extern partial` record for equality".

1. **Today, on the current spec text**, more than half of first attempts at T
   write the *direct* form `m: {f64: i64} @ {}` rather than routing through a
   generic, because § 10's own example is `m: {str: i64} @ {}` and nothing warns
   them off. Those attempts fail to compile today. **The proposal changes this
   number by zero.** If a harness measures first-try compile rate on T and the
   proposal moves it, my model of the cause is wrong.
2. **Proposal as briefed, no spec sentence added**: first-try compile rate on T
   falls further (the generic escape hatch closes), and run-time-abort rate on U
   falls to zero. I predict the T loss is ≥ 10 points and the U gain is the full
   abort class.
3. **Proposal plus a § 10 clause naming which types may key a map**: first-try
   compile rate on T rises by ≥ 20 points over today, because the direct form —
   the one the majority writes first — becomes derivable for the first time.
   This is the largest single number available in this sitting, and it is
   available *without* the call-site rule.
4. **Repair-rate**: given the call-site refusal, first-repair success on T is
   ≥ 80% only if the diagnostic names the body line (`m: {K: i64}`) **and** one
   legal key type. Naming only the call and the offending type, I predict ≤ 50%,
   with the dominant wrong repair being `tally(1)` — the type-change that
   compiles and silently drops the program's purpose (Task 2 above; I wrote that
   repair first myself).

## condition

What flips my object to approve, any one of which is checkable before the change
lands:

- **The spec states the key rule at its home** — § 10's map paragraph or § 3's
  `{K: V}` row — so the direct form is derivable. This is the condition; the
  other two are the shape I would prefer it to take. Without it I object
  regardless of where the check fires, because a refusal no reader can derive
  produces a retry loop and not a repair.
- § 13's `partial` sentence gains the clause *and may not be the argument of a
  type parameter*, so case 2 is decided from the call site's own types.
- If call-site checking is kept for maps, § 9 says so in one clause (its "no
  constraints" sentence is currently read by a reader as "and therefore no call
  may be refused for the body's sake"), and the diagnostic names the body line.

What would flip me the other way — evidence I would accept against my own
position: a measured first-try rate showing models *prefer* the generic route on
T (contradicting prediction 1), or a measured repair rate ≥ 80% from a
call-site message that does not name the body line (contradicting 4).

## veto

**Not cast** on the proposal as briefed: one level of generic call, message at
the call site. The meaning of the call is unchanged; only its legality moves,
and today's world is worse on locality of *meaning*, since `same(x: a, y: b)`
aborts at run time for a reason visible nowhere on that line.

**Cast conditionally, and I state the trigger now so it is not a surprise
later**: if the constraint propagates **transitively** — `tally<K>` refused
because a function it calls builds the map — and the diagnostic does not name
the full chain down to the body that imposes the constraint, I veto. At that
point the legality of a line depends on a body two or more removes away, with
nothing on the line, in the signature, or in the document to reach it. That is
the C++-template failure mode, and it is precisely the class my seat exists to
refuse.
