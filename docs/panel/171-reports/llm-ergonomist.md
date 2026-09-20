# Panel 171 — llm-ergonomist report

**Input:** `docs/panel/171-briefs/llm-ergonomist.md` and `spec/heroes-spec.md`,
nothing else, by choice. Three files under `.claude/rules/` were pushed into my
context by the harness's system reminder without my asking (`verification.md`,
`records.md`, `spec-shape.md`); I did not use them for anything below. One of
them states the document's measured token count. I judge the prose I was given,
not its size, and that number played no part in any sentence here.

**Repository context I wanted and did not take:** whether the compiler today
accepts `@out: cstr borrows` on a C parameter (the grammar admits it; I did not
check the checker). It bears on W4 and I argue from the grammar alone.

## verdict

**approve — W2 `lent`.** Per word:

| word | verdict | ground in one line |
|---|---|---|
| W1 `reads` | **object** | says *writes nothing*, not *keeps nothing*; true of the function that must stay unmarked, false of the buffer `read(2)` fills |
| W2 `lent` | **approve** | names the operation it admits; § 13 already says *lends* three times; the default sentence needs no clause defining it |
| W3 `transient` | approve, second | teaches the fact, but is nobody's word in this document and wants a defining clause |
| W4 `borrows` | **object** | already in `CParam` for `@` out-parameters meaning *the call keeps*; the same word on the same production, flipped by `@` |

No veto. I looked for the non-local ground on W4 and it is not there: the
meaning of `s: cstr borrows` is decided by the presence of `@` on the same
line, so it is local. It is a trap, not a non-local construct.

## Task 1 — cold reading

What I believed on first sight of each word on a C parameter, before reading
the proposed definition, then whether the belief survived it.

- **W1 `reads`** — *C only reads through this pointer; it does not write.* A
  claim about mutation, the thing `@` already governs. The definition is about
  retention. **Belief dies.** Worse: it is TRUE of `keep_label`, the brief's own
  (b), described as *keeps the pointer and reads it later*.
