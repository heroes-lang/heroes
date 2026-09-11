# Panel 132 — can the compiler write the conversion, and what shape can it take?

**Convened** 2026-09-11, step 2 of **M-reflection-verdict**, the corrected sitting
panel 131 called for. **Lane:** full five seats.
**Status:** `provisional — author ratification pending`.

## Why there is a second sitting at all

Panel 131 ruled two of its three questions and **deliberately did not rule the
third**, because its own brief had put it wrongly: its drafts rendered Heroes'
own construction call, which is display, while the precedent and the measured
pressure are about a per-type walk aimed at a **foreign notation**, which is
derivation. Three seats answered the question and two answered the brief. The
author ratified that, and this is the corrected question:

> Can the compiler synthesise, per record type, a walk over that type's fields
> that a program aims at a foreign notation — so the field name comes from the
> field and cannot be mistyped — under the constraints this language has already
> ruled?

**The brief this time gave the walls and the candidate shapes with their known
objections, and said in as many words that inventing a fifth was the most
valuable thing a seat could do, and that "no shape survives" is a verdict.**
Two seats invented a fifth. Two others invented a different fifth. That is this
sitting's structure and it is why it is worth reading.

## The verdict table

| shape | compiler-engineer | llm-ergonomist | spec-warden | ffi-pragmatist | historian |
|---|---|---|---|---|---|
| **A** compiler generates `to_json` | refuse | dead on arrival | refuse | refuse | dies on the package wall, which Ada solved with an override clause |
| **B** a record of handlers, accumulator fixed | **veto** | **veto** | **veto** | **veto** | adopt, as Ada's `'Write` |
| **B′** handlers as parameters, accumulator generic | not put to it | not put to it | not put to it | **adopt**, compiled | **the shape its own Ada reading requires** |
| **C** the type as data | refuse | dead | refuse | refuse | where Haskell's measured cost actually went |
| **D** nothing enters | object | fallback | **adopt**, and it is a removal | **refuse** | — |
| **E** `Point.width` is the `str` `"width"` | **adopt**, built and run | **adopt**, invented | refuse at ≈+46, a `constant` does it at 0 | not put to it | not put to it |

## The two convergences, and they are the sitting's finding

**E was invented twice, independently, by the two seats furthest apart.** The
llm-ergonomist had **only** `spec/heroes-spec.md` and no repository, and wrote
`Room::width`. The compiler-engineer had the whole compiler and wrote
`Point.width`. Neither saw the other. Same construct: **a field's name becomes a
name the checker resolves, rather than a string the program types.**

**B′ was reached twice the same way.** The ffi-pragmatist found that a record of
handlers cannot work because **a record cannot be generic**, so its accumulator is
fixed at one type; it moved the handlers into parameters, freeing the
accumulator, and compiled the result. The historian, searching precedent with no
knowledge of that experiment, wrote of Ada's `'Write` that the precedent transfers
*"**only** if the per-scalar writers are passed as ordinary function arguments.
That is the monomorphised Ada shape, and it is the fifth thing you asked me to
look for: not a fifth candidate, but the missing justification for B."*

Two seats found the same repair; two other seats found the same different repair.
Neither pair was a compromise and neither was in the brief.

## B as briefed is refused, and four seats vetoed it on four different compiled grounds

Not one of these is an opinion, and no two are the same objection.

- **compiler-engineer**: B is **core** by design.md §1.7's own test — it adds a
  name the checker must synthesise, a generic type the lowering must build a body
  for, and a per-type C function the backend must write. Four compiled obstacles:
  a leaf record's walk is refused by `error[unused_binding]` on handlers it never
  calls; a generic call site gives `error[cannot_infer]` where the non-generic one
  compiles; **the handler set is the transitive scalar closure, so adding `z: f64`
  to `Point` changes `room_walk`'s arity** and the note points at a declaration no
  program can open; and the fold's accumulator is refcounted, which `_eq` and
  `_hash` never face because they return scalars.
- **llm-ergonomist**: B removes one mistake that compiles and **installs three**.
  At a `[str]` element there is no name to pass, and all three plausible answers
  compile and produce different JSON. `enter_record` and `enter_array` have
  **identical types** and swap in silence. With no closure and no `@` in a
  function type, a handler cannot know whether it is the first field, so it must
  sniff the buffer's last byte — and a string value ending in `{` then suppresses
  a comma. It counted **thirteen** questions the document would have to answer
  first, twenty-five to forty lines, a tenth of the whole language for one
  library.
