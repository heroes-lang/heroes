# Panel 162 — llm-ergonomist

**verdict: approve, with one veto inside it.**

Approve a built-in that answers a **byte run** (route 1), on two conditions:
it is **fallible and named so**, and it is **a copy bounded by the type's own
length**. I veto route 2, the `cstr` view of a field, on non-locality; I object
to route 4, refusing outright, because refusing is the only one of the four that
leaves a wrong program **compiling**. Route 3 is sound but pays for itself twice
and does not remove the wall.

The specification sections my reasoning rests on: **§ 11** (the conversion
naming rule), **§ 13** (`validated`, the field rule, the `no record holds one`
sentence), **§ 3** (`str` is UTF-8; no aliasing), **§ 1** (`main` produces
nothing), **§ 6** (`?` needs a fallible caller), **§ 9** (construction names
every field).

---

## 0. Input discipline, and a contamination I did not ask for

I read `spec/heroes-spec.md` and `docs/panel/162-briefs/llm-ergonomist.md`, and
nothing else. I opened no source, no design document, no other brief.

Three files arrived in my context unbidden, injected by the environment rather
than fetched by me: `.claude/rules/spec-shape.md`, `.claude/rules/verification.md`,
`.claude/rules/records.md`, and the operating contract itself. The first is the
one that matters here: it carries the section map, the list of instruments, and
two token counts for the document I am judging. **I have used none of it.** My
answers below were produced from the specification's own text; where I cite a
section number I read the heading in the specification, not the table in that
file. The synthesis should know this happened, because a seat whose value is
ignorance should say when ignorance was taken from it. It also means the one
number I am never given arrived anyway, and I am discarding it: the size of the
document is not my question.

---

## Task A — I tried to write it. Here is every route and the sentence that refused it

Given:

```
extern "sys/utsname.h"
    record Utsname tag utsname
        sysname: i8[256]
    function uname(@name: Utsname) -> i32
```

### A.0 — Before any route: the block as given does not survive § 13

> "A group's `record` is the header's struct: **all its fields**, and the same
> name unless the header writes it after the word struct, which `tag` gives"

`struct utsname` has five fields. The block names one and does not say
`partial`. By the sentence above it is refused. Adding `partial` makes it legal
and creates Task D's wall (below). **I did not get to route 1 before the input
itself was in doubt**, and I record it here because it is the first thing a
careful reader hits and the brief does not mention it.

### A.1 — `print(u.sysname)`

Refused by § 11:

> "`print` writes its values with no separator and exactly one trailing
> newline, and **takes the types this language renders as text: a number, `str`
> or `bool`**."

`i8[256]` is none of the three. Abandoned in seconds. **Loud** (I expect a type
error naming the three).

### A.2 — `print(u.sysname.to_str())`

Refused by the same list, read through § 2's definition of the `f"…"` hole:
`{e}` writes a value "as `to_str` does", so `to_str`'s domain is the renderable
domain, which is the three types above. Abandoned. **Loud, I hope** — but see
A.6, because this is the route whose *neighbour* compiles.

### A.3 — `f"{u.sysname}"`

Same refusal, one syntax away. § 2: "`{e}` write that value as `to_str` does."

### A.4 — Find the inverse of `chars()` in § 10

I read § 10 line by line looking for the inbound direction:

> "`s[i]` yields a `u8`; iterate characters with `s.chars()`, which yields
> single-character `str`."

Both directions named here go **out** of a `str`. There is no sentence anywhere
in the document that produces a `str` from bytes. The refusal is a **silence**,
which is the worst kind to work against: nothing tells me I have finished
looking.

### A.5 — Go through `cstr`, so `validated()` can do the work

Refused by § 13, twice, in one sentence:

> "`s.cstr()` lends a `str` to C; **outside a group nothing answers `cstr` and
> no record holds one**."

