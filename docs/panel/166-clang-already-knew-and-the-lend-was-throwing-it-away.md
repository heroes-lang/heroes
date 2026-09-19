# Panel 166 — clang already knew, and the lend was throwing it away

Convened 2026-09-19, at M-declared-extents, on the two defects panel 165 found
under route 6. Five seats, a completeness critic, six briefs written to disk
before any seat started, and the working tree frozen from the briefs going out
until this file was written.

The question: **what should a `ptr` lend of a fixed byte field promise, who checks
the extent, and who marks the write?**

## The resolution, in one line

**Route H is ADOPTED — a route no seat listed and the critic measured end to end:
the lend emits `const void *` unless the header says C writes, and clang refuses
the write at the boundary with the project's own flag. It closes defect 065 at
the class for zero spec tokens and zero new surface. Route C is adopted for
defect 063 in its C-emitted form, a `_Static_assert` the probe already knows how
to write. Route G is REFUSED on three vetoes. And the sitting found a fourth
defect: the lend has no lifetime rule.**

## Route H, and why five differentiated seats missed it

The engineer's objection to refusing an immutable lend was that **the language
cannot see whether C writes**. That is true of the language and false of the
toolchain, and the difference is the whole sitting:

```
$ clang -std=gnu11 -c h.c -Werror=incompatible-pointer-types-discards-qualifiers
h.c:6:39: error: passing 'const void *' to parameter of type 'void *' discards
  qualifiers [-Werror,-Wincompatible-pointer-types-discards-qualifiers]
./immut.h:3:34: note: passing argument to parameter 'p' here
    3 | static inline void sl_fill(void *p, long n) { char *b = p; … }
```

The read direction compiles clean; the write is refused **with a note pointing at
the header's own declaration**. And the flag is not a proposal —
`selfhost/cli/flags.hero:106` already ships it, and the net asserts it at `:213`.

**So the mark does not have to be invented. It has to stop being discarded.**
`selfhost/emit/field_lend.hero:17-22` emits `(void *)` today and throws the
`const` away, on a dichotomy its own comment states — always-const or never-const
— which the critic measured is false: the header decides, per call.

This answers the engineer's veto-lifting condition word for word, costs **zero
spec tokens**, needs no mark, no grammar, no parameter mode, and leaves
`check/lending.hero:304` and `:309` green — the over-refusal the engineer
measured against route A, gone.

## The verdict table

| route | compiler-engineer | ffi-pragmatist | spec-warden | llm-ergonomist | historian |
|---|---|---|---|---|---|
| **A** refuse the lend from `=` | object | object | approve, amended, **+3 real** | approve | sound but blunt — every precedent draws the line per **direction**, not per lend |
| **B** `@`-marked `ptr` takes `@field` | **VETO**, soundness | approve only with C | approve in principle, unpriced | **approve, land first** | the strongest precedent of the seven |
| **C** the counted parameter | approve, conditional, ~210 lines | **approve — the only route that closes 063** | approve, **+51 real** | — | approve **narrowed** to compile-time constants |
| **D** `len()` on a fixed field | approve as ergonomics, **veto as a repair** | object as a repair | object, waits | — | approve; its absence is the anomaly |
| **E** a named extent | — | object on priority | object, **+36** and closes nothing alone | — | approve, load-bearing |
| **F** withdraw the lend | — | object, **cheaper than priced** | object, **−44** | — | object, no precedent |
| **G** write the hole into the spec | — | **VETO** | **VETO** on its second half | **VETO** | object, no precedent |

**Three vetoes on G**, and each from a different ground. The ffi-pragmatist:
*"G refuses a check that has already been compiled."* The spec-warden: D2b writes
a reachable memory corruption into the document, and §1.12 makes no-corruption a
goal of the **language** while §1.6 makes the document the language. The
ergonomist: under G the fact that a binding changed *"is written nowhere in the
Heroes program; it lives in a C header"*, which charges every ordinary line of
every program for one FFI form.

**No budget veto anywhere.** The warden stated it before vetoing, as it did at
panel 165: ceiling 10240, FFI floor 60, spendable 2074, and the dearest draft on
the table leaves 1984.

## The disagreements, and the critic settled all four

**1. Route B: the engineer is right, and wider than it stated.** `@p: ptr` on an
`extern` **already means** a C out-parameter — it emits `give((void *)&h0_q, t2)`,
a `void **`. And `@` is occupied at the **call site** too: `not_a_place` says
*"the callee copies its result back into the argument."* So `@` carries two
meanings already and B would make three.

