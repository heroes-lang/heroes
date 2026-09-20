# Panel 169 — llm-ergonomist

**Input discipline observed.** I read `docs/panel/169-briefs/llm-ergonomist.md`
and `spec/heroes-spec.md`, and nothing else. Every claim below about what "the
document says" is a claim about that one file. Where I wanted repository
context I wrote down the wanting instead of satisfying it; those are marked
**[wanted]**.

---

## verdict

**V1 — object.** Two of its three clauses restate sentences § 13 already
carries, and its one prescription (*takes a handle, never a lent pointer*) is
addressed to whoever wrote the C header, not to the Heroes programmer. It
cannot be obeyed from the Heroes side when the header is fixed, and it produces
no diagnostic for any of A, B or C.

**V2 — object, and it is the better half of the pair.** It is the only sentence
in the pair that turns a silent wrong answer into a compile error, and it does
so for exactly one of the three programs (C). It is objectionable as written
because its trigger — *while the lend can still be read* — names a quantity the
document never states. The two readings a reader can give it produce two
different programs, one of which compiles.

**Neither is vetoed.** V2's error is reported at two lines in one function body
and names both; that is local. (See § Veto, deliberately not exercised.)

**The finding that outranks the pair** is Task 3's: **the document contains no
sentence about how long a lend is readable**, and both candidates presuppose
one. V1 asserts it in passing and buries it inside an unusable prescription; V2
depends on it and does not supply it. I propose a third sentence, T, at the end.

---

## experiment

### The extern group I had to write before any program could exist

The brief gives program bodies, not declarations, so every run below starts
from the same invented header binding. What I invented is itemised in
§ hesitation_points; here is the artefact.

```
extern "keeper.h" link "keeper"
    record Sl
        name: u8[8]
        id: i64
    function mk() -> Sl
    function c_keeps(p: ptr counted_by n, n: u64)
    function c_reads_later() -> i32
    function c_keeps_text(s: cstr)
    function c_text_length() -> i32
```

