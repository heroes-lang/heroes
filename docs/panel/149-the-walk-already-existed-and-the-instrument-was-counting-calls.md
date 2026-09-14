# Panel 149 — the walk already existed, and the instrument was counting calls

**Convened** 2026-09-14 on `docs/work/DEFECTS.md` 033, by author instruction
(*"fix the open defect with the panel"*). **Full panel**, five seats: the
proposal widens what a diagnostic refuses, which CLAUDE.md § 4 names as a
diagnostic CLASS. Briefs at `docs/panel/149-briefs/`, reports at
`docs/panel/149-reports/`, all written before any seat started.

## The proposal, verbatim

> How deep does `unmarked_handle_producer` look for a handle, and can the mark
> that answers it be written for a record carrying more than one?

Three resolutions were put: **R1**, what the rule SEES; **R2**, what the
diagnostic SAYS when the handle is two fields down; **R3**, the record reaching
more than one handle, which the surface appeared unable to express.

## What the coordinator measured before convening, and it removed half the question

The defect asked *"how deep: behind two records, behind an optional, behind a
list"*. **Two of the three do not exist.** `-> Slot?` and `-> [Slot]` are already
`error[ffi_type]`, and so is a result record declared outside the group, because
`crosses_the_boundary` (`selfhost/check/ffi.hero:89`) refuses `.array`, `.fixed`,
`.map` and `.fallible`. The reachability graph is closed and finite.

**Four shapes reach the runtime unmarked, all aborting 134**: a handle in a
group-record field, a handle two records deep, a handle inside a fixed array
field, and a group record delivered through an `@` out-parameter.

**The surface already works for the single-handle case.** `function pair_make(n:
i64) -> Pair acquires slot_close` parses, checks, emits, counts, and exits **0**;
`-> Pair borrows` exits **0**. Nothing in the parser, the emitter or the runtime
was missing. **What was missing is only that the rule did not demand the mark.**

**A recursion over declarations needs a guard.** `sized.order_and_cycles` refuses
a record cycle at `selfhost/checker.hero:46` but does **not** stop `acquiring` at
`:74`: a program holding both prints both diagnostics, measured.

## The verdict table

| seat | R1 — what the rule sees | R2 — what it says | R3 — the multi-handle record |
|---|---|---|---|
| **compiler-engineer** | adopt with condition, **+66 lines net** | adopt, **0 lines** | **refuse with veto** on repeat-the-mark and name-the-field |
| **ffi-pragmatist** | adopt with condition | adopt (full path) | **refuse the refusal**, narrow veto |
| **llm-ergonomist** | adopt with condition (4 conditions) | — | **refuse with veto**: refuse the shape |
| **spec-warden** | adopt, **0 spec tokens** | adopt, 0 spec tokens | adopt as a refusal, **+14 real** |
| **historian** | approve (advisory) | approve (path) | object in part |

## The three findings that reframed the sitting

**1. The walk already existed, and so did the message.**
`selfhost/check/map_keys.hero:103-160` is `reaches_handle` + `inside_handle`, 58
lines: a reachability walk over group-record fields, fixed-array elements,
variant payloads, arrays, maps and fallibles, **returning a dotted path**. It
ships, it has three tests, and
`tests/golden/check/ffi-handle-refusals.expected:3` already prints
`` `Conn.handle` ``. So **R1 is a second caller, not a new walk, and R2 is free.**
Found by the compiler-engineer.

**2. The instrument was counting CALLS, not MARKS — and it was inverted.** Found
by the spec-warden while pricing R1's spec cost, reproduced independently by the
compiler-engineer with two parameters of one handle type, and re-run by the
coordinator with two distinct handle types:

| the program | exit |
|---|---|
| two `acquires` marks, **both** handles released — correct | **abort 134** |
| two `acquires` marks, **one** released — leaking | **exit 0** |

Cause: `selfhost/emit/ops.hero:162` asks a per-DECLARATION **boolean**,
`handles.hands_a_handle_over`, and emits one increment per call. **Filed as
defect 034**, and the compiler-engineer's words are that it *"is not conditional
on anything"*.

**3. The depth bound reopens a hole the compiler calls unclosable.**
`map_keys.hero:104` reads `if depth > 16`, and the compiler-engineer bisected it:
a handle used as a map key is refused at sixteen levels of nesting and
**accepted in silence at seventeen**. The comment justifying the bound is true
for its float twin and false for the handle twin, because behind the handle rule
there is no runtime guard, only an address.