**But the veto kills the SPELLING, not the ground.** The ergonomist and the
historian both argued for marking the mutation where a reader sees it — Swift
refuses a constant to an `inout` parameter, Hylo's `&` marks a mutation rather
than an address, Ada makes the mode change the C prototype. Nobody separated the
spelling from the ground, and separating them is what dissolves the deadlock:
**route H puts the mark in the C the reader never writes, and the ergonomist's
locality objection is answered by the diagnostic rather than by a sigil.**

**2. `access` versus `_Static_assert`: both facts are true and the historian's
conclusion is false.** `__has_attribute(access)` is **0** on this Mac — the
historian predicted it and the coordinator settled it, `unknown attribute
'access' ignored`. But the check does not need it:

```
$ clang -std=c11 cchk.c
error: static assertion failed due to requirement
  '(long)64 <= (long)sizeof (((struct sl *)0)->name)'
note: expression evaluates to '64 <= 8'
```

Plain C11, no flag, no runtime cost. **So route C's check is emittable**, and the
historian's *"route E is load-bearing"* does not follow — the assertion's
right-hand side is `sizeof(…)`, **C's own number**, and needs no named extent at
all.

**And the critic found the sting nobody saw**: a group `constant` extent lowers to
a function call, `t5 = h_konst_SL_NAME_LEN()`, so a Heroes-side check **refuses
the careful reader and accepts the careless one's literal**. The C-emitted form
has no such inversion.

**3. `spec § 3` is not false.** Measured by the warden and re-run by the
coordinator: the lent binding reads **65**, a copy **72**, a record field **72**,
an array element **72**. A lend stands only as an argument of a call, so no two
owned values ever alias. **Exactly one sentence is false**, § 5's *"only a
declared `@` name can be mutated"*, and § 13 already licenses what falsifies it —
the document is **contradictory rather than incomplete**. And the falsification is
**wider than the `=` binding**: an immutable **parameter**'s field is written and
read back inside the callee while the caller sees the old bytes.

**4. `getcwd` is writable today and the shared brief said it was not.**
`getcwd(buf: nullptr, size: 0) -> cstr owned free` binds with no lend and printed
the byte count of `pwd`. The coordinator wrote that sentence; the pragmatist ran
it. Its caveat, which the pragmatist did not mark: the allocating behaviour is
**unrun on glibc and Windows**, and POSIX leaves it unspecified.

## The fourth defect, which no brief and no route touched

**The lend has no lifetime rule.** C may keep the address past the frame:

```
function lend_and_return()
    s: Sl2 @ mk()
    keep(p: s.name.ptr())        # C parks the address in a static

function main()
    lend_and_return()            # the frame dies here
    print(to_str(later()))       # C reads it anyway

C reads the dead frame: 1        exit 0, no diagnostic
--sanitize: AddressSanitizer: stack-use-after-scope
```

The compiler refuses the two escapes **Heroes** can see and nothing looks at the
C side. `sqlite3_bind_text` with a null destructor is declared in this repository
at `examples/ledger/db/sqlite.hero:107` — the same shape with a real library
behind it. **Panel 164 opened the address route and did not carry the lifetime
rule across**, though the language owns its shape already: a lease COPIES, and one
nobody ends aborts when `main` returns. Filed as **defect 066**.

## What the briefs and the reports got wrong

- **"Leaves `getcwd` unwritable"** — false, above. The coordinator's.
- **The denominator was wrong three times over.** The shared brief's 66/18 is a
  regex over duplicates; the engineer's dedup gives 46/12 and does not reproduce
  (the critic gets 59/21 or 34/10 depending on the dedup). **And all of them
  measure the wrong population**: route C is about the lend's argument position,
  which is **4 call sites in 1 file**.
- **"Real C library functions whose extent is a single named sibling: zero"** is
  contradicted by the pragmatist's own output two sections earlier,
  `read(int, void *__sized_by(__nbyte), size_t __nbyte)`.
- **The engineer's "exit 133" does not reproduce.** The coordinator swept n = 9,
  12, 16, 24, 32, 64, 128, 512 and **every one exits 0 and corrupts**; at n = 9 a
  single byte lands in the sibling. The critic swept further and agrees. So *"a
  big lie traps, a small lie corrupts in silence"* is not the shape — **every
  lie corrupts in silence**.