Derivations I could cite: `record Sl` with no `tag` is the header's `struct Sl`
("A group's `record` is the header's struct: all its fields, and the same name
unless the header writes it after the word struct"); `name: u8[8]` because "a
field is a number, `bool`, `ptr`, `cstr`, another record of the group, or a
fixed array of one: `i32[4]`, never a `[T]`", and eight elements because "build
one with `[a, b, c, d]`, as many elements as the type says" matches the brief's
`n: 8`; `p: ptr counted_by n` because "`f.ptr()` lends a binding's field to a
`ptr` parameter declared `counted_by n`"; no `->` on a void function because
`Member`'s `[ "->" Type … ]` is optional and `()` is `void`.

### Today's document — programs A, B, C as given

All three transcribe with no change, and I believe all three compile. The
statement-level rule that could have caught me is § 5's, and the document
answers it: `c_keeps(…)` returns `()`, "a line that computes a value must use
it … which a `()` line refuses: it stands alone", so the bare call line is
correct and `_ = c_keeps(…)` would be the error. That is a plausible mistake
that errors loudly — the good kind.

**Result: 3 of 3 compile, 3 of 3 give a wrong answer at run time, 0 diagnostics.**

### Task 3 — the correct version of each, from the document alone

**C — writable, not justifiable. (Counted as a failure.)**

```
function main()
    s = Sl(name: [72, 2, 3, 4, 5, 6, 7, 8], id: 1)
    c_keeps(p: s.name.ptr(), n: 8)
    print(to_str(c_reads_later()))
    t = Sl(name: [1, 2, 3, 4, 5, 6, 7, 8], id: 1)
    c_keeps(p: t.name.ptr(), n: 8)
    print(to_str(c_reads_later()))
```

Two lines from the original, and it prints 72 then 1 honestly. But I cannot cite
a sentence for **why** the original was wrong or **why** this is right. Nothing
in § 13 says that rewriting `s` disturbs bytes C holds, and nothing says the
first lend is still readable at the first `print`. I wrote this from C knowledge,
not from the document. Worse, the document argues the other way: § 3 says "Every
value behaves as an independent copy … **No aliasing exists among the values this
language owns**", and then qualifies only foreign things ("A `ptr` or a `cstr` is
a copied ADDRESS … two copies reach one foreign thing"). A reader who takes § 3
at its word concludes that `s @ Sl(…)` makes a fresh independent value and cannot
reach the bytes C is holding. Program C is precisely the aliasing § 3 promises
does not exist, and § 13 never withdraws the promise for `f.ptr()`.

**B — writable and justifiable. The one success. (1 of 1.)**

```
function main()
    label: cstr @ "row-0-payload".lease()
    c_keeps_text(s: label)
    print(to_str(c_text_length()))
    end_lease(@label)
```

Cited: "`x: cstr @ s.lease()` is a COPY of the bytes that C may read for as long
as the program says, and `end_lease(@x)` frees it and empties the cell", plus "a
lease nobody ends … aborts when `main` returns". Move the free after the last
read; keep it before `main` returns. The document decides this for me.

**A — not writable. (0 of 1.) This is the missing sentence.**

Route 1, the one I would ship:

```
function main()
    s: Sl @ mk()
    c_keeps(p: s.name.ptr(), n: 8)
    print(to_str(c_reads_later()))
```

Same two-line distance (inline the helper). I believe it is correct. **The
document gives me nothing to believe it with**: there is no sentence about when a
binding's bytes stop existing, no sentence about frames, and no sentence about
how long a lend is readable. I am repairing a defect the document does not admit
exists.

Route 2, the one the situation actually calls for — give C bytes that outlive the
frame — **is unwritable**. The only long-lived route in § 13 is `lease`, whose
type is fixed: "`x: cstr @ s.lease()`". `c_keeps` takes `ptr counted_by n`. There
is no `cstr`→`ptr`, no `lease` for a field, and "Nothing lends a field to
`cstr`". So:

> **For a `ptr counted_by n` parameter, the document offers no way to hand C bytes
> that survive the call.** Lease is `str`→`cstr` only.

That is a hole a reader falls into with no rope, and it is the thing I most
wanted repository context to check. **[wanted]** — I could not, and the wanting is
the finding: if a route exists, it is not in the document a model is given.

### V1 — program C

V1's instruction is that the keeping function take a handle. A handle is C's
pointer to a tagged struct; **Heroes cannot make one**, only receive one through
`acquires`. So obeying V1 means the C library must expose a constructor that
copies, and I must write that header myself:

```
extern "keeper.h" link "keeper"
    record Sl
        name: u8[8]
        id: i64
    record Kept tag kept
    function c_keep_bytes(p: ptr counted_by n, n: u64) -> Kept acquires c_drop
    function c_drop(k: Kept consumes)
    function c_reads_later(k: Kept) -> i32

function main()
    s = Sl(name: [72, 2, 3, 4, 5, 6, 7, 8], id: 1)
    k = c_keep_bytes(p: s.name.ptr(), n: 8)
    print(to_str(c_reads_later(k: k)))
    s2 = Sl(name: [1, 2, 3, 4, 5, 6, 7, 8], id: 1)
    print(to_str(c_reads_later(k: k)))
    c_drop(k: k)
```

It compiles as far as I can tell, and it is honest. Note what it cost: **the
constructor still takes a lent pointer**, the very thing V1 forbids, and V1 is
satisfied only by the unwritten, uncheckable claim that `c_keep_bytes` copies
before returning. The compiler cannot know that. So V1's main clause is a
convention about C libraries, wearing the grammar of a language rule.

`s2` rather than `s @ …` is my own caution, not V1's: V1 says nothing about
writing a lent binding, so under V1 alone the original `s @ Sl(…)` line stays
legal. Because the handle holds a copy, that is fine — but only if the reader
trusts the copy, which no sentence states.

### V1 — program B

V1 read straight makes B collapse to a lend, because a copying callee only needs
the call:

```
    t = c_keep_text(s: "row-0-payload".cstr())
    print(to_str(c_text_length(t: t)))
    c_drop_text(t: t)
```

Again a new header. And here is V1's hole: **V1 does not remove `lease`, and does
not mention it.** A reader facing `c_keeps_text(s: cstr)` — a header they cannot
change — still reaches for the lease, because the lease is the document's own
answer to "C wants my bytes for longer than a call". V1 produces no diagnostic on
that path. Program B under V1 is program B.

**V1 result: 2 of 2 written, 2 of 2 required inventing a C header the task did
not give me, 0 diagnostics on the original programs.**

### V2 — program C

```
function main()
    s: Sl @ Sl(name: [72, 2, 3, 4, 5, 6, 7, 8], id: 1)
    c_keeps(p: s.name.ptr(), n: 8)
    print(to_str(c_reads_later()))
    s @ Sl(name: [1, 2, 3, 4, 5, 6, 7, 8], id: 1)     # error, if the lend outlives the call
```

and the repair is the Task-3 C program above, with `t` instead of the write.
**This is the only place in the whole experiment where a wrong program stopped
being a wrong program instead of a wrong answer.** It is worth a lot.

It is worth a lot **only under one of the two readings of the sentence**. "while
the lend can still be read":

- *reading (i)* the lend is readable until the callee returns → line 5 is after
  the return → **legal** → V2 closes nothing and program C keeps printing 1;
- *reading (ii)* the lend is readable for the rest of the binding's life → line
  5 is **a compile error** → V2 closes C.

Nothing in the document chooses. The word "lends" and "A lend and a lease name
stand only as an argument of a call" both lean toward (i), which is the reading
that makes V2 inert. I wrote the program twice and could not tell which one the
compiler would accept.

V2 is silent on A (no write happens; the frame just ends) and silent on B (no
field lend exists).

**V2 result: 1 of 3 programs improved, and that one contingent on an undefined
word.**

### Scoreboard

| | A | B | C |
|---|---|---|---|
| today: compiles | yes | yes | yes |
| today: right answer | no | no | no |
| today: any message | none | none | none |
| today: can I write the correct one from the document? | **no** | **yes** | writable, **not justifiable** |
| V1: diagnostic on the program as given | none | none | none |
| V1: correct version writable | only by rewriting the C header | only by rewriting the C header | only by rewriting the C header |
| V2: diagnostic on the program as given | none | none | **yes, under reading (ii) only** |

First-try counts: **9 programs written** (3 today, 2 under V1, 3 under V2, 1
extra route for A). **Programs I could write and defend with a citation: 1** (B's
repair). **Programs I could write but not defend: 4.** **Programs I could not
write at all: 1** (A's real repair — bytes that outlive the frame through a
`ptr` parameter).

---

## hesitation_points

Ordered by what a wrong guess costs. The good news first: most of § 13's
hesitations fail loudly. The two that fail silently are both about extent.

**Silently different program (the enemy):**

1. **How long may C read what `f.ptr()` handed it?** No sentence. I guessed "the
   call". A reader who guesses "as long as the binding lives" writes program A
   and gets a dead frame; a reader who guesses "forever, it is a pointer" writes
   program C. Both compile. **This is the whole sitting.**
2. **What does a write to a lent binding do to bytes C holds?** No sentence, and
   § 3's no-aliasing promise points the wrong way. Wrong guess = program C,
   compiles, prints a number the program never handed over.
3. **"for as long as the program says" — who says, and where?** I took it to mean
   "until your `end_lease` call", forced by the following clause. The competing
   reading is "the lease lives as long as its binding's scope and `end_lease` is
   tidy-up". Under the second reading a reader places `end_lease` immediately
   after the call, which is program B. Note the pressure the document applies:
   "a lease nobody ends … aborts when `main` returns, saying how many" tells the
   reader they **must** end it, and the nearest safe-looking place is right after
   the call that used it. **The abort rule pushes the reader into the bug.**
4. **Under V2, which reading of "can still be read".** (i) makes my repair
   unnecessary and the original legal; (ii) makes the original an error.

**Compile error (the win) — I list these because they are the design working:**

5. `_ = c_keeps(…)` vs a bare call: § 5 settles it and the wrong one errors.
6. `counted_by n` — is `n` a sibling **parameter** or a sibling **field**? The
   prose says "naming the sibling", and C's own `counted_by` is a struct-member
   attribute, so the field reading is live. The `Fields` production has no slot
   for it, so the wrong guess will not parse. Loud.
7. `n: 9` against `name: u8[8]` — "one past the field is refused". Loud.
8. A `[T]` field in a group record — "never a `[T]`". Loud.
9. `consumes` with or without `@` — the sqlite example shows `db: Db consumes`
   with no `@`, and "mark the parameter `@` and the value does not survive the
   call" reads as an extra thing I may do, not a thing I must. I guessed no `@`.
   Whether using `k` after `c_drop(k: k)` is a compile error or the run-time
   "giving one back twice aborts" is not settled by the document — this one is
   half-silent.
10. V1 says "`acquires` names the call that begins its life and `consumes` the
    one that ends it", while today's § 13 says `acquires <ident>` names the
    ender. A reader taking V1 literally may write bare `acquires`; `Member`
    requires the ident, so it will not parse. Loud, but it is a contradiction
    inside one section.

**What I had to invent because the document does not say it:**

- the whole header binding (unavoidable — the brief gave bodies only);
- whether `Sl` is a group record or an ordinary Heroes record, and hence whether
  `name` is `u8[8]` or `[u8]`. **The document never says whether `f.ptr()` may
  lend a field of an ordinary Heroes record**, or a `[T]` field of one. I chose
  the group record because `n: 8` and "one past the field is refused" only make
  sense against a declared extent;
- whether a group function may **return a group record by value** (`mk() -> Sl`);
- whether `end_lease` and `.lease()` are usable outside a group. Neither appears
  in § 11's `Built-ins:` sentence, so § 13's prose is their only introduction;
- whether `.lease()` may be applied to a string literal rather than a `str` name;
- whether a lease may be handed to two calls, and whether the name is readable
  after `end_lease` "empties the cell" (does the cell hold `nullptr`? "empties"
  is the only word);
- for V1, an entire second C API per program.

---

## argument

Three programs, nine writes, one repair I could defend. The document prices
`lease` precisely and never prices a lend at all, so **A and C are not reader
errors, they are questions the document declines to answer**. V1 answers it in a
subordinate clause and spends its main clause instructing the C library's author;
its other two clauses restate § 13's handle definition and its abort rule, giving
one rule two homes. V2 is the only candidate that converts a silent wrong answer
into a compile error, and it converts exactly one, on a trigger whose meaning it
leaves to the reader — the same gap that made C possible. Fix the extent and V2
becomes approvable; state the extent alone and half the damage goes anyway.

*(118 words)*

---

## the third sentence

Both candidates presuppose the extent. Say it, and attach the keeping case to the
signature, where a reader can see it without leaving the line:

> **T.** *A lend lives for the call and no longer: `s.cstr()` and `f.ptr()` are
> readable until the callee returns, and writing the binding a lend names before
> then is a compile error naming both lines. A parameter the callee keeps past
> the call is marked `keeps end_fn`, naming the call that gives the bytes back:
> it refuses a lend, takes a lease, and that lease is ended by `end_fn` and not
> by `end_lease`.*

What it does to the three programs:

- **A** — `c_keeps`'s header is bound `p: ptr counted_by n keeps c_forget`, so
  `s.name.ptr()` is refused at the call: *a `keeps` parameter takes a lease, not
  a lend*. The dead frame becomes a compile error. It also names the route out,
  which today's document does not have for a `ptr` parameter — that gap must be
  closed too, by letting a lease reach a `ptr counted_by` parameter, or T only
  makes the hole visible instead of fixing it.
- **B** — `end_lease(@label)` on a lease handed to `keeps c_forget` is refused,
  naming `c_forget`. The read-after-free becomes a compile error. The abort rule
  no longer pushes the reader into it, because the reader is told which call ends
  this one.
- **C** — the write to `s` while the lend is live is a compile error, which is
  V2's contribution, kept.

Every clause is decidable from the line plus the signature it calls. `keeps
end_fn` reuses `acquires end_fn`'s exact shape, so a reader who has met handles
has already met it — one shape, twice, is cheaper to read than two shapes. And
it costs the reader nothing to ignore: a header with no `keeps` behaves as today.

**The cheap half, if T is too much.** The first clause alone — *a lend lives for
the call and no longer, and writing the binding it names before then is a compile
error naming both lines* — is V2 with its hole filled, closes C, and makes A's
and B's wrongness at least **derivable** by a careful reader rather than
unknowable. I would take that over either candidate as written.

---

## prediction

Falsifiable, with numbers, on the three tasks as the brief states them. Twenty
generations per cell from a fresh model given only `spec/heroes-spec.md`:

| | today | V1 added | V2 added | T added |
|---|---|---|---|---|
| **A** compiles-and-wrong | 16/20 or more | 14/20 or more | 16/20 or more (no change, ±2) | 4/20 or fewer |
| **B** compiles-and-wrong | 10/20 or more | 10/20 or more (no change, ±2) | 10/20 or more (no change, ±2) | 3/20 or fewer |
| **C** compiles-and-wrong | 16/20 or more | 12/20 or more | **3/20 or fewer** under reading (ii); **no change** under reading (i) | 3/20 or fewer |

The three sharpest, each cheap to run:

1. **V1 moves no cell by more than 4/20 unless the model is also allowed to write
   the C header.** Where the header is given and fixed, V1's delta on all three
   is zero within sampling error, because it emits no diagnostic.
2. **V2's entire effect is one cell**, and it is zero everywhere else. If V2 is
   adopted and a later harness shows A or B improving, the improvement came from
   something other than V2's sentence.
3. **The extent question, answerable today with no code.** Ask twenty fresh
   models, against today's § 13: *how long may C read what `f.ptr()` handed it?*
   I predict **8 or more of 20** answer something other than "until the call
   returns" — most often "until the binding goes out of scope" — because no
   sentence says, and § 3's no-aliasing promise leans the other way. If fewer
   than 4 of 20 diverge, I am wrong that the gap is load-bearing, and V2's
   undefined trigger is a smaller problem than I claim.

---

## condition

- **I withdraw the objection to V2** if it is amended to state the extent in the
  same sentence (reading (ii) written out: the lend borrows the binding **for the
  rest of the binding's life**, not for the call), or if a measurement shows
  prediction 3 landing under 4/20 — in which case readers already share the
  model V2 presupposes and the word is safe.
- **I withdraw the objection to V1** if it is cut to its first clause (*lends
  live for the call and no longer*), which is the sentence the document is
  missing, and its two restatements of § 13 are dropped. As a whole sentence I do
  not withdraw it: it instructs the wrong author.
- **I would move to VETO on V1** if it is adopted **while `lease` stays in the
  document unmodified**, and a measured run shows program B's rate unmoved. Two
  answers to one question, one of which the document does not connect to the
  other, is a reader holding two rules in mind to write one line.

---

## veto, deliberately not exercised

I hold a veto on non-local constructs and I am not using it here, for the record
and its reason.

V2's diagnostic is reported at two lines in one function body and names both.
The reader does not have to open the extern group to know they broke the rule:
the lend is spelled `.ptr()` on the earlier line, and the write is spelled `@` on
the later one. That is local.

The construct that **would** earn a veto is the one V1 gestures at without
naming: a rule whose trigger is *whether this C function keeps what it is
handed*. That fact is in the header, which is not the enclosing signature and is
frequently not even in this file. If any resolution makes correctness depend on
that fact **without writing it into the `extern` declaration**, I veto it. T's
`keeps end_fn` exists precisely to write it there.

**One process note, offered as a finding rather than a complaint.** Three times
in this experiment I wanted to check something against the repository — whether
`f.ptr()` accepts an ordinary record's field, whether a lease can reach a `ptr`
parameter, whether `end_lease` is a real built-in. I did not look. Each of those
is a question a reader of the document will also have, and will answer by
guessing. Two of the three guesses compile.
