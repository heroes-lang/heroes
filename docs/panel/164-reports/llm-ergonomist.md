# Panel 164 — llm-ergonomist report (verbatim)

## Disclosure first

I read exactly the two files I was given. Three `.claude/rules/*.md` files were pushed into my context by a system-reminder without my asking (`spec-shape.md`, `verification.md`, `records.md`); I did not act on them and nothing below rests on them. They carry no FFI semantics, so my reading of § 13 is still a fresh reader's.

## verdict

**veto** — of route 1. Per route:

| route | verdict |
|---|---|
| (1) field accepted silently wherever `cstr`/`ptr` is declared | **veto** (non-local) |
| (2) explicit lend `f.cstr()` | **object** |
| (3) explicit lend to `ptr` only, `cstr` refused | **approve, only alongside (4)** |
| (4) refuse; read the field with `validated_bytes` | **approve, with one clause added** |

**The section it rests on:** § 13 FFI, the paragraph beginning *"`s.cstr()` lends a `str` to C"* and its clause *"`f.validated_bytes()` does the same for a field of bytes, reading to its first zero or the whole field"*. Supporting: § 3 Types (*"No implicit conversions, widths included"*; *"Every value behaves as an independent copy"*; the `ptr` `cstr` row: *"an opaque pointer and a C string"*), and § 9 (*"There are no methods"*, UFCS).

## experiment

The group, as the brief gives it:

```
extern "slots.h" link "slots"
    record Slot
        name: i8[16]
    function slot_len(s: cstr) -> u64
```

Under the **current spec** (= route 4), the only `main` I could derive:

```
function main()
    s = Slot(name: [104, 101, 108, 108, 111, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
    match s.name.validated_bytes()
        .ok text => print(slot_len(text.cstr()))
        .err e   => print(e.code)
```

Under **route 1**: `print(slot_len(s.name))`.
Under **route 2**: `print(slot_len(s.name.cstr()))`.
Under **route 3**: `print(slot_len(s.name.ptr()))` is a compile error against `s: cstr`; to write the program at all the binding must become `function slot_len(s: ptr) -> u64`, and then no `str` can reach it, because § 3 forbids the `cstr`→`ptr` conversion. Binding `strlen` twice under two Heroes names is not derivable either: § 13's examples (`sqlite3_open`) and *"clang checks every result type, constant and record field against that header"* tell me the Heroes name is the C name.

## Task A, in full, including the wrong answers

Seven routes, in the order I actually tried them.

1. `slot_len(s.name)` — abandoned in under a second. § 3: *"No implicit conversions, widths included"*, and § 13 holds a parameter to the header's own width and sign. Refused loudly.
2. `slot_len(s.name.cstr())` — my second reflex, and the one I expected to work. Refused by § 13's own words: *"`s.cstr()` lends a `str` to C"*. `i8[16]` is not a `str`. Also unreachable by UFCS: § 9 says `x.f(y)` is `f(x, y)` and there are no methods, and `cstr` is not in § 11's `Built-ins:` sentence — so `.cstr()` is a form § 13 introduces privately, and I have no rule that says which receivers it accepts beyond the one letter `s`.
3. `slot_len(to_str(s.name).cstr())` — abandoned in a second. § 11: `to_str` is for *"the types this language renders as text: a number, `str` or `bool`"*. A fixed array is none.
4. `s.name.validated()` — I wrote this before re-reading. § 13: *"`c.validated()` copies one back as a `str?`"* — receiver `c`, a `cstr`. Wrong receiver.
5. `while s.name[i] != 0` — the pure-Heroes route, computing the length myself. Abandoned because **nothing in the specification says a fixed-array field can be indexed.** § 13 says only how to *build* one (*"build one with `[a, b, c, d]`"*); § 10's indexing is stated over `str`, `[T]` and `{K: V}`; § 3's table has no row for `T[N]` at all — the form exists only in the `Type` production and in § 13's prose. I could not tell whether this compiles.
6. `len(s.name)` — same gap. § 11 offers `len`, but over what is never said for a fixed array. And it would give 16, not the C answer.
7. `s.name.validated_bytes()` — the one door. It is the only construct in the document that takes a fixed-array field and produces something Heroes can then hand to C.

