# Panel 166 — llm-ergonomist

**Input read: `spec/heroes-spec.md` and this seat's brief. Nothing else.**

**Every "would it compile" claim below is DERIVED FROM THE TEXT AND UNRUN.** I
did not build, did not run, and did not open the compiler. Where I say a form is
accepted or refused, I mean *a reader of this document has no ground to expect
otherwise*, and a measured run may contradict me. That is the point of the seat.

---

## verdict

| route | verdict | rests on |
|---|---|---|
| **A** — a `ptr` lend from a `=` binding is refused | **approve** | § 5 (`=` binds once, forever), § 3 (no aliasing among owned values), § 13 (`f.ptr()`) |
| **B** — a `ptr` C writes through is `@` at the declaration and the call site | **approve**, and it is the one I would land first | § 13 (*A C out-parameter is an `@` parameter*), § 9 (*Mutable parameters are marked `@` in the signature and at the call site*), § 5 (*only a declared `@` name can be mutated*) |
| **G** — narrow § 3 and § 5 to admit the hole, check nothing | **VETO** | § 3, § 5, and the locality rule |

**B subsumes A.** Under B the Task-1 program is already refused without a new
rule: § 9 says the `@` mark is written at the call site, and § 5 says *only a
declared `@` name can be mutated*, so `blob_fill(@b.bytes.ptr(), 8)` where `b`
came from `=` is a mutation of a non-`@` name. I would still state A explicitly,
because that inference crosses two sections and this seat's job is to not make
readers do that.

**Neither A nor B reaches Task 2 or Task 3.** The extent is a separate hole and
it is the one that silently corrupts memory. Said plainly below, because a
recommendation is a claim about the option set and I do not want this sitting to
read "approve B" as "closed".

---

## Task 1 — the immutable binding

### The sentences I used, quoted

§ 3 Types:

> Every value behaves as an independent copy: after `b = a`, mutating `b` never
> changes `a`. **No aliasing exists among the values this language owns.** A
> `ptr` or a `cstr` is a copied ADDRESS, wherever it sits: two copies reach one
> foreign thing, so a function taking one without `@` may still change, or free,
> **what C holds**.

§ 3 again:

> A record or variant holds its fields **by value** […]

§ 5 Bindings:

> `x = 5              # immutable binding, type inferred`
> `v: i64 @ 0         # mutable declaration — the type is REQUIRED`
> `v @ v + 1          # mutation; only a declared @ name can be mutated`

> `=` binds once, forever. `@` declares a mutable cell and re-binds it, or a
> field or element inside one.

§ 13 FFI:

> A C out-parameter is an `@` parameter, and what it points at is held to the
> same width and sign — `@n: u64` where it says `size_t *`

> `f.ptr()` lends **a binding's field** to a `ptr` parameter the call gives the
> extent to, and C may write back through it.

### The program, as a reader of that document writes it

```
extern "blob.h" link "blob"
    record Blob
        bytes: u8[8]
    function blob_fill(p: ptr, n: i64)

function main()
    b = Blob(bytes: [0, 0, 0, 0, 0, 0, 0, 0])
    blob_fill(b.bytes.ptr(), 8)
    print(b.bytes[0])
```

Accepted, DERIVED AND UNRUN. `b` is bound with `=`; C fills eight bytes of it;
the last line prints what C wrote. `b` changed after `=` bound it *forever*.

### Does the document answer, contradict itself, or stay silent?

**It contradicts itself, and the contradiction is invisible at the point of
use.**

- § 3's carve-out is scoped: *"a function taking one without `@` may still change,
  or free, **what C holds**."* What `f.ptr()` hands over is not what C holds. It
  is a field of a value **this language owns** — the same sentence's other half
  says no aliasing exists among those.
- § 5's `=` is stated with no exception anywhere in the document.
- § 13's fourteen words grant the exception without naming either sentence it
  overrides.

**What I think a reader concludes: that the program above is correct and
ordinary** — not that the document is silent, and certainly not that it is
contradictory. Two reasons, both textual:

1. The § 13 sentence says *"a binding's field"*. § 5 introduces `=` as *"immutable
   **binding**"* and `@` as *"mutable **declaration**"*. The word chosen in § 13
   is § 5's word for the `=` form. A reader matching vocabulary lands on `=`.
