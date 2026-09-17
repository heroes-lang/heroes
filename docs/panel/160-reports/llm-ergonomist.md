# Panel 160 — llm-ergonomist

## Input discipline

Read: `docs/panel/160-briefs/llm-ergonomist.md` and `spec/heroes-spec.md`.
Nothing else was opened.

Refused: `CLAUDE.md`, the memory index, and three `.claude/rules/` files
(`verification.md`, `records.md`, `spec-shape.md`) which the harness pushed
into my context unasked, the last three after my two reads. None of them is
consulted below; the third carries measurements and design history that this
seat must not see.

No compiler was available. Every statement about what the compiler does is an
inference from the document and is marked `(inference)`.

One disclosure about the experiment itself: the brief's first paragraph names
the two levels before the tasks begin, so my "first reach" is partly a
reconstruction. I report what I wrote before I went back to § 10 a second time,
and I say where the priming may have helped me.

## verdict

**approve — Version Q.** If Version P is adopted alone, **object**.
No veto: neither version is non-local.

## experiment

### Task 1 — what I reached for first (current specification)

```
function parse(s: str) -> i64?
    if s.len() == 0
        return fail(code: "empty", msg: "no digits")
    n: i64 @ 0
    for i in range(from: 0, to: s.len())
        d = s[i]
        if d < '0' || d > '9'
            return fail(code: "not_a_digit", msg: s)
        n @ n * 10 + to_i64(d - '0')?
    return ok(n)

function main()
    inputs = ["12", "x7", "300", "", "42"]
    m: {str: i64?} @ {}
    for s in inputs
        m[s] @ parse(s)
    if m["x7"].is_err()
        print("x7: the parse failed")
    else
        print("x7: the parse succeeded")
```

**Built-in reached for at the question:** `.is_err()`, and I believed it was
asking *"did the parse stored under `"x7"` fail?"*, because the map was declared
to hold parse results and the name of the test matches the name of the question.

What it asks instead, by § 10's own sentence (*`m[k]` is a `V?` with code
`missing_key`*) and with `V = i64?`: `m["x7"]` is an `i64??`, and `.is_err()`
tests the wrapper `m[k]` added, not the value I stored. `"x7"` is present, so
the test is `false`, and the program prints **"x7: the parse succeeded"** for a
string that has no digits. `(inference)` that `.is_err()` reads the outer level
under the current text; the document does not say, which is the whole matter.
The program compiles `(inference)`: every type on the line is well-formed.

Whether I would have caught this unprimed: I doubt it. § 6's table says
`.is_err()` is *"boolean test"* and stops; § 10's example `m["b"].default(0)`
shows a single peel on a `{str: i64}`, and the eye carries that shape to a
`{str: i64?}`.

### Task 1 — the program I write once the type is computed (same under P and Q)

```
function main()
    inputs = ["12", "x7", "300", "", "42"]
    m: {str: i64?} @ {}
    for s in inputs
        m[s] @ parse(s)
    match m["x7"]
        .ok r =>
            if r.is_err()
                print("x7: the parse failed")
            else
                print("x7: the parse succeeded")
        .err absent => print("x7 was never stored: ", absent.code)
```

Or, accepting an abort when the key is absent:

```
    if m["x7"].must().is_err()
        print("x7: the parse failed")
    else
        print("x7: the parse succeeded")
```

Both name two operations for the two `?` in `m`'s declaration. The first names
each level on its own line (`.err absent` is *missing*, `r.is_err()` is
*failed*); the second shows two peels on one line.

### Task 2 — first failed result, its `code`, with `find`

```
function failed(r: i64?) -> bool
    return r.is_err()

function main()
    results: [i64?] = ["1", "x7", "3"].map(parse)
    match results.find(failed)
        .ok r =>
            match r
                .ok _  => print("find handed back a success; unreachable")
                .err e => print(e.code)
        .err none => print("no failure: ", none.code)
```

