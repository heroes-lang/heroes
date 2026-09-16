# Panel 159 — llm-ergonomist

**verdict: approve** (sentences 1 and 2 as written; sentence 3's placement
approved, its wording gated on one compile — see `condition`). **No veto.**

## Input discipline, disclosed

I read exactly two files: my brief and `spec/heroes-spec.md`. I opened nothing
else.

Four other documents were **injected into my context by the harness**, without
my asking: `CLAUDE.md`, `.claude/rules/verification.md`,
`.claude/rules/records.md` and `.claude/rules/spec-shape.md`. I did not read
them as inputs and I refuse them for this verdict. This matters most for
`spec-shape.md`, which describes where a rule should live and which instruments
read which sentence — exactly the knowledge my seat exists to lack. My homing
judgements below ("merge into § 7", "merge into § 10") were formed from the
document's own arrangement and from my brief's line *the document's rule is one
rule, one home*. Where I would otherwise have had to guess which sentence an
instrument parses, I say so rather than assume.

I was not told what the compiler does. Every statement below about compiler
behaviour is marked as an inference.

---

## Task 1 — sort records by a key, with a tie-break

### What I wrote, from the document alone

```
record Person
    name: str
    score: i64

function before(a: Person, b: Person) -> bool
    if a.score != b.score
        return a.score > b.score
    return false

function ranked(people: [Person]) -> [Person]
    out: [Person] @ []
    for p in people
        out @ out.push(p)
        i: i64 @ out.len() - 1
        while i > 0 && before(a: out[i], b: out[i - 1])
            t = out[i]
            out[i] @ out[i - 1]
            out[i - 1] @ t
            i @ i - 1
    return out

function main()
    people = [
        Person(name: "ada", score: 10)
        Person(name: "bo", score: 12)
        Person(name: "cy", score: 10)
    ]
    for p in ranked(people)
        print(f"{p.name} {p.score}")
```

Intended output: `bo 12`, `ada 10`, `cy 10` — highest score first, and between
the two tens the one that came first in the input.

### Does it print what I intended, and how do I know?

It does, **and I know it from my own code, not from the document.** `before`
returns `false` on equal scores, so the `while` stops the moment it meets an
equal element and nothing with an equal key is ever moved past another. The
stability that the task requires is a property of the loop I wrote.

That is the whole finding for task 1. The document could not have told me,
because there is no route through the built-in: `sort` is documented as taking
"a number, `str` or `bool`, never a type parameter", so `sort(people)` is a
compile error (inference: I am reading the parenthetical as a type restriction
the checker enforces). Every ordering of records in this language is therefore
hand-written, and every tie-break is the programmer's own.

### The silently wrong program the document invites

Having been refused by `sort`, the obvious recovery is to sort the keys and
look the records back up. This compiles and is wrong twice:

```
function score_of(p: Person) -> i64
    return p.score

function ranked_wrong(people: [Person]) -> [Person]
    out: [Person] @ []
    for s in sort(people.map(score_of))
        for p in people
            if p.score == s
                out @ out.push(p)
                break
    return out
```

Output: `ada 10`, `ada 10`, `bo 12`. The tied pair became the same person
twice, and the direction is ascending because nothing told me `sort` was not
descending. Two silent defects, one compile, zero diagnostics.

### What the document does not decide about `sort`

Three things, and they are not equally serious.

- **Direction.** Never stated. The nearest is § 10, "`for k in sort(keys(m))`
  walks them in order". *In order* names no order.
- **The order on `str`.** This one is a genuine hole rather than an omission,
  and it is quotable. § 7 says of comparison: "`< <= > >=`: a number only". So
  `"a" < "b"` does not exist in this language. But `sort` accepts `str`, and
  § 10 walks map keys of type `str` "in order". **The document asserts an order
  on `str` that it defines nowhere and denies to the operator that would have
  defined it.** Byte order is the overwhelmingly likely answer for a language
  whose `str` is "indexed and measured in bytes", but likely is not stated, and
  a program whose golden output is a sorted key list is exactly the program that
  cannot tolerate a guess.
- **The order on `bool`.** Never stated, and unlike the other two a model has no
  convention to fall back on. Rare in practice.

### And one silence that is a **ruling**, not an omission: stability