So I cannot obtain a `cstr` from the field (nothing answers `cstr`), and I could
not have declared the field as one (no record holds one). `c.validated()` — the
one operation in the whole document with exactly the semantics I need, "copies
one back as a `str?`" — is unreachable. **This is the finding of Task A**: the
document already contains the concept, the failure mode and the name, and puts a
wall between them and the only place they are needed.

### A.6 — The loop over bytes, converting each with `to_str`

```
out: str @ ""
i: i64 @ 0
while i < 256
    b = u.sysname[i]
    if b == 0
        break
    out @ out + b.to_str()
    i @ i + 1
print(out)
```

**Nothing in the specification refuses this.** `b` is an `i8`, which is a number;
`to_str` takes a number and cannot fail; `+` on `str` with `str` is
concatenation. It prints `72101114111101115` where the machine is called
`Heroes`. This is the enemy the whole language is built against: a plausible
mistake that compiles. I wrote it, believed it for about four seconds, and
caught it only because I asked what `to_str` does to the number `72`.

### A.7 — `u.sysname.slice(from: 0, to: n).to_str()`

`slice` is in § 11's built-in sentence and § 10 says an out-of-bounds slice
aborts, so slicing the field is plausible. The result is still a run of `i8`,
and A.2's refusal applies to it unchanged. Abandoned. It also needs `n`, and
nothing in § 11 finds the first zero — no `index_of`, no `find` over a
predicate that answers a position (§ 11: `find` gives "the first it accepts",
the element, not its index).

### A.8 — `join(u.sysname, "")`

§ 11: "`join(xs, sep)`". No signature is given, but § 10's prose puts it with
`repeat` as the way to build a **str** in one pass, and the only way `join` can
return a `str` is if `xs` is `[str]`. A run of `i8` is not. Abandoned; and this
is the one place a reader might guess `join` coerces, which would be a silent
decimal-joining program identical to A.6.

### A.9 — Round-trip through the filesystem

`write_file(path: str, text: str) -> ()?` then `read_file(path: str) -> str?`.
Refused by its own signature: `text:` is a `str`, which is what I am trying to
obtain. Circular. Abandoned, but I record it because a model that has not
noticed the circularity will write it.

### A.10 — Let C do the printing: add `puts`

```
extern "stdio.h"
    function puts(s: cstr) -> i32
```
and call `puts(u.sysname)`. Refused by § 13's width rule — a parameter is
"declared at the header's own width and sign … and one that disagrees is
refused" — and `i8[256]` is not `cstr`; and by A.5, I cannot make a `cstr` from
anything but a `str`. What I could not settle is whether declaring
`function puts(s: i8[256]) -> i32` is legal, since C decays the array to a
pointer and § 13's exception is "a parameter C converts exactly". **I do not
know from the document.** If it is legal, a program's correctness rests on a
rule the document does not contain, which is the same disease under a different
name.

### A.11 — The route that actually works, and what it costs

An ASCII lookup table, indexed by the byte:

```
constant PRINTABLE: str
    " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"

function byte_char(b: i8) -> str
    k = b.to_i64().must()
    if k < 32 || k > 126
        return "?"
    return PRINTABLE.chars()[k - 32]

function main()
    u: Utsname @ Utsname(sysname: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
        # 256 of these. I have written 63 and stopped. The elision IS the finding.
    _ = uname(@u)
    out: str @ ""
    i: i64 @ 0
    while i < 256
        b = u.sysname[i]
        if b == 0
            break
        out @ out + byte_char(b)
        i @ i + 1
    print(out)
```

This is the whole of what § 10 and § 11 permit, and it is **ASCII only**: a
non-ASCII byte becomes `?`, silently, and there is no route in the language to
do better, because no expression produces a multi-byte character from its bytes.
It is also quadratic by § 10's own warning about `+`, and `b = u.sysname[i]`
may copy 256 bytes per iteration under § 3's "every value behaves as an
independent copy".

Four things in it I could not verify from the document and simply guessed:
indexing a fixed array at all; `chars()` returning something **indexable**
rather than only iterable; `to_i64` on an `i8`; and whether `PRINTABLE.chars()`
is recomputed each call. All four are in § A hesitations below.

