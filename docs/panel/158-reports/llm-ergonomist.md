# Panel 158 — llm-ergonomist report

**Input discipline.** I read `docs/panel/158-briefs/llm-ergonomist.md` and
`spec/heroes-spec.md`, and nothing else. No design.md, no compiler, no other
brief, no test. Three `.claude/rules/` files arrived unbidden in my context
window through the harness's reminder mechanism; I did not seek them and no
sentence of this verdict rests on them. Where I might have leaned on one, I say
so and re-derive the point from the specification instead.

---

## verdict

**veto** — cast on repair 2 (flatten `m[k]` when `V` is itself fallible), on the
locality rule.

**approve** repair 3 (make `T??` legal and writable), as the complete repair.
**object** to repair 1 (refuse `{K: V?}`) as incomplete. Repair 4 (a sentence
alone) is acceptable and strictly better than today, but leaves a type that can
never appear in a signature.

Ranking for a reader, best first: **3 > 4 > 1 > 2**.

---

## experiment

### Task 1 — what is the type of `m["a"]`, and how do I get the `i64` out?

I could not answer this from the document. I finished uncertain, and the
uncertainty is not vagueness — it is a contradiction between two sections that
both look authoritative.

- § 10: "`m[k]` is a `V?` with code `missing_key`". Substituting `V = i64?`
  mechanically gives `i64??`.
- § 3, the production: `Type = Prefix { "[" integer "]" } [ "?" ] .` The opening
  paragraph says `[ ]` encloses what is optional. So a type carries **at most
  one** `?`. `i64??` is not a type this language has.

Both cannot be true. A reader cannot tell which sentence is wrong, and the two
live three sections apart with nothing pointing from one to the other.

I wrote the program under the reading I judged more likely (§ 10 is a semantic
rule, § 3's production is syntax, so the compiler computes a type the syntax
cannot write):

```
# Task 1 — current spec, my best guess at the intended program
function main()
    m: {str: i8?} @ {}
    m["a"] @ to_i8(300)          # stores a fail: 300 does not fit an i8
    m["b"] @ to_i8(7)            # stores ok(7)
    print(m["b"].must().must())
```

