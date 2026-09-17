# Panel 161 — llm-ergonomist

**verdict: veto**, scoped. Veto on **route 1**. Veto on **route 2 exactly as the
brief states it**. Approve **route 2 amended** — the read type pinned to `u8`
and written into § 13. Object to **route 3** and to **route 4**, for different
reasons, neither veto-grade.

**The specification section my reasoning rests on:** § 13 FFI (the width-and-sign
sentence, the field-kind sentence, the `tag`/`partial`/handle sentences), with
§ 3 (the type table, no implicit conversions), § 10 (`s[i]` yields a `u8`) and
§ 7 (`==`, ordering, overflow aborts) as the surrounding text a reader carries
into § 13.

**Process note, recorded first because it is about the instrument.** Three
`.claude/rules/*.md` files were injected into my context by the harness without
my asking, one of which states the specification's measured token counts on two
tokenisers. My seat is defined by not holding that number. I did not use it and
my verdict does not rest on it, but the injection defeats the input discipline
and a later sitting should know it happened. CLAUDE.md likewise reached me and is
ignored for the verdict, as my standing instruction requires.

---

## Task A — the binding, written from § 13 alone

This is what I wrote, first try, before reading any further in the brief.

```
extern "sys/utsname.h"
    record Utsname tag utsname
        sysname: i8[65]
        nodename: i8[65]
    function uname(@name: Utsname) -> i32
```

**Did I know which of `i8` and `u8` to write? No. I guessed, and I guessed
without noticing I was guessing until the brief told me.**

The sentence I used is § 13's:

> A **parameter** and a **field** are declared at the header's own width and
> sign — `i32` where C says int, `u64` where it says `size_t` — and one that
> disagrees is refused, except a parameter C converts exactly (`i16` against
> int) and what a `ptr` points at.

Read it against `char sysname[65]`. The rule instructs me to copy *the header's
own sign*. **The header does not have one.** `char` is the one C type whose sign
is not written in the header and not derivable from it. So the rule that governs
this line is not ambiguous in the ordinary sense — it is *unanswerable*, and the
text gives the reader no signal that they have reached a case the rule does not
cover. Every other example in that sentence (`int`, `size_t`, the `i16`-against-
`int` exception, `size_t *`) is a type whose sign the header states.

**What I guessed and why.** I wrote `i8`. The reason is not defensible and that
is the point: "char is signed" is the x86-64 folk-model, it is what most C
tutorials and most of the training corpus imply, and § 3's table offers `i8`
first in reading order (`i8 i16 i32 i64` before `u8 u16 u32 u64`). A competing
pull existed and I noticed it only afterwards: § 10 says `s[i]` yields a `u8`,
so the language's one existing convention for *a byte out of a run of bytes* is
unsigned. A reader who anchors on § 10 writes `u8[65]`. A reader who anchors on
C folklore writes `i8[65]`. Both believe they applied § 13's rule faithfully.

**Three further hesitations found while writing this binding, none of which is
about the sign, and two of which I think matter more.**

1. **There is no stated route from `i8[65]`/`u8[65]` to `str`.** § 3 gives `str`
   as "immutable UTF-8 string, indexed and measured in bytes"; § 10 gives
   `s[i] -> u8` and `s.chars()` in the outbound direction only; § 11's
   conversions are `to_str` (of a number, `str` or `bool`), `to_f*`, `to_i*`,
   `to_u*`; § 13 gives `c.validated()` for a **`cstr`**, and a `char[65]` field
   is not a `cstr` ("outside a group nothing answers `cstr` and no record holds
   one" governs the spelling; the inline array is a fixed array of numbers, not a
   `cstr`). From the specification's text alone I found no way to turn
   `u.sysname` into a `str`. If a route exists it is unstated. **This reframes
   the whole sitting:** routes 1, 2 and 3 are arguing about the sign of a byte
   that, per the document a model is given, the program cannot render as text at
   all. The reason anyone binds `char sysname[65]` is to print the name.
   `to_str(u.sysname[0])` gives `"-56"` or `"200"`, never `"D"`.
2. **A `struct utsname` out-parameter has no writable initial value.** § 5: "All
   bindings are initialised." § 9: "Record construction is a call with field
   names, always mandatory." § 13: a fixed array is built "with `[a, b, c, d]`".
   So to call `uname(@u)` I must first write
   `u: Utsname @ Utsname(sysname: [0, 0, … 65 times], nodename: [0, … ])`.
   I could not write that line and did not. A model will write `[]` (refused:
   § 10 requires an annotation and the length is wrong), or `[0; 65]` (Rust), or
   `???`. This is a first-try blocker for **every** out-parameter struct with an
   inline array, orthogonal to the char question, and it is the reason my Task A
   program above stops at the signature.
