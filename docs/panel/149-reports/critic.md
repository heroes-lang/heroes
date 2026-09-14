# Panel 149 — report of the completeness critic

No verdict. What is named here is what the sitting is MISSING: a route nobody
listed, a claim written without the command that settles it, a contradiction
between seats, and the question that was not asked.

Read: `149-briefs/00-shared.md`, all five reports, panel 147 in full,
`selfhost/check/map_keys.hero:95-180`, `selfhost/check/acquiring.hero` in full,
`selfhost/emit/ops.hero:150-175`, `spec/heroes-spec.md:348-394`.

**Written in two passes.** The four contradictions and the missing question were
written first, from reading alone; the verification section below was appended
after, and each claim there says whether it was run.

---

## A. The specification: two seats read one sentence and neither measured the same thing

**Both readings are of the same three sentences, and the disagreement is not
about what they say. It is about what a text DOES.** `spec:375-380`, verbatim:

> `acquires sqlite3_finalize` after a handle result or `@` out-parameter says the
> call begins that handle's life and names the one that ends it, which the
> program owes it. The owing is counted, so a handle consumed twice hides one
> never consumed. `borrows` says the call hands back one it keeps, and where a
> group consumes a handle type every call handing one back says which it is.

The warden reads the third clause as a DUTY whose trigger is an act, so the duty
already binds `pair_make` and R1 costs zero. The ergonomist reads the first
clause as the definition of where the mark may STAND, so the third clause cannot
reach a result that is not a handle. **A text can license a mark and still fail
to require it, and those are the two different things being measured.** They are
compatible, and the sitting must not record them as one seat being wrong.

**The warden's decisive argument is the grammar production, and the production
does not carry it.** The argument is that `Member` admits `acquires` after any
`Type`, not after a handle type, so the prose sentence must be definitional.
`spec:388-393`:

```
Member = "function" ident "(" [ CParam { "," CParam } ] ")"
           [ "->" Type [ "owned" ident ] [ "acquires" ident | "borrows" ] ] NEWLINE
CParam = [ "@" ] ident ":" Type [ "owned" ident ]
         [ "consumes" | "acquires" ident | "borrows" ] .
```

**`owned ident` sits in the same position, after the same `Type`, in both
productions** — and nobody in this sitting believes `owned sqlite3_free` is legal
after an `i64` result. `spec:370` restricts it in prose exactly as `spec:375`
restricts `acquires`: *"after a `cstr` result or a `char **` out-parameter"*. So
permissiveness of the production is not evidence that the neighbouring prose is
definitional rather than restrictive; the document uses that same shape twice and
means the narrow thing once by everyone's reading. **The checkable side is the
warden's, and it is the weaker half of its own case.** (The runnable half —
whether the compiler refuses `owned` after a non-`cstr` result — is in
Verification 3 below.)

**A fifth reading dissolves it, and the warden already wrote it without saying
so.** The warden's own preferred draft `dsub` substitutes *"after a result or `@`
out-parameter **reaching** a handle"* at **+4 real**. A seat that believes the
text already states the duty does not need +4 to *"kill the second reading at its
root"*. The +4 is the admission. **Adopt `dsub`. R1 is not free and it is nearly
free, and the sitting should say the second thing.**

**What nobody measured, and it is the cheapest gap in the sitting to close.** The
ergonomist priced its candidate B at **nine words** and the warden priced
thirteen drafts in **real tokens**, and **B is not any row in the warden's
table**. The two seats priced two different texts and no one mapped one onto the
other. So:

- the nine words are **unpriced in the unit that judges the budget**;
- `dsub` (+4) is **untested for comprehension** — the ergonomist never saw it,
  and the A/B that produced the whole Task-2 finding was run on A and B only.

**And the consequence the two verdicts cannot both have.** The ergonomist's
adoption carries four conditions and says *any one unmet drops the verdict to
object*. Conditions 1 (depth stated in the text) and 2 (fixed arrays stated in
the text) **are spec sentences**. R1 "at ZERO required spec tokens" leaves both
unwritten. **Zero tokens and the ergonomist's adoption are not simultaneously
satisfiable**, and no seat noticed because the warden priced text and the
ergonomist priced belief.