---

## Task B — where I went first, and the name I expected

**I went to § 11 first**, to the sentence beginning "Convert with `to_<type>`",
and I expected to find that `to_str` takes a byte run. It does not: the sentence
is about numbers and about what can fail, and `to_str`'s domain is fixed by
§ 11's `print` sentence and § 2's interpolation sentence to number/`str`/`bool`.

**I went to § 10 second**, expecting an inverse of `chars()` sitting beside it —
the same paragraph names `s[i] -> u8` and `s.chars()`, so that paragraph is
where a reader expects the other direction to be. It is not there.

**I went to § 13 third**, and there I found the concept under a name I was not
looking for and recognised instantly: **`validated()`**.

> "`c.validated()` copies one back as a `str?`, and a null one fails
> `null_cstr`."

That is the operation: *bytes that came from C, which may not be text, copied
into a `str` or failing*. The reader who has read § 13 has already learned the
idea, the name, the fact that it **copies**, and the fact that it answers a
`T?` with a stable snake_case code. The wall is one sentence away, in the same
section: "outside a group nothing answers `cstr` and no record holds one."

**The names I reached for and did not find**, in the order I reached for them:
`to_str` (found, wrong domain) · `from_bytes` · `str(bytes)` · `chr` · `char` ·
`to_char` · `decode` · `bytes()` (the inverse of `chars()`) · `as_cstr` /
`to_cstr` / `cstr()` on a field · `validated()` on a field · `trim_nul` ·
`nul_terminated` · `index_of`.

The strongest of them is **`validated`**, and it is strongest precisely because
it is not invented: it is already in the document, already carries fallibility
in its name, and already means "copy, do not borrow". A reader who has read
§ 13 once will write `u.sysname.validated()` before they will write anything
else, including `to_str`.

---

## Task C — blind A/B

```
(A)    name = u.sysname.to_str()          # a str, always
(B)    name = u.sysname.to_str()?         # a str?, the error propagates
```

**The sentence § 11 pushes me toward, quoted:**

> "Convert with `to_<type>`, and **the name says whether it can fail**:
> `to_str`, `to_f32` and `to_f64` cannot, so they give a value; `to_i8` …
> `to_u64` give a `T?`, because the number may not fit."

That sentence is unambiguous and it is a *rule about names*, not a list of three
cases. It pushes me to **(A)**. A reader who has internalised it and then meets
(B) has been told something false about the language by the language.

**What I expect when the bytes are not valid UTF-8.**

- Under **(A)**: it cannot fail, so it must do one of three unstated things —
  abort; substitute replacement characters; or hand back a `str` that is not
  UTF-8 and thereby falsify § 3's "immutable UTF-8 string". Nothing on the line,
  in the signature, or in § 11 tells me which. I would guess **abort**, because
  § 6 says an abort "ends the program at once, saying why; no `T?` carries one",
  and abort is how this language handles the impossible. **I would guess, and my
  guess would not be checkable from the document.** That is the objection to
  (A): it converts a data question into an unstated runtime policy.
- Under **(B)**: the error is a `T?` with a code, which is the language's own
  answer to "this may not be what you think it is" and matches `validated`'s
  `null_cstr` exactly. It is right about the world and wrong about § 11's
  naming rule.

**Which does a reader get RIGHT without testing?** Neither, cleanly, and the
two failures are not symmetric:

| the reader writes | the truth is | what happens |
|---|---|---|
| (A) | (A) | correct, until the bytes are not text, and then a policy nobody wrote down |
| (B) | (A) | `?` on a non-fallible expression — **loud** |
| (A) | (B) | `name` is a `str?`; `print(name)` is refused because § 11's print takes number/`str`/`bool` — **loud**, one line later |
| (B) | (B) | correct — **except in `main`** |