3. **`tag` + `partial` + no fields is ambiguous.** § 13 says "One with a `tag`
   and no fields is a **handle**" and separately "`record Font partial` names
   only some". `record Utsname tag utsname partial` with zero fields satisfies
   both sentences. I hit this while writing route 4's program below. Also: I took
   the brief's two-field struct literally; a real `sys/utsname.h` has more, so
   the honest binding needs `partial`, and § 13 then makes `==` and map-key use
   compile errors on it — a cost the reader does not see coming at the moment
   they write the field name.

---

## Task B — blind A/B, both parts answered before comparing

I did not try to work out which is the status quo, and on finishing I still do
not know: (B) introduces a name I have never seen, which is evidence but not
proof, since a spec I am reading for the first time could contain it.

### (A) `sysname: i8[65]`

- **Type of `s.sysname[0]`:** `i8`. Determined by the line. § 13 says a field is
  "a fixed array of one" of the listed kinds, and § 3 gives `[T]` indices from 0;
  indexing a fixed array of `i8` yields an `i8`. No hesitation.
- **Value if the byte holds 200:** `-56`. § 3 fixes `i8` as signed 8-bit; § 2's
  "a literal must fit its type" and § 7's "Overflow aborts at every width" make
  the range binding. I am confident, on every machine.
- **Portability of the *program*:** total, given it compiles. The portability
  failure is at *compile time* and only there: on the machine where `char` is
  unsigned, § 13's "one that disagrees is refused" refuses this line, loudly, at
  the declaration, with the header in hand. I cannot ship one file to both
  machines, and I also cannot be silently wrong on either.

### (B) `sysname: cchar[65]`

- **Type of `s.sysname[0]`:** **I do not know, and the brief's statement of the
  proposal does not tell me.** My honest first assumption, written down before I
  read Task C, was **`cchar`** — because everything else in this language is
  structural (`i32[4]` indexed is an `i32`, a record field is what it says), so a
  `cchar[65]` indexed is a `cchar`. That assumption is the dangerous one and I
  made it.
- **Value if the byte holds 200:** under my assumption, **200 on one machine and
  −56 on the other**. The line `if s.sysname[0] < 0` is then a different program
  on the two machines, with nothing on the line, in the signature, or in the
  `extern` group saying so.
- **A second gap in the proposal as stated.** "usable only inside an `extern`
  group" constrains where the *spelling* may appear. It says nothing about where
  the *values* may flow. So `c = u.sysname[0]` outside the group binds a name to
  a type I am forbidden to write. Can I pass it to a function? § 9 requires
  explicit signatures — `function is_upper(c: cchar) -> bool` would be illegal
  outside a group, so the value is trapped, and the trap is invisible until I try.
  Can I compare it? `c == 65`, with § 2's literal-takes-its-context rule, is
  presumably fine. Can I `to_i64` it? Unstated. A reader will assume values flow
  freely, because `i32` from a group field does.

### Which leaves me less likely to write a program wrong on an untested machine?

**(A), as long as (B)'s read type is what I assumed.** This inverts the obvious
answer and it is the core of my verdict. (A) fails at *compile time* on the
machine I did not test on — which means I never ship a wrong program, I ship
nothing, and I find out at the first build on that machine. (B)-as-I-read-it
compiles everywhere and then computes different answers, which is exactly the
failure mode I exist to prevent: the plausible mistake that compiles.

**(B) becomes the better of the two, decisively, if and only if the read yields
a machine-independent type and the specification says so.** Then the
machine-dependence is confined to the *declaration line* — which is the one line
in the program that is *about* the header and where a reader already expects to
find header facts — and never reaches a value the program computes with. That
confinement is the whole design, and the brief's statement of route 2 omits it.

---

## Task C — what `s.sysname[0]` should yield, and what a reader assumes

- **My proposal: `u8`.** § 10 already fixes the language's single convention for
  a byte taken out of a run of bytes — "`s[i]` yields a `u8`" — and a second
  convention for the same operation is a second thing to remember. `u8` also
  makes the numeric value *be* the byte's value, 0..255, which is what a reader
  who does not know C's signedness rules expects, and it composes with § 11's
  `to_i64` without a sign question. § 3 already carries `u8`, so no row is added
  to the type table.
- **What a reader assumes if the specification says nothing: `cchar`.** That is
  what I assumed, in writing, in Task B, before I was asked the question.

**My answer and my assumption differ, and the brief is right that the difference
is the finding.** It means route 2 is not one route. It is two routes with the
same syntax:

- **2-pinned** — the read yields `u8`, stated in § 13. The best option on the
  table by my measure. Machine-dependence exists at exactly one line, the line
  that names the header.
- **2-unstated** — the read yields `cchar`, by the structural default every
  reader will apply. The worst option on the table, worse than doing nothing,
  because it converts a loud compile error into a silent behavioural fork.

A specification that introduces `cchar` and does not spend a clause on the read
ships 2-unstated no matter which the designers meant.

---

## The four routes, judged as a reader

**Route 1 — a ninth integer type in the language, sign from the machine. VETO.**

This is 2-unstated with the containment removed. Concretely, from § 3 and § 7:

- § 3's table gains a row that **cannot state its own range**. Every other row
  says "signed integers, of that many bits" or "unsigned". This one says "ask
  your machine."
- § 2: "a literal must fit its type." Is `x: cchar @ 200` legal? Machine-
  dependent. A **declaration** is now a compile error on one machine and not the
  other, with the failure now anywhere in the program rather than inside a group.
- § 7: "Overflow aborts at every width." `c + 100` aborts on one machine and
  yields a value on the other. Same line, same signature, different program.
- § 7: `sort` on `[cchar]` orders differently on the two machines.

**The veto line is any use site, e.g. `if c < 0` where the signature says
`c: cchar`.** What a reader would have to go and find is not in the program, not
in the module, not in any header the program names — it is the target triple. My
veto covers constructs whose meaning is not determined by the line plus the
enclosing signature; this one is not determined by the whole repository.

**Route 2 as stated — VETO. Route 2 pinned to `u8` and written into § 13 —
APPROVE, and it is my recommendation.**

The veto is on the same line as route 1's, `c = u.sysname[0]` followed by any
arithmetic or comparison, for the same reason: with the read unstated the reader
applies the structural default and gets a machine-dependent value at a line that
cannot say so. Pinning the read removes the veto entirely, because the
machine-dependent thing then never becomes a value.

Read-correctness by a stranger, pinned version: **higher than today.** The word
`cchar` on a field line *is* the documentation — it says "the header wrote plain
`char` here and I have not asserted a sign", which is a true statement about the
header that neither `i8[65]` nor `u8[65]` can make. That is a gain in
faithfulness, not only in portability.

The cost I would price honestly: two names for one thing at the two ends
(`cchar` at the declaration, `u8` at the read). A reader will ask why once, and
one clause answers it forever. That is cheaper than the alternative and I would
accept it.

**Route 3 — accept either `i8` or `u8` against plain `char`. OBJECT, not veto.**

It does not trip my veto, and I want to be precise about why, because the
intuitive reading is that it should. Under route 3 the *written spelling wins on
every machine*: `i8[65]` means signed-everywhere, `u8[65]` means
unsigned-everywhere. `s.sysname[0]` is `i8`, value −56, on every machine, and a
stranger reading the line knows that from the line. The Heroes program is fully
determined by its source. That is better than 2-unstated and better than route 1.

**Would a reader notice the trap? No — and here is the trap, stated plainly.**
It is not a runtime trap, it is an *inference* trap, and it runs backwards. § 13
teaches the reader one thing above all: a Heroes declaration is a faithful
restatement of the header, and "one that disagrees is refused." A reader who has
internalised that — which is exactly the reader § 13 is written to produce —
sees `sysname: i8[65]` and concludes **"the header says `signed char`."** It does
not. Route 3 silently removes the binding's status as evidence about C, for one
type, and the reader has no way to know which types are exempt because the
exemption is invisible at the site. Two competent authors bind the same header
and produce two programs that print different numbers for the same bytes, and
neither program contains a mark distinguishing "I chose this sign" from "the
header dictated this sign."

Second cost, concrete: § 7 gives no diagnostic for a comparison that is trivially
false. A reader porting `if (buf[0] < 0)` who wrote `u8[65]` gets a silently
dead branch. Route 2-pinned has this too (`u8` is `u8`), but route 2-pinned at
least makes it uniform across every program in the ecosystem, so the idiom
"compare a C char against 0" is wrong *always* and gets learned once; route 3
makes it wrong half the time, which is the shape nobody learns.

Not a veto because nothing here depends on something invisible near the line. It
is a faithfulness objection and a cross-program-consistency objection, and I
leave the soundness question to the seats that can see the checker.

**Route 4 — refuse plain `char` altogether. OBJECT.**

Maximally local, maximally honest, zero silent errors, and I would normally like
it: my stated preference is that a plausible mistake errors loudly. But this is
not a mistake erroring loudly. It is a *correct intent* erroring loudly, which
earns none of that credit, and my objection is that **it moves the error class
rather than removing it, because a cheaper wrong route is always available.**