## The disagreements, stated plainly and not smoothed over

**A. Does the specification already require the mark?** The **spec-warden** says
YES — *"where a group consumes a handle type every call handing one back says
which it is"*, and the grammar admits `acquires` after any `Type`, not only a
handle type — so R1 costs **zero** tokens and the compiler has the bug under
CLAUDE.md § 12. The **llm-ergonomist**, reading only the spec, concluded the
OPPOSITE, and reported that it **considered writing the mark and deleted it as
unlicensed**.

**B. A veto collision on R3.** The **compiler-engineer** adopts *refuse a mark
whose type reaches N ≠ 1 handles*. The **ffi-pragmatist** compiled raylib's
`Font` against the real installed header: it reaches **two** handles, carries
**one** `acquires UnloadFont`, and exits **0**; `Model` reaches four. That seat
holds a veto on refusing correct bindings, and the compiler seat's rule would
refuse both.

**C. The historian's R3 would overturn a ratified sitting.** Its preferred
resolution is to move the releaser onto the handle TYPE. Panel 147 is titled
*the obligation is created by a call and not by a type*. **No seat noticed.**

**D. Two experiments on the fixed array, opposite conclusions.** The
**compiler-engineer** ran byte-identical Heroes programs against two headers, one
filling four slots and one filling two, and concluded *"the number of handles a
`Slot[4]` carries is chosen by C at runtime"*, so the route is a refusal and not
a count. The **ffi-pragmatist** built a shim with two of four filled, one
`acquires`, and got exit **0**, concluding *"one mark is one increment, whatever
the arity"*.

## The precedent, and it changed the shape of the question

The historian's survey found that **the split is not how deep the rule looks, it
is WHERE THE WORD IS WRITTEN.** Every system that writes the mark on the FUNCTION
stops at the function's own signature: clang's `cf_returns_retained` family,
Swift's `SWIFT_RETURNS_RETAINED`, GObject-Introspection's schema, SAL's
allocation family, Core Foundation's Create Rule. Every system that reaches a
field writes the mark on the FIELD or the TYPE: ARC qualifies the field with
`__strong` and fixes a calling convention so no producer speaks, Splint annotates
the field, Vala defaults it.

Two warnings the sitting should carry forward. **Swift shipped panel 148 R5's
rule and retracted it to an experimental flag inside one release cycle, for
noise.** And **Cyclone's own authors wrote that their warnings *"were of little
help, since there were too many false positives"***, at a cost of 21% of one
program's lines.

## What the completeness critic found, and it changed every one of them

Report at `docs/panel/149-reports/critic.md`. Step 3b of `/panel`, run after the
seats and before this section.

**A — both seats are right, because they measured different things.** The warden
measures what the text LICENSES, the ergonomist what it CAUSES a reader to do,
and a text can license a mark and still fail to require it. **The warden's
decisive argument does not carry**: `owned ident` sits in the *identical*
position in the same two productions and is narrowed in prose one line above
`acquires`, and nobody believes `owned sqlite3_free` after an `i64` result is
licensed. The critic found the fifth reading inside the warden's own table: its
`dsub` draft, **+4 real**. **R1 is not free; it is nearly free.** Two gaps
nobody closed: the ergonomist's candidate B was never priced in the unit that
judges the budget, and `dsub` was never shown to the ergonomist. And **the two
verdicts are not simultaneously satisfiable** — the ergonomist's conditions on
depth and fixed arrays are spec sentences, which *zero tokens* leaves unwritten,
and it said any unmet condition drops it to object.

**B — the collision is real, and the compiler seat's veto fell to its own stated
condition, inside the sitting.** `record Recs tag Rectangle` and `record Glyphs
tag GlyphInfo` are each a tag with no fields, so `Font` reaches **two** handles
and *refuse N ≠ 1* refuses a binding that compiled against the installed header
and exited 0. The compiler seat wrote that its veto lifts on *"a named C function
in a real header returning a struct BY VALUE carrying two or more pointers the
caller must release. None found"* — and the ffi seat had already compiled one,
`LoadFont`. **Neither noticed.** The critic also holds that the ffi seat's veto
is **not formal here**: its scope is ABI breakage and its own report places the
veto narrowly on the counter, saying *"It does not veto R1."*