---

## B. Veto collision on `Font` — the compiler seat's adopted rule refuses the one
## real-library binding in the sitting

**Would the engineer's rule refuse `Font`? Yes, on its own wording.** The rule
adopted is *refuse a mark whose type reaches N ≠ 1 handles*. The binding the ffi
seat compiled against `/opt/homebrew/include/raylib.h:1484-1498` declares
`record Recs tag Rectangle` and `record Glyphs tag GlyphInfo` — two records with
a tag and no fields, which `spec:355` makes handles — and `record Font` holds
both. **N = 2.** `LoadFont(...) -> Font acquires UnloadFont` is one mark, ran at
**exit 0**, and under the adopted rule it is a compile error. `Model` is N = 4
and the same. This is not a shape somebody imagined: it is a shipped header on
this machine, compiled, linked and executed inside this sitting.

**Is it a formal veto? No, and the sitting must not count one.** Panel 147 drew
this line explicitly and refused to count three refusals as three vetoes. The
ffi seat's declared scope is **ABI breakage**, and its own report places its veto
narrowly and elsewhere: *"a resolution in which the counter counts reachable
handles… The seat vetoes that reading specifically. It does not veto R1."*
Refusing `Font` breaks no ABI. **So this is an objection at full force from the
seat that compiled the evidence, against an adoption from the seat that did
not — and calling it a veto would be a number this sitting did not earn.**
Calling it *only* an objection and moving on would be worse.

**And the engineer's own lift-condition fired inside the sitting, unnoticed.**
It wrote: *"R3 veto lifted: a named C function in a real header returning a
struct BY VALUE carrying two or more pointers the caller must release. None found
in this repository's corpus."* `LoadFont` is that function, in a real header, by
value, two owning pointers. **The negative claim was the seat's vocabulary and
the ffi seat's grep was the world**, exactly as CL-018 says.

**The two seats wrote near-identical falsifiers that differ on the load-bearing
word, and nobody reconciled them:**

| seat | the falsifier it wrote |
|---|---|
| compiler-engineer | two or more pointers **the caller must release** |
| ffi-pragmatist | two owned pointers **released by two different calls** |

`Font` satisfies the first and not the second. **Which one is the real falsifier
decides whether the engineer's veto stands**, and it is a one-line editorial
question the sitting can settle without another measurement.

**The dissolving fifth reading.** Both seats are right about their own axis and
the axis is misnamed. What a mark asserts is **how many release CALLS the program
owes**, and the ffi seat enumerated from 3400 headers that C's answer is almost
always one whatever the pointer count: `UnloadFont` one for two arrays,
`UnloadModel` one for four, `freeaddrinfo` one for a chain, `xmlFreeDoc` one for
a tree. **N-reachable-handles is not the number of obligations and never was.**
A refusal keyed on it refuses correct bindings by construction. See D.

---

## C. The historian's preferred R3 is panel 147 Route A, refused on its axis and
## ratified by the author the same day — and no seat noticed

**The recommendation, verbatim:** *"The robust resolution precedent supports is
to move the releaser onto the handle TYPE — a `Slot` is closed by `slot_close`
whoever produced it."*

**Panel 147 R1, ratified 2026-09-14:** *"Route A is REFUSED, on the axis rather
than on the spelling. A releaser keyed on the TYPE cannot be right, because the
obligation is created by a **call**: the same handle type is handed back owned by
one C function and borrowed by another, compiled at this sitting. Refusing it is
not a cost judgement and **no spelling repairs it**."* The author's ratification
that day: *"all six resolutions as adopted, nothing changed."* Panel 147's title
is the sentence.

**So yes, it overturns a ratified sitting, and it is the same proposal down to
its precedent.** Panel 147's historian argued Route A from **Vala**, naming
`free_function = "sqlite3_finalize"`; panel 149's historian argues the type route
from **Vala and ARC**. Same seat, same precedent, same route, nine days later,
with no citation of the sitting that refused it.