Two `.must()`s: the first peels `missing_key`, the second peels the stored
conversion failure. I reached this by reading § 6's table as operating on the
outermost layer, which § 5 confirms for a different operation ("Both read the
OUTERMOST type") and for no other.

**The natural program a model writes is the one-liner § 10 itself teaches.**
§ 10's own example line is `print(m["b"].default(0))`. Transplanted onto a
`{str: i8?}` it becomes:

```
print(m["b"].default(0))     # .default on an i8?? wants an i8?, and 0 is not one
```

Under today's document this is a compile error, loudly — **provided** a bare
literal does not lift into `T?`. See hesitation 2: the document does not settle
that, and the whole loudness of this line hangs on it.

**How long was I unsure: I never stopped being unsure.** Five passes over four
sections (§ 10 → § 3 production → § 6 table → § 5's outermost sentence → § 11's
`args_checked`/`find`) established what the answer *would be* under each
reading, and established that the document does not choose.

### Task 2 — write the type down

```
function take(x: i8??) -> i8      # NOT DERIVABLE
```

**There is no sentence that lets me, and the production forbids it.**
`Type = Prefix { "[" integer "]" } [ "?" ]` admits one `?`, and
`Prefix = ident [ "." ident ] | "[" Type "]" | "{" Type ":" Type "}" | "(" ")"
| "(" "function" TypeArgs "->" Type ")"` has **no `"(" Type ")"` alternative**, so
parenthesising is not an escape either: `(i8?)?` is not a type.

Two things I found while failing:

1. **Nesting is already expressible one layer out.** `[i8?]?` and `{str: i8?}?`
   both derive cleanly. Only the *bare* nesting is unwritable. So the composite
   is not foreign to the type system; it is foreign to one production.
2. **A generic parameter receives it.** This compiles, as far as the document
   lets me judge:

```
function take<T>(x: T?) -> T
    return x.must()

# call site: y = take(m["b"])   → T = i8?, y : i8?
```

§ 5's "a type parameter that arrived fallible" is the sentence that tells me a
type parameter may be bound to a fallible type. So the value can be *passed* and
never *named*: a monomorphic helper for this shape cannot be written, and inside
the generic body `T` is opaque, so nothing can be done with the peeled value
except hand it on. The factoring a reader wants — one function that handles both
layers and reports which one failed — is unwritable in this language today.

### Task 3 — `error[type_mismatch]: expected i64?, found i64??`

My first action, concretely: **search the document for `??`.** What I find is
`???` in § 12, the hole, which is a different construct and is not related. That
is the whole yield. The token in the error message appears nowhere in the one
document I was told is the whole language.

My second action: search for `fallible`. Three hits — § 3's table row, § 6's
`expr?` row ("whose return type must be fallible"), § 5's "a type parameter that
arrived fallible". None of them mentions nesting.

My third action: re-derive by substitution into § 10, arriving at task 1's
contradiction.

The dangerous branch, and it is the likely one: a reader who does not reach step
three concludes **the compiler is wrong** — because the document forbids the
type the compiler just named — and then edits until the error goes away. The
edits that make it go away are `.must()` (turning a compile-time question into a
runtime abort on a missing key) or a signature change. Neither is reported as a
guess by anything; the program compiles and the reader believes the document.

### Task 4 — `find` over a `[i64?]`

```
function is_ok(x: i64?) -> bool
    return !x.is_err()

function main()
    xs: [i64?] = [to_i64(1.5), to_i64(2.5)]
    hit = find(xs, is_ok)          # § 11: the first it accepts, else not_found
    print(hit.must().must())
```

`find` returns "the first it accepts, else `not_found`" — `not_found` is a code,
so `find` returns `T?`, and here `T = i64?`, so `hit : i64??`. **I cannot write
that type down**, for exactly the reason in task 2.

**This is the finding that decides my ranking.** The composite does not need a
map at all, and it does not need an unusual program. § 11 hands the reader
`args_checked() -> [str?]` and `find` **in the same section, thirteen lines
apart**, and § 5 blesses `[T?]` by name. `find(args_checked(), is_ok)` is a
doubly-fallible value built entirely out of built-ins, with no user type in
sight. Any claim that the shape "can never be produced accidentally" is false on
the document's own text.

### The silent program that exists today

Writing the four tasks turned up one line that compiles under the current
document and answers a different question than it appears to:

```
if m["b"].is_err()            # asks "was the key missing?", reads as "did the value fail?"
```

Same for `hit.is_err()` in task 4, and for a `match` whose `.err e` arm prints
`e.code` and thereby reports `missing_key` where the reader expected the
conversion failure's code. These compile, and nothing on the line says which of
two layers is being tested. So the status quo is not silent-free; it is silent in
a narrow band (`is_err`, and `match` arms whose bodies type-check for either
layer) and loud everywhere else (`print` of a `T?`, `.default` of a bare
literal, `return` of the wrong depth).

---

## hesitation_points

1. **Is `m["a"]` an `i64??`, or does the compiler flatten?** (§ 10 against § 3's
   production.) A wrong guess here changes *the number of `.must()`s I write*. If
   I guess "one layer" and the truth is two: **compile error, loud** at
   `print(...)` or at `return`. If I guess "two layers" and the truth is one:
   **compile error, loud**. This one is survivable today because the wrong guess
   errors. Under repair 2 it stops erroring — see the argument.
2. **Does a bare literal lift into a `T?` context?** § 2 says "a literal takes
   the type its context asks for"; § 3 says "No implicit conversions"; § 6 says a
   value is built with `ok(v)`. The document never says whether `T?` is a context
   a literal can take. This decides whether `m["b"].default(0)` — the exact
   shape of § 10's own example — is a loud error or a silently accepted program
   on a `{str: i8?}`. **I guessed "no lift".** If I guessed wrong, the wrong
   program *compiles*. This is an independent defect in the same family and I
   report it whatever the panel decides about `??`.
3. **Is an indented block legal as a `match` arm body?** `Arm = Pattern
   { "|" Pattern } "=>" ( Inline | Block )` allows it, `Block = INDENT …`, but no
   example in the document shows one, and the production writes no NEWLINE
   between `=>` and the INDENT. I needed a nested `match` for task 1's general
   form and hesitated over whether to write it inline or indented. Wrong guess:
   **parse error, loud**. Cost: time, not correctness.
4. **`m["a"] @ to_i8(300)` — is storing a `T?` a "use" under § 5's "A `_` never
   drops a `T?`"?** I guessed yes, storing is using. Wrong guess: **compile
   error, loud**.
5. **`find`'s signature.** § 11 gives prose, not a type. "The first it accepts,
   else `not_found`" told me the result is `T?` because `not_found` is a code, a
   § 6 word. I am confident but I inferred it. Wrong guess: **compile error,
   loud**.

Four of five hesitations fail loudly today. Number 2 does not, and repair 2
would move number 1 into that same silent class.

---

## argument

I could not answer task 1 from the document. § 10 says `m[k]` is a `V?`; § 3's
production allows one `?` per type. Both cannot be true and a reader cannot tell
which is wrong. The composite is not exotic: § 11 hands out `[str?]` from
`args_checked()` and a `find` returning `T?`, thirteen lines apart. Flattening is
the only repair that turns today's loud `type_mismatch` into a program that
compiles and answers a different question, and it makes `m[k]`'s result *shape*
depend on a property of `V` invisible at the line. That is the non-local class,
so I veto it. Making `??` writable costs one character in the production and puts
both layers into the signature, where a reader sees them without leaving the
line.

---

## the four repairs, ranked, and what each costs me

**1st — repair 3: make `T??` legal and writable.**
Cost to me as a writer: I must learn that `?` repeats. That is one character in
the production, `[ "?" ]` → `{ "?" }`, which is the same number of tokens. Every
§ 6 operation already composes: `.must()`, `?`, `match .ok/.err` and `.default`
each peel one layer, and § 5's "Both read the OUTERMOST type" already covers the
nested case correctly without amendment. What I *gain* is the only thing that
fixes tasks 2 and 4: `function get(m: {str: i8?}, k: str) -> i8??` becomes
writable, so the two layers are visible in a signature at every call site instead
of being discoverable only through a compile error. Under every other repair the
depth is invisible on the page forever.
One cost I checked and found small: `???` in § 12. Maximal munch makes `x???`
lex as `x` then `???`, two adjacent primaries, which is a parse error — loud.
A reader could be confused for a moment; they cannot be confused silently.

**2nd — repair 4: leave it and add a sentence.**
Cheapest, honest, and it fixes task 3 outright: my first action was to search for
`??` and find nothing, and a sentence ends that. It does not fix tasks 2 and 4 —
the type stays unwritable, so the handling can never be factored into a function
and `[T?]` stays a shape you can build and cannot name. It also has a
self-consistency cost a reader can see: the sentence must print `i64??` in a code
span, which is a token the document's own grammar refuses. Repair 3 does not pay
that.

**3rd — repair 1: refuse `{K: V?}`.**
Loud, which is the Heroes bargain, and cheap to obey. Two objections. It is
**incomplete**: it closes one of at least three doors, leaving `find`/`map`/
`fold` over a `[T?]`, `args_checked()`, and any generic whose parameter arrives
fallible — all blessed by §§ 5 and 11. Worse than incomplete, it is
*misleadingly* incomplete: a reader told `{K: V?}` is forbidden reasonably infers
the composite cannot arise, and then § 11 hands them one. And it costs a program
I would write: a map from field name to parse result is natural, and the
workaround is a one-field record, three lines and a new name for something that
needed none.

**4th, and vetoed — repair 2: flatten.**
See below.

---

## the veto, cast

**I cast it, on repair 2.**

The rule I hold it under is the locality rule as the specification states it:
§ 5, "Signatures are always explicit; inference is local only", together with
§ 3's "No implicit conversions". Under every other repair, `m[k]` adds **exactly
one** `?` — a rule I can apply from the line. Under flattening it adds one `?`
or zero, and which one depends on whether `V` is itself fallible, a fact not
present on the line and not present in the enclosing signature when `m` is a
local. Reading `m[k].must()` I could no longer tell whether I am holding an `i8`
or an `i8?` without going to find where `m` was built.

The concrete exhibit, and it is the document's own example line:

```
# on a {str: i64}, § 10's example. Loud on a {str: i8?} today. Compiles under repair 2.
print(m["b"].default(0))
```

Under flattening this line compiles on a `{str: i8?}` and prints `0` in **two
different situations that the program was written to distinguish**: the key is
absent, and the key is present holding a failure. The author cannot recover the
difference afterwards, because both arrive as one error and the stored code is
gone. That is precisely the shape this language exists to make impossible: a
plausible mistake that compiles. Every other repair leaves that line a compile
error.

A second, quieter cost of flattening: `m[k] @ ok(7)` followed by `m[k]` would
not round-trip, so a map would be the one container in the language that does
not give back what was put in it. § 3's "Every value behaves as an independent
copy" sets the expectation that containers preserve values.

---

## the sentence, if the answer is a sentence

For repair 3, the production changes `[ "?" ]` to `{ "?" }` — no tokens added —
and one clause merges into § 6's opening sentence, which is the home of the
`T?` rule:

> A `T?` is a `T` or an error: `ok(v)` or `fail(code:, msg:)`; `ok()` is the
> `()?`. `T` may itself be fallible — `m[k]` on a `{str: i64?}` is an `i64??` —
> and every operation below peels one.

The merged clause is about twenty-six tokens. It answers all four of my tasks:
it names the type (task 3's search now hits), it shows the exact construction
that produces it (task 1), it licenses writing it in a signature (task 2), and
"every operation below peels one" tells me how many `.must()`s to write for
`find` over a `[i64?]` without my having to infer it (task 4). I chose `m[k]` as
the example over `find` because `m[k]` is the construction a reader meets first
and the one § 10 already teaches with `.default`.

If the panel takes repair 4 instead, the same clause serves, minus the licence —
but it must then also say the type cannot be written, because a reader who learns
the name `i64??` and cannot put it in a signature will spend the time I spent on
task 2 discovering that by failure.

---

## answering the verdict question directly

**Is a type the compiler computes and the syntax cannot write acceptable?** No —
and the escape clause does not apply here. It would be acceptable if the type
were genuinely unreachable from written programs, the way a compiler's internal
inference variables are. This one is reachable with `find`, `map`, `fold`,
`args_checked()`, any generic, and a three-token map declaration, all of them
blessed in §§ 5, 10 and 11. Asking whether it can be "produced accidentally" has
one answer in this document: yes, by a reader following § 11.

---

## prediction

Falsifiable, on a task set whose programs touch a doubly-fallible value — a
`{str: i64?}` read, and a `find` over `args_checked()`:

1. **Status quo:** first-try compile rate on that set below 40%, and the modal
   diagnostic is a `type_mismatch` naming a type (`??`) that appears nowhere in
   the prompt. Falsified if the rate is above 60% or if `??` is not the modal
   diagnostic.
2. **Repair 3:** first-try rate on the same set rises by at least 25 points
   against the status quo, and residual failures are not about `?` depth.
   Falsified by a rise under 10 points.
3. **Repair 2 is the trap for the harness, and this is the prediction I most
   want run.** Flattening raises first-try *compile* rate higher than any other
   repair — it deletes the error — while silent-behaviour mismatch on the task
   "report whether the key was absent or the value failed" goes to effectively
   100%, because the information no longer exists at runtime. **Any harness that
   scores only "does it compile" will rank repair 2 first.** Falsified if a
   behavioural check on that task passes under flattening.
4. **Repair 1:** first-try rate on the map half of the set rises to near the
   status quo of ordinary map programs, and on the `find`/`args_checked()` half
   it does not move at all. That split is the measurement of its incompleteness.
5. Independent of the four: **hesitation 2** is checkable now. Compile
   `m["b"].default(0)` against a `{str: i8?}`. If it compiles today, the status
   quo already has a second silent class and the sentence is owed regardless of
   which repair lands.

---

## condition

- I withdraw the **veto** on repair 2 only if a program can distinguish "key
  absent" from "stored value failed" **from the line alone**, without reading
  where the map was built. Distinct codes are not enough: the result *type* would
  still vary with `V`, so I would move from veto to object rather than to
  approve. A demonstration that no program in the corpus ever needs the
  distinction does not move me; I judge what a reader can write, not what a
  corpus happens to contain.
- I raise **repair 1** above repair 4 if a measurement shows the non-map doors
  are unreachable in practice — that no generated program reaches a
  doubly-fallible value through `find`, `map`, `fold`, `args_checked()` or a
  generic. My objection is empirical and that measurement is the one that
  answers it.
- I drop **repair 3** below repair 4 if a harness shows models writing `i64??`
  where they meant `i64?` and getting a program that compiles and behaves
  differently. I looked for that failure and could not construct one — a wrongly
  deep type errors at the first use — but it is the right thing to measure
  before making the form writable.
- Nothing about the document's size changes any of the above. I am not told the
  budget and I do not judge it.