**C — the historian's route is closed, and no seat saw it.** `grep "147"` over
five reports and six briefs returns **zero** citations. Moving the releaser onto
the handle TYPE is panel 147's Route A, refused on its own axis and ratified the
same day, and it is falsified again inside this sitting by `LoadFont … acquires
UnloadFont` beside `GetFontDefault() -> Font borrows`: two functions of identical
C result type, one owning and one lending. **The coordinator's shared brief is
where that citation was owed and missing.**

**D — not the same experiment: two halves of one arithmetic, both correct.** The
compiler seat's program consumes the individual ELEMENTS, +1/−2, abort 134. The
ffi seat's consumes the COMPOSITE, +1/−1, exit 0. **The route nobody listed**:
all three R3 candidates act on the PRODUCER's declaration and the two programs
differ on the CONSUMER's.

**The question the sitting did not ask, and it is the one that matters.** **Does
the rule's CONSUMER side reach as deep as its producer side?** It does not.
`consumed_types` asks `handle_behind` at ONE level and `bindings_say_which`
returns immediately when that map is empty, so `UnloadFont(font: Font consumes)`
— a record with fields — leaves the map empty. **R1 as framed does not diagnose
the sitting's only shipped-library case.** Verified: the raylib-shaped binding
checks at exit 0, while the same `Font` as a map key errors with ``because
`Font.recs` is a handle``, **proving the walk sees it and the rule simply does
not call it.**

**Three verification findings.** The depth bound is confirmed and **wider than
reported**: 15 and 16 refuse, 17 and 18 exit 0, and **it reproduces for ordinary
Heroes records too**, so no `ffi_field` gate bounds it. `reaches_handle` carries
**no justification for its bound at all** — the sentence licensing it sits on
`reaches_float`, where it is true. *The walk was copied and its premise was left
behind on the original.* And a third, unrelated class: `labs(x: i64) -> i64 owned
free` checks clean, silently retypes the result as `str?`, and clang refuses at
exit **2**, the compiler blaming itself for a binding the author wrote.

## Resolution — provisional, author ratification pending

Adopted under CLAUDE.md § 4: the most robust and complete resolution, never the
cheapest and never a compromise.

**R1 — ADOPT, ON BOTH SIDES.** A handle is what a type REACHES, through
group-record fields and fixed-array elements, to any depth. The rule asks it of
the producer (result, `@` out-parameter) **and of the consumer (`consumes`
parameters)**, because the critic measured that a producer-only rule stays silent
on the only shipped-library case while erroring on the contrived one. Landed by
moving `reaches_handle` and `inside_handle` out of `check/map_keys.hero` into one
module both rules call, and **the `depth > 16` bound dies in that commit**,
replaced by a visited set keyed on the declaration index — not because the bound
is untidy but because the compiler's own words for the rule it guards are *a hole
nothing can close*.

**R2 — ADOPT the full dotted path**, which costs nothing: the walk already builds
it and already prints it for another diagnostic.

**R3 — NO REFUSAL LANDS, and this is where robust and conservative part.** All
three candidates are withdrawn by measurements taken in this sitting: *refuse
N ≠ 1* by its own author's condition firing on `LoadFont`; *the mark on the type*
by panel 147; *refuse the shape* by `Font` being correct with one mark. **What is
adopted instead: one mark is one obligation on the whole value it is written
on**, which is what C ships — `freeaddrinfo` is one call for an N-long chain,
`UnloadFont` one for two arrays, `UnloadModel` one for four.

What the mark must earn, and it is the ffi seat's condition promoted to the
resolution: **the releaser a mark names must resolve to an `extern` in this
module and must carry `consumes` on a parameter of either the marked type itself
or a handle type the marked type reaches.** Measured in this sitting: `acquires
sqlite3_notafunction` and `acquires sqlite3_finalize` on a `Db` both check and
build at exit 0 today. design.md Part 6 already rules on that shape — *"A tag
nobody reads is a comment that looks like a guarantee, which is the one thing an
FFI must never carry."*

**What conservative would have been, recorded so the author can choose it**
(CL-040): land R1 producer-side only, leave the consumer side at one level, and
refuse nothing. It is cheaper by the visited set and by the releaser check, and
it buys a rule that does not fire on raylib.