- **`_LIBC_COUNT` expands to nothing *without the flag*.** With
  `-fbounds-safety`, `__LIBC_STAGED_BOUNDS_SAFETY_ATTRIBUTES` is 1 and the SDK
  yields `getcwd(char *__counted_by_or_null(__size), size_t)` — and it
  **enforces**: honest n=8 exit 0, overstated n=1024 exit 133. Panel 165's
  historian and the coordinator both stopped one flag short. **The data exists;
  the dialect is the wall**, and that dialect reds two of §4.19's own
  `_Static_assert`s on the shipped golden, 7 errors in 560 lines.
- **Route C is three proposals under one letter** and each seat priced a
  different one: C-check in Heroes (~210 lines, literal only), C-emit in C
  (compiled, covers products, macros and `partial`, unpriced in lines), C-spec
  (+51 real).
- **The `grammar` suite passes with an unimplemented production** — 7 passed, 0
  failed with a production no parser accepts. A net defect, filed by the warden
  as a footnote and recorded here so it is not lost.

## The resolution — `provisional — author ratification pending`

Per CLAUDE.md § 4, the most robust and complete resolution, never the cheapest
and never a compromise.

1. **Route H is ADOPTED for defect 065.** The lend emits `const void *`; the
   write direction is reached by the header declaring a non-`const` parameter,
   and clang refuses the mismatch with the flag that already ships. Zero spec
   tokens, no new surface, and the over-refusal route A was objected to for does
   not arise. **What is unpriced and owed: the checker half and the
   `cli/pointee.hero` diagnostic row**, so a reader gets a Heroes sentence rather
   than a clang one.
2. **Route C is ADOPTED for defect 063, in its C-emitted form only.** The probe
   emits `_Static_assert(<extent> <= sizeof(<field>), …)`; the check is C's, the
   number is C's, and the extent is checked **only where it is a compile-time
   constant**. That is the C99 `[static N]` discipline the historian's survey
   found is the one member of this family nobody has had to switch off — GCC's
   inference engine was disabled kernel-wide in Linux 6.8, and this adopts the
   declaration without the inference.
3. **Routes A, B, D, E and F are NOT adopted.** A and B are superseded by H,
   which reaches their ground without their cost; D and E are ergonomics that
   close no defect and wait under Principle 0; F withdraws a shipped capability
   and has no precedent in nine surveyed languages.
4. **Route G is REFUSED on three vetoes.** Not recorded in design.md Part 6: what
   is recorded is the **reason** — every language that documents such a hole
   documents it next to a word the programmer typed, and Heroes' hole is
   reachable from unmarked, ordinary-looking code.
5. **Defect 066 is filed and left open**, because the two routes for it — a
   lifetime rule on the lend, or a sentence saying the address dies with the
   frame — are the shape this sitting just vetoed for the write direction, and
   pricing them properly is its own sitting.
6. **Panel 165's resolution 3 is SUPERSEDED.** Its route 14 clause was corrected
   as unimplementable the day it was written; route C in its C-emitted form is
   what replaces it, and `docs/work/DECIDE.md`'s `panel 165` item should be read
   with this file beside it.

**What a veto compels.** G stays refused whatever the author decides among the
rest: three seats refuse it on soundness and locality rather than on price.

**What conservative would have been, so the author can choose it:** route A
alone, +3 real, which closes the `=` half of 065 and leaves the immutable
parameter, defect 063 and defect 066 open. It is cheaper by the whole of H and C
and it leaves memory corruption at exit 0 in shipped code, which is why rank 3
decides it.

## Predictions to score

- **compiler-engineer**: if route C lands, its commit shows **zero lines under
  `selfhost/emit/`** and ≥9 modified files under `selfhost/`. *Note: route C is
  adopted in the form the engineer did not price, so this prediction is scored
  against C-check and is expected to be falsified by C-emit — which is itself the
  measurement.*
- **ffi-pragmatist**: under route C, none of the four real-library `ptr` externs
  in `examples/` needs a clause, and `examples/sqlite/main.hero`'s emitted C is
  byte-identical to today's.
- **spec-warden**: A/B with D1 lands the document at **8109 ± 2 real** and reds
  exactly **two** of eighteen `.ptr()` sites, `lending.hero:304` and `:309`.
- **llm-ergonomist**: under the current text, ≥70% bind with `=`, ≥25% write an
  extent other than 8, 0% get any diagnostic; under a marked write, ≥80% of the
  `=` programs stop compiling and the extent-error rate moves **less than 5
  points**.
- **historian**: clang does not implement `__attribute__((access))`. **Scored
  CORRECT** by the coordinator, 2026-09-19.

