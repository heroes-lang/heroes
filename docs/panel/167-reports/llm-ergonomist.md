# Panel 167 — llm-ergonomist

**Input discipline.** `spec/heroes-spec.md` and `docs/panel/167-briefs/llm-ergonomist.md`,
nothing else. The session's harness injected several `.claude/rules/*.md` files
into my context without my asking; they are process rather than design and I
excluded them from every judgement below. I read no design document, no
compiler source, no other brief. Where I wanted a fact about the compiler I
wrote the wish down as a question instead, which is what § Questions the
document does not answer is.

Method: write the program under each variant, and record every point where I
guessed. A plausible mistake that compiles is the enemy; a plausible mistake
that errors loudly is the win.

---

## verdict

| candidate | verdict |
|---|---|
| (a) `f.pin()` / `end_pin(@p)` | **object** — a correct new construct that does not close the hole, and whose own escape rule is unstated; under it Task 1 has no writable program |
| (b) `keeps` on the parameter, a lend may not reach one | **approve** |
| (c) "a lent address dies when the record's binding does" | **veto** (non-locality), as a *resolution*. As a sentence added beside (b) it is fine and I would welcome it |

My veto fires, once, on (c).

---

## Task 1 — the binding, written from § 13 alone

The C I was given, with the one fact that matters written where C writes it,
in the header's comment rather than in its types:

```c
/* kreg.h */
struct kslot { unsigned char bytes[8]; };
void k_register(const void *p, long n);   /* stores p; reads it later */
```

What I wrote, first try, from § 13 alone:

```
extern "kreg.h" link "kreg"
    record Slot tag kslot
        bytes: u8[8]
    function k_register(p: ptr counted_by n, n: i64)

function install()
    s: Slot @ Slot(bytes: [1, 2, 3, 4, 5, 6, 7, 8])
    k_register(s.bytes.ptr(), 8)

function main()
    install()
    print("registered")
```

I put the registration in `install` rather than in `main` on purpose, because
that is where the question bites: `main` outliving the binding hides it.

**The question: after `install` returns, what does C's stored pointer point at?**

**The document does not answer it.** I looked for a duration sentence attached
to a lend and there is none. What § 13 gives `f.ptr()` is three rules and not a
fourth:

- a bounds rule: *"one past the field is refused"*;
- a write-back rule: *"C writes back through the lend only where the binding is
  a `@` name"*;
- a placement rule: *"A lend and a lease name stand only as an argument of a
  call"*.

Every other duration word in § 13 is attached to something that is not a lend:
*"a COPY of the bytes that C may read **for as long as the program says**"*
(lease), *"the call **begins that handle's life** and names the one that ends
it"* (`acquires`), *"the call **ends that value's life**"* (`consumes`), and the
leak check, *"a lease nobody ends, like a handle nobody consumes, aborts when
`main` returns"*. The lend is named in that leak sentence's neighbourhood and
is absent from it.

The nearest sentence that could be read as an answer is § 3's, and reading it
as one makes the program *more* wrong, not less: *"Every value behaves as an
independent copy... **No aliasing exists among the values this language owns.**"*
Taken at face value that says the alias I just created cannot exist. A reader
who trusts it concludes there is nothing to worry about.

So the honest answer from the document is: **unspecified, and two incompatible
readings are each supportable from the text.**

- **Reading A.** `ptr()` is the address of the binding's own storage, so it is
  good while `s` lives and dead after `install` returns. C reads reused memory.
- **Reading B.** `ptr()` is the address of a temporary made for the call, by
  analogy with § 9's `@` parameters (*"copy in, copy out"*) and with `lease`
  being explicitly a COPY. Then the address is dead the instant the call
  returns, even inside `install`.

Under both readings the program above compiles and is a use-after-free. Under
a charitable third reading (§ 3) it is correct. **The line is the same in all
three.**

---

## Task 2 — the three candidates, each with the program I would write

### (a) `f.pin()` is a COPY, `end_pin(@p)` frees it, a pin nobody ends aborts

**What a program may do.** Take a copy of a field's bytes with a lifetime the
program controls, exactly as `lease` does for a `str`. **What it may not do:**
nothing new. (a) adds a route and forbids no route: `s.bytes.ptr()` into
`k_register` still compiles and still means whatever it meant in Task 1. The
question Task 1 asked is still unanswered after (a) lands.

**Could I write Task 1 correctly first try? No — I believe it is not writable at
all.** Here is how far I get:

```
function install()
    s: Slot @ Slot(bytes: [1, 2, 3, 4, 5, 6, 7, 8])
    p: ptr @ s.bytes.pin()
    k_register(p, 8)
    # and now what? C reads p for the rest of the program.
```