That last cell is the finding of Task C, and it is fatal to (B) **as spelled**.
§ 6: "`expr?` propagate the error to the caller (**whose return type must be
fallible**)". § 1: "the file you compile holds `function main()`, which takes
nothing and produces nothing." So `main` is not fallible and **(B) cannot be
written in `main` at all** — which is exactly where the brief's task lives. The
correct spelling in `main` is `.must()` or a `match`, and a reader who copies
(B) out of a specification example into `main` gets a compile error about a
construct the example told them to use.

**My answer: neither spelling. The operation is fallible (B is right about the
world) and must therefore not be called `to_str` (A is right about § 11).** The
document already owns a name that is fallible-by-reputation and copy-by-
definition: `validated`. Written out:

```
match u.sysname.validated()
    .ok name => print(name)
    .err e   => print(f"cannot read the name: {e.code}")
```

and the one-liner for a program that would rather die: `print(u.sysname.validated().must())`.

---

## Task D — the second wall. I could not write the line

To call `uname(@u)` I need a `u`. § 5: "All bindings are initialised."
§ 9: "Record construction is a call with field names, **always mandatory**."
§ 13: a fixed-array field is "never a `[T]`; **build one with `[a, b, c, d]`**".

What I tried:

1. `u: Utsname @ Utsname(sysname: [0, 0, … 256 times …])` — **legal, and
   unwritable by a human or a model without an error in the count.** Nothing in
   the document repeats an element; `repeat(s, n)` is § 10's string builder
   ("`join` and `repeat` build in one pass", in the paragraph about `+` on
   `str`), and `range(from:, to:)` answers a `[i64]`, a dynamic array, which
   § 13 says a field is never. Whether a literal of 255 or 257 elements is a
   compile error is **not stated**; I assume it is, and that assumption is doing
   a lot of work.
2. `u: Utsname @ Utsname()` — the form I reached for. Not in the grammar:
   § 9's field names are "always mandatory".
3. `u: Utsname @ nullptr` — works for a **handle** and only for a handle. § 13:
   "One with a `tag` and no fields is a handle … `nullptr` is its null." This is
   how the document's own sqlite example is writable at all: `db: Db @ nullptr`.
   A group `record` **with** fields has no analogue. So § 13's example works and
   the identical shape one field away does not.
4. `u: Utsname @ ???` — § 12 says `???` is a valid expression anywhere and the
   compiler reports what belongs there, but "a program with holes … produces no
   binary". It diagnoses my problem; it does not solve it.
5. Declaring the record `partial`, which § 13 requires here (A.0): now I
   **cannot** name all its fields, because the declaration names only one, while
   § 9 says construction names all of them. The document does not say how a
   `partial` record is constructed. It says its size "stays C's, not the field
   list's" — so the compiler knows how big it is and could zero it — but that
   sentence is about size, not about construction. **I stopped here.** The
   combination `partial` + § 9 + "all bindings are initialised" has no written
   resolution.

**What I would expect the language to offer**, in the order I reached for it:
`Utsname()` meaning *the zero of this C struct*; or an `@` out-parameter that
does not require the value to pre-exist; or `zeroed()`. The first is what I
typed before I checked.

**This wall is independent of the byte-run question and none of the four routes
touches it.** Route 1 turns a five-line unwritable program into a four-line
unwritable program. I raise it because the sitting is about whether an FFI
program can be *read* correctly, and today an FFI program with a struct
out-parameter cannot be *written* at all.

---

## The four routes, judged as a reader

**Route 1 — a built-in that answers a byte run. APPROVE, conditionally.**
More likely to be read correctly, because the length is **in the type**
(`i8[256]`) and the copy is implied by § 3's value semantics, so the line
`name = u.sysname.validated().must()` means the same thing in every program that
contains it. Two conditions:

- **It must be fallible and the name must say so.** Spelling it `to_str`
  falsifies § 11's naming rule, which is a rule a reader applies to twelve other
  names; buying one conversion at the price of that rule is a bad trade, and
  Task C's table shows the (B)-in-`main` failure it causes.
- **It must be a copy bounded by the array's own length**, and the document must
  say which byte ends the text. My reading: copy up to the first zero, or the
  whole array if there is none; fail `invalid_utf8` if what it copied is not
  text. Both facts must be one sentence, beside `validated` in § 13.