**Did any seat notice? No.** `grep -n "147" docs/panel/149-reports/*.md
docs/panel/149-briefs/*.md` returns **two hits, both reading "148"** in other
words — `ffi-pragmatist.md:85` and `compiler-engineer.md:4` cite panel 148.
**Zero citations of panel 147 in five reports and six briefs**, including the
shared brief, which carried none of panel 147 into the sitting. The historian
cites nine external systems and no Heroes panel; that seat's mandate is
precedent, and this project's own ratified precedent was the one body of it
nobody searched.

**And it is falsified by a measurement inside this sitting's own record.** Panel
147's refutation of Route A was `sqlite3_db_handle` handing back a borrowed
instance of the marked type. Panel 149's ffi seat re-ran that refutation on a
different library without knowing it was one:

> `LoadFont … acquires UnloadFont` and `GetFontDefault() -> Font borrows` are two
> functions of identical C result type, one owning and one lending

**A releaser on the `Font` type cannot express that table.** The historian's R3
dies on the ffi seat's experiment 3, in the same sitting, and the synthesis must
say so rather than record it as an advisory preference.

**What survives of it, and it is worth keeping.** The historian's *structural*
finding is independent of the route and is the sitting's best framing: every
system that writes the mark on the FUNCTION stops at the function's signature,
and every system that reaches a field writes the mark on the FIELD or the TYPE.
Heroes has ratified that it will never write it on the type. **Panel 147 therefore
does not merely refuse the historian's recommendation, it PREDICTS the sitting's
whole difficulty**: asking a producer-level word to describe a field-level fact
is what R3 is, and the language has already closed the exit the industry took.

---

## D. The fixed array: two correct measurements of two different programs, and
## the axis is what each program CONSUMES

**Not the same experiment. Not a contradiction. The two halves of one
arithmetic**, and both numbers are right.

| | the engineer's program | the ffi seat's program |
|---|---|---|
| the composite | `record Four` holding `a: Slot[4]`, C fills 2 | jpeglib-shaped shim, 4 slots, C fills 2 |
| the mark | one `acquires` | one `acquires quant_destroy` |
| **what the program consumes** | **each element**, two calls | **the composite**, one call |
| increments / decrements | +1 / −2 | +1 / −1 |
| exit | **134** | **0** |

**One mark is one increment** (ffi seat) and **the count of reachable handles is
C's runtime choice** (engineer) are both true, and they only collide if the
counter is asked to reconcile the two sides. The engineer's program releases
what the composite REACHES; the ffi seat's releases the composite ITSELF. The
historian's confirmed prediction (`Slot[4]`, four released, **+3 abort 134**) is
the engineer's shape at arity four, not a third result.

**Which shape should the language support? Both are writable today, and only one
is countable.** The per-element shape needs the program to know N, and both real
headers the ffi seat found say in their own text that N is not knowable —
`jpeglib.h:638` *"or NULL if not defined"*, `<net/route.h>`'s filled slots in a
sibling bitmask. **So the per-element shape cannot be counted, by the header's own
statement, and no mark and no constant repairs it.** The composite shape is
countable, is what C overwhelmingly ships, and already works at exit 0 with one
mark.

**The route nobody listed.** All three R3 candidates — refuse the producer,
repeat the mark, name the field — act on the PRODUCER's declaration. The two
programs above differ on the CONSUMER's, and **no seat proposed a rule about the
consuming side at all.** Two shapes of it exist and neither was named:

1. **Refuse or diagnose the element release** — a program that consumes a handle
   reached through a composite an `extern` handed back under a single mark. This
   is the engineer's own aborting program, and it is the one that is wrong.
2. **Let the mark say WHOLE or PER-ELEMENT**, which is the axis the two programs
   actually differ on, and which no candidate in R3 expresses.

The sitting can refuse both; it has not refused them, because it did not list
them. **CL-057: a recommendation is a claim about the option set, and this option
set has a hole exactly where the two measurements disagree.**

**And there is a fourth number nobody put beside these three**: the warden's two
`@` out-parameters, **two marks, both correct, abort 134**, with the leaking
program at exit 0. Same arithmetic, same root cause at `emit/ops.hero:162`, and
it is the one case where the instrument **rewards the leak**. Any R3 resolution
that leaves the emitter's boolean standing is a resolution about the wrong
defect.

