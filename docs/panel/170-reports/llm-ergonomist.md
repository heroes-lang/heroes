# Panel 170 — llm-ergonomist

Input: `spec/heroes-spec.md` and this seat's brief. Nothing else was read.

| | verdict |
|---|---|
| **W1** (`CParam` gains `keeps end_fn`) | **object** |
| **W2** (`extern` head gains `retains`) | **VETO** — locality |
| **W0 / W3** (this seat's third wording, Task 4) | **approve** |

Programs written: **8**. First-try under the document alone: CURRENT 2/2,
W1 **0/2**, W2 1/2, W3 2/2. Detail below.

---

## 0. The sentence I proposed, read fresh

> *A lend lives for its call and no longer: C keeping the pointer reads bytes
> the program may have changed or freed since, and nothing checks it.*

**It lands where I meant it and it says four fifths of what I needed.**

Where it sits is better than I expected. It closes the lend paragraph and the
next sentence opens `x: cstr @ s.lease()` **is a COPY of the bytes that C may
read for as long as the program says**. Read in sequence the two make a
contrast a careful reader completes: *for its call and no longer* against *for
as long as the program says*. So the brief's framing — *the document does not
say what [to do instead]* — is not quite right, and I record the disagreement
because I am the seat reading the document as a document. The answer is in the
very next sentence; what is missing is the word that says it **is** the answer.

The missing fifth is an imperative. Today the reader gets a hazard and,
adjacent, a facility, with no `so` between them. Every token of both proposals
is being spent to buy that `so` in grammar. It costs about eight tokens in
prose (§ 4 below).

One thing I would change if I could re-propose it: *nothing checks it* is a
universal quantifier over lends. **Any mark that checks some of it makes my own
sentence false as written**, and neither diff amends it. That is not a quibble;
it is the whole of Task 3.

---

## 1. Task 1 — bind a C function that keeps a string

Target: `putenv`. The string becomes part of the environment and is read after
the call returns. (That the library keeps is a fact about libc, not about this
document — true under every variant, and it is the fact the mark cannot supply.)

### CURRENT — first try, compiles, correct

```
extern "stdlib.h"
    function putenv(s: cstr) -> i32
    function getenv(name: cstr) -> cstr

function main()
    kept: cstr @ "HEROES_TZ=UTC".lease()
    rc = putenv(kept)
    if rc != 0
        print("putenv failed")
        end_lease(@kept)
        exit(1)
    match getenv("HEROES_TZ".cstr()).validated()
        .ok v  => print(v)
        .err e => print(e.code)
    end_lease(@kept)
```

Every line is derivable from § 13 and § 5. The lend at `getenv` is right (it
does not keep), the lease at `putenv` is right (it does), the two are visibly
different at the call site, and the unended-lease abort is the backstop.

### W1 — **blocked, 0/2**

```
extern "stdlib.h"
    function putenv(s: cstr keeps ????) -> i32
```

`keeps` takes a mandatory `ident` and **there is no function in libc that ends
this keep**. The environment holds the bytes until the process exits. My three
options were all bad:

1. write no mark — which under W1 asserts *this call does not keep*, the exact
   opposite of the truth;
2. invent `keeps free` and declare `free` in the group — then the compiler
   **demands** I call `free(kept)` before `main` returns, while the environment
   still points at those bytes. W1 converts *you may forget* into *you are
   required to do the unsafe thing*;
3. abandon W1's mark and write the CURRENT program, with a `keeps` slot in the
   grammar staring at me unused.

Second binding, the document's own example library:

```
extern "sqlite3.h" link "sqlite3"
    record Stmt tag sqlite3_stmt
    function sqlite3_bind_text(st: Stmt, i: i32, s: cstr keeps ????, n: i32,
                               d: ptr) -> i32
```

With `SQLITE_STATIC` the statement keeps the bytes until `sqlite3_finalize(st)`.
So the ender exists — but **it takes the statement, not the string**. W1's
prose (*`end_fn` … is what ends that lease*) can only mean `end_fn` applied to
the lease. It cannot be applied to a handle that merely encloses the lease's
life.

**The finding, stated as shapes rather than as libraries.** A keeping C
parameter comes in three shapes, and W1 expresses one:

| shape | ender | W1 |
|---|---|---|
| kept for the process (`putenv`, an options table) | none exists | inexpressible |
| kept until an enclosing handle dies (`sqlite3_bind_text`, a stream buffer) | takes the **handle** | inexpressible |
| kept until a function is called **on the pointer** | takes the pointer | expressible |

I could name a candidate for the third shape only with effort, and every
candidate I could name is really shape two wearing a convenience wrapper. I did
not run anything against a real header, so treat the table as three shapes I
could enumerate, not as a census.

### W2 — compiles, 1/1, at twice the ceremony

```
extern "stdlib.h" retains
    function putenv(s: cstr) -> i32
    function getenv(name: cstr) -> cstr

function main()
    kept: cstr @ "HEROES_TZ=UTC".lease()
    rc = putenv(kept)
    if rc != 0
        print("putenv failed")
        end_lease(@kept)
        exit(1)
    k: cstr @ "HEROES_TZ".lease()          # forced: the lend is refused group-wide
    match getenv(k).validated()
        .ok v  => print(v)
        .err e => print(e.code)
    end_lease(@k)
    end_lease(@kept)
```

`getenv` does not keep anything. W2 refuses its lend anyway, so a call that was
**statically** safe becomes a lease with a **runtime** obligation. That is the
trade W2 makes everywhere in a group: it converts compile-time-checked lends
into runtime-checked leases for every parameter that does not keep, which in a
real header is nearly all of them. The document's own abort for a missed one
*says how many*, not which.

The escape is to split the header into a plain group and a `retains` group.
**The document does not say whether two `extern` groups may name one header.**
Only `tag` carries a uniqueness rule.

---

## 2. Task 2 — the program the mark is supposed to make impossible

The program, and it is the one a fresh model writes:

```
function main()
    rc = putenv("HEROES_TZ=UTC".cstr())
    if rc != 0
        print("putenv failed")
```

**CURRENT:** legal. A lend *stands as an argument of a call*, which is exactly
what this is. The document tells me the bytes die with the call and that
**nothing checks it** — so the document tells me this program is wrong, and
tells me nothing will stop me. Honest, and the danger is stated.

**W1:** refused at the parameter, *a lend is refused there*. The refusal is
legible on the line I am reading — `s: cstr keeps free` — and I can obey it
without leaving the signature. **W1 passes the locality rule.** But the refusal
only exists for a binding whose author already knew the answer, and a binding
author who knew would have reached for `.lease()` anyway (§ 5, prediction).

**W2:** refused, *throughout the group*. Standing at the call site I cannot see
the refusal. I must scroll to the group head; and if the group came in by `use
cbind`, the word that decides whether this line compiles is in another file.

---

## 3. Task 3 — what a reader believes about the UNMARKED case

**This is the one that decides the verdict, and the answer is yes: both marks
make a reader trust the unmarked case more than they do today, and neither diff
buys that trust back.**

### Today

A reader gets one universal sentence — *nothing checks it* — and no signal
anywhere. The duty it creates is **uniform**: at every `.cstr()` I must ask
whether this C function keeps, because the declaration will never tell me. A
uniform duty is expensive but it has no false negatives. There is no declaration
shape that says *safe*.

### Under W1 or W2

A signal exists, so its absence becomes information. The document does not say
**what** information. Neither diff contains a sentence about the unmarked case.
A reader supplies one, and the reading they will supply is fixed by the rest of
§ 13, because **every other optional mark there has a defined absence**:

| mark | what absence means |
|---|---|
| `@` | not an out-parameter |
| `counted_by n` | `f.ptr()` is refused here |
| `partial` | all the header's fields |
| `tag` | the struct's name is the record's name |
| `owned f` | the compiler does not free it |
| `consumes` | the value survives the call |
| `acquires f` | no handle life begins |

Six of seven absences are enforced by the compiler. So the habit § 13 teaches is
*absence is the safe, checked default*. `keeps`/`retains` would be the first
mark in the section whose absence is an **unbacked claim about a C library's
runtime behaviour** — and the reader will not notice the difference, because it
is written in the same slot, in the same grammar, next to six marks that mean
what they say.

Yes — I see the counter-argument, and I put it here rather than let another
seat find it: `owned`, `consumes` and `acquires` are *also* unverifiable
assertions about a library. Forgetting `consumes` is also a silent
use-after-free. The difference is what my sentence does. Those three marks make
the compiler **do bookkeeping**, and their absence means *no bookkeeping, you
are on your own* — which the reader can feel, because they can see nothing
happening. The lend is the one place where being on your own is **invisible**,
which is why § 13 spends a sentence saying so. A `keeps` mark puts a visible
absence where the invisible danger is, and that is a worse combination than
either alone.

### The contradiction neither diff repairs

Three lines apart the document would say:

> … and nothing checks it.
> A parameter marked `keeps end_fn` … a lend is refused there …

A reader resolves that. The cheap resolution — the one a model under a token
budget takes — is *"nothing checks it" was the old state; the mark is the
check*. My sentence stops being a standing duty and becomes a footnote about
unmarked legacy bindings. **The proposals do not add a check to the unmarked
case; they subtract the warning from it.**

---

## 4. Task 4 — a third wording

### W0, prose only, no grammar (recommended)

Amend the sentence I proposed, in place:

> A lend lives for its call and no longer: C keeping the pointer reads bytes the
> program may have changed or freed since. **Nothing checks it, so a call that
> keeps takes a lease.**

Eight tokens, one `so`, no new form. It keeps *nothing checks it* **true**,
which is the load-bearing half; it supplies the imperative, which is the missing
fifth; it creates no declaration shape that can be read as *safe*; and it
attaches the instruction to the hazard rather than to the next paragraph.

### W3, if the sitting wants grammar anyway

> `CParam` gains `[ "keeps" ]` — **no ident**.
> *A parameter marked `keeps` is one the call holds on to: a lend is refused
> there and a lease is what it takes, ended by `end_lease` when the program says
> the keep is over. Unmarked, the language cannot tell, which is what nothing
> checks it means.*

What dropping the ident buys, measured against the programs above:

- **all three keeping shapes become expressible.** `putenv`:
  `function putenv(s: cstr keeps) -> i32`, ended by `end_lease(@kept)` on the
  last line of `main`. `sqlite3_bind_text`: `s: cstr keeps`, ended after
  `sqlite3_finalize`. No invented `free`, no ender that takes the wrong value;
- **the enforcement already exists** — *a lease nobody ends … aborts when `main`
  returns* — so W3 adds a refusal and no new machinery;
- **it says what absence means, in the same breath**, which is the only thing
  that stops Task 3's damage;
- it is local: the mark, the refusal and the obligation are all on the parameter
  line.

W3's rewrite under W1's own binding, first try, 2/2:

```
extern "stdlib.h"
    function putenv(s: cstr keeps) -> i32
    function getenv(name: cstr) -> cstr
```

`getenv` keeps its lend. `putenv` refuses it. One word, one line, no group.

---

## 5. hesitation_points

Where I guessed, and what a wrong guess produces.

| # | hesitation | wrong guess gives |
|---|---|---|
| 1 | *outside a group nothing answers `cstr`* — may a **local binding** be typed `cstr`? I read "answers" as *is a parameter or result of a function outside a group*; the document's own `x: cstr @ s.lease()` backs me | compile error — **loud** |
| 2 | does `exit(1)` discharge lease obligations, or does *aborts when `main` returns* still fire? I ended every lease before `exit` defensively | runtime abort — loud, but late |
| 3 | `end_lease`, `.lease()`, `.cstr()`, `.validated()`, `.validated_bytes()`, `.ptr()` are **not in § 11's `Built-ins:` sentence**. I assumed § 13 offers them | compile error — loud. But it is a real friction, and any proposal that makes leases more common (W2 above all) pays it more often |
| 4 | **W1**: where in `CParam` does `keeps ident` go — its own slot, or the alternation with `consumes`/`acquires`/`borrows`? If its own slot, `s: cstr keeps free consumes` is derivable and meaningless | parse error — loud; but the diff is underspecified as a diff |
| 5 | **W1**: must `end_fn`'s own parameter be marked `consumes`, as `sqlite3_close` is for `acquires`? The prose does not say | unknown — possibly a **silent** under-check |
| 6 | **W1**: does calling `end_fn(x)` **empty the lease cell**? `end_lease(@x)` does, and says so, and takes `@`. A C `function free(p: cstr)` cannot | **silent** — a dangling cstr in a live cell, and it is the one case the mark exists to prevent |
| 7 | **W2**: may two `extern` groups name one header, so a mixed library can be split? Unstated | unknown; if refused, loud |
| 8 | **W2**: *a buffer C owns* appears in the prose and **nowhere in the document**. There is no such notion in § 13 | I cannot write the program the prose recommends |
| 9 | **W2**: may `retains` appear without `link`/`package` (`extern "stdlib.h" retains`)? The grammar line is not given | parse error — loud |
| 10 | CURRENT, tangential but mine to find: the document's own example library's keeping mode is `SQLITE_STATIC`, a **null function pointer**. `nullptr` is documented as the null of `ptr` and `cstr` only, and a callback is a parameter of function type. So the canonical *C keeps the pointer* call **cannot be written**, and neither proposal touches that | clang error — loud. Worth a separate question |

Hesitation 6 is the one I would press at the sitting. W1's ender is the only
mark in § 13 whose discharge the compiler cannot observe emptying the thing it
discharged.

### What I wanted from the repository and did not take

Three wants, written down instead of satisfied, per the brief: whether two
`extern` groups may name one header; whether `exit` runs the end-of-`main` lease
check; whether `end_lease` is a built-in name the checker knows or a § 13-local
form. All three are questions the document leaves open to the reader it is
written for, which is why they belong in this column rather than in a grep.

---

## 6. argument (118 words)

Today § 13 says nothing checks a lend's lifetime, and that sentence is a
reader's whole duty: uniform suspicion at every `.cstr()`. Both variants replace
suspicion with a signal, and neither says what the signal's absence means. Every
other optional mark in § 13 governs bookkeeping the compiler then performs, so a
reader reads absence as safety; here absence is an unbacked claim about a C
library. W1 demands an ender that two of three keeping shapes do not have. W2
puts a parameter's meaning on a line that is not its signature, and in another
file once `use` is involved: the locality rule, hence the veto. W0 keeps
"nothing checks it" true and adds the missing imperative.

---

## 7. prediction

Falsifiable, with numbers, checkable when the harness next runs. Two task
shapes, because the mark's value depends entirely on which one is measured and
I expect the harness to measure only the first.

**Shape A — the model writes the binding AND the call** (the harness's usual
shape: *bind `putenv` and set an environment variable*).

> Under W1 and under W2, the count of lifetime-correct programs changes by
> **≤ 1 in 20** against CURRENT. The mark is self-certifying: a model that knows
> `putenv` keeps will write `.lease()` under the current document, and a model
> that does not know will not write `keeps` either. Under W1 I predict a
> **further ≥ 2 in 20 regression**, from programs that write `keeps free` and
> are then forced to call it.

**Shape B — the model is HANDED an unmarked binding of a keeping function and
writes the call.** This is the shape Task 3 is about and I predict nobody runs
it unless the sitting asks.

> Given `function putenv(s: cstr) -> i32` with no mark, models choose the lend
> `putenv(s.cstr())` in **≥ 8 of 10** runs under W1 or W2, against **≤ 6 of 10**
> under CURRENT — a swing of **≥ +2 in 10 toward the silent bug**, and never
> negative. Under **W0** I predict **≤ 4 of 10**, because *so a call that keeps
> takes a lease* is an instruction attached to the hazard rather than a property
> attached to a declaration somebody else wrote.

**W2 only, third measurement.** On a task with one keeping function and two
non-keeping ones in the same group:

> **≥ 2 in 10** programs acquire a new unended-lease abort at `main`'s return
> that the same task produces zero of under CURRENT, and the abort's text says
> *how many*, not which.

---

## 8. condition

What would change each verdict.

**W1 (object → approve).** Both of these, together:

1. an amended `keeps` that either drops the mandatory ident (W3) or shows me,
   in the diff, how the two shapes I could not write are written — `putenv`
   with no ender, and `sqlite3_bind_text` whose ender takes the statement;
2. **one added sentence saying what the unmarked case means**, in the diff, not
   in the synthesis. Without it my verdict stays object whatever else changes,
   and if the panel adopts W1 with *nothing checks it* left standing three lines
   away I will ask for the contradiction to be recorded as a known defect.

**W2 (veto → object).** The veto lifts only if `retains` stops deciding what an
argument expression may be. A group-head word that changes a **default** which
each parameter may then restate on its own line is local; one that decides from
the head is not. I would also need *a buffer C owns* defined in the document,
and shape-A measurements showing the group-wide lease ceremony does not cost
more in unended-lease aborts than the lends it refuses.

**W0 (approve → object).** A measurement showing shape-B lend-choices do **not**
fall under the added `so` clause — i.e. that the imperative buys nothing and the
eight tokens are unpaid for.