**What `find` handed me:** an `i64??`. § 11 says `find` gives *"the first it
accepts, else `not_found`"*, and `not_found` is a code, so the result is `T?`
with `T` the element type `i64?`. **Two levels opened**: the outer is *did find
find anything*, the inner is the element's own success or failure. The
`.ok _ => …` arm is dead by construction (the predicate only accepts failures)
but exhaustiveness demands it, and that is right: the document cannot know the
predicate's meaning.

The tempting shorter program, which I also wrote:

```
    first = results.find(failed)
    if first.is_err()
        print("no failure")
    else
        print(first.must().code)
```

Here `first.is_err()` asks the OUTER question, and in this task the outer
question is the one wanted, so it is right. `first.must()` is an `i64?`, and
`.code` on it is, I infer, refused, since the document only ever reads `.code`
from an `e` bound by `.err e` `(inference)`. Loud. Note what this shows next to
Task 1: the same line shape, `x.is_err()` on an `i64??`, is the right question
in Task 2 and the wrong question in Task 1, and nothing on the line says which.

### Task 3 — blind reading of P and Q, after re-reading § 3 and § 10

**Version P** (*"every operation below peels one"*). Yes, it changes Task 1: I
now know the bare `.is_err()` asks *missing*, so I reach for `.must()` and then
`.is_err()`, or for `match`. But it changes it only for the reader who reaches
that sentence. It sits in § 6 after `ok()`; a model reading § 6's TABLE sees
`.is_err() | boolean test` and stops. And the wrong program of Task 1 still
compiles under P; P defines its meaning without refusing it.

