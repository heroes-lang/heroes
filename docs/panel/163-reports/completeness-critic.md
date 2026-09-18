# Panel 163 — completeness critic

Not a sixth judge. No verdict on the question. What follows is what is MISSING
from the six briefs and the five reports.

**Every number below came from a command run on 2026-09-18 in a copy of the tree**
at `…/scratchpad/crit`, seeded with
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (`heroes 0.2.0`).
`rm -rf build` first. Nothing was rebuilt from `selfhost/`;
`archive/bootstrap-rs/` was not read. The repository working tree was not
modified; my only write is this file.

---

## 0. The one measurement that changes the sitting

**Route 4 does not need a shim.** Three seats ran the shim variant and the
synthesis is about to adopt it. There is a variant with **zero author-written C**
and nobody separated the two:

```
extern "sys/utsname.h"
    record Utsname tag utsname partial
        machine: i8[256]
    function uname(@u: Utsname) -> i32

function blank() -> Utsname
    return Utsname(machine: [0, … 256 of them …])

function main()
    u: Utsname @ blank()
    rc = uname(@u)
    assert rc == 0
    print(u.machine.validated_bytes().must())
```

`./heroes run c/e1.hero` → **`arm64`**. `./heroes run --sanitize c/e1.hero` →
**`arm64`**. No diagnostic, either way.

So there are **two** route 4s and they have different prices. Measured on one
instrument, `./heroes measure`, `maximum` row, **vendored and therefore a lower
bound** (`--refresh` refuses every path but `spec/heroes-spec.md` and
`CLAUDE.md`):

