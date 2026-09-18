# Panel 163 — llm-ergonomist

**Input discipline.** My inputs were `spec/heroes-spec.md` and
`docs/panel/163-briefs/llm-ergonomist.md`. My context was pre-loaded, without my
asking, with `CLAUDE.md` and three files from `.claude/rules/` (`spec-shape.md`,
`verification.md`, `records.md`). I refused them: nothing below rests on any of
them, and in particular I assert no token count and no budget, because I am the
one seat that cannot check one. I did not open design.md, the compiler,
`00-shared.md` or the other briefs.

Tasks A, B and C are answered first and in order. The opinion is at the end.

---

## Task A — write the line

The binding, which I wrote first because nothing else can be written without it:

```
extern "sys/utsname.h"
    record Utsname tag utsname
        sysname: i8[256]
        nodename: i8[256]
        release: i8[256]
        version: i8[256]
        machine: i8[256]
    function uname(@name: Utsname) -> i32
```

### The routes I attempted, in the order I attempted them, and the sentence that refused each

**Route 1 — declare the cell and let the callee fill it, as in C.**
I wrote `u: Utsname` and stopped inside a second. § 5's production has no such
statement: `Statement = ident ":" Type ( "@" | "=" ) Expression NEWLINE`. There
is no form with a type and no expression. And the prose above it: *All bindings
are initialised.* Refused by the grammar and the sentence together.

**Route 2 — `repeat(0, 256)`.** This was my first real attempt, and I was fairly
confident for about ten seconds. Refused twice over:

- § 11 lists `repeat(s, n)`, and § 10's prose puts it in the string paragraph:
  *"`+` on `str` copies both sides — a concatenation loop is quadratic; `join`
  and `repeat` build in one pass."* The parameter is named `s`. `repeat` is a
  `str` builder.
- Even granting a generic `repeat`, its result is a `[T]`, and § 13 says a field
  is *"a fixed array of one: `i32[4]`, **never a `[T]`**"*. `[i8]` and `i8[256]`
  are different types under § 3's `Type = Prefix { "[" integer "]" }`.

**Route 3 — `Utsname()`, hoping for a zero default.** Refused by § 9: *"Record
construction is a call with field names, always mandatory"*, and by the same
section's *"no default values"*. § 5's *All bindings are initialised* closes the
door from the other side. I note that neither sentence literally says "every
field must be named", so this route dies on a reading rather than on a
statement — see hesitation H2.

**Route 4 — build the array in a loop.** `a: i8[256] @ …` then `for i in
range(from: 0, to: 256)` / `a[i] @ 0`. Dead at the declaration: the `@` line
still needs an initialising expression, which is the problem I was trying to
solve. Abandoned in a second.

**Route 5 — a helper, `function zeros() -> i8[256]`.** § 3's grammar lets me
write that return type. The body still has to produce the value, so this moves
the 256 zeros rather than removing them — but it does let me write them **once**
instead of five times. I kept this and it is in the program below. It is a
workaround for a token cost, not for the semantic problem.

**Route 6 — `partial`.** § 13: *"`record Font partial` names only some"*. So
declare `record Utsname tag utsname partial` with only `machine`, and build one
literal instead of five. Then I stopped, because I could not answer the question
this raises: **what do the fields I did not declare hold?** The program cannot
name them; C's `struct utsname` has them; something must be in them when
`uname` writes. The document does not say. Abandoned as unanswerable from the
text — and see Finding F1, because this is the sitting's question already
present in the document.

**Route 7 — `???`.** § 12: *"`???` is a valid expression anywhere… A program
with holes type-checks everything else but produces no binary."* This is what I
would actually do at a keyboard: write `u: Utsname @ ???` and let the compiler
tell me what belongs there. It is not a program. Refused by *"produces no
binary"*.

**Route 8 — never build one; get it from the library.** The header declares no
constructor for `struct utsname`. Refused by the world rather than by a
sentence. I record that this is route four of the brief's four, and that it does
not merely make this task harder — it makes it impossible, because the only
producer of a `struct utsname` is `uname` itself, which requires one first.

### The program, under the current spec

```
extern "sys/utsname.h"
    record Utsname tag utsname
        sysname: i8[256]
        nodename: i8[256]
        release: i8[256]
        version: i8[256]
        machine: i8[256]
    function uname(@name: Utsname) -> i32

function zeros256() -> i8[256]
    return [
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ]

function main()
    u: Utsname @ Utsname(
        sysname: zeros256(),
        nodename: zeros256(),
        release: zeros256(),
        version: zeros256(),
        machine: zeros256())
    rc = uname(@u)
    if rc != 0
        print("uname failed")
        exit(1)
    match u.machine.validated_bytes()
        .ok m  => print(m)
        .err e => print(e.code)
```