C keeps the pointer until the process ends, so the pin must not be ended until
`main` returns — at which point (a) says *"a pin nobody ends aborts... saying how
many"*. To end it in `main` I must carry `p` out of `install`, and two rules of
the document close that off:

- § 4: **"There are no mutable globals."** There is nowhere to park it.
- § 13: **"A lend and a lease name stand only as an argument of a call."** If a
  pin name inherits that sentence, `return p` is a compile error. If it does
  not, (a) does not say so. **The document would not tell me which**, and this
  is (a)'s central defect as drafted: it introduces a third bound name without
  saying whether it may escape its function, and the whole question is whether
  it may escape.

So under (a) the correct program for this very ordinary C API is either
inexpressible or a deliberate abort at `main`'s return. Note that this is not
new with (a): the same collision exists today for a `cstr` that C keeps forever.
(a) inherits it and widens it from strings to fields.

**Locality.** (a)'s own construct is local and self-describing: the line
`p: ptr @ s.bytes.pin()` says copy, and `end_pin(@p)` says release, both at the
line. (a) does not *create* non-locality. It leaves in place the one that
exists: the choice between `.pin()` and `.ptr()` at a call site is decided by a
fact stated nowhere in the Heroes text.

### (b) A `ptr` parameter C keeps past the call is declared `keeps`, and a lend may not reach one

**What a program may not do:** hand a Heroes-owned value's address to a C
function that keeps it. That is a **compile error**, and it is the exact program
I wrote in Task 1.