Two guesses I could not close from the document:

- **Is `i8[16]` "a field of bytes"?** § 3 says `s[i]` yields a `u8` and § 10's byte vocabulary is `u8` throughout, so a reader may well conclude `validated_bytes` wants `u8[N]` and that an `i8[16]` bound from `char` is outside it. I guessed yes. Wrong guess is a compile error — loud, so acceptable, but it costs a first try.
- **May a Heroes program construct a group `record` with `Slot(name: [...])`?** § 9 says construction is a call with field names; § 13 restricts `partial` records (no comparison, no map key) and handles, and says nothing about constructing a plain one. I guessed yes.

## Task B — the sentence I looked for

I looked in exactly one place: the second half of § 13's `cstr` paragraph, at the clause `f.validated_bytes()`. It is the only sentence in the whole document that says the words *a field of bytes*, and it has already introduced a receiver letter of its own, `f`, distinct from `s` (a `str`) and `c` (a `cstr`). Having established a third receiver, I expected that sentence to be complete about it — to say what a field can do in **both** directions.

What I expected to read, in one of two shapes:

- the outbound door: *"…and `f.cstr()` lends the field's bytes, which end where C put a zero and not before"* — i.e. a form; or
- the refusal with its reason, which is what a reader needs more: *"nothing lends a field to C: its bytes carry no terminator, so pass the `str` that `validated_bytes` gives."*

What I found was neither: the sentence is a one-way door and says nothing about the direction every C programmer will try first. The absence is the defect, independent of which route wins. A reader arriving from C makes two attempts (`s.name`, then `s.name.cstr()`), collects two compile errors, and the document contains no sentence that routes them to the answer. `validated_bytes` is findable only by reading § 13 whole and noticing the letter `f`; it is absent from § 11's `Built-ins:` sentence, which is where a reader looks for the set of names the compiler offers.

**What a reader carries from `s.cstr()` into a field lend, and whether it survives.** They carry three things. *(a)* No copy: C reads the bytes in place — still true of a field, though now the bytes live in a by-value copy of the record (§ 3) rather than in an immutable `str`. *(b)* The lend may stand only as a call argument and lives for that call — still true. *(c)* **C receives something that ends at a zero, because that is what the type `cstr` — "a C string" (§ 3) — names, and because a `str` is a Heroes value whose extent Heroes knows.** This is the half that dies. A `char[16]` ends where C put a zero, or nowhere. (c) is the only one of the three that any C string function actually consumes.

## Task C — blind A/B, all three answered before comparing

**(A) `slot_len(s.name)`.** Belief: C receives the field's address, by the decay every C programmer has internalised. What the spec lets them believe about where the bytes end: **nothing at all.** `i8[16]` is sixteen bytes and § 13 promises no zero among them. Worse, § 3's *"No implicit conversions, widths included"* is a global rule a reader leans on everywhere; a line that quietly breaks it teaches them the compiler knows something they do not, and they will trust the next such line too. Overrun risk: high, and the damage is not confined to this line.

**(B) `slot_len(s.name.cstr())`.** Belief: precisely what the one citable sentence says — *lends a `str` to C* — therefore a terminated run of bytes whose length Heroes knows. **That belief is false here and the spelling is what makes it false**: the reader is not guessing, they are correctly applying the only rule the document gives them. Overrun risk: high, and uniquely blameless — the reader did everything right.

**(C) `slot_len(s.name.ptr())`.** Belief: C receives a raw address. § 3 gives `ptr` exactly one property — *"an opaque pointer"* — and no termination anywhere. The reader carries no false promise, because none was offered. Against `slot_len(s: cstr)` this is a compile error, loud, at the call.

**Comparing.** (C) is the spelling least likely to produce a program that reads past the field on an untested machine, and the reason is structural rather than rhetorical: it produces a type that cannot reach a terminator-assuming parameter, so the type system does the stopping. (B) is the most dangerous of the three, above (A), because (A) at least *looks* like raw C and invites the suspicion a reader reserves for raw C, while (B) launders the same overrun through a Heroes verb and a type whose name asserts the missing property.