## Also recorded, so it is not rediscovered

- **The `static inline` shim in the author's own header** is a route A–G never
  listed and the only one where 063 is **impossible by construction**: the extent
  is `sizeof x->b`, C's own, unoverstateable. It works today, and it corrects
  design.md §4.19's *"how a shim is compiled is undecided"* — the header-only
  form needs no deferred candidate and no `--include`.
- **Two corrupting shapes no static route can reach**: an extent read at run
  time, and a `partial` record whose size is C's rather than the field list's.
- **`-fbounds-safety` is refused on a number**: 7 errors in 560 lines of the
  shipped golden's emitted C, two of them §4.19's own `_Static_assert`s going red.
- **Every run in this sitting is `arm64-apple-darwin`.**

## ROUTE H DOES NOT LAND IN THE SHAPE THIS FILE DESCRIBES — measured 2026-09-19, after ratification

Written underneath rather than rewritten. **The route was implemented far enough
to run, and it fails at a place no seat and no brief named.**

**What was built.** `SlotKind.local_slot` gained `mutable: bool`, carried from
`resolved.Local` at the three lowering sites in `ir/flatten.hero` — the fact is
available there and the language's own *"built with every field, named"* rule
turned every construction site into a compile error, so nothing was missed. The
emitter then chose its cast from the lend's root slot: `(void *)` for a `@` cell,
`(const void *)` for a `=` binding. The compiler built and the fixpoint held.

**Where it fails.** The lend's result is assigned to a **temporary**, and
`emit/body.hero`'s prologue declares temporaries **by type alone** — `for ty in
f.values`, one `ctype.c_type` per value. The Heroes type is `ptr` and `ptr` has
one C spelling, `void *`. So the qualifier dies on the assignment, one line
before it could reach any call:

```
t4 = (const void *)(h0_t.name);
error: assigning to 'void *' from 'const void *' discards qualifiers
  [-Werror,-Wincompatible-pointer-types-discards-qualifiers]