**Version Q** (*"`match` names both levels; `.is_err()` asks only the outer one
and is refused on such a value"*). Yes, and more: it tells me that if I forget,
the compiler will not let the line stand. The built-in I now reach for at the
question is `match` (one per level), or `.must()` followed by `.is_err()`, which
Q permits because `.must()` peels the value to an `i64?` and the test then lands
on a single-level value `(inference)`. What Q takes away: the outer question
alone, *is the key present*, no longer has a one-call test and costs a `match`.

**Is the forced `match` more local, or only more text?** More local. Locality
here is whether a reader holding `if m["b"].is_err()` and `m: {str: i64?}` can
tell which question is asked. Under P the reader can, but only by carrying a
rule from § 6 (*peels one*) to the line and counting `?` in the declaration; the
line itself reads as either question, and Task 2 shows the same line meaning
the right thing elsewhere. Under Q the line cannot exist; what replaces it shows
one operation per level, on the line or on its own arm. The reader counts
operations against `?`s and needs no peel rule.

## hesitation_points

Each with what a wrong guess produces.

1. **`.is_err()` on `m[k]` when `V` is fallible** — the point of the sitting.
   Wrong guess compiles and prints the opposite sentence (silent) under current
   and P; compile error under Q `(inference)`.
2. **Parsing a `str` into a number.** § 11 says `to_i8 … to_u64` give a `T?`
   *"because the number may not fit"* and `to_i64` *"takes a float too"*; it
   never says `to_i64` takes a `str`. I wrote my own `parse` rather than guess.
   A model that writes `to_i64(s)` on a `str` gets a compile error if the
   overload does not exist (loud) `(inference)`; if it does exist the program is
   right. Not this sitting's question, but it is the first hesitation on the
   page.
3. **`to_i64(d - '0')` on a `u8`** gives an `i64?` by § 11's sentence, so I
   wrote `?` after it. If it gave a plain `i64`, the `?` is refused (loud).
4. **`'0'` taking type `u8` from `d`** by § 2's context rule. Wrong: `u8`
   against `i64` is refused (loud).
5. **Block arm `.ok r =>` followed by an indented block.** § 8 says an arm body
   is *"one statement, inline, or an indented block"*; I trust the INDENT after
   `=>` the way a function body follows its signature. Wrong: parse error (loud).
6. **`["1", "x7", "3"].map(parse)` giving `[i64?]`** by § 9's *"`xs.map(double)`
   takes both from `double`'s signature"*. Wrong: type error (loud).
7. **`.code` read directly on an `i64?`** (the tempting Task 2). I infer refused
   (loud). If it were permitted, the tempting program would be right and the
   document would be missing a sentence.
8. **Q's phrase "`match` names both levels".** I first read it as promising a
   nested pattern, `.ok .err e =>`. § 8's `Pattern` production offers no nesting,
   so that reading yields a parse error (loud, but a wasted first try). *"`match`
   opens one level per arm"* or *"two `match`es name the two levels"* would not
   invite it.
9. **Under Q, the repair for the OUTER question.** A model refused on
   `m[k].is_err()` while wanting *is the key present* may "fix" it as
   `m[k].must().is_err()`. That compiles `(inference)`: it aborts on an absent
   key (loud at run time) and answers the INNER question on a present one, so a
   present key holding a failed parse reads as *missing*. Silent on that branch.
   The refusal's message must name both questions and the route to each, or Q
   moves part of the silent error rather than removing it.
10. **Does Q's refusal reach a `T??` that arrives through a type parameter?**
    § 5 says `_` reads the OUTERMOST type even when a type parameter *"arrived
    fallible"*; Q says nothing of the kind. If the refusal stops at the written
    type, `x.is_err()` inside a generic body with `x: A?` and `A = i64?` asks
    the outer level silently. Undecided by the sentence `(inference)`.

## argument

`.is_err()` is the one § 6 operation whose result forgets the level. `.must()`,
`.default`, `?` and `match` hand back a value still typed `i64?`, so a wrong
depth fails at its next use; `.is_err()` hands back `bool`, and the wrong
question compiles. I wrote that wrong program first for Task 1. P defines the
answer as the outer level but leaves the line reading as either question; the
same line is right in Task 2 and wrong in Task 1, and nothing on it says which.
Q refuses the line, so the mistake becomes a compile error and its replacement
shows one operation per `?` in `m`'s declaration. That is more local, not only
more text. Approve Q; object to P alone.

## prediction

Falsifiable when the harness next runs Task 1 and Task 2 over N first tries.

- **Task 1, current text and P:** at least 4 in 10 first-try programs contain a
  single-peel `.is_err()` on `m[k]`, compile, and print the wrong sentence.
  P lowers this from the current rate by less than half, because the sentence
  sits after the table a reader stops at.
- **Task 1, Q:** the silent-wrong count is 0; the same programs fail to
  compile. First-try compile rate falls by at most the fraction that was
  silently wrong. Second-try correctness is at least 9 in 10 **if** the
  refusal's message names both questions and both routes; if it names only
  `match`, at least 1 in 5 second tries for the *is the key present* variant
  land `.must().is_err()` (hesitation 9).
- **Task 2:** no delta between P and Q for programs using `match` on `find`'s
  result (I predict 7 in 10 or more do). The `if first.is_err()` idiom, at most
  2 in 10, is refused under Q at the cost of one more `match`, with no silent
  error either way, because in Task 2 the outer question is the one wanted.
- **Hesitation 8:** at least 1 in 10 first tries under Q's exact wording write
  a nested pattern `.ok .err e`, which the grammar refuses.

## condition

I would withdraw approval of Q if the harness shows any of:

- (a) at least 1 in 5 post-refusal repairs ask the wrong level
  (`.must().is_err()` where *is the key present* was wanted), which means the
  refusal relocated the silent error; the cure is the message, not the rule;
- (b) the refusal does not reach a `T??` that arrives through a type parameter
  (hesitation 10), so `find`'s or a user generic's body tests the outer level
  silently, which makes the rule porous exactly where the document already
  admits nested fallibles;
- (c) the nested-pattern misreading of *"names both levels"* costs more first
  tries than the bare `.is_err()` it prevents.

I would move P to approve if its measured silent-wrong rate on Task 1 is at or
under 1 in 20, which would mean the sentence alone suffices and the refusal buys
nothing. I would move to veto neither: both versions keep the meaning of the
line derivable from the line and `m`'s declaration.