- **spec-warden**: it wrote both versions, **byte-identical output, hand 17 lines
  against B's 24** — the mechanism meant to make you write less makes you write
  **41% more**. And B cannot name its own callbacks: a field of type
  `{str: [i64]}` needs a parameter name that a type spelling cannot be
  (`error[expected_parameter_type]`). **That is the type system, not policy.**
  Measured spec cost **+173 real**.
- **ffi-pragmatist**: **a record cannot be generic**, so B's accumulator is fixed
  at one type, and fixed at `str` every byte must land in a Heroes string before
  reaching C. **That is the unbox-in/box-out glue design.md §1.11 refused when it
  refused level 3**, inverted: the seat compiled one walk driving a Heroes string
  and a live `FILE *`, and B can only ever do the first.

**B also compiles today with no language change**, the warden found, everything
but the generated walk being writable now, because top-level callbacks need no
closure. So it is not blocked. **It is declined on its own numbers.**

## What the historian settled, and what it withdrew

**The negative claim is false and it is thirty years old.** Ada shipped
compiler-derived per-component serialisation with no macro, no compile-time
execution of user code, no trait and no separate generator, twice, both in the ISO
standard: `T'Write` in Ada 95 (approved 1994) and `T'Put_Image` in Ada 2022. The
mechanism is an **attribute** — a compiler-known per-type slot resolved statically
by name, defaulted by the compiler, replaceable by an ordinary procedure — and
that is how it works without traits. Ada pays for genericity over sinks with
dynamic dispatch on a class-wide type, which Heroes has not got; Heroes must pay
with explicit function parameters, which is B′ and not B.

**And Ada carries a warning Heroes should read twice.** GNAT documents that the
default derived image format is *"deliberately not documented and subject to
change"* and tells users not to depend on field order. **Ada shipped a derived
walk whose output it refuses to specify.** A Heroes walk feeding a JSON package
must fix field order in the document or a golden will pin what the compiler never
promised.

**Two withdrawals, both of the seat's own panel 131 claims, and the record keeps
them.** *Erlang's `record_info` does not transfer*: the path from names to values
runs through the untyped tuple representation into a heterogeneous list, and
Heroes has no tuples and no positional field access, so the seat withdraws it as a
design source. And *its own quadratic-cost warning does not reach B or B′*: every
sourced report of quadratic `deriving` names `Generic` and `Rep`, the type-level
representation, **which is candidate C** — the seat states plainly that it could
not source a comparison against plain `deriving Show` and is therefore not
entitled to call flat per-field emission linear, only to say nobody has reported
it. **That is an argument against C, and the sitting relocates it there.**

**Haskell is the precedent for the uncomfortable half of any derivation**: the
2010 Report allows deriving only for classes *"known to the compiler"*, users
extend nothing, it held for thirty years, and the pressure to escape it produced
`DeriveGeneric` — which is where the cost went.

**The negative claim's vocabulary is on the record** with the terms searched, and
the seat names the place it did not look and where a counterexample may hide:
Modula-3's `Pickle`.

## E, measured and built

**`Point.width` is the `str` `"width"`, resolved against `record Point`;
`Point.witdh` does not compile.** The program still writes every byte of its
output: quoting, escapes, `null`, sorted keys, `shown(Date)`'s zero-padding,
`shown(Amount)`'s cents. **Nothing is generated**, so no output format is promised
and Ada's own warning does not bite.

The compiler seat built it, and it compiles itself and runs:

```
run   ->  {"name":"hall","width":3.0}
check ->  error[no_such_field]: `Font` has no field `witdh` — `Font` has `name`, `width`
```