| variant | `.hero` | `.h` | **total** | author-written C | runs |
|---|---|---|---|---|---|
| **L** — full literal, five fields | 3979 | — | **3979** | none | yes |
| **Lp** — literal + `partial`, one field (spec-warden's) | 860 | — | **860** | none | yes |
| **B** — Heroes producer + `partial` (**route 4b**, mine) | 874 | — | **874** | **none** | yes |
| **A** — `static inline` shim + `partial` (**route 4a**) | 98 | 61 | **159** | 9 lines | yes |
| R — the route-1 spelling `repeat(0, 256)`, token count only | 99 | — | **99** | none | **no** |

Bytes, for the two seats who reported bytes: L 4241, Lp 1031, B 1079,
A.hero 284 + A.h 161 = 445. The compiler-engineer's 281 + 214 = 495 and the
spec-warden's 903 vs 4052 both reproduce to within the small differences in our
three programs; **they are not the same program and the synthesis must not
average them.**

---

## Contradictions

### C1. Two seats disagree on whether route 4 is what the completeness rule calls failure, and the disagreement is decidable

`.claude/rules/c-boundary.md` and CL-028 (`docs/records/contract/case-law.md:500-508`)
state the instruction in one sentence: the FFI must be **complete**, *"because a
library Heroes cannot bind is a library the author must leave C code around
for."*

- **compiler-engineer** (report § Question 4): *"a shim header **is** C code the
  author leaves around, which is exactly what CL-028's *complete* FFI
  instruction dislikes"* — and then puts it below robustness on § Precedence.
- **ffi-pragmatist** (report § 6): *"§1.11 already says everything a real program
  needs comes from C. A producer for a C struct is C."*

**Neither is checkable as stated, because both argue about the word *around*.**
The checkable form of CL-028's rule is not *how many lines of C* but *is there a
library Heroes cannot bind*. Run: **no.** Route 4b binds `uname` with zero
author-written C, at 874 tokens. So:

> **The sitting is not adopting the thing the completeness rule names as
> failure — provided the resolution states 4b as the route and 4a as an
> optimisation.** If the synthesis writes "the answer is a shim", then the shim
> becomes the only stated answer and the objection lands, because a reader has
> then been told that binding `uname` requires C.

**Two records neither seat cited, and both are on the shim's side:**

- `docs/design/design.md:3553` — *"`sqlite3_exec` binds today through **28 lines
  of `static inline` in a header the group names**, with no build rule."* The
  repository already ships this pattern in its motivating example.
- `docs/design/design.md:497` — *"Modern C++ libraries are reachable only through
  a C shim you write yourself. **This is a real limitation and it is
  accepted.**"*

So design.md has already priced a hand-written shim as an accepted limitation,
in writing, twice. **The sitting rediscovered an accepted cost and is about to
re-litigate it.** What is genuinely new is that here the shim is optional, which
is strictly better than the sqlite3_exec case — and that sentence is the one the
synthesis owes § 13.

### C2. The llm-ergonomist's OBJECT on route 4 rests on a premise four runs falsify

Its report, Task A route 8: *"The header declares no constructor for
`struct utsname`. Refused by the world rather than by a sentence… it does not
merely make this task harder — it makes it impossible, because the only producer
of a `struct utsname` is `uname` itself, which requires one first."*
And its verdict: *"**Route 4 — refusing: OBJECT.** It does not make this task
hard, it makes it impossible."*

Measured four times in this sitting (compiler-engineer, ffi-pragmatist,
spec-warden, me): the task is **possible today, with and without C**. The seat
was correct about the header and wrong about the world, and it was the one seat
forbidden to check. Its objection to route 4 is therefore **inadmissible as
written**, and the synthesis must not count it as a seat against route 4. The
seat's *reasons* survive (see A2); its conclusion does not.

### C3. `partial` is claimed to cut the cost and it does not cut the C

The spec-warden's finding is right on tokens and silent on the emitted C.
Measured with `heroes build --emit-c` on the two programs:

| | `int8_t` temporaries in `main` | lines | bytes |
|---|---|---|---|
| **Lp** (literal + `partial`, ONE 256-field) | **256** | 1070 | 29926 |
| **A** (shim + `partial`) | **0** | 304 | 12852 |

The ffi-pragmatist measured 1280 temporaries for five fields and called the cost
*"the **shape**, not the clock: O(N) stack slots per fixed-array literal"*.
**`partial` does not touch that shape.** It cuts the token cost 4.6× by cutting
the field count 5×; per *declared* long field the O(N) frame stands. So
"`partial` helps" is true of tokens and false of the emitted C, and the sitting
has only the first half.

### C4. Three seats ran three different experiments and the convergence is narrower than it looks

- ffi-pragmatist: shim + **all five fields** declared (no `partial`).
- compiler-engineer: shim + `partial`, one field.
- spec-warden: **no shim**, a Heroes `function`, `partial`, one field.

Only two of the three are the same experiment. The spec-warden already ran 4b
and reported it as "route 4 with a helper function", so the sitting has the
zero-C route in its record without anyone naming it as a distinct route with a
distinct price. **That is the CL-057 failure one level down: the seats
enumerated verdicts, not variants.**

---

## Unmeasured claims

### U1. The historian's falsifier, settled on the platform I can reach

The historian's prediction hinges on `sa_mask`, and the seat says of the layout:
*"That `sigset_t` is internally an array is something I did **not** source —
unverified."*

Run on this Mac (`clang -E` over `<signal.h>`, then a compiled `sizeof` probe):

```
typedef __uint32_t __darwin_sigset_t;
typedef __darwin_sigset_t sigset_t;
sizeof sigset_t=4
sizeof sa_mask=4
```

**On Darwin `sa_mask` is a 4-byte scalar, not an array.** The historian's
falsifier — *"a C function that reads a **fixed-array** struct field before
writing it and for which all-zero is wrong"* — **does not fire on this platform
through `sigaction`**. On Linux it is unchecked; see § What I could not check.

Note what this does to the finding either way: under `spec § 13` a field is *"a
number, `bool`, `ptr`, `cstr`, another record of the group, or a fixed array of
one"*. A Darwin `sa_mask` is a number, so it is a **scalar** field, and the
historian's own pattern — *"every field where zero is wrong is a scalar"* —
would keep scalars mandatory and leave it required. The prediction survives on
Darwin for the reason the historian gave, not by luck.

### U2. The §1.2 break-even was computed against a baseline the sitting rejects

The spec-warden's verdict turns on this arithmetic: *"a saving of **761**
program tokens… Break-even on tokens alone: **761 / 60 ≈ 1 program in 13**."*

The 761 is **route 1 against the 256-zero literal**. But the sitting is not
keeping the literal — it is adopting route 4. The honest denominator is route 1
against **the route being adopted**, measured above on one instrument:

```
route-1 spelling (R.hero)          99
route 4a, all in (A.hero + A.h)   159   → saving 60, not 761
route 4a, the .hero file alone     98   → route 1 is ONE TOKEN LARGER
```

Both numerators come from the same vendored table, so the **correction factor is
unit-consistent even though the absolute break-even is not**: the spec-warden's
saving is overstated by **761 / 60 ≈ 12.7×**. Break-even moves from *1 program in
13* to **1 program in 1** — every generated program would have to bind a
long-array struct for route 1 to pay for itself.

And the sharper form: **route 4a's `.hero` file is 98 tokens against route 1's
99.** All of route 1's saving over the adopted route is the 61-token header, and
a header is written **once per struct per platform**, not once per program. The
spec-warden's conclusion is right and its strongest argument is one it did not
make.

*Caveat I am obliged to state and the spec-warden was too: these are vendored
program counts compared against a `real` spec delta. Mixing the two is not sound
for an absolute break-even. The ratio between two vendored numbers is.*

### U3. `partial`'s real cost, measured, and the sitting has not counted it

The brief and four reports treat `partial` as free. `spec/heroes-spec.md:352`
does not: *"`record Font partial` names only some, and then comparing it and
using it as a map key are compile errors — **for it and for any value holding
it**."* Run:

```
error[ffi_partial_operation]: `Utsname` is `partial`, so it cannot be compared
with `==`: the fields it does not name are part of the answer, and this program
cannot see them
  note: a `partial` record may be read, copied, passed to C and returned from it
  — what it gives up is `==`, `hash` and being a map key. Name every field of
  the header's struct to get them back, and drop `partial` from `Utsname`
```

For `Utsname` this costs nothing — nobody compares uname results. **The cost is
in the advice, not the example.** The resolution as three seats phrase it is
*declare only the fields you read*, which makes `partial` the default shape for
every header record. The diagnostic's own words say what that then propagates:
`==`, `hash` and map-key are lost **for any value holding it** — transitively,
to every Heroes record with a header-record field. Nobody priced that, and the
diagnostic is the instrument that would.

### U4. Route 1's veto was measured on the call form and asserted for the form two seats actually proposed

The compiler-engineer's veto: *"a `repeat(0, 256)` typed `i8[256]` must be
erased at compile time… `grep -rn "const_eval\|constant_fold" selfhost/` returns
**nothing**: there is no compile-time evaluator in this compiler."*

That is a measurement about a **call**. Both the historian (§ 6) and the
llm-ergonomist (§ "A fifth route") proposed a **literal** instead —
Rust's `[0; 128]`, *"the length lives inside the form"*. The veto does not
transfer, and the seams for the literal already exist:

- `selfhost/parse/type.hero:258` — `function fixed_length(@c, text, open) -> i64`
  **already reads an integer literal as a length**, with its own diagnostic
  `empty_fixed_array` at `:291`. No evaluator is involved; the length is a token.
- `selfhost/check/walk.hero:616` already compares `elements.len()` against
  `x.length`; a repetition form supplies the count instead of counting nodes.
- `selfhost/emit/storageless.hero:1-8` invites the producer in its own doc:
  *"A third would want this module rather than another arm."*

**I did not implement it and I am not pricing it.** The claim is narrower and it
is the gap: *the one seat that could price the literal form was asked only about
the call form, and vetoed the call form.* No number in this sitting bears on
`[0; 256]`.

### U5. Claims explicitly marked unrun by their own seats, carried forward so the synthesis does not promote them

- ffi-pragmatist's prediction on SQLite and raylib: *"**Neither was run**; both
  are cheap to check and I am naming the file so they can be."*
- historian's Swift cost numbers (~1600 lines, ~8 s, ~750 KB): *"mark those
  numbers **unverified**"*.
- historian's Ada `(others => 0)` and Zig `**`: *"**unverified** — I did not
  fetch either this session."*
- historian: could not check the `design.md §4.9` and `spec § 5` citations —
  *"this seat had no shell"*. The whole seat ran without the repository.

---

## Routes nobody listed

### R1. Route 4b — the same route with no C at all (measured above)

874 tokens, runs, sanitizes clean, zero author-written C. It is in the
spec-warden's report as an unnamed variant and in nobody's option set. **It is
the variant that answers the completeness objection**, and it is 5.5× the token
cost of 4a — which is the honest trade the synthesis should put in front of the
author, instead of one number.

### R2. A repetition literal on the ARRAY, not a call on the record

`sysname: [0; 256]`. Named independently by the historian (Rust precedent,
verified) and the llm-ergonomist (*"the option set handed to me had four members
and the world has at least five"*), priced by neither and by no seat that could.
Its seams are U4's three. It is the only proposal in the sitting that
simultaneously:

- meets the llm-ergonomist's condition for **unconditional** approval (*"if the
  new form is on the array rather than the built-in"*), because `Point()` never
  becomes legal;
- avoids the compiler-engineer's route-2 price (120–180 lines, five modules, a
  diagnostic-class change), because `missing_fields` is untouched;
- removes the O(N) emitted-C shape C3 measures, because 256 elements stop being
  256 AST nodes and 256 `int8_t` temporaries;
- does not overload `repeat` and so does not falsify `spec § 9`'s *no
  overloading* (the spec-warden's +60 is a price on the **call** form only).

**Unpriced. That is the finding, not an advocacy.** What would have to be
measured: the spec delta in `real` for one § 13 clause plus one § 10 production;
the diff in `parse/`, `check/walk.hero` and `storageless.hero`; and whether
`heroes fmt`, `heroes mutate`, `suite_grammar`, the TextMate grammar and
`site/src/lib/highlight.ts` take a new literal form (the
`.claude/rules/diagnostics-and-goldens.md` walk the compiler-engineer costed for
route 3 applies here too and nobody has costed it here).

### R3. A generated shim — checked, and it collapses into route 2 with a better spelling

The brief asks whether the shim could be generated. **The compiler already emits
its own C beside the include**: `c/m/A.c` lines 8, 13, 14, 35–37 are
compiler-synthesised `_Static_assert`s, including
`_Static_assert(HERO_RET_RECORD(hero_blank(), struct utsname), …)`. So emitting
`static inline struct utsname hero_zero_Utsname(void) { struct utsname u = {0};
return u; }` into `out.c` is mechanically available.

What it needs is a **spelling** the program can name. And that spelling is the
route nobody listed: a zeroed-record expression **distinct from construction**
(`Utsname.zeroed()`, `zeroed(Utsname)` — the name is not my business). It is
route 2's semantics without route 2's price, because it never touches
`check_named_fields` or `missing_fields` — the compiler-engineer's 120–180 lines
are the cost of **relaxing construction**, not the cost of zeroing. And it meets
the llm-ergonomist's condition by construction. **Also unpriced.** The
ffi-pragmatist already noticed the collapse from the other end: *"at which point
route 3 has become route 2 and should be argued as route 2."*

### R4. A group declaring a function the header does not have — shut, confirmed

```
error[ffi_unknown_name]: `sys/utsname.h` declares no `hero_no_such_thing` —
clang read the header and could not find it
  note: an `extern` names what the header already has (§4.19)
```

Reproduces the ffi-pragmatist. The brief's fourth question has a measured no.
Related and also confirmed: `selfhost/cli/pointee.hero:283` passes
`["-I", source_dir]`, so a header beside the `.hero` file needs no new flag —
route 4a is landable with no CLI change, as that seat said.

---

## The question not asked

**Which document is wrong?** Every seat asked what to ADD. Nobody asked what the
spec already fails to say, and the llm-ergonomist — the one seat reading only the
spec — hit it and could not settle it:

> **H1 — the element count.** No sentence in the document says a fixed-array
> literal's length must equal the field's. I assumed it must. *Wrong guess:
> unknown…* **This is the single most important unknown in my report and it is
> unresolvable from the text.**

And its condition: *"Somebody who is allowed to run a compiler should settle H1
before this sitting resolves."*

**Settled.** The compiler enforces it:

```
error[fixed_array_length]: `i8[256]` holds exactly 256, and this literal has 1
    — a fixed array's length is part of its type, so the two must agree (§4.19)
```

The spec does not. `grep -n "fixed array\|i32\[4\]\|\[a, b, c, d\]"
spec/heroes-spec.md` returns **exactly one line**, `:351`, and it says only
*"a fixed array of one: `i32[4]`, never a `[T]`; build one with `[a, b, c, d]`"*.
No agreement rule. The diagnostic cites **§4.19**, which is design.md, not the
reader's document.

Two consequences the sitting should carry:

1. **CLAUDE.md § 12: spec beats compiler.** A rule the compiler enforces and the
   reader-facing document omits is a spec gap, and it is the gap that made a
   judge unable to answer its own task. It is also cheap: one clause on line 351.
2. **It resolves the llm-ergonomist's standing condition in favour of the status
   quo.** That seat wrote: *"If a fixed-array literal of the wrong length is
   **not** a compile error, then route 4 and the status quo are both unacceptable
   on robustness grounds."* It **is** a compile error, with the count in it. The
   robustness objection to route 4 does not fire.

---

## Admissibility

### A1. Six brief premises falsified, by four different readers, in one sitting

The brief asked for four; there are six, and the sixth decides the sitting:

| # | premise | where | falsified by | how |
|---|---|---|---|---|
| 1 | *"A call typed by context does not exist here"*, 6 literal sites | `00-shared.md:48` | compiler-engineer | 7 context-consuming arms; `.call` `:508` and `.method` `:579` both pass `expected` |
| 2 | `zero_of` already emits `(Color){0}`, `walk.hero:616` is the single wall | `compiler-engineer.md:26-31` | compiler-engineer | a `_Static_assert` type probe, C11-unevaluated, one non-test caller; the real wall is `missing_fields` `:2036` |
| 3 | *"`partial` does not help"* | `00-shared.md:30` | spec-warden, me | 3979 → 860 tokens, measured |
| 4 | route 1 *"bends nothing in the document"* | `spec-warden.md:24` | spec-warden | `repeat` is str-only; § 9 says *no overloading*; +18 → **+60** |
| 5 | route 4 = *"a long-array struct is… never built"*, and *"owes an answer"* for `uname` | `00-shared.md:83-87` | all four seats with a shell | it is built, four ways, two of them with no C |
| 6 | the option set has four members | `00-shared.md:72` (*"four is not a claim about the set"*) | historian, llm-ergonomist, me | at least seven: 4a, 4b, `[0; 256]`, a distinct zeroed spelling |

**What the pattern says.** Premises 1, 2 and 4 are all *the compiler does not
have X*. Every one was wrong, and every one was written by a coordinator who had
a shell and did not run it. The brief obeyed CL-077 for its **numbers** (*"every
number here was produced by a command… and the command is named beside it"*) and
not for its **negations** — which is CL-018 exactly: *"a negative claim rests on
the searcher's vocabulary rather than the world, so 'X cannot be done' goes out
as a question naming what was searched for."* Premise 5 is the same shape one
level up: route 4 was written as a **refusal** and it is a **capability**, so the
brief handed every seat a false framing of the option it was going to adopt. The
spec-warden calls this *"the eleventh correction to a coordinator's brief in
seven sittings."* **On the evidence of this sitting the rule to add is not about
numbers: it is that a brief's negative sentences are run, or they go out as
questions.**

### A2. Whose verdict still rests on something unchecked

| seat | verdict | rests on | status |
|---|---|---|---|
| compiler-engineer | veto routes 1 & 3 | no const evaluator (route 1) | **measured — but for the call form only** (U4) |
| compiler-engineer | route 2 at 120–180 lines | `check_named_fields` shared by two callers | measured, cited by line |
| ffi-pragmatist | veto route 3 | ASan/UBSan blind, MSan absent on arm64-darwin | measured, reproduced |
| ffi-pragmatist | SQLite/raylib prediction | — | **explicitly unrun by its own seat** |
| historian | route 2 prediction | `sigset_t` layout | **Darwin settled (scalar); Linux unchecked** (U1) |
| llm-ergonomist | **OBJECT route 4** | *"the only producer is `uname` itself"* | **FALSIFIED** (C2) |
| llm-ergonomist | F1, *"§ 13's `stat` example is uncallable"* | a reading of § 9 + § 13 | **FALSIFIED** (A3) |
| llm-ergonomist | H1 undecidable | the spec text | **true of the spec, false of the compiler** (§ The question not asked) |
| spec-warden | Principle 0 veto, 1-in-13 break-even | 761-token saving | **arithmetic correct, baseline wrong by 12.7×** (U2) — conclusion survives, strengthened |

**Three of the llm-ergonomist's load-bearing claims are falsified, and it is the
seat whose input discipline forbids checking.** That is the design working, not
failing: its *reasons* (F0, the veto on route 3, the reading of § 5's four
words, H1 as a question) are the sitting's best material and all four survive.
Only the conclusions that depended on the repository fall. The synthesis should
carry the reasons and drop the verdict on route 4.

### A3. F1 and F0 — are they defects to file?

**F1 — § 13's `record FileStat tag stat partial` is uncallable: NOT a defect. It
is falsified.** Run, on the document's own example:

```
extern "sys/stat.h"
    record FileStat tag stat partial
        st_size: i64
        st_nlink: u16
    function stat(path: cstr, @buf: FileStat) -> i32

function main()
    s: FileStat @ FileStat(st_size: 0, st_nlink: 0)
    rc = stat("/etc/hosts".cstr(), @s)
    …
```

→ prints **`544`** and **`1`**, and `ls -l /etc/hosts` says **544** bytes. Same
under `--sanitize`.

The seat's reading was that *"record construction names every field, always
mandatory"* ranges over the C struct. It ranges over the **declared** set. Proof,
by omitting a declared field:

```
error[missing_fields]: `FileStat` is built with every field, named:
st_size:, st_nlink: — all of them, always
```

Two names, not `struct stat`'s twenty. **But the seat could not have known**, and
that is the real finding: `spec § 13` never says *declared*. So F1 files as a
**spec clarification**, in the same clause as H1 — one sentence on line 351/352
covering both *the literal's length must equal the declared length* and *every
field means every declared field*. Not a defect; a `docs/work/DECIDE.md` item
or this milestone's file, and § 13 is where it lands.

**F0 — three out-parameter kinds, three ceremonies: real, measured, and not a
defect either.** The repository's own golden case is the evidence
(`tests/golden/run/ffi-owned-cell-is-freed.hero:48`):

```
    first: str? @ fail(code: "none", msg: "nothing yet")
    _ = hero_owned_describe(code: 0, out: @first)
```

So the three ceremonies the seat named are:

| what C fills | what the caller writes | evidence |
|---|---|---|
| `sqlite3 **` handle | `nullptr` | `spec § 13` |
| `char **` with `owned` | an **invented sentinel**, `fail(code: "none", msg: "nothing yet")` | the repository's own golden, above |
| `struct utsname *` | 256 zeros, or `partial` + 256, or a producer | this sitting |

The seat said *"the document names **no** empty `str?`"*. True of the spec, and
the repository answers it by inventing one per call site. Nothing is broken, so
it is not `docs/work/DEFECTS.md`. **It is the sitting's best surviving
observation** and it belongs where the next milestone will see it — the § 13
clause above is the cheapest place to settle the third row, and the second row
(an empty `str?` with a written name) is a question worth filing rather than
answering here.

**The one thing I would file as a defect is H1**, and only if the author reads
CLAUDE.md § 12 as I do: the compiler enforces `fixed_array_length` and the
reader's document states no such rule.

### A4. The formatter, since § 9 makes it a gate

`heroes fmt --in-place` on each variant, then again:

| | fmt | longest line | formatted form runs | idempotent |
|---|---|---|---|---|
| Lp (literal + partial) | **reshapes** | 803 → 785 | yes (`arm64`) | yes |
| B (route 4b) | **reshapes** | 797 → 785 | yes (`arm64`) | yes |
| A (route 4a, shim) | **leaves it alone** | — | — | — |

This extends the compiler-engineer's observation and cuts one way the sitting
should say out loud: **the two zero-C routes are the ones the formatter rewrites
into a 785-character line, and the shim route is already canonical.** No
round-trip is broken — that is the good news, and it is measured rather than
assumed.

---

## What I could not check

In those words, and each with what I searched.

- **`sigset_t` on Linux.** The decisive half of the historian's falsifier. The
  `docker` CLI is on this box (`/usr/local/bin/docker`) and `docker info` fails —
  **no daemon, so no Linux container was reachable from this seat**. I have no
  Linux leg, no sysroot in the tree and no vendored glibc header, and I will not
  assert glibc's layout from memory (CL-017). **Darwin is settled — `sa_mask` is
  a 4-byte `__uint32_t`, not an array — and Linux is unchecked.** One command on
  the Linux leg settles it: `clang -E -include signal.h - </dev/null | grep -A3
  __sigset_t`.
- **The price of R2 (`[0; 256]`) and R3 (a distinct zeroed spelling).** I did not
  implement either, did not price either in `real` spec tokens, and did not run
  `--refresh` (I did not source `.env`; I made no spec edit, so nothing of mine
  needed the judged instrument). **Every statement I make about either is about
  which seams exist, not about what they cost.** The three seams are cited by
  file and line in U4 and R3 so the next session can price them.
- **Whether route 4a/4b behave on Linux or Windows.** Every run here is
  arm64-apple-darwin. `.claude/rules/platforms.md` governs and a platform fact is
  run on a platform or it is an inference (CL-048). `struct utsname` is
  `char[65]` and **six** fields on Linux (the historian sourced the sixth,
  `Domainname`, from `golang.org/x/sys`) against five here, so the `partial`
  advice and the per-platform source file interact in a way **nobody in this
  sitting has run**.
- **The ffi-pragmatist's SQLite and raylib predictions.** Its own seat says
  neither was run. I did not run them either.
- **Whether the token ratios hold on the reader's tokeniser.** Every program
  count in this report is **vendored `maximum`, a lower bound**, because
  `--refresh` refuses every path but `spec/heroes-spec.md` and `CLAUDE.md`
  (`heroes measure` says so itself: *"No ceiling judges … so this is a count and
  not a verdict"*). U2's **ratio** between two vendored numbers is sound; the
  absolute break-even against a `real` spec delta is not, for my arithmetic and
  for the spec-warden's alike.
- **Whether any of this changes a seat's verdict.** I did not ask them. Three
  seats hold conditions that my measurements touch — the compiler-engineer's
  route-1 veto (U4), the historian's route-2 prediction (U1), and all three of
  the llm-ergonomist's falsified claims (C2, A3) — and **a condition is the
  seat's to discharge, not mine.**