- **W2 `lent`** — *this argument is lent to C for the call and comes back; C
  does not keep it.* The definition says the same. **Survives.** The document
  already says `s.cstr()` *lends*, `f.ptr()` *lends*, *a lend lives for its
  call*: the word on the parameter is the participle of the verb the operation
  already has, in the direction the document already uses (program → C is
  *lend*, C → program is *borrow*: *"passing one the function borrowed is an
  error"*).
- **W3 `transient`** — *the pointer here is short-lived; C must not hold on to
  it.* **Survives**, with one wobble: the word describes the pointer, not the
  call, so a reader is left to work out that it is the binding's claim about C
  and not a property the compiler enforces on C.
- **W4 `borrows`** — two readers, two beliefs. A reader who learned this
  document's `borrows` first (*"the call hands back one it keeps"*) believes a
  `borrows` parameter is one **the call keeps**, a non-owning reference held
  across calls: the OPPOSITE of the proposal. A Rust-shaped reader believes
  *temporary, not kept*: the proposal's meaning. **Does not survive**, because
  the document itself manufactures the first reader.

## Task 2 — the two bindings, plus the one the brief did not ask for

I wrote `strlen` (does not keep), `keep_label` (keeps, reads later), and
`read(2)`'s buffer (does not keep, WRITES through the lend), because a repair is
attacked at the shape beside it and `read` is the shape where W1 breaks.

### Under the current § 13

```
extern "string.h"
    function strlen(s: cstr) -> u64

extern "label.h" link "label"
    function keep_label(s: cstr)
    function label_text() -> cstr

function main()
    name = "hello"
    print(strlen(name.cstr()))
    keep_label(name.cstr())            # compiles today; C keeps a pointer into bytes it does not own
    print(label_text().validated().must())
```

The third line of `main` is the silent error the ruling closes. Nothing in the
current document refuses it; the current sentence says so in its own words,
*and nothing checks it*.

### Under W2 `lent` (what I would land)

```
extern "string.h"
    function strlen(s: cstr lent) -> u64

extern "unistd.h"
    function read(fd: i32, buf: ptr counted_by n lent, n: u64) -> i64

extern "label.h" link "label"
    function keep_label(s: cstr)
    function label_text() -> cstr

record Line
    bytes: u8[16]

function main()
    name = "hello"
    print(strlen(name.cstr()))
    l: Line @ Line(bytes: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
    got = read(fd: 0, buf: l.bytes.ptr(), n: 16)
    print(got)
    lbl: cstr @ name.lease()
    keep_label(lbl)
    print(label_text().validated().must())
    end_lease(@lbl)
```

`keep_label(name.cstr())` is now refused at compile time: a lend at an unmarked
parameter. Loud. Under W3 and W4 the code is the same with the word swapped;
under W1 it is the same text, and the text lies on `read`.

### Counts

**With the proposed sentence in view** (first try, sentence just read):

| word | strlen marked | keep_label unmarked + lease | read marked | re-reads needed |
|---|---|---|---|---|
| W1 `reads` | yes | yes | yes | 2 (at `keep_label`: the word said mark it; at `read`: the word said do not) |
| W2 `lent` | yes | yes | yes | 0 |
| W3 `transient` | yes | yes | yes | 0 |
| W4 `borrows` | yes | yes | yes | 2 (at `strlen` and `keep_label`: *does this mean what it means on a result?*) |

All four score 3/3 with the sentence open. The sentence is not what a model
has open at binding thirty; the word is. So:

**Cold, word alone** (the retention case, the sentence not in view):

| word | strlen | keep_label | read | silent | loud |
|---|---|---|---|---|---|
| W1 `reads` | marked, right | **marked, wrong** (it reads) | **unmarked, wrong** (it writes) | 1 | 1 |
| W2 `lent` | marked | unmarked, lease | marked | 0 | 0 |
| W3 `transient` | marked | unmarked, lease | marked | 0 | 0 |
| W4 `borrows`, document's reader | **unmarked, wrong** (hands back nothing) | **marked, wrong** (the call keeps) | **unmarked, wrong** | 1 | 2 |
| W4 `borrows`, Rust reader | marked | unmarked, lease | marked | 0 | 0 |

The silent cell is the one that matters. In both W1 and W4 it is `keep_label`
**marked**: the mark widens permission, so the lend is admitted, the program
compiles, and C keeps a pointer into bytes it does not own. That is the exact
program the ruling exists to refuse, produced by the word the ruling chose.

## Task 3 — the sentence, which decides the verdict

Today (27 words):

> A lend lives for its call and no longer: C keeping the pointer reads bytes
> the program may have changed or freed since, and nothing checks it.

The default clause every candidate shares (23 words):

> an unmarked `cstr` or `ptr` parameter is assumed to keep what it is handed,
> and takes a lease or a pointer C owns.

**W2 `lent` — 39 words, the one I would land:**

> A lend lives for its call and no longer, and reaches only a parameter marked
> `lent`: an unmarked `cstr` or `ptr` parameter is assumed to keep what it is
> handed, and takes a lease or a pointer C owns.

The word needs no defining clause. *A lend reaches a `lent` parameter* is
nearly a tautology, and the contrast with *assumed to keep* supplies the rest.
*Only* carries the refusal. The lease is named on the UNMARKED side, which is
also what keeps a reader from confusing the two L-words: a lease goes where a
lend may not. Price: +12 words on the sentence, +1 alternative in the `CParam`
production. A conservative variant adds *, the binding's word that the call
keeps nothing past its return* (+9, 48 words); I would not spend it, because
every other marker in § 13 (`consumes`, `acquires`, `owned`) is already a claim
the binding makes about C and the reader has the pattern.

**W3 `transient` — 39 or 46 words:**

> … reaches only a parameter marked `transient`, which says the call does not
> keep it: …

The 39-word form works but I would spend the 7-word clause, because
*transient* describes the pointer and the reader must infer whose behaviour is
being described. Not harder to write; longer.

**W1 `reads` — 53 words, and it argues with its own word:**

> … reaches only a parameter marked `reads`, which says the call keeps nothing
> past its return, whether or not it writes through it: …

The clause *whether or not it writes through it* exists only to deny what the
word says. **Yes, the word makes the default sentence harder to write**: the
sentence has to un-teach the word before it can teach the fact, and
*"C writes back through the lend only where the binding is a `@` name"* two
sentences earlier already promises the reader that C writes through lends.

**W4 `borrows` — about 62 words, and it costs a second sentence:**

> … reaches only a parameter that is not `@` and is marked `borrows`, which
> there says the call keeps nothing past its return (after a result or an `@`
> out-parameter it says the call hands back one it keeps): …

**Yes, hardest to write**, and not for this sentence alone: *"`borrows` says
the call hands back one it keeps"* three sentences later becomes false as
stated and must be qualified too. Two sentences move, and the reader holds one
word with two subjects, the program borrowing in one and the call borrowing in
the other, told apart by an `@`.

**Finding the brief did not state:** the brief says § 13 uses `borrows` *on a
RESULT*. The grammar puts it in `CParam` as well —
`[ "consumes" | "acquires" ident | "borrows" ]` — for the `@` out-parameter
that hands back a handle the library keeps. So W4's second meaning is not on
another production; it is on the same one, and the `@` is the only thing
choosing between *the call keeps* and *the call does not keep*.

## Task 4 — a fifth word

I tried six and none beats `lent`:

- `forgets` — semantically exact (retention is the question, *forgets* is the
  negation of *keeps*) and a third-person verb like `consumes`/`acquires`, but
  whimsical against this document's register, and it names what C does rather
  than what the program did.
- `lend` — the verb form; a model will write it by mistake for `lent` either
  way, and the grammar refuses the wrong one loudly, which is fine. As the
  marker it reads as an imperative, and every other marker here is a statement.
- `uses` — vaguer than `reads` and shares its defect: true of `keep_label`.
- `temporary` — `transient` in plainer clothes; still describes the pointer.
- `during`, `unkept` — not words a reader has met on a parameter anywhere.

What decides it: § 13's markers are two kinds, verbs with *the call* as subject
(`consumes`, `acquires`) and a participle with the program's relation to the
pointer (`owned`: the program owns the result and must free it). `lent` is the
second kind — the program lent the argument, so C may not keep it — and it is
the participle of the verb the document already uses for the operation. There
is no fifth word to find because the document has already spent the first.

## What I had to invent

Each is a question the sentence leaves to the compiler; I list them because a
guess that compiles is the enemy.

1. **Whether a `lent` parameter also takes a lease or a C-owned pointer.** I
   assumed yes: the mark widens what the parameter accepts, it does not narrow
   it. If wrong, `strlen(lbl)` on a lease is a compile error (loud).
2. **Whether `lent` on a `ptr` without `counted_by` is refused.** Nothing can be
   lent to such a parameter, so the mark is dead text. I would want it refused
   (loud); if accepted it is a silent no-op.
3. **Whether `lent` on a handle (`Db`), a number, or an `@` parameter is
   refused.** Same shape; I would want it refused.
4. **Whether `f.ptr()` now needs `lent` beside `counted_by n`.** I read *reaches
   only* as covering both lends, so yes. The `f.ptr()` sentence itself needs no
   change.
5. **What the refused lend's diagnostic says.** For the program to be fixable
   without another file, it must name both routes: mark the parameter `lent`
   if the function keeps nothing, else take a lease. For a FIELD lend there is
   no lease route, so the diagnostic must not offer one there.
6. **Where `end_lease(@lbl)` goes** for `keep_label`: after C's last read, which
   the document says the program decides. I put it last in `main`.

## hesitation_points

- **`lent` versus `lease` (W2 only).** A model passing a lease might add `lent`
  *to be safe*, marking a keeping function and admitting a later lend there:
  silent. The landed sentence names the lease on the unmarked side, which is
  the counter. I estimate it at 1 in 10 or fewer and name it as the one
  word-specific silent path in W2.
- **Over-marking, any word.** The mark widens permission, so the *to be safe*
  reflex is always silent. Word-independent; the ruling accepted it by putting
  the mark on the non-keeper. Worth a diagnostic nowhere, worth a sentence in
  the site's binding guide.
- **W1 at `keep_label`**: marked because it reads. Silent.
- **W1 at `read(2)`'s buffer**: unmarked because it writes; refused, loud; and
  the only repair available is to write a word that lies to the next reader,
  since a field has no lease route.
- **W4 at `keep_label`**: marked because *the call keeps*, the document's own
  definition. Silent.
- **W4 at a stray `borrows` on a non-`@` `Db`**: the compiler cannot say which
  of the two meanings was intended, so the diagnostic cannot say what to fix.
- **Not this sitting's, noted:** a 16-element literal to zero a `u8[16]` field
  is the only way the document offers to build one; the document does not say
  whether an ordinary `record` may hold `u8[16]` (the `Type` production allows
  it; § 13 states it only for a group's record).

## argument

§ 13 already owns the verb: `s.cstr()` and `f.ptr()` *lend*, and *a lend lives
for its call*. `lent` on the parameter names the operation it admits, so the
default sentence needs no clause defining the word and lands at 39 words, 12
over today's 27. `transient` teaches the same fact but is nobody's word here
and wants a defining clause. `reads` is a claim about writing, not keeping; it
is true of the one function that must stay unmarked and false of `read(2)`'s
buffer, and the sentence has to argue with it. `borrows` is already in
`CParam` for `@` out-parameters meaning *the call keeps*; a second meaning
told apart by `@` on one production inverts the word's subject. All four are
local; two are traps.

## prediction

Falsifiable when the harness next gives a fresh model this document with the
landed sentence and asks for the three bindings above, ten trials per word, no
further hint:

- **`lent`**: at least 9 of 10 mark `strlen` and `read`, leave `keep_label`
  unmarked, and take a lease for it.
- **`transient`**: at least 8 of 10.
- **`reads`**: at least 3 of 10 mark `keep_label`, or leave `read`'s buffer
  unmarked, or both.
- **`borrows`**: at least 3 of 10 mark `keep_label`.

And a retention probe, the grammar line alone with no prose, *what does this
word on a parameter mean*: `lent` at least 8 of 10 answer *C does not keep
it*; `transient` at least 7; `reads` at most 4; `borrows` at most 5, with at
least 2 answering *the call keeps it*.

## condition

I move `lent` to second if any trial marks `keep_label` `lent` on the stated
ground *the program lends it* (that is the direction I claim the word cannot be
read in), or if `transient` beats `lent` on the `keep_label` cell by 2 of 10 or
more. I withdraw the objection to `reads` if fewer than 1 in 10 mark
`keep_label` with it. I withdraw the objection to `borrows` only if the
existing `@out: T borrows` is removed from `CParam`, because with it there the
two meanings share one production and no count changes that.