**What a program may do:** hand C an address C owns. That route already exists in
the document (§ 3: *"A `ptr` or a `cstr` is a copied ADDRESS, wherever it sits:
two copies reach one foreign thing"*), and § 13 lets me declare the allocator.
Task 1 under (b), and I checked each line against the document:

```
extern "kreg.h" link "kreg"
    record Slot tag kslot
        bytes: u8[8]
    function k_register(p: ptr counted_by n keeps, n: i64)

extern "stdlib.h"
    function malloc(n: u64) -> ptr

extern "string.h"
    function memcpy(dst: ptr, src: ptr counted_by n, n: u64) -> ptr

function install()
    s: Slot @ Slot(bytes: [1, 2, 3, 4, 5, 6, 7, 8])
    buf = malloc(8)
    _ = memcpy(dst: buf, src: s.bytes.ptr(), n: 8)
    k_register(buf, 8)
```

**Could I write it first try? Yes, and the document caught two of my errors on
the way.** My first draft wrote `memcpy(buf, s.bytes.ptr(), 8)` positionally;
§ 9's rule — *"When two parameters in a signature share a type, named arguments
are mandatory at the call site"* — makes that a compile error, because `dst` and
`src` are both `ptr`. My first draft also dropped `memcpy`'s result on the
floor; § 5's *"A line that computes a value must use it"* makes that a compile
error too, and `_ =` is legal here because `ptr` is not fallible. Both mistakes
were plausible and both were loud. That is the document working.

**Locality — and this is why I do not veto (b).** The refusal is triggered by a
declaration written in Heroes, in an `extern` group, in this file or a `use`d
one, and its effect is a **refusal** rather than a change of meaning. The line
`k_register(buf, 8)` then means precisely what § 3 already says any `ptr`
argument means: an address handed to C, whose life is C's business. (b) creates
no new silent reading of any line. It deletes one.

The price is honest and I will name it: after (b) the memory C keeps is memory
the language does not track, so `free(buf)` too early is still a C-shaped bug.
But § 3 already says that of every `ptr`, in those words, and the line drawn is
a clean one: **a value this language owns never escapes; memory C owns is C's,
and the document says so.**

### (c) A lent address dies when the record's binding does

**What a program may do:** everything it does today. **What it may not do:** the
sentence names a mistake and forbids nothing — no construct changes, no check
changes, no word is added to any line.

**Could I write Task 1 correctly first try?** Only by a chain of three steps the
document does not join for me: read one sentence in § 13, connect it to a fact
printed in `kreg.h`'s comment, and then invent the malloc route on my own,
because (c) offers no replacement for a field. `lease` does not help: it is
`str` to `cstr`. The program I wrote in Task 1 still compiles, unchanged, and
(c)'s effect is to make the project's own document describe it as a
use-after-free while emitting it.

There is also a shape (c) gets wrong on its own terms. *"Dies when the record's
binding does"* is falsified by the document's own `@` parameter rule if the
binding is a parameter:

```
function fill(@s: Slot)
    k_register(s.bytes.ptr(), 8)
```

§ 9 says `@` parameters are *"copy in, copy out (copy-out always happens...)"*.
So the storage `s` names inside `fill` is not the storage the caller's binding
names, and the lent address dies when the **call** returns, not when the
caller's binding does. (c) as written tells the reader the wrong thing about
this shape, which is the shape a lend will most often be written in, since the
`@` is how C writes back.

**Locality: the veto.** Under (c), whether `k_register(s.bytes.ptr(), 8)` is a
correct program or a use-after-free is determined by a sentence in a C header's
comment. It is not visible at the line. It is not visible in the enclosing
signature. It is not visible anywhere in the Heroes program, not even in the
`extern` declaration, because `k_register`'s declaration under (c) is identical
whether C keeps the pointer or not. That is exactly the class my veto is for,
and (c) does not merely leave it standing — it adopts it as the language's
answer. **Veto.**

---

## Task 3 — ranked by how many ordinary programs pay

The honest answer to the question as asked is that **it does not discriminate,
and that finding is the useful part.**

| candidate | what an FFI-free program pays |
|---|---|
| (b) | nothing. `keeps` can only be written inside an `extern` group, and the refusal can only fire where a lend meets one |
| (c) | nothing. One sentence in the section such a program never reaches for |
| (a) | nothing at run time. A pin-leak abort at `main`'s return cannot fire in a program with no pins |

All three are zero. **So "it is cheap for ordinary programs" is not an argument
for (c) over (b): they are tied at zero, and the tie is exact.** Any ranking has
to be made on the FFI programs and on the reader, and there the order inverts.

The ranking, by total cost including who actually pays:

1. **(b), cheapest.** Zero ordinary programs, one word in one `extern`
   signature, written at the place where the fact it encodes is true. The
   reader who never writes an `extern` never meets it; the reader who does meets
   it beside `consumes`, `acquires` and `borrows`, which are the same kind of
   word doing the same kind of job, so it costs a slot in a category that
   already exists rather than a new category.
2. **(a), most expensive in the only currency that is not zero.** Every reader
   of the document carries two more names and a third release discipline
   (`end_lease`, the named releaser of `acquires`, now `end_pin`) and a third
   leak abort, whether or not any program of theirs uses one. That is paid per
   reader, not per program — and the document is the prompt, so a name that
   never fires still occupies the reader. And it buys the least: the wrong
   program still compiles.
3. **(c), zero to learn and unbounded to get wrong.** Its cost does not appear
   in the column the question asks about, which is the reason to distrust the
   column. It converts a compile-time cost of one word into a run-time cost
   borne by whoever runs the binary, which is the trade this language exists to
   refuse.

---

## hesitation_points

Where I guessed, and what a wrong guess produces. This is the load-bearing part
of my report.

| # | the guess | wrong guess produces |
|---|---|---|
| 1 | **Does the lent address survive the call?** Task 1's whole question. Readings A, B and the § 3 reading all fit the text | **silently different program.** A use-after-free that compiles. This is the defect |
| 2 | Is the receiving binding required to be `@`? § 13 names `@` only for write-back, so I read a non-`@` lend as legal, and then chose `@` anyway, defensively | **silently different program.** If `@` means a copy-out temporary is what was lent, the address differs and the text does not say |
| 3 | Spelling: `s.bytes.ptr()`, from § 9's UFCS. Not `ptr(s.bytes)` | compile error. Loud, good |
| 4 | `counted_by n` names a sibling **parameter**, not a sibling field. The grammar decides it: `counted_by` sits inside `CParam` | compile error. Loud, good — and this one is settled by the production, which is why it cost me no time |
| 5 | `counted_by n` names a parameter declared **after** it. The document does not say whether the sibling may be later | compile error, probably. Loud |
| 6 | Is a parameter of an `extern` function exempt from § 5's *"An unused binding or parameter is a compile error"*? There is no body to use `n` in | compile error. Loud, but it would have stopped me, and the document does not say |
| 7 | `long` is `i64` here. § 13's width rule sends me to the header; this is 32 bits on Windows | refused by clang per § 13. Loud, good |
| 8 | Passing `8` where the field is 8 bytes. *"one past the field is refused"* says 9 errors; it does not say whether 7 is allowed | silently different, but harmless |
| 9 | **May a lend be bound to a name at all?** *"A lend and a lease name stand only as an argument of a call"* implies yes, and restricts where the name may then appear | compile error either way. Loud |
| 10 | **(a) only.** May a pin name be returned from a function? Inherits #9 or does not; the candidate is silent, and Task 1 turns on it | compile error, or an unavoidable abort at `main`. Either way the program cannot be written |
| 11 | **(b) only.** Where in `CParam` does `keeps` go — a fourth alternative beside `consumes`, `acquires`, `borrows`, or its own optional slot? | parse error. Loud |
| 12 | **(b) only.** May `keeps` combine with `counted_by`, and with `@`? | compile error. Loud |
| 13 | `memcpy`'s `dst` and `src` share a type, so § 9 makes named arguments mandatory. I got this wrong in my first draft | compile error. Loud, good — the document caught me |
| 14 | `_ = memcpy(...)`. Legal because `ptr` is not fallible; § 5's `_` rule reads the outermost type | compile error. Loud, good |

Count of hesitations whose wrong guess is **silent**: three (#1, #2, #8), and
two of the three are the lend's lifetime. Every other hesitation in this
document errors loudly. That ratio is the case for acting.

---

## Questions the document does not answer

Written as questions rather than as facts about a compiler I did not read.

1. How long is a lent address good for? (The sitting's question.)
2. May a lend be taken from a binding that is an `@` parameter, and if so is the
   address the caller's storage or the callee's copy? § 9's copy-out makes this
   a real fork and § 13 does not name it.
3. May a lend name, or a lease name, be returned from a function? Task 1 under
   (a) is writable or not writable depending on the answer.
4. Is a `ptr` obtained from C (a `malloc` result, say) tracked at all? § 13's
   leak check names leases and handles; `acquires` is described as reaching *a
   handle*, and `ptr` is not one. I concluded untracked, from a silence.
5. Is an `extern` function's parameter subject to § 5's unused rule?

---

## argument

§ 13 gives a lend a bounds rule, a write-back rule and a placement rule, and no
duration rule. The one question a keeping C function asks is the one it does not
answer. (c) answers it in prose and leaves the wrong program compiling: the
correctness of `s.bytes.ptr()` then lives in `kreg.h`'s comment, outside the
language. (b) makes that program a compile error for one word, written where
the fact lives, in the extern signature; what remains hands C memory C owns,
which § 3 already governs. (a) adds a name and a release duty, does not forbid
the lend, and collides with an API that keeps forever: no mutable globals, so
nothing ends the pin.

---

## prediction

Falsifiable, and checkable the next time the harness puts the document in front
of a model. Given `spec/heroes-spec.md`, the `kreg.h` above with its comment
`/* stores p; reads it later */`, and the instruction to write the binding:

- **Under the current text and under (c)**, at least **7 of 10** first tries
  produce a program containing `k_register(<field>.ptr(), 8)` or its spelling
  variants — a program that compiles and hands C an address the program will
  reuse. Under (c) specifically I predict the rate falls by **less than 2 of
  10** against the current text, because the sentence names the mistake and
  offers no replacement for a field.
- **Under (b)**, **0 of 10** such programs compile, and at least **6 of 10**
  first tries either take the C-owned-buffer route or fail with a diagnostic
  naming `keeps` — a loud failure, not a silent one.
- **Under (a)**, at least **4 of 10** first tries produce a program that either
  never calls `end_pin` (and so aborts at `main`'s return) or tries to carry the
  pin name out of its function by `return` or by a record field, because the
  candidate does not say whether a pin name may escape. And at least **3 of 10**
  still write `.ptr()` rather than `.pin()`, because no line of Heroes says
  which one this C function wants.

The number to watch is the silent-error rate, not the first-try rate: (b) is
allowed to lower the first-try rate and still win, provided every failure it
causes is a diagnostic.

---

## condition

What would change each verdict:

- **(b) approve → object** if the C-owned-buffer route turns out to be
  unwritable — if `malloc`'s `ptr` result cannot be passed to a `ptr` parameter,
  if a `ptr` that is not a lend cannot reach a `counted_by` parameter, or if
  `keeps` combined with `counted_by` is refused. (b) would then be a refusal
  with no route out, which is a worse failure than the one it repairs. This is
  checkable by compiling the eleven-line program in this report.
- **(a) object → approve** if a run shows models choose `.pin()` over `.ptr()`
  at **90% or better** when the header comment says the pointer is stored —
  that would mean the naming carries the fact well enough that a refusal is not
  needed — **and** the candidate is amended to say whether a pin name may
  escape its function, because Task 1 is unwritable until it does.
- **(c) veto → object** if (c) is adopted **beside** (b) rather than instead of
  it. As documentation of what a lend is, the sentence is good and I would keep
  it; my veto is against it standing alone as the answer, because then the
  meaning of a line lives in a C header.
- **My whole ranking inverts** if it turns out that ordinary FFI-free programs
  do pay for (b) in some way I could not see from the document — for instance
  if `keeps` must also be written at the call site. Task 3 rests on the claim
  that `keeps` appears only inside an `extern` group.
