# Panel 165 — llm-ergonomist

**Input read: two files and no others.** `docs/panel/165-briefs/llm-ergonomist.md`
and `spec/heroes-spec.md`. No design document, no compiler source, no other
report. Everything below is derived from the specification's text.

**Nothing here was compiled, and that is deliberate.** The proposed form has no
compiler, the today form needs a `slots.h` and a library this machine does not
have, and — the real reason — running the compiler would tell me what the
specification does not say, which is the one thing my seat exists to measure. So
every "would it compile" sentence below is a **prediction derived from the
document, explicitly unrun**, and says which sentence it rests on.

---

## verdict

**object.**

The read-only half of the proposal is a clear first-try win and I would approve
it alone. The proposal as written also admits `dst: i8[8]` — a parameter C writes
through — with no mark at the declaration and no mark at the call site. **On that
specific form I hold the veto**: its meaning cannot be worked out from the line
plus the enclosing signature, and the two possible meanings differ silently
rather than loudly. The amendment that turns this into an approve is one
sentence and uses machinery § 13 already has (see § condition).

**Specification sections the verdict rests on**

| § | the sentence that bears weight |
|---|---|
| 13 | "`f.ptr()` lends a binding's field to a `ptr` parameter the call gives the extent to, and C may write back through it." |
| 13 | "A C out-parameter is an `@` parameter, and what it points at is held to the same width and sign." |
| 13 | "clang checks every result type, constant and record field against that header" — **parameters are not in that list** |
| 13 | "A group's `constant` has no body: the header holds the value." |
| 3 | "Every value behaves as an independent copy: after `b = a`, mutating `b` never changes `a`. No aliasing exists among the values this language owns." |
| 3 | `Type = Prefix { "[" integer "]" } [ "?" ] .` — the extent is the lexer's `integer`, never a name |
| 5 | "only a declared `@` name can be mutated" |
| 9 | "Mutable parameters are marked `@` in the signature **and at the call site**… copy in, copy out" |

---

## Task 1 — write the program, twice

Header: `struct slot { char name[8]; int id; }; long arr_len(const char s[8]);
struct slot slot_make(void);`

### Today

```
extern "slots.h"
    record Slot tag slot
        name: i8[8]
        id: i32
    function arr_len(s: ptr) -> i64
    function slot_make() -> Slot

function main()
    t = slot_make()
    print(arr_len(s: t.name.ptr()))
```