---

## The question the sitting did not ask

**Both candidates the coordinator named are real, and the second is the deeper
one. But neither is the question.**

### The question: does the rule's CONSUMER side reach as deep as its producer side — and if not, R1 does not diagnose `Font`

R1 is framed, in the shared brief and in every report, as a question about what
the rule SEES when it looks at a **result or `@` out-parameter**. The rule has a
second half nobody widened. `check/acquiring.hero`:

- `consumed_types` asks `handle_behind` of every `consumes` parameter — **one
  level, no reachability**;
- `bindings_say_which` **returns immediately** when that map is empty;
- the producer branch fires only when the reached handle is a **key of that
  map**.

The raylib binding the ffi seat compiled declares `UnloadFont(font: Font
consumes)` — `consumes` on the **group record**, not on a handle. `Font` has
fields, so `handle_behind` fails, so `consumed_types` is **empty**, so
`bindings_say_which` returns at its first line **whatever the producer side
learns to see**. See Verification 2: this is run, not reasoned.

**So R1 as framed does not close the sitting's only shipped-library case.**
Deepening the producer alone widens the rule for `pair_make`-shaped programs
whose group happens to `consumes` a bare handle, and leaves untouched the shape
that motivated the ffi seat's entire experiment 3. **Nobody asked how deep the
`consumes` side looks, and it is the same question with the same answer, on the
other side of the same rule.**

### The two candidates the coordinator named, both real

**The runtime message.** `runtime/parts/alloc.c:181-184` predicted its own
obsolescence in writing, and the engineer read the prediction and quoted it; the
warden priced the removal as *"60-odd words… not spec text, so it pays
nothing"*. **What neither asked is whether the cause is actually abolished.**
Under R1 the negative count still fires for the engineer's per-element program,
for the diamond, and for the warden's two-`@` case until the emitter is fixed —
**three live producers of that message, and only one of them is what R1
abolishes.** A message deleted on R1's authority would be deleted while its
causes still ship. This is a repair whose shapes next to it were not attacked
(CL-061).

**`borrows` on a record reaching a handle.** Nobody asked, and it is the sharper
of the two. `GetFontDefault() -> Font borrows` **ran at exit 0**, and it is in
the ffi seat's table as evidence that R1 does not refuse correct bindings. But
what does it MEAN? On a handle result, `borrows` says the library keeps that
handle. On a composite, the ffi seat's own `Font` shows the composite may reach
**two** handles — so `borrows` on a record is a claim about all of them at once,
and no seat said whether a record reaching one borrowed handle and one owned one
is expressible, refusable, or silently wrong. **It is the same arity question as
`acquires`, asked of the word that R3 never mentions**, and `borrows` is nine days
old (panel 148 R5). The ergonomist's condition 3 — *the releaser a mark names
consumes a handle of the type reached* — has no counterpart for `borrows`,
because `borrows` names no function to check.

**A third nobody named, from the ffi seat's own evidence.** Its condition on R1
is that `acquires <releaser>` does not check its name: `acquires
sqlite3_notafunction` and `acquires sqlite3_finalize` on a `Db` both pass at exit
0 today. R1 makes that unchecked word **mandatory in more places**. The condition
is right and the sitting should adopt it; what nobody asked is what the check
CAN be under R1's own reachability: *(ii) that function carries `consumes` on a
parameter of the type actually reached* is exactly what `UnloadFont(font: Font
consumes)` fails, because it consumes the composite and not the reached handle.
**The ffi seat's condition on R1 and the ffi seat's own raylib binding contradict
each other**, and both are in the same report.

---

# Verification — appended after the above was on disk

Copy: `seed/` and `runtime/` only, into the session scratchpad; nothing in the
repository was written but this file. Compiler from the seed,
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, **real 3.21 /
user 2.96 / sys 0.14**. `heroes doctor` ok, clang 21, arm64. **No selfhost
rebuild.** Each claim below says whether it was run.

