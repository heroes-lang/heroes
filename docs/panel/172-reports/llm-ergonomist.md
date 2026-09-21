# Panel 172 — llm-ergonomist report

Inputs: `spec/heroes-spec.md` and `docs/panel/172-briefs/llm-ergonomist.md`,
nothing else. Two `.claude/rules/` files were injected into my context by the
harness as process; they were not consulted for anything below.

## verdict

**approve — variant Q, no veto.** P closes the brief's crash only for the
reader who already knows to write `consumes`; Q closes it for the reader who
writes nothing, which is the reader who did not know. P's residual silent class
is *no word or wrong word*; Q's is *wrong word* alone.

## experiment

### Task 1, cold word test

**Today's § 13**, first try, as written before reading either variant:

```
extern "stdlib.h"
    function free(p: ptr consumes)
    function putenv(string: cstr) -> i32

extern "string.h"
    function strdup(s: cstr lent) -> cstr owned free
```

- `free`: I hesitated between unmarked (§ 3: *a function taking one without `@`
  may still change, or free, what C holds*) and `consumes` (§ 13: *the call ends
  that value's life*). I wrote `consumes` because § 13 is the FFI section, but
  I could not tell from the document whether a plain `ptr` has a life the
  compiler tracks. Both lines compile, I believe; the difference is silent.
- `putenv`: I wrote **no word**, because § 13 says *a parameter is taken to
  keep what it is handed unless declared `lent`*, and putenv keeps. My second
  candidate was `consumes` (the environment *takes* the string), rejected only
  because *ends that value's life* is false of putenv. A reader who skips that
  clause writes `consumes`, and today the document does not say whether a
  lease passed to a `consumes` `cstr` is then ended (no `end_lease` owed, leak)
  or not (`end_lease` owed, and the environment holds a freed address). Either
  way it compiles: **silent**.
- `strdup`: `lent`, and `owned free` on the result so I never call `free`
  myself. No hesitation.

**Variant P:**

```
extern "stdlib.h"
    function free(p: ptr consumes)
    function putenv(string: cstr) -> i32

extern "string.h"
    function strdup(s: cstr lent) -> cstr owned free
```

On `putenv` I reached for **no word**, by P's default. The trap is that *no
word* is also what I write when I have not thought about the parameter at all,
so the correct putenv line and the unthinking `eat` line are the same shape.

**Variant Q:**

```
extern "stdlib.h"
    function free(p: ptr consumes)
    function putenv(string: cstr borrows) -> i32

extern "string.h"
    function strdup(s: cstr lent) -> cstr owned free
```

On `putenv` I reached for **`borrows`**, by direct lookup: *kept past the call
and never freed* is the POSIX sentence in other words. The trap Q carries is
English, not the rule: *C borrows the string* would also describe `strdup` to
an ear that hears "borrow" as "give back later", so `lent` and `borrows` are the
two ends of one act with opposite meanings. With the three-row rule in front of
me I did not confuse them; cold and without the table I might. The sentence I
would want to read, so the two uses of `borrows` are one word with one meaning:

> On a `cstr` or `ptr` parameter one word says what C does with the address:
> `lent`, reads it while the call runs and lets it go; `borrows`, keeps it and
> never frees it; `consumes`, frees it. A lend reaches only a `lent` parameter
> and a lease a `lent` or a `borrows` one; a parameter that says nothing takes
> only an address C itself handed out, or `nullptr`. `borrows` after a result
> says the same from the other side: the program keeps what C hands back and
> never frees it.

The last clause is my reading of what an unmarked parameter must still accept;
if the sitting's rule differs, the sentence should say what does reach one,
because *neither reaches* alone reads as *nothing reaches*.

### Task 2, prediction test

Format guessed as `error[name]: sentence`. Program F1 is the brief's program;
F2 is F1 with `function eat(s: cstr consumes)`; F3 is `putenv(string: c)` with
`c` a lease.

**Under P:**

- F1: **no diagnostic.** Builds; dies inside `eat`, and if `eat` returns, again
  at `end_lease(@c)`.
- F2: `error[lease_consumed]: `c` is a lease, and `eat`'s parameter `s` is
  declared `consumes`: C frees what reaches it, and `end_lease` frees a lease,
  so the bytes would be freed twice.`
- F3 with putenv unmarked: **no diagnostic**, and the program is correct.

**Under Q:**

- F1: `error[lease_to_unmarked]: `c` is a lease, and `eat`'s parameter `s`
  says nothing about what C does with it: declare `s` `lent`, `borrows` or
  `consumes`.`
- F2: `error[lease_consumed]`, the same sentence as under P.
- F3 with `putenv(string: cstr borrows)`: **no diagnostic.** With putenv
  unmarked: F1's `lease_to_unmarked`. With `putenv(string: cstr lent)`: no
  diagnostic (a lease reaches `lent`), and the program is correct so long as
  `end_lease` follows the last `getenv`, which no variant can see.
- One more Q enables and P cannot phrase, a lend to a keeper:
  `error[lend_to_borrows]: `kv.cstr()` lends `kv` for this call only, and
  `putenv`'s parameter `string` is declared `borrows`, so C would keep an
  address that dies when the call returns: lease it, `kv.lease()`.` Under P the
  same program is refused too (a lend reaches only `lent`), but the message can
  only say *declare it `lent`*, which for putenv is the wrong repair.

### Task 3, word candidates for the freeing case

Ranked by *would I write it on `free`, and not on `putenv`, and not on
`strdup`, first try*:

1. **`frees`** — the C documentation's own verb, no translation step; excludes
   putenv (does not free) and strdup (only reads) by its plain meaning. The one
   word that passes all three cold. Cost: a second word beside `consumes` on
   handles, unless the handle word moves too.
2. **`consumes`** — passes once the spec's sentence is read; fails putenv for a
   reader with the ownership prior (*the environment consumes the string*).
   Already the handle word, so one word for one concept, but it needs the
   sentence, and `frees` does not.
3. **`takes`** — putenv *takes* the string; strdup *takes* an argument. Fails
   two of three.
4. **`owns`** — the environment *owns* the string after putenv; and `owned`
   already means *the compiler frees the result with this function*. Fails
   putenv and collides.
5. **`given`** — the string is *given* to the environment. Fails putenv, and
   is the one candidate that is not a verb the C side performs.

First try on `free`: `frees`. If the sitting keeps `consumes`, both P and Q
make the putenv mistake loud in the common case, because the only things a
program has to hand putenv are a lend, a lease or `nullptr`, and a lease to
`consumes` is refused under both. Today it is silent.

### Task 4, locality

**No veto, and no.** Under Q, a call's legality follows one word on the
callee's parameter, exactly as `lent` already works and as `@` has always
worked: the word sits on the parameter it governs, and the line plus that
signature decide. P has the same shape with one word fewer. Q reuses `borrows`
with a result-side meaning already in the document, but which side a word is
on is visible on the line, so that is a naming cost and not a locality one.
The one non-local sentence in today's § 13 is *where any `extern` consumes a
handle type every call handing one back says which it is*; Q as briefed does
not extend that shape to `cstr` or `ptr`, and should not.

### Task 5, cost of reading

Q is easier to hold: three words, one meaning each, all visible on the line;
P's *kept* case is the absence of a word, and an absence is the one thing a
reader cannot attend to and a model cannot see. P also leaves § 3's *a
function taking one without `@` may still change, or free, what C holds*
contradicting § 13's *unmarked keeps*; under Q an unmarked parameter promises
nothing, which is what § 3 says.

## hesitation_points

1. Today, `free(p: ptr)` vs `free(p: ptr consumes)`: both compile; silent.
2. Today, `consumes` on a `cstr` receiving a lease: is the lease ended? Unsaid;
   silent either way.
3. P: the correct putenv line and the unthinking `eat` line are both unmarked;
   the brief's crash compiles for anyone who did not read the header note.
   Silent.
4. Q: `lent` written on a keeper (`putenv`) admits a lend and leaves the
   environment holding a dead address. Silent. Same under P and today: no
   variant closes *wrong word*.
5. Q: `borrows` written on a reader (`strdup`) refuses a lend, loud, and admits
   a lease, safe but over-constrained.
6. Q: `borrows` written on a freer (`eat`) is today's crash again. Silent.
7. Both: `cstr` against `void *p` for `free` itself; § 13 exempts *what a `ptr`
   points at*, not a `cstr` where C says `void *`. Unknown; orthogonal.
8. Both: the `CParam` production has `lent` in one slot and
   `consumes | acquires | borrows` in another, so `s: cstr lent borrows`
   derives. Under Q it must be refused; loud only if the checker says so.
9. Q: what an unmarked `cstr` parameter still accepts is not stated by
   *neither reaches*; I inferred *a C-born address or `nullptr`*.

## argument

The enemy is the mistake that compiles. Under P, the brief's crash is exactly
the program a reader writes before thinking, and it compiles; P protects only
the reader who already knew `eat` frees. Under Q the same program is refused
and the message can name the three-way choice, so the first try that was silent
becomes a loud question with its own answer. Q's added cost is one word
(`borrows`) with an English trap against `lent`, but a wrong `lent` is silent
under both variants, so Q adds no silent class P lacks. `frees` beats
`consumes` as the freeing word cold; either is loud under Q where today's is
silent.

## prediction

Falsifiable with the compiler on this header and program, written in full.

`giveaway.h`:

```c
#include <stdlib.h>
static inline void eat(const char *s) { free((void *)s); }
```

Program F1:

```
extern "giveaway.h"
    function eat(s: cstr)

function main()
    x = "payload"
    c: cstr @ x.lease()
    eat(s: c)
    end_lease(@c)
```

- **Under Q, F1 does not build**, and the diagnostic's message contains the
  parameter name `s` and all three words `lent`, `borrows`, `consumes`.
  Under P, F1 builds and the binary exits by signal.
- **F2** (F1 with `function eat(s: cstr consumes)`) does not build under
  either variant, and the message names `c` as a lease.
- **F5**, the residual class, builds under both and is refused by neither:

```
extern "stdlib.h"
    function putenv(string: cstr lent) -> i32
    function getenv(name: cstr lent) -> cstr

function main()
    kv = "HEROES_PANEL=172"
    _ = putenv(string: kv.cstr())
    k = "HEROES_PANEL"
    match getenv(name: k.cstr()).validated()
        .ok v  => print(v)
        .err e => print(e.code)
```

- **Harness rate**, when it next runs the brief's task (bind a C function
  documented *frees*, call it with a program-owned string): under P, at least
  40% of first tries omit `consumes` and produce a binary that dies; under Q,
  0% build without a word, and the wrong-word rate (`lent` or `borrows` on the
  freer) is at most 10%. Silent-error delta P to Q: at least 30 points.

## condition

I move to P if the harness shows Q's wrong-word rate on read-only and keeper
parameters (`borrows` on a `strdup`, `lent` on a `putenv`) exceeds P's
omitted-`consumes` rate on freers, since then the three-way choice produces
more silent programs than the default it replaces. I would object to Q if its
unmarked-parameter diagnostic does not name the three words, because the
loudness gain rests on the reader being told the choice at the line that
failed. Nothing measurable would make this a veto: the word is on the line.