2. § 13 spells `@` out for the neighbouring case — *"A C out-parameter is an `@`
   parameter"* — two sentences earlier. A mark present on one form and absent on
   the next reads as deliberate. The absence of `@` on `f.ptr()` is evidence, to
   a careful reader, that `f.ptr()` is **not** an out-parameter.

That second point is the one that worries me most. **The document teaches the
rule and then shows the exception unmarked.** A careless reader gets it wrong; a
careful reader gets it wrong *with a reason*.

### One more thing the sentence does not bound

The subject is *"a binding's field"*, not *a group record's field*. Nothing in
§ 13 restricts `f.ptr()` to a `record` declared inside an `extern` group. So this
reads as licensed too, DERIVED AND UNRUN:

```
record Point
    x: i64
    y: i64

function main()
    p = Point(x: 3, y: 4)
    blob_fill(p.x.ptr(), 8)      # C writes into an ordinary Heroes record
    print(p.x)
```

If that is accepted, § 3's aliasing sentence is false for plain records, not only
for FFI structs, and the reach of the hole is unbounded by the text.

---

## Task 2 — the extent

### The program

```
extern "sum.h" link "sum"
    record Buf
        data: u8[8]
    function sum_n(p: ptr, n: i64) -> i64

function main()
    b = Buf(data: [1, 2, 3, 4, 5, 6, 7, 8])
    total = sum_n(b.data.ptr(), 64)
    print(total)
```

Named arguments are not mandatory here: § 9 requires them *"When two parameters
in a signature share a type"*, and `ptr` and `i64` differ.

### What stops a reader writing `n: 64`?

**Nothing. Not one sentence in the document.** I looked for four things and found
none of them:

1. **A unit for the word "extent".** The word occurs exactly once, in the
   `f.ptr()` sentence, and is never defined. Elsewhere the document mixes units
   deliberately: § 3 says `str` is *"indexed and measured in bytes"*, while § 11's
   `len` over an array is elements (nowhere stated either, only implied by
   `xs.len() == 0` in § 6's example). So **bytes or elements is a coin flip**, and
   for `u8[8]` the coin lands the same way both times — the reader's test passes.
   Change the field to `i32[4]` and `4` and `16` are both defensible readings of
   the same sentence.
2. **Any syntactic link between the lend and the number.** `b.data.ptr()` and
   `64` are two independent arguments. The number's type is `i64` and it accepts
   every `i64`.
3. **A bound the runtime would catch.** § 10 says *"An out-of-bounds index or
   slice aborts"* — that is this language's indexing. C's read of 64 bytes from an
   8-byte field is not an index, and § 6's abort has no site here.
4. **An example.** The only fenced FFI example in § 13 is the sqlite one: a
   handle record, no field record, no fixed array, no `f.ptr()`, no extent. **The
   form with the worst failure mode is the one form in § 13 with no example.**

`n: 64` — bits instead of bytes — is the single most likely wrong number a model
writes for an 8-byte field, because "8 bytes" and "64 bits" are the same fact and
only one of them is the argument. It compiles (DERIVED, UNRUN) and reads 56 bytes
past the field.

### What the document would have to say for something to stop it

Three shapes, cheapest first, all absent today:

- **Define the unit in the sentence** — *"the extent in bytes"*. Costs three
  words, converts a coin flip into a lookup. Does not stop `n: 64`.
- **Check the literal when both are visible.** *"Where the call gives a literal
  extent and the field's size is declared, a larger extent is a compile error."*
  Stops `n: 64` and `n: 4096`; stops nothing computed.
- **Name the extent in the signature**, so the pair is one thing rather than two
  arguments: the `ptr` parameter cites the parameter carrying its extent, and the
  call site supplies one number the compiler derives from the field. This is the
  only shape that makes the line self-explaining, which is what the locality rule
  asks for.

---

## Task 3 — naming the number

The reader's motive is good: the header may spell it differently on another
machine. Here is every route the document licenses, with what I expect. **All
four are DERIVED AND UNRUN.**

**(a) Ask the field its length — `b.data.len()`.**
§ 11 lists `len` among the built-ins with no signature. § 3's table defines `[T]`
as *"dynamic array"*; § 13 says a field is *"a fixed array of one: `i32[4]`,
**never a `[T]`**"*. So the document explicitly says the fixed array is not the
type `len` is documented against, and never says whether `len` accepts it.
**Expectation: genuinely 50/50, and both outcomes are bad in different ways.** If
refused, the reader's most natural route is closed with a diagnostic (survivable).
If accepted, it yields elements, which equals bytes for `u8[N]` and is wrong by
4x for `i32[N]` — **the reader tests on bytes, it works, and the same expression
is silently wrong on the next field.**

**(b) Declare the field with a named extent — `data: u8[BUF_LEN]`.**
§ 3's production is explicit:

>     Type     = Prefix { "[" integer "]" } [ "?" ] .

`integer` is *"what the lexer builds"* (opening paragraph); `BUF_LEN` is an
`ident`. **Expectation: syntax error.** This is the right answer loudly given —
and it means **the one route a careful reader reaches for first is closed**, which
pushes them to (c).

**(c) A group `constant`.**

```
extern "sum.h" link "sum"
    constant BUF_LEN: i64
    record Buf
        data: u8[8]
    function sum_n(p: ptr, n: i64) -> i64

function main()
    b = Buf(data: [1, 2, 3, 4, 5, 6, 7, 8])
    total = sum_n(b.data.ptr(), BUF_LEN)
    print(total)
```

Licensed by § 13: *"A group's `constant` has no body: the header holds the
value"*, and *"clang checks every result type, constant and record field against
that header"*. **Expectation: accepted.** And this is the worst outcome on the
page. `BUF_LEN` is checked against the header; `u8[8]` is checked against the
header; **the two are never checked against each other**, because the document
names no relation between a field's declared size and a `ptr` parameter's extent.
A header that says 16 hands C sixteen bytes to write into an eight-byte field,
and the program that does it is the one written by the reader who refused to
hardcode.

**(d) A top-level `constant BUF_LEN: i64` with body `8`.**
Legal (§ 4), and unusable where it is needed: (b) says the type cannot name it. So
the literal `8` is written in the type and the name is written at the call, and
nothing ties them. **Two places, silent drift.**

**The finding.** Of four routes, the document closes the safe one at the grammar,
leaves the natural one unspecified, and licenses only the one that can be silently
wrong by construction.

---

## experiment — the same task under B

Declaration and call, under route B:

```
extern "blob.h" link "blob"
    record Blob
        bytes: u8[8]
    function blob_fill(@p: ptr, n: i64)

function main()
    b: Blob @ Blob(bytes: [0, 0, 0, 0, 0, 0, 0, 0])
    blob_fill(@b.bytes.ptr(), 8)
    print(b.bytes[0])
```

§ 9's `Arg = [ ident ":" ] [ "@" ] Expression` already admits `@` before any
expression, so **B needs no grammar change** — only prose. § 9 already shows the
shape: *"`function advance(@l: Lex)` … `advance(@l)`"*.

Two spellings are available and the sitting must pick one:

- `@b.bytes.ptr()` — keeps one lend form; `f.ptr()` stays the read-only lend and
  `@` adds the write. **My recommendation**, because it leaves a reader able to
  lend read-only without learning a second form.
- `@b.bytes` — the `@` on a field argument *is* the lend. Shorter, but it is a
  second lend spelling and § 13 already carries four (`cstr()`, `lease()`,
  `validated()`, `validated_bytes()`).

And the same program under B with the `=` binding kept:

```
    b = Blob(bytes: [0, 0, 0, 0, 0, 0, 0, 0])
    blob_fill(@b.bytes.ptr(), 8)
```

**Refused** (DERIVED, UNRUN), by § 5's *"only a declared @ name can be mutated"* —
which is route A arriving for free.

Under A alone, the call line is `blob_fill(b.bytes.ptr(), 8)` with `b: Blob @ …`
above it. The immutability lie is gone; **the locality problem is not**: nothing
on the call line says C writes.

---

## hesitation_points

Where I guessed, and what a wrong guess produces. All DERIVED AND UNRUN.

| # | I guessed | wrong guess produces |
|---|---|---|
| 1 | that `b = Blob(...)` may be lent to a writing `ptr` at all | **silently different program** — today's whole question |
| 2 | that the extent is bytes, not elements | **silent out-of-bounds C access**; identical for `u8[N]`, 4x wrong for `i32[N]` |
| 3 | that `f.ptr()` needs a *fixed array* field rather than any field | compile error, probably — but if any field works, § 3 breaks for plain records too |
| 4 | that a group `record` can be constructed with `Blob(bytes: [...])` — § 9's construction rule is stated for this language's records and § 13's only example is a handle | compile error (survivable) |
| 5 | that `b.bytes[0]` indexes a fixed array; § 3's table defines indexing for `[T]` and `str`, and § 13 says the field is *never a `[T]`* | compile error (survivable) |
| 6 | that omitting `->` on an extern `function` means void; § 13 says *"`()` is `void`"* only about callbacks | compile error (survivable) |
| 7 | that lending counts as a **use** of `b`. § 5: *"a read is a use and a write is not, except through an `@` parameter."* A program that only lends and never reads — is `b` unused? | unknown; a false `unused binding` error, or a real one missed |
| 8 | that the lend must be written inline. § 13 is clear here — *"A lend and a lease name stand only as an argument of a call"* — and it is the one rule in this area I did **not** have to guess | — |

Row 8 is worth naming for the sitting: where § 13 states a restriction plainly, I
guessed nothing. The hole is not a hard subject, it is an unwritten sentence.

---

## argument (≤120 words)

§ 3's *"No aliasing exists among the values this language owns"* is the sentence a
reader uses to reason about every assignment in every program; § 5's *"`=` binds
once, forever"* is the second. § 13 grants C a write through both in fourteen
words, with no example, using § 5's own word for the immutable form. The call site
carries no mark, so the fact that a binding changed is written nowhere in the
program — it lives in a C header. **G writes that violation into the document
instead of closing it, and charges every ordinary line for one FFI form.** B costs
one character at two sites, reuses § 13's own *"A C out-parameter is an `@`
parameter"*, needs no grammar change, and makes A derivable. Neither reaches the
extent, which is where my measured mistakes were.

---

## prediction (one, falsifiable, scoreable at the next harness run)

Give N ≥ 10 fresh models `spec/heroes-spec.md` and this task, verbatim:

> Declare an extern group for a header with `struct Buf { unsigned char data[8]; }`
> and `long sum_n(void *p, long n);`. In `main`, build a `Buf`, lend its `data`
> field to `sum_n`, and print the result.

**Under the current text I predict:**

- **≥ 70%** bind the record with `=`, not `@` (driven by § 5's first line and
  § 13's word *"binding"*);
- **≥ 25%** write an extent other than `8` — `64` foremost, then a non-`u8` field's
  element/byte confusion if the task is repeated with `i32[4]`;
- **0%** produce any diagnostic from either mistake.

**Under B I predict:**

- the `=` rate is unchanged at the point of writing, but **≥ 80% of those programs
  now fail to compile** rather than running, because the `@` at the call site
  meets a non-`@` name;
- the **extent-error rate moves by less than 5 percentage points** — B does not
  touch it.

That last line is the sharp one. **If B is adopted and the extent-error rate
falls materially, I was wrong about which hole is which**, and this sitting should
be told that the extent needs no separate work. I do not expect that.

---

## condition — what would change each verdict

**Veto on G lifts if:** a measured run shows that, under G's narrowed § 3 and § 5,
readers correctly predict the write ≥ 90% of the time *when asked what the program
prints* — i.e. that reading the unmarked `extern` group in the same file is enough
to make the call line self-explaining. I doubt it, because under G the fact is not
in the Heroes program at all; the mark would be in the C header. But it is
measurable: show ten models the Task-1 program and ask what `print(b.bytes[0])`
writes.

**Approve on B weakens to object if:** a measured run shows `@` at the call site
of a `.ptr()` lend is produced correctly under B less than ~60% of the time (the
composed form `@b.bytes.ptr()` is unfamiliar and may itself be a guess site), **or**
if B forces `@` onto read-only lends and the read-only-lend error rate rises. Both
are checkable with a second task that lends a field to a C function that only
reads.

**Approve on A weakens to a shrug if:** under the current text readers already
write `b: Blob @ …` ≥ 90% of the time for a buffer C fills. Then A refuses
programs nobody writes, and B is the whole repair.

**And one thing that would change nothing:** a finding that the shipped compiler
already refuses some of this. The question here is what the *document* says, and
the document is what a fresh model is handed.