## 1. The depth bound is real, and it is wider than the engineer reported — RUN

Chains generated mechanically, `R0` holding a handle and `R<i>` holding
`R<i-1>`, the last one used as a map key, **in two variants**: records inside
the `extern` group, and ordinary Heroes records outside it.

| records in the chain | group records | ordinary records |
|---|---|---|
| 15 | exit 1, ``error[handle_map_key] … because `R0.s` is a handle`` | exit 1, same |
| 16 | exit 1, same | exit 1, same |
| **17** | **exit 0, no diagnostic** | **exit 0, no diagnostic** |
| **18** | **exit 0, no diagnostic** | **exit 0, no diagnostic** |

**The engineer's defect is confirmed on its own number**, and one thing it did
not report: **the hole is not an FFI-group hole.** It opens for ordinary
`record` declarations, which have no `ffi_field` gate and no group to bound
them, so the seventeen levels are free for anyone to write. The arithmetic is
readable at `map_keys.hero:104` with `:148`: each record level costs one
through `inside_handle`'s `depth: depth + 1`, so a chain of seventeen reaches
the handle at depth 17 and `if depth > 16` returns *"past any nesting worth
naming"* — absent, which the caller reads as **no handle here**.

**Two corrections to the engineer's report, both cheap and both citations.**

- It writes that `selfhost/handles.hero:139-160` calls the map-key refusal *"a
  hole nothing can close"*. **That phrase is at `:135` and belongs to
  `duplicate_tag`, a different diagnostic.** The map-key comment
  (`handles.hero:138-146`) says something else, and it is **stronger** for the
  engineer's case: the harm it names is a value *"found again by a fresh one
  that has nothing to do with it"* — a **wrong answer**, with no runtime guard
  named anywhere in it.
- Which settles the engineer's real point precisely. The sentence that licenses
  the bound — *"Absence at the bound is safe in the direction that matters: the
  runtime guard still sits behind this rule, so a missed refusal costs a named
  abort rather than a wrong answer"* — sits at `map_keys.hero:161-163`, **on
  `reaches_float`**. `reaches_handle`'s own comment at `:99-102` justifies why
  the two walks are separate and **says nothing about the bound at all**. The
  walk was copied and its justification was left behind on the original, where
  it is true. That is `.claude/rules/module-shape.md`'s premise-about-the-world
  in its purest shipped form: the argument stayed valid and the premise died in
  the copy.

**The engineer's prediction itself is UNRUN and unrunnable today**: it is about
`unmarked_handle_producer` at seventeen levels under R1, and R1 is not
implemented. Its stated precondition — *"the map-key half of this experiment was
already run"* — is now independently confirmed here, at both variants.

## 2. The consumer side: R1 as framed does not diagnose the sitting's only
## shipped-library case — RUN

The raylib shape, written from the ffi seat's own binding:

```
extern "raylibish.h"
    record Recs tag Rectangle
    record Glyphs tag GlyphInfo
    record Font
        baseSize: i32
        recs: Recs
        glyphs: Glyphs
    function LoadFont(fileName: cstr) -> Font
    function UnloadFont(font: Font consumes)
```

| program | `heroes check` |
|---|---|
| the binding above, `LoadFont` unmarked | **exit 0** |
| the same, plus `UnloadRecs(r: Recs consumes)` | **exit 0** |
| the same `Font` used as a map key | **exit 1**, ``error[handle_map_key]: `Font` cannot be a map key, because `Font.recs` is a handle`` |
| `Pair{s: Slot}`, `slot_close(s: Slot consumes)`, `pair_make -> Pair` unmarked | **exit 0** (defect 033) |
| `Slot` returned directly, same group | **exit 1**, `error[unmarked_handle_producer]` |

**Row 3 is the one that matters.** The shipped walk reaches `Font.recs` and
prints the path, so the producer side's blindness is **not** a walk that cannot
see; it is a walk the rule does not call. And row 1 and row 2 differ only in
whether a **bare handle** is consumed anywhere in the group. Reading
`check/acquiring.hero` for the reason — **read, not run**, because R1 does not
exist to run: `consumed_types` asks `handle_behind` of each `consumes`
parameter, which fails for `Font` because a record with fields is not a handle;
`bindings_say_which` then returns at `if taken.len() == 0`. **A producer-side
widening changes neither line.** So under R1 as framed, row 2 starts erroring
and **row 1 — the actual raylib binding, which aborts 134 unmarked — stays
silent.**