| | measured |
|---|---|
| `selfhost/check/access.hero` | 161 → 201 |
| `selfhost/check/walk.hero` | 1842 → 1853 |
| `selfhost/ir/flatten.hero` | 1123 → 1131 |
| **total** | **+59 code lines**, 39 excluding comments |
| `selfhost/emit/`, `runtime/`, the lexer, the parser, ownership, mono | **zero** |
| the compiler's own tests · `check` goldens | **631 pass** · **109 pass** |
| `heroes fmt` round-trip · double emit | byte-identical · byte-identical |
| `heroes check selfhost/main.hero` | 14.41 s → **13.90 s**, machine still |
| `DECIDED.len()` | stays **18**, two numbers move |
| spec | **+36 vendored, ≈ +46 real** (coordinator's wording, measured offline this session) |

**The grammar already parses `Point.x`**, and today it is
`error[record_name_alone]` at `selfhost/check/walk.hero:337`, so **the slot is
free**. And the erasure is compiled rather than argued: `ir/flatten.hero` turns it
into an interned string literal indistinguishable from one the program wrote,
taking the route `.module`-in-base-position already takes. **By design.md §1.7's
own test — sugar is erased by one function on the way into the IR — E is sugar and
B is core.**

## D is not adopted, and the case for it is the strongest thing in this file

The warden's argument is recorded whole, because it very nearly carried.

**It re-ran the Principle 0 count in its corrected form and it got smaller, not
bigger.** Of 129 text-producing functions in `examples/`, 24 take a record, 6 are
cursor-advancers, and of the remaining 18 **exactly one** is a type-uniform
foreign-notation field walk — a CSV row writer — **which writes no field name at
all**, so it saves zero lines. **1 of 56, 0 lines deleted.** The corrected
question answers the way the old one did, for a different reason.

**And the repair already exists at zero spec tokens**, in the idiom documented at
the top of `examples/json/value.hero` — the very file the ffi-pragmatist imitated
when it wrote the experiment that convened this sitting. Declare the key once:

```
constant K_WIDTH: str
    "width"
```

and `K_WITDH` is, run today, `error[unknown_name]: nothing named K_WITDH is in
scope — did you mean K_WIDTH?` with a **certain** fix.

**It also answered the Part 11 question better than the brief asked it.** *A
plausible mistake made unspellable* is metric 3, silent-error rate — but that
metric's operators mutate the **program**, and `"witdh"` mutates **data**. The
general form proves far too much: it would demand the compiler check
`"SELCT * FROM t"`, `"/etc/hots"`, every flag and every URL, which is a schema for
every foreign notation, and design.md §1.11 refuses that permanently. **It is
admissible only where the string's referent exists in the same program** — and
both E and a `constant` are exactly that confinement.

**What answers it, and it is the warden's own stated residual**: a `constant` is
**not bound to the field**. `constant K_WIDTH: str` whose body is `"widht"`
compiles and ships. `Point.width` cannot be wrong about the record it names. The
`constant` catches a typo in a *reference to the constant*; E catches a typo in
the *field name itself*, which is the mistake the ffi-pragmatist actually made in
the experiment.

## The disagreement the sitting does not resolve, stated rather than smoothed

**E kills the typo. It does not kill the rot.** Add a fifth field to a record and
the marshaller still compiles, still reads the value, and silently never mentions
the new field — the llm-ergonomist's panel 131 finding, unclosed, and the
compiler seat names the gap in its own verdict rather than hiding it. **Only a
walk that enumerates fields closes rot**, which is B′, and B′ is not adopted here
because nobody has built its compiler side.

**That distinction is exactly what cost panel 117 and it is not repeated.** What
the ffi-pragmatist compiled is a **hand-written simulation of what the compiler
would generate**, 78 lines of it, not the generation. Its measurements are real
and its shape is sound at the boundary — `HeroDesc` still **5** members, the
header's struct passed by value and unredeclared, `_Static_assert`s intact,
monomorphised `HERO_TU_LOCAL` so dead-code elimination survives, **nothing in the
value ever**, and its panel 131 condition met so its veto lifts — and **the
compiler side is unrun**. It is scheduled with what it owes, not adopted.

**And B′ carries three refusals its own proposing seat calls blocking**, each
compiled: a walk over a `partial` record builds at exit 0 reporting 12 bytes of a
measured `sizeof(struct stat) == 144`, and **consuming one is not merely an abort
but impossible**, there being no honest construction call; `sort(keys(m))` on
`{Point: i64}` is `error[unordered_element]`, and brute force found **four** pairs
of `==`-equal maps marshalling to different bytes; and the ergonomic cost is a
13-argument call whose generated signature is a **577-character line the formatter
does not wrap**.

## The resolution adopted, provisionally

**Robust, and it is neither the cheapest option nor a compromise between the two
convergences.** Six parts.

**1. A, B and C are REFUSED.** A on design.md §1.11 — the compiler cannot know a
package — with Ada's override clause recorded as the way that wall was solved
elsewhere, so it is not reinvented as new. **B on four vetoes with four distinct
compiled grounds**, the deepest being that a record cannot be generic, so its
accumulator is fixed and every byte must buffer in a Heroes string before reaching
C, which is §1.11's level-3 glue inverted. C on the same ground panel 131 refused
run-time reflection, plus the historian's relocated cost finding: **every sourced
report of quadratic derivation names the type-level representation, which is C.**

**2. E is ADOPTED**, and it is the only shape in this sitting that is built,
measured, green and free of a slowdown. It is sugar and not core by §1.7's own
test; it costs **+59 code lines** with **zero** in the emitter, the runtime, the
lexer, the parser, ownership and mono; it takes a grammar slot that is already
parsed and today refused; and it takes **nothing away from the program**, which
was the clause the corrected question said was not optional.

**3. E's landing owes three things, and they are named now rather than
discovered.** The exact surface — `Point.width` against `Room::width` — is a
question for the landing and not settled here, because `Point.width` reads as
field access on a value while `Point` is a type; the compiler seat built the first
and the ergonomist proposed the second, and one of them has to be chosen with the
diagnostic in front of the author. **`record_name_alone` gets the golden it has
never had**, `grep -rln record_name_alone tests/` being empty today, because E
lands in exactly that diagnostic's slot. And **the real token count** replaces the
≈+46 estimate at the landing commit, since `heroes measure --refresh` refuses any
file but the spec itself (panel 123 R5).

**4. B′ is NOT adopted and is SCHEDULED as an open question with what it owes** —
the compiler side built rather than simulated, and the three refusals its own seat
calls blocking. It is named as **the route for the rot**, which E does not close,
so the record says which half is unclosed rather than implying the sitting closed
both. **The distinction between a compiled simulation and a compiled mechanism is
what panel 117 got wrong four days ago**, and this sitting does not repeat it.

**5. The mutation operator lands regardless of shape, and it is what PAYS for E
under design.md §1.6.** Two seats converged on it with neither knowing of the
other: `selfhost/mutate/ops.hero` holds **14** operators and **not one of them
typos a string literal**, so `heroes mutate` is blind to the exact mistake this
sitting exists to kill. On a marshaller written with bare strings the new operator
scores **0% killed**; on one written `Record.field` it scores **100%**, because
the mutation lands on an identifier the checker resolves. It costs **zero spec
tokens**, it turns the sitting's motivating defect into a scored metric instead of
a ruling, and it is a pre-registered falsifiable prediction naming an instrument
that exists, which is what §1.6 accepts as payment.

**6. Panel 117's restoration clause goes FALSE-BY-STANDING and its removal is put
to the author separately.** The warden asked panel 131 to declare a branch and
panel 131 said *still being worked toward*. That is no longer true: a derived
render of Heroes' own construction call was refused at 131 on 0-of-56, nothing
here revives it, and neither E nor B′ is a renderer `assert` could use, since both
require a program to supply what to do with each field and `assert` has no program
to ask. So `spec § 12`'s *"until this language renders one"* now names a condition
nothing is working toward, which is the one shape this project's own rules exist
to end. **Removing it is reversing an author decision of 2026-09-07** — the author
took the fuller wording over the warden's cheaper one on *better robust than
cheap* — so it is queued as its own item, the way the panel 117 withdrawal was.
Measured **−7 vendored, ≈ −9 real**.

## What the conservative resolution would have been, so the author can choose it

**Adopt D and land nothing.** The warden's case is above in full and it is not
weak: 1 of 56 with 0 lines deleted, the `constant` idiom already giving the
compile error at zero spec tokens, and the Part 11 argument confined so it does
not prove too much. Under D the milestone closes on three refusals, the spec
**shrinks** by ≈9 tokens, and the only thing that lands is the mutation operator,
which costs nothing and measures everything.

**It was not adopted for one reason, measured**: a `constant` is not bound to the
field, so `constant K_WIDTH: str` whose body reads `"widht"` compiles and ships
the wrong key — which is the mistake this sitting exists for, moved one line
sideways rather than removed. E is the only shape measured here that cannot be
wrong about the record it names, and it costs 59 lines of sugar and no slowdown.

## Predictions to score

| seat | prediction | scored at |
|---|---|---|
| compiler-engineer | if E lands, its diff touches **zero** files under `selfhost/emit/` and `runtime/`, `selfhost/` gains **≤ 70** code lines, and `DECIDED.len()` stays **18** with exactly two numbers moved | E's landing commit |
| compiler-engineer | with the new operator, a marshaller written with bare strings is caught **0%** and one written `Record.field` **100%** | M-core-packages step 4 |
| llm-ergonomist | under a B-shaped spec, ≥ 30% of model pairs disagree on what `name` holds at a `[str]` element, and ≥ half the compiling programs emit malformed or unsorted JSON | if B is ever re-briefed |
| llm-ergonomist | a seeded enter/leave swap in a B-shaped `Sink` is caught **0%** at compile time against **100%** for a seeded scalar-handler swap | if B is ever re-briefed |
| llm-ergonomist | E is overturned if models write a bare `"width"` rather than `Point.width` in more than half of first tries — E is opt-in, and if nobody opts in it bought nothing | M-core-packages step 4 |
| spec-warden | `typo-key` scores ≈0% on `examples/` today and ≥80% once the corpus's ~10 keys are `constant`s | M-core-packages step 4 |
| ffi-pragmatist | if B′ lands, `heroes build --emit-c examples/sqlite/main.hero` is byte-identical but for `#line`, and `struct HeroDesc` stays at **5** members | B′'s landing, if it lands |
| ffi-pragmatist | `examples/curl/main.hero` gains a JSON request body in ≤ 15 program lines containing **zero** field-name literals | M-core-packages step 4 |
| ffi-pragmatist | **no program exists** that writes a JSON body straight into a `FILE *` or `cJSON *` with the accumulator fixed at `str` and no intermediate Heroes string; name one and its veto on B falls | this milestone's close |
| historian | B′ fails on a record whose field is a **list of records**, because the nested walk must itself receive the writer set and the instantiation cannot be inferred at the inner call site | B′'s owed prototype |
| historian | derivation is **linear** in field count at 8, 64 and 512 fields — this scores the seat's own panel 131 warning, and if the 512 case exceeds 8× the 64 case that warning was right and C's disease is in B′ too | B′'s owed prototype |
| historian | within one milestone of any derived walk landing, a golden pins an output whose field order the spec never fixed — Ada's own documented mistake | B′'s landing, if it lands |

## A scored prediction from panel 131

The ffi-pragmatist's: *SQLite step 3 of §4.19's ladder needs no shim and no new
line under the `partial`-abort rule.* **TRUE**, scored this sitting:
`examples/sqlite/main.hero` declares **0** records and **0** `partial`, so the
diff is zero.

## What the seats could not source or could not run

The historian could not source a comparison of plain `deriving Show` against
`deriving Generic`, and says so rather than filling it; it did not search
Modula-3's `Pickle`, which it names as the likeliest hiding place for a
counterexample; and it could not run the compiler from its seat, so its question
about whether a generic can be instantiated at a record type the calling package
names was answered by the other two seats' monomorphised `HERO_TU_LOCAL` walks
rather than by it.

The llm-ergonomist, reading only the spec, could not determine whether
`sink.on_str(...)` calls the record's field or is UFCS on a top-level function of
the same name — **both type-check**, and under B a top-level `on_str` always
exists because the programmer wrote it to put in the sink. It reports the wanting
rather than satisfying it, as its seat requires.

Every draft delta in this sitting is **provisional by construction**:
`heroes measure --refresh` refuses any file but `spec/heroes-spec.md`, so no
unlanded wording can have a binding count.

## Two defects found beside the sitting

**028**, from the ffi-pragmatist and reproduced by the coordinator on the frozen
tree: a record field of function type that is never called through passes
`heroes check` at exit 0 and then **aborts `heroes build` at exit 134 with
`assert failed: !found.is_err()`** — no file, no line, no diagnostic code. The
control, the same record with the function actually called, builds clean. It is
the `internal error` class `.claude/rules/c-boundary.md` names as the one that
accuses the compiler and repairs nothing, and the seat calls it blocking for any
record-of-handlers form.

**And `error[record_name_alone]` has no golden case**, `grep -rln
record_name_alone tests/` returning empty — a live diagnostic nothing judges. It
is not filed separately because **E lands in that diagnostic's own slot**, so the
golden is owed with E rather than beside it.

## Author's verdict

**Pending.** The resolution is `provisional — author ratification pending` and
work proceeds on it. It is queued as **two** items in `docs/work/DECIDE.md`: the
resolution itself, and separately the removal of panel 117's restoration clause,
because that reverses a wording the author chose explicitly against a seat's
recommendation on 2026-09-07 and a reversal must not ride inside a longer yes.