Do **not** state whether `sort` is stable. `sort`'s domain is number, `str`,
`bool`. Two elements that compare equal in that domain are structurally
identical, so no program can observe which of them came out first: stability is
semantically invisible, and the restriction "never a type parameter" is what
makes it invisible. A sentence about it would be tokens spent on an
unobservable.

The exception, and I name it so the ruling is not mistaken for luck: `-0.0` and
`0.0`. § 7 says `==` is structural "on any two values of one type", then
immediately carves out IEEE behaviour for `nan` ("equals nothing, itself
included"), so float equality is IEEE and `-0.0 == 0.0` holds while
`print(-0.0)` and `print(0.0)` differ (§ 11: a float "reads back as the same
value"). `sort([0.0, -0.0])` is therefore the one program whose output can
distinguish a stable `sort` from an unstable one. If the panel wants that hole
closed it is a clause about floats, not a stability rule, and I would not spend
the tokens.

---

## Task 2 — swap two elements of an array in place

### What I wrote

```
function swap(@xs: [i64], i: i64, j: i64)
    t = xs[i]
    xs[i] @ xs[j]
    xs[j] @ t

function main()
    v: [i64] @ [3, 1, 2]
    swap(@v, i: 0, j: 2)
    print(f"{v[0]} {v[1]} {v[2]}")
```

`i` and `j` share a type, so § 9 makes their names mandatory at the call site;
`xs` is `@` so it is passed `@v`; and UFCS does not apply because the first
parameter is `@`, so `v.swap(...)` is not available — all three stated.

The three-line body rests on two things the document does settle: `t = xs[i]` is
an independent copy (§ 3, "after `b = a`, mutating `b` never changes `a`"), and
an `@` parameter is copy-in copy-out (§ 9), so the caller sees the writes.

### What it rests on that the document does **not** settle

`xs[i] @ xs[j]` where `xs` is an array. The production says it parses —
`Place = ident { "." ident | "[" Expression "]" }` — and § 5 permits mutating a
declared `@` name. But **no prose sentence and no example anywhere in the
document assigns into an array element.** The only indexed assignment shown is
`m["a"] @ 1`, a map, in § 10, and maps are the one container whose read
(`m[k]`) has a different type from an array's. So the reader with the map
example in hand has evidence about maps and none about arrays.

### What I built around the problem, and why

Before settling, I wrote the version that needs no element assignment at all:

```
function swapped(xs: [i64], i: i64, j: i64) -> [i64]
    out: [i64] @ []
    k: i64 @ 0
    while k < xs.len()
        if k == i
            out @ out.push(xs[j])
        else if k == j
            out @ out.push(xs[i])
        else
            out @ out.push(xs[k])
        k @ k + 1
    return out
```

This is what "building something around the problem" looks like: a swap that is
O(n) in time and allocation, correct under every reading, needing no `@`
parameter and no `Place`. It is the safe choice for a model that is unsure, and
it is the choice that turns a hand-written sort — which task 1 proved is the
only kind of record sort there is — from O(n²) comparisons into O(n²)
**array copies**.

That is the sharpest thing I found: **holes 1 and 2 compound.** Because `sort`
refuses records, every record ordering is hand-written; because the document
never shows an array element being assigned, the hand-written one is written
around the missing construct. Neither hole is dangerous alone.

### Cost of guessing wrong

Guess "arrays are not assignable" and you write the rebuild version: it
compiles and is correct. Guess "they are" and you are either right or you get a
compile error. **Hole 2 cannot produce a silently wrong program.** It produces
slow programs and wasted tokens.

---

## Task 3 — read a file and report failure

### What I wrote

```
function main()
    match read_file(path: "input.txt")
        .ok text => print(text)
        .err e   =>
            print(f"input.txt: {e.code}: {e.msg}")
            exit(code: 1)
```

### Is `?` available to me in `main`? And the sentence that decides it

**No single sentence decides it.** That is the answer, and I spent longer on it
than on anything else in the four tasks. Three sentences bear on it and none
closes it:

- § 6, the table row: "`expr?` | propagate the error to the caller (whose
  return type must be fallible)".
- § 3, the type table: "`()` | nothing: what a function with no `->` returns".
- § 1, first bullet: "One file is one module; the file you compile holds
  `function main()`."

Chaining them gives: *if* `main`'s signature is exactly `function main()`, its
return type is `()`, `()` is not fallible, so `?` in `main` is a compile error.
Every link is stated except the first. § 1's bullet is about **where `main`
lives**, not about what it may be declared as, and it reads equally well as
naming the function the compiler looks for. Nothing in the document forbids

```
function main() -> ()?
    text = read_file(path: "input.txt")?
    print(text)
    return ok()
```

which is four lines against five, reports the error for free if the runtime
reports it, and is the shape a model reaches for first.

I broke the tie by conservatism — I took `function main()` literally and wrote
the `match` — and conservatism is not a language feature. I also note that my
`match` version leans on `exit(code: 1)` standing as the last statement of a
block arm, which the document does not type (`exit` "ends the program", so
whether it is a `()` value is unstated; the `.ok` arm is `()`, so the `match`
types as a statement either way).

### Cost of guessing wrong: this is the conditional one

Two readings, two very different costs, and **I cannot tell which holds without
compiling one file**, which I am not doing here.

- If the compiler pins `main` to `()`: the wrong guess is a compile error at the
  first `?`. Loud, cheap, one retry. The hole costs first-try rate and nothing
  else.
- If the compiler accepts `function main() -> ()?`: the wrong guess produces a
  program that compiles, works on every input the author tests, and does
  something unspecified on the day the file is missing. Does it print `e.msg`?
  Does it exit non-zero? The document says neither. **This is the only one of
  the three whose defect lives exclusively on the path nobody runs.**

---

## Task 4 — what I assumed, in one line each

1. **`sort`.** I assumed nothing about `sort` at all: I routed around it and
   wrote my own comparison. The document left me to assume it by restricting
   `sort` to number/`str`/`bool` and never saying what "in order" means, so the
   only safe use of the built-in in a tie-break task is not to use it.
2. **`xs[i] @ v`.** I assumed an array element is an assignable place because
   the `Place` production admits it. The document left me to assume it by
   demonstrating indexed assignment only on maps (`m["a"] @ 1`), the one
   container whose indexing behaves differently from an array's.
3. **`main`.** I assumed `function main()` in § 1 is a signature and not just a
   name, hence that `?` is unavailable. The document left me to assume it by
   stating `?`'s rule in terms of "the caller's return type" and never fixing
   `main`'s.

---

## Ranking, by what a wrong guess costs

| # | hole | wrong guess produces | verdict |
|---|---|---|---|
| 1 | `sort`'s order (direction, `str`, `bool`) | **a program that compiles and prints a different order**, and, through the lookup recovery, one that duplicates tied records | dangerous |
| 2 | `main`'s signature / `?` in `main` | a compile error **or** an unspecified failure path — depends on the compiler | conditionally dangerous |
| 3 | `xs[i] @ v` on an array | a slower correct program, or a compile error | annoying |

---

## The three sentences, exactly as I would write them, each merged

### 1 — merge into § 7's last ordering sentence

Today: *"`nan` equals nothing, itself included, so `x != x` asks whether it is
one; ordering one aborts, in `< <= > >=` and in `sort`."*

Proposed:

> `nan` equals nothing, itself included, so `x != x` asks whether it is one;
> ordering one aborts, in `< <= > >=` and in `sort`, which orders ascending —
> `str` by byte, `false` first — and takes no key and no comparator.

Merged rather than appended: that sentence already couples `sort` to ordering,
which is why § 7 and not § 11 is the home — the rule is about what order means,
and § 11 is about which names exist. The clause answers all three unstated
facts and its last six words are the ones that redirect a model away from the
sort-the-keys-and-look-them-up recovery. **It deliberately does not mention
stability** (see the ruling above).

### 2 — merge into § 10's array-cost sentence

Today: *"`xs @ xs.push(4)` grows in place when `xs` is a plain name; reached
through a field or an index it copies the whole array, so lend the array itself
to an `@` parameter or hoist it into a name."*

Proposed:

> `xs[i] @ v` replaces one element; `xs @ xs.push(4)` grows in place when `xs`
> is a plain name; reached through a field or an index it copies the whole
> array, so lend the array itself to an `@` parameter or hoist it into a name.

Four words at the head of the sentence that already governs mutating an array,
one line below the map's `m["a"] @ 1`, so the two containers are shown
side by side. This is the cheapest of the three and the one I am most confident
is worth its tokens, not because the hole is dangerous but because it is
load-bearing for every hand-written sort the § 11 restriction forces.

### 3 — merge into § 1's first bullet

Today: *"One file is one module; the file you compile holds `function main()`."*

Proposed, **if the compiler pins `main`** (my recommendation if both are open):

> One file is one module; the file you compile holds `function main()`, that
> signature and no other, so `?` has no caller to propagate to there.

Proposed, **if the compiler already accepts a fallible `main`**:

> One file is one module; the file you compile holds `function main()`, or
> `function main() -> ()?` whose error ends the program with its `code`, its
> `msg` and a non-zero status.

I am not choosing between these from the document, because the document does not
contain the answer — that is the finding. If both are genuinely open as a design
question, I want the first: it makes a wrong `?` a compile error on the line
that wrote it, and the alternative it forces (`match`, or `.must()`, which
aborts saying why) is two lines and loses nothing. My standing preference is a
plausible mistake that errors loudly over a plausible mistake that compiles.

---

## Argument (120 words)

Three silences, one dangerous. `sort` is the dangerous one because it is the
only one whose wrong guess compiles and runs and prints: direction is never
stated, and the order on `str` is asserted by § 10 while § 7 denies `<` to
`str` entirely, so the document promises an order it never defines. The array
place and `main`'s signature cost tokens and retries, not correctness — except
under one reading of `main`, where the defect hides on the failure path.
Stability is different in kind: `sort`'s domain makes equal elements
indistinguishable, so the silence is a ruling and should stay. All three
proposals merge into sentences already present, and all three increase
locality, so there is nothing here for my veto to reach.

---

## Prediction (falsifiable at the next harness run)

1. **`sort`.** On a "sort records by a key, ties by input order" task under the
   current document, more than half of first attempts call `sort` on a
   non-scalar and take a compile error (the restriction working as designed),
   and **of the recoveries, at least one in three lands on the sorted-keys plus
   lookup route and is silently wrong on ties**. Sentence 1's last clause
   ("takes no key and no comparator") should cut that silent rate by at least
   half. Its ordering clause should take `str`-ordering and direction
   disagreements to zero, from a rate I predict is small but non-zero today.
2. **`xs[i] @ v`.** At least one first attempt in four at an in-place swap under
   the current document avoids element assignment — rebuilding the array or
   returning a new one. Sentence 2 takes that to approximately zero and changes
   the compile-error rate by nothing.
3. **`main`.** At least one in three first attempts at a `read_file` `main`
   under the current document is written `function main() -> ()?` and uses `?`.
   This one is checkable today, without a harness, by compiling four lines.

## Condition — what would change my verdict

- **On sentence 1:** if a harness run shows first attempts agree on ascending
  and on byte order at a rate indistinguishable from 100%, I withdraw the
  ordering clause and keep only "takes no key and no comparator", which is the
  half that addresses the measured silent defect.
- **On sentence 2:** if the current-document rate of building around element
  assignment is below one in ten, the sentence is not worth its tokens and I
  withdraw it.
- **On sentence 3:** compile `function main() -> ()?` with a `?` in it. If the
  compiler refuses it, hole 3 is merely annoying, the first wording lands, and I
  rank it below hole 1. **If the compiler accepts it, hole 3 overtakes `sort` as
  the dangerous one** — an accepted-but-unspecified failure path beats a
  differently-ordered output — and the second wording lands, with the exit
  status and the message settled by running it rather than by drafting it.
- **On stability:** if anyone produces a Heroes program, other than one printing
  a sorted `[f64]` containing both `0.0` and `-0.0`, whose output distinguishes
  a stable `sort` from an unstable one, my "silence is a ruling" finding is
  falsified and stability needs a sentence.

## Veto

**Not cast.** None of the three constructs is non-local. `xs[i] @ v` is
readable from the line plus whether `xs` was declared `@`; `sort`'s meaning is
fixed by the element type visible in the expression; and `?` in `main` is
decided by the enclosing signature, which is precisely what the locality rule
permits a reader to consult. All three proposals move meaning *onto* the line.