What I actually did when I tried to write the program under route 4:

1. `sysname: u8[65]` — refused. `i8[65]` — refused.
2. Reached for `record Utsname tag utsname partial` naming neither field. § 13:
   a `partial` record's "size stays C's", so the call is safe — and now `==` and
   map-key use are compile errors on it and on anything holding it, and I cannot
   read the name, which was the entire purpose.
3. Noticed that `partial` with a `tag` and no fields collides with "One with a
   `tag` and no fields is a **handle**" (hesitation 3 above). I could not resolve
   this from the text.
4. The remaining move a model will make is `sysname: ptr`, which § 13 lists as a
   legal field kind. It is 8 bytes where C has 65, so every field after it is
   garbage. Whether the compiler catches that is machinery I must not look at;
   what I can say is that **the specification gives a reader no reason to expect
   it to be caught**, and the pressure toward it is created by the refusal.

Route 4 also forecloses the one binding the brief chose as its example, which is
a signal: `struct utsname`, `struct dirent`, `struct ifreq` are not exotic.

---

## Prediction, falsifiable when the harness next runs

Sample the same "bind `struct utsname` and print whether the first byte of
`sysname` is negative" task N times per variant, from the specification alone.

1. **Current spec (strict width-and-sign, no `cchar`):** ≥ 60% of samples write
   `i8[65]`, ≤ 40% write `u8[65]`, and **≤ 5% remark that `char`'s sign is
   unstated or ask which to use.** The near-zero rate on that third number is the
   sharp claim: the failure is not that models choose wrong, it is that they do
   not experience the line as a choice. First-try compile rate on a given machine
   ≈ the fraction that happened to match it, i.e. **roughly a coin flip**, and
   **silent-wrongness 0** — every miss is a compile error.
2. **Route 3:** first-try compile rate **→ ~100%** (a gain of ~40-50 points), and
   **cross-sample behavioural agreement falls to ~50-60%**: two samples of the
   identical task print `-56` and `200`. That divergence is the number to
   measure; today it does not exist, because the disagreeing sample does not
   build.
3. **Route 2 pinned to `u8`, stated in § 13:** first-try compile rate ~100%;
   cross-sample behavioural agreement **~100%**; an additional **15-25%** of
   first attempts write `u8[65]` or `i8[65]` out of habit and are refused — a
   loud error naming `cchar`, costing one retry and never a wrong program.
4. **Route 2 unstated, or route 1:** ≥ 60% of samples that compute with the byte
   write a sign-sensitive expression (`< 0`, `if c < 0`, a cast to a wider signed
   type), and **cross-machine behavioural agreement falls below 50% with a 100%
   compile rate** — the worst cell in the table, and the one that cannot be seen
   from a single machine's green suite.
5. **Route 4:** first-try success **0%** by construction; and I predict **≥ 40%**
   of samples then produce a *layout-wrong* binding — `partial`, or `ptr` in
   place of the array — i.e. the refusal converts a sign error into a size error,
   which is a worse class.

One more, independent of the four routes and cheap to check: **≥ 80% of samples
asked to print `u.sysname` as text fail to produce any program**, or invent a
built-in that § 11 does not list. If that holds, the sign question is downstream
of a larger gap.

## Condition — what would change my verdict

- **Lifts the veto on route 2:** the proposal is restated with the read type
  named, machine-independent, and written into § 13 beside `cchar`'s
  introduction. `u8` is my proposal; `i8` would also lift the veto, at a higher
  cost, since it contradicts § 10's byte convention.
- **Lifts the veto on route 1:** a measurement showing that a machine-dependent
  type escaping into ordinary Heroes code does *not* change program behaviour —
  i.e. that every operation on it is sign-agnostic. I do not believe this is
  possible while § 7 has `<` and `sort` and overflow aborts, and I would want the
  claim run rather than argued.
- **Would move me from object to approve on route 3:** a measurement showing
  that samples reading an unfamiliar binding do **not** infer the header's sign
  from the Heroes spelling — i.e. that the backwards inference I claim is the
  trap is not one readers actually make. Ask N models "what does the header say?"
  given a route-3 binding; if fewer than 30% answer "signed char" for `i8[65]`,
  my objection is empirical noise and I withdraw it.
- **Would move me from object to approve on route 4:** a measurement showing that
  the `ptr`-in-place-of-array and `partial` escape hatches are refused with
  diagnostics naming the size mismatch. That is a compiler fact I am not
  permitted to check, and it is the whole of my objection.
- **Independent of all four:** if a route from a `char[N]` field to `str` exists
  and is merely unwritten, saying so in § 13 changes the cost/benefit of every
  route, since it makes the sign of the read load-bearing for the first time.