**The per-ELEMENT release is not resolved here and is filed rather than
guessed.** Releasing four array elements against one mark aborts at 134, and both
real headers say in their own text that the count is unknowable — `jpeglib.h:638`
*"or NULL if not defined"*, `net/route.h`'s bitmask. The critic's route nobody
listed — refuse the element release, or let the mark say WHOLE versus
PER-ELEMENT — acts on the consumer's declaration, which no seat was asked about.
It becomes its own defect with the falsifier named.

**Spec cost: +4 real**, the warden's `dsub`, substituting *"after a result or `@`
out-parameter reaching a handle"*. Adopted over zero because the critic showed
the zero-token reading rests on an argument the document refutes one line above,
and over the ergonomist's nine words because those were never priced in the unit
that judges the budget. 7974 → 7978, against a ceiling of 10240.

## What this sitting produced besides its resolution

| defect | what it is | found by |
|---|---|---|
| **034** | the counter counts CALLS not MARKS: the correct program aborts, the leaking one exits 0 | the spec-warden, pricing R1 |
| **035** | `reaches_handle`'s `depth > 16` accepts a handle map key at seventeen levels, in silence, for ordinary records too | the compiler-engineer, verified and widened by the critic |
| **036** | `owned <freer>` after a non-`cstr` result checks clean and makes clang blame the compiler at exit 2 | the critic |

## Predictions to score

- **llm-ergonomist**: a model omits the mark on a record-returning producer in
  ≥ 8 of 10 samples under the current text and ≤ 1 of 10 under the widened one.
  Checkable when a harness runs the three-header experiment it specified.
- **historian**: *the fixed array breaks the mark's arity before anything else.*
  **RUN AND CONFIRMED on its own number** — `Slot[4]`, one mark, four releases,
  `+3`, abort 134.
- **spec-warden**: at the close of the milestone settling 033, `heroes measure
  spec/heroes-spec.md --refresh` reads ≤ 7991 real and five suites are green with
  zero existing programs newly refused.
- **ffi-pragmatist**: exactly one file changes verdict,
  `tests/golden/run/abort-handle-given-back-unmarked.hero`; zero under
  `examples/`.
- **compiler-engineer**: if R1 lands calling the walk as it stands, a
  seventeen-deep chain checks at exit 0 with no diagnostic.

## Author's verdict

**PENDING.** The resolution above is `provisional — author ratification
pending`, and the item is queued in `docs/work/DECIDE.md` as `panel 149`. Work
proceeds on it as the default; this section is where the answer is appended,
whenever it is given.

**What a yes settles.** That a handle is what a type REACHES rather than what it
IS, to any depth, on the consumer side as well as the producer side. That the
dotted path is printed. That **no refusal lands** for a record reaching several
handles, so `LoadFont(fileName: cstr) -> Font acquires UnloadFont` stays legal
and one mark stays one obligation on the whole value. That the releaser a mark
names is checked. And the +4 real on the specification.

**What a yes does NOT settle, and each is filed rather than folded in.** The
per-ELEMENT release — four array elements released against one mark — stays
unresolved, because both real headers say in their own text that the count is
unknowable, and because the route that would answer it acts on the CONSUMER's
declaration, which no seat was asked about. The three defects this sitting
produced are repairs and not design questions, so they need no ratification:
**034** (the counter counts calls, not marks), **035** (the depth bound gives up
in silence), **036** (`owned` after a non-`cstr` result). And the `ptr` blind
spot the ffi-pragmatist measured — a producer whose whole result is a `ptr`,
exit 0 with a real leak — is a **silent** class where this sitting's is a loud
one, so by §1.12 it outranks what was repaired here and is owed its own sitting.

**The conservative alternative is on the table and named** (CL-040): R1
producer-side only, the consumer side left at one level, no releaser check. A yes
to that instead is a complete answer and the work reshapes to it.

## Seat track record from this sitting

The **completeness critic changed the resolution again**, for the sixth sitting
running: it dissolved contradiction A into two compatible measurements, found a
seat's veto lifted by its own condition inside the sitting, caught a ratified
sitting nobody cited, and asked the question that makes R1 work at all. The
**coordinator's shared brief is where panel 147's citation was owed**, and that
omission propagated to five seats.