**First try: I do not know, and the document does not let me know.** That is the
result. The declaration `s: ptr` is the only route § 13 leaves me — `cstr` is
refused outright ("Nothing lends a field to `cstr`, which promises a zero the
field does not"), and a fixed array is a field type only. But the one sentence
that authorises the route says the field is lent "to a `ptr` parameter **the call
gives the extent to**", and this call gives no extent: the C function takes one
argument. Two readings, and the specification does not separate them:

- *descriptive* — the caller is responsible for knowing the extent. My program is
  legal.
- *normative* — a `.ptr()` argument requires a sibling argument carrying the
  extent. My program is refused, and the header **cannot be bound at all** as the
  language stands.

I wrote it under the descriptive reading and could not check myself. Note which
way the failure falls: under the normative reading I get a compile error, which
is the good outcome; but I cannot then bind this header at all, and the honest
next move for a model is to invent a second parameter that C does not have.

### Proposed

```
    function arr_len(s: i8[8]) -> i64
    …
    print(arr_len(s: t.name))
```

**First try, no hesitation, and I can check myself against the header by
eye.** `char name[8]` → `i8[8]` in the field, `const char s[8]` → `i8[8]` in the
parameter, one spelling, one rule: write what the header writes. This is the
proposal's real merit and I want it recorded as measured rather than conceded:
the today form made me choose between two readings of one sentence and the
proposed form made me copy a header line.

### Where I hesitated on both, and what a wrong guess produces

| hesitation | wrong guess produces |
|---|---|
| `char` → `i8` or `u8`? § 13 says "the header's own width and sign"; C's plain `char` has no fixed sign, and the document says nothing about it. I guessed `i8`. | **loud** if § 13's "one that disagrees is refused" reaches it; otherwise silent sign confusion in `validated_bytes` and comparisons |
| `record Slot tag slot` or just `record slot`? § 4 gives records no case convention; the `tag` clause reads "the same name unless the header writes it after the word struct". | **loud** (clang) if wrong; but two readers write two different programs for one header |
| `-> Slot` — a struct returned **by value**. § 13 enumerates admissible *field* types and *parameter* types and never enumerates admissible *result* types; `record Db tag sqlite3` with no fields is a handle, mine has fields so it is the struct. | plausibly **loud**; the document leaves it to inference |
| `.ptr()` on `t`, an **immutable** binding, when C "may write back through it" (§ 5: only an `@` name can be mutated) | plausibly **loud**; unstated either way |
| `print(arr_len(…))` — § 11 says `print` takes a number, so the brief's `to_str` is optional. Harmless. | none |

Only the first and fourth are the proposal's business. The rest are today's
document and are noted for the warden, not argued here.

---

## Task 2 — the blind comparison

```
A:  function copy_name(dst: ptr, n: i64) -> i64
B:  function copy_name(dst: i8[8]) -> i64
```

**What A tells the reader.** That there is an address and a count, and — through
§ 13's `.ptr()` sentence, which the reader meets at the *call site* — that C may
write back through it. It tells them **nothing** about the element type, nothing
about the required size, and nothing about which direction the bytes flow. The
`8` is not in the declaration, so the reader must open the header to get it.

**What A lets them get wrong.** The count. `n: 8` is a number the reader invents.
Wrong by too much and C writes past a field of a Heroes record: no diagnostic,
no abort, a corrupted neighbouring field or worse. This is the worst failure
shape in the whole comparison, because it is silent at compile time *and* at run
time.

**What B tells the reader.** The element type and the extent, checked. They
cannot get the size wrong: a mismatch is a compile error, by construction of the
proposal.

**What B lets them get wrong, and this is my objection.** Whether the call
changes their value. Read B with the rest of the language in hand: § 3 says every
value is an independent copy and no aliasing exists; § 5 says only a declared `@`
name is mutated; § 9 says a mutable parameter is marked `@` **in the signature
and at the call site**. Every one of those tells the reader that
`copy_name(dst: t.name)` cannot touch `t.name`. For a function named `copy_name`
with a parameter named `dst`, it certainly does. The line

```
    n = copy_name(dst: t.name)
```

is indistinguishable, in the language as documented, from a call that reads a
copy. Whichever way the compiler resolves it, the reader is not told:

- if it passes the address of the field, § 3's no-aliasing sentence is false at
  that line and a value changed without `@` — and nothing in the source says so;
- if it passes the address of a temporary copy, the program compiles, runs,
  prints stale bytes, and is **wrong with no diagnostic at any stage**.

Form A has no such ambiguity, and the reason is instructive: the `.ptr()` is
written **at the call site**, where the reader is, and the sentence that
authorises it is the sentence that says C may write back. The write is marked in
the place it happens. B moves the whole question into a header the reader was
told they need not open.

**Which is more likely to be called correctly without opening the header?** Split
by direction, because the answer flips:

- **read-only array (`const char s[8]`)**: B, decisively. A forces the reader to
  the header to learn the `8`, which is the one thing they were trying to avoid.
- **out-buffer (`char dst[8]`)**: A. B will compile, run, and print the wrong
  thing, or will mutate through a name § 5 says cannot be mutated. A is uglier and
  the reader ends up at the header anyway — which, for a buffer C fills, is the
  correct place to send them.

---

## Task 3 — the trap

**Does anything in the language as documented warn the reader whose header says
20 where their source says 8? No, and I looked for three instruments.**

1. **clang.** § 13's own list is "clang checks every result type, constant and
   record field against that header". **Parameters are not on that list.** The
   field `name: i8[8]` is on it — so today's extent literal has a checker behind
   it. A parameter extent, as the diff proposes it, has none the document names.
   (My own knowledge of C says an array parameter's extent is not part of its
   type at all, so no C compiler could catch it even if asked; I flag that as
   mine, not the specification's, and the specification's silence is enough on
   its own.)
2. **The proposal's own check.** It "checks that the argument's extent matches" —
   the argument against the *declaration*. Both come from the Heroes source. Two
   copies of the same guess agreeing is not a measurement of the header.
3. **The `str`/bytes conversions.** `f.validated_bytes()` reads "to its first zero
   or the whole field" and fails `not_text`. It cannot fail *short*: an 8-byte
   read of a 20-byte buffer is text.

**The specification does offer a way to name a size that comes from the header,
and it is quotable:**

> A group's `constant` has no body: the header holds the value.

So `constant SLOT_NAME_LEN: i64` inside the group is a number the *header*
supplies, and clang is stated to check it. **But it cannot be used where the
proposal needs it**, and § 3's production is why:

> `Type = Prefix { "[" integer "]" } [ "?" ] .`

`integer` is a lexer token. A type's extent is a literal and can never be a
name. So the one mechanism the language has for "this number lives in the header"
is unusable in the one position the proposal creates, while it *is* usable in the
position the proposal replaces:

```
    arr_len_p(p: t.name.ptr(), n: SLOT_NAME_LEN)     # today: tracks the header
    arr_len(s: i8[SLOT_NAME_LEN])                    # proposed: ungrammatical
```

That is a real ergonomic reversal and I did not expect it before writing it out.
Today's caller-states-the-extent form can be made header-tracking by a reader who
knows § 13's constant rule. The proposed form cannot be, by anyone.

---

## argument (≤120 words)

The read-only case is a genuine win: I wrote it once, correctly, by copying the
header, where today's `ptr` route left me choosing between two readings of "the
call gives the extent to" and unable to check either. But the same syntax admits
`dst: i8[8]`, and there the call site `copy_name(dst: t.name)` reads, under § 3,
§ 5 and § 9, as a call that cannot change `t.name`. It does. Every other
caller-visible mutation in Heroes is marked `@` at both sites; this would be the
first that is not. The failure is silent either way it is resolved. And § 3's
`integer` extent blocks the header-supplied `constant` that today's form allows.

---

## prediction (falsifiable, scorable by a later sitting)

**P1.** Take twelve fresh model runs given *only* the amended specification and
this task: *"bind `long copy_name(char dst[8]);`, call it, and print the name it
wrote"*, with the group's record already written.

- Under the diff **as proposed** (fixed-array parameter, no `@`, no sentence on
  direction): **at least 9 of 12** write `copy_name(dst: t.name)` with no `@` at
  either site, and then read `t.name` expecting C's bytes. Score the *call site*
  text, not the output.
- Under the diff **amended** so a written-through fixed array is `@dst: i8[8]`
  and the call is `copy_name(@dst: t.name)`: **at least 10 of 12** either write
  the `@` at both sites or produce a program the compiler refuses with a
  diagnostic naming the missing `@`. Silent wrong-answer rate on that task:
  **0 of 12**.

**P2, subordinate and I expect it to hold for the proposal.** On the read-only
task (Task 1's header), first-try-correct rate rises: **≤ 6 of 12** today, **≥ 10
of 12** under either version of the proposal — and of today's failures, the
majority invent a length parameter the C function does not have.

**What would make me wrong.**

- If the amended and unamended forms score within 2 of each other on P1, my
  locality objection is about my reading habits and not about readers, and I
  withdraw it.
- If models under the unamended diff spontaneously write `@` anyway — because
  `dst` and `copy_name` are that suggestive — the construct is self-describing
  after all, and the objection is worth less than the syntax it costs.
- If P2 fails (today's `ptr` route scores ≥ 10 of 12), then my Task 1 hesitation
  was mine alone, the proposal buys little, and the honest verdict is object on
  cost rather than object on locality.

---

## condition — what turns this into approve

Any one of these, stated **in the specification text** and not in a design
document, since the reader gets only the specification:

1. **A fixed-array parameter through which C writes is `@`**, at the declaration
   and at the call site, with § 9's copy-in/copy-out meaning. § 13 already says
   "A C out-parameter is an `@` parameter" — this is that rule reaching one more
   type, costs a clause, and restores locality completely: the `@` at the call
   site is the mark that the value may change. **This is the resolution I
   recommend.** With it I approve.
2. **Or** the plain form is defined as copy-in-only, in words, at the point the
   type is admitted — and then a header whose array is written through is refused
   with a diagnostic that names `@`, rather than accepted and silently copied.
3. **And, either way**, one sentence saying what does or does not check the
   parameter's extent against the header. § 13's clang sentence lists result
   types, constants and fields; if parameters stay off that list, say so, because
   a refusal is held to the same standard as a feature and a reader who believes
   the `8` is checked against the header will believe it forever.

Not required, but the sitting should notice it: **§ 3's `integer` extent means
the header's own number can never be named in a type.** Whatever this sitting
resolves, that sentence is the reason Task 3's trap has no exit, and it is a
question for the document rather than for this proposal.