**Yes, I wrote 256 zeros by hand, and here is what I thought while doing it.**
Sixteen rows of sixteen, because that is the only layout in which I can
*audit* the count — I cannot look at a run of 256 zeros and see 256. I counted
the row once (`0` ×16, checked by counting to sixteen aloud) and then counted
rows (sixteen). I did **not** verify the total by counting the zeros; I verified
16×16 and trusted multiplication. That is the whole finding in one sentence:
**the only way I could get this right was to stop writing Heroes and start doing
arithmetic about the shape of my own source text.** Halfway through I noticed I
was about to write this four more times inline, which is why route 5 exists in
the program above; the helper is not elegance, it is fear.

A second thing I thought: **nobody will ever read this.** A reviewer opening
`zeros256` sees a block of zeros and takes my word for the count. They cannot do
otherwise. So the most explicit possible spelling of this value is also the one
that conveys the least to a reader, and I had that thought while writing it, not
afterwards.

---

## Task B — the sentence I looked for

**Where I went first: § 5, Bindings.** Not § 13. The question in my head was not
"how does the FFI work", it was "how do I make a box for someone else to fill",
and § 5 is the section about making boxes. I read *All bindings are initialised*
and went looking for its exception, because every language that says this has
one (Go's zero value, C's `= {0}`, Rust's `MaybeUninit`, Ada's `others =>`). §
5 has no exception and does not say it has none.

**Then § 10**, for the container literal, expecting a repetition form. § 10 says
*"A container literal separates elements by newline across lines and by comma on
one, and an empty one needs an annotation"* — three facts about literals, none
of them repetition.

**Then § 11**, where I found `repeat(s, n)` and briefly believed the problem was
solved.

**Then § 13**, and this is where I stopped looking and started counting.

**The three forms I reached for and did not find, in order of how hard I reached:**

1. **A repetition array literal** — `[0; 256]`, `[0] * 256`, `(others => 0)`.
   This is the one I expected hardest, and § 11's `repeat` is a **name collision
   with it**: a reader who half-remembers the language will write `repeat(0,
   256)` because the name is the name of the thing they want. That is not a
   hypothetical; it is route 2 above, which I wrote before I wrote anything else.
2. **A zero of a type** — `Utsname()`, `zero(Utsname)`, `Utsname.zero`.
3. **An out-parameter that declares its own cell** — `uname(@u: Utsname)`.

**And the sentence I found that almost answers it, which is the important one.**
§ 13, of an `owned` out-parameter: *"the compiler frees that string with that
function, hands it over as a `str?` **(the `@` cell is only written)**"*. The
document already knows the concept "a cell whose incoming value nobody reads".
It states it in a parenthesis and then still requires the caller to produce an
incoming value anyway.

**Finding F0 — the asymmetry, which I think is the actual defect.** The document
has three kinds of C out-parameter and gives each a different amount of ceremony:

| what C fills | the cell the caller must produce first | cost |
|---|---|---|
| `sqlite3 **` (a handle) | `db: Db @ nullptr` — § 13: *"`nullptr` is its null"* | one word |
| `char **` with `owned` | a `str?` cell — and the document names **no** empty `str?`. `ok("")`? `fail(code: "", msg: "")`? | unknown |
| `struct utsname *` | five 256-element literals | 1280 zeros |

The handle case works because § 13 gave handles a written empty value. Nothing
else in § 13 has one. **That, and not § 5's four words, is what fails here.**

**Finding F1 — § 13's own `partial` example is uncallable today.** The document
writes `record FileStat tag stat partial`. The canonical use of `struct stat` is
`int stat(const char *path, struct stat *buf)` — a caller-owned out-struct,
exactly this task. Under *all bindings are initialised* + *record construction
names every field, always mandatory* + *`partial` names only some*, a `FileStat`
can never be constructed by any program, so `stat` can never be called. Either
the document already intends undeclared `partial` fields to receive something it
does not name, or its own example is decorative. I could not settle this from the
text, and I did not go looking in the compiler. It is a question, not a premise.

---

## Task C — blind A/B, all three answered before comparing

### (A) `u: Utsname @ Utsname(sysname: [0, 0, … 256 times], …)`

**What I believe the fields hold: 256 zero `i8`s each, 1280 zero bytes.**
What tells me: § 13's *"build one with `[a, b, c, d]`"* for a fixed-array field,
and § 2's *"A literal takes the type its context asks for"*, which is what makes
a bracketed literal become `i8[256]` rather than `[i64]`.