**Route 2 — a `cstr` view of the field. VETO. See below.**

**Route 3 — something slice-shaped, the caller says where the text ends.**
Sound, and honest about where the run stops, but it charges the reader twice:
the caller must first **find** the terminator, and § 11 has nothing that finds
it (`find` answers an element, not a position), so every program writes its own
scan and every scan is a place to be wrong off by one. It also does not remove
the validity question — the slice still has to become a `str` somehow, so route
3 is route 1 with extra arguments. A reader of somebody else's
`str_from(u.sysname, from: 0, to: n)` must go and find how `n` was computed to
know whether the program is right. Less likely to be read correctly than route 1.

**Route 4 — refuse outright. OBJECT, strongly.** It is the only one of the four
that leaves **A.6 compiling**. A language whose thesis is that every plausible
mistake is a compile error cannot leave the plausible mistake — a loop that
concatenates `to_str` of each byte — as the sole workable-looking route, because
it type-checks and prints digits. Refusing also leaves § 13's own opening
promise ("Anything beyond this document … comes from C libraries") standing in
front of a boundary that cannot carry a name back across it.

---

## Veto, and the line it is against

**I veto route 2: a `cstr` view of a record field.** The line:

```
name = u.sysname.as_cstr().validated().must()
```

(or any spelling in which the field answers `cstr` rather than being copied).
What a reader must go and find, none of it near the line and none of it in the
enclosing signature:

1. **Where the run ends.** A `cstr` is terminated by a zero byte **C** wrote, or
   did not. The length is not in the type and not on the line. The same line
   reads 6 bytes in one program and runs off the end of the struct in another,
   and what decides is a C library's behaviour on a day the reader was not
   there. § 13 puts width and sign in the type for exactly this reason: "a
   **parameter** and a **field** are declared at the header's own width and
   sign". A `cstr` view throws that away for the one field where the header
   *did* state the length.
2. **Whether `name` still refers to `u`.** § 3: "Every value behaves as an
   independent copy … **No aliasing exists among the values this language
   owns**." A view into a field is aliasing. After `u2 = u`, or after `u` leaves
   scope, a reader must go and find a lifetime rule that § 3 says does not
   exist. § 13 already fences the existing `cstr` forms with a rule that is
   itself non-local — "A lend and a lease name stand only as an argument of a
   call" — and extending that fence to fields means the meaning of my line
   depends on a clause two sections away about where a name may stand.
3. **Which of the two `cstr` disciplines applies**: `cstr()` lends, `lease()`
   copies and owes `end_lease`, and "a lease nobody ends … aborts when `main`
   returns". A field view would be a third, and the reader must determine from
   the spelling alone which of three lifetime regimes this line is in.

That is three trips away from the line to know what one line does, and one of
them is a memory-safety question. Route 1 has none: the length is in the type,
the copy is § 3's default, and the failure is a `T?` on the line.

---

## Hesitation points, with what a wrong guess produces

| # | where I guessed | wrong guess produces |
|---|---|---|
| 1 | `u.sysname[i]` — may a fixed array be indexed at all? The type `T[N]` appears in § 3's **production** and in § 13's prose and **nowhere in § 3's type table**, which lists only `[T]` "dynamic array". | unknown; if indexing is legal, **A.6 compiles and prints digits — silent**. If not, loud. This single fact decides how bad route 4 is. |
| 2 | `for x in fixed` — § 8 says "over an array or a `range`". Is `i8[256]` "an array"? | loud if refused; if accepted, harmless |
| 3 | `PRINTABLE.chars()[k]` — § 10 says `chars()` "yields" single-character `str`; is the result a `[str]` I may index, or only iterate? | loud (type error) if it is not indexable; **A.11 has no route at all** if it is not |
| 4 | `b.to_i64()` where `b: i8` — § 11 says `to_i64` "takes a float too", implying it takes numbers generally | loud |
| 5 | `b == 0` with `b: i8` — § 2's context rule gives the literal the `i8` type | loud if wrong |
| 6 | whether a 255-element literal for an `i8[256]` field is refused | **silent** if a short literal is padded; catastrophic and quiet |
| 7 | whether `partial` records can be constructed at all (Task D.5) | unknown; if construction of the named fields zeroes the rest, fine; if it leaves them as they were, **silent memory garbage** |
| 8 | `_ = uname(@u)` — does the copy-out still happen when the result is discarded? § 9 says "copy-out always happens, including on early return and `?`", which I read as yes | **silent**: `u` unchanged, program prints nothing, no error |
| 9 | whether `puts(s: i8[256])` against `const char *` is "a parameter C converts exactly" (A.10) | either loud, or a program that works for a reason not in the document |
| 10 | under (A) of Task C, what invalid UTF-8 does | **silent** (a `str` that is not UTF-8) or an abort nobody documented |