## hesitation_points

| where I guessed | wrong guess produces |
|---|---|
| Does `validated_bytes` accept `i8[N]` or only `u8[N]`? | compile error — loud |
| Can a fixed-array field be indexed, `s.name[i]`? | unknown to me; if it compiles, a hand-written scan is the natural answer and nobody reaches `validated_bytes` |
| May a program construct a group `record`? | compile error — loud |
| Is `.cstr()` a built-in (§ 11 does not list it) or a § 13-only form? | affects which receivers I believe it has; under route 2 this becomes a **silent** difference, since the answer decides whether `s.name.cstr()` was ever meant to work |
| Under route 1: does `s.name` copy sixteen bytes or hand over an address? | **silently different program** — decided by the callee's parameter type, off the line |
| Under route 2: does the field end in a zero? | **silently different program** — decided by C at runtime, on a machine I did not test |

## argument

The document gives a reader exactly one sentence about `cstr`, and it says a lend is of a `str`. Everything a reader carries from it survives into a field lend except the terminator, which is the only part C consumes. Route 2 spells the false half and route 1 hides it; both convert a compile error into an overrun that appears on somebody else's machine. Route 3 is honest but does not make this program writable — it buys the `void *` case and should land only beside route 4. Route 4 already works and costs one clause: the `validated_bytes` sentence must say that nothing lends a field, and why. A refusal a reader can find beats a form a reader can misread.

## prediction

Falsifiable, checkable next harness run. Give fresh models Task A (spec plus the group, asked for the length of the C string in `name: i8[16]`):

- **current spec:** fewer than 1 in 3 first attempts compile; `slot_len(s.name)` or `slot_len(s.name.cstr())` appears in more than half of them; silent-overrun rate 0.
- **route 4 + the clause** (`validated_bytes`'s sentence naming the refusal and the reason): compile-on-first-try above 2 in 3; silent-overrun rate still 0.
- **routes 1 or 2:** compile-on-first-try above 4 in 5, and on a fixture whose sixteen bytes are all non-zero, **at least 1 in 3 of the compiling programs read past the field** — zero of the route 3/4 programs can, because no expression of type `cstr` is derivable from the field.
- **route 3 alone:** compile-on-first-try *below* the current spec's, because the only way through is to rewrite the header binding.

## veto (non-local constructs)

**Vetoed: route 1.** The line is

```
    slot_len(s.name)
```

and its meaning is not determined by the line plus the enclosing signature, which is `function main()`. The same expression `s.name` means "sixteen bytes, an independent copy" (§ 3) when passed to a Heroes function and "the address of the bytes inside my copy" when passed to a `cstr` or `ptr` parameter. To know which, a reader must go and find **`slot_len`'s `extern` declaration** — and then, to know where the bytes end, must go and find **the C source that filled the field**, which is not in the program at all. Route 1 also silently revokes § 3's *"No implicit conversions, widths included"* for one case with no sentence marking the exception at the call site.

Routes 2, 3 and 4 are all local — each line names the conversion it performs — so the veto does not reach them. Route 2 gets an objection instead, on the false promise, not on locality.

## condition

What would change my verdict:

- If a measured run shows that under **route 4 plus the clause**, fewer than half of fresh models reach `validated_bytes`, the clause has not fixed discoverability and I move to **route 3 beside route 4**, so there is a local, promise-free spelling to find.
- If it is measured that the diagnostic for `slot_len(s.name)` does **not** name `validated_bytes` in its own text, route 4's cost is higher than I priced it, and the clause stops being a recommendation.
- If it is measured that the checker already accepts `s.name[i]` (a hand-written scan), then route 4's discoverability problem is worse than I said — readers will write the scan and never meet `validated_bytes` — and § 13 owes a sentence either way about whether a fixed-array field indexes.
- I would drop the veto on route 1 only on a demonstration that the decay cannot be written without a marker visible on the line itself, which is route 2 or 3 by another name.