```

**And it refuses the honest READ**, which is a regression rather than a repair:
`r3.hero`, an immutable binding lent to a C function that only reads, stopped
compiling. The change was reverted for that reason; the documents stand.

**What the route actually costs, now that it is measured.** For the `const` to
survive to the call site, a lend's result value must carry a **different C
spelling from the one its Heroes type implies**. That is a design decision — a
second spelling for a `ptr`-typed temporary, or the cast rendered at the argument
position rather than at the temporary — and **no seat priced it**. This file
called the unpriced half *"the checker half and the `cli/pointee.hero` diagnostic
row"*; the unpriced half is larger than that and it is in `emit/body.hero`, a
module that today knows nothing about lends.

**This is the third mechanism in this milestone that did not survive being
tried** — panel 165's route 14, this file's own route H, and in between the
`len()` widening that turned out to be a `spec § 11` change. The pattern is worth
more than any of the three: **a sitting can measure that a route's EFFECT is
right and still be wrong about the machinery, because the seats price what they
can reach and the coordinator writes the resolution from their reports.** What
caught all three was the same thing — someone typing the code.

**What survives, and a later session should not re-derive it:**

- the IR *can* carry the binding's mutability cheaply: 3 real lowering sites, and
  `resolved.Local.mutable` is in hand at each;
- `ir.Place.root` is the slot index, so the emitter reaches the root slot from a
  lend with no span threading;
- the four C crossings behave exactly as the route needs — only
  `const void *` into `void *` is an error, the other three compile clean;
- the flag is already shipped and asserted by the net.

The blocker is one question: **what C type does a lend's temporary have?**

## Author's verdict

**Ratified 2026-09-19, and the author READ the sitting.** Their instruction was
*ratify the decisions*.

**The coordinator first wrote this as a ratification by delegation, in CL-058's
own words — *"the author has not read this file"* — and the author corrected it
in the same session: *that is not true, I read it.* The claim was a supposition
and not a measurement, which is the failure CL-058 exists to prevent, arriving
from the other side.** CL-058 refuses a reading credited where none happened; it
equally refuses a delegation recorded where a reading did. The correction is the
author's own and is dated here rather than quietly applied.

The resolution stands as written: **route H adopted** for defect 065, **route C
in its C-emitted form** adopted for defect 063, **route G refused on three
vetoes**, routes A, B, D, E and F not adopted, defect 066 filed and left open,
and panel 165's resolution 3 superseded.

**What the ratification makes due.** Routes H and C stop being a provisional
default and become the milestone's work: the lend's `const`, the probe's
`_Static_assert`, and the two halves this file marked unpriced — the checker side
of H and the `cli/pointee.hero` diagnostic row, so a reader gets a Heroes sentence
rather than a clang one. Defect 066 is **not** covered by this ratification: its
two routes are the shape three seats vetoed for the write direction, and pricing
them is its own sitting.

The conservative resolution — route A alone, +3 real, leaving 063, 066 and the
immutable parameter open — is recorded above and the author may take it at any
time.

## ROUTE H LANDS — 2026-09-19, M-declared-extents step 5, written underneath

**The question the section above ends on — what C type does a lend's temporary
have — is answered: the lend's own.** `emit/field_lend.hero` reports which
values of a function are const lends and `emit/body.hero`'s prologue declares
those `const void *`, so the qualifier survives the assignment and reaches the
call, where clang refuses it against the header's parameter with the flag this
file names. The honest read from a `=` binding compiles again, which is the
regression that reverted the first landing. `SlotKind.local_slot` carries
`mutable: bool` as that section said it could, through one helper at the three
lowering sites.

**The diagnostic half this file marked unpriced is `emit/ffi_lend.hero`**, and it
is not a `cli/pointee.hero` row: the refusal is an implicit conversion at an
argument, which no `_Static_assert` can carry a message for, and the dump that
would give a marker the header's text is `extern __typeof__(name)`, which no
macro-named function survives. The reader gates on clang's own `discards
qualifiers` at the **call's** line — where the IR holds an extern call whose
argument is a lend from an immutable root — and points at the lend. It is
`ffi_mutable`'s gate since panel 058, at the other end of the boundary.

**Measured on eleven shapes**, in
`docs/records/done/2026-09-19-1645-defect-065-closes-and-the-header-decides-per-call.md`:
three legal roots print their sums, every immutable root handed to a writer is
refused at the lend, and `--emit-c` writes no artifact for the refused program.
**And one shape no seat named forced a checker clause**: a lend handed to a
Heroes function taking `ptr` went from exit 0 to exit 2 under the cast alone,
clang refusing `const void *` into the wrapper's `void *` at a line no reader
owns. A `ptr` lend now stands only as an argument of an `extern` call
(`field_lend_needs_a_header`), which is what route C's declaration on the
group's parameter requires anyway. **Defect 065 is closed.** Route C and defect
066 stand as the resolution left them.

## ROUTE C LANDS TOO — 2026-09-19, M-declared-extents step 6, written underneath

**`counted_by <sibling>` on a group's `ptr` parameter**, and the extent judged
three ways rather than one. The `_Static_assert` this file priced is emitted
where the call states a constant, against C's own `sizeof` of the field — right
on a `partial` record and on a group `constant`, both measured refusing
correctly, which is the critic's own § 1.2 finding arriving as code.

**And the half this resolution left open did not stay open.** Resolution 2 says
the extent is checked *"only where it is a compile-time constant"*; the
compiler-engineer's condition for approving route C says a variable extent
*"must be refused, not admitted. Admitting it silently is the defect wearing a
mark."* Measured with the mark in place and no runtime half: `n: k` with `k` a
parameter read **4096 bytes of the stack at exit 0**. So one compare lands
before the call, with `hero_panic` and no new runtime entry point, timed at
`user 0.81` against `0.81` unguarded over 200 million lends — the ffi-pragmatist
had priced 1% and this run cannot see it. What conservative would have been is
the condition read literally, refusing every non-constant extent at `check`,
which also refuses a count the program computes and gets right.

**The absence of the mark is a refusal**, `field_lend_uncounted`, which is the
engineer's other condition.

**The spec moved +37 vendored and +48 real**, under `DELTA_GATE`, by merging
into the sentence rather than appending beside it — cheaper than the warden's
D1 and D3 priced apart, which is that seat's own finding about merging. The
`CParam` production and the parser land in one commit, which is what the
warden's `grammar` footnote asked of the first change to owe it anything.

**The ffi-pragmatist's prediction is SCORED CORRECT**: none of the four
real-library `ptr` externs in `examples/` needs a clause, and `examples/` is
untouched by this step. **The compiler-engineer's prediction is FALSIFIED as it
expected to be**: the commit changes `selfhost/emit/` — which the sitting
already knew, having adopted the C-emitted form the engineer did not price.

**Defect 063 is closed. Defect 066 stands**, and its sitting is next.