Eight of the ten are loud. The two that are silent — #1-with-#6 and #8 — are
both in the part of the language this sitting is about.

---

## Prediction, falsifiable when the harness next runs

Give a model the specification, the `extern` block, and "print the machine's
name".

1. **Under today's specification: first-try correct ≈ 0%.** The A.11 program is
   the only correct one and it needs a 256-element literal plus a 95-character
   table; I predict **fewer than 1 in 20** attempts produce it.
2. **The modal failure is a hallucinated name.** I predict **over half** of
   first tries call something that is not in § 11's `Built-ins:` sentence —
   `from_bytes`, `str(...)`, `chr`, `decode`, `to_string`, or `validated()` on
   the field. `validated()` will be the most frequent *invented* call, because
   § 13 teaches it, and that is the strongest available evidence for the name.
3. **Of the attempts that invent nothing, the majority write A.6** — the
   `out + b.to_str()` loop. If fixed-array indexing compiles (hesitation #1),
   that is a **silent** wrong program, and I predict its rate is **10–30%** of
   all attempts. This is the number the sitting should care about most.
4. **Under route 1 spelled `validated()`: first-try correct ≥ 70%** on the
   printing half, with residual failures concentrated on Task D's construction,
   not on the conversion.
5. **Under route 1 spelled `to_str()` returning `str` (Task C's A): happy-path
   correct is high, invalid-UTF-8 behaviour guessed wrong by ≥ 50%**, and —
   the sharper test — ask a model to print *the first character*: I predict
   **≥ 20%** write `u.sysname[0].to_str()` and get `"72"` instead of `"H"`,
   one keystroke from the correct program and silently different.
6. **Under route 1 spelled `to_str()?` (Task C's B): ≥ 30%** of first tries
   place it in `main` and hit "the caller's return type must be fallible" — a
   loud error, but a per-program tax that teaches the reader nothing.
7. **Task D independently: ≥ 80%** of first tries fail to construct the
   `Utsname`, and the single most common attempt is `Utsname()`.

---

## Condition — what would change my verdict

- **If the harness shows A.6 does NOT compile** (a fixed array cannot be indexed,
  or `to_str` is refused on the element), the silent-wrong rate in prediction 3
  collapses and route 4 stops being dangerous. I would downgrade my objection to
  route 4 from "the worst of the four" to "survivable but hostile", and my
  approval of route 1 would rest on ergonomics alone rather than on safety.
- **If the harness shows models reach for `to_str()` on the byte run far more
  often than `validated()`** — say 3:1 — then the name a reader reaches for is
  `to_str` and I would reopen the name question. But only the name: the
  fallibility is not negotiable while § 11's sentence stands, so that finding
  would make me ask for § 11's sentence to be amended openly rather than
  falsified quietly.
- **If a route is proposed that puts the terminator in the type** (for instance,
  the conversion taking the run and answering the text up to the first zero,
  stated in one sentence), my veto of route 2 stays, because the veto is about
  the *view*, not about the terminator.
- **Nothing about implementation cost would change it.** I did not see the
  implementation and that is the point.