**What I could not determine from the document:** whether a literal with the
wrong number of elements is a compile error. § 13's example shows four elements
for `i32[4]`, which implies a match, but **no sentence states the rule**. If a
255-element literal is refused, this route is tedious-but-loud. If it is padded
or truncated, this route is the worst thing in the document. The text does not
say, and I am not permitted to look.

**Readability: the worst of the three.** A reader cannot verify 256 by eye and
will not try. So the most explicit spelling conveys the least checkable
information. It is explicit to the compiler and opaque to the human.

### (B) `u: Utsname @ Utsname(sysname: repeat(0, 256), …)`

**Under the current spec I believe this does not compile**, for the two reasons
in route 2 above: `repeat(s, n)` is a `str` builder by § 10's prose and its own
parameter name, and a `[T]` is refused as a field by § 13's *"never a `[T]`"*.

**If a proposal makes it compile, what I believe the fields hold: 256 zeros** —
and now the reader must reconcile **two numbers written in two places**: the
`256` on this line and the `256` in the record declaration. A reader of the line
alone cannot tell whether they agree.

**The question the proposal must answer and I could not:** what happens when
they disagree, and what happens when the count is not a literal. `repeat(0, n)`
with a runtime `n` is either a compile error, a runtime abort, or a silent
truncation. Only the third is unacceptable, and the document would have to say
which.

### (C) `u: Utsname @ Utsname()`

**Under the current spec I believe this is a compile error**, by § 9's *"with
field names, always mandatory"* and *"no default values"*.

**If a proposal makes it legal, what I believe the fields hold: zeros** — and I
would believe it before being told, from C's `= {0}` and from § 13's own
`validated_bytes`, *"reading to its first zero or the whole field"*, which
already assumes a zero-terminated byte field is the normal case. But belief is
not the document: the proposal must write the word **zero**, because "default"
and "empty" are not the same word and a reader will guess differently for
`f64`, `bool`, `ptr` and a nested record.

**Readability: the best of the three, and this surprised me.** A reader of
`u: Utsname @ Utsname()` who did not write it needs **one** rule to know exactly
what C receives, and that rule is the same rule for every record. A reader of
(A) needs to count to 256 five times and will not. **Explicitness and
readability come apart at 256**, and this is the case that shows it.

### Which leaves a reader least likely to be wrong about what C receives

**(C), then (B), then (A).** Ranked by what a reader must do to be sure:
(C) knows one uniform rule; (B) fetches one number from the declaration; (A)
performs an audit no reader performs. (A) is the only one of the three where a
reader who is wrong has no way of noticing.

---

## Verdict

**veto**, scoped, because a veto anywhere in the option set dominates the
headline:

- **Route 3 — the out-parameter that needs no initial value, so the `u:` line
  disappears: VETO.** Non-local.
- **Route 2 — the zero default, as in (C): APPROVE, with one condition.**
- **Route 1 — a call whose type comes from context, as in (B): OBJECT.** Second
  best, and it fails loudly, but the name `repeat` is already taken by a `str`
  built-in and the length is written twice.
- **Route 4 — refusing: OBJECT.** It does not make this task hard, it makes it
  impossible, and by Finding F1 it retroactively makes § 13's own `FileStat`
  example uncallable.

### The veto, and what a reader must go and find

The line is the one that disappears, and its absence is the construct. Given

```
    rc = uname(@u)
    print(u.machine.validated_bytes().must())
```

a reader of the second line cannot determine, from that line plus the enclosing
signature, whether `u` holds anything. They must scan **backwards through every
path** to find the call that filled it. That is definite assignment — the
analysis this language has never asked anyone to simulate — and once one
construct needs it, a reader must run it mentally on every `@` cell in every
branchy function, because they cannot tell by looking which cells are the
uninitialised kind. It also breaks § 9's stated semantics: *"copy in, copy out"*
has no source for the copy-in.

If instead route 3 means the cell is **born at the call**, `rc = uname(@u:
Utsname)`, the veto is softer but still stands on a second ground: a name
enters scope inside an argument list, and the reader of a later line that uses
`u` must find a declaration that is not at the start of a statement. I would
hear that variant as an objection rather than a veto if it came with the rule
that such a name is scoped to the rest of the block and cannot shadow, which §
5 already forbids.

### What § 5's four words buy a reader today — the price being paid

*All bindings are initialised* is not a safety slogan; it buys four concrete
things, and the fourth is the expensive one:

1. **No "not yet a value" state exists**, so no read can be of garbage, and no
   bug in any Heroes program has the shape "read before write".