This is the shape of the finding, not a call for more scope: **R1 must widen
`consumed_types` by the same walk it widens the producer with, or the sitting
ships a rule that misses the case it was shown.**

## 3. `owned` after a non-`cstr` result: accepted at check, and clang blames the
## compiler — RUN, and it is a robustness defect of its own

Section A argued that the production's permissiveness for `acquires` proves
nothing, because `owned ident` sits in the same position under prose that
everyone reads narrowly. **The compiler does not enforce that prose either, and
the consequence is worse than a missing refusal.**

```
extern "stdlib.h"
    function labs(x: i64) -> i64 owned free
    function free(p: ptr consumes)
```

- `heroes check` — **exit 0**, no diagnostic.
- The result's Heroes type becomes **`str?`**: `v.to_str()` fails with
  ``error[bad_operand]: `to_str` takes an integer, a float, `bool` or `str`,
  found `str?` ``. The mark silently **reinterprets the integer as a `char *`**.
- `--emit-c` shows what it would do: `t2 = labs(t1);` then `h0_owned0 = t2;`
  then `t4 = h_library_validated(t3);` then `t9 = (void *)(char *)t8;` and
  `(void)free(t9);` — **strlen and free on an integer value.**
- `heroes build` — **exit 2**, *"internal error: compiling the generated C
  failed"*, two `-Wint-conversion` errors, then *"error: clang refused the
  generated C"*.

**Three things follow, all of them inside this sitting's question.**

1. The grammar argument in A is answered by the compiler as well as by the
   document: the production admits `owned` after any `Type`, the prose
   (`spec:370`) restricts it to a `cstr` result, and **nothing in the checker
   holds the prose.** Permissive production, narrow prose, unheld — three times
   over, for the mark next door.
2. It is **a sixth instance of the ffi seat's exit-1 class**, which that seat
   raised for a `tag` needing the `struct` keyword and the engineer raised for a
   `-Wconditional-uninitialized` warning. Three seats found three instances of
   one class in one sitting and **none of them named it as a class**: *the
   checker admits a binding the language does not license, and clang refuses it
   as the compiler's own fault.* CL-061 asks that a repair be attacked at the
   shapes next to it; this is the shape next to `acquires`.
3. It sharpens the ffi seat's condition on R1 from a nicety to the sitting's
   load-bearing clause. R1 makes `acquires <releaser>` **mandatory in more
   places** while its sibling `owned` is unchecked in kind and its own name is
   unchecked in existence (`acquires sqlite3_notafunction` passes). design.md
   Part 6, quoted by that seat: *"A tag nobody reads is a comment that looks
   like a guarantee, which is the one thing an FFI must never carry."* **R1
   without the name check makes more of those, not fewer.**

## What I could not run, said plainly

- **Every price in the engineer's report is an uncompiled line count**, said so
  in its own words, and I did not compile any of them either. The +99 / −53 /
  +20 and the +25 and the +13 are **unrun**, in this report as in that one.
- **Nothing here re-measures the ffi seat's raylib or jpeglib runs**: I have no
  raylib binding of my own and did not install one. Its exit codes are its
  measurement, and my `Font`-shaped programs are the *checker's* answer to the
  same declarations, which is a different instrument.
- **The multi-handle runtime arithmetic (+2/−1, +4/−1, the diamond) was not
  re-run here.** Three seats measured it independently and agree on every
  number; what I add is that they were measuring **different programs**, which
  is section D and needs no fourth run.
- **The two-`@` emitter defect was not re-run.** The warden ran it, the
  coordinator re-ran it into defect 034, and `emit/ops.hero:162-166` reads as
  both describe: two booleans, one `hero_handle_acquired();` and one
  `hero_handle_consumed();` per call, whatever the arity.