2. **The first value of a name is on the line that introduces it.** Locality,
   for free, on every declaration in the language.
3. **Every value is always a legal value of its type**, which is what makes §
   3's *"every value behaves as an independent copy"* and § 7's structural `==`
   total rather than conditional. An uninitialised cell is a value you cannot
   copy or compare, and the document has no vocabulary for one.
4. **No reader and no writer ever performs definite-assignment analysis.** This
   is the one that costs real accuracy. Every language with an uninitialised
   state makes both the writer and the reader trace, per path, whether a
   write happened before a read. Heroes deletes that job outright. For a model
   writing on the first try, the class of bug it deletes is exactly the class a
   model is worst at: the one that lives between two lines rather than on one.

**(A), (B) and (C) all pay for none of this.** Each of them writes a value on
the line; they disagree only about how many characters the value costs. The
route that spends the four words is route 3 alone, and it spends all four of
them to save one line of typing. That trade is what the veto refuses.

The condition on approving (C):

> **Confine the zero default to `extern` records, or give it a spelling that
> cannot be confused with ordinary construction.** If `Utsname()` is legal
> language-wide, then `Point()` is legal, and a model that means `Point(x: 3,
> y: 4)` and writes `Point()` gets a program that compiles and is silently
> wrong. That is the precise thing this language exists to refuse, and (C)
> manufactures it unless it is fenced. Fencing it to `extern` records costs one
> non-locality — you cannot tell from `Utsname()` which group declared
> `Utsname` — but that non-locality **fails as a compile error**, which is the
> good kind and is already how § 13 treats `cstr` (*"outside a group nothing
> answers `cstr`"*).

### A fifth route nobody listed

The four routes all act on the **record**. The thing that is actually missing
acts on the **fixed array**: a written empty value for `i8[256]`, e.g. a
repetition literal, so that `sysname: [0; 256]` is the line. It keeps record
construction naming every field (a reader still sees five names and knows
nothing is hidden), keeps the length **on the line** rather than only in the
declaration, is strictly more local than (B), and does not touch ordinary
records at all, so `Point()` never becomes legal. It costs more tokens at each
call site than (C) and it is the variant I would approve without any condition.
I have no way to price it and I am not claiming it is cheap — only that the
option set handed to me had four members and the world has at least five.

---

## hesitation_points

Every place I guessed while writing Task A's program, with what a wrong guess
produces.

- **H1 — the element count.** No sentence in the document says a fixed-array
  literal's length must equal the field's. I assumed it must. *Wrong guess:
  unknown — compile error if the rule exists, a silently short struct handed to
  C if it does not.* This is the single most important unknown in my report and
  it is unresolvable from the text.
- **H2 — whether construction may omit fields.** § 9 says field names are
  *"always mandatory"*, which literally constrains the **form** of an argument,
  not the **set** of fields. I read it as requiring all fields. *Wrong guess:
  compile error (good) — unless it is not, in which case `Utsname()` already
  works today and the four fields hold something unnamed.*
- **H3 — trailing comma across lines.** § 10 says *"separates elements by
  newline across lines and by comma on one"*, and the production is `{ Sep
  Expression }` with `Sep = "," | NEWLINE` — one separator between two elements.
  So is `0,` NEWLINE `0` two separators and a parse error? I chose no trailing
  comma. *Wrong guess: compile error (good).* But note where it bites: a
  256-element literal is the **first place in the language where anyone meets
  this question at scale**, and I hit it in the first program I wrote.
- **H4 — continuation indentation inside a bracket.** § 1 says indentation is
  *"significant and rigid: exactly 4 spaces per level"*, and the opening
  paragraph exempts bracketed NEWLINEs from ending statements but says nothing
  about INDENT/DEDENT inside brackets. I used one level in. *Wrong guess:
  compile error (good), or a bizarre one.*
- **H5 — `validated_bytes` on `i8[256]`.** § 13 says *"for a field of bytes"*.
  `i8` is signed; "bytes" reads as `u8`. I guessed it applies. *Wrong guess:
  compile error (good) — but with no route forward, since § 13 also demands the
  header's own sign and C's `char` has no portable one.*
- **H6 — `i8` vs `u8` for `char`.** § 13: *"declared at the header's own width
  and sign… one that disagrees is refused"*. C's plain `char` has neither answer
  portably. I took the brief's `i8`. *Wrong guess: compile error (good), and
  possibly an unwinnable loop.*
- **H7 — a fixed array as a function return type.** § 3's grammar permits
  `i8[256]` wherever a `Type` is written, and § 13 only says a *field* may be
  one. So is `function zeros256() -> i8[256]` legal outside a group? I assumed
  yes from the grammar. *Wrong guess: compile error (good) — and then the 256
  zeros must be written five times inline, 1280 of them.*
- **H8 — whether `u` must be `@`.** § 5: *"only a declared `@` name can be
  mutated"*, and § 9's `@` is copy-out. I used `@`. *Wrong guess: compile error
  (good).*
- **H9 — `exit(1)` positional.** § 11 writes `exit(code: i64)`. § 9 makes names
  mandatory only when two parameters share a type. I used positional. *Wrong
  guess: compile error (good).* Minor, and not this sitting's business, but § 11
  writes built-in parameter names in three different notations (`exit(code:
  i64)`, `slice(from:, to:)`, `join(xs, sep)`) and a reader must infer which
  imply mandatory names.

Score: of nine hesitations, **eight fail loudly and one (H1) I cannot classify
from the document.** That ratio is the language working. The problem this
sitting is about is not that the current spec is dangerous — it is that it is
unwritable, and an unwritable form gets replaced by a guess, which is where
the silent errors come from.

---

## argument (≤120 words)

Today the task is unwritable: five 256-element literals, 1280 zeros, none
verifiable by eye. Explicitness and readability come apart at 256. The document
already gives a handle an empty value (`nullptr`) and a struct none; that
asymmetry, not § 5's four words, is what breaks. `Utsname()` keeps the value on
the line and costs one uniform rule, so any reader knows what C receives: zeros.
I veto only the variant where the declaration disappears — then a reader must
trace which call filled the cell, which is definite assignment, the one analysis
this language has never made anyone simulate. Confine the zero default to
`extern` records, or `Point()` becomes a mistake that compiles.

---

## prediction

Falsifiable, checkable when the harness next runs a first-try measurement on the
`uname` task (and on any caller-owned out-struct: `stat`, `getrusage`,
`gettimeofday`).

1. **Current spec.** First-try compile rate **under 10%**. The dominant failure
   is not a miscount: **at least 70% of first attempts will contain either
   `repeat(…)` or a record construction that omits fields**, because a model
   will not emit 1280 zeros — it will invent one of routes 1 or 2 and be wrong.
   Among the minority that do emit literals, **at least 1 in 5 will have a wrong
   element count.**
2. **Route 2 (zero default, confined to `extern`).** First-try compile rate on
   this task **above 80%**. New failure mode: `Point()` written for an ordinary
   record. I predict it appears in **at least 5%** of generated programs that
   construct records — and if it compiles, that is a new silent-wrongness class
   at that rate, which is worse than the problem being solved.
3. **Route 2 unconfined.** Same 80% here, plus that 5% becoming silent. I
   predict a measurable rise in programs whose records are constructed with the
   wrong values and which pass every check.
4. **Route 1 (`repeat` generalised).** First-try rate **60–75%**; length
   mismatches (`repeat(0, 255)`, `repeat(0, PATH_MAX)`) in **at least 15%**, all
   loud if the rule says so; and persistent `repeat`-on-`str` confusion showing
   up as type errors rather than silent bugs.
5. **Route 3.** At least one generated program per ten that reads a cell on a
   path where no call filled it, unless a definite-assignment checker lands. If
   one lands, I predict its diagnostics will need to name a **path**, not a
   line, which is the first such diagnostic in the language.
6. **Route 4 (refusal).** First-try rate stays at zero and does not move,
   because the task has no answer. Checkable today at zero cost: try to call
   `stat` with the document's own `record FileStat tag stat partial`.

---

## condition

What would change my verdict.

- **The veto on route 3 lifts** if the harness measures, on a branchy corpus,
  both a higher first-try rate **and zero** programs that reach an unfilled
  cell, **and** the diagnostic for the refused case names the path that failed
  to fill it from the line the read is on. Absent that last part it stays a
  veto, because the construct is then correct only for writers who can already
  simulate the analysis.
- **My approval of route 2 becomes unconditional** if the spelling cannot be
  confused with ordinary construction, or if `Point()` is measured to be a
  compile error. It becomes an **objection** if `Point()` compiles.
- **My objection to route 1 becomes approval** if `repeat` is left alone and the
  new form is on the array rather than the built-in — i.e. if the sitting lands
  the fifth route, with the length written on the line.
- **H1 decides a great deal and I could not settle it.** If a fixed-array
  literal of the wrong length is **not** a compile error, then route 4 and the
  status quo are both unacceptable on robustness grounds and anything that
  removes hand-counted literals wins on that alone. Somebody who is allowed to
  run a compiler should settle H1 before this sitting resolves; from where I
  sit it is a question, not a premise.
